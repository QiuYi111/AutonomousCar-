#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "STM32Type.h"
#include "AGVUart1.h"
#include "misc.h"
#include "stm32f10x_tim.h"

//---------------------------------------------
UINT16 iUSRxOut = 0, iUSRxIn = 0;
UINT16 iUSTxOut = 0, iUSTxIn = 0;

UINT8 cUSRxFIFO[SIZE_US_RXBUF];
UINT8 cUSTxFIFO[SIZE_US_TXBUF];

HBOOL bIsDirectUart1RxRdy = HFALSE;

//UART1 TxD GPIOA9   RxD GPIOA10
void vInitUart1(uint32_t baudRate)
{
	USART_InitTypeDef USART_InitSturct;//���崮��1�ĳ�ʼ���ṹ��
	GPIO_InitTypeDef GPIO_InitStruct;//���崮�ڶ�Ӧ�ܽŵĽṹ��
    NVIC_InitTypeDef NVIC_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//�򿪴��ڹܽ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);//ʱ��ʹ��	

//USART1_Tx_Pin Configure 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//��������ٶ�50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//���츴�����
	GPIO_Init(GPIOA , &GPIO_InitStruct);//����ʼ���õĽṹ��װ��Ĵ���

//USART1_Rx_Pin Configure
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//��������ٶ�50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIOģʽ��������
	GPIO_Init(GPIOA, &GPIO_InitStruct);//����ʼ���õĽṹ��װ��Ĵ���

//USART1 Configure	
	USART_InitSturct.USART_BaudRate = baudRate;//������
	USART_InitSturct.USART_WordLength = USART_WordLength_8b;//���ݿ��8λ
	USART_InitSturct.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitSturct.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitSturct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ʹ�ܷ��������
	USART_Init(USART1 , &USART_InitSturct);//����ʼ���õĽṹ��װ��Ĵ���	

//USART1_INT Configure
	USART_ITConfig(USART1 , USART_IT_RXNE, ENABLE);//ʹ�ܽ����ж�
	USART_ITConfig(USART1 , USART_IT_TC, ENABLE);
	USART_Cmd(USART1 , ENABLE);//�򿪴���
	USART_ClearFlag(USART1 , USART_FLAG_TC);//�����һ�����ݷ���ʧ�ܵ�����	
	
//configure interrupt vector
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void vUart1TxBuf(INT8 *cbuf, INT8 csize)
{
	HBOOL bIsNotIntStat = (iUSTxOut == iUSTxIn);
	
	uint16_t idx;
	for(idx=0; idx<csize; idx++)
	{
		cUSTxFIFO[iUSTxIn] = cbuf[idx];
		iUSTxIn++; iUSTxIn %= SIZE_US_TXBUF;
	}
	if(bIsNotIntStat)
	{
		idx = (cUSTxFIFO[iUSTxOut] & (uint16_t)0x01FF);
		iUSTxOut++; iUSTxOut%=SIZE_US_TXBUF;	
		USART1->DR = (idx & (uint16_t)0x01FF);
	}
}

void vUart1RxTxISR(void)
{
	uint16_t iIntStat = USART1->SR;
	
	if(iIntStat & UART_SR_TC)
	{//�����ж�
		if(iUSTxOut != iUSTxIn)
		{
			USART1->DR = (cUSTxFIFO[iUSTxOut] & (uint16_t)0x01FF);
			iUSTxOut++; iUSTxOut%=SIZE_US_TXBUF;
		}
		USART1->SR = iIntStat & (~UART_SR_TC);
	}
	
	if(iIntStat & UART_SR_RXNE)
	{//�����ж�
		cUSRxFIFO[iUSRxIn] = (USART1->DR & (uint16_t)0x01FF);
		iUSRxIn++; iUSRxIn %= SIZE_US_RXBUF;
		bIsDirectUart1RxRdy = HTRUE;

		//if(iUSRxOut == iUSRxIn) iUSRxOut = (SIZE_US_RXBUF + iUSRxIn - 1) % SIZE_US_RXBUF;
	}	
}
