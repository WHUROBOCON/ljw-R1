#include "switch.h"

void Switch_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	Set9_ON();
	Reset9_OFF();
}

int m=0xff,n=0xff;
void Switch_Ctrl(void)
{
	m=GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9);
	n=GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10);
	Valve_Ctrl(255);
}
