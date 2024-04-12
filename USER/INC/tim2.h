#ifndef _TIM2_H
#define _TIM2_H

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#include "macro.h"
#include "param.h"
//#include "queue.h"
//#include "canctrllist.h"

// TIM3 中 DJ每1ms塞一条 AK80每1ms塞一条  每0.4s出队一条
#define TIM2_Cycle 0.4 //单位ms

void TIM2_Configuration(void);

#endif
