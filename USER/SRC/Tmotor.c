#include "Tmotor.h"	//AK电机

TMotorTypedef Tmotor[9];	//AK电机类

TmotorParamTypedef AK80G6Param;	//电机种类参数，包含减速比和齿数比
TmotorLimitTypedef TmotorLimit;	//限制参数，
MotionPARMTypedef ContinousLoadPARM;

void Tmotor_Init(void)	//电机初始化
{
	AK80G6Param.angle2RAD = 0.017453293f ;		//角度到弧度
	AK80G6Param.RATIO = 7.6f;		//AK80-6自带减速比
	AK80G6Param.GearRadio = 1.0f;		//机构减速比
	
	TmotorLimit.ifReleaseWhenStuck = true; //堵转时释放
	TmotorLimit.If_StuckDect = true;	   //堵转检测

	TmotorLimit.MaxPosSP = 60; //单位°/s，最大位置速度
	TmotorLimit.MinPosSP = 10; //最小位置速度

	Tmotor[TmotorID].param = AK80G6Param; // AK80，ID为1
	Tmotor[TmotorID].limit = TmotorLimit; //限制条件
	Tmotor[TmotorID].limit.MaxAng = 1800;	  //最大角度
	Tmotor[TmotorID].limit.MinAng = -1800; //最小角度

	Tmotor[TmotorID].valueSet.kp_BG = 150; //开始时的比例
	Tmotor[TmotorID].valueSet.kd_BG = 3;   //开始运动时的微分
	Tmotor[TmotorID].valueSet.kp_ST = 180; //停止时的比例pid
	Tmotor[TmotorID].valueSet.kd_ST = 1.5; //停止运动时的微分
	Tmotor[TmotorID].valueSet.Acc = 0;
	Tmotor[TmotorID].valueSet.AccRange = 0;
	Tmotor[TmotorID].valueSet.Dcc = 0;
	Tmotor[TmotorID].valueSet.DccRange = 0;
	Tmotor[TmotorID].valueSet.SPStart = 0;
	Tmotor[TmotorID].valueSet.LockRange = 1.5;
}



void Tmotor_Comm_Init(CAN_TypeDef *CANx, u8 ID)	//通讯初始化
{
	CanTxMsg tx_message;
	tx_message.StdId = 0x00 + ID;
	tx_message.RTR = CAN_RTR_Data;	  //数据帧
	tx_message.IDE = CAN_Id_Standard; //标准帧
	tx_message.DLC = 8;
	tx_message.Data[0] = 0xFF;
	tx_message.Data[1] = 0xFF;
	tx_message.Data[2] = 0xFF;
	tx_message.Data[3] = 0xFF;

	tx_message.Data[4] = 0xFF;
	tx_message.Data[5] = 0xFF;
	tx_message.Data[6] = 0xFF;
	tx_message.Data[7] = 0xFC;
	CAN_Transmit(CANx, &tx_message); //发送
}




u16 float2uint(float x, float x_min, float x_max, u8 bits)	//浮点型化为整型
{
	float span = x_max - x_min;
	float offset = x_min;

	return (u16)((x - offset) * ((float)((1 << bits) - 1)) / span);
}
float uint2float(int x_int, float x_min, float x_max, int bits)	//整型化为浮点型
{
	float span = x_max - x_min;
	float offset = x_min;
	return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
}
void Tmotor_Control(u8 ID, float f_pos, float f_vel, float f_kp, float f_kd, float f_torque, bool Mail_If2CtrlList)	//将控制参数传给电机，相当于大疆的DJ_CurrentTransmit
																																																										//利用该函数分别实现位置控制或速度控制
{
	u16 p, v, kp, kd, t;
	f_pos *= Tmotor[ID].param.angle2RAD * Tmotor[ID].param.GearRadio * Tmotor[ID].param.RATIO;
	f_vel *= Tmotor[ID].param.angle2RAD * Tmotor[ID].param.GearRadio * Tmotor[ID].param.RATIO;
	/* 根据协议，对float参数进行转换 */
	p = float2uint(f_pos, P_MIN, P_MAX, 16);
	v = float2uint(f_vel, V_MIN, V_MAX, 12);
	kp = float2uint(f_kp, KP_MIN, KP_MAX, 12);
	kd = float2uint(f_kd, KD_MIN, KD_MAX, 12);
	t = float2uint(f_torque, T_MIN, T_MAX, 12);

	if (IncCAN2Rear == CAN2_SendQueue.Front) // IncCAN2Rear = Rear+1
	{
		SystemFlag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID = 0x00 + ID;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC = 0x08;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0] = p >> 8;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1] = p & 0xFF;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2] = v >> 4;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3] = ((v & 0xF) << 4) | (kp >> 8);
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4] = kp & 0xFF;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[5] = kd >> 4;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[6] = ((kd & 0xF) << 4) | (t >> 8);
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[7] = t & 0xff;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	CAN2_SendQueue.Rear = IncCAN2Rear; //入队，队尾加1
}



void Tmotor_SetZZero(u8 ID, bool Mail_If2CtrlList)	//设置当前位置为零位置
{
	if (IncCAN2Rear == CAN2_SendQueue.Front) // IncCAN2Rear = Rear+1
	{
		SystemFlag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID = 0x00 + ID;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC = 0x08;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0] = 0xFF;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1] = 0xFF;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2] = 0xFF;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3] = 0xFF;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4] = 0xFF;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[5] = 0xFF;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[6] = 0xFF;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[7] = 0xFE;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	CAN2_SendQueue.Rear = IncCAN2Rear; //入队，队尾加1
}



void TmotorPosMode(u8 ID, bool Mail_If2CtrlList)	//位置速度模式
{
	switch (TMD.AKStatus)
	{

	case 0:
	{
		Tmotor[ID].valueSet.angle = Tmotor[ID].valueReal.angle;
		TMD.AKStatus = 2;
		break;
	}

	case 1:
	{
		if (ABS(Tmotor[ID].valueSet.angle - Tmotor[ID].argum.AngStart) > (Tmotor[ID].valueSet.AccRange + Tmotor[ID].valueSet.DccRange + 5))
		{

			Tmotor[ID].argum.TimeNOW = Tclk;

			if (ABS(Tmotor[ID].argum.AngStart - Tmotor[ID].argum.AngCal) < Tmotor[ID].valueSet.AccRange) //加速阶段
			{
				if (Tmotor[ID].argum.VelCal < Tmotor[ID].limit.MaxPosSP)
				{
					Tmotor[ID].argum.VelCal += Tmotor[ID].valueSet.Acc * (Tmotor[ID].argum.TimeNOW - Tmotor[ID].argum.TimeBG); // v = a*t
					if (Tmotor[ID].argum.VelCal >= Tmotor[ID].limit.MaxPosSP)
						Tmotor[ID].argum.VelCal = Tmotor[ID].limit.MaxPosSP;
				}
			}
			else
				Tmotor[ID].argum.VelCal = Tmotor[ID].limit.MaxPosSP;

			if (ABS(Tmotor[ID].valueSet.angle - Tmotor[ID].argum.AngCal) < Tmotor[ID].valueSet.DccRange)
			{
				if (Tmotor[ID].argum.VelCal > Tmotor[ID].limit.MinPosSP)
				{
					Tmotor[ID].DeCnt++;
					Tmotor[ID].argum.VelCal = Tmotor[ID].limit.MaxPosSP - (Tmotor[ID].valueSet.Dcc * (Tmotor[ID].argum.TimeNOW - Tmotor[ID].argum.TimeBG));
					if (Tmotor[ID].argum.VelCal <= Tmotor[ID].limit.MinPosSP)
						Tmotor[ID].argum.VelCal = Tmotor[ID].limit.MinPosSP;
				}
			}
		}
		else
			Tmotor[ID].argum.VelCal = Tmotor[ID].limit.MaxPosSP;

		//通过不断改变位置把角度拉过去
		Tmotor[ID].argum.AngCal += (PlusOrMinus(Tmotor[ID].valueSet.angle - Tmotor[ID].valueReal.angle) * Tmotor[ID].argum.VelCal / (TIM3_Cycle * 1000));

		/*防止超调*/
		// 我感觉这段应该没用啊
		if (Tmotor[ID].argum.AngStart < Tmotor[ID].valueSet.angle)
		{
			if (Tmotor[ID].argum.AngCal > Tmotor[ID].valueSet.angle)
				Tmotor[ID].argum.AngCal = Tmotor[ID].valueSet.angle;
		}
		if (Tmotor[ID].argum.AngStart > Tmotor[ID].valueSet.angle)
		{
			if (Tmotor[ID].argum.AngCal < Tmotor[ID].valueSet.angle)
				Tmotor[ID].argum.AngCal = Tmotor[ID].valueSet.angle;
		}

		Tmotor_Control(ID, Tmotor[ID].argum.AngCal, 0, Tmotor[ID].valueSet.kp_BG, Tmotor[ID].valueSet.kd_BG, Tmotor[ID].valueSet.torque, Mail_If2CtrlList);

		//改成判断计算值之后，不存在超调的现象了
		if (ABS(Tmotor[ID].valueSet.angle - Tmotor[ID].argum.AngCal) < Tmotor[ID].valueSet.LockRange)
		{
			Tmotor[ID].TimeUse = Tclk - Tmotor[ID].argum.TimeBG;
			TMD.AKStatus = 2;
		}

		break;
	}

	case 2:
	{
		// 到位的时候用这套p大d小的参数 不知道会不会过冲
		//	Tmotor_Control(ID, Tmotor[ID].valueSet.angle, 0, Tmotor[ID].valueSet.kp_ST, Tmotor[ID].valueSet.kd_ST, Tmotor[ID].valueSet.torque, Mail_If2CtrlList);
		Tmotor_Control(ID, Tmotor[ID].valueSet.angle, 0, Tmotor[ID].valueSet.kp_BG, Tmotor[ID].valueSet.kd_BG, Tmotor[ID].valueSet.torque, Mail_If2CtrlList);
		break;
	}

	default:
		Tmotor_Control(TmotorID, 0, 0, 0, 0, 0, 0);
		break;
	}
}



void TmotorFunc(void)
{
	if (Tmotor[TmotorID].SetZero)
	{
		Tmotor_SetZZero(TmotorID, 0);
		Tmotor[TmotorID].SetZero = 0;
	}

	if (Tmotor[TmotorID].enable) //在这里打断点,为啥只有0和1？
	{
		//			if(Tmotor[8].limit.If_TimeoutDect) {DJ_ifTimeOut();}
		//			if(Tmotor[8].limit.If_StuckDect)	{DJ_ifStuck(i);}
		switch (Tmotor[TmotorID].mode)
		{
		case TPos:	//位置模式。位置取决于valueSet.angle
			Tmotor_Control(TmotorID, Tmotor[TmotorID].valueSet.angle, 0, Tmotor[TmotorID].valueSet.kp_BG, Tmotor[TmotorID].valueSet.kd_BG, Tmotor[TmotorID].valueSet.torque, 0);
			break;
		case TVel:	//速度模式。速度取决于valueSet.speed
			Tmotor_Control(TmotorID, 0, Tmotor[TmotorID].valueSet.speed, 0, Tmotor[TmotorID].valueSet.kd_BG, 0, 0);
			break;
		case TPosVel:
			TmotorPosMode(TmotorID, 0);
			break;
		default:
			break;
		}
	}
	else
		Tmotor_Control(TmotorID, 0, 0, 0, 0, 0, 0);
}



void TmotorReceiveHandler(CanRxMsg rx_message)	//将电机传来的参数做处理得到电机的真实数据valueReal
{
	int id = rx_message.Data[0];
	u16 p_int = (rx_message.Data[1] << 8) | rx_message.Data[2];		   //电机位置
	u16 v_int = (rx_message.Data[3] << 4) | (rx_message.Data[4] >> 4); //电机速度
	u16 t_int = (rx_message.Data[4] & 0x0f) << 8 | rx_message.Data[5]; //电机扭矩

	float p = uint2float(p_int, P_MIN, P_MAX, 16);
	float v = uint2float(v_int, V_MIN, V_MAX, 12);
	Tmotor[id].valueReal.angle = (float)(p / Tmotor[id].param.angle2RAD / Tmotor[id].param.GearRadio / Tmotor[id].param.RATIO);
	Tmotor[id].valueReal.speed = (float)(v / Tmotor[id].param.angle2RAD / Tmotor[id].param.GearRadio / Tmotor[id].param.RATIO);
	Tmotor[id].valueReal.torque = uint2float(t_int, T_MIN, T_MAX, 12);
}



/**************************************************************************************************************************************/
void TmotorComInit(u8 ID);
void TmotorEnable(u8 ID);
void TmotorDisable(u8 ID);
void TmotorBG(u8 ID);
void TmotorST(u8 ID);
void TmotorPA(u8 ID, float Ang);
void TmotorUM(u8 ID, u8 mode);
void TmotorSP(u8 ID, float PosSP);
void TmotorSetZero(u8 ID);
void AK80MotionPlus(MotionPARMTypedef MotionPARM);

TMotorStruct TMD = // T Motor Driver
	{
		.ComInit = TmotorComInit,
		.MO1 = TmotorEnable,  //使能
		.MO0 = TmotorDisable, //失能
		.BG = TmotorBG,				//启动
		.ST = TmotorST,				//制动
		.PA = TmotorPA,				//转角
		.UM = TmotorUM,				//运动模式
		.SP = TmotorSP,				//速度
		.PX0 = TmotorSetZero,	//设置当前位置为零位置
		.AK80_MotionPlus = AK80MotionPlus,																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																													
};


void TmotorComInit(u8 ID)	//通讯初始化
{
	Tmotor_Comm_Init(CAN2, ID); // 使用CAN2发送
}

void TmotorEnable(u8 ID)
{
	TMD.AKStatus = 0; //使能不乱动
	Tmotor[ID].enable = 1;
	Tmotor[ID].argum.AngCal = Tmotor[ID].valueReal.angle; //计算中间变量！
}

void TmotorDisable(u8 ID)
{
	Tmotor[ID].enable = 0;
}


void TmotorBG(u8 ID)	//开始运动
{
	Tmotor[ID].limit.MaxPosSP = Tmotor[ID].argum.SPTemp; //限定速度
	TMD.AKStatus = 1;
	Tmotor[ID].valueSet.angle = Tmotor[ID].argum.AngBG;		//目标角度
	Tmotor[ID].argum.AngStart = Tmotor[ID].valueReal.angle; //纪录开始运动的角度
	Tmotor[ID].argum.TimeBG = Tclk;							//纪录开始运动的时间
	Tmotor[ID].argum.VelCal = Tmotor[ID].valueSet.SPStart;	//初速度
}

//停止运动，制动电机
void TmotorST(u8 ID)
{
	Tmotor[ID].valueSet.angle = Tmotor[ID].valueReal.angle;
	TMD.AKStatus = 0;
}

//设定位置模式下转动的角度
void TmotorPA(u8 ID, float Ang)
{
	if (Ang > Tmotor[ID].limit.MaxAng)
		Ang = Tmotor[ID].limit.MaxAng;
	if (Ang < Tmotor[ID].limit.MinAng)
		Ang = Tmotor[ID].limit.MinAng;

	Tmotor[ID].argum.AngBG = Ang;
}

//选择运动模式
void TmotorUM(u8 ID, u8 mode)
{
	Tmotor[ID].mode = mode;
}

//设置位置模式下的转速
void TmotorSP(u8 ID, float PosSP)
{
	if (PosSP < 0)
	{
		PosSP = 0;
	}
	if (PosSP > 2500) // 45.0f Rad/s = 2578 °/s
	{
		PosSP = 2500;
	}
	Tmotor[ID].argum.SPTemp = PosSP;
}

// PX0，查询电机主位置，主位置置零
void TmotorSetZero(u8 ID)
{
	Tmotor_SetZZero(ID, 0);
}

void AK80MotionPlus(MotionPARMTypedef MotionPARM)
{
	Tmotor[TmotorID].valueSet.kp_BG = MotionPARM.kp;		  // Kp
	Tmotor[TmotorID].valueSet.kd_BG = MotionPARM.kd;		  // Kd
	Tmotor[TmotorID].valueSet.torque = MotionPARM.torque;	  //扭矩
	Tmotor[TmotorID].valueSet.SPStart = MotionPARM.SPStart;	  //初速度
	Tmotor[TmotorID].valueSet.Acc = MotionPARM.Acc;			  //加速度
	Tmotor[TmotorID].valueSet.AccRange = MotionPARM.AccRange; //加速距离
	Tmotor[TmotorID].valueSet.Dcc = MotionPARM.Dcc;			  //减速度
	Tmotor[TmotorID].valueSet.DccRange = MotionPARM.DccRange; //减速距离
	TMD.SP(TmotorID, MotionPARM.MaxSP);						  //加速的最大速度
	Tmotor[TmotorID].limit.MinPosSP = MotionPARM.MinSP;		  //减速的最小速度
	TMD.PA(TmotorID, MotionPARM.Angdest);					  //目标位置
	TMD.BG(TmotorID);										  //开始运动
}
