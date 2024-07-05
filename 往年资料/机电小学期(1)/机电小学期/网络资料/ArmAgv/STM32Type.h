#ifndef	_STM32TYPE_H_	
#define _STM32TYPE_H_

	#include "stm32f10x.h"
	#define HBOOL 	int8_t
	
	#define UINT8	uint8_t
	#define INT8	int8_t	
	#define UINT16	uint16_t
	#define INT16	int16_t
	#define UINT32	uint32_t
	#define INT32	int32_t
	
	//#define false	0
	//#define true	1

	#define HFALSE	0
	#define HTRUE	1
		
	typedef union
	{
		struct
		{
			UINT16  LOW : 8;
			UINT16  HIGH : 8;
		}C;
		struct
		{
			UINT16  I;
		}I;
	}UNN_UINT16_2BYTES;	

	typedef union
	{
		struct
		{
			UINT32  LOW : 8;
			UINT32  MIDLOW : 8;
			UINT32  MIDHIGH : 8;
			UINT32  HIGH : 8;
		}C;
		struct
		{
			UINT32  I;
		}I;
	}UNN_UINT32_4BYTES;

	typedef union
	{
		INT16  	SI16;
		UINT16  UI16;
	}UNN_VAR_USW;

	typedef struct
	{
		UINT16 ULW;
		UINT16 UHW;
	}STRN_VAR_UDW;

	typedef struct
	{
		INT16 SLW;
		INT16 SHW;
	}STRN_VAR_SDW;

	typedef union
	{
		INT32  SI32;
		UINT32 UI32;
		STRN_VAR_UDW UDW;
		STRN_VAR_SDW SDW;
	}UN32;

	typedef union
	{
		float f;
		UINT32  UI32;
		INT32   SI32;
		STRN_VAR_UDW UDW;
		STRN_VAR_SDW SDW;
	}UN4BYTES;	
	
#endif
