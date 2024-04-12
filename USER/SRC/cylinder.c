#include "cylinder.h"
//CyLinderStruct Cyl;																			//{}}
//unsigned char CYL_table[8] = {0x01,0x01,0x01,0x01,0x00,0x01,0x00 ,0x01} ;
//														
////pc0 sclk pc1 io 
//void CYL_Configuration(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//    
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
//  
//	Set_CYL8CLK();		//GPIO_SetBits(GPIOB, GPIO_Pin_8)
//	Reset_CYL8CLK();		//GPIO_ResetBits(GPIOB, GPIO_Pin_8)
//}



//void CYL_Ctrl(u8 IO,u8 status)		//��ô�����޸�һ��byte�е�һλ
//{
//	IO = 8 - IO;		//��λ��0����λ��8����һ��˳��
//	for(u8 i=0;i<8;i++)		//��Ϊ��8λ��������ѭ��8��
//	{
//		Reset_CYL8CLK();
//		
//		if(i==IO)		//�ı䵱ǰλ
//		{
//			CYL_table[i] = status;		
//		}			
//		
//		if(CYL_table[i] & 0x01)
//			Set_CYLData();		//GPIO_SetBits(GPIOB, GPIO_Pin_9)
//		else 
//			Reset_CYLData();		//GPIO_ResetBits(GPIOB, GPIO_Pin_9)
//		
//		Set_CYL8CLK();

//	}	
//}
void Valve_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;//U2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    Set_CYLCLK();//1
    Reset_CYLDATA();//0��������
}

/**
 * @brief: Valve control by bits
 */
void Valve_Ctrl(u8 Data)
{
    u8 i;
    if (Data > 0xFF)
        Data = 0XFF;
    for (i = 0; i < 8; i++)
    {
        Reset_CYLCLK();
        if (Data & 0x01)//����������һλΪ1
            Set_CYLDATA();//1
        else//���Ϊ0
            Reset_CYLDATA();//0
        Set_CYLCLK();//��ʱ�ӣ�0
        Data >>= 1;//����һλ
    }
		Set_CYLDATA();
}

/*
I0 0��Ӧ 8
IO 1��Ӧ7
��һ������զҲ��䣿

*/

