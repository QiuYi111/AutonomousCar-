/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "OLED.h"
#include "OLED_ZIKU.h"
#include "PID.h"
#include "HY_SRF05.h"
extern u8 Parameter[2];
extern u8 juli;
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
  * @brief  This function handles NMI exception.
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

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

void USART3_IRQHandler(void)
{

	extern u8 count;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{	
		Parameter[count]=USART_ReceiveData(USART3);
		Parameter[count]=Parameter[count]/16*10+Parameter[count]%16;
		count++;
		if(count>1)
			count=0;
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);	//����жϱ�־λ
	}	 
}
void TIM3_IRQHandler(void)
{
	// ��Ҫ��������źŵ����ڴ��ڶ�ʱ�������ʱʱ����ʱ���ͻ���������������ж�
	// ���ʱ��������Ҫ�������Ķ�ʱ���ڼӵ������źŵ�ʱ������ȥ
	if ( TIM_GetITStatus ( TIM3, TIM_IT_Update) != RESET )               
	{	
		TIM_ICUserValueStructure.Capture_Period ++;		
		TIM_ClearITPendingBit ( TIM3, TIM_FLAG_Update ); 		
	}

	// �����ز����ж�
	if ( TIM_GetITStatus (TIM3, TIM_IT_CC4 ) != RESET)
	{
		// ��һ�β���
		if ( TIM_ICUserValueStructure.Capture_StartFlag == 0 )
		{
			// ��������0
			TIM_SetCounter ( TIM3, 0 );
			// �Զ���װ�ؼĴ������±�־��0
			TIM_ICUserValueStructure.Capture_Period = 0;
      // �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValueStructure.Capture_CcrValue = 0;

			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Falling);
      // ��ʼ�����׼��1			
			TIM_ICUserValueStructure.Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValueStructure.Capture_CcrValue = TIM_GetCapture4(TIM3);

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Rising);
      // ��ʼ�����־��0		
			TIM_ICUserValueStructure.Capture_StartFlag = 0;
      // ������ɱ�־��1			
			TIM_ICUserValueStructure.Capture_FinishFlag = 1;
		}

		TIM_ClearITPendingBit (TIM3,TIM_IT_CC4);	    
	}		
}



void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{		
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);	//����жϱ�־λ
	}	 
}
//void EXTI3_IRQHandler(void)
//{
//	extern u8 Temp;
//	if(EXTI_GetITStatus(EXTI_Line3)!=RESET)//
//	{
//		Temp=1;
//		EXTI_ClearITPendingBit(EXTI_Line3);		//
//	}
//}

//void EXTI4_IRQHandler(void)
//{
//	extern u8 Temp;
//	if(EXTI_GetITStatus(EXTI_Line4)!=RESET)//
//	{
//		Temp=2;
//		EXTI_ClearITPendingBit(EXTI_Line4);		//
//	}
//}

//void EXTI9_5_IRQHandler(void)
//{
//	extern u8 Temp;
//	if(EXTI_GetITStatus(EXTI_Line5)!=RESET)//
//	{
//		Temp=3;
//		EXTI_ClearITPendingBit(EXTI_Line5);		//
//	}
//}

void TIM2_IRQHandler(void)
{
	extern u8 Time;
	if (TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET) //����Ƿ����TIM3�ж�
	{
		Time=1;

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update ); //����жϱ�־
	}		
	

}
void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1,TIM_IT_Update)!= RESET) //����Ƿ����TIM3�ж�
	{
		juli=1;
		TIM_Cmd(TIM1,DISABLE);
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update ); //����жϱ�־
	}		
	

}
/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
