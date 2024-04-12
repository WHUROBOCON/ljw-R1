#include "can2.h"
int max;
void CAN2_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);
 	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);

  CAN_InitStructure.CAN_TTCM=DISABLE;  									//��ʱ�䴥��ͨ��ģʽ
	CAN_InitStructure.CAN_ABOM=ENABLE;  									//�����CAN_MCR�Ĵ�����INRQλ��1�������0��һ����⵽128������11λ������λ�����˳�����״̬
	CAN_InitStructure.CAN_AWUM=DISABLE;  									//˯��ģʽ���������
	CAN_InitStructure.CAN_NART=DISABLE;  									//��ֹ�����Զ����ͣ���ֻ����һ�Σ����۽�����
	CAN_InitStructure.CAN_RFLM=DISABLE;  									//���Ĳ��������µĸ��Ǿɵ�
	CAN_InitStructure.CAN_TXFP=DISABLE;  									//����FIFO�����ȼ��ɱ�ʶ������
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;						//CANӲ������������ģʽ

	/* Seting BaudRate */	
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;									//����ͬ����Ծ���Ϊһ��ʱ�䵥λ
	CAN_InitStructure.CAN_BS1=CAN_BS1_9tq; 									//ʱ���1ռ��8��ʱ�䵥λ
	CAN_InitStructure.CAN_BS2=CAN_BS2_4tq;									//ʱ���2ռ��7��ʱ�䵥λ
	CAN_InitStructure.CAN_Prescaler=3; 										//��Ƶϵ����Fdiv��
	CAN_Init(CAN2, &CAN_InitStructure); 									//��ʼ��CAN1
	//can_BaudRate=42M/��1+CAN_BS1+CAN_BS2��/CAN_Prescaler=42000/��1+9+4��/3 = 1Mbps
	//F4��CAN1��CAN2�ڵ���APB1�ϣ�ϵͳƵ��Ϊ42MHZ.����ʱ��APB2Ϊ84MHZ.��F1��F4ʱ�ӵ�Ƶ���ǲ�һ����
	
	/*DJ������*/
	CAN_FilterInitStructure.CAN_FilterNumber = 14;		
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;		
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;	
	CAN_FilterInitStructure.CAN_FilterIdHigh =DJ_MOTOR1_RX<< 5;				
	CAN_FilterInitStructure.CAN_FilterIdLow =DJ_MOTOR2_RX << 5;		
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =DJ_MOTOR3_RX << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =DJ_MOTOR4_RX << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;		
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;		
	CAN_FilterInit(&CAN_FilterInitStructure);	
	
	CAN_FilterInitStructure.CAN_FilterNumber = 20;		
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;		
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;	
	CAN_FilterInitStructure.CAN_FilterIdHigh =DJ_MOTOR5_RX<< 5;				
	CAN_FilterInitStructure.CAN_FilterIdLow =DJ_MOTOR6_RX << 5;		
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh =DJ_MOTOR7_RX << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =DJ_MOTOR8_RX << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;		
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;		
	CAN_FilterInit(&CAN_FilterInitStructure);	
	
		/*VESC������*/
	CAN_FilterInitStructure.CAN_FilterNumber = 15;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = ((((uint32_t)CAN_PACKET_STATUS << 8) << 3) & 0xffff0000) >> 16;
	CAN_FilterInitStructure.CAN_FilterIdLow = (((uint32_t)CAN_PACKET_STATUS << 8) << 3) & 0xffff;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (0xffffff00 << 3) >> 16;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = (0xffffff00 << 3) & 0xffff;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
		/*AK��ZDrive������*/
	CAN_FilterInitStructure.CAN_FilterNumber = 16;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = ZDriveRX << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = ZDriveRX << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = ZDriveRX << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = ZDriveRX << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO1;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=CAN2_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=CAN2_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	CAN_ITConfig(CAN2,CAN_IT_FMP0, ENABLE);		
	CAN_ITConfig(CAN2,CAN_IT_FMP1, ENABLE);	
}

static u32 firstPos = 0;
void CAN2_RX0_IRQHandler(void)
{
  if (CAN_GetITStatus(CAN2, CAN_IT_FMP0) != RESET) 	//FIFO 0 message pending Interrupt
  {
	  CanRxMsg rx_message;
	  CAN_Receive(CAN2, CAN_FIFO0, &rx_message);
	  CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);		//��ռĴ���
	  CAN_ClearFlag(CAN2, CAN_IT_FMP0);
		
		if( (rx_message.StdId>=DJ_MOTOR1_RX) && (rx_message.StdId<=DJ_MOTOR8_RX) ) //�޸�������ʵ��
		{
			unsigned char id=rx_message.StdId-0x201;
			DJmotor[id].valueReal.pulseRead = (rx_message.Data[0]<<8 ) | (rx_message.Data[1]);
			DJmotor[id].valueReal.speed = (rx_message.Data[2] << 8) | (rx_message.Data[3]);		//��������������ת��			
			if(ABS(DJmotor[id].valueReal.speed)>max)
			{
				max=ABS(DJmotor[id].valueReal.speed);
			}
			DJmotor[id].valueReal.current=(rx_message.Data[4] << 8) | (rx_message.Data[5]);
			DJmotor[id].valueReal.temperature = rx_message.Data[6];	
			
			DJ_Position_Calculate(id);
	
			DJmotor[id].argum.lastRxTime = 0;
			
			if(firstPos++==10)
			{
				for(u8 i=0;i<=7;i++)		// 1 2 3
				{
					if(i==7)
						DJ_SetZero(i,-180);
					else if(i==5)
						DJ_SetZero(i,-59);
					else
						DJ_SetZero(i,0);
					DJmotor[i].enable = 0;
					DJmotor[i].mode = DJPOSITION;
					DJmotor[i].begin = 1;
				}
			}
		}
	}
}

void CAN2_RX1_IRQHandler(void)
{
	if (CAN_GetITStatus(CAN2, CAN_IT_FMP1) != RESET)
	{
		CanRxMsg rx_message;
		CAN_Receive(CAN2, CAN_FIFO1, &rx_message);
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
		CAN_ClearFlag(CAN2, CAN_IT_FMP1);
		#ifdef USE_VESC
		if(rx_message.ExtId>=0x00000901&&rx_message.ExtId<=0x00000908)
			VESCReceiveHandler(rx_message);
		#endif
		#ifdef USE_AK
		if(rx_message.StdId>=0x00000001&&rx_message.StdId<=0x00000008)
			TmotorReceiveHandler(rx_message);
		#endif
		#ifdef USE_ZDRIVE
		if(rx_message.StdId>=0x01&&rx_message.StdId<=0x08)
			ZdriveReceiveHandler(rx_message);
		#endif
	}
}
