#include "DJ3508.h"

static int i;
void DJ3508_SCOOP(void)
{
	DJmotor[0].begin=1;
	DJmotor[1].begin=1;
	if(i==1)
	{
		DJmotor[0].enable=1;
		DJmotor[0].valueSet.angle=angle1;
		OSTimeDly(100);
		DJmotor[0].Limit.PosSPLimit=1000;
		DJmotor[0].valueSet.angle=angle2;
		DJmotor[0].Limit.PosSPLimit=2000;
		DJmotor[0].valueSet.angle=angle1;
		DJmotor[0].enable=0;
	}
	if(i==2)
	{
		DJmotor[1].enable=1;
		DJmotor[1].Limit.PosSPLimit=1500;
		DJmotor[1].valueSet.angle=angle3;
		Valve_Ctrl(255);
		OSTimeDly(2000);
		Valve_Ctrl(0);
	}
	if(i==0)
	{
		DJmotor[0].enable=0;
		DJmotor[1].enable=0;
		DJmotor[0].begin=0;
		DJmotor[1].begin=0;
	}
}
