
#pragma once
#include "afxwin.h"

#include "VcType.h"

#ifndef _VARNET_H_
#define _VARNET_H_

	#define XORBYTE_NET		0X00
	#define SIZE_NETRXBUF	512
	#define SIZE_NETTXBUF	512

	class CNPort
	{
		// 构造
		public:
			CNPort( );	// 标准构造函数
			~CNPort( );

		// 实现
		protected:
			SOCKET socketClient;
			CString m_sAddress;
			UINT16 m_iPort;
			unsigned int  m_iRxHead, m_iRxTail;
			unsigned char cRxFIFO[SIZE_NETRXBUF];

		public:
			bool m_bOpen;
			UINT16 m_iRxFmCnt;
			//unsigned char m_cXorByte;
			unsigned char m_cCmdFunc, m_cResFunc;
			CString sGetAddress(void);
			UINT16 iGetPort(void);
			//void vSetBasic(CString sAddress, UINT16 iPort);
			//void vSetXorByte(unsigned char);
			bool bOpen(const char *p, int iport);
			void vClose(void);
			void vRxPushData(void);
			void vSimRxPushData(unsigned char *cRxBuf, unsigned int iLen);
			void vTxVarFm(unsigned char *cSioTxFm, CString &stmp);
			bool bRxVarFm(unsigned char *cSioRxFm, CString &stmp);
	};

#endif
