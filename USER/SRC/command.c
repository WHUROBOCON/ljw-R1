#include "command.h"

//void Check_DUOJ ( u8 Mail_If2CtrlList)
//{
//	
//	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
//	{
//		SystemFlag.Can1SendqueueFULL ++;
//		return;
//	}
//	
//	else
//	{																											
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'C';
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'D';
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 'J';
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 0;		
//		
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
//	}
//	CAN1_SendQueue.Rear=IncCAN1Rear;	
//}

void PB0_ON ( u8 Mail_If2CtrlList)
{
	
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{																											
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'B';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 0;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 1;		
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PB0_OFF ( u8 Mail_If2CtrlList)
{
	
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'B';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 0;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 0;
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PB1_ON ( u8 Mail_If2CtrlList)
{
	
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'B';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 1;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 1;		
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PB1_OFF ( u8 Mail_If2CtrlList)
{
	 
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'B';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 1;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 0;	
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PB4_ON ( u8 Mail_If2CtrlList)
{
	 
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'B';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 4;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 1;	
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PB4_OFF ( u8 Mail_If2CtrlList)
{
	 
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'B';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 4;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 0;	
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PB5_ON ( u8 Mail_If2CtrlList)
{
	 
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'B';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 5;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 1;	
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PB5_OFF ( u8 Mail_If2CtrlList)
{
	 
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'B';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 5;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 0;	
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PB6_ON ( u8 Mail_If2CtrlList)
{
	 
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'B';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 6;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 1;	
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PB6_OFF ( u8 Mail_If2CtrlList)
{
	 
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'B';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 6;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 0;	
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PB7_ON ( u8 Mail_If2CtrlList)
{
	 
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'B';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 7;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 1;	
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PB7_OFF ( u8 Mail_If2CtrlList)
{
	 
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'B';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 7;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 0;	
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PA1_ON ( u8 Mail_If2CtrlList)
{
	
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'A';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 1;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 1;
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PA1_OFF ( u8 Mail_If2CtrlList)
{
	
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'A';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 1;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 0;	
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PA2_ON ( u8 Mail_If2CtrlList)
{
	
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'A';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 2;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 1;		
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PA2_OFF ( u8 Mail_If2CtrlList)
{
	
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'A';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 2;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 0;		
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PA3_ON ( u8 Mail_If2CtrlList)
{
	
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'A';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 3;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 1;		
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PA3_OFF ( u8 Mail_If2CtrlList)
{
	
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'A';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 3;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 0;
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PA6_ON ( u8 Mail_If2CtrlList)
{
	
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'A';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 6;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 1;		
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PA6_OFF ( u8 Mail_If2CtrlList)
{
	
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'A';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 6;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 0;
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PA7_ON ( u8 Mail_If2CtrlList)
{
	
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'A';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 7;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 1;

		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void PA7_OFF ( u8 Mail_If2CtrlList)
{
	
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'P';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'A';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 7;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 0;	
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void F1_CHECK ( u8 Mail_If2CtrlList)
{
	
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X04;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 'C';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = 'W';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[2] = 'H';
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[3] = 'U';
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}

void Ask_Magnet_Status (char Com,u8 num,u8 Mail_If2CtrlList)
{
	 if(IncCAN1Rear==CAN1_SendQueue.Front)	
	{
		SystemFlag.Can1SendqueueFULL ++;
		return;
	}
	
	else
	{
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID = Master2DuoJi_TX;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC = 0X02;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = Com;
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[1] = num;
		
		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList = Mail_If2CtrlList;		
	}
	CAN1_SendQueue.Rear=IncCAN1Rear;	
}
