#include "Dri_Buzzer.h"

void Buzzer_Init()
{
	Buzzer_PinInit();
	return;
}

void Buzzer_ON()
{
	//高电平响
	Buzzer_PinSet();
	return;
}

void Buzzer_OFF()
{
	Buzzer_PinReSet();
	return;
}

void Buzzer_Negative()
{
	Buzzer_PinNegative();
	return;
}