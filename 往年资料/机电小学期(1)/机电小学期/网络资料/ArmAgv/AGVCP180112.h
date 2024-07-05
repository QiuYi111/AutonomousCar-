#ifndef _AGVCP_H_
#define _AGVCP_H_

	//////////////////////////////////////////////////////////////////
	#define DEVSN_DAFAULT		0X0003
	
	#define DEVTYPE_NULL		0XFFFF//
	#define DEVTYPE_MEELAB_AGV	0XC3A5//device type
	#define CMDFMID_NULL		0XFF
	#define RESFMID_NULL		0XFF
	#define SIZE_FM_MIN			6
	#define VER_SOFTWARE		0X20180125
	#define VER_HARDWARE		0X20150101
	#define PORT_TCPLINK		20181
	
	#define PROC_DEBUG			0X00
	#define PROC_XUNJI			0X01
	
	#define XJDETECTOR_TWO		2
	#define XJDETECTOR_THREE	3
	#define XJDETECTOR_FOUR		4

	typedef struct
	{
		INT16   cLFM_TurnRight : 8;
		INT16   cLFM_TurnLeft  : 8;
		INT16   cLBM_TurnRight : 8;
		INT16   cLBM_TurnLeft  : 8;
		INT16   cRFM_TurnRight : 8;
		INT16   cRFM_TurnLeft  : 8;
		INT16   cRBM_TurnRight : 8;
		INT16   cRBM_TurnLeft  : 8;
	}STRN_CFG_RLSCOFF;//转弯与执行协同时电机分配系数
	
	typedef struct
	{		
		UINT16  bIsStampWire  : 1;//1=压线模式, 0=夹线模式
		UINT16  bR2XJBeepEn : 1;
		UINT16  bR1XJBeepEn : 1;
		UINT16  bL1XJBeepEn : 1;
		UINT16  bL2XJBeepEn : 1;		
		UINT16  cNoUse0       : 3;
		UINT16  cDectorSel    : 8;//传感器个数

		INT16   cMaxGoSpeed   : 8;//循迹时直线最大速度
		INT16   cMaxTurnSpeed : 8;//循迹时转弯最大速度
	}STRN_CFG_XJPARA;

	typedef struct
	{//system config
		UNN_UINT16_2BYTES unnDevSN;
		STRN_CFG_RLSCOFF  RLSCOFF;
		STRN_CFG_XJPARA   XJPARA;
	}STRN_DEV_CFG;

	#define PWM_MAXTWC		+125//最大PWM常数

	typedef struct
	{
		INT8   cLFM_Pwm;
		INT8   cLBM_Pwm;
		INT8   cRFM_Pwm;
		INT8   cRBM_Pwm;
	}STRN_MOTOR_PWM;

	typedef union
	{
		struct
		{
			UINT16  bR2XJ   : 1;  //最低位
			UINT16  bR1XJ   : 1;  //trace reflector indiction
			UINT16  bL1XJ   : 1;  //trace reflector indiction
			UINT16  bL2XJ   : 1;  //trace reflector indiction
			UINT16  cNoUse0 : 3;
			UINT16  bRunDetector : 1;//use true detector			

			UINT16  bLBZ   : 1;  //trace reflector indiction
			UINT16  bRBZ   : 1;  //trace reflector indiction
			UINT16  cNoUse1 :6;
		}B;
		struct
		{
			UINT16  cXJDector : 8; //trace reflector indiction
			UINT16  cBZDector : 8; //trace reflector indiction
		}C;
	}UNN_DEV_DETECTOR;	

	typedef struct
	{
		UINT16 cMainProc  :  8;
		UINT16 cSubProc   :  8;
		STRN_MOTOR_PWM    PWM;
		UNN_DEV_DETECTOR  DETECTOR;
	}STRN_DEV_KVAR;

	//////////////////////////////////////////////////////////////////
	typedef struct
	{//uart frame head 4 bytes
		UINT16  iFmHead;     //??EB90
		UINT16  cFmBLen   :8;//?8bit
		UINT16  cFmID     :8;//?8bit
	}STRN_FM_GENHEAD;

	#define	CMDFMID_TSTALT	0X00
	typedef struct
	{//command: set test mode
		UINT16  iFmHead;      //frame head: EB90
		UINT16  cFmBLen   :8; //frame length(low 8bits)
		UINT16  cFmID     :8; //frame identification(high 8bits)
		UINT16  iTstMode;     //enable test
		UINT16 	cNoUse    :8; //no meaning
		UINT16 	cChkSum   :8; //check sum for whole frame
	}STRN_CMDFM_TSTALT;
	#define	SIZE_CMDFM_TSTALT	sizeof(STRN_CMDFM_TSTALT)

	#define	CMDFMID_GETRESFM	0X01
	typedef struct
	{//command: get specified response frame
		UINT16  iFmHead;      //frame head: EB90
		UINT16  cFmBLen   :8; //frame length(low 8bits)
		UINT16  cFmID     :8; //frame identification(high 8bits)

		UINT16	cResID    :8; //response frame ID
		UINT16 	cChkSum   :8; //whole frame check sum
	}STRN_CMDFM_GETRESFM;
	#define	SIZE_CMDFM_GETRESFM	sizeof(STRN_CMDFM_GETRESFM)
		
	#define	CMDFMID_SIMDETECTOR    0X02
	typedef struct
	{//command: set motor pwm to tune speed
		UINT16  iFmHead;        //frame head: EB90
		UINT16  cFmBLen   :8; //frame length(low 8bits)
		UINT16  cFmID     :8; //frame identification(high 8bits)
		
		UNN_DEV_DETECTOR  DETECTOR;

		UINT16	cResID    :8; //response frame ID
		UINT16 	cChkSum   :8; //whole frame check sum
	}STRN_CMDFM_SIMDETECTOR;
	#define	SIZE_CMDFM_SIMDETECTOR	sizeof(STRN_CMDFM_SIMDETECTOR)	
		
	#define	CMDFMID_DBGMOTORPWM	0X03
	typedef struct
	{//command: set motor pwm to tune speed
		UINT16  iFmHead;        //frame head: EB90
		UINT16  cFmBLen   :8; //frame length(low 8bits)
		UINT16  cFmID     :8; //frame identification(high 8bits)
		
		STRN_MOTOR_PWM    PWM;

		UINT16	cResID    :8; //response frame ID
		UINT16 	cChkSum   :8; //whole frame check sum
	}STRN_CMDFM_DBGMOTORPWM;
	#define	SIZE_CMDFM_DBGMOTORPWM	sizeof(STRN_CMDFM_DBGMOTORPWM)
		
	#define	CMDFMID_DBGDEVSPEED	0X04
	typedef struct
	{//command: set motor pwm to tune speed
		UINT16  iFmHead;        //frame head: EB90
		UINT16  cFmBLen   :8; //frame length(low 8bits)
		UINT16  cFmID     :8; //frame identification(high 8bits)
		
		INT16   cTurnSpeed   :8; //turn speed
		INT16   cGoSpeed  :8; //go straight speed

		UINT16	cResID    :8; //response frame ID
		UINT16 	cChkSum   :8; //whole frame check sum
	}STRN_CMDFM_DBGDEVSPEED;
	#define	SIZE_CMDFM_DBGDEVSPEED	sizeof(STRN_CMDFM_DBGDEVSPEED)

	#define	CMDFMID_SETRLSCOFF	0X05
	typedef struct
	{//command: set motor pwm to tune speed
		UINT16  iFmHead;        //frame head: EB90
		UINT16  cFmBLen   :8; //frame length(low 8bits)
		UINT16  cFmID     :8; //frame identification(high 8bits)
		
		STRN_CFG_RLSCOFF	RLSCOFF;

		UINT16	cResID    :8; //response frame ID
		UINT16 	cChkSum   :8; //whole frame check sum
	}STRN_CMDFM_SETRLSCOFF;
	#define	SIZE_CMDFM_SETRLSCOFF	sizeof(STRN_CMDFM_SETRLSCOFF)	

	#define	CMDFMID_SETXJPARA	0X06
	typedef struct
	{//command: set motor pwm to tune speed
		UINT16  iFmHead;        //frame head: EB90
		UINT16  cFmBLen   :8; //frame length(low 8bits)
		UINT16  cFmID     :8; //frame identification(high 8bits)
		
		STRN_CFG_XJPARA   XJPARA;

		UINT16	cResID    :8; //response frame ID
		UINT16 	cChkSum   :8; //whole frame check sum
	}STRN_CMDFM_SETXJPARA;
	#define	SIZE_CMDFM_SETXJPARA	sizeof(STRN_CMDFM_SETXJPARA)

	#define	CMDFMID_XJGO		0X07
	typedef struct
	{//command: set motor pwm to tune speed
		UINT16  iFmHead;        //frame head: EB90
		UINT16  cFmBLen   :8; //frame length(low 8bits)
		UINT16  cFmID     :8; //frame identification(high 8bits)

		UINT16	cResID    :8; //response frame ID
		UINT16 	cChkSum   :8; //whole frame check sum
	}STRN_CMDFM_XJGO;
	#define	SIZE_CMDFM_XJGO	sizeof(STRN_CMDFM_XJGO)	
		
	// ================================================
	#define RESFMID_GENDEV		0X80
	typedef struct
	{//response: set test mode
		UINT32  iFmHead   :16;      //frame head: EB90
		UINT32  cFmBLen   :8; //frame length(low 8bits)
		UINT32  cFmID     :8; //frame identification(high 8bits)

		UINT32  lSoftVer;     //software version
		UINT32  lHardVer;     //hardware version

		UINT32	iDevType  :16;     //device type
		UINT32 	cFmSN     :8; //no meaning
		UINT32 	cChkSum   :8; //whole frame check sum
	}STRN_RESFM_GENDEV;
	#define SIZE_RESFM_GENDEV	sizeof(STRN_RESFM_GENDEV)

	#define RESFMID_DEVCFG		0X81
	typedef struct
	{//response: set test mode
		UINT32  iFmHead   :16;//frame head: EB90
		UINT32  cFmBLen   :8; //frame length(low 8bits)
		UINT32  cFmID     :8; //frame identification(high 8bits)

		STRN_DEV_CFG      CFG;//

		UINT32 	cFmSN     :8; //no meaning
		UINT32 	cChkSum   :8; //whole frame check sum
	}STRN_RESFM_DEVCFG;
	#define SIZE_RESFM_DEVCFG	sizeof(STRN_RESFM_DEVCFG)

	#define RESFMID_DEVKVAR		0X82
	typedef struct
	{//response: set test mode
		UINT32  iFmHead   :16;//frame head: EB90
		UINT32  cFmBLen   :8; //frame length(low 8bits)
		UINT32  cFmID     :8; //frame identification(high 8bits)

		STRN_DEV_KVAR     KVAR;//

		UINT32 	cFmSN     :8; //no meaning
		UINT32 	cChkSum   :8; //whole frame check sum
	}STRN_RESFM_DEVKVAR;
	#define SIZE_RESFM_DEVKVAR	sizeof(STRN_RESFM_DEVKVAR)

#endif
