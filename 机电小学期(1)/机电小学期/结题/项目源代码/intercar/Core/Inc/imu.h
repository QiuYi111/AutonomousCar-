/*
 * imu.h
 *
 *  Created on: Jul 24, 2022
 *      Author: 86152
 */

#ifndef INC_IMU_H_
#define INC_IMU_H_

#endif /* INC_IMU_H_ */
#include "main.h"
#include "stdio.h"
#include <string.h>

#define IMUBUFFER_LEN 11		//接收3类数据，一共33位


float theta;
float theta0;
uint8_t IMURxBuffer[IMUBUFFER_LEN];
short imu_check;

void JY901_Process();
