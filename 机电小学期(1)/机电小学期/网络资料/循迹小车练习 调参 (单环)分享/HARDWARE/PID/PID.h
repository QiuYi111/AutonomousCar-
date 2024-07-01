#ifndef __PID_H
#define __PID_H



typedef struct
{
	float Sv;
	float Kp;
	float Kd;
	float Ki;
	float EK;
	float EK_1;//�ϴ�ƫ��
	float SEK;//��ʷƫ��֮��
	float OUT;

}PID;


extern PID pid;

void PID_Calc();
//void  Position_P();
void PID_Init();
void PID_OUT();



//int Position_PID (int Encoder,int Target);   //����PID������//����Encoder=error��Target=0;




#endif
