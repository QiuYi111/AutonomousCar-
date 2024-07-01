/*
 * servo.c
 *
 *  Created on: May 18, 2022
 *      Author: qcllc
 */
#include "servo.h"

short target_servo_degree;
short servo_clock;
short direction;
short echo_distance = 1000;
short min_distance = 1000;
short wave = 0;

void servo_init()
{
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	target_servo_degree = 90;
	servo_set(90);
	direction = 1;
	servo_clock = 0;
}
void servo_set(short degree)
{
	short set_degree = (float)degree/180*(MAX_SERVO-MIN_SERVO)+MIN_SERVO;
	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,set_degree);
}

void servo_scan()
{
	servo_clock = 0;
	if(servo_clock==0)
	{

		min_distance = fmin(echo_distance,min_distance);
		target_servo_degree +=  direction *SERVO_STEP ;
		target_servo_degree = fmax(fmin(110,target_servo_degree),55);
		if(target_servo_degree == 110)
			direction = -1;
		if(target_servo_degree == 55)
		{
			if(min_distance>400 && abs(mode)==2 && started)
			{
				wave++;
				if(wave ==5)
				{
					mode = 3*mode/2;
					wave = 0;

				}
			}

			min_distance = 1000;
			direction = 1;
		}
		servo_set(target_servo_degree);
	}
}

void TRIG_pulse()
{
	LL_GPIO_SetOutputPin(HCSR04_PORT, HCSR04_TRIG); // 开始发送超声波
	LL_mDelay(0.2); // 等待20ms
	LL_GPIO_ResetOutputPin(HCSR04_PORT, HCSR04_TRIG); // 停止发送超声波
}

// 全局变量
void TRIG_echo()
{
	int count;
	while(LL_GPIO_IsInputPinSet(HCSR04_PORT,HCSR04_ECHO)==1); //echo为高电平，等待至其低电平，即没有在接受阶段时，才启动超声波
	TRIG_pulse(); //启动超声波
	int j;
	while(LL_GPIO_IsInputPinSet(HCSR04_PORT,HCSR04_ECHO) == 0)
	{

	} //等待echo的高电平到来
	LL_TIM_SetCounter(TIM2,0); //清零计数
	LL_TIM_EnableCounter(TIM2); //开始计数
	while(LL_GPIO_IsInputPinSet(HCSR04_PORT,HCSR04_ECHO) ==1); //等待echo的高电平结束
	LL_TIM_DisableCounter(TIM2); //截止计数
	count=LL_TIM_GetCounter(TIM2); //记录定时器计数
	echo_distance=count * 340 / 2 / 1000; //时间到距离换算，单位为cm
}
