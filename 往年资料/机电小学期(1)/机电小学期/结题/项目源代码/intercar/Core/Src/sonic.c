#include "sonic.h"
/*
 * sonic.c
 *
 *  Created on: Jul 9, 2022
 *      Author: 86152
 */
void sonic_delay(uint16_t time)
{
	__HAL_TIM_SET_COUNTER(&htim4,0);
	while(__HAL_TIM_GET_COUNTER(&htim4)<time);
}
int get_distance1()
{
	//int tempt=0;
	//for(int i=0;i<2;i++){
	   HAL_GPIO_WritePin(TRIG1_GPIO_Port,TRIG1_Pin,GPIO_PIN_RESET);
	   sonic_delay(2);
	   HAL_GPIO_WritePin(TRIG1_GPIO_Port,TRIG1_Pin,GPIO_PIN_SET);
	   sonic_delay(12);
	   HAL_GPIO_WritePin(TRIG1_GPIO_Port,TRIG1_Pin,GPIO_PIN_RESET);
	   __HAL_TIM_ENABLE_IT(&htim4,TIM_IT_CC1);
	   total_delay(125);
    //tempt+=distance1;
    //}
    //return tempt/2;
    return distance1;
}
int get_distance2()
{
   //int tempt=0;
   //for(int i=0;i<2;i++){
      HAL_GPIO_WritePin(TRIG2_GPIO_Port,TRIG2_Pin,GPIO_PIN_RESET);
      sonic_delay(2);
      HAL_GPIO_WritePin(TRIG2_GPIO_Port,TRIG2_Pin,GPIO_PIN_SET);
      sonic_delay(12);
      HAL_GPIO_WritePin(TRIG2_GPIO_Port,TRIG2_Pin,GPIO_PIN_RESET);
      __HAL_TIM_ENABLE_IT(&htim4,TIM_IT_CC2);
      total_delay(125);
      //tempt+=distance2;
   //}
   //return tempt/2;
      return distance2;
}
int get_distance3()
{
   //int tempt=0;
   //for(int i=0;i<2;i++){
      HAL_GPIO_WritePin(TRIG3_GPIO_Port,TRIG3_Pin,GPIO_PIN_RESET);
      sonic_delay(2);
      HAL_GPIO_WritePin(TRIG3_GPIO_Port,TRIG3_Pin,GPIO_PIN_SET);
      sonic_delay(12);
      HAL_GPIO_WritePin(TRIG3_GPIO_Port,TRIG3_Pin,GPIO_PIN_RESET);
      __HAL_TIM_ENABLE_IT(&htim4,TIM_IT_CC3);
      total_delay(125);
      //tempt+=distance3;
   //}
   //return tempt/2;
      return distance3;
}
int get_distance4()
{
   //int tempt=0;
   //for(int i=0;i<2;i++){
      HAL_GPIO_WritePin(TRIG4_GPIO_Port,TRIG4_Pin,GPIO_PIN_RESET);
      sonic_delay(2);
      HAL_GPIO_WritePin(TRIG4_GPIO_Port,TRIG4_Pin,GPIO_PIN_SET);
      sonic_delay(12);
      HAL_GPIO_WritePin(TRIG4_GPIO_Port,TRIG4_Pin,GPIO_PIN_RESET);
      __HAL_TIM_ENABLE_IT(&htim4,TIM_IT_CC4);
      total_delay(125);
      //tempt+=distance4;
   //}
   //return tempt/2;
      return distance4;
}


