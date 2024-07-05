#include "stdafx.h"
#include "math.h"
#include "process.h"
#include "stdio.h"
#include "NetVarSFm.h"
#include <winsock2.h>
#include <mswsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNPort::CNPort(void)
{
	m_bOpen = false;
	m_sAddress = "192.168.4.1";
	m_iPort = 20179;
	m_iRxFmCnt = 0;

	socketClient = NULL;
	m_iRxHead = 0; m_iRxTail = 0;
	m_cCmdFunc = 0XFF; m_cCmdFunc = 0XFF; //m_cXorByte = XORBYTE_NET;
}

CNPort::~CNPort( )
{
	vClose();
}

CString CNPort::sGetAddress(void)
{		
	return(m_sAddress);
}

UINT16 CNPort::iGetPort(void)
{		
	return(m_iPort);
}

void CNPort::vClose(void)
{		
	WSACleanup();
	if(socketClient != NULL)
		closesocket(socketClient);//关闭连接
	m_bOpen = false;
	socketClient = NULL;
}

bool CNPort::bOpen(const char *p,int iport)
{
	if(m_bOpen) vClose();
	m_bOpen = false;
	m_iPort = iport;
	m_sAddress = *p;

	WORD wVersionRequested;
	WSADATA wsaData;//WSAata用来存储系统传回的关于WinSocket的资料。
	int err;
    
	wVersionRequested = MAKEWORD( 1, 1 );
    
	err = WSAStartup( wVersionRequested, &wsaData );

	if ( (err==0) && ((LOBYTE( wsaData.wVersion ) != 1) || (HIBYTE( wsaData.wVersion ) != 1 ) ) ) 
	{
		WSACleanup( );
		err = 1;
	}
	if(err==0)
	{
		socketClient = socket(AF_INET,SOCK_STREAM,0);// AF_INET, 建立遵从TCP/IP协议的socket，第二个参数type应为SOCK_STREAM; 如为UDP，应为SOCK_DGRAM
		//初始化连接与端口号
		SOCKADDR_IN addrSrv;
		addrSrv.sin_addr.S_un.S_addr=inet_addr(p);//"192.168.4.1");//服务器地址
		addrSrv.sin_family=AF_INET;
		addrSrv.sin_port = htons(iport);// 设置服务器端口号
		err=connect(socketClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));//连接服务器
	}
	if(err) m_bOpen = false;
	else m_bOpen = true;
	return m_bOpen;
}

void CNPort::vRxPushData(void)
{
	int   idx, dwRead;
	char  cCharBuf[2048];

	if(m_bOpen)
	{
		dwRead=recv(socketClient,cCharBuf,2048,0);//接收数据
		if(dwRead!=SOCKET_ERROR)
		{
			for(idx=0; idx<dwRead; idx++)
			{
				cRxFIFO[m_iRxHead] = cCharBuf[idx];// ^ m_cXorByte;
				m_iRxHead++; m_iRxHead %= SIZE_NETRXBUF;
			}
		}
	}
}

void CNPort::vSimRxPushData(unsigned char *cRxBuf, unsigned int iLen)
{
	unsigned int idx;
	for(idx=0; idx<iLen; idx++)
	{
		cRxFIFO[m_iRxHead] = cRxBuf[idx];
		m_iRxHead++; m_iRxHead %= SIZE_NETRXBUF;
	}
}


void CNPort::vTxVarFm(unsigned char *ctxfm, CString &stmp)
{
	unsigned char idx, csum, cfmwsize;
	unsigned char cSioTxBuf[256];

	CString s, sall=_T("");

	cSioTxBuf[0] = 0XEB;
	cSioTxBuf[1] = 0X90;
	
	csum = 0X00;
	cfmwsize  = ctxfm[2];
	m_cCmdFunc  = ctxfm[3];
	
	for (idx = 0; idx <cfmwsize-1; idx++)
	{
		cSioTxBuf[idx] = ctxfm[idx];
		csum += ctxfm[idx];
	}
	cSioTxBuf[cfmwsize-1] = -csum;	

	for (idx = 0; idx <cfmwsize; idx++)
	{
		s.Format(_T("%02X "),cSioTxBuf[idx]); sall += s;
		cSioTxBuf[idx] = cSioTxBuf[idx];// ^ m_cXorByte;
	}
	stmp = sall;

	send(socketClient, (const char *)cSioTxBuf, cfmwsize, 0);//发送到服务器端
}

bool CNPort::bRxVarFm(unsigned char *cSioRxFm, CString &stmp)
{
	int idx,dlen;
	unsigned char csum,ctmp,cfmwsize;
	bool bSearchFm, bFoundFm = false;
	CString s, sall=_T("");

	if(m_iRxHead>=m_iRxTail) dlen = m_iRxHead - m_iRxTail;
	else dlen = SIZE_NETRXBUF + m_iRxHead - m_iRxTail;
	bSearchFm = (dlen>4);
	while( (bSearchFm) && (bFoundFm==false))
	{
		if((cRxFIFO[m_iRxTail]==0XEB) && (cRxFIFO[(m_iRxTail+1)%SIZE_NETRXBUF]==0X90))
		{
			cSioRxFm[0] = cRxFIFO[(m_iRxTail+0)%SIZE_NETRXBUF];
			cSioRxFm[1] = cRxFIFO[(m_iRxTail+1)%SIZE_NETRXBUF];
			cfmwsize    = cRxFIFO[(m_iRxTail+2)%SIZE_NETRXBUF];
			m_cResFunc  = cRxFIFO[(m_iRxTail+3)%SIZE_NETRXBUF];
			if(dlen>=cfmwsize)
			{
				csum = 0X00;
				for(idx=0; idx<cfmwsize; idx++)
				{
					ctmp = cRxFIFO[(m_iRxTail+idx)%SIZE_NETRXBUF];
					csum += ctmp;  cSioRxFm[idx] = ctmp;
					s.Format(_T("%02X "),ctmp); sall += s;
				}
				if(csum==0X00)
				{
					stmp = sall;
					m_iRxTail += cfmwsize; m_iRxTail %= SIZE_NETRXBUF;
					m_iRxFmCnt++;
					bFoundFm = true;    bSearchFm = false;
				}
				else
				{
					m_iRxTail+=2;  m_iRxTail %= SIZE_NETRXBUF;
				}
			}
			else  bSearchFm = false;
		}
		else
		{
			m_iRxTail++;  m_iRxTail %= SIZE_NETRXBUF;
		}
		if(bSearchFm)
		{
			if(m_iRxHead>=m_iRxTail) dlen = m_iRxHead- m_iRxTail;
			else dlen = SIZE_NETRXBUF + m_iRxHead - m_iRxTail;
			bSearchFm = (dlen>4);
		}
	}
	return bFoundFm;
}
