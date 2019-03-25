#ifndef _DRI_SYSTICK_H__
#define _DRI_SYSTICK_H__

#include "Har_SysTick.h"

uint8 SystemTime_Init();
uint32 GetSystemTime();

void GetSysTime(uint32 *obj);

#define UINT_HZ		1
#define UINT_MS		2
#define UINT_SEC	3
#define UINT_MIN	4
#define UINT_HOUR	5

enum TimerState{
	UN_INIT=0,	//时间未初始化
	NORMAL_COUNTING,	//正在计时
	OVER_FLOW_COUNTING,	//溢出计时
	TIME_OUT,			//期待时间已到
	PAUSE,	//暂停计时
	
};

struct TimerTemp{
	uint32	Time;
	enum TimerState	state;
};

uint32 GetSystemTime();

//等待时间，如果时间未到返回FALSE=0;时间到返回TRUE
//大量使用指针原因是因为该单片机为8bit单片机，直接值传递消耗很多资源
//期待值不可超过0.5小时，否则会出错
BOOL WaitSysTime(struct TimerTemp *timerTemp,uint32 expect,uint8 uint);
void ReSetTimerTemp(struct TimerTemp *timerTemp);


#endif