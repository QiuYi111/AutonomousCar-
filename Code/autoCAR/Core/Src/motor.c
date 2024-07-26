/*
 * motor.c
 *
 *  Created on: Jul 20, 2024
 *      Author: 25138
 */
#include "motor.h"

extern float kp,ki,kd,rpmLeft,rpmRight;extern int pulseLeft,pulseRight;extern int spin,preScaller,dt;
extern float leftError,rightError, error_last_left ,error_before_left, error_last_right ,error_before_right ;
uint16_t count_num_left_past=-1;
uint16_t count_num_right_past=-1;
extern int countnum_rightcircuance;
extern int countnum_leftcircuance;
extern float currentRpm_left,currentRpm_right;
void setLeftRpm(float rpm){
	 currentRpm_left=getLeftRpm(&htim2);
	pulseLeft+=pidDiffLeft(rpm, currentRpm_left,&leftError);

}
void setRightRpm(float rpm){
	currentRpm_right=getRightRpm(&htim4);
	pulseRight+=pidDiffRight(rpm, currentRpm_right,&rightError);

}
void setPID(char factor,float index){
	if (factor=='p'){kp=index;}
	else if (factor=='i'){ki=index;}
	else if (factor=='d'){kd=index;};
}
int pidDiffLeft(float target,float present,float * erro){
	float error = target - present;

		float pwm_pid=0;
		*erro = error;
		pwm_pid = kp*(error- error_last_left)+ki* error
				+kd*( error-2* error_last_left+ error_before_left);
		 error_before_left =  error_last_left;	  	    // 保存上上次误差
		 error_last_left =  error;	              // 保存上次偏差
		 return (int)pwm_pid;
}
int pidDiffRight(float target,float present,float * erro){
	float error = target - present;

		float pwm_pid=0;
		*erro = error;
		pwm_pid = kp*(error- error_last_right)+ki* error
				+kd*( error-2* error_last_right+ error_before_right);
		 error_before_right =  error_last_right;	  	    // 保存上上次误差
		 error_last_right =  error;	              // 保存上次偏差
		 return (int)pwm_pid;
}
float getLeftRpm(TIM_HandleTypeDef * htim){
	uint16_t countnum=0;
	if(countnum_leftcircuance!=0){
	if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim)){
		countnum =32768-__HAL_TIM_GET_COUNTER(htim);
		count_num_left_past=countnum;
		countnum_leftcircuance=1;
	}else{
		countnum=__HAL_TIM_GET_COUNTER(htim)-32768;
		count_num_left_past=countnum;
		countnum_leftcircuance=1;
	}
	float rpm;
	rpm = (4*countnum)/3;//读取编码器数�????
	__HAL_TIM_SET_COUNTER(htim,32768);
	return rpm;
	}
	else{
		float rpm=0;
		__HAL_TIM_SET_COUNTER(htim,32768);
		countnum_leftcircuance=1;
		return rpm;
	}

}
float getRightRpm(TIM_HandleTypeDef * htim){
	uint16_t countnum=0;
	if(countnum_rightcircuance!=0){
	if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim)){
		countnum =32768-__HAL_TIM_GET_COUNTER(htim);
		count_num_right_past=countnum;
		countnum_rightcircuance=1;
	}else{
		countnum=__HAL_TIM_GET_COUNTER(htim)-32768;
		count_num_right_past=countnum;
		countnum_rightcircuance=1;
	}
	float rpm;
	rpm = (4*countnum)/3;//读取编码器数�????
	__HAL_TIM_SET_COUNTER(htim,32768);
	return rpm;
	}
	else{
			float rpm=0;
			__HAL_TIM_SET_COUNTER(htim,32768);
			countnum_rightcircuance=1;
			return rpm;
		}
}
void setRightPWM(int pulse,uint32_t channel){
	int pwm_pulse;
	if(pulse>0){pwm_pulse=pulse;}else{pwm_pulse=0;};
	if(pwm_pulse>=719){pwm_pulse=719;};
	__HAL_TIM_SET_COMPARE(&htim5,channel,pwm_pulse);
}
void setLeftPWM(int pulse,uint32_t channel){
	int pwm_pulse;
	if(pulse>0){pwm_pulse=pulse;}else{pwm_pulse=0;};
	if(pwm_pulse>=719){pwm_pulse=719;};
	__HAL_TIM_SET_COMPARE(&htim5,channel,pwm_pulse);
}

void setDirection(char dir){
	if (dir=='w'){
		HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, GPIO_PIN_SET);
	}
	else if (dir=='s'){
		HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, GPIO_PIN_RESET);
	}
	else if (dir=='a'){
		HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, GPIO_PIN_SET);
	}
	else if (dir=='d'){
		HAL_GPIO_WritePin(IN1_GPIO_Port, IN1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN2_GPIO_Port, IN2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN3_GPIO_Port, IN3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN4_GPIO_Port, IN4_Pin, GPIO_PIN_RESET);
	}
}
void setSpin(int angel,char dir){

	if (dir=='A'){
		spin=1;
		setDirection('a');
	}else if (dir=='D'){
		spin=1;
		setDirection('d');
	}else if(dir=='F'){
		spin=1;
		setDirection('w');
	}else if(dir=='S'){
		spin=1;
		setDirection('s');
	}
	rpmLeft=rpmRight=55;
	dt=angel;

}
