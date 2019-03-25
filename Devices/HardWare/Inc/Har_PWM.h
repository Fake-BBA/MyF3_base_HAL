#ifndef _HAR_PWM_H__
#define _HAR_PWM_H__

#include "MyTypedef.h"

#define PWM_PERIOD	36000	//ÖÜÆÚ	

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