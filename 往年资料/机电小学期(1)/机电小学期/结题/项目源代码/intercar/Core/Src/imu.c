/*
 * imu.c
 *
 *  Created on: Jul 24, 2022
 *      Author: 86152
 */
#include "imu.h"

void JY901_Process()
{
	for(int i=0;i<IMUBUFFER_LEN;i++){
	    if(IMURxBuffer[i]== 0x55 && IMURxBuffer[i+1]==0x53)
	    {
	    	if(imu_check==0){
	    	    theta0 =(float)(((short)IMURxBuffer[i+7]<<8)|IMURxBuffer[i+6])/32768*180;
	    	    imu_check=1;
	    	}
	    	else if(imu_check==1){theta=((float)(((short)IMURxBuffer[i+7]<<8)|IMURxBuffer[i+6])/32768*180)-theta0;}
	    }//官方角度解算
	    IMURxBuffer[i]=0;
	}
}

