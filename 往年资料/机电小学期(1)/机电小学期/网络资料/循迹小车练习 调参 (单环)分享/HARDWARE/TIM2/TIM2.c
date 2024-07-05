#include "stm32f10x.h"                  // Device header

void TIM2_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;  //�ж����ȼ��Ľṹ�嶨��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 60;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�жϷ�����������
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ��IRQ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM2, DISABLE);   //�ر�TIM2
	TIM2->CNT=0;
	
	
}

void TIM1_Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;  //�ж����ȼ��Ľṹ�嶨��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 600;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x00;
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE );
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1�жϷ�����������
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ��IRQ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM1, DISABLE);  //�ر�TIM1
	TIM1->CNT=0;
	
	
}