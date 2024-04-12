#ifndef __VESC_H
#define __VESC_H

#include "queue.h"
#include "mathFunc.h"
#include "pid.h"

#define MCPWM_PID_TIME_K				0.001f	// Pid controller sample time in seconds
#define UTILS_LP_FAST(value, sample, filter_constant)	(value -= (filter_constant) * (value - (sample)))


enum VESC_Motor_Mode{VESCCURRENT,VESCRPM,VESCPOSITION,VESCDUTY,VESCHANDBREAK,VESCRPM_I};

typedef struct{
	u8 Motor_PolePairs;
}VESCParamTypedef;

typedef struct{
	float current;
	float speed;
	//float speed2;
	float angle;
	float duty;
	vs32 HandBreak_Current;
	vs32 Break_Current;
	float Angle_ABS;
}VESCValueTypedef;

typedef struct{
	bool timeout;
	bool stuck;
}VESCStatusTypedef;

typedef struct{
	bool Timeout_IfDect;
	bool Stuck_IfDect;
	bool IfReleaseWhenStuck;
	u16  MAX_Current;
}VESCLimitTypedef;

typedef struct{
	u32 lastRxTim;
	u32 timeoutTicks;
	u32 stuckCnt;
	
	vu16 angleNow;
	vu16 anglePrv;
	vs16 distance;
	vs32 position;
}VESCArgumTypedef;

typedef struct
{
  float kp;
  float ki;
  float kd;
	float targetPosition;//
  float currentPosition;
  float pidOutput;
  float error; 
	float lastError;
	float previousError;
  s32 P;
  s32 I;
	s32 D;
}VESC_PID_setTypeDef;

typedef struct{
	bool enable;
	bool begin;
	u8 mode;
	VESCParamTypedef Param;
	VESCValueTypedef valueSet,valueReal;
	VESCStatusTypedef	Status;
	VESCLimitTypedef limit;		
	VESCArgumTypedef Argum;		//[计算中间量]
	VESC_PID_setTypeDef S_Pid;	//[PID]
}VESCMotorTypedef;


/****VESC的can报文命令枚举体****/
typedef enum
{
	CAN_PACKET_SET_DUTY = 0,
	CAN_PACKET_SET_CURRENT,
	CAN_PACKET_SET_CURRENT_BRAKE,
	CAN_PACKET_SET_RPM,
	CAN_PACKET_SET_POS,
	CAN_PACKET_FILL_RX_BUFFER,
	CAN_PACKET_FILL_RX_BUFFER_LONG,
	CAN_PACKET_PROCESS_RX_BUFFER,
	CAN_PACKET_PROCESS_SHORT_BUFFER,
	CAN_PACKET_STATUS,
	CAN_PACKET_SET_CURRENT_REL,
	CAN_PACKET_SET_CURRENT_BRAKE_REL,
	CAN_PACKET_SET_CURRENT_HANDBRAKE,
	CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
	CAN_PACKET_STATUS_2,
	CAN_PACKET_STATUS_3,
	CAN_PACKET_STATUS_4,
	CAN_PACKET_PING,
	CAN_PACKET_PONG,
	CAN_PACKET_DETECT_APPLY_ALL_FOC,
	CAN_PACKET_DETECT_APPLY_ALL_FOC_RES,
	CAN_PACKET_CONF_CURRENT_LIMITS,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS,
	CAN_PACKET_CONF_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_STORE_CURRENT_LIMITS_IN,
	CAN_PACKET_CONF_FOC_ERPMS,
	CAN_PACKET_CONF_STORE_FOC_ERPMS,
	CAN_PACKET_STATUS_5
} CAN_PACKET_ID_Enum;




extern VESCMotorTypedef VESCmotor[5];
void VESC_Init(void);
void VESC_Set_Pos(u8 controller_ID, float pos, u8 Mail_If2CtrlList);
void VESC_Set_Duty_Cycle(u8 controller_ID, float duty_cycle, u8 Mail_If2CtrlList);
void VESC_Set_Speed(u8 controller_ID, float speed, u8 Mail_If2CtrlList);
void VESC_Set_Current(u8 controller_ID, float current, u8 Mail_If2CtrlList);
void VESC_Set_Brake_Current(u8 controller_ID, float brake_current, u8 Mail_If2CtrlList);
void VESC_Set_Handbrake_Current(u8 controller_ID, float handbrake_current, u8 Mail_If2CtrlList);
void VESC_IfTimeout(u8 id);
void VESC_IfStuck(u8 id);
void VESCFunc(void);
void VESCReceiveHandler(CanRxMsg rx_message);
void VESCPID_Init(VESC_PID_setTypeDef *pid, float kp, float ki, float kd, int32_t set);

void VESC_PID_Init(VESC_PID_setTypeDef* PID, float KP, float KI, float KD, float KD_Filter, float KS);

#endif
