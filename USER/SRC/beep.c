#include "beep.h"
void Beep_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	BEEP_OFF;
}

/*BB响开场特效*/
void Beep_Start(void)
{
	BEEP_ON;
	Delay_ms(60);
	BEEP_OFF;
	Delay_ms(60);
	BEEP_ON;
	Delay_ms(120);
	BEEP_OFF;
	Delay_ms(120);
	BEEP_ON;
	Delay_ms(240);
	BEEP_OFF;
	Delay_ms(240);
}

void BeepTask(void)
{
	for(;;)
	{
		if(B1p.AllowFlag)
		{
			if(B1p.BeepOnNum)
			{
				//BEEP_ON;
				OSTimeDly(2000);
				//BEEP_OFF;
				OSTimeDly(2000);
				B1p.BeepOnNum--;
			}

		}
		else
		{
			B1p.BeepOnNum = 0;
		}
			
		OSTimeDly(500);
	}
}
