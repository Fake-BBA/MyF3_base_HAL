#ifndef _HAR_BUZZER_H__
#define _HAR_BUZZER_H__


#include "stm32f3xx_hal.h"
#include "MyConfiguration.h"

/*
PIN: PORT_C PIN15
DRE: LOW IS aval
*/

void Buzzer_PinInit();
void Buzzer_PinSet();
void Buzzer_PinReSet();
void Buzzer_PinNegative();
uint8 Buzzer_PinState();
#endif