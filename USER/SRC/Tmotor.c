#include "Tmotor.h"	//AK���

TMotorTypedef Tmotor[9];	//AK�����

TmotorParamTypedef AK80G6Param;	//�������������������ٱȺͳ�����
TmotorLimitTypedef TmotorLimit;	//���Ʋ�����
MotionPARMTypedef ContinousLoadPARM;

void Tmotor_Init(void)	//�����ʼ��
{
	AK80G6Param.angle2RAD = 0.017453293f ;		//�Ƕȵ�����
	AK80G6Param.RATIO = 7.6f;		//AK80-6�Դ����ٱ�
	AK80G6Param.GearRadio = 1.0f;		//�������ٱ�
	
	TmotorLimit.ifReleaseWhenStuck = true; //��תʱ�ͷ�
	TmotorLimit.If_StuckDect = true;	   //��ת���

	TmotorLimit.MaxPosSP = 60; //��λ��/s�����λ���ٶ�
	TmotorLimit.MinPosSP = 10; //��Сλ���ٶ�

	Tmotor[TmotorID].param = AK80G6Param; // AK80��IDΪ1
	Tmotor[TmotorID].limit = TmotorLimit; //��������
	Tmotor[TmotorID].limit.MaxAng = 1800;	  //���Ƕ�
	Tmotor[TmotorID].limit.MinAng = -1800; //��С�Ƕ�

	Tmotor[TmotorID].valueSet.kp_BG = 150; //��ʼʱ�ı���
	Tmotor[TmotorID].valueSet.kd_BG = 3;   //��ʼ�˶�ʱ��΢��
	Tmotor[TmotorID].valueSet.kp_ST = 180; //ֹͣʱ�ı���pid
	Tmotor[TmotorID].valueSet.kd_ST = 1.5; //ֹͣ�˶�ʱ��΢��
	Tmotor[TmotorID].valueSet.Acc = 0;
	Tmotor[TmotorID].valueSet.AccRange = 0;
	Tmotor[TmotorID].valueSet.Dcc = 0;
	Tmotor[TmotorID].valueSet.DccRange = 0;
	Tmotor[TmotorID].valueSet.SPStart = 0;
	Tmotor[TmotorID].valueSet.LockRange = 1.5;
}



void Tmotor_Comm_Init(CAN_TypeDef *CANx, u8 ID)	//ͨѶ��ʼ��
{
	CanTxMsg tx_message;
	tx_message.StdId = 0x00 + ID;
	tx_message.RTR = CAN_RTR_Data;	  //����֡
	tx_message.IDE = CAN_Id_Standard; //��׼֡
	tx_message.DLC = 8;
	tx_message.Data[0] = 0xFF;
	tx_message.Data[1] = 0xFF;
	tx_message.Data[2] = 0xFF;
	tx_message.Data[3] = 0xFF;

	tx_message.Data[4] = 0xFF;
	tx_message.Data[5] = 0xFF;
	tx_message.Data[6] = 0xFF;
	tx_message.Data[7] = 0xFC;
	CAN_Transmit(CANx, &tx_message); //����
}




u16 float2uint(float x, float x_min, float x_max, u8 bits)	//�����ͻ�Ϊ����
{
	float span = x_max - x_min;
	float offset = x_min;

	return (u16)((x - offset) * ((float)((1 << bits) - 1)) / span);
}
float uint2float(int x_int, float x_min, float x_max, int bits)	//���ͻ�Ϊ������
{
	float span = x_max - x_min;
	float offset = x_min;
	return ((float)x_int) * span / ((float)((1 << bits) - 1)) + offset;
}
void Tmotor_Control(u8 ID, float f_pos, float f_vel, float f_kp, float f_kd, float f_torque, bool Mail_If2CtrlList)	//�����Ʋ�������������൱�ڴ󽮵�DJ_CurrentTransmit
																																																										//���øú����ֱ�ʵ��λ�ÿ��ƻ��ٶȿ���
{
	u16 p, v, kp, kd, t;
	f_pos *= Tmotor[ID].param.angle2RAD * Tmotor[ID].param.GearRadio * Tmotor[ID].param.RATIO;
	f_vel *= Tmotor[ID].param.angle2RAD * Tmotor[ID].param.GearRadio * Tmotor[ID].param.RATIO;
	/* ����Э�飬��float��������ת�� */
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
	CAN2_SendQueue.Rear = IncCAN2Rear; //��ӣ���β��1
}



void Tmotor_SetZZero(u8 ID, bool Mail_If2CtrlList)	//���õ�ǰλ��Ϊ��λ��
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
	CAN2_SendQueue.Rear = IncCAN2Rear; //��ӣ���β��1
}



void TmotorPosMode(u8 ID, bool Mail_If2CtrlList)	//λ���ٶ�ģʽ
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

			if (ABS(Tmotor[ID].argum.AngStart - Tmotor[ID].argum.AngCal) < Tmotor[ID].valueSet.AccRange) //���ٽ׶�
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

		//ͨ�����ϸı�λ�ðѽǶ�����ȥ
		Tmotor[ID].argum.AngCal += (PlusOrMinus(Tmotor[ID].valueSet.angle - Tmotor[ID].valueReal.angle) * Tmotor[ID].argum.VelCal / (TIM3_Cycle * 1000));

		/*��ֹ����*/
		// �Ҹо����Ӧ��û�ð�
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

		//�ĳ��жϼ���ֵ֮�󣬲����ڳ�����������
		if (ABS(Tmotor[ID].valueSet.angle - Tmotor[ID].argum.AngCal) < Tmotor[ID].valueSet.LockRange)
		{
			Tmotor[ID].TimeUse = Tclk - Tmotor[ID].argum.TimeBG;
			TMD.AKStatus = 2;
		}

		break;
	}

	case 2:
	{
		// ��λ��ʱ��������p��dС�Ĳ��� ��֪���᲻�����
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

	if (Tmotor[TmotorID].enable) //�������ϵ�,Ϊɶֻ��0��1��
	{
		//			if(Tmotor[8].limit.If_TimeoutDect) {DJ_ifTimeOut();}
		//			if(Tmotor[8].limit.If_StuckDect)	{DJ_ifStuck(i);}
		switch (Tmotor[TmotorID].mode)
		{
		case TPos:	//λ��ģʽ��λ��ȡ����valueSet.angle
			Tmotor_Control(TmotorID, Tmotor[TmotorID].valueSet.angle, 0, Tmotor[TmotorID].valueSet.kp_BG, Tmotor[TmotorID].valueSet.kd_BG, Tmotor[TmotorID].valueSet.torque, 0);
			break;
		case TVel:	//�ٶ�ģʽ���ٶ�ȡ����valueSet.speed
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



void TmotorReceiveHandler(CanRxMsg rx_message)	//����������Ĳ���������õ��������ʵ����valueReal
{
	int id = rx_message.Data[0];
	u16 p_int = (rx_message.Data[1] << 8) | rx_message.Data[2];		   //���λ��
	u16 v_int = (rx_message.Data[3] << 4) | (rx_message.Data[4] >> 4); //����ٶ�
	u16 t_int = (rx_message.Data[4] & 0x0f) << 8 | rx_message.Data[5]; //���Ť��

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
		.MO1 = TmotorEnable,  //ʹ��
		.MO0 = TmotorDisable, //ʧ��
		.BG = TmotorBG,				//����
		.ST = TmotorST,				//�ƶ�
		.PA = TmotorPA,				//ת��
		.UM = TmotorUM,				//�˶�ģʽ
		.SP = TmotorSP,				//�ٶ�
		.PX0 = TmotorSetZero,	//���õ�ǰλ��Ϊ��λ��
		.AK80_MotionPlus = AK80MotionPlus,																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																													
};


void TmotorComInit(u8 ID)	//ͨѶ��ʼ��
{
	Tmotor_Comm_Init(CAN2, ID); // ʹ��CAN2����
}

void TmotorEnable(u8 ID)
{
	TMD.AKStatus = 0; //ʹ�ܲ��Ҷ�
	Tmotor[ID].enable = 1;
	Tmotor[ID].argum.AngCal = Tmotor[ID].valueReal.angle; //�����м������
}

void TmotorDisable(u8 ID)
{
	Tmotor[ID].enable = 0;
}


void TmotorBG(u8 ID)	//��ʼ�˶�
{
	Tmotor[ID].limit.MaxPosSP = Tmotor[ID].argum.SPTemp; //�޶��ٶ�
	TMD.AKStatus = 1;
	Tmotor[ID].valueSet.angle = Tmotor[ID].argum.AngBG;		//Ŀ��Ƕ�
	Tmotor[ID].argum.AngStart = Tmotor[ID].valueReal.angle; //��¼��ʼ�˶��ĽǶ�
	Tmotor[ID].argum.TimeBG = Tclk;							//��¼��ʼ�˶���ʱ��
	Tmotor[ID].argum.VelCal = Tmotor[ID].valueSet.SPStart;	//���ٶ�
}

//ֹͣ�˶����ƶ����
void TmotorST(u8 ID)
{
	Tmotor[ID].valueSet.angle = Tmotor[ID].valueReal.angle;
	TMD.AKStatus = 0;
}

//�趨λ��ģʽ��ת���ĽǶ�
void TmotorPA(u8 ID, float Ang)
{
	if (Ang > Tmotor[ID].limit.MaxAng)
		Ang = Tmotor[ID].limit.MaxAng;
	if (Ang < Tmotor[ID].limit.MinAng)
		Ang = Tmotor[ID].limit.MinAng;

	Tmotor[ID].argum.AngBG = Ang;
}

//ѡ���˶�ģʽ
void TmotorUM(u8 ID, u8 mode)
{
	Tmotor[ID].mode = mode;
}

//����λ��ģʽ�µ�ת��
void TmotorSP(u8 ID, float PosSP)
{
	if (PosSP < 0)
	{
		PosSP = 0;
	}
	if (PosSP > 2500) // 45.0f Rad/s = 2578 ��/s
	{
		PosSP = 2500;
	}
	Tmotor[ID].argum.SPTemp = PosSP;
}

// PX0����ѯ�����λ�ã���λ������
void TmotorSetZero(u8 ID)
{
	Tmotor_SetZZero(ID, 0);
}

void AK80MotionPlus(MotionPARMTypedef MotionPARM)
{
	Tmotor[TmotorID].valueSet.kp_BG = MotionPARM.kp;		  // Kp
	Tmotor[TmotorID].valueSet.kd_BG = MotionPARM.kd;		  // Kd
	Tmotor[TmotorID].valueSet.torque = MotionPARM.torque;	  //Ť��
	Tmotor[TmotorID].valueSet.SPStart = MotionPARM.SPStart;	  //���ٶ�
	Tmotor[TmotorID].valueSet.Acc = MotionPARM.Acc;			  //���ٶ�
	Tmotor[TmotorID].valueSet.AccRange = MotionPARM.AccRange; //���پ���
	Tmotor[TmotorID].valueSet.Dcc = MotionPARM.Dcc;			  //���ٶ�
	Tmotor[TmotorID].valueSet.DccRange = MotionPARM.DccRange; //���پ���
	TMD.SP(TmotorID, MotionPARM.MaxSP);						  //���ٵ�����ٶ�
	Tmotor[TmotorID].limit.MinPosSP = MotionPARM.MinSP;		  //���ٵ���С�ٶ�
	TMD.PA(TmotorID, MotionPARM.Angdest);					  //Ŀ��λ��
	TMD.BG(TmotorID);										  //��ʼ�˶�
}
