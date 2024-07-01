#include "STM32Type.h"
#include "AGvConfig.h"


STRN_DEV_CFG strDevCfg;

void vInitConfig(void)
{
	strDevCfg.unnDevSN.I.I = DEVSN_DAFAULT;
	
	strDevCfg.RLSCOFF.cLFM_TurnRight = +60;//+0.6
	strDevCfg.RLSCOFF.cLBM_TurnRight = +100;//+1.0
	strDevCfg.RLSCOFF.cRFM_TurnRight = -60;//-0.6
	strDevCfg.RLSCOFF.cRBM_TurnRight = -60;//-0.6
	
	strDevCfg.RLSCOFF.cRFM_TurnLeft = +60;//+0.6
	strDevCfg.RLSCOFF.cRBM_TurnLeft = +100;//+1.0
	strDevCfg.RLSCOFF.cLFM_TurnLeft = -60;//-0.6
	strDevCfg.RLSCOFF.cLBM_TurnLeft = -60;//-0.6
/*	
	strDevCfg.RLSCOFF.cLFM_GoStraight = +100;//-1.0
	strDevCfg.RLSCOFF.cLBM_GoStraight = +100;//-1.0
	strDevCfg.RLSCOFF.cRFM_GoStraight = +100;//-1.0
	strDevCfg.RLSCOFF.cRBM_GoStraight = +100;//-1.0
*/
	strDevCfg.XJPARA.bIsStampWire = 0;	
	strDevCfg.XJPARA.cDectorSel = 2;
	strDevCfg.XJPARA.cMaxGoSpeed = 50;
	strDevCfg.XJPARA.cMaxTurnSpeed = 80;
}
