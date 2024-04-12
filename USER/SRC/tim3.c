#include "tim3.h"
void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure; 
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	
	
	TIM_TimeBaseInitStructure.TIM_Period=	(TIM3_Cycle*1000)-1;	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84-1;		
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	/*  Tout=(1000*84)/84M=1ms  
	Tout= ((arr+1)*(psc+1))/Tclk£»	arr=9000 prs=8
	*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM3,ENABLE); 
}


float Time_cnt=0; //¼ÆÊ±Æ÷
float Start_cnt=0;
float Tclk = 0;
void TIM3_IRQHandler(void)	//????DJFunc,VESCFunc,TmotorFunc(??????)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
  {
		Tclk += 0.001f;
		static int VESCCount = 0;
		#ifdef USE_DJ
			DJFunc();
		#endif
		#ifdef USE_VESC
			if((VESCCount++)%10 == 1)		// 10ms
			{
				VESCFunc();
			}
		#endif
		#ifdef USE_AK
			TmotorFunc();
		#endif
		#ifdef USE_ZDRIVE
			static int ZDriveCount;
			if((ZDriveCount++)%40==1)		//5ms1??
			{
				ZdriveFunction();
			}
		#endif
  }
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //???????
}
