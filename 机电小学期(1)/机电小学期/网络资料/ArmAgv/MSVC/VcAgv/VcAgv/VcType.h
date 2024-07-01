#ifndef _VCTYPE_H_
#define _VCTYPE_H_

	typedef unsigned char UINT8;
	typedef signed   char INT8;

	typedef unsigned short UINT16;
	typedef signed   short INT16;

	typedef unsigned int UINT32;
	typedef signed   int INT32;
	typedef __int64      INT64;


	typedef union
	{
		UINT8  cBytes[8];
		INT16  SI16[2];
		UINT16 UI16[2];
		INT32  SI32;
		UINT32 UI32;
		struct
		{
			UINT16 LW;
			UINT16 HW;
		}UPART;
		struct
		{
			INT16 LW;
			INT16 HW;
		}SPART;
	}UN32;

	typedef union
	{
		UINT16  LOW : 8;
		UINT16  HIGH : 8;
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
		UINT8 cBytes[8];
		struct
		{
			UINT16 iIntgers[4];
		}INT16N;
		struct
		{
			UINT32 iLong[2];
		}INT32N;
		__int64	INT64N;
		__int64	SI64;
	}UN64;	

#endif
