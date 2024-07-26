/*
 * servo.c
 *
 *  Created on: Jul 17, 2024
 *      Author: DELL
 */
#include "servo.h"
#include "tim.h"
//Servo1 是夹爪夹取电机
TIM_HandleTypeDef *Servo1 = &htim1;
uint32_t Servo1_Channel = TIM_CHANNEL_2;
//Servo2 是夹爪转动电机 Mg995
TIM_HandleTypeDef *Servo2=&htim1;
uint32_t Servo2_Channel = TIM_CHANNEL_1;
//Servo3 是openMV转动电机
TIM_HandleTypeDef *Servo3=&htim1;
uint32_t Servo3_Channel = TIM_CHANNEL_3;
//下面是各个电机转动的总函数
extern int craw_state;
float Servo_turn(int servo_number,float turning_theta,float turning_theta_0){
	float turning_time = 0;
	float turning_t=0;
	float turning_time1 = 0;
	float turning_t1=0;
	//uint32_t waiting_time_servo=0;
	turning_time=turning_theta/90;
	turning_time1=turning_theta_0/90;
	//waiting_time_servo=turning_time*waiting_time_servo_max;
	turning_time=turning_time+0.5;
	turning_time1=turning_time1+0.5;
	turning_t=turning_time*1000;
	turning_t1=turning_time1*1000;

	if(turning_theta>180){
		return 0;
	}
	float turning_time2=0;//真正转过的角度
	turning_time2=turning_t-turning_t1;
	float turning_theta2=0;
	turning_theta2=turning_theta-turning_theta_0;
	if(turning_time2>0){
	float p=turning_theta2;
	p=p/180;
	p=p*60;
	int delay_time=0;
	delay_time=(int)p;
if(servo_number==1){
	for(float i=0;i<turning_time2;i+=turning_time2/100){
		__HAL_TIM_SET_COMPARE(Servo1,Servo1_Channel,(uint32_t)(turning_t));
		HAL_Delay(delay_time);
	}

}
else if(servo_number==2){
	for(float i=0;i<turning_time2;i+=turning_time2/100){
			__HAL_TIM_SET_COMPARE(Servo2,Servo2_Channel,(uint32_t)(turning_t));
	HAL_Delay(delay_time);
		}
}
else if(servo_number==3){
	for(float i=0;i<turning_time2;i+=turning_time2/100){
			__HAL_TIM_SET_COMPARE(Servo3,Servo3_Channel,(uint32_t)(turning_t));
	HAL_Delay(delay_time);
		}
}
else{
	return 0;
}
	}
	else{
		float p=-turning_theta2;
			p=p/180;
			p=p*250;
			int delay_time=0;
			delay_time=(int)p;
	if(servo_number==1){
		for(float i=0;i>turning_time2;i+=turning_time2/100){
			__HAL_TIM_SET_COMPARE(Servo1,Servo1_Channel,(uint32_t)(i+turning_t1));
			HAL_Delay(delay_time);
		}

	}
	else if(servo_number==2){
		for(float i=0;i>turning_time2;i+=turning_time2/100){
				__HAL_TIM_SET_COMPARE(Servo2,Servo2_Channel,(uint32_t)(i+turning_t1));
			HAL_Delay(delay_time);
			}
	}
	else if(servo_number==3){
		for(float i=0;i>turning_time2;i+=turning_time2/100){
				__HAL_TIM_SET_COMPARE(Servo3,Servo3_Channel,(uint32_t)(i+turning_t1));
				HAL_Delay(delay_time);
		}
	}
	else{
		return 0;
	}
		}


return turning_theta;
}
//下面是Servo1的各项函数
float Servo1_init(float turning_theta_0){
	//HAL_TIM_PWM_Start(Servo1,Servo1_Channel);
	float init_data=Servo1_initial_data;
	float whe_w=0;
	whe_w=Servo_turn(1,init_data,turning_theta_0);
	return whe_w;
}
float Servo1_craw(float turning_theta_0){
		float craw_data=Servo1_craw_theta;
		float whe_w=0;
		whe_w=Servo_turn(1,craw_data,turning_theta_0);
		return whe_w;
}
float Servo1_put(float turning_theta_0){
			float put_data=Servo1_put_down_theta;
			float whe_w=0;
			whe_w=Servo_turn(1,put_data,turning_theta_0);
			return whe_w;
}
//下面是Servol2的各项函数
float Servo2_init(float turning_theta_0){
	//HAL_TIM_PWM_Start(Servo2,Servo2_Channel);
		float init_data=Servo2_initial_data;
		float whe_w=0;
		whe_w=Servo_turn(2,init_data,turning_theta_0);
		return whe_w;
}
float Servo2_driving(float turning_theta_0){
			float drive_data=Servo2_driving_theta;
			float whe_w=0;
			whe_w=Servo_turn(2,drive_data,turning_theta_0);
			return whe_w;
}
float Servo2_puting(float turning_theta_0){
			float put_data=Servo2_puting_theta;
			float whe_w=0;
			whe_w=Servo_turn(2,put_data,turning_theta_0);
			return whe_w;
}
//下面是Servol3的各项函数
float Servo3_init(float turning_theta_0){
	HAL_TIM_PWM_Start(Servo3,Servo3_Channel);
		float init_data=Servo3_initial_data;
		float whe_w=0;
		whe_w=Servo_turn(3,init_data,turning_theta_0);
		return whe_w;
}
float Servo3_driving(float turning_theta_0){
			float drive_data=Servo3_driving_theta;
			float whe_w=0;
			whe_w=Servo_turn(3,drive_data,turning_theta_0);
			return whe_w;
}
float Servo3_puting(float turning_theta_0){
			float put_data=Servo3_puting_theta;
			float whe_w=0;
			whe_w=Servo_turn(3,put_data,turning_theta_0);
			return whe_w;
}
float craw_up_trail(int if_crow){
	if(if_crow==1){

		float turning_theta_crow_1_S2=Servo2_initial_data;
		turning_theta_crow_1_S2=Servo_turn(2,60,25);
		float turning_theta_crow_1_S1=Servo1_initial_data;
		turning_theta_crow_1_S1=Servo1_put(turning_theta_crow_1_S1);
		turning_theta_crow_1_S2=Servo2_puting(turning_theta_crow_1_S2);
		turning_theta_crow_1_S1=Servo1_craw(turning_theta_crow_1_S1);
		turning_theta_crow_1_S2=Servo2_driving(turning_theta_crow_1_S2);
		craw_state=0;
		return 1;
	}
	else return 0;
}
float put_down_trail(int if_put){

	if(if_put==1){

			float turning_theta_crow_1_S2=Servo2_driving_theta;
			float turning_theta_crow_1_S1=Servo1_craw_theta;
			turning_theta_crow_1_S2=Servo2_puting(turning_theta_crow_1_S2);
			turning_theta_crow_1_S1=Servo1_put(turning_theta_crow_1_S1);
			turning_theta_crow_1_S2=Servo_turn(2,60,turning_theta_crow_1_S2);
			turning_theta_crow_1_S1=Servo1_craw(turning_theta_crow_1_S1);
			turning_theta_crow_1_S2=Servo2_driving(turning_theta_crow_1_S2);
			craw_state=0;
			return 1;
		}
		else return 0;
}
