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
extern uint8_t rxDataBT[50],rxDataOp[30];
extern float rpmLeft,rpmRight,deriSpeed,centralSpeed;//定义在main里的，用于存放rpm数据
extern int pulseLeft,pulseRight;
float kp=1,ki=0.5,kd=1.5; int pulseLeft=0;int pulseRight=0;
float leftError=0,rightError=0; float error_last_left = 0,error_before_left= 0;float error_last_right = 0,error_before_right = 0;
uint8_t cRt[512]={};
int delayer=0;
float currentRpm_left=0;
float currentRpm_right=0;int ultraLoop=0;
extern float craw_state;
int float_to_uint8_arry(uint8_t* u8Arry, float floatdata, int precision) {//float给定精度转换为uint8_t
	int points = 0;
	int apoints = 0;
	float data1 = floatdata;
	for (; data1 >= 1; points++) {
		data1 /= 10;
	}
	if (points == 0) {
		points = 1;
		apoints = 1;
	}
	for (int i = 0; i < precision; i++) {
		floatdata *= 10;
	}
	for (int i = 0; i < points + precision + 1; i++) {
		if (i != precision) {
			if (i == 0) {
				u8Arry[points + precision - i] = (uint8_t)((floor(fmod(floatdata, 10) + 0.5)) + '0');
				floatdata /= 10;
			}
			else {
				u8Arry[points + precision - i] = (uint8_t)((floor(fmod(floatdata, 10))) + '0');
				floatdata /= 10;
			}
		}
		else if(precision!=0){
			u8Arry[points] = '.';
		}

	}
	if (apoints == 1) {
		points = 0;
		return points + precision + 2;
	}
	else if(precision==0){
		return points + precision;
	}
	else {
		return points + precision + 1;
	}

}

// Function to convert uint8_t array to float
float uint8_to_float(uint8_t* u8arry, int point_length) {
	int points = 0;
	int status = 0;
	int num_length = 0;
	float number = 0;
	for (; num_length < 256; num_length++) {
		if (u8arry[num_length] == '.') {
			points = num_length;
		}
		if (u8arry[num_length] == 'o') {
			status = num_length;
			break;
		}
	}
	if (status == 0) {
		return -1;
	}
	else {
		float tens = 1;

		if (points == 0) {
			for (size_t i= 0; i < num_length; i++) {
			number += (float)(u8arry[i] - '0');
			number *= 10;
		}
			}
			else {
				for (int i = 0; i <= points + point_length; i++) {
				if (i < points) {
					number += (float)(u8arry[i] - '0');
					number *= 10;
				}
				else if (i > points) {
					tens /= 10;
					number += ((float)(u8arry[i] - '0')) * tens;
				}
				else {
					number /= 10;
				}
				}
			}

		if (points == 0) {
			number /= 10;
		}
		return number;
	}

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	 if (htim==&htim7){
		 setRightRpm(rpmRight);
		 setLeftRpm(rpmLeft);
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

	}

}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){//需要在main里启动接收函数
	if(huart==&huart5){
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
			//memset(rxDataBT,0,sizeof rxDataBT);
			//rxDataBT[0]=0;
			/*HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, GPIO_PIN_RESET);*/
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

		}
		uint8_t message[]="Order Received!";
		//float_to_uint8_arry(message,pulseLeft,0);
		//float_to_uint8_arry(message+4,pulseRight,0);
		HAL_UART_Transmit_DMA(&huart5, message, sizeof message);
		memset(rxDataBT,0,sizeof rxDataBT);
		rxDataBT[0]=0;
		leftError=0;rightError=0;error_last_left = 0;error_before_left= 0;error_last_right = 0;error_before_right = 0;
		HAL_UARTEx_ReceiveToIdle_DMA(&huart5, rxDataBT, sizeof rxDataBT);
	}else if (huart==&huart4){
		int i=0;
		for(;i < sizeof rxDataOp;i++){
			if(rxDataOp[i]=='x'){
				break;
			}
		}
		deriSpeed=uint8_to_float(rxDataOp+i+1,0);
		rpmLeft=centralSpeed+deriSpeed;
		rpmRight=centralSpeed-deriSpeed;
		memset(rxDataOp,0,sizeof rxDataOp);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart4, rxDataOp, sizeof rxDataOp);
	}

}


