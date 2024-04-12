#include "elmo.h"

ELMOParamTypedef U10_Param,EC_4P30_Param,U8_Param;
ELMOStatusTypedef ElmoStatus;
ELMOLimitTypedef ELMOLimit;
ElmoMotorTypedef ElmoMotor[5];
 
ElmoCtrlTypedef ElmoCtrl;

void ElmoMotor_Init(void)
{
	//编码器线数和机构齿数比需按照实际情况修改
	U10_Param.PulsePerRound = 16384;	
	U10_Param.RATIO = 1.0;
	U10_Param.GearRadio = 1.0;

	EC_4P30_Param.PulsePerRound = 4000;
	EC_4P30_Param.RATIO = 169.f/9;
	EC_4P30_Param.GearRadio = 1.0;
	
	U8_Param.PulsePerRound = 16384;
	U8_Param.RATIO = 1.0;
	U8_Param.GearRadio = 1.0;
	
	ELMOLimit.ifReleaseWhenStuck = 1;
	ELMOLimit.If_StuckDect=1;	
	ELMOLimit.If_TimeoutDect = 1;
	
	for(u8 i=0;i<=4;i++)
	{
		ElmoMotor[i].status = ElmoStatus;
	}

	//Elmo驱动器ID设置最小为1，即0x301，所以程序内的ElmoMotor[0]对应0x301
	ElmoMotor[0].param = EC_4P30_Param;		//摇杆EC
	ElmoMotor[1].param = U10_Param;
	ElmoMotor[2].param = U10_Param;
	ElmoMotor[3].param = U10_Param;
	ElmoMotor[4].param = U10_Param;
}

/*ELMO通信初始化*/
void ELMO_Comm_Init(CAN_TypeDef* CANx)	
{
	CanTxMsg tx_message;
	tx_message.StdId=0x000;
	tx_message.RTR = CAN_RTR_Data;		//数据帧
	tx_message.IDE = CAN_Id_Standard;	
	tx_message.DLC=8;
	tx_message.Data[0]=0x01;
	tx_message.Data[1]=0x00;
	tx_message.Data[2]=0x00;
	tx_message.Data[3]=0x00;
	
	tx_message.Data[4]=0x00;
	tx_message.Data[5]=0x00;
	tx_message.Data[6]=0x00;
	tx_message.Data[7]=0x00;
	CAN_Transmit(CANx,&tx_message);
}

void Elmo_Set_PosUM(u8 ID,u8 Mail_If2CtrlList)
{	
	if(IncCAN2Rear==CAN2_SendQueue.Front)	//IncCAN2Rear = Rear+1
	{
		SystemFlag.Can2SendqueueFULL ++;
		return;
	}
	else	
	{		
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID =0x301+ID;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC = 0x08;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]= 'U';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]= 'M';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4] = 5;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[5] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[6] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[7] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	CAN2_SendQueue.Rear = IncCAN2Rear;  	//入队，队尾加1
}

void Elmo_Set_SpUM(u8 ID,u8 Mail_If2CtrlList)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)	//IncCAN2Rear = Rear+1
	{
		SystemFlag.Can2SendqueueFULL ++;
		return;
	}
	else
	{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID = 0x301+ID;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC = 0x08;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0] = 'U';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1] = 'M';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4] = 2;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[5] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[6] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[7] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}

void Elmo_Set_MO1(u8 ID,u8 Mail_If2CtrlList)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)	//IncCAN2Rear = Rear+1
	{
		SystemFlag.Can2SendqueueFULL ++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID = 0x301 +ID;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC = 0x08;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0] = 'M';		//4D
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1] = 'O';		//4F
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4] = 1;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[5] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[6] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[7] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}

void Elmo_Set_MO0(u8 ID,u8 Mail_If2CtrlList)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)	//IncCAN2Rear = Rear+1
	{
		SystemFlag.Can2SendqueueFULL ++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID = 0x301 + ID;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC = 0x08;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0] = 'M';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1] = 'O';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[5] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[6] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[7] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}

void Elmo_Set_SP(u8 ID,s32 speed,u8 Mail_If2CtrlList)
{
	//Sp的单位为 cnt/s speed的单位为rpm  输入为r/min * cnt/r / 60 = cnt * s (输入为轴后速度)
	s32 Sp;
	Sp = ElmoMotor[ID].param.PulsePerRound  / 60 * speed * ElmoMotor[ID].param.RATIO; 
	if(IncCAN2Rear==CAN2_SendQueue.Front)	//IncCAN2Rear = Rear+1
	{
		SystemFlag.Can2SendqueueFULL ++;
		return;
	}
	else
	{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID =0x301+ID;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC =0X08;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0] = 'S';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1] = 'P';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3] = 0;
		EncodeS32Data(&Sp,&CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]);
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}

void Elmo_Set_JV(u8 ID,s32 speed,u8 Mail_If2CtrlList)
{
	s32 Jv;	//输入speed为轴前速度rpm，,Jv为发给驱动器的轴后转速cnt/s
	Jv = ElmoMotor[ID].param.PulsePerRound / 60 *  speed * ElmoMotor[ID].param.RATIO;
	if(IncCAN2Rear==CAN2_SendQueue.Front)	//IncCAN2Rear = Rear+1
	{
		SystemFlag.Can2SendqueueFULL ++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID = 0x301+ID;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC = 0x08;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0] = 'J';		//4A
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1] = 'V';		//56
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3] = 0;
		EncodeS32Data(&Jv,&CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]);
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList =  Mail_If2CtrlList;
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}

/*主位置*/
void Elmo_Set_PX(u8 ID,s32 angle, u8 Mail_If2CtrlList)
{		
	s32 Px;
	Px = ElmoMotor[ID].param.PulsePerRound / 360 * angle * ElmoMotor[ID].param.RATIO;		//u8转一圈是16384
	if(IncCAN2Rear==CAN2_SendQueue.Front)	//IncCAN2Rear = Rear+1
	{
		SystemFlag.Can2SendqueueFULL ++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID = 0x301+ID;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC = 0X08;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0] = 'P';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1] = 'X';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3] = 0;
		EncodeS32Data(&Px,&CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]);	
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}

void Elmo_Set_PA(u8 ID,s32 angle, u8 Mail_If2CtrlList)
{		// pa的单位是cnt angle的单位是° 所以cnt= °/360* cnt/min
	s32 Pa;
	Pa = ElmoMotor[ID].param.PulsePerRound / 360 * angle * ElmoMotor[ID].param.RATIO;		//u8转一圈是16384
	if(IncCAN2Rear==CAN2_SendQueue.Front)	//IncCAN2Rear = Rear+1
	{
		SystemFlag.Can2SendqueueFULL ++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID = 0x301+ID;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC = 0X08;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0] = 'P';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1] = 'A';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3] = 0;
		EncodeS32Data(&Pa,&CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]);	
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}

void Elmo_Set_PA_f(u8 ID,float angle, u8 Mail_If2CtrlList)
{		// pa的单位是cnt angle的单位是° 所以cnt= °/360* cnt/min
	s32 Pa;
	Pa = ElmoMotor[ID].param.PulsePerRound / 360.0f * angle * ElmoMotor[ID].param.RATIO;		//u8转一圈是16384

	if(IncCAN2Rear==CAN2_SendQueue.Front)	//IncCAN2Rear = Rear+1
	{
		SystemFlag.Can2SendqueueFULL ++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID = 0x301+ID;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC = 0X08;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0] = 'P';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1] = 'A';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3] = 0;
		EncodeS32Data(&Pa,&CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[4]);	
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}

vu32 BG_CNT;
void Elmo_BG(u8 ID,u8 Mail_If2CtrlList)
{
	BG_CNT++;
	if(IncCAN2Rear==CAN2_SendQueue.Front)	//IncCAN2Rear = Rear+1
	{
		SystemFlag.Can2SendqueueFULL ++;
		return;
	}
	else
	{
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID =0x301+ID;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC =0X04;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0] ='B';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1] ='G';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}

void Elmo_ST(u8 ID , u8 Mail_If2CtrlList)
{
	if(IncCAN2Rear==CAN2_SendQueue.Front)	//IncCAN2Rear = Rear+1
	{
		SystemFlag.Can2SendqueueFULL ++;
		return;
	}
	else
	{	
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID =0x301+ID;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC  =0X04;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0] = 'S';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1] = 'T';
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3] = 0;
		CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	CAN2_SendQueue.Rear=IncCAN2Rear;
}

void Elmo_Ask_PX(u8 ID,u8 Mail_If2CtrlList)		
{

//			if(IncCAN2Rear==CAN2_SendQueue.Front)
//			{
//				SystemFlag.Can2SendqueueFULL++;
//				return;
//			}
//			else
//			{
//				CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID  =0x301+ID;
//				CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC =0X04;
//				CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='P';
//				CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='X';
//				CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
//				CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0x00;
//				CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList=Mail_If2CtrlList;
//			}
//			CAN2_SendQueue.Rear=IncCAN2Rear;	
			
	CanTxMsg tx_message;
	tx_message.StdId=0x301+ID;
	tx_message.RTR = CAN_RTR_Data;		//数据帧
	tx_message.IDE = CAN_Id_Standard;	
	tx_message.DLC=4;
	tx_message.Data[0]='P';
	tx_message.Data[1]='X';
	tx_message.Data[2]=0;
	tx_message.Data[3]=0x00;

	CAN_Transmit(CAN2,&tx_message);
			
}

void Elmo_Ask_VX(u8 ID,u8 Mail_If2CtrlList)		
{

//			if(IncCAN2Rear==CAN2_SendQueue.Front)
//			{
//				SystemFlag.Can2SendqueueFULL++;
//				return;
//			}
//			else
//			{
//				CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID  =0x301+ID;
//				CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC =0X04;
//				CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='V';
//				CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='X';
//				CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
//				CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0x00;
//				CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList=Mail_If2CtrlList;
//			}
//			CAN2_SendQueue.Rear=IncCAN2Rear;		

				CanTxMsg tx_message;
	tx_message.StdId=0x301+ID;
	tx_message.RTR = CAN_RTR_Data;		//数据帧
	tx_message.IDE = CAN_Id_Standard;	
	tx_message.DLC=4;
	tx_message.Data[0]='V';
	tx_message.Data[1]='X';
	tx_message.Data[2]=0;
	tx_message.Data[3]=0x00;

	CAN_Transmit(CAN2,&tx_message);			
}

void Elmo_Ask_IQ(u8 ID,u8 Mail_If2CtrlList)		
{
		if(IncCAN2Rear==CAN2_SendQueue.Front)
		{
			SystemFlag.Can2SendqueueFULL++;
			return;
		}
		else
		{
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].ID  =0x301+ID;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].DLC =0X04;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[0]='I';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[1]='Q';
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[2]=0;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Data[3]=0x00;
			CAN2_SendQueue.Can_DataSend[CAN2_SendQueue.Rear].Mail_If2CtrlList=Mail_If2CtrlList;
		}
		CAN2_SendQueue.Rear=IncCAN2Rear;			
}

void Elmo_IfStuck(u8 ID)
{
	if(ElmoMotor[ID].valueReal.current>35)
	{
		if((ElmoMotor[ID].argum.stuckCnt++)>50)
		{
			ElmoMotor[ID].status.stuck =1;
			if(ElmoMotor[ID].limit.ifReleaseWhenStuck)	{Elmo_Set_MO0(ID,1);}
			LedTask.Stuck = 1;	BeepTask.ErrorOccur=1;	Led8DisData(ID+1);
		}
	}
	else
		ElmoMotor[ID].argum.stuckCnt = 0;
}

void Elmo_IfTimeout(u8 ID)
{
	
	if(ElmoMotor[ID].argum.lastRxTim++>50)
	{
		ElmoMotor[ID].status.timeout = 1;
		LedTask.Timeout = 1;	BeepTask.ErrorOccur=1;	Led8DisData(ID+1);
	}

}

void Elmo_Control(u8 id)
{
	if(ElmoCtrl.Init_Can)	{ELMO_Comm_Init(CAN2);	ElmoCtrl.Init_Can=0;}
	if(ElmoCtrl.Set_Enable) {Elmo_Set_MO1(id,1);ElmoCtrl.Set_Enable=0;}
	if(ElmoCtrl.Set_Disable) {Elmo_Set_MO0(id,1);ElmoCtrl.Set_Disable=0;}
	if(ElmoCtrl.Set_BG)	{Elmo_BG(id,1);ElmoCtrl.Set_BG=0;}
	if(ElmoCtrl.Set_ST)	{Elmo_ST(id,1);ElmoCtrl.Set_ST=0;}
	if(ElmoCtrl.Set_PosMode)	{Elmo_Set_PosUM(id,1);ElmoCtrl.Set_PosMode=0;}
	if(ElmoCtrl.Set_SpMode)	{Elmo_Set_SpUM(id,1);ElmoCtrl.Set_SpMode=0;}
	if(ElmoCtrl.Set_JV) {Elmo_Set_JV(id,ElmoMotor[id].valueSet.speed,1);ElmoCtrl.Set_JV=0;}
	if(ElmoCtrl.Set_SP)	{Elmo_Set_SP(id,ElmoMotor[id].valueSet.PosSP,1);ElmoCtrl.Set_SP=0;}
	if(ElmoCtrl.Set_PA)	{/*Elmo_Set_PA(id,ElmoMotor[id].valueSet.angle,1)*/Elmo_Set_PA_f(id,ElmoMotor[id].valueSet.angle_f,1);ElmoCtrl.Set_PA=0;}
	if(ElmoCtrl.Set_PX)	{Elmo_Set_PX(id,ElmoMotor[id].valueSet.PX,1);ElmoCtrl.Set_PX=0;}
	if(ElmoCtrl.Ask_PX)	{Elmo_Ask_PX(id,1);ElmoCtrl.Ask_PX=0;}
	if(ElmoCtrl.Ask_VX)	{Elmo_Ask_VX(id,1);ElmoCtrl.Ask_VX=0;}
}
