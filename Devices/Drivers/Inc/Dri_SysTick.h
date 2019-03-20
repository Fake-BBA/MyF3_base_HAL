#ifndef _DRI_SYSTICK_H__
#define _DRI_SYSTICK_H__

#include "Har_SysTick.h"

uint8 SystemTime_Init();
uint32 GetSystemTime();
void DelayUs_Block(uint32 nTime);
void DelayMS_Block(uint32 nTime);
void DelaySec_Block(uint32 nTime);
void DelayMin_Block(uint32 nTime);
void DelayHour_Block(uint32 nTime);

#define UINT_US		0
#define UINT_MS		1
#define UINT_SEC	2
#define UINT_MIN	3
#define UINT_HOUR	4

enum TimerState{
	UN_INIT=0,	//时间未初始化
	COUNTING=1,	//正在计时
	PAUSE=2,	//暂停计时
};

struct TimerTemp{
	uint32	Time;
	enum TimerState	state;
};


//等待时间，如果时间未到返回FALSE=0;时间到返回TRUE
//大量使用指针原因是因为该单片机直接值传递消耗很多资源
BOOL WaitSysTime_UnBlocked(struct TimerTemp *timerTemp,uint32 expect,uint8 uint);
void ReSetTimerTemp(struct TimerTemp *timerTemp);


#endif