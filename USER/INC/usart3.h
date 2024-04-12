#ifndef __USART3_H
#define __USART3_H

#include "dma.h"
#include "sys.h"
#include "stdio.h"
#include "string.h"
#include "param.h"
#include "stm32f4xx_usart.h"
#include "beep.h"
#include "mech.h"
void USART3_Configuration(void);
void USART3_RecHandler(void);
void USART3_Send(void);
u8 Check_Sum(u8* buff,u8 len);

#endif
