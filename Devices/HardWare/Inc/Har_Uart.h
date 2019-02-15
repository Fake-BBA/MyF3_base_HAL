#ifndef _HAR_UART_H__
#define _HAR_UART_H__

#include "stm32f3xx_hal.h"
#include "MyConfiguration.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

#define UART1_BUFF_SIZE	1000

enum BufferState{
	SEND_BUSY=0,	//���ڷ���
	SEND_FREE=1,	//���ڷ���
	SEND_COMPLETE=2,//�������
	
	RECV_BUSY=0,	//���ڽ���
	RECV_READED=1,	//���������Ѷ�
	RECV_COMPLETE=2	//�������
	
};

struct UartBuffer{
	enum BufferState stateFlag;
	uint32 length;	//���ݳ���
	uint32 startTime;	//��ʼ����ʱ��
	uint32 lastTime;	//��һ�ν���ʱ��
	uint32 buffSize;	//
	uint8 buffer[UART1_BUFF_SIZE];	//�����������Ϊ100
	
};

extern struct UartBuffer uart1_RX_Buffer;
extern struct UartBuffer uart1_TX_Buffer; 

void USART1_UART_Init(uint32 baudRate);
void USART3_UART_Init(uint32 baudRate);

uint8 UART1_SendBytes(uint8* p_bytes, uint8 len);
uint32 DMA_Usart1_Read(uint8_t *Data);
void DMA_UART1_SendThread();
#endif