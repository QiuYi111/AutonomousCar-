
// VcAgv.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVcAgvApp:
// �йش����ʵ�֣������ VcAgv.cpp
//

class CVcAgvApp : public CWinApp
{
public:
	CVcAgvApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVcAgvApp theApp;