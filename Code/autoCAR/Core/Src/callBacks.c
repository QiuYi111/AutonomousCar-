/*
 * callBacks.c
 *
 *  Created on: Jul 21, 2024
 *      Author: 25138
 */
#include "main.h"
#include "usart.h"
#include "tim.h"
#include "motor.h"
#include <math.h>
#include <string.h>
#include "ultraSonic.h"
#include "servo.h"
#include "float_uint8.h"
extern uint8_t rxDataBT[50],rxDataOp[30];
int preScaller=0,spin=0,dt;
extern float rpmLeft,rpmRight,deriSpeed,centralSpeed;//定义在main里的，用于存放rpm数据
extern int pulseLeft,pulseRight,mode;
float kp=1,ki=0.5,kd=1.5; int pulseLeft=0;int pulseRight=0;
float leftError=0,rightError=0; float error_last_left = 0,error_before_left= 0;float error_last_right = 0,error_before_right = 0;
uint8_t cRt[512]={};
int delayer=0;extern float is_crawed;
float currentRpm_left=0;
float currentRpm_right=0;int ultraLoop=0;
extern float craw_state;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	 if (htim==&htim7){
		 setRightRpm(rpmRight);
		 setLeftRpm(rpmLeft);
		 if (spin==1){
			 if (preScaller==dt){
			 	 rpmLeft=rpmRight=0;
			 	 preScaller=0;
			 	 spin=0;
			 	if (is_crawed==1){
			 		setDirection('w');
			 	    centralSpeed = 55;
			 	    rpmLeft=rpmRight=centralSpeed;
			 		mode=1;
			 	}
			 }else{preScaller++;}
		 }

		 /*if (ultraLoop==0){
			 distanceFront=MeasureDistance(&ultraSonicFront);
			 ultraLoop++;
		 }else if(ultraLoop==1){
			 distanceLeft=MeasureDistance(&ultraSonicLeft);
			 ultraLoop++;
		 }else if (ultraLoop==2){
			 distanceRight=MeasureDistance(&ultraSonicRight);
			 ultraLoop=0;
		 }*/
	}else if (htim ==&htim12){
		//currentRpm_left=getLeftRpm(&htim2);
		//currentRpm_right=getRightRpm(&htim4);
		/*
		int right_Rpm_length = 0, left_Rpm_length = 0,deriSpeed_array=0;
		cRt[0] = 'l';
		cRt[1] = ':';
		left_Rpm_length = float_to_uint8_arry(cRt + 2, currentRpm_left, 2);
		cRt[left_Rpm_length + 2] = ' ';
		cRt[left_Rpm_length + 3] = 'r';
		cRt[left_Rpm_length + 4] = ':';
		right_Rpm_length = float_to_uint8_arry(cRt + left_Rpm_length + 5, currentRpm_right, 2);
		cRt[left_Rpm_length + right_Rpm_length + 5] = ' ';

		int distanceFront_length = 0, distanceLeft_length = 0, distanceRight_length = 0;
		cRt[left_Rpm_length + right_Rpm_length + 6] = 'F';
		cRt[left_Rpm_length + right_Rpm_length + 7] = ':';
		distanceFront_length = float_to_uint8_arry(cRt + left_Rpm_length + right_Rpm_length + 8, (float)distanceFront, 3);
		cRt[distanceFront_length + left_Rpm_length + right_Rpm_length + 8] = ' ';
		cRt[distanceFront_length + left_Rpm_length + right_Rpm_length + 9] = 'L';
		cRt[distanceFront_length + left_Rpm_length + right_Rpm_length + 10] = ':';
		distanceLeft_length = float_to_uint8_arry(cRt + distanceFront_length + left_Rpm_length + right_Rpm_length + 11, (float)distanceLeft, 2);
		cRt[distanceLeft_length + distanceFront_length + left_Rpm_length + right_Rpm_length + 11] = ' ';
		cRt[distanceLeft_length + distanceFront_length + left_Rpm_length + right_Rpm_length + 12] = 'R';
		cRt[distanceLeft_length + distanceFront_length + left_Rpm_length + right_Rpm_length + 13] = ':';
		distanceRight_length = float_to_uint8_arry(cRt + distanceLeft_length + distanceFront_length + left_Rpm_length + right_Rpm_length + 14, (float)distanceRight, 2);
		cRt[distanceRight_length + distanceLeft_length + distanceFront_length + left_Rpm_length + right_Rpm_length + 14] = '\n';
		cRt[distanceRight_length + distanceLeft_length + distanceFront_length + left_Rpm_length + right_Rpm_length + 15] = 'D';
		cRt[distanceRight_length + distanceLeft_length + distanceFront_length + left_Rpm_length + right_Rpm_length + 16] = ':';
		deriSpeed_array=float_to_uint8_arry(cRt+distanceRight_length + distanceLeft_length + distanceFront_length + left_Rpm_length + right_Rpm_length + 17, deriSpeed, 0);
		cRt[distanceRight_length + distanceLeft_length + distanceFront_length + left_Rpm_length + right_Rpm_length + 18] = '\n';
		HAL_UART_Transmit_DMA(&huart5, cRt, sizeof cRt);
		*/
		 int lengths[6]; // Array to store lengths
int index = 0;  // Index for cRt array

cRt[index++] = 'l';
cRt[index++] = ':';
lengths[0] = float_to_uint8_arry(cRt + index, currentRpm_left, 2);
index += lengths[0];
cRt[index++] = ' ';
cRt[index++] = 'r';
cRt[index++] = ':';
lengths[1] = float_to_uint8_arry(cRt + index, currentRpm_right, 2);
index += lengths[1];
cRt[index++] = ' ';

cRt[index++] = 'F';
cRt[index++] = ':';
lengths[2] = float_to_uint8_arry(cRt + index, (float)distanceFront, 3);
index += lengths[2];
cRt[index++] = ' ';
cRt[index++] = 'L';
cRt[index++] = ':';
lengths[3] = float_to_uint8_arry(cRt + index, (float)distanceLeft, 2);
index += lengths[3];
cRt[index++] = ' ';
cRt[index++] = 'R';
cRt[index++] = ':';
lengths[4] = float_to_uint8_arry(cRt + index, (float)distanceRight, 2);
index += lengths[4];
cRt[index++] = '\n';
cRt[index++] = 'D';
cRt[index++] = ':';
lengths[5] = float_to_uint8_arry(cRt + index, deriSpeed, 0);
index += lengths[5];
cRt[index++] = '\n';

HAL_UART_Transmit_DMA(&huart5, cRt, index); // Use index for the length of the array



	}

}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){//需要在main里启动接收函数
	if(huart==&huart5){

		switch (rxDataBT[0]) {
		    case 'w':
		    case 's':
		    case 'd':
		    case 'a': //数据形式应该是“w1000”这样的
		    	//memset(rxDataOp,0,sizeof rxDataOp);
		    	//centralSpeed=deriSpeed=0;
		        setDirection(rxDataBT[0]);
		        rpmLeft = uint8_to_float(rxDataBT + 1, 0);
		        rpmRight = uint8_to_float(rxDataBT + 1, 0);
		        mode=0;
		        break;
		    /*case 'P': //数据形式应该是"p0.5"这样的
		        kp = uint8_to_float(rxDataBT + 1, 2);
		        break;
		    case 'I':
		        ki = uint8_to_float(rxDataBT + 1, 2);
		        break;
		    case 'D':
		        kd = uint8_to_float(rxDataBT + 1, 2);
		        break;*/
		    case 'q':
		    	memset(rxDataOp,0,sizeof rxDataOp);
		    	centralSpeed=deriSpeed=0;
		    	rpmLeft=0;
		    	rpmRight=0;
		    	mode=0;
		        break;
		    case 'C':
		        craw_state = 1;
		        break;
		    case 'M':
		        craw_state = 2;
		        break;
		    case 'l':
		    	//memset(rxDataOp,0,sizeof rxDataOp);
		    	//centralSpeed=deriSpeed=0;
		        setDirection('w');
		        centralSpeed = uint8_to_float(rxDataBT+1,0);
		        rpmLeft=rpmRight=centralSpeed;
		        mode=1;
		        break;
		    case 'A':
		    	setSpin((int)uint8_to_float(rxDataBT+1,0), 'A');
		    	break;
		    case 'D':
		    	setSpin((int)uint8_to_float(rxDataBT+1,0), 'D');
		    	break;
		    case 'F':
		    	setSpin((int)uint8_to_float(rxDataBT+1,0), 'F');
		    	break;
		    case 'S':
		    	setSpin((int)uint8_to_float(rxDataBT+1,0), 'S');
		    	break;
		    case 'k':
		    	mode=-1;
		    	break;
		    default:
		        // Handle unexpected cases
		        break;
		}
/*
			if (rxDataBT[0]=='w'||rxDataBT[0]=='s'||rxDataBT[0]=='d'||rxDataBT[0]=='a'){//数据形式应该是“w1000”这样的
			setDirection(rxDataBT[0]);
			rpmLeft=uint8_to_float(rxDataBT+1,0);
			rpmRight=uint8_to_float(rxDataBT+1,0);
			}else if (rxDataBT[0]=='P'){//数据形式应该是"p0.5"这样的
				kp=uint8_to_float(rxDataBT+1,2);
			}else if (rxDataBT[0]=='I'){
				ki=uint8_to_float(rxDataBT+1,2);
			}else if (rxDataBT[0]=='D'){
				kd=uint8_to_float(rxDataBT+1,2);
			}else if (rxDataBT[0]=='q'){
				rpmLeft=0;
				rpmRight=0;
			}
			else if(rxDataBT[0]=='C'){

				craw_state=1;


			}
			else if(rxDataBT[0]=='M'){
				craw_state=2;

			}else if(rxDataBT[0]=='l'){
				setDirection('w');
				centralSpeed=uint8_to_float(rxDataBT+1,0);
				rpmLeft=centralSpeed;
				rpmRight=centralSpeed;

			}*/

		uint8_t message[]="Order Received!";
		//float_to_uint8_arry(message,pulseLeft,0);
		//float_to_uint8_arry(message+4,pulseRight,0);
		HAL_UART_Transmit_DMA(&huart5, message, sizeof message);
		memset(rxDataBT,0,sizeof rxDataBT);
		rxDataBT[0]=0;
		leftError=0;rightError=0;error_last_left = 0;error_before_left= 0;error_last_right = 0;error_before_right = 0;
		HAL_UARTEx_ReceiveToIdle_DMA(&huart5, rxDataBT, sizeof rxDataBT);
	}else if (huart==&huart4){
		if (mode==-1){
			switch (rxDataOp[0]) {
			   case 'S':
				   setDirection('a');
				   rpmLeft = 20;
				   rpmRight = 20;
				  // memset(rxDataOp,0,sizeof rxDataOp);
				   break;
			   case 'x':
				   rpmLeft=0;
				   rpmRight=0;
				   //memset(rxDataOp,0,sizeof rxDataOp);
				   break;
			   case 'y':
				   craw_state=1;
				   mode=1;
				   break;
			   case 'w':
				   if (currentRpm_left==0&&currentRpm_right==0){
				   setDirection('w');
				   rpmLeft = 20;
				   rpmRight = 20;}
				   break;
			   case 's':
				   if (currentRpm_left==0&&currentRpm_right==0){
					   setDirection('s');
					   rpmLeft = 20;
					   rpmRight = 20;}
				   break;
			   default:
				   break;
			}
			uint8_t mess[1]={};
			mess[0]=rxDataOp[0];
			HAL_UART_Transmit_DMA(&huart5, mess, 1);
			memset(rxDataOp,0,sizeof rxDataOp);
			}

		if (mode==1){
		int i=0;
		for(;i < sizeof rxDataOp;i++){
			if(rxDataOp[i]=='x'){
				break;
			}
		}
		deriSpeed=uint8_to_float(rxDataOp+i+1,0);
		if (deriSpeed>centralSpeed){

			deriSpeed=centralSpeed;
		}else if (deriSpeed<-centralSpeed){
			deriSpeed=-centralSpeed;
		}
		rpmLeft=centralSpeed+deriSpeed;
		rpmRight=centralSpeed-deriSpeed;
		if (rpmLeft>250  ){
			rpmLeft=250;
		}else if (rpmLeft<0){
			rpmLeft=0;
		}
		if (rpmRight>250  ){
			rpmRight=250;
		}else if (rpmRight<0){
			rpmRight=0;
		}
		memset(rxDataOp,0,sizeof rxDataOp);
		}
		HAL_UARTEx_ReceiveToIdle_DMA(&huart4, rxDataOp, sizeof rxDataOp);
	}

}


