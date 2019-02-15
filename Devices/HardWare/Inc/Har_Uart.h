#ifndef _HAR_UART_H__
#define _HAR_UART_H__

#include "stm32f3xx_hal.h"
#include "MyConfiguration.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern unsigned char Uart1_Rx_Buffer[];

void USART1_UART_Init(void);
void USART3_UART_Init(void);
#endif