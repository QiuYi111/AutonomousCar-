#include "STM32Type.h"
#include "AGVTimer.h"
#include "AGVBeepLed.h"
#include "AGVUart1.h"
#include "AGVUart3.h"
#include "AGVComm.h"
#include "AGVMotor.h"
#include "AGVXunJi.h"
#include "AGVConfig.h"

HBOOL bBeepOn = HTRUE;

int main(void)
{
	vInitBeepLed();
	vInitXunJi();
	vInitMotor();
	vInitUart1(115200);
	vInitUart3(115200);
	vInitConfig();
	vInitSysTimer(100);//us
	
	strDevKVar.DETECTOR.B.bRunDetector = 1;//默认使用真实循迹传感器
	
	while(1)
	{
		if(bGen_GetRxVarFm()==HTRUE)
		{
			vGen_ExpRxVarFm();
			if(cWifiState==ST_WIFI_RTXDATA) XOR_SECLED;
		}		
	
		if(strSysTimer.bTag100ms)
		{//wifi 只能每秒10帧 --- too low
			vGen_EdtTxVarFm();
			if(bIsDirectUart1RxRdy) vDirectUart1_TxVarFm();
			else if(bIsWifiUart3RxRdy && (cWifiState==ST_WIFI_RTXDATA)) vWifiUart3_TxVarFm();
			else;
			strSysTimer.bTag100ms = HFALSE;
		}
		if(strSysTimer.bTag200ms)
		{		
			if(strDevKVar.DETECTOR.B.bRunDetector)
			{//真实传感器工作
				//下面探测必须是有黑线时为1，左边在高位，右边在低位
				if(L1XJ_IN) { strDevKVar.DETECTOR.B.bL1XJ = 1; if(strDevCfg.XJPARA.bL1XJBeepEn) bBeepOn = HTRUE; }
				else          strDevKVar.DETECTOR.B.bL1XJ = 0;

				if(L2XJ_IN) { strDevKVar.DETECTOR.B.bL2XJ = 1; if(strDevCfg.XJPARA.bL2XJBeepEn) bBeepOn = HTRUE; }
				else          strDevKVar.DETECTOR.B.bL2XJ = 0;

				if(R1XJ_IN) { strDevKVar.DETECTOR.B.bR1XJ = 1; if(strDevCfg.XJPARA.bR1XJBeepEn) bBeepOn = HTRUE; }
				else          strDevKVar.DETECTOR.B.bR1XJ = 0;

				if(R2XJ_IN) { strDevKVar.DETECTOR.B.bR2XJ = 1; if(strDevCfg.XJPARA.bR2XJBeepEn) bBeepOn = HTRUE; }
				else          strDevKVar.DETECTOR.B.bR2XJ = 0;
				
				if(bBeepOn==HTRUE)
					{ ON_BEEP;  bBeepOn = HFALSE; }
				else
					{ OFF_BEEP;  }
			}
			switch(strDevKVar.cMainProc)
			{
				case PROC_XUNJI:
					vXunJiProc();
					break;
				
				default:
					break;
			}
			strSysTimer.bTag200ms = HFALSE;
		}		
		
		if(strSysTimer.bTag1s==1)
		{
			vWifiUart3_InitModule();
			strSysTimer.bTag1s = 0;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////
