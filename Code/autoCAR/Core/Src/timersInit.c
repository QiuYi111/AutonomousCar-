/*
 * timersInit.c
 *
 *  Created on: Jul 21, 2024
 *      Author: 25138
 */
#include "timersInit.h"
void timersInit(){

	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);//开启电机PWM，最值255
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);//开启舵机PWM，最值1999
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);//左轮编码器
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);//右轮编码器
	HAL_TIM_Base_Start_IT(&htim7);//中断定时器，50ms一次中断
	HAL_TIM_Base_Start_IT(&htim9);



}


