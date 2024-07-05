/*
 * sonic.h
 *
 *  Created on: Jul 9, 2022
 *      Author: 86152
 */

#ifndef INC_SONIC_H_
#define INC_SONIC_H_

#endif /* INC_SONIC_H_ */

#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "math.h"
#include "stdio.h"
#include "main.h"

int distance1;
int distance2;
int distance3;
int distance4;
int is_close1;
int is_close2;
int is_close3;
int is_close4;

void sonic_delay(uint16_t time);
int get_distance1();
int get_distance2();
int get_distance3();
int get_distance4();
