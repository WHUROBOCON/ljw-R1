#include "param.h"

Usart_Struct usart;
FlagTypeDef SystemFlag;
LEDTaskTypedef LedTask;

BeepStructTypedef  B1p;

float start_t;

void param_Init(void)
{
	SystemFlag.Can1MsgCtrllistEnable=1; SystemFlag.Can2MsgCtrllistEnable=0;
	
	Led8DisData(0);
	LedTask.Normal=1;
	B1p.AllowFlag=false;
	
		
	DJmotor_Init();
	VESC_Init();
	Zdrive_Init();
	Tmotor_Init();
	
	TR_PARM_Init();
}

void TR_PARM_Init(void)
{
	//取环
	TRParm.Rotate.Rotate_Pos_Behind = 180;
	TRParm.Rotate.Rotate_Pos_Left = - 90;
	TRParm.Rotate.Rotate_Pos_Right = 90;
	TRParm.Rotate.Rotate_SP = 2160;
	TRParm.Rotate.Return_Pos = 2160;
	TRParm.Rotate.Return_SP = 2160;
	TRParm.Rotate.Turnzero_SP = 2160;
	TRParm.Rotate.Turnpick_SP = 2160;
	TRParm.Rotate.Fire_Pos = 3000; 
	TRParm.Rotate.Fire_SP = 200;
	
	//AK一圈上升12mm，
	TRParm.Lift.Pick_Pos = 2000;      //捡环时的位置
  TRParm.Lift.Pick_SP = 200;         //到达捡环位置的速度
	TRParm.Lift.Takering_Pos = 1555;
	TRParm.Lift.Takering_SP = 2000;
	TRParm.Lift.Load_Pos = 350; 
	TRParm.Lift.Load_SP = 800;
	TRParm.Lift.Normal_Pos = 1400;        
	TRParm.Lift.Normal_SP = 2500;
	TRParm.Lift.Back_Pos = 5;       
	TRParm.Lift.Back_SP = 2000;
	TRParm.Lift.Liftzero_SP = -2000;

	//门槛
	TRParm.Thresold.TurnAngle = 2.0; // 2.5
	TRParm.Thresold.LiftPos = 2.0;
	TRParm.Thresold.Pitch_Thresold=0.5f;
	TRParm.Thresold.Yaw_Thresold=0.5f;
	TRParm.Thresold.VESCSP_Thresold = 10;
	
	mech.FLAG.LiftUpDownFlag=0;
	mech.FLAG.Lift_State=9;
	//发射
}
