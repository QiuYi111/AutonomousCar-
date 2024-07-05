// VcAgvDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VcAgv.h"
#include "VcAgvDlg.h"
#include "afxdialogex.h"

#include "StringBuf.h"
#include "GenFun.h"
#include "VcType.h"
#include "DrawCurve.h"
#include "UartVarSFm.h"
#include "NetVarSFm.h"

#include "..\\..\\..\\AGVCP180112.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//定时相关
#define ITIME_DISPLAYTIMER_MS      50//普通定时器时间(整形):ms
UINT	iGenTimerId = 0;
UINT	wMultiTimerAccuracy = 1; //the accuracy of the MultiTimer
//操作提示显示
CStringBuf cSysInfo;

bool bDispTxHex = false;
bool bDispRxHex = false;
int  iOpBoardType = DEVTYPE_NULL;
bool bCommByUart = true;

//串口相关
CSPort  CUartPort;
CNPort  CNetPort;

#define BAUD_FIXRATE	115200

//网络相关
STRN_CMDFM_TSTALT		strCmdFmTstAlt;
STRN_CMDFM_GETRESFM		strCmdFmGetResFm;
STRN_CMDFM_SIMDETECTOR	strCmdFmSimDetector;
STRN_CMDFM_DBGMOTORPWM	strCmdFmDbgMotorPwm;
STRN_CMDFM_DBGDEVSPEED	strCmdFmDbgeDevSpeed;
STRN_CMDFM_SETRLSCOFF   strCmdFmSetDRLSCoff;
STRN_CMDFM_SETXJPARA	strCmdFmSetXJPara;
STRN_CMDFM_XJGO     	strCmdFmXJGo;

bool bCalendaThreadRun = false;
HANDLE hCalendaThread = NULL;
DWORD iCalendaThreadID;
extern void CALLBACK vCalendaThreadProc(CWnd* pWnd);

bool bCommThreadRun = false;
HANDLE hCommThread = NULL;
DWORD iCommThreadID;
extern void CALLBACK vCommThreadProc(void);

STRN_DEV_KVAR strDevKVar;
bool bDevKVarUpdated = true;

STRN_DEV_CFG strDevCfg;
bool bDevCfgUpdated = true;

const UINT16 iBitsTag[16]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,0x1000,0x2000,0x4000,0x8000};
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVcAgvDlg 对话框
CVcAgvDlg::CVcAgvDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVcAgvDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVcAgvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_OPINFO, m_EditSysInfo);
	DDX_Control(pDX, IDC_CHECK_BUILDLINK, m_ChkLinkWifi);
	DDX_Control(pDX, IDC_COMBO_UARTPORT, m_CComUartPort);
	DDX_Control(pDX, IDC_CHECK_WIFIMODE, m_ChkWifiMode);
	DDX_Control(pDX, IDC_CHECK_DISPTXHEX, m_ChkDispTxHex);
	DDX_Control(pDX, IDC_CHECK_DISPRXHEX, m_ChkDispRxHex);
	DDX_Control(pDX, IDC_STATIC_RXFLAG, m_UartRxFmFlag);
	DDX_Control(pDX, IDC_CHECK_RUNMODE, m_ChkRunMode);
	DDX_Control(pDX, IDC_CHECK_EXTENDUI, m_ChkExtendUI);

	DDX_Control(pDX, IDC_CHECK_L2XJ, m_ChkL2XJ);
	DDX_Control(pDX, IDC_CHECK_L1XJ, m_ChkL1XJ);
	DDX_Control(pDX, IDC_CHECK_R1XJ, m_ChkR1XJ);
	DDX_Control(pDX, IDC_CHECK_R2XJ, m_ChkR2XJ);
	DDX_Control(pDX, IDC_CHECK_SIMXJDETECTOR, m_ChkSimXJDector);

	DDX_Control(pDX, IDC_STATIC_DEBUGMOTORFRAME, m_FrameDbgMotorPwm);
	DDX_Control(pDX, IDC_EDIT_LFM_PWM, m_Edit_LFM_Pwm);
	DDX_Control(pDX, IDC_EDIT_LBM_PWM, m_Edit_LBM_Pwm);
	DDX_Control(pDX, IDC_EDIT_RFM_PWM, m_Edit_RFM_Pwm);
	DDX_Control(pDX, IDC_EDIT_RBM_PWM, m_Edit_RBM_Pwm);
	DDX_Control(pDX, IDC_BUTTON_DBGMOTORPWM, m_BtnSetMotorPwm);
	DDX_Control(pDX, IDC_BUTTON_DBGMOTORSTOP, m_BtnStopMotors);

	DDX_Control(pDX, IDC_EDIT_DBGDEVTURNSPEED, m_EditDevTurnSpeed);
	DDX_Control(pDX, IDC_EDIT_DBGDEVGOSPEED, m_EditDevGoSpeed);
	DDX_Control(pDX, IDC_BUTTON_DBGDEVSPEED, m_BtnSetDevSpeed);
	DDX_Control(pDX, IDC_STATIC_DEBUGDEVFRAME, m_FrameDbgDevSpeed);

	DDX_Control(pDX, IDC_STATIC_TRUNCOFFPIC, m_PicTurnCoff);
	DDX_Control(pDX, IDC_EDIT_LFMCOFF_TURNLEFT, m_EditLFMCoff_TurnLeft);
	DDX_Control(pDX, IDC_EDIT_LFMCOFF_TURNRIGHT, m_EditLFMCoff_TurnRight);
	DDX_Control(pDX, IDC_EDIT_LBMCOFF_TURNLEFT, m_EditLBMCoff_TurnLeft);
	DDX_Control(pDX, IDC_EDIT_LBMCOFF_TURNRIGHT, m_EditLBMCoff_TurnRight);
	DDX_Control(pDX, IDC_EDIT_RFMCOFF_TURNLEFT, m_EditRFMCoff_TurnLeft);
	DDX_Control(pDX, IDC_EDIT_RFMCOFF_TURNRIGHT, m_EditRFMCoff_TurnRight);
	DDX_Control(pDX, IDC_EDIT_RBMCOFF_TURNLEFT, m_EditRBMCoff_TurnLeft);
	DDX_Control(pDX, IDC_EDIT_RBMCOFF_TURNRIGHT, m_EditRBMCoff_TurnRight);
	DDX_Control(pDX, IDC_BUTTON_SETTURNCOFF, m_BtnSetTurnCoff);

	DDX_Control(pDX, IDC_STATIC_XJOPTIONFRAME, m_FrameXJOption);
	DDX_Control(pDX, IDC_COMBO_XJSENSORNUM, m_ComboXJSensorNum);
	DDX_Control(pDX, IDC_CHECK_XJSTAMPWIRE, m_ChkStampWire);
	DDX_Control(pDX, IDC_CHECK_XJGRIPWIRE, m_ChkGripWire);
	DDX_Control(pDX, IDC_EDIT_XJLSPEED, m_EditXJMaxLSpeed);
	DDX_Control(pDX, IDC_EDIT_XJRSPEED, m_EditXJMaxRSpeed);
	DDX_Control(pDX, IDC_BUTTON_XJSETPARA, m_BtnSetXJPara);
	DDX_Control(pDX, IDC_BUTTON_RUNXJCMD, m_BtnRunXJCmd);

	DDX_Control(pDX, IDOK, m_BtnExitSystem);
	DDX_Control(pDX, IDC_CHECK_GOTURNJOG, m_ChkGoTurnJog);
	DDX_Control(pDX, IDC_BUTTON_DGBTURNLEFT, m_BtnDbgTurnLeft);
	DDX_Control(pDX, IDC_BUTTON_DGBTURNRIGHT, m_BtnDbgTurnRight);
	DDX_Control(pDX, IDC_BUTTON_DGBGGOSTRAIGHT, m_BtnDbgGoStraight);
	DDX_Control(pDX, IDC_BUTTON_DGBGGOBACK, m_BtnDbgGoBack);
	DDX_Control(pDX, IDC_CHECK_L2BPEN, m_ChkL2BpEn);
	DDX_Control(pDX, IDC_CHECK_L1BPEN, m_ChkL1BpEn);
	DDX_Control(pDX, IDC_CHECK_R1BPEN, m_ChkR1BpEn);
	DDX_Control(pDX, IDC_CHECK_R2BPEN, m_ChkR2BpEn);
}

BEGIN_MESSAGE_MAP(CVcAgvDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_BUILDLINK, &CVcAgvDlg::OnBnClickedCheckTest)
	ON_CBN_SELCHANGE(IDC_COMBO_UARTPORT, &CVcAgvDlg::OnCbnSelchangeComboUartport)
	ON_BN_CLICKED(IDC_CHECK_DISPTXHEX, &CVcAgvDlg::OnBnClickedCheckDispuarttxhex)
	ON_BN_CLICKED(IDC_CHECK_DISPRXHEX, &CVcAgvDlg::OnBnClickedCheckDispuartrxhex)
	ON_BN_CLICKED(IDC_CHECK_WIFIMODE, &CVcAgvDlg::OnBnClickedCheckAltmode)
	ON_BN_CLICKED(IDC_BUTTON_DBGMOTORPWM, &CVcAgvDlg::OnBnClickedButtonSetMotorPwm)
	ON_BN_CLICKED(IDC_BUTTON_DBGMOTORSTOP, &CVcAgvDlg::OnBnClickedButtonSetmotorstop)
	ON_BN_CLICKED(IDC_BUTTON_DBGDEVSPEED, &CVcAgvDlg::OnBnClickedButtonSetdevspeed)
	ON_BN_CLICKED(IDC_BUTTON_XJSETPARA, &CVcAgvDlg::OnBnClickedButtonSetXJPara)
	ON_BN_CLICKED(IDC_CHECK_XJSTAMPWIRE, &CVcAgvDlg::OnBnClickedCheckStampwire)
	ON_BN_CLICKED(IDC_CHECK_XJGRIPWIRE, &CVcAgvDlg::OnBnClickedCheckGripwire)
	ON_BN_CLICKED(IDC_BUTTON_SETTURNCOFF, &CVcAgvDlg::OnBnClickedButtonSetTurnCoff)
	ON_BN_CLICKED(IDC_BUTTON_RUNXJCMD, &CVcAgvDlg::OnBnClickedButtonXJGo)	
	ON_BN_CLICKED(IDC_CHECK_RUNMODE, &CVcAgvDlg::OnBnClickedCheckRunMode)
	ON_BN_CLICKED(IDC_CHECK_L2XJ, &CVcAgvDlg::OnBnClickedCheckL2xj)
	ON_BN_CLICKED(IDC_CHECK_L1XJ, &CVcAgvDlg::OnBnClickedCheckL1xj)
	ON_BN_CLICKED(IDC_CHECK_R1XJ, &CVcAgvDlg::OnBnClickedCheckR1xj)
	ON_BN_CLICKED(IDC_CHECK_R2XJ, &CVcAgvDlg::OnBnClickedCheckR2xj)
	ON_BN_CLICKED(IDC_CHECK_SIMXJDETECTOR, &CVcAgvDlg::OnBnClickedCheckSimxjdetector)
	ON_BN_CLICKED(IDC_BUTTON_DGBGGOSTRAIGHT, &CVcAgvDlg::OnBnClickedButtonDgbGoStraight)
	ON_BN_CLICKED(IDC_BUTTON_DGBGGOBACK, &CVcAgvDlg::OnBnClickedButtonDgbgGoBack)
	ON_BN_CLICKED(IDC_BUTTON_DGBTURNLEFT, &CVcAgvDlg::OnBnClickedButtonDgbTurnLeft)
	ON_BN_CLICKED(IDC_BUTTON_DGBTURNRIGHT, &CVcAgvDlg::OnBnClickedButtonDgbTurnRight)
	ON_BN_CLICKED(IDC_CHECK_EXTENDUI, &CVcAgvDlg::OnBnClickedCheckExtendUI)
	ON_BN_CLICKED(IDOK, &CVcAgvDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_GOTURNJOG, &CVcAgvDlg::OnBnClickedCheckGoturnjog)
END_MESSAGE_MAP()

// CVcAgvDlg 消息处理程序

BOOL CVcAgvDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	vInitAll();
	m_ChkWifiMode.SetCheck(1);
	OnBnClickedCheckAltmode();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVcAgvDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

BOOL CVcAgvDlg::PreTranslateMessage(MSG* pMsg)
{//重载函数PreTranslateMessage
	switch(pMsg->message)
	{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
		m_BtnBuildLinkTooTip.RelayEvent(pMsg);
		m_BtnXunJiTooTip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);
} 

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVcAgvDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVcAgvDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CVcAgvDlg::OnBnClickedCheckTest()
{
	CString stmp;

	m_ChkLinkWifi.EnableWindow(false);
	if(bCommByUart)
	{
		if(CUartPort.m_bOpen==false)
		{
			if(CUartPort.bOpen())
			{
				stmp.Format(_T("打开串口 %s ... 成功！"),CUartPort.SGetName());
				cSysInfo.vAddString(stmp);
				stmp.Format(_T("搜索设备..."));
				cSysInfo.vAddString(stmp);
			}
			else
			{
				stmp.Format(_T("打开串口%s .... 失败！"),CUartPort.SGetName());
				cSysInfo.vAddString(stmp);
			}
			if(CUartPort.m_bOpen==true)
			{
				strCmdFmTstAlt.iTstMode = 1;
				CUartPort.vTxVarFm((unsigned char *)&strCmdFmTstAlt,stmp);  if(bDispTxHex) cSysInfo.vAddString(stmp);
				Sleep(100);

				strCmdFmGetResFm.cResID = RESFMID_DEVCFG;
				CUartPort.vTxVarFm((unsigned char *)&strCmdFmGetResFm,stmp);  if(bDispTxHex) cSysInfo.vAddString(stmp);
				Sleep(100);

				hCommThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)vCommThreadProc, NULL, 0, &iCommThreadID );
				bCommThreadRun = true;
				iGenTimerId = (UINT)SetTimer(1,ITIME_DISPLAYTIMER_MS,NULL);

				m_ChkLinkWifi.SetWindowText(_T("断开"));
				stmp.Format(_T("建立通讯连接")); cSysInfo.vAddString(stmp);
						
				m_CComUartPort.EnableWindow(false);
				m_ChkWifiMode.EnableWindow(false);
			}
		}
		else
		{
			strCmdFmTstAlt.iTstMode = 0;
			CUartPort.vTxVarFm((unsigned char *)&strCmdFmTstAlt,stmp);  if(bDispTxHex) cSysInfo.vAddString(stmp);

			m_ChkLinkWifi.SetWindowText(_T("连接" ));
			stmp.Format(_T("断开通讯连接")); cSysInfo.vAddString(stmp);

			bCommThreadRun = false;
			CloseHandle(hCommThread);
			if(iGenTimerId) KillTimer(iGenTimerId);
			iGenTimerId = 0;

			m_CComUartPort.EnableWindow(true);
			m_ChkWifiMode.EnableWindow(true);
			CUartPort.vClose();
		}
	}
	else
	{
		if(CNetPort.m_bOpen==false)
		{
			if(CNetPort.bOpen("192.168.4.1",PORT_TCPLINK))
			{
				stmp.Format(_T("连接服务器（192.168.4.1:%d) ... 成功！"),CNetPort.iGetPort());
				cSysInfo.vAddString(stmp);
				stmp.Format(_T("搜索设备..."));
				cSysInfo.vAddString(stmp);
			}
			else
			{				
				stmp.Format(_T("连接服务器（192.168.4.1:%d) ... 失败！"),CNetPort.iGetPort());
				cSysInfo.vAddString(stmp);
			}
			if(CNetPort.m_bOpen==true)
			{
				strCmdFmTstAlt.iTstMode = 1;
				CNetPort.vTxVarFm((unsigned char *)&strCmdFmTstAlt,stmp);  if(bDispTxHex) cSysInfo.vAddString(stmp);
				Sleep(100);

				strCmdFmGetResFm.cResID = RESFMID_DEVCFG;
				CNetPort.vTxVarFm((unsigned char *)&strCmdFmGetResFm,stmp);  if(bDispTxHex) cSysInfo.vAddString(stmp);
				Sleep(100);

				hCommThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)vCommThreadProc, NULL, 0, &iCommThreadID );
				bCommThreadRun = true;
				iGenTimerId = (UINT)SetTimer(1,ITIME_DISPLAYTIMER_MS,NULL);

				m_ChkLinkWifi.SetWindowText(_T("断开"));
				stmp.Format(_T("建立通讯连接")); cSysInfo.vAddString(stmp);
				m_ChkWifiMode.EnableWindow(false);
			}
		}
		else
		{
			strCmdFmTstAlt.iTstMode = 0;
			CNetPort.vTxVarFm((unsigned char *)&strCmdFmTstAlt,stmp);  if(bDispTxHex) cSysInfo.vAddString(stmp);
			Sleep(50);

			m_ChkLinkWifi.SetWindowText(_T("连接" ));
			stmp.Format(_T("断开通讯连接")); cSysInfo.vAddString(stmp);

			bCommThreadRun = false;
			CloseHandle(hCommThread);
			if(iGenTimerId) KillTimer(iGenTimerId);
			iGenTimerId = 0;

			Sleep(50);
			CNetPort.vClose();
			m_ChkWifiMode.EnableWindow(true);
		}
	}	
	vUpdateScreen();
	OnBnClickedCheckRunMode();
	m_ChkLinkWifi.EnableWindow(true);
}

void CVcAgvDlg::vInitAll(void)
{
	int idx;
	int items;
	CString stmp;

	m_EditSysInfo.SetWindowText(""); 
	cSysInfo.vClearAll();
	bCommByUart = true;  m_ChkWifiMode.SetWindowText("当前串口模式");

	m_ChkDispTxHex.SetCheck(1); bDispTxHex = true;
	m_ChkDispRxHex.SetCheck(0); bDispRxHex = false;
	
	stmp = _T("==================================="); cSysInfo.vAddString(stmp);
	stmp = _T("在使用本软件前请确认已通过 WIFI 连接了设备"); cSysInfo.vAddString(stmp);
	stmp = _T("WIFI名称：M&ELAB_XXXX(编号) 密码：12345678"); cSysInfo.vAddString(stmp);
	stmp = _T("==================================="); cSysInfo.vAddString(stmp);

	////////////////////////////////////////////////////////////////////////////
	items = m_CComUartPort.GetCount();
	for(idx=0; idx<items; idx++)  m_CComUartPort.DeleteString(0);
	for(idx=0; idx<24; idx++) { stmp.Format(_T("COM%d"),idx+1); m_CComUartPort.InsertString(idx,stmp); }
	m_CComUartPort.SetCurSel(2);
	CUartPort.vSetBasic(_T("COM2"),BAUD_FIXRATE);
	m_CComUartPort.SetWindowText(CUartPort.SGetName());
	vFillCmdFmBasic();
	////////////////////////////////////////////////////////////////////////////
	bCalendaThreadRun = true;
	hCalendaThread = CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)vCalendaThreadProc,
		(LPVOID)AfxGetApp()->GetMainWnd(),//得到父窗口句柄
		0,
		&iCalendaThreadID);
	////////////////////////////////////////////////////////////////////////////
	m_ChkGoTurnJog.SetCheck(1);

	m_Edit_LFM_Pwm.SetWindowTextA("0");
	m_Edit_LBM_Pwm.SetWindowTextA("0");
	m_Edit_RFM_Pwm.SetWindowTextA("0");
	m_Edit_RBM_Pwm.SetWindowTextA("0");

	m_EditDevGoSpeed.SetWindowTextA("+50");
	m_EditDevTurnSpeed.SetWindowTextA("-50");

	stmp.Format(_T("B) 电机速度调试(%d~%+d)"),-PWM_MAXTWC,+PWM_MAXTWC);
	m_FrameDbgMotorPwm.SetWindowText(stmp);
	stmp.Format(_T("D) 车体速度调试(%d~%+d)"),-PWM_MAXTWC,+PWM_MAXTWC);
	m_FrameDbgDevSpeed.SetWindowText(stmp);
	stmp.Format(_T("E) 循迹选项与速度控制(%-d~%+d)"),-PWM_MAXTWC,+PWM_MAXTWC);
	m_FrameXJOption.SetWindowText(stmp);
	////////////////////////////////////////////////////////////////////////////
	OnBnClickedCheckGripwire();
	items = m_ComboXJSensorNum.GetCount();
	for(idx=0; idx<items; idx++)  m_ComboXJSensorNum.DeleteString(0);
	for(idx=0; idx<3; idx++) { stmp.Format(_T("%1d 只红外"),idx+2); m_ComboXJSensorNum.InsertString(idx,stmp); }
	m_ComboXJSensorNum.SetCurSel(0);
	m_EditXJMaxLSpeed.SetWindowText("100");
	m_EditXJMaxRSpeed.SetWindowText("100");
	////////////////////////////////////////////////////////////////////////////
	m_EditLFMCoff_TurnLeft.SetWindowText("-0.6");
	m_EditLBMCoff_TurnLeft.SetWindowText("-0.6");
	m_EditRFMCoff_TurnLeft.SetWindowText("+0.6");
	m_EditRBMCoff_TurnLeft.SetWindowText("+1.0");

	m_EditLFMCoff_TurnRight.SetWindowText("+0.6");
	m_EditLBMCoff_TurnRight.SetWindowText("+1.0");
	m_EditRFMCoff_TurnRight.SetWindowText("-0.6");
	m_EditRBMCoff_TurnRight.SetWindowText("-0.6");
	////////////////////////////////////////////////////////////////////////////
	m_ChkRunMode.SetCheck(1); OnBnClickedCheckRunMode();
	bDevKVarUpdated = true;
	bDevCfgUpdated = true;
	////////////////////////////////////////////////////////////////////////////
	m_ChkExtendUI.SetCheck(0);  this->ShowWindow(true);  this->GetWindowRect(&m_rcMainWin); OnBnClickedCheckExtendUI();
	m_fontBtnStop.CreatePointFont(450,"黑体");  m_BtnStopMotors.SetFont(&m_fontBtnStop);// m_fontBtnStop.DeleteObject();
	m_fontLinkWifi.CreatePointFont(150,"黑体");  m_ChkLinkWifi.SetFont(&m_fontLinkWifi); m_BtnRunXJCmd.SetFont(&m_fontLinkWifi); m_BtnExitSystem.SetFont(&m_fontLinkWifi);// m_fontLinkWifi.DeleteObject();
	////////////////////////////////////////////////////////////////////////////
	 m_BtnBuildLinkTooTip.Create(this); 
	 m_BtnBuildLinkTooTip.AddTool( GetDlgItem(IDC_CHECK_BUILDLINK), "每次重启自主移动车后需确认WINDOWS系统连接网络 M&ELAB_XXXX？密码：12345678" );
	 m_BtnBuildLinkTooTip.SetDelayTime(500);
	 m_BtnBuildLinkTooTip.SetTipTextColor(RGB(255,0,0));//这儿在我测试的时候提示出错，要换成RGB（128,100,255）
	 m_BtnBuildLinkTooTip.SetTipBkColor(RGB(255,255,255));//RGB(255,255,255)//这两个颜色值就是文字颜色和背景的实际值
	 m_BtnBuildLinkTooTip.Activate(TRUE);

	 m_BtnXunJiTooTip.Create(this); 
	 m_BtnXunJiTooTip.AddTool( GetDlgItem(IDC_BUTTON_RUNXJCMD), "确认所有参数已设置，请准备随时停车以避免意外！" );//按钮一的提示
	 m_BtnXunJiTooTip.SetDelayTime(500);
	 m_BtnXunJiTooTip.SetTipTextColor(RGB(255,255,0));//这儿在我测试的时候提示出错，要换成RGB（128,100,255）
	 m_BtnXunJiTooTip.SetTipBkColor(RGB(0,0,0));//这两个颜色值就是文字颜色和背景的实际值
	 m_BtnXunJiTooTip.Activate(TRUE);

	vUpdateScreen();
}

void CALLBACK vCalendaThreadProc(CWnd* pWnd)
{
	CTime time;
	CString strTime;

	CStatic *pStaticTime = (CStatic *)pWnd->GetDlgItem(IDC_STATIC_CALENDA);
	while(bCalendaThreadRun)
	{
		time=CTime::GetCurrentTime();
		strTime=time.Format("%H:%M:%S");
		pStaticTime->SetWindowText(strTime);
		Sleep(1000);
	}
}

void CVcAgvDlg::vFillCmdFmBasic(void)
{
	CString stmp;
	
//	STRN_CMDFM_TSTALT		strCmdFmTstAlt;
	strCmdFmTstAlt.iFmHead = 0X90EB;
	strCmdFmTstAlt.cFmID = CMDFMID_TSTALT;
	strCmdFmTstAlt.cFmBLen = SIZE_CMDFM_TSTALT;

//	STRN_CMDFM_GETRESFM		strCmdFmGetResFm;
	strCmdFmGetResFm.iFmHead = 0X90EB;
	strCmdFmGetResFm.cFmID = CMDFMID_GETRESFM;
	strCmdFmGetResFm.cFmBLen = SIZE_CMDFM_GETRESFM;

//	STRN_CMDFM_DBGMOTORPWM		strCmdFmDbgMotorPwm;
	strCmdFmDbgMotorPwm.iFmHead = 0X90EB;
	strCmdFmDbgMotorPwm.cFmID = CMDFMID_DBGMOTORPWM;
	strCmdFmDbgMotorPwm.cFmBLen = SIZE_CMDFM_DBGMOTORPWM;

//	STRN_CMDFM_DBGDEVSPEED		strCmdFmDbgeDevSpeed;
	strCmdFmDbgeDevSpeed.iFmHead = 0X90EB;
	strCmdFmDbgeDevSpeed.cFmID = CMDFMID_DBGDEVSPEED;
	strCmdFmDbgeDevSpeed.cFmBLen = SIZE_CMDFM_DBGDEVSPEED;

//	STRN_CMDFM_SETRLSCOFF	strCmdFmSetDRLSCoff;
	strCmdFmSetDRLSCoff.iFmHead = 0X90EB;
	strCmdFmSetDRLSCoff.cFmID = CMDFMID_SETRLSCOFF;
	strCmdFmSetDRLSCoff.cFmBLen = SIZE_CMDFM_SETRLSCOFF;

//	STRN_CMDFM_SETXJPARA    strCmdFmSetXJPara;
	strCmdFmSetXJPara.iFmHead = 0X90EB;
	strCmdFmSetXJPara.cFmID = CMDFMID_SETXJPARA;
	strCmdFmSetXJPara.cFmBLen = SIZE_CMDFM_SETXJPARA;

//	STRN_CMDFM_XJGO    strCmdFmXJGo;
	strCmdFmXJGo.iFmHead = 0X90EB;
	strCmdFmXJGo.cFmID = CMDFMID_XJGO;
	strCmdFmXJGo.cFmBLen = SIZE_CMDFM_XJGO;

//STRN_CMDFM_SIMDETECTOR	strCmdFmSimDetector;
	strCmdFmSimDetector.iFmHead = 0X90EB;
	strCmdFmSimDetector.cFmID = CMDFMID_SIMDETECTOR;
	strCmdFmSimDetector.cFmBLen = SIZE_CMDFM_SIMDETECTOR;
}

void CALLBACK vCommThreadProc(void)
{
	CTime time;
	CString strTime, stmp;
	CString sall;

	unsigned char cRxFmBuf[256];
	STRN_FM_GENHEAD  *pResFmGenHead = (STRN_FM_GENHEAD *)&cRxFmBuf[0];
	STRN_RESFM_GENDEV *pGenDev = (STRN_RESFM_GENDEV *)cRxFmBuf;
	STRN_RESFM_DEVKVAR *pDevKVar = (STRN_RESFM_DEVKVAR *)cRxFmBuf;
	STRN_RESFM_DEVCFG *pDevCfg = (STRN_RESFM_DEVCFG *)cRxFmBuf;

	while(bCommThreadRun)
	{
		if(bCommByUart) CUartPort.vRxPushData();//接收串口数据放缓冲区
		else CNetPort.vRxPushData();//接收串口数据放缓冲区
		if( (CUartPort.bRxVarFm(&cRxFmBuf[0],stmp)) || (CNetPort.bRxVarFm(&cRxFmBuf[0],stmp)) )
		{//解帧正确，下面对响应帧解释
			if(bDispRxHex==true) cSysInfo.vAddString(stmp);

			switch(pResFmGenHead->cFmID)
			{
				case RESFMID_GENDEV:
					if(iOpBoardType != ((STRN_RESFM_GENDEV *)cRxFmBuf)->iDevType)
					{					
						stmp = _T("==================================="); cSysInfo.vAddString(stmp);
						stmp.Format(_T("||发现设备【类型: %04X 硬件版本：%04X，软件版本：%04X】"),pGenDev->iDevType, pGenDev->lHardVer, pGenDev->lSoftVer); cSysInfo.vAddString(stmp);
						stmp = _T("==================================="); cSysInfo.vAddString(stmp);
						iOpBoardType = ((STRN_RESFM_GENDEV *)cRxFmBuf)->iDevType;
					}
					break;

				case RESFMID_DEVCFG:
					vMemCpy((unsigned char *)&pDevCfg->CFG, (unsigned char *)&strDevCfg, sizeof(STRN_DEV_CFG));
					bDevCfgUpdated = true;
					break;

				case RESFMID_DEVKVAR:
					vMemCpy((unsigned char *)&pDevKVar->KVAR, (unsigned char *)&strDevKVar, sizeof(STRN_DEV_KVAR));
					bDevKVarUpdated = true;
					break;

				default:
					break;
			}
		}
		Sleep(5);
	}
}

void CVcAgvDlg::OnCbnSelchangeComboUartport()
{
	CString stmp;
	stmp.Format(_T("COM%d"),m_CComUartPort.GetCurSel()+1);
	CUartPort.vSetBasic(stmp,BAUD_FIXRATE);
}


void CVcAgvDlg::OnBnClickedCheckDispuarttxhex()
{
	if(m_ChkDispTxHex.GetCheck()==1) bDispTxHex = true;
	else bDispTxHex = false;
}

void CVcAgvDlg::OnBnClickedCheckDispuartrxhex()
{
	if(m_ChkDispRxHex.GetCheck()==1) bDispRxHex = true;
	else bDispRxHex = false;
}

void CVcAgvDlg::OnBnClickedCheckAltmode()
{
	if(m_ChkWifiMode.GetCheck()==1)
	{
		bCommByUart = false;
		m_ChkWifiMode.SetWindowText("当前网络模式");
		m_CComUartPort.EnableWindow(false);
	}
	else
	{
		bCommByUart = true;
		m_ChkWifiMode.SetWindowText("当前串口模式");
		m_CComUartPort.EnableWindow(true);
	}
}

void CVcAgvDlg::OnTimer(UINT_PTR nIDEvent)
{
	vUpdateScreen();
}

void CVcAgvDlg::OnBnClickedButtonSetMotorPwm()
{
	int itmp;
	unsigned char cbuf[32];
	CString stmp;

	m_BtnSetMotorPwm.EnableWindow(false);

	m_Edit_LFM_Pwm.GetWindowText((LPTSTR)cbuf,10);
	try { itmp = atoi((const char *)cbuf); }
	catch(...){ }
	if(itmp>+PWM_MAXTWC) itmp=+PWM_MAXTWC;
	if(itmp<-PWM_MAXTWC) itmp=-PWM_MAXTWC;
	strCmdFmDbgMotorPwm.PWM.cLFM_Pwm = itmp;

	m_Edit_LBM_Pwm.GetWindowText((LPTSTR)cbuf,10);
	try { itmp = atoi((const char *)cbuf); }
	catch(...){ }
	if(itmp>+PWM_MAXTWC) itmp=+PWM_MAXTWC;
	if(itmp<-PWM_MAXTWC) itmp=-PWM_MAXTWC;
	strCmdFmDbgMotorPwm.PWM.cLBM_Pwm = itmp;

	m_Edit_RFM_Pwm.GetWindowText((LPTSTR)cbuf,10);
	try { itmp = atoi((const char *)cbuf); }
	catch(...){ }
	if(itmp>+PWM_MAXTWC) itmp=+PWM_MAXTWC;
	if(itmp<-PWM_MAXTWC) itmp=-PWM_MAXTWC;
	strCmdFmDbgMotorPwm.PWM.cRFM_Pwm = itmp;

	m_Edit_RBM_Pwm.GetWindowText((LPTSTR)cbuf,10);
	try { itmp = atoi((const char *)cbuf); }
	catch(...){ }
	if(itmp>+PWM_MAXTWC) itmp=+PWM_MAXTWC;
	if(itmp<-PWM_MAXTWC) itmp=-PWM_MAXTWC;
	strCmdFmDbgMotorPwm.PWM.cRBM_Pwm = itmp;

	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgMotorPwm,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgMotorPwm,stmp);
	Sleep(500);
	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgMotorPwm,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgMotorPwm,stmp);

	if(bDispTxHex) cSysInfo.vAddString(stmp);
	stmp.Format(_T("调试4只电机速度(PWM)")); cSysInfo.vAddString(stmp);

	m_BtnSetMotorPwm.EnableWindow(true);
}

void CVcAgvDlg::OnBnClickedButtonSetmotorstop()
{
	CString stmp;
	
	m_BtnStopMotors.EnableWindow(false);

	m_EditDevTurnSpeed.EnableWindow(true);
	m_EditDevGoSpeed.EnableWindow(true);
	m_BtnSetDevSpeed.EnableWindow(true);

	m_Edit_LFM_Pwm.SetWindowTextA("0");
	m_Edit_LBM_Pwm.SetWindowTextA("0");
	m_Edit_RFM_Pwm.SetWindowTextA("0");
	m_Edit_RBM_Pwm.SetWindowTextA("0");

	strCmdFmDbgMotorPwm.PWM.cLFM_Pwm = 0;
	strCmdFmDbgMotorPwm.PWM.cLBM_Pwm = 0;
	strCmdFmDbgMotorPwm.PWM.cRFM_Pwm = 0;
	strCmdFmDbgMotorPwm.PWM.cRBM_Pwm = 0;

	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgMotorPwm,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgMotorPwm,stmp);
	Sleep(500);

	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgMotorPwm,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgMotorPwm,stmp);
	if(bDispTxHex) cSysInfo.vAddString(stmp);
	stmp.Format(_T("设置所有电机停止运行(PWM=0)")); cSysInfo.vAddString(stmp);

	m_BtnStopMotors.EnableWindow(true);
}

void CVcAgvDlg::OnBnClickedButtonSetdevspeed()
{
	int itmp;
	unsigned char cbuf[32];
	CString stmp;

	m_BtnSetDevSpeed.EnableWindow(false);	

	m_EditDevTurnSpeed.GetWindowText((LPTSTR)cbuf,10);
	try { itmp = atoi((const char *)cbuf); }
	catch(...){ }
	if(itmp>+PWM_MAXTWC) itmp=+PWM_MAXTWC;
	if(itmp<-PWM_MAXTWC) itmp=-PWM_MAXTWC;
	strCmdFmDbgeDevSpeed.cTurnSpeed = itmp;

	m_EditDevGoSpeed.GetWindowText((LPTSTR)cbuf,10);
	try { itmp = atoi((const char *)cbuf); }
	catch(...){ }
	if(itmp>+PWM_MAXTWC) itmp=+PWM_MAXTWC;
	if(itmp<-PWM_MAXTWC) itmp=-PWM_MAXTWC;
	strCmdFmDbgeDevSpeed.cGoSpeed = itmp;
	
	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	stmp.Format(_T("调试车辆转弯速度(%d)与直行速度(%d)"), (INT16)strCmdFmDbgeDevSpeed.cTurnSpeed, (INT16)strCmdFmDbgeDevSpeed.cGoSpeed); cSysInfo.vAddString(stmp);
	Sleep(100);
	if(m_ChkGoTurnJog.GetCheck()==1)
	{
		strCmdFmDbgeDevSpeed.cTurnSpeed = 0;
		strCmdFmDbgeDevSpeed.cGoSpeed = 0;
	}
	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	if(bDispTxHex) cSysInfo.vAddString(stmp);
	stmp.Format(_T("调试车辆转弯速度(%d)与直行速度(%d)"), (INT16)strCmdFmDbgeDevSpeed.cTurnSpeed, (INT16)strCmdFmDbgeDevSpeed.cGoSpeed); cSysInfo.vAddString(stmp);

	m_BtnSetDevSpeed.EnableWindow(true);
}

void CVcAgvDlg::OnBnClickedButtonSetXJPara()
{
	CString stmp;
	int itmp;
	unsigned char cbuf[32];

	m_BtnSetXJPara.EnableWindow(false);	

	if(m_ChkL2BpEn.GetCheck()==1) strCmdFmSetXJPara.XJPARA.bL2XJBeepEn = 1;
	else strCmdFmSetXJPara.XJPARA.bL2XJBeepEn = 0;

	if(m_ChkL1BpEn.GetCheck()==1) strCmdFmSetXJPara.XJPARA.bL1XJBeepEn = 1;
	else strCmdFmSetXJPara.XJPARA.bL1XJBeepEn = 0;

	if(m_ChkR1BpEn.GetCheck()==1) strCmdFmSetXJPara.XJPARA.bR1XJBeepEn = 1;
	else strCmdFmSetXJPara.XJPARA.bR1XJBeepEn = 0;

	if(m_ChkR2BpEn.GetCheck()==1) strCmdFmSetXJPara.XJPARA.bR2XJBeepEn = 1;
	else strCmdFmSetXJPara.XJPARA.bR2XJBeepEn = 0;

	strCmdFmSetXJPara.XJPARA.cDectorSel = m_ComboXJSensorNum.GetCurSel()+2;
	if(m_ChkStampWire.GetCheck()==1) strCmdFmSetXJPara.XJPARA.bIsStampWire = 1;
	else strCmdFmSetXJPara.XJPARA.bIsStampWire = 0;

	m_EditXJMaxLSpeed.GetWindowText((LPTSTR)cbuf,10);
	try { itmp = atoi((const char *)cbuf); }
	catch(...){ }
	if(itmp>+PWM_MAXTWC) itmp=+PWM_MAXTWC;
	if(itmp<-PWM_MAXTWC) itmp=-PWM_MAXTWC;
	strCmdFmSetXJPara.XJPARA.cMaxGoSpeed = itmp;

	m_EditXJMaxRSpeed.GetWindowText((LPTSTR)cbuf,10);
	try { itmp = atoi((const char *)cbuf); }
	catch(...){ }
	if(itmp>+PWM_MAXTWC) itmp=+PWM_MAXTWC;
	if(itmp<-PWM_MAXTWC) itmp=-PWM_MAXTWC;
	strCmdFmSetXJPara.XJPARA.cMaxTurnSpeed = itmp;

	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmSetXJPara,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmSetXJPara,stmp);
	Sleep(500);
	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmSetXJPara,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmSetXJPara,stmp);

	if(bDispTxHex) cSysInfo.vAddString(stmp);
	stmp.Format(_T("设置循迹参数")); cSysInfo.vAddString(stmp);

	m_BtnSetXJPara.EnableWindow(true);	
}

void CVcAgvDlg::OnBnClickedCheckStampwire()
{
	m_ChkStampWire.SetCheck(1);
	m_ChkGripWire.SetCheck(0);
}

void CVcAgvDlg::OnBnClickedCheckGripwire()
{
	m_ChkStampWire.SetCheck(0);
	m_ChkGripWire.SetCheck(1);
}

void CVcAgvDlg::OnBnClickedButtonSetTurnCoff()
{
	CString stmp;
	double ftmp;
	unsigned char cbuf[32];

	m_BtnSetTurnCoff.EnableWindow(false);	

	m_EditLFMCoff_TurnLeft.GetWindowText((LPTSTR)cbuf,10);
	try { ftmp = atof((const char *)cbuf); }
	catch(...){ }
	if(ftmp>+1.0) ftmp=+1.0;
	if(ftmp<-1.0) ftmp=-1.0;
	strCmdFmSetDRLSCoff.RLSCOFF.cLFM_TurnLeft = (signed char)(ftmp*100.0);

	m_EditLFMCoff_TurnRight.GetWindowText((LPTSTR)cbuf,10);
	try { ftmp = atof((const char *)cbuf); }
	catch(...){ }
	if(ftmp>+1.0) ftmp=+1.0;
	if(ftmp<-1.0) ftmp=-1.0;
	strCmdFmSetDRLSCoff.RLSCOFF.cLFM_TurnRight = (signed char)(ftmp*100.0);

	m_EditLBMCoff_TurnLeft.GetWindowText((LPTSTR)cbuf,10);
	try { ftmp = atof((const char *)cbuf); }
	catch(...){ }
	if(ftmp>+1.0) ftmp=+1.0;
	if(ftmp<-1.0) ftmp=-1.0;
	strCmdFmSetDRLSCoff.RLSCOFF.cLBM_TurnLeft = (signed char)(ftmp*100.0);

	m_EditLBMCoff_TurnRight.GetWindowText((LPTSTR)cbuf,10);
	try { ftmp = atof((const char *)cbuf); }
	catch(...){ }
	if(ftmp>+1.0) ftmp=+1.0;
	if(ftmp<-1.0) ftmp=-1.0;
	strCmdFmSetDRLSCoff.RLSCOFF.cLBM_TurnRight = (signed char)(ftmp*100.0);

	m_EditRFMCoff_TurnLeft.GetWindowText((LPTSTR)cbuf,10);
	try { ftmp = atof((const char *)cbuf); }
	catch(...){ }
	if(ftmp>+1.0) ftmp=+1.0;
	if(ftmp<-1.0) ftmp=-1.0;
	strCmdFmSetDRLSCoff.RLSCOFF.cRFM_TurnLeft = (signed char)(ftmp*100.0);

	m_EditRFMCoff_TurnRight.GetWindowText((LPTSTR)cbuf,10);
	try { ftmp = atof((const char *)cbuf); }
	catch(...){ }
	if(ftmp>+1.0) ftmp=+1.0;
	if(ftmp<-1.0) ftmp=-1.0;
	strCmdFmSetDRLSCoff.RLSCOFF.cRFM_TurnRight = (signed char)(ftmp*100.0);

	m_EditRBMCoff_TurnLeft.GetWindowText((LPTSTR)cbuf,10);
	try { ftmp = atof((const char *)cbuf); }
	catch(...){ }
	if(ftmp>+1.0) ftmp=+1.0;
	if(ftmp<-1.0) ftmp=-1.0;
	strCmdFmSetDRLSCoff.RLSCOFF.cRBM_TurnLeft = (signed char)(ftmp*100.0);

	m_EditRBMCoff_TurnRight.GetWindowText((LPTSTR)cbuf,10);
	try { ftmp = atof((const char *)cbuf); }
	catch(...){ }
	if(ftmp>+1.0) ftmp=+1.0;
	if(ftmp<-1.0) ftmp=-1.0;
	strCmdFmSetDRLSCoff.RLSCOFF.cRBM_TurnRight = (signed char)(ftmp*100.0);

	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmSetDRLSCoff,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmSetDRLSCoff,stmp);
	Sleep(500);
	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmSetDRLSCoff,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmSetDRLSCoff,stmp);
	if(bDispTxHex) cSysInfo.vAddString(stmp);
	stmp.Format(_T("设置转弯参数")); cSysInfo.vAddString(stmp);

	m_BtnSetTurnCoff.EnableWindow(true);	
}

void CVcAgvDlg::OnBnClickedButtonXJGo()
{
	CString stmp;

	m_BtnRunXJCmd.EnableWindow(false);

	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmXJGo,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmXJGo,stmp);
	Sleep(500);
	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmXJGo,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmXJGo,stmp);
	if(bDispTxHex) cSysInfo.vAddString(stmp);
	stmp.Format(_T("开始循迹前行...")); cSysInfo.vAddString(stmp);

	m_BtnRunXJCmd.EnableWindow(true);
}

void CVcAgvDlg::OnBnClickedCheckRunMode()
{
	if(m_ChkRunMode.GetCheck()==1)
	{
		m_ComboXJSensorNum.EnableWindow(false);
		m_ChkStampWire.EnableWindow(false);
		m_ChkGripWire.EnableWindow(false);
		m_EditXJMaxRSpeed.EnableWindow(false);
		m_EditXJMaxLSpeed.EnableWindow(false);
		m_BtnSetXJPara.EnableWindow(false);

		m_ChkL2BpEn.EnableWindow(false);
		m_ChkL1BpEn.EnableWindow(false);
		m_ChkR1BpEn.EnableWindow(false);
		m_ChkR2BpEn.EnableWindow(false);

		m_EditDevTurnSpeed.EnableWindow(false);
		m_EditDevGoSpeed.EnableWindow(false);
		m_BtnSetDevSpeed.EnableWindow(false);

		m_Edit_LFM_Pwm.EnableWindow(false);
		m_Edit_LBM_Pwm.EnableWindow(false);
		m_Edit_RFM_Pwm.EnableWindow(false);
		m_Edit_RBM_Pwm.EnableWindow(false);
		m_ChkRunMode.SetWindowText("运行模式");
		m_BtnSetMotorPwm.EnableWindow(false);

		m_EditLFMCoff_TurnLeft.EnableWindow(false);
		m_EditLFMCoff_TurnRight.EnableWindow(false);
		m_EditLBMCoff_TurnLeft.EnableWindow(false);
		m_EditLBMCoff_TurnRight.EnableWindow(false);
		m_EditRFMCoff_TurnLeft.EnableWindow(false);
		m_EditRFMCoff_TurnRight.EnableWindow(false);
		m_EditRBMCoff_TurnLeft.EnableWindow(false);
		m_EditRBMCoff_TurnRight.EnableWindow(false);
		m_BtnSetTurnCoff.EnableWindow(false);
	}
	else 
	{
		m_ComboXJSensorNum.EnableWindow(true);
		m_ChkStampWire.EnableWindow(true);
		m_ChkGripWire.EnableWindow(true);
		m_EditXJMaxRSpeed.EnableWindow(true);
		m_EditXJMaxLSpeed.EnableWindow(true);
		m_BtnSetXJPara.EnableWindow(true);

		m_ChkL2BpEn.EnableWindow(true);
		m_ChkL1BpEn.EnableWindow(true);
		m_ChkR1BpEn.EnableWindow(true);
		m_ChkR2BpEn.EnableWindow(true);

		m_EditDevTurnSpeed.EnableWindow(true);
		m_EditDevGoSpeed.EnableWindow(true);
		m_BtnSetDevSpeed.EnableWindow(true);

		m_Edit_LFM_Pwm.EnableWindow(true);
		m_Edit_LBM_Pwm.EnableWindow(true);
		m_Edit_RFM_Pwm.EnableWindow(true);
		m_Edit_RBM_Pwm.EnableWindow(true);
		m_ChkRunMode.SetWindowText("调试模式");
		m_BtnSetMotorPwm.EnableWindow(true);

		m_EditLFMCoff_TurnLeft.EnableWindow(true);
		m_EditLFMCoff_TurnRight.EnableWindow(true);
		m_EditLBMCoff_TurnLeft.EnableWindow(true);
		m_EditLBMCoff_TurnRight.EnableWindow(true);
		m_EditRFMCoff_TurnLeft.EnableWindow(true);
		m_EditRFMCoff_TurnRight.EnableWindow(true);
		m_EditRBMCoff_TurnLeft.EnableWindow(true);
		m_EditRBMCoff_TurnRight.EnableWindow(true);
		m_BtnSetTurnCoff.EnableWindow(true);
	}
}
void CVcAgvDlg::OnBnClickedCheckL2xj()
{
	CString stmp;
	UNN_DEV_DETECTOR unnXJDector;

	if(m_ChkSimXJDector.GetCheck()==1)
	{

		unnXJDector.C.cXJDector = 0;

		if(m_ChkL2XJ.GetCheck()==1) 	unnXJDector.B.bL2XJ = 1;
		else unnXJDector.B.bL2XJ = 0;
		unnXJDector.B.bRunDetector = 0;//通知设备不使用真实传感器，使用仿真数据，停止检测（循迹启动的时候自动清除）
	
		strCmdFmSimDetector.DETECTOR.C.cXJDector = unnXJDector.C.cXJDector;
		if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmSimDetector,stmp);
		else CNetPort.vTxVarFm((unsigned char *)&strCmdFmSimDetector,stmp);
		if(bDispTxHex) cSysInfo.vAddString(stmp);
		stmp.Format(_T("模拟传感器检测信号")); cSysInfo.vAddString(stmp);

		m_ChkL1XJ.SetCheck(0);
		m_ChkR1XJ.SetCheck(0);
		m_ChkR2XJ.SetCheck(0);
	}
	else
	{
		stmp.Format(_T("请选择模拟选项后...")); cSysInfo.vAddString(stmp);
	}
}

void CVcAgvDlg::OnBnClickedCheckL1xj()
{
	CString stmp;
	UNN_DEV_DETECTOR unnXJDector;

	if(m_ChkSimXJDector.GetCheck()==1)
	{
		unnXJDector.C.cXJDector = 0;

		if(m_ChkL1XJ.GetCheck()==1) 	unnXJDector.B.bL1XJ = 1;
		else unnXJDector.B.bL1XJ = 0;
		unnXJDector.B.bRunDetector = 0;//通知设备不使用真实传感器，使用仿真数据，停止检测（循迹启动的时候自动清除）
	
		strCmdFmSimDetector.DETECTOR.C.cXJDector = unnXJDector.C.cXJDector;
		if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmSimDetector,stmp);
		else CNetPort.vTxVarFm((unsigned char *)&strCmdFmSimDetector,stmp);
		if(bDispTxHex) cSysInfo.vAddString(stmp);
		stmp.Format(_T("模拟传感器检测信号")); cSysInfo.vAddString(stmp);

		m_ChkL2XJ.SetCheck(0);
		m_ChkR1XJ.SetCheck(0);
		m_ChkR2XJ.SetCheck(0);
	}
	else
	{
		stmp.Format(_T("请选择模拟选项后...")); cSysInfo.vAddString(stmp);
	}
}

void CVcAgvDlg::OnBnClickedCheckR1xj()
{
	CString stmp;
	UNN_DEV_DETECTOR unnXJDector;

	if(m_ChkSimXJDector.GetCheck()==1)
	{
		unnXJDector.C.cXJDector = 0;

		if(m_ChkR1XJ.GetCheck()==1) 	unnXJDector.B.bR1XJ = 1;
		else unnXJDector.B.bR1XJ = 0;
		unnXJDector.B.bRunDetector = 0;//通知设备不使用真实传感器，使用仿真数据，停止检测（循迹启动的时候自动清除）
	
		strCmdFmSimDetector.DETECTOR.C.cXJDector = unnXJDector.C.cXJDector;
		if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmSimDetector,stmp);
		else CNetPort.vTxVarFm((unsigned char *)&strCmdFmSimDetector,stmp);
		if(bDispTxHex) cSysInfo.vAddString(stmp);
		stmp.Format(_T("模拟传感器检测信号")); cSysInfo.vAddString(stmp);

		m_ChkL2XJ.SetCheck(0);
		m_ChkL1XJ.SetCheck(0);
		m_ChkR2XJ.SetCheck(0);
	}
	else
	{
		stmp.Format(_T("请选择模拟选项后...")); cSysInfo.vAddString(stmp);
	}
}

void CVcAgvDlg::OnBnClickedCheckR2xj()
{
	CString stmp;
	UNN_DEV_DETECTOR unnXJDector;

	if(m_ChkSimXJDector.GetCheck()==1)
	{
		unnXJDector.C.cXJDector = 0;

		if(m_ChkR2XJ.GetCheck()==1) 	unnXJDector.B.bR2XJ = 1;
		else unnXJDector.B.bR2XJ = 0;
		unnXJDector.B.bRunDetector = 0;//通知设备不使用真实传感器，使用仿真数据，停止检测（循迹启动的时候自动清除）
	
		strCmdFmSimDetector.DETECTOR.C.cXJDector = unnXJDector.C.cXJDector;
		if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmSimDetector,stmp);
		else CNetPort.vTxVarFm((unsigned char *)&strCmdFmSimDetector,stmp);
		if(bDispTxHex) cSysInfo.vAddString(stmp);
		stmp.Format(_T("模拟传感器检测信号")); cSysInfo.vAddString(stmp);

		m_ChkL2XJ.SetCheck(0);
		m_ChkL1XJ.SetCheck(0);
		m_ChkR1XJ.SetCheck(0);
	}
	else
	{
		stmp.Format(_T("请选择模拟选项后...")); cSysInfo.vAddString(stmp);
	}
}

void CVcAgvDlg::OnBnClickedCheckSimxjdetector()
{
	CString stmp;

	if(m_ChkSimXJDector.GetCheck()==1)
	{
		strDevKVar.DETECTOR.C.cXJDector = 0;
		strDevKVar.DETECTOR.B.bRunDetector = 0;//通知设备不使用真实传感器，使用仿真数据，停止检测（循迹启动的时候自动清除）
	
		strCmdFmSimDetector.DETECTOR.C.cXJDector = strDevKVar.DETECTOR.C.cXJDector;
		if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmSimDetector,stmp);
		else CNetPort.vTxVarFm((unsigned char *)&strCmdFmSimDetector,stmp);
		if(bDispTxHex) cSysInfo.vAddString(stmp);
		stmp.Format(_T("模拟传感器检测信号")); cSysInfo.vAddString(stmp);

		m_ChkL2XJ.SetCheck(0);
		m_ChkL1XJ.SetCheck(0);
		m_ChkR1XJ.SetCheck(0);
		m_ChkR2XJ.SetCheck(0);
	}
}

void CVcAgvDlg::OnBnClickedButtonDgbGoStraight()
{
	int itmp;
	unsigned char cbuf[32];
	CString stmp;

	m_BtnDbgGoStraight.EnableWindow(false);

	m_EditDevGoSpeed.GetWindowText((LPTSTR)cbuf,10);
	try { itmp = atoi((const char *)cbuf); }
	catch(...){ }
	if(itmp<0) itmp=-itmp;
	if(itmp>PWM_MAXTWC) itmp=+PWM_MAXTWC;
	strCmdFmDbgeDevSpeed.cTurnSpeed = 0;
	strCmdFmDbgeDevSpeed.cGoSpeed = itmp;
	stmp.Format(_T("%+d"),itmp); m_EditDevGoSpeed.SetWindowText(stmp);

	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	stmp.Format(_T("调试车辆直线前行(速度=%d))"), (INT16)strCmdFmDbgeDevSpeed.cGoSpeed); cSysInfo.vAddString(stmp);
	Sleep(100);
	if(m_ChkGoTurnJog.GetCheck()==1)
	{
		strCmdFmDbgeDevSpeed.cTurnSpeed = 0;
		strCmdFmDbgeDevSpeed.cGoSpeed = 0;
	}
	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	if(bDispTxHex) cSysInfo.vAddString(stmp);

	m_BtnDbgGoStraight.EnableWindow(true);
}

void CVcAgvDlg::OnBnClickedButtonDgbgGoBack()
{
	int itmp;
	unsigned char cbuf[32];
	CString stmp;

	m_BtnDbgGoBack.EnableWindow(false);

	m_EditDevGoSpeed.GetWindowText((LPTSTR)cbuf,10);
	try { itmp = atoi((const char *)cbuf); }
	catch(...){ }
	if(itmp>0) itmp=-itmp;
	if(itmp<-PWM_MAXTWC) itmp=-PWM_MAXTWC;
	strCmdFmDbgeDevSpeed.cTurnSpeed = 0;
	strCmdFmDbgeDevSpeed.cGoSpeed = itmp;
	stmp.Format(_T("%-d"),itmp); m_EditDevGoSpeed.SetWindowText(stmp);

	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	stmp.Format(_T("调试车辆直线后退(速度=%d))"), (INT16)strCmdFmDbgeDevSpeed.cGoSpeed); cSysInfo.vAddString(stmp);
	Sleep(100);
	if(m_ChkGoTurnJog.GetCheck()==1)
	{
		strCmdFmDbgeDevSpeed.cTurnSpeed = 0;
		strCmdFmDbgeDevSpeed.cGoSpeed = 0;
	}
	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	if(bDispTxHex) cSysInfo.vAddString(stmp);

	m_BtnDbgGoBack.EnableWindow(true);
}

void CVcAgvDlg::OnBnClickedButtonDgbTurnLeft()
{
	int itmp;
	unsigned char cbuf[32];
	CString stmp;

	m_BtnDbgTurnLeft.EnableWindow(false);

	m_EditDevTurnSpeed.GetWindowText((LPTSTR)cbuf,10);
	try { itmp = atoi((const char *)cbuf); }
	catch(...){ }
	if(itmp<0) itmp=-itmp;
	if(itmp>+PWM_MAXTWC) itmp=+PWM_MAXTWC;
	strCmdFmDbgeDevSpeed.cTurnSpeed = itmp;
	strCmdFmDbgeDevSpeed.cGoSpeed = 0;
	stmp.Format(_T("%+d"),itmp); m_EditDevTurnSpeed.SetWindowText(stmp);

	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	stmp.Format(_T("调试车辆左转弯(速度=%d))"), (INT16)strCmdFmDbgeDevSpeed.cTurnSpeed); cSysInfo.vAddString(stmp);
	Sleep(100);
	if(m_ChkGoTurnJog.GetCheck()==1)
	{
		strCmdFmDbgeDevSpeed.cTurnSpeed = 0;
		strCmdFmDbgeDevSpeed.cGoSpeed = 0;
	}
	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	if(bDispTxHex) cSysInfo.vAddString(stmp);

	m_BtnDbgTurnLeft.EnableWindow(true);
}

void CVcAgvDlg::OnBnClickedButtonDgbTurnRight()
{
	int itmp;
	unsigned char cbuf[32];
	CString stmp;

	m_BtnDbgTurnRight.EnableWindow(false);

	m_EditDevTurnSpeed.GetWindowText((LPTSTR)cbuf,10);
	try { itmp = atoi((const char *)cbuf); }
	catch(...){ }
	if(itmp>0) itmp=-itmp;
	if(itmp<-PWM_MAXTWC) itmp=-PWM_MAXTWC;
	strCmdFmDbgeDevSpeed.cTurnSpeed = itmp;
	strCmdFmDbgeDevSpeed.cGoSpeed = 0;
	stmp.Format(_T("%-d"),itmp); m_EditDevTurnSpeed.SetWindowText(stmp);

	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	stmp.Format(_T("调试车辆右转弯(速度=%d))"), (INT16)strCmdFmDbgeDevSpeed.cTurnSpeed); cSysInfo.vAddString(stmp);
	Sleep(100);
	if(m_ChkGoTurnJog.GetCheck()==1)
	{
		strCmdFmDbgeDevSpeed.cTurnSpeed = 0;
		strCmdFmDbgeDevSpeed.cGoSpeed = 0;
	}
	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgeDevSpeed,stmp);
	if(bDispTxHex) cSysInfo.vAddString(stmp);

	m_BtnDbgTurnRight.EnableWindow(true);
}

void CVcAgvDlg::OnBnClickedCheckExtendUI()
{

	if(m_ChkExtendUI.GetCheck()==0) 
	{
		CRect rc;
		m_ChkExtendUI.SetWindowTextA(">>");
		m_EditSysInfo.GetWindowRect(&rc);
		m_ChkSimXJDector.ShowWindow(false);
		m_ChkDispTxHex.ShowWindow(false);
		m_ChkDispRxHex.ShowWindow(false);
		m_EditSysInfo.ShowWindow(false);
		this->SetWindowPos(NULL, m_rcMainWin.left, m_rcMainWin.top, m_rcMainWin.right-m_rcMainWin.left-(rc.right-rc.left), m_rcMainWin.bottom-m_rcMainWin.top, SWP_NOMOVE);
	}
	else
	{
		m_ChkExtendUI.SetWindowTextA("<<");
		m_ChkSimXJDector.ShowWindow(true);
		m_ChkDispTxHex.ShowWindow(true);
		m_ChkDispRxHex.ShowWindow(true);
		m_EditSysInfo.ShowWindow(true);
		this->SetWindowPos(NULL, m_rcMainWin.left, m_rcMainWin.top, m_rcMainWin.right-m_rcMainWin.left, m_rcMainWin.bottom-m_rcMainWin.top, SWP_NOMOVE);
	}
}

char   cFlagChar[10]="/-\\|*";
UINT16 iFlagIndex = 0;
UINT16 iNetWDogCnt = 0;
void CVcAgvDlg::vUpdateScreen(void)
{
	CString stmp;

	////////////////////////////////////////////////////////////////////////////
	if(CUartPort.m_iRxFmCnt) 
	{
		iFlagIndex++; iFlagIndex %= 5;
		stmp.Format(_T("[ %c ]"),cFlagChar[iFlagIndex]);  m_UartRxFmFlag.SetWindowText(stmp);
	}
	if(CNetPort.m_iRxFmCnt)
	{
		iFlagIndex++; iFlagIndex %= 5; iNetWDogCnt=0;
		stmp.Format(_T("< %c >"),cFlagChar[iFlagIndex]);  m_UartRxFmFlag.SetWindowText(stmp);
	}
	else if(CNetPort.m_bOpen==true)
	{
		iNetWDogCnt++;
		if(iNetWDogCnt%100==0) 
		{
			stmp.Format(_T("网络错误:设备已关闭或信道问题，请检查并重新连接测试...%d"),iNetWDogCnt/100);
			cSysInfo.vAddString(stmp);
		}
	}

	if(bDevCfgUpdated)
	{
		stmp.Format(_T("%3.2f"),(double)strDevCfg.RLSCOFF.cLFM_TurnLeft/100.0); m_EditLFMCoff_TurnLeft.SetWindowText(stmp);
		stmp.Format(_T("%3.2f"),(double)strDevCfg.RLSCOFF.cLFM_TurnRight/100.0); m_EditLFMCoff_TurnRight.SetWindowText(stmp);
		stmp.Format(_T("%3.2f"),(double)strDevCfg.RLSCOFF.cLBM_TurnLeft/100.0); m_EditLBMCoff_TurnLeft.SetWindowText(stmp);
		stmp.Format(_T("%3.2f"),(double)strDevCfg.RLSCOFF.cLBM_TurnRight/100.0); m_EditLBMCoff_TurnRight.SetWindowText(stmp);
		stmp.Format(_T("%3.2f"),(double)strDevCfg.RLSCOFF.cRFM_TurnLeft/100.0); m_EditRFMCoff_TurnLeft.SetWindowText(stmp);
		stmp.Format(_T("%3.2f"),(double)strDevCfg.RLSCOFF.cRFM_TurnRight/100.0); m_EditRFMCoff_TurnRight.SetWindowText(stmp);
		stmp.Format(_T("%3.2f"),(double)strDevCfg.RLSCOFF.cRBM_TurnLeft/100.0); m_EditRBMCoff_TurnLeft.SetWindowText(stmp);
		stmp.Format(_T("%3.2f"),(double)strDevCfg.RLSCOFF.cRBM_TurnRight/100.0); m_EditRBMCoff_TurnRight.SetWindowText(stmp);

		stmp.Format(_T("%d"),strDevCfg.XJPARA.cMaxGoSpeed); m_EditXJMaxLSpeed.SetWindowText(stmp);
		stmp.Format(_T("%d"),strDevCfg.XJPARA.cMaxTurnSpeed); m_EditXJMaxRSpeed.SetWindowText(stmp);

		if(strDevCfg.XJPARA.cDectorSel>=2)
		{
			m_ComboXJSensorNum.SetCurSel(strDevCfg.XJPARA.cDectorSel-2);
			stmp.Format(_T("%d只传感器"),strDevCfg.XJPARA.cDectorSel); m_ComboXJSensorNum.SetWindowText(stmp);
		}
		else
		{
			strDevCfg.XJPARA.cDectorSel = 2;
			m_ComboXJSensorNum.SetCurSel(strDevCfg.XJPARA.cDectorSel-2);
		}
		stmp.Format(_T("%1d 只红外"),strDevCfg.XJPARA.cDectorSel); m_ComboXJSensorNum.SetWindowText(stmp);

		if(strDevCfg.XJPARA.bIsStampWire) 
		{
			m_ChkStampWire.SetCheck(1);
			m_ChkGripWire.SetCheck(0);
		}
		else
		{
			m_ChkStampWire.SetCheck(0);
			m_ChkGripWire.SetCheck(1);
		}
		bDevCfgUpdated = false;
	}

	if((m_ChkRunMode.GetCheck()==1) && bDevKVarUpdated)
	{
		stmp.Format(_T("%d"),strDevKVar.PWM.cLFM_Pwm); m_Edit_LFM_Pwm.SetWindowText(stmp);
		stmp.Format(_T("%d"),strDevKVar.PWM.cLBM_Pwm); m_Edit_LBM_Pwm.SetWindowText(stmp);
		stmp.Format(_T("%d"),strDevKVar.PWM.cRFM_Pwm); m_Edit_RFM_Pwm.SetWindowText(stmp);
		stmp.Format(_T("%d"),strDevKVar.PWM.cRBM_Pwm); m_Edit_RBM_Pwm.SetWindowText(stmp);
	}

	if(bDevKVarUpdated && (m_ChkSimXJDector.GetCheck()==0))
	{
			if(strDevKVar.DETECTOR.B.bL2XJ) m_ChkL2XJ.SetCheck(1);
			else m_ChkL2XJ.SetCheck(0);

			if(strDevKVar.DETECTOR.B.bL1XJ) m_ChkL1XJ.SetCheck(1);
			else m_ChkL1XJ.SetCheck(0);

			if(strDevKVar.DETECTOR.B.bR1XJ) m_ChkR1XJ.SetCheck(1);
			else m_ChkR1XJ.SetCheck(0);

			if(strDevKVar.DETECTOR.B.bR2XJ) m_ChkR2XJ.SetCheck(1);
			else m_ChkR2XJ.SetCheck(0);
	}
	bDevKVarUpdated = false;
	////////////////////////////////////////////////////////////////////////////
	if(cSysInfo.m_bUpdated)
	{
		m_EditSysInfo.SetWindowText("");
		stmp = cSysInfo.vGetLasest(40); m_EditSysInfo.SetWindowText(stmp);  //m_EditSysInfo.SetSel(
		int nLen=m_EditSysInfo.GetWindowTextLength();//获取控件中内容的大小
		m_EditSysInfo.SetSel(nLen, nLen);//将光标位置跳到最后一个字符
	}

	CNetPort.m_iRxFmCnt = 0;
	CUartPort.m_iRxFmCnt = 0;
}

void CVcAgvDlg::OnBnClickedOk()
{
	CString stmp;

	strCmdFmDbgMotorPwm.PWM.cLFM_Pwm = 0;
	strCmdFmDbgMotorPwm.PWM.cLBM_Pwm = 0;
	strCmdFmDbgMotorPwm.PWM.cRFM_Pwm = 0;
	strCmdFmDbgMotorPwm.PWM.cRBM_Pwm = 0;

	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgMotorPwm,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgMotorPwm,stmp);
	if(bDispTxHex) cSysInfo.vAddString(stmp);
	stmp.Format(_T("电机停止运行，准备退出...)")); cSysInfo.vAddString(stmp);
	Sleep(500);
	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgMotorPwm,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgMotorPwm,stmp);	
	Sleep(500);
	if(bCommByUart) CUartPort.vTxVarFm((unsigned char *)&strCmdFmDbgMotorPwm,stmp);
	else CNetPort.vTxVarFm((unsigned char *)&strCmdFmDbgMotorPwm,stmp);	
	CDialogEx::OnOK();
}


void CVcAgvDlg::OnBnClickedCheckGoturnjog()
{
	// TODO: 在此添加控件通知处理程序代码
}
