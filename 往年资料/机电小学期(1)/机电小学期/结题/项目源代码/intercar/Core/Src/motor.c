/*
 * motor.c
 *
 *  Created on: 2022年7月8日
 *      Author: 86152
 */
#include "motor.h"

const double compute_factor =reduction_ratio*4*pulsenum*delay;

void motor_init()
{
	  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
	  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);
	  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	  HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
	  HAL_TIM_Base_Start_IT((TIM_HandleTypeDef *)&htim7);
	  motor_PWM(0, 0);
}
void motor_PWM(int16_t leftspeed,int16_t rightspeed)
{
	if(leftspeed > maxspeed)
		leftspeed = maxspeed;
	if(rightspeed > maxspeed)
		rightspeed = maxspeed;
	if(leftspeed < -maxspeed)
		leftspeed = -maxspeed;
	if(rightspeed < -maxspeed)
		rightspeed = -maxspeed;
	if(leftspeed > 0){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET); //
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);//
	}
	else if(leftspeed < 0){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET); //
	}
	else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);//
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET);//
	}

	if(rightspeed > 0){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET);//
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET); //
	}
	else if(rightspeed < 0){
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);//
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET); //
	}
	else{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);//
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);//
	}
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_3,abs(leftspeed));
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_4,abs(rightspeed));
}

int motor_leftfeedback()
{
	int countnum =(short)__HAL_TIM_GET_COUNTER(&htim2);	  //读取编码器数�????
	__HAL_TIM_SET_COUNTER(&htim2,0);
	int speed;
	speed =1050*diameter*pi*countnum/ compute_factor;
	return speed;
}

int motor_rightfeedback()
{
	int countnum =(short)__HAL_TIM_GET_COUNTER(&htim8);	  //读取编码器数�????
	__HAL_TIM_SET_COUNTER(&htim8,0);
	int speed;
	speed = 1050*diameter*pi*countnum/ compute_factor;
	return speed;
}

int leftspeedPID(int targetspeed,int speed,int *error)
{
	int lefterror = targetspeed - speed;
	static int lefterror_last = 0,lefterror_before = 0;
	int pwm_pid=0;
	*error = lefterror;
	pwm_pid = KP*(lefterror-lefterror_last)+KI*lefterror
			+KD*(lefterror-2*lefterror_last+lefterror_before);
	lefterror_before = lefterror_last;	  	    // 保存上上次误差
	lefterror_last = lefterror;	              // 保存上次偏差
	return pwm_pid;
}

int rightspeedPID(int targetspeed,int speed,int *error)
{
	int righterror = targetspeed - speed;
	static int righterror_last = 0,righterror_before = 0;
	int pwm_pid=0;
	*error =righterror;
	pwm_pid = KP*(righterror-righterror_last)+KI*righterror+
			KD*(righterror-2*righterror_last+righterror_before);
	righterror_before = righterror_last;	  	    // 保存上上次误差
	righterror_last = righterror;	              // 保存上次偏差
	return pwm_pid;
}
