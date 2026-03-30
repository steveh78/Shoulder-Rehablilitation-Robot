#include "Stdafx.h"
#include <stdint.h>  // 비트 길이 고유한 데이터 형식 헤더파일

#include <stdio.h>   // C언어의 표준 라이브러리 함수의 매크로 정의, 상수, 여러 형의 입출력 함수가 포함된 헤더 파일.
#include <windows.h> // 윈도우 매크로, 데이터 타입, 윈도우 API 함수들을 정의하는 C/C++ 헤더파일

#define _USE_MATH_DEFINES
#include <math.h>

#include "NIDAQmx.h" // Include file for NI-DAQmx  library support.

int FTsensor_Initialize();				// FTsensor 초기화
int FTsensor_Read(double* FTvalue);     // FTsensor Read
int FTsensor_Terminate();				// FTsensor 터미널
