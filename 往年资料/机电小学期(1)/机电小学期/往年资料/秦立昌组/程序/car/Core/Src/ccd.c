#include "ccd.h"
/*
 * ccd.c
 *
 *  Created on: May 25, 2022
 *      Author: qcllc
 */
int ADC_Value = 0;
int ADV[128];
int bool_array[128];
//int result[4];
int result[2];
short last_pos = 64;
short mode = 0;
short count = 0;
short P_count = 0;
short edge_count = 0;
int special = 0;
int Get_Adc(void)
{
	HAL_ADC_Start(&hadc1);//ADC1�????????始采�????????
 if(HAL_ADC_PollForConversion(&hadc1,10) == HAL_OK)//判断ADC采集是否完成
  {
   ADC_Value=HAL_ADC_GetValue(&hadc1);//读取ADC的�??
  }

 HAL_ADC_Stop(&hadc1);//停止ADC采样
 return ADC_Value;
}

void filter(int gray_array[], int bool_array[])
{
	int i;
	int biggest = -1;
	int smallest = 5000;
	// find biggest and smallest
	for (i = 1; i < 127; i ++)
	{
		gray_array[i] = (gray_array[i-1]+gray_array[i]*2+gray_array[i+1])/4;
		if (gray_array[i] > biggest)
		{
			biggest = gray_array[i];
		}
		if (gray_array[i] < smallest)
		{
			smallest = gray_array[i];
		}
	}
	// loop and normalize and binarize
	for (i = 0; i < 128; i ++)
	{
		if (((float)gray_array[i] - (float)smallest) / ((float)biggest-(float)smallest) < 0.2)
		{
			bool_array[i] = 1;
		}
		else
		{
			bool_array[i] = 0;
		}
	}
//	double area = ((float)biggest-(float)smallest);
//	int temp = 0;
//	for(i = 1;i<127;i++)
//	{
//		if((float)abs(gray_array[i]-gray_array[i-1])/area>0.2)
//			temp = 1-temp;
//		bool_array[i] = temp;
//	}
	return 0;
}

void count_continuous(int bool_array[], int result[])
{
	// init
	int i;
	short line_L = 0;
	short width_L = 0;
	short line_R = 128;
	short width_R = 0;
	short line_LL = 0;
	short width_LL = 0;
	short line_RR = 128;
	short width_RR = 0;
	// set right and left corner to 0 to avoid error
	//bool_array[0] = 0;
	//bool_array[127] = 0;
	// count
	short is_line = 0;
	if(special == 1)
	{
		last_pos = 64;
		special = 0;
	}
	for(i = last_pos;i<127;i++)
	{
		if(bool_array[i]&&bool_array[i+1]&&(!is_line))
		{
			if(line_R==128)
			{
				line_R = i;
				is_line = 1;
				width_R++;
			}
			else if(line_RR==128)
			{
				line_RR=i;
				is_line = 2;
				width_RR++;
			}
		}
		else if(bool_array[i]&&is_line==1)
		{
			width_R++;
		}
		else if(!bool_array[i]&&is_line==1)
		{
			is_line =0;
		}
		else if(bool_array[i]&&is_line==2)
		{
			width_RR++;
		}
		else if(!bool_array[i]&&is_line==2)
		{
			is_line =0;
		}
	}
	is_line = 0;
	for(i = last_pos;i>0;i--)
	{
		if(bool_array[i]&&bool_array[i-1]&&(!is_line))
		{
			if(line_L==0)
			{
				line_L = i;
				is_line = 1;
				width_L++;
			}
			else if(line_LL==0)
			{
				line_LL= i;
				is_line = 2;
				width_LL++;
			}
		}
		else if(bool_array[i]&&is_line==1)
		{
			width_L++;
		}
		else if(!bool_array[i]&&is_line==1)
		{
			is_line =0;
		}
		else if(bool_array[i]&&is_line==2)
		{
			width_LL++;
		}
		else if(!bool_array[i]&&is_line==2)
		{
			is_line =0;
		}
	}

	// save to result
	if(mode == 0)
	{
		result[0] = line_L-width_L/2;
		result[1] = line_R+width_R/2;
		if(line_L == 0 && line_RR!=128)
		{
			result[0] = line_RR-width_RR/2;
		}
		if(line_R == 128 && line_LL!=0)
		{
			result[1] = line_LL-width_LL/2;
		}
//		result[2] = second_longest_index;
//		result[3] = second_longest_length;
//		last_pos = abs(result[0]+result[1]/2+result[2]+result[3]/2)/2-64;
		last_pos = (result[0]+result[1])/2;
	}
	if(abs(mode) == 1)//1 left -1 right
	{
		result[0] = line_L-width_L/2;
		result[1] = line_R+width_R/2;
		if(count<3)
		{
			special == 1;
			if(count==0)
			{
				if(mode == 1)
					last_pos = 32;
				else
					last_pos = 110;
				count++;
			}
			else
			{
				if(result[0]==0||result[1]==128)
				{
					if(P_count)
						count++;
					if(count==3)
						last_pos = 64;
				}
			}

		}
		else if(count)
		{
			if(line_L-width_L<=0)
				result[0] = line_RR+width_RR/2;
			else if(line_R+width_R>=128)
				result[1] = line_LL-width_LL/2;
			if(abs(last_pos-result[0])>abs(last_pos-result[1]))
				last_pos = result[1];
			else
				last_pos = result[0];

		//		result[2] = second_longest_index;
		//		result[3] = second_longest_length;
		//		last_pos = abs(result[0]+result[1]/2+result[2]+result[3]/2)/2-64;
		if(abs(get_camera_pos()-target_pos)<10)
		{
			if(P_count)
				count++;
			if (count ==5)
			{
				count = 0;
				mode = 2*mode;
			}
		}
		}
	}
	if(abs(mode) == 2)
	{
		special = 1;
		result[0] = line_L-width_L/2;
		result[1] = line_R+width_R/2;
		if(line_L-width_L <=0)
			result[0]  =result[1];
		if(line_R+width_R >=128)
			result[1]  =result[0];
		if(abs(last_pos-result[0])>abs(last_pos-result[1]))
			last_pos = result[1];
		else
			last_pos = result[0];
		if(abs(get_camera_pos()-target_pos)<15)
		{
			target_pos+=mode/2;
			if(target_pos>5)
			{
				target_pos = 5;
			}
			if(target_pos<-5)
			{
				target_pos = -5;
			}
		}
	}
	if(abs(mode) == 3)
	{
		result[0] = line_L-width_L/2;
		result[1] = line_R+width_R/2;
		if(line_L-width_L<=0)
			result[0] = line_RR+width_RR/2;
		else if(line_R+width_R>=128)
			result[1] = line_LL-width_LL/2;
		if(abs(last_pos-result[0])>abs(last_pos-result[1]))
			last_pos = result[1];
		else
			last_pos = result[0];
		if(abs(get_camera_pos()-target_pos)<20)
		{
			if(P_count)
			{
				count++;
				if(count%10==0)
					target_pos-=mode/3*2;
			}
			if(target_pos>20)
			{
				mode = 0;
				count = 0;
				special = 1;
				target_pos = 0;
			}
			if(target_pos<-20)
			{

				mode = 0;
				count = 0;
				special = 1;
				target_pos = 0;

			}

		}
	}
//	if(abs(get_camera_pos())==64)
//	{
//		edge_count++;
//		if(edge_count == 20)
//		{
//			edge_count = 0;
//			last_pos =64;
//		}
//	}
	if(P_count==1)
		P_count  = 0;
	return 0;
}

void camera_take()
{
	int i=0,tslp=0;
	HAL_GPIO_WritePin(GPIOC, CLK_Pin|CLK_Pin|SI_Pin|SI_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOC,CLK_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,SI_Pin,GPIO_PIN_RESET);
		  	  //delay();
	/*
		  	  HAL_GPIO_WritePin(GPIOC,CLKR_Pin,GPIO_PIN_SET);
		  	  HAL_GPIO_WritePin(GPIOC,SIR_Pin,GPIO_PIN_RESET);
		  	  //delay();*/

	HAL_GPIO_WritePin(GPIOC,SI_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,CLK_Pin,GPIO_PIN_RESET);
		  	  //delay();
	/*
		  	  HAL_GPIO_WritePin(GPIOC,CLKR_Pin,GPIO_PIN_RESET);
		  	  HAL_GPIO_WritePin(GPIOC,SIR_Pin,GPIO_PIN_SET);
		  	  //delay();*/

	HAL_GPIO_WritePin(GPIOC,CLK_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,SI_Pin,GPIO_PIN_RESET);
		  	  //delay();
	/*
		  	  HAL_GPIO_WritePin(GPIOC,CLKR_Pin,GPIO_PIN_SET);
		  	  HAL_GPIO_WritePin(GPIOC,SIR_Pin,GPIO_PIN_RESET);
		  	  //delay();*/

	for(i=0;i<128;i++)
	{
		HAL_GPIO_WritePin(GPIOC,CLK_Pin, GPIO_PIN_RESET);
		  		  //delay();
		//ADV[tslp]=(float)Get_Adc();
		ADV[tslp]=(float)Get_Adc()*((float)(abs(i-64))/96+1);

		++tslp;
		HAL_GPIO_WritePin(GPIOC,CLK_Pin, GPIO_PIN_SET);
		  		 // delay();
	}
		  	  // filter
	filter(ADV, bool_array);
	count_continuous(bool_array, result);

}

int get_camera_pos()
{
	return last_pos-64;
}

short PID_cal_rot(short target_rot,short rot,short *error)
{
	double error_rot = target_rot - rot;
	static double error_last_rot = 0,error_prev_rot = 0;
	double rot_add=0;
	*error = error_rot;
	rot_add = KPR*(error_rot - error_last_rot) + KIR*error_rot + KDR*(error_rot-2.0f*error_last_rot+error_prev_rot);

	error_prev_rot = error_last_rot;	  	    // 保存上上次误差
	error_last_rot = error_rot;	              // 保存上次偏差

	return (short)rot_add;
}
