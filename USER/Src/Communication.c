#include "Communication.h"
#include "Har_Uart.h"
#include "sensor.h"
#include "Attitude.h"
#include "Har_PWM.h"

enum FlightFuntionWord funtionWorld;

struct RcData rcData;

void SendStatus()
{
	int16 int16_roll=flightState.attitude.pitch*100
	,int16_pitch=flightState.attitude.roll*100,
	int16_yaw=flightState.attitude.yaw*100;
	
	int32 ALT_USE=0;	//高度
	uint8 flyModel=1;	//飞行模式
	uint8 armed=0;		//0加锁
	
	uint8 sendBuff[20];
	uint8 pos=0;
	
	sendBuff[pos++]=0XAA;
	sendBuff[pos++]=0XAA;
	sendBuff[pos++]=0x01;	//功能字
	sendBuff[pos++]=0x00;		//数据长度
	
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
	sendBuff[pos++]=0x02;	//功能字
	sendBuff[pos++]=0x00;		//数据长度
	
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

void SendLoopTime()
{
	extern uint32 loopTimeIMU1,loopTimeIMU2,loopTimeMPU1,loopTimeMPU2;
	
	uint8 sendBuff[20];
	uint8 pos=0;
	
	sendBuff[pos++]=0XAA;
	sendBuff[pos++]=0XAA;
	sendBuff[pos++]=LOOP_TIME;	//功能字
	sendBuff[pos++]=0x00;		//数据长度
	
	sendBuff[pos++]=loopTimeIMU1>>24;
	sendBuff[pos++]=loopTimeIMU1>>16;
	sendBuff[pos++]=loopTimeIMU1>>8;
	sendBuff[pos++]=loopTimeIMU1;
	
	sendBuff[pos++]=loopTimeMPU1>>24;
	sendBuff[pos++]=loopTimeMPU1>>16;
	sendBuff[pos++]=loopTimeMPU1>>8;
	sendBuff[pos++]=loopTimeMPU1;
	
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
	static uint8 recvBuff[100];
	uint16 length=DMA_Usart1_Read(recvBuff);
	
	rcData.Throttle=(int16)(recvBuff[4]<<8)+recvBuff[5];
	rcData.Yaw=(int16)recvBuff[6]<<8|recvBuff[7];
	rcData.Roll=(int16)recvBuff[8]<<8|recvBuff[9];
	rcData.Pitch=(int16)recvBuff[10]<<8|recvBuff[11];
	rcData.AUX1=(int16)recvBuff[12]<<8|recvBuff[13];
	rcData.AUX2=(int16)recvBuff[14]<<8|recvBuff[15];
	rcData.AUX3=(int16)recvBuff[16]<<8|recvBuff[17];
	rcData.AUX4=(int16)recvBuff[18]<<8|recvBuff[19];
	rcData.AUX5=(int16)recvBuff[20]<<8|recvBuff[21];
	rcData.AUX6=(int16)recvBuff[22]<<8|recvBuff[23];

	
	PWM1(PWM_PERIOD/100*rcData.Throttle);
	PWM2(PWM_PERIOD/100*rcData.Yaw);
	PWM3(PWM_PERIOD/100*rcData.Roll);
	PWM4(PWM_PERIOD/100*rcData.Pitch);
	
}