#include "STM32Type.h"
#include "AGvUart1.h"
#include "AGvUart3.h"
#include "AGVConfig.h"
#include "AGVComm.h"
#include "AGVMotor.h"
#include "AGVBeepLed.h"
#include "AGVTimer.h"
#include "AGVCP180112.h"
#include "AGVGenFun.h"

UINT8 cResFmID = RESFMID_GENDEV;//RESFMID_NULL;
UINT8 cCmdFmID = CMDFMID_NULL;
UINT8 cWifiState = ST_WIFI_RST1;
STRN_DEV_KVAR strDevKVar;
UINT8 cCfgTxTimes = 0;

const INT8 *WifiCmd_cRst = "AT+RST";
const INT8 *WifiCmd_cCwmod = "AT+CWMODE=2";
      INT8  WifiCmd_cCwsap[64] = "AT+CWSAP='M&ELAB_XXXX','XXXXXXXXXXXXXXXX',1,4,XX";//AT+CWSAP="M&ELAB_0009","88888888",1,4
const INT8 *WifiCmd_cCIpMux = "AT+CIPMUX=1";
const INT8 *WifiCmd_cCIpServer = "AT+CIPSERVER=1,20181";
const INT8 *WifiCmd_cCIpSto = "AT+CIPSTO=5000";
const INT8 *WifiCmd_cCIfsr = "AT+CIFSR";
const INT8 *WifiCmd_cCIpSend = "AT+CIPSEND=0,";

UINT8 iRTxFmBuf[SIZE_FMBUFLEN];

STRN_FM_GENHEAD            *pCmdGenHead    		= (STRN_FM_GENHEAD *)&iRTxFmBuf[0];
STRN_CMDFM_TSTALT          *pCmdTstAlt     		= (STRN_CMDFM_TSTALT *)&iRTxFmBuf[0];
STRN_CMDFM_GETRESFM        *pCmdGetRes		    = (STRN_CMDFM_GETRESFM *)&iRTxFmBuf[0];
STRN_CMDFM_SIMDETECTOR     *pCmdSimDetector     = (STRN_CMDFM_SIMDETECTOR *)&iRTxFmBuf[0];
STRN_CMDFM_DBGMOTORPWM     *pCmdDbgMotorPwm     = (STRN_CMDFM_DBGMOTORPWM *)&iRTxFmBuf[0];
STRN_CMDFM_DBGDEVSPEED     *pCmdDbgDevSpeed     = (STRN_CMDFM_DBGDEVSPEED *)&iRTxFmBuf[0];
STRN_CMDFM_SETRLSCOFF      *pCmdSetRLSCoff      = (STRN_CMDFM_SETRLSCOFF *)&iRTxFmBuf[0];
STRN_CMDFM_SETXJPARA       *pCmdSetXJPara        = (STRN_CMDFM_SETXJPARA *)&iRTxFmBuf[0];
STRN_CMDFM_XJGO            *pCmdXJGo            = (STRN_CMDFM_XJGO *)&iRTxFmBuf[0];

STRN_RESFM_GENDEV          *pResGenDev          = (STRN_RESFM_GENDEV *)&iRTxFmBuf[0];
STRN_RESFM_DEVKVAR         *pResDevKVar         = (STRN_RESFM_DEVKVAR *)&iRTxFmBuf[0];
STRN_RESFM_DEVCFG          *pResDevCfg          = (STRN_RESFM_DEVCFG *)&iRTxFmBuf[0];

HBOOL bGen_GetRxVarFm(void)
{//get frame from UART receive buffer
	UINT8 idx, cFmCSize, iptr;
	UINT8 ctmp, iByteSum;

	idx = iUSRxIn - iUSRxOut;
	if(iUSRxIn<iUSRxOut) idx += SIZE_US_RXBUF;
	if(idx<SIZE_FM_MIN)	return HFALSE;

	if( ((cUSRxFIFO[iUSRxOut]) != 0xEB) || ((cUSRxFIFO[(iUSRxOut+1)%SIZE_US_RXBUF]) != 0x90))
	{
		iUSRxOut++;  iUSRxOut %= SIZE_US_RXBUF;
		return HFALSE;
	}
	else
	{
		cFmCSize = cUSRxFIFO[(iUSRxOut+2)%SIZE_US_RXBUF] ;
		if(idx<cFmCSize) return HFALSE;

		iByteSum = 0; iptr = iUSRxOut;
		for(idx=0; idx<cFmCSize; idx++)
		{
			ctmp = cUSRxFIFO[iptr];
			iRTxFmBuf[idx] = ctmp;
			iByteSum += ctmp;
			iptr++; iptr %= SIZE_US_RXBUF;
		}
		if(iByteSum == 0X00)
		{
			iUSRxOut += cFmCSize; iUSRxOut %= SIZE_US_RXBUF;
			return HTRUE;
		}
		else
		{
			iUSRxOut += 2; iUSRxOut %= SIZE_US_RXBUF;
			return HFALSE;
		}
	}
}

void vGen_ExpRxVarFm(void)
{
	cCmdFmID = pCmdGenHead->cFmID;

	switch(cCmdFmID)
	{
	case CMDFMID_TSTALT:
		if(pCmdTstAlt->iTstMode)
		{
			cResFmID = RESFMID_GENDEV;
		}
		else
		{
			cResFmID = RESFMID_NULL;
		}
		strDevKVar.cMainProc = PROC_DEBUG;
		break;

	case CMDFMID_GETRESFM:
		cResFmID = pCmdGetRes->cResID; cCfgTxTimes = 5;
		break;
	
	case CMDFMID_SIMDETECTOR:
		strDevKVar.DETECTOR.C.cXJDector = pCmdSimDetector->DETECTOR.C.cXJDector;
		strDevKVar.DETECTOR.B.bRunDetector = 0;//通知设备使用仿真传感器, 真实传感器停止连续检测（循迹启动过程中可以使用仿真传感器）
		cResFmID = RESFMID_DEVKVAR;
		break;
		
	case CMDFMID_DBGMOTORPWM:
		if(strDevKVar.cMainProc==PROC_DEBUG)
		{
			vCopyMemAsUINT8((UINT8 *)&pCmdDbgMotorPwm->PWM, (UINT8 *)&strDevKVar.PWM, sizeof(STRN_MOTOR_PWM));
		}
		strDevKVar.cMainProc = PROC_DEBUG;
		cResFmID = RESFMID_DEVKVAR;
		break;
	
	case CMDFMID_DBGDEVSPEED:
		if(pCmdDbgDevSpeed->cTurnSpeed==0) vGenMotorPwm(pCmdDbgDevSpeed->cGoSpeed, GO_FAST, pCmdDbgDevSpeed->cTurnSpeed, TURN_ZERO);
		else vGenMotorPwm(pCmdDbgDevSpeed->cGoSpeed, GO_FAST, pCmdDbgDevSpeed->cTurnSpeed, TURN_FAST);
		strDevKVar.cMainProc = PROC_DEBUG;
		cResFmID = RESFMID_DEVKVAR;
		break;
		
	case CMDFMID_SETRLSCOFF:
		vCopyMemAsUINT8((UINT8 *)&pCmdSetRLSCoff->RLSCOFF, (UINT8 *)&strDevCfg.RLSCOFF, sizeof(STRN_CFG_RLSCOFF));
		cResFmID = RESFMID_DEVCFG; cCfgTxTimes = 5;
		break;
	
	case CMDFMID_SETXJPARA:
		vCopyMemAsUINT8((UINT8 *)&pCmdSetXJPara->XJPARA, (UINT8 *)&strDevCfg.XJPARA, sizeof(STRN_CFG_XJPARA));
		cResFmID = RESFMID_DEVCFG; cCfgTxTimes = 5;
		break;

	case CMDFMID_XJGO:
		strDevKVar.cMainProc = PROC_XUNJI;
		strDevKVar.DETECTOR.B.bRunDetector = 1;//通知设备不使用仿真传感器, 真实传感器连续检测（循迹启动过程中可以使用仿真传感器）
		cResFmID = RESFMID_DEVKVAR;
		break;	
	
	default:
		cResFmID = RESFMID_GENDEV;
		break;
	}
}

UINT8 iCommSN = 0;
UINT8 iTxFmSize = 0;
void vGen_EdtTxVarFm(void)
{//edit transfer frame
	iCommSN++; iTxFmSize = 0;

	switch(cResFmID)
	{
		case RESFMID_GENDEV:
			pResGenDev->iFmHead    = 0x90EB;
			iTxFmSize              = SIZE_RESFM_GENDEV;
			pResGenDev->cFmBLen    = iTxFmSize;
			pResGenDev->cFmID      = cResFmID;
			pResGenDev->lSoftVer   = VER_SOFTWARE;
			pResGenDev->lHardVer   = VER_HARDWARE;
			pResGenDev->iDevType   = DEVTYPE_MEELAB_AGV;		
			pResGenDev->cFmSN      = iCommSN;
			break;

		case RESFMID_DEVCFG:
			pResDevCfg->iFmHead    = 0x90EB;
			iTxFmSize              = SIZE_RESFM_DEVCFG;
			pResDevCfg->cFmBLen    = iTxFmSize;
			pResDevCfg->cFmID      = cResFmID;
			pResDevCfg->cFmSN      = iCommSN;
			vCopyMemAsUINT8((UINT8 *)&strDevCfg, (UINT8 *)&(pResDevCfg->CFG), sizeof(STRN_DEV_CFG));
			if(cCfgTxTimes) cCfgTxTimes--;
			else cResFmID = RESFMID_DEVKVAR;
			break;
		
		case RESFMID_DEVKVAR:
			pResDevKVar->iFmHead    = 0x90EB;
			iTxFmSize               = SIZE_RESFM_DEVKVAR;
			pResDevKVar->cFmBLen    = iTxFmSize;
			pResDevKVar->cFmID      = cResFmID;
			pResDevKVar->cFmSN      = iCommSN;
			vCopyMemAsUINT8((UINT8 *)&strDevKVar, (UINT8 *)&(pResDevKVar->KVAR), sizeof(STRN_DEV_KVAR));
			break;		
		
		default:
			iTxFmSize = 0;
			break;
	}
}

void vDirectUart1_TxVarFm(void)
{
	UINT8 idx, cchksum;
	HBOOL bIsNotIntStat;
	
	if(iTxFmSize!=0)
	{
		cchksum = 0X00;
		for(idx=0; idx<iTxFmSize-1; idx++)
		{
			cchksum += iRTxFmBuf[idx];
		}
		iRTxFmBuf[iTxFmSize-1] = -cchksum;

		bIsNotIntStat = (iUSTxOut == iUSTxIn);
		for(idx=0; idx<iTxFmSize; idx++)
		{
			cUSTxFIFO[iUSTxIn] = iRTxFmBuf[idx];
			iUSTxIn++; iUSTxIn%=SIZE_US_TXBUF;
		}
		
		if(bIsNotIntStat)
		{
			idx = cUSTxFIFO[iUSTxOut];
			iUSTxOut++; iUSTxOut%=SIZE_US_TXBUF;	
			USART1->DR = idx;
		}
	}
}

void vWifiUart3_TxVarFm(void)
{
	volatile UINT8 idx, cchksum;
	HBOOL  bWifiDgtChar;
	UINT16 iremain;

	if(iTxFmSize==0) return;
	
	idx = 0;
	while( WifiCmd_cCIpSend[idx] != '\0' )
	{
		if(WifiCmd_cCIpSend[idx]=='\'') cUSTxFIFO[iUSTxIn++] = (UINT8)'"';
		else cUSTxFIFO[iUSTxIn++] = WifiCmd_cCIpSend[idx];
		iUSTxIn %= SIZE_US_TXBUF; idx++;
	}

	//translate 'iTxFmSize' to ASCII format
	iremain = iTxFmSize;
	bWifiDgtChar = HFALSE;

	if(iremain>=100)
	{
		bWifiDgtChar = HTRUE;
		cUSTxFIFO[iUSTxIn] = iremain/100 + '0';
		iUSTxIn++; iUSTxIn%=SIZE_US_TXBUF;
	}
	else
	{
		if(bWifiDgtChar==HTRUE)
		{
			cUSTxFIFO[iUSTxIn] = '0';
			iUSTxIn++; iUSTxIn%=SIZE_US_TXBUF;
		}
		//else
	}
	iremain %= 100;

	if(iremain>=10)
	{
		bWifiDgtChar = HTRUE;
		cUSTxFIFO[iUSTxIn] = iremain/10 + '0';
		iUSTxIn++; iUSTxIn%=SIZE_US_TXBUF;
	}
	else
	{
		if(bWifiDgtChar==HTRUE)
		{
			cUSTxFIFO[iUSTxIn] = '0';
			iUSTxIn++; iUSTxIn%=SIZE_US_TXBUF;
		}
		//else
	}
	iremain %= 10;
	cUSTxFIFO[iUSTxIn] = iremain + '0';	
	iUSTxIn++; iUSTxIn%=SIZE_US_TXBUF;
	
	cUSTxFIFO[iUSTxIn++] = 0X0D; iUSTxIn %= SIZE_US_TXBUF;
	cUSTxFIFO[iUSTxIn++] = 0X0A; iUSTxIn %= SIZE_US_TXBUF;

	idx = cUSTxFIFO[iUSTxOut];
	iUSTxOut++; iUSTxOut%=SIZE_US_TXBUF;	
	USART3->DR = idx;
	while(iUSTxIn!=iUSTxOut);

	//wait 2ms for sever is ready
	strSysTimer.bTag1ms = HFALSE; while(strSysTimer.bTag1ms == HFALSE);
	strSysTimer.bTag1ms = HFALSE; while(strSysTimer.bTag1ms == HFALSE);
	strSysTimer.bTag1ms = HFALSE; while(strSysTimer.bTag1ms == HFALSE);
	strSysTimer.bTag1ms = HFALSE; while(strSysTimer.bTag1ms == HFALSE);
	
	cchksum = 0X00;
	for(idx=0; idx<iTxFmSize-1; idx++) cchksum += iRTxFmBuf[idx];
	iRTxFmBuf[iTxFmSize-1] = -cchksum;

	for(idx=0; idx<iTxFmSize; idx++)
	{
		cUSTxFIFO[iUSTxIn] = iRTxFmBuf[idx];
		iUSTxIn++; iUSTxIn%=SIZE_US_TXBUF;
	}
	idx = cUSTxFIFO[iUSTxOut];
	iUSTxOut++; iUSTxOut%=SIZE_US_TXBUF;
	USART3->DR = idx;
}

void vWifiUart3_TxInitCmd(UINT8 *cCmdBuf, HBOOL bReturn)
{
	UINT8 idx;
	HBOOL bIsNotIntStat;
	
	bIsNotIntStat = (iUSTxOut == iUSTxIn);
	idx = 0;
	while( cCmdBuf[idx] != '\0' )
	{
		if(cCmdBuf[idx]=='\'') cUSTxFIFO[iUSTxIn++] = (UINT8)'"';
		else cUSTxFIFO[iUSTxIn++] = cCmdBuf[idx];
		iUSTxIn %= SIZE_US_TXBUF; idx++;
	}
	if(bReturn==HTRUE)
	{
		cUSTxFIFO[iUSTxIn++] = 0X0D; iUSTxIn %= SIZE_US_TXBUF;
		cUSTxFIFO[iUSTxIn++] = 0X0A; iUSTxIn %= SIZE_US_TXBUF;
	}
	if(bIsNotIntStat)
	{
		idx = cUSTxFIFO[iUSTxOut];
		iUSTxOut++; iUSTxOut%=SIZE_US_TXBUF;	
		USART3->DR = idx;
	}	
}

void vWifiUart3_InitModule(void)
{
	UINT16 idx;

	switch(cWifiState)
	{
		case ST_WIFI_NULL:
			break;
		case ST_WIFI_RST1:
			CLR_SECLED;
			vWifiUart3_TxInitCmd((UINT8 *)WifiCmd_cRst,HTRUE);
			cWifiState = ST_WIFI_CWMOD;
			break;
		case ST_WIFI_CWMOD:
			vWifiUart3_TxInitCmd((UINT8 *)WifiCmd_cCwmod,HTRUE);
			cWifiState = ST_WIFI_RST2;
			break;
		case ST_WIFI_RST2:
			vWifiUart3_TxInitCmd((UINT8 *)WifiCmd_cRst,HTRUE);
			cWifiState = ST_WIFI_CWSAP;
			break;
		case ST_WIFI_CWSAP://server name & port---//BCD Code: device serial number
			WifiCmd_cCwsap[17] = strDevCfg.unnDevSN.C.HIGH/16+'0';
			WifiCmd_cCwsap[18] = strDevCfg.unnDevSN.C.HIGH%16+'0';
			WifiCmd_cCwsap[19] = strDevCfg.unnDevSN.C.LOW/16+'0';
			WifiCmd_cCwsap[20] = strDevCfg.unnDevSN.C.LOW%16+'0';
			idx = 0;
			WifiCmd_cCwsap[24+idx] = '1'; idx++;
			WifiCmd_cCwsap[24+idx] = '2'; idx++;
			WifiCmd_cCwsap[24+idx] = '3'; idx++;
			WifiCmd_cCwsap[24+idx] = '4'; idx++;
			WifiCmd_cCwsap[24+idx] = '5'; idx++;
			WifiCmd_cCwsap[24+idx] = '6'; idx++;
			WifiCmd_cCwsap[24+idx] = '7'; idx++;
			WifiCmd_cCwsap[24+idx] = '8'; idx++;
/*			do{
				WifiCmd_cCwsap[24+idx] = '8';
				idx++;
			} while(idx<8);//8 password chars
*/		
			WifiCmd_cCwsap[24+idx] = '\''; idx++;
			WifiCmd_cCwsap[24+idx] = ','; idx++;
			WifiCmd_cCwsap[24+idx] = '1'; idx++;
			WifiCmd_cCwsap[24+idx] = ','; idx++;
			WifiCmd_cCwsap[24+idx] = '4'; idx++;
			WifiCmd_cCwsap[24+idx] = '\0';
			vWifiUart3_TxInitCmd((UINT8 *)WifiCmd_cCwsap,HTRUE);
			cWifiState = ST_WIFI_CIPMUX;
			break;
		case ST_WIFI_CIPMUX:
			vWifiUart3_TxInitCmd((UINT8 *)WifiCmd_cCIpMux,HTRUE);
			cWifiState = ST_WIFI_CIPSEVSER;
			break;
		case ST_WIFI_CIPSEVSER:
			vWifiUart3_TxInitCmd((UINT8 *)WifiCmd_cCIpServer,HTRUE);
			cWifiState = ST_WIFI_CIPSTO;
			break;
		case ST_WIFI_CIPSTO:
			vWifiUart3_TxInitCmd((UINT8 *)WifiCmd_cCIpSto,HTRUE);
			cWifiState = ST_WIFI_CIFSR;
			break;
		case ST_WIFI_CIFSR:
			vWifiUart3_TxInitCmd((UINT8 *)WifiCmd_cCIfsr,HTRUE);
			cWifiState = ST_WIFI_INITEND;
			break;
		case ST_WIFI_INITEND:
			vInitUart3(115200);//reinitialize UART3, or FIFO runs wrong!
			cWifiState = ST_WIFI_RTXDATA;
			SET_SECLED;
			break;
		case ST_WIFI_RTXDATA:
			break;
		default:
			cWifiState = ST_WIFI_RST1;
			break;
	}
}
//===========================================================================
// No more.
