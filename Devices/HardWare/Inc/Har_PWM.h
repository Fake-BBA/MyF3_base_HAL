#ifndef _HAR_PWM_H__
#define _HAR_PWM_H__

#include "MyTypedef.h"

//500HZ
#define PWM_PERIOD	36000	//周期	

//400HZ
//#define PWM_PERIOD	45000	//周期

#define MAX_PWM	PWM_PERIOD-200
#define MIN_PWM		1000

void Init_PWM();

void PWM1(uint32 compare);
void PWM2(uint32 compare);
void PWM3(uint32 compare);
void PWM4(uint32 compare);
void PWM5(uint32 compare);
void PWM6(uint32 compare);
void PWM7(uint32 compare);
void PWM8(uint32 compare);
	
#endif