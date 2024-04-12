#include "pid.h"

void Inc_PID_Init(Inc_PIDTypedef* PID,float KP,float KI,float KD,int Set)
{
	PID->CurVal=0;
	PID->SetVal=Set;
	PID->midVal[0]=0;
	PID->midVal[1]=0;
	PID->midVal[2]=0;
	PID->Kp=KP;
	PID->Ki=KI;
	PID->Kd=KD;
}

// 位置环的 I应该是P P应该是d
void Inc_PID_Operation(Inc_PIDTypedef* PID)
{
	PID->midVal[0]=PID->SetVal-PID->CurVal;
	PID->delta = PID->Kp * (PID->midVal[0]-PID->midVal[1]) + PID->Ki * PID->midVal[0] + PID->Kd * (PID->midVal[0]+PID->midVal[2]-2*PID->midVal[1]);
	PID->midVal[2]=PID->midVal[1];
	PID->midVal[1]=PID->midVal[0];
}

//void myPID_Init(myPIDTypedef* PID,float KP,float KI,float KD,int Set)
//{
//	PID->CurVal=0;
//	PID->SetVal=Set;
//	PID->Kp=KP;
//	PID->Ki=KI;
//	PID->Kd=KD;
//	PID->err=0;
//	PID->err_last=0;
//	PID->integral=0;
//	PID->result=0;
//}

//void myPID_Operation(myPIDTypedef* PID)
//{
//	float index;
//	PID->err=PID->SetVal-PID->CurVal;
//	if(ABS(PID->err)>PID->SetVal)
//		index=0;
//	else if(ABS(PID->err)<0.9*PID->SetVal)
//		index=1;
//	else
//		index=(PID->SetVal-ABS(PID->err))/(0.1*PID->SetVal);
//	PID->integral+=PID->err;
//	PID->result=PID->Kp*PID->err+index*PID->Ki*PID->integral+PID->Kd*(PID->err-PID->err_last);
//	PID->err_last=PID->err;
//}
