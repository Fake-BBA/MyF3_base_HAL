#include "Dri_MPU6050.h"
#include "sensor.h"

uint8_t MPU6050_Init1[3] = {0x00,0x01,0x00};
uint8_t MPU6050_Init2[2] = {0x80,0x00};
uint8_t MPU6050_Init3[1] = {0x00};
uint8_t MPU6050_Init4[4] = {0x13,0x04,0x18,0x08};

uint8_t MPU6050_Data[14];																							//例程为了兼顾气压计（AD时间至少20ms），系统频率定为10Hz
uint8_t Read_OK = 0;
int16_t ACC_X;
int16_t ACC_Y;
int16_t ACC_Z;
int16_t GYR_X;
int16_t GYR_Y;
int16_t GYR_Z;
float MPU6050_TEMP;


void Init_MPU6050()
{
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
