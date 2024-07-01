#include "bluetooth.h"
/*
 * bluetooth.c
 *
 *  Created on: Jul 10, 2022
 *      Author: 86152
 */
#include "servo.h"

void extract_RX()    //蓝牙遥控命令提取函数
{
	for(uint8_t i = 0;i<Rx_len-1;i++){
		if((Rx_buffer[i]=='A')&&(Rx_buffer[i+1]=='T')&&(i<4)){
			if(Rx_buffer[i+3]=='E'&&Rx_buffer[i+4]=='D'){
                Rx_command = Rx_buffer[i+2];
                for(uint8_t j=0;j<Rx_len;j++)
                {Rx_buffer[j] = 0;}
                Rx_len=0;
                break;
			}
		}
	}
}
void mode_switch(char command)
{
	if(command== '1')//开关(打开后默认进入遥控模式)，在其他模式下按此键回到遥控状态，再按即可关闭
	{
		setspeed(0,0);
		if(mode!=1){
			mode=1;
			is_down=0;
		}
		else if(mode==1){mode=0;}
	}
    else if(command== '2')//自动巡线模式
	{
		if(mode!=0){
			MODE=1;
			uint8_t  CO1AskBuffer[3]={0xb3,1,0xb4};
			HAL_UART_Transmit_IT(&huart1, (uint8_t *)CO1AskBuffer, sizeof(CO1AskBuffer));
			for(int i=0;i<16;i++)
			{servo4_turn(90-3*i);}
			mode=2;
		}
	}
	else if(command== '3')//自动避障模式
	{
		if(mode!=0){
			MODE=2;
			uint8_t  CO2AskBuffer[3]={0xb3,2,0xb4};
			HAL_UART_Transmit_IT(&huart1, (uint8_t *)CO2AskBuffer, sizeof(CO2AskBuffer));
			for(int i=0;i<16;i++)
			{servo4_turn(90-3*i);}
			mode=2;
		}
	}
	else if(command== '4')//测试巡线模式
	{
		if(mode!=0){
			mode=3;
			avoid_dir=0;
		}
	}
	else if(command== '5')//测试避障模式
		{
			if(mode!=0){
				mode=4;
				avoid_dir=0;
				servo3_init();
			}
		}
}
void remote_control(char command)
{
	if(command== 'w'){setspeed(targetspeed,0);}//前进
	else if(command== 's'){setspeed(-0.6*targetspeed,0);}//后退
	else if(command== 'a'){spin(1);}//左转
	else if(command== 'd'){spin(2);}//右转
	else if(command== 'j'){        //机械臂升降
		servo1_down();
	    if(is_down==0){is_down=1;}
	    else if(is_down==1){is_down=0;}
	    command= 'q';
	}
	else if(command== 'k'){       //机械爪开合
	    servo2_shut();
		if(is_shut==0){is_shut=1;}
	    else if(is_shut==1){is_shut=0;}
		command= 'q';
	}
	else{                        //运动状态归零
	    setspeed(0,0);
	    spin(0);
	}
}

