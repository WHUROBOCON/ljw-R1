#ifndef __USART2_H
#define __USART2_H

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"

#include "param.h"
#include "vesc.h"
#include "command.h"
#include "beep.h"
#include "delay.h"
#include "mech.h"

	#define lcd_Button_Set(pic_id)            \
{																			\
	usart.TxBuffer_USART2[i++]=0xee;			\
	usart.TxBuffer_USART2[i++]=0xb1;			\
	usart.TxBuffer_USART2[i++]=0x10;			\
	usart.TxBuffer_USART2[i++]=0x00;		\
	usart.TxBuffer_USART2[i++]=pic_id;	\
}



#define lcd_Head(pic_id)            \
{																			\
	usart.TxBuffer_USART2[i++]=0xee;			\
	usart.TxBuffer_USART2[i++]=0xb1;			\
	usart.TxBuffer_USART2[i++]=0x12;			\
	usart.TxBuffer_USART2[i++]=0x00;		\
	usart.TxBuffer_USART2[i++]=pic_id;	\
}

#define lcd_End()            \
{																			\
	usart.TxBuffer_USART2[i++]=0xff;			\
	usart.TxBuffer_USART2[i++]=0xfc;			\
	usart.TxBuffer_USART2[i++]=0xff;			\
	usart.TxBuffer_USART2[i++]=0xff;		\
}


#define lcd_text_update(sub_id, str, type)            \
{                                                 \
	usart.TxBuffer_USART2[i++] = 0x00;                     \
	usart.TxBuffer_USART2[i++] = (u8)sub_id;               \
	usart.TxBuffer_USART2[i++] = 0x00;                     \
	sprintf(str_temp, type, str);                 \
	usart.TxBuffer_USART2[i++] = strlen(str_temp);         \
	strcpy((char *)(&usart.TxBuffer_USART2[i]), str_temp); \
	i += strlen(str_temp);                        \
}

#define lcd_button_update(sub_id, status) \
{                                     \
	usart.TxBuffer_USART2[i++] = 0x00;         \
	usart.TxBuffer_USART2[i++] = (u8)sub_id;   \
	usart.TxBuffer_USART2[i++] = 0x00;         \
	usart.TxBuffer_USART2[i++] = 1;            \
	usart.TxBuffer_USART2[i++] = status;       \
}

void USART2_Configuration(void);
void UsartLCDshow(void);

#endif
