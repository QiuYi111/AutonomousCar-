/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
#include "tools.h"
#include "servo.h"
#include "ccd.h"
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
short started = 0;
int speed_L=0;
int speed_R=0;
int camera_pos = 0;
short ERROR_L;
short ERROR_R;
short ERROR_cam;
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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM8_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_DMA_Init();
  MX_TIM7_Init();
  MX_TIM4_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */


  motor_init();
  servo_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int k = 0;
  while (1)
  {
	  k++;
	  if(k%18==0&&started)
	  {

			TRIG_echo();
		  		if(echo_distance<=350 && started)
		  		{
		  			printf("!!!!!!!!!!!!%d\n",echo_distance);
		  			if(target_servo_degree< 80 && mode == 0)
		  			{
		  				mode = 1;
		  			}
		  			if(target_servo_degree> 100 && mode == 0)
		  			{
		  				mode = -1;
		  			}
		  		}
		  HAL_Delay(3);
		  //k = 0;
		  servo_scan();
	  }
	  if(k%36==0 )
	  {
		  if(started)
		  {
			  //printf("Now speed:\n%d,%d\n",speed_L,speed_R);
			  //printf("Now adjust:\n%d,%d\n",diff_speed_L,diff_speed_R);
			  //printf("Now motor:\n%d,%d\n",motor_speed_L,motor_speed_R);
			  //printf("Now target:\n%d,%d\n",target_speed_L,target_speed_R);
			  printf("Now camera:\n%d\nNow count:\n%d\nNow mode:\n%d\nNow target_pos:\n%d\n",camera_pos,count,mode,target_pos);
			  printf("Now result0:\n%d\nNow result1:\n%d\n",result[0],result[1]);
			  printf("///////////////////\n");
		  }
		  k = 0;
	  }
	  camera_take();
	  camera_pos = get_camera_pos();
	  if(!started)
	  {
		  last_pos = 64;
		  mode = 0;
	  }
	  if(Rx_command[0])
	  {
		  printf("now command is:%s\n",Rx_command);
		  excute_command(Rx_command);
		  clear_command();
	  }

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)	// 判断是由哪个串口触发的中�????????
	{
		// 接收到数据马上使用串�????????1发�?�出�????????
		Rx_buffer[Rx_len] = Rx_byte;
		Rx_len++;
		if(Rx_len==RXBUFFER_LEN)
		{
			Rx_len=0;
		}
		Check_Rx();
		HAL_UART_Receive_IT(&huart1,&Rx_byte,1);		// 重新使能串口1接收中断
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t i = 0;

	if(htim->Instance == TIM7 && started)
	{
		P_count = 1;
		short tar_total_speed = 100;
		short diff_rot = PID_cal_rot(target_pos,camera_pos,&ERROR_cam);
		rotation_curvature +=diff_rot;
		if(abs(mode)==1||abs(mode) ==3)
		{
			if(rotation_curvature>MAX_CURV_M)
				rotation_curvature = MAX_CURV_M;
			if(rotation_curvature<-MAX_CURV_M)
				rotation_curvature = -MAX_CURV_M;
		}
		else
		{
			if(rotation_curvature>MAX_CURV)
				rotation_curvature = MAX_CURV;
			if(rotation_curvature<-MAX_CURV)
				rotation_curvature = -MAX_CURV;
		}
		if (rotation_curvature == 0)
			drive(tar_total_speed,0);
		else
		{
			double temp_radius =  10000/(float)rotation_curvature;
			short temp_speed = tar_total_speed-abs(rotation_curvature)/4;
			if(mode!=0)
				temp_speed*=0.8;
			drive(temp_speed*0.6,(short)temp_radius);
		}


		speed_L = motor_read_speed_L();			//获取当前速度
		speed_R = motor_read_speed_R();
		short diff_speed_L = PID_cal_L(target_speed_L,speed_L,&ERROR_L);//当前PWM数�?�加上增量式PID计算�??????????�??????????
		short diff_speed_R = PID_cal_R(target_speed_R,speed_R,&ERROR_R);//当前PWM数�?�加上增量式PID计算�??????????�??????????
		motor_speed_L += diff_speed_L;
		motor_speed_R += diff_speed_R;
		//限制PWM�??????????大范�??????????
		if(motor_speed_L>MAX_SPEED)
			motor_speed_L = MAX_SPEED;
		if(motor_speed_L<-MAX_SPEED)
			motor_speed_L = -MAX_SPEED;
		if(motor_speed_R>MAX_SPEED)
			motor_speed_R = MAX_SPEED;
		if(motor_speed_R<-MAX_SPEED)
			motor_speed_R = -MAX_SPEED;
		if(target_speed_L == 0 && speed_L ==0)
			motor_speed_L = 0;
		if(target_speed_R == 0 && speed_R ==0)
			motor_speed_R = 0;
		//将PID计算后的PWM写入系统
		motor_PWM(motor_speed_L,motor_speed_R);
		//motor_PWM(-50,-50);

//		i++;
//		if(i >= 30)
//		{
//
//		}

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

