#ifndef _TIM2_H
#define _TIM2_H

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#include "macro.h"
#include "param.h"
//#include "queue.h"
//#include "canctrllist.h"

// TIM3 �� DJÿ1ms��һ�� AK80ÿ1ms��һ��  ÿ0.4s����һ��
#define TIM2_Cycle 0.4 //��λms

void TIM2_Configuration(void);

#endif
