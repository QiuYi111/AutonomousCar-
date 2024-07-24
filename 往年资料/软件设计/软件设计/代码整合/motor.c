/*
 * motor.c
 *
 *  Created on: Jul 21, 2023
 *      Author: 21671
 */

#include"motor.h"
#include"PID.h"

int wheel1_encoder_num = 0;
int wheel1_last_encoder_num = 0;
int wheel1_last_speed = 0;

int wheel2_encoder_num = 0;
int wheel2_last_encoder_num = 0;
int wheel2_last_speed = 0;

void wheel_init(){
	  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);      //�???启编码器定时�???
	  HAL_TIM_Base_Start_IT(&htim5);                       //�???�???10ms定时器中�???
	  __HAL_TIM_SET_COUNTER(&htim4, 0);                //编码器定时器初始值设定为0

	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);				//�???启timer1ch1 pwm


	  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);      //�???启编码器定时�???
	  HAL_TIM_Base_Start_IT(&htim5);                       //�???�???10ms定时器中�???
	  __HAL_TIM_SET_COUNTER(&htim2, 0);                //编码器定时器初始值设定为0

	  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);				//�???启timer1ch1 pwm
};

void wheel1_back(){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
};

void wheel1_forward(){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
};

void wheel1_stop(){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
};

int wheel1_speed(){
	wheel1_last_encoder_num = wheel1_encoder_num;
	wheel1_encoder_num = COUNTERNUM1;
	int tmp = wheel1_encoder_num - wheel1_last_encoder_num;
	//float speed = tmp * 22.4591;
	float speed = tmp * 22.4591 / 5;
	printf("wheel1  Encoder: %d; Speed: %f mm/s \n", tmp, speed);
	return speed;
};

void set_wheel1_speed(int set_speed){
	if(set_speed > 0)
		wheel1_forward();
	else if(set_speed < 0)
		wheel1_back();
	else
		wheel1_stop();

	int PWM_adjust = 0;
	int now_speed = wheel1_speed();

	if(now_speed < -5000 || now_speed > 5000)
		now_speed = wheel1_last_speed;
	else
		wheel1_last_speed = now_speed;

	PWM_adjust = speed_PID1(set_speed, now_speed);
	printf("%d\n",  PWM_adjust);

	if(PWM_adjust >= 0)__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, PWM_adjust);
	else{
		if(set_speed > 0){
			wheel1_back();
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, -PWM_adjust);
		}
		else if(set_speed < 0){
			wheel1_forward();
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, -PWM_adjust);
		}
	}
}

void wheel2_back(){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
};

void wheel2_forward(){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
};

void wheel2_stop(){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
};

int wheel2_speed(){
	wheel2_last_encoder_num = wheel2_encoder_num;
	wheel2_encoder_num = COUNTERNUM2;
	int tmp = wheel2_encoder_num - wheel2_last_encoder_num;
	//float speed = tmp * 22.4591;
	float speed = tmp * 22.4591 / 5;
	printf("wheel2  Encoder: %d; Speed: %f mm/s \n", tmp, speed);
	return speed;
};

void set_wheel2_speed(int set_speed){
	if(set_speed > 0)
		wheel2_forward();
	else if(set_speed < 0)
		wheel2_back();
	else
		wheel2_stop();

	int PWM_adjust = 0;
	int now_speed = wheel2_speed();

	if(now_speed < -5000 || now_speed > 5000)
		now_speed = wheel2_last_speed;
	else
		wheel2_last_speed = now_speed;

	PWM_adjust = speed_PID2(set_speed, now_speed);
	printf("%d\n",  PWM_adjust);

	if(PWM_adjust >= 0)__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, PWM_adjust);
	else{
		if(set_speed > 0){
			wheel2_back();
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, -PWM_adjust);
		}
		else if(set_speed < 0){
			wheel2_forward();
			__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, -PWM_adjust);
		}
	}
}
