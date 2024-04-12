#ifndef __MACRO_H
#define __MACRO_H

#define USE_Wheel

#define USE_VESC 1
#define USE_DJ 1
//#define USE_AK
//#define USE_ZDRIVE 0

#define LCD_TASK
#define SCOPE_TASK


#define USART1_Tx_BufferSize 256
#define USART2_Tx_BufferSize 256
#define USART3_Tx_BufferSize 32
#define USART4_Tx_BufferSize 32
#define USART1_Rx_BufferSize 32
#define USART2_Rx_BufferSize 32
#define USART3_Rx_BufferSize 32
#define USART4_Rx_BufferSize 32

#define BroadCast_RX 0x1020100
#define ZhuKong_RX 0x01020500
#define ZhuKong_TX 0x00050101		// 新加的板子
#define Master2DuoJi_TX  0x00070800
#define Fire2TakeRing_TX 0x00020200
#define Fire2TakeRing_RX 0x00020100

#define ANGLE 360
#define LENGTH 352.5  //491-138.5

#define L2S  0x00010002
#define S2L  0x00020001

#define PI 3.1415926 
#define RESET_PRO {__set_FAULTMASK(1);NVIC_SystemReset();}

/* 为了解决请求信号量失败，采用宏定义重定向了ossempend，每次请求前，先清零sem*/
#define SemPend(sem)                   \
    {                                  \
        sem->OSEventCnt = 0;           \
        OSSemPend(sem, 0, Error##sem); \
    }

#endif

/*
VESC的速度和位置模式改成了自己来算，为了实时更改PID 参数
拿个轮子试一下自己的PID算法		（可能没必要）
3.AK80驱动代码		(找乔老板要)
加上接收队列（可以解决角度丢失？），把大疆的数据放在定时器里处理，然后在CAN里边每接收一次发送一个信号量，在任务中等待信号量发送 
(可能没必要，说是因为安全性考虑，但是这样的话创建一个优先级很高而且频率很快的任务会增加操作系统的负担)

TODO:
试一下新的电磁阀板		(没问题)


*/

