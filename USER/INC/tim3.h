#ifndef __TIM3_H
#define __TIM3_H

#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#include "macro.h"
#include "DJmotor.h"
#include "VESC.h"
#include "Tmotor.h"
#include "zdrive.h"
#include "cylinder.h"

#define TIM3_Cycle 1 //µ¥Î»ms

extern float Start_cnt;
extern float Time_cnt;
extern float Tclk;

void TIM3_Configuration(void);

#endif
