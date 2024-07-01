#ifndef _AGVCOMM_H_
#define _AGVCOMM_H_
	
	#include "STM32Type.h"
	#include "AGVCP180112.h"

	#define ST_WIFI_NULL		0
	#define	ST_WIFI_RST1		1
	#define	ST_WIFI_CWMOD		2
	#define	ST_WIFI_RST2		3
	#define	ST_WIFI_CWSAP		4
	#define	ST_WIFI_CIPMUX		5
	#define	ST_WIFI_CIPSEVSER	6
	#define	ST_WIFI_CIPSTO		7
	#define	ST_WIFI_CIFSR		8
	#define	ST_WIFI_INITEND		9
	#define	ST_WIFI_RTXDATA		10

	#define SIZE_FMBUFLEN		128

	extern UINT8 iRTxFmBuf[SIZE_FMBUFLEN];
	extern UINT8 cResFmID, cCmdFmID;
	extern UINT8 cWifiState;
	extern STRN_DEV_KVAR strDevKVar;

	extern HBOOL bGen_GetRxVarFm(void);
	extern void  vGen_ExpRxVarFm(void);
	extern void  vGen_EdtTxVarFm(void);
	
	extern void  vDirectUart1_TxVarFm(void);
	
	extern void  vWifiUart3_InitModule(void);
	extern void  vWifiUart3_TxInitCmd(UINT8 *cCmdBuf, HBOOL bReturn);
	extern void  vWifiUart3_TxVarFm(void);	

#endif
