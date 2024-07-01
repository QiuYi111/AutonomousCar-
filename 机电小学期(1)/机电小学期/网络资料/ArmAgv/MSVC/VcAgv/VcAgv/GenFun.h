#ifndef _GENFUN_H_
#define _GENFUN_H_

	extern void vMemCpy(unsigned char *src, unsigned char *tgt, int ibytes);
	extern bool bGetRightByPos(CString scrc, int ipos, CString& sres);
	extern int  iSearchGetEPos(CString scrc, CString stgt);
	extern bool bSearchGetRightAll(CString scrc, CString stgt, CString& sres);
	extern int  iGetStringLen(CString scrc);
	extern bool bGetNumbersAfterString(CString scrc, CString stgt, CString& sres);
	extern bool bGetNumbersByPos(CString scrc, int ipos, CString& sres);
	extern double fRand(double fMax, double fMin);
	void vFillBox(CStatic *pPicCtrl, int color);
#endif
