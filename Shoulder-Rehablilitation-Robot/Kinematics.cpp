
#include "SinglePort.h"


/************************************************************************
저장된 Init Original Position 값을 불러온다. (Right)
************************************************************************/
void Kinematics::R_BackUp_OriginPosition()
{	
	int vel, st;

	fopen_s(&R_Origin_Pos_file, "R_Origin_Pos.txt", "r+");

	if (R_Origin_Pos_file == NULL)
		std::cout << "Start_Pos_file Open Failed..." << endl;
	else						
		std::cout << "Start_Pos_file Open Success..." << endl;

	fscanf_s(R_Origin_Pos_file, "%d %ld %ld %ld %ld", &f_Num, &f_Data1, &f_Data2, &f_Data3, &f_Data4);
	fscanf_s(R_Origin_Pos_file, "%d %ld %ld %ld %ld", &f_Num, &f_Data1, &f_Data2, &f_Data3, &f_Data4);
	Motor_Origin[1] = f_Data1;
	Motor_Origin[2] = f_Data2;
	Motor_Origin[3] = f_Data3;
	Motor_Origin[4] = f_Data4;
	
	for (int i = 1; i < MOTOR_NUMBER + 1; i++)
		Origin_Pos[i] = Motor_Origin[i];

	for (int i = 1; i < MOTOR_NUMBER + 1; i++)
	{
		st = 5.0;
		vel = abs(abs(KIN.OutputPos[i] * ROBOT_DPP) - abs(Motor_Origin[i] * ROBOT_DPP)) * 100 * 60 / 360 / st;

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
		EPOS2S.MotorMove(i, Origin_Pos[i]);
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
	fclose(R_Origin_Pos_file);
}

/************************************************************************
저장된 Init Original Position 값을 불러온다. (Left)
************************************************************************/
void Kinematics::L_BackUp_OriginPosition()
{
	int st, vel;

	fopen_s(&L_Origin_Pos_file, "L_Origin_Pos.txt", "r+");

	if (L_Origin_Pos_file == NULL)
		std::cout << "Start_Pos_file Open Failed..." << endl;
	else
		std::cout << "Start_Pos_file Open Success..." << endl;

	fscanf_s(L_Origin_Pos_file, "%d %ld %ld %ld %ld", &f_Num, &f_Data1, &f_Data2, &f_Data3, &f_Data4);
	fscanf_s(L_Origin_Pos_file, "%d %ld %ld %ld %ld", &f_Num, &f_Data1, &f_Data2, &f_Data3, &f_Data4);
	Motor_Origin[1] = f_Data1;
	Motor_Origin[2] = f_Data2;
	Motor_Origin[3] = f_Data3;
	Motor_Origin[4] = f_Data4;

	for (int i = 1; i < MOTOR_NUMBER + 1; i++)
	{
		Origin_Pos[i] = Motor_Origin[i]; 
	}

	for (int i = 1; i < MOTOR_NUMBER + 1; i++)
	{
		st = 5.0;
		vel = abs(abs(KIN.OutputPos[i] * ROBOT_DPP) - abs(Motor_Origin[i] * ROBOT_DPP)) * 100 * 60 / 360 / st;

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
		EPOS2S.MotorMove(i, Origin_Pos[i]);
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
	fclose(L_Origin_Pos_file);
}

/************************************************************************
저장된 Start Position 값을 불러온다. (Right)
************************************************************************/
void Kinematics::R_BackUp_StartPosition()
{
	fopen_s(&Start_Pos_file, "R_Start_Pos_file.txt", "r+");

	if (Start_Pos_file == NULL)	
		std::cout << "R_Start_Pos_file Open Failed..." << endl;
	//else						
		//std::cout << "R_Start_Pos_file Open Success..." << endl;

	for (int i = 1; i <= MOTOR_NUMBER; i++)
	{
		fscanf_s(Start_Pos_file, "%d %ld", &f_Num, &f_Data);
		Start_Pos[f_Num] = f_Data;
	}
	fclose(Start_Pos_file);
}

/************************************************************************
저장된 Start Position 값을 불러온다. (Left)
************************************************************************/
void Kinematics::L_BackUp_StartPosition()
{
	fopen_s(&Start_Pos_file, "L_Start_Pos_file.txt", "r+");

	if (Start_Pos_file == NULL)	std::cout << "Start_Pos_file Open Failed..." << endl;
	//else						std::cout << "Start_Pos_file Open Success..." << endl;

	for (int i = 1; i < MOTOR_NUMBER + 1; i++)
	{
		fscanf_s(Start_Pos_file, "%d %ld", &f_Num, &f_Data);
		Start_Pos[f_Num] = f_Data;
	}
	fclose(Start_Pos_file);
}

/************************************************************************
저장된 End Position 값을 불러온다. (Right)
************************************************************************/
void Kinematics::R_BackUp_EndPosition()
{	
	fopen_s(&End_Pos_file, "R_End_Pos_file.txt", "r+");

	if (End_Pos_file == NULL)	
		std::cout << "End_Pos_file Open Failed..." << endl;
	//else						
		//std::cout << "End_Pos_file Open Success..." << endl;

	for (int i = 1; i <= MOTOR_NUMBER; i++)
	{
		fscanf_s(End_Pos_file, "%d %ld", &f_Num, &f_Data);
		End_Pos[f_Num] = f_Data;
	}
	fclose(End_Pos_file);
}

/************************************************************************
저장된 End Position 값을 불러온다. (Left)
************************************************************************/
void Kinematics::L_BackUp_EndPosition()
{
	fopen_s(&End_Pos_file, "L_End_Pos_file.txt", "r+");

	if (End_Pos_file == NULL)	std::cout << "End_Pos_file Open Failed..." << endl;
	//else						std::cout << "End_Pos_file Open Success..." << endl;

	for (int i = 1; i < MOTOR_NUMBER + 1; i++)
	{
		fscanf_s(End_Pos_file, "%d %ld", &f_Num, &f_Data);
		End_Pos[f_Num] = f_Data;
	}
	fclose(End_Pos_file);
}

int Kinematics :: Start_End_Limit()
{
	int result;

	EPOS2S.GetPosition(1, &KIN.OutputPos[1]);
	EPOS2S.GetPosition(2, &KIN.OutputPos[2]);
	EPOS2S.GetPosition(3, &KIN.OutputPos[3]);
	EPOS2S.GetPosition(4, &KIN.OutputPos[4]);

	for (int i = 1; i <= MOTOR_NUMBER; i++)
	{
		if ((KIN.OutputPos[i] > KIN.Start_Pos[i]  - START_END_MARGIN - M_DOF_RANGE) 
			&& (KIN.OutputPos[i] < KIN.End_Pos[i] + START_END_MARGIN + M_DOF_RANGE))
			result = True;
		else if (KIN.OutputPos[i] < KIN.Start_Pos[i] - START_END_MARGIN - M_DOF_RANGE)
		{
			printf("\n============== MOTOR %d break away Start_position ===============\n", i);
			printf("OutputPos = %d, KIN.Start_Pos = %d\n", KIN.OutputPos[i], KIN.Start_Pos[i]);
			result = False;
			break;
		}
		else if (KIN.OutputPos[i] > KIN.End_Pos[i] + START_END_MARGIN + M_DOF_RANGE)
		{
			printf("\n============== MOTOR %d break away End_position ===============\n", i);
			printf("OutputPos = %d, KIN.End_Pos = %d\n", KIN.OutputPos[i], KIN.End_Pos[i]);
			result = False;
			break;
		}
	}
	return result;
}

/************************************************************************
저장된 Pulse Range 값을 불러온다.
************************************************************************/
void Kinematics::BackUp_PulseRange()
{	
	fopen_s(&PR_DOF_file, "PR_DOF_file.txt", "r+");

	if (PR_DOF_file == NULL)	std::cout << "PR_DOF_file Open Failed..." << endl;
	//else						std::cout << "PR_DOF_file Open Success..." << endl;

	for (int i = 1; i < 8; i++)
	{
		fscanf_s(PR_DOF_file, "%d %ld", &f_Num, &f_Data);
		PR_DOF[f_Num] = f_Data;
	}
	fclose(PR_DOF_file);
}

/************************************************************************
저장된 Step Move 정보 값을 불러온다.
************************************************************************/
void Kinematics::BackUp_StepInfo()
{	
	fopen_s(&Step_Deg_Range_file, "Step_Deg_Range_file.txt", "rt");

	if (Step_Deg_Range_file == NULL)	std::cout << "Step_Deg_Range_file Open Failed..." << endl;
	//else								std::cout << "Step_Deg_Range_file Open Success..." << endl;

	for (int i = 1; i < 50; i++)
	{
		fscanf_s(Step_Deg_Range_file, "%c", &f_name);
		fscanf_s(Step_Deg_Range_file, "%d %d %d %d %d %d %d %d", &f_Num, &f_Data1, &f_Data2, &f_Data3, &f_Data4, &f_Data5, &f_Data6, &f_Data7);
	
		if (f_name == 's')
		{
			Step_Deg_Range[f_Num][1] = f_Data1;
			Step_Deg_Range[f_Num][2] = f_Data2;
			Step_Deg_Range[f_Num][3] = f_Data3;
			Step_Deg_Range[f_Num][4] = f_Data4;
			Step_Deg_Range[f_Num][5] = f_Data5;
			Step_Deg_Range[f_Num][6] = f_Data6;
			Step_Deg_Range[f_Num][7] = f_Data7;
			fscanf_s(Step_Deg_Range_file, "%c", &f_name);
		}
		else
		{
			Teaching_Point = f_Num;
			break;
		}

	}
	fclose(Step_Deg_Range_file);

}

/************************************************************************
기준이 되는 Angle 값 초기화.
************************************************************************/
void Kinematics::Init_BaseAngle()
{
	//Robot_PulsePerDegre[DCX01] = abs(Start_Pos[DCX01] - End_Pos[DCX01]) / RANGE_DOF01;	// 90000 Pulse  / 60 Degree  = 1500
	//Robot_PulsePerDegre[DCX02] = abs(Start_Pos[DCX02] - End_Pos[DCX02]) / RANGE_DOF02;
	//Robot_PulsePerDegre[DCX03] = abs(Start_Pos[DCX03] - End_Pos[DCX03]) / RANGE_DOF03;
	//Robot_PulsePerDegre[DCX04] = abs(Start_Pos[DCX04] - End_Pos[DCX04]) / RANGE_DOF04;
	//Robot_PulsePerDegre[DCX05] = abs(Start_Pos[DCX05] - End_Pos[DCX05]) / RANGE_DOF05;
	//Robot_PulsePerDegre[DCX06] = abs(Start_Pos[DCX06] - End_Pos[DCX06]) / RANGE_DOF06;
	//Robot_PulsePerDegre[DCX07] = abs(Start_Pos[DCX07] - End_Pos[DCX07]) / RANGE_DOF07;
	//Robot_PulsePerDegre[DCX08] = abs(Start_Pos[DCX08] - End_Pos[DCX08]) / RANGE_DOF08;
	//Robot_PulsePerDegre[DCX09] = abs(Start_Pos[DCX09] - End_Pos[DCX09]) / RANGE_DOF09;
	//Robot_PulsePerDegre[DCX10] = abs(Start_Pos[DCX10] - End_Pos[DCX10]) / RANGE_DOF10;
	//Robot_PulsePerDegre[DCX11] = abs(Start_Pos[DCX11] - End_Pos[DCX11]) / RANGE_DOF11;
	//Robot_PulsePerDegre[DCX12] = abs(Start_Pos[DCX12] - End_Pos[DCX12]) / RANGE_DOF12;
	//Robot_PulsePerDegre[DCX13] = abs(Start_Pos[DCX13] - End_Pos[DCX13]) / RANGE_DOF13;
	//Robot_PulsePerDegre[DCX14] = abs(Start_Pos[DCX14] - End_Pos[DCX14]) / RANGE_DOF14;

	//Robot_DegrePerPulse[DCX01] = RANGE_DOF01 / abs(Start_Pos[DCX01] - End_Pos[DCX01]);
	//Robot_DegrePerPulse[DCX02] = RANGE_DOF02 / abs(Start_Pos[DCX02] - End_Pos[DCX02]);
	//Robot_DegrePerPulse[DCX03] = RANGE_DOF03 / abs(Start_Pos[DCX03] - End_Pos[DCX03]);
	//Robot_DegrePerPulse[DCX04] = RANGE_DOF04 / abs(Start_Pos[DCX04] - End_Pos[DCX04]);
	//Robot_DegrePerPulse[DCX05] = RANGE_DOF05 / abs(Start_Pos[DCX05] - End_Pos[DCX05]);
	//Robot_DegrePerPulse[DCX06] = RANGE_DOF06 / abs(Start_Pos[DCX06] - End_Pos[DCX06]);
	//Robot_DegrePerPulse[DCX07] = RANGE_DOF07 / abs(Start_Pos[DCX07] - End_Pos[DCX07]);
	//Robot_DegrePerPulse[DCX08] = RANGE_DOF08 / abs(Start_Pos[DCX08] - End_Pos[DCX08]);
	//Robot_DegrePerPulse[DCX09] = RANGE_DOF09 / abs(Start_Pos[DCX09] - End_Pos[DCX09]);
	//Robot_DegrePerPulse[DCX10] = RANGE_DOF10 / abs(Start_Pos[DCX10] - End_Pos[DCX10]);
	//Robot_DegrePerPulse[DCX11] = RANGE_DOF11 / abs(Start_Pos[DCX11] - End_Pos[DCX11]);
	//Robot_DegrePerPulse[DCX12] = RANGE_DOF12 / abs(Start_Pos[DCX12] - End_Pos[DCX12]);
	//Robot_DegrePerPulse[DCX13] = RANGE_DOF13 / abs(Start_Pos[DCX13] - End_Pos[DCX13]);
	//14									  14					14				 14

	//// Degree / Pulse_Range = 1펄스당 이동 각도
	//Cal_DPP_DOF[DOF01] = RANGE_DOF01 / (float)PR_DOF[DOF01];	// 5  / 10007  = 0.00049966
	//Cal_DPP_DOF[DOF02] = RANGE_DOF02 / (float)PR_DOF[DOF02];	// 90 / 770048 = 0.00011688
	//Cal_DPP_DOF[DOF03] = RANGE_DOF03 / (float)PR_DOF[DOF03];	// 90 / 680443 = 0.00013227
	//Cal_DPP_DOF[DOF04] = RANGE_DOF04 / (float)PR_DOF[DOF04];	// 90 / 610370 = 0.00014746
	//Cal_DPP_DOF[DOF05] = RANGE_DOF05 / (float)PR_DOF[DOF05];	// 5  / 10035  = 0.00049826
	//Cal_DPP_DOF[DOF06] = RANGE_DOF06 / (float)PR_DOF[DOF06];	// 90 / 600307 = 0.00014993
	//Cal_DPP_DOF[DOF07] = RANGE_DOF07 / (float)PR_DOF[DOF07];	// 20 / 190010 = 0.00010526

	//// Pulse_Range / Degree = 1도당 이동 Pulse
	//Cal_PPD_DOF[DOF01] = (float)PR_DOF[DOF01] / RANGE_DOF01;	// 10007  / 5  = 2001.4
	//Cal_PPD_DOF[DOF02] = (float)PR_DOF[DOF02] / RANGE_DOF02;	// 770048 / 90 = 8556.0889
	//Cal_PPD_DOF[DOF03] = (float)PR_DOF[DOF03] / RANGE_DOF03;	// 680443 / 90 = 7560.4778
	//Cal_PPD_DOF[DOF04] = (float)PR_DOF[DOF04] / RANGE_DOF04;	// 610370 / 90 = 6781.8889
	//Cal_PPD_DOF[DOF05] = (float)PR_DOF[DOF05] / RANGE_DOF05;	// 10035  / 5  = 2007
	//Cal_PPD_DOF[DOF06] = (float)PR_DOF[DOF06] / RANGE_DOF06;	// 600307 / 90 = 6670.0778
	//Cal_PPD_DOF[DOF07] = (float)PR_DOF[DOF07] / RANGE_DOF07;	// 190010 / 20 = 9500.5

	//std::cout << "========== Init Position ==========" << endl;
	//std::cout << Start_Pos[1] << " // " << Start_Pos[2] << " // " << Start_Pos[3] << " // " << Start_Pos[4] << " // " << Start_Pos[5]  << endl;
	//std::cout << Start_Pos[6] << " // " << Start_Pos[7] << " // " << Start_Pos[8] << " // " << Start_Pos[9] << " // " << Start_Pos[10] << endl;
	//std::cout << Start_Pos[11]<< " // " << Start_Pos[12]<< " // " << Start_Pos[13]<< " // " << Start_Pos[14]<< " // " << Start_Pos[15] << endl;
	//std::cout << "===================================" << endl << endl;
}


/************************************************************************
	Master System의 EndPoint Fq1과 Fq1_r 값을 Forward Kinematics로 구한다.
************************************************************************/
void Kinematics::Find_Fq1()
{
	Fq1_X = D6 * sin(MR_Th[BASE]) * sin(MR_Th[ROLL]) * sin(MR_Th[PITCH]) + cos(MR_Th[BASE]) * (cos(MR_Th[SHOULDER]) * 
		(L2 + (D4 + D6 * cos(MR_Th[PITCH])) * sin(MR_Th[ELBOW]) + D6 * cos(MR_Th[ELBOW]) * cos(MR_Th[ROLL]) * sin(MR_Th[PITCH])) + 
		sin(MR_Th[SHOULDER]) * (cos(MR_Th[ELBOW]) *	(D4 + D6 * cos(MR_Th[PITCH])) - D6 * cos(MR_Th[ROLL]) * sin(MR_Th[ELBOW]) *	
		sin(MR_Th[PITCH])));

	Fq1_Y = cos(MR_Th[ELBOW]) * (D4 + D6 * cos(MR_Th[PITCH])) * sin(MR_Th[BASE]) * sin(MR_Th[SHOULDER]) - D6 * (cos(MR_Th[ROLL]) *
		sin(MR_Th[BASE]) * sin(MR_Th[SHOULDER]) * sin(MR_Th[ELBOW]) + cos(MR_Th[BASE]) * sin(MR_Th[ROLL])) * sin(MR_Th[PITCH]) +
		cos(MR_Th[SHOULDER]) * sin(MR_Th[BASE]) * (L2 + (D4 + D6 * cos(MR_Th[PITCH])) *	sin(MR_Th[ELBOW]) + D6 * cos(MR_Th[ELBOW]) * 
		cos(MR_Th[ROLL]) * sin(MR_Th[PITCH]));

	Fq1_Z = D1 + sin(MR_Th[SHOULDER]) * (L2 + (D4 +	D6 * cos(MR_Th[PITCH])) * sin(MR_Th[ELBOW]) + D6 * cos(MR_Th[ELBOW]) * 
		cos(MR_Th[ROLL]) * sin(MR_Th[PITCH])) - cos(MR_Th[SHOULDER]) * (cos(MR_Th[ELBOW]) * (D4 + D6 * cos(MR_Th[PITCH])) - 
		D6 * cos(MR_Th[ROLL]) * sin(MR_Th[ELBOW]) * sin(MR_Th[PITCH]));

	Fq1_r11 = sin(MR_Th[BASE]) * (cos(MR_Th[PITCH]) * cos(MR_Th[YAW]) *	sin(MR_Th[ROLL]) + cos(MR_Th[ROLL]) * sin(MR_Th[YAW])) + cos(MR_Th[BASE]) *
		(-cos(MR_Th[YAW]) * sin(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[PITCH]) + cos(MR_Th[SHOULDER] + MR_Th[ELBOW]) * (cos(MR_Th[ROLL]) *
		cos(MR_Th[PITCH]) *	cos(MR_Th[YAW]) - sin(MR_Th[ROLL]) * sin(MR_Th[YAW])));

	Fq1_r21 = cos(MR_Th[YAW]) * (cos(MR_Th[ROLL]) * sin(MR_Th[BASE]) - cos(MR_Th[BASE]) * cos(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[ROLL])) -
		(cos(MR_Th[PITCH]) * sin(MR_Th[BASE]) * sin(MR_Th[ROLL]) + cos(MR_Th[BASE]) * (cos(MR_Th[SHOULDER] + MR_Th[ELBOW]) * cos(MR_Th[ROLL]) *
		cos(MR_Th[PITCH]) - sin(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[PITCH]))) * sin(MR_Th[YAW]);

	Fq1_r31 = sin(MR_Th[BASE]) * sin(MR_Th[ROLL]) * sin(MR_Th[PITCH]) + cos(MR_Th[BASE]) * (cos(MR_Th[ELBOW]) * (cos(MR_Th[PITCH]) *
		sin(MR_Th[SHOULDER]) + cos(MR_Th[SHOULDER]) * cos(MR_Th[ROLL]) * sin(MR_Th[PITCH])) + sin(MR_Th[ELBOW]) * (cos(MR_Th[SHOULDER]) *
		cos(MR_Th[PITCH]) - cos(MR_Th[ROLL]) * sin(MR_Th[SHOULDER]) * sin(MR_Th[PITCH])));

	Fq1_r12 = cos(MR_Th[YAW]) * (cos(MR_Th[PITCH]) * (cos(MR_Th[SHOULDER]) * cos(MR_Th[ELBOW]) * cos(MR_Th[ROLL]) * sin(MR_Th[BASE]) -
		cos(MR_Th[ROLL]) * sin(MR_Th[BASE]) * sin(MR_Th[SHOULDER]) * sin(MR_Th[ELBOW]) - cos(MR_Th[BASE]) * sin(MR_Th[ROLL])) -
		sin(MR_Th[BASE]) * sin(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[PITCH])) - (cos(MR_Th[BASE]) * cos(MR_Th[ROLL]) + 
		cos(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[BASE]) * sin(MR_Th[ROLL])) * sin(MR_Th[YAW]);

	Fq1_r22 = cos(MR_Th[BASE]) * (-cos(MR_Th[ROLL]) * cos(MR_Th[YAW]) + cos(MR_Th[PITCH]) *	sin(MR_Th[ROLL]) * sin(MR_Th[YAW])) +
		sin(MR_Th[BASE]) * (sin(MR_Th[SHOULDER]) * (cos(MR_Th[YAW]) * sin(MR_Th[ELBOW]) * sin(MR_Th[ROLL]) + (cos(MR_Th[ROLL]) *
		cos(MR_Th[PITCH]) * sin(MR_Th[ELBOW]) + cos(MR_Th[ELBOW]) * sin(MR_Th[PITCH])) * sin(MR_Th[YAW])) + cos(MR_Th[SHOULDER]) * 
		(sin(MR_Th[ELBOW]) * sin(MR_Th[PITCH]) * sin(MR_Th[YAW]) - cos(MR_Th[ELBOW]) * (cos(MR_Th[YAW]) * sin(MR_Th[ROLL]) +
		cos(MR_Th[ROLL]) * cos(MR_Th[PITCH]) * sin(MR_Th[YAW]))));

	Fq1_r32 = cos(MR_Th[SHOULDER]) * cos(MR_Th[PITCH]) * sin(MR_Th[BASE]) * sin(MR_Th[ELBOW]) -	(cos(MR_Th[ROLL]) * sin(MR_Th[BASE]) *
		sin(MR_Th[SHOULDER]) * sin(MR_Th[ELBOW]) + cos(MR_Th[BASE]) * sin(MR_Th[ROLL])) * sin(MR_Th[PITCH]) + cos(MR_Th[ELBOW]) *
		sin(MR_Th[BASE]) * (cos(MR_Th[PITCH]) * sin(MR_Th[SHOULDER]) + cos(MR_Th[SHOULDER]) * cos(MR_Th[ROLL]) * sin(MR_Th[PITCH]));

	Fq1_r13 = cos(MR_Th[ROLL]) * cos(MR_Th[PITCH]) * cos(MR_Th[YAW]) * sin(MR_Th[SHOULDER] + MR_Th[ELBOW]) + cos(MR_Th[SHOULDER] +
		MR_Th[ELBOW]) * cos(MR_Th[YAW]) * sin(MR_Th[PITCH]) - sin(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[ROLL]) * sin(MR_Th[YAW]);

	Fq1_r23 = -cos(MR_Th[YAW]) * sin(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[ROLL]) - (cos(MR_Th[ROLL]) * cos(MR_Th[PITCH]) *
		sin(MR_Th[SHOULDER] + MR_Th[ELBOW]) + cos(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[PITCH])) * sin(MR_Th[YAW]);

	Fq1_r33 = sin(MR_Th[SHOULDER]) * (cos(MR_Th[PITCH]) * sin(MR_Th[ELBOW]) + cos(MR_Th[ELBOW]) * cos(MR_Th[ROLL]) * sin(MR_Th[PITCH])) +
		cos(MR_Th[SHOULDER]) * (-cos(MR_Th[ELBOW]) * cos(MR_Th[PITCH]) + cos(MR_Th[ROLL]) * sin(MR_Th[ELBOW]) * sin(MR_Th[PITCH]));
	
	//cout << "  [ Fq1 Information ] " << endl;
	//cout << "  Fq1_X :  " << Fq1_X << endl << "  Fq1_Y :  " << Fq1_Y << endl << "  Fq1_Z :  " << Fq1_Z << endl << endl;
	//cout << "  < Fq1 Rotation Matrix > " << endl;
	//cout << "  " << Fq1_r11 << ", " << Fq1_r21 << ", " << Fq1_r31 << endl;
	//cout << "  " << Fq1_r12 << ", " << Fq1_r22 << ", " << Fq1_r32 << endl;
	//cout << "  " << Fq1_r13 << ", " << Fq1_r23 << ", " << Fq1_r33 << endl << endl;
}

/************************************************************************
	Master System의 EndPoint Fq2와 Fq2_r 값을 Forward Kinematics로 구한다.
************************************************************************/
void Kinematics::Find_Fq2()
{
	Fq2_X = D6 * sin(MR_Th[BASE]) * sin(MR_Th[ROLL]) * sin(MR_Th[PITCH]) + cos(MR_Th[BASE]) * (cos(MR_Th[SHOULDER]) *
		(L2 + (D4 + D6 * cos(MR_Th[PITCH])) * sin(MR_Th[ELBOW]) + D6 * cos(MR_Th[ELBOW]) * cos(MR_Th[ROLL]) * sin(MR_Th[PITCH])) +
		sin(MR_Th[SHOULDER]) * (cos(MR_Th[ELBOW]) * (D4 + D6 * cos(MR_Th[PITCH])) - D6 * cos(MR_Th[ROLL]) * sin(MR_Th[ELBOW]) *
			sin(MR_Th[PITCH])));

	Fq2_Y = cos(MR_Th[ELBOW]) * (D4 + D6 * cos(MR_Th[PITCH])) * sin(MR_Th[BASE]) * sin(MR_Th[SHOULDER]) - 
			D6 * (	cos(MR_Th[ROLL]) * sin(MR_Th[BASE]) * sin(MR_Th[SHOULDER]) * sin(MR_Th[ELBOW]) + 
					cos(MR_Th[BASE]) * sin(MR_Th[ROLL])) * sin(MR_Th[PITCH]) +
					cos(MR_Th[SHOULDER]) * sin(MR_Th[BASE]) * (L2 + (D4 + D6 * cos(MR_Th[PITCH])) * sin(MR_Th[ELBOW]) + 
					D6 * cos(MR_Th[ELBOW]) * cos(MR_Th[ROLL]) * sin(MR_Th[PITCH]));

	Fq2_Z = D1 + sin(MR_Th[SHOULDER]) * (L2 + (D4 + D6 * cos(MR_Th[PITCH])) * sin(MR_Th[ELBOW]) + D6 * cos(MR_Th[ELBOW]) *
		cos(MR_Th[ROLL]) * sin(MR_Th[PITCH])) - cos(MR_Th[SHOULDER]) * (cos(MR_Th[ELBOW]) * (D4 + D6 * cos(MR_Th[PITCH])) -
			D6 * cos(MR_Th[ROLL]) * sin(MR_Th[ELBOW]) * sin(MR_Th[PITCH]));

	Fq2_r11 = sin(MR_Th[BASE]) * (cos(MR_Th[PITCH]) * cos(MR_Th[YAW]) * sin(MR_Th[ROLL]) + cos(MR_Th[ROLL]) * sin(MR_Th[YAW])) + cos(MR_Th[BASE]) *
		(-cos(MR_Th[YAW]) * sin(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[PITCH]) + cos(MR_Th[SHOULDER] + MR_Th[ELBOW]) * (cos(MR_Th[ROLL]) *
			cos(MR_Th[PITCH]) * cos(MR_Th[YAW]) - sin(MR_Th[ROLL]) * sin(MR_Th[YAW])));

	Fq2_r21 = cos(MR_Th[YAW]) * (cos(MR_Th[ROLL]) * sin(MR_Th[BASE]) - cos(MR_Th[BASE]) * cos(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[ROLL])) -
		(cos(MR_Th[PITCH]) * sin(MR_Th[BASE]) * sin(MR_Th[ROLL]) + cos(MR_Th[BASE]) * (cos(MR_Th[SHOULDER] + MR_Th[ELBOW]) * cos(MR_Th[ROLL]) *
			cos(MR_Th[PITCH]) - sin(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[PITCH]))) * sin(MR_Th[YAW]);

	Fq2_r31 = sin(MR_Th[BASE]) * sin(MR_Th[ROLL]) * sin(MR_Th[PITCH]) + cos(MR_Th[BASE]) * (cos(MR_Th[ELBOW]) * (cos(MR_Th[PITCH]) *
		sin(MR_Th[SHOULDER]) + cos(MR_Th[SHOULDER]) * cos(MR_Th[ROLL]) * sin(MR_Th[PITCH])) + sin(MR_Th[ELBOW]) * (cos(MR_Th[SHOULDER]) *
			cos(MR_Th[PITCH]) - cos(MR_Th[ROLL]) * sin(MR_Th[SHOULDER]) * sin(MR_Th[PITCH])));

	Fq2_r12 = cos(MR_Th[YAW]) * (cos(MR_Th[PITCH]) * (cos(MR_Th[SHOULDER]) * cos(MR_Th[ELBOW]) * cos(MR_Th[ROLL]) * sin(MR_Th[BASE]) -
		cos(MR_Th[ROLL]) * sin(MR_Th[BASE]) * sin(MR_Th[SHOULDER]) * sin(MR_Th[ELBOW]) - cos(MR_Th[BASE]) * sin(MR_Th[ROLL])) -
		sin(MR_Th[BASE]) * sin(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[PITCH])) - (cos(MR_Th[BASE]) * cos(MR_Th[ROLL]) +
			cos(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[BASE]) * sin(MR_Th[ROLL])) * sin(MR_Th[YAW]);

	Fq2_r22 = cos(MR_Th[BASE]) * (-cos(MR_Th[ROLL]) * cos(MR_Th[YAW]) + cos(MR_Th[PITCH]) * sin(MR_Th[ROLL]) * sin(MR_Th[YAW])) +
		sin(MR_Th[BASE]) * (sin(MR_Th[SHOULDER]) * (cos(MR_Th[YAW]) * sin(MR_Th[ELBOW]) * sin(MR_Th[ROLL]) + (cos(MR_Th[ROLL]) *
			cos(MR_Th[PITCH]) * sin(MR_Th[ELBOW]) + cos(MR_Th[ELBOW]) * sin(MR_Th[PITCH])) * sin(MR_Th[YAW])) + cos(MR_Th[SHOULDER]) *
			(sin(MR_Th[ELBOW]) * sin(MR_Th[PITCH]) * sin(MR_Th[YAW]) - cos(MR_Th[ELBOW]) * (cos(MR_Th[YAW]) * sin(MR_Th[ROLL]) +
				cos(MR_Th[ROLL]) * cos(MR_Th[PITCH]) * sin(MR_Th[YAW]))));

	Fq2_r32 = cos(MR_Th[SHOULDER]) * cos(MR_Th[PITCH]) * sin(MR_Th[BASE]) * sin(MR_Th[ELBOW]) - (cos(MR_Th[ROLL]) * sin(MR_Th[BASE]) *
		sin(MR_Th[SHOULDER]) * sin(MR_Th[ELBOW]) + cos(MR_Th[BASE]) * sin(MR_Th[ROLL])) * sin(MR_Th[PITCH]) + cos(MR_Th[ELBOW]) *
		sin(MR_Th[BASE]) * (cos(MR_Th[PITCH]) * sin(MR_Th[SHOULDER]) + cos(MR_Th[SHOULDER]) * cos(MR_Th[ROLL]) * sin(MR_Th[PITCH]));

	Fq2_r13 = cos(MR_Th[ROLL]) * cos(MR_Th[PITCH]) * cos(MR_Th[YAW]) * sin(MR_Th[SHOULDER] + MR_Th[ELBOW]) + cos(MR_Th[SHOULDER] +
		MR_Th[ELBOW]) * cos(MR_Th[YAW]) * sin(MR_Th[PITCH]) - sin(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[ROLL]) * sin(MR_Th[YAW]);

	Fq2_r23 = -cos(MR_Th[YAW]) * sin(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[ROLL]) - (cos(MR_Th[ROLL]) * cos(MR_Th[PITCH]) *
		sin(MR_Th[SHOULDER] + MR_Th[ELBOW]) + cos(MR_Th[SHOULDER] + MR_Th[ELBOW]) * sin(MR_Th[PITCH])) * sin(MR_Th[YAW]);

	Fq2_r33 = sin(MR_Th[SHOULDER]) * (cos(MR_Th[PITCH]) * sin(MR_Th[ELBOW]) + cos(MR_Th[ELBOW]) * cos(MR_Th[ROLL]) * sin(MR_Th[PITCH])) +
		cos(MR_Th[SHOULDER]) * (-cos(MR_Th[ELBOW]) * cos(MR_Th[PITCH]) + cos(MR_Th[ROLL]) * sin(MR_Th[ELBOW]) * sin(MR_Th[PITCH]));
		
	//cout << "  Fq2_X :  " << Fq2_X << endl << "  Fq2_Y :  " << Fq2_Y << endl << "  Fq2_Z :  " << Fq2_Z << endl << endl;
	//cout << "  [ Fq2 Rotation Matrix ] " << endl;
	//cout << "  " << Fq2_r11 << ", " << Fq2_r21 << ", " << Fq2_r31 << endl;
	//cout << "  " << Fq2_r12 << ", " << Fq2_r22 << ", " << Fq2_r32 << endl;
	//cout << "  " << Fq2_r13 << ", " << Fq2_r23 << ", " << Fq2_r33 << endl << endl;
}

/************************************************************************
	로봇 Base의 원점좌표 FB, FB_r을 구한다.
	Fq1, Fq1_r 그리고 Robot의 각 축 Inverse행렬을 이용하여 상대변환 적용.
************************************************************************/
void Kinematics::Find_FB()
{
	FB_X =	Fq1_X + Fq1_r31 * (RL3 * cos(RR_Th[ROLL]) + RL2 * cos(RR_Th[ELBOW] + RR_Th[ROLL]) + 
					RD1 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL])) * sin(RR_Th[PITCH]) + 
			Fq1_r21 * (cos(RR_Th[YAW]) * (RD5 - RD1 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL]) + RL3 * sin(RR_Th[ROLL]) + RL2 * sin(RR_Th[ELBOW] + RR_Th[ROLL])) +
					cos(RR_Th[PITCH]) * (RL3 * cos(RR_Th[ROLL]) + RL2 * cos(RR_Th[ELBOW] + RR_Th[ROLL]) + 
					RD1 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL])) * sin(RR_Th[YAW])) +
			Fq1_r11 * (-RL6 - RL3 * cos(RR_Th[ROLL]) * cos(RR_Th[PITCH]) * cos(RR_Th[YAW]) + 
					RD5 * sin(RR_Th[YAW]) + RL3 * sin(RR_Th[ROLL]) * sin(RR_Th[YAW]) + 
					RL2 * (sin(RR_Th[ELBOW]) * (cos(RR_Th[PITCH]) * cos(RR_Th[YAW]) * sin(RR_Th[ROLL]) + 
					cos(RR_Th[ROLL]) * sin(RR_Th[YAW])) + cos(RR_Th[ELBOW]) * (-cos(RR_Th[ROLL]) * 
					cos(RR_Th[PITCH]) * cos(RR_Th[YAW]) + sin(RR_Th[ROLL]) * sin(RR_Th[YAW]))) - 
					(0.25) * RD1 * ((-2) * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[YAW]) * 2 + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])));
	
	FB_Y =	Fq1_Y + Fq1_r32 * (RL3 * cos(RR_Th[ROLL]) + RL2 * cos(RR_Th[ELBOW] + RR_Th[ROLL]) + 
					RD1 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL])) * sin(RR_Th[PITCH]) + 		
			Fq1_r22 * (cos(RR_Th[YAW]) * (RD5 - RD1 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL]) + RL3 * sin(RR_Th[ROLL]) + RL2 * sin(RR_Th[ELBOW] + RR_Th[ROLL])) +
					cos(RR_Th[PITCH]) * (RL3 * cos(RR_Th[ROLL]) + RL2 * cos(RR_Th[ELBOW] + RR_Th[ROLL]) + 
					RD1 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL])) * sin(RR_Th[YAW])) + 		
			Fq1_r12 * (-RL6 - RL3 * cos(RR_Th[ROLL]) * cos(RR_Th[PITCH]) * cos(RR_Th[YAW]) + 
					RD5 * sin(RR_Th[YAW]) + RL3 * sin(RR_Th[ROLL]) * sin(RR_Th[YAW]) + 
					RL2 * (sin(RR_Th[ELBOW]) * (cos(RR_Th[PITCH]) * cos(RR_Th[YAW]) * sin(RR_Th[ROLL]) + 
					cos(RR_Th[ROLL]) * sin(RR_Th[YAW])) + cos(RR_Th[ELBOW]) * (-cos(RR_Th[ROLL]) * 
					cos(RR_Th[PITCH]) * cos(RR_Th[YAW]) + sin(RR_Th[ROLL]) * sin(RR_Th[YAW]))) - 
					(0.25) * RD1 * (-2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 
					2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[YAW]) + sin(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + sin(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])));

	FB_Z =	Fq1_Z + Fq1_r33 * (RL3 * cos(RR_Th[ROLL]) + RL2 * cos(RR_Th[ELBOW] + RR_Th[ROLL]) + 
					RD1 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL])) * 
			sin(RR_Th[PITCH]) + Fq1_r23 * (cos(RR_Th[YAW]) * (RD5 - RD1 * cos(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL]) + RL3 * sin(RR_Th[ROLL]) + RL2 * sin(RR_Th[ELBOW] + RR_Th[ROLL])) +	
					cos(RR_Th[PITCH]) * (RL3 * cos(RR_Th[ROLL]) + RL2 * cos(RR_Th[ELBOW] + RR_Th[ROLL]) +
					RD1 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL])) * sin(RR_Th[YAW])) + 
			Fq1_r13 * (-RL6 - RL3 * cos(RR_Th[ROLL]) * cos(RR_Th[PITCH]) * cos(RR_Th[YAW]) + 
					RD5 * sin(RR_Th[YAW]) + RL3 * sin(RR_Th[ROLL]) * sin(RR_Th[YAW]) + 
					RL2 * (sin(RR_Th[ELBOW]) * (cos(RR_Th[PITCH]) * cos(RR_Th[YAW]) * sin(RR_Th[ROLL]) +
					cos(RR_Th[ROLL]) * sin(RR_Th[YAW])) + cos(RR_Th[ELBOW]) * (-cos(RR_Th[ROLL]) * 
					cos(RR_Th[PITCH]) * cos(RR_Th[YAW]) + sin(RR_Th[ROLL]) * sin(RR_Th[YAW]))) -
					(0.25) * RD1 * (-2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 
					2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[YAW]) + sin(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + sin(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])));

	FB_r11 = Fq1_r31 * 
					(cos(RR_Th[PITCH]) * sin(RR_Th[BASE]) - cos(RR_Th[BASE]) * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL]) * sin(RR_Th[PITCH])) + 
			 Fq1_r11 * ((0.25) * cos(RR_Th[BASE]) * 
					(-2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 
					2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])) + 				
					cos(RR_Th[YAW]) * sin(RR_Th[BASE]) * sin(RR_Th[PITCH])) - 
			 Fq1_r21 * (0.25) * 
					(4 * sin(RR_Th[BASE]) * sin(RR_Th[PITCH]) * sin(RR_Th[YAW]) + cos(RR_Th[BASE]) * 
					(2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) - 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) - 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 
					2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW]) ));
	
	FB_r12 = Fq1_r32 * (cos(RR_Th[PITCH]) * sin(RR_Th[BASE]) - cos(RR_Th[BASE]) * cos(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL]) * sin(RR_Th[PITCH])) + 
			 Fq1_r12 * ((0.25) * cos(RR_Th[BASE]) * 
					(-2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) +
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) +
					2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[YAW]) + cos(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + cos(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])) + 
					cos(RR_Th[YAW]) * sin(RR_Th[BASE]) * sin(RR_Th[PITCH])) - 
			 Fq1_r22 * (0.25)* 
					(4 * sin(RR_Th[BASE]) * sin(RR_Th[PITCH]) * sin(RR_Th[YAW]) + cos(RR_Th[BASE]) *
					(2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) - sin(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) - sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + 
					RR_Th[YAW]) + sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])));

	FB_r13 = Fq1_r33 * (cos(RR_Th[PITCH]) * sin(RR_Th[BASE]) - cos(RR_Th[BASE]) * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] +
					RR_Th[ROLL]) * sin(RR_Th[PITCH])) + 
			 Fq1_r13 * ((0.25) * cos(RR_Th[BASE]) * (-2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - 
					RR_Th[YAW]) + cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 
					2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[YAW]) + cos(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])) + cos(RR_Th[YAW]) * sin(RR_Th[BASE]) * sin(RR_Th[PITCH])) - 
			Fq1_r23 * (0.25) * (4 * sin(RR_Th[BASE]) * sin(RR_Th[PITCH]) * sin(RR_Th[YAW]) + cos(RR_Th[BASE]) *
					(2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) - sin(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) - sin(RR_Th[SHOULDER] + RR_Th[ELBOW] +
					RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + 
					RR_Th[YAW]) + sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])));

	FB_r21 = Fq1_r11 * ((0.25) * (-2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 
					2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[YAW]) + cos(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])) * sin(RR_Th[BASE]) - cos(RR_Th[BASE]) * 
					cos(RR_Th[YAW]) * sin(RR_Th[PITCH])) - 
			 Fq1_r31 * (cos(RR_Th[BASE]) * cos(RR_Th[PITCH]) + cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL]) *
					sin(RR_Th[BASE]) * sin(RR_Th[PITCH])) + 
			 Fq1_r21 * (cos(RR_Th[BASE]) * sin(RR_Th[PITCH]) * sin(RR_Th[YAW]) - (0.25) * sin(RR_Th[BASE]) * 
					(2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) - sin(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) - sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) +	2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[YAW]) + sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + 
					RR_Th[YAW]) + sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])));

	FB_r22 = Fq1_r12 * ((0.25) * (-2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 
					2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[YAW]) + cos(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])) * sin(RR_Th[BASE]) - cos(RR_Th[BASE]) * 
					cos(RR_Th[YAW]) * sin(RR_Th[PITCH])) - 
			 Fq1_r32 * (cos(RR_Th[BASE]) * cos(RR_Th[PITCH]) + cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL]) * sin(RR_Th[BASE]) * sin(RR_Th[PITCH])) + 
			 Fq1_r22 * (cos(RR_Th[BASE]) * sin(RR_Th[PITCH]) * sin(RR_Th[YAW]) - (0.25) * sin(RR_Th[BASE]) * 
					(2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) - sin(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) - sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + 
					RR_Th[YAW]) + sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])));

	FB_r23 = Fq1_r13 * ((0.25) * (-2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 
					2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[YAW]) + cos(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])) * sin(RR_Th[BASE]) - cos(RR_Th[BASE]) * 
					cos(RR_Th[YAW]) * sin(RR_Th[PITCH])) - Fq1_r33 * (cos(RR_Th[BASE]) * cos(RR_Th[PITCH]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL]) * sin(RR_Th[BASE]) * sin(RR_Th[PITCH])) + 
			 Fq1_r23 * (cos(RR_Th[BASE]) * sin(RR_Th[PITCH]) * sin(RR_Th[YAW]) - (0.25) * sin(RR_Th[BASE]) * 
					(2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) - sin(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) - sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + 
					RR_Th[YAW]) + sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])));

	FB_r31 = (0.25) * ((2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) - cos(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) - cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + 
					RR_Th[YAW]) + cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])) * 
			 Fq1_r21 - 4 * Fq1_r31 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL]) * sin(RR_Th[PITCH]) + 
					Fq1_r11 * (-2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) + sin(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) + sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + 
					RR_Th[YAW]) +	sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])));

	FB_r32 = (0.25) * ((2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) - cos(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) - cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + 
					RR_Th[YAW]) + cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])) * 
			 Fq1_r22 - 4 * Fq1_r32 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL]) * sin(RR_Th[PITCH]) + 
					Fq1_r12 * (-2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 
					2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[YAW]) + sin(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])));

	FB_r33 = (0.25) * ((2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) - cos(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) - cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 2 * cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + 
					RR_Th[YAW]) + cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + 
					cos(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])) * 
			 Fq1_r23 - 4 * Fq1_r33 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL]) * sin(RR_Th[PITCH]) + 
					Fq1_r13 * (-2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] - RR_Th[YAW]) + 
					sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[PITCH] - RR_Th[YAW]) + 
					2 * sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + RR_Th[ROLL] + RR_Th[YAW]) +	sin(RR_Th[SHOULDER] + 
					RR_Th[ELBOW] + RR_Th[ROLL] - RR_Th[PITCH] + RR_Th[YAW]) + sin(RR_Th[SHOULDER] + RR_Th[ELBOW] + 
					RR_Th[ROLL] + RR_Th[PITCH] + RR_Th[YAW])));

	//cout << "  [ FB Information ] " << endl;
	//cout << "  FB_X :  " << FB_X << endl << "  FB_Y :  " << FB_Y << endl << "  FB_Z :  " << FB_Z << endl << endl;
	//cout << "  < FB Rotation Matrix > " << endl;
	//cout << "  " << FB_r11 << ", " << FB_r21 << ", " << FB_r31 << endl;
	//cout << "  " << FB_r12 << ", " << FB_r22 << ", " << FB_r32 << endl;
	//cout << "  " << FB_r13 << ", " << FB_r23 << ", " << FB_r33 << endl << endl;
}

/********************************************************************************
Master System의 End Point 좌표계를 Robot의 End Point 좌표계와 일치되도록 변환한다.
********************************************************************************/
void Kinematics::Find_Fq1_To_Base()
{
	double Temp1, Temp2, Temp3, Temp4, Temp5, Temp6;

	// Master의 End Point 좌표계를 Rotate(Y, -90Degree).Rotate(X, 90Degree)로
	// 변환 시켜줘야 Robot좌표계와 일치 한다.
	//	FTq1 = | Fq1_r11  Fq1_r21  Fq1_r31  Fq1_X |		Master_Trans_Robot = |   0  -1   0   0  |          	   
	//	       | Fq1_r12  Fq1_r22  Fq1_r32  Fq1_Y | 						 |   0   0  -1   0  |     
	//		   | Fq1_r13  Fq1_r23  Fq1_r33  Fq1_Z | 						 |   1   0   0   0  |		
	//		   |    0        0        0       1   |	, 						 |   0   0   0   1  | 	
	//
	//	FTq1* M_T_R = | Fq1_r11	 Fq1_r21  Fq1_r31  Fq1_X |   |  0 - 1   0   0  |   | Fq1_r31  -Fq1_r11  -Fq1_r21  Fq1_X |
	//				  | Fq1_r12	 Fq1_r22  Fq1_r32  Fq1_Y | * |  0   0 - 1   0  | = | Fq1_r32  -Fq1_r12  -Fq1_r22  Fq1_Y |
	//				  | Fq1_r13	 Fq1_r23  Fq1_r33  Fq1_Z |   |  1   0   0   0  |   | Fq1_r33  -Fq1_r13  -Fq1_r23  Fq1_Z |
	//				  |    0         0        0      1   |	 |  0   0   0   1  |   |    0         0         0       1   |

	Temp1 = Fq1_r11;	Temp2 = Fq1_r12;	Temp3 = Fq1_r13;
	Temp4 = Fq1_r21;	Temp5 = Fq1_r22;	Temp6 = Fq1_r23;

	Fq1_r11 =  Fq1_r31;		Fq1_r12 =  Fq1_r32;		Fq1_r13 =  Fq1_r33;
	Fq1_r21 = -Temp1;		Fq1_r22 = -Temp2;		Fq1_r23 = -Temp3;
	Fq1_r31 = -Temp4;		Fq1_r32 = -Temp5;		Fq1_r33 = -Temp6;

	//cout << "  [ Fq1_To_Base Information ] " << endl;
	//cout << "  Fq1_X :  " << Fq1_X << endl << "  Fq1_Y :  " << Fq1_Y << endl << "  Fq1_Z :  " << Fq1_Z << endl << endl;
	//cout << "  < Fq1_To_Base Rotation Matrix > " << endl;
	//cout << "  " << Fq1_r11 << ", " << Fq1_r21 << ", " << Fq1_r31 << endl;
	//cout << "  " << Fq1_r12 << ", " << Fq1_r22 << ", " << Fq1_r32 << endl;
	//cout << "  " << Fq1_r13 << ", " << Fq1_r23 << ", " << Fq1_r33 << endl << endl;
}

/************************************************************************
Master System과 Robot의 좌표계를 맵핑 시켜 준다.
************************************************************************/
void Kinematics::Mapping()
{
	// Master System의 End Point인 Fq1과 Fq1_rotation 값을 Forward Kinematics로 구한다.
	Find_Fq1();

	// Master System의 End Point 좌표계를 Robot의 End Point 좌표계와 일치되도록 변환한다.
	Find_Fq1_To_Base();

	// 로봇 Base의 원점좌표인 FB, FB_rotation 값을 구한다.
	// (Fq1, Fq1_r 그리고 Robot의 각 축 Inverse행렬을 이용하여 상대변환 적용.)
	Find_FB();
}

/************************************************************************
Fq2와 Fq2_rotation 값에 대해 역변환을 통한 Robot Base에서 바라본 
Mq2와 Mq2_rotation 값을 구한다.
************************************************************************/
void Kinematics::Find_Mq2()
{
	//	Mq = Inverse(FTM) * Master_Trans_Robot * Fq
	//
	//	Inverse(FTM) = | FB_r11	 FB_r12  FB_r13  -(FB_X * FB_r11 + FB_Y * FB_r12 + FB_Z * FB_r13) |	 
	//				   | FB_r21	 FB_r22  FB_r23  -(FB_X * FB_r21 + FB_Y * FB_r22 + FB_Z * FB_r23) | 
	//				   | FB_r31	 FB_r32  FB_r33  -(FB_X * FB_r31 + FB_Y * FB_r32 + FB_Z * FB_r33) | 
	//				   |   0       0       0                              1                       |	,
	//	Mst_Trans_Rbt = |  0 - 1   0   0  |   Fq2 = | Fq2_r11 Fq2_r21 Fq2_r31 Fq2_X |
	//					|  0   0 - 1   0  |	        | Fq2_r12 Fq2_r22 Fq2_r32 Fq2_Y |
	//					|  1   0   0   0  |	  	    | Fq2_r13 Fq2_r23 Fq2_r33 Fq2_Z |
	//					|  0   0   0   1  | , 	    |    0       0       0      1   |
	//
	//	Mq = | FB_r11	 FB_r12  FB_r13  -(FB_X * FB_r11 + FB_Y * FB_r12 + FB_Z * FB_r13) |   | Fq2_r11 Fq2_r21 Fq2_r31 Fq2_X |   |  0 - 1   0   0  |
	//		 | FB_r21	 FB_r22  FB_r23  -(FB_X * FB_r21 + FB_Y * FB_r22 + FB_Z * FB_r23) | * | Fq2_r12 Fq2_r22 Fq2_r32 Fq2_Y | * |  0   0 - 1   0  |
	//		 | FB_r31	 FB_r32  FB_r33  -(FB_X * FB_r31 + FB_Y * FB_r32 + FB_Z * FB_r33) |   | Fq2_r13 Fq2_r23 Fq2_r33 Fq2_Z |   |  1   0   0   0  |
	//		 |   0         0       0                              1                       |   |    0       0       0      1   |   |  0   0   0   1  |
		
	Mq2_X = (-FB_r11) * FB_X - (FB_r12 * FB_Y) - (FB_r13 * FB_Z) + (FB_r11 * Fq2_X) + (FB_r12 * Fq2_Y) + (FB_r13 * Fq2_Z);
	Mq2_Y = (-FB_r21) * FB_X - (FB_r22 * FB_Y) - (FB_r23 * FB_Z) + (FB_r21 * Fq2_X) + (FB_r22 * Fq2_Y) + (FB_r23 * Fq2_Z);
	Mq2_Z = (-FB_r31) * FB_X - (FB_r32 * FB_Y) - (FB_r33 * FB_Z) + (FB_r31 * Fq2_X) + (FB_r32 * Fq2_Y) + (FB_r33 * Fq2_Z);

	Mq2_r11 = FB_r11 * Fq2_r31 + FB_r12 * Fq2_r32 + FB_r13 * Fq2_r33;
	Mq2_r12 = FB_r21 * Fq2_r31 + FB_r22 * Fq2_r32 + FB_r23 * Fq2_r33;
	Mq2_r13 = FB_r31 * Fq2_r31 + FB_r32 * Fq2_r32 + FB_r33 * Fq2_r33;

	Mq2_r21 = (-FB_r11) * Fq2_r11 - FB_r12 * Fq2_r12 - FB_r13 * Fq2_r13;
	Mq2_r22 = (-FB_r21) * Fq2_r11 - FB_r22 * Fq2_r12 - FB_r23 * Fq2_r13;
	Mq2_r23 = (-FB_r31) * Fq2_r11 - FB_r32 * Fq2_r12 - FB_r33 * Fq2_r13;

	Mq2_r31 = (-FB_r11) * Fq2_r21 - FB_r12 * Fq2_r22 - FB_r13 * Fq2_r23;
	Mq2_r32 = (-FB_r21) * Fq2_r21 - FB_r22 * Fq2_r22 - FB_r23 * Fq2_r23;
	Mq2_r33 = (-FB_r31) * Fq2_r21 - FB_r32 * Fq2_r22 - FB_r33 * Fq2_r23;

	//cout << "  [ Fq1 Information ] " << endl;
	//cout << "  Fq1_X :  " << Fq1_X << endl << "  Fq1_Y :  " << Fq1_Y << endl << "  Fq1_Z :  " << Fq1_Z << endl << endl;
	//cout << "  < Rotation Matrix > " << endl;
	//cout << "  " << Fq1_r11 << ", " << Fq1_r21 << ", " << Fq1_r31 << endl;
	//cout << "  " << Fq1_r12 << ", " << Fq1_r22 << ", " << Fq1_r32 << endl;
	//cout << "  " << Fq1_r13 << ", " << Fq1_r23 << ", " << Fq1_r33 << endl << endl;
	//
	//cout << "  [ FB Information ] " << endl;
	//cout << "  FB_X :  " << FB_X << endl << "  FB_Y :  " << FB_Y << endl << "  FB_Z :  " << FB_Z << endl << endl;
	//cout << "  < FB Rotation Matrix > " << endl;
	//cout << "  " << FB_r11 << ", " << FB_r21 << ", " << FB_r31 << endl;
	//cout << "  " << FB_r12 << ", " << FB_r22 << ", " << FB_r32 << endl;
	//cout << "  " << FB_r13 << ", " << FB_r23 << ", " << FB_r33 << endl << endl;

	//cout << "  [ Fq2 Information ] " << endl;
	//cout << "  Fq2_X :  " << Fq2_X << endl << "  Fq2_Y :  " << Fq2_Y << endl << "  Fq2_Z :  " << Fq2_Z << endl << endl;
	//cout << "  < Fq2 Rotation Matrix > " << endl;
	//cout << "  " << Fq2_r11 << ", " << Fq2_r21 << ", " << Fq2_r31 << endl;
	//cout << "  " << Fq2_r12 << ", " << Fq2_r22 << ", " << Fq2_r32 << endl;
	//cout << "  " << Fq2_r13 << ", " << Fq2_r23 << ", " << Fq2_r33 << endl << endl;

	cout << "  [ Mq2_X Information ] " << endl;
	cout << "  Mq2_X :  " << Mq2_X << endl << "  Mq2_Y :  " << Mq2_Y << endl << "  Mq2_Z :  " << Mq2_Z << endl << endl;
	cout << "  < Mq2_X Rotation Matrix > " << endl;
	cout << "  " << Mq2_r11 << ", " << Mq2_r21 << ", " << Mq2_r31 << endl;
	cout << "  " << Mq2_r12 << ", " << Mq2_r22 << ", " << Mq2_r32 << endl;
	cout << "  " << Mq2_r13 << ", " << Mq2_r23 << ", " << Mq2_r33 << endl << endl;

}

/************************************************************************
End Point값과 Rotation Matrix를 Robot Inverse Kinematics에 적용하여
각 축의 Theta값을 구한다.
************************************************************************/
void Kinematics::Inverse()
{	
	int check = 0;

	//A1 = atan2f(1,1);		//  45도
	//A1 = atan2f(-1,1);		// -45도
	//A1 = atan2f(1,-1);		//  135도
	//A1 = atan2f(-1, -1);	// -135도
	A1 = atan2f(Mq2_Y - RL6 * Mq2_r12, Mq2_X - RL6 * Mq2_r11);
	
	if (A1 < (-PI / 2))		A1 += PI;
	else if (A1 > (PI / 2))	A1 += -PI;
	
	Q = -cos(A1) * Mq2_r32 + sin(A1) * Mq2_r31;	// Q <= 1
	//Q = RoundD(Q);	// Q <= 1
	if (Q > 1 || Q < -1) Q = FloorD(Q);	// Q의 값이 1.000000001로 1보다 커지면 Error발생.
	//else if (Q == 0) Q = 0.0000001;

	//Mq2_X   = FloorD(Mq2_X);
	//Mq2_Y   = FloorD(Mq2_Y);
	//Mq2_Z   = FloorD(Mq2_Z);
	//Mq2_r11 = FloorD(Mq2_r11);
	//Mq2_r12 = FloorD(Mq2_r12);
	//Mq2_r13 = FloorD(Mq2_r13);
	//Mq2_r21 = FloorD(Mq2_r21);
	//Mq2_r22 = FloorD(Mq2_r22);
	//Mq2_r23 = FloorD(Mq2_r23);
	//Mq2_r31 = FloorD(Mq2_r31);
	//Mq2_r32 = FloorD(Mq2_r32);
	//Mq2_r33 = FloorD(Mq2_r33);

	A51 = atan2f(sqrt(1 - Q * Q), Q);
	if (A51 == 0)	A51 += 0.00000001;
	A61 = atan2f((cos(A1) * Mq2_r22 - sin(A1) * Mq2_r21) / sin(A51), (-cos(A1) * Mq2_r12 + sin(A1) * Mq2_r11) / sin(A51));
	
	A52 = atan2f(-sqrt(1 - Q * Q), Q);
	if (A52 == 0)	A52 -= 0.00000001;
	A62 = atan2f((cos(A1) * Mq2_r22 - sin(A1) * Mq2_r21) / sin(A52), (-cos(A1) * Mq2_r12 + sin(A1) * Mq2_r11) / sin(A52));

	//A51 = RoundD(A51);
	//A61 = RoundD(A61);
	//A52 = RoundD(A52);
	//A62 = RoundD(A62);

	X31 = RD5 * cos(A61) * Mq2_r21 + Mq2_X + Mq2_r11 * (-RL6 + RD5 * sin(A61));	
	Y31 = RD5 * cos(A61) * Mq2_r22 + Mq2_Y + Mq2_r12 * (-RL6 + RD5 * sin(A61));
	Z31 = RD5 * cos(A61) * Mq2_r23 + Mq2_Z + Mq2_r13 * (-RL6 + RD5 * sin(A61));
	X32 = RD5 * cos(A62) * Mq2_r21 + Mq2_X + Mq2_r11 * (-RL6 + RD5 * sin(A62));
	Y32 = RD5 * cos(A62) * Mq2_r22 + Mq2_Y + Mq2_r12 * (-RL6 + RD5 * sin(A62));	
	Z32 = RD5 * cos(A62) * Mq2_r23 + Mq2_Z + Mq2_r13 * (-RL6 + RD5 * sin(A62));

	//X31 = RoundD(X31);
	//Y31 = RoundD(Y31);
	//Z31 = RoundD(Z31);
	//X32 = RoundD(X32);
	//Y32 = RoundD(Y32);
	//Z32 = RoundD(Z32);

	M1 = (X31 * X31 + Y31 * Y31 + (Z31 - RD1) * (Z31 - RD1) - RL2 * RL2 - RL3 * RL3) / (2 * RL2 * RL3);
	M2 = (X32 * X32 + Y32 * Y32 + (Z32 - RD1) * (Z32 - RD1) - RL2 * RL2 - RL3 * RL3) / (2 * RL2 * RL3);

	M1 = RoundD(M1);
	M2 = RoundD(M2);
	
	if (M1 <= 1 && M1 >= -1)
	{
		M = M1;

		A1 = A1;
		A5 = A51;
		A6 = A61;

		X3 = X31;
		Y3 = Y31;
		Z3 = Z31;
	}
	else if (M2 <= 1 && M2 >= -1)
	{
		M = M2;

		A1 = A1;
		A5 = A52;
		A6 = A62;

		X3 = X32;
		Y3 = Y32;
		Z3 = Z32;
	}
	else
	{
		//cout << "  << Error Inverse Kinematics >>  " << endl;
		return;
	}

	A31 = atan2f(+sqrt(1 - M * M), M);
	A32 = atan2f(-sqrt(1 - M * M), M);

	Alp1 = atan2f(Z3 - RD1, +sqrt(X3 * X3 + Y3 * Y3));
	Alp2 = atan2f(Z3 - RD1, -sqrt(X3 * X3 + Y3 * Y3));
	Beta1 = atan2f(RL3 * sin(A31), RL2 + RL3 * cos(A31));
	Beta2 = atan2f(RL3 * sin(A32), RL2 + RL3 * cos(A32));

	A21 = Alp1 - Beta1;
	A22 = Alp2 - Beta1;
	A23 = Alp1 - Beta2;
	A24 = Alp2 - Beta2;

	A41 = atan2f(cos(A5) * cos(A6) * Mq2_r13 - sin(A5) * Mq2_r33 - cos(A5) * Mq2_r23 * sin(A6), cos(A6) * Mq2_r23 + sin(A6) * Mq2_r13) - A21 - A31;
	A42 = atan2f(cos(A5) * cos(A6) * Mq2_r13 - sin(A5) * Mq2_r33 - cos(A5) * Mq2_r23 * sin(A6), cos(A6) * Mq2_r23 + sin(A6) * Mq2_r13) - A22 - A31;
	A43 = atan2f(cos(A5) * cos(A6) * Mq2_r13 - sin(A5) * Mq2_r33 - cos(A5) * Mq2_r23 * sin(A6), cos(A6) * Mq2_r23 + sin(A6) * Mq2_r13) - A23 - A32;
	A44 = atan2f(cos(A5) * cos(A6) * Mq2_r13 - sin(A5) * Mq2_r33 - cos(A5) * Mq2_r23 * sin(A6), cos(A6) * Mq2_r23 + sin(A6) * Mq2_r13) - A24 - A32;

	//A31 - A21 - A41
	//A31 - A22 - A42
	//A32 - A23 - A43
	//A32 - A24 - A44
	//
	// A1 = -60 ~ 60  /  A2 = 180 ~ 90  /  A3 =   0 ~ -90    
	// A4 =   0 ~ 90  /  A5 = -60 ~ 60  /  A6 = -90 ~ -180 
	
	if (RoundD(A1) >= -PI / 3 && RoundD(A1) <= PI / 3)
	{
		if (RoundD(A5) >= -PI / 3 && RoundD(A1) <= PI / 3)
		{
			if (RoundD(A6) >= -PI && RoundD(A6) <= -PI / 2)
			{
				if (RoundD(A31) <= 0 && RoundD(A31) >= -PI / 2)
				{
					if (A21 <= PI && A21 >= PI / 2)
					{
						RF_X = RL6 * cos(A6) * sin(A1) * sin(A5) +
							cos(A1) * (cos(A21) * (RL2 + RL3 * cos(A31)) +
								RL6 * cos(A21 + A31 + A41) * cos(A5) * cos(A6) - RL3 * sin(A21) * sin(A31) +
								RD5 * sin(A21 + A31 + A41) - RL6 * sin(A21 + A31 + A41) * sin(A6));

						if (RoundD03(Mq2_X) == RoundD03(RF_X))
						{
							cout << "  <<< A24, A32 , A44 is OK>>> " << endl << endl;
							A2 = A21;	A3 = A31;	A4 = A41;					

							check = 1;
						}
					}
					else if (A22 <= PI && A22 >= PI / 2)
					{
						RF_X = RL6 * cos(A6) * sin(A1) * sin(A5) +
							cos(A1) * (cos(A22) * (RL2 + RL3 * cos(A31)) +
								RL6 * cos(A22 + A31 + A42) * cos(A5) * cos(A6) - RL3 * sin(A22) * sin(A31) +
								RD5 * sin(A22 + A31 + A42) - RL6 * sin(A22 + A31 + A42) * sin(A6));

						if (RoundD03(Mq2_X) == RoundD03(RF_X))
						{
							cout << "  <<< A24, A32 , A44 is OK>>> " << endl << endl;
							A2 = A22;	A3 = A31;	A4 = A42;
														
							check = 1;
						}
					}
				}

				if (RoundD(A32) <= 0 && RoundD(A32) >= -PI / 2)
				{
					if (RoundD(A23) <= PI && RoundD(A23) >= PI / 2)
					{
						RF_X = RL6 * cos(A6) * sin(A1) * sin(A5) +
							cos(A1) * (cos(A23) * (RL2 + RL3 * cos(A32)) +
								RL6 * cos(A23 + A32 + A43) * cos(A5) * cos(A6) - RL3 * sin(A23) * sin(A32) +
								RD5 * sin(A23 + A32 + A43) - RL6 * sin(A23 + A32 + A43) * sin(A6));

						if (RoundD03(Mq2_X) == RoundD03(RF_X))
						{
							cout << "  <<< A24, A32 , A44 is OK>>> " << endl << endl;
							A2 = A23;	A3 = A32;	A4 = A43;

							check = 1;
						}
					}
					else if (RoundD(A24) <= PI && RoundD(A24) >= PI / 2)
					{
						RF_X = RL6 * cos(A6) * sin(A1) * sin(A5) +
							cos(A1) * (cos(A24) * (RL2 + RL3 * cos(A32)) +
								RL6 * cos(A24 + A32 + A44) * cos(A5) * cos(A6) - RL3 * sin(A24) * sin(A32) +
								RD5 * sin(A24 + A32 + A44) - RL6 * sin(A24 + A32 + A44) * sin(A6));

						if (RoundD03(Mq2_X) == RoundD03(RF_X))
						{
							cout << "  <<< A24, A32 , A44 is OK>>> " << endl << endl;
							A2 = A24;	A3 = A32;	A4 = A44;

							check = 1;
						}
					}
				}
			}
		}
	}

	

	if (check == 1)
	{
		cout << "  [ Mq2_X Information ] " << endl;
		cout << "  Mq2_X :  " << Mq2_X << endl << "  Mq2_Y :  " << Mq2_Y << endl << "  Mq2_Z :  " << Mq2_Z << endl << endl;
		cout << "  < Mq2_X Rotation Matrix > " << endl;
		cout << "  " << Mq2_r11 << ", " << Mq2_r21 << ", " << Mq2_r31 << endl;
		cout << "  " << Mq2_r12 << ", " << Mq2_r22 << ", " << Mq2_r32 << endl;
		cout << "  " << Mq2_r13 << ", " << Mq2_r23 << ", " << Mq2_r33 << endl << endl;

		cout << "  A1  : " << RTD * A1  << endl;
		//cout << "  A21 : " << RTD * A21 << endl;
		//cout << "  A22 : " << RTD * A22 << endl;
		//cout << "  A23 : " << RTD * A23 << endl;
		//cout << "  A24 : " << RTD * A24 << endl;
		cout << "  A2  : " << RTD * A2  << endl;
		//cout << "  A31 : " << RTD * A31 << endl;
		//cout << "  A32 : " << RTD * A32 << endl;
		cout << "  A3  : " << RTD * A3  << endl;
		//cout << "  A41 : " << RTD * A41 << endl;
		//cout << "  A42 : " << RTD * A42 << endl;
		//cout << "  A43 : " << RTD * A43 << endl;
		//cout << "  A44 : " << RTD * A44 << endl;
		cout << "  A4  : " << RTD * A4  << endl;
		//cout << "  A51 : " << RTD * A51 << endl;
		//cout << "  A61 : " << RTD * A61 << endl;
		//cout << "  A52 : " << RTD * A52 << endl;
		//cout << "  A62 : " << RTD * A62 << endl;
		cout << "  A5  : " << RTD * A5  << endl;
		cout << "  A6  : " << RTD * A6  << endl;

		check = 0;
		cout << " [[[[[     Complete     ]]]]]] " << endl;
		return;
	}	
}

/************************************************************************
각 축의 Theta값으로 로봇을 구동 하기 전 커플링 Data값을 구한다.
************************************************************************/
void Kinematics::Cal_CouplingData(void)
{	
	Upper1 = -0.027627 * MD_Th[R_ROT1] / Robot_DOF_Range[R_ROT1];
	Low1   = -0.005533 * MD_Th[R_ROT1] / Robot_DOF_Range[R_ROT1];

	Upper2  = 11.3 - (sqrt(6.586 * 6.586 + 6.5   * 6.5   - 2 * 6.586 * 6.5   * cos(PI - (30.07 + 30.51 + MD_Th[R_ANG2]) * DTR)) + 1.35 * MD_Th[R_ANG2] * DTR + Toler[2] * MD_Th[R_ANG2] / Robot_DOF_Range[R_ANG2]);
	Middle2 = 11.3 - (sqrt(5.714 * 5.714 + 5.614 * 5.614 - 2 * 5.714 * 5.614 * cos(PI - (4.01  + 4.09  + MD_Th[R_ANG2]) * DTR)) + 1.35 * MD_Th[R_ANG2] * DTR + Toler[3] * MD_Th[R_ANG2] / Robot_DOF_Range[R_ANG2]);
	Low2    =  4.5 - (sqrt(3.701 * 3.701 + 3.64  * 3.64  - 2 * 3.701 * 3.64  * cos((38.42 + 37.18 + MD_Th[R_ANG2]) * DTR))      + 1.35 * MD_Th[R_ANG2] * DTR + Toler[4] * MD_Th[R_ANG2] / Robot_DOF_Range[R_ANG2]);

	Upper3  =  4.6 - (sqrt(3.701 * 3.701 + 3.701 * 3.701 - 2 * 3.701 * 3.701 * cos((38.42 + 38.42 + MD_Th[R_ANG3]) * DTR))      + 1.35 * MD_Th[R_ANG3] * DTR + Toler[5] * MD_Th[R_ANG3] / Robot_DOF_Range[R_ANG3]);
	Middle3 = 11.2 - (sqrt(5.614 * 5.614 + 5.614 * 5.614 - 2 * 5.614 * 5.614 * cos(PI - (4.09  + 4.09  + MD_Th[R_ANG3]) * DTR)) + 1.35 * MD_Th[R_ANG3] * DTR + Toler[6] * MD_Th[R_ANG3] / Robot_DOF_Range[R_ANG3]);
	Low3    = 11.2 - (sqrt(6.5   * 6.5   + 6.5   * 6.5   - 2 * 6.5   * 6.5   * cos(PI - (30.51 + 30.51 + MD_Th[R_ANG3]) * DTR)) + 1.35 * MD_Th[R_ANG3] * DTR + Toler[7] * MD_Th[R_ANG3] / Robot_DOF_Range[R_ANG3]);

	Upper4  = 4.4 - (sqrt(3.33  * 3.33  + 3.33  * 3.33  - 2 * 3.33  * 3.33  * cos((41.35 + 41.35 + MD_Th[R_ANG4]) * DTR))       + 1.3  * MD_Th[R_ANG4] * DTR + Toler[8] * MD_Th[R_ANG4] / Robot_DOF_Range[R_ANG4]);
	Middle4 = 7.4 - (sqrt(4.206 * 4.206 + 4.206 * 4.206 - 2 * 4.206 * 4.206 * cos((61.61 + 61.61 + MD_Th[R_ANG4]) * DTR))       + 1.35 * MD_Th[R_ANG4] * DTR + Toler[9] * MD_Th[R_ANG4] / Robot_DOF_Range[R_ANG4]);
	Low4    = 7.4 - (sqrt(3.846 * 3.846 + 3.846 * 3.846 - 2 * 3.846 * 3.846 * cos(PI - (15.84 + 15.84 + MD_Th[R_ANG4]) * DTR))  + 1.35 * MD_Th[R_ANG4] * DTR + Toler[10] * MD_Th[R_ANG4] / Robot_DOF_Range[R_ANG4]);

	Upper5 = -0.041 * MD_Th[R_ROT5] / Robot_DOF_Range[R_ROT5] - Toler[11] * MD_Th[R_ROT5] / Robot_DOF_Range[R_ROT5];
	Low5   = -0.031 * MD_Th[R_ROT5] / Robot_DOF_Range[R_ROT5] - Toler[12] * MD_Th[R_ROT5] / Robot_DOF_Range[R_ROT5];

	Upper6 = -2.2 * MD_Th[R_ANG6] * DTR - Toler[13] * MD_Th[R_ANG6] / Robot_DOF_Range[R_ANG6];
	
	Couple_K2 = Upper1;
	Couple_K3 = Upper1 + Low2;
	Couple_K4 = Low1   + Upper2 + Upper3;
	Couple_K5 = Upper1 + Upper2 + Upper3 + Upper4;
	Couple_K6 = Upper1 + Low2 + Low3 + Low4 + Low5;
	Couple_K7 = Low1   + Middle2 + Middle3 + Middle4 + Upper5 + Upper6;

	// +는 wire를 당겨 주는 방향, -는 Wire를 풀어 주는 방향을 의미한다.
	Couple_K2 *= ROBOT_PPM;
	Couple_K3 *= ROBOT_PPM;
	Couple_K4 *= ROBOT_PPM;
	Couple_K5 *= ROBOT_PPM;
	Couple_K6 *= ROBOT_PPM;
	Couple_K7 *= ROBOT_PPM;
}


/************************************************************************
n축의 이동 Pulse값을 구한다.
************************************************************************/
void Kinematics::Cal_Move_Pulse(int DOF, int DOF_n, int Wire_Direction)
{
	EPOS2S.GetPosition(DOF_n, &OutputPos[DOF_n]);
	Robot_PulsePerDegre[DOF_n] = abs(Start_Pos[DOF_n] - End_Pos[DOF_n]) / Robot_DOF_Range[DOF];
	   
	if (DOF != R_FCEP)
	{
		// Step 1. Extension / Flexion시 Input 각도 값에 따른 1단계 Move Pulse값 계산.
		if (Wire_Direction == FLEXION)
		{
			InputPos[DOF_n] = -MD_Th[DOF] * Robot_PulsePerDegre[DOF_n];
#if CAL_INPUT
			cout << "[[[   " << DOF + 1 << " - " << DOF_n << " Step 1. InputPos : " << InputPos[DOF_n];
#endif // CAL_INPUT
			
			// Limit 설정.
			if (EPOS2S.m_oInit_Position == TRUE)
			{
				if (InputPos[DOF_n] >= Start_Pos[DOF_n])	InputPos[DOF_n] = Start_Pos[DOF_n];
				else if (InputPos[DOF_n] <= End_Pos[DOF_n])	InputPos[DOF_n] = End_Pos[DOF_n];
			}
		}
		else if (Wire_Direction == EXTENSION)
		{
			InputPos[DOF_n] = MD_Th[DOF] * Robot_PulsePerDegre[DOF_n];
#if CAL_INPUT			
			cout << "[[[   " << DOF + 1 << " - " << DOF_n << " Step 1. InputPos : " << InputPos[DOF_n];
#endif

			// Limit 설정.
			if (EPOS2S.m_oInit_Position == TRUE)
			{
				if (InputPos[DOF_n] >= End_Pos[DOF_n])	InputPos[DOF_n] = End_Pos[DOF_n];
				else if (InputPos[DOF_n] <= Start_Pos[DOF_n])	InputPos[DOF_n] = Start_Pos[DOF_n];
			}
		}
		else
		{
			system("PAUSE");
			exit(0);
		}
			   
		// Step 2. 1단계 Move Pulse값에 Robot wire의 여유분을 더하여 2단계 Move Pulse값 계산.		
		// Extension Mode
		if (KIN.MD_Th[DOF] < KIN.MD_Pre_Th[DOF])
		{
#if CAL_INPUT			
			cout << " / Crt_Dir : EXTENSION or STOP => Step 2. InputPos : " << InputPos[DOF_n] << "  ";
#endif
			Move_State[DOF] = EXTENSION;
		}		
		// Flexion Mode
		else if (KIN.MD_Th[DOF] > KIN.MD_Pre_Th[DOF])
		{
			if (Wire_Direction == EXTENSION)	InputPos[DOF_n] += Margin[DOF];
			else								InputPos[DOF_n] -= Margin[DOF];
#if CAL_INPUT					
			cout << " / Crt_Dir : FLEXION => Step 2. InputPos + Margin(+-" << Margin[DOF] << ") : " << InputPos[DOF_n] << "  ";
#endif
			Move_State[DOF] = FLEXION;
		}
		//Mvoe Stop Mode
		else
		{
			if (Move_State[DOF] == EXTENSION)
			{
#if CAL_INPUT			
				cout << " / Crt_Dir : EXTENSION or STOP => Step 2. InputPos : " << InputPos[DOF_n] << "  ";
#endif
			}
			else if (Move_State[DOF] == FLEXION)
			{
				if (Wire_Direction == EXTENSION)	InputPos[DOF_n] += Margin[DOF];
				else								InputPos[DOF_n] -= Margin[DOF];
#if CAL_INPUT		
				cout << " / Crt_Dir : FLEXION => Step 2. InputPos + Margin(+-" << Margin[DOF] << ") : " << InputPos[DOF_n] << "  ";
#endif
			}
			else
				system("PAUSE");
		}
		
		// Step 3. 2단계 Move Pulse값에 Coupling Pulse를 합산하여 3단계 Move Pulse값 계산.
		if (DOF == R_ROT1)
		{
#if CAL_INPUT
			cout << endl ;
#endif
		}
		else if (DOF == R_ANG2)
		{
			InputPos[DOF_n] += -Couple_K2;
#if CAL_INPUT
			cout << " / K : " << Couple_K2 << " Pulse ( " << Couple_K2 * ROBOT_MPP << " mm ) => Step 3. InputPos + K : " << InputPos[DOF_n] << endl;
#endif
		}
		else if (DOF == R_ANG3)
		{
			InputPos[DOF_n] += -Couple_K3;
#if CAL_INPUT
			cout << " / K : " << Couple_K3 << " Pulse ( " << Couple_K3 * ROBOT_MPP << " mm ) => Step 3. InputPos + K : " << InputPos[DOF_n] << endl;
#endif
		}
		else if (DOF == R_ANG4)
		{
			InputPos[DOF_n] += -Couple_K4;
#if CAL_INPUT
			cout << " / K : " << Couple_K4 << " Pulse ( " << Couple_K4 * ROBOT_MPP << " mm ) => Step 3. InputPos + K : " << InputPos[DOF_n] << endl;
#endif
		}
		else if (DOF == R_ROT5)
		{
			InputPos[DOF_n] += -Couple_K5;
#if CAL_INPUT
			cout << " / K : " << Couple_K5 << " Pulse ( " << Couple_K5 * ROBOT_MPP << " mm ) => Step 3. InputPos + K : " << InputPos[DOF_n] << endl;
#endif
		}
		else if (DOF == R_ANG6)
		{
			InputPos[DOF_n] += -Couple_K6;
#if CAL_INPUT
			cout << " / K : " << Couple_K6 << " Pulse ( " << Couple_K6 * ROBOT_MPP << " mm ) => Step 3. InputPos + K : " << InputPos[DOF_n] << endl;
#endif
		}
	}
	else if (DOF == R_FCEP)
	{
		// 7축의 이동 값을(Pulse, Velocity) 구한다.
		if (COM.Data_Result[FORCEP_B] == ON)
		{
			// 포셉 Open.
			InputPos[DOF07_EXT] = End_Pos[DOF07_EXT] + Margin[DOF] - Couple_K7;
			InputPos[DOF07_FLD] = End_Pos[DOF07_FLD] - Margin[DOF] - Couple_K7;
#if CAL_INPUT
			if (DOF_n == DOF07_EXT)
			{			
				cout << "[[[   " << DOF + 1 << " - " << DOF_n << " Step 1. InputPos(EXT) : " << End_Pos[DOF_n];
				cout << " / Crt_Dir : FLEXION => Step 2. InputPos + Margin(" << Margin[DOF] << ") : " << (End_Pos[DOF_n] + Margin[DOF]);
				cout << " / K : " << Couple_K7 << " Pulse ( " << Couple_K7 * ROBOT_MPP << " mm ) => Step 3. [ Open ] InputPos + K : " << InputPos[DOF_n] << endl;
			}
			else if (DOF_n == DOF07_FLD)
			{
				cout << "[[[   " << DOF + 1 << " - " << DOF_n << " Step 1. InputPos(FLS) : " << End_Pos[DOF_n];
				cout << " / Crt_Dir : FLEXION => Step 2. InputPos - Margin(" << Margin[DOF] << ") : " << (End_Pos[DOF_n] - Margin[DOF]);
				cout << "  / K : " << Couple_K7 << " Pulse ( " << Couple_K7 * ROBOT_MPP << " mm ) => Step 3. [ Open ] InputPos + K : " << InputPos[DOF_n] << endl;
			}
#endif
		}
		else
		{
			// 포셉 Close.
			InputPos[DOF07_EXT] = Start_Pos[DOF07_EXT] - Couple_K7;
			InputPos[DOF07_FLD] = Start_Pos[DOF07_FLD] - Couple_K7;
#if CAL_INPUT
			if (DOF_n == DOF07_EXT)
			{
				cout << "[[[   " << DOF + 1 << " - " << DOF_n << "  Step 1. InputPos(EXT)  :  " << Start_Pos[DOF_n];
				cout << " / Crt_Dir : EXTENSION => Step 2. InputPos : " << Start_Pos[DOF_n];
				cout << " / K : " << Couple_K7 << " Pulse ( " << Couple_K7 * ROBOT_MPP << " mm ) => Step 3. [ Close ] InputPos + K : " << InputPos[DOF_n] << endl;
			}
			else if (DOF_n == DOF07_FLD)
			{
				cout << "[[[   " << DOF + 1 << " - " << DOF_n << "  Step 1. InputPos(EXT)  :  " << Start_Pos[DOF_n];
				cout << " / Crt_Dir : EXTENSION => Step 2. InputPos : " << Start_Pos[DOF_n];
				cout << " / K : " << Couple_K7 << " Pulse ( " << Couple_K7 * ROBOT_MPP << " mm ) => Step 3. [ Close ] InputPos + K : " << InputPos[DOF_n] << endl;
			}
#endif
		}
	}

	//EPOS2S.Cal_Motor_Vel(DOF_n, OutputPos[DOF_n], InputPos[DOF_n]);
}


/************************************************************************
n축의 이동 시간을 구한다.
************************************************************************/
double Kinematics::Cal_Move_Time(int M_Num, long Start_Pulse, long End_Pulse)
{
	double Time=0;
	
	Time = (double)(abs(End_Pulse - Start_Pulse) * 150.0 * 60.0 / 614400.0 / MAX_VELOCITY);

	return	Time;
}

/************************************************************************
Calculate Velocity(Pulse)
************************************************************************/
void Kinematics::Cal_Motor_Vel(WORD M_Num, long Start_Pulse, long End_Pulse)
{	
	DWORD A_Vel;

	// 이동 Pulse : (End_Pulse - Start_Pulse), 감속비 : 150, RPM의 분을 기준으로 초를 분으로 환산 : 60, 
	// Pusle 환산 : 1/614400, 이동시간 : tf(sec)  
	if (Max_Time[TIME_RESULT] != 0)	A_Vel = (double)(abs(End_Pulse - Start_Pulse) * 150.0 / 614400.0 * 60.0 / Max_Time[TIME_RESULT]);
	else							A_Vel = 0;

#if CAL_INPUT
	cout << "<<<<<<<<<<   " << M_Num << "  A_Vel :  " << A_Vel << endl << endl;
#endif

	if (A_Vel != 0)
	{
		//Write Profile Position Objects
		if (!VCS_SetPositionProfile(EPOS2S.m_KeyHandle, M_Num, A_Vel, EPOS2S.m_ulProfileAcceleration, EPOS2S.m_ulProfileDeceleration, &EPOS2S.m_ulErrorCode))
		{
			cout << "Failed Cal_Motor_Vel nodeID_DCX" << M_Num << " - Error Code : " << EPOS2S.m_ulErrorCode << endl;
			system("PAUSE");
			exit(0);
		}
	}
}
///************************************************************************
//n축의 이동 Pulse와 Velocity값을 구한다.
//************************************************************************/
//void Kinematics::Cal_Move_Pulse(int DOF, int DOF_n, int Direction)
//{
//	EPOS2S.GetPosition(DOF_n, &OutputPos[DOF_n]);
//
//	if (DOF != R_FCEP)
//	{
//		if (Direction == FLEXION)
//		{
//			InputPos[DOF_n] = -MD_Th[DOF] * ROBOT_PPD;
//			cout << "[[[   " << DOF + 1 << " - " << DOF_n << " InputPos  :  " << InputPos[DOF_n] << "  ";
//
//			// Limit 설정.
//			if (EPOS2S.m_oInit_Position == TRUE)
//			{
//				if (InputPos[DOF_n] >= Start_Pos[DOF_n])	InputPos[DOF_n] = Start_Pos[DOF_n];
//				else if (InputPos[DOF_n] <= End_Pos[DOF_n])	InputPos[DOF_n] = End_Pos[DOF_n];
//			}
//		}
//		else if (Direction == EXTENSION)
//		{
//			InputPos[DOF_n] = MD_Th[DOF] * ROBOT_PPD;
//			cout << "[[[   " << DOF + 1 << " - " << DOF_n << " InputPos  :  " << InputPos[DOF_n] << "  ";
//
//			// Limit 설정.
//			if (EPOS2S.m_oInit_Position == TRUE)
//			{
//				if (InputPos[DOF_n] >= End_Pos[DOF_n])	InputPos[DOF_n] = End_Pos[DOF_n];
//				else if (InputPos[DOF_n] <= Start_Pos[DOF_n])	InputPos[DOF_n] = Start_Pos[DOF_n];
//			}
//		}
//		else
//		{
//			system("PAUSE");
//			exit(0);
//		}
//
//		if (DOF == R_ANG2)
//		{
//			InputPos[DOF_n] += -Couple_K2;
//			cout << "  /   K  :  " << Couple_K2 << " Pulse (  " << Couple_K2 * ROBOT_MPP << " mm )  /  InputPos + K  :  " << InputPos[DOF_n] << "  ";
//		}
//		else if (DOF == R_ANG3)
//		{
//			InputPos[DOF_n] += -Couple_K3;
//			cout << "  /   K  :  " << Couple_K3 << " Pulse (  " << Couple_K3 * ROBOT_MPP << " mm )  /  InputPos + K  :  " << InputPos[DOF_n] << "  ";
//		}
//		else if (DOF == R_ANG4)
//		{
//			InputPos[DOF_n] += -Couple_K4;
//			cout << "  /   K  :  " << Couple_K4 << " Pulse (  " << Couple_K4 * ROBOT_MPP << " mm )  /  InputPos + K  :  " << InputPos[DOF_n] << "  ";
//		}
//		else if (DOF == R_ROT5)
//		{
//			InputPos[DOF_n] += -Couple_K5;
//			cout << "  /   K  :  " << Couple_K5 << " Pulse (  " << Couple_K5 * ROBOT_MPP << " mm )  /  InputPos + K  :  " << InputPos[DOF_n] << "  ";
//		}
//		else if (DOF == R_ANG6)
//		{
//			InputPos[DOF_n] += -Couple_K6;
//			cout << "  /   K  :  " << Couple_K6 << " Pulse (  " << Couple_K6 * ROBOT_MPP << " mm )  /  InputPos + K  :  " << InputPos[DOF_n] << "  ";
//		}
//
//
//	}
//	else if (DOF == R_FCEP)
//	{
//		// 7축의 이동 값을(Pulse, Velocity) 구한다.
//		if (COM.Data_Result[FORCEP_B] == ON)
//		{
//			// 포셉 Open.
//			InputPos[DOF07_EXT] = End_Pos[DOF07_EXT] - Couple_K7;
//			InputPos[DOF07_FLD] = End_Pos[DOF07_FLD] - Couple_K7;
//
//			if (DOF_n == DOF07_EXT)
//			{
//				cout << "[[[   " << DOF + 1 << " - " << DOF_n << "  InputPos(EXT)  :  " << End_Pos[DOF07_EXT];
//				cout << "  /   K  :  " << Couple_K7 << " Pulse (  " << Couple_K7 * ROBOT_MPP << " mm )  /  [ Open ]  InputPos + K(EXT)  :  " << InputPos[DOF07_EXT] << "  ";
//
//			}
//			else if (DOF_n == DOF07_FLD)
//			{
//				cout << "[[[   " << DOF + 1 << " - " << DOF_n << " InputPos(FLD)  :  " << End_Pos[DOF07_FLD];
//				cout << "  /   K  :  " << Couple_K7 << " Pulse (  " << Couple_K7 * ROBOT_MPP << " mm )  /  [ Open ]  InputPos + K(FLD)  :  " << InputPos[DOF07_FLD] << "  ";
//			}
//		}
//		else
//		{
//			// 포셉 Close.
//			InputPos[DOF07_EXT] = Start_Pos[DOF07_EXT] - Couple_K7;
//			InputPos[DOF07_FLD] = Start_Pos[DOF07_FLD] - Couple_K7;
//
//			if (DOF_n == DOF07_EXT)
//			{
//				cout << "[[[   " << DOF + 1 << " - " << DOF_n << "  InputPos(EXT)  :  " << Start_Pos[DOF07_EXT];
//				cout << "  /   K  :  " << Couple_K7 << " Pulse (  " << Couple_K7 * ROBOT_MPP << " mm )  /  [ Close ]  InputPos + K(EXT)  :  " << InputPos[DOF07_EXT] << "  ";
//
//			}
//			else if (DOF_n == DOF07_FLD)
//			{
//				cout << "[[[   " << DOF + 1 << " - " << DOF_n << " InputPos(FLD)  :  " << Start_Pos[DOF07_FLD];
//				cout << "  /   K  :  " << Couple_K7 << " Pulse (  " << Couple_K7 * ROBOT_MPP << " mm )  /  [ Close ]  InputPos + K(FLD)  :  " << InputPos[DOF07_FLD] << "  ";
//			}
//		}
//	}
//
//	EPOS2S.Cal_Motor_Vel(DOF_n, abs(OutputPos[DOF_n]), abs(InputPos[DOF_n]));
//}
