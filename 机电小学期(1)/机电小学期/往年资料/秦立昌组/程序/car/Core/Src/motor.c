#include "motor.h"

const double ti = WHEEL_DIAMETER;
const double rt = (REDUCTION_RATIO * PULSE_NUMBER * RESOLUTION)*(DELAY/1000);
short target_speed_L=0;
short target_speed_R=0;
short motor_speed_L=0;
short motor_speed_R=0;
short total_speed=0;
short rotation_radius=0;
short target_pos = 0;
short rotation_curvature = 0;

void motor_init()
{
	  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
	  HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
	  HAL_TIM_Base_Start_IT((TIM_HandleTypeDef *)&htim7);
	  motor_PWM(0, 0);
}
void motor_PWM(int16_t PWM_L,int16_t PWM_R)
{
	if(PWM_L > MAX_SPEED)
		PWM_L = MAX_SPEED;
	if(PWM_R > MAX_SPEED)
		PWM_R = MAX_SPEED;
	if(PWM_L < -MAX_SPEED)
		PWM_L = -MAX_SPEED;
	if(PWM_R < -MAX_SPEED)
		PWM_R = -MAX_SPEED;
	if(PWM_L > 0)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);//
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET); //
	}
	else if(PWM_L < 0)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET); //
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);//
	}
	else
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);//
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);//
	}
	if(PWM_R > 0)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);//
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET); //
	}
	else if(PWM_R < 0)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);//
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET); //
	}
	else
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);//
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);//
	}
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,abs(PWM_R));
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,abs(PWM_L));
}

int motor_read_speed_L()
{
	int Direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1);   //读取电机转动方向
	int CaptureNumber =	(short)__HAL_TIM_GET_COUNTER(&htim1);	  //读取编码器数�????
	__HAL_TIM_SET_COUNTER(&htim1,0);
	float speed;
	speed = ti*CaptureNumber/rt;
	return (int)speed;
}

int motor_read_speed_R()
{
	int Direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim8);   //读取电机转动方向
	int CaptureNumber =	(short)__HAL_TIM_GET_COUNTER(&htim8);	  //读取编码器数�????
	__HAL_TIM_SET_COUNTER(&htim8,0);
	float speed;
	speed = ti*CaptureNumber/rt;
	return (int)speed;
}

short PID_cal_L(short target_speed,short speed,short *error)
{
	double error_L = target_speed - speed;
	static double error_last_L = 0,error_prev_L = 0;
	double pwm_add=0;
	*error = error_L;
	pwm_add = KP*(error_L - error_last_L) + KI*error_L + KD*(error_L-2.0f*error_last_L+error_prev_L);

	error_prev_L = error_last_L;	  	    // 保存上上次误差
	error_last_L = error_L;	              // 保存上次偏差

	return (short)pwm_add;
}

short PID_cal_R(short target_speed,short speed,short *error)
{
	double error_R = target_speed - speed;
	static double error_last_R = 0,error_prev_R = 0;
	double pwm_add=0;
	*error = error_R;
	pwm_add = KP*(error_R - error_last_R) + KI*error_R + KD*(error_R-2.0f*error_last_R+error_prev_R);

	error_prev_R = error_last_R;	  	    // 保存上上次误差
	error_last_R = error_R;	              // 保存上次偏差

	return (short)pwm_add;
}

void drive(short speed,short radius)
{
	if(!radius)
	{
		target_speed_L = speed;
		target_speed_R = speed;
	}
	else
	{
		double omega = (float)speed/(float)radius;
		target_speed_L = speed - omega*D/2;
		target_speed_R = speed + omega*D/2;
	}
	if (target_speed_L > MAX_TARGET_SPEED)
		target_speed_L = MAX_TARGET_SPEED;
	if (target_speed_R > MAX_TARGET_SPEED)
		target_speed_R = MAX_TARGET_SPEED;
	if (target_speed_L < -MAX_TARGET_SPEED)
		target_speed_L = -MAX_TARGET_SPEED;
	if (target_speed_R < -MAX_TARGET_SPEED)
		target_speed_R = -MAX_TARGET_SPEED;
}
