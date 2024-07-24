/*
 * PID.c
 *
 *  Created on: Jul 21, 2023
 *      Author: 21671
 */

#include"PID.h"

int Um = 2000;

int Ukk1 = 0;
int ekk1 = 0;
int ekkk1 = 0;
float Kp1 = 2;//0.4
float Ki1 = 0.4;
float Kd1 = 0.4;

int Ukk2 = 0;
int ekk2 = 0;
int ekkk2 = 0;
float Kp2 = 2;//0.4
float Ki2 = 0.4;
float Kd2 = 0.4;

int speed_PID1(int set_speed, int now_speed){
	int ek=0,Uk=0;

	if(set_speed >= 0) ek=set_speed - now_speed;
	else ek = now_speed - set_speed;

	Uk=Ukk1 + Kp1*(ek - ekk1) + Ki1*ek + Kd1*(ek - 2*ekk1 + ekkk1);

	Ukk1=Uk;
	ekkk1=ekk1;
	ekk1=ek;

	if(Uk > Um) Uk = Um;
	if(Uk < -Um) Uk = -Um;

	return Uk;
};

int speed_PID2(int set_speed, int now_speed){
	int ek=0,Uk=0;

	if(set_speed >= 0) ek=set_speed - now_speed;
	else ek = now_speed - set_speed;

	Uk=Ukk2 + Kp2*(ek - ekk2) + Ki2*ek + Kd2*(ek - 2*ekk2 + ekkk2);

	Ukk2=Uk;
	ekkk2=ekk2;
	ekk2=ek;

	if(Uk > Um) Uk = Um;
	if(Uk < -Um) Uk = -Um;

	return Uk;
};
