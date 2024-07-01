
// VcAgvDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CVcAgvDlg 对话框
class CVcAgvDlg : public CDialogEx
{
// 构造
public:
	CVcAgvDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VCAGV_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HCURSOR OnQueryDragIcon();
	BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

public:
	CToolTipCtrl m_BtnBuildLinkTooTip;
	CToolTipCtrl m_BtnXunJiTooTip;

public:
	void vInitAll(void);
	void vUpdateScreen(void);
	void vFillCmdFmBasic(void);

	CEdit m_EditSysInfo;
	CButton m_ChkLinkWifi;
	afx_msg void OnBnClickedCheckTest();

	CButton m_ChkWifiMode;
	CButton m_ChkDispTxHex;
	CButton m_ChkDispRxHex;
	CStatic m_UartRxFmFlag;
	afx_msg void OnCbnSelchangeComboUartport();
	afx_msg void OnBnClickedCheckDispuarttxhex();
	afx_msg void OnBnClickedCheckDispuartrxhex();
	afx_msg void OnBnClickedCheckAltmode();

	CButton m_ChkL2XJ;
	CButton m_ChkL1XJ;
	CButton m_ChkR1XJ;
	CButton m_ChkR2XJ;

	CEdit m_Edit_LFM_Pwm;
	CEdit m_Edit_LBM_Pwm;
	CEdit m_Edit_RFM_Pwm;
	CEdit m_Edit_RBM_Pwm;
	CButton m_BtnSetMotorPwm;
	afx_msg void OnBnClickedButtonSetMotorPwm();
	afx_msg void OnBnClickedButtonSetmotorstop();
	CEdit m_EditDevTurnSpeed;
	CEdit m_EditDevGoSpeed;
	CButton m_BtnSetDevSpeed;
	afx_msg void OnBnClickedButtonSetdevspeed();
	
	CStatic m_FrameDbgMotorPwm;
	CStatic m_FrameDbgDevSpeed;
	CComboBox m_ComboXJSensorNum;
	CComboBox m_CComUartPort;
	CButton m_ChkStampWire;
	CButton m_ChkGripWire;
	CStatic m_PicTurnCoff;

	CEdit m_EditLFMCoff_TurnLeft;
	CEdit m_EditLFMCoff_TurnRight;
	CEdit m_EditLBMCoff_TurnLeft;
	CEdit m_EditLBMCoff_TurnRight;
	CEdit m_EditRFMCoff_TurnLeft;
	CEdit m_EditRFMCoff_TurnRight;
	CEdit m_EditRBMCoff_TurnLeft;
	CEdit m_EditRBMCoff_TurnRight;

	afx_msg void OnBnClickedButtonSetXJPara();
	CButton m_BtnSetXJPara;
	CEdit m_EditXJMaxLSpeed;
	CEdit m_EditXJMaxRSpeed;
	afx_msg void OnBnClickedCheckStampwire();
	afx_msg void OnBnClickedCheckGripwire();
	afx_msg void OnBnClickedButtonSetTurnCoff();
	CButton m_BtnSetTurnCoff;
	afx_msg void OnBnClickedButtonXJGo();
	CButton m_ChkRunMode;
	afx_msg void OnBnClickedCheckRunMode();
	CStatic m_FrameXJOption;
	afx_msg void OnBnClickedCheckL2xj();
	afx_msg void OnBnClickedCheckL1xj();
	afx_msg void OnBnClickedCheckR1xj();
	afx_msg void OnBnClickedCheckR2xj();
	CButton m_BtnStopMotors;
	CButton m_ChkSimXJDector;
	afx_msg void OnBnClickedCheckSimxjdetector();
	CButton m_BtnRunXJCmd;
	afx_msg void OnBnClickedButtonDgbGoStraight();
	afx_msg void OnBnClickedButtonDgbgGoBack();
	afx_msg void OnBnClickedButtonDgbTurnLeft();
	afx_msg void OnBnClickedButtonDgbTurnRight();
	afx_msg void OnBnClickedCheckExtendUI();
	CButton m_ChkExtendUI;
	
	CFont m_fontBtnStop;
	CFont m_fontLinkWifi;
	CRect m_rcMainWin;
	CButton m_BtnExitSystem;
	afx_msg void OnBnClickedOk();
	CButton m_ChkGoTurnJog;

	CButton m_BtnDbgTurnLeft;
	CButton m_BtnDbgTurnRight;
	CButton m_BtnDbgGoStraight;
	CButton m_BtnDbgGoBack;
	CButton m_ChkL2BpEn;
	CButton m_ChkL1BpEn;
	CButton m_ChkR1BpEn;
	CButton m_ChkR2BpEn;
	afx_msg void OnBnClickedCheckGoturnjog();
};
