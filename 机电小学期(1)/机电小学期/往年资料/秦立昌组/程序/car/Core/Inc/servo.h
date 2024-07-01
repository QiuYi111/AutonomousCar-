/*
 * servo.h
 *
 *  Created on: May 18, 2022
 *      Author: qcllc
 */

#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#endif /* INC_SERVO_H_ */
#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "tools.h"
#include "math.h"
#include "stdio.h"
#include "main.h"

#define SERVO_STEP 80
#define SERVO_TIME 30
#define S_DELAY 20
#define HCSR04_PORT GPIOD
#define HCSR04_TRIG LL_GPIO_PIN_11
#define HCSR04_ECHO LL_GPIO_PIN_9
#define ECHO_Reci LL_GPIO_IsInputPinSet(HCSR04_PORT, LL_GPIO_PIN_9)

extern short target_servo_degree;
extern short servo_clock;
extern short echo_distance;
extern short min_distance;
void servo_init();
void servo_set(short degree);
void servo_scan();
void TRIG_echo();
void TRIG_pulse();
