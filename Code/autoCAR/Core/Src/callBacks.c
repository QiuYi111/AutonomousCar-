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
extern uint8_t rxData[50];
extern float rpmLeft,rpmRight;//定义在main里的，用于存放rpm数据
extern int pulseLeft,pulseRight;
 float kp=1,ki=0.5,kd=1.5; int pulseLeft=0;int pulseRight=0;
float leftError=0,rightError=0; float error_last_left = 0,error_before_left= 0;float error_last_right = 0,error_before_right = 0;
uint8_t cRt[256]={};
int delayer=0;
float currentRpm_left=0;
float currentRpm_right=0;
int float_to_uint8_arry(uint8_t* u8Arry, float floatdata, int precision) {//float给定精度转换为uint8_t
	int points = 0;
	float data1 = floatdata;
	for (; data1 >=1; points++) {
		data1 /= 10;
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
		else {
			u8Arry[points] = '.';
		}
	}
	return points + precision + 1;

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
	}else if (htim ==&htim9){
		//currentRpm_left=getLeftRpm(&htim2);
		//currentRpm_right=getRightRpm(&htim4);
		int right_Rpm_length=0,left_Rpm_length=0;
		cRt[0]='l';
		cRt[1]=':';
		left_Rpm_length=float_to_uint8_arry(cRt+2, currentRpm_left, 2);
		cRt[left_Rpm_length+3]=' ';
		cRt[left_Rpm_length+4]='r';
		cRt[left_Rpm_length+5]=':';
		right_Rpm_length=float_to_uint8_arry(cRt+left_Rpm_length+6, currentRpm_right, 2);
		cRt[left_Rpm_length+6+right_Rpm_length]='\n';
		HAL_UART_Transmit_DMA(&huart5, cRt, sizeof cRt);

	}

}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){//需要在main里启动接收函数
	if(huart==&huart5){
		if (rxData[0]=='w'||rxData[0]=='s'||rxData[0]=='d'||rxData[0]=='a'){//数据形式应该是“w1000”这样的
		setDirection(rxData[0]);
		rpmLeft=uint8_to_float(rxData+1,0);
		rpmRight=uint8_to_float(rxData+1,0);
		}else if (rxData[0]=='P'){//数据形式应该是"p0.5"这样的
			kp=uint8_to_float(rxData+1,2);
		}else if (rxData[0]=='I'){
			ki=uint8_to_float(rxData+1,2);
		}else if (rxData[0]=='D'){
			kd=uint8_to_float(rxData+1,2);
		}else if (rxData[0]=='q'){
			rpmLeft=0;
			rpmRight=0;
			//memset(rxData,0,sizeof rxData);
			//rxData[0]=0;
			/*HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, GPIO_PIN_RESET);*/
		}
		uint8_t message[]="Order Received!";
		//float_to_uint8_arry(message,pulseLeft,0);
		//float_to_uint8_arry(message+4,pulseRight,0);
		HAL_UART_Transmit_DMA(&huart5, message, sizeof message);
		memset(rxData,0,sizeof rxData);
		rxData[0]=0;
		leftError=0;rightError=0;error_last_left = 0;error_before_left= 0;error_last_right = 0;error_before_right = 0;
		HAL_UARTEx_ReceiveToIdle_DMA(&huart5, rxData, sizeof rxData);
	}

}

