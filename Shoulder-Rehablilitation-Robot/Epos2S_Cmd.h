#pragma once


class Epos2S_Cmd
{

public:
	// 생성자&파괴자.
	Epos2S_Cmd();
	~Epos2S_Cmd();

	BOOL OpenDevice();	
	BOOL SetPositionMode();
	BOOL SetDriveToPosition(int nodeID);
	BOOL SetCurrentMode();
	BOOL SetDriveToCurrent(int nodeID);
	BOOL RestoreEPOS();

	void InitEPOS2S(void);
	void RestoreSetting(int nodeID);

	void MotorEnable(int nodeID);
	void MotorDisable(int nodeID);
	void MotorStop(int nodeID);
	void MotorMove(int nodeID, long Pulse_Position);
	void MoveDegree(int DOF, long Deg_Position);
	void MoveDOF_PULSE(int DOF, long PULSE_Position);
	void MoveDOF(int Motor, long Deg_Position);
	void MoveDOF01(long Deg_Position);
	void MoveDOF02(long Deg_Position);
	void MoveDOF03(long Deg_Position);
	void MoveDOF04(long Deg_Position);
	void MoveDOF05(long Deg_Position);
	void MoveDOF06(long Deg_Position);
	void MoveDOF07(long Deg_Position);
	void MoveSTEP(int Step, float tf);

	void Cal_vel(int Motor, float Start_Angle, int End_Angle, float tf);
	
	void GetPositionDOF(int DOF);
	void Wire_Tension(int Motor, int state);
	void GetPositionALL(long * pos);
	void GetPosition(int nodeID, long * pos);

	__int8 m_bMode;

	int     m_oRadio;

	long    m_lActualValue;
	long    m_lStartPosition;
	long    m_lTargetPosition;
	
	short	m_uCurrentMust;
	short	m_uCurrentIs; 

	BOOL	m_oInitialisation;
	BOOL	m_oImmediately;
	BOOL	m_oInit_Position;

	WORD	m_usNodeId;

	DWORD	m_ulErrorCode;	
	DWORD	m_ulProfileAcceleration;
	DWORD	m_ulProfileDeceleration;
	DWORD	m_ulProfileVelocity;

	HANDLE	m_KeyHandle;
		
	int		nodeID_DCX01;
	int		nodeID_DCX02;
	int		nodeID_DCX03;
	int		nodeID_DCX04;
	int		nodeID_DCX05;
	int		nodeID_DCX06;
	int		nodeID_DCX07;
	int		nodeID_DCX08;
	int		nodeID_DCX09;
	int		nodeID_DCX10;
	int		nodeID_DCX11;
	int		nodeID_DCX12;
	int		nodeID_DCX13;
	int		nodeID_DCX14;

	

	//int		m_DeviceSettings;
	//int		m_Disable;
	//int		m_Enable;
	//int		m_Halt;
	//int		m_Move;
	//int		m_strActiveMode;
	//int		m_strNodeId;
	//int     m_oRadio;
	//long    m_lActualValue;
	//long    m_lStartPosition;
	//long    m_lTargetPosition;


private:

	



	//char* PortName_COM;

	//BYTE nodeId_EPOS1;
	//BYTE nodeId_EPOS2;
	//BYTE nodeId_EPOS3;
	//BYTE nodeId_EPOS4;
	//BYTE nodeId_EPOS5;
	//BYTE nodeId_EPOS6;
	//BYTE nodeId_EPOS7;
	//BYTE nodeId_EPOS8;
	//BYTE nodeId_EPOS9;
	//BYTE nodeId_EPOS10;
	//BYTE nodeId_EPOS11;
	//BYTE nodeId_EPOS12;
	//BYTE nodeId_EPOS13;
	//BYTE nodeId_EPOS14;
	//BYTE nodeId_EPOS15;
	//DWORD ErrorCode;

	// 전역 클래스 !!!
public:

	//void ActivateEpos2P();
	//void DriveSettingEPOS2();
	//void MoveJoint(LONG DesiredPosition[7]);
	//void SingleMoveJoint(int DOF, LONG DesiredPosition);
	//void GetEncorder(LONG pos[15]);
	//void GetEncorder_Single(int DOF, long * pos);



private:
	// 지역 클래스 !!

	//// 모터 운영
	//void EnableDevice(HANDLE keyHandle_);
	//void disableDevice(HANDLE keyHandle_);
	//void HaltDevice(HANDLE keyHandle_);
	//void ClearFault(HANDLE keyHandle_);
	//void MoveDevice(HANDLE keyHandle_, LONG TargetPosition);

	//// 오류체크
	//void GetError();

	////종료
	//void CloseDrives(void *keyHandle_);
	//void CloseGateways(HANDLE CommunicationHandle, void *keyHandle_);
	//void CloseComm(void *keyHandle_);



private:
	//// 공용 핸들
	//void *keyHandle_BASE;

	//// 통신 핸들
	//void *keyHandle_COM;

	//// 게이트웨이 핸들
	//void *keyHandle_CAN;

	//// 모터드라이버 핸들
	//void *keyHandle_EPOS1;
	//void *keyHandle_EPOS2;
	//void *keyHandle_EPOS3;
	//void *keyHandle_EPOS4;
	//void *keyHandle_EPOS5;
	//void *keyHandle_EPOS6;
	//void *keyHandle_EPOS7;
	//void *keyHandle_EPOS8;
	//void *keyHandle_EPOS9;
	//void *keyHandle_EPOS10;
	//void *keyHandle_EPOS11;
	//void *keyHandle_EPOS12;
	//void *keyHandle_EPOS13;
	//void *keyHandle_EPOS14;

	

	//void *keyhandle_DCX1;
	//void *keyhandle_DCX2;
	//void *keyhandle_DCX3;
	//void *keyhandle_DCX4;
	//void *keyhandle_DCX5;
	//void *keyhandle_DCX6;
	//void *keyhandle_DCX7;
	//void *keyhandle_DCX8;
	//void *keyhandle_DCX9;
	//void *keyhandle_DCX10;
	//void *keyhandle_DCX11;
	//void *keyhandle_DCX12;
	//void *keyhandle_DCX13;
	//void *keyhandle_DCX14;

};

extern Epos2S_Cmd	EPOS2S;