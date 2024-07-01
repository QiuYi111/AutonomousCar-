
#pragma once
#include "afxwin.h"

#define SIZE_STRBUF		100

class CStringBuf
{
// 构造
public:
	CStringBuf( );	// 标准构造函数
	~CStringBuf( );

// 实现
private:
	int m_iHead;
	int m_iLen;
	CString m_strBuf[SIZE_STRBUF];

public:
	bool m_bUpdated;
	void vClearAll(void);
	void vAddString(CString str);
	CString vGetLasest(int items);
};
