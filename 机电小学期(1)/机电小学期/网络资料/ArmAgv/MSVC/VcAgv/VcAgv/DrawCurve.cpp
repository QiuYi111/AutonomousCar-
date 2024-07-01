#include "stdafx.h"
#include "math.h"
#include "process.h"
#include "stdio.h"
#include "DrawCurve.h"
#include "VcType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDrawCurve::CDrawCurve(void)
{
	m_iBufLen = 0;
	m_pStrAllCurve = (struct STRN_CURVEPROPERTY *)NULL;
	m_PicCtrl = (CStatic *)NULL;
	m_iCurveNum = 0;
	m_iColorIndex = 0;
	m_iCallCnt = 0;
}

void CDrawCurve::vSetBasic(CStatic *oPicCtrl, int iTotalBufLen, double fSampleTime, bool bMiniGrid)
{
	if(oPicCtrl) m_PicCtrl = oPicCtrl;
	m_iBufLen = iTotalBufLen;
	m_fSampleTime = fSampleTime;
	m_bMiniGrid = bMiniGrid;
}

//修改曲线属性
void CDrawCurve::vModifyCurveProperty(int iCurveIndex, STRN_CURVSE_BASIC *pStrPropty)
{
	struct STRN_CURVEPROPERTY *curptr = m_pStrAllCurve;

 	if( iCurveIndex >= m_iCurveNum ) return;

	while( (curptr!=NULL) && (curptr->m_iIndex!= iCurveIndex) )
		curptr = curptr->m_pStrNextCurve;

	if( (curptr->m_iIndex == iCurveIndex) && (curptr!=NULL) )
	{
		curptr->m_bVisible = pStrPropty->bVisible;
		curptr->m_iColor = pStrPropty->iColor;
		curptr->m_fMaxValue = pStrPropty->fMax;
		curptr->m_fMinValue = pStrPropty->fMin;
	}
	else
		return;
}

//修改曲线属性
void CDrawCurve::vGetCurveProperty(int iCurveIndex, STRN_CURVSE_BASIC *pStrPropty)
{
	struct STRN_CURVEPROPERTY *curptr = m_pStrAllCurve;

 	if( iCurveIndex >= m_iCurveNum )
	{
		pStrPropty->bVisible = false;
		pStrPropty->iColor = 0;
		pStrPropty->fMax = +5;
		pStrPropty->fMin = -5;
		return;
	}

	while( (curptr!=NULL) && (curptr->m_iIndex!= iCurveIndex) )
		curptr = curptr->m_pStrNextCurve;

	if( (curptr->m_iIndex == iCurveIndex) && (curptr!=NULL) )
	{
		pStrPropty->bVisible = curptr->m_bVisible;
		pStrPropty->iColor = curptr->m_iColor;
		pStrPropty->fMax = curptr->m_fMaxValue;
		pStrPropty->fMin = curptr->m_fMinValue;
	}
	else
	{
		pStrPropty->bVisible = false;
		pStrPropty->iColor = 0;
		pStrPropty->fMax = +5;
		pStrPropty->fMin = -5;
	}
}

CDrawCurve::~CDrawCurve( )
{
	vDelteAllCurve();
}
/*
void CDrawCurve::vMoveSingleUp(int index)
{
	int iIndexIdx = 0;
	struct STRN_CURVEPROPERTY *curptr;
	
	if( index >= m_iCurveNum ) return;
	if( m_pStrAllCurve == NULL ) return;

	curptr = m_pStrAllCurve;
	if(index)
	{
		do{
			iIndexIdx++;
			curptr = curptr->m_pStrNextCurve;
		}while( (curptr!=NULL) && (iIndexIdx<index) );
	}
	if( (iIndexIdx!=index) || (curptr==NULL) ) return;

	if(curptr->m_fOffsetValue + (curptr->m_fMaxValue - curptr->m_fMinValue)/40.0 < curptr->m_fMaxValue)
		curptr->m_fOffsetValue = curptr->m_fOffsetValue + (curptr->m_fMaxValue - curptr->m_fMinValue)/40.0;
}

void CDrawCurve::vMoveSingleReset(int index)
{
	int iIndexIdx = 0;
	struct STRN_CURVEPROPERTY *curptr;

	if( index >= m_iCurveNum ) return;
	if( m_pStrAllCurve == NULL ) return;


	curptr = m_pStrAllCurve;
	if(index)
	{
		do{
			iIndexIdx++;
			curptr = curptr->m_pStrNextCurve;
		}while( (curptr!=NULL) && (iIndexIdx<index) );
	}
	if( (iIndexIdx!=index) || (curptr==NULL) ) return;
	curptr->m_fOffsetValue = 0;
}

void CDrawCurve::vMoveSingleDown(int index)
{
	int iIndexIdx = 0;
	struct STRN_CURVEPROPERTY *curptr;

	if( index >= m_iCurveNum ) return;
	if( m_pStrAllCurve == NULL ) return;

	curptr = m_pStrAllCurve;
	if(index)
	{
		do{
			iIndexIdx++;
			curptr = curptr->m_pStrNextCurve;
		}while( (curptr!=NULL) && (iIndexIdx<index) );
	}
	if( (iIndexIdx!=index) || (curptr==NULL) ) return;

	if(curptr->m_fOffsetValue - (curptr->m_fMaxValue - curptr->m_fMinValue)/40.0 > curptr->m_fMinValue)
		curptr->m_fOffsetValue = curptr->m_fOffsetValue - (curptr->m_fMaxValue - curptr->m_fMinValue)/40.0;
}

void CDrawCurve::vMoveAllDown(void)
{
	struct STRN_CURVEPROPERTY *curptr;

	curptr = m_pStrAllCurve;
	while(curptr!=NULL)
	{
		if( curptr->m_fOffsetValue - (curptr->m_fMaxValue - curptr->m_fMinValue)/40.0 > curptr->m_fMinValue)
			curptr->m_fOffsetValue = curptr->m_fOffsetValue - (curptr->m_fMaxValue - curptr->m_fMinValue)/40.0;
		curptr = curptr->m_pStrNextCurve;
	};
}

void CDrawCurve::vMoveAllReset(void)
{
	struct STRN_CURVEPROPERTY *curptr;

	curptr = m_pStrAllCurve;
	while(curptr!=NULL)
	{
		curptr->m_fOffsetValue = 0.0;
		curptr = curptr->m_pStrNextCurve;
	};
}

void CDrawCurve::vMoveAllUp(void)
{
	struct STRN_CURVEPROPERTY *curptr;

	curptr = m_pStrAllCurve;
	while(curptr!=NULL)
	{
		if( curptr->m_fOffsetValue + (curptr->m_fMaxValue - curptr->m_fMinValue)/40.0 < curptr->m_fMaxValue)
			curptr->m_fOffsetValue = curptr->m_fOffsetValue + (curptr->m_fMaxValue - curptr->m_fMinValue)/40.0;
		curptr = curptr->m_pStrNextCurve;
	};
}
*/

void CDrawCurve::vSetAllVisible(bool bvisible)
{
	struct STRN_CURVEPROPERTY *curptr;

	curptr = m_pStrAllCurve;
	while( curptr != NULL )
	{
		curptr->m_bVisible = bvisible;
		curptr = curptr->m_pStrNextCurve;
	};
}
////////////////////////////////////曲线可见性/////////////////////////////////////////
void CDrawCurve::vSetSingleVisible(int iCurveIndex, bool bvisible)
{
	struct STRN_CURVEPROPERTY *curptr = m_pStrAllCurve;

 	if( iCurveIndex >= m_iCurveNum ) return;

	while( (curptr!=NULL) && (curptr->m_iIndex!= iCurveIndex) )
		curptr = curptr->m_pStrNextCurve;

	if( (curptr->m_iIndex == iCurveIndex) && (curptr!=NULL) )
		curptr->m_bVisible = bvisible;
}

bool CDrawCurve::bGetSingleVisible(int iCurveIndex)
{
	struct STRN_CURVEPROPERTY *curptr = m_pStrAllCurve;

 	if( iCurveIndex >= m_iCurveNum ) return false;

	while( (curptr!=NULL) && (curptr->m_iIndex!= iCurveIndex) )
		curptr = curptr->m_pStrNextCurve;

	if( (curptr->m_iIndex == iCurveIndex) && (curptr!=NULL) )
		return(curptr->m_bVisible);
	else return false;
}
////////////////////////////////////曲线颜色///////////////////////////////////

void CDrawCurve::vSetSingleColor(int iCurveIndex, COLORREF color)
{
	struct STRN_CURVEPROPERTY *curptr = m_pStrAllCurve;;

 	if( iCurveIndex >= m_iCurveNum ) return;

	while( (curptr!=NULL) && (curptr->m_iIndex!= iCurveIndex) )
		curptr = curptr->m_pStrNextCurve;

	if( (curptr->m_iIndex == iCurveIndex) && (curptr!=NULL) )
		curptr->m_iColor = color;
}

int CDrawCurve::iGetSingleColor(int iCurveIndex)
{
	struct STRN_CURVEPROPERTY *curptr = m_pStrAllCurve;;

 	if( iCurveIndex >= m_iCurveNum ) return 0;

	while( (curptr!=NULL) && (curptr->m_iIndex!= iCurveIndex) )
		curptr = curptr->m_pStrNextCurve;

	if( (curptr->m_iIndex == iCurveIndex) && (curptr!=NULL) )
		return(curptr->m_iColor);
	else return 0;
}
/////////////////////////////////曲线最大值///////////////////////////////////////

void CDrawCurve::vSetSingleMax(int iCurveIndex, double fmax)
{
	struct STRN_CURVEPROPERTY *curptr = m_pStrAllCurve;;

 	if( iCurveIndex >= m_iCurveNum ) return;

	while( (curptr!=NULL) && (curptr->m_iIndex!= iCurveIndex) )
		curptr = curptr->m_pStrNextCurve;

	if( (curptr->m_iIndex == iCurveIndex) && (curptr!=NULL) )
		curptr->m_fMaxValue = fmax;
}


double CDrawCurve::fGetSingleMax(int iCurveIndex)
{
	struct STRN_CURVEPROPERTY *curptr = m_pStrAllCurve;;

 	if( iCurveIndex >= m_iCurveNum ) return 0.0;

	while( (curptr!=NULL) && (curptr->m_iIndex!= iCurveIndex) )
		curptr = curptr->m_pStrNextCurve;

	if( (curptr->m_iIndex == iCurveIndex) && (curptr!=NULL) )
		return(curptr->m_fMaxValue);
	else return 0;
}
/////////////////////////////////////曲线最小值///////////////////////////////////////
void CDrawCurve::vSetSingleMin(int iCurveIndex, double fmin)
{
	struct STRN_CURVEPROPERTY *curptr = m_pStrAllCurve;;

 	if( iCurveIndex >= m_iCurveNum ) return;

	while( (curptr!=NULL) && (curptr->m_iIndex!= iCurveIndex) )
		curptr = curptr->m_pStrNextCurve;

	if( (curptr->m_iIndex == iCurveIndex) && (curptr!=NULL) )
		curptr->m_fMinValue = fmin;
}


double CDrawCurve::fGetSingleMin(int iCurveIndex)
{
	struct STRN_CURVEPROPERTY *curptr = m_pStrAllCurve;;

 	if( iCurveIndex >= m_iCurveNum ) return 0.0;

	while( (curptr!=NULL) && (curptr->m_iIndex!= iCurveIndex) )
		curptr = curptr->m_pStrNextCurve;

	if( (curptr->m_iIndex == iCurveIndex) && (curptr!=NULL) )
		return(curptr->m_fMinValue);
	else return 0;
}
////////////////////////////////////////////////////////////////////////
void CDrawCurve::vClearAllData(double fasdata)
{
	int idx;
	double *pdata;
	struct STRN_CURVEPROPERTY *curptr;

	curptr = m_pStrAllCurve;
	while( curptr != NULL )
	{
		pdata = curptr->m_fValue;
		for(idx=0; idx<m_iBufLen; idx++)
			pdata[idx] = fasdata;
		curptr = curptr->m_pStrNextCurve;
	};
}

bool CDrawCurve::bIsSingleVisible(int iCurveIndex)
{
	struct STRN_CURVEPROPERTY *curptr = m_pStrAllCurve;

 	if( iCurveIndex >= m_iCurveNum ) return false;

	while( (curptr!=NULL) && (curptr->m_iIndex!= iCurveIndex) )
		curptr = curptr->m_pStrNextCurve;

	if( (curptr->m_iIndex == iCurveIndex) && (curptr!=NULL) )
		return(curptr->m_bVisible);	
	else return false;
}

void CDrawCurve::vDelteAllCurve(void)
{
	struct STRN_CURVEPROPERTY *curptr,*nextptr,*lastptr;

	if(m_pStrAllCurve!=NULL)
	{//起始节点存在
		curptr = m_pStrAllCurve;
		nextptr = m_pStrAllCurve->m_pStrNextCurve;

		while(nextptr!=NULL)
		{//从起始节点开始,搜索最后一个节点
			curptr = nextptr;
			nextptr = nextptr->m_pStrNextCurve;
		}

		lastptr = curptr->m_pStrLastCurve;
		while(lastptr!=NULL)
		{//从最后一个节点开始，依次向回删除节点至起始节点
			delete curptr;
			curptr = lastptr;
			lastptr = lastptr->m_pStrLastCurve;
		}
		delete curptr;
		m_iCurveNum = 0;
		m_pStrAllCurve=NULL;
	}
}

int CDrawCurve::iInsert(int iIndex, struct STRN_CURVEPROPERTY *strCurve)
{
	struct STRN_CURVEPROPERTY *newptr, *curptr, *nextptr;
/*
	COLORREF wSysColors[ ] = 
	{
		RGB(255, 255,   0),
		RGB(128, 255, 255),
		RGB(255,   0,   0),
		RGB(0,   255,   0),

		RGB(255, 128,   0),
		RGB(0,   255,   0),
		RGB(0,   255, 255),
		RGB(0,   255, 128),
		RGB(255, 0,   255),
		RGB(128, 255, 128),
	};
*/
	int r,g,b;
//	r = (m_iCurveNum/25)%5;
//	g = (m_iCurveNum%25)/5;
//	b = m_iCurveNum%5;
	r = rand()%5;
	g = rand()%5;
	b = rand()%5;

	r=r*64; if(r==256) r=r-1;
	g=g*64; if(g==256) g=g-1;
	b=b*64; if(b==256) b=b-1;

	newptr = (struct STRN_CURVEPROPERTY *)malloc(sizeof(struct STRN_CURVEPROPERTY));

	if(newptr!=NULL)
	{
		strCurve->m_iIndex = iIndex;
		strCurve->m_iColor = RGB(r, g, b);//wSysColors[m_iColorIndex];
		m_iColorIndex++; m_iColorIndex %= 10;

		if(m_pStrAllCurve==NULL)
		{
			strCurve->m_pStrLastCurve = NULL;
			strCurve->m_pStrNextCurve = NULL;
			memcpy(newptr,strCurve,sizeof(struct STRN_CURVEPROPERTY));
			m_pStrAllCurve = newptr;
		}
		else
		{
			curptr = m_pStrAllCurve;
			nextptr = m_pStrAllCurve->m_pStrNextCurve;
			while(nextptr!=NULL)
			{
				curptr = nextptr;
				nextptr = nextptr->m_pStrNextCurve;
			}

			strCurve->m_pStrLastCurve = curptr;
			strCurve->m_pStrNextCurve = NULL;
			memcpy(newptr,strCurve,sizeof(struct STRN_CURVEPROPERTY));			
			curptr->m_pStrNextCurve = newptr;
		}
		m_iCurveNum++;
		return m_iCurveNum-1;
	}
	else
		return 0;
}

int CDrawCurve::iStrLen(char *cStringChars)
{
	int idx = 0;
	while(cStringChars[idx]!='\0') idx++;
	return idx;
}

void CDrawCurve::vDrawAll(HDC hMemDC, int iFromPos, int iToPos)
{
	//from 画在最右边，一般代表最新数据（不画该点）， to画在最左边，一般代表早前数据(必须画点)

	bool brst;
	int x, y;
	int idx, iPosIdx, iPointLen;
	double fratex, fratey;
	
	CString stmp;
	HPEN holdpen, hCurvePen;
	int  iSelColor = 0;

	struct STRN_CURVEPROPERTY *curptr;

	//from 画在最右边，一般代表最新数据（不画该点）， to画在最左边，一般代表早前数据(必须画点)
	iPointLen = iFromPos - iToPos;
	while(iPointLen<0) iPointLen += m_iBufLen;
	while(iPointLen>=m_iBufLen) iPointLen -= m_iBufLen;
	if(iPointLen==0) return;

	m_iCurInfoX = m_iWinGapX + 4;
	m_iCurInfoY = m_fWinLenY - m_iWinGapY - 20;

	curptr = m_pStrAllCurve;
	while(curptr!=NULL)
	{
		if(curptr->m_bVisible)
		{
			brst = true;

			fratex = (double)(m_fWinLenX-2*m_iWinGapX)/(double)iPointLen;
			fratey = (double)(m_fWinLenY-2*m_iWinGapY)/(curptr->m_fMaxValue - curptr->m_fMinValue);

			hCurvePen = CreatePen(PS_SOLID, 1, curptr->m_iColor);
			holdpen = (HPEN)SelectObject(hMemDC,hCurvePen);

			SetBkMode(hMemDC,TRANSPARENT);
			SetTextColor(hMemDC,curptr->m_iColor);
			stmp.Format(_T("%s[%6.2f]"),curptr->m_cTipCont,(curptr->m_fMaxValue - curptr->m_fMinValue)/10);
			if( m_iCurInfoX + iStrLen(curptr->m_cTipCont) * 10 + 72 > m_fWinLenX-m_iWinGapX )
			{	m_iCurInfoX = m_iWinGapX + 4; m_iCurInfoY = m_iCurInfoY - 18; }
			TextOut(hMemDC,m_iCurInfoX, m_iCurInfoY, stmp, stmp.GetLength());
			m_iCurInfoX += iStrLen(curptr->m_cTipCont) * 10 + 56;

			for(idx = 1; idx<=iPointLen; idx++ )//不画idx=0
			{//from 画在最右边，一般代表最新数据（不画该点）， to画在最左边，一般代表早前数据(必须画点)
				iPosIdx = (iFromPos - idx + m_iBufLen) % m_iBufLen;
				x = m_fWinLenX - ( m_iWinGapX + (int)(fratex * idx) );
				y = m_fWinLenY - ( m_iWinGapY + (int)(fratey * (curptr->m_fValue[iPosIdx] - curptr->m_fMinValue)) );
				if( (x<m_iWinGapX) || (x>m_fWinLenX-m_iWinGapX) || (y<m_iWinGapY) || (y>m_fWinLenY - m_iWinGapY) )
					brst = true;
				else
				{
					if(curptr->m_bLineTag)
					{
						MoveToEx(hMemDC,x, m_fWinLenY - ( m_iWinGapY + (int)(fratey * (0 - curptr->m_fMinValue)) ),NULL);
						LineTo(hMemDC,x, y);
					}
					else if(brst)
					{
						MoveToEx(hMemDC,x, y, NULL);
						LineTo(hMemDC,x, y);
					}
					else LineTo(hMemDC,x, y);
					brst = false;
				}
			}

			SelectObject(hMemDC,holdpen);
			DeleteObject(hCurvePen);

			iSelColor++;
		}
		curptr = curptr->m_pStrNextCurve;
	}
	return;
}

//char cxflag[10]="//-\\|";
//int  ittt = 0;
void CDrawCurve::vRefreshDynamic(int iFromPos, int iToPos)
{
	//from 画在最右边，一般代表最新数据（不画该点）， to画在最左边，一般代表早前数据(必须画点)

	if(m_iBufLen<1) return;

	CRect RectPicWin;
	m_PicCtrl->GetClientRect(&RectPicWin);
	RectPicWin.NormalizeRect();

	m_fWinLenX = RectPicWin.Width();  m_fWinLenY = RectPicWin.Height();
	m_iWinGapX = 5;  m_iWinGapY = 5;
	m_iCurInfoX = m_iWinGapX + 4;  m_iCurInfoY = m_fWinLenY - m_iWinGapY - 20;

	CDC *pDCD;
	HDC hPicDC, hMemDC;
	HBITMAP newbmp, oldbmp;

	pDCD = m_PicCtrl->GetDC();
	hPicDC = pDCD->m_hDC;
	hMemDC = CreateCompatibleDC(hPicDC);
	newbmp = CreateCompatibleBitmap(hPicDC,m_fWinLenX,m_fWinLenY);
	oldbmp = (HBITMAP)SelectObject(hMemDC,newbmp);
///////////////////////////////////////////////////////////////////////////////
	HPEN hGridPen, hCoordinatePen, hZeroPen;
	HGDIOBJ holdpen;
	int x,y,midx1,midy1,midx2,midy2;
	CString stmp;
	int idx;

	hCoordinatePen = CreatePen(PS_SOLID, 1, RGB(0,64,0));
	//hCoordinatePen = CreatePen(PS_DOT, 1, RGB(0,48,0));
	holdpen = (HPEN)SelectObject(hMemDC,hCoordinatePen);
	//水平线粗格线
	for(idx = 0; idx < 11; idx ++)
	{
		x = m_iWinGapX;
		y = int(m_iWinGapY + (double)(m_fWinLenY-2*m_iWinGapY)/10.0*idx);
		MoveToEx(hMemDC,x,y,NULL);
		if(idx==5){ midx1 = x;   midy1  = y; }
		x = m_fWinLenX - m_iWinGapX;
		LineTo(hMemDC,x,y);
		if(idx==5){ midx2 = x;   midy2  = y; }
	}
	//垂直线粗格线
	for(idx = 0; idx < 11; idx ++)
	{
		x = int(m_iWinGapX + (double)(m_fWinLenX-2*m_iWinGapX)/10.0*idx);
		y = m_iWinGapY;
		MoveToEx(hMemDC,x,y,NULL);
		y = m_fWinLenY - m_iWinGapY;
		LineTo(hMemDC,x,y);
	}
	SelectObject(hMemDC,holdpen);
	DeleteObject(hCoordinatePen);

	//零位粗线
	hZeroPen = CreatePen(PS_DOT, 2, RGB(128,128,128));
	holdpen = (HPEN)SelectObject(hMemDC,hZeroPen);
	MoveToEx(hMemDC,midx1,midy1,NULL);
	LineTo(hMemDC,midx2,midy2);
	SelectObject(hMemDC,holdpen);
	DeleteObject(hZeroPen);

	//初始化第一个信息位置
	m_iCurInfoX = m_iWinGapX + 4;
	m_iCurInfoY = m_fWinLenY - m_iWinGapY - 20;
	
	if(m_bMiniGrid)
	{
		hGridPen = CreatePen(PS_SOLID, 1, RGB(0,32,0));
		holdpen = (HPEN)SelectObject(hMemDC,hGridPen);

		//水平细格线
		for(idx = 0; idx < 51; idx ++)
		{
			if(idx%5!=0)
			{
			x = m_iWinGapX;
			y = int(m_iWinGapY + (double)(m_fWinLenY-2.0*m_iWinGapY)/50.0*idx);
			MoveToEx(hMemDC,x,y,NULL);
			x = m_fWinLenX - m_iWinGapX;
			LineTo(hMemDC,x,y);
			}
		}
		//垂直线细格线
		for(idx = 0; idx < 51; idx ++)
		{
			if(idx%5!=0)
			{
			x = int(m_iWinGapX + (double)(m_fWinLenX-2.0*m_iWinGapX)/50.0*idx);
			y = m_iWinGapY;
			MoveToEx(hMemDC,x,y,NULL);
			y = m_fWinLenY - m_iWinGapY;
			LineTo(hMemDC,x,y);
			}
		}
		SelectObject(hMemDC,holdpen);
		DeleteObject(hGridPen);
	}

	//画所有曲线
	vDrawAll(hMemDC, iFromPos, iToPos);

	double ftmp = ((iFromPos-iToPos+m_iBufLen)%m_iBufLen+1)*m_fSampleTime/10.0;
	if(ftmp<0.001) stmp.Format(_T("%3.2f us"),ftmp*1000000);
	else if(ftmp<1) stmp.Format(_T("%3.2f ms"),ftmp*1000);
	else stmp.Format(_T("%3.2f s"), ftmp);

	SetBkMode(hMemDC,TRANSPARENT);
	SetTextColor(hMemDC,RGB(0,255,0));
	TextOut(hMemDC,m_fWinLenX-100, 8, stmp, stmp.GetLength());

///////////////////////////////////////////////////////////////////////////////
	BitBlt(hPicDC,0, 0, m_fWinLenX, m_fWinLenY, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC,oldbmp);
	DeleteObject(newbmp);
	DeleteObject(oldbmp);
	DeleteDC(hMemDC);
	ReleaseDC(m_PicCtrl->m_hWnd,hPicDC);
}

void CDrawCurve::vRefreshStatic(void)
{
	vRefreshDynamic(m_iBufLen-1,0);
}

void CDrawCurve::vDrawXY(HDC hMemDC, int icvx, int icvy, int iFromPos, int iToPos)
{//from 画在最右边，一般代表最新数据（不画该点）， to画在最左边，一般代表早前数据(必须画点)
//	callcnt = 0;
	bool brst = false, bfoundx, bfoundy;
	int x, y, idx, iPosIdx, iPointLen;
	double fratex, fratey;
	
	CString stmp;
	
	struct STRN_CURVEPROPERTY *curptr;
	struct STRN_CURVEPROPERTY *ptrx = 0, *ptry = 0;

	//from 画在最右边，一般代表最新数据， to画在最左边，一般代表早前数据
	HPEN hOldpen, hCurvePen;
	HBRUSH hOldBrush, hNewBrush;
	
	iPointLen = iFromPos - iToPos;
	while(iPointLen<0) iPointLen += m_iBufLen;
	while(iPointLen>=m_iBufLen) iPointLen -= m_iBufLen;
	if(iPointLen==0) return;

	curptr = m_pStrAllCurve; bfoundx = false;
	while( (curptr!=NULL) && (curptr->m_iIndex!= icvx) ) curptr = curptr->m_pStrNextCurve;
	if( (curptr->m_iIndex == icvx) && (curptr!=NULL) ) { ptrx = curptr; bfoundx = true; }

	curptr = m_pStrAllCurve; bfoundy = false;
	while( (curptr!=NULL) && (curptr->m_iIndex!= icvy) ) curptr = curptr->m_pStrNextCurve;
	if( (curptr->m_iIndex == icvy) && (curptr!=NULL) ) { ptry = curptr; bfoundy = true; }

	if( (!bfoundx) || (!bfoundy) ) return;

	//显示提示信息
	SetBkMode(hMemDC,TRANSPARENT);

	SetTextColor(hMemDC,ptrx->m_iColor);
	stmp.Format(_T("X: %s[%2.1f]"), ptrx->m_cTipCont, (ptrx->m_fMaxValue - ptrx->m_fMinValue)/10.0);
	TextOut(hMemDC,m_iCurInfoX, m_iCurInfoY, stmp, stmp.GetLength());
	m_iCurInfoY = m_iCurInfoY + 18;

	SetTextColor(hMemDC,ptrx->m_iColor);
	stmp.Format(_T("Y: %s[%2.1f]"), ptry->m_cTipCont, (ptry->m_fMaxValue - ptry->m_fMinValue)/10.0);
	TextOut(hMemDC,m_iCurInfoX, m_iCurInfoY, stmp, stmp.GetLength());
	m_iCurInfoY = m_iCurInfoY + 24;

	hCurvePen = CreatePen(PS_SOLID, 1, ptrx->m_iColor);//RGB(255,0,0));
	hOldpen = (HPEN)SelectObject(hMemDC,hCurvePen);

	//画位置圆
	fratey = (double)(m_fWinLenY-2*m_iWinGapY)/(ptry->m_fMaxValue - ptry->m_fMinValue);
	fratex = fratey;
	int irad = m_iCallCnt % 5 + 2;
	for(idx = 0; idx<iPointLen; idx++ )
	{
		if(idx==0)
		{
			hNewBrush = CreateSolidBrush(ptry->m_iColor);//RGB(0,255,0));
			hOldBrush = (HBRUSH)SelectObject(hMemDC, hNewBrush);
			iPosIdx = (iFromPos - idx + m_iBufLen) % m_iBufLen;
			x = m_fWinLenX/2  + (int)(fratex * (ptrx->m_fValue[iPosIdx]));// - ptrx->m_fMinValue));
			y = m_fWinLenY/2  + (int)(fratey * (ptry->m_fValue[iPosIdx]));// - ptry->m_fMinValue));
			Ellipse(hMemDC,x-irad,y-irad,x+irad,y+irad);
			SelectObject(hMemDC,hOldBrush);
			DeleteObject(hNewBrush);
		}
		else
		{
			//hNewBrush = CreateSolidBrush(RGB(255,0,255));
			hNewBrush = CreateSolidBrush(ptrx->m_iColor);//RGB(0,255,0));
			hOldBrush = (HBRUSH)SelectObject(hMemDC, hNewBrush);
			iPosIdx = (iFromPos - idx + m_iBufLen) % m_iBufLen;
			x = m_fWinLenX/2  + (int)(fratex * (ptrx->m_fValue[iPosIdx]));// - ptrx->m_fMinValue));
			y = m_fWinLenY/2  + (int)(fratey * (ptry->m_fValue[iPosIdx]));// - ptry->m_fMinValue));
			Ellipse(hMemDC,x-2,y-2,x+2,y+2);
			SelectObject(hMemDC,hOldBrush);
			DeleteObject(hNewBrush);
		}
	}
	m_iCallCnt++;

	SelectObject(hMemDC,hOldpen);
	DeleteObject(hCurvePen);

	return;
}

void CDrawCurve::vRefreshXY(int icvx0, int icvy0, int icvx1, int icvy1, int icvx2, int icvy2, int iFromPos, int iToPos)
{//from 画在最右边，一般代表最新数据（不画该点）， to画在最左边，一般代表早前数据(必须画点)
	if(m_iBufLen<1) return;

	CRect RectPicWin;
	m_PicCtrl->GetClientRect(&RectPicWin);
	RectPicWin.NormalizeRect();

	m_fWinLenX = RectPicWin.Width();  m_fWinLenY = RectPicWin.Height();
	m_iWinGapX = 4;  m_iWinGapY = 4;

	CDC *pDCD;
	HDC hPicDC, hMemDC;
	HBITMAP newbmp, oldbmp;

	pDCD = m_PicCtrl->GetDC();
	hPicDC = pDCD->m_hDC;
	hMemDC = CreateCompatibleDC(hPicDC);
	newbmp = CreateCompatibleBitmap(hPicDC,m_fWinLenX,m_fWinLenY);
	oldbmp = (HBITMAP)SelectObject(hMemDC,newbmp);
///////////////////////////////////////////////////////////////////////////////
	HPEN hGridPen, hCoordinatePen, hZeroPen;
	HGDIOBJ holdpen;
	int idx,x,y;
	CString stmp;
	
	//零位粗线
	hZeroPen = CreatePen(PS_DOT, 2, RGB(128,128,128));
	holdpen = (HPEN)SelectObject(hMemDC,hZeroPen);

	MoveToEx(hMemDC, m_iWinGapX, m_fWinLenY/2, NULL);
	LineTo(hMemDC, m_fWinLenX-m_iWinGapX, m_fWinLenY/2);

	MoveToEx(hMemDC, m_fWinLenX/2, m_iWinGapY, NULL);
	LineTo(hMemDC, m_fWinLenX/2, m_fWinLenY-m_iWinGapY);

	SelectObject(hMemDC,holdpen);
	DeleteObject(hZeroPen);

	hCoordinatePen = CreatePen(PS_SOLID, 1, RGB(0,64,0));
	holdpen = (HPEN)SelectObject(hMemDC,hCoordinatePen);
	//水平线粗格线
	for(idx = 0; idx < 11; idx ++)
	{
		x = m_iWinGapX;
		y = int(m_iWinGapY + (double)(m_fWinLenY-2*m_iWinGapY)/10.0*idx);
		MoveToEx(hMemDC,x,y,NULL);
		x = m_fWinLenX - m_iWinGapX;
		LineTo(hMemDC,x,y);
	}
	//垂直线粗格线
	for(idx = 0; idx < 11; idx ++)
	{
		x = (int) ( (double)(m_fWinLenX/2.0) + (double)((m_fWinLenY-2*m_iWinGapY)/10.0*idx) );
		y = m_iWinGapY;
		MoveToEx(hMemDC,x,y,NULL);
		y = m_fWinLenY - m_iWinGapY;
		LineTo(hMemDC,x,y);

		x = (int) ( (double)(m_fWinLenX/2.0) - (double)((m_fWinLenY-2*m_iWinGapY)/10.0*idx) );
		y = m_iWinGapY;
		MoveToEx(hMemDC,x,y,NULL);
		y = m_fWinLenY - m_iWinGapY;
		LineTo(hMemDC,x,y);
	}
	SelectObject(hMemDC,holdpen);
	DeleteObject(hCoordinatePen);

	if(m_bMiniGrid)
	{
		hGridPen = CreatePen(PS_SOLID, 1, RGB(0,32,0));
		holdpen = (HPEN)SelectObject(hMemDC,hGridPen);
		//水平细格线
		for(idx = 0; idx < 51; idx ++)
		{
			if(idx%5!=0)
			{
				x = m_iWinGapX;
				y = int(m_iWinGapY + (double)(m_fWinLenY-2.0*m_iWinGapY)/50.0*idx);
				MoveToEx(hMemDC,x,y,NULL);
				x = m_fWinLenX - m_iWinGapX;
				LineTo(hMemDC,x,y);
			}
		}
		//垂直线细格线
		for(idx = 0; idx < 51; idx ++)
		{
			if(idx%5!=0)
			{
				x = (int) ( (double)(m_fWinLenX/2.0) + (double)((m_fWinLenY-2*m_iWinGapY)/50.0*idx) );
				y = m_iWinGapY;
				MoveToEx(hMemDC,x,y,NULL);
				y = m_fWinLenY - m_iWinGapY;
				LineTo(hMemDC,x,y);
				x = (int) ( (double)(m_fWinLenX/2.0) - (double)((m_fWinLenY-2*m_iWinGapY)/50.0*idx) );
				y = m_iWinGapY;
				MoveToEx(hMemDC,x,y,NULL);
				y = m_fWinLenY - m_iWinGapY;
				LineTo(hMemDC,x,y);
			}
		}
		SelectObject(hMemDC,holdpen);
		DeleteObject(hGridPen);
	}
	
	m_iCurInfoX = m_iWinGapX + 2;  m_iCurInfoY = m_iWinGapY + 2;

	vDrawXY(hMemDC, icvx0, icvy0, iFromPos, iToPos);
	vDrawXY(hMemDC, icvx1, icvy1, iFromPos, iToPos);
	vDrawXY(hMemDC, icvx2, icvy2, iFromPos, iToPos);
///////////////////////////////////////////////////////////////////////////////
	BitBlt(hPicDC,0, 0, m_fWinLenX, m_fWinLenY, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC,oldbmp);
	DeleteObject(newbmp);
	DeleteObject(oldbmp);
	DeleteDC(hMemDC);
	ReleaseDC(m_PicCtrl->m_hWnd,hPicDC);
}
