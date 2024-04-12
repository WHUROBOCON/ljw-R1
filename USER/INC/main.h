#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f4xx.h"
#include "includes.h"

#include "can1.h"
#include "can2.h"
#include "tim2.h"
#include "tim3.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "usart4.h"
#include "led8.h"
#include "led.h"
#include "beep.h"
#include "param.h"
#include "DJmotor.h"
#include "elmo.h"
#include "visual_scope.h"
#include "cylinder.h"
#include "mech.h"
#include "DataScope_DP.h"
#include "DJ3508.h"
#include "switch.h"

#define START_TASK_PRIO 5

//#define WheelRelease_TASK_PRIO 20

//#define VESCMonitor_TASK_PRIO 21
//#define Shoot_TASK_PRIO 22
//#define Push_TASK_PRIO 23
#define Prepare_TASK_PRIO 24
//#define F1dBac_TASK_PRIO 25
//#define RESET_TASK_PRIO 19

#define	LCD_TASK_PRIO	42	//设置任务优先级
#define	SCOPE_TASK_PRIO	43	//设置任务优先级
#define	BEEP_TASK_PRIO	44	//设置任务优先级
#define	LED_TASK_PRIO	45//设置任务优先级

#define NY_TASK_PRIO 5
#define LJW_TASK_PRIO 6
#define BWC_TASK_PRIO 7

#define SCOOPUP_TASK_PRIO 8
#define SCOOPDOWN_TASK_PRIO 9


#define TASK_STK_SIZE 1024

__align(8) OS_STK START_TASK_STK[TASK_STK_SIZE];	//任务堆栈

__align(8) OS_STK WheelRelease_TASK_STK[TASK_STK_SIZE];	//任务堆栈

__align(8) OS_STK Shoot_TASK_STK[TASK_STK_SIZE];	//任务堆栈
__align(8) OS_STK Push_TASK_STK[TASK_STK_SIZE];	//任务堆栈
__align(8) OS_STK Prepare_TASK_STK[TASK_STK_SIZE];	//任务堆栈
__align(8) OS_STK F1dBac_TASK_STK[TASK_STK_SIZE];	//任务堆栈
__align(8) OS_STK RESET_TASK_STK[TASK_STK_SIZE];	//任务堆栈
__align(8) OS_STK VESCMonitor_TASK_STK[TASK_STK_SIZE];	//任务堆栈


__align(8) OS_STK LCD_TASK_STK[TASK_STK_SIZE];	//任务堆栈
__align(8) OS_STK SCOPE_TASK_STK[TASK_STK_SIZE];	//任务堆栈
__align(8) OS_STK BEEP_TASK_STK[TASK_STK_SIZE];	//任务堆栈
__align(8) OS_STK LED_TASK_STK[TASK_STK_SIZE];	//任务堆栈

__align(8) OS_STK SCOOPUP_TASK_STK[TASK_STK_SIZE];
__align(8) OS_STK SCOOPDOWN_TASK_STK[TASK_STK_SIZE];

__align(8) OS_STK NY_TASK_STK[TASK_STK_SIZE];
__align(8) OS_STK LJW_TASK_STK[TASK_STK_SIZE];
__align(8) OS_STK BWC_TASK_STK[TASK_STK_SIZE];


static void Task_Start(void *pdata);	//任务函数

static void Task_WheelRelease(void *pdata);	//任务函数

static void Task_FastShoot(void *pdata);	//任务函数
static void Task_Shoot(void *pdata);	//任务函数
static void Task_Push(void *pdata);	//任务函数
static void Task_Prepare(void *pdata);	//任务函数
static void Task_F1dBac(void *pdata);	//任务函数
static void Task_RESET(void *pdata);	//任务函数
static void Task_VESCMonitor(void *pdata);	//任务函数

static void Task_LCD(void *pdata);	//任务函数
static void Task_SCOPE(void *pdata);	//任务函数
static void Task_BEEP(void *pdata);	//任务函数
static void Task_LED(void *pdata);	//任务函数


static void Task_SCOOPUP(void *pdata);
static void Task_SCOOPDOWN(void *pdata);

static void Task_NY(void *pdata);
static void Task_LJW(void *pdata);
static void Task_BWC(void *pdata);


#endif
