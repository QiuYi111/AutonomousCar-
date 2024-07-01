/*
 * servo.c
 *
 *  Created on: Jul 7, 2022
 *      Author: 86152
 */

#include "servo.h"

void servo1_init()
{
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	servo servo1={20,0};
	servo1_turn(servo1.angle);
}
void servo1_turn(short angle)
{
	short degree = 50+10*angle/9;
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,degree);
}
void servo2_init()
{
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	servo servo2={80,0};
	servo2_turn(servo2.angle);
}
void servo2_turn(short angle)
{
	short degree = 50+10*angle/9;
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,degree);
}
void servo3_init()
{
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	servo_angle=0;
	servo_dir=0;
	short degree = 50+servo_angle;
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,degree);
}

void servo3_scan()
{
	if (servo_dir==0){servo_angle+=30;}
	else if(servo_dir==1){servo_angle-=30;}
	if(servo_angle==180){servo_dir=1;}
	if(servo_angle==0){servo_dir=0;}
	short degree = 40+7*servo_angle/6;
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,degree);
}
void servo4_init()
{
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	servo servo4={60,0};
	servo4_turn(servo4.angle);
}
void servo4_turn(short angle)
{
	short degree = 50+10*angle/9;
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,degree);
}
void servo1_down()
{
	for(int i=0;i<34;i++){
		if(is_down==0){servo1_turn(20+3*i);}
		else if(is_down==1){servo1_turn(122-3*i);}
		total_delay(500);
	}
}
void servo2_shut()
{
	for(int i=0;i<21;i++){
		if(is_shut==0){servo2_turn(80-3*i);}
		else if(is_shut==1){servo2_turn(17+3*i);}
		total_delay(500);
	}
}
