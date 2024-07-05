/*
 * openmv.c
 *
 *  Created on: Jul 15, 2022
 *      Author: 86152
 */
#include "openmv.h"
#include "servo.h"
static uint8_t  Cx=0,Cy=0,Cw=0,Ch=0;
void Openmv_Receive_Data(int16_t Com_Data)
{
  /*循环体变量*/
    uint8_t i;
	/*计数变量*/
	static uint8_t RxCounter1=0;//计数
	/*数据接收数组*/
	static uint16_t RxBuffer1[10]={0};
	/*数据传输状态位*/
	static uint8_t RxState = 0;
	/*对数据进行校准，判断是否为有效数据*/
    if(RxState==0&&Com_Data==0x2C){  //0x2c帧头
		RxState=1;
		RxBuffer1[RxCounter1++]=Com_Data;
	}
	else if(RxState==1&&Com_Data==0x12){  //0x12帧头
		RxState=2;
		RxBuffer1[RxCounter1++]=Com_Data;
	}
	else if(RxState==2){
		RxBuffer1[RxCounter1++]=Com_Data;
		if(RxCounter1>=10||Com_Data == 0x5B){       //RxBuffer1接受满了,接收数据结束
			RxState=3;
			Cx=RxBuffer1[RxCounter1-5];
			Cy=RxBuffer1[RxCounter1-4];
			Cw=RxBuffer1[RxCounter1-3];
			Ch=RxBuffer1[RxCounter1-2];
		}
	}
	else if(RxState==3){//检测是否接受到结束标志
		if(RxBuffer1[RxCounter1-1] == 0x5B){
			RxCounter1 = 0;
			RxState = 0;
		}
		else{  //接收错误
			RxState = 0;
			RxCounter1=0;
			for(i=0;i<10;i++)
			{RxBuffer1[i]=0x00;}      //将存放数据数组清零
		}
	}
	else{   //接收异常
		RxState = 0;
		RxCounter1=0;
		for(i=0;i<10;i++)
		{RxBuffer1[i]=0x00;}     //将存放数据数组清零
	}
}
void extract_MVRX1()
{
	if(mode==3){
		if(Cx==3){
			if(Cy==0)
			{deviation=Cw+Ch/10;}
			else if(Cy==1)
			{deviation=-Cw-Ch/10;}
		}
	}
}
void extract_MVRX2()
{
	if(Cx==2&&Ch==0)
	{
		MODE=Cy;
		mode=Cw;
		if(mode==5){
			setspeed(0,0);
			is_stopping=1;
		}
		if(mode==4){servo3_init();}
	}
}
void extract_MVRX3()
{
	if((mode==2)||(mode==5)){
		if(Cx==0){deviation_f=Cy+Cw/10;}
		else if(Cx==1){deviation_f=-Cy-Cw/10;}
		if(Cx==0||Cx==1){deviation_s=Ch;}
	}
}

