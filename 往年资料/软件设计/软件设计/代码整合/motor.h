/*
 * motor.h
 *
 *  Created on: Jul 21, 2023
 *      Author: 21671
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "tim.h"
#include "gpio.h"

#define COUNTERNUM1 __HAL_TIM_GetCounter(&htim4)        //获取编码器定时器中的计数�??
#define COUNTERNUM2 __HAL_TIM_GetCounter(&htim2)

void wheel_init();

void wheel1_forward();
void wheel1_back();
void wheel1_stop();
int wheel1_speed();
void set_wheel1_speed(int set_speed);

void wheel2_forward();
void wheel2_back();
void wheel2_stop();
int wheel2_speed();
void set_wheel2_speed(int set_speed);

#endif /* INC_MOTOR_H_ */
