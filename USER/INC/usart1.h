#ifndef _USART1_H
#define _USART1_H

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"
#include "includes.h"
#include "param.h"


void USART1_Configuration(void);
void USART1_sendData(u8 *a,u8 count);


#endif
