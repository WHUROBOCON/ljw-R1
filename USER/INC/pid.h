#ifndef _PID_H
#define _PID_H

#include "stm32f4xx.h"
#include "mathFunc.h"

typedef struct
{
	vs32 SetVal;
	float Kp;
	float Ki;
	float Kd;
	vs32 delta;
	vs32 CurVal;
	vs32 midVal[3];
}Inc_PIDTypedef;

typedef struct
{
	vs32 SetVal,CurVal;
	float Kp,Ki,Kd;
	float err,err_last;
	float integral;
	float result;
}myPIDTypedef;

void Inc_PID_Init(Inc_PIDTypedef* PID,float KP,float KI,float KD,int Set);
void Inc_PID_Operation(Inc_PIDTypedef* PID);
void myPID_Init(myPIDTypedef* PID,float KP,float KI,float KD,int Set);
void myPID_Operation(myPIDTypedef* PID);

#endif
