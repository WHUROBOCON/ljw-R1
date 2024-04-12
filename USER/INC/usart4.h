#ifndef __USART4_H
#define __USART4_H

#include "string.h"
#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"
#include "param.h"
#include "mech.h"
#include "mathFunc.h"
#include <math.h>
#include "macro.h"


void USART4_Configuration (void);
void QP_PotPosition(void);

#endif
