/*
 * AGV_Timer.h
 *
 *  Created on: 2017-4-12
 *      Author: kesaihu
 */
#ifndef _AGV_TIMER_H_
#define _AGV_TIMER_H_
	
	#include "stm32f10x.h"

	typedef struct
	{
		UINT16 bTag10us : 1;
		UINT16 bTag20us : 1;
		UINT16 bTag50us : 1;
		UINT16 bTag100us : 1;

		UINT16 bTag1ms : 1;
		UINT16 bTag5ms : 1;
		UINT16 bTag20ms : 1;
		UINT16 bTag25ms : 1;

		UINT16 bTag50ms : 1;
		UINT16 bTag100ms : 1;
		UINT16 bTag200ms : 1;
		UINT16 bTag500ms : 1;

		UINT16 bTag1s : 1;
		UINT16 bTag1D1s : 1;
		UINT16 bTag2s : 1;
		UINT16 bTag2D5s : 1;
	}STRN_SYS_TIMER;

	extern UINT8  cTriWaveCnt;
	
	extern volatile STRN_SYS_TIMER strSysTimer;

	extern void vInitSysTimer(UINT16 iTimerUs);
	extern void vSysTimer_TIM2ISR(void);

#endif /* AGVTimer.h */
