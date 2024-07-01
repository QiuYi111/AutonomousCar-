#include "STM32Type.h"
#include "stm32f10x_rcc.h"
#include "AGVXunJi.h"
#include "misc.h"
#include "AGVComm.h"
#include "stm32f10x_tim.h"

#include "AGvUart1.h"
#include "AGvUart3.h"
#include "AGVConfig.h"
#include "AGVComm.h"
#include "AGVMotor.h"
#include "AGVBeepLed.h"
#include "AGVTimer.h"
#include "AGVCP180112.h"
#include "AGVGenFun.h"

void vInitXunJi(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(L1XJ_CLK , ENABLE);//ʹ��GPIOʱ��	
	GPIO_InitStructure.GPIO_Pin = L1XJ_PIN;//����ʹ��GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//����GPIO�˿��ٶ�
	GPIO_Init(L1XJ_GPIO , &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(L2XJ_CLK , ENABLE);//ʹ��GPIOʱ��	
	GPIO_InitStructure.GPIO_Pin = L2XJ_PIN;//����ʹ��GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//����GPIO�˿��ٶ�
	GPIO_Init(L2XJ_GPIO , &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(R1XJ_CLK , ENABLE);//ʹ��GPIOʱ��	
	GPIO_InitStructure.GPIO_Pin = R1XJ_PIN;//����ʹ��GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//����GPIO�˿��ٶ�
	GPIO_Init(R1XJ_GPIO , &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(R2XJ_CLK , ENABLE);//ʹ��GPIOʱ��	
	GPIO_InitStructure.GPIO_Pin = R2XJ_PIN;//����ʹ��GPIO�ܽ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//����GPIOģʽ,��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//����GPIO�˿��ٶ�
	GPIO_Init(R2XJ_GPIO , &GPIO_InitStructure);	
}
	
void vXunJiProc(void)
{
//���������߹���
//�ٶ���װ��4ֻ�������������ҷֱ�װ�ڣ�PE2,4,6,5����
//�ֱ��Ӧ����ѭ�����м�ѭ������ѭ�����ұ���
//�����������У�������õ����ٵĴ��������������������Բ���װ
	switch(strDevCfg.XJPARA.cDectorSel)
	{
		case XJDETECTOR_TWO:
//1) 2ֻ��������ʹ������������ұ�(PE2,6)����2ֻ���������벼�������к��ߵ������
//   ѹ��ģʽ--�Ҳഫ����ѹ���Ҳ��������
//   ����ģʽ--����2ֻ�����������������м�����
			if( (strDevKVar.DETECTOR.B.bL2XJ==1) && (strDevKVar.DETECTOR.B.bL1XJ==1) && (strDevKVar.DETECTOR.B.bR1XJ==1) && (strDevKVar.DETECTOR.B.bR2XJ==1))//��⵽ȫ������
				vGenMotorPwm(0, GO_FAST, 0, TURN_ZERO);//ͣ��
			else if(strDevCfg.XJPARA.bIsStampWire)
			{//ѹ��ģʽ---����ѡ��2ֻ������
				vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//�޷��ж�
			}
			else
			{//����ģʽ
				if(strDevKVar.DETECTOR.B.bR2XJ)      vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_SLOW);//�Ҳഫ����ѹ���Һ���
				else if(strDevKVar.DETECTOR.B.bL2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, -strDevCfg.XJPARA.cMaxTurnSpeed, TURN_SLOW);//�Ҳഫ����ѹ���Һ���
				else                                 vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//�޷��ж�
			}
			break;

//2) 3ֻ��������ʹ������������ұ����м�(PE2,4,5)��������������ұ�2ֻ���������벼�������к��ߵ������
//   ѹ��ģʽ--�м䴫����ѹ���Ҳ��������
//   ����ģʽ--����������ұ�2ֻ�����������������м�����			
		case XJDETECTOR_THREE:
			if( (strDevKVar.DETECTOR.B.bL2XJ==1) && (strDevKVar.DETECTOR.B.bL1XJ==1) && (strDevKVar.DETECTOR.B.bR1XJ==1) && (strDevKVar.DETECTOR.B.bR2XJ==1))//��⵽ȫ������
				vGenMotorPwm(0, GO_FAST, 0, TURN_ZERO);//ͣ��
			else if(strDevCfg.XJPARA.bIsStampWire)
			{//ѹ��ģʽ
				if(strDevKVar.DETECTOR.B.bL1XJ)      vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//�м䴫����ѹ�������ܣ�����---ֱ��
				else if(strDevKVar.DETECTOR.B.bL2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_FAST);//��ഫ����ѹ�������ܣ��Һ���-----�ҹ���
				else if(strDevKVar.DETECTOR.B.bR2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, -strDevCfg.XJPARA.cMaxTurnSpeed, TURN_FAST);//�Ҳഫ����ѹ�������ܣ��Һ���-----�����
				else                                 vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_SLOW, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//�޷��ж�----ֱ��
			}
			else
			{//����ģʽ
				if(strDevKVar.DETECTOR.B.bL1XJ)      vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, -strDevCfg.XJPARA.cMaxTurnSpeed, TURN_FAST);//�Ҳഫ����ѹ���Һ���--������ת
				else if(strDevKVar.DETECTOR.B.bR2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_SLOW);//�Ҳഫ����ѹ���Һ���--������ת
				else if(strDevKVar.DETECTOR.B.bL2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, -strDevCfg.XJPARA.cMaxTurnSpeed, TURN_SLOW);//�Ҳഫ����ѹ���Һ���--������ת
				else                                 vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//�޷��ж�
			}
			break;
			
//3) 4ֻ��������ʹ������������ұ����м�(PE2,4,6,5)��������������ұ�2ֻ���������벼�������к��ߵ������
//   ѹ��ģʽ--�м䴫����ѹ���Ҳ��������
//   ����ģʽ--����������ұ�2ֻ�����������������м�����			
		case XJDETECTOR_FOUR:
			if( (strDevKVar.DETECTOR.B.bL2XJ==1) && (strDevKVar.DETECTOR.B.bL1XJ==1) && (strDevKVar.DETECTOR.B.bR1XJ==1) && (strDevKVar.DETECTOR.B.bR2XJ==1))//��⵽ȫ������
				vGenMotorPwm(0, GO_FAST, 0, TURN_ZERO);//ͣ��
			else if(strDevCfg.XJPARA.bIsStampWire)
			{//ѹ��ģʽ
				if(strDevKVar.DETECTOR.B.bL1XJ)      vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//�м䴫����ѹ�������ܣ�����---ֱ��
				else if(strDevKVar.DETECTOR.B.bR1XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//�м䴫����ѹ�������ܣ�����---ֱ��
				else if(strDevKVar.DETECTOR.B.bL2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_FAST);//��ഫ����ѹ�������ܣ��Һ���-----�ҹ���
				else if(strDevKVar.DETECTOR.B.bR2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, -strDevCfg.XJPARA.cMaxTurnSpeed, TURN_FAST);//�Ҳഫ����ѹ�������ܣ��Һ���-----�����
				else                                 vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_SLOW, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//�޷��ж�----ֱ��
			}
			else
			{//����ģʽ
				if(strDevKVar.DETECTOR.B.bL1XJ)      vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, -strDevCfg.XJPARA.cMaxTurnSpeed, TURN_FAST);//��ഫ����ѹ���Һ���--������ת
				else if(strDevKVar.DETECTOR.B.bR1XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_FAST);//�Ҳഫ����ѹ���Һ���--������ת
				else if(strDevKVar.DETECTOR.B.bL2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, -strDevCfg.XJPARA.cMaxTurnSpeed, TURN_SLOW);//��ഫ����ѹ���Һ���--������ת
				else if(strDevKVar.DETECTOR.B.bR2XJ) vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_SLOW);//�Ҳഫ����ѹ���Һ���--������ת
				else                                 vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, +strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);//�޷��ж�----ֱ��
			}
			break;			
			
		default:
			vGenMotorPwm(strDevCfg.XJPARA.cMaxGoSpeed, GO_FAST, strDevCfg.XJPARA.cMaxTurnSpeed, TURN_ZERO);
			break;			
	}
}


