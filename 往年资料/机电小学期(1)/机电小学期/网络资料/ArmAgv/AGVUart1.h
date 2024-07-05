#ifndef _AGVUART1_H_
#define _AGVUART1_H_
		
	#include "misc.h"
	#include "STM32Type.h"
		
	#define UART_SR_TXE		0X0080
	#define UART_SR_TC		0X0040
	#define UART_SR_RXNE	0X0020
	#define UART_SR_IDEL	0X0010

	#define UART_CR1_UE		0X2000
	#define UART_CR1_M		0X1000
	#define UART_CR1_WAKE	0X0800
	#define UART_CR1_PCE	0X0400
	#define UART_CR1_PS		0X0200
	#define UART_CR1_PEIE	0X0100
	#define UART_CR1_TXEIE	0X0080
	#define UART_CR1_TCIE	0X0040
	#define UART_CR1_RXNEIE	0X0020
	#define UART_CR1_IDELIE	0X0010
	#define UART_CR1_TE		0X0008
	#define UART_CR1_RE		0X0004
	#define UART_CR1_RWU	0X0002
	#define UART_CR1_SBK	0X0001

	#define	SIZE_US_RXBUF	128
	#define	SIZE_US_TXBUF	256

	extern UINT16 iUSRxOut, iUSRxIn;
	extern UINT16 iUSTxOut, iUSTxIn;

	extern UINT8 cUSRxFIFO[SIZE_US_RXBUF];
	extern UINT8 cUSTxFIFO[SIZE_US_TXBUF];			

	extern HBOOL bIsDirectUart1RxRdy;
	
	void vInitUart1(UINT32 baudRate);
	void vUart1RxTxISR(void);
	
	void vUart1TxBuf(INT8 *cbuf, INT8 csize);
		
#endif
