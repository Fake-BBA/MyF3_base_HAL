#include "Har_LED.h"

void Sys_LED_Init()
{
	Sys_LED_PinInit();
	return;
}
void Sys_LED_ON()
{
	//低电平点亮
	Sys_LED_PinReSet();
	return;
}

void Sys_LED_OFF()
{
	//高电平熄灭
	Sys_LED_PinSet();
	return;
}

void Sys_LED_Negative()
{
	Sys_LED_PinNegative();
	return;
}

