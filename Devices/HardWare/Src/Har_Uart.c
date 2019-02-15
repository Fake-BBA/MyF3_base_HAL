#include "Har_Uart.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

struct UartBuffer uart1_RX_Buffer;
struct UartBuffer uart1_TX_Buffer;

void UART_DMA_Init(UART_HandleTypeDef* uartHandle);

void USART1_UART_Init(uint32 baudRate)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = baudRate;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;	//高级设置，不取反电平
	
	__HAL_RCC_USART1_CLK_ENABLE();	//使能串口时钟
	__HAL_RCC_GPIOA_CLK_ENABLE();	//使能串口GPIO_A时钟
	HAL_UART_Init(&huart1);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	uart1_RX_Buffer.stateFlag=SEND_FREE;
	uart1_RX_Buffer.buffSize=UART1_BUFF_SIZE;
	
	uart1_TX_Buffer.stateFlag=RECV_READED;
	uart1_TX_Buffer.buffSize=UART1_BUFF_SIZE;
	
	/* USART1 interrupt Init */
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);	//使能IDLE中断
		
	UART_DMA_Init(&huart1);
}

void USART3_UART_Init(uint32 baudRate)
{
	huart3.Instance = USART3;
	huart3.Init.BaudRate = baudRate;
	huart3.Init.WordLength = UART_WORDLENGTH_9B;
	huart3.Init.StopBits = UART_STOPBITS_2;
	huart3.Init.Parity = UART_PARITY_EVEN;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_TXINVERT_INIT|UART_ADVFEATURE_RXINVERT_INIT;
	huart3.AdvancedInit.TxPinLevelInvert = UART_ADVFEATURE_TXINV_ENABLE;	//开启电平取反
	huart3.AdvancedInit.RxPinLevelInvert = UART_ADVFEATURE_RXINV_ENABLE;	//开启电平取反
	HAL_UART_Init(&huart3);
	
	/* USART3 interrupt Init */
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
	
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);	//使能IDLE中断
		
	UART_DMA_Init(&huart3);
}



//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄
void UART_DMA_Init(UART_HandleTypeDef* uartHandle)
{
	
	if(uartHandle->Instance==USART1)
	{
		/* USART1 DMA Init */
		/* USART1_RX Init */
		__HAL_RCC_DMA1_CLK_ENABLE();
		/* DMA1_Channel4_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
		
		 /* DMA1_Channel5_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
		
		hdma_usart1_rx.Instance = DMA1_Channel5;
		hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
		hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
		HAL_DMA_Init(&hdma_usart1_rx);

		__HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

		/* USART1_TX Init */
		hdma_usart1_tx.Instance = DMA1_Channel4;
		hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart1_tx.Init.Mode = DMA_NORMAL;
		hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
		HAL_DMA_Init(&hdma_usart1_tx);

		__HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

		
		//DMA接收函数，此句一定要加，不加接收不到第一次传进来的实数据，是空的，且此时接收到的数据长度为缓存器的数据长度
		HAL_UART_Receive_DMA(&huart1,uart1_RX_Buffer.buffer,uart1_RX_Buffer.buffSize);
	}
	else if(uartHandle->Instance==USART3)
	{
		/* USART3 DMA Init */
		/* USART3_RX Init */
		hdma_usart3_rx.Instance = DMA1_Channel3;
		hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart3_rx.Init.Mode = DMA_CIRCULAR;
		hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
		HAL_DMA_Init(&hdma_usart3_rx);

		__HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);

		/* USART3_TX Init */
		hdma_usart3_tx.Instance = DMA1_Channel2;
		hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
		hdma_usart3_tx.Init.Mode = DMA_NORMAL;
		hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
		HAL_DMA_Init(&hdma_usart3_tx);

		__HAL_LINKDMA(uartHandle,hdmatx,hdma_usart3_tx);		
	}	
}


uint8 UART1_SendBytes(uint8* p_bytes, uint8 len)
{
	extern struct UartBuffer uart1_TX_Buffer;
	memcpy(uart1_TX_Buffer.buffer+uart1_TX_Buffer.length,p_bytes,len);
	uart1_TX_Buffer.length+=len;			
}
/*
*********************************************************************************************************
*    函 数 名: DMA_Usart_Send
*    功能说明: 串口发送功能函数
*    形    参: buf，len
*    返 回 值: 无
*********************************************************************************************************
*/


void DMA_UART1_SendThread()
{
	if(uart1_TX_Buffer.length==0) return;
	if(HAL_UART_Transmit_DMA(&huart1, uart1_TX_Buffer.buffer,uart1_TX_Buffer.length)!= HAL_OK)
	{
		uart1_TX_Buffer.stateFlag = SEND_BUSY;
	}
	else
	{
		uart1_TX_Buffer.stateFlag = SEND_FREE;
		uart1_TX_Buffer.length=0;
	}	
}

/*
*********************************************************************************************************
*    函 数 名: DMA_Usart1_Read
*    功能说明: 串口接收功能函数
*    形    参: Data,len
*    返 回 值: 无
*********************************************************************************************************
*/
uint32 DMA_Usart1_Read(uint8_t *Data)//串口接收封装
{
	uint32 len;
	if(uart1_RX_Buffer.stateFlag ==RECV_COMPLETE)
	{
		uart1_RX_Buffer.stateFlag=RECV_READED;	//数据已读
		len=uart1_RX_Buffer.length;
		memcpy(Data,uart1_RX_Buffer.buffer,len);
		HAL_UART_Receive_DMA(&huart1,uart1_RX_Buffer.buffer,uart1_RX_Buffer.buffSize);//重新打开DMA接收
		//HAL_UART_DMASart(&huart1);
		return len;
	}
	else
	{
		return 0;
	}
		
}
