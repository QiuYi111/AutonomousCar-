/*
 * float_uint8.h
 *
 *  Created on: Jul 24, 2024
 *      Author: 25138
 */
#include "main.h"
#include <math.h>
#include <string.h>
#ifndef INC_FLOAT_UINT8_H_
#define INC_FLOAT_UINT8_H_
#ifdef __cplusplus
extern "C" {
#endif
int float_to_uint8_arry(uint8_t* u8Arry, float floatdata, int precision);
float uint8_to_float(uint8_t* u8arry, int point_length) ;

#ifdef __cplusplus
}
#endif
#endif /* INC_FLOAT_UINT8_H_ */
