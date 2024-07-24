/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include"servo.h"
#include"motor.h"
#include"PID.h"
#include"control.h"
#include"OpenMV.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define open 90
#define close 40
#define up 40
#define down 105

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//int last_speed = 0;

extern uint8_t Rx_OpMV[size];
extern int open_mv_output; //可能的无用参数
extern uint8_t Rx_BtBuffer;
uint8_t fetch_flag = 0; //是否抓取到物体
int direction_adjust_flag = 0; //巡线方向控制分频参数
uint8_t openmv_flag = 0; //巡线方向控制分频参数（是否读取openmv传输的参数）
int find_way_order = 0; //opemmv传入的参数，控制车的运动
int car_speed = 400; //车辆线速度（巡线）
int car_radius = 0; //车辆转弯半径（巡线）
int car_direction = 0; //车辆转弯方向，1左转，-1y
int track_arrive_flag = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void capture(){
	  //__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,96);
	  __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_2,down);
	  printf('down\n');
	  HAL_Delay(500);

	  __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,close);
	  printf('close\n');
	  HAL_Delay(500);
	  __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_2,up);
	  printf('up\n');
}

void laydown(){
	  __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_2,down);
	  HAL_Delay(500);
	  __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,open);
	  HAL_Delay(500);
	  //__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,55);
	  __HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_2,up);
}

int fabs(int a){
	if(a > 0) return a;
	else return (-a);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim5){
		//printf("direction_adjust_flag : %d\n", direction_adjust_flag);
		/*
		if(direction_adjust_flag >= 2){
			openmv_flag = 1;
			direction_adjust_flag = 0;
		}
		else{
		direction_adjust_flag ++;
		}
		*/

		if(find_way_order == 99){
			set_wheel1_speed(0);
			set_wheel2_speed(0);
			track_arrive_flag = 1;
		}
		else if(find_way_order == 0 && track_arrive_flag == 0){
			car_direction = 0;
			set_wheel1_speed(car_speed);
			set_wheel2_speed(car_speed);
		}
		else if(track_arrive_flag == 0){
			if(find_way_order < 0) car_direction = -1;
			else car_direction = 1;
			if(fabs(find_way_order) == 1){
				car_speed = 400;
				car_radius = 450;
			}
			else if(fabs(find_way_order) == 2){
				car_speed = 350;
				car_radius = 300;
			}
			else if(fabs(find_way_order) == 3){
				car_speed = 350;
				car_radius = 200;
			}
			set_car_speed(car_direction, car_speed, car_radius);
		}

		/*
		if(find_way_order == 0){
					car_direction = 0;
					set_wheel1_speed(car_speed);
					set_wheel2_speed(car_speed);
				}
		else{
					if(find_way_order < 0) car_direction = -1;
					else car_direction = 1;
					car_speed = fabs(find_way_order);
					if(car_speed >= 400) car_speed = 400;
					set_car_speed(car_direction, car_speed, fabs(find_way_order));
				}
		*/
		//printf("%d\n", find_way_order);

	}
}


#ifdef __GNUC__									//串口重定�?????????????????????????????????????
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart2 , (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}


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
  MX_DMA_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  MX_TIM5_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_UART_Receive_IT(&huart1,Rx_OpMV,size);
  HAL_UART_Receive_IT(&huart2,&Rx_BtBuffer,1);
  wheel_init();

  servo1Init();
  servo2Init();

  //__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_2,up);
  //__HAL_TIM_SetCompare(&htim8,TIM_CHANNEL_1,open);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /*
	  if(fetch_flag == 1){
		  capture();
	  }
	   */


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
