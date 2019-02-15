#ifndef _HAR_UART_H__
#define _HAR_UART_H__

#include "stm32f3xx_hal.h"
#include "MyConfiguration.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

#define UART1_BUFF_SIZE	1000

enum BufferState{
	SEND_BUSY=0,	//正在发送
	SEND_FREE=1,	//不在发送
	SEND_COMPLETE=2,//发送完成
	
	RECV_BUSY=0,	//正在接收
	RECV_READED=1,	//接收数据已读
	RECV_COMPLETE=2	//接收完成
	
};

struct UartBuffer{
	enum BufferState stateFlag;
	uint32 length;	//内容长度
	uint32 startTime;	//开始接收时间
	uint32 lastTime;	//上一次接收时间
	uint32 buffSize;	//
	uint8 buffer[UART1_BUFF_SIZE];	//最大数据容量为100
	
};

extern struct UartBuffer uart1_RX_Buffer;
extern struct UartBuffer uart1_TX_Buffer; 

void USART1_UART_Init(uint32 baudRate);
void USART3_UART_Init(uint32 baudRate);

uint8 UART1_SendBytes(uint8* p_bytes, uint8 len);
uint32 DMA_Usart1_Read(uint8_t *Data);
void DMA_UART1_SendThread();
#endif