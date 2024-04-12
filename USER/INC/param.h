#ifndef _PARAM_H_
#define _PARAM_H_

#include "stm32f4xx.h"
#include "stdbool.h"

#include "macro.h"
#include "DJmotor.h"
#include "Tmotor.h"
#include "elmo.h"
#include "led8.h"

#define true 1
#define false 0


/*�����ṹ��*/
typedef struct{
  u8 TxBuffer_USART1[USART1_Tx_BufferSize]; //����һ��������
  u8 RxBuffer_USART1[USART1_Rx_BufferSize]; //����һ��������

  u8 TxBuffer_USART2[USART2_Tx_BufferSize]; //���ڶ���������
  u8 RxBuffer_USART2[USART2_Rx_BufferSize]; //���ڶ���������

	u8 TxBuffer_USART3[USART3_Tx_BufferSize]; //���ڶ���������
  u8 RxBuffer_USART3[USART3_Rx_BufferSize]; //���ڶ���������
	
  u8 TxBuffer_USART4[USART4_Tx_BufferSize]; //��������������
  u8 RxBuffer_USART4[USART4_Rx_BufferSize]; //��������������
} Usart_Struct;

typedef struct{	
    volatile bool Can1SendqueueEMPTY;   // CAN����Ϊ��
		volatile bool Can2SendqueueEMPTY;
	
    volatile bool Can1SendqueueFULL;   // CAN1���Ͷ�����
    volatile bool Can2SendqueueFULL;   // CAN2���Ͷ�����

		volatile bool Can1TxMailFULL;		//CAN����no mail box
		volatile bool Can2TxMailFULL;
	
		volatile bool Can1MsgCtrllistEnable;
		volatile bool Can2MsgCtrllistEnable;
	
    volatile bool Usart1DMASendFinish; // Usart1_DMA����
    volatile bool Usart2DMASendFinish; // Usart2_DMA����
		volatile bool Usart3DMASendFinish; // Usart3_DMA����
} FlagTypeDef;

typedef struct{
	bool AllowFlag;
	u16 BeepOnNum;
}BeepStructTypedef;

typedef struct{
	volatile bool Normal;
	volatile bool Stuck;
	volatile bool Timeout;
	volatile bool CANErr;
	volatile bool CloseAll;
}LEDTaskTypedef;

//typedef struct{
//	u8 io;
//	u8 status;
//	bool GO;
//	bool LidPress;
//	bool LidLift;
//	bool LockTrigger;
//	bool PullTrigger;
//}TempStructTypedef;


/*�������𴦵��á�*/
extern Usart_Struct usart;
extern FlagTypeDef SystemFlag;

extern BeepStructTypedef  B1p;
extern LEDTaskTypedef LedTask;
//extern TempStructTypedef Temp;

//extern OS_EVENT *CatchSem;
//extern INT8U *ErrorCatchSem;

//extern OS_EVENT *LeftHandSem;
//extern INT8U *ErrorLeftHandSem;

//extern OS_EVENT *RightHandSem;
//extern INT8U *ErrorRightHandSem;

//extern OS_EVENT *LoadSem;
//extern INT8U *ErrorLoadSem;

extern float start_t;
//extern float Rcontinue_t;
//extern float Lcontinue_t;
//extern float Wheel_backSP;

/*��������*/
void param_Init(void);
void TR_PARM_Init(void);

#endif
