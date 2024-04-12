#ifndef __CAN1_H
#define __CAN1_H

#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#include "macro.h"
#include "queue.h"
#include "command.h"
#include "vesc.h"
#include "mech.h"
#include "mathFunc.h"

extern int scoopCase;

void CAN1_Init(void);
static void ResponseMaterEnableStatus(CanRxMsg rx_message);
static void ResponseMaterZerofindingStatus(CanRxMsg rx_message);
static void ResponseMaterPositionStatus(CanRxMsg rx_message);
static void ResponseMaterAngleStatus(CanRxMsg rx_message);
static void ResponseMaterCYLStatus(CanRxMsg rx_message);

#endif
