#include "SinglePort.h"					// C/C++ 기본 함수 및 라이브러리 정의 
#include "ATI_function_wrapper.h"		// FTsensor_Initialize, FTsensor_Read, FTsensor_Terminate 함수 관련 헤더파일, NIDAQmx.h 헤더파일
#include <corecrt_math.h> 
#include <stdint.h>						// 비트 길이 고유한 데이터 형식 헤더파일
#define _USE_MATH_DEFINES				// 수학 상수를 표준 C/C++에 정의
#include <math.h>						// 기초 수식 함수들을 구현하는 표준 라이브러리 함수 모임
#include <time.h>
#include <stdio.h>
#include <algorithm>
#include <malloc.h>

Epos2S_Cmd	EPOS2S;						// EPOS 관련 함수 클래스 선언
Comm_Cmd	COM;						// 통신 관련 함수 클래스 선언
Kinematics  KIN;						// 로봇 관련 함수 클래스 선언

// 특수 키 ex)	0xe0/0x48 [->]
// 펑션 키 ex)	0x00/0x3b [F1], 0x00/0x3c [F2], 0x00/0x3d [F3], 0x00/0x3e [F4],  0x00/0x3f [F5],  0x00/0x40 [F6]
//				0x00/0x41 [F7], 0x00/0x42 [F8], 0x00/0x43 [F9], 0x00/0x44 [F10], 0xE0/0x85 [F11], 0xE0/0x86 [F12], 
// 일반 키 ex)	0x31 [1], 0x2b [+], 0x2d [-], 0x08 [BACKSPACE], 0x1b [ESC], 0x0d [ENTER], 

// 기능키(Function) + 일반키(Normal) n자리  + Enter키 입력.
// (F10, ESC, -99 ~ 99, Enter)
int FNxEnter_getkey(void)
{
	int i = 0, Value = 0, Number[10];

	while (_kbhit() == 0);			      // 키보드를 입력 받고 출력은 해주지 않는 함수

	while (1)
	{
		Number[i] = _getch();			  // 키 입력 받기
		if (Number[i] == ESC_ASCII_VALUE) // ESC : 무한반복
			return Number[i];
		else if (Number[i] == 0x00)	
			return _getch();			  // 0x00 : 문자 입력 받음
		else if ((Number[i] >= 0x30 && Number[i] <= 0x39) || (Number[i] == ENTER) || (Number[i] == MINUS_ASCII_VALUE)) // 1~9, 0x0d, 0x2d
		{
			if (Number[i] == ENTER)		  // Enter key : +,- 숫자의 문자값을 정수형 변수로 Value 값 변환
			{
				for (int j = 0; j < i; j++)
				{
					if (Number[0] == MINUS_ASCII_VALUE)
					{
						if (Number[j] >= 0x30 && Number[j] <= 0x39)	 
								Value = Value * 10 - (Number[j] - 0x30); // ex) '-138' = 십진법 -1 : 십진법 -3, 십진법 -8 -> -138 
					}
					else // Enter key를 누르고 - 없을 경우 --> ex) 문자 '138' = 0x01 : 십진법 1, 십진법3, 십진법 8 : 1 -> 13 -> 138
						 // 문자형 숫자를 정수형 변수 값으로 변환
					{
						if (Number[j] >= 0x30 && Number[j] <= 0x39)	 
								Value = Value * 10 + (Number[j] - 0x30);
					}
				}
				std::cout << endl;
				return Value;
			}

			if (Number[i] == MINUS_ASCII_VALUE)	
				std::cout << "-";
			else								
				std::cout << Number[i] - 0x30; // ESC_ASCII_VALUE 와 ENTER가 아닐 경우 --> Number[i] - 0x30 : 0~9 의 정수형 변수값을 가짐. (그냥 숫자)

			i++;
		}
		else 
			return NULL;
	}
}

// 기능키(Function) + 일반키(Normal) 1자리 입력.
// (F10, ESC, +, -, 1 ~ 7)
int FN01_getkey(void) 
{
	int Ch_H, Ch_L;

	while (_kbhit() == 0);

	std::cout << endl;		
	Ch_H = _getch(); // 사용자에게 키보드로 하나의 키를 입력받는 함수

	if (Ch_H == 0x00)	
		return Ch_L = _getch();
	else if ((Ch_H >= 0x30 && Ch_H <= 0x39) || (Ch_H == PLUS_ASCII_VALUE) || (Ch_H == MINUS_ASCII_VALUE) || (Ch_H == ESC_ASCII_VALUE))	
		return Ch_H;
	// 0x30 ~ 0x39, 0x2b : +key, 0x2d : -key,  0x1b : ESC Key
	else 
		return NULL;
}

/*****************************************************/
/******** (R) Mode / (L) Mode 선택 인터페이스. *******/
/*****************************************************/
int Left_Right_Select(void) 
{
	int Key_value = 0;
	int L_R_Key   = 0;

	while (1)
	{
		std::cout << endl << "=========================================================================================================";
		std::cout << endl << "================ Select ( Right / Left ) Operation! (ESC: Quit), (F1: Right), (F4: Left) ================";
		std::cout << endl << "=========================================================================================================";
		Key_value = FN01_getkey();

		if (Key_value == ESC_ASCII_VALUE)
		{
			L_R_Key = Key_value;
			break;
		}
		else if (Key_value == LEFT_OPERATION)
		{
			std::cout << endl << "==========================================" << endl;
			std::cout		  << "====== ( Left ) Operation Selected. ======" << endl;
			std::cout		  << "==========================================" << endl << endl;
			L_R_Key = Key_value;
			break;
		}
		else if (Key_value == RIGHT_OPERATION)
		{
			std::cout << endl << "===========================================" << endl;
			std::cout		  << "====== ( Right ) Operation Selected. ======" << endl;
			std::cout		  << "===========================================" << endl << endl;
			L_R_Key = Key_value;
			break;
		}
		else
			continue;
	}

	return L_R_Key;
} 

/*****************************************************/
/******* 초기 위치 이동 (R)***************************/
/*****************************************************/
int R_Origin_Position(int L_R_Key) 
{
	int Move_Direction = FORWARD; // FORWARD = 1, REVERSE = 0
	int Step_data, M1_data, M2_data, M3_data, M4_data;
	int f_Num, f_Data, ChangePos[] = { 0, 0, 0, 0, 0 };
	int vel, st;

	// (R) 초기위치 데이터
	fopen_s(&KIN.R_Origin_Pos_file, "R_Origin_Pos.txt", "r+"); // +r : 현재 있는 파일에 read/write 모두 덮어씀
	if (KIN.R_Origin_Pos_file == NULL)
		std::cout << "File Open is Failed..." << endl;
	fscanf_s(KIN.R_Origin_Pos_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
	fscanf_s(KIN.R_Origin_Pos_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
	fclose(KIN.R_Origin_Pos_file);
	
	for (int i = 1; i < MOTOR_NUMBER + 1; i++)
	{
		if (i == 1)
		{
			KIN.InputPos[i] = M1_data;
		}
		else if (i == 2)
		{
			KIN.InputPos[i] = M2_data;
		}
		else if (i == 3)
		{
			KIN.InputPos[i] = M3_data;
		}
		else if (i == 4)
		{
			KIN.InputPos[i] = M4_data;
		}
		else
			break;
		
		st = 3.0;
		vel = abs(abs(KIN.OutputPos[i] * ROBOT_DPP) - abs(KIN.InputPos[i] * ROBOT_DPP)) * 100 * 60 / 360 / st;

		if (vel <= 1)
			vel = 1;
		else if (vel >= 4000)
			vel = 4000;
		
		EPOS2S.m_ulProfileAcceleration = vel;
		EPOS2S.m_ulProfileDeceleration = vel;

		if (!VCS_SetPositionProfile(EPOS2S.m_KeyHandle, i, vel, EPOS2S.m_ulProfileAcceleration, EPOS2S.m_ulProfileDeceleration, &EPOS2S.m_ulErrorCode))
		{
			cout << "Failed Cal_vel nodeID_DCX" << i << " - Error Code : " << EPOS2S.m_ulErrorCode << endl;
			system("PAUSE");
			exit(0);
		}

		EPOS2S.MotorMove(i, KIN.InputPos[i]);
	}

	while (1)
	{
		for (int i = 1; i <= MOTOR_NUMBER + 1; i++) // 모터가 움직이는 동안 Position 추출
		{
			EPOS2S.GetPosition(i, &KIN.OutputPos[i]); // Get position with pulse data

			std::cout << " [Motor_" << i << "]  ";
			std::cout << " [ OutputPos : " << KIN.OutputPos[i] << " ]" << endl;
		}
		if (((KIN.OutputPos[DCX01] > (KIN.InputPos[DCX01] - ERROR_MARGIN)) && (KIN.OutputPos[DCX01] < (KIN.InputPos[DCX01] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX02] > (KIN.InputPos[DCX02] - ERROR_MARGIN)) && (KIN.OutputPos[DCX02] < (KIN.InputPos[DCX02] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX03] > (KIN.InputPos[DCX03] - ERROR_MARGIN)) && (KIN.OutputPos[DCX03] < (KIN.InputPos[DCX03] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX04] > (KIN.InputPos[DCX04] - ERROR_MARGIN)) && (KIN.OutputPos[DCX04] < (KIN.InputPos[DCX04] + ERROR_MARGIN))))
			break;
		else
			continue;
	}
	cout << "InitPosition Done" << endl << endl;
	
	return 0;
} 

/*****************************************************/
/******* 초기 위치 이동 (L)***************************/
/*****************************************************/
int L_Origin_Position(int L_R_Key) 
{
	int Move_Direction = FORWARD; // FORWARD = 1, REVERSE = 0
	int Step_data, M1_data, M2_data, M3_data, M4_data;
	int f_Num, f_Data, ChangePos[] = {0, 0, 0, 0, 0};
	int vel, st;

	// (L) 초기위치 데이터
	fopen_s(&KIN.L_Origin_Pos_file, "L_Origin_Pos.txt", "r+"); // +r : 현재 있는 파일에 read/write 모두 덮어씀
	if (KIN.L_Origin_Pos_file == NULL)
		std::cout << "File Open is Failed..." << endl;
	fscanf_s(KIN.L_Origin_Pos_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
	fscanf_s(KIN.L_Origin_Pos_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
	fclose(KIN.L_Origin_Pos_file);
	
	for (int i = 1; i < MOTOR_NUMBER + 1; i++)
	{
		if (i == 1)
		{
			KIN.InputPos[i] = M1_data;
		}	
		else if (i == 2)
		{
			KIN.InputPos[i] = M2_data;
		}
		else if (i == 3)
		{
			KIN.InputPos[i] = M3_data;
		}
		else if (i == 4)
		{
			KIN.InputPos[i] = M4_data;
		}
		else
			break;

		st = 3.0;
		vel = abs(KIN.OutputPos[i] * ROBOT_DPP - KIN.InputPos[i] * ROBOT_DPP) * 100 * 60 / 360 / st;

		if (vel <= 1)
			vel = 1;
		else if (vel >= 4000)
			vel = 4000;

		EPOS2S.m_ulProfileAcceleration = vel;
		EPOS2S.m_ulProfileDeceleration = vel;

		if (!VCS_SetPositionProfile(EPOS2S.m_KeyHandle, i, vel, EPOS2S.m_ulProfileAcceleration, EPOS2S.m_ulProfileDeceleration, &EPOS2S.m_ulErrorCode))
		{
			cout << "Failed Cal_vel nodeID_DCX" << i << " - Error Code : " << EPOS2S.m_ulErrorCode << endl;
			system("PAUSE");
			exit(0);
		}

		EPOS2S.MotorMove(i, KIN.InputPos[i]);   // nodeID, Pulse_Position
	}

	while (1)
	{
		for (int i = 1; i <= MOTOR_NUMBER+1; i++) 
		{
			EPOS2S.GetPosition(i, &KIN.OutputPos[i]); // Get position with pulse data

			std::cout << " [Motor_" << i << "]  ";
			std::cout << " [ OutputPos : " << KIN.OutputPos[i] << " ]" << endl;
		}
		if (((KIN.OutputPos[DCX01] > (KIN.InputPos[DCX01] - ERROR_MARGIN)) && (KIN.OutputPos[DCX01] < (KIN.InputPos[DCX01] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX02] > (KIN.InputPos[DCX02] - ERROR_MARGIN)) && (KIN.OutputPos[DCX02] < (KIN.InputPos[DCX02] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX03] > (KIN.InputPos[DCX03] - ERROR_MARGIN)) && (KIN.OutputPos[DCX03] < (KIN.InputPos[DCX03] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX04] > (KIN.InputPos[DCX04] - ERROR_MARGIN)) && (KIN.OutputPos[DCX04] < (KIN.InputPos[DCX04] + ERROR_MARGIN))))
			break;
		else
			continue;
	}
	cout << "InitPosition Done" << endl << endl;

	return 0;
}

/*****************************************************/
/******* 속도 계산 ***********************************/
/*****************************************************/
float cal_speed(int step)
{
	float speed = 1.0 + step * 0.1;
	return speed;
}

float cal_time(float Start_Angle, int End_Angle, float dt, float dm)  // Active-assisted
{	
	float time = abs(End_Angle - Start_Angle) * dm / dt;
	return time;
}

/*****************************************************/
/****** (R) Mode --> (L) Mode 초기 위치 이동. ********/
/*****************************************************/
int Right_Go_Left(void)
{
	int select_operation = 0;
	int f_Num, f_Data;
	int st, vel;

	std::cout << endl;
	std::cout << "==================================================================================================" << endl;
	std::cout << "==================== (R.I.G.H.T) Mode Change (L.E.F.T) Mode? (YES: F1), (NO: F2) =================" << endl;
	std::cout << "==================================================================================================" << endl;
	select_operation = FN01_getkey();

	if (select_operation == YES)
	{
		std::cout << "==============================================================================================" << endl;
		std::cout << "====================== Change (L.E.F.T) MODE ===================================================" << endl;
		std::cout << "==============================================================================================" << endl;

		fopen_s(&KIN.Right_Go_Left_file, "Right_Go_Left.txt", "r+");

		if (KIN.Right_Go_Left_file == NULL)
			std::cout << "Right_Go_Left_file Open Failed..." << endl;
		for (int i = 1; i < MOTOR_NUMBER + 1; i++)
		{
			fscanf_s(KIN.Right_Go_Left_file, "%d %ld", &f_Num, &f_Data);
			KIN.InputPos[f_Num] = f_Data;
		}
		fclose(KIN.Right_Go_Left_file);
																   
		for (int i = 1; i < MOTOR_NUMBER + 1; i++)
		{
			st = 4.0;
			vel = abs(abs(KIN.OutputPos[i] * ROBOT_DPP) - abs(KIN.InputPos[i] * ROBOT_DPP)) * 100 * 60 / 360 / st;

			if (vel <= 1)
				vel = 1;
			else if (vel >= 4000)
				vel = 4000;

			EPOS2S.m_ulProfileAcceleration = vel;
			EPOS2S.m_ulProfileDeceleration = vel;

			if (!VCS_SetPositionProfile(EPOS2S.m_KeyHandle, i, vel, EPOS2S.m_ulProfileAcceleration, EPOS2S.m_ulProfileDeceleration, &EPOS2S.m_ulErrorCode))
			{
				cout << "Failed Cal_vel nodeID_DCX" << i << " - Error Code : " << EPOS2S.m_ulErrorCode << endl;
				system("PAUSE");
				exit(0);
			}
			EPOS2S.MotorMove(i, KIN.InputPos[i]);
		}
		EPOS2S.InitEPOS2S();
		return R_LEFT_OPERATION;
	}
	else if (select_operation == NO)
	{
		std::cout << "==============================================================================================" << endl;
		std::cout << "========================== (R.I.G.H.T) MODE ==================================================" << endl;
		std::cout << "==============================================================================================" << endl;
		return RIGHT_OPERATION;
	}
}

/*****************************************************/
/****** (L) Mode --> (R) Mode 초기 위치 이동. ********/
/*****************************************************/
int Left_Go_Right(void)
{
	int select_operation = 0;
	int f_Num, f_Data;
	int st, vel;

	std::cout << endl;
	std::cout << "==================================================================================================" << endl;
	std::cout << "==================== (L.E.F.T) Mode Change (R.I.G.H.T) Mode? (YES: F1), (NO: F2) =================" << endl;
	std::cout << "==================================================================================================" << endl;
	select_operation = FN01_getkey();

	if (select_operation == YES)
	{
		std::cout << "==============================================================================================" << endl;
		std::cout << "====================== Change (R.I.G.H.T) MODE ===============================================" << endl;
		std::cout << "==============================================================================================" << endl;

		fopen_s(&KIN.Left_Go_Right_file, "Left_Go_Right.txt", "r+");

		if (KIN.Left_Go_Right_file == NULL)
			std::cout << "Left_Go_Right_file Open Failed..." << endl;
		for (int i = 1; i < MOTOR_NUMBER + 1; i++)
		{
			fscanf_s(KIN.Left_Go_Right_file, "%d %ld", &f_Num, &f_Data);
			KIN.InputPos[f_Num] = f_Data;
		}
		fclose(KIN.Left_Go_Right_file);

		for (int i = 1; i < MOTOR_NUMBER + 1; i++)
		{
			st = 4.0;
			vel = abs(abs(KIN.OutputPos[i] * ROBOT_DPP) - abs(KIN.InputPos[i] * ROBOT_DPP)) * 100 * 60 / 360 / st;

			if (vel <= 1)
				vel = 1;
			else if (vel >= 4000)
				vel = 4000;

			EPOS2S.m_ulProfileAcceleration = vel;
			EPOS2S.m_ulProfileDeceleration = vel;

			if (!VCS_SetPositionProfile(EPOS2S.m_KeyHandle, i, vel, EPOS2S.m_ulProfileAcceleration, EPOS2S.m_ulProfileDeceleration, &EPOS2S.m_ulErrorCode))
			{
				cout << "Failed Cal_vel nodeID_DCX" << i << " - Error Code : " << EPOS2S.m_ulErrorCode << endl;
				system("PAUSE");
				exit(0);
			}
			EPOS2S.MotorMove(i, KIN.InputPos[i]);
		}
		Sleep(4000);
		EPOS2S.InitEPOS2S();

		return L_RIGHT_OPERATION;
	}
	else if (select_operation == NO)
	{
		std::cout << "==============================================================================================" << endl;
		std::cout << "========================== (L.E.F.T) MODE ====================================================" << endl;
		std::cout << "==============================================================================================" << endl;
		return LEFT_OPERATION;
	}
}

/*****************************************************/
/******* ROM --> Teaching 변환 알고리즘 **************/
/*****************************************************/
int ROM_to_Teaching(int Key_value, int ROM_value[])
{
	float Teaching; // 알고리즘 작성 이후 초기화 필요!
	int Teaching_value;
	int ROM;

	while (1)
	{
		switch (Key_value)
		{
		case ABDUCTION:
			std::cout << "[ Start ROM : 42 ] | [ Range : 43-128 ] || [ Save ROM : "<< ROM_value[1] << " ]" << endl << endl;
			std::cout << "Select the ROM : "; 
			scanf_s("%d", &ROM);
			if (ROM >= 43 && ROM <= 128)
			{
				Teaching = 1.045 * ROM - 42.89; break;
			}
			else
				continue;
		case ADDUCTION:
			std::cout << "[ Start ROM : 0 ] | [ Range : 1-47 ] || [ Save ROM : " << ROM_value[2] << " ]" << endl << endl;
			std::cout << "Select the ROM : ";
			scanf_s("%d", &ROM);
			if (ROM >= 1 && ROM <= 47)
			{
				Teaching = 0.854 * ROM + 1; break;
			}
			else
				continue;
		case EX_ROTATION:
			std::cout << "[ Start ROM : 0 ] | [ Range : 1-85  || [ Save ROM : " << ROM_value[3] << " ]" << endl << endl;
			std::cout << "Select the ROM : ";
			scanf_s("%d", &ROM);
			if (ROM >= 1 && ROM <= 85)
			{
				Teaching = 0.709 * ROM + 1; break;
			}
			else
				continue;
		case IN_ROTATION:
			std::cout << "[ Start ROM : 0 ] | [ Range : 1-60  || [ Save ROM : " << ROM_value[4] << " ]" << endl << endl;
			std::cout << "Select the ROM : ";
			scanf_s("%d", &ROM);
			if (ROM >= 1 && ROM <= 60)
			{
				Teaching = 0.918 * ROM + 1; break;
			}
			else
				continue;
		case FLEXION:
			std::cout << "[ Start ROM : 31 ] | [ Range : 32-153  || [ Save ROM : " << ROM_value[5] << " ]" << endl << endl;
			std::cout << "Select the ROM : ";
			scanf_s("%d", &ROM);
			if (ROM >= 32 && ROM <= 153)
			{
				Teaching = 0.805 * ROM - 23.955; break;
				
			}
			else
				continue;
		case EXTENSION:
			std::cout << "[ Start ROM : 0 ] | [ Range : 1-45  || [ Save ROM : " << ROM_value[6] << " ]" << endl << endl;
			std::cout << "Select the ROM : ";
			scanf_s("%d", &ROM);
			if (ROM >= 1 && ROM <= 45)
			{
				Teaching = 1.043 * ROM + 1; break;
			}
			else
				continue;
		}
		break;
	}

	Teaching_value = round(Teaching);
	std::cout << "----------------------------------------------" << endl;
	std::cout << "ROM : (" << ROM << ") " << "Teaching_Point : (" << Teaching_value << ")" << endl;
	std::cout << "----------------------------------------------" << endl << endl;

	return Teaching_value;
}

/*****************************************************/
/******* ROM 선택 지정 인터페이스 ********************/
/*****************************************************/
int Select_ROM(int Key_value, int Teaching_Point, int ROM_value[])
{
	int ROM;
	int ROM_Mode;
	int Out_Teaching;

	std::cout << endl << "Select ROM (F1 : Origin, F2 : Change)" << endl << endl;
	

	while (1)
	{
		ROM_Mode = FNxEnter_getkey();

		if (ROM_Mode == Origin) // F1
		{
			std::cout << "Teaching Point is Origin" << endl;
			Out_Teaching = Teaching_Point;
			break;
		}
		else if (ROM_Mode == Change) // F2
		{
			Out_Teaching = ROM_to_Teaching(Key_value, ROM_value);
			Sleep(3000);
			break;
		}
		else if (ROM_Mode == ESC_ASCII_VALUE)
		{
			Out_Teaching = NULL;
			break;
		}
		else
		{
			std::cout << endl << "Select ROM (F1 : Origin, F2 : Change)" << endl << endl;
			continue;
		}
	}
	return Out_Teaching;
}


/*****************************************************/
/******* Threshold 선택 지정 인터페이스 **************/
/*****************************************************/
float Select_FTthreshold(int Key_value)
{
	static float Hard_TH = 2.5, Normal_TH = 2.0, Easy_TH = 1.5;
	float FTthreshold = NULL;
	int Level_Key;

	printf("\n");
	printf("Hard : %.1f, Normal : %.1f, Easy : %.1f\n", Hard_TH, Normal_TH, Easy_TH);

	while (1)
	{
		switch (Key_value)
		{
		case ABDUCTION:
			std::cout << "Select ABDUCTION Level!" << endl << endl; 
			std::cout << "(F1 : Hard, F2 : Normal, F3 : Easy, F4 : Check Mode, F5 : Change Mode Value)" << endl << endl; break;
		case ADDUCTION:
			std::cout << "Select ADDUCTION Level!" << endl << endl;
			std::cout << "(F1 : Hard, F2 : Normal, F3 : Easy, F4 : Check Mode, F5 : Change Mode Value)" << endl << endl; break;
		case EX_ROTATION:
			std::cout << "Select EX_ROTAITON Level!" << endl << endl;
			std::cout << "(F1 : Hard, F2 : Normal, F3 : Easy, F4 : Check Mode, F5 : Change Mode Value)" << endl << endl; break;
		case IN_ROTATION:
			std::cout << "Select IN_ROTATION Level!" << endl << endl;
			std::cout << "(F1 : Hard, F2 : Normal, F3 : Easy, F4 : Check Mode, F5 : Change Mode Value)" << endl << endl; break;
		case EXTENSION:
			std::cout << "Select EXTENSION Level!" << endl << endl;
			std::cout << "(F1 : Hard, F2 : Normal, F3 : Easy, F4 : Check Mode, F5 : Change Mode Value)" << endl << endl; break;
		case FLEXION:
			std::cout << "Select FLEXION Level" << endl << endl;
			std::cout << "(F1 : Hard, F2 : Normal, F3 : Easy, F4 : Check Mode, F5 : Change Mode Value)" << endl << endl; break;
		}

		Level_Key = FNxEnter_getkey();

		if (Level_Key == ESC_ASCII_VALUE)
		{
			FTthreshold = NULL; break;
		}
		else if (Level_Key == HARD)
		{
			FTthreshold = Hard_TH; break;
		}
		else if (Level_Key == NORMAL)
		{
			FTthreshold = Normal_TH; break;
		}
		else if (Level_Key == EASY)
		{
			FTthreshold = Easy_TH; break;
		}
		else if (Level_Key == CheckMode)
		{
			printf("\n");
			printf("Hard : %.1f, Normal : %.1f, Easy : %.1f\n", Hard_TH, Normal_TH, Easy_TH);
			continue;
		}
		else if (Level_Key == ChangeThresholdValue)
		{
			printf("\n");
			printf("Typing (Hard), (Normal), (Easy) Mode value.\n");
			printf("Hard: "); scanf_s("%f", &Hard_TH);
			printf("Normal: "); scanf_s("%f", &Normal_TH);
			printf("Easy: "); scanf_s("%f", &Easy_TH);
			printf("Hard : %.1f, Normal : %.1f, Easy : %.1f\n", Hard_TH, Normal_TH, Easy_TH);
			continue;
		}
		else
			continue;
	}

	return FTthreshold;
}

/*****************************************************/
/******* 운동 별 초기위치 이동 (R) *******************/
/*****************************************************/
void R_Start_Degree(int Key_value)
{
	int Step_data, M1_data, M2_data, M3_data, M4_data, M5_data;
	int R_mode;
	int Motor_deg[5] = { 0, };
	int Motor_Pulse[5] = { 0, };
	int st, vel;

	fopen_s(&KIN.R_Start_file, "R_start_degree.txt", "r+"); // +r : 현재 있는 파일에 read/write 모두 덮어씀
	if (KIN.R_Start_file == NULL)
		std::cout << "File Open is Failed..." << endl;
	
	switch (Key_value)
	{
	case ABDUCTION:		R_mode = 1; break;
	case ADDUCTION:		R_mode = 2; break;
	case EX_ROTATION:	R_mode = 3; break;
	case IN_ROTATION:	R_mode = 4; break;
	case FLEXION:		R_mode = 5; break;
	case EXTENSION:		R_mode = 6; break;
	}
	for (int i = 0; i < R_mode; i++)
	{
		fscanf_s(KIN.R_Start_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
	}
	Motor_deg[1] = M1_data;
	Motor_deg[2] = M2_data;
	Motor_deg[3] = M3_data;
	Motor_deg[4] = M4_data;
	Motor_Pulse[1] = Motor_deg[1] * ROBOT_PPD_80;
	Motor_Pulse[2] = Motor_deg[2] * ROBOT_PPD;
	Motor_Pulse[3] = Motor_deg[3] * ROBOT_PPD;
	Motor_Pulse[4] = Motor_deg[4] * ROBOT_PPD;
	fclose(KIN.R_Start_file);

	for (int i = 1; i < MOTOR_NUMBER + 1; i++)
	{
		st = 2.0;
		vel = abs(abs(KIN.OutputPos[i] * ROBOT_DPP) - abs(Motor_Pulse[i] * ROBOT_DPP)) * 100 * 60 / 360 / st;

		if (vel <= 1)
			vel = 1;
		else if (vel >= 4000)
			vel = 4000;

		EPOS2S.m_ulProfileAcceleration = vel;
		EPOS2S.m_ulProfileDeceleration = vel;

		if (!VCS_SetPositionProfile(EPOS2S.m_KeyHandle, i, vel, EPOS2S.m_ulProfileAcceleration, EPOS2S.m_ulProfileDeceleration, &EPOS2S.m_ulErrorCode))
		{
			cout << "Failed Cal_vel nodeID_DCX" << i << " - Error Code : " << EPOS2S.m_ulErrorCode << endl;
			system("PAUSE");
			exit(0);
		}
		EPOS2S.MotorMove(i, Motor_Pulse[i]);
	}

	while (1)
	{
		for (int i = 1; i < MOTOR_NUMBER + 1; i++) // 모터가 움직이는 동안 Position 추출
		{
			EPOS2S.GetPosition(i, &KIN.OutputPos[i]); // Get position with pulse data

			std::cout << " [Motor_" << i << "]  ";
			std::cout << " [ OutputPos : " << KIN.OutputPos[i] << " ]" << endl;
		}
		if (((KIN.OutputPos[DCX01] > (KIN.InputPos[DCX01] - ERROR_MARGIN)) && (KIN.OutputPos[DCX01] < (KIN.InputPos[DCX01] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX02] > (KIN.InputPos[DCX02] - ERROR_MARGIN)) && (KIN.OutputPos[DCX02] < (KIN.InputPos[DCX02] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX03] > (KIN.InputPos[DCX03] - ERROR_MARGIN)) && (KIN.OutputPos[DCX03] < (KIN.InputPos[DCX03] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX04] > (KIN.InputPos[DCX04] - ERROR_MARGIN)) && (KIN.OutputPos[DCX04] < (KIN.InputPos[DCX04] + ERROR_MARGIN))))
			break;
		else
			continue;
	}
}

/*****************************************************/
/******* 운동 별 초기위치 이동 (L) *******************/
/*****************************************************/
void L_Start_Degree(int Key_value)
{
	int Step_data, M1_data, M2_data, M3_data, M4_data, M5_data;
	int L_mode;
	int Motor_deg[5] = { 0, };
	int Motor_Pulse[5] = { 0, };
	int st, vel;

	fopen_s(&KIN.L_Start_file, "L_start_degree.txt", "r+"); // +r : 현재 있는 파일에 read/write 모두 덮어씀
	if (KIN.L_Start_file == NULL)
		std::cout << "File Open is Failed..." << endl;

	switch (Key_value)
	{
	case ABDUCTION:		L_mode = 1; break;
	case ADDUCTION:		L_mode = 2; break;
	case EX_ROTATION:	L_mode = 3; break;
	case IN_ROTATION:	L_mode = 4; break;
	case FLEXION:		L_mode = 5; break;
	case EXTENSION:		L_mode = 6; break;
	}
	for (int i = 0; i < L_mode; i++)
	{
		fscanf_s(KIN.L_Start_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
	}
	Motor_deg[1] = M1_data;
	Motor_deg[2] = M2_data;
	Motor_deg[3] = M3_data;
	Motor_deg[4] = M4_data;
	Motor_Pulse[1] = Motor_deg[1] * ROBOT_PPD;
	Motor_Pulse[2] = Motor_deg[2] * ROBOT_PPD;
	Motor_Pulse[3] = Motor_deg[3] * ROBOT_PPD;
	Motor_Pulse[4] = Motor_deg[4] * ROBOT_PPD;
	fclose(KIN.L_Start_file);

	for (int i = 1; i < MOTOR_NUMBER + 1; i++)
	{
		st = 2.0;
		vel = abs(KIN.OutputPos[i] * ROBOT_DPP - Motor_Pulse[i] * ROBOT_DPP) * 100 * 60 / 360 / st;

		if (vel <= 1)
			vel = 1;
		else if (vel >= 4000)
			vel = 4000;

		EPOS2S.m_ulProfileAcceleration = vel;
		EPOS2S.m_ulProfileDeceleration = vel;

		if (!VCS_SetPositionProfile(EPOS2S.m_KeyHandle, i, vel, EPOS2S.m_ulProfileAcceleration, EPOS2S.m_ulProfileDeceleration, &EPOS2S.m_ulErrorCode))
		{
			cout << "Failed Cal_vel nodeID_DCX" << i << " - Error Code : " << EPOS2S.m_ulErrorCode << endl;
			system("PAUSE");
			exit(0);
		}
		EPOS2S.MotorMove(i, Motor_Pulse[i]);
	}

	while (1)
	{
		for (int i = 1; i < MOTOR_NUMBER + 1; i++) // 모터가 움직이는 동안 Position 추출
		{
			EPOS2S.GetPosition(i, &KIN.OutputPos[i]); // Get position with pulse data

			std::cout << " [Motor_" << i << "]  ";
			std::cout << " [ OutputPos : " << KIN.OutputPos[i] << " ]" << endl;
		}
		if (((KIN.OutputPos[DCX01] > (KIN.InputPos[DCX01] - ERROR_MARGIN)) && (KIN.OutputPos[DCX01] < (KIN.InputPos[DCX01] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX02] > (KIN.InputPos[DCX02] - ERROR_MARGIN)) && (KIN.OutputPos[DCX02] < (KIN.InputPos[DCX02] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX03] > (KIN.InputPos[DCX03] - ERROR_MARGIN)) && (KIN.OutputPos[DCX03] < (KIN.InputPos[DCX03] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX04] > (KIN.InputPos[DCX04] - ERROR_MARGIN)) && (KIN.OutputPos[DCX04] < (KIN.InputPos[DCX04] + ERROR_MARGIN))))
			break;
		else
			continue;
	}
}

/*****************************************************/
/******* Teaching --> ROM 변환 알고리즘 **************/
/*****************************************************/
int Teaching_to_ROM(int L_R_Key, int Key_value, int T_point)
{
	float* avg_error = (float*)malloc(sizeof(float)*T_point + sizeof(float)); // 동적 할당 배열
	float M1_error = 0, M2_error = 0, M3_error = 0, M4_error = 0;
	int size;
	float min;
	int Teaching_Point = 0;
	int ROM_value = 0;

	std::cout << endl;
	std::cout << "T_point: " << T_point << endl;
	size = _msize(avg_error) / sizeof(avg_error);
	std::cout << "size : " << size << endl;

	for (int i = 1; i <= MOTOR_NUMBER; i++)
	{
		EPOS2S.GetPositionDOF(i);
		std::cout << "OutputPos (degree) : " << KIN.OutputPos[i] * ROBOT_DPP << endl;
	}

	for (int j = 1; j <= T_point; j++) // 위치 오차 계산
	{
		M1_error = fabsf(KIN.Step_Deg_Range[j][1] - (KIN.OutputPos[1] * ROBOT_DPP));
		M2_error = fabsf(KIN.Step_Deg_Range[j][2] - (KIN.OutputPos[2] * ROBOT_DPP));
		M3_error = fabsf(KIN.Step_Deg_Range[j][3] - (KIN.OutputPos[3] * ROBOT_DPP));
		M4_error = fabsf(KIN.Step_Deg_Range[j][4] - (KIN.OutputPos[4] * ROBOT_DPP));
		avg_error[j] = (M1_error + M2_error + M3_error + M4_error / 4);
		std::cout << "avg_error[" << j << "] = " << avg_error[j] << endl;
	}
			
	min = avg_error[1];					// 위치 오차 최소값 계산
	for (int i = 1; i <= T_point; i++) 
	{
		if (min > avg_error[i])
			min = avg_error[i];
	}
	for (int i = 1; i <= T_point; i++)	// 위치 오차의 Teaching_Point와 ROM 계산
	{
		if ((avg_error[i] - min) == 0)
		{
			Teaching_Point = i;
			std::cout << "Min_value = " << avg_error[i] << endl << endl;
			switch (Key_value)
			{
				case ABDUCTION:		ROM_value = round((Teaching_Point + 42.89) / 1.045); break;
				case ADDUCTION:		ROM_value = round((Teaching_Point - 1) / 0.854); break;
				case EX_ROTATION:	ROM_value = round((Teaching_Point - 1) / 0.709); break;
				case IN_ROTATION:	ROM_value = round((Teaching_Point - 1) / 0.918); break;
				case FLEXION:		ROM_value = round((Teaching_Point + 23.955) / 0.805); break;
				case EXTENSION:		ROM_value = round((Teaching_Point - 1) / 1.043); break;
			}
		}
	}
	std::cout << "----------------------------------------------" << endl;
	std::cout << "ROM : (" << ROM_value << ") " << "Teaching_Point : (" << Teaching_Point << ")" << endl;
	std::cout << "----------------------------------------------" << endl << endl;
	return ROM_value;
}


/*************************************************************************************/
/********************** Main *********************************************************/
/*************************************************************************************/

int main()
{
	double	FTvalue[6] = { 0, };			// FT센서 값 배열 초기화
	double	FToffset[6] = { 0, };			// FToffset  배열 초기화
	double	FTthreshold = 1.0;				// FT센서 Threshold 1.0으로 초기화 
	double	FTtrigger = 0.0;				// FT 트리거 0 초기화
	double	ArmWeight = 0.0;				// ArmWeight 0 초기화
	long	measure_time = 0;
	clock_t start_time, end_time;
	int		Move_Direction = FORWARD;		// 이동방향 FORWARD로 초기화
	int		limit = 0;						// Teaching_Point 제한 변수 값 
	int		Move_Done = DONE;				
	int		step = 0, Input_Step = 0, Step_count;							// Step 관련 변수 선언
	int		measure_count = 0;
	int		Key_value, Key_value2, DOF, DOF1, DOF2, Motor_Num, Mode_Change; // 동작 설정 관련 변수 선언
	int		Deg_value;														// 각도값 
	char	Temp, Test=0, T_point;										    
	int		ROM_value[7] = { 0, };
	int		Step_data = 0, M1_data = 0, M2_data = 0, M3_data = 0, M4_data = 0;     // 메모장 데이터
	int		L_R_Key = 0;
	int		st, vel;

	std::cout << "====================================================================" << endl;
	std::cout << "============================  ON_system ============================" << endl;
	std::cout << "====================================================================" << endl;

	// FT 센서 초기화
	FTsensor_Initialize();	// FTsensor 기본 정의

	// EPOS2S 초기화
	EPOS2S.InitEPOS2S();	// position 기본 정의
	
	// COM Port 초기화
	// COM.Comm_Open();		// 컴퓨터 통신 open : 싱글포트 에서 사용됨으로 추정
	
	// Pulse Range변수 초기화
	//KIN.BackUp_PulseRange();

	// Step Move변수 초기화
	//KIN.BackUp_StepInfo();
	
	//// Calculation of Degree Unit										
	//KIN.Init_BaseAngle();	

	/*
	while (1) // FT센서 값 확인 코드
	{
		FTsensor_Read(FTvalue);
		printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);
	}
	*/

	std::cout << "==================================================================================" << endl << endl;
	while (1)
	{
		L_R_Key = Left_Right_Select(); 

		if (L_R_Key == RIGHT_OPERATION)
		{
			KIN.R_BackUp_OriginPosition();
			KIN.R_BackUp_StartPosition();
			KIN.R_BackUp_EndPosition();
		}
		else if (L_R_Key == LEFT_OPERATION)
		{
			KIN.L_BackUp_OriginPosition();
			KIN.L_BackUp_StartPosition();
			KIN.L_BackUp_EndPosition();
		}
		else if (L_R_Key == ESC_ASCII_VALUE)
		{
			FTsensor_Terminate();
			EPOS2S.RestoreEPOS();
			exit(0); // 동작 종료
		}
		else if (L_R_Key == LEFT_OPERATION)
		{
			continue;
		}
		else
			continue;
		
		while (1)
		{
			std::cout << endl << "Select Mode! (ESC : Quit, F1 : Single Motor, F2 : PASSIVE, F3 : STRETCHING, F4:ACTIVE_ASSIST, F8 : ORIGIN_POSITION)" << endl;
			Key_value = FN01_getkey();

			if (Key_value == ESC_ASCII_VALUE) // 0x1b : ESC Key
				break;
			else
			{
				switch (Key_value)
				{
				case ALONE_MOTOR: // 0x3b : F1 key (Single Motor) : Start_Pos_file.txt(w), End_Pos_file.txt(w)
					while (1)
					{
						std::cout << "Select MOTOR! (ESC, 1 ~ 4) or (Read_Start_End_File[5]): ";
						Key_value = FNxEnter_getkey();				

						if (Key_value == ESC_ASCII_VALUE)			
							break;									
						else if (Key_value >= 1 && Key_value <= MOTOR_NUMBER) // 1~4 모터 선택 
						{
							Motor_Num = Key_value;							  // Motor_num : Motor 번호
							while (1)
							{
								std::cout << "Select Change Value! (SAVE_Start[F9], SAVE_End[F10], ESC, + , - )" << endl;
								Key_value = FN01_getkey();

								if (Key_value == ESC_ASCII_VALUE)	// ESC Key
									break;
								else if (Key_value == SAVE_START)	// F9 Key : Position 저장 시작
								{
									KIN.Start_Pos[Motor_Num] = KIN.OutputPos[Motor_Num];

									if (L_R_Key == RIGHT_OPERATION)
									{
										fopen_s(&KIN.Start_Pos_file, "R_Start_Pos_file.txt", "w"); // 쓰기용으로 파일 생성
										for (int i = 1; i < MOTOR_NUMBER + 1; i++)
											fprintf(KIN.Start_Pos_file, "%d %ld\n", i, KIN.Start_Pos[i]);
										fclose(KIN.Start_Pos_file);
									}
									else if (L_R_Key == LEFT_OPERATION)
									{
										fopen_s(&KIN.Start_Pos_file, "L_Start_Pos_file.txt", "w");
										for (int i = 1; i < MOTOR_NUMBER + 1; i++)
											fprintf(KIN.Start_Pos_file, "%d %ld\n", i, KIN.Start_Pos[i]); 
										fclose(KIN.Start_Pos_file);
									}

									std::cout << "SAVE Start_Pos of " << Motor_Num << "_MOTOR [ " << KIN.Start_Pos[Motor_Num] << " ]" << endl;
									std::cout << " [Motor_" << Motor_Num << "]  ";
									std::cout << " [ Start_Pos : " << KIN.Start_Pos[Motor_Num] << " ] / ";
									std::cout << " [ End_Pos : " << KIN.End_Pos[Motor_Num] << " ] / ";
									std::cout << " [ InputPos : " << KIN.InputPos[Motor_Num] << " ] / ";
									std::cout << " [ OutputPos : " << KIN.OutputPos[Motor_Num] << " ]" << endl;
								}
								else if (Key_value == SAVE_END) // F10 Key : Position 저장 종료
								{
									KIN.End_Pos[Motor_Num] = KIN.OutputPos[Motor_Num]; // OutputPosition --> EndPosition

									if (L_R_Key == RIGHT_OPERATION)
									{
										fopen_s(&KIN.End_Pos_file, "R_End_Pos_file.txt", "w"); // 쓰기용으로 파일 생성
										for (int i = 1; i < MOTOR_NUMBER + 1; i++)
											fprintf(KIN.End_Pos_file, "%d %ld\n", i, KIN.End_Pos[i]); // 파일에 End Position 값 모터별로 순차적 대입
										fclose(KIN.End_Pos_file);
									}
									else if (L_R_Key == LEFT_OPERATION)
									{
										fopen_s(&KIN.End_Pos_file, "L_End_Pos_file.txt", "w");				
										for (int i = 1; i < MOTOR_NUMBER + 1; i++)
											fprintf(KIN.End_Pos_file, "%d %ld\n", i, KIN.End_Pos[i]);
										fclose(KIN.End_Pos_file);
									}

									std::cout << "SAVE End_Pos of " << Motor_Num << "_MOTOR [ " << KIN.End_Pos[Motor_Num] << " ]" << endl;
									std::cout << " [Motor_" << Motor_Num << "]  ";
									std::cout << " [ Start_Pos : " << KIN.Start_Pos[Motor_Num] << " ] / ";
									std::cout << " [ End_Pos : " << KIN.End_Pos[Motor_Num] << " ] / ";
									std::cout << " [ InputPos : " << KIN.InputPos[Motor_Num] << " ] / ";
									std::cout << " [ OutputPos : " << KIN.OutputPos[Motor_Num] << " ]" << endl;
								}
								else if (Key_value == PLUS_ASCII_VALUE || Key_value == MINUS_ASCII_VALUE)	 
								{
									if (Key_value == PLUS_ASCII_VALUE) // + Key
									{
										EPOS2S.GetPosition(Motor_Num, &KIN.OutputPos[Motor_Num]);
										KIN.InputPos[Motor_Num] = KIN.OutputPos[Motor_Num] + M_DOF_RANGE;

										vel = 1800;
										EPOS2S.m_ulProfileAcceleration = 1660;
										EPOS2S.m_ulProfileDeceleration = 1660;

										if (!VCS_SetPositionProfile(EPOS2S.m_KeyHandle, Motor_Num, vel, EPOS2S.m_ulProfileAcceleration, EPOS2S.m_ulProfileDeceleration, &EPOS2S.m_ulErrorCode))
										{
											cout << "Failed Cal_vel nodeID_DCX" << Motor_Num << " - Error Code : " << EPOS2S.m_ulErrorCode << endl;
											system("PAUSE");
											exit(0);
										}

										EPOS2S.MotorMove(Motor_Num, KIN.InputPos[Motor_Num]); 
										
										while (1)
										{
											EPOS2S.GetPosition(Motor_Num, &KIN.OutputPos[Motor_Num]);

											std::cout << " [Motor_" << Motor_Num << "]  ";
											std::cout << " [ Start_Pos : " << KIN.Start_Pos[Motor_Num] << " ] / ";
											std::cout << " [ End_Pos : " << KIN.End_Pos[Motor_Num] << " ] / ";
											std::cout << " [ InputPos : " << KIN.InputPos[Motor_Num] << " ] / ";
											std::cout << " [ OutputPos : " << KIN.OutputPos[Motor_Num] << " ]" << endl;
											
											if (KIN.OutputPos[Motor_Num] > (KIN.InputPos[Motor_Num] - ERROR_MARGIN) && KIN.OutputPos[Motor_Num] < (KIN.InputPos[Motor_Num] + ERROR_MARGIN))
												break; 
											else
												continue;

										}
									}
									else if (Key_value == MINUS_ASCII_VALUE) // - Key
									{
										EPOS2S.GetPosition(Motor_Num, &KIN.OutputPos[Motor_Num]);
										KIN.InputPos[Motor_Num] = KIN.OutputPos[Motor_Num] - M_DOF_RANGE;	// InputPosition = OutputPosition - 1024 --> 감속기 고려 회전 각도 = -0.225도
										
										vel = 1800;
										EPOS2S.m_ulProfileAcceleration = 1660;
										EPOS2S.m_ulProfileDeceleration = 1660;

										if (!VCS_SetPositionProfile(EPOS2S.m_KeyHandle, Motor_Num, vel, EPOS2S.m_ulProfileAcceleration, EPOS2S.m_ulProfileDeceleration, &EPOS2S.m_ulErrorCode))
										{
											cout << "Failed Cal_vel nodeID_DCX" << Motor_Num << " - Error Code : " << EPOS2S.m_ulErrorCode << endl;
											system("PAUSE");
											exit(0);
										}

										EPOS2S.MotorMove(Motor_Num, KIN.InputPos[Motor_Num]);
										
										while (1)
										{
											EPOS2S.GetPosition(Motor_Num, &KIN.OutputPos[Motor_Num]);

											std::cout << " [Motor_" << Motor_Num << "]  ";
											std::cout << " [ Start_Pos : " << KIN.Start_Pos[Motor_Num] << " ] / ";
											std::cout << " [ End_Pos : " << KIN.End_Pos[Motor_Num] << " ] / ";
											std::cout << " [ InputPos : " << KIN.InputPos[Motor_Num] << " ] / ";
											std::cout << " [ OutputPos : " << KIN.OutputPos[Motor_Num] << " ]" << endl;

											if (KIN.OutputPos[Motor_Num] > (KIN.InputPos[Motor_Num] - ERROR_MARGIN) && KIN.OutputPos[Motor_Num] < (KIN.InputPos[Motor_Num] + ERROR_MARGIN))
												break;
											else
												continue;
										}
										
									}
								}
								else
									continue;
							}
						}
						else if (Key_value == READ_START_END) // 5 Key : Read_Start_End_File 
						{
							if (L_R_Key == RIGHT_OPERATION)
							{
								fopen_s(&KIN.Start_Pos_file, "R_Start_Pos_file.txt", "r+");
								fopen_s(&KIN.End_Pos_file, "R_End_Pos_file.txt", "r+");
								if (KIN.Start_Pos_file == NULL)
									std::cout << "Start_Pos_file Open Failed..." << endl;
								else if (KIN.End_Pos_file == NULL)
									std::cout << "End_Pos_file Open Failed..." << endl;
								else
								{
									for (int i = 1; i < MOTOR_NUMBER + 1; i++)
									{
										fscanf_s(KIN.Start_Pos_file, "%d %d", &KIN.Motor_NUM[i], &KIN.Start_Pos[i]);
										printf("Motor Start_Pos (%d) : %d\n", KIN.Motor_NUM[i], KIN.Start_Pos[i]);
									}
									for (int i = 1; i < MOTOR_NUMBER + 1; i++)
									{
										fscanf_s(KIN.End_Pos_file, "%d %d", &KIN.Motor_NUM[i], &KIN.End_Pos[i]);
										printf("Motor End_Pos (%d) : %d\n", KIN.Motor_NUM[i], KIN.End_Pos[i]);
									}
								}
							}
							else if (L_R_Key == LEFT_OPERATION)
							{
								fopen_s(&KIN.Start_Pos_file, "L_Start_Pos_file.txt", "r+");
								fopen_s(&KIN.End_Pos_file, "L_End_Pos_file.txt", "r+");
								if (KIN.Start_Pos_file == NULL)
									std::cout << "Start_Pos_file Open Failed..." << endl;
								else if (KIN.End_Pos_file == NULL)
									std::cout << "End_Pos_file Open Failed..." << endl;
								else
								{
									for (int i = 1; i < MOTOR_NUMBER + 1; i++)
									{
										fscanf_s(KIN.Start_Pos_file, "%d %d", &KIN.Motor_NUM[i], &KIN.Start_Pos[i]);
										printf("Motor Start_Pos (%d) : %d\n", KIN.Motor_NUM[i], KIN.Start_Pos[i]);
									}
									for (int i = 1; i < MOTOR_NUMBER + 1; i++)
									{
										fscanf_s(KIN.End_Pos_file, "%d %d", &KIN.Motor_NUM[i], &KIN.End_Pos[i]);
										printf("Motor End_Pos (%d) : %d\n", KIN.Motor_NUM[i], KIN.End_Pos[i]);
									}
								}
							}
						}
						else // 모터 선택 x	
							continue;
					}
					break;

				case PASSIVE: // F2 Key
					while (1)
					{
						std::cout << endl << "Select PASSIVE! (F5 : ABDUCTION, F6 : ADDUCTION, F7 : EX_ROTATION, F8 : IN_ROTATION, F9 : FLEXION, F10 : EXTENSION)" << endl;
						Key_value = FN01_getkey();
						Step_count = 1;

						if (Key_value == ESC_ASCII_VALUE) 
							break;
						else
						{
							switch (Key_value)
							{
							case ABDUCTION: // F5 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);  // move to start position
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.AB_ADDUCTION_file, "R_ABDUCTION.txt", "r+"); // 1. 메모장 데이터 입력하기
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.AB_ADDUCTION_file, "L_ABDUCTION.txt", "r+");

								if (KIN.AB_ADDUCTION_file == NULL)
									std::cout << "Abduction file Open Failed..." << endl;
								fscanf_s(KIN.AB_ADDUCTION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data); // 0 0 0 0 0

								KIN.Teaching_Point = M1_data; // 메모장 맨처음 2번째 값 = Teaching_Point 값

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;

								for (int i = 1; i <= KIN.Teaching_Point; i++) // N번째 Step에 1번~4번 모터의 각도 값 입력
								{
									fscanf_s(KIN.AB_ADDUCTION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.AB_ADDUCTION_file);

								FTsensor_Read(FTvalue); // Torque sensor 초기 데이터를 오프셋 데이터로 지정
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}
								ArmWeight = sqrt(pow(FTvalue[0], 2) + pow(FTvalue[1], 2)); // ArmWeight = sqrt(Fx^2 + Fy^2)

								while (1) // 모터 동작 
								{
									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									if (Step_count < KIN.Teaching_Point)
									{
										Step_count = KIN.Teaching_Point;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
									}
									else
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
									}

									if (_kbhit()) // 키보드 입력을 받되, 출력은 하지 않음. --> 이후 Key button을 눌렀을 경우, ROM을 저장하는 구문
									{
										Temp = _getch();
										break;
									}
								}
								break;

							case ADDUCTION: // F6 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.AB_ADDUCTION_file, "R_ADDUCTION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.AB_ADDUCTION_file, "L_ADDUCTION.txt", "r+");
								if (KIN.AB_ADDUCTION_file == NULL)
									std::cout << "Adduction file Open Failed..." << endl;
								fscanf_s(KIN.AB_ADDUCTION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
								KIN.Teaching_Point = M1_data;

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.AB_ADDUCTION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.AB_ADDUCTION_file);

								FTsensor_Read(FTvalue);
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}

								ArmWeight = sqrt(pow(FTvalue[0], 2) + pow(FTvalue[1], 2));

								while (1)
								{
									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									if (Step_count < KIN.Teaching_Point)
									{
										Step_count = KIN.Teaching_Point;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
									}
									else
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
									}

									if (_kbhit())
									{
										Temp = _getch();
										// cout << "Press Button = " << Temp << endl;
										break;
									}
								}

								break;

							case EX_ROTATION: // F8 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.EX_IN_ROTATION_file, "R_EX_ROTATION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.EX_IN_ROTATION_file, "L_EX_ROTATION.txt", "r+");

								if (KIN.EX_IN_ROTATION_file == NULL)
									std::cout << "EX_ROTATION_file Open Failed..." << endl;
								fscanf_s(KIN.EX_IN_ROTATION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
								KIN.Teaching_Point = M1_data;

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.EX_IN_ROTATION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.EX_IN_ROTATION_file);

								FTsensor_Read(FTvalue);
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}

								while (1)
								{
									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									if (Step_count < KIN.Teaching_Point)
									{
										Step_count = KIN.Teaching_Point;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
									}
									else
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
									}

									if (_kbhit())
									{
										Temp = _getch();
										break;
									}
								}
								break;

							case IN_ROTATION: // F8 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.EX_IN_ROTATION_file, "R_IN_ROTATION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.EX_IN_ROTATION_file, "L_IN_ROTATION.txt", "r+");

								if (KIN.EX_IN_ROTATION_file == NULL)
									std::cout << "IN_ROTATION_file Open Failed..." << endl;
								fscanf_s(KIN.EX_IN_ROTATION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
								KIN.Teaching_Point = M1_data;

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.EX_IN_ROTATION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.EX_IN_ROTATION_file);

								FTsensor_Read(FTvalue);
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}

								while (1)
								{
									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									if (Step_count < KIN.Teaching_Point)
									{
										Step_count = KIN.Teaching_Point;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
									}
									else
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
									}

									if (_kbhit())
									{
										Temp = _getch();
										break;
									}
								}
								break;

							case FLEXION: // F10 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.EX_FLEXION_file, "R_FLEXION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.EX_FLEXION_file, "L_FLEXION.txt", "r+");

								if (KIN.EX_FLEXION_file == NULL)
									std::cout << "FLEXION_file Open Failed..." << endl;
								fscanf_s(KIN.EX_FLEXION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
								KIN.Teaching_Point = M1_data;

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.EX_FLEXION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.EX_FLEXION_file);

								while (1)
								{
									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									if (Step_count < KIN.Teaching_Point)
									{
										Step_count = KIN.Teaching_Point;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
									}
									else
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
									}

									if (_kbhit())
									{
										Temp = _getch();
										break;
									}
								}
								break;

							case EXTENSION: // F9 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.EX_FLEXION_file, "R_EXTENSION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.EX_FLEXION_file, "L_EXTENSION.txt", "r+");

								if (KIN.EX_FLEXION_file == NULL)
									std::cout << "Extension_file Open Failed..." << endl;
								fscanf_s(KIN.EX_FLEXION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
								KIN.Teaching_Point = M1_data;

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.EX_FLEXION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.EX_FLEXION_file);

								while (1)
								{
									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									if (Step_count < KIN.Teaching_Point)
									{
										Step_count = KIN.Teaching_Point;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
									}
									else
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
									}

									if (_kbhit())
									{
										Temp = _getch();
										break;
									}
								}
								break;

							default:
								break;
							}
						}
					}
					break;

				case STRETCHING: // F3 Key (Streching Mode)
					while (1)
					{
						std::cout << endl << "Select STRETCHING! (F5 : ABDUCTION, F6 : ADDUCTION, F7 : EX_ROTATION, F8 : IN_ROTATION, F9 : FLEXION, F10 : EXTENSION)" << endl;
						Key_value = FN01_getkey();
						Step_count = 1;
						Move_Direction = FORWARD;

						if (Key_value == ESC_ASCII_VALUE) 
							break;
						else
						{
							switch (Key_value)
							{
							case ABDUCTION: // F5 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.AB_ADDUCTION_file, "R_ABDUCTION.txt", "r+"); // 1. 메모장 데이터 입력하기
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.AB_ADDUCTION_file, "L_ABDUCTION.txt", "r+"); 

								if (KIN.AB_ADDUCTION_file == NULL)
									std::cout << "Abduction file Open Failed..." << endl;
								fscanf_s(KIN.AB_ADDUCTION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data); // 0 0 0 0 0

								KIN.Teaching_Point = M1_data; // 메모장 맨처음 2번째 값 = Teaching_Point 값
								limit = KIN.Teaching_Point;

								for (int i = 1; i <= KIN.Teaching_Point; i++) // N번째 Step에 1번~4번 모터의 각도 값 입력
								{
									fscanf_s(KIN.AB_ADDUCTION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.AB_ADDUCTION_file);

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;

								FTsensor_Read(FTvalue); // Torque sensor 초기 데이터를 오프셋 데이터로 지정
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}
								ArmWeight = sqrt(pow(FToffset[0], 2) + pow(FToffset[1], 2)); // ArmWeight = sqrt(Fx^2 + Fy^2)

								while (1) // 모터 동작 
								{
									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									if (Move_Direction == FORWARD)
									{
										Step_count = KIN.Teaching_Point-5;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(Step_count));
										
										if (_kbhit())
										{
											Temp = _getch();
											break;
										}

										while (1)
										{
											FTsensor_Read(FTvalue);
											printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);
											double reactionForce = sqrt(pow(FTvalue[0], 2) + pow(FTvalue[1], 2)) - ArmWeight;
											if (reactionForce > 10)
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
												{
													EPOS2S.MotorStop(i);
												}
												Move_Direction = REVERSE;
												break;
											}
											else
											{
												if (Step_count < KIN.Teaching_Point)
													Step_count++;
												else
												{
													Step_count = KIN.Teaching_Point;
													Move_Direction = REVERSE;
													break;
												}
												cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
												EPOS2S.MoveSTEP(Step_count, cal_speed(1));

												if (_kbhit()) 
												{
													Move_Direction = REVERSE;
													break;
												}
											}

										}
									}
									else if (Move_Direction == REVERSE)
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
										Move_Direction = FORWARD;
									}

									if (_kbhit())
									{
										Temp = _getch();
										break;
									}
								}
								break;

							case ADDUCTION: // F6 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.AB_ADDUCTION_file, "R_ADDUCTION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.AB_ADDUCTION_file, "L_ADDUCTION.txt", "r+");
								if (KIN.AB_ADDUCTION_file == NULL)
									std::cout << "Adduction file Open Failed..." << endl;
								fscanf_s(KIN.AB_ADDUCTION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
								KIN.Teaching_Point = M1_data;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.AB_ADDUCTION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.AB_ADDUCTION_file);

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;

								FTsensor_Read(FTvalue); // Torque sensor 초기 데이터를 오프셋 데이터로 지정
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}
								//ArmWeight = sqrt(pow(FToffset[0], 2) + pow(FToffset[1], 2)); // ArmWeight = sqrt(Fx^2 + Fy^2)

								while (1) // 모터 동작 
								{
									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									if (Move_Direction == FORWARD)
									{
										Step_count = KIN.Teaching_Point-5;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(Step_count));
										
										if (_kbhit())
										{
											break;
										}

										while (1)
										{
											FTsensor_Read(FTvalue);
											printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

											if ((FTvalue[2]- FToffset[2]) < -20.0 )
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
												{
													EPOS2S.MotorStop(i);
												}
												Move_Direction = REVERSE;
												break;
											}

											else
											{
												if (Step_count < KIN.Teaching_Point)
													Step_count++;
												else
												{
													Step_count = KIN.Teaching_Point;
													Move_Direction = REVERSE;
													break;
												}
												cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
												EPOS2S.MoveSTEP(Step_count, cal_speed(1));

												if (_kbhit())
												{
													Move_Direction = REVERSE;
													break;
												}
											}
										}
									}
									else if (Move_Direction == REVERSE)
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
										Move_Direction = FORWARD;
									}

									if (_kbhit()) 
									{
										break;
									}
								}
								break;

							case EX_ROTATION: // F8 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.EX_IN_ROTATION_file, "R_EX_ROTATION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.EX_IN_ROTATION_file, "L_EX_ROTATION.txt", "r+");

								if (KIN.EX_IN_ROTATION_file == NULL)
									std::cout << "EX_ROTATION_file Open Failed..." << endl;
								fscanf_s(KIN.EX_IN_ROTATION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
								KIN.Teaching_Point = M1_data;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.EX_IN_ROTATION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.EX_IN_ROTATION_file);

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;

								FTsensor_Read(FTvalue); // Torque sensor 초기 데이터를 오프셋 데이터로 지정
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}

								while (1) // 모터 동작 
								{
									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									if (Move_Direction == FORWARD)
									{
										Step_count = KIN.Teaching_Point-5;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(Step_count));

										if (_kbhit())
										{
											break;
										}

										while (1)
										{
											FTsensor_Read(FTvalue);
											printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);
											if ((FTvalue[4] - FToffset[4]) > 1.0)
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
												{
													EPOS2S.MotorStop(i);
												}
												Move_Direction = REVERSE;
												break;
											}

											else
											{
												if (Step_count < KIN.Teaching_Point)
													Step_count++;
												else
												{
													Step_count = KIN.Teaching_Point;
													Move_Direction = REVERSE;
													break;
												}
												cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
												EPOS2S.MoveSTEP(Step_count, cal_speed(1));

												if (_kbhit())
												{
													Move_Direction = REVERSE;
													break;
												}
											}
										}
									}

									else if (Move_Direction == REVERSE)
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
										Move_Direction = FORWARD;
									}

									if (_kbhit()) 
									{
										break;
									}
								}
								break;

							case IN_ROTATION: // F8 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.EX_IN_ROTATION_file, "R_IN_ROTATION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.EX_IN_ROTATION_file, "L_IN_ROTATION.txt", "r+");

								if (KIN.EX_IN_ROTATION_file == NULL)
									std::cout << "IN_ROTATION_file Open Failed..." << endl;
								fscanf_s(KIN.EX_IN_ROTATION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
								KIN.Teaching_Point = M1_data;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.EX_IN_ROTATION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.EX_IN_ROTATION_file);

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;

								FTsensor_Read(FTvalue); // Torque sensor 초기 데이터를 오프셋 데이터로 지정
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}

								while (1) // 모터 동작 
								{
									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									if (Move_Direction == FORWARD)
									{
										Step_count = KIN.Teaching_Point - 5;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(Step_count));

										if (_kbhit())
										{
											break;
										}

										while (1)
										{
											FTsensor_Read(FTvalue);
											printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);
											if ((FTvalue[4] - FToffset[4]) < -1.0)  // 토크 조건으로 수정 필요
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
												{
													EPOS2S.MotorStop(i);
												}
												Move_Direction = REVERSE;
												break;
											}
											else
											{
												if (Step_count < KIN.Teaching_Point)
													Step_count++;
												else
												{
													Step_count = KIN.Teaching_Point;
													Move_Direction = REVERSE;
													break;
												}
												cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
												EPOS2S.MoveSTEP(Step_count, cal_speed(1));

												if (_kbhit())
												{
													Move_Direction = REVERSE;
													break;
												}
											}
										}
									}

									else if (Move_Direction == REVERSE)
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
										Move_Direction = FORWARD;
									}

									if (_kbhit())
									{
										break;
									}
								}
								break;

							case FLEXION: // F10 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.EX_FLEXION_file, "R_FLEXION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.EX_FLEXION_file, "L_FLEXION.txt", "r+");

								if (KIN.EX_FLEXION_file == NULL)
									std::cout << "FLEXION_file Open Failed..." << endl;
								fscanf_s(KIN.EX_FLEXION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);

								KIN.Teaching_Point = M1_data;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.EX_FLEXION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.EX_FLEXION_file);

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;

								FTsensor_Read(FTvalue); // Torque sensor 초기 데이터를 오프셋 데이터로 지정
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}
								ArmWeight = sqrt(pow(FToffset[0], 2) + pow(FToffset[1], 2)); // ArmWeight = sqrt(Fx^2 + Fy^2)

								while (1) // 모터 동작 
								{
									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									if (Move_Direction == FORWARD)
									{
										Step_count = KIN.Teaching_Point - 5;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(Step_count));

										if (_kbhit())
										{
											break;
										}

										while (1)
										{
											FTsensor_Read(FTvalue);
											printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);
											double reactionForce = sqrt(pow(FTvalue[0], 2) + pow(FTvalue[1], 2)) - ArmWeight;

											if (reactionForce > 20)
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
												{
													EPOS2S.MotorStop(i);
												}
												Move_Direction = REVERSE;
												break;
											}
											else
											{
												if (Step_count < KIN.Teaching_Point)
													Step_count++;
												else
												{
													Step_count = KIN.Teaching_Point;
													Move_Direction = REVERSE;
													break;
												}
												cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
												EPOS2S.MoveSTEP(Step_count, cal_speed(1));

												if (_kbhit())
												{
													Move_Direction = REVERSE;
													break;
												}
											}
										}
									}
									else if (Move_Direction == REVERSE)
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
										Move_Direction = FORWARD;
									}

									if (_kbhit())
									{
										break;
									}
								}
								break;

							case EXTENSION: // F9 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.EX_FLEXION_file, "R_EXTENSION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.EX_FLEXION_file, "L_EXTENSION.txt", "r+");

								if (KIN.EX_FLEXION_file == NULL)
									std::cout << "Extension_file Open Failed..." << endl;
								fscanf_s(KIN.EX_FLEXION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);

								KIN.Teaching_Point = M1_data;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.EX_FLEXION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.EX_FLEXION_file);

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;

								FTsensor_Read(FTvalue); // Torque sensor 초기 데이터를 오프셋 데이터로 지정
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}

								while (1) // 모터 동작 
								{
									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									if (Move_Direction == FORWARD)
									{
										Step_count = KIN.Teaching_Point - 5;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(Step_count));

										if (_kbhit())
										{
											break;
										}

										while (1)
										{
											FTsensor_Read(FTvalue);
											printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);
											if (FTvalue[0]- FToffset[0] < -20)
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
												{
													EPOS2S.MotorStop(i);
												}
												Move_Direction = REVERSE;
												break;
											}

											else
											{
												if (Step_count < KIN.Teaching_Point)
													Step_count++;
												else
												{
													Step_count = KIN.Teaching_Point;
													Move_Direction = REVERSE;
													break;
												}
												cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
												EPOS2S.MoveSTEP(Step_count, cal_speed(1));

												if (_kbhit())
												{
													Move_Direction = REVERSE;
													break;
												}
											}
										}
									}

									else if (Move_Direction == REVERSE)
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;
										EPOS2S.MoveSTEP(Step_count, cal_speed(KIN.Teaching_Point));
										Move_Direction = FORWARD;
									}

									if (_kbhit())
									{
										break;
									}
								}
								break;

							default:
								break;
							}
						}
					}
					break;

				case ACTIVE_ASSIST: // F4 Key
					while (1)
					{
						std::cout << endl << "Select ACTIVE_ASSIST! (F5 : ABDUCTION, F6 : ADDUCTION, F7 : EX_ROTATION, F8 : IN_ROTATION, F9 : FLEXION, F10 : EXTENSION)" << endl;
						Key_value = FN01_getkey();
						Step_count = 1;

						if (Key_value == ESC_ASCII_VALUE)
							break;
						else
						{
							switch (Key_value)
							{
							case ABDUCTION: // F5 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								FTthreshold = Select_FTthreshold(Key_value); // Threshold 설정 함수
								if (FTthreshold == NULL)
									break;
								
								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.AB_ADDUCTION_file, "R_ABDUCTION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.AB_ADDUCTION_file, "L_ABDUCTION.txt", "r+");

								if (KIN.AB_ADDUCTION_file == NULL)
									std::cout << "Abduction file Open Failed..." << endl;
								fscanf_s(KIN.AB_ADDUCTION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
								KIN.Teaching_Point = M1_data;
								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;
								T_point = KIN.Teaching_Point;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.AB_ADDUCTION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.AB_ADDUCTION_file);
								
								FTsensor_Read(FTvalue);
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}
								ArmWeight = sqrt(pow(FToffset[0], 2) + pow(FToffset[1], 2));

								start_time = clock();  // 시작시간 기록
								fopen_s(&KIN.Measure_file, "Measure_file.txt", "w"); // 쓰기용으로 파일 생성

								while (1)
								{
									end_time = clock();  // 현재시간 측정
									measure_time = (long)(end_time - start_time); // 수행시간 계산
									
									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);
									/* Data출력 : 순서 시간 모터1 모터2 모터3 모터4 Fx Fy Fz Tx Ty Tz */
									fprintf(KIN.Measure_file, "%d %ld %ld %ld %ld %ld %f %f %f %f %f %f\n", 
										measure_count, measure_time, KIN.OutputPos[1], KIN.OutputPos[2], KIN.OutputPos[3], KIN.OutputPos[4], 
										FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									FTtrigger = FTvalue[0] - FToffset[0];
									if (L_R_Key == LEFT_OPERATION)
										FTtrigger = FTtrigger * (-1);

									if (FTtrigger < FTthreshold*(-1))  // 올림
									{
										Step_count = KIN.Teaching_Point;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;

										for (int i = 1; i <= MOTOR_NUMBER; i++)
										{
											EPOS2S.GetPositionDOF(i);
											float distance = abs(KIN.Step_Deg_Range[Step_count][i] - KIN.Step_Deg_Range[1][i]); // 전체 이동 각도 - 초기 각도
											float movetime = cal_time((KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], distance, cal_speed(KIN.Teaching_Point)); // cal_time(현재 각도, 이동 각도, 전체 이동각도 - 초기각도, 전체 이동 시간)
											EPOS2S.Cal_vel(i, (KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], movetime);
										}

										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MoveDOF(i, KIN.Step_Deg_Range[Step_count][i]);
										
										if (_kbhit())
										{
											Temp = _getch();
											if (Temp == S || Temp == s)
											{
												ROM_value[1] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
											else
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
										}
									}
										
									else if (FTtrigger > FTthreshold)  // 내림
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;

										for (int i = 1; i <= MOTOR_NUMBER; i++)
										{
											EPOS2S.GetPositionDOF(i);
											float distance = abs(KIN.Step_Deg_Range[Step_count][i] - KIN.Step_Deg_Range[KIN.Teaching_Point][i]);
											float movetime = cal_time((KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], distance, cal_speed(KIN.Teaching_Point));
											EPOS2S.Cal_vel(i, (KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], movetime);
										}

										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MoveDOF(i, KIN.Step_Deg_Range[Step_count][i]);

										if (_kbhit())
										{
											Temp = _getch();
											if (Temp == S || Temp == s)
											{
												ROM_value[1] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
											else
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
										}
									}

									else
									{
										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MotorStop(i);
									}

									if (_kbhit())
									{
										Temp = _getch();
										if (Temp == S || Temp == s)
										{
											ROM_value[1] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
											for (int i = 1; i <= MOTOR_NUMBER; i++)
												EPOS2S.MotorStop(i);
											break;
										}
										else
										{
											for (int i = 1; i <= MOTOR_NUMBER; i++)
												EPOS2S.MotorStop(i);
											break;
										}
									}
									measure_count++;  // 측정카운트
								}
								fclose(KIN.Measure_file);  //측정파일 닫기
								break;

							case ADDUCTION: // F6 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								FTthreshold = Select_FTthreshold(Key_value); // Threshold 설정 함수
								if (FTthreshold == NULL)
									break;

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.AB_ADDUCTION_file, "R_ADDUCTION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.AB_ADDUCTION_file, "L_ADDUCTION.txt", "r+");

								if (KIN.AB_ADDUCTION_file == NULL)
									std::cout << "Adduction file Open Failed..." << endl;
								fscanf_s(KIN.AB_ADDUCTION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
								KIN.Teaching_Point = M1_data;

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;
								T_point = KIN.Teaching_Point;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.AB_ADDUCTION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.AB_ADDUCTION_file);

								FTsensor_Read(FTvalue);
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}
								ArmWeight = sqrt(pow(FTvalue[0], 2) + pow(FTvalue[1], 2));

								start_time = clock();  // 시작시간 기록
								fopen_s(&KIN.Measure_file, "Measure_file.txt", "w"); // 쓰기용으로 파일 생성

								while (1)
								{
									end_time = clock();  // 현재시간 측정
									measure_time = (long)(end_time - start_time); // 수행시간 계산
									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);
									/* Data출력 : 순서 시간(ms) 모터1 모터2 모터3 모터4 Fx Fy Fz Tx Ty Tz */
									fprintf(KIN.Measure_file, "%d %ld %ld %ld %ld %ld %f %f %f %f %f %f\n",
										measure_count, measure_time, KIN.OutputPos[1], KIN.OutputPos[2], KIN.OutputPos[3], KIN.OutputPos[4],
										FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									FTtrigger = (FTvalue[2] - FToffset[2]);
									if (FTtrigger > FTthreshold)
									{
										Step_count = KIN.Teaching_Point;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;

										for (int i = 1; i <= MOTOR_NUMBER; i++)
										{
											EPOS2S.GetPositionDOF(i);
											float distance = abs(KIN.Step_Deg_Range[Step_count][i] - KIN.Step_Deg_Range[1][i]);
											float movetime = cal_time((KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], distance, cal_speed(KIN.Teaching_Point));
											EPOS2S.Cal_vel(i, (KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], movetime);
										}

										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MoveDOF(i, KIN.Step_Deg_Range[Step_count][i]);
										
										if (_kbhit())
										{
											Temp = _getch();
											if (Temp == S || Temp == s)
											{
												ROM_value[2] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
											else
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
										}
									}

									else if (FTtrigger < FTthreshold*(-1))
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;

										for (int i = 1; i <= MOTOR_NUMBER; i++)
										{
											EPOS2S.GetPositionDOF(i);
											float distance = abs(KIN.Step_Deg_Range[Step_count][i] - KIN.Step_Deg_Range[KIN.Teaching_Point][i]);
											float movetime = cal_time((KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], distance, cal_speed(KIN.Teaching_Point));
											EPOS2S.Cal_vel(i, (KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], movetime);
										}

										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MoveDOF(i, KIN.Step_Deg_Range[Step_count][i]);
										
										if (_kbhit())
										{
											Temp = _getch();
											if (Temp == S || Temp == s)
											{
												ROM_value[2] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
											else
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
										}
									}

									else
									{
										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MotorStop(i);
									}

									if (_kbhit())
									{
										Temp = _getch();
										if (Temp == S || Temp == s)
										{
											ROM_value[2] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
											for (int i = 1; i <= MOTOR_NUMBER; i++)
												EPOS2S.MotorStop(i);
											break;
										}
										else
										{
											for (int i = 1; i <= MOTOR_NUMBER; i++)
												EPOS2S.MotorStop(i);
											break;
										}
									}
									measure_count++;  // 측정카운트
								}
								fclose(KIN.Measure_file);  //측정파일 닫기
								break;

							case EX_ROTATION: // F7 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								FTthreshold = Select_FTthreshold(Key_value); // Threshold 설정 함수
								if (FTthreshold == NULL)
									break;

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.EX_IN_ROTATION_file, "R_EX_ROTATION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.EX_IN_ROTATION_file, "L_EX_ROTATION.txt", "r+");

								if (KIN.EX_IN_ROTATION_file == NULL)
									std::cout << "EX_ROTATION_file Open Failed..." << endl;
								fscanf_s(KIN.EX_IN_ROTATION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
								KIN.Teaching_Point = M1_data;

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;
								T_point = KIN.Teaching_Point;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.EX_IN_ROTATION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.EX_IN_ROTATION_file);

								FTsensor_Read(FTvalue);
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}

								start_time = clock();  // 시작시간 기록
								fopen_s(&KIN.Measure_file, "Measure_file.txt", "w"); // 쓰기용으로 파일 생성

								while (1)
								{
									end_time = clock();  // 현재시간 측정
									measure_time = (long)(end_time - start_time); // 수행시간 계산

									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);
									/* Data출력 : 순서 시간(ms) 모터1 모터2 모터3 모터4 Fx Fy Fz Tx Ty Tz */
									fprintf(KIN.Measure_file, "%d %ld %ld %ld %ld %ld %f %f %f %f %f %f\n",
										measure_count, measure_time, KIN.OutputPos[1], KIN.OutputPos[2], KIN.OutputPos[3], KIN.OutputPos[4],
										FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									FTtrigger = (FTvalue[4] - FToffset[4]) * 20;
									if (L_R_Key == LEFT_OPERATION)
										FTtrigger = FTtrigger * (-1);

									if (FTtrigger < FTthreshold*(-1))
									{
										Step_count = KIN.Teaching_Point;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;

										for (int i = 1; i <= MOTOR_NUMBER; i++)
										{
											EPOS2S.GetPositionDOF(i);
											float distance = abs(KIN.Step_Deg_Range[Step_count][i] - KIN.Step_Deg_Range[1][i]);
											float movetime = cal_time((KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], distance, cal_speed(KIN.Teaching_Point));
											EPOS2S.Cal_vel(i, (KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], movetime);
										}

										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MoveDOF(i, KIN.Step_Deg_Range[Step_count][i]);

										if (_kbhit())
										{
											Temp = _getch();
											if (Temp == S || Temp == s)
											{
												ROM_value[3] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
											else
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
										}
									}

									else if (FTtrigger > FTthreshold)
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;

										for (int i = 1; i <= MOTOR_NUMBER; i++)
										{
											EPOS2S.GetPositionDOF(i);
											float distance = abs(KIN.Step_Deg_Range[Step_count][i] - KIN.Step_Deg_Range[KIN.Teaching_Point][i]);
											float movetime = cal_time((KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], distance, cal_speed(KIN.Teaching_Point));
											EPOS2S.Cal_vel(i, (KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], movetime);
										}

										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MoveDOF(i, KIN.Step_Deg_Range[Step_count][i]);

										if (_kbhit())
										{
											Temp = _getch();
											if (Temp == S || Temp == s)
											{
												ROM_value[3] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
											else
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
										}
									}

									else
									{
										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MotorStop(i);
									}

									if (_kbhit())
									{
										Temp = _getch();
										if (Temp == S || Temp == s)
										{
											ROM_value[3] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
											for (int i = 1; i <= MOTOR_NUMBER; i++)
												EPOS2S.MotorStop(i);
											break;
										}
										else
										{
											for (int i = 1; i <= MOTOR_NUMBER; i++)
												EPOS2S.MotorStop(i);
											break;
										}
									}
									measure_count++;  // 측정카운트
								}
								fclose(KIN.Measure_file);  //측정파일 닫기
								break;

							case IN_ROTATION: // F8 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								FTthreshold = Select_FTthreshold(Key_value); // Threshold 설정 함수
								if (FTthreshold == NULL)
									break;

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.EX_IN_ROTATION_file, "R_IN_ROTATION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.EX_IN_ROTATION_file, "L_IN_ROTATION.txt", "r+");

								if (KIN.EX_IN_ROTATION_file == NULL)
									std::cout << "IN_ROTATION_file Open Failed..." << endl;
								fscanf_s(KIN.EX_IN_ROTATION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
								KIN.Teaching_Point = M1_data;

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;
								T_point = KIN.Teaching_Point;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.EX_IN_ROTATION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.EX_IN_ROTATION_file);

								FTsensor_Read(FTvalue);
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}
								start_time = clock();  // 시작시간 기록
								fopen_s(&KIN.Measure_file, "Measure_file.txt", "w"); // 쓰기용으로 파일 생성

								while (1)
								{
									end_time = clock();  // 현재시간 측정
									measure_time = (long)(end_time - start_time); // 수행시간 계산

									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);
									/* Data출력 : 순서 시간(ms) 모터1 모터2 모터3 모터4 Fx Fy Fz Tx Ty Tz */
									fprintf(KIN.Measure_file, "%d %ld %ld %ld %ld %ld %f %f %f %f %f %f\n",
										measure_count, measure_time, KIN.OutputPos[1], KIN.OutputPos[2], KIN.OutputPos[3], KIN.OutputPos[4],
										FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									FTtrigger = (FTvalue[4] - FToffset[4])*20;
									if (L_R_Key == LEFT_OPERATION)
										FTtrigger = FTtrigger * (-1);

									if (FTtrigger > FTthreshold)
									{
										Step_count = KIN.Teaching_Point;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;

										for (int i = 1; i <= MOTOR_NUMBER; i++)
										{
											EPOS2S.GetPositionDOF(i);
											float distance = abs(KIN.Step_Deg_Range[Step_count][i] - KIN.Step_Deg_Range[1][i]);
											float movetime = cal_time((KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], distance, cal_speed(KIN.Teaching_Point));
											EPOS2S.Cal_vel(i, (KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], movetime);
										}

										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MoveDOF(i, KIN.Step_Deg_Range[Step_count][i]);

										if (_kbhit())
										{
											Temp = _getch();
											if (Temp == S || Temp == s)
											{
												ROM_value[4] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
											else
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
										}
									}

									else if (FTtrigger < FTthreshold*(-1))
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;

										for (int i = 1; i <= MOTOR_NUMBER; i++)
										{
											EPOS2S.GetPositionDOF(i);
											float distance = abs(KIN.Step_Deg_Range[Step_count][i] - KIN.Step_Deg_Range[KIN.Teaching_Point][i]);
											float movetime = cal_time((KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], distance, cal_speed(KIN.Teaching_Point));
											EPOS2S.Cal_vel(i, (KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], movetime);
										}

										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MoveDOF(i, KIN.Step_Deg_Range[Step_count][i]);

										if (_kbhit())
										{
											Temp = _getch();
											if (Temp == S || Temp == s)
											{
												ROM_value[4] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
											else
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
										}
									}

									else
									{
										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MotorStop(i);
									}

									if (_kbhit())
									{
										Temp = _getch();
										if (Temp == S || Temp == s)
										{
											ROM_value[4] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
											for (int i = 1; i <= MOTOR_NUMBER; i++)
												EPOS2S.MotorStop(i);
											break;
										}
										else
										{
											for (int i = 1; i <= MOTOR_NUMBER; i++)
												EPOS2S.MotorStop(i);
											break;
										}
									}
									measure_count++;  // 측정카운트
								}
								fclose(KIN.Measure_file);  //측정파일 닫기
								break;

							case FLEXION: // F9 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								FTthreshold = Select_FTthreshold(Key_value); // Threshold 설정 함수
								if (FTthreshold == NULL)
									break;

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.EX_FLEXION_file, "R_FLEXION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.EX_FLEXION_file, "L_FLEXION.txt", "r+");

								if (KIN.EX_FLEXION_file == NULL)
									std::cout << "Extension file Open Failed..." << endl;
								fscanf_s(KIN.EX_FLEXION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
								KIN.Teaching_Point = M1_data;

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;
								T_point = KIN.Teaching_Point;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.EX_FLEXION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}
								fclose(KIN.EX_FLEXION_file);

								FTsensor_Read(FTvalue);
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}
								ArmWeight = sqrt(pow(FToffset[0], 2) + pow(FToffset[1], 2));
								start_time = clock();  // 시작시간 기록
								fopen_s(&KIN.Measure_file, "Measure_file.txt", "w"); // 쓰기용으로 파일 생성

								while (1)
								{
									end_time = clock();  // 현재시간 측정
									measure_time = (long)(end_time - start_time); // 수행시간 계산

									FTsensor_Read(FTvalue);
									printf("Offset: %f\n", FToffset[0]);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);
									/* Data출력 : 순서 시간(ms) 모터1 모터2 모터3 모터4 Fx Fy Fz Tx Ty Tz */
									fprintf(KIN.Measure_file, "%d %ld %ld %ld %ld %ld %f %f %f %f %f %f\n",
										measure_count, measure_time, KIN.OutputPos[1], KIN.OutputPos[2], KIN.OutputPos[3], KIN.OutputPos[4],
										FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									FTtrigger = FTvalue[0] - FToffset[0];
									if (L_R_Key == LEFT_OPERATION)
										FTtrigger = FTtrigger * (-1);

									if (FTtrigger < FTthreshold*(-1))
									{
										Step_count = KIN.Teaching_Point;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;

										for (int i = 1; i <= MOTOR_NUMBER; i++)
										{
											EPOS2S.GetPositionDOF(i);
											float distance = abs(KIN.Step_Deg_Range[Step_count][i] - KIN.Step_Deg_Range[1][i]);
											float movetime = cal_time((KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], distance, cal_speed(KIN.Teaching_Point));
											EPOS2S.Cal_vel(i, (KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], movetime);
										}

										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MoveDOF(i, KIN.Step_Deg_Range[Step_count][i]);

										if (_kbhit())
										{
											Temp = _getch();
											if (Temp == S || Temp == s)
											{
												ROM_value[5] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
											else
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
										}

									}

									else if (FTtrigger > FTthreshold)
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;

										for (int i = 1; i <= MOTOR_NUMBER; i++)
										{
											EPOS2S.GetPositionDOF(i);
											float distance = abs(KIN.Step_Deg_Range[Step_count][i] - KIN.Step_Deg_Range[KIN.Teaching_Point][i]);
											float movetime = cal_time((KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], distance, cal_speed(KIN.Teaching_Point));
											EPOS2S.Cal_vel(i, (KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], movetime);
										}

										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MoveDOF(i, KIN.Step_Deg_Range[Step_count][i]);

										if (_kbhit())
										{
											Temp = _getch();
											if (Temp == S || Temp == s)
											{
												ROM_value[5] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
											else
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
										}

									}

									else
									{
										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MotorStop(i);
									}

									if (_kbhit())
									{
										Temp = _getch();
										if (Temp == S || Temp == s)
										{
											ROM_value[5] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
											for (int i = 1; i <= MOTOR_NUMBER; i++)
												EPOS2S.MotorStop(i);
											break;
										}
										else
										{
											for (int i = 1; i <= MOTOR_NUMBER; i++)
												EPOS2S.MotorStop(i);
											break;
										}
									}
									measure_count++;  // 측정카운트
								}
								fclose(KIN.Measure_file);  //측정파일 닫기
								break;


							case EXTENSION: // F10 Key
								if (L_R_Key == RIGHT_OPERATION)
									R_Start_Degree(Key_value);
								else if (L_R_Key == LEFT_OPERATION)
									L_Start_Degree(Key_value);

								FTthreshold = Select_FTthreshold(Key_value); // Threshold 설정 함수
								if (FTthreshold == NULL)
									break;

								if (L_R_Key == RIGHT_OPERATION)
									fopen_s(&KIN.EX_FLEXION_file, "R_EXTENSION.txt", "r+");
								else if (L_R_Key == LEFT_OPERATION)
									fopen_s(&KIN.EX_FLEXION_file, "L_EXTENSION.txt", "r+");

								if (KIN.EX_FLEXION_file == NULL)
									std::cout << "Flexion file Open Failed..." << endl;
								fscanf_s(KIN.EX_FLEXION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
								KIN.Teaching_Point = M1_data;

								KIN.Teaching_Point = Select_ROM(Key_value, KIN.Teaching_Point, ROM_value); // ROM to Teaching_Point
								if (KIN.Teaching_Point == NULL)
									break;
								T_point = KIN.Teaching_Point;

								for (int i = 1; i <= KIN.Teaching_Point; i++)
								{
									fscanf_s(KIN.EX_FLEXION_file, "%d %d %d %d %d\n", &Step_data, &M1_data, &M2_data, &M3_data, &M4_data);
									KIN.Step_Deg_Range[Step_data][1] = M1_data;
									KIN.Step_Deg_Range[Step_data][2] = M2_data;
									KIN.Step_Deg_Range[Step_data][3] = M3_data;
									KIN.Step_Deg_Range[Step_data][4] = M4_data;
								}

								fclose(KIN.EX_FLEXION_file);

								FTsensor_Read(FTvalue);
								for (int i = 0; i < 6; i++)
								{
									FToffset[i] = FTvalue[i];
								}
								ArmWeight = sqrt(pow(FTvalue[0], 2) + pow(FTvalue[1], 2));
								start_time = clock();  // 시작시간 기록
								fopen_s(&KIN.Measure_file, "Measure_file.txt", "w"); // 쓰기용으로 파일 생성

								while (1)
								{
									end_time = clock();  // 현재시간 측정
									measure_time = (long)(end_time - start_time); // 수행시간 계산

									FTsensor_Read(FTvalue);
									printf("Fx: %f, Fy: %f, Fz: %f, Tx: %f, Ty: %f, Tz: %f\n", FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);
									/* Data출력 : 순서 시간(ms) 모터1 모터2 모터3 모터4 Fx Fy Fz Tx Ty Tz */
									fprintf(KIN.Measure_file, "%d %ld %ld %ld %ld %ld %f %f %f %f %f %f\n",
										measure_count, measure_time, KIN.OutputPos[1], KIN.OutputPos[2], KIN.OutputPos[3], KIN.OutputPos[4],
										FTvalue[0], FTvalue[1], FTvalue[2], FTvalue[3], FTvalue[4], FTvalue[5]);

									FTtrigger = ((FTvalue[0] - FToffset[0]) + (FTvalue[4] - FToffset[4])*(10)) / 2;
									if (L_R_Key == LEFT_OPERATION)
										FTtrigger = FTtrigger * (-1);

									if (FTtrigger > FTthreshold)
									{
										Step_count = KIN.Teaching_Point;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;

										for (int i = 1; i <= MOTOR_NUMBER; i++)
										{
											EPOS2S.GetPositionDOF(i);
											float distance = abs(KIN.Step_Deg_Range[Step_count][i] - KIN.Step_Deg_Range[1][i]);
											float movetime = cal_time((KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], distance, cal_speed(KIN.Teaching_Point));
											EPOS2S.Cal_vel(i, (KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], movetime);
										}

										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MoveDOF(i, KIN.Step_Deg_Range[Step_count][i]);

										if (_kbhit())
										{
											Temp = _getch();
											if (Temp == S || Temp == s)
											{
												ROM_value[6] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
											else
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
										}
									}

									else if (FTtrigger < FTthreshold*(-1))
									{
										Step_count = 1;
										cout << "[ Move Teaching Point_" << Step_count << " ]" << endl;

										for (int i = 1; i <= MOTOR_NUMBER; i++)
										{
											EPOS2S.GetPositionDOF(i);
											float distance = abs(KIN.Step_Deg_Range[Step_count][i] - KIN.Step_Deg_Range[KIN.Teaching_Point][i]);
											float movetime = cal_time((KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], distance, cal_speed(KIN.Teaching_Point));
											EPOS2S.Cal_vel(i, (KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step_count][i], movetime);
										}

										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MoveDOF(i, KIN.Step_Deg_Range[Step_count][i]);

										if (_kbhit())
										{
											Temp = _getch();
											if (Temp == S || Temp == s)
											{
												ROM_value[6] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
											else
											{
												for (int i = 1; i <= MOTOR_NUMBER; i++)
													EPOS2S.MotorStop(i);
												break;
											}
										}
									}

									else
									{
										for (int i = 1; i <= MOTOR_NUMBER; i++)
											EPOS2S.MotorStop(i);
									}

									if (_kbhit())
									{
										Temp = _getch();
										if (Temp == S || Temp == s)
										{
											ROM_value[6] = Teaching_to_ROM(L_R_Key, Key_value, T_point);
											for (int i = 1; i <= MOTOR_NUMBER; i++)
												EPOS2S.MotorStop(i);
											break;
										}
										else
										{
											for (int i = 1; i <= MOTOR_NUMBER; i++)
												EPOS2S.MotorStop(i);
											break;
										}
									}
									measure_count++;  // 측정카운트
								}
								fclose(KIN.Measure_file);  //측정파일 닫기
								break;

							default:
								break;
							}
						}
					}
					break;

				case ORIGIN_POSITION:
					if (L_R_Key == RIGHT_OPERATION)
						R_Origin_Position(L_R_Key);
					else if (L_R_Key == LEFT_OPERATION)
						L_Origin_Position(L_R_Key);
					else
						break;

				default:
					break;
				}
			}
		}
	}
}