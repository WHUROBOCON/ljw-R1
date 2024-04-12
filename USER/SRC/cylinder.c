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



//void CYL_Ctrl(u8 IO,u8 status)		//怎么单独修改一个byte中的一位
//{
//	IO = 8 - IO;		//高位是0，低位是8，反一下顺序
//	for(u8 i=0;i<8;i++)		//因为是8位，所以是循环8次
//	{
//		Reset_CYL8CLK();
//		
//		if(i==IO)		//改变当前位
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
    Reset_CYLDATA();//0，都开灯
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
        if (Data & 0x01)//如果数据最后一位为1
            Set_CYLDATA();//1
        else//如果为0
            Reset_CYLDATA();//0
        Set_CYLCLK();//打开时钟，0
        Data >>= 1;//右移一位
    }
		Set_CYLDATA();
}

/*
I0 0对应 8
IO 1对应7
改一个其他咋也会变？

*/

