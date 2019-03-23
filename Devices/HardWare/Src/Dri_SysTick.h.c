#include "Dri_SysTick.h"

uint32 sysTime;
//一个标准时间单位会包含多少个芯片的计时单位
const uint32 SYSTEM_HZ=1;	//1us
const uint32 SYSTEM_US=SYSTEM_HZ;
const uint32 SYSTEM_MS=SYSTEM_US*1000;
const uint32 SYSTEM_SEC=SYSTEM_MS*1000;
const uint32 SYSTEM_MIN=SYSTEM_SEC*60;
const uint32 SYSTEM_HOUR=SYSTEM_MIN*60;

uint8 SystemTime_Init()
{
	return SysTick_Init();
}

uint32 GetSystemTime()
{
	return sysTime;
}

/*
	timerTemp:时间记录器
	expect:期望等待的时间
	UINT:时间单位 可写 UINT_HZ、UINT_SEC、UINT_MIN、UINT_HOUR

*/
BOOL WaitSysTime(struct TimerTemp *timerTemp,uint32 expect,uint8 UINT)
{
	switch(timerTemp->state)
	{
		case UN_INIT:
					
			switch(UINT)
			{
				case UINT_HZ:
				
				break;
				case UINT_MS:
				expect*=SYSTEM_MS;
				break;
				case UINT_SEC:
				expect*=SYSTEM_SEC;
				break;
				case UINT_MIN:
				expect*=SYSTEM_MIN;
				break;
				case UINT_HOUR:
				expect*=SYSTEM_HOUR;
				break;
			}
			timerTemp->Time=sysTime+expect;	//初始化时间
			if(timerTemp->Time>sysTime)
			{
				timerTemp->state=NORMAL_COUNTING;	//开始计时状态
			}
			else
			{
				//时间溢出
				timerTemp->state=OVER_FLOW_COUNTING;	//开始计时状态
			}
			return FALSE;
			break;
			
		case NORMAL_COUNTING:
			if(timerTemp->Time>sysTime)		//比较时间
				return FALSE;
			else 
			{
				timerTemp->state=TIME_OUT;
				return TRUE;
			}
			break;
			
		case OVER_FLOW_COUNTING:
			if(sysTime&0x80000000) return FALSE;
			
			if(timerTemp->Time>sysTime)		//比较时间
				return FALSE;
			else 
			{
				timerTemp->state=TIME_OUT;
				return TRUE;
			}
			break;
			
		case TIME_OUT:
			return TRUE;
			break;
	}
	
	return FALSE;	
}

void ReSetTimerTemp(struct TimerTemp *timerTemp)
{
	timerTemp->state=UN_INIT;
	return;
}