#include "stm32f3xx_hal.h"
#include "MyConfiguration.h"
#include "Devices.h"

int main(void)
{
	HAL_Init();				//初始化库
	Sys_LED_Init();
	Buzzer_Init();
	
	//STM32_DMA_Init();
	USART1_UART_Init(115200);
	//USART3_UART_Init();
	//HAL_UART_Receive_DMA(&huart1,RxBuffer,100);
	
	//HAL_UART_Transmit(&huart1,"Init",4,9);
	uint8 test[100];
	uint32 n;
	while (1)
	{
		
		//HAL_Delay(500);	//5ms
		Sys_LED_Negative();
		n=DMA_Usart1_Read(test);
		UART1_SendBytes(test,n);
		DMA_UART1_SendThread();
	}
	
}

//UART_DMATransmitCplt
