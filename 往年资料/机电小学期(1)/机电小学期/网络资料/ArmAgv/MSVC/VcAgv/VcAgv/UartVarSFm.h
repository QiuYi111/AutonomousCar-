
#pragma once
#include "afxwin.h"
#include "VcType.h"

#ifndef _UARTVSFM_H_
#define _UARTVSFM_H_

	#define XORBYTE_UART		0X00
	#define SIZE_UARTRXBUF		512
	#define SIZE_UARTTXBUF		512

	class CSPort
	{
		// 构造
		public:
			CSPort( );	// 标准构造函数
			~CSPort( );

		// 实现
		protected:
			CString m_sName;
			unsigned long m_lBaud;
			HANDLE m_hHandle;
			unsigned int  m_iRxHead, m_iRxTail;
			unsigned char cRxFIFO[SIZE_UARTRXBUF];

		public:
			bool m_bOpen;
			UINT16 m_iRxFmCnt;
			//unsigned char m_cXorByte;
			unsigned char m_cCmdFunc, m_cResFunc;
			CString SGetName(void);
			unsigned long lGetBaud(void);
			void vSetName(CString sName);
			void vSetBaud(unsigned long lBaud);
			void vSetBasic(CString sName, unsigned long lBaud);
			void vSetXorByte(unsigned char);
			bool bOpen(void);
			void vClose(void);
			void vRxPushData(void);
			void vSimRxPushData(unsigned char *cRxBuf, unsigned int iLen);
			void vTxVarFm(unsigned char *cSioTxFm, CString &stmp);
			bool bRxVarFm(unsigned char *cSioRxFm, CString &stmp);
	};

#endif
