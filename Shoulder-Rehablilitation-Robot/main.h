#pragma once

#define CAL_INPUT	1
#define CAL_DEGREE	1

#define ON			1
#define OFF			0

#define MOVING		1
#define DONE		0

#define FORWARD		1
#define REVERSE		0

#define True		1
#define False		0

#define STX			0x02
#define ETX			0x03
#define DTL			0x0A
#define	CMA			0x2C
#define DIGIT		10

#define	INIT		0
#define FAIL		1
#define SUCCESS		2
#define OK			3

#define PI		3.141593
#define RTD		180 / PI
#define DTR		PI / 180
//#define CENTER	2048

#define NtoKg	0.102

//#define	RoudF(T) ((float)(int)((T)*10000000) / 10000000)
#define	RoundD(T) round(T * 1000000) / 1000000
#define	RoundD03(T) round(T * 1000) / 1000
#define	FloorD(T) floor(T * 1000000) / 1000000

//**********************************
#define MOTOR_NUMBER	4 
#define DOF_NUM			7 
#define REPEAT			2
#define M_VALUE			1		// Degree

#define	TIME_RESULT		0

#define MAX_VELOCITY	6000.0


// 마스터 시스템 단위 변환.
#define Poten_Range		1030			//	90도에 1070펄스
#define	MASTER_DPP		0.084112		//	90/1070 = 0.084112

// Robot의 단위 변환
#define	ROBOT_DPP		0.000219726563		// 360/(4096*4*100) = 0.000219726563	degree per pulse
#define ROBOT_DPP_80	0.000274658203		// 360/ (4096*4*80) = 0.000274658203125	degree per pulse

#define	ROBOT_PPD		4551				// (4096*4*100)/ 360 = 4551.111111		pulse per degree	
#define	ROBOT_PPD_80	3640.88889		// (4096*4*80) / 360 = 3640.88889		pulse per degree

#define Encoder			0.0179054054054054	// 55(deg) / 2900 pluse

#define ROBOT_MPP		0.00008846		//	[mm] 1펄스에 이동하는 거리 => 17.3*3.141592/614,400
#define ROBOT_PPM		11304.603165	//	[Pulse] 1mm 이동 펄스 => 614,400 / (17.3*3.141592)
#define M_DOF_RANGE		4551	//  [Pulse] 1도 이동 펄스
//#define M_DOF_RANGE		1024	//  [Pulse] 0.5mm 이동 펄스
#define M_DOF_RANGE_80	3640//  [Pulse] 감속비 80일때 1도 이동 펄스

#define START_END_MARGIN 600000

#define ERROR_MARGIN	4551.1 * (0.05) // steady state 위치 5% 이내 

#define PASSIVE_SPEED_L		10.0
#define PASSIVE_SPEED_H		5.0
#define ACTIVE_SPEED		0.2
#define HOLDTIME			3000

// 4번 컨트롤러 정보
#define DOF01_EXT		2	// R1-Right		  0도(실측)		0(EXT),		0(FLD)
#define DOF01_FLD		7	// R1-Left		120도			0(EXT),		0(FLD)

#define DOF02_EXT		11	//				  0도(실측)		0(EXT),		0(FLD)
#define DOF02_FLD		4	//				 90도			0(EXT),		0(FLD)

#define DOF03_EXT		6	//				  0도(실측)		0(EXT),		0(FLD)
#define DOF03_FLD		10	//				 90도			0(EXT),		0(FLD)

#define DOF04_EXT		3	//				  0도(실측)		0(EXT),		0(FLD)
#define DOF04_FLD		8	//				 90도			0(EXT),		0(FLD)

#define DOF05_EXT		1   // R2 - Right	  0도(실측)		0(EXT),		0(FLD)
#define DOF05_FLD		9   // R2 - Left	 150도			0(EXT),		0(FLD)

#define DOF06_EXT		5	//				  0도(실측)		0(EXT),		0(FLD)
#define DOF06_FLD		12	//				 90도			0(EXT),		0(FLD)

#define DOF07_EXT		13	// Close		  0도(실측)		0(EXT),		0(FLD)
#define DOF07_FLD		14	// Open			 60도			0(EXT),		0(FLD)

#define RANGE_DOF01		60
#define RANGE_DOF02		90
#define RANGE_DOF03		90
#define RANGE_DOF04		90
#define RANGE_DOF05		75
#define RANGE_DOF06		90
#define RANGE_DOF07		60

#define	DONT_MOVE		0
// #define	EXTENSION		1
#define	FLEXION			2
//**********************************

#define INIT_STATE			1
#define END_STATE			2

#define ENTER				0x0d	// ENTER Key
#define ESC_ASCII_VALUE		0x1b	// ESC Key	
#define ALONE_MOTOR			0x3b	// F1 Key	
#define TEACHING			0x3c	// F2 Key	
#define PASSIVE				0x3c	// F2 Key
#define STRETCHING			0x3d	// F3 Key	
#define REPEAT_ALL			0x3e	// F4 Key	
#define ACTIVE_ASSIST		0x3e	// F4 Key	
#define DUAL_MOTOR			0x3f	// F5 Key	
#define REPEAT_DOF			0x40	// F6 Key
#define MASTER_ACTIVE		0x41	// F7 Key
#define ORIGIN_POSITION		0x42	// F8 Key
#define SAVE_START			0x43	// F9 Key
#define SAVE_END			0x44	// F10 Key
#define SAVE_RANGE			0x44	// F10 Key
#define PLUS_ASCII_VALUE	0x2b	// + Key
#define MINUS_ASCII_VALUE	0x2d	// - Key

#define L_RIGHT_OPERATION	0x3e	
#define R_LEFT_OPERATION	0x3d	
#define LEFT_OPERATION      0x3e	// F4 Key
#define RIGHT_OPERATION     0x3b	// F1 Key

#define YES					0x3b	// F1 Key
#define NO					0X3C	// F2 Key

#define S					0x53	// 01010011 S Key 1 2 16 64 
#define s					0x73	// 01110011 s Key 83 32 115

#define ABDUCTION			0x3f	// F5 Key	
#define ADDUCTION			0x40	// F6 Key
#define EX_ROTATION			0x41	// F7 Key
#define IN_ROTATION			0x42	// F8 Key
#define FLEXION				0x43	// F9 Key
#define EXTENSION			0x44	// F10 Key

#define AUTO_INIT			0x3b	// F1 Key	
#define INITPOSITION		0x3c	// F2 Key	
#define HOMEPOSITION		0x3d	// F3 Key	
#define	CURRENT_CONTROL		0x3e	// F4 Key	
#define	CURRENT_INPUT		0x3f	// F5 Key	

#define HARD				0X3b	// F1 Key
#define NORMAL				0x3c	// F2 Key
#define EASY				0x3d	// F3 Key
#define CheckMode			0x3e	// F4 Key
#define ChangeThresholdValue 0x3f	// F5 Key

#define Origin				0x3b	// F1 Key
#define Change				0x3c	// F2 Key

#define READ_START_END 5 // 5 Key

#define DCX01		1
#define DCX02		2
#define DCX03		3
#define DCX04		4
#define DCX05		5
#define DCX06		6
#define DCX07		7
#define DCX08		8
#define DCX09		9
#define DCX10		10
#define DCX11		11
#define DCX12		12
#define DCX13		13
#define DCX14		14

#define DOF01		1
#define DOF02		2
#define DOF03		3
#define DOF04		4
#define DOF05		5
#define DOF06		6
#define DOF07		7
//**********************************

#define BASE		0
#define SHOULDER	1
#define ELBOW		2
#define ROLL		3
#define PITCH		4
#define YAW			5
#define FORCEP_B	6
#define FOOT_B		7
#define INIT_B		8

#define R_ROT1		0
#define R_ANG2		1
#define R_ANG3		2
#define R_ANG4		3
#define R_ROT5		4
#define R_ANG6		5
#define R_FCEP		6

#define D1		16
#define L2		25
#define D4		30.5
#define D6		 4

#define RD1		58.485
#define RL2		28.6
#define RL3		28.3
#define RD5		38.2
#define RL6		56.415

#define ITERNUM	 10
#define BOUND_RANGE 1
//**********************************

