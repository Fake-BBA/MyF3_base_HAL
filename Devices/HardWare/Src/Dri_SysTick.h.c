#include "Dri_SysTick.h"

uint32 sysTime;
//һ����׼ʱ�䵥λ��������ٸ�оƬ�ļ�ʱ��λ
const uint32 SYSTEM_HZ=1;	//1us
const uint32 SYSTEM_US=SYSTEM_HZ;
const uint32 SYSTEM_MS=SYSTEM_US*1;	//1ms Ϊһ��������ʱ��λ
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
	timerTemp:ʱ���¼��
	expect:�����ȴ���ʱ��
	UINT:ʱ�䵥λ ��д UINT_HZ��UINT_SEC��UINT_MIN��UINT_HOUR

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
			timerTemp->expectTime=expect;
			timerTemp->Time=sysTime+expect;	//��ʼ��ʱ��
			if(timerTemp->Time>sysTime)
			{
				timerTemp->state=NORMAL_COUNTING;	//��ʼ��ʱ״̬
			}
			else
			{
				//ʱ�����
				timerTemp->state=OVER_FLOW_COUNTING;	//��ʼ��ʱ״̬
			}
			return FALSE;
			break;
			
		case NORMAL_COUNTING:
			if(timerTemp->Time>sysTime)		//�Ƚ�ʱ��
				return FALSE;
			else 
			{
				timerTemp->state=TIME_OUT;
				return TRUE;
			}
			break;
			
		case OVER_FLOW_COUNTING:
			if(sysTime&0x80000000) return FALSE;
			
			if(timerTemp->Time>sysTime)		//�Ƚ�ʱ��
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

void InitTimerTemp(struct TimerTemp *timerTemp)
{
	timerTemp->state=UN_INIT;
}

void ReSetTimerTemp_Start(struct TimerTemp *timerTemp)
{
	timerTemp->Time=sysTime+timerTemp->expectTime;
	if(timerTemp->Time>sysTime)
	{
		timerTemp->state=NORMAL_COUNTING;	//��ʼ��ʱ״̬
	}
	else
	{
		//ʱ�����
		timerTemp->state=OVER_FLOW_COUNTING;	//��ʼ��ʱ״̬
	}
	
	return;
}