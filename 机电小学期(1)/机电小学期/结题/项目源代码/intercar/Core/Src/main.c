/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
#include "servo.h"
#include "control.h"
#include "sonic.h"
#include "bluetooth.h"
#include "openmv.h"
#include "imu.h"
#include <stdio.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t IS_FIRDT_CAPTURED;
uint8_t USART1_RXbuff;
uint16_t echo1=0;
uint16_t echo2=0;
uint16_t count=0;
int i=0;
UART_HandleTypeDef huart3;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM7_Init();
  MX_TIM8_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  mode=0;
  motor_init();
  servo1_init();
  servo2_init();
  servo4_init();
  is_close1=is_close2=is_close3=is_close4=500;
  is_down=is_groundturn=is_shut=is_stopping=is_fetch=0;
  deviation=deviation_f=turn_target=check_flag=imu_check=0;
  deviation_s=100;
  targetspeed=550;
  lefttarget=righttarget=0;
  curvature=curv_error=0;
  total_distance=theta0=0;
  Rx_command='0';
  avoid_dir=turn_flag=stop_flag=0;
  avoid_comfirm=1;
  IS_FIRDT_CAPTURED=0;
  HAL_TIM_Base_Start(&htim4);
  HAL_TIM_Base_Start(&htim3);
  HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_2);
  HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_3);
  HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_4);
  HAL_UART_Receive_IT(&huart1,(void *)&USART1_RXbuff,1);
  HAL_UART_Receive_IT(&huart4,&Rx_byte,sizeof(Rx_byte));
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM7&&(mode!=0))
	{
		if(mode==1){
			targetspeed=500;
			//spinspeed=300;
		}
		if(mode==2){
			if(is_fetch==0){
				targetspeed=120;
				//spinspeed=100;
			    if(deviation_f==99){spin(1);}
			    else if(deviation_f!=99){
			    	alongside(deviation_f-3);
                    if(deviation_s<=55){
                    	setspeed(0,0);
                    	stop_flag+=1;
                    	if(stop_flag==10){
                    	    fetchplace();
                    	    uint8_t  CO3AskBuffer[3]={0xa0,1,0xa1};
                    	    HAL_UART_Transmit_IT(&huart1, (uint8_t *)CO3AskBuffer, sizeof(CO3AskBuffer));
                    	    for(int i=0;i<18;i++)
                    	    {servo4_turn(42+i);}
                    	    stop_flag=0;
                    	}
                    }
			    }
			}
			else if(is_fetch==1){
				targetspeed=200;
				setspeed(targetspeed,0);
	            turn_flag+=1;
				if(turn_flag>=60){
				     setspeed(0,0);
				     stop_flag+=1;
				     if(stop_flag==10){
				          stop_flag=0;
				          turn_flag=0;
				     }
				}
			}
		}
		if(mode==3){
			targetspeed=460;
			alongside(deviation);
		}
		if(mode==4){
			targetspeed=320;
		    servo3_scan();
		    HAL_UART_Receive(&huart3,IMURxBuffer,22,40);
		    JY901_Process();
            if(is_groundturn==0&&is_stopping==0){avoidance();}
            else if(is_groundturn!=0&&is_stopping==0){
            	turn_flag+=1;
            	if(turn_flag<turn_target)
            	{spin(is_groundturn);}
            	else if(turn_flag==turn_target){
            		avoid_stop();
            	}
            	else if((turn_flag>turn_target)&&(turn_flag<=turn_target+4)){setspeed(targetspeed,0);}
            	else if(turn_flag>turn_target+4){
            		avoid_stop();
            		is_groundturn=0;
            		turn_flag=0;
            	}
            }
            else if(is_stopping==1){
            	stop_flag+=1;
            	setspeed(0,0);
            	if(stop_flag==8){
            	    is_stopping=0;
            	    stop_flag=0;
            	}
            }
		}
		if(mode==5){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_RESET);
			targetspeed=150;
			if(is_stopping==0){
			    if(is_fetch==1){
				    if(deviation_f==99){spin(1);}
				    else if(deviation_f!=99){
				    	alongside(deviation_f);
				    	if(deviation_s<=17){
				    		setspeed(0,0);
				    		stop_flag+=1;
				    		if(stop_flag==10){
				    			fetchplace();
				    			is_fetch=0;
				    			stop_flag=0;
				    		}
				    	}
				    }
			    }
			    else if(is_fetch==0){
			    	setspeed(0,0);
			    	mode=0;
			    }
			}
			else if(is_stopping==1){
				stop_flag+=1;
				setspeed(0,0);
				if(stop_flag==20){
				     is_stopping=0;
				     stop_flag=0;
				}
			}
		}
		leftfeedback = motor_leftfeedback();			//获取当前速度
		rightfeedback = motor_rightfeedback();
		int leftdifference = leftspeedPID(lefttarget,leftfeedback,&lefterror);//当前PWM数�?�加上增量式PID计算�???????????????????????????????????????????????�???????????????????????????????????????????????
		int rightdifference = rightspeedPID(righttarget,rightfeedback,&righterror);//当前PWM数�?�加上增量式PID计算�???????????????????????????????????????????????�???????????????????????????????????????????????
		leftspeed += leftdifference;
		rightspeed += rightdifference;
		//限制PWM占空比范围
		if(leftspeed>maxspeed){leftspeed = maxspeed;}
		if(leftspeed<-maxspeed){leftspeed = -maxspeed;}
		if(rightspeed>maxspeed){rightspeed = maxspeed;}
		if(rightspeed<-maxspeed){rightspeed = -maxspeed;}
		if(lefttarget == 0 && leftfeedback ==0){leftspeed = 0;}
		if(righttarget == 0 && rightfeedback ==0){rightspeed = 0;}
		//将PID处理后的目标速度写入系统
		motor_PWM(leftspeed,rightspeed);
	}
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM4){
	if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1)
	{
		if(IS_FIRDT_CAPTURED==0)
		{
			IS_FIRDT_CAPTURED=1;
			echo1=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_1);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim4,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if(IS_FIRDT_CAPTURED==1)
		{
			IS_FIRDT_CAPTURED=0;
			echo2=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_1);
			__HAL_TIM_SET_COUNTER(&htim4,0);
			if(echo1<echo2){count=echo2-echo1;}
			else if(echo1>echo2){count=0xffff-echo1+echo2;}
			distance1=count*0.17;//测得距离单位为mm
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim4,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim4,TIM_IT_CC1);
		}
	}
	if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2)
	{
		if(IS_FIRDT_CAPTURED==0)
		{
			IS_FIRDT_CAPTURED=1;
			echo1=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_2);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim4,TIM_CHANNEL_2,TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if(IS_FIRDT_CAPTURED==1)
		{
			IS_FIRDT_CAPTURED=0;
			echo2=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_2);
			__HAL_TIM_SET_COUNTER(&htim4,0);
			if(echo1<echo2){count=echo2-echo1;}
			else if(echo1>echo2){count=0xffff-echo1+echo2;}
			distance2=count*0.17;//测得距离单位为mm
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim4,TIM_CHANNEL_2,TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim4,TIM_IT_CC2);
		}
	}
	if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_3)
	{
		if(IS_FIRDT_CAPTURED==0)
		{
			IS_FIRDT_CAPTURED=1;
			echo1=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_3);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim4,TIM_CHANNEL_3,TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if(IS_FIRDT_CAPTURED==1)
		{
			IS_FIRDT_CAPTURED=0;
			echo2=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_3);
			__HAL_TIM_SET_COUNTER(&htim4,0);
			if(echo1<echo2){count=echo2-echo1;}
			else if(echo1>echo2){count=0xffff-echo1+echo2;}
			distance3=count*0.17;//测得距离单位为mm
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim4,TIM_CHANNEL_3,TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim4,TIM_IT_CC3);
		}
	}
	if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_4)
	{
		if(IS_FIRDT_CAPTURED==0)
		{
			IS_FIRDT_CAPTURED=1;
			echo1=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_4);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim4,TIM_CHANNEL_4,TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if(IS_FIRDT_CAPTURED==1)
		{
			IS_FIRDT_CAPTURED=0;
			echo2=HAL_TIM_ReadCapturedValue(&htim4,TIM_CHANNEL_4);
			__HAL_TIM_SET_COUNTER(&htim4,0);
			if(echo1<echo2){count=echo2-echo1;}
			else if(echo1>echo2){count=0xffff-echo1+echo2;}
			distance4=count*0.17;//测得距离单位为mm
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim4,TIM_CHANNEL_4,TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim4,TIM_IT_CC4);
		}
	}
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == UART4)
	{
		Rx_buffer[Rx_len] = Rx_byte;
		Rx_len++;
		if(Rx_len==RXBUFFER_LEN)
		{
			extract_RX();
			Rx_len=0;
			mode_switch(Rx_command);
			if(mode==1){remote_control(Rx_command);}
		}
		HAL_UART_Receive_IT(&huart4,&Rx_byte,sizeof(Rx_byte));		// 重新使能串口4接收中断
	}
	if(huart->Instance == USART1)
	{
		uint16_t tempt /*定义临时变量存放接受的数�????????????????*/;
		tempt=USART1_RXbuff;
		Openmv_Receive_Data(tempt);
		extract_MVRX1();
		extract_MVRX2();
		extract_MVRX3();
	    HAL_UART_Receive_IT(&huart1,(void *)&USART1_RXbuff,1);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
