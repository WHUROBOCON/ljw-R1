#ifndef __CAN2_H
#define __CAN2_H

#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "includes.h"

#include "macro.h"
#include "vesc.h"
#include "DJmotor.h"
#include "elmo.h"
#include "Tmotor.h"
#include "zdrive.h"

void CAN2_Init(void);

#endif
