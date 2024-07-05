#ifndef __HY_SRF05_H
#define __HY_SRF05_H



#define Trig  GPIO_Pin_5    //������  PB5
#define Echo  GPIO_Pin_1	//���ն�  PB1

// ��ʱ�����벶���û��Զ�������ṹ������
typedef struct
{   
	uint8_t   Capture_FinishFlag;   // ���������־λ
	uint8_t   Capture_StartFlag;    // ����ʼ��־λ
	uint16_t  Capture_CcrValue;     // ����Ĵ�����ֵ
	uint16_t  Capture_Period;       // �Զ���װ�ؼĴ������±�־ 
}TIM_ICUserValueTypeDef;

extern TIM_ICUserValueTypeDef TIM_ICUserValueStructure;


void HY_SRF05_ceju();

void HY_SRF05_Config();

#endif
