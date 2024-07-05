#ifndef _AGVMOTOR_H_
#define _AGVMOTOR_H_
	
	#include "STM32Type.h"
	
	///////////////////////////////////////////////////////////////////////////////
	//                            ��ǰ����������Ŷ���
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
	//                            ������������Ŷ���
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
	//                            ��ǰ����������Ŷ���
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
	//                            �Һ����������Ŷ���
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
	//                            ��ǰ�ֵ���˶�����
	///////////////////////////////////////////////////////////////////////////////
	#define LFM_RUNFORWARD    LEFTFRONTMOTOR_POS_SET; LEFTFRONTMOTOR_NEG_CLR    //ǰ������ת��
	#define LFM_RUNBACKWARD   LEFTFRONTMOTOR_POS_CLR; LEFTFRONTMOTOR_NEG_SET    //���ˣ���ת��
	#define LFM_RUNSTOP       LEFTFRONTMOTOR_POS_CLR; LEFTFRONTMOTOR_NEG_CLR    //ֹͣ����ת��
	///////////////////////////////////////////////////////////////////////////////
	//                            ����ֵ���˶�����
	///////////////////////////////////////////////////////////////////////////////
	#define LBM_RUNFORWARD     LEFTBACKMOTOR_POS_SET; LEFTBACKMOTOR_NEG_CLR    //ǰ������ת��
	#define LBM_RUNBACKWARD    LEFTBACKMOTOR_POS_CLR; LEFTBACKMOTOR_NEG_SET    //���ˣ���ת��
	#define LBM_RUNSTOP        LEFTBACKMOTOR_POS_CLR; LEFTBACKMOTOR_NEG_CLR    //ֹͣ����ת��
	///////////////////////////////////////////////////////////////////////////////
	//                            ��ǰ�ֵ���˶�����
	///////////////////////////////////////////////////////////////////////////////
	#define RFM_RUNFORWARD   RIGHTFRONTMOTOR_POS_SET; RIGHTFRONTMOTOR_NEG_CLR    //ǰ������ת��
	#define RFM_RUNBACKWARD  RIGHTFRONTMOTOR_POS_CLR; RIGHTFRONTMOTOR_NEG_SET    //���ˣ���ת��
	#define RFM_RUNSTOP      RIGHTFRONTMOTOR_POS_CLR; RIGHTFRONTMOTOR_NEG_CLR    //ֹͣ����ת��
	///////////////////////////////////////////////////////////////////////////////
	//                            �Һ��ֵ���˶�����
	///////////////////////////////////////////////////////////////////////////////
	#define RBM_RUNFORWARD    RIGHTBACKMOTOR_POS_SET; RIGHTBACKMOTOR_NEG_CLR   //ǰ������ת��
	#define RBM_RUNBACKWARD   RIGHTBACKMOTOR_POS_CLR; RIGHTBACKMOTOR_NEG_SET   //���ˣ���ת��
	#define RBM_RUNSTOP       RIGHTBACKMOTOR_POS_CLR; RIGHTBACKMOTOR_NEG_CLR   //ֹͣ����ת��
	///////////////////////////////////////////////////////////////////////////////\
	
	#define PWM_OFFSET		25
	
	#define TURN_FAST		100//���ٹ���---���ٶ�
	#define TURN_SLOW		143//100/70%���Զ�70%���ٶȣ�
	#define TURN_ZERO		12500//������
	
	#define GO_FAST			100//����ֱ��ǰ��---���ٶ�
	#define GO_SLOW			143//125/70%���Զ�70%���ٶȣ�
	#define GO_ZERO			12500//ֱֹͣ��ǰ��

	extern void vInitMotor(void);	
	extern void vGenMotorPwm(INT8 cGoSpeed, INT16 iGoGrade, INT8 cTurnSpeed, INT16 iTurnGrade);//ת��������Ӧ���ҹ���
#endif
