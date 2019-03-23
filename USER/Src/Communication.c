#include "Communication.h"
#include "Har_Uart.h"
#include "sensor.h"
#include "Attitude.h"

void SendStatus()
{
	int16 int16_pitch=flightState.attitude.pitch*100
	,int16_roll=flightState.attitude.roll*100,
	int16_yaw=flightState.attitude.yaw*100;
	
	int32 ALT_USE=0;	//�߶�
	uint8 flyModel=1;	//����ģʽ
	uint8 armed=0;		//0����
	
	uint8 sendBuff[20];
	uint8 pos=0;
	
	sendBuff[pos++]=0XAA;
	sendBuff[pos++]=0XAA;
	sendBuff[pos++]=0x01;	//������
	sendBuff[pos++]=0x00;		//���ݳ���
	
	sendBuff[pos++]=int16_pitch>>8;
	sendBuff[pos++]=int16_pitch;
	sendBuff[pos++]=int16_roll>>8;
	sendBuff[pos++]=int16_roll;
	sendBuff[pos++]=int16_yaw>>8;
	sendBuff[pos++]=int16_yaw;
	
	sendBuff[pos++]=ALT_USE>>24;
	sendBuff[pos++]=ALT_USE>>16;
	sendBuff[pos++]=ALT_USE>>8;
	sendBuff[pos++]=ALT_USE;
	
	sendBuff[pos++]=flyModel;
	
	sendBuff[pos++]=armed;
	
	sendBuff[3]=pos-4;
	uint8 sum=0;
	for(uint8 i=0;i<pos;i++)
	{
		sum+=sendBuff[i];
	}
	
	sendBuff[pos++]=sum;
	UART1_SendBytes(sendBuff,pos);
}

void SendSensor()
{
	uint8 sendBuff[20];
	uint8 pos=0;
	
	sendBuff[pos++]=0XAA;
	sendBuff[pos++]=0XAA;
	sendBuff[pos++]=0x02;	//������
	sendBuff[pos++]=0x00;		//���ݳ���
	
	sendBuff[pos++]=sensor.mpu6050.acc.x.H_L[1];
	sendBuff[pos++]=sensor.mpu6050.acc.y.H_L[0];
	sendBuff[pos++]=sensor.mpu6050.acc.y.H_L[1];
	sendBuff[pos++]=sensor.mpu6050.acc.y.H_L[0];
	sendBuff[pos++]=sensor.mpu6050.acc.z.H_L[1];
	sendBuff[pos++]=sensor.mpu6050.acc.z.H_L[0];
		
	sendBuff[pos++]=sensor.mpu6050.gyro.x.H_L[1];
	sendBuff[pos++]=sensor.mpu6050.gyro.x.H_L[0];
	sendBuff[pos++]=sensor.mpu6050.gyro.y.H_L[1];
	sendBuff[pos++]=sensor.mpu6050.gyro.y.H_L[0];
	sendBuff[pos++]=sensor.mpu6050.gyro.z.H_L[1];
	sendBuff[pos++]=sensor.mpu6050.gyro.z.H_L[0];
	
	sendBuff[pos++]=sensor.hmc5883l.x.H_L[1];
	sendBuff[pos++]=sensor.hmc5883l.x.H_L[0];
	sendBuff[pos++]=sensor.hmc5883l.y.H_L[1];
	sendBuff[pos++]=sensor.hmc5883l.y.H_L[0];
	sendBuff[pos++]=sensor.hmc5883l.z.H_L[1];
	sendBuff[pos++]=sensor.hmc5883l.z.H_L[0];
	
	
	sendBuff[3]=pos-4;
	uint8 sum=0;
	for(uint8 i=0;i<pos;i++)
	{
		sum+=sendBuff[i];
	}
	
	sendBuff[pos++]=sum;
	UART1_SendBytes(sendBuff,pos);
		
}

void RecvMessageThread()
{
	uint8 recvBuff[100];
	DMA_Usart1_Read(recvBuff);
	
	switch(recvBuff[2])
	{
		case 1:
			break;
		default:
			break;
		
	}
	
}