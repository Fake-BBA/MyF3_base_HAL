#ifndef __HAR_SYS_LED_H__
#define __HAR_SYS_LED_H__

#include "stm32f3xx_hal.h"
#include "MyConfiguration.h"

void Sys_LED_PinInit();
void Sys_LED_PinSet();
void Sys_LED_PinReSet();
void Sys_LED_PinNegative();	//引脚电平取反
uint8 Sys_LED_PinState();
#endif