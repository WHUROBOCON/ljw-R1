#ifndef _DJMOTOR_H
#define _DJMOTOR_H

#include "stm32f4xx.h"
#include "stdbool.h"
#include "stm32f4xx_can.h"
#include "includes.h"

#include "pid.h"
#include "macro.h"
#include "mathFunc.h"
#include "param.h"

#define DJ_MOTOR1_RX 0x201
#define DJ_MOTOR2_RX 0x202
#define DJ_MOTOR3_RX 0x203
#define DJ_MOTOR4_RX 0x204
#define DJ_MOTOR5_RX 0x205		
#define DJ_MOTOR6_RX 0x206		
#define DJ_MOTOR7_RX 0x207
#define DJ_MOTOR8_RX 0x208


enum DJ_Motor_Mode{DJRPM,DJPOSITION,DJZERO};

typedef struct{
	volatile float angle;	//输出轴角度
	vs16 current;
	vs16 speed;
	vs32 pulse;	//累计脉冲
	vs16 pulseRead;	//编码器反馈脉冲
	vs16 temperature;
	float current_Read;
}DJMotorValueTypedef;

typedef struct{
	u16 PulsePerRound;	//编码器线数
	u8 RATIO;	//电机自带减速比
	s16 Current_Limit;
	float GearRadio;	//机构齿数比
}DJMotorParamTypedef;

typedef struct{
	bool arrived;
	bool stuck;
	bool timeout;
	bool isSetZero; 
}DJMotorStatusTypedef;

typedef struct{
	bool PosLimit_ON;
	float maxAngle;	float minAngle;

	bool PosSPLimit_ON;
	u16 PosSPLimit;

	bool isReleaseWhenStuck;
	bool stuckDetection_ON;
	bool timeoutDetection_ON;

	s16 ZeroSP;
	u16 ZeroCurrent;
}DJMotorLimitTypedef;

typedef struct
{
	vs32 lockPulse;	//锁位置脉冲
	s16 distance;	//当前反馈脉冲与上次反馈脉冲之差
	u32 lastRxTime;
  u16 ZeroCnt;	
	u16 TimeoutCnt;		//范围是从0-65535，超过后会重新从0开始计数
	u16 StuckCnt;
}DJMotorArgumTypedef;

typedef struct{
	bool enable;
	bool begin;
	unsigned char mode;
	DJMotorValueTypedef valueSet,valueReal,valuePrv;
	DJMotorParamTypedef Param;
	DJMotorStatusTypedef Status;
	DJMotorLimitTypedef Limit;
	DJMotorArgumTypedef argum;
	Inc_PIDTypedef PID_POS,PID_RPM;
	myPIDTypedef myPID_POS,myPID_RPM;
}DJMotor;

extern DJMotor DJmotor[8];
extern u16 ShouZhua_Sp;

void DJmotor_Init(void);
void DJ_SetZero(u8 id,float angle);
void DJ_Position_Calculate(u8 id);	//在can接收中断中计算
void DJ_SpeedMode(u8 id);
void DJ_LockPosition(u8 id);
void DJ_PostionMode(u8 id);
void DJ_ZeroMode(u8 id);
void DJ0To3_CurrentTransmit(u8 Mail_If2CtrlList);
void DJ4To7_CurrentTransmit(u8 Mail_If2CtrlList);
void DJ_ifTimeOut(u8 id);		//在任务里面检测超时还有堵转
void DJ_ifStuck(u8 id);
void DJFunc(void);

void DJ_CurrentTransmit(u8 id);

extern float DJMotor3CCurrent;

#endif
