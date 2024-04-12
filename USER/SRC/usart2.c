#include "usart2.h"

u8 USART2_RX_STA = 0;

void USART2_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
  DMA_InitTypeDef 	DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//ʹ��GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ�ܴ���2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	//ʹ��DMA1
	
	//��PA2\PA3����ΪUSART2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //�����ж�

	//����NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;		//�͸���һ���ж����ȼ��ͽ���ȥ�ж��ˣ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_DeInit(DMA1_Stream6);
	DMA_InitStructure.DMA_Channel =DMA_Channel_4;
	DMA_InitStructure.DMA_BufferSize =0;
	DMA_InitStructure.DMA_DIR =DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)(usart.TxBuffer_USART2);
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&USART2->DR);
	DMA_InitStructure.DMA_MemoryInc =DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralInc =DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryDataSize =DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority =DMA_Priority_VeryHigh;//����Ϊf4����
	DMA_InitStructure.DMA_FIFOMode =DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold =DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_PeripheralBurst =DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_MemoryBurst =DMA_MemoryBurst_Single;
	DMA_Init(DMA1_Stream6,&DMA_InitStructure);
	
	DMA_ClearFlag (DMA1_Stream6,DMA_IT_TCIF6);					//����жϱ�־
	DMA_ClearITPendingBit(DMA1_Stream6,DMA_IT_TCIF6);
	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);//�ر���DMA��������жϣ����������޷����䡣10/14/2018
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;			//�����ң���޷������ذ巢��Ϣ������Ϊ���ȼ����ͣ�7/1��		11/16/2020
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);

	USART_Cmd(USART2, ENABLE);
}

void DMA1_Stream6_IRQHandler(void) //���ݴ�����ɣ������жϣ�����Ƿ���û�д�������ݣ���������
{
  if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) == SET) 
  {
    DMA_ClearFlag(DMA1_Stream6, DMA_IT_TCIF6); //����жϱ�־
    DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
    SystemFlag.Usart2DMASendFinish = 0;
  }
}
vu8 interface = 0;
int ELMO_ID = 0;	//�����ж����涨�壬����ÿ�ν��ж�ֵ���ᱻ���

void USART2_IRQHandler(void)
{
	u8 temp =0;
	if(USART_GetITStatus(USART2,USART_IT_ORE_RX)!=RESET ) 
	{
		temp = USART_ReceiveData(USART2);
		USART2_RX_STA = 0;
		memset(usart.RxBuffer_USART2, 0, sizeof(usart.RxBuffer_USART2) );	//�Ƚ�RXBUFFER����
	}
	else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET )
	{
		
		USART_ClearFlag(USART2,USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	//�����־λ
		
		temp = USART_ReceiveData(USART2);	//��ʼ��������,�ݴ浽��ʱ����temp��
		
		if( (USART2_RX_STA & 0X40)!=0 )	//��־�Ĵ����ĵ���λ������0�����տ�ʼ
		{
			if( (USART2_RX_STA & 0x80)==0 )	//��־λ�ĵ���λ����0������δ���
			{
				if (temp ==0xff)	//֡β��������ɣ�����ָ���ж���
					USART2_RX_STA |= 0X80;
				
				else
				{
					usart.RxBuffer_USART2[USART2_RX_STA&0X3F] =temp;	//�洢
					USART2_RX_STA++;  
					
					if((USART2_RX_STA & 0X3F)>40 )	//�������շ�Χ
						USART2_RX_STA=0;
				}
				
			}
		
		}
		
		else if(temp == 0xee)		//֡ͷ���գ�׼����ʼ��������	���մ����������ذ巢�͹�ȥ������������һ���ֽڵģ���Ϊ��һ��0xee��ֱ�ӱ�if����
		USART2_RX_STA |= 0X40;
	
	
/**
 *@brief	attention!attention! ��������ʼ��
*/
		if( ( (USART2_RX_STA&0X80)!=0 ) )		//������ָ���ж�
		{
					
			 if (usart.RxBuffer_USART2[0] == 0x12) //��ʵ�������
      {
    switch (usart.RxBuffer_USART2[1])
        {
        case 0:			//���½�
					mech.WheelTeleoperate.flag.FIRE = 1;
          break;
				
				case 1:			//���½����
					DJmotor[0].enable = 0;
					DJmotor[1].enable = 0;
					DJmotor[2].enable = 0;
					VESCmotor[0].enable = 0;
					VESCmotor[1].enable = 0;
				break;
				
				 case 2:
					DJmotor[0].enable = 1;
					DJmotor[1].enable = 1;
					DJmotor[2].enable = 1;
					VESCmotor[0].enable = 1;
					VESCmotor[1].enable = 1;
         break;
				
				case 3:
					
					break;
				
				case 4:
					
          break;
				
				case 5:
					
					break;
				
				case 6:
					
          break;
				
				case 7:
					
					break;
				
        case 8:
					
          break;
				
				case 9:
					
					break;
				
				 case 10:
					
          break;
				
				case 11:
					
					break;
				
				case 12:
					
          break;
				
				case 13:
					
					break;
				
				case 14:
					
          break;
				
				case 15:
					
					break;

        default:
          break;
        }
      }
			
			//�������һ����ӭ���棬Ӧ��ûʲôӰ�죬��ΪLCDshow�Ƿ���������ģ�����Ƶ�ʺܸߣ�����һ��ʼ�������Ӧ��û��ϵ
			/*ҳ��ѡ������֮��������*/
			if(usart.RxBuffer_USART2[1]==0x01)		//�л�����ָ��ĵڶ�bitΪ0x01
			{
				interface = usart.RxBuffer_USART2[3];	// ���ĸ��ֽڴ�Ż���ID���������interface,����֮���LCDshow�����е�switch case  
			}
			
			switch(usart.RxBuffer_USART2[3])	//����ID
			{
				
				/*��ӭ����*/
				case 0x00:
				{
					switch(usart.RxBuffer_USART2[5])
					{
							case 0x02: 
								B1p.AllowFlag = 0;		//��ʼ��	����������ʹ�ã� Ӧ�������bug��
							break;
						
						default:	;
					}
				}break;

				/*��������*/
				case 0x01:
				{
	
					switch(usart.RxBuffer_USART2[5])
					{
						case 0x01: 
								BEEP_ON;
								Delay_ms(1500);
								RESET_PRO;		//��ʼ��	����������ʹ�ã� Ӧ�������bug��
							break;
						
						default:	;
					}
				
			}break;

				/****��������****/
				case 0x02:	
				{
					switch (usart.RxBuffer_USART2[5])
					{	
						
						case 0x02:
						{
							if(usart.RxBuffer_USART2[8])
							{	
								PB0_ON(1);
								break;
							}
							else
							{
								PB0_OFF(1);
								break;
							}
						}
							
							case 0x03:
							{
							if(usart.RxBuffer_USART2[8])
							{	
								PB1_ON(1);
								break;
							}
							else
							{
								PB1_OFF(1);
								break;
							}
							}
						
							case 0x04:
							{
							if(usart.RxBuffer_USART2[8])
							{	
								PA1_ON(1);
								break;
							}
							else
							{
								PA1_OFF(1);
								break;
							}
							}
							
							case 0x05:
							{
							if(usart.RxBuffer_USART2[8])
							{	
								PA2_ON(1);
								break;
							}
							else
							{
								PA2_OFF(1);
								break;
							}
							}
							
							case 0x06:
							{
							if(usart.RxBuffer_USART2[8])
							{	
								PA3_ON(1);
								break;
							}
							else
							{
								PA3_OFF(1);
								break;
							}
							}
							
							case 0x07:
							{
							if(usart.RxBuffer_USART2[8])
							{	
								PA6_ON(1);
								break;
							}
							else
							{
								PA6_OFF(1);
								break;
							}
							}
							
							case 0x08:
							{
							if(usart.RxBuffer_USART2[8])
							{	
								PA7_ON(1);
								break;
							}
							else
							{
								PA7_OFF(1);
								break;
							}
							}
							
						default:;	
					}
								
				}break;
				
				/****3508����****/
				case 0x03:	
				{
				
					switch (usart.RxBuffer_USART2[5])
					{
						
						
						case 0x03:
							DJmotor[0].mode = atof( (char*) (&usart.RxBuffer_USART2[7] ) );	//����DJmotor[0]��ģʽ
						break;
						
						case 0x04:
							DJmotor[1].mode=atof((char*)(&usart.RxBuffer_USART2[7]));	//����DJmotor[1]��ģʽ
						break;
						
						case 0x05:
							DJmotor[2].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						 case 0x06:
							DJmotor[3].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x07:
							DJmotor[0].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x08:
							DJmotor[1].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x09:
							DJmotor[2].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x0A:
							DJmotor[3].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x0B:
						    DJmotor[0].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x0C:
						    DJmotor[1].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x0D:
							DJmotor[2].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x0E:
							DJmotor[3].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x0F:
							DJmotor[0].enable=usart.RxBuffer_USART2[8];
						break;
						
						case 0x10:
							DJmotor[1].enable=usart.RxBuffer_USART2[8];
						break;
						
						case 0x11:
							DJmotor[2].enable=usart.RxBuffer_USART2[8];
						break;
						
						case 0x12:
							DJmotor[3].enable=usart.RxBuffer_USART2[8];
						break;
						
						case 0x13:
							DJmotor[0].begin=usart.RxBuffer_USART2[8];
						break;
						
						case 0x14:
							DJmotor[1].begin=usart.RxBuffer_USART2[8];
						break;
						
						case 0x15:
							DJmotor[2].begin=usart.RxBuffer_USART2[8];
						break;
						
						case 0x16:
							DJmotor[3].begin=usart.RxBuffer_USART2[8];
						break;
						
						case 0x17:
							DJ_SetZero(0);
						break;	
						
						case 0x18:
							DJ_SetZero(1);
						break;	
						
						case 0x19:
							DJ_SetZero(2);
						break;	
						
						case 0x1A:
							DJ_SetZero(3);
						break;	
						
						case 0x1B:				//��ͣ
						for(int i = 0;i<8;i++)
							DJmotor[i].enable = false;		//���еĵ��ȫ��ʧ��
						break;
						
						default:;	
					}		
					
				}break;
				
				/****2006����****/
				case 0x04:		
				{

					switch (usart.RxBuffer_USART2[5])
					{
						
						
						case 0x03:
							DJmotor[4].mode = atof( (char*) (&usart.RxBuffer_USART2[7] ) );	//����DJmotor[0]��ģʽ
						break;
						
						case 0x04:
							DJmotor[5 ].mode=atof((char*)(&usart.RxBuffer_USART2[7]));	//����DJmotor[1]��ģʽ
						break;
						
						case 0x05:
							DJmotor[6].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						 case 0x06:
							DJmotor[7].mode=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x07:
							DJmotor[4].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x08:
							DJmotor[5].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x09:
							DJmotor[6].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x0A:
							DJmotor[7].valueSet.speed=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x0B:
						    DJmotor[4].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x0C:
						    DJmotor[5].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x0D:
							DJmotor[6].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x0E:
							DJmotor[7].valueSet.angle=atof((char*)(&usart.RxBuffer_USART2[7]));
						break;
						
						case 0x0F:
							DJmotor[4].enable=usart.RxBuffer_USART2[8];
						break;
						
						case 0x10:
							DJmotor[5].enable=usart.RxBuffer_USART2[8];
						break;
						
						case 0x11:
							DJmotor[6].enable=usart.RxBuffer_USART2[8];
						break;
						
						case 0x12:
							DJmotor[7].enable=usart.RxBuffer_USART2[8];
						break;
						
						case 0x13:
							DJmotor[4].begin=usart.RxBuffer_USART2[8];
						break;
						
						case 0x14:
							DJmotor[5].begin=usart.RxBuffer_USART2[8];
						break;
						
						case 0x15:
							DJmotor[6].begin=usart.RxBuffer_USART2[8];
						break;
						
						case 0x16:
							DJmotor[7].begin=usart.RxBuffer_USART2[8];
						break;
						
						case 0x17:
							DJ_SetZero(4);
						break;	
						
						case 0x18:
							DJ_SetZero(5);
						break;	
						
						case 0x19:
							DJ_SetZero(6);
						break;	
						
						case 0x1A:
							DJ_SetZero(7);
						break;	
						
						case 0x1B:				//��ͣ
						for(int i = 0;i<8;i++)
							DJmotor[i].enable = false;		//���еĵ��ȫ��ʧ��
						break;
						
						default:;	
					}	
					
				}break;
				
						case 0x05:		
						{
							switch (usart.RxBuffer_USART2[5])
							{
								case 1:
									if(usart.RxBuffer_USART2[8])
									{
										mech.WheelTeleoperate.flag.USERAD =1;
									}
									else
									{
										mech.WheelTeleoperate.flag.USERAD =0;
									}
								break;
								
								case 2:
									if(usart.RxBuffer_USART2[8])
									{
										mech.WheelTeleoperate.flag.USECAM =1;
									}
									else
									{
										mech.WheelTeleoperate.flag.USECAM =0;
									}
								break;
									
								case 3:
									mech.WheelTeleoperate.Rounds = 0;
								break;
								
								case 4:
										mech.fire.TEST.Wheel_Release();
								break;
								
								case 5:
										mech.WheelTeleoperate.flag.GO = 1;
								break;
								
								case 6:
									mech.WheelTeleoperate.flag.BREAK=1;
								break;
								
								case 7:
									mech.WheelTeleoperate.flag.SHOOT=1;
								break;
								
								case 8:
									mech.WheelTeleoperate.flag.FIRE = 1;
								break;
								
								case 9:
									mech.WheelTeleoperate.BackUpDataID = atof((char*)(&usart.RxBuffer_USART2[7]));
								break;
									
								case 10:
										mech.WheelTeleoperate.data = mech.WheelTeleoperate.BACKUP[mech.WheelTeleoperate.BackUpDataID];
								break;
									
								case 11:
										mech.WheelTeleoperate.BACKUP[mech.WheelTeleoperate.BackUpDataID] = mech.WheelTeleoperate.data;
								break;
									
								case 12:
								{
									mech.WheelTeleoperate.data.Wheel_SHOOT_SP=atof((char*)(&usart.RxBuffer_USART2[7]));
								}break;
									
								case 13:
								{
									mech.WheelTeleoperate.data.Yaw_Ang_Shot=atof((char*)(&usart.RxBuffer_USART2[7]));
								}break;
								
								case 14:
								{																																					
									mech.WheelTeleoperate.data.Pitch_Ang_Shot=atof((char*)(&usart.RxBuffer_USART2[7]));
								}break;
								
								case 15:
								{
									mech.WheelTeleoperate.data.Wheel_Back_SP=atof((char*)(&usart.RxBuffer_USART2[7]));
								}break;
								
								case 16:
								{
									TRParm.Yaw.Yaw_Ang_Back=atof((char*)(&usart.RxBuffer_USART2[7]));
								}break;
								
								case 17:
								{
									TRParm.Pitch.Pitch_Ang_Back = atof((char*)(&usart.RxBuffer_USART2[7]));
								}break;
								
								case 18:
								{
									 mech.WheelTeleoperate.data.middleDelayTime = atof((char*)(&usart.RxBuffer_USART2[7]));
								}break;
								
								case 19:
								{
									 mech.WheelTeleoperate.data.Sp_Thresold = atof((char*)(&usart.RxBuffer_USART2[7]));
								}break;
								
								case 20:
								{
									 mech.WheelTeleoperate.data.Wait_cnt = atof((char*)(&usart.RxBuffer_USART2[7]));
								}break;
								
								case 21:
								{
									mech.WheelTeleoperate.data.ANG_BC = atof((char*)(&usart.RxBuffer_USART2[7]));
								}break;
								
								case 22:
								{
									mech.WheelTeleoperate.data.SP_BC = atof((char*)(&usart.RxBuffer_USART2[7]));
								}break;
								
								case 25:
								{
									mech.WheelTeleoperate.data.Aim_POT = atof((char*)(&usart.RxBuffer_USART2[7]));
								}break;
								
								case 29:
								{
									mech.WheelTeleoperate.flag.connected[mech.WheelTeleoperate.data.Aim_POT] = 0;
								}break;
								
								
								default: break;
							}
						
						}break;
								

					
								
					default:;
						
			}
			
			USART2_RX_STA = 0;
			memset(usart.RxBuffer_USART2, 0, sizeof(usart.RxBuffer_USART2));	
			
		}
	}
}


static void USART2_Send(u8 count)
{
	if(!SystemFlag.Usart2DMASendFinish)	//����һ�δ���δ��ɣ����������δ���
	{
		DMA_SetCurrDataCounter(DMA1_Stream6,count);
		DMA_Cmd(DMA1_Stream6, ENABLE);
		SystemFlag.Usart2DMASendFinish = 1;
	}
}

char str_temp[32];
void UsartLCDshow(void)		//���ذ巢��������
{
	u8 i = 0;	
	
	/*****������*****/
	usart.TxBuffer_USART2[i++] = 0xee;		//֡ͷ
	
	usart.TxBuffer_USART2[i++]=0xb1;		//0xb1+0x10���ð�ť̸��Ͱ���״̬
	usart.TxBuffer_USART2[i++]=0x10;
	
	usart.TxBuffer_USART2[i++]=0x00;		//����ID1
	usart.TxBuffer_USART2[i++]=0x01;
	
	usart.TxBuffer_USART2[i++]=0x00;		//�ؼ�ID2
	usart.TxBuffer_USART2[i++]=0x02;
	
	usart.TxBuffer_USART2[i++]=0x01;	//��ť״̬��0x01��ť�ɵ���䰴��
		
	usart.TxBuffer_USART2[i++]=0xff;	//֡β
	usart.TxBuffer_USART2[i++]=0xfc;
	usart.TxBuffer_USART2[i++]=0xff;
	usart.TxBuffer_USART2[i++]=0xff;	
	
	switch(interface)
	{
		case 0x02:	//���������
		{
		
		}break;
		
		case 0x03:	//3508������
		{
		
		usart.TxBuffer_USART2[i++]=0xee;	//֡ͷ
		usart.TxBuffer_USART2[i++]=0xb1;	//0xb1+0x12�������¿ؼ���ֵ
		usart.TxBuffer_USART2[i++]=0x12;	
		usart.TxBuffer_USART2[i++]=0x00;	//������
		usart.TxBuffer_USART2[i++]=0x03;	
	
		usart.TxBuffer_USART2[i++]=0x00;	//�ؼ�ID
		usart.TxBuffer_USART2[i++]=0x03;
		usart.TxBuffer_USART2[i++]=0x00;	//�ؼ�3�ֽڳ���
		sprintf(str_temp,"%d",DJmotor[0].mode); //(��mode��ӡ�������Ҹ�ֵ���β�str_temp)
		usart.TxBuffer_USART2[i++]=strlen(str_temp);	//(��ȡ�β��ֽڳ�)
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);	//�ؼ�3����ֵ
		i += strlen(str_temp);    

		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x07;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%d",DJmotor[0].valueReal.speed);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);    
	 
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x0B;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%f",DJmotor[0].valueReal.angle);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);
		 
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x0F;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[0].enable;
		
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x13;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[0].begin;			   

		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x04;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%d",DJmotor[1].mode);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);    

		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x08;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%d",DJmotor[1].valueReal.speed);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);    

		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x0C;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%f",DJmotor[1].valueReal.angle);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);
	 
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x10;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[1].enable;
		
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x14;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[1].begin;			    
	

		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x05;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%d",DJmotor[2].mode);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);    
		
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x09;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%d",DJmotor[2].valueReal.speed);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);   

		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x0D;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%f",DJmotor[2].valueReal.angle);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);
	 
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x11;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[2].enable;
		
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x15;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[2].begin;

		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x06;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%d",DJmotor[3].mode);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);    

		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x0A;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%d",DJmotor[3].valueReal.speed);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);  
	 
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x0E;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%f",DJmotor[3].valueReal.angle);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);
		
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x12;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[3].enable;
		
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x16;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[3].begin;

		usart.TxBuffer_USART2[i++]=0xff;		//֡β
		usart.TxBuffer_USART2[i++]=0xfc;
		usart.TxBuffer_USART2[i++]=0xff;
		usart.TxBuffer_USART2[i++]=0xff;

		}break;
	
		case 0x04:
		{
					
		usart.TxBuffer_USART2[i++]=0xee;	//֡ͷ
		usart.TxBuffer_USART2[i++]=0xb1;	//0xb1+0x12�������¿ؼ���ֵ
		usart.TxBuffer_USART2[i++]=0x12;	
		usart.TxBuffer_USART2[i++]=0x00;	//������
		usart.TxBuffer_USART2[i++]=0x04;	
		
		usart.TxBuffer_USART2[i++]=0x00;	//�ؼ�ID
		usart.TxBuffer_USART2[i++]=0x03;
		usart.TxBuffer_USART2[i++]=0x00;	//�ؼ�3�ֽڳ���
		sprintf(str_temp,"%d",DJmotor[4].mode); //(��mode��ӡ�������Ҹ�ֵ���β�str_temp)
		usart.TxBuffer_USART2[i++]=strlen(str_temp);	//(��ȡ�β��ֽڳ�)
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);	//�ؼ�3����ֵ
		i += strlen(str_temp);    
	 
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x07;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%d",DJmotor[4].valueReal.speed);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);    
	 
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x0B;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%f",DJmotor[4].valueReal.angle);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);
		 
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x0F;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[4].enable;
		
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x13;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[4].begin;

		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x04;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%d",DJmotor[5].mode);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);    
	
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x08;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%d",DJmotor[5].valueReal.speed);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);    
 
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x0C;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%f",DJmotor[5].valueReal.angle);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);
	 
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x10;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[5].enable;
		
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x14;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[5].begin;

		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x05;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%d",DJmotor[6].mode);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);    
		
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x09;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%d",DJmotor[6].valueReal.speed);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);   
	
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x0D;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%f",DJmotor[6].valueReal.angle);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);
	 
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x11;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[6].enable;
		
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x15;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[6].begin;

		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x06;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%d",DJmotor[7].mode);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);    
	
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x0A;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%d",DJmotor[7].valueReal.speed);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);  
	 
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x0E;
		usart.TxBuffer_USART2[i++]=0x00;
		sprintf(str_temp,"%f",DJmotor[7].valueReal.angle);
		usart.TxBuffer_USART2[i++]=strlen(str_temp);
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);
		i += strlen(str_temp);
		
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x12;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[7].enable;
		
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x16;
		usart.TxBuffer_USART2[i++]=0x00;
		usart.TxBuffer_USART2[i++]=0x01;
		usart.TxBuffer_USART2[i++]=DJmotor[7].begin;

		usart.TxBuffer_USART2[i++]=0xff;		//֡β
		usart.TxBuffer_USART2[i++]=0xfc;
		usart.TxBuffer_USART2[i++]=0xff;
		usart.TxBuffer_USART2[i++]=0xff;

		}break;
		
		case 0x05:
		{
			lcd_Head(5);
			
			lcd_button_update(1, mech.WheelTeleoperate.flag.USERAD);
			lcd_button_update(2, mech.WheelTeleoperate.flag.USECAM);
			
			lcd_text_update(9, mech.WheelTeleoperate.BackUpDataID, "%d");
			lcd_text_update(12, mech.WheelTeleoperate.data.Wheel_SHOOT_SP, "%5.2f");
			lcd_text_update(13, mech.WheelTeleoperate.data.Yaw_Ang_Shot, "%5.2f");
			lcd_text_update(14, mech.WheelTeleoperate.data.Pitch_Ang_Shot, "%5.2f");
			lcd_text_update(15, mech.WheelTeleoperate.data.Wheel_Back_SP, "%5.2f");
			lcd_text_update(16, TRParm.Yaw.Yaw_Ang_Back, "%5.2f");
			lcd_text_update(17, TRParm.Pitch.Pitch_Ang_Back, "%5.2f");
			lcd_text_update(18, mech.WheelTeleoperate.data.middleDelayTime, "%d");
			lcd_text_update(19, mech.WheelTeleoperate.data.Sp_Thresold,"%5.2f");
			lcd_text_update(20, mech.WheelTeleoperate.data.Wait_cnt, "%d");
			lcd_text_update(21, mech.WheelTeleoperate.data.ANG_BC, "%5.2f");
			lcd_text_update(22, mech.WheelTeleoperate.data.SP_BC, "%5.2f");
			lcd_text_update(23, VESCmotor[0].valueReal.speed, "%5.2f");
			lcd_text_update(24, VESCmotor[1].valueReal.speed, "%5.2f");
			lcd_text_update(25, mech.WheelTeleoperate.data.Aim_POT, "%d");
			lcd_text_update(26, mech.WheelTeleoperate.Rounds, "%d");
			
			if(mech.WheelTeleoperate.flag.USERAD)
			{
				lcd_text_update(27, mech.POTCordself.DIS[mech.WheelTeleoperate.data.Aim_POT], "%5.2f");
				lcd_text_update(28, mech.POTCordself.ANG[mech.WheelTeleoperate.data.Aim_POT], "%5.2f");
			}
			else if(mech.WheelTeleoperate.flag.USECAM)
			{
				lcd_text_update(27, mech.POTCordself.CameraANG[mech.WheelTeleoperate.data.Aim_POT], "%5.2f")
				lcd_text_update(28, mech.POTCordself.status[mech.WheelTeleoperate.data.Aim_POT], "%d");
			}
			
			lcd_button_update(29, mech.WheelTeleoperate.flag.connected[mech.WheelTeleoperate.data.Aim_POT]);
			
			lcd_End();
		}break;
			
		case 0x06:
		{}break;

		case 0x07:
		{}break;
			
		case 0x08:
		{}break;
			
		case 0x09:
		{
	

		}
			
			
			

		default:;
		}
	
		USART2_Send(i);
}

