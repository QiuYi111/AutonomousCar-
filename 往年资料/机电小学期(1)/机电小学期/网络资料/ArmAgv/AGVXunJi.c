#include "STM32Type.h"
#include "stm32f10x_rcc.h"
#include "AGVXunJi.h"
#include "misc.h"
#include "AGVComm.h"
#include "stm32f10x_tim.h"

#include "AGvUart1.h"
#include "AGvUart3.h"
#include "AGVConfig.h"
#include "AGVComm.h"
#include "AGVMotor.h"
#include "AGVBeepLed.h"
#include "AGVTimer.h"
#include "AGVCP180112.h"
#include "AGVGenFun.h"

void vInitXunJi(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(L1XJ_CLK , ENABLE);//使能GPIO时钟	
	GPIO_InitStructure.GPIO_Pin = L1XJ_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO端口速度
	GPIO_Init(L1XJ_GPIO , &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(L2XJ_CLK , ENABLE);//使能GPIO时钟	
	GPIO_InitStructure.GPIO_Pin = L2XJ_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO端口速度
	GPIO_Init(L2XJ_GPIO , &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(R1XJ_CLK , ENABLE);//使能GPIO时钟	
	GPIO_InitStructure.GPIO_Pin = R1XJ_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO端口速度
	GPIO_Init(R1XJ_GPIO , &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(R2XJ_CLK , ENABLE);//使能GPIO时钟	
	GPIO_InitStructure.GPIO_Pin = R2XJ_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//配置GPIO端口速度
	GPIO_Init(R2XJ_GPIO , &GPIO_InitStructure);	
}
	
void vXunJiProc(void)
{
//自主车行走规则
//假定安装了4只传感器，从左到右分别安装于：PE2,4,6,5引脚
//分别对应：左循迹，中间循迹，右循迹，右避障
//在如下叙述中，如果仅用到更少的传感器，其他传感器可以不安装
	switch(strDevCfg.XJPARA.cDectorSel)
	{
		case XJDETECTOR_TWO:
//1) 2只传感器：使用最左边与最右边(PE2,6)，且2只传感器必须布置在所有黑线的两外侧
//   压线模式--右侧传感器压着右侧黑线行走
//   夹线模式--左右2只传感器在两条黑线中间行走
			if( (strDevKVar.DETECTOR.B.bL2XJ==1) && (strDevKVar.DETECTOR.B.bL1XJ==1) && (strDevKVar.DETECTOR.B.bR1XJ==1) && (strDevKVar.DETECTOR.B.bR2XJ==1))//检测到全部黑线
				vGenMotorPwm(0, GO_FAST, 0, TURN_ZERO);//停车
			else if(strDevCfg.XJPARA.bIsStampWire)
			{//压线模式---不能选择2只传感器
				vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//无法判断
			}
			else
			{//夹线模式
				if(strDevKVar.DETECTOR.B.bR2XJ)      vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_SLOW);//右侧传感器压到右黑线
				else if(strDevKVar.DETECTOR.B.bL2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, -strDevCfg.XJPARA.cMaxTurnSpeed, TURN_SLOW);//右侧传感器压到右黑线
				else                                 vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//无法判断
			}
			break;

//2) 3只传感器：使用最左边与最右边与中间(PE2,4,5)，且最左边与最右边2只传感器必须布置在所有黑线的两外侧
//   压线模式--中间传感器压着右侧黑线行走
//   夹线模式--最左边与最右边2只传感器在两条黑线中间行走			
		case XJDETECTOR_THREE:
			if( (strDevKVar.DETECTOR.B.bL2XJ==1) && (strDevKVar.DETECTOR.B.bL1XJ==1) && (strDevKVar.DETECTOR.B.bR1XJ==1) && (strDevKVar.DETECTOR.B.bR2XJ==1))//检测到全部黑线
				vGenMotorPwm(0, GO_FAST, 0, TURN_ZERO);//停车
			else if(strDevCfg.XJPARA.bIsStampWire)
			{//压线模式
				if(strDevKVar.DETECTOR.B.bL1XJ)      vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//中间传感器压到（可能）黑线---直行
				else if(strDevKVar.DETECTOR.B.bL2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_FAST);//左侧传感器压到（可能）右黑线-----右拐弯
				else if(strDevKVar.DETECTOR.B.bR2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, -strDevCfg.XJPARA.cMaxTurnSpeed, TURN_FAST);//右侧传感器压到（可能）右黑线-----左拐弯
				else                                 vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_SLOW, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//无法判断----直行
			}
			else
			{//夹线模式
				if(strDevKVar.DETECTOR.B.bL1XJ)      vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, -strDevCfg.XJPARA.cMaxTurnSpeed, TURN_FAST);//右侧传感器压到右黑线--慢速右转
				else if(strDevKVar.DETECTOR.B.bR2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_SLOW);//右侧传感器压到右黑线--快速左转
				else if(strDevKVar.DETECTOR.B.bL2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, -strDevCfg.XJPARA.cMaxTurnSpeed, TURN_SLOW);//右侧传感器压到右黑线--块速右转
				else                                 vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//无法判断
			}
			break;
			
//3) 4只传感器：使用最左边与最右边与中间(PE2,4,6,5)，且最左边与最右边2只传感器必须布置在所有黑线的两外侧
//   压线模式--中间传感器压着右侧黑线行走
//   夹线模式--最左边与最右边2只传感器在两条黑线中间行走			
		case XJDETECTOR_FOUR:
			if( (strDevKVar.DETECTOR.B.bL2XJ==1) && (strDevKVar.DETECTOR.B.bL1XJ==1) && (strDevKVar.DETECTOR.B.bR1XJ==1) && (strDevKVar.DETECTOR.B.bR2XJ==1))//检测到全部黑线
				vGenMotorPwm(0, GO_FAST, 0, TURN_ZERO);//停车
			else if(strDevCfg.XJPARA.bIsStampWire)
			{//压线模式
				if(strDevKVar.DETECTOR.B.bL1XJ)      vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//中间传感器压到（可能）黑线---直行
				else if(strDevKVar.DETECTOR.B.bR1XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//中间传感器压到（可能）黑线---直行
				else if(strDevKVar.DETECTOR.B.bL2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_FAST);//左侧传感器压到（可能）右黑线-----右拐弯
				else if(strDevKVar.DETECTOR.B.bR2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, -strDevCfg.XJPARA.cMaxTurnSpeed, TURN_FAST);//右侧传感器压到（可能）右黑线-----左拐弯
				else                                 vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_SLOW, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//无法判断----直行
			}
			else
			{//夹线模式
				if(strDevKVar.DETECTOR.B.bL1XJ)      vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, -strDevCfg.XJPARA.cMaxTurnSpeed, TURN_FAST);//左侧传感器压到右黑线--快速右转
				else if(strDevKVar.DETECTOR.B.bR1XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_FAST);//右侧传感器压到右黑线--快速左转
				else if(strDevKVar.DETECTOR.B.bL2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, -strDevCfg.XJPARA.cMaxTurnSpeed, TURN_SLOW);//左侧传感器压到右黑线--慢速右转
				else if(strDevKVar.DETECTOR.B.bR2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_SLOW);//右侧传感器压到右黑线--慢速左转
				else                                 vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//无法判断----直行
			}
			break;			
			
		default:
			vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);
			break;			
	}
}


