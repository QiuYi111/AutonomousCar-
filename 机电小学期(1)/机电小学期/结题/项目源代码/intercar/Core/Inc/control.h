/*
 * control.h
 *
 *  Created on: 2022年7月9日
 *      Author: 86152
 */
#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_
#endif /* INC_CONTROL_H_ */
#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "motor.h"
#include "stm32f1xx_hal_def.h"
#include "stdint-gcc.h"
#include "sonic.h"
#include "stdio.h"
#include "math.h"
#include "imu.h"
#define KPR 1.97
#define KIR 0.04
#define KDR 0.06
#define width 170
#define maxtarget 1000
#define maxcurvature 100
#define spinspeed 260

int targetspeed;
//int spinspeed;
int lefttarget;
int righttarget;
int curvature;
double curv_error;
extern double deviation;
extern double deviation_f;
extern int deviation_s;
short is_fetch;
short is_down;
short is_shut;
short is_groundturn;
short is_stopping;
short avoid_dir;
short avoid_comfirm;
short turn_flag;
short stop_flag;
short check_flag;
short back_flag;
int total_distance;
short turn_target;

double roundPID(double targetposition,double position,double *error); //寻线误差的PID计算函数
void setspeed(int targetspeed,int curvature);  //根据运动需求设置两轮转速
void spin(short spin_dir);   //原地旋转运动函数
void groundturn(short spin_dir,short angle); //原地旋转固定角度
void alongside(double deviation); //巡线总函数
void avoidance();  //避障总函数
void fetchplace();  //自动取物放物控制函数
void avoid_stop();

