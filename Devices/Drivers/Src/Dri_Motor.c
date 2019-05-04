#include "Dri_Motor.h"
#include "Har_PWM.h"
#include "Dri_SysTick.h"

struct TimerTemp hight_6s;
struct TimerTemp low_4s;

bool motoInitDone;
	
void Motor_Init()
{
	Init_PWM();	//初始化PWM，包括引脚
	InitTimerTemp(&hight_6s);
	InitTimerTemp(&low_4s);
	motoInitDone=0;	//未初始化完成
}

void Motor_Thread()
{
	//上电3S内
	if(!WaitSysTime(&hight_6s,6,UINT_SEC))
	{
		PWM1(MAX_PWM);
		PWM2(MAX_PWM);
		PWM3(MAX_PWM);
		PWM4(MAX_PWM);
	}
	else
	if(!WaitSysTime(&low_4s,4,UINT_SEC))
	{
		PWM1(MIN_PWM);
		PWM2(MIN_PWM);
		PWM3(MIN_PWM);
		PWM4(MIN_PWM);
		motoInitDone=1;
	}
	else
	{
		//测试
		SetMotor_1(MIN_PWM+200);
		SetMotor_2(MIN_PWM+200);
		SetMotor_3(MIN_PWM+200);
		SetMotor_4(MIN_PWM+200);
	}
}

void SetMotor_1(uint32 value)
{
	if(motoInitDone)
		PWM1(value);
}

void SetMotor_2(uint32 value)
{
	if(motoInitDone)
		PWM2(value);
}

void SetMotor_3(uint32 value)
{
	if(motoInitDone)
		PWM3(value);
}

void SetMotor_4(uint32 value)
{
	if(motoInitDone)
		PWM4(value);
}