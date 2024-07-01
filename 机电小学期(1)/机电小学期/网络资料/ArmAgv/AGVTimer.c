//#include "misc.h"
#include "stm32f10x_tim.h"
//#include "stm32f10x_rcc.h"
#include "STM32Type.h"
#include "AGVTimer.h"
#include "AGVMotor.h"
#include "AGVComm.h"
#include "AGVCP180112.h"


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void EXTI9_5_IRQHandler()	 //中断服务函数，函数定义在启动文件里
{
/*	
	if(EXTI_GetITStatus(IRIN_EXITLINE) != RESET)	//红外中断
	{
		EXTI_ClearITPendingBit(IRIN_EXITLINE);//清除中断标志
		IRIntIsr();
	}
	
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)	//超声波中断
	{
		EXTI_ClearITPendingBit(EXTI_Line8);//清除中断标志
	}
*/	
}

/**-------------------------------------------------------
  * @函数名 TIM2_IRQHandler
  * @功能   TIM2中断处理函数，每0.1ms中断一次 
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/

void vInitSysTimer(uint16_t iTimerUs)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;	

    /* TIM2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* Time base configuration */
    //预分频系数，为0时表示不分频
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);//1MHz
    //自动装阅计数值，计是从0开始，周期当量1us(1MHz)
    TIM_TimeBaseStructure.TIM_Period = iTimerUs - 1;//10kHz
	
    // 高级应用本次不涉及。定义在定时器时钟(CK_INT)频率与数字滤波器(ETR,TIx)
    // 使用的采样频率之间的分频比例
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    //向上计数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //初始化定时器5
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* Clear TIM5 update pending flag[清除TIM2溢出中断标志] */
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

    /* TIM IT enable */ //打开溢出中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    /* TIM5 enable counter */
    TIM_Cmd(TIM2, ENABLE);  //计数器使能，开始工作

    /* 中断参数配置 */


    /* Set the Vector Table base address at 0x08000000 */
    //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
    /* Enable the TIMx gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);	
}

UINT8 cTriWaveCnt = 0;
UINT32 lSystemCnt = 0;
volatile STRN_SYS_TIMER strSysTimer = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};

void vSysTimer_TIM2ISR(void)
{
	if( ((TIM2->SR & TIM_IT_Update)!=0) && ((TIM2->DIER & TIM_IT_Update)!=0) )
	{
		TIM2->SR = ~TIM_IT_Update;//clear interrupt source
		lSystemCnt++; lSystemCnt%=1000000;

		//pwm out
		if( (strDevKVar.PWM.cLFM_Pwm>(INT8)(+cTriWaveCnt)) && (strDevKVar.PWM.cLFM_Pwm>0) ) { LFM_RUNFORWARD; }
		else if( (strDevKVar.PWM.cLFM_Pwm<(INT8)(-cTriWaveCnt)) && (strDevKVar.PWM.cLFM_Pwm<0) ) { LFM_RUNBACKWARD; }
		else{ LFM_RUNSTOP; }

		if( (strDevKVar.PWM.cRFM_Pwm>(INT8)(+cTriWaveCnt)) && (strDevKVar.PWM.cRFM_Pwm>0) ) { RFM_RUNFORWARD; }
		else if( (strDevKVar.PWM.cRFM_Pwm<(INT8)(-cTriWaveCnt)) && (strDevKVar.PWM.cRFM_Pwm<0) ) { RFM_RUNBACKWARD; }
		else{ RFM_RUNSTOP; }

		if( (strDevKVar.PWM.cLBM_Pwm>(INT8)(+cTriWaveCnt)) && (strDevKVar.PWM.cLBM_Pwm>0) ) { LBM_RUNFORWARD; }
		else if( (strDevKVar.PWM.cLBM_Pwm<(INT8)(-cTriWaveCnt)) && (strDevKVar.PWM.cLBM_Pwm<0) ) { LBM_RUNBACKWARD; }
		else{ LBM_RUNSTOP; }

		if( (strDevKVar.PWM.cRBM_Pwm>(INT8)(+cTriWaveCnt)) && (strDevKVar.PWM.cRBM_Pwm>0) ) { RBM_RUNFORWARD; }
		else if( (strDevKVar.PWM.cRBM_Pwm<(INT8)(-cTriWaveCnt)) && (strDevKVar.PWM.cRBM_Pwm<0) ) { RBM_RUNBACKWARD; }
		else{ RBM_RUNSTOP; }

		cTriWaveCnt++; cTriWaveCnt %= PWM_MAXTWC;
		
//		strSysTimer.bTag100us = HTRUE;
		if( lSystemCnt%10 == 9 ) strSysTimer.bTag1ms = HTRUE;//----must be kept for Wifi Module Initialization
//		if( lSystemCnt%500 == 499 )  strSysTimer.bTag50ms = HTRUE;
		if( lSystemCnt%1000 == 999 )  strSysTimer.bTag100ms = HTRUE;
		if( lSystemCnt%250 == 249 )  strSysTimer.bTag200ms = HTRUE;
		if( lSystemCnt%10000 == 9999 )  strSysTimer.bTag1s = HTRUE;
	}
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
