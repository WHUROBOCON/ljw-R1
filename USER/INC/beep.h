#ifndef _BEEP_H
#define _BEEP_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "delay.h"
#include "param.h"

#define BEEP_OFF     	 GPIOA->BSRRH = GPIO_Pin_8		//输出低电平
#define BEEP_ON     	 GPIOA->BSRRL = GPIO_Pin_8		//输出高电平
#define BEEP_TOGGLE	   GPIOA->ODR ^= GPIO_Pin_8		//这个是什么意思	之前试了一次，效果就是一直响
#define beep_show(x) B1p.BeepOnNum += (x)
#define beep_clear() B1p.BeepOnNum = 0

void Beep_Configuration(void);
void Beep_Show(u8 num);
void Beep_Start(void);
void BeepTask(void);

#endif
