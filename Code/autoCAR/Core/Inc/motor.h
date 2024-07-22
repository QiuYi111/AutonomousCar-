/*
 * motor.h
 *
 *  Created on: Jul 20, 2024
 *      Author: 25138
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "tim.h"
#include "main.h"
extern float kp,ki,kd,erro;extern int countnum_rightcircuance,countnum_leftcircuance;
void setLeftRpm(float rpm);
void setRightRpm(float rpm);
int pidDiffLeft(float target,float present,float * erro);
int pidDiffRight(float target,float present,float * erro);
float getLeftRpm(TIM_HandleTypeDef * htim);
float getRightRpm(TIM_HandleTypeDef * htim);
void setLeftPWM(int pulse,uint32_t channel);
void setRightPWM(int pulse,uint32_t channel);
void setSpeed(int pulse);
void setPID(char factor,float index);
void setDirection(char dir);
#ifdef __cplusplus
}
#endif
#endif /* INC_MOTOR_H_ */
