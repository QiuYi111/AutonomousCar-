/*
 * servo.h
 *
 *  Created on: Jul 7, 2022
 *      Author: 86152
 */
#ifndef INC_SERVO_H_
#define INC_SERVO_H_

#endif /* INC_SERVO_H_ */

#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "math.h"
#include "stdio.h"


extern short target_servo_degree;
extern short mode;
short is_shut;
short is_down;
short servo_angle;
short servo_dir;
typedef struct
{
	short angle;
	short dir;
}servo;

void servo1_init();
void servo2_init();
void servo3_init();
void servo4_init();
void servo1_turn(short angle);
void servo2_turn(short angle);
void servo4_turn(short angle);
void servo3_scan();
void servo1_down();
void servo2_shut();


