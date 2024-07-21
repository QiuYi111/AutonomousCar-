/*
 * ultraSonic.c
 *
 *  Created on: Jul 21, 2024
 *      Author: 25138
 */
#include "stm32f4xx_hal.h"
#include "ultrasonic.h"
#include <stdint.h>
/* Private variables ---------------------------------------------------------*/

// 函数定义
uint16_t MeasureDistance(TIM_HandleTypeDef TIMER, GPIO_TypeDef* TRIG_PORT, uint16_t TRIG_PIN, GPIO_TypeDef* ECHO_PORT, uint16_t ECHO_PIN) {
    uint32_t pMillis;
    uint32_t Value1 = 0;
    uint32_t Value2 = 0;
    uint16_t Distance;
	// 启动定时器
   // HAL_TIM_Base_Start(&TIMER);
    __HAL_TIM_SET_COUNTER(&TIMER,0);
    // 拉高触发引脚
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
    __HAL_TIM_SET_COUNTER(&TIMER, 0);
    while (__HAL_TIM_GET_COUNTER(&TIMER) < 10);  // 等待10微秒
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);

    // 避免无限循环
    pMillis = HAL_GetTick();
    while (!(HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN)) && pMillis + 10 > HAL_GetTick());
    Value1 = __HAL_TIM_GET_COUNTER(&TIMER);

    pMillis = HAL_GetTick();
    while ((HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN)) && pMillis + 50 > HAL_GetTick());
    Value2 = __HAL_TIM_GET_COUNTER(&TIMER);

    // 计算距离
    Distance = (Value2 - Value1) * 0.34 / 2;

    return Distance;
}





