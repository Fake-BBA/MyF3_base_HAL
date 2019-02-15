#include "Har_LED.h"

void Sys_LED_PinInit()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	
	/*Configure GPIO pin : PB3 */											//LED
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	return;
}

void Sys_LED_PinSet()
{
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
	return;
}

void Sys_LED_PinReSet()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	return;
}

/*
	引脚电平取反

*/
void Sys_LED_PinNegative()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,
		(1-HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)));
	return;
}

/*
	返还引脚输出的值
*/
uint8 Sys_LED_PinState()
{
	return HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3);
}