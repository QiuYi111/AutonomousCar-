#include "stdafx.h"
#include "math.h"
#include "process.h"
#include "stdio.h"
#include "UartVarSFm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSPort::CSPort(void)
{
	m_bOpen = false;
	m_sName = _T("COM2");
	m_lBaud = 115200;
	m_hHandle = INVALID_HANDLE_VALUE;

	m_iRxFmCnt = 0;
	m_iRxHead = 0; m_iRxTail = 0;
	m_cCmdFunc = 0XFF; m_cCmdFunc = 0XFF; //m_cXorByte = XORBYTE_UART;
}

CSPort::~CSPort( )
{
	vClose();
}

void CSPort::vSetXorByte(unsigned char cByte)
{
//	m_cXorByte = cByte;
}

CString CSPort::SGetName(void)
{
	return(m_sName);
}

unsigned long CSPort::lGetBaud(void)
{
	return m_lBaud;
}

void CSPort::vSetBasic(CString sName, unsigned long lBaud)
{
	vClose();
	m_sName = sName;
	m_lBaud = lBaud;
}

void CSPort::vSetName(CString sName)
{
	vClose();
	m_sName = sName;
}

void CSPort::vSetBaud(unsigned long lBaud)
{
	vClose();
	m_lBaud = lBaud;
}

void CSPort::vClose(void)
{
	if(m_hHandle != INVALID_HANDLE_VALUE)
		CloseHandle(m_hHandle);
	m_bOpen = false;
	m_hHandle = INVALID_HANDLE_VALUE;
}

bool CSPort::bOpen(void)
{
	DWORD	dwError;
	BOOL	bReadCommOk;

	if(m_bOpen) vClose();
	m_bOpen = false;

	//创建串口
	m_hHandle = CreateFile(m_sName,
					  GENERIC_READ|GENERIC_WRITE,
					  0,
					  NULL,
					  OPEN_EXISTING,
					  0,//FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
					  NULL);
	if (m_hHandle == INVALID_HANDLE_VALUE) 
	{
		dwError = GetLastError();
		return FALSE;
	}
	bReadCommOk = SetupComm(m_hHandle,512,512);//串口输入、输出缓冲区大小都是128字节

	DCB *pDCB = new DCB;
	bReadCommOk = GetCommState(m_hHandle,pDCB);//设置串口,波特率9600，数据位8，无奇偶校验，一位停止位
	pDCB->BaudRate = m_lBaud;
	pDCB->ByteSize = 8;
	pDCB->Parity = NOPARITY;
	pDCB->StopBits = ONESTOPBIT;
	bReadCommOk = SetCommState(m_hHandle,pDCB);
	delete(pDCB);

	LPCOMMTIMEOUTS lpct = new COMMTIMEOUTS;
	bReadCommOk = GetCommTimeouts(m_hHandle,lpct);	//设置超时
	lpct->ReadIntervalTimeout = MAXDWORD;
	lpct->ReadTotalTimeoutConstant = 0;
	lpct->ReadTotalTimeoutMultiplier = 0;
	lpct->WriteTotalTimeoutConstant = 0;
	lpct->WriteTotalTimeoutMultiplier = 0;
	bReadCommOk = SetCommTimeouts(m_hHandle,lpct);
    if(ClearCommError(m_hHandle, &dwError, NULL) == 0)
    {
        CloseHandle(m_hHandle);
        m_hHandle = INVALID_HANDLE_VALUE;
    }
	PurgeComm(m_hHandle,PURGE_RXCLEAR);			//清空串口缓冲区
	PurgeComm(m_hHandle,PURGE_TXCLEAR);
	delete lpct;
	
	m_bOpen = true;
	return TRUE;
}

void CSPort::vRxPushData(void)
{
	BOOL     bReadCommOk;//设置一个布尔变量
	DWORD    idx, dwRead, lpErrors;//lpErrors为读取串口时错误返回值
    COMSTAT  lpStat;//串口状态,可以得到缓冲区有多少字节

	unsigned char cCharBuf[4096];

	if(m_bOpen)
	{
		dwRead = 0;
		bReadCommOk = ClearCommError(m_hHandle,&lpErrors,&lpStat);
		if(bReadCommOk) bReadCommOk = ReadFile(m_hHandle,(LPVOID )cCharBuf,lpStat.cbInQue,&dwRead,NULL);
		if(bReadCommOk)
		{
			for(idx=0; idx<dwRead; idx++)
			{
				cRxFIFO[m_iRxHead] = cCharBuf[idx];//^m_cXorByte;
				m_iRxHead++; m_iRxHead %= SIZE_UARTRXBUF;
			}
		}
	}
}

void CSPort::vSimRxPushData(unsigned char *cRxBuf, unsigned int iLen)
{
	unsigned int idx;
	for(idx=0; idx<iLen; idx++)
	{
		cRxFIFO[m_iRxHead] = cRxBuf[idx];
		m_iRxHead++; m_iRxHead %= SIZE_UARTRXBUF;
	}
}

unsigned char cSioTxBuf[512];
void CSPort::vTxVarFm(unsigned char *ctxfm, CString &stmp)
{
	DWORD dwWrite;
	unsigned char idx, csum, cfmwsize;

	CString s, sall=_T("");

	cSioTxBuf[0] = 0XEB;
	cSioTxBuf[1] = 0X90;
	
	csum = 0X00;
	cfmwsize  = ctxfm[2];
	m_cCmdFunc  = ctxfm[3];
	
	for(idx=0; idx<cfmwsize-1; idx++)
	{
		cSioTxBuf[idx] = ctxfm[idx];
		csum += ctxfm[idx];
	}
	cSioTxBuf[cfmwsize-1] = -csum;	

	for (idx=0; idx<cfmwsize; idx++)
	{
		s.Format(_T("%02X "),cSioTxBuf[idx]); sall += s;
		cSioTxBuf[idx] = cSioTxBuf[idx];// ^ m_cXorByte;
	}
	stmp = sall;

	WriteFile(m_hHandle,(unsigned char *)cSioTxBuf, cfmwsize, &dwWrite, NULL);
}

bool CSPort::bRxVarFm(unsigned char *cSioRxFm, CString &stmp)
{
	int idx,dlen;
	unsigned char csum,ctmp,cfmwsize;
	bool bSearchFm, bFoundFm = false;
	CString s, sall=_T("");

	if(m_iRxHead>=m_iRxTail) dlen = m_iRxHead - m_iRxTail;
	else dlen = SIZE_UARTRXBUF + m_iRxHead - m_iRxTail;
	bSearchFm = (dlen>4);
	while( (bSearchFm) && (bFoundFm==false))
	{
		if((cRxFIFO[m_iRxTail]==0XEB) && (cRxFIFO[(m_iRxTail+1)%SIZE_UARTRXBUF]==0X90))
		{
			cSioRxFm[0] = cRxFIFO[(m_iRxTail+0)%SIZE_UARTRXBUF];
			cSioRxFm[1] = cRxFIFO[(m_iRxTail+1)%SIZE_UARTRXBUF];
			cfmwsize    = cRxFIFO[(m_iRxTail+2)%SIZE_UARTRXBUF];
			m_cResFunc  = cRxFIFO[(m_iRxTail+3)%SIZE_UARTRXBUF];
			if(dlen>=cfmwsize)
			{
				csum = 0X00;
				for(idx=0; idx<cfmwsize; idx++)
				{
					ctmp = cRxFIFO[(m_iRxTail+idx)%SIZE_UARTRXBUF];
					csum += ctmp;  cSioRxFm[idx] = ctmp;
					s.Format(_T("%02X "),ctmp); sall += s;
					//if(idx%32==31) sall += _T("\r\n");
				}
				if(csum==0X00)
				{
					stmp = sall;
					m_iRxFmCnt++;
					m_iRxTail += cfmwsize; m_iRxTail %= SIZE_UARTRXBUF;
					bFoundFm = true;    bSearchFm = false;//  m_bDevLinked = true;
				}
				else
				{
					stmp = sall;
					m_iRxTail+=2;  m_iRxTail %= SIZE_UARTRXBUF;
				}
			}
			else  bSearchFm = false;
		}
		else
		{
			m_iRxTail++;  m_iRxTail %= SIZE_UARTRXBUF;
		}
		if(bSearchFm)
		{
			if(m_iRxHead>=m_iRxTail) dlen = m_iRxHead- m_iRxTail;
			else dlen = SIZE_UARTRXBUF + m_iRxHead - m_iRxTail;
			bSearchFm = (dlen>4);
		}
	}
	return bFoundFm;
}
