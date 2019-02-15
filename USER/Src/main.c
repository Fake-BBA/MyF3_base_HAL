#include "stm32f3xx_hal.h"
#include "MyConfiguration.h"
#include "Devices.h"

unsigned char TxBuffer[100]="HelloWorld!";
unsigned char RxBuffer[100]="";
void SystemClock_Config(void);
int main(void)
{
	HAL_Init();				//初始化库
	//SystemClock_Config();	//初始化时钟
	Sys_LED_Init();
	Buzzer_Init();
	
	//Buzzer_ON();
	//HAL_Delay(500);	//5ms
	//Buzzer_OFF();
	/* GPIO Ports Clock Enable */
	//STM32_DMA_Init();
	USART1_UART_Init();
	//USART3_UART_Init();
	
	//HAL_UART_Receive_DMA(&huart1,RxBuffer,100);
	
	HAL_UART_Transmit(&huart1,"Init",4,9);
	while (1)
	{
		HAL_Delay(500);	//5ms
		Sys_LED_Negative();
		//Buzzer_Negative();
		HAL_UART_Transmit(&huart1,TxBuffer,4,15);
		
	}
}

//void SystemClock_Config(void)
//{

//  RCC_OscInitTypeDef RCC_OscInitStruct;
//  RCC_ClkInitTypeDef RCC_ClkInitStruct;
//  RCC_PeriphCLKInitTypeDef PeriphClkInit;

//    /**Initializes the CPU, AHB and APB busses clocks 
//    */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
//  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
//  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//  RCC_OscInitStruct.HSICalibrationValue = 16;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
//  HAL_RCC_OscConfig(&RCC_OscInitStruct);

//    /**Initializes the CPU, AHB and APB busses clocks 
//    */
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

//  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1;
//  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
//  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
//  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

//    /**Configure the Systick interrupt time 
//    */
//  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

//    /**Configure the Systick 
//    */
//  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

//  /* SysTick_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
//	
//}

//weak定义，库自动调用此接收中断函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{ 
	if(huart == &huart1)
	{
		HAL_UART_Receive_IT(&huart1,Uart1_Rx_Buffer,1);
		HAL_UART_Transmit(&huart1,TxBuffer,4,15);
	}
	
	if(huart == &huart3)
	{
		
	}
}



