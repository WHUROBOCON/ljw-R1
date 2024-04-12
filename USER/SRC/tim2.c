#include "tim2.h"

void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitStructure.TIM_Period = (TIM2_Cycle*1000)-1; //0.4ms
	TIM_TimeBaseInitStructure.TIM_Prescaler =84-1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	//tout = 100*84/84Mhz = 0.1ms

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

u32 VESC_Dequeue_Cnt=0;
u32 ZDrive_Dequeue_Cnt=0;
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		Can_DeQueue(CAN2,&CAN2_SendQueue);
		Can_DeQueue(CAN1,&CAN1_SendQueue);
		#ifdef USE_VESC
		Can_DeQueue(CAN2,&VESC_SendQueue);
		#endif
		Can_DeQueue(CAN2,&ZDrive_SendQueue);
	}
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
