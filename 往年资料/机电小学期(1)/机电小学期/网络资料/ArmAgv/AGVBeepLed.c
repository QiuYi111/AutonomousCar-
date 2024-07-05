#include "stm32f10x_gpio.h"
#include "STM32Type.h"
#include "AGVBeepLed.h"

//GPIO���ú���
void vInitBeepLed(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;//����һ���ṹ���Ա��IO���Ž�������
	
	RCC_APB2PeriphClockCmd(SECLED_CLK , ENABLE);//ʹ��GPIOʱ��
	GPIO_InitStructure.GPIO_Pin = SECLED_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SECLED_GPIO, &GPIO_InitStructure);
	SET_SECLED;
	
	RCC_APB2PeriphClockCmd(BEEP_CLK , ENABLE);//??GPIO??
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 	
	GPIO_Init(BEEP_GPIO, &GPIO_InitStructure);	
	SET_BEEP;	
}
