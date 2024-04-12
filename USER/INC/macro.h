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
#define ZhuKong_TX 0x00050101		// �¼ӵİ���
#define Master2DuoJi_TX  0x00070800
#define Fire2TakeRing_TX 0x00020200
#define Fire2TakeRing_RX 0x00020100

#define ANGLE 360
#define LENGTH 352.5  //491-138.5

#define L2S  0x00010002
#define S2L  0x00020001

#define PI 3.1415926 
#define RESET_PRO {__set_FAULTMASK(1);NVIC_SystemReset();}

/* Ϊ�˽�������ź���ʧ�ܣ����ú궨���ض�����ossempend��ÿ������ǰ��������sem*/
#define SemPend(sem)                   \
    {                                  \
        sem->OSEventCnt = 0;           \
        OSSemPend(sem, 0, Error##sem); \
    }

#endif

/*
VESC���ٶȺ�λ��ģʽ�ĳ����Լ����㣬Ϊ��ʵʱ����PID ����
�ø�������һ���Լ���PID�㷨		������û��Ҫ��
3.AK80��������		(�����ϰ�Ҫ)
���Ͻ��ն��У����Խ���Ƕȶ�ʧ�������Ѵ󽮵����ݷ��ڶ�ʱ���ﴦ��Ȼ����CAN���ÿ����һ�η���һ���ź������������еȴ��ź������� 
(����û��Ҫ��˵����Ϊ��ȫ�Կ��ǣ����������Ļ�����һ�����ȼ��ܸ߶���Ƶ�ʺܿ����������Ӳ���ϵͳ�ĸ���)

TODO:
��һ���µĵ�ŷ���		(û����)


*/

