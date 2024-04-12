#include "usart3.h"

void USART3_Configuration()
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //接受中断

	//配置NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_DeInit(DMA1_Stream3);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_BufferSize = 0;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(usart.TxBuffer_USART3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //以下为f4特有
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_Init(DMA1_Stream3, &DMA_InitStructure);

	DMA_ClearFlag(DMA1_Stream3, DMA_IT_TCIF3); //清除中断标志
	DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE); //关闭了DMA传输完成中断，导致数据无法传输。10/14/2018

	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);

	//使能USART
	USART_Cmd(USART3, ENABLE);
}

void DMA1_Stream3_IRQHandler(void) //数据传输完成,产生中断,检查是否还有没有传输的数据，继续传输
{
	if (DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3) == SET)
	{
		DMA_ClearFlag(DMA1_Stream3, DMA_IT_TCIF3); //清除中断标志
		DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
		SystemFlag.Usart3DMASendFinish = 0;
	}
}

u8 Check_Sum(u8* buff,u8 len)
{
		u8 sum=0;
		for(int i=0;i<len;i++)
			sum+=buff[i];
		return sum;
}

float cnt=0;
u8 USART3_RX_STA=0;

bool Shoot=0,Enable=0,Disable=0,Begin=0;
float FireV=0;
void USART3_IRQHandler(void)
{
	u8 temp = 0;
	if (USART_GetITStatus(USART3, USART_IT_ORE_RX) != RESET)
	{
		USART_ReceiveData(USART3);
		USART3_RX_STA = 0;
		memset(usart.RxBuffer_USART3, 0, sizeof(usart.RxBuffer_USART2));
	}
	else if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		cnt+=1;
		USART_ClearFlag(USART3, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		temp = USART_ReceiveData(USART3);	//一次读八位
		
		if ((USART3_RX_STA & 0x40) != 0)	//之前接收到包头，但还没读取完成（STA未置0）
		{
			if ((USART3_RX_STA & 0x80) == 0) //没接收到包尾
			{
				if (temp == 0x5A)
					USART3_RX_STA |= 0x80; //接收到包尾，将STA最高位置1
				else
				{
					usart.RxBuffer_USART3[USART3_RX_STA & 0X3F] = temp;	//将temp存入串口接收数组,0是除包头外的第一位
					USART3_RX_STA++;
					if ((USART3_RX_STA & 0X3F) > 0x40)
						USART3_RX_STA = 0;
				}
			}
		}
		else if (temp == 0xA5)	//接收到包头，将STA第二位置1
			USART3_RX_STA |= 0x40;
		
		if((USART3_RX_STA & 0x80) != 0) //接收完成，进行处理
		{
				if(usart.RxBuffer_USART3[5]!=Check_Sum(usart.RxBuffer_USART3,5)) //check failed
				{
						USART3_RX_STA=0;
						return;
				}

				u8 tempgroupbool=usart.RxBuffer_USART3[0];
				mech.FLAG.ShootFlag=tempgroupbool&0x01;
				Enable=tempgroupbool&0x02;
				Disable=tempgroupbool&0x04;
				Begin=tempgroupbool&0x08;
				
				memcpy(&FireV,&usart.RxBuffer_USART3[1],4);	
				DJmotor[4].valueSet.angle=FireV;
				
				USART3_RX_STA=0;
		}
	}
}

//void USART3_Send(void)
//{
//	if (!SystemFlag.Usart3DMASendFinish) //若上一次传输未完成，则舍弃本次传输
//	{
//		DMA_SetCurrDataCounter(DMA1_Stream3, count);
//		DMA_Cmd(DMA1_Stream3, ENABLE);
//		SystemFlag.Usart3DMASendFinish = 1;
//	}
//}
