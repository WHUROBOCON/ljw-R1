#ifndef _TMOTOR_H
#define _TMOTOR_H

#include "queue.h"
#include "canctrllist.h"
#include "mathFunc.h"
#include "tim3.h"
#include "param.h"
#include "mech.h"
#define TmotorRX 0x00
 #define ZDriveRX 0x01

//电机极限参数（见电机说明）
#define P_MIN -95.5f    // 位置极限（rad）
#define P_MAX 95.5f        
#define V_MIN -50.0f    // 速度极限（Rad/s）
#define V_MAX 50.0f
#define KP_MIN 0.0f     // KP极限（N-m/rad）
#define KP_MAX 500.0f
#define KD_MIN 0.0f     // KD极限（N-m/rad/s）
#define KD_MAX 5.0f
#define T_MIN -18.0f		//扭矩极限（Nm）
#define T_MAX 18.0f 

enum TmotorMode
{
	TPos,		//位置模式-0
	TVel,		//速度模式-1
	TPosVel	//速度位置模式-2
};

typedef struct
{
	float angle2RAD;
	float RATIO;
	float GearRadio;
} TmotorParamTypedef;

typedef struct
{
	float angle;	//角度
	float speed;	//速度
	float torque;	//扭矩
	float kp_BG;
	float kd_BG;
	float kp_ST;
	float kd_ST;
	float Acc;
	float AccRange;
	float Dcc;
	float DccRange;
	float SPStart;
	float LockRange;
} TmotorValueTypedef;

typedef struct
{
	u32 timeoutCnt;	//超时计数器
	u32 stuckCnt;	//阻塞计时器
	float LockAngle;	//
	float TimeBG;
	float TimeNOW;
	float AngStart;
	float AngBG;
	float AngCal;
	float SPTemp;
	float VelCal;
} TmotrArgumTypedef;

typedef struct
{
	bool timeout;
	bool stuck;
} TmotorStatusTypedef;

typedef struct
{
	bool If_TimeoutDect;
	bool If_StuckDect;
	bool ifReleaseWhenStuck;
	float MaxPosSP;
	float MinPosSP;
	float MaxAng;
	float MinAng;
} TmotorLimitTypedef;

typedef struct
{
	u32 DeCnt;
	bool enable;
	bool SetZero;
	float TimeUse;
	u8 mode;
	TmotorValueTypedef valueReal, valueSet;
	TmotorParamTypedef param;
	TmotrArgumTypedef argum;
	TmotorStatusTypedef status;
	TmotorLimitTypedef limit;
} TMotorTypedef;

typedef struct
{
	float Angdest;
	float MaxSP;
	float MinSP;
	float torque;
	float kp;
	float kd;
	float SPStart;
	float Acc;
	float AccRange;
	float Dcc;
	float DccRange;
} MotionPARMTypedef;

typedef struct _TmotorSTRUCT
{
	u8 AKStatus;
	void (*ComInit)(u8 ID);
	void (*MO1)(u8 ID);
	void (*MO0)(u8 ID);
	void (*BG)(u8 ID);
	void (*ST)(u8 ID);
	void (*PA)(u8 ID, float Ang);
	void (*UM)(u8 ID, u8 mode);
	void (*SP)(u8 ID, float PosSP);
	void (*PX0)(u8 ID);
	void (*AK80_MotionPlus)(MotionPARMTypedef MotionPARM);
} TMotorStruct;

u16 float2uint(float x, float x_min, float x_max, u8 bits);
float uint2float(int x_int, float x_min, float x_max, int bits);

void Tmotor_Init(void);
void Tmotor_Comm_Init(CAN_TypeDef *CANx, u8 ID);
void Tmotor_Control(u8 ID, float f_pos, float f_vel, float f_kp, float f_kd, float f_torque, bool Mail_If2CtrlList);
void TmotorPosMode(u8 ID, bool Mail_If2CtrlList);
void Tmotor_SetZZero(u8 ID, bool Mail_If2CtrlList);
void TmotorFunc(void);
void TmotorReceiveHandler(CanRxMsg rx_message);
void TmotorCtrl(u8 ID);

extern TMotorTypedef Tmotor[9];
extern TMotorStruct TMD;
extern MotionPARMTypedef ContinousLoadPARM;
#endif
