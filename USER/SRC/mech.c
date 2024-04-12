#include "mech.h"
TRParamTypedef TRParm;

//???? 
//????
void DJMotion(int ID,float ang);//2006??
void DJZero(int ID,s16 PosSp);//2006??
void DJSetPosSP(int ID,s16 PosSP);//2006????????

//??
void DisableALLFire(void);//????
void EnableALLFire(void);//????			
void BrakeALLFire(void);   //????
void SetYawAng(CanRxMsg rx_message);//????????
void SetPitchAng(CanRxMsg rx_message);//????????
void SetPitchPosSP(CanRxMsg rx_message);//????????
void SetYawPosSP(CanRxMsg rx_message);//????????
void SetWheelSP(CanRxMsg rx_message);//?????????

//??
void DisableALLCatch(void); //????
void EnableALLCatch(void);  //????
void BrakeALLCatch(void);   //????
void LiftRPM(int flag);//????????
void LiftPOS(int flag);   //????????(??????)
void SetRotatePosSP(CanRxMsg rx_message);//????????
void SetRotateAng(CanRxMsg rx_message); //????????
//??
void AnswerPitchAng(void);//???????
void AnswerYawAng(void);//???????
void AnswerWheelSP(void);//?????????
void AnswerLiftPos(void); //????????
void AnswerRotateAng(void); //??????????
void TakeDone(void);//???????? 
void LiftDone(void);

MechaStruct mech =
{
	/*******????********/
	//????
	.FUNC.DJ_Motion = DJMotion,
	.FUNC.DJ_Zero = DJZero,
	.FUNC.DJ_SetPosSP=DJSetPosSP,
	/*****?????******/
	//??
	.fire.DisableALLFire = DisableALLFire,
	.fire.EnableALLFire = EnableALLFire,
	.fire.BrakeALLFire = BrakeALLFire,
	.fire.Set_Yaw_Ang=SetYawAng,
	.fire.Set_Yaw_PosSP= SetYawPosSP,
	.fire.Set_Pitch_Ang=SetPitchAng,
	.fire.Set_Pitch_PosSP=SetPitchPosSP,
	.fire.Set_Wheel_SP=SetWheelSP,
	//??
	.take.DisableALLCatch = DisableALLCatch,
	.take.EnableALLCatch = EnableALLCatch,
	.take.BrakeALLCatch = BrakeALLCatch,
	.take.Lift_RPM = LiftRPM,
	.take.Lift_POS = LiftPOS,
	.take.Set_Rotate_PosSP = SetRotatePosSP,
	.take.Set_Rotate_Ang = SetRotateAng,
	//??(????)
		//????
	.FDBC.Answer_PitchAng = AnswerPitchAng,//??????
	.FDBC.Answer_YawAng = AnswerYawAng,//??????
	.FDBC.Answer_WheelSP = AnswerWheelSP,//?????????
		//????
	.FDBC.Answer_Lift_Pos = AnswerLiftPos,//????????
	.FDBC.Answer_Rotate_Ang = AnswerRotateAng,//????????
	.FDBC.Take_Done = TakeDone,   //????????
	.FDBC.Lift_Done=LiftDone,
};

/********************************************????***************************************************/
/***********????***********/

void DJMotion(int ID,float ang)	//??????????M2006???????????
{
	DJmotor[ID].mode = DJPOSITION;	DJmotor[ID].begin=1;
	DJmotor[ID].valueSet.angle = ang;
}

//2006??
void DJZero(int ID,s16 PosSp)
{
	DJmotor[ID].Limit.ZeroSP = PosSp;
	DJmotor[ID].mode = DJZERO;
	DJmotor[ID].valueSet.angle = 0;//?????????0,????????????
}

void DJSetPosSP(int ID,s16 PosSP)
{
	DJmotor[ID].Limit.PosSPLimit=PosSP;
}
/*****************??*********************/

/**********************??********************/

/************************************************????************************************************/
/***************??************************/
void DisableALLFire(void)	//??
{
	PitchMotor.enable = 0;
	YawMotor.enable = 0;				
	UpWheelMotor.enable = 0;		UpWheelMotor.valueSet.speed = 0;
	DownWheelMotor.enable = 0;		DownWheelMotor.valueSet.speed = 0;
}

void EnableALLFire(void)	//??
{
	PitchMotor.enable = 1;				PitchMotor.begin = 1;
	YawMotor.enable = 1;					YawMotor.begin = 1;
	UpWheelMotor.enable = 1;			UpWheelMotor.valueSet.speed = 0;
	DownWheelMotor.enable = 1;		DownWheelMotor.valueSet.speed = 0;
}

void BrakeALLFire(void)  //??
{
	PitchMotor.begin = 0;
	YawMotor.begin = 0;
	UpWheelMotor.enable = 0;		UpWheelMotor.valueSet.speed = 0;
	DownWheelMotor.enable = 0;			DownWheelMotor.valueSet.speed = 0;
}

void SetYawAng(CanRxMsg rx_message)	//??????
{
	s32 temp;
  DecodeS32Data(&temp, &rx_message.Data[1]);                  //????????????????temp
  mech.FUNC.DJ_Motion(YawID,temp / 100.0f);
}	

void SetYawPosSP(CanRxMsg rx_message)	//????????	
{
	s32 temp;
	DecodeS32Data(&temp, &rx_message.Data[1]);                  //????????????????temp
	mech.FUNC.DJ_SetPosSP(YawID,temp); 
}

void SetPitchAng(CanRxMsg rx_message)//??????
{
	s32 temp;
  DecodeS32Data(&temp, &rx_message.Data[1]);                  //????????????????temp
  mech.FUNC.DJ_Motion(PitchID,temp / 100.0f); //??2006???,?temp/100?
}
	
void SetPitchPosSP(CanRxMsg rx_message)//????????	
{
	s32 temp;
	DecodeS32Data(&temp, &rx_message.Data[1]);                  //????????????????temp
	mech.FUNC.DJ_SetPosSP(PitchID,temp); 
}	

void SetWheelSP(CanRxMsg rx_message)	//?????????	
{
	u16 tempU,tempD;
	DecodeU16Data(&tempU,&rx_message.Data[1]);
	DecodeU16Data(&tempD,&rx_message.Data[3]);
	UpWheelMotor.mode=VESCRPM;	DownWheelMotor.mode=VESCRPM;
	UpWheelMotor.valueSet.speed=-tempU;	DownWheelMotor.valueSet.speed=-tempD;		//????
}

/****************??*********************/
void DisableALLCatch(void)	//????
{
	RotateM.enable = 0;
  LiftM.enable=0;
}

void EnableALLCatch(void)	//????
{
  RotateM.enable = 1;		RotateM.begin = 1;
  LiftM.enable=1;
}

void BrakeALLCatch(void)	//????
{
	RotateM.begin = 0;
  LiftM.valueSet.speed=0;
}

void SetRotateAng(CanRxMsg rx_message)	//??2006??
{
	s32 temp;
	DecodeS32Data(&temp, &rx_message.Data[1]);                  //????????????????temp
	mech.FUNC.DJ_Motion(RotateID,temp / 100.0f); //??2006???,?temp/100?
}

void SetRotatePosSP(CanRxMsg rx_message)	//????????
{
	u16 temp;
	DecodeS32Data(&temp, &rx_message.Data[1]);                  //????????????????temp
	mech.FUNC.DJ_SetPosSP(RotateID,temp);
}

void LiftPOS(int flag)	//??????
{
	LiftM.mode=VESCPOSITION;
	if(flag==1)
		LiftM.valueSet.angle=0;
	else if(flag==0)
		LiftM.valueSet.angle=20;
	else if(flag==2)
		LiftM.valueSet.angle=140;
	else if(flag==3)
		LiftM.valueSet.angle=0;
	
	while(ABS(LiftM.valueSet.angle-LiftM.valueReal.angle)>1)
		OSTimeDly(50);
	
	mech.FLAG.Lift_State=flag;
}

void LiftRPM(int flag)	//1ÉÏÉý£¬0ÏÂ½µ
{
	mech.FLAG.Lift_State=9;
	LiftM.mode=VESCRPM;
	if(flag==1)
	{
		LiftM.valueSet.speed=-5000;		
	}
	else if(flag==0)
	{
		LiftM.valueSet.speed=5000;		
	}

}
/**************************************************??**************************************************/
/********************??*********************/
void AnswerPitchAng(void)	//????????
{
	s32 temp = -PitchMotor.valueReal.angle * 100;	//????
	if(IncCAN1Rear==CAN1_SendQueue.Front)
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = ZhuKong_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 5;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 0x40+40;
		EncodeS32Data(&temp,&CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1]);		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;
}

void AnswerYawAng(void)//????????
{
	s32 temp = -YawMotor.valueReal.angle * 100;	//????
	if(IncCAN1Rear==CAN1_SendQueue.Front)
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = ZhuKong_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 5;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 0x40+41;
		EncodeS32Data(&temp,&CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1]);	
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;
}

void AnswerWheelSP(void)	//?????????
{
	u16 tempU = -UpWheelMotor.valueReal.speed;
	u16 tempD = -DownWheelMotor.valueReal.speed;
	if(IncCAN1Rear==CAN1_SendQueue.Front)	//IncCAN2Rear = Rear+1
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = ZhuKong_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 5;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 0x40+42;
		EncodeU16Data(&tempU,&CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1]);	
		EncodeU16Data(&tempD,&CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3]);	
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;
}

/********************??*********************/
void AnswerLiftPos(void)	//??????????(AK)
{
	s32 temp = LiftM.valueReal.angle / 300.0f;
	if (IncCAN1Rear == CAN1_SendQueue.Front) // IncCAN2Rear = Rear+1
	{
		SystemFlag.Can1SendqueueFULL++;
		return;
	}
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = ZhuKong_TX; // ???????
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 5;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 0x40 + 43;
		EncodeS32Data(&temp, &CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1]);
	}
	CAN1_SendQueue.Rear = IncCAN1Rear;
}

void AnswerRotateAng(void)	//????????(??2006)
{
	s32 temp = RotateM.valueReal.angle * 100;
	if (IncCAN1Rear == CAN1_SendQueue.Front) // IncCAN2Rear = Rear+1
	{
		SystemFlag.Can1SendqueueFULL++;
		return;
	}
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = ZhuKong_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 5;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 0x40+44;
		EncodeS32Data(&temp, &CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1]);
	}
	CAN1_SendQueue.Rear = IncCAN1Rear;
}

void TakeDone(void)	//?????,?????
{
	if (IncCAN1Rear == CAN1_SendQueue.Front) // IncCAN2Rear = Rear+1
	{
		SystemFlag.Can1SendqueueFULL++;
		return;
	}
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = ZhuKong_TX; 
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 2;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 0x40 + 45;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = mech.FLAG.Paw_State;//????,?1???,?0???
	}
	CAN1_SendQueue.Rear = IncCAN1Rear;
}

void LiftDone(void)	//?????,?????
{
	if (IncCAN1Rear == CAN1_SendQueue.Front) // IncCAN2Rear = Rear+1
	{
		SystemFlag.Can1SendqueueFULL++;
		return;
	}
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = ZhuKong_TX; 
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 2;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 0x40 + 46;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = mech.FLAG.Lift_State;//????,?1???,?0???
	}
	CAN1_SendQueue.Rear = IncCAN1Rear;
}

