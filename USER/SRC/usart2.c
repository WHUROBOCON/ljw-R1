#include "usart2.h"

u8 USART2_RX_STA = 0;

void USART2_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
  DMA_InitTypeDef 	DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//使能GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能串口2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	//使能DMA1
	
	//将PA2\PA3复用为USART2
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
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);          //接受中断

	//配置NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;		//就改了一下中断优先级就进不去中断了？
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
	DMA_InitStructure.DMA_Priority =DMA_Priority_VeryHigh;//以下为f4特有
	DMA_InitStructure.DMA_FIFOMode =DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold =DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_PeripheralBurst =DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_MemoryBurst =DMA_MemoryBurst_Single;
	DMA_Init(DMA1_Stream6,&DMA_InitStructure);
	
	DMA_ClearFlag (DMA1_Stream6,DMA_IT_TCIF6);					//清除中断标志
	DMA_ClearITPendingBit(DMA1_Stream6,DMA_IT_TCIF6);
	DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);//关闭了DMA传输完成中断，导致数据无法传输。10/14/2018
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;			//解决了遥控无法给主控板发消息，是因为优先级过低（7/1）		11/16/2020
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);

	USART_Cmd(USART2, ENABLE);
}

void DMA1_Stream6_IRQHandler(void) //数据传输完成，产生中断，检查是否还有没有传输的数据，继续传输
{
  if (DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6) == SET) 
  {
    DMA_ClearFlag(DMA1_Stream6, DMA_IT_TCIF6); //清除中断标志
    DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
    SystemFlag.Usart2DMASendFinish = 0;
  }
}
vu8 interface = 0;
int ELMO_ID = 0;	//放在中断外面定义，这样每次进中断值不会被清掉

void USART2_IRQHandler(void)
{
	u8 temp =0;
	if(USART_GetITStatus(USART2,USART_IT_ORE_RX)!=RESET ) 
	{
		temp = USART_ReceiveData(USART2);
		USART2_RX_STA = 0;
		memset(usart.RxBuffer_USART2, 0, sizeof(usart.RxBuffer_USART2) );	//先将RXBUFFER清零
	}
	else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET )
	{
		
		USART_ClearFlag(USART2,USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	//清楚标志位
		
		temp = USART_ReceiveData(USART2);	//开始接收数据,暂存到临时变量temp中
		
		if( (USART2_RX_STA & 0X40)!=0 )	//标志寄存器的第三位不等于0，接收开始
		{
			if( (USART2_RX_STA & 0x80)==0 )	//标志位的第四位等于0，接收未完成
			{
				if (temp ==0xff)	//帧尾，接收完成，进入指令判断区
					USART2_RX_STA |= 0X80;
				
				else
				{
					usart.RxBuffer_USART2[USART2_RX_STA&0X3F] =temp;	//存储
					USART2_RX_STA++;  
					
					if((USART2_RX_STA & 0X3F)>40 )	//超出接收范围
						USART2_RX_STA=0;
				}
				
			}
		
		}
		
		else if(temp == 0xee)		//帧头接收，准备开始接收数据	接收串口屏比主控板发送过去的数据是少了一个字节的，因为第一个0xee是直接被if掉了
		USART2_RX_STA |= 0X40;
	
	
/**
 *@brief	attention!attention! 串口屏开始！
*/
		if( ( (USART2_RX_STA&0X80)!=0 ) )		//串口屏指令判断
		{
					
			 if (usart.RxBuffer_USART2[0] == 0x12) //真实矩阵键盘
      {
    switch (usart.RxBuffer_USART2[1])
        {
        case 0:			//右下角
					mech.WheelTeleoperate.flag.FIRE = 1;
          break;
				
				case 1:			//右下角左边
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
			
			//这里多了一个欢迎界面，应该没什么影响，因为LCDshow是放在任务里的，发送频率很高，就算一开始错过几条应该没关系
			/*页面选择，用于之后发送命令*/
			if(usart.RxBuffer_USART2[1]==0x01)		//切换画面指令的第二bit为0x01
			{
				interface = usart.RxBuffer_USART2[3];	// 第四个字节存放画面ID，存入参数interface,用于之后的LCDshow函数中的switch case  
			}
			
			switch(usart.RxBuffer_USART2[3])	//画面ID
			{
				
				/*欢迎界面*/
				case 0x00:
				{
					switch(usart.RxBuffer_USART2[5])
					{
							case 0x02: 
								B1p.AllowFlag = 0;		//初始化	（可以正常使用， 应该是软件bug）
							break;
						
						default:	;
					}
				}break;

				/*待机界面*/
				case 0x01:
				{
	
					switch(usart.RxBuffer_USART2[5])
					{
						case 0x01: 
								BEEP_ON;
								Delay_ms(1500);
								RESET_PRO;		//初始化	（可以正常使用， 应该是软件bug）
							break;
						
						default:	;
					}
				
			}break;

				/****舵机板界面****/
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
				
				/****3508界面****/
				case 0x03:	
				{
				
					switch (usart.RxBuffer_USART2[5])
					{
						
						
						case 0x03:
							DJmotor[0].mode = atof( (char*) (&usart.RxBuffer_USART2[7] ) );	//设置DJmotor[0]的模式
						break;
						
						case 0x04:
							DJmotor[1].mode=atof((char*)(&usart.RxBuffer_USART2[7]));	//设置DJmotor[1]的模式
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
						
						case 0x1B:				//急停
						for(int i = 0;i<8;i++)
							DJmotor[i].enable = false;		//所有的电机全部失能
						break;
						
						default:;	
					}		
					
				}break;
				
				/****2006界面****/
				case 0x04:		
				{

					switch (usart.RxBuffer_USART2[5])
					{
						
						
						case 0x03:
							DJmotor[4].mode = atof( (char*) (&usart.RxBuffer_USART2[7] ) );	//设置DJmotor[0]的模式
						break;
						
						case 0x04:
							DJmotor[5 ].mode=atof((char*)(&usart.RxBuffer_USART2[7]));	//设置DJmotor[1]的模式
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
						
						case 0x1B:				//急停
						for(int i = 0;i<8;i++)
							DJmotor[i].enable = false;		//所有的电机全部失能
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
	if(!SystemFlag.Usart2DMASendFinish)	//若上一次传输未完成，则舍弃本次传输
	{
		DMA_SetCurrDataCounter(DMA1_Stream6,count);
		DMA_Cmd(DMA1_Stream6, ENABLE);
		SystemFlag.Usart2DMASendFinish = 1;
	}
}

char str_temp[32];
void UsartLCDshow(void)		//主控板发给串口屏
{
	u8 i = 0;	
	
	/*****主界面*****/
	usart.TxBuffer_USART2[i++] = 0xee;		//帧头
	
	usart.TxBuffer_USART2[i++]=0xb1;		//0xb1+0x10设置按钮谈起和按下状态
	usart.TxBuffer_USART2[i++]=0x10;
	
	usart.TxBuffer_USART2[i++]=0x00;		//画面ID1
	usart.TxBuffer_USART2[i++]=0x01;
	
	usart.TxBuffer_USART2[i++]=0x00;		//控件ID2
	usart.TxBuffer_USART2[i++]=0x02;
	
	usart.TxBuffer_USART2[i++]=0x01;	//按钮状态：0x01按钮由弹起变按下
		
	usart.TxBuffer_USART2[i++]=0xff;	//帧尾
	usart.TxBuffer_USART2[i++]=0xfc;
	usart.TxBuffer_USART2[i++]=0xff;
	usart.TxBuffer_USART2[i++]=0xff;	
	
	switch(interface)
	{
		case 0x02:	//舵机主界面
		{
		
		}break;
		
		case 0x03:	//3508主界面
		{
		
		usart.TxBuffer_USART2[i++]=0xee;	//帧头
		usart.TxBuffer_USART2[i++]=0xb1;	//0xb1+0x12批量更新控件数值
		usart.TxBuffer_USART2[i++]=0x12;	
		usart.TxBuffer_USART2[i++]=0x00;	//画面编号
		usart.TxBuffer_USART2[i++]=0x03;	
	
		usart.TxBuffer_USART2[i++]=0x00;	//控件ID
		usart.TxBuffer_USART2[i++]=0x03;
		usart.TxBuffer_USART2[i++]=0x00;	//控件3字节长度
		sprintf(str_temp,"%d",DJmotor[0].mode); //(将mode打印出来，且赋值给形参str_temp)
		usart.TxBuffer_USART2[i++]=strlen(str_temp);	//(获取形参字节长)
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);	//控件3的数值
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

		usart.TxBuffer_USART2[i++]=0xff;		//帧尾
		usart.TxBuffer_USART2[i++]=0xfc;
		usart.TxBuffer_USART2[i++]=0xff;
		usart.TxBuffer_USART2[i++]=0xff;

		}break;
	
		case 0x04:
		{
					
		usart.TxBuffer_USART2[i++]=0xee;	//帧头
		usart.TxBuffer_USART2[i++]=0xb1;	//0xb1+0x12批量更新控件数值
		usart.TxBuffer_USART2[i++]=0x12;	
		usart.TxBuffer_USART2[i++]=0x00;	//画面编号
		usart.TxBuffer_USART2[i++]=0x04;	
		
		usart.TxBuffer_USART2[i++]=0x00;	//控件ID
		usart.TxBuffer_USART2[i++]=0x03;
		usart.TxBuffer_USART2[i++]=0x00;	//控件3字节长度
		sprintf(str_temp,"%d",DJmotor[4].mode); //(将mode打印出来，且赋值给形参str_temp)
		usart.TxBuffer_USART2[i++]=strlen(str_temp);	//(获取形参字节长)
		strcpy((char*)(&usart.TxBuffer_USART2[i]),str_temp);	//控件3的数值
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

		usart.TxBuffer_USART2[i++]=0xff;		//帧尾
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

