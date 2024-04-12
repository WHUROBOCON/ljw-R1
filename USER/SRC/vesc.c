#include "vesc.h"

VESCParamTypedef VESC_U10,VESC_U8,VESC_U5,VESC_MAD1,VESC_XT4255,VESC_XT2000;
VESCLimitTypedef VESCLimit;
VESCMotorTypedef VESCmotor[5];

void VESC_Init(void)
{
	VESC_U10.Motor_PolePairs = 21;
	VESC_U8.Motor_PolePairs = 21;
	VESC_U5.Motor_PolePairs = 7;
	VESC_MAD1.Motor_PolePairs = 12;
	VESC_XT4255.Motor_PolePairs =7;
	VESC_XT2000.Motor_PolePairs =14;
	
	VESCLimit.Stuck_IfDect = 1;
	VESCLimit.Timeout_IfDect = 1;
	
	VESCLimit.IfReleaseWhenStuck = 1;
	VESCLimit.MAX_Current = 20;	//单位[A]
	
	for(u8 i=0;i<3;i++)
	{
		VESCmotor[i].limit = VESCLimit;
	}

	VESCmotor[0].Param = VESC_XT4255;
	VESCmotor[0].enable = 0;
	VESCmotor[0].begin = 0;
	VESCmotor[0].mode = VESCRPM;
	VESCmotor[0].valueSet.speed = 0;
	VESCmotor[0].valueSet.duty = 0.1;
	VESCmotor[0].valueSet.Break_Current = 10;
	VESCmotor[0].valueSet.HandBreak_Current = 10;
	
	VESCmotor[1].Param = VESC_XT4255;
	VESCmotor[1].enable = 0;
	VESCmotor[1].begin = 0;
	VESCmotor[1].mode = VESCRPM;
	VESCmotor[1].valueSet.speed = 0;
	VESCmotor[1].valueSet.duty = 0.1;
	VESCmotor[1].valueSet.Break_Current = 10;
	VESCmotor[1].valueSet.HandBreak_Current = 10;
	
	VESCmotor[2].Param = VESC_XT4255;
	VESCmotor[2].enable = 0;
	VESCmotor[2].begin = 0;
	VESCmotor[2].mode = VESCRPM;
	VESCmotor[2].valueSet.speed = 0;
	VESCmotor[2].valueSet.duty = 0.1;
	VESCmotor[2].valueSet.Break_Current = 10;
	VESCmotor[2].valueSet.HandBreak_Current = 10;
	
	VESCPID_Init(&VESCmotor[0].S_Pid,0,0,0,VESCmotor[0].valueSet.angle);
	VESCPID_Init(&VESCmotor[1].S_Pid,0,0,0,VESCmotor[1].valueSet.angle);
	
	
//	//0号随便了，为了对应
//	VESCmotor[UpWheelID].Param = VESC_U10;
//	VESCmotor[UpWheelID].enable = 0;
//	VESCmotor[UpWheelID].begin = 0;
//	VESCmotor[UpWheelID].mode = VESCRPM;
//	VESCmotor[UpWheelID].valueSet.speed = 0;
//	VESCmotor[UpWheelID].valueSet.duty = 0.1;
//	VESCmotor[UpWheelID].valueSet.Break_Current = 10;
//	VESCmotor[UpWheelID].valueSet.HandBreak_Current = 10;
//	
////	VESC_PID_Init(&VESCmotor[0].S_Pid, 0.1, 0.0001, 0.000001, 0.2 , 0.05);
//	
//	VESCmotor[DownWheelID].Param = VESC_U10;
//	VESCmotor[DownWheelID].enable = 0;
//	VESCmotor[DownWheelID].begin = 0;
//	VESCmotor[DownWheelID].mode = VESCRPM;
//	VESCmotor[DownWheelID].valueSet.speed = 0;
//	VESCmotor[DownWheelID].valueSet.duty = 0.1;
//	VESCmotor[DownWheelID].valueSet.Break_Current = 10;
//	VESCmotor[DownWheelID].valueSet.HandBreak_Current = 10;
//	
////	VESC_PID_Init(&VESCmotor[1].S_Pid, 0.1, 0.0001, 0.000001, 0.2, 0.05 );

//	VESCmotor[2].Param = VESC_U10;
//	VESCmotor[2].enable = 0;
//	VESCmotor[2].begin = 0;
//	VESCmotor[2].mode = VESCRPM;
//	VESCmotor[2].valueSet.speed = 0;
//	VESCmotor[2].valueSet.duty = 0.1;
//	VESCmotor[2].valueSet.Break_Current = 10;
//	VESCmotor[2].valueSet.HandBreak_Current = 10;
//	
//	VESCmotor[3].Param = VESC_U10;
//	VESCmotor[3].enable = 0;
//	VESCmotor[3].begin = 0;
//	VESCmotor[3].mode = VESCRPM;
//	VESCmotor[3].valueSet.speed = 0;
//	VESCmotor[3].valueSet.duty = 0.1;
//	VESCmotor[3].valueSet.Break_Current = 10;
//	VESCmotor[3].valueSet.HandBreak_Current = 10;
}

/**
  * @brief 设定VESC PID
  */
void VESCPID_Init(VESC_PID_setTypeDef *S_Pid, float kp, float ki, float kd, int32_t targetPosition)
{
//	int ID;
//	VESCmotor[ID].S_Pid.kp=kp;
//	VESCmotor[ID].S_Pid.ki=ki;
//	VESCmotor[ID].S_Pid.kd=kd;
//	VESCmotor[ID].S_Pid.targetPosition=targetPosition;
//	
//	VESCmotor[ID].S_Pid.currentPosition=0;
//	VESCmotor[ID].S_Pid.error=0;
//	VESCmotor[ID].S_Pid.lastError=0;
//	VESCmotor[ID].S_Pid.previousError=0;
//	
//	VESCmotor[ID].S_Pid.error=VESCmotor[ID].S_Pid.targetPosition-VESCmotor[ID].S_Pid.currentPosition;
//	VESCmotor[ID].S_Pid.P=VESCmotor[ID].S_Pid.kp*(VESCmotor[ID].S_Pid.error - VESCmotor[ID].S_Pid.lastError);
//	VESCmotor[ID].S_Pid.I=VESCmotor[ID].S_Pid.ki*VESCmotor[ID].S_Pid.error;
//	VESCmotor[ID].S_Pid.D=VESCmotor[ID].S_Pid.kd*(VESCmotor[ID].S_Pid.error+VESCmotor[ID].S_Pid.previousError-2*VESCmotor[ID].S_Pid.lastError);
//	VESCmotor[ID].S_Pid.pidOutput=VESCmotor[ID].S_Pid.P+VESCmotor[ID].S_Pid.I+VESCmotor[ID].S_Pid.D;
//	VESCmotor[ID].S_Pid.previousError=VESCmotor[ID].S_Pid.lastError;
//	VESCmotor[ID].S_Pid.lastError=VESCmotor[ID].S_Pid.error;
}

/**
  * @brief 设定VESC角度
  */
void VESC_Set_Pos(u8 controller_ID, float pos, u8 Mail_If2CtrlList)
{
  int32_t send_index = 0;
  
	if(IncCAN3Rear==VESC_SendQueue.Front)
	{
		SystemFlag.Can2SendqueueFULL++;		
		return;
	}
	else
	{
		buffer_append_int32(VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Data, (int32_t)(pos * 1e6f), &send_index);		//commomcan里接收的时候除了10的6次方
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_POS << 8);		
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].DLC = send_index;				
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Mail_If2CtrlList=Mail_If2CtrlList;
	}
	VESC_SendQueue.Rear=IncCAN3Rear; 
}

/** 
  * @brief 设定VESC占空比
  */
void VESC_Set_Duty_Cycle(u8 controller_ID, float duty_cycle, u8 Mail_If2CtrlList)
{
	int32_t send_index = 0;
	if (IncCAN3Rear == VESC_SendQueue.Front)
	{
		SystemFlag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		buffer_append_int32(VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Data, (int32_t)(duty_cycle * 100000), &send_index);
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_DUTY << 8);
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].DLC = send_index;
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	VESC_SendQueue.Rear = IncCAN3Rear;
}

/** 
	* @brief 设定VESC转速
	*/
void VESC_Set_Speed(u8 controller_ID, float speed, u8 Mail_If2CtrlList)
{
	int32_t send_index = 0;
	if (IncCAN3Rear == VESC_SendQueue.Front)
	{
		SystemFlag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		buffer_append_int32(VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Data, speed, &send_index);
		//send_index = 0.即将格式为int32的speed放到数组buffer中，number的高位对应buffer的低字节
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_RPM << 8);
//																											   第二位：0			最低位								第三位：	3
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].DLC = send_index;
//																													3在 buffer_append中++了
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
		VESC_SendQueue.Rear = IncCAN3Rear;	//Rear=Rear+1

}

/** 
	* @brief 设定VESC电流
	*/
void VESC_Set_Current(u8 controller_ID, float current, u8 Mail_If2CtrlList)
{
	int32_t send_index = 0;
	if (IncCAN3Rear == VESC_SendQueue.Front)
	{
		SystemFlag.Can2SendqueueFULL++;
		return;
	}
	else
	{

		buffer_append_int32(VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Data, (int32_t)(current * 1000), &send_index);
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_CURRENT << 8);
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].DLC = send_index;
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	VESC_SendQueue.Rear = IncCAN3Rear;
}

/** 
	* @brief 设定VESC刹车电流
	*/
void VESC_Set_Brake_Current(u8 controller_ID, float brake_current, u8 Mail_If2CtrlList)
{
	int32_t send_index = 0;
	if (IncCAN3Rear == VESC_SendQueue.Front)
	{
		SystemFlag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		buffer_append_int32(VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Data, (int32_t)(brake_current * 1000), &send_index);
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_CURRENT_BRAKE << 8);
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].DLC = send_index;
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	VESC_SendQueue.Rear = IncCAN3Rear;
}

/** 
	* @brief 设定VESC锁电机电流
	*/
void VESC_Set_Handbrake_Current(u8 controller_ID, float handbrake_current, u8 Mail_If2CtrlList)
{
	int32_t send_index = 0;
	if (IncCAN3Rear == VESC_SendQueue.Front)
	{
		SystemFlag.Can2SendqueueFULL++;
		return;
	}
	else
	{
		buffer_append_int32(VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Data, (int32_t)(handbrake_current * 1000), &send_index);
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].ID = 0xf0000000 | controller_ID | ((uint32_t)CAN_PACKET_SET_CURRENT_HANDBRAKE << 8);
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].DLC = send_index;
		VESC_SendQueue.Can_DataSend[VESC_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;
	}
	VESC_SendQueue.Rear = IncCAN3Rear;
}

void VESC_IfTimeout(u8 id)
{
	if( VESCmotor[id].enable && (VESCmotor[id].Argum.lastRxTim++)>1000 )	
	{
		VESCmotor[id].Argum.timeoutTicks++;
		if(VESCmotor[id].Argum.timeoutTicks>50)
		{
			VESCmotor[id].Status.timeout = 1;
//			LedTask.Timeout = 1;	BeepTask.ErrorOccur=1;	Led8DisData(id+1);
		}
		else
		{
			VESCmotor[id].Status.timeout = 0;
		}
	}
	else 
		VESCmotor[id].Argum.timeoutTicks = 0;
}

void VESC_IfStuck(u8 id)
{
	if( (VESCmotor[id].valueReal.current>45)&&(VESCmotor[id].enable) )
	{
		VESCmotor[id].Argum.stuckCnt++;
		if(VESCmotor[id].Argum.stuckCnt>3000)
		{
			VESCmotor[id].Status.stuck = true;
//			LedTask.Stuck = 1;	BeepTask.ErrorOccur=1;	Led8DisData(id+1);
			if(VESCmotor[id].limit.IfReleaseWhenStuck)	VESCmotor[id].enable = 0;			
		}
		else 
		{
			VESCmotor[id].Status.stuck = false;
		}
	}
	else
		VESCmotor[id].Argum.stuckCnt = 0;
}

u16 Accnt = 0;
void VESCFunc(void)
{
	for(u8 i=0;i<=3;i++)
	{
		if(VESCmotor[i].enable)
		{
			if(VESCmotor[i].limit.Stuck_IfDect)	VESC_IfStuck(i);
			if(VESCmotor[i].limit.Timeout_IfDect) VESC_IfTimeout(i);
			if(VESCmotor[i].begin)	
			{
				switch (VESCmotor[i].mode)	
				{	//VESC驱动器中ID从0x901开始
					case VESCCURRENT: VESC_Set_Current(i+1, VESCmotor[i].valueSet.current, 0);	break;
					case VESCRPM: 
					{
						VESC_Set_Speed(i+1,VESCmotor[i].valueSet.speed* VESCmotor[i].Param.Motor_PolePairs,0);	
						if((VESCmotor[i].Argum.distance==0)&&(VESCmotor[i].valueSet.speed!=0))
						{
							Accnt++;
							if(Accnt>=80)
							{
								B1p.BeepOnNum = 10; 
								LedTask.CloseAll  = true;
								VESCmotor[i].enable = 0;
								Accnt=0;
							}
						}
//								if(VESCmotor[i].valueReal.current>=1.7f)
//								{
//									VESCmotor[i].enable = 0;
//								}
						else
							Accnt = 0;
					}
					break;							
					case VESCPOSITION: VESC_Set_Pos(i+1,VESCmotor[i].valueSet.angle,0);	break;
					case VESCDUTY: VESC_Set_Duty_Cycle(i+1,VESCmotor[i].valueSet.duty,0);	break;
					case VESCHANDBREAK: VESC_Set_Handbrake_Current(i+1,VESCmotor[i].valueSet.HandBreak_Current,0);	break;
					default: break;
				}
			}
			else
				VESC_Set_Handbrake_Current(i+1,VESCmotor[i].valueSet.HandBreak_Current,0);
		}
	}
}

void VESCReceiveHandler(CanRxMsg rx_message)
{
			int32_t ind=0;
			u8 id = (rx_message.ExtId&0xff)-1;		//901 在我这是0 
			if((rx_message.ExtId>>8)==CAN_PACKET_STATUS)
			{
				//这里的速度有问题
				VESCmotor[id].valueReal.speed=buffer_32_to_float(rx_message.Data,1e0,&ind)/VESCmotor[id].Param.Motor_PolePairs;		//0 1 2 3
				VESCmotor[id].valueReal.current=buffer_16_to_float(rx_message.Data,1e1,&ind);		// 4 5  得到的电流除以10（手转的时候没有电流）
				VESCmotor[id].valueReal.angle=buffer_16_to_float(rx_message.Data,1e1,&ind);		//6 7			得到的角度除以10		只供阅读，为0~360间的值
			
				ChangeData(&rx_message.Data[6],&rx_message.Data[7]);
				DecodeU16Data(&VESCmotor[id].Argum.angleNow,&rx_message.Data[6]);
				VESCmotor[id].Argum.distance = VESCmotor[id].Argum.angleNow - VESCmotor[id].Argum.anglePrv;
				VESCmotor[id].Argum.anglePrv = VESCmotor[id].Argum.angleNow;
				if(ABS(VESCmotor[id].Argum.distance)>1800) VESCmotor[id].Argum.distance -= 3600;
				VESCmotor[id].Argum.position += VESCmotor[id].Argum.distance;
				VESCmotor[id].valueReal.Angle_ABS = VESCmotor[id].Argum.position / 10.0;	
				
//				VESCmotor[id].enable = 1;		
//				VESCmotor[id].mode = VESCPOSITION;
				VESCmotor[id].begin = 1; //如果收到报文，begin置1
			}
			VESCmotor[id].Argum.lastRxTim = 0;
}

