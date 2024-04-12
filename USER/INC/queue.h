#ifndef _QUEUE_H
#define _QUEUE_H

#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"	//include后memcpy函数才不会报warning 

#include "param.h"
#include "can1.h"
#include "led.h"

#define CAN_HaveSentQueueSize 8
#define CAN_QUEUESIZE 100		//[发送队列长度]

#define IncCAN1Rear (CAN1_SendQueue.Rear+1)%CAN_QUEUESIZE
#define IncCAN2Rear	(CAN2_SendQueue.Rear+1)%CAN_QUEUESIZE
#define IncCAN3Rear	(VESC_SendQueue.Rear+1)%CAN_QUEUESIZE
#define IncZDriveRear (ZDrive_SendQueue.Rear+1)%CAN_QUEUESIZE

#define CAN_1 1
#define CAN_2 2

#define CAN1_NodeNumber 6u
#define CAN2_NodeNumber 6u

typedef struct{
	u32 ID;
	u8 DLC;
	u8 Data[8];
	u8 Mail_If2CtrlList;
}Can_DatasendTypeDef;

typedef struct{
	u8 Front,Rear;
	Can_DatasendTypeDef Can_DataSend[CAN_QUEUESIZE];
}Can_SendQueueTypedef;

typedef struct{
	s32 MailsInList;	//[进入节点的报文总数]
	s32 MailsOutList;	//[节点收到的报文反馈数]
	u32 NodeTimeout;	//[队列满无法往里面塞mail计数]
	u32 MailTimeout;	//[报文没发出去，需控制列表强制操作]
	s32 Mails2Send;	//[目前在节点内的报文数]
	Can_SendQueueTypedef SentQueue;	//[发送队列结构体]
}MsgCtrlGrpTypedef;

extern Can_DatasendTypeDef Can_Datasend;
extern Can_SendQueueTypedef CAN1_SendQueue,CAN2_SendQueue,VESC_SendQueue,ZDrive_SendQueue;


void Can_SendQueueInit(void);
bool CanQueue_ifEmpty(Can_SendQueueTypedef* can_queue);
bool CanQueue_ifFull(Can_SendQueueTypedef* can_queue);
void Can_DeQueue(CAN_TypeDef* CANx,Can_SendQueueTypedef* can_queue);

void MesgCtrlList_Init(MsgCtrlGrpTypedef* CtrlList, u8 CAN_x);
void Mail_Queue2CtlList(MsgCtrlGrpTypedef* CtrlList,Can_SendQueueTypedef* can_queue,u8 CAN_x);
void CtrlList_Func(MsgCtrlGrpTypedef* CtrlList, Can_SendQueueTypedef* can_queue,u8 CAN_x );

extern MsgCtrlGrpTypedef Can1MsgCtrllist[CAN1_NodeNumber],Can2MsgCtrllist[CAN2_NodeNumber];

#endif
