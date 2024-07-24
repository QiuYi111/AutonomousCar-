/*
 * servo.h
 *
 *  Created on: Jul 17, 2024
 *      Author: DELL
 */
/*
 * Tips：当前设置时钟为Tim2，使用频率为84MHZ转1MHZ，计数最高2000个，即20ms
*/
#ifndef INC_SERVO_H_
#define INC_SERVO_H_
#include "tim.h"
#include "gpio.h"
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_it.h"

//下面进行转动等待时间的配置
#define waiting_time_servo_max 1000;
//下面进行Servo1转动角度的定义(夹爪）
#define Servo1_initial_data 15;
#define Servo1_craw_theta 10;
#define Servo1_put_down_theta 60;
//下面进行Servo2转动角度的定义（大舵机-夹爪抬起角度）
#define Servo2_initial_data 30;
#define Servo2_driving_theta 30;
#define Servo2_puting_theta 130;
//下面进行Servo3转动角度的定义（openMV）
#define Servo3_initial_data 0;
#define Servo3_driving_theta 50;
#define Servo3_puting_theta 100;
//下面是各个电机转动的总函数
float Servo_turn(int servo_number,float turning_theta,float turning_theta_0);
//下面是Servo1的各项函数
float Servo1_init(float turning_theta_0);
float Servo1_craw(float turning_theta_0);
float Servo1_put(float turning_theta_0);
//下面是Servol2的各项函数
float Servo2_init(float turning_theta_0);
float Servo2_driving(float turning_theta_0);
float Servo2_puting(float turning_theta_0);
//下面是Servol3的各项函数
float Servo3_init(float turning_theta_0);
float Servo3_driving(float turning_theta_0);
float Servo3_puting(float turning_theta_0);
//抓取函数集成
float craw_up_trail(int if_crow);//if_crow==1时，调用循迹时的程序；if_crow==2时，调用避障时的程序
float put_down_trail(int if_put);//if_put==1时，调用循迹时的程序；if_put==2时，调用避障时的程序
#endif /* INC_SERVO_H_ */
