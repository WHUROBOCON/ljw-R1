#include "canctrllist.h"

void MesgCtrlList_Init(MsgCtrlGrpTypedef* CtrlList, u8 CAN_x)
{
	unsigned char NodeNum=0;
	if(CAN_1==CAN_x)
	{
		NodeNum = CAN1_NodeNumber;
	}
	else
	{
		NodeNum = CAN2_NodeNumber;
	}
	for(unsigned char i=0;i<NodeNum;i++)
	{
		CtrlList[i].Mails2Send=0;
		CtrlList[i].MailsInList=0;
		CtrlList[i].MailsOutList=0;
		CtrlList[i].MailTimeout=0;
		CtrlList[i].NodeTimeout=0;
		CtrlList[i].SentQueue.Rear=0;
		CtrlList[i].SentQueue.Front=0;
		CtrlList[i].SentQueue.Can_DataSend[0].Mail_If2CtrlList=0;
	}
}

void Mail_Queue2CtlList(MsgCtrlGrpTypedef* CtrlList,Can_SendQueueTypedef* can_queue,u8 CAN_x)
{
	unsigned char NodeNum=0;
	unsigned short ListID=0xff;	//如果ListID不是所选的，判断ListID>NodeNum,就直接跳过下面的操作了
	
	if(can_queue->Can_DataSend[can_queue->Front].Mail_If2CtrlList==0)
		return;
	
	if( CAN_1 == CAN_x )
	{
		NodeNum = CAN1_NodeNumber;
		switch(can_queue->Can_DataSend[can_queue->Front].ID)
		{
			case Master2DuoJi_TX: ListID = 0;	break;
			default:	break;
		}
	}
	else
	{
		NodeNum = CAN2_NodeNumber;
		switch(can_queue->Can_DataSend[can_queue->Front].ID)
		{
			case Elmo_Motor1_TX:		ListID = 0;		break;
			case Elmo_Motor2_TX:    ListID = 1;		break;
			case Elmo_Motor3_TX:		ListID = 2;		break;
			case Elmo_Motor4_TX:		ListID = 3;		break;
			default:  break;
		}
	}
	
	if((ListID<NodeNum)&&(ListID!=Elmo_Motor1_TX))
	{
		if( CanQueue_ifFull(&CtrlList[ListID].SentQueue) )	//判断控制表的发送队列是否已满	,因为如果控制列表满了，就是没有收到反馈，所以就是列表满了
		{
			if(CAN_1==CAN_x)	SystemFlag.Can1SendqueueFULL++;
			else SystemFlag.Can2SendqueueFULL++;
		}
		else
		{
			CtrlList[ListID].Mails2Send++;	//请求出控制表报文数加一
			CtrlList[ListID].MailsInList++;	//进入控制表报文数加一
			/*将队列队首的报文放到控制表的队尾*/
			CtrlList[ListID].SentQueue.Can_DataSend[ CtrlList[ListID].SentQueue.Rear ].ID = can_queue->Can_DataSend[can_queue->Front].ID;
			CtrlList[ListID].SentQueue.Can_DataSend[ CtrlList[ListID].SentQueue.Rear ].DLC = can_queue->Can_DataSend[can_queue->Front].DLC;
			CtrlList[ListID].SentQueue.Can_DataSend[ CtrlList[ListID].SentQueue.Rear ].Mail_If2CtrlList = can_queue->Can_DataSend[can_queue->Front].Mail_If2CtrlList;
			memcpy( CtrlList[ListID].SentQueue.Can_DataSend[CtrlList[ListID].SentQueue.Rear].Data , can_queue->Can_DataSend[can_queue->Front].Data,
			CtrlList[ListID].SentQueue.Can_DataSend[CtrlList[ListID].SentQueue.Rear].DLC * sizeof(char) );
			CtrlList[ListID].SentQueue.Rear = (CtrlList[ListID].SentQueue.Rear+1)%CAN_QUEUESIZE;		//控制列表是和列表同样大小的
		}
	}
}

void CtrlList_Func(MsgCtrlGrpTypedef* CtrlList, Can_SendQueueTypedef* can_queue,u8 CAN_x )
{
	unsigned char NodeNum=0;
	if(CAN_1==CAN_x) NodeNum = CAN1_NodeNumber;
	else NodeNum = CAN2_NodeNumber;
	/*遍历报文控制表各个节点*/
	
	for(unsigned char i=0;i<NodeNum;i++)
	{
		if(CtrlList[i].Mails2Send>=1)
		{
			if(CtrlList[i].MailTimeout++ >= 100)		//该节点的报文10ms未收到回复
			{
				if(CanQueue_ifFull(can_queue))
				{
					CtrlList[i].NodeTimeout++;
					return;
				}
				else if(CanQueue_ifEmpty(&CtrlList[i].SentQueue))
				{
					CtrlList[i].NodeTimeout=0;
					return;
				}
				else
				{
					CtrlList[i].Mails2Send --;			//请求出控制表报文数减一
					CtrlList[i].MailTimeout = 0;		//
					CtrlList[i].NodeTimeout = 0;		//节点超时计数位清零
					/*将控制表队首的报文塞入发送队列的队尾*/
					can_queue->Can_DataSend[can_queue->Rear].ID= CtrlList[i].SentQueue.Can_DataSend[CtrlList[i].SentQueue.Front].ID;
					can_queue->Can_DataSend[can_queue->Rear].DLC= CtrlList[i].SentQueue.Can_DataSend[CtrlList[i].SentQueue.Front].DLC;
					can_queue->Can_DataSend[can_queue->Rear].Mail_If2CtrlList= CtrlList[i].SentQueue.Can_DataSend[CtrlList[i].SentQueue.Front].Mail_If2CtrlList;
					memcpy( can_queue->Can_DataSend[can_queue->Rear].Data, CtrlList[i].SentQueue.Can_DataSend[CtrlList[i].SentQueue.Front].Data,
									sizeof(char)*can_queue->Can_DataSend[can_queue->Rear].DLC);
					can_queue->Rear = (can_queue->Rear+1)%CAN_QUEUESIZE;
					CtrlList[i].SentQueue.Front = (CtrlList[i].SentQueue.Front+1)%CAN_QUEUESIZE;		//出表，队首加一
				}
			}	
			if(CtrlList[i].NodeTimeout>1000 || (CtrlList[i].MailsInList-CtrlList[i].MailsOutList)>150 )	//100ms内队列一直是满的&&进入控制表的报文数与收到的报文反馈数大于150
			{
				CtrlList[i].Mails2Send=-10000;	//故障节点释放
				if(CAN_1==CAN_x)
				{
					switch(i) 
					{
//						case 0:	BeepTask.ErrorOccur=1;	LedTask.CANErr=1; Led8DisData(8);	break;		//舵机板节点故障		
//						case 1:					break;
//						case 2:					break;
//						case 3:					break;
						default:				break;
					}
				}
				else
				{
					switch(i)
					{
//						case 0:	ElmoMotor[0].CanError=1; BeepTask.ErrorOccur=1;	LedTask.CANErr=1; Led8DisData(i+1);	break;		//ELMO节点故障
		//				case 1:	ElmoMotor[1].CanError=1; BeepTask.ErrorOccur=1;	LedTask.CANErr=1; Led8DisData(i); break;
		//				case 2:	ElmoMotor[2].CanError=1; BeepTask.ErrorOccur=1;	LedTask.CANErr=1; Led8DisData(i);	break;
		//				case 3:	ElmoMotor[3].CanError=1; BeepTask.ErrorOccur=1;	LedTask.CANErr=1; Led8DisData(i);	break;
						default:				break;
					}
				}		
			}
		}
	}
}
