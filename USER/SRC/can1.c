#include "can1.h"
void CAN1_Init(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;		//CAN1使用PA11和PA12
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE; //非时间触发通道模式
  CAN_InitStructure.CAN_ABOM = DISABLE; //软件对CAN_MCR寄存器的INRQ位置1，随后清0，一旦监测到128次连续11位的隐性位，就退出离线状态
  CAN_InitStructure.CAN_AWUM = DISABLE; //睡眠模式由软件唤醒
  CAN_InitStructure.CAN_NART = DISABLE; //禁止报文自动发送，即只发送一次，无论结果如何
  CAN_InitStructure.CAN_RFLM = DISABLE; //报文不锁定，新的覆盖旧的
  CAN_InitStructure.CAN_TXFP = DISABLE; //发送FIFO的优先级由标识符决定
   CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  

  /* Seting BaudRate */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; //重新同步跳跃宽度为一个时间单位
  CAN_InitStructure.CAN_BS1 = CAN_BS1_9tq; //时间段1占用8个时间单位
  CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq; //时间段2占用7个时间单位
  CAN_InitStructure.CAN_Prescaler = 3;     //分频系数（Fdiv）
  CAN_Init(CAN1, &CAN_InitStructure);      //初始化CAN1
 //can波特率=42M/（1+CAN_BS1+CAN_BS2）/CAN_Prescaler=42000/（1+9+4）/3 = 1Mbps
  //CAN1通信波特率=42MHZ
		
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=((ZhuKong_RX<<3)&0xffff0000)>>16;
	CAN_FilterInitStructure.CAN_FilterIdLow=(ZhuKong_RX<<3)&0xffff;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(0xffff00<<3)>>16;							   
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=(0xffff00<<3)&0xffff;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber = 1;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=((BroadCast_RX<<3)&0xffff0000)>>16;
	CAN_FilterInitStructure.CAN_FilterIdLow=(BroadCast_RX<<3)&0xffff;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(0xffff00<<3)>>16;							   
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=(0xffff00<<3)&0xffff;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
//	CAN_FilterInitStructure.CAN_FilterNumber = 2;
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
//	CAN_FilterInitStructure.CAN_FilterIdHigh=((Fire2TakeRing_RX<<3)&0xffff0000)>>16;
//	CAN_FilterInitStructure.CAN_FilterIdLow=(Fire2TakeRing_RX<<3)&0xffff;
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(0xffff00<<3)>>16;							   
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow=(0xffff00<<3)&0xffff;
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
//	CAN_FilterInit(&CAN_FilterInitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=CAN1_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE);
}

void CAN1_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!=RESET)
	{
		CanRxMsg rx_message;
		CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_ClearFlag(CAN1, CAN_IT_FMP0);
		
		uint8_t _order = 0x000000ff & rx_message.ExtId;
		if ((rx_message.ExtId & 0xffffff00) == ZhuKong_RX)
		{
			switch (_order) 
			{
				case 1: 
				{
					if(rx_message.Data[0] == 'M'&& (rx_message.Data[1]) == 'O') 
					{
						if(rx_message.Data[1]==1)
							VESCmotor[0].enable =1;
						  VESCmotor[1].enable =1;
						if(rx_message.Data[1]==0)
							VESCmotor[0].enable =0;
						  VESCmotor[1].enable =0;
					}
					ResponseMaterEnableStatus(rx_message);
					break;
				}
			
				case 2: 
				{
					if(rx_message.Data[0] == 'F'&& (rx_message.Data[1]) == 'Z') 
					{
						VESCmotor[0].begin=1;
						VESCmotor[1].begin=1;
						VESCmotor[0].valueSet.angle =VESCmotor[0].valueReal.angle;
					}
					ResponseMaterZerofindingStatus(rx_message);
					break;
				}
			  
				case 3:
				{
					u16 s= (rx_message.Data[0] << 8) | rx_message.Data[1];
					u32 _angle= ANGLE / LENGTH;
					VESCmotor[1].valueSet.angle = s*_angle;
					ResponseMaterPositionStatus(rx_message);
					break;
				}
				
				case 4:
				{
					buffer32tofloat(rx_message.Data, &(VESCmotor[0].valueSet.angle));
					ResponseMaterAngleStatus(rx_message);
					break;
				}
			
				case 5:
				{
					if(rx_message.Data[0] == 'P')
					{
						if(rx_message.Data[1]==1)//close
						{
							Valve_Ctrl(255);
						}
						if(rx_message.Data[1]==0)//open 
						{
							Valve_Ctrl(0);
						}
					}
					ResponseMaterCYLStatus(rx_message);
					break;
				}
		
				case 0xEE:
				{
					break;
				}
				
				case 0xFF:
				{
					break;
				}
					
        default:
          break;
			} 
		} 
	}
}

void CAN1_RX1_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN1, CAN_IT_FMP1) != RESET)
    {
        CanRxMsg rx_message;
        CAN_Receive(CAN1, CAN_FIFO1, &rx_message);
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP1); //?????
        CAN_ClearFlag(CAN1, CAN_IT_FMP1);
    }
}


static void ResponseMaterEnableStatus(CanRxMsg rx_message)
{
	if (CanQueue_ifFull(&CAN1_SendQueue))
	{
		 SystemFlag.Can1SendqueueFULL = true;
			return;
	}
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = 0x05020101;
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].IDE =CAN_ID_EXT;
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 4 ;
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'M';
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'O';	
	if (rx_message.Data[2] == 1&&VESCmotor[0].enable ==1)
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 1;
	else 
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 0;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList  = 1;
		CAN1_SendQueue.Rear = IncCAN1Rear;
}


static void ResponseMaterZerofindingStatus(CanRxMsg rx_message)
{
	if (CanQueue_ifFull(&CAN1_SendQueue))
	{
		 SystemFlag.Can1SendqueueFULL = true;
			return;
	}
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = 0x05020102;
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].IDE =CAN_ID_EXT;
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 2;
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'O';
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'K';	
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = 1;
	CAN1_SendQueue.Rear = IncCAN1Rear;
}


static void ResponseMaterPositionStatus(CanRxMsg rx_message)
{
	if (CanQueue_ifFull(&CAN1_SendQueue))
	{
		 SystemFlag.Can1SendqueueFULL = true;
			return;
	}
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = 0x05020103;
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].IDE =CAN_ID_EXT;
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 2;
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'O';
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'K';	
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList  = 1;
	CAN1_SendQueue.Rear = IncCAN1Rear;
}


static void ResponseMaterAngleStatus(CanRxMsg rx_message)
{
	if (CanQueue_ifFull(&CAN1_SendQueue))
	{
		 SystemFlag.Can1SendqueueFULL = true;
			return;
	}
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = 0x05020104;
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].IDE =CAN_ID_EXT;
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 2;
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'O';
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'K';	
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = 1;
	CAN1_SendQueue.Rear = IncCAN1Rear;
}


static void ResponseMaterCYLStatus(CanRxMsg rx_message)
{
	if (CanQueue_ifFull(&CAN1_SendQueue))
	{
		 SystemFlag.Can1SendqueueFULL = true;
			return;
	}
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = 0x05020105;
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].IDE =CAN_ID_EXT;
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 2;
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
	if(scoopCase==0)
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 0;	
	if(scoopCase==1)
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 1;	
	if(scoopCase==2)
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 2;
	CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = 1;
	CAN1_SendQueue.Rear = IncCAN1Rear;
}
