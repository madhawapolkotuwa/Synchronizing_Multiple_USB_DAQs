/*********************************************************************
*
* ANSI C Example program:
*    NiDaqSync.c
*
* Example Category:
*    AI
*
* Description:
*    This example demonstrates how to acquire a continuous amount of
*    data using the DAQ device's internal clock.
*
* Instructions for Running:
*    1. Select the physical channels to correspond to where your
*       signals are input on the DAQ device.
*    2. Enter the minimum and maximum voltage range.
*    Note: For better accuracy try to match the input range to the
*          expected voltage level of the measured signal.
*    3. Set the rate of the acquisition. Also set the Samples per
*       Channel control. This will determine how many samples are
*       read at a time. This also determines how many points are
*       plotted on the graph each time.
*    Note: The rate should be at least twice as fast as the maximum
*          frequency component of the signal being acquired.
*
* Steps:
*    1. Create a task.
*    2. Create an analog input voltage channel.
*    3. Set the rate for the sample clock. Additionally, define the
*       sample mode to be continuous.
*    4. Call the Start function to start the acquistion.
*    5. Read the data in the EveryNCallback function until the stop
*       button is pressed or an error occurs.
*    6. Call the Clear Task function to clear the task.
*    7. Display an error if any.
*
* I/O Connections Overview:
*    Make sure your signal input terminal matches the Physical
*    Channel I/O control. For further connection information, refer
*    to your hardware reference manual.
*
*********************************************************************/

#include <stdio.h>
#include "NIDAQmx.h"

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);

TaskHandle  taskHandle1; 
TaskHandle  taskHandle2;// = 1;

float64     data1[1000*8]; // 1000 pointers each channel
float64     data2[1000*8];

FILE *file = NULL;

int main(void)
{
	int32       error=0;
	
	char        errBuff[2048]={'\0'};
	
	/*********************************************/
	// DAQmx Configure Code
	/*********************************************/
	DAQmxErrChk (DAQmxCreateTask("DAQ1",&taskHandle1));
	DAQmxErrChk(DAQmxCreateTask("DAQ2", &taskHandle2));

	
	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandle1, "Dev1/ai0:7", "", DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, NULL)); // Device one
	DAQmxErrChk(DAQmxCreateAIVoltageChan(taskHandle2, "Dev3/ai0:7", "", DAQmx_Val_RSE, -10.0, 10.0, DAQmx_Val_Volts, NULL)); // Device two

	DAQmxErrChk (DAQmxCfgSampClkTiming(taskHandle1,"",1000.0,DAQmx_Val_Rising,DAQmx_Val_ContSamps,1000));
	DAQmxErrChk(DAQmxCfgSampClkTiming(taskHandle2, "", 1000.0, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 1000));

	DAQmxErrChk (DAQmxRegisterEveryNSamplesEvent(taskHandle1,DAQmx_Val_Acquired_Into_Buffer,1000,0,EveryNCallback,NULL));
	DAQmxErrChk(DAQmxRegisterEveryNSamplesEvent(taskHandle2, DAQmx_Val_Acquired_Into_Buffer, 1000, 0, NULL, NULL));

	DAQmxErrChk (DAQmxRegisterDoneEvent(taskHandle1,0,DoneCallback,NULL));


	// Open the text file
	file = fopen("data.txt", "w");

	/*********************************************/
	// DAQmx Start Code
	/*********************************************/
	DAQmxErrChk (DAQmxStartTask(taskHandle1));
	DAQmxErrChk(DAQmxStartTask(taskHandle2));

	printf("Acquiring samples continuously. Press Enter to interrupt\n");
	getchar();


	

Error:
	if( DAQmxFailed(error) )
		DAQmxGetExtendedErrorInfo(errBuff,2048);
	if( taskHandle1!=0 ) {
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle1);
		DAQmxClearTask(taskHandle1);
		DAQmxStopTask(taskHandle2);
		DAQmxClearTask(taskHandle2);
	}
	if( DAQmxFailed(error) )
		printf("DAQmx Error: %s\n",errBuff);
	printf("End of program, press Enter key to quit\n");
	getchar();
	fclose(file);
	return 0;
}

int32 CVICALLBACK EveryNCallback(TaskHandle task, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
	int32       error=0;
	char        errBuff[2048]={'\0'};
	static int  totalRead1=0;
	static int  totalRead2 = 0;
	int32       read1=0;
	int32       read2 = 0;
	float64*    pwData1 = &data1[0];
	float64*    pwData2 = &data2[0];
	

	/*********************************************/
	// DAQmx Read Code
	/*********************************************/
	DAQmxErrChk (DAQmxReadAnalogF64(taskHandle1,-1,10.0,DAQmx_Val_GroupByScanNumber, pwData1,1000*8,&read1,NULL));
	DAQmxErrChk(DAQmxReadAnalogF64(taskHandle2, -1, 10.0, DAQmx_Val_GroupByScanNumber, pwData2, 1000*8, &read2, NULL));

	/*if( read>0 ) {
		printf("Acquired %d samples. Total %d\r",(int)read1,(int)(totalRead1+=read1));
		fflush(stdout);
	}*/
	if (read2>0) {
		printf("Acquired %d samples. Total %d\r", (int)read2, (int)(totalRead2 += read2));
		fflush(stdout);
	}
	if (read2 > 0) {
		int count = 0;
		while (count < (int)read2) {
			fprintf(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \n",
				*(pwData1 + 0),
				*(pwData1 + 1),
				*(pwData1 + 2),
				*(pwData1 + 3),
				*(pwData1 + 4),
				*(pwData1 + 5),
				*(pwData1 + 6),
				*(pwData1 + 7),
				*(pwData2 + 0),
				*(pwData2 + 1),
				*(pwData2 + 2),
				*(pwData2 + 3),
				*(pwData2 + 4),
				*(pwData2 + 5),
				*(pwData2 + 6),
				*(pwData2 + 7));
			pwData1 += 8;
			pwData2 += 8;
			count++;
		}
	}
Error:
	if( DAQmxFailed(error) ) {
		DAQmxGetExtendedErrorInfo(errBuff,2048);
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle1);
		DAQmxClearTask(taskHandle1);
		DAQmxStopTask(taskHandle2);
		DAQmxClearTask(taskHandle2);
		printf("DAQmx Error: %s\n",errBuff);
	}
	return 0;
}

int32 CVICALLBACK DoneCallback(TaskHandle task, int32 status, void *callbackData)
{
	int32   error=0;
	char    errBuff[2048]={'\0'};

	// Check to see if an error stopped the task.
	DAQmxErrChk (status);

Error:
	if( DAQmxFailed(error) ) {
		DAQmxGetExtendedErrorInfo(errBuff,2048);
		DAQmxClearTask(taskHandle1);
		DAQmxClearTask(taskHandle2);
		printf("DAQmx Error: %s\n",errBuff);
	}
	return 0;
}
