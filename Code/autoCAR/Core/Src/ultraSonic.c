/*
 * ultraSonic.c
 *
 *  Created on: Jul 21, 2024
 *      Author: 25138
 */
#include "stm32f4xx_hal.h"
#include "ultrasonic.h"
#include "main.h"
#include "tim.h"
#include <stdint.h>
/* Private variables ---------------------------------------------------------*/
ultraSonic ultraSonicFront={trigFront_GPIO_Port,trigFront_Pin,echoFront_GPIO_Port,echoFront_Pin};
ultraSonic ultraSonicLeft={trigLeft_GPIO_Port,trigLeft_Pin,echoLeft_GPIO_Port,echoLeft_Pin};
ultraSonic ultraSonicRight={trigRight_GPIO_Port,trigRight_Pin,echoRight_GPIO_Port,echoRight_Pin};
uint16_t distanceLeft=0;
uint16_t distanceRight=0;
uint16_t distanceFront=0;
// 函数定义
uint16_t MeasureDistance(ultraSonic* ultraSonicInst) {
    uint32_t pMillis;
    uint32_t Value1 = 0;
    uint32_t Value2 = 0;
    uint16_t Distance;
    	GPIO_TypeDef* TRIG_PORT=ultraSonicInst->Trig_PORT;
    	uint16_t TRIG_PIN=ultraSonicInst->Trig_PIN;
    	GPIO_TypeDef* ECHO_PORT=ultraSonicInst->Echo_PORT;
    	uint16_t ECHO_PIN=ultraSonicInst->Echo_PIN;
	// 启动定时器
    //HAL_TIM_Base_Start(&htim10);
    //__HAL_TIM_SET_COUNTER(&TIMER,0);
    // 拉高触发引脚
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
    __HAL_TIM_SET_COUNTER(&htim10, 0);
    while (__HAL_TIM_GET_COUNTER(&htim10) < 10);  // 等待10微秒
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);

    // 避免无限循环
    pMillis = HAL_GetTick();
    while (!(HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN)) && pMillis + 10 > HAL_GetTick());
    Value1 = __HAL_TIM_GET_COUNTER(&htim10);

    pMillis = HAL_GetTick();
    while ((HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN)) && pMillis + 50 > HAL_GetTick());
    Value2 = __HAL_TIM_GET_COUNTER(&htim10);

    // 计算距离
    Distance = (Value2 - Value1) * 0.34 / 2;
   // HAL_TIM_Base_Stop(&htim10);
    return Distance;
}





