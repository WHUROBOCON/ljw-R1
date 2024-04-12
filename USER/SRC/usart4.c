#include "usart4.h"
void USART4_Configuration ()
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	DMA_InitTypeDef 	DMA_InitStructure;

	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd (RCC_APB1Periph_UART4, ENABLE);
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_DMA1, ENABLE);

	GPIO_PinAFConfig (GPIOA, GPIO_PinSource0, GPIO_AF_UART4);
	GPIO_PinAFConfig (GPIOA, GPIO_PinSource1, GPIO_AF_UART4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init (GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init (UART4, &USART_InitStructure);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);          //�����ж�
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_DeInit (DMA1_Stream4);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_BufferSize = 0;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(usart.TxBuffer_USART4);		//�� ���û�� ���Ǵ��ڶ�
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&UART4->DR);
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//����Ϊf4����
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_Init (DMA1_Stream4, &DMA_InitStructure);
	
	DMA_ClearFlag (DMA1_Stream4, DMA_IT_TCIF4);					//����жϱ�־
	DMA_ClearITPendingBit (DMA1_Stream4, DMA_IT_TCIF4);
	DMA_ITConfig (DMA1_Stream4, DMA_IT_TC,ENABLE);//�ر���DMA��������жϣ����������޷����䡣10/14/2018
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init (&NVIC_InitStructure);

	USART_DMACmd (UART4, USART_DMAReq_Tx, ENABLE);
	USART_Cmd (UART4, ENABLE);
}

void DMA1_Stream4_IRQHandler (void)//���ݴ�����ɣ������жϣ�����Ƿ���û�д�������ݣ���������
{
	if(DMA_GetITStatus (DMA1_Stream4, DMA_IT_TCIF4) == SET)
	{
		DMA_ClearFlag (DMA1_Stream4, DMA_IT_TCIF4);//����жϱ�־
		DMA_ClearITPendingBit (DMA1_Stream4, DMA_IT_TCIF4);
		SystemFlag.Usart4DMASendFinish = 0;
	}
}

static u8 USART4_RX_STA = 0;     //����״̬���


u32 a,b,c;
void UART4_IRQHandler (void)
{
	u8 temp = 0;

		if(USART_GetITStatus (UART4, USART_IT_RXNE) != RESET) //���ռĴ����ǿ�
  {
		USART_ClearFlag(UART4, USART_FLAG_RXNE);
		USART_ClearITPendingBit (UART4, USART_IT_RXNE);
		temp = USART_ReceiveData(UART4);
		
    if((USART4_RX_STA & 0x40) != 0) //�����Ѿ���ʼ
	//		 ����״̬���
    {
			if((USART4_RX_STA & 0x80) == 0) //����δ���
      {
				if(temp == 0x0a)		//��β��0x0a
					USART4_RX_STA |= 0x80;  //���������
				
        else //���ڽ���
        {
					usart.RxBuffer_USART4[USART4_RX_STA&0X3F] =temp;
					// STA = 0x 01xx xxxx ��6λ������������
					
          USART4_RX_STA++;
					
          if((USART4_RX_STA & 0X3F) > 32)		//32�ǽ�������ĳ���
						USART4_RX_STA = 0; //�������ݴ������¿�ʼ����
					
        }
				
      }
    }
		
    else if(temp == '#')    //���յ���ͷ
      USART4_RX_STA |= 0x40;		// 0100 0000

    if((USART4_RX_STA & 0x80) != 0)     //�����Ѿ���ɣ�������������
		{
			
			if(mech.WheelTeleoperate.flag.USECAM)
			{
				if(usart.RxBuffer_USART4[0] == 'P'&&usart.RxBuffer_USART4[1] == 'O')		//��ͷ��#pu
				{
					mech.WheelTeleoperate.flag.connected[6] = 1;
					mech.POTCordself.CameraANG[6] = (atof((char *)(&usart.RxBuffer_USART4[2])))/10;
						mech.POTCordself.status[6] = (atof((char *)(&usart.RxBuffer_USART4[6])));
					
					mech.WheelTeleoperate.flag.connected[4] = 1;
					mech.POTCordself.CameraANG[4] = (atof((char *)(&usart.RxBuffer_USART4[8])))/10;
						mech.POTCordself.status[4] = (atof((char *)(&usart.RxBuffer_USART4[12])));
				}
			}
			
			else if(mech.WheelTeleoperate.flag.USERAD)
			{
				if(usart.RxBuffer_USART4[0] == 'R'&&usart.RxBuffer_USART4[1] == 'P')		//��ͷ��#pu
				{
					u8 ID = usart.RxBuffer_USART4[2];
					mech.WheelTeleoperate.flag.connected[ID] = 1;
					Char2float(&mech.PotCordDR.x[ID], &usart.RxBuffer_USART4[3]);
					Char2float(&mech.PotCordDR.y[ID], &usart.RxBuffer_USART4[7]);
				}
			}


			USART4_RX_STA = 0;
			memset(usart.RxBuffer_USART4, 0, sizeof(usart.RxBuffer_USART4));	
		}
	}
}


void QP_PotPosition(void) //��λ��
{
	if (0 == SystemFlag.Usart4DMASendFinish) //����һ�δ���δ��ɣ����������δ���
	{
		usart.TxBuffer_USART4[0] = '#';
		usart.TxBuffer_USART4[1] = 'P';
		usart.TxBuffer_USART4[2]= 'O';
		usart.TxBuffer_USART4[3] = 0x0a;

		DMA_SetCurrDataCounter(DMA1_Stream4, 4);	//����DMA���ݴ���Ĵ�С
		DMA_Cmd(DMA1_Stream4, ENABLE);
    SystemFlag.Usart4DMASendFinish = 1;
	}
}
