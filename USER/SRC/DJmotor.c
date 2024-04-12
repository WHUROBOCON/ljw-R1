#include "DJmotor.h"

u16 ShouZhua_Sp=3000; 

DJMotor DJmotor[8];

float TestKP=1.3,TestKI=0.2,TestKD=0;
DJMotorParamTypedef M2006Param,M3508Param;
DJMotorStatusTypedef Status;
DJMotorLimitTypedef MotorLimit;
DJMotorArgumTypedef MotorArgum;
Inc_PIDTypedef PID_POS,PID_RPM;
myPIDTypedef myPID_POS,myPID_RPM;

void DJmotor_Init(void)
{
	M3508Param.PulsePerRound = 8192;
	M3508Param.RATIO = 19;
	M3508Param.Current_Limit = 15000;	//-16384 ~ 16384 - -20A ~20A
	M3508Param.GearRadio = 3.0;

	M2006Param.PulsePerRound = 8192;
	M2006Param.RATIO = 36;
	M2006Param.Current_Limit =9000;  //-10000 ~ 10000 - -10A~10A
	M2006Param.GearRadio = 1.0f;
		
	MotorLimit.PosSPLimit_ON = true;
	MotorLimit.PosSPLimit = 2500;	//轴前 
	MotorLimit.maxAngle = 270;
	MotorLimit.minAngle = -270;
	
	MotorLimit.ZeroSP = 500;
	MotorLimit.ZeroCurrent = 3000;
	
	MotorLimit.stuckDetection_ON =true;
	MotorLimit.timeoutDetection_ON = true;		
	MotorLimit.isReleaseWhenStuck = true;
		
	MotorArgum.lockPulse = 0;

	for(short i=0;i<=7;i++)
	{
		DJmotor[i].Limit = MotorLimit;
		DJmotor[i].argum = MotorArgum;
	}
//	DJmotor[6].Limit.PosSPLimit=10000;
	
	DJmotor[0].Param = M3508Param;
	DJmotor[0].enable = false;
	DJmotor[0].begin = false;
	DJmotor[0].mode = DJPOSITION;
	DJmotor[0].valueSet.angle=0;
	DJmotor[0].valueSet.speed=4000;
	DJmotor[0].valueSet.current=0;
	DJmotor[0].Limit.PosSPLimit=3000;
	Inc_PID_Init(&DJmotor[0].PID_POS, 2.0, 0.08, 0.0,DJmotor[0].valueSet.pulse);
	Inc_PID_Init(&DJmotor[0].PID_RPM, 8.0,  0.3, 0.0,DJmotor[0].valueSet.speed);

	DJmotor[1].Param = M2006Param;
	DJmotor[1].enable = false;
	DJmotor[1].begin = false;
	DJmotor[1].mode = DJPOSITION;
	DJmotor[1].valueSet.angle=0;
	DJmotor[1].valueSet.speed=100;
	DJmotor[1].valueSet.current=0;
	Inc_PID_Init(&DJmotor[1].PID_POS, 2.0, 0.08, 0.0,DJmotor[1].valueSet.pulse);
	Inc_PID_Init(&DJmotor[1].PID_RPM, 8.0,  0.3, 0.0,DJmotor[1].valueSet.speed);
	//myPID_Init(&DJmotor[1].myPID_RPM, 0.4,  0.2, 0.2,DJmotor[1].valueSet.speed);

	DJmotor[2].Param = M3508Param;
	DJmotor[2].enable = false;
	DJmotor[2].begin = false;
	DJmotor[2].mode = DJPOSITION;
	DJmotor[2].valueSet.angle=0;
	DJmotor[2].valueSet.speed=100;
	DJmotor[2].valueSet.current=0;
	Inc_PID_Init(&DJmotor[2].PID_POS, 2.0, 0.08, 0.0,DJmotor[2].valueSet.pulse);
	Inc_PID_Init(&DJmotor[2].PID_RPM, 8.0,  0.3, 0.0,DJmotor[2].valueSet.speed);

	DJmotor[3].Param = M3508Param;
	DJmotor[3].enable = false;
	DJmotor[3].begin = false;
	DJmotor[3].mode = DJPOSITION;
	DJmotor[3].valueSet.angle=0;
	DJmotor[3].valueSet.speed=100;
	DJmotor[3].valueSet.current=0;
	Inc_PID_Init(&DJmotor[3].PID_POS, 2.0, 0.08, 0.0,DJmotor[3].valueSet.pulse);
	Inc_PID_Init(&DJmotor[3].PID_RPM, 8.0,  0.3, 0.0,DJmotor[3].valueSet.speed);

	/*转向电机*/
	DJmotor[PitchID].Param = M2006Param;
	DJmotor[PitchID].enable = false;
	DJmotor[PitchID].begin = false;
	DJmotor[PitchID].mode = DJPOSITION;
	DJmotor[PitchID].valueSet.angle=0;
	DJmotor[PitchID].valueSet.speed=100;
	DJmotor[PitchID].valueSet.current=0;
	Inc_PID_Init(&DJmotor[PitchID].PID_POS, 1.3, 0.2, 0.0,DJmotor[PitchID].valueSet.pulse);
	Inc_PID_Init(&DJmotor[PitchID].PID_RPM, 8.0,  0.5, 0.0,DJmotor[PitchID].valueSet.speed);
	DJmotor[PitchID].Param.GearRadio=8.0f;
	
	DJmotor[YawID].Param = 	M2006Param;
	DJmotor[YawID].enable = false;
	DJmotor[YawID].begin = false;
	DJmotor[YawID].mode = DJPOSITION;
	DJmotor[YawID].valueSet.angle=-70;
	DJmotor[YawID].valueSet.speed=100;
	DJmotor[YawID].valueSet.current=0;
	Inc_PID_Init(&DJmotor[YawID].PID_POS, TestKP, TestKI, TestKD ,DJmotor[YawID].valueSet.pulse);
	Inc_PID_Init(&DJmotor[YawID].PID_RPM, 8.0,  0.5, 0.0,DJmotor[YawID].valueSet.speed);
	DJmotor[YawID].Param.GearRadio=4.0f;
	
	DJmotor[6].Param = M2006Param;
	DJmotor[6].enable = false;
	DJmotor[6].begin = false;
	DJmotor[6].mode = DJPOSITION;
	DJmotor[6].valueSet.angle=0;
	DJmotor[6].valueSet.speed=100;
	DJmotor[6].valueSet.current=0;
	Inc_PID_Init(&DJmotor[6].PID_POS, 0.4, 0.13, 0,DJmotor[6].valueSet.pulse);
	Inc_PID_Init(&DJmotor[6].PID_RPM, 8.0,  0.2, 0.0,DJmotor[6].valueSet.speed);
	DJmotor[6].Param.GearRadio=105.0f/20.0f;
	
	DJmotor[RotateID].Param = M2006Param;
	DJmotor[RotateID].enable = false;
	DJmotor[RotateID].begin = false;
	DJmotor[RotateID].mode = DJPOSITION;
	DJmotor[RotateID].valueSet.angle=-180;
	DJmotor[RotateID].valueSet.speed=0;
	DJmotor[RotateID].valueSet.current=0;
	Inc_PID_Init(&DJmotor[RotateID].PID_POS, 1.3, 0.2, 0.0,DJmotor[RotateID].valueSet.pulse);
	Inc_PID_Init(&DJmotor[RotateID].PID_RPM, 8.0,  0.5, 0.0,DJmotor[RotateID].valueSet.speed);
	DJmotor[RotateID].Param.GearRadio=108.0f/17.0f;
}

void DJ_SetZero(u8 id,float angle)
{
	DJmotor[id].Status.isSetZero = false;
	DJmotor[id].valueReal.pulse = (angle/360)*DJmotor[id].Param.PulsePerRound * DJmotor[id].Param.RATIO * DJmotor[id].Param.GearRadio;		
	DJmotor[id].valueReal.angle = angle;
	DJmotor[id].argum.lockPulse = (angle/360)*DJmotor[id].Param.PulsePerRound * DJmotor[id].Param.RATIO * DJmotor[id].Param.GearRadio;
}

float DJMotor3CCurrent;
void DJ_Position_Calculate(u8 id)
{
	DJmotor[id].argum.distance = DJmotor[id].valueReal.pulseRead - DJmotor[id].valuePrv.pulseRead;	//每次计算转过的距离
	DJmotor[id].valuePrv = DJmotor[id].valueReal;
	
	if( ABS(DJmotor[id].argum.distance)>4096)	//限定阈值，判断正反
		DJmotor[id].argum.distance = DJmotor[id].argum.distance - PlusOrMinus(DJmotor[id].argum.distance) * DJmotor[id].Param.PulsePerRound;
	
	DJmotor[id].valueReal.pulse = DJmotor[id].valueReal.pulse + DJmotor[id].argum.distance; //通过pulse和angle纪录绝对位置
	DJmotor[id].valueReal.angle = DJmotor[id].valueReal.pulse * 360.0 / DJmotor[id].Param.PulsePerRound / DJmotor[id].Param.RATIO / DJmotor[id].Param.GearRadio;
	
	if(DJmotor[id].begin)	//锁点位置纪录
		DJmotor[id].argum.lockPulse = DJmotor[id].valueReal.pulse;
	if(DJmotor[id].Status.isSetZero)
	{
		if(id==7)
			DJ_SetZero(id,-180);
		else if(id==5)
			DJ_SetZero(id,-59);
		else
			DJ_SetZero(id,0);
	}
	
	DJMotor3CCurrent = (float)(DJmotor[3].valueReal.current / 16384.0 * 20.0);
}

void DJ_SpeedMode(u8 id)		//速度是增量式PID
{
	DJmotor[id].PID_RPM.SetVal = DJmotor[id].valueSet.speed;
	DJmotor[id].PID_RPM.CurVal = DJmotor[id].valueReal.speed;
	Inc_PID_Operation(&DJmotor[id].PID_RPM);
	DJmotor[id].valueSet.current += DJmotor[id].PID_RPM.delta;
//		DJmotor[id].myPID_RPM.SetVal = DJmotor[id].valueSet.speed;
//		DJmotor[id].myPID_RPM.CurVal = DJmotor[id].valueReal.speed;
//		myPID_Operation(&DJmotor[id].myPID_RPM);
//		DJmotor[id].valueSet.current = DJmotor[id].myPID_RPM.result;
}

void DJ_LockPosition(u8 id)
{
	DJmotor[id].PID_POS.SetVal = DJmotor[id].argum.lockPulse;
	DJmotor[id].PID_POS.CurVal = DJmotor[id].valueReal.pulse;
	Inc_PID_Operation(&DJmotor[id].PID_POS);
	
	DJmotor[id].PID_RPM.SetVal = DJmotor[id].PID_POS.delta;
	DJmotor[id].PID_RPM.CurVal = DJmotor[id].valueReal.speed;
	Inc_PID_Operation(&DJmotor[id].PID_RPM);
	DJmotor[id].valueSet.current += DJmotor[id].PID_RPM.delta;
}

void DJ_PostionMode(u8 id)		//位置环是奇奇怪怪的PID，速度环是增量式PID
{
	DJmotor[id].valueSet.pulse = DJmotor[id].valueSet.angle * DJmotor[id].Param.RATIO *DJmotor[id].Param.PulsePerRound *DJmotor[id].Param.GearRadio /360;
  DJmotor[id].PID_POS.SetVal = DJmotor[id].valueSet.pulse;
	if(DJmotor[id].Limit.PosLimit_ON)
	{
		if(DJmotor[id].PID_POS.SetVal>DJmotor[id].Limit.maxAngle *DJmotor[id].Param.PulsePerRound *DJmotor[id].Param.RATIO *DJmotor[id].Param.GearRadio /360)
			DJmotor[id].PID_POS.SetVal = DJmotor[id].Limit.maxAngle *DJmotor[id].Param.PulsePerRound *DJmotor[id].Param.RATIO *DJmotor[id].Param.GearRadio /360;
		if(DJmotor[id].PID_POS.SetVal<DJmotor[id].Limit.minAngle *DJmotor[id].Param.PulsePerRound *DJmotor[id].Param.RATIO *DJmotor[id].Param.GearRadio /360)
			DJmotor[id].PID_POS.SetVal = DJmotor[id].Limit.minAngle *DJmotor[id].Param.PulsePerRound *DJmotor[id].Param.RATIO *DJmotor[id].Param.GearRadio /360;
	}
	DJmotor[id].PID_POS.CurVal = DJmotor[id].valueReal.pulse;
	Inc_PID_Operation(&DJmotor[id].PID_POS);
	DJmotor[id].PID_RPM.SetVal = DJmotor[id].PID_POS.delta;
	 if(DJmotor[id].Limit.PosSPLimit_ON)
		 PEAK(DJmotor[id].PID_RPM.SetVal,DJmotor[id].Limit.PosSPLimit);
	 DJmotor[id].PID_RPM.CurVal = DJmotor[id].valueReal.speed;
	 Inc_PID_Operation(&DJmotor[id].PID_RPM);
	 DJmotor[id].valueSet.current += DJmotor[id].PID_RPM.delta;
	 
	 if(ABS(DJmotor[id].valueSet.pulse - DJmotor[id].valueReal.pulse)<60)
		 DJmotor[id].Status.arrived = true;
	 else
		 DJmotor[id].Status.arrived = false;
}

void DJ_ZeroMode(u8 id)
{
	DJmotor[id].PID_RPM.SetVal = DJmotor[id].Limit.ZeroSP;
	DJmotor[id].PID_RPM.CurVal = DJmotor[id].valueReal.speed;
	Inc_PID_Operation(&DJmotor[id].PID_RPM);
	DJmotor[id].valueSet.current += DJmotor[id].PID_RPM.delta;
	
	PEAK(DJmotor[id].valueSet.current,DJmotor[id].Limit.ZeroCurrent);
	 if(ABS(DJmotor[id].argum.distance)<10)
		 DJmotor[id].argum.ZeroCnt++;
	 else 
		 DJmotor[id].argum.ZeroCnt = 0;
	 
	 if(DJmotor[id].argum.ZeroCnt>100)
	 {
		 DJmotor[id].argum.ZeroCnt = 0;
		 DJ_SetZero(id,0);
		 DJmotor[id].begin=0;
		 DJmotor[id].mode= DJPOSITION;
//		 if(id==4||id==5)
//		 {
//			 mech.fire.FLAG.PitchRESETOver=1;
//		 }
//		 if(id==6)
//		 {
//			 mech.fire.FLAG.YawRESETOver=1;
//		 }
	 }
}

void DJ_CurrentTransmit(u8 id)
{
	static CanTxMsg DJtx_message;		//放在里面/放在外面？
	
	PEAK(DJmotor[id].valueSet.current,DJmotor[id].Param.Current_Limit);
	
	if(!DJmotor[id].enable)
		DJmotor[id].valueSet.current=0;
	
	if(id<4)
		DJtx_message.StdId = 0x200;
	else
		DJtx_message.StdId = 0x1FF;
	
	DJtx_message.RTR = CAN_RTR_DATA;
	DJtx_message.IDE = CAN_Id_Standard;
	DJtx_message.DLC = 8;
	
	u8 temp = 2*(id&0x0B);
	EncodeS16Data(&DJmotor[id].valueSet.current,&DJtx_message.Data[temp]);
	ChangeData(&DJtx_message.Data[temp],&DJtx_message.Data[temp+1]);
		
	if((id==3)||(id==7))
		CAN_Transmit(CAN2,&DJtx_message);
}

void DJ_ifTimeOut(u8 id)
{
	if( (DJmotor[id].enable==1) && (DJmotor[id].argum.lastRxTime++)>30 )	//300ms未检测到反馈信号报错
	{
		DJmotor[id].argum.TimeoutCnt++;
		if( DJmotor[id].argum.TimeoutCnt >10 )
		{
			DJmotor[id].Status.timeout = 1;
		}
		else 
		{
			DJmotor[id].Status.timeout=0;
		}
	}
	else 
		DJmotor[id].argum.TimeoutCnt=0;
}

void DJ_ifStuck(u8 id)
{
	if( DJmotor[id].enable == 1 )
	{
		if(DJmotor[id].mode==DJRPM)
		{	
			if(ABS(DJmotor[id].argum.distance)<5)		
			{
				DJmotor[id].argum.StuckCnt++;
					if(DJmotor[id].argum.StuckCnt>3000)	
						{	
							DJmotor[id].Status.stuck=1;
							DJmotor[id].argum.StuckCnt=0;
							if(DJmotor[id].Limit.isReleaseWhenStuck)	DJmotor[id].enable=0;
						}
			}
			else DJmotor[id].argum.StuckCnt=0;	
		}
		else if(DJmotor[id].mode==DJPOSITION)
		{
			if( (ABS(DJmotor[id].valueSet.angle-DJmotor[id].valueReal.angle)>1) && (ABS(DJmotor[id].argum.distance)<5) )
			{
				DJmotor[id].argum.StuckCnt++;
					if( DJmotor[id].argum.StuckCnt>3000 )	
					{
						DJmotor[id].Status.stuck=1;
						DJmotor[id].argum.StuckCnt=0;
						if(DJmotor[id].Limit.isReleaseWhenStuck)	DJmotor[id].enable=0;
					}
			}
			else 
				DJmotor[id].argum.StuckCnt=0;
		}
	}
}

void DJFunc(void)
{
		for(u8 i=0;i<=7;i++)
		{
			if(DJmotor[i].enable)		//在这里打断点,为啥只有0和1？
			{
				if(DJmotor[i].Limit.timeoutDetection_ON) {DJ_ifTimeOut(i);}	
				if(DJmotor[i].Limit.stuckDetection_ON)	{DJ_ifStuck(i);}
				
				if(DJmotor[i].begin)
				{
					switch(DJmotor[i].mode)
					{
						case DJRPM: DJ_SpeedMode(i);	break;
						case DJPOSITION:	DJ_PostionMode(i);	break;
						case DJZERO:	DJ_ZeroMode(i);		break;
						default: 	break;
					}
				}
				else
					DJ_LockPosition(i);
			}
			DJ_CurrentTransmit(i);
		}
}

