#pragma once


class Kinematics
{
public:

	int Start_End_Limit(void);
	void R_BackUp_OriginPosition(void);
	void R_BackUp_StartPosition(void);
	void R_BackUp_EndPosition(void);
	

	void L_BackUp_OriginPosition(void);
	void L_BackUp_StartPosition(void);
	void L_BackUp_EndPosition(void);
	

	void BackUp_PulseRange(void);
	void BackUp_StepInfo(void);
	void Init_BaseAngle(void);
	

	void Mapping(void);
	void Find_Fq1(void);	
	void Find_Fq1_To_Base(void);
	void Find_FB(void);
	void Find_Fq2(void);
	void Find_Mq2(void);
	void Inverse(void);
	void Cal_Move_Pulse(int DOF, int DOF_n, int Direction);
	double Cal_Move_Time(int DOF, long Start_Pulse, long End_Pulse);
	void Cal_Motor_Vel(WORD M_Num, long Start_Pulse, long End_Pulse);
	void Cal_CouplingData(void);

	FILE* Vel_Pos_file = NULL;
	FILE* R_Origin_Pos_file = NULL;
	FILE* L_Origin_Pos_file = NULL;
	FILE* Start_Pos_file = NULL;
	FILE* End_Pos_file = NULL;
	FILE* PR_DOF_file = NULL;
	FILE* Step_Deg_Range_file = NULL;
	FILE* AB_ADDUCTION_file = NULL;
	FILE* EX_IN_ROTATION_file = NULL;
	FILE* EX_FLEXION_file = NULL;
	FILE* Right_Go_Left_file = NULL;
	FILE* Left_Go_Right_file = NULL;
	FILE* R_Start_file = NULL;
	FILE* L_Start_file = NULL;
	FILE* Measure_file = NULL;

	char	f_name;
	int		Teaching_Point = 1;
	int		f_Num, f_Data1, f_Data2, f_Data3, f_Data4, f_Data5, f_Data6, f_Data7;
	long	f_Data;

	int		Degree_range;
	int		DOF_Range[8];

	int		Motor_Origin[5] = { 0, };
	int		Motor_NUM[5] = { 0, 1, 2, 3, 4};
	//double	Cal_DPP_DOF[8], Cal_PPD_DOF[8];

	long	PR_DOF[8];
	//long	Pre_Pos[MOTOR_NUMBER + 1];
	long	OutputPos[MOTOR_NUMBER + 1];
	long	Start_Pos[MOTOR_NUMBER + 1];
	long	Origin_Pos[MOTOR_NUMBER + 1];
	long	End_Pos[MOTOR_NUMBER + 1];
	long	InputPos[MOTOR_NUMBER + 1];
	long	Vel_Pos[MOTOR_NUMBER + 1];
	long	R_Go_L[MOTOR_NUMBER + 1];
	long	Step_Deg_Range[50][8];
		
	double	Robot_DOF_Range[7] = { 120, 90, 90, 90, 100, 90, 20 };
	double  Toler[14] = { 0, 0, 2, 2, 2, 0, 0, 0, -1, -1, -0.5, 0, 0, 0 };
	double	Margin[DOF_NUM] = { 35000, 45000, 45000, 45000, 45000, 45000, 45000 }; //{0, 0 .0, 8번, 13번, 6번, 7번}
	double  Move_State[DOF_NUM] = { 1, 1, 1, 1, 1, 1, 1 };

	double	Max_Time[MOTOR_NUMBER + 1] = { 0, };
	double  Robot_DegrePerPulse[MOTOR_NUMBER + 1] = { 0, }, Robot_PulsePerDegre[MOTOR_NUMBER + 1] = { 0, };
	double	Couple_K2, Couple_K3, Couple_K4, Couple_K5, Couple_K6, Couple_K7;
	double	Upper1, Low1, Upper2, Middle2, Low2, Upper3, Middle3, Low3, Upper4, Middle4, Low4, Upper5, Low5, Upper6;	

	double	MR_Th[DTL] = { 0, }, MD_Th[DTL] = { 0, }, MD_Temp[DTL] = { 0, }, MD_Pre_Th[DTL] = { 0, };
	
	double	RR_Th[DTL] = { 0, }, RD_Th[DTL] = { 0, };

	double	X4, Y4, Z4, X3, X31, X32, Y3, Y31, Y32, Z3, Z31, Z32;
	double	A1, A2, A21, A22, A23, A24, A3, A31, A32, A4, A41, A42, A43, A44, A5, A51, A52, A6, A61, A62;
	double	Q, M, M1, M2, Alp1, Alp2, Beta1, Beta2, RF_X, RF_Y, RF_Z;

	double	Fq1_X	= 0, Fq1_Y	 = 0, Fq1_Z	  = 0;	// Master 기준 Init End Point의 위치좌표(Fq1). 
	double	Fq1_r11 = 1, Fq1_r12 = 0, Fq1_r13 = 0;	// Master 기준 Init End Point의 Rotation좌표(Fq1_r). 
	double	Fq1_r21 = 0, Fq1_r22 = 1, Fq1_r23 = 0;
	double	Fq1_r31 = 0, Fq1_r32 = 0, Fq1_r33 = 1;
	
	double	Fq2_X = 0, Fq2_Y = 0, Fq2_Z = 0;		// Master 기준 Move End Point의 위치좌표(Fq2). 
	double	Fq2_r11 = 1, Fq2_r12 = 0, Fq2_r13 = 0;	// Master 기준 Move End Point의 Rotation좌표(Fq2_r). 
	double	Fq2_r21 = 0, Fq2_r22 = 1, Fq2_r23 = 0;
	double	Fq2_r31 = 0, Fq2_r32 = 0, Fq2_r33 = 1;

			
	double	FB_X   = 0, FB_Y   = 0, FB_Z   = 0;		// Master 기준 Robot Base의 원점좌표(FB). 
	double	FB_r11 = 1, FB_r12 = 0, FB_r13 = 0;		// Master 기준 Robot Base의 Rotation좌표(FB_r).
	double	FB_r21 = 0, FB_r22 = 1, FB_r23 = 0;
	double	FB_r31 = 0, FB_r32 = 0, FB_r33 = 1;

	double	Mq2_X	= 0, Mq2_Y	 = 0, Mq2_Z	  = 0;	// Master 기준 End Point의 위치좌표(Fq2)를 
	double	Mq2_r11 = 1, Mq2_r12 = 0, Mq2_r13 = 0;	// Robot Base에서 바라본 값으로 변환한 위치좌표(Mq2). 
	double	Mq2_r21 = 0, Mq2_r22 = 1, Mq2_r23 = 0;	// Master 기준 End Point의 Rotation좌표(Fq2_r)를 
	double	Mq2_r31 = 0, Mq2_r32 = 0, Mq2_r33 = 1;	// Robot Base에서 바라본 값으로 변환한 Rtation좌표(Mq2_r). 



private:

};

extern Kinematics KIN;