#include "stdafx.h"
#include "math.h"
#include "process.h"
#include "stdio.h"
#include "StringBuf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CStringBuf::vAddString(CString str)
{
	m_strBuf[m_iHead++] = str; m_iHead %= SIZE_STRBUF;
	if(m_iLen<SIZE_STRBUF) m_iLen++;
	m_bUpdated = true;
}

CString CStringBuf::vGetLasest(int items)
{
	int idx, ifrom;
	CString ssum,stmp;

	if(items>=m_iLen) items = m_iLen;
	ifrom = m_iHead + SIZE_STRBUF - items;

	ssum = _T("\r\n");
	for(idx=0; idx<items-1; idx++)
	{
		stmp.Format(_T("%2d >> "),idx+1);
		ssum += stmp + m_strBuf[(ifrom+idx)%SIZE_STRBUF] + _T("\r\n");
	}
	if(items>=1)
	{
		stmp.Format(_T("%2d >> "),idx+1);
		ssum += stmp + m_strBuf[(ifrom+idx)%SIZE_STRBUF];
	}
	m_bUpdated = false;
	return ssum;
}

void CStringBuf::vClearAll(void)
{
	m_iHead = 0;
	m_iLen  = 0;
}

CStringBuf::CStringBuf(void)
{
	m_bUpdated = false;
	m_iHead = 0;
	m_iLen  = 0;
}

CStringBuf::~CStringBuf(void)
{

}
