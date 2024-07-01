#ifndef _AGVMOTOR_H_
#define _AGVMOTOR_H_
	
	#include "STM32Type.h"
	
	///////////////////////////////////////////////////////////////////////////////
	//                            左前电机控制引脚定义
	///////////////////////////////////////////////////////////////////////////////
	#define LEFTFRONTMOTOR_POS_PIN      GPIO_Pin_13
	#define LEFTFRONTMOTOR_POS_GPIO     GPIOG
	#define LEFTFRONTMOTOR_POS_SET      LEFTFRONTMOTOR_POS_GPIO->BSRR = LEFTFRONTMOTOR_POS_PIN
	#define LEFTFRONTMOTOR_POS_CLR      LEFTFRONTMOTOR_POS_GPIO->BRR = LEFTFRONTMOTOR_POS_PIN
//	#define LEFTFRONTMOTOR_POS_SET      GPIO_SetBits(LEFTFRONTMOTOR_POS_GPIO , LEFTFRONTMOTOR_POS_PIN)
//	#define LEFTFRONTMOTOR_POS_CLR      GPIO_ResetBits(LEFTFRONTMOTOR_POS_GPIO , LEFTFRONTMOTOR_POS_PIN)		
									
	#define LEFTFRONTMOTOR_NEG_PIN      GPIO_Pin_14
	#define LEFTFRONTMOTOR_NEG_GPIO     GPIOG
	#define LEFTFRONTMOTOR_NEG_SET      LEFTFRONTMOTOR_NEG_GPIO->BSRR = LEFTFRONTMOTOR_NEG_PIN
	#define LEFTFRONTMOTOR_NEG_CLR      LEFTFRONTMOTOR_NEG_GPIO->BRR = LEFTFRONTMOTOR_NEG_PIN
//	#define LEFTFRONTMOTOR_NEG_SET      GPIO_SetBits(LEFTFRONTMOTOR_NEG_GPIO , LEFTFRONTMOTOR_NEG_PIN)
//	#define LEFTFRONTMOTOR_NEG_CLR      GPIO_ResetBits(LEFTFRONTMOTOR_NEG_GPIO , LEFTFRONTMOTOR_NEG_PIN)
	///////////////////////////////////////////////////////////////////////////////
	//                            左后电机控制引脚定义
	///////////////////////////////////////////////////////////////////////////////
	#define LEFTBACKMOTOR_POS_PIN       GPIO_Pin_11
	#define LEFTBACKMOTOR_POS_GPIO      GPIOG
	#define LEFTBACKMOTOR_POS_SET       LEFTBACKMOTOR_POS_GPIO->BSRR = LEFTBACKMOTOR_POS_PIN
	#define LEFTBACKMOTOR_POS_CLR       LEFTBACKMOTOR_POS_GPIO->BRR = LEFTBACKMOTOR_POS_PIN
									
	#define LEFTBACKMOTOR_NEG_PIN       GPIO_Pin_12
	#define LEFTBACKMOTOR_NEG_GPIO      GPIOG
	#define LEFTBACKMOTOR_NEG_SET       LEFTBACKMOTOR_NEG_GPIO->BSRR = LEFTBACKMOTOR_NEG_PIN
	#define LEFTBACKMOTOR_NEG_CLR       LEFTBACKMOTOR_NEG_GPIO->BRR = LEFTBACKMOTOR_NEG_PIN
	///////////////////////////////////////////////////////////////////////////////
	//                            右前电机控制引脚定义
	///////////////////////////////////////////////////////////////////////////////
	#define RIGHTFRONTMOTOR_POS_PIN      GPIO_Pin_15
	#define RIGHTFRONTMOTOR_POS_GPIO     GPIOG
	#define RIGHTFRONTMOTOR_POS_SET      RIGHTFRONTMOTOR_POS_GPIO->BSRR = RIGHTFRONTMOTOR_POS_PIN
	#define RIGHTFRONTMOTOR_POS_CLR      RIGHTFRONTMOTOR_POS_GPIO->BRR = RIGHTFRONTMOTOR_POS_PIN
									
	#define RIGHTFRONTMOTOR_NEG_PIN      GPIO_Pin_9
	#define RIGHTFRONTMOTOR_NEG_GPIO     GPIOG
	#define RIGHTFRONTMOTOR_NEG_SET      RIGHTFRONTMOTOR_NEG_GPIO->BSRR = RIGHTFRONTMOTOR_NEG_PIN
	#define RIGHTFRONTMOTOR_NEG_CLR      RIGHTFRONTMOTOR_NEG_GPIO->BRR = RIGHTFRONTMOTOR_NEG_PIN
	///////////////////////////////////////////////////////////////////////////////
	//                            右后电机控制引脚定义
	///////////////////////////////////////////////////////////////////////////////
	#define RIGHTBACKMOTOR_POS_PIN       GPIO_Pin_2
	#define RIGHTBACKMOTOR_POS_GPIO      GPIOD
	#define RIGHTBACKMOTOR_POS_SET       RIGHTBACKMOTOR_POS_GPIO->BSRR = RIGHTBACKMOTOR_POS_PIN
	#define RIGHTBACKMOTOR_POS_CLR       RIGHTBACKMOTOR_POS_GPIO->BRR = RIGHTBACKMOTOR_POS_PIN
									
	#define RIGHTBACKMOTOR_NEG_PIN       GPIO_Pin_3
	#define RIGHTBACKMOTOR_NEG_GPIO      GPIOD
	#define RIGHTBACKMOTOR_NEG_SET       RIGHTBACKMOTOR_NEG_GPIO->BSRR = RIGHTBACKMOTOR_NEG_PIN
	#define RIGHTBACKMOTOR_NEG_CLR       RIGHTBACKMOTOR_NEG_GPIO->BRR = RIGHTBACKMOTOR_NEG_PIN	
	///////////////////////////////////////////////////////////////////////////////
	//                            左前轮电机运动定义
	///////////////////////////////////////////////////////////////////////////////
	#define LFM_RUNFORWARD    LEFTFRONTMOTOR_POS_SET; LEFTFRONTMOTOR_NEG_CLR    //前进（正转）
	#define LFM_RUNBACKWARD   LEFTFRONTMOTOR_POS_CLR; LEFTFRONTMOTOR_NEG_SET    //后退（反转）
	#define LFM_RUNSTOP       LEFTFRONTMOTOR_POS_CLR; LEFTFRONTMOTOR_NEG_CLR    //停止（不转）
	///////////////////////////////////////////////////////////////////////////////
	//                            左后轮电机运动定义
	///////////////////////////////////////////////////////////////////////////////
	#define LBM_RUNFORWARD     LEFTBACKMOTOR_POS_SET; LEFTBACKMOTOR_NEG_CLR    //前进（正转）
	#define LBM_RUNBACKWARD    LEFTBACKMOTOR_POS_CLR; LEFTBACKMOTOR_NEG_SET    //后退（反转）
	#define LBM_RUNSTOP        LEFTBACKMOTOR_POS_CLR; LEFTBACKMOTOR_NEG_CLR    //停止（不转）
	///////////////////////////////////////////////////////////////////////////////
	//                            右前轮电机运动定义
	///////////////////////////////////////////////////////////////////////////////
	#define RFM_RUNFORWARD   RIGHTFRONTMOTOR_POS_SET; RIGHTFRONTMOTOR_NEG_CLR    //前进（正转）
	#define RFM_RUNBACKWARD  RIGHTFRONTMOTOR_POS_CLR; RIGHTFRONTMOTOR_NEG_SET    //后退（反转）
	#define RFM_RUNSTOP      RIGHTFRONTMOTOR_POS_CLR; RIGHTFRONTMOTOR_NEG_CLR    //停止（不转）
	///////////////////////////////////////////////////////////////////////////////
	//                            右后轮电机运动定义
	///////////////////////////////////////////////////////////////////////////////
	#define RBM_RUNFORWARD    RIGHTBACKMOTOR_POS_SET; RIGHTBACKMOTOR_NEG_CLR   //前进（正转）
	#define RBM_RUNBACKWARD   RIGHTBACKMOTOR_POS_CLR; RIGHTBACKMOTOR_NEG_SET   //后退（反转）
	#define RBM_RUNSTOP       RIGHTBACKMOTOR_POS_CLR; RIGHTBACKMOTOR_NEG_CLR   //停止（不转）
	///////////////////////////////////////////////////////////////////////////////\
	
	#define PWM_OFFSET		25
	
	#define TURN_FAST		100//快速拐弯---满速度
	#define TURN_SLOW		143//100/70%（自动70%满速度）
	#define TURN_ZERO		12500//不拐弯
	
	#define GO_FAST			100//快速直线前行---满速度
	#define GO_SLOW			143//125/70%（自动70%满速度）
	#define GO_ZERO			12500//停止直线前行

	extern void vInitMotor(void);	
	extern void vGenMotorPwm(INT8 cGoSpeed, INT16 iGoGrade, INT8 cTurnSpeed, INT16 iTurnGrade);//转速正负对应左、右拐弯
#endif
