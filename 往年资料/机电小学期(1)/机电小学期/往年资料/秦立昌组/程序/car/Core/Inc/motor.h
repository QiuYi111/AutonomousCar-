
#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "tools.h"
#include "stm32f1xx_hal_def.h"
#include "stdint-gcc.h"

#define REDUCTION_RATIO 21.3
#define PULSE_NUMBER 4.0
#define RESOLUTION 11.0
#define WHEEL_DIAMETER 65.0//mm

#define KP 1.2
#define KI 0.05
#define KD 0.05

#define D 184




extern short target_speed_L;
extern short target_speed_R;
extern short motor_speed_L;
extern short motor_speed_R;
extern short total_speed;
extern short rotation_curvature;
extern short target_pos;


void motor_init();//init basic settings for motors
//void motor_reach_speed(uint16_t speed_left,uint16_t speed_right);
void motor_PWM(int16_t PWM_L,int16_t PWM_R);//set the PWM value of motors
int motor_read_speed_L();//read the left side motor's encoder
int motor_read_speed_R();//read the right side motor's encoder
short PID_cal_L(short target_speed,short speed,short *error);//calculate needed adjustment of left motor speed
short PID_cal_R(short target_speed,short speed,short *error);//calculate needed adjustment of right motor speed
void drive(short speed,short radius);
