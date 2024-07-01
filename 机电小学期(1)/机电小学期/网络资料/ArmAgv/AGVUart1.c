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
	USART_InitTypeDef USART_InitSturct;//定义串口1的初始化结构体
	GPIO_InitTypeDef GPIO_InitStruct;//定义串口对应管脚的结构体
    NVIC_InitTypeDef NVIC_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//打开串口管脚时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);//时钟使能	

//USART1_Tx_Pin Configure 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//输出引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//设置最高速度50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//推挽复用输出
	GPIO_Init(GPIOA , &GPIO_InitStruct);//将初始化好的结构体装入寄存器

//USART1_Rx_Pin Configure
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;//输入引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//设置最高速度50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO模式悬浮输入
	GPIO_Init(GPIOA, &GPIO_InitStruct);//将初始化好的结构体装入寄存器

//USART1 Configure	
	USART_InitSturct.USART_BaudRate = baudRate;//波特率
	USART_InitSturct.USART_WordLength = USART_WordLength_8b;//数据宽度8位
	USART_InitSturct.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitSturct.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitSturct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能发送与接收
	USART_Init(USART1 , &USART_InitSturct);//将初始化好的结构体装入寄存器	

//USART1_INT Configure
	USART_ITConfig(USART1 , USART_IT_RXNE, ENABLE);//使能接收中断
	USART_ITConfig(USART1 , USART_IT_TC, ENABLE);
	USART_Cmd(USART1 , ENABLE);//打开串口
	USART_ClearFlag(USART1 , USART_FLAG_TC);//解决第一个数据发送失败的问题	
	
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
	{//发送中断
		if(iUSTxOut != iUSTxIn)
		{
			USART1->DR = (cUSTxFIFO[iUSTxOut] & (uint16_t)0x01FF);
			iUSTxOut++; iUSTxOut%=SIZE_US_TXBUF;
		}
		USART1->SR = iIntStat & (~UART_SR_TC);
	}
	
	if(iIntStat & UART_SR_RXNE)
	{//接收中断
		cUSRxFIFO[iUSRxIn] = (USART1->DR & (uint16_t)0x01FF);
		iUSRxIn++; iUSRxIn %= SIZE_US_RXBUF;
		bIsDirectUart1RxRdy = HTRUE;

		//if(iUSRxOut == iUSRxIn) iUSRxOut = (SIZE_US_RXBUF + iUSRxIn - 1) % SIZE_US_RXBUF;
	}	
}
