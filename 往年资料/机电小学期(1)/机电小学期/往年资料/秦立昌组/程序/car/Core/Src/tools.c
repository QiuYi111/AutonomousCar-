/*
 * tools.c
 *
 *  Created on: May 11, 2022
 *      Author: qcllc
 */

#include "tools.h"

void excute_command(char *command)
{
	int value_1 = 0;
	int value_2 = 0;
	char command_type = command[0];
	char command_value_1[10];
	char command_value_2[10];
	short command_num = 0;
	uint8_t j = 0;
	for(uint8_t i = 2;i < RXBUFFER_LEN && command[i]!='\0'&& j<9 ; i++)
	{
		if(command[i] == ' ')
		{
			command_value_1[j] = '\0';
			command_num = 1;
			j = 0;
			i++;
		}
		if(command_num == 0)
		{command_value_1[j] = command[i];}
		else{command_value_2[j] = command[i];}
		j++;
	}
	command_value_2[j] = '\0';
	sscanf(command_value_1,"%d",&value_1);
	if(command_num)
	{
		sscanf(command_value_2,"%d",&value_2);
	}
	switch(command_type)
	{
	case 'D'://drive
	{
		drive(value_1, value_2);
		mode = 0;
		last_pos = 64;
		break;
	}
	case 'S'://stop
	{
		started = 0;
		drive(0,0);
		motor_PWM(0,0);
		mode = 0;
		echo_distance = 1000;
		count =0;
		break;
	}
	case 'B'://begin
	{
		started = 1;
		mode = 0;
		target_pos = 0;
		break;
	}
	case 'L'://turn left
	{
		mode = 1;
		count = 0;
		break;
	}
	case 'R'://turn left
	{
		mode = -1;
		count = 0;
		break;
	}
	case 'M':
	{
		target_pos = 0;
		if(mode>0)
			mode = 3;
		if(mode <0)
			mode = -3;
		count = 0;
		break;
	}
	}
}



char *reverse(char *s)
{
    char temp;
    char *p = s;    //p指向s的头部
    char *q = s;    //q指向s的尾部
    while(*q)
        ++q;
    q--;

    //交换移动指针，直到p和q交叉
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    }
    return s;
}

/*
 * 功能：整数转换为字符串
 * char s[] 的作用是存储整数的每一位
 */
int int2char(char *arr, int num, int arr_size)
{
        if(arr_size <= 0)
        return -1;

        int i=0;
        /*待填充字符的下标*/

        int symbol = 1;
        int base = 1;

        if(num < 0){
                *(arr+0)='-';
                i++;
                symbol = -1;
        }

                while(num/base >= 10 || num / base <= -10)
                        base *= 10;
                /* 计算num的基数base, 例如:
                 * num = 0  , base = 1;
                 * num = 112, base = 100;
                 * num = -3400, base = 1000;
                 */

                for(; i < arr_size - 2 && base != 0; i++){
                        *(arr+i) = (num / base)*symbol + 48;
                        num    %= base;
                        base   /= 10;
                }
        *(arr+i) = '\n';
        *(arr+i+1) = '\0';
        return i;
}

//void float2char(char *str, double number, uint8_t g, uint8_t l)
//{
//    uint8_t i;
//    int temp = number/1;
//    double t2 = 0.0;
//    for (i = 1; i<=g; i++)
//    {
//        if (temp==0)
//            str[g-i] = table[0];
//        else
//            str[g-i] = table[temp%10];
//        temp = temp/10;
//    }
//    *(str+g) = '.';
//    temp = 0;
//    t2 = number;
//    for(i=1; i<=l; i++)
//    {
//        temp = t2*10;
//        str[g+i] = table[temp%10];
//        t2 = t2*10;
//    }
//    *(str+g+l+1) = '\n';
//    *(str+g+l+2) = '\0';
//}
