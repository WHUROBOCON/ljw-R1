#include "queue.h"

Can_DatasendTypeDef Can_Datasend;
Can_SendQueueTypedef CAN1_SendQueue,CAN2_SendQueue,VESC_SendQueue,ZDrive_SendQueue;
MsgCtrlGrpTypedef Can1MsgCtrllist[CAN1_NodeNumber],Can2MsgCtrllist[CAN2_NodeNumber];


u32 cnt111;
u8 CANTransmit_ERROR;
u8 FirstIgnore;

void Can_SendQueueInit(void)
{
	CAN1_SendQueue.Front = CAN1_SendQueue.Rear = 0;
	CAN2_SendQueue.Front = CAN2_SendQueue.Rear = 0;
//	VESC_SendQueue.Front = VESC_SendQueue.Rear = 0;		//ELMO专用发送队列，查询指令不在里面
}

bool CanQueue_ifEmpty(Can_SendQueueTypedef* can_queue)
{
	return( (can_queue->Front)==(can_queue->Rear) );
}

bool CanQueue_ifFull(Can_SendQueueTypedef* can_queue)
{
	return( ((can_queue->Rear+1)%CAN_QUEUESIZE) == can_queue->Front );
}

void Can_DeQueue(CAN_TypeDef* CANx,Can_SendQueueTypedef* can_queue)
{
	CanTxMsg TxMessage;
//	CanSendCount = 0;
	
	if(CanQueue_ifEmpty(can_queue))
	{
		if(CAN1==CANx)
			SystemFlag.Can1SendqueueEMPTY = 1;
		else
			SystemFlag.Can2SendqueueEMPTY = 1;
	}
	else
	{

		/*主控板*/
		if(can_queue->Can_DataSend[can_queue->Front].ID==ZhuKong_TX)
		{
			TxMessage.IDE = CAN_ID_EXT;
			TxMessage.ExtId = can_queue->Can_DataSend[can_queue->Front].ID;
		}
		/*给L版*/
		else if(can_queue->Can_DataSend[can_queue->Front].ID==S2L)
		{
			TxMessage.IDE = CAN_ID_EXT;
			TxMessage.ExtId = can_queue->Can_DataSend[can_queue->Front].ID;
		}
		/*ZDRIVE*/
		else if(can_queue->Can_DataSend[can_queue->Front].ID>=0x0001&&can_queue->Can_DataSend[can_queue->Front].ID<=0x0008)
		{
			TxMessage.IDE = CAN_ID_STD;
			TxMessage.StdId = can_queue->Can_DataSend[can_queue->Front].ID;
		}
		/*VESC*/
		else
		{
			if( (can_queue->Can_DataSend[can_queue->Front].ID & 0xf0000000) != 0 )	//虽然不太知道为啥要这样写
			{
				can_queue->Can_DataSend[can_queue->Front].ID = can_queue->Can_DataSend[can_queue->Front].ID & 0x0fffffff;
			}
			TxMessage.IDE = CAN_ID_EXT;
			TxMessage.ExtId = can_queue->Can_DataSend[can_queue->Front].ID;
		}

		
		if(CAN1==CANx)
			Mail_Queue2CtlList( Can1MsgCtrllist, &CAN1_SendQueue, CAN_1);
		else 
			Mail_Queue2CtlList( Can2MsgCtrllist, &CAN2_SendQueue, CAN_2);
		 
			
		TxMessage.DLC = can_queue->Can_DataSend[can_queue->Front].DLC;
		TxMessage.RTR = CAN_RTR_DATA;		//数据帧
		
		memcpy(TxMessage.Data, (can_queue->Can_DataSend[can_queue->Front].Data), TxMessage.DLC*sizeof(int8_t) );	//之前这里是uint8_t,改成这个应该是一样的

		
		can_queue->Front = (can_queue->Front+1)%CAN_QUEUESIZE;
		
		CAN_Transmit(CANx,&TxMessage);

	}
}
