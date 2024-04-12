#ifndef __FLASH_H
#define __FLASH_H
#include "stm32f4xx.h"
#include "stm32f4xx_flash.h"
#include "stdbool.h"

#define address_increment 100

void Flash_Write(u8 kind);
void Flash_Read(u8 kind);


#endif

