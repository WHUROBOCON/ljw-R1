#include "stm32f4xx.h"
#include "includes.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "cylinder.h"



#define Set9_ON() 	GPIO_ResetBits(GPIOA, GPIO_Pin_9)
#define Reset9_OFF()	GPIO_SetBits(GPIOA, GPIO_Pin_9)
#define Set10_ON() 	GPIO_ResetBits(GPIOA, GPIO_Pin_10)
#define Reset10_OFF()	GPIO_SetBits(GPIOA, GPIO_Pin_10)


void Switch_Init(void);
void Switch_Ctrl(void);
