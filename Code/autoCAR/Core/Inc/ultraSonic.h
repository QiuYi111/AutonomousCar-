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

uint16_t MeasureDistance(TIM_HandleTypeDef TIMER, GPIO_TypeDef* TRIG_PORT, uint16_t TRIG_PIN, GPIO_TypeDef* ECHO_PORT, uint16_t ECHO_PIN);

#ifdef __cplusplus
}
#endif

#endif // ULTRASONIC_H
