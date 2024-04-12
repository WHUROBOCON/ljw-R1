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
	unsigned short ListID=0xff;	//���ListID������ѡ�ģ��ж�ListID>NodeNum,��ֱ����������Ĳ�����
	
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
		if( CanQueue_ifFull(&CtrlList[ListID].SentQueue) )	//�жϿ��Ʊ�ķ��Ͷ����Ƿ�����	,��Ϊ��������б����ˣ�����û���յ����������Ծ����б�����
		{
			if(CAN_1==CAN_x)	SystemFlag.Can1SendqueueFULL++;
			else SystemFlag.Can2SendqueueFULL++;
		}
		else
		{
			CtrlList[ListID].Mails2Send++;	//��������Ʊ�������һ
			CtrlList[ListID].MailsInList++;	//������Ʊ�������һ
			/*�����ж��׵ı��ķŵ����Ʊ�Ķ�β*/
			CtrlList[ListID].SentQueue.Can_DataSend[ CtrlList[ListID].SentQueue.Rear ].ID = can_queue->Can_DataSend[can_queue->Front].ID;
			CtrlList[ListID].SentQueue.Can_DataSend[ CtrlList[ListID].SentQueue.Rear ].DLC = can_queue->Can_DataSend[can_queue->Front].DLC;
			CtrlList[ListID].SentQueue.Can_DataSend[ CtrlList[ListID].SentQueue.Rear ].Mail_If2CtrlList = can_queue->Can_DataSend[can_queue->Front].Mail_If2CtrlList;
			memcpy( CtrlList[ListID].SentQueue.Can_DataSend[CtrlList[ListID].SentQueue.Rear].Data , can_queue->Can_DataSend[can_queue->Front].Data,
			CtrlList[ListID].SentQueue.Can_DataSend[CtrlList[ListID].SentQueue.Rear].DLC * sizeof(char) );
			CtrlList[ListID].SentQueue.Rear = (CtrlList[ListID].SentQueue.Rear+1)%CAN_QUEUESIZE;		//�����б��Ǻ��б�ͬ����С��
		}
	}
}

void CtrlList_Func(MsgCtrlGrpTypedef* CtrlList, Can_SendQueueTypedef* can_queue,u8 CAN_x )
{
	unsigned char NodeNum=0;
	if(CAN_1==CAN_x) NodeNum = CAN1_NodeNumber;
	else NodeNum = CAN2_NodeNumber;
	/*�������Ŀ��Ʊ�����ڵ�*/
	
	for(unsigned char i=0;i<NodeNum;i++)
	{
		if(CtrlList[i].Mails2Send>=1)
		{
			if(CtrlList[i].MailTimeout++ >= 100)		//�ýڵ�ı���10msδ�յ��ظ�
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
					CtrlList[i].Mails2Send --;			//��������Ʊ�������һ
					CtrlList[i].MailTimeout = 0;		//
					CtrlList[i].NodeTimeout = 0;		//�ڵ㳬ʱ����λ����
					/*�����Ʊ���׵ı������뷢�Ͷ��еĶ�β*/
					can_queue->Can_DataSend[can_queue->Rear].ID= CtrlList[i].SentQueue.Can_DataSend[CtrlList[i].SentQueue.Front].ID;
					can_queue->Can_DataSend[can_queue->Rear].DLC= CtrlList[i].SentQueue.Can_DataSend[CtrlList[i].SentQueue.Front].DLC;
					can_queue->Can_DataSend[can_queue->Rear].Mail_If2CtrlList= CtrlList[i].SentQueue.Can_DataSend[CtrlList[i].SentQueue.Front].Mail_If2CtrlList;
					memcpy( can_queue->Can_DataSend[can_queue->Rear].Data, CtrlList[i].SentQueue.Can_DataSend[CtrlList[i].SentQueue.Front].Data,
									sizeof(char)*can_queue->Can_DataSend[can_queue->Rear].DLC);
					can_queue->Rear = (can_queue->Rear+1)%CAN_QUEUESIZE;
					CtrlList[i].SentQueue.Front = (CtrlList[i].SentQueue.Front+1)%CAN_QUEUESIZE;		//�������׼�һ
				}
			}	
			if(CtrlList[i].NodeTimeout>1000 || (CtrlList[i].MailsInList-CtrlList[i].MailsOutList)>150 )	//100ms�ڶ���һֱ������&&������Ʊ�ı��������յ��ı��ķ���������150
			{
				CtrlList[i].Mails2Send=-10000;	//���Ͻڵ��ͷ�
				if(CAN_1==CAN_x)
				{
					switch(i) 
					{
//						case 0:	BeepTask.ErrorOccur=1;	LedTask.CANErr=1; Led8DisData(8);	break;		//�����ڵ����		
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
//						case 0:	ElmoMotor[0].CanError=1; BeepTask.ErrorOccur=1;	LedTask.CANErr=1; Led8DisData(i+1);	break;		//ELMO�ڵ����
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
