#ifndef _LED_H
#define _LED_H

#include "sys.h"
#include "stm32f4xx_rcc.h"
#include "includes.h"

#define LED_RED           PIN4
#define LED_YELLOW        PIN5
#define LED_BLUE          PIN6
#define LED_GREEN         PIN7

#define LED_RED_OFF  			GPIOA->BSRRL = PIN4		//����ߵ�ƽ
#define LED_YELLOW_OFF  		GPIOA->BSRRL = PIN5
#define LED_BLUE_OFF    	    GPIOA->BSRRL = PIN6
#define LED_GREEN_OFF  			GPIOA->BSRRL = PIN7

#define LED_RED_ON   			GPIOA->BSRRH = PIN4		//����͵�ƽ��
#define LED_YELLOW_ON   		GPIOA->BSRRH = PIN5
#define LED_BLUE_ON   			GPIOA->BSRRH = PIN6
#define LED_GREEN_ON   			GPIOA->BSRRH = PIN7

#define LED_RED_TOGGLE			GPIOA->ODR ^= PIN4		//toggle�л�
#define LED_YELLOW_TOGGLE 	    GPIOA->ODR ^= PIN5
#define LED_BLUE_TOGGLE			GPIOA->ODR ^= PIN6
#define LED_GREEN_TOGGLE		GPIOA->ODR ^= PIN7

void LED_Configuration(void);
void Led_Show(void);
void LedDisplay(u8 led, u8 num);

#endif
