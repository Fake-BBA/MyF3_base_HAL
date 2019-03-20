#include "Dri_MPU6050.h"
#include "sensor.h"

uint8_t MPU6050_Init1[3] = {0x00, //0x6A 采用IIC接口
							0x01,	//0x6B 以陀螺仪X轴作为时钟，唤醒MPU6050，启用温度传感器
							0x00};	//0x6C 
uint8_t MPU6050_Init2[2] = {0x80,	//0x37 表示低电平为中断的有效信号
							0x00};	//0x38	禁止了所有中断源
uint8_t MPU6050_Init3[1] = {0x00};	//0x23 表示加速度计，陀螺仪、温度计等的值都不写进FIFO
uint8_t MPU6050_Init4[4] = {0x13,	//0x19 表示陀螺仪采样率 设置采样速率: 1000 / (1 + 0x13) = 50Hz,设置采样速率: 1000 / (1 + 0) = 1000Hz
							0x02,	//0x1A 设置低通滤波为98HZ
							0x18,	//0x1B 陀螺仪不自检，并且量程为 2000 °/s
							0x18};	//0x1C 加速度计不自检，并且量程为16g

uint8_t MPU6050_Data[14];																							//例程为了兼顾气压计（AD时间至少20ms），系统频率定为10Hz


void Init_MPU6050()
{
	/*
	*@param  DevAddress Target device address: The device 7 bits address value
	*         in datasheet must be shift at right before call interface@param  MemAddress Internal memory address
	* @param  MemAddSize Size of internal memory address 目标器件内部的大小，即起始地址
	* @param  pData Pointer to data buffer
	* @param  Size Amount of data to be sent
	* @param  Timeout Timeout duration
	* @retval HAL status
	*/
	HAL_I2C_Mem_Write(&hi2c1,ADDR_MPU6050_Write,0x6A,I2C_MEMADD_SIZE_8BIT,MPU6050_Init1,3,100);
	HAL_I2C_Mem_Write(&hi2c1,ADDR_MPU6050_Write,0x37,I2C_MEMADD_SIZE_8BIT,MPU6050_Init2,2,100);
	HAL_I2C_Mem_Write(&hi2c1,ADDR_MPU6050_Write,0x23,I2C_MEMADD_SIZE_8BIT,MPU6050_Init3,1,100);	
	HAL_I2C_Mem_Write(&hi2c1,ADDR_MPU6050_Write,0x19,I2C_MEMADD_SIZE_8BIT,MPU6050_Init4,4,100);
	
}

uint8 Read_MPU6050()
{
	HAL_I2C_Mem_Read_DMA(&hi2c1,ADDR_MPU6050_Read,0x3B,I2C_MEMADD_SIZE_8BIT,MPU6050_Data,14);
	
	sensor.mpu6050.acc.x.H_L[1]=MPU6050_Data[0];
	sensor.mpu6050.acc.x.H_L[0]=MPU6050_Data[1];
	sensor.mpu6050.acc.y.H_L[1]=MPU6050_Data[2];
	sensor.mpu6050.acc.y.H_L[0]=MPU6050_Data[3];
	sensor.mpu6050.acc.z.H_L[1]=MPU6050_Data[4];
	sensor.mpu6050.acc.z.H_L[0]=MPU6050_Data[5];
	
	sensor.mpu6050.thermometer.Temp.H_L[1]=MPU6050_Data[6];
	sensor.mpu6050.thermometer.Temp.H_L[0]=MPU6050_Data[7];
	
	sensor.mpu6050.gyro.x.H_L[1]=MPU6050_Data[8];
	sensor.mpu6050.gyro.x.H_L[0]=MPU6050_Data[9];
	sensor.mpu6050.gyro.y.H_L[1]=MPU6050_Data[10];
	sensor.mpu6050.gyro.y.H_L[0]=MPU6050_Data[11];
	sensor.mpu6050.gyro.z.H_L[1]=MPU6050_Data[12];
	sensor.mpu6050.gyro.z.H_L[0]=MPU6050_Data[13];
	
}
