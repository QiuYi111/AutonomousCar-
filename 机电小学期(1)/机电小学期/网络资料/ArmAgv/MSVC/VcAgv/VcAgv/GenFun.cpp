#include "stdafx.h"
#include "genfun.h"
//#include "PD2T_CP130102.H"

//a+rand()%(b-a+1);
//j = (int)(ｎ*rand()/(RAND_MAX+1.0))产生一个0到ｎ之间的随机数
double fRand(double fMin, double fMax)
{
	int iMin = (int)(fMin * 10000.0);
	int iMax = (int)(fMax * 10000.0);
	int iVal = (iMax-iMin) * (rand()%10000-5000) / 10000;
	return (double)iVal/10000.0;
}

//内存块拷贝
void vMemCpy(unsigned char *src, unsigned char *tgt, int ibytes)
{
	for(int idx=0; idx<ibytes; idx++) tgt[idx] = src[idx];
}

//找特定的字符串，然后取出后面的所有数字，遇到非数字或结束符结束
bool bGetNumbersAfterString(CString scrc, CString stgt, CString& sres)
{
		int itgtpos = 0;

		itgtpos = iSearchGetEPos(scrc,stgt);
		if(itgtpos!=-1)
		{
			return (bGetNumbersByPos(scrc,itgtpos+1,sres));
		}
		else return false;
}
//从指定数字位置开始取出所有数字，遇到非数字或结束符结束
bool bGetNumbersByPos(CString scrc, int ipos, CString& sres)
{
	char csrc[256],ctgt[256];
	int  idx;

	strcpy_s(csrc,scrc.GetBuffer());
	while( csrc[ipos]== ' ') ipos++;

	ctgt[0] = '\0';
	ipos; idx = 0;
	do
	{
		ctgt[idx++]  =  csrc[ipos++];
	}while( (csrc[ipos] != '\0')  && ( ((csrc[ipos] >= '0') && (csrc[ipos] <= '9')) || (csrc[ipos] == '.')) );
	ctgt[idx] = '\0';

	sres = (CString)(ctgt);

	if(idx>0) return true;
	else return false;
}

//找特定的字符串，然后取出后面所有字符，遇到结束符结束
bool bSearchGetRightAll(CString scrc, CString stgt, CString& sres)
{
		int itgtpos = 0;

		itgtpos = iSearchGetEPos(scrc,stgt);
		if(itgtpos!=-1)
		{
			bGetRightByPos(scrc,itgtpos+1,sres);
			return true;
		}
		else return false;
}

//找特定的字符串最后一个结束位置
int iSearchGetEPos(CString scrc, CString stgt)
{
	char csrc[256],ctgt[256];
	int  idx,jdx,ipos = 0;
	bool bhead = false, bfound = false;

	strcpy_s(csrc,scrc.GetBuffer());
	strcpy_s(ctgt,stgt.GetBuffer());

	idx = 0; jdx = 0;
	while( (csrc[idx] != '\0') && (!bfound) )
	{
		if(bhead)
		{//已经发现第一个字母
			if(csrc[idx]==ctgt[jdx])
			{
				if(ctgt[jdx+1]=='\0') 
				{
					ipos = idx;
					bfound = true;
				}
				else
				jdx++;
			}
			else
			{
				bhead = false;
				jdx = 0;
			}
		}
		else
		{
			if(csrc[idx]==ctgt[jdx])
			{
				jdx++;
				bhead = true;
			}
		}
		idx++;
	}

	if(bfound) return ipos;
	else return -1;
}
//计算字符串长度
int iGetStringLen(CString scrc)
{
	char csrc[256];
	int  idx=0;

	strcpy_s(csrc,scrc.GetBuffer());
	while( csrc[idx++] != '\0');
	return idx-1;
}
//取出指定位置后面所有字符，遇到结束符结束
bool bGetRightByPos(CString scrc, int ipos, CString& sres)
{
	char csrc[256],ctgt[256];
	int  idx,jdx,ilen = iGetStringLen(scrc);

	ctgt[0] = '\0';
	if( ipos<ilen  )
	{
		strcpy_s(csrc,scrc.GetBuffer());
		idx = ipos; jdx = 0;
		do
		{
			ctgt[jdx++]  =  csrc[idx++];
		}while( csrc[idx] != '\0');
		ctgt[jdx] = '\0';

		sres = (CString)(ctgt);

		return true;
	}
	else
		return false;
}
/*
#define VEL_ACC_500US

#define fM0ACC_C2MM_500us  (1.0/(0.0005*0.0005*strFlashCfg.G0.TPM.unlDisDefCnt.SI32))//cnt/500us/500us to m/s
#define fM0ACC_MM2C500us  (0.0005*0.0005*strFlashCfg.G0.TPM.unlDisDefCnt.SI32)//cnt/500us/500us to m/s
#define fM0VEL_C2MM_500us  (1.0/(0.0005*strFlashCfg.G0.TPM.unlDisDefCnt.SI32))//cnt/500us to m/s
#define fM0VEL_MM2C_500us  (0.0005*strFlashCfg.G0.TPM.unlDisDefCnt.SI32)//cnt/500us to m/s
#define fM0DIS_C2MM_500us  (1.0/strFlashCfg.G0.TPM.unlDisDefCnt.SI32)//cnt to m
#define fM0DIS_MM2C500us  (strFlashCfg.G0.TPM.unlDisDefCnt.SI32)//m to cnt

#define fM0ACC_C2MM_2MS  (1.0/(0.005*0.005*strFlashCfg.G0.TPM.unlDisDefCnt.SI32))//cnt/2ms/2ms to m/s
#define fM0ACC_MM2C_2MS  (0.005*0.005*strFlashCfg.G0.TPM.unlDisDefCnt.SI32)//cnt/2ms/2ms to m/s
#define fM0VEL_C2MM_5ms  (200.0/strFlashCfg.G0.TPM.unlDisDefCnt.SI32)//cnt/2ms to m/s
#define fM0VEL_MM2C_5ms  (strFlashCfg.G0.TPM.unlDisDefCnt.SI32/200.0)//cnt/2ms to m/s
#define fM0DIS_C2MM_5ms  (1.0/strFlashCfg.G0.TPM.unlDisDefCnt.SI32)//cnt to m
#define fM0DIS_MM2C5ms  (strFlashCfg.G0.TPM.unlDisDefCnt.SI32)//m to cnt

#ifdef VEL_ACC_500US
#define fM0ACC_C2MM  (fM0ACC_C2MM_500us*1)
#define fM0ACC_MM2C  (fM0ACC_MM2C500us*1)
#define fM0VEL_C2MM  (fM0VEL_C2MM_500us*1)
#define fM0VEL_MM2C  (fM0VEL_MM2C_500us*1)
#define fM0DIS_C2MM  (fM0DIS_C2MM_500us*1)
#define fM0DIS_MM2C  (fM0DIS_MM2C500us*1)
#endif

#define fANG_DEG2INT (4095.0/90.0)
#define fANG_INT2DEG (90.0/4095.0)

#ifdef VEL_ACC_2MS
#define fM0ACC_C2MM  fM0ACC_C2MM_2MS*1
#define fM0ACC_MM2C  fM0ACC_MM2C_2MS*1
#define fM0VEL_C2MM  fM0VEL_C2MM_5ms*4
#define fM0VEL_MM2C  fM0VEL_MM2C_5ms*4
#define fM0DIS_C2MM  fM0DIS_C2MM_5ms*1
#define fM0DIS_MM2C  fM0DIS_MM2C5ms*1
#endif

#define fM1VEL_C2MMMMM_500us  (1.0/(0.0005*0.0005*strFlashCfg.G1.TPM.unlDisDefCnt.SI32))//cnt/500us/500us to m/s
#define fM1ACC_MM2C500us  (0.0005*0.0005*strFlashCfg.G1.TPM.unlDisDefCnt.SI32)//cnt/500us/500us to m/s
#define fM1DIS_C2MM_500us  (1.0/strFlashCfg.G1.TPM.unlDisDefCnt.SI32)//cnt to m
#define fM1DIS_MM2C500us  (strFlashCfg.G1.TPM.unlDisDefCnt.SI32)//m to cnt

#define fM1VEL_C2MMMMM_2MS  (1.0/(0.005*0.005*strFlashCfg.G1.TPM.unlDisDefCnt.SI32))//cnt/2ms/2ms to m/s
#define fM1ACC_MM2C_2MS  (0.005*0.005*strFlashCfg.G1.TPM.unlDisDefCnt.SI32)//cnt/2ms/2ms to m/s
#define fM1VEL_C2MM_5ms  (200.0/strFlashCfg.G1.TPM.unlDisDefCnt.SI32)//cnt/2ms to m/s
#define fM1VEL_MM2C_5ms  (strFlashCfg.G1.TPM.unlDisDefCnt.SI32/200.0)//cnt/2ms to m/s
#define fM1DIS_C2MM_5ms  (1.0/strFlashCfg.G1.TPM.unlDisDefCnt.SI32)//cnt to m
#define fM1DIS_MM2C5ms  (strFlashCfg.G1.TPM.unlDisDefCnt.SI32)//m to cnt

#ifdef VEL_ACC_500US
#define fM1VEL_C2MMMM  (fM1VEL_C2MMMMM_500us*1)
#define fM1ACC_MM2C  (fM1ACC_MM2C500us*1)
#define fM1VEL_C2MM  (fM1VEL_C2MM_500us*1)
#define fM1VEL_MM2C  (fM1VEL_MM2C_500us*1)
#define fM1DIS_C2MM  (fM1DIS_C2MM_500us*1)
#define fM1DIS_MM2C  (fM1DIS_MM2C500us*1)
#endif

double fM1Vel_CNT2MM(STRN_CFG_X5045 *pStrCfg, double fVelCnt)
{
	return (fVelCnt/(0.0000005*pStrCfg->TPM1.unlDisDefCnt.SI32));
}
double fM1VEL_MM2CNT(STRN_CFG_X5045 *pStrCfg, double fVelCnt)
{
	return (fVelCnt*(0.0000005*pStrCfg->TPM1.unlDisDefCnt.SI32));
}
double fM0Vel_CNT2MM(STRN_CFG_X5045 *pStrCfg, double fVelCnt)
{
	return (fVelCnt/(0.0000005*pStrCfg->TPM0.unlDisDefCnt.SI32));
}
double fM0VEL_MM2CNT(STRN_CFG_X5045 *pStrCfg, double fVelCnt)
{
	return (fVelCnt*(0.0000005*pStrCfg->TPM0.unlDisDefCnt.SI32));
}
double fM0DIS_MM2CNT(STRN_CFG_X5045 *pStrCfg, double fDisCnt)
{
	return fDisCnt*1000.0/pStrCfg->TPM0.unlDisDefCnt.SI32);
}
*/

void vFillBox(CStatic *pPicCtrl, int color)
{
	HPEN holdpen, hCurvePen;

	CRect RectPicWin;
	pPicCtrl->GetClientRect(&RectPicWin);
	RectPicWin.NormalizeRect();

	int m_fWinLenX = RectPicWin.Width();
	int m_fWinLenY = RectPicWin.Height();

	CDC *pDCD;
	HDC hPicDC, hMemDC;
	HBITMAP newbmp, oldbmp;

	pDCD = pPicCtrl->GetDC();
	hPicDC = pDCD->m_hDC;
	hMemDC = CreateCompatibleDC(hPicDC);
	newbmp = CreateCompatibleBitmap(hPicDC,m_fWinLenX,m_fWinLenY);   
	oldbmp = (HBITMAP)SelectObject(hMemDC,newbmp);
///////////////////////////////////////////////////////////////////////////////
	hCurvePen = CreatePen(PS_SOLID, 100, color);
	holdpen = (HPEN)SelectObject(hMemDC,hCurvePen);

	SetBkMode(hMemDC,TRANSPARENT);
	MoveToEx(hMemDC,0,0,NULL);
	LineTo(hMemDC,m_fWinLenX-1,m_fWinLenY-1);
	//Rectangle(hMemDC, 1, 1, m_fWinLenX-2, m_fWinLenY-2);

	SelectObject(hMemDC,holdpen);
	DeleteObject(hCurvePen);
///////////////////////////////////////////////////////////////////////////////
	BitBlt(hPicDC,0, 0, m_fWinLenX, m_fWinLenY, hMemDC, 0, 0, SRCCOPY);

	SelectObject(hMemDC,oldbmp);
	DeleteObject(newbmp);
	DeleteObject(oldbmp);
	DeleteDC(hMemDC);
	ReleaseDC(NULL,hMemDC);
}
