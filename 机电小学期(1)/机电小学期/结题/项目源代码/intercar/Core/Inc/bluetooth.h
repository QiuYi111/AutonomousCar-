/*
 * bluetooth.h
 *
 *  Created on: Jul 10, 2022
 *      Author: 86152
 */

#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_

#endif /* INC_BLUETOOTH_H_ */
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "control.h"
#include "stdio.h"
#define RXBUFFER_LEN 5

int Rx_len;
int Rx_flag;
uint8_t Rx_byte;
uint8_t Rx_command;
uint8_t Rx_buffer[RXBUFFER_LEN];
short mode;
short MODE;
extern short avoid_dir;

void extract_RX();//蓝牙遥控命令提取函数
void mode_switch(char command);  //遥控改变模式
void remote_control(char command);//运动遥控
