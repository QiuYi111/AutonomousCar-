/*
 * motor.h
 *
 *  Created on: 2022年7月8日
 *      Author: 86152
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#endif /* INC_MOTOR_H_ */
#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "stm32f1xx_hal_def.h"
#include "stdint-gcc.h"
#include <stdio.h>
#include <stdlib.h>

#define reduction_ratio 21.3
#define pulsenum 11.0
#define diameter 65.0//mm
#define KP 0.9
#define KI 0.3
#define KD 0.1
#define pi 3.1416
#define maxspeed 800

int leftspeed;
int rightspeed;
int leftfeedback;
int rightfeedback;
int lefterror;
int righterror;

void motor_init();//init basic settings for motors
void motor_PWM(int16_t leftspeed,int16_t rightspeed);//set the PWM value of motors
int motor_leftfeedback();//read the left side motor's encoder
int motor_rightfeedback();//read the right side motor's encoder
int leftspeedPID(int target_speed,int speed,int *error);//calculate needed adjustment of left motor speed
int rightspeedPID(int target_speed,int speed,int *error);//calculate needed adjustment of right motor speed
