/*
 * motor.c
 *
 *  Created on: Jul 20, 2024
 *      Author: 25138
 */
#include "motor.h"

float kp,ki,kd;
float leftError,rightError;
void setLeftRpm(float rpm){
	float currentRpm=getRpm(&htim2);
	int pulse=0;pulse+=pidDiff(rpm, currentRpm, &leftError);
	setPWM(pulse,TIM_CHANNEL_3);
}
void setRightRpm(float rpm){
	float currentRpm=getRpm(&htim2);
	int pulse=0;pulse+=pidDiff(rpm, currentRpm, &rightError);
	setPWM(pulse,TIM_CHANNEL_4);
}
void setPID(char factor,float index){
	if (factor=='p'){kp=index;}
	else if (factor=='i'){ki=index;}
	else if (factor=='d'){kd=index;};
}
int pidDiff(float target,float present,float * erro){
	float error = target - present;
		static int error_last = 0,error_before = 0;
		float pwm_pid=0;
		*erro = error;
		pwm_pid = kp*(error- error_last)+ki* error
				+kd*( error-2* error_last+ error_before);
		 error_before =  error_last;	  	    // 保存上上次误差
		 error_last =  error;	              // 保存上次偏差
		 return (int)pwm_pid;
}
float getRpm(TIM_HandleTypeDef * htim){
	uint16_t countnum =__HAL_TIM_GET_COUNTER(htim);
	float rpm;
	rpm = (4*countnum)/3;//读取编码器数�????
	__HAL_TIM_SET_COUNTER(htim,0);
	return rpm;
}
void setPWM(int pulse,uint32_t channel){
	int pwm_pulse;
	if(pulse>0){pwm_pulse=pulse;}else{pwm_pulse=-pulse;};
	if(pwm_pulse>=255){pwm_pulse=255;};
	__HAL_TIM_SET_COMPARE(&htim5,channel,pwm_pulse);
}
void setDirection(char dir){
	if (dir=='w'){
		HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, GPIO_PIN_RESET);
	}
	else if (dir=='s'){
		HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, GPIO_PIN_SET);
	}
	else if (dir=='a'){
		HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, GPIO_PIN_SET);
	}
	else if (dir=='d'){
		HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, GPIO_PIN_RESET);
	}
}
