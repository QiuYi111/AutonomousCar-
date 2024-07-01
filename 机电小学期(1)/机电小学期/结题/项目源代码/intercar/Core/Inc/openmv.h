/*
 * openmv.h
 *
 *  Created on: Jul 15, 2022
 *      Author: 86152
 */

#ifndef INC_OPENMV_H_
#define INC_OPENMV_H_


#endif /* INC_OPENMV_H_ */

#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "control.h"
#include "stdio.h"

extern short mode;
extern short MODE;
double deviation;
double deviation_f;
int deviation_s;

void Openmv_Receive_Data(int16_t Com_Data);
void extract_MVRX1();
void extract_MVRX2();
void extract_MVRX3();
