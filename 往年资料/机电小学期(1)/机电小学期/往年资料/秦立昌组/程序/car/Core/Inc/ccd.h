/*
 * ccd.h
 *
 *  Created on: May 25, 2022
 *      Author: qcllc
 */

#ifndef INC_CCD_H_
#define INC_CCD_H_

#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "tools.h"
#include "stm32f1xx_hal_def.h"
#include "stdint-gcc.h"
#include "adc.h"

#endif /* INC_CCD_H_ */

#define KPR 5
#define KIR 0.02
#define KDR 0.02




extern int ADC_Value;
extern int ADV[128];
extern int bool_array[128];
extern int result[2];
extern short last_pos;
extern short mode;
extern short count;
extern short P_count;

int Get_Adc(void);
void filter(int gray_array[], int bool_array[]);
void count_continuous(int bool_array[], int result[]);
void carmera_take();
int get_camera_pos();
short PID_cal_rot(short target_rot,short rot,short *error);

