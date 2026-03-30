
#include "SinglePort.h"


/************************************************************************
Opens communication interface of the Master System
************************************************************************/
void Comm_Cmd::Comm_Open()
{
	DCB dcb;

	BOOL fSuccess;
	char pcCommPort[] = "COM5";

	hCom = CreateFile(pcCommPort,
		GENERIC_READ | GENERIC_WRITE,
		0,    // must be opened with exclusive-access
		NULL, // no security attributes
		OPEN_EXISTING, // must use OPEN_EXISTING
		0,    // not overlapped I/O
		NULL  // hTemplate must be NULL for comm devices
	);

	if (hCom == INVALID_HANDLE_VALUE)
	{
		// Handle the error.
		printf("Master System : %s Open Failed...\n", pcCommPort);
		printf("%S", hCom);
		system("PAUSE");
		exit(0);
	}

	// Build on the current configuration, and skip setting the size
	// of the input and output buffers with SetupComm.

	fSuccess = GetCommState(hCom, &dcb);

	if (!fSuccess)
	{
		// Handle the error.
		printf("Master System : GetCommState failed with error %d.\n", GetLastError());
		system("PAUSE");
		exit(0);
	}

	// Fill in DCB: 57,600 bps, 8 data bits, no parity, and 1 stop bit.
	dcb.BaudRate =   115200;     // set the baud rate
	//dcb.BaudRate = 1000000;     // set the baud rate
								//dcb.BaudRate = CBR_57600;     // set the baud rate
	dcb.ByteSize = 8;             // data size, xmit, and rcv
	dcb.Parity = NOPARITY;        // no parity bit
	dcb.StopBits = ONESTOPBIT;    // one stop bit

	fSuccess = SetCommState(hCom, &dcb);

	if (!fSuccess)
	{
		// Handle the error.
		printf("Master System : SetCommState failed with error %d.\n", GetLastError());
		system("PAUSE");
		exit(0);
	}

	printf("Master System : Serial port %s success...\n", pcCommPort);

	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;

	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (!SetCommTimeouts(hCom, &timeouts)) {
		// Error set timeouts
		printf("Error : Setting timeouts\n");
	}
}

/************************************************************************
Send data
************************************************************************/
void Comm_Cmd::TxData()
{
	DWORD dwBytesWrite = 0;
	int writeSize = 10;
	unsigned char str[10] = "MagmaTart";

	if (WriteFile(hCom, str, writeSize, &dwBytesWrite, NULL)) {
		// Write Complete
	}
}

/************************************************************************
Receive data
************************************************************************/
BOOL Comm_Cmd::RxData(int readSize)
{
	DWORD dwBytesRead = 0;
	//int readSize = 1;

	if (ReadFile(hCom, Comm_buff, readSize, &dwBytesRead, NULL))	return 1;
	else															return 0;
	//ReadFile(hCom, Comm_buff, readSize, &dwBytesRead, NULL);

	//if (ReadFile(hCom, Comm_buff, readSize, &dwBytesRead, NULL)) {
	//	for (int i = 0; i < 100; i++) printf("%c", Comm_buff[i]);
	//	//printf("\n");
	//}
}

/************************************************************************
Get Position Value
************************************************************************/
void Comm_Cmd::GeMasterPosition()
{
	COM.Data_status = 0;
	CheckSum = 0;
	Sum = 0;

	for (int i = 0; i < DTL; i++)
		fill(&COM.Data[i][0], &COM.Data[i][0] + DIGIT, 0);
		//for (int j = 0; j < DIGIT; j++)
		//	fill(&COM.Data[i][j], &COM.Data[i][j], 0);

	fill(COM.Data_Result, COM.Data_Result + DTL, 0);
	
	COM.RxData(1);
	if (COM.Comm_buff[0] == STX)
	{						
		COM.RxData(3);						
		if (COM.Comm_buff[0] == CMA && COM.Comm_buff[1] == 0x10 && COM.Comm_buff[2] == CMA)
		{			
			for (int i = 0; i < DTL; i++)
			{
				for (int j = 0; j < DIGIT; j++)
				{					
					COM.RxData(1);
					if (COM.Comm_buff[0] >= 0x30 && COM.Comm_buff[0] <= 0x39)
					{								
						COM.Data[i][j] = COM.Comm_buff[0] - 0x30;
						COM.Data_Result[i] = COM.Data_Result[i] * 10 + COM.Data[i][j];						
					}
					else if (COM.Comm_buff[0] == CMA)
					{
						Sum += COM.Data_Result[i];
						COM.Data_status = INIT;						
						break;
					}
					else
					{
						COM.Data_status = FAIL;
						break;
					}					
				}				

				if (COM.Data_status == FAIL)	break;
				else if (i >= (DTL - 1))
				{
					CheckSum = Sum;
					CheckSum = CheckSum & 0xFF;

					if ((CheckSum != 0) ||
						(COM.Data_Result[FORCEP_B] > 1 || COM.Data_Result[FORCEP_B] < 0) ||
						(COM.Data_Result[FOOT_B]   > 1 || COM.Data_Result[FOOT_B]   < 0) ||
						(COM.Data_Result[INIT_B]   > 1 || COM.Data_Result[INIT_B]   < 0))
					{
						//cout << "[[[[[[ COM.Data_status = FAIL ]]]]]" << endl;
						COM.Data_status = FAIL;
						break;
					}
					else
						COM.Data_status = OK;
				}
			}
			
			if (COM.Data_status == OK)
			{
				if (COM.Data_Result[INIT_B] == ON)
				{
					// Kinemaics 수식 상의 Z축 +방향과 실제 마스터의 포텐셔미터 Z축 +방향이 바뀌어서 맵핑을 시켜 준다.
					// 포텐셔미터 Range상의 90도는 1070 Pulse. 
					COM.Master_InitP[BASE] = COM.Data_Result[BASE];
					COM.Master_InitP[SHOULDER] = COM.Data_Result[SHOULDER] - Poten_Range;
					COM.Master_InitP[ELBOW] = COM.Data_Result[ELBOW] + Poten_Range;
					COM.Master_InitP[ROLL] = COM.Data_Result[ROLL] + Poten_Range;
					COM.Master_InitP[PITCH] = COM.Data_Result[PITCH];
					COM.Master_InitP[YAW] = COM.Data_Result[YAW] + Poten_Range;
				}

				//Z축 정방향 회전이 +이지만 마스터 시스템의 각 Z축(Base, YAW제외) 포텐셔미터 정방향이 - 이다. 이에 따른 맵핑 코드 작성. 
				KIN.MD_Temp[BASE] = (COM.Master_InitP[BASE] - COM.Data_Result[BASE]) * MASTER_DPP;
				KIN.MD_Temp[SHOULDER] = (-COM.Master_InitP[SHOULDER] + COM.Data_Result[SHOULDER]) * MASTER_DPP;
				KIN.MD_Temp[ELBOW] = (COM.Master_InitP[ELBOW] - COM.Data_Result[ELBOW]) * MASTER_DPP;
				KIN.MD_Temp[ROLL] = (COM.Master_InitP[ROLL] - COM.Data_Result[ROLL]) * MASTER_DPP;
				KIN.MD_Temp[PITCH] = (-COM.Master_InitP[PITCH] + COM.Data_Result[PITCH]) * MASTER_DPP;
				KIN.MD_Temp[YAW] = (COM.Master_InitP[YAW] - COM.Data_Result[YAW]) * MASTER_DPP;

				KIN.MD_Th[FORCEP_B] = COM.Data_Result[FORCEP_B];
				KIN.MD_Th[FOOT_B] = COM.Data_Result[FOOT_B];
				KIN.MD_Th[INIT_B] = COM.Data_Result[INIT_B];

				// 5회 평균 각도 값 추출 알고리즘 적용.
				// 5개 값 중 한 개의 값은 버리고, 4개 값을 쉬프트 시킨다.
				for (int i = 0; i <= YAW; i++)
					for (int j = 0; j < (ITERNUM - 1); j++)
						A_Buff[i][j] = A_Buff[i][j + 1];

				// 새로 받은 각도 값을 배열의 맨 끝에 입력 한다.
				A_Buff[BASE][(ITERNUM - 1)] = KIN.MD_Temp[BASE];
				A_Buff[SHOULDER][(ITERNUM - 1)] = KIN.MD_Temp[SHOULDER];
				A_Buff[ELBOW][(ITERNUM - 1)] = KIN.MD_Temp[ELBOW];
				A_Buff[ROLL][(ITERNUM - 1)] = KIN.MD_Temp[ROLL];
				A_Buff[PITCH][(ITERNUM - 1)] = KIN.MD_Temp[PITCH];
				A_Buff[YAW][(ITERNUM - 1)] = KIN.MD_Temp[YAW];

				// 5개 값을 모두 더한다.
				for (int i = 0; i <= YAW; i++)
					for (int j = 0; j < ITERNUM; j++)
						M_Sum[i] += A_Buff[i][j];

				// 평균 값을 구한다.
				M_Avr[BASE] = M_Sum[BASE] / ITERNUM;
				M_Avr[SHOULDER] = M_Sum[SHOULDER] / ITERNUM;
				M_Avr[ELBOW] = M_Sum[ELBOW] / ITERNUM;
				M_Avr[ROLL] = M_Sum[ROLL] / ITERNUM;
				M_Avr[PITCH] = M_Sum[PITCH] / ITERNUM;
				M_Avr[YAW] = M_Sum[YAW] / ITERNUM;

				// 합계 값 저장소 초기화
				fill(M_Sum, M_Sum + (YAW + 1), 0);


				// BOUND_RANGE 값 이상일 때 각도 값을 Update한다.
				//for(int i=0; i<= YAW;i++)
				//	if (abs(KIN.MD_Th[i] - M_Avr[i]) > BOUND_RANGE) KIN.MD_Th[i] = M_Avr[i];

				for (int i = 0; i <= YAW; i++)
				{
					if (abs(KIN.MD_Th[i] - M_Avr[i]) > BOUND_RANGE)
					{
						if (M_Avr[i] < 0)	KIN.MD_Th[i] = 0;
						else if (M_Avr[i] > KIN.Robot_DOF_Range[i])	KIN.MD_Th[i] = KIN.Robot_DOF_Range[i];
						else
							KIN.MD_Th[i] = (int)M_Avr[i];
					}
				}

				// Degree 값을 Radian값으로 변경
				KIN.MR_Th[BASE] = KIN.MD_Th[BASE] * DTR;
				KIN.MR_Th[SHOULDER] = KIN.MD_Th[SHOULDER] * DTR;
				KIN.MR_Th[ELBOW] = KIN.MD_Th[ELBOW] * DTR;
				KIN.MR_Th[ROLL] = KIN.MD_Th[ROLL] * DTR;
				KIN.MR_Th[PITCH] = KIN.MD_Th[PITCH] * DTR;
				KIN.MR_Th[YAW] = KIN.MD_Th[YAW] * DTR;
				KIN.MR_Th[FORCEP_B] = KIN.MD_Th[FORCEP_B];
				KIN.MR_Th[FOOT_B] = KIN.MD_Th[FOOT_B];
				KIN.MR_Th[INIT_B] = KIN.MD_Th[INIT_B];
#if CAL_DEGREE
				cout.precision(6);
				cout << " 1_DOF : " << KIN.MD_Th[BASE] << " ";
				cout << " 2_DOF : " << KIN.MD_Th[SHOULDER] << " ";
				cout << " 3_DOF : " << KIN.MD_Th[ELBOW] << " ";
				cout << " 4_DOF : " << KIN.MD_Th[ROLL] << " ";
				cout << " 5_DOF : " << KIN.MD_Th[PITCH] << " ";
				cout << " 6_DOF : " << KIN.MD_Th[YAW] << " ";
				cout << " Fcep  : " << KIN.MD_Th[FORCEP_B] << " ";
				cout << " Foot  : " << KIN.MD_Th[FOOT_B] << " ";
				cout << " Init  : " << KIN.MD_Th[INIT_B] << endl;
#endif
				//if (KIN.MD_Th[FORCEP_B] != OFF)
				//{
				//	if (KIN.MD_Th[FOOT_B] == ON && KIN.MD_Th[R_ANG6] >= 30)
				//	{
				//		KIN.MD_Th[R_ANG6] = 30;
				//	}
				//}
				//else if (KIN.MD_Th[FOOT_B] == ON && KIN.MD_Th[R_ANG6] >= 88)
				//{
				//	KIN.MD_Th[R_ANG6] = 30;
				//}
			}
		}
	}
}



