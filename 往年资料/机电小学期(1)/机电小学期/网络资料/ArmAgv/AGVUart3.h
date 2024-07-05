#ifndef _AGVUART3_H_
#define _AGVUART3_H_
		
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

	//#define	SIZE_U1_RXBUF	128
	//#define	SIZE_U1_TXBUF	128

	//extern UINT16 iU1RxOut, iU1RxIn;
	//extern UINT16 iU1TxOut, iU1TxIn;

	//extern UINT16 cU1RxFIFO[SIZE_U1_RXBUF];
	//extern UINT16 cU1TxFIFO[SIZE_U1_TXBUF];	
/*	
	#define U3TX_PIN		GPIO_Pin_10
	#define U3TX_GPIO		GPIOB
	#define U3TX_CLK		RCC_APB2Periph_GPIOB

	#define SET_U3TX		U3TX_GPIO->BSRR = U3TX_PIN
	#define CLR_U3TX		U3TX_GPIO->BRR = U3TX_PIN
	#define XOR_U3TX		U3TX_GPIO->ODR ^= U3TX_PIN
*/
	extern HBOOL bIsWifiUart3RxRdy;
	extern void vInitUart3(UINT32 baudRate);
	extern void vUart3RxTxISR(void);
	
	extern void vUart3TxBuf(UINT8 *cbuf, UINT8 csize);
		
#endif
