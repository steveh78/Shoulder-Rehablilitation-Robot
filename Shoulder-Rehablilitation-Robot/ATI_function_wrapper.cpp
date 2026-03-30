#include "Stdafx.h"
#include <stdint.h>

#include <stdio.h>
#include <windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "NIDAQmx.h"

#include <stdlib.h>
#include "ftconfig.h"

TaskHandle taskHandle;

int FTsensor_Initialize()
{
	int result;
	result = DAQmxCreateTask("", &taskHandle);
	result = DAQmxCreateAIVoltageChan(taskHandle,
		"/Dev1/ai0:5",
		"",
		DAQmx_Val_RSE,
		-10.,
		10.,
		DAQmx_Val_Volts,
		NULL);

	result = DAQmxStartTask(taskHandle);

	printf("DAQ for ATI Initialization Complete\n");

	return 0;
}

int FTsensor_Read(double* FTvalue)
{
	int32 numRead;
	double Vol[6];

	DAQmxReadAnalogF64(taskHandle,
		1,
		0.005,
		DAQmx_Val_GroupByChannel,
		Vol, // Vol --> Read Array
		6,
		&numRead,
		NULL);
	/* 
	TaskHandle taskHandle, 
	int32 numSampsPerChan,
	float64 timeout,
	bool32 fillMode, 
	float64 readArray[], 
	uInt32 arraySizeInSamps, 
	int32 *sampsPerChanRead, 
	bool32 *reserved
	*/

	//float SampleBias[6] = { 0.03154,-0.1166,0.02294,-0.1850,0.0669,-0.1862 };
	//float SampleBias[6] = { 0., };
	// float SampleBias[6] = { -0.0507, -0.5192, -0.0423, 0.1135, 0.1379, 0.1467 }; 
	// float SampleBias[6] = {0.1990, -1.2436, 0.6808, 0.1432, 0.1856, 0.3670}; 최종본 1
	// float SampleBias[6] = {0.1800, -1.2436, 0.6808, 0.1432, 0.1856, 0.3670}; 최종본 2
	float SampleBias[6] = { -0.68, 2.15, -1.43, 1.01, -0.45, 0.64 };
	//float SampleBias[6] = { 0, 0, 0, 0, 0, 0 };
	//float SampleBias[6] = {0.42, -1.13, 0.43, 0.33, 0.20, 0.45};
	float FT[6] = { 0., };
	char *calfilepath;      // name of calibration file
	unsigned short index;   // index of calibration in file (second parameter; default = 1)
	Calibration *cal;		// struct containing calibration information
	unsigned short i;       // loop variable used to print results
	short sts;              // return value from functions
	float Vol_float[6] = { 0., };

	for (int i = 0; i < 6; i++)
	{
		Vol_float[i] = Vol[i];
	}

	calfilepath = (char*)"FT15279.cal";
	index = 1;

	cal = createCalibration(calfilepath, index);
	sts = SetForceUnits(cal, (char*)"N");
	sts = SetTorqueUnits(cal, (char*)"N-m");
	Bias(cal, SampleBias);

	ConvertToFT(cal, Vol_float, FT);


	for (int i = 0; i < 6; i++)
	{
		FTvalue[i] = FT[i];
		// FTvalue[i] = Vol_float[i];
	}
	return 0;

}

int FTsensor_Terminate()
{
	return 0;
}
