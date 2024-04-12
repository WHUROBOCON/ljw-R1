
#ifndef _CYLINDER_H
#define _CYLINDER_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

//#define Set_CYL8CLK() 		GPIO_SetBits(GPIOC, GPIO_Pin_0)
//#define Reset_CYL8CLK() 	GPIO_ResetBits(GPIOC, GPIO_Pin_0)
//#define Set_CYLData() 		GPIO_SetBits(GPIOC, GPIO_Pin_1)
//#define Reset_CYLData() 	GPIO_ResetBits(GPIOC, GPIO_Pin_1)

//typedef struct 
//{
//    void (*CYL_Ctrl)(u8 IO, u8 status);

//} CyLinderStruct;

//void CYL_Configuration(void);
//void CYL_Ctrl(u8 IO,u8 status);

//extern CyLinderStruct Cyl;

/* ----------------------------- Macro ----------------------------- */

//#define Set_CYLCLK() 		GPIO_SetBits(GPIOA, GPIO_Pin_2)//USART2-TX
//#define Reset_CYLCLK() 	    GPIO_ResetBits(GPIOA, GPIO_Pin_2)
//#define Set_CYLDATA() 		GPIO_SetBits(GPIOA, GPIO_Pin_3)
//#define Reset_CYLDATA() 	GPIO_ResetBits(GPIOA, GPIO_Pin_3)
/* Tx-Rx reversed */
//#define Set_CYLDATA() 		GPIO_SetBits(GPIOA, GPIO_Pin_2)
//#define Reset_CYLDATA()	GPIO_ResetBits(GPIOA, GPIO_Pin_2)
//#define Set_CYLCLK() 		GPIO_SetBits(GPIOA, GPIO_Pin_3)
//#define Reset_CYLCLK() 	GPIO_ResetBits(GPIOA, GPIO_Pin_3)

//#define Set_CYLCLK() GPIO_SetBits(GPIOA, GPIO_Pin_0) // USART4-TX
//#define Reset_CYLCLK() GPIO_ResetBits(GPIOA, GPIO_Pin_0)
//#define Set_CYLDATA() GPIO_SetBits(GPIOA, GPIO_Pin_1)
//#define Reset_CYLDATA() GPIO_ResetBits(GPIOA, GPIO_Pin_1)
/* Tx-Rx reversed */
#define Set_CYLDATA() 	GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define Reset_CYLDATA()	GPIO_ResetBits(GPIOA, GPIO_Pin_2)
#define Set_CYLCLK() 		GPIO_SetBits(GPIOA, GPIO_Pin_3)
#define Reset_CYLCLK() 	GPIO_ResetBits(GPIOA, GPIO_Pin_3)

/* ----------------------------- Decleration ----------------------------- */

void Valve_Init(void);
void Valve_Ctrl(u8 data);

#endif

/* ----------------------------- End of file ----------------------------- */
