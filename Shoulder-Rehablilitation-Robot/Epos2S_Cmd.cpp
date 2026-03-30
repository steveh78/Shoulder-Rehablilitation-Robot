
#include "SinglePort.h"


Epos2S_Cmd::Epos2S_Cmd()
{
	m_oRadio = 1;
	m_oImmediately = 0;
	m_oInitialisation = 0;

	m_ulProfileVelocity		= 1000;
	m_ulProfileAcceleration	= 1000;
	m_ulProfileDeceleration	= 1000;	

	m_uCurrentMust = 10;

	nodeID_DCX01 = 1;
	nodeID_DCX02 = 2;
	nodeID_DCX03 = 3;
	nodeID_DCX04 = 4;
	nodeID_DCX05 = 5;
	nodeID_DCX06 = 6;
	nodeID_DCX07 = 7;
	nodeID_DCX08 = 8;
	nodeID_DCX09 = 9;
	nodeID_DCX10 = 10;
	nodeID_DCX11 = 11;
	nodeID_DCX12 = 12;
	nodeID_DCX13 = 13;
	nodeID_DCX14 = 14;
}
Epos2S_Cmd::~Epos2S_Cmd()
{

}

/************************************************************************
Init EPOS2S.
************************************************************************/
void Epos2S_Cmd::InitEPOS2S(void)
{
	OpenDevice();
 	SetPositionMode();

	// Init Position.
	//EPOS2S.GetPositionALL(KIN.Start_Pos);
	//EPOS2S.GetPositionALL(KIN.End_Pos);
	GetPositionALL(KIN.InputPos);
	GetPositionALL(KIN.OutputPos);

	//EPOS2S.m_oInit_Position = TRUE;
	m_oInit_Position = FALSE;
}

/************************************************************************
Opens communication interface.
************************************************************************/
BOOL Epos2S_Cmd::OpenDevice()
{
	HANDLE hNewKeyHandle;

	char DeviceName[] = "EPOS4";
	char ProtocolStackName[] = "MAXON SERIAL V2";
	char InterfaceName[] = "USB";
	char PortName[] = "USB0";

	//Close Previous Device
	if (m_KeyHandle)
	{
		if (m_KeyHandle) VCS_CloseDevice(m_KeyHandle, &m_ulErrorCode);
		m_KeyHandle = 0;
	}

	//hNewKeyHandle = VCS_OpenDeviceDlg(&m_ulErrorCode);
	m_KeyHandle = VCS_OpenDevice(DeviceName, ProtocolStackName, InterfaceName, PortName, &m_ulErrorCode);

	if (m_KeyHandle == 0)
	{
		m_oInitialisation = FALSE;
		cout << "Can't open device!" << endl;
		system("PAUSE");
		exit(0);
	}
}

/************************************************************************
Opens communication interface via the OpenDeviceDlg
************************************************************************/
BOOL Epos2S_Cmd::SetPositionMode()
{
	int Count = 0;

	if (m_KeyHandle)
	{		
		for (int i = 1; i < MOTOR_NUMBER + 1; i++)
		{
			if (!SetDriveToPosition(i))
			{
				Count = 0;
				cout << "Can't SetPositionMode nodeID_DCX" << i << endl;
			}
			else
			{
				Count++;
				cout << "Success SetPositionMode nodeID_DCX" << i << endl;
			}
		}

		if (Count == MOTOR_NUMBER) // Count가 4가 될경우 m_oInitialsation True 활성화
		{
			m_oInitialisation = TRUE;
			return TRUE;
		}
		else
		{
			m_oInitialisation = FALSE;
			return FALSE;
		}
	}

	return TRUE;
}
/************************************************************************
Opens communication interface via the OpenDeviceDlg
************************************************************************/
BOOL Epos2S_Cmd::SetDriveToPosition(int nodeID)
{
	//Clear Error History
	if (VCS_ClearFault(m_KeyHandle, nodeID, &m_ulErrorCode))
	{
		//Write Profile Position Mode
		if (VCS_SetOperationMode(m_KeyHandle, nodeID, OMD_PROFILE_POSITION_MODE, &m_ulErrorCode))
		{
			//Write Profile Position Objects
			if (VCS_SetPositionProfile(m_KeyHandle, nodeID, m_ulProfileVelocity, m_ulProfileAcceleration, m_ulProfileDeceleration, &m_ulErrorCode))
			{
				//Read Actual Position
				if (VCS_GetPositionIs(m_KeyHandle, nodeID, &m_lStartPosition, &m_ulErrorCode))
				{
					MotorEnable(nodeID);
					return TRUE;
				}
				else
				{
					cout << "[SetPositionMode] Failed GetPosition nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
					system("PAUSE");
					exit(0);
				}
			}
			else
			{
				cout << "[SetPositionMode] Failed SetPositionProfile nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
				system("PAUSE");
				exit(0);
			}
		}
		else
		{
			cout << "[SetPositionMode] Failed SetOperationMode nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
			system("PAUSE");
			exit(0);
		}
	}
	else
	{
		cout << "[SetPositionMode] Failed OpenGateway nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}

	return FALSE;
}

/************************************************************************
Opens communication interface via the OpenDeviceDlg
************************************************************************/
BOOL Epos2S_Cmd::SetCurrentMode()
{
	int Count = 0;

	if (m_KeyHandle)
	{
		for (int i = 1; i < MOTOR_NUMBER + 1; i++)
		{
			if (!SetDriveToCurrent(i))
			{
				Count = 0;
				cout << "Can't SetCurrentMode nodeID_DCX" << i << endl;
			}
			else
			{
				Count++;
				cout << "Success SetCurrentMode nodeID_DCX" << i << endl;
			}
		}

		if (Count == MOTOR_NUMBER)
		{
			m_oInitialisation = TRUE;
			return TRUE;
		}
		else
		{
			m_oInitialisation = FALSE;
			return FALSE;
		}
	}

	return TRUE;
}
/************************************************************************
Opens communication interface via the OpenDeviceDlg
************************************************************************/
BOOL Epos2S_Cmd::SetDriveToCurrent(int nodeID)
{
	//Clear Error History
	if (VCS_ClearFault(m_KeyHandle, nodeID, &m_ulErrorCode))
	{
		//Write Current Mode
		if (VCS_SetOperationMode(m_KeyHandle, nodeID, OMD_CURRENT_MODE, &m_ulErrorCode))
		{
			//Write Current Objects
			if(VCS_SetCurrentMust(m_KeyHandle, nodeID, m_uCurrentMust, &m_ulErrorCode))
			{
				//Read Actual Position
				if (VCS_GetCurrentIs(m_KeyHandle, nodeID, &m_uCurrentIs, &m_ulErrorCode))
				{
					MotorEnable(nodeID);
					return TRUE;
				}
				else
				{
					cout << "[SetCurrentMode] Failed VCS_GetCurrentIs nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
					system("PAUSE");
					exit(0);
				}
			}
			else
			{
				cout << "[SetCurrentMode] Failed VCS_SetCurrentMust nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
				system("PAUSE");
				exit(0);
			}
		}
		else
		{
			cout << "[SetCurrentMode] Failed SetOperationMode nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
			system("PAUSE");
			exit(0);
		}
	}
	else
	{
		cout << "[SetCurrentMode] Failed OpenGateway nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}

	return FALSE;
}

/************************************************************************
Restores the old settings, stops the timer and delete the key handle
************************************************************************/
BOOL Epos2S_Cmd::RestoreEPOS()
{
	//Restore Settings
	if (m_oInitialisation)
	{
		for (int i = 1; i <= MOTOR_NUMBER; i++)
			RestoreSetting(i);
	}

	//Close Device
	if (m_KeyHandle) VCS_CloseAllDevices(&m_ulErrorCode);

	return TRUE;
}

/************************************************************************
Restores the old settings, delete the key handle
************************************************************************/
void Epos2S_Cmd::RestoreSetting(int nodeID)
{
	//VCS_SetDisableState(m_KeyHandle, nodeID, &m_ulErrorCode);
	VCS_SetOperationMode(m_KeyHandle, nodeID, OMD_PROFILE_POSITION_MODE, &m_ulErrorCode);
	VCS_SetPositionProfile(m_KeyHandle, nodeID, m_ulProfileVelocity, m_ulProfileAcceleration, m_ulProfileDeceleration, &m_ulErrorCode);	
}

/************************************************************************
Sets device to enable state
************************************************************************/
void Epos2S_Cmd::MotorEnable(int nodeID)
{
	BOOL oFault = FALSE;

	if (!VCS_GetFaultState(m_KeyHandle, nodeID, &oFault, &m_ulErrorCode))
	{
		cout << "Failed GetFaultState nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
		return;
	}

	if (oFault)
	{
		if (!VCS_ClearFault(m_KeyHandle, nodeID, &m_ulErrorCode))
		{
			cout << "Failed ClearFault nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
			system("PAUSE");
			exit(0);
			return;
		}
	}

	if (!VCS_SetDisableState(m_KeyHandle, nodeID, &m_ulErrorCode))
	{
		cout << "Failed SetDisableState nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}

	if (!VCS_SetEnableState(m_KeyHandle, nodeID, &m_ulErrorCode))
	{
		cout << "Failed SetEnableState nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}

/************************************************************************
Sets device to disable state
************************************************************************/
void Epos2S_Cmd::MotorDisable(int nodeID)
{
	if (!VCS_SetDisableState(m_KeyHandle, nodeID, &m_ulErrorCode))
	{
		cout << "Failed SetDisableState nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}

/************************************************************************
Stops the movement
************************************************************************/
void Epos2S_Cmd::MotorStop(int nodeID)
{
	if (!VCS_HaltPositionMovement(m_KeyHandle, nodeID, &m_ulErrorCode))
	{
		cout << "Failed HaltPositionMovement nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}

/************************************************************************
Starts the movement(Pulse)
************************************************************************/
void Epos2S_Cmd::MotorMove(int nodeID, long Pulse_Position) //----------------------------------------------------------------------------------------------
{
	//Limit 설정
	//if (Pulse_Position > KIN.End_Pos[nodeID])
	//	Pulse_Position = KIN.End_Pos[nodeID];
	//else if (Pulse_Position < KIN.Start_Pos[nodeID])
	//	Pulse_Position = KIN.Start_Pos[nodeID];

	if (!VCS_MoveToPosition(m_KeyHandle, nodeID, Pulse_Position, m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}
/************************************************************************
Starts the movement Motor(Degree)
************************************************************************/
void Epos2S_Cmd::MoveDegree(int DOF, long Deg_Position)
{

	KIN.InputPos[DOF] = KIN.Start_Pos[DOF] + Deg_Position * ROBOT_PPD;

	//Limit 설정
	/*
	if (KIN.InputPos[DOF] > KIN.End_Pos[DOF])
		KIN.InputPos[DOF] = KIN.End_Pos[DOF];
	else if (KIN.InputPos[DOF] < KIN.Start_Pos[DOF])
		KIN.InputPos[DOF] = KIN.Start_Pos[DOF];
	*/

	if (!VCS_MoveToPosition(m_KeyHandle, DOF, KIN.InputPos[DOF], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}
/************************************************************************
Starts the movement DOF(PULSE)
************************************************************************/
void Epos2S_Cmd::MoveDOF_PULSE(int DOF, long PULSE_Position)
{
	int DOF1, DOF2;

	switch (DOF)
	{
	case 1:	DOF1 = DOF01_FLD;	DOF2 = DOF01_EXT;	break;
	case 2:	DOF1 = DOF02_FLD;	DOF2 = DOF02_EXT;	break;
	case 3:	DOF1 = DOF03_FLD;	DOF2 = DOF03_EXT;	break;
	case 4:	DOF1 = DOF04_FLD;	DOF2 = DOF04_EXT;	break;
	case 5:	DOF1 = DOF05_FLD;	DOF2 = DOF05_EXT;	break;
	case 6:	DOF1 = DOF06_FLD;	DOF2 = DOF06_EXT;	break;
	case 7:	DOF1 = DOF07_FLD;	DOF2 = DOF07_EXT;	break;

	default:
		break;
	}

	GetPositionDOF(DOF);

	KIN.InputPos[DOF1] = KIN.OutputPos[DOF1] - PULSE_Position;
	KIN.InputPos[DOF2] = KIN.OutputPos[DOF2] + PULSE_Position;

	// Limit 설정.
	//if (m_oInit_Position == TRUE)
	//{
	//	if ((KIN.InputPos[DOF1] >= KIN.Start_Pos[DOF1]) || (KIN.InputPos[DOF1] <= KIN.End_Pos[DOF1]))	KIN.InputPos[DOF1] = KIN.OutputPos[DOF1] + PULSE_Position;
	//	if ((KIN.InputPos[DOF2] <= KIN.Start_Pos[DOF2]) || (KIN.InputPos[DOF2] >= KIN.End_Pos[DOF2]))	KIN.InputPos[DOF2] = KIN.OutputPos[DOF2] - PULSE_Position;
	//}



	//if (PULSE_Position > 0)
	//{
	//	// Flexion
	//	KIN.Robot_PulsePerDegre[DOF1] = abs(KIN.Start_Pos[DOF1] - KIN.End_Pos[DOF1]) / KIN.Robot_DOF_Range[DOF - 1];
	//	KIN.Robot_PulsePerDegre[DOF2] = abs(KIN.Start_Pos[DOF2] - KIN.End_Pos[DOF2]) / KIN.Robot_DOF_Range[DOF - 1];
	//	//KIN.InputPos[DOF1] = KIN.OutputPos[DOF1] - KIN.Robot_PulsePerDegre[DOF1];
	//	//KIN.InputPos[DOF2] = KIN.OutputPos[DOF2] + KIN.Robot_PulsePerDegre[DOF2];

	//	KIN.InputPos[DOF1] = -1000 - 235000;
	//	Cal_Motor_Vel(DOF1, abs(KIN.OutputPos[DOF1]), abs(KIN.InputPos[DOF1]));

	//	KIN.InputPos[DOF2] =  1000 + 250000;
	//	Cal_Motor_Vel(DOF2, abs(KIN.OutputPos[DOF2]), abs(KIN.InputPos[DOF2]));
	//	
	//	
	//}
	//else
	//{
	//	// Extention
	//	KIN.Robot_PulsePerDegre[DOF1] = abs(KIN.Start_Pos[DOF1] - KIN.End_Pos[DOF1]) / KIN.Robot_DOF_Range[DOF - 1];
	//	KIN.Robot_PulsePerDegre[DOF2] = abs(KIN.Start_Pos[DOF2] - KIN.End_Pos[DOF2]) / KIN.Robot_DOF_Range[DOF - 1];
	//	//KIN.InputPos[DOF1] = KIN.OutputPos[DOF1] + KIN.Robot_PulsePerDegre[DOF1];
	//	//KIN.InputPos[DOF2] = KIN.OutputPos[DOF2] - KIN.Robot_PulsePerDegre[DOF2];

	//	KIN.InputPos[DOF1] =  0;
	//	Cal_Motor_Vel(DOF1, abs(KIN.OutputPos[DOF1]), abs(KIN.InputPos[DOF1]));

	//	KIN.InputPos[DOF2] = 0;
	//	Cal_Motor_Vel(DOF2, abs(KIN.OutputPos[DOF2]), abs(KIN.InputPos[DOF2]));
	//}
	//// Limit 설정.
	//m_oInit_Position = TRUE;
	//if (m_oInit_Position == TRUE)
	//{
	//	if (KIN.InputPos[DOF1] >= KIN.Start_Pos[DOF1])		KIN.InputPos[DOF1] = KIN.Start_Pos[DOF1];
	//	else if (KIN.InputPos[DOF1] <= KIN.End_Pos[DOF1])	KIN.InputPos[DOF1] = KIN.End_Pos[DOF1];


	//	if (KIN.InputPos[DOF2] <= KIN.Start_Pos[DOF2])	KIN.InputPos[DOF2] = KIN.Start_Pos[DOF2];
	//	else if (KIN.InputPos[DOF2] >= KIN.End_Pos[DOF2])	KIN.InputPos[DOF2] = KIN.End_Pos[DOF2];
	//}


	
	if (!VCS_MoveToPosition(m_KeyHandle, DOF1, KIN.InputPos[DOF1], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF1 << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
	if (!VCS_MoveToPosition(m_KeyHandle, DOF2, KIN.InputPos[DOF2], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF2 << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}

/************************************************************************
Starts the movement DOF(Degree)
************************************************************************/
void Epos2S_Cmd::MoveDOF(int Motor, long Deg_Position)
{
	if (Motor == 1)
	{
		KIN.InputPos[Motor] = KIN.Start_Pos[Motor] + Deg_Position * ROBOT_PPD_80;
	}
	else
	{
		KIN.InputPos[Motor] = KIN.Start_Pos[Motor] + Deg_Position * ROBOT_PPD;
	}
	//KIN.InputPos[Motor] = KIN.Start_Pos[Motor] + Deg_Position * ROBOT_PPD;

	/*
	if (KIN.InputPos[Motor] > KIN.End_Pos[Motor])
	{
		KIN.InputPos[Motor] = KIN.End_Pos[Motor];
	}
	else if (KIN.InputPos[Motor] < KIN.Start_Pos[Motor])
	{
		KIN.InputPos[Motor] = KIN.Start_Pos[Motor];
	}
	*/

	if (!VCS_MoveToPosition(m_KeyHandle, Motor, KIN.InputPos[Motor], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << Motor << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}

}
/************************************************************************
Starts the movement DOF01(Degree)
************************************************************************/
void Epos2S_Cmd::MoveDOF01(long Deg_Position)
{
	KIN.InputPos[DOF01_FLD] = KIN.Start_Pos[DOF01_FLD] - Deg_Position * ROBOT_PPD;
	KIN.InputPos[DOF01_EXT] = KIN.Start_Pos[DOF01_EXT] + Deg_Position * ROBOT_PPD;

	if (!VCS_MoveToPosition(m_KeyHandle, DOF01_FLD, KIN.InputPos[DOF01_FLD], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF01_FLD << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
	if (!VCS_MoveToPosition(m_KeyHandle, DOF01_EXT, KIN.InputPos[DOF01_EXT], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF01_EXT << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}
/************************************************************************
Starts the movement DOF02(Degree)
************************************************************************/
void Epos2S_Cmd::MoveDOF02(long Deg_Position)
{
	KIN.InputPos[DOF02_FLD] = KIN.Start_Pos[DOF02_FLD] - Deg_Position * ROBOT_PPD;
	KIN.InputPos[DOF02_EXT] = KIN.Start_Pos[DOF02_EXT] + Deg_Position * ROBOT_PPD;

	if (!VCS_MoveToPosition(m_KeyHandle, DOF02_FLD, KIN.InputPos[DOF02_FLD], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF02_FLD << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
	if (!VCS_MoveToPosition(m_KeyHandle, DOF02_EXT, KIN.InputPos[DOF02_EXT], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF02_EXT << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}
/************************************************************************
Starts the movement DOF03(Degree)
************************************************************************/
void Epos2S_Cmd::MoveDOF03(long Deg_Position)
{
	KIN.InputPos[DOF03_FLD] = KIN.Start_Pos[DOF03_FLD] - Deg_Position * ROBOT_PPD;
	KIN.InputPos[DOF03_EXT] = KIN.Start_Pos[DOF03_EXT] + Deg_Position * ROBOT_PPD;

	if (!VCS_MoveToPosition(m_KeyHandle, DOF03_FLD, KIN.InputPos[DOF03_FLD], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF03_FLD << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
	if (!VCS_MoveToPosition(m_KeyHandle, DOF03_EXT, KIN.InputPos[DOF03_EXT], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF03_EXT << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}
/************************************************************************
Starts the movement DOF04(Degree)
************************************************************************/
void Epos2S_Cmd::MoveDOF04(long Deg_Position)
{
	KIN.InputPos[DOF04_FLD] = KIN.Start_Pos[DOF04_FLD] - Deg_Position * ROBOT_PPD;
	KIN.InputPos[DOF04_EXT] = KIN.Start_Pos[DOF04_EXT] + Deg_Position * ROBOT_PPD;

	if (!VCS_MoveToPosition(m_KeyHandle, DOF04_FLD, KIN.InputPos[DOF04_FLD], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF04_FLD << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
	if (!VCS_MoveToPosition(m_KeyHandle, DOF04_EXT, KIN.InputPos[DOF04_EXT], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF04_EXT << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}
/************************************************************************
Starts the movement DOF05(Degree)
************************************************************************/
void Epos2S_Cmd::MoveDOF05(long Deg_Position)
{
	KIN.InputPos[DOF05_FLD] = KIN.Start_Pos[DOF05_FLD] - Deg_Position * ROBOT_PPD;
	KIN.InputPos[DOF05_EXT] = KIN.Start_Pos[DOF05_EXT] + Deg_Position * ROBOT_PPD;

	if (!VCS_MoveToPosition(m_KeyHandle, DOF05_FLD, KIN.InputPos[DOF05_FLD], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF05_FLD << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
	if (!VCS_MoveToPosition(m_KeyHandle, DOF05_EXT, KIN.InputPos[DOF05_EXT], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF05_EXT << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}
/************************************************************************
Starts the movement DOF06(Degree)
************************************************************************/
void Epos2S_Cmd::MoveDOF06(long Deg_Position)
{
	KIN.InputPos[DOF06_FLD] = KIN.Start_Pos[DOF06_FLD] - Deg_Position * ROBOT_PPD;
	KIN.InputPos[DOF06_EXT] = KIN.Start_Pos[DOF06_EXT] + Deg_Position * ROBOT_PPD;

	if (!VCS_MoveToPosition(m_KeyHandle, DOF06_FLD, KIN.InputPos[DOF06_FLD], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF06_FLD << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
	if (!VCS_MoveToPosition(m_KeyHandle, DOF06_EXT, KIN.InputPos[DOF06_EXT], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF06_EXT << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}
/************************************************************************
Starts the movement DOF07(Degree)
************************************************************************/
void Epos2S_Cmd::MoveDOF07(long Deg_Position)
{
	KIN.InputPos[DOF07_FLD] = KIN.Start_Pos[DOF07_FLD] - Deg_Position * ROBOT_PPD;
	KIN.InputPos[DOF07_EXT] = KIN.Start_Pos[DOF07_EXT] + Deg_Position * ROBOT_PPD;

	if (!VCS_MoveToPosition(m_KeyHandle, DOF07_FLD, KIN.InputPos[DOF07_FLD], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF07_FLD << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
	if (!VCS_MoveToPosition(m_KeyHandle, DOF07_EXT, KIN.InputPos[DOF07_EXT], m_oRadio, m_oImmediately, &m_ulErrorCode))
	{
		cout << "Failed MoveToPosition nodeID_DCX" << DOF07_EXT << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}

/************************************************************************
MOVE to Teaching Point(Degree)
************************************************************************/
void Epos2S_Cmd::MoveSTEP(int Step, float tf) //--------------------------------------------------------------------------------------------------------------------------
{	
	GetPositionDOF(1);
	Cal_vel(1, (KIN.OutputPos[1] * ROBOT_DPP_80), KIN.Step_Deg_Range[Step][1], tf); //1번 모터 기어비 80:1

	for (int i = 2; i <= MOTOR_NUMBER; i++)
	{
		GetPositionDOF(i);
		Cal_vel(i, (KIN.OutputPos[i] * ROBOT_DPP), KIN.Step_Deg_Range[Step][i], tf); // Cal_vel(현재위치, 목표 스텝의 모터 위치, 전체 이동 시간)
	}

	for (int i = 1; i <= MOTOR_NUMBER; i++)
		MoveDOF(i, KIN.Step_Deg_Range[Step][i]);

	while (1)
	{
		//GetPositionALL(KIN.OutputPos);
		for (int i = 1; i <= MOTOR_NUMBER; i++)
		{
			GetPositionDOF(i);
		}

		if (((KIN.OutputPos[DCX01] > (KIN.InputPos[DCX01] - ERROR_MARGIN)) && (KIN.OutputPos[DCX01] < (KIN.InputPos[DCX01] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX02] > (KIN.InputPos[DCX02] - ERROR_MARGIN)) && (KIN.OutputPos[DCX02] < (KIN.InputPos[DCX02] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX03] > (KIN.InputPos[DCX03] - ERROR_MARGIN)) && (KIN.OutputPos[DCX03] < (KIN.InputPos[DCX03] + ERROR_MARGIN))) &&
			((KIN.OutputPos[DCX04] > (KIN.InputPos[DCX04] - ERROR_MARGIN)) && (KIN.OutputPos[DCX04] < (KIN.InputPos[DCX04] + ERROR_MARGIN))))
			break;
		else
		{
			if (_kbhit())
			{
				for (int i = 1; i <= MOTOR_NUMBER; i++)
					MotorStop(i);
				break;
			}
			else
				continue;
		}
			
	}
	/*
	if (KIN.Start_End_Limit() == False)     // Start 와 End Position 구간 설정
	{
		Sleep(10000);
		exit(0);
	}*/

	cout << " Completed...." << endl << endl;
}

/************************************************************************
Calculate Velocity(Degree)
************************************************************************/
void Epos2S_Cmd::Cal_vel(int Motor, float Start_Angle, int End_Angle, float tf) 
{	
	//float tf;  // 이동 시간
	float ta;  // 가감속 시간
	DWORD A_Vel; // Unsiged long type 지정

	//tf = 0.2; // 5,000RPM, 100:1 , 360도 회전 시간은 1.2초, 180도는 0.6초

	//m_ulProfileAcceleration = 1667;
	//m_ulProfileDeceleration = 1667;

	/*
	switch (Motor)
	{
	case 1:
		m_ulProfileAcceleration = 833;
		m_ulProfileDeceleration = 833;
	case 2:
		m_ulProfileAcceleration = 833;
		m_ulProfileDeceleration = 833;
	case 3:
		m_ulProfileAcceleration = 1667;
		m_ulProfileDeceleration = 1667;
	case 4:
		m_ulProfileAcceleration = 833;
		m_ulProfileDeceleration = 833;
	}
	*/

	m_ulProfileAcceleration = 1500;
	m_ulProfileDeceleration = 1500;


	if ((End_Angle - Start_Angle) != 0)
	{
		if (Motor == 1)
		{
			A_Vel = abs((End_Angle)-(Start_Angle)) * 80 * 60 / 360 / tf;
		}
		else
		{
			A_Vel = abs((End_Angle)-(Start_Angle)) * 100 * 60 / 360 / tf;
		}
		// 이동 각도 : (End_Angle - Start_Angle), 감속비 : 100, RPM의 분을 초로 환산 : 60, 각도 환산 : 1/360, 이동시간 : tf(sec),  
		//A_Vel = abs(abs(End_Angle) - abs(Start_Angle)) * 100 * 60 / 360 / tf;
		//A_Vel = abs((End_Angle) - (Start_Angle)) * 100 * 60 / 360 / tf;
		
		if (A_Vel < 1)
			A_Vel = 1;
		else if (A_Vel >= 5000)
			A_Vel = 5000;
		
		/*
		if (A_Vel < 1)
			A_Vel = 1;
		else if (A_Vel >= 78 && A_Vel <= 88)
			A_Vel = 83;
		else if (A_Vel >= 161 && A_Vel <= 171)
			A_Vel = 166;
		*/
		printf("A_Vel = %u\n", A_Vel);

		// 속도 : A_Vel(RPM), 감속비 : 100, RPM의 분을 초로 환산 : 60, 각도 환산 : 1/360, 이동시간 : tf(sec)
		ta = A_Vel * tf * tf / 2 * 360 / 60 / 100;

		if (tf < 1.0)
		{
			m_ulProfileAcceleration = A_Vel / ta;
			m_ulProfileDeceleration = A_Vel / ta;
		}
		
		else
		{
			if (A_Vel < 100)
			{
				m_ulProfileAcceleration = 100;
				m_ulProfileDeceleration = 100;
			}
			else
			{
				m_ulProfileAcceleration = A_Vel;
				m_ulProfileDeceleration = A_Vel;
			}
		}
		
		//printf("Acceleration = %u\n", m_ulProfileAcceleration);

		//Write Profile Position Objects
		if (!VCS_SetPositionProfile(m_KeyHandle, Motor, A_Vel, m_ulProfileAcceleration, m_ulProfileDeceleration, &m_ulErrorCode))
		{
			cout << "Failed Cal_vel nodeID_DCX" << Motor << " - Error Code : " << m_ulErrorCode << endl;
			system("PAUSE");
			exit(0);
		}
	}
}

/************************************************************************
Get Position Value
************************************************************************/
void Epos2S_Cmd::GetPositionALL(long* pos)
{
	for(int i= 1; i<=MOTOR_NUMBER+1; i++)
		VCS_GetPositionIs(m_KeyHandle, i,  (pos + i),  &m_ulErrorCode); 
}

/************************************************************************
Get Position Value
************************************************************************/
void Epos2S_Cmd::GetPosition(int nodeID, long* pos)  //------------------------------------------------------------------------------------------------------
{
	if (!VCS_GetPositionIs(m_KeyHandle, nodeID, pos, &m_ulErrorCode))
	{
		cout << "Failed GetPositionIs nodeID_DCX" << nodeID << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}

/************************************************************************
Get Position Value DOF
************************************************************************/
void Epos2S_Cmd::GetPositionDOF(int DOF)
{	
	if (!VCS_GetPositionIs(m_KeyHandle, DOF, &KIN.OutputPos[DOF], &m_ulErrorCode))
	{
		cout << "Failed GetPositionIs nodeID_DCX" << DOF << " - Error Code : " << m_ulErrorCode << endl;
		system("PAUSE");
		exit(0);
	}
}

/************************************************************************
Init Wire Tension
************************************************************************/
void Epos2S_Cmd::Wire_Tension(int Motor, int state)
{
	long Before_Pos[MOTOR_NUMBER + 1];
	int PosCount = 0;
	short C_Averaged = 0, C_Must = 0;

	// 전체 와이어들의 텐션 정리.	
	Before_Pos[Motor] = 0;
		
	C_Must = -25;
	VCS_SetCurrentMust(m_KeyHandle, Motor, C_Must, &m_ulErrorCode);
	
	while (1)
	{
		GetPosition(Motor, &KIN.OutputPos[Motor]);
		VCS_GetCurrentIsAveraged(m_KeyHandle, Motor, &C_Averaged, &m_ulErrorCode);
		std::cout << " [ Motor_" << Motor << " ] [ Before Position : " << Before_Pos[Motor] << " ] [ Current Position : " << KIN.OutputPos[Motor] << " ]  [ CurrentIsAveraged : " << C_Averaged << " ]" << endl;

		if (KIN.OutputPos[Motor] == Before_Pos[Motor])
		{
			PosCount++;
			if (PosCount == 5)
			{
				PosCount = 0;

				if (state == INIT_STATE)
				{
					if ((Motor == DOF01_EXT) || (Motor == DOF02_EXT) || (Motor == DOF03_EXT) || (Motor == DOF04_EXT) || (Motor == DOF05_EXT) || (Motor == DOF06_EXT) || (Motor == DOF07_EXT))
					{
						if (C_Must <= -55)
						{							
							VCS_GetCurrentIsAveraged(m_KeyHandle, Motor, &C_Averaged, &m_ulErrorCode);
							std::cout << " [ Motor_" << Motor << " ] [ Before Position : " << Before_Pos[Motor] << " ] [ Current Position : " << KIN.OutputPos[Motor] << " ]  [ CurrentIsAveraged : " << C_Averaged << " ]" << endl;
							break;
						}
						else
						{
							C_Must -= 5;
							VCS_SetCurrentMust(m_KeyHandle, Motor, C_Must, &m_ulErrorCode);
						}

						//if ((Motor == 13) || (Motor == 14))	// 13번 14번 모터는 구동토크가 크기때문에...
						//{
						//	if (C_Must <= -55)
						//	{
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, 0, &m_ulErrorCode);
						//		Sleep(300);
						//		VCS_GetCurrentIsAveraged(m_KeyHandle, Motor, &C_Averaged, &m_ulErrorCode);
						//		std::cout << " [ Motor_" << Motor << " ] [ Before Position : " << Before_Pos[Motor] << " ] [ Current Position : " << KIN.OutputPos[Motor] << " ]  [ CurrentIsAveraged : " << C_Averaged << " ]" << endl;
						//		break;
						//	}
						//	else
						//	{
						//		C_Must -= 5;
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, C_Must, &m_ulErrorCode);
						//	}
						//}
						//else
						//{
						//	if (C_Must <= -55)
						//	{
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, 0, &m_ulErrorCode);
						//		Sleep(300);
						//		VCS_GetCurrentIsAveraged(m_KeyHandle, Motor, &C_Averaged, &m_ulErrorCode);
						//		std::cout << " [ Motor_" << Motor << " ] [ Before Position : " << Before_Pos[Motor] << " ] [ Current Position : " << KIN.OutputPos[Motor] << " ]  [ CurrentIsAveraged : " << C_Averaged << " ]" << endl;
						//		break;
						//	}
						//	else
						//	{
						//		C_Must -= 5;
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, C_Must, &m_ulErrorCode);
						//	}
						//}
					}
					else
					{
						if (C_Must <= -35)
						{
							//VCS_SetCurrentMust(m_KeyHandle, Motor, 0, &m_ulErrorCode);
							//Sleep(300);
							VCS_GetCurrentIsAveraged(m_KeyHandle, Motor, &C_Averaged, &m_ulErrorCode);
							std::cout << " [ Motor_" << Motor << " ] [ Before Position : " << Before_Pos[Motor] << " ] [ Current Position : " << KIN.OutputPos[Motor] << " ]  [ CurrentIsAveraged : " << C_Averaged << " ]" << endl;
							break;
						}
						else
						{
							C_Must -= 5;
							VCS_SetCurrentMust(m_KeyHandle, Motor, C_Must, &m_ulErrorCode);
						}
						//if ((Motor == 13) || (Motor == 14))
						//{
						//	if (C_Must <= -35)
						//	{
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, 0, &m_ulErrorCode);
						//		Sleep(300);
						//		VCS_GetCurrentIsAveraged(m_KeyHandle, Motor, &C_Averaged, &m_ulErrorCode);
						//		std::cout << " [ Motor_" << Motor << " ] [ Before Position : " << Before_Pos[Motor] << " ] [ Current Position : " << KIN.OutputPos[Motor] << " ]  [ CurrentIsAveraged : " << C_Averaged << " ]" << endl;
						//		break;
						//	}
						//	else
						//	{
						//		C_Must -= 5;
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, C_Must, &m_ulErrorCode);
						//	}
						//}
						//else
						//{
						//	if (C_Must <= -30)
						//	{
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, 0, &m_ulErrorCode);
						//		Sleep(300);
						//		VCS_GetCurrentIsAveraged(m_KeyHandle, Motor, &C_Averaged, &m_ulErrorCode);
						//		std::cout << " [ Motor_" << Motor << " ] [ Before Position : " << Before_Pos[Motor] << " ] [ Current Position : " << KIN.OutputPos[Motor] << " ]  [ CurrentIsAveraged : " << C_Averaged << " ]" << endl;
						//		break;
						//	}
						//	else
						//	{
						//		C_Must -= 5;
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, C_Must, &m_ulErrorCode);
						//	}
						//}
					}
				}
				else
				{
					if ((Motor == DOF01_FLD) || (Motor == DOF02_FLD) || (Motor == DOF03_FLD) || (Motor == DOF04_FLD) || (Motor == DOF05_FLD) || (Motor == DOF06_FLD) || (Motor == DOF07_FLD))
					{
						if (C_Must <= -55)
						{
							//VCS_SetCurrentMust(m_KeyHandle, Motor, 0, &m_ulErrorCode);
							//Sleep(300);
							VCS_GetCurrentIsAveraged(m_KeyHandle, Motor, &C_Averaged, &m_ulErrorCode);
							std::cout << " [ Motor_" << Motor << " ] [ Before Position : " << Before_Pos[Motor] << " ] [ Current Position : " << KIN.OutputPos[Motor] << " ]  [ CurrentIsAveraged : " << C_Averaged << " ]" << endl;
							break;
						}
						else
						{
							C_Must -= 5;
							VCS_SetCurrentMust(m_KeyHandle, Motor, C_Must, &m_ulErrorCode);
						}

						//if ((Motor == 13) || (Motor == 14))
						//{
						//	if (C_Must <= -50)
						//	{
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, 0, &m_ulErrorCode);
						//		Sleep(300);
						//		VCS_GetCurrentIsAveraged(m_KeyHandle, Motor, &C_Averaged, &m_ulErrorCode);
						//		std::cout << " [ Motor_" << Motor << " ] [ Before Position : " << Before_Pos[Motor] << " ] [ Current Position : " << KIN.OutputPos[Motor] << " ]  [ CurrentIsAveraged : " << C_Averaged << " ]" << endl;
						//		break;
						//	}
						//	else
						//	{
						//		C_Must -= 5;
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, C_Must, &m_ulErrorCode);
						//	}
						//}
						//else
						//{
						//	if (C_Must <= -40)
						//	{
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, 0, &m_ulErrorCode);
						//		Sleep(300);
						//		VCS_GetCurrentIsAveraged(m_KeyHandle, Motor, &C_Averaged, &m_ulErrorCode);
						//		std::cout << " [ Motor_" << Motor << " ] [ Before Position : " << Before_Pos[Motor] << " ] [ Current Position : " << KIN.OutputPos[Motor] << " ]  [ CurrentIsAveraged : " << C_Averaged << " ]" << endl;
						//		break;
						//	}
						//	else
						//	{
						//		C_Must -= 5;
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, C_Must, &m_ulErrorCode);
						//	}
						//}
					}
					else
					{
						if (C_Must <= -35)
						{
							//VCS_SetCurrentMust(m_KeyHandle, Motor, 0, &m_ulErrorCode);
							//Sleep(300);
							VCS_GetCurrentIsAveraged(m_KeyHandle, Motor, &C_Averaged, &m_ulErrorCode);
							std::cout << " [ Motor_" << Motor << " ] [ Before Position : " << Before_Pos[Motor] << " ] [ Current Position : " << KIN.OutputPos[Motor] << " ]  [ CurrentIsAveraged : " << C_Averaged << " ]" << endl;
							break;
						}
						else
						{
							C_Must -= 5;
							VCS_SetCurrentMust(m_KeyHandle, Motor, C_Must, &m_ulErrorCode);
						}

						//if ((Motor == 13) || (Motor == 14))
						//{
						//	if (C_Must <= -35)
						//	{
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, 0, &m_ulErrorCode);
						//		Sleep(300);
						//		VCS_GetCurrentIsAveraged(m_KeyHandle, Motor, &C_Averaged, &m_ulErrorCode);
						//		std::cout << " [ Motor_" << Motor << " ] [ Before Position : " << Before_Pos[Motor] << " ] [ Current Position : " << KIN.OutputPos[Motor] << " ]  [ CurrentIsAveraged : " << C_Averaged << " ]" << endl;
						//		break;
						//	}
						//	else
						//	{
						//		C_Must -= 5;
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, C_Must, &m_ulErrorCode);
						//	}
						//}
						//else
						//{
						//	if (C_Must <= -30)
						//	{
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, 0, &m_ulErrorCode);
						//		Sleep(300);
						//		VCS_GetCurrentIsAveraged(m_KeyHandle, Motor, &C_Averaged, &m_ulErrorCode);
						//		std::cout << " [ Motor_" << Motor << " ] [ Before Position : " << Before_Pos[Motor] << " ] [ Current Position : " << KIN.OutputPos[Motor] << " ]  [ CurrentIsAveraged : " << C_Averaged << " ]" << endl;
						//		break;
						//	}
						//	else
						//	{
						//		C_Must -= 5;
						//		VCS_SetCurrentMust(m_KeyHandle, Motor, C_Must, &m_ulErrorCode);
						//	}
						//}
					}
				}
			}
		}
		else
			PosCount = 0;

		Before_Pos[Motor] = KIN.OutputPos[Motor];
	}
}


