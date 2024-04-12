//#include "flash.h"

//#define Start_ADDRESS 0x080E 0000

//float Flash_ReadFloat(u32 address)
//{
//	return *(float*)address;
//}

//void Flash_ReadMocaLunData(u32 address_Read,float *Buffer,u32 NumberToRead)
//{
//	u32 i;
//	for(i=0;i<NumberToRead;i++)
//	{
//		Buffer[i] = Flash_ReadFloat(address_Read);
//		address_Read+=4;
//	}
//}

//void Flash_WriteMoCaLunData(u32 address_Start,u8 *Buffer)
//{
//	bool IfComplete = 0;
//	u32 address_Now,address_End = 0;
//	
//	FLASH_Unlock();
//	FLASH_DataCacheCmd(DISABLE);	//FLASH擦除期间，禁止数据缓存
//	
//	address_Now = address_Start;
//	address_End = address_Start+address_increment;
//	
//	while(address_Now<address_End)
//	{
//		if(FLASH_ProgramWord(address_Now,*Buffer)!=FLASH_COMPLETE)
//		{
//			break;
//		}
//		address_Now+=4;
//		Buffer++;
//	}

//	FLASH_DataCacheCmd(ENABLE);
//	FLASH_Lock();
//}


