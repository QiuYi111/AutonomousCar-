/*
 * PID.h
 *
 *  Created on: Jul 21, 2023
 *      Author: 21671
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "tim.h"
#include "gpio.h"
#include"motor.h"

int speed_PID1(int set_speed, int now_speed);
int speed_PID2(int set_speed, int now_speed);

#endif /* INC_PID_H_ */
