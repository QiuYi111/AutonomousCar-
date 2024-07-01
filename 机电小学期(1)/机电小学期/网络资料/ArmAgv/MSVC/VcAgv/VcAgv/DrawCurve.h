
#pragma once
#include "afxwin.h"

#define SIZE_INFOCONT	100
#define NULL_CURVEINDEX	-1

typedef struct
{
	double fMax;
	double fMin;
	bool   bVisible;
	int    iColor;
}STRN_CURVSE_BASIC;

struct STRN_CURVEPROPERTY
{
	int m_iIndex;
	bool m_bVisible;

	double *m_fValue;
	double m_fMaxValue;
	double m_fMinValue;
	
	bool m_bLineTag;

	COLORREF m_iColor;
	char m_cTipCont[SIZE_INFOCONT];

	struct STRN_CURVEPROPERTY *m_pStrLastCurve;
	struct STRN_CURVEPROPERTY *m_pStrNextCurve;
};

class CDrawCurve
{
// 构造
public:
	CDrawCurve( );	// 标准构造函数
	~CDrawCurve( );

// 实现
protected:
	CStatic *m_PicCtrl;

	bool m_bMiniGrid;
	int m_iCurveNum;
	int m_fWinLenX;
	int m_fWinLenY;
	int m_iWinGapX;
	int m_iWinGapY;
	double m_fSampleTime;

	int m_iCurInfoX;
	int m_iCurInfoY;
	int m_iColorIndex;

	int m_iBufLen;
	int m_iCallCnt;
	struct STRN_CURVEPROPERTY *m_pStrAllCurve;

public:
	int  iInsert(int iCurveIndex, struct STRN_CURVEPROPERTY *strCurve);
	int  iStrLen(char *cStringChars);
	void vSetBasic(CStatic *PicCtrl, int iTotalLen, double fSampleTime, bool bMiniGrid);
	void vSetSingleVisible(int iIndex, bool bvisible);
	void vSetAllVisible(bool bvisible);
	bool bIsSingleVisible(int iCurveIndex);
	void vClearAllData(double fasdata);
	void vDelteAllCurve(void);

	void vSetSingleMax(int iCurveIndex, double fmax);
	void vSetSingleMin(int iCurveIndex, double fmin);
	void vSetSingleColor(int iCurveIndex, COLORREF color);

	int    iGetSingleColor(int iCurveIndex);
	double fGetSingleMax(int iCurveIndex);
	double fGetSingleMin(int iCurveIndex);
	bool   bGetSingleVisible(int iCurveIndex);
	void   vModifyCurveProperty(int index, STRN_CURVSE_BASIC *pStrPropty);
	void   vGetCurveProperty(int index, STRN_CURVSE_BASIC *pStrPropty);
/*	
	void vMoveSingleDown(int index);
	void vMoveSingleUp(int index);
	void vMoveSingleReset(int index);
	void vMoveAllDown(void);
	void vMoveAllUp(void);
	void vMoveAllReset(void);
*/
//	void vRefresh(int iFromPos, int iToPos);
	void vRefreshStatic(void);
	void vRefreshDynamic(int iFromPos, int iToPos);
//	void vDrawSingle(HDC hDC, int iCurveIndex, int iFromPos, int iToPos);
	void vDrawAll(HDC hMemDC, int iFromPos, int iToPos);

	void vRefreshXY(int icvx0, int icvy0, int icvx1, int icvy1, int icvx2, int icvy2, int iFromPos, int iToPos);
	void vDrawXY(HDC hMemDC, int icvx, int icvy, int iFromPos, int iToPos);
};
