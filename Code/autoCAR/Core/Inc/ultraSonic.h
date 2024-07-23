/*
 * ultraSonic.h
 *
 *  Created on: Jul 15, 2024
 *      Author: 25138
 */

#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H
#include <stdint.h>
#include "stm32f4xx_hal.h"


// 函数声明
#ifdef __cplusplus
extern "C" {
#endif
typedef struct{
	GPIO_TypeDef* Trig_PORT;
	uint16_t Trig_PIN;
	GPIO_TypeDef* Echo_PORT;
	uint16_t Echo_PIN;
}ultraSonic;
extern  ultraSonic ultraSonicFront, ultraSonicLeft, ultraSonicRight;
extern uint16_t  distanceLeft,distanceRight,distanceFront;
uint16_t MeasureDistance(ultraSonic * ultraSonicInst);

#ifdef __cplusplus
}
#endif

#endif // ULTRASONIC_H
