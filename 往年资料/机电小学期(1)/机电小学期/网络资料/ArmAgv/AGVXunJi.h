#ifndef _AGVIRLED_H_
#define _AGVIRLED_H_

	#define L2XJ_PIN         GPIO_Pin_2
	#define L2XJ_GPIO        GPIOE
	#define L2XJ_CLK         RCC_APB2Periph_GPIOE
	#define L2XJ_IN          GPIOE->IDR & L2XJ_PIN
	
	#define L1XJ_PIN         GPIO_Pin_4
	#define L1XJ_GPIO        GPIOE
	#define L1XJ_CLK         RCC_APB2Periph_GPIOE
	#define L1XJ_IN          GPIOE->IDR & L1XJ_PIN
	
	#define R1XJ_PIN         GPIO_Pin_6
	#define R1XJ_GPIO        GPIOE
	#define R1XJ_CLK         RCC_APB2Periph_GPIOE
	#define R1XJ_IN          GPIOE->IDR & R1XJ_PIN
	
	#define R2XJ_PIN         GPIO_Pin_5
	#define R2XJ_GPIO        GPIOE
	#define R2XJ_CLK         RCC_APB2Periph_GPIOE
	#define R2XJ_IN          GPIOE->IDR & R2XJ_PIN	

	extern void vInitXunJi(void);
	extern void vXunJiProc(void);

#endif
