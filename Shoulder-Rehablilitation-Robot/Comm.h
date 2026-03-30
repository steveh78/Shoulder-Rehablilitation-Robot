#pragma once

using namespace std;

class Comm_Cmd
{

public:
	// 생성자&파괴자.
	//Comm_Cmd();
	//~Comm_Cmd();

	void	Comm_Open();	
	void	TxData();
	void	GeMasterPosition();
	BOOL	RxData(int readSize);

	HANDLE hCom;
	unsigned char Comm_buff[50];

	int		Data_status, CheckSum, Sum;
	int		Data[DTL][DIGIT], Data_Result[DTL], Master_InitP[DOF_NUM];
	int		A_Count = 0;
	double	A_Buff[DTL][50] = { { 0, }, }, M_Sum[DTL] = { 0, }, M_Avr[DTL] = { 0, };


private:

};

extern Comm_Cmd	COM;
