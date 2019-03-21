/* Includes ------------------------------------------------------------------*/
#include "Devices.h"
#include "stm32f3xx_it.h"
   
extern DMA_HandleTypeDef hdma_i2c1_rx;
extern DMA_HandleTypeDef hdma_i2c1_tx;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim7;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;


void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
	extern uint32 systemTime;
	extern bool bool_mainLoop;
	systemTime++;
	bool_mainLoop=1;
}

/******************************************************************************/
/*                 STM32F3xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) , for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f3xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
void DMA1_Channel2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart3_tx);
}

void DMA1_Channel3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart3_rx);
}

void DMA1_Channel4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

void DMA1_Channel5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

void DMA1_Channel6_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_i2c1_tx);
}

void DMA1_Channel7_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&hdma_i2c1_rx);
}

void I2C1_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(&hi2c1);
}

void I2C1_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(&hi2c1);
}

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart1);
	if(USART1 == huart1.Instance)
	{
		if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE) != RESET)//idle标志被置位
		{
			uart1_RX_Buffer.stateFlag = RECV_COMPLETE;// 接受完成标志位
			__HAL_UART_CLEAR_IDLEFLAG(&huart1);//清除标志位

			HAL_UART_DMAStop(&huart1); //
			//总计数减去未传输的数据个数，得到已经接收的数据个数
			uart1_RX_Buffer.length =uart1_RX_Buffer.buffSize - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
		}
	}
}

void USART3_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart3);
}

void TIM7_IRQHandler(void)
{
	//HAL_TIM_IRQHandler(&htim7);
}

void EXTI15_10_IRQHandler()
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);	//PC13
}

uint32 INT_Times=0;	//中断次数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	INT_Times++;
	SensorThread();	//传感器数据读取与处理
}
