#include "imu.h"

uint8_t angle1 = 0;
uint8_t angle2 = 0;
uint8_t acc1 = 0;
uint8_t acc2 = 0;
uint8_t w1 = 0;
uint8_t w2 = 0;
uint8_t rx_data;
uint8_t YawL;
uint8_t YawH;
uint8_t RollL;
uint8_t RollH;
uint8_t PitchL;
uint8_t PitchH;
uint8_t axL;
uint8_t axH;
uint8_t ayL;
uint8_t ayH;
uint8_t azL;
uint8_t azH;
uint8_t wxL;
uint8_t wxH;
uint8_t wyL;
uint8_t wyH;
uint8_t wzL;
uint8_t wzH;
uint8_t g = 9.8;
short ax;
short ay;
short az;
short Pitch;
short Roll;
short Yaw;
short wx;
short wy;
short wz;

void get_angle(){
if (angle1 == 0) {
				if (rx_data == 0x55) {
					angle1 = 1;
				}
			}
			else {
				if (angle2 == 0) {
					if (rx_data == 0x53) {
						angle2= 1;
					}
					else {
						angle1 = 0;
					}
				}
			else {
				for (uint8_t i = 0; i < 6; i++) {
					if (i == 0) { RollL = rx_data;}
					if (i == 1) { RollH = rx_data;}
					if (i == 2) { PitchL = rx_data;}
					if (i == 3) { PitchH = rx_data;}
					if (i == 4) { YawL = rx_data; }
					if (i == 5) { YawH = rx_data; }
					HAL_UART_Receive(&huart2, &rx_data, 1, HAL_MAX_DELAY);
				}

				Roll = ((short)RollH << 8 | RollL) / 32768.0 * 180;
				Pitch = ((short)PitchH << 8 | PitchL) / 32768.0 * 180;
				Yaw = ((short)YawH << 8 | YawL) / 32768.0 * 180;

				angle1 = 0;
				angle2 = 0;
			}
		}
}

void get_acc(){
    if (acc1 == 0) {
					if (rx_data == 0x55) {
						acc1 = 1;
					}
				}
				else {
					if (acc2 == 0) {
						if (rx_data == 0x51) {
							acc2= 1;
						}
						else {
							acc1 = 0;
						}
					}
				else {
					for (uint8_t i = 0; i < 6; i++) {
						if (i == 0) { axL = rx_data;}
						if (i == 1) { axH = rx_data;}
						if (i == 2) { ayL = rx_data;}
						if (i == 3) { ayH = rx_data;}
						if (i == 4) { azL = rx_data; }
						if (i == 5) { azH = rx_data; }
						HAL_UART_Receive(&huart2, &rx_data, 1, HAL_MAX_DELAY);
					}


					ax = ((short)axH << 8 | axL) / 32768.0 * 16 * 9.8;
					ay = ((short)ayH << 8 | ayL) / 32768.0 * 16 * 9.8;
					az = ((short)azH << 8 | azL) / 32768.0 * 16 * 9.8;


					printf("\n ax: %d",ax);
					printf("\n ay: %d",ay);
					printf("\n az: %d",az);

					acc1 = 0;
					acc2 = 0;
				}
			}
}

void get_omega(){
    if (w1 == 0) {
					if (rx_data == 0x55) {
						w1 = 1;
					}
				}
				else {
					if (w2 == 0) {
						if (rx_data == 0x52) {
							w2= 1;
						}
						else {
							w1 = 0;
						}
					}
				else {
					for (uint8_t i = 0; i < 6; i++) {
						if (i == 0) { wxL = rx_data;}
						if (i == 1) { wxH = rx_data;}
						if (i == 2) { wyL = rx_data;}
						if (i == 3) { wyH = rx_data;}
						if (i == 4) { wzL = rx_data; }
						if (i == 5) { wzH = rx_data; }
						HAL_UART_Receive(&huart2, &rx_data, 1, HAL_MAX_DELAY);
					}


					wx = ((short)wxH << 8 | wxL) / 32768.0 * 16 * 9.8;
					wy = ((short)wyH << 8 | wyL) / 32768.0 * 16 * 9.8;
					wz = ((short)wzH << 8 | wzL) / 32768.0 * 16 * 9.8;


					printf("\n wx: %d",wx);
					printf("\n wy: %d",wy);
					printf("\n wz: %d",wz);

					w1 = 0;
					w2 = 0;
				}
			}
		}