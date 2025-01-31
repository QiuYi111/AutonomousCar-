/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
#include "ultraSonic.h"
#include "timersInit.h"
#include "string.h"
#include "servo.h"
extern int float_to_uint8_arry(uint8_t* u8Arry, float floatdata, int precision);
extern float uint8_to_float(uint8_t* u8arry, int point_length);
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
float rpmRight=0,rpmLeft=0,deriSpeed=0,centralSpeed;
uint8_t rxDataBT[50];
uint8_t rxDataOp[50];
int countnum_rightcircuance=0;
int countnum_leftcircuance=0;
float currentLeft=0,currentRight=0;
extern int pulseLeft,pulseRight;
extern int ultraLoop;int mode=0;
extern uint8_t cRt[256];
float craw_state=0,is_crawed=0,is_put=0;
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
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM1_Init();
  MX_UART5_Init();
  MX_UART4_Init();
  MX_TIM7_Init();
  MX_TIM3_Init();
  MX_TIM8_Init();
  MX_TIM6_Init();
  MX_TIM10_Init();
  MX_TIM12_Init();
  //MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
  	/*HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_ALL);//�?????????????????启电机PWM，最�?????????????????255
  	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_ALL);//�?????????????????启舵机PWM，最�?????????????????1999
  	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);//左轮编码�?????????????????
  	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);//右轮编码�?????????????????
  	HAL_TIM_Base_Start_IT(&htim7);//中断定时器，50ms�?????????????????次中�?????????????????*/


  //turning_theta_Servo2=Servo2_init(turning_theta_Servo2);

  //turning_theta_Servo2=Servo2_puting(turning_theta_Servo2);

  //turning_theta_Servo1=Servo1_craw(turning_theta_Servo1);

  //turning_theta_Servo2=Servo2_driving(turning_theta_Servo2);



  /*turning_theta_Servo3=Servo3_init(turning_theta_Servo3);*/
  timersInit();
  __HAL_TIM_SET_COUNTER(&htim2,32768);
   __HAL_TIM_SET_COUNTER(&htim4,32768);
   //float turning_theta_Servo1=50;
   //float turning_theta_Servo2=25;
  // float turning_theta_Servo3=55;
   //turning_theta_Servo1=Servo1_init(turning_theta_Servo1);
   //turning_theta_Servo2=Servo2_init(turning_theta_Servo2);

   //craw_up_trail(1);

   //put_down_trail(1);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart5, rxDataBT, sizeof rxDataBT);
  HAL_UARTEx_ReceiveToIdle_DMA(&huart4, rxDataOp, sizeof rxDataOp);
  //Servo_turn(2,60,25);
  //Servo_turn(1,120,25);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  setLeftPWM(pulseLeft,TIM_CHANNEL_3);
	  setRightPWM(pulseRight,TIM_CHANNEL_4);
	  if(craw_state==1){
		  is_crawed=craw_up_trail(1);//if_crow==1时，调用循迹时的程序；if_crow==2时，调用避障时的程序
	  }
	  else if(craw_state==2){
		  //if_crow==1时，调用循迹时的程序；if_crow==2时，调用避障时的程序
		  is_put=put_down_trail(1);
	  }

	  if (is_crawed==1){
		  //setSpin(20, 'F');
		  centralSpeed = 60;
		  rpmLeft=rpmRight=centralSpeed;
		  mode=1;
		  is_crawed=0;

	  }else if (is_put==1){
		  is_put=0;
	  }
	  /*
	  turning_theta_Servo1=Servo1_craw(turning_theta_Servo1);
	  turning_theta_Servo2=Servo2_driving(turning_theta_Servo2);
	  turning_theta_Servo1=Servo1_put(turning_theta_Servo1);
	  turning_theta_Servo2=Servo2_puting(turning_theta_Servo2);                                                                                                                                                                                                                                  	  setLeftPWM(pulseLeft,TIM_CHANNEL_3);
	  setRightPWM(pulseRight,TIM_CHANNEL_4);*/
	  if (ultraLoop==0){
	  	distanceFront=MeasureDistance(&ultraSonicFront);
	  }else if(ultraLoop==1){
	  	 distanceLeft=MeasureDistance(&ultraSonicLeft);
	  }else if (ultraLoop==2){
	  	 distanceRight=MeasureDistance(&ultraSonicRight);
}
	  if (ultraLoop<3){ultraLoop++;}
	  else{ultraLoop=0;}
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
