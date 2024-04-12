#include "main.h"
//俯仰角度为负，减速比为8
//-45,5820
int main(void)
{
	SystemInit();
  NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);	// 7/1
	
	CAN1_Init();
	CAN2_Init();
	 
	TIM2_Configuration();
	TIM3_Configuration();
	USART1_Configuration();	
	USART3_Configuration();
//	USART2_Configuration();
//	USART4_Configuration();
	
	LED_Configuration();
	Led8_Configuration(); 
//	Beep_Configuration();
	Valve_Init();
//	CYL_Configuration();	
	Can_SendQueueInit();
	MesgCtrlList_Init(Can1MsgCtrllist,CAN_1);	MesgCtrlList_Init(Can2MsgCtrllist,CAN_2);
	
	param_Init();
	
	Switch_Init();

  OSInit();	//初始化操作系统
	OSTaskCreate(Task_Start,(void *)0,&START_TASK_STK[TASK_STK_SIZE-1],START_TASK_PRIO);
	OSStart();	//启动操作系统
}
/**
  * @brief  起始任务
  */
static void Task_Start(void *pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata = pdata;
	OS_CPU_SysTickInit();		//启动操作系统时钟
	
	OS_ENTER_CRITICAL();
	
	
/**************创建任务*****************/	

//	#ifdef LCD_TASK
//	OSTaskCreate(Task_LCD, (void *)0, (OS_STK *)&LCD_TASK_STK[TASK_STK_SIZE - 1],LCD_TASK_PRIO);
//	#endif
//	#ifdef SCOPE_TASK
//	OSTaskCreate(Task_SCOPE, (void *)0, (OS_STK *)&SCOPE_TASK_STK[TASK_STK_SIZE - 1],SCOPE_TASK_PRIO);
//	#endif
	OSTaskCreate(Task_BEEP, (void *)0, &BEEP_TASK_STK[TASK_STK_SIZE - 1], BEEP_TASK_PRIO);
	OSTaskCreate(Task_LED, (void *)0, (OS_STK *)&LED_TASK_STK[TASK_STK_SIZE - 1],LED_TASK_PRIO);	
//	OSTaskCreate(Task_WheelRelease, (void *)0, &WheelRelease_TASK_STK[TASK_STK_SIZE - 1], WheelRelease_TASK_PRIO);
//	OSTaskCreate(Task_Shoot, (void *)0, (OS_STK *)&Shoot_TASK_STK[TASK_STK_SIZE - 1],Shoot_TASK_PRIO);
//	OSTaskCreate(Task_Push, (void *)0, (OS_STK *)&Push_TASK_STK[TASK_STK_SIZE - 1],Push_TASK_PRIO);
	OSTaskCreate(Task_Prepare, (void *)0, (OS_STK *)&Prepare_TASK_STK[TASK_STK_SIZE - 1],Prepare_TASK_PRIO);
//	OSTaskCreate(Task_F1dBac, (void *)0, (OS_STK *)&F1dBac_TASK_STK[TASK_STK_SIZE - 1],F1dBac_TASK_PRIO);
//	OSTaskCreate(Task_RESET, (void *)0, (OS_STK *)&RESET_TASK_STK[TASK_STK_SIZE - 1],RESET_TASK_PRIO);
//	OSTaskCreate(Task_VESCMonitor, (void *)0, (OS_STK *)&VESCMonitor_TASK_STK[TASK_STK_SIZE - 1],VESCMonitor_TASK_PRIO);
//	OSTaskCreate(Task_SCOOPUP, (void *)0, (OS_STK *)&SCOOPUP_TASK_STK[TASK_STK_SIZE - 1],SCOOPUP_TASK_PRIO);
//	OSTaskCreate(Task_SCOOPDOWN, (void *)0, (OS_STK *)&SCOOPDOWN_TASK_STK[TASK_STK_SIZE - 1],SCOOPDOWN_TASK_PRIO);
//	OSTaskCreate(Task_NY, (void *)0, (OS_STK *)&NY_TASK_STK[TASK_STK_SIZE - 1],NY_TASK_PRIO);
	OSTaskCreate(Task_LJW, (void *)0, (OS_STK *)&LJW_TASK_STK[TASK_STK_SIZE - 1],LJW_TASK_PRIO);
//	OSTaskCreate(Task_BWC, (void *)0, (OS_STK *)&BWC_TASK_STK[TASK_STK_SIZE - 1],BWC_TASK_PRIO);
	
	OS_EXIT_CRITICAL();		//退出临界区
	
	OSTimeDly(1000);
	
	Beep_Start();
	
#ifdef USE_AK
	TMD.ComInit(TmotorID); // ?????
	OSTimeDly(100);
	TMD.ComInit(TmotorID);
	OSTimeDly(100);
	
	TMD.PX0(TmotorID); // ?????
	OSTimeDly(100);
	TMD.PX0(TmotorID);
	OSTimeDly(100);
	TMD.PX0(TmotorID);
	OSTimeDly(100);
	TMD.PX0(TmotorID);
	OSTimeDly(100);
	TMD.PX0(TmotorID);
	OSTimeDly(1000);
	
	TMD.MO1(TmotorID);		   		// ??
	TMD.UM(TmotorID, TPos); 	// ??????
	TMD.SP(TmotorID, 60);	   		// ??????????
#endif

	for (;;)
	{
		OSTimeDly(2000);
	}
	
}

#define _ANGLE -1500
#define _LENGTH 320
double distance1=385;
double distance2=0;
int angle_Pitching=0;
int process=0xff;
static void Task_NY(void *pdata) 
{
	while(1)
	{
		if(process==1)
		{
			DJmotor[0].enable=1;
	  	DJmotor[1].enable=1;
			DJmotor[0].begin=1;
			DJmotor[1].begin=1;
			process=0xff;
		}

		if(process==2)
		{
	  	DJmotor[1].valueSet.angle=angle_Pitching;
	  	while(ABS(DJmotor[1].valueSet.angle-angle_Pitching)>1)
		 {
			OSTimeDly(50);
    	}
		 process=0xff;
		}
		
		if(process==3)
		{
			float _angle = _ANGLE / _LENGTH;
			DJmotor[0].valueSet.angle =  _angle * distance1;
			while(ABS(DJmotor[0].valueReal.angle-(_angle * distance1))>1)
			{
				OSTimeDly(100);
			}
				Valve_Ctrl (0);// lock
				DJmotor[0].Limit.PosSPLimit=2000;
				DJmotor[0].valueSet.angle =  _angle * distance2;
				while(ABS(DJmotor[0].valueReal.angle-(_angle * distance2))>1)
				{
					OSTimeDly(100);
				}
			process=0xff;
		}
		
		if(process==4)
		{
			Valve_Ctrl(255);
			process=0xff;
		}
		
		if(process==5)
    {
			DJmotor[0].valueSet.angle=0;
			while(ABS(DJmotor[0].valueSet.angle-0)>1)
			{
				OSTimeDly(50);
			}
			DJmotor[1].valueSet.angle=0;
			while(ABS(DJmotor[1].valueSet.angle-0)>1)
			{
				OSTimeDly(50);
			}
			process=0xff;
		}
		
		if(process==6)
		{
			Valve_Ctrl(255);
			process=0xff;
		}
		if(process==7)
		{
			Valve_Ctrl(0);
			process=0xff;
		}
		
    if(process==0)
		{
			DJmotor[0].enable=0;
	  	DJmotor[1].enable=0;
			DJmotor[0].begin=0;
			DJmotor[1].begin=0;
			process=0xff;
		}
		OSTimeDly(2000);
	}
}


int step=0xff;
int _pos=0;
float angle_findzero=560;
float angle_up=150;
float angle_down=15;
float angle_fire=560;
int out1=-2000;
int out2=-2500;
int out3=-2500; 
int in1=700;
int in2=700;
int in3=700;
// -4600 -100 
// -4000 -100
// -2700 300

//-3000 -100
//-4000 -100 450
static void Task_LJW(void *pdata) 
{
	while(1)
	{
		if(step==0)
		{
			VESCmotor[0].enable =0;
			VESCmotor[1].enable =0;
			VESCmotor[2].enable =0;
			DJmotor[1].enable =0;
			VESCmotor[0].begin =0;
			VESCmotor[1].begin =0;
			VESCmotor[2].begin =0;
			DJmotor[1].begin=0;
			step=0xff;
		}
		
		if(step==1)
		{
			VESCmotor[0].enable =1;
			VESCmotor[1].enable =1;
			VESCmotor[2].enable =1;
			DJmotor[1].enable =1;
			VESCmotor[0].begin =1;
			VESCmotor[1].begin =1;
			VESCmotor[2].begin =1;
			DJmotor[1].begin=1;
			step=0xff;
		}
		
		if(step==2)
		{
			DJmotor[1].valueSet.angle=angle_findzero;
			while(ABS(DJmotor[1].valueReal.angle -angle_findzero)>1)
			{
				OSTimeDly(50);
			}
			step=0xff;
		}
		
		if(step==3)
		{
			DJmotor[1].valueSet.angle=angle_up;
			while(ABS(DJmotor[1].valueReal.angle -angle_up)>1)
			{
				OSTimeDly(50);
			}
			step=0xff;
		}
			
		if(step==4)
		{
			Valve_Ctrl (255);
			VESCmotor[0].valueSet.speed=in1 ;
			VESCmotor[1].valueSet.speed=in2 ;
			VESCmotor[2].valueSet.speed=in3 ;
			step=0xff;
		}
			
		if(step==5)
		{
			DJmotor[1].valueSet.angle=angle_down;
			while(ABS(DJmotor[1].valueReal.angle -angle_down)>1)
			{
				OSTimeDly(50);
			}
			step=0xff;
		}
			
		if(step==6)
		{
			VESCmotor[0].valueSet.speed=0;
			VESCmotor[1].valueSet.speed=0;
			VESCmotor[2].valueSet.speed=0;
			step=0xff;
		}
		
		if(step==7)
		{
			DJmotor[1].valueSet.angle=angle_fire;
			while(ABS(DJmotor[1].valueReal.angle -angle_fire)>1)
			{
				OSTimeDly(50);
			}
			step=0xff;
		}
			
		if(step==8)
		{
//			while(ABS(VESCmotor[0].valueReal .speed-out1)>30)
//			{
//				OSTimeDly(50);
//			}
//			while(ABS(VESCmotor[1].valueReal .speed-out2)>30)
//			{
//				OSTimeDly(50);
//			}
//			while(ABS(VESCmotor[2].valueReal .speed-out3)>30)
//			{
//				OSTimeDly(50);
//			}
//			switch(_pos)
//			{
//				case 1:		//pot 1 closest
//				{
//					out1 = 150;
//					out2 = -3800;
//					out3 = -3800;
//					VESCmotor[0].valueSet.speed=out1 ;
//			    VESCmotor[1].valueSet.speed=out2 ;
//					VESCmotor[2].valueSet.speed=out3 ;
//					OSTimeDly(5000);
//					Valve_Ctrl (0);
//					step=0xff;
//					break;
//				}
//				
//				case 2:		//pot 1 furthest
//				{
//					out1 = -150;
//					out2 = -5700;
//					out3 = -5700;
//					VESCmotor[0].valueSet.speed=out1 ;
//			    VESCmotor[1].valueSet.speed=out2 ;
//					VESCmotor[2].valueSet.speed=out3 ;
//					OSTimeDly(5000);
//					Valve_Ctrl (0);
//					step=0xff;
//					break;
//				}
//				
//				case 3:		//pot 2 closer
//				{
//					out1 = 200;
//					out2 = -4200;
//					out3 = -4200;
//					VESCmotor[0].valueSet.speed=out1 ;
//			    VESCmotor[1].valueSet.speed=out2 ;
//					VESCmotor[2].valueSet.speed=out3 ;
//					OSTimeDly(5000);
//					Valve_Ctrl (0);
//					step=0xff;
//					break;
//				}
//				
//				case 4:		//pot 2 closest
//				{
//					out1 = 200;
//					out2 = -3700;
//					out3 = -3700;
//					VESCmotor[0].valueSet.speed=out1 ;
//			    VESCmotor[1].valueSet.speed=out2 ;
//					VESCmotor[2].valueSet.speed=out3 ;
//					OSTimeDly(5000);
//					Valve_Ctrl (0);
//					step=0xff;
//					break;
//				}
//				
//				case 5:		//pot 2 furthest
//				{
//					out1 = -150;
//					out2 = -6000;
//					out3 = -6000;
//					VESCmotor[0].valueSet.speed=out1 ;
//			    VESCmotor[1].valueSet.speed=out2 ;
//					VESCmotor[2].valueSet.speed=out3 ;
//					OSTimeDly(5000);
//					Valve_Ctrl (0);
//					step=0xff;
//					break;
//				}
//				
//				case 6:
//				{
//					angle_fire=450;
//					DJmotor[1].valueSet.angle=angle_fire;
//					while(ABS(DJmotor[1].valueReal.angle -angle_fire)>1)
//					{
//						OSTimeDly(50);
//					}
//					out1 = -100;
//					out2 = -4000;
//					out3 = -4000;
//					VESCmotor[0].valueSet.speed=out1 ;
//			    VESCmotor[1].valueSet.speed=out2 ;
//					VESCmotor[2].valueSet.speed=out3 ;
//					OSTimeDly(5000);
//					Valve_Ctrl (0);
//					step=0xff;
//					break;
//				}
//				
//				default:
//          break;
//			}
//			
			VESCmotor[0].valueSet.speed=out1 ;
			VESCmotor[1].valueSet.speed=out2 ;
			VESCmotor[2].valueSet.speed=out3 ;
			OSTimeDly(5000);
			Valve_Ctrl (0);
			step=0xff;
		}
		
		if(step==9)
		{
			VESCmotor[0].valueSet.speed=0;
			VESCmotor[1].valueSet.speed=0;
			VESCmotor[2].valueSet.speed=0;
			DJmotor[1].valueSet.angle=0;
			while(ABS(DJmotor[1].valueReal.angle -0)>1)
			{
				OSTimeDly(50);
			}
			step=0xff;
		}
		
		if(step==10)
		{
			Valve_Ctrl (255);
			step=0xff;
		}
		if(step==11)
		{
			Valve_Ctrl (0);
			step=0xff;
		}
		
		
		OSTimeDly(2000);
	}
	
}


static void Task_BWC(void *pdata) 
{
	while(1)
	{
		
		OSTimeDly(2000);
	}
}


int scoopCase;
//int count=0;
//int judge=0;
//static void Task_SCOOPUP(void *pdata) 
//{
//	while(1)
//	{
//		for(count=0;count<5000;count++)
//		{
//			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9)==0)
//			{
//				OSTimeDly(10);
//				scoopCase=2;
//			}
//			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9)==1)
//			{
//				Valve_Ctrl(255);//scoop
//				count=5000;
//				scoopCase=1;
//			}
//		}
//		if(count>=5000)
//    {
//			Valve_Ctrl(255);//scoop
//			for(judge=0;judge<1000;judge++)
//			{
//				if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9)==1)
//				{
//					OSTimeDly(10);
//					scoopCase=1;
//				}
//				else scoopCase=0;
//			}
//			scoopCase=1;
//		}
//		OSTimeDly(2000);
//	}
//}


int i=0xff;
static void Task_SCOOPDOWN(void *pdata) 
{
	while(1)
  {
//		DJmotor[0].begin=1;
////		Zdrive[0].begin =1;
////		if(i==1)
////		{
////			Zdrive[0].mode=Zdrive_Position;
////			Zdrive[0].PVTMode.PVTModeFlag =1;
////			Zdrive[0].PVTMode.ReachTime[0]=1;
////			Zdrive[0].PVTMode.TargetAngle[0]=angle1;
////			Zdrive[0].PVTMode.TargetSpeed[0] =0.1;
////			Zdrive[0].enable=1;
////			while(ABS(Zdrive[0].ValueReal.angle-angle1)>1)
////			{
////				OSTimeDly(50);
////			}
////			i=0xff;
////		}
////		if(i==2)
////		{
////			Zdrive[0].PVTMode.TargetAngle[0] =angle2;
////			Zdrive[0].PVTMode.PVTModeFlag=1;
////			while(ABS(Zdrive[0].ValueReal.angle-angle2)>1)
////			{
////				OSTimeDly(50);
////			}
////			OSTimeDly(5000);
////			Valve_Ctrl(0);
////			i=0xff;
////		}
//		if(i==3)
//		{
////			Zdrive[0].PVTMode.TargetAngle[0]=0;
////			Zdrive[0].PVTMode.PVTModeFlag=1;
////			while(ABS(Zdrive[0].ValueReal.angle)>1)
////			{
////				OSTimeDly(50);
////			}
////			OSTimeDly(5000);
//			
//			DJmotor[0].mode =1;
//			DJmotor[0].enable=1;
//			DJmotor[0].Limit.PosSPLimit=1500;
//			DJmotor[0].valueSet.angle=angle3;
//			while(ABS(DJmotor[0].valueReal.angle-angle3)>1)
//			{
//				OSTimeDly(50);
//			}
//			OSTimeDly(10000);
//			Valve_Ctrl(0);
//			Switch_Ctrl();
//			i=0xff;
//		}
//		if(i==4)
//		{
//			DJmotor[0].Limit.PosSPLimit=2000;
//			DJmotor[0].valueSet.angle=angle4;
//			while(ABS(DJmotor[0].valueReal.angle-150)>1)
//			{
//				OSTimeDly(50);
//			}
////			OSTimeDly(5000);
//			Valve_Ctrl(0);
//			OSTimeDly(15000);
//			DJmotor[0].valueSet.angle=0;
//			i=0xff;
//		}
//			if(i==5)
//		{
//			Switch_Ctrl();
//			i=0xff;
//		}
//		if(i==6)
//		{
//			Valve_Ctrl(255);//scoop
//			i=0xff;
//		}
//		if(i==0)
//		{
//			DJmotor[0].begin=0;
//		  Zdrive[0].begin=0;
//			DJmotor[0].enable=0;
//			Zdrive[0].enable=0;
//			i=0xff;
//		}
		OSTimeDly(2000);	
	}
}




//int TestFlag=0;
//float SetAng=360,SetPosSP=1000;
//static void Task_VESCMonitor(void *pdata) //修正速度
//{
//	while(1)
//	{ 
//		if(TestFlag==1)
//		{
//			TMD.PX0(TmotorID);
//			TMD.MO1(TmotorID);
//			TMD.SP(TmotorID,SetPosSP);
//			TMD.PA(TmotorID,SetAng);
//			TMD.BG(TmotorID);
//			TestFlag=0;
//		}
//		if(TestFlag==2)
//		{
//			TMD.PX0(TmotorID);
//			TMD.MO0(TmotorID);	
//		}
//		if(TestFlag==3)	//夹爪
//		{
//			for(int i=0;i<=7;i++)
//			{
//				Valve_Ctrl(1<<i);
//				OSTimeDly(5000);
//			}
//		}
//		if(TestFlag==4)
//		{
//			Valve_Ctrl(0);
//		}
//		if(TestFlag==5)	//推环
//		{
//			Valve_Ctrl(1);
//			OSTimeDly(10000);
//			TestFlag=4;
//		}
//		OSTimeDly(2000);	
//	}
//}

//int ControlFlag=0,AllTestFlag=1;
//float Speed1=100,Speed2=100,Speed3=100,Speed4=100,Angle=0,AllSpeed=100,PushAngle=62;
//static void Task_Shoot(void *pdata)
//{
//	while(1)
//	{
//		if(ControlFlag==1)	//使能
//		{
//			VESCmotor[0].enable=1;
//			VESCmotor[1].enable=1;
//			VESCmotor[2].enable=1;
//			VESCmotor[3].enable=1;
//			DJmotor[4].enable=1;
//			DJmotor[6].enable=1;
//			ControlFlag=0;
//		}
//		if(ControlFlag==2)	//定速
//		{
//			if(AllTestFlag==1)	//同速
//			{
//				VESCmotor[0].valueSet.speed=AllSpeed;
//				VESCmotor[1].valueSet.speed=-AllSpeed;
//				VESCmotor[2].valueSet.speed=AllSpeed;
//				VESCmotor[3].valueSet.speed=-AllSpeed;
//			}
//			if(AllTestFlag==0)	//差速
//			{
//				VESCmotor[0].valueSet.speed=Speed1;
//				VESCmotor[1].valueSet.speed=-Speed2;
//				VESCmotor[2].valueSet.speed=Speed3;
//				VESCmotor[3].valueSet.speed=-Speed4;
//			}
//			//判断速度到位
//			if((ABS(VESCmotor[0].valueReal.speed-VESCmotor[0].valueSet.speed)<10)&&(ABS(VESCmotor[1].valueReal.speed-VESCmotor[1].valueSet.speed)<10)&&(ABS(VESCmotor[2].valueReal.speed-VESCmotor[2].valueSet.speed)<10)&&(ABS(VESCmotor[3].valueReal.speed-VESCmotor[3].valueSet.speed)<10))
//			{
//				OSTimeDly(10000);
//				ControlFlag=4;
//			}
//		}
//		if(ControlFlag==3)	//俯仰角度
//		{
//			DJmotor[4].valueSet.angle=Angle;
//			ControlFlag=0;
//		}
//		if(ControlFlag==4)	//推
//		{
//			DJmotor[6].valueSet.angle=PushAngle;
//			if(ABS(DJmotor[6].valueReal.angle-DJmotor[6].valueSet.angle)<1.0f)
//			{
//				OSTimeDly(10000);
//				DJmotor[6].valueSet.angle=8;
//				VESCmotor[0].valueSet.speed=0;
//				VESCmotor[1].valueSet.speed=0;
//				VESCmotor[2].valueSet.speed=0;
//				VESCmotor[3].valueSet.speed=0;
//				ControlFlag=0;
//			}
//		}
//		if(ControlFlag==5)	//失能
//		{
//			VESCmotor[0].enable=0;
//			VESCmotor[1].enable=0;
//			VESCmotor[2].enable=0;
//			VESCmotor[3].enable=0;
//			ControlFlag=0;
//		}
//		OSTimeDly(2000);
//	}
//}

//int InitCnt=0;

//int check=0;
//static void Task_WheelRelease(void *pdata)
//{
//	while(1)
//	{
//		if(mech.FLAG.ShootFlag==1)
//		{
//			check++;
//		}
//		OSTimeDly(2000);
//	}
//}

//int ThrowFlag=0;
//float	tempangle=360.0f,tempvel=1000.0f;
//static void Task_Push(void *pdata)
//{
//	while(1)
//	{
//		if(ThrowFlag==1)
//		{
//			TMD.UM(TmotorID,TPosVel);
//			TMD.MO1(TmotorID);
//			TMD.SP(TmotorID,tempvel);
//			TMD.PA(TmotorID,tempangle);
//			TMD.BG(TmotorID);
//			ThrowFlag=0;
//		}
//		else if(ThrowFlag==2)
//		{
//			TMD.MO0(TmotorID);
//			TMD.PX0(TmotorID);
//			Tmotor[TmotorID].valueSet.angle=0;
//			ThrowFlag=0;
//		}
//		else if(ThrowFlag==3)
//		{
//			TMD.UM(TmotorID,TVel);
//			TMD.MO1(TmotorID);
//			TMD.SP(TmotorID,tempvel);
//			TMD.BG(TmotorID);
//		}
//		if(ThrowFlag==4)
//		{
//			DJmotor[6].enable=1;
//			Tmotor[TmotorID].enable=1;
//			Tmotor[TmotorID].valueSet.angle=360;
//			OSTimeDly(50000);
//			ThrowFlag=2;
//		}
//		OSTimeDly(200);
//	}
//}

static void Task_Prepare(void *pdata)
{
	while(1)
	{
		OSTimeDly(2000);		
	}
}

////void SendPitchFinish(void)
//{
//	if(IncCAN1Rear==CAN1_SendQueue.Front)
//	{
//		SystemFlag.Can1SendqueueFULL++;
//		return;
//	}
//	else
//	{
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID  =Fire2TakeRing_TX;
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC =1;
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 11;
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList=0;
//	}
//	CAN1_SendQueue.Rear=IncCAN1Rear;
//}
//void SendRESETFinish(void)
//{
//	if(IncCAN1Rear==CAN1_SendQueue.Front)
//	{
//		SystemFlag.Can1SendqueueFULL++;
//		return;
//	}
//	else
//	{
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].ID  =Fire2TakeRing_TX;
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].DLC =1;
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Data[0] = 12;
//		CAN1_SendQueue.Can_DataSend[CAN1_SendQueue.Rear].Mail_If2CtrlList=0;
//	}
//	CAN1_SendQueue.Rear=IncCAN1Rear;
//}
//int cnt=0;
//static void Task_RESET(void *pdata)
//{
//	while(1)
//	{
//		if(mech.fire.FLAG.RESET==1)
//		{
//			if(mech.fire.FLAG.PitchRESETOver==0)
//			{
//				mech.fire.FUNC.Pitch_ZeroMode();
//			}
//			if(mech.fire.FLAG.PitchRESETOver==1)
//			{
////				cnt++;  
////				if(cnt==1)SendPitchFinish();
////				if(mech.fire.FLAG.UPReady==1)
////				{
////					mech.fire.ARGU.Pitch_Goal=10;
////					mech.fire.FUNC.Pitch_Motion(mech.fire.ARGU.Pitch_Goal,mech.fire.ARGU.PitchPosSP,0);
////				
//					if(mech.fire.FLAG.YawRESETOver==0)
//					{
//						mech.fire.FUNC.Yaw_ZeroMode();
//					}
//					if(mech.fire.FLAG.YawRESETOver==1)
//					{
//						mech.fire.FLAG.RESET=2;
//						mech.fire.FLAG.YawRESETOver=0;		mech.fire.FLAG.PitchRESETOver=0;
//						mech.fire.FLAG.UPReady=0;
//					}
////				}
//			}
//		}
//		if(mech.fire.FLAG.RESET==2)
//		{
//			mech.fire.ARGU.Yaw_Goal=-45;
//			mech.fire.FUNC.Yaw_Motion(mech.fire.ARGU.Yaw_Goal,mech.fire.ARGU.YawPosSP,0);
//			if(mech.fire.FLAG.YawReachFlag==1)
//			{
//				mech.fire.ARGU.Pitch_Goal=10;
//				mech.fire.FUNC.Pitch_Motion(mech.fire.ARGU.Pitch_Goal,mech.fire.ARGU.PitchPosSP,0);
//				if(mech.fire.FLAG.PitchReachFlag==1)
//				{
//					DJ_SetZero(4); DJ_SetZero(5); DJ_SetZero(6);
//					mech.fire.ARGU.Yaw_Goal=0; mech.fire.ARGU.Pitch_Goal=0;
//					SendRESETFinish();
//					mech.fire.FLAG.RESET=0;
//				}
//			}
//		}
//		OSTimeDly(200);	
//	}
//}

//static void Task_RESET(void *pdata)
//{
//	while(1)
//	{
//		OSTimeDly(200);	
//	}
//}


//static void Task_F1dBac(void *pdata)
//{
//	while(1)
//	{
//		OSTimeDly(200);
//	}
//}

//#ifdef LCD_TASK
//static void Task_LCD(void *pdata)
//{
//	while(1)
//	{
//		UsartLCDshow();
//		OSTimeDly(1000);
//	}
//}
//#endif

//#ifdef SCOPE_TASK
//static void Task_SCOPE(void *pdata)
//{
//	while(1)
//	{
//		VS4Channal_Send(VESCmotor[0].valueReal.speed, VESCmotor[0].valueSet.speed,\
//										VESCmotor[1].valueReal.speed, VESCmotor[1].valueSet.speed);	//visual_scope.c中的示波器发送函数
//		OSTimeDly(10);		//每1ms算一次，与定时器同步
//	}
//}

//#endif

static void Task_BEEP(void *pdata)
{
	pdata = pdata;
	for (;;)
	{
			BeepTask();
	}
}

static void Task_LED(void *pdata)
{
	while(1)
	{

		if(LedTask.Normal)
		{
			LED_GREEN_TOGGLE;
		}
		if(LedTask.Stuck)
		{
			LED_RED_TOGGLE;
		}
		if(LedTask.Timeout)
		{
			LED_YELLOW_TOGGLE;
		}
		if(LedTask.CANErr)
		{
			LED_BLUE_TOGGLE;
		}
		if(LedTask.CloseAll)
		{
			LED_BLUE_OFF;
			LED_GREEN_OFF;
			LED_RED_OFF;
			LED_YELLOW_OFF;
		}
		
		OSTimeDly(2000);
	}
}
