#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "STM32Type.h"
#include "AGVUart1.h"
#include "AGVUart3.h"
#include "misc.h"
#include "stm32f10x_tim.h"

HBOOL bIsWifiUart3RxRdy = HFALSE;

//UART3 TxD=GPIOB10   RxD=GPIOB11
void vInitUart3(uint32_t baudRate)
{
	USART_InitTypeDef USART_InitSturct;//���崮��1�ĳ�ʼ���ṹ��
	GPIO_InitTypeDef GPIO_InitStruct;//���崮�ڶ�Ӧ�ܽŵĽṹ��
    NVIC_InitTypeDef NVIC_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//�򿪴��ڹܽ�ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);//ʱ��ʹ��	

//USART3_Tx_Pin Configure 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;//�������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;//��������ٶ�50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//���츴�����
	GPIO_Init(GPIOB , &GPIO_InitStruct);//����ʼ���õĽṹ��װ��Ĵ���

//USART3_Rx_Pin Configure
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;//��������ٶ�50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIOģʽ��������
	GPIO_Init(GPIOB, &GPIO_InitStruct);//����ʼ���õĽṹ��װ��Ĵ���

//USART3 Configure	
	USART_InitSturct.USART_BaudRate = baudRate;//������
	USART_InitSturct.USART_WordLength = USART_WordLength_8b;//���ݿ��8λ
	USART_InitSturct.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitSturct.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitSturct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ʹ�ܷ��������
	USART_Init(USART3 , &USART_InitSturct);//����ʼ���õĽṹ��װ��Ĵ���	

//USART3_INT Configure
	USART_ITConfig(USART3 , USART_IT_RXNE, ENABLE);//ʹ�ܽ����ж�
	USART_ITConfig(USART3 , USART_IT_TC, ENABLE);
	USART_Cmd(USART3 , ENABLE);//�򿪴���
	USART_ClearFlag(USART3 , USART_FLAG_TC);//�����һ�����ݷ���ʧ�ܵ�����	
	
//configure interrupt vector
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	iUSTxOut = iUSTxIn;
}

void vUart3TxBuf(UINT8 *cbuf, UINT8 csize)
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
		idx = cUSTxFIFO[iUSTxOut];
		iUSTxOut++; iUSTxOut%=SIZE_US_TXBUF;	
		USART3->DR = idx;
	}
}

void vUart3RxTxISR(void)
{
	UINT16 iIntStat = USART3->SR;
	
	if(iIntStat & UART_SR_TC)
	{//�����ж�
		USART3->SR = iIntStat & (~UART_SR_TC);
		if(iUSTxOut != iUSTxIn)
		{
			USART3->DR = cUSTxFIFO[iUSTxOut];
			iUSTxOut++; iUSTxOut %= SIZE_US_TXBUF;
		}
	}
	
	if(iIntStat & UART_SR_RXNE)
	{//�����ж�
		cUSRxFIFO[iUSRxIn] = USART3->DR;
		iUSRxIn++; iUSRxIn %= SIZE_US_RXBUF;
		bIsWifiUart3RxRdy = HTRUE;
	}
}
