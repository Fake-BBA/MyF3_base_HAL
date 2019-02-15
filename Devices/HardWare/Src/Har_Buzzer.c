#include "Har_Buzzer.h"

void Buzzer_PinInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();	
	
	/*Configure GPIO pin : PC15 */											//LED
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	return;
} 

void Buzzer_PinSet()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_SET);
	return;
}

void Buzzer_PinReSet()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
	return;
}

/*
	引脚电平取反

*/
void Buzzer_PinNegative()
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,
		(1-HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_15)));
	return;
}

uint8 Buzzer_PinState()
{
	return HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_15);
}