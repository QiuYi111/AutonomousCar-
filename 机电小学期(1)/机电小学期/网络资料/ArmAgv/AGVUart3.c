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
	USART_InitTypeDef USART_InitSturct;//定义串口1的初始化结构体
	GPIO_InitTypeDef GPIO_InitStruct;//定义串口对应管脚的结构体
    NVIC_InitTypeDef NVIC_InitStructure;	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//打开串口管脚时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);//时钟使能	

//USART3_Tx_Pin Configure 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;//输出引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;//设置最高速度50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//推挽复用输出
	GPIO_Init(GPIOB , &GPIO_InitStruct);//将初始化好的结构体装入寄存器

//USART3_Rx_Pin Configure
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;//输入引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;//设置最高速度50MHz
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO模式悬浮输入
	GPIO_Init(GPIOB, &GPIO_InitStruct);//将初始化好的结构体装入寄存器

//USART3 Configure	
	USART_InitSturct.USART_BaudRate = baudRate;//波特率
	USART_InitSturct.USART_WordLength = USART_WordLength_8b;//数据宽度8位
	USART_InitSturct.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitSturct.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitSturct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitSturct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//使能发送与接收
	USART_Init(USART3 , &USART_InitSturct);//将初始化好的结构体装入寄存器	

//USART3_INT Configure
	USART_ITConfig(USART3 , USART_IT_RXNE, ENABLE);//使能接收中断
	USART_ITConfig(USART3 , USART_IT_TC, ENABLE);
	USART_Cmd(USART3 , ENABLE);//打开串口
	USART_ClearFlag(USART3 , USART_FLAG_TC);//解决第一个数据发送失败的问题	
	
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
	{//发送中断
		USART3->SR = iIntStat & (~UART_SR_TC);
		if(iUSTxOut != iUSTxIn)
		{
			USART3->DR = cUSTxFIFO[iUSTxOut];
			iUSTxOut++; iUSTxOut %= SIZE_US_TXBUF;
		}
	}
	
	if(iIntStat & UART_SR_RXNE)
	{//接收中断
		cUSRxFIFO[iUSRxIn] = USART3->DR;
		iUSRxIn++; iUSRxIn %= SIZE_US_RXBUF;
		bIsWifiUart3RxRdy = HTRUE;
	}
}
