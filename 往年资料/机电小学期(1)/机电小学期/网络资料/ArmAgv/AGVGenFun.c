/*
 * ES_GenFun.c
 *
 *  Created on: 2017-6-26
 *      Author: kesaihu
 */
#include "STM32Type.h"

void vCopyMemAsUINT8(UINT8 *csrc, UINT8 *ctgt, UINT16 intsize)
{
	UINT16 idx;
	for(idx=0; idx<intsize; idx++)
	{
		*ctgt++ = *csrc++;
	}
}

//===========================================================================
// No more.
