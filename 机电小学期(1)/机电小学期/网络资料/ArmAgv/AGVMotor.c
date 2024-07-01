#include "STM32Type.h"
#include "stm32f10x_rcc.h"
#include "AGVMotor.h"
#include "AGVTimer.h"
#include "AGVComm.h"
#include "AGVConfig.h"
#include "misc.h"
#include "stm32f10x_tim.h"

void vInitMotor(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG , ENABLE);//使能GPIO时钟
	GPIO_InitStructure.GPIO_Pin = LEFTFRONTMOTOR_POS_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LEFTFRONTMOTOR_POS_GPIO, &GPIO_InitStructure); 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG , ENABLE);//使能GPIO时钟
	GPIO_InitStructure.GPIO_Pin = LEFTFRONTMOTOR_NEG_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LEFTFRONTMOTOR_NEG_GPIO, &GPIO_InitStructure); 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG , ENABLE);//使能GPIO时钟
	GPIO_InitStructure.GPIO_Pin = LEFTBACKMOTOR_POS_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LEFTBACKMOTOR_POS_GPIO, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG , ENABLE);//使能GPIO时钟
	GPIO_InitStructure.GPIO_Pin = LEFTBACKMOTOR_NEG_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LEFTBACKMOTOR_NEG_GPIO, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG , ENABLE);//使能GPIO时钟
	GPIO_InitStructure.GPIO_Pin = RIGHTFRONTMOTOR_POS_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(RIGHTFRONTMOTOR_POS_GPIO, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG , ENABLE);//使能GPIO时钟
	GPIO_InitStructure.GPIO_Pin = RIGHTFRONTMOTOR_NEG_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(RIGHTFRONTMOTOR_NEG_GPIO, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);//使能GPIO时钟
	GPIO_InitStructure.GPIO_Pin = RIGHTBACKMOTOR_POS_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(RIGHTBACKMOTOR_POS_GPIO, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);//使能GPIO时钟
	GPIO_InitStructure.GPIO_Pin = RIGHTBACKMOTOR_NEG_PIN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(RIGHTBACKMOTOR_NEG_GPIO, &GPIO_InitStructure);
	
	strDevKVar.PWM.cLBM_Pwm = 0;
	strDevKVar.PWM.cLFM_Pwm = 0;
	strDevKVar.PWM.cRFM_Pwm = 0;
	strDevKVar.PWM.cRBM_Pwm = 0;
}

void vGenMotorPwm(INT8 cGoSpeed, INT16 iGoGrade, INT8 cTurnSpeed, INT16 iTurnGrade)
{//转速正负对应左、右转弯
	INT16 itmp;//只能转弯或直行，不可以同时--同时时转弯能力差，车轮摩擦力不够
	
	///////////////////////////////////////////////////////////////////////////
	if(iTurnGrade==TURN_ZERO) itmp = (INT16)cGoSpeed*100/iGoGrade;
	else if(cTurnSpeed>0) itmp = +(INT16)strDevCfg.RLSCOFF.cLFM_TurnLeft * (INT16)cTurnSpeed/iTurnGrade;
	else itmp = -(INT16)strDevCfg.RLSCOFF.cLFM_TurnRight * (INT16)cTurnSpeed/iTurnGrade;
	if(itmp>0) itmp += PWM_OFFSET;//postive offset
	if(itmp<0) itmp -= PWM_OFFSET;//negtive offset
	if(itmp>+PWM_MAXTWC) itmp = +PWM_MAXTWC;
	if(itmp<-PWM_MAXTWC) itmp = -PWM_MAXTWC;
	strDevKVar.PWM.cLFM_Pwm = itmp;
	
	///////////////////////////////////////////////////////////////////////////					
	if(iTurnGrade==TURN_ZERO) itmp = (INT16)cGoSpeed*100/iGoGrade;
	else if(cTurnSpeed>0) itmp = +(INT16)strDevCfg.RLSCOFF.cLBM_TurnLeft * (INT16)cTurnSpeed/iTurnGrade;
	else itmp = -(INT16)strDevCfg.RLSCOFF.cLBM_TurnRight * (INT16)cTurnSpeed/iTurnGrade;
	if(itmp>0) itmp += PWM_OFFSET;//postive offset
	if(itmp<0) itmp -= PWM_OFFSET;//negtive offset
	if(itmp>+PWM_MAXTWC) itmp = +PWM_MAXTWC;
	if(itmp<-PWM_MAXTWC) itmp = -PWM_MAXTWC;
	strDevKVar.PWM.cLBM_Pwm = itmp;
	
	///////////////////////////////////////////////////////////////////////////
	if(iTurnGrade==TURN_ZERO) itmp = (INT16)cGoSpeed*100/iGoGrade;
	else if(cTurnSpeed>0) itmp = +(INT16)strDevCfg.RLSCOFF.cRFM_TurnLeft * (INT16)cTurnSpeed/iTurnGrade;
	else itmp = -(INT16)strDevCfg.RLSCOFF.cRFM_TurnRight * (INT16)cTurnSpeed/iTurnGrade;	
	if(itmp>0) itmp += PWM_OFFSET;//postive offset
	if(itmp<0) itmp -= PWM_OFFSET;//negtive offset
	if(itmp>+PWM_MAXTWC) itmp = +PWM_MAXTWC;
	if(itmp<-PWM_MAXTWC) itmp = -PWM_MAXTWC;
	strDevKVar.PWM.cRFM_Pwm = itmp;
	
	///////////////////////////////////////////////////////////////////////////
	if(iTurnGrade==TURN_ZERO) itmp = (INT16)cGoSpeed*100/iGoGrade;
	else if(cTurnSpeed>0) itmp = +(INT16)strDevCfg.RLSCOFF.cRBM_TurnLeft * (INT16)cTurnSpeed/iTurnGrade;
	else itmp = -(INT16)strDevCfg.RLSCOFF.cRBM_TurnRight * (INT16)cTurnSpeed/iTurnGrade;	
	if(itmp>0) itmp += PWM_OFFSET;//postive offset
	if(itmp<0) itmp -= PWM_OFFSET;//negtive offset
	if(itmp>+PWM_MAXTWC) itmp = +PWM_MAXTWC;
	if(itmp<-PWM_MAXTWC) itmp = -PWM_MAXTWC;
	strDevKVar.PWM.cRBM_Pwm = itmp;
	
	///////////////////////////////////////////////////////////////////////////
}
