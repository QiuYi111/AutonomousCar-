/*
 * tools.h
 *
 *  Created on: May 11, 2022
 *      Author: qcllc
 */

#ifndef INC_TOOLS_H_
#define INC_TOOLS_H_

#include "stdio.h"
#include "tim.h"
#include "gpio.h"
#include "motor.h"
#include "usart.h"
#include "dma.h"
#include "servo.h"
#include "ccd.h"
#include "stm32f1xx_hal_def.h"
#include "stdint-gcc.h"

void excute_command(char *command);
void char2int(char *arr);
void float2char(char *str, double number, uint8_t g, uint8_t l);
int int2char(char *arr, int num, int arr_size);
char *reverse(char *s);


#endif /* INC_TOOLS_H_ */

