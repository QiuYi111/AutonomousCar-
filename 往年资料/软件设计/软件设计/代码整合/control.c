/*
 * control.c
 *
 *  Created on: Aug 1, 2023
 *      Author: 21671
 */

#include"motor.h"
#include"control.h"

void set_car_speed(int8_t direction, int speed, int radius){ //第一个参数为±1， 1表示左转，-1表示右转
	float omega = (float)speed / (float)radius;
	int v1 = speed - (int)omega * car_width / 2;
	int v2 = speed + (int)omega * car_width / 2;
	if(direction > 0){
		set_wheel1_speed(v1);
		set_wheel2_speed(v2);
	}
	else{
		set_wheel1_speed(v2);
		set_wheel2_speed(v1);
	}
}
