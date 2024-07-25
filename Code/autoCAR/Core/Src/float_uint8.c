/*
 * float_uint8.c
 *
 *  Created on: Jul 24, 2024
 *      Author: 25138
 */
#include "float_uint8.h"
int float_to_uint8_arry(uint8_t* u8Arry, float floatdata, int precision) {//float给定精度转换为uint8_t
	int points = 0;
	int apoints = 0;
	float data1 = 0;
	float data2=0;
	int if_neg=0;
	if(floatdata<0){
			u8Arry[0]=0;
			data1=-floatdata;
			data2=-floatdata;
			if_neg=1;
		}
	else{
		data1=floatdata;
		data2=floatdata;
	}
	for (; data1 >= 1; points++) {
		data1 /= 10;
	}
	if (points == 0) {
		points = 1;
		apoints = 1;
	}
	for (int i = 0; i < precision; i++) {
		data2 *= 10;
	}
	for (int i = 0; i < points + precision + 1; i++) {
		if (i != precision) {
			if (i == 0) {
				u8Arry[points + precision +if_neg - i] = (uint8_t)((floor(fmod(data2, 10) + 0.5)) + '0');
				data2 /= 10;
			}
			else {
				u8Arry[points + precision +if_neg - i] = (uint8_t)((floor(fmod(data2, 10))) + '0');
				data2 /= 10;
			}
		}
		else if(precision!=0){
			u8Arry[points] = '.';
		}

	}
	if (apoints == 1) {
		points = 0;
		return points + precision + 2+if_neg;
	}
	else if(precision==0){
		return points + precision + if_neg;
	}
	else {
		return points + precision + if_neg;
	}

}

// Function to convert uint8_t array to float
float uint8_to_float(uint8_t* u8arry, int point_length) {
	int points = 0;
	int status = 0;
	int num_length = 0;
	float number = 0;
	int if_neg=0;
	if(u8arry[0]=='-'){
		num_length++;
		if_neg=1;
	}
	for (; num_length < 256; num_length++) {
		if (u8arry[num_length] == '.') {
			points = num_length;
		}
		if (u8arry[num_length] == 'o') {
			status = num_length;
			break;
		}
	}
	if (status == 0) {
		return 0;
	}
	else {
		float tens = 1;

		if (points == 0) {
			for (size_t i= if_neg; i < num_length; i++) {
			number += (float)(u8arry[i] - '0');
			number *= 10;
		}
			}
			else {
				for (int i = if_neg; i <= points + point_length; i++) {
				if (i < points) {
					number += (float)(u8arry[i] - '0');
					number *= 10;
				}
				else if (i > points) {
					if(i<num_length){
					tens /= 10;
					number += ((float)(u8arry[i] - '0')) * tens;
					}
					else{
						number+=0;
					}
				}
				else {
					number /= 10;
				}
				}
			}

		if (points == 0) {
			number /= 10;
		}
		if(if_neg){
			return -number;
		}
		return number;
	}

}


