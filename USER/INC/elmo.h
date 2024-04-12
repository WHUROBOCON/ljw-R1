#ifndef _ELMO_H
#define _ELMO_H

#include "queue.h"
#include "canctrllist.h"
#include "mathFunc.h"

#define Elmo_Motor1_RX	0x281
#define Elmo_Motor2_RX	0x282
#define Elmo_Motor3_RX	0x283
#define Elmo_Motor4_RX	0x284
#define Elmo_Motor5_RX	0x285
#define Elmo_Motor6_RX	0x286
#define Elmo_Motor7_RX	0x287
#define Elmo_Motor8_RX	0x288

#define Elmo_Motor1_TX	0x301
#define Elmo_Motor2_TX	0x302
#define Elmo_Motor3_TX	0x303
#define Elmo_Motor4_TX	0x304
#define Elmo_Motor5_TX	0x305
#define Elmo_Motor6_TX	0x306
#define Elmo_Motor7_TX	0x307
#define Elmo_Motor8_TX	0x308

#define Elmo_Motor1_error 0x81
#define Elmo_Motor2_error 0x82
#define Elmo_Motor3_error 0x83
#define Elmo_Motor4_error 0x84
#define Elmo_Motor5_error 0x85
#define Elmo_Motor6_error 0x86
#define Elmo_Motor7_error 0x87
#define Elmo_Motor8_error 0x88

typedef struct{
	u16 PulsePerRound;
	float RATIO;
	float GearRadio;
}ELMOParamTypedef;

typedef struct{
	vs32 speed;
	vs32 pulse;
	float current;
//	vs32 angle;
	float angle_f;
	s32 PosSP;
	s32 PX;
}ELMOValueTypedef;

typedef struct{
	u32 lastRxTim;
	u32 stuckCnt;
}ELMOArgumTypedef;

typedef struct{
	bool timeout;
	bool stuck;
}ELMOStatusTypedef;

typedef struct{
	bool If_TimeoutDect;
	bool If_StuckDect;
	bool ifReleaseWhenStuck;
}ELMOLimitTypedef;

/*ID不知道怎么读，就先暂时不用了*/
typedef struct{
	bool Can_Mesg_Lost;		//0x81
	bool Protocal_ERR;		//
	bool Over_Current;
	bool SpTrack_ERR;
	bool PosTrack_ERR;
}ElmoERRTypedef;

typedef struct{
	volatile bool enabled;
	bool Electrified;	//[上电提醒]
	float Sp_Read;		//[低转速时。速度太小，定义int读不出来]
	ELMOValueTypedef	valueReal,valueSet;
	ELMOParamTypedef	param;
	ELMOArgumTypedef 	argum;
	ELMOStatusTypedef status;
	ELMOLimitTypedef limit;
//	ElmoERRTypedef	Err;
	bool CanError;
	bool ErrMsgOccur;
}ElmoMotorTypedef;

typedef struct
{
	bool Init_Can;
	bool Set_Enable;
	bool Set_Disable;
	bool Set_PosMode;
	bool Set_SpMode;
	bool Set_BG;
	bool Set_ST;
	bool Set_SP;
	bool Set_PA;
	bool Set_JV;
	bool Set_PX;
	bool Ask_PX;
	bool Ask_VX;
}ElmoCtrlTypedef;

extern ElmoMotorTypedef ElmoMotor[5];

void ElmoMotor_Init(void);
void ELMO_Comm_Init(CAN_TypeDef* CANx);
void Elmo_Set_PosUM(u8 ID,u8 Mail_If2CtrlList);
void Elmo_Set_SpUM(u8 ID,u8 Mail_If2CtrlList);
void Elmo_Set_MO1(u8 ID,u8 Mail_If2CtrlList);
void Elmo_Set_MO0(u8 ID,u8 Mail_If2CtrlList);
void Elmo_Set_SP(u8 ID,s32 speed,u8 Mail_If2CtrlList);
void Elmo_Set_JV(u8 ID,s32 speed,u8 Mail_If2CtrlList);
void Elmo_Set_PX(u8 ID,s32 angle, u8 Mail_If2CtrlList);
void Elmo_Set_PA(u8 ID,s32 angle, u8 Mail_If2CtrlList);
void Elmo_Set_PA_f(u8 ID,float angle, u8 Mail_If2CtrlList);	//attention
void Elmo_BG(u8 ID,u8 Mail_If2CtrlList);
void Elmo_ST(u8 ID , u8 Mail_If2CtrlList);
void Elmo_Ask_PX(u8 ID,u8 Mail_If2CtrlList);
void Elmo_Ask_VX(u8 ID,u8 Mail_If2CtrlList);
void Elmo_Ask_IQ(u8 ID,u8 Mail_If2CtrlList);
void Elmo_Control(u8 id);

#endif































