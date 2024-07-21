/*
 * motor.c
 *
 *  Created on: Jul 20, 2024
 *      Author: 25138
 */
#include "motor.h"

float kp=0.8,ki=0,kd=0.5;int pulseLeft=0;int pulseRight=0;
float leftError=0,rightError=0;float error_last_left = 0,error_before_left= 0;float error_last_right = 0,error_before_right = 0;
void setLeftRpm(float rpm){
	float currentRpm=getRpm(&htim2);
	pulseLeft+=pidDiffLeft(rpm, currentRpm, &leftError);
	setPWM(pulseLeft,TIM_CHANNEL_3);
}
void setRightRpm(float rpm){
	float currentRpm=getRpm(&htim4);
	pulseRight+=pidDiffRight(rpm, currentRpm, &rightError);
	setPWM(pulseRight,TIM_CHANNEL_4);
}
void setPID(char factor,float index){
	if (factor=='p'){kp=index;}
	else if (factor=='i'){ki=index;}
	else if (factor=='d'){kd=index;};
}
int pidDiffLeft(float target,float present,float * erro){
	float error = target - present;

		float pwm_pid=0;
		*erro = error;
		pwm_pid = kp*(error- error_last_left)+ki* error
				+kd*( error-2* error_last_left+ error_before_left);
		 error_before_left =  error_last_left;	  	    // 保存上上次误差
		 error_last_left =  error;	              // 保存上次偏差
		 return (int)pwm_pid;
}
int pidDiffRight(float target,float present,float * erro){
	float error = target - present;

		float pwm_pid=0;
		*erro = error;
		pwm_pid = kp*(error- error_last_right)+ki* error
				+kd*( error-2* error_last_right+ error_before_right);
		 error_before_right =  error_last_right;	  	    // 保存上上次误差
		 error_last_right =  error;	              // 保存上次偏差
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
	if(pulse>0){pwm_pulse=pulse;}else{pwm_pulse=0;};
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
