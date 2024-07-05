#ifndef _BEEPLED_H_
#define _BEEPLED_H_

	#include "stm32f10x.h"

	///////////////////////////////////// LED //////////////////////////////////////////////
	#define SECLED_PIN		GPIO_Pin_13
	#define SECLED_GPIO		GPIOC
	#define SECLED_CLK		RCC_APB2Periph_GPIOC

	#define SET_SECLED		SECLED_GPIO->BSRR = SECLED_PIN
	#define CLR_SECLED		SECLED_GPIO->BRR = SECLED_PIN
	#define XOR_SECLED		SECLED_GPIO->ODR ^= SECLED_PIN
	//#define SET_SECLED		GPIO_SetBits(SECLED_GPIO , SECLED_PIN)
	//#define CLR_SECLED		GPIO_ResetBits(SECLED_GPIO , SECLED_PIN)
	
	///////////////////////////////////// BEEP /////////////////////////////////////////////
	#define BEEP_PIN		GPIO_Pin_3
	#define BEEP_GPIO		GPIOG
	#define BEEP_CLK		RCC_APB2Periph_GPIOG

	#define SET_BEEP		BEEP_GPIO->BSRR = BEEP_PIN
	#define CLR_BEEP		BEEP_GPIO->BRR = BEEP_PIN
	#define XOR_BEEP		BEEP_GPIO->ODR ^= BEEP_PIN
	
	#define OFF_BEEP		BEEP_GPIO->BSRR = BEEP_PIN
	#define ON_BEEP		    BEEP_GPIO->BRR = BEEP_PIN
	
	void vInitBeepLed(void);
	
#endif
