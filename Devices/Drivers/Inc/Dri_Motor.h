#ifndef _DRI_MOTOR_H__
#define _DRI_MOTOR_H__

#include "MyTypedef.h"

void Motor_Init();
void Motor_Thread();

void SetMotor_1(uint32 value);
void SetMotor_2(uint32 value);
void SetMotor_3(uint32 value);
void SetMotor_4(uint32 value);
#endif