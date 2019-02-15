#include "Dri_MPU6050.h"

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
	uint16 tempture;
	ACC_X = (((uint16_t)MPU6050_Data[0]) << 8) + MPU6050_Data[1];
	ACC_Y = (((uint16_t)MPU6050_Data[2]) << 8) + MPU6050_Data[3];
	ACC_Z = (((uint16_t)MPU6050_Data[4]) << 8) + MPU6050_Data[5];
	tempture=(((uint16_t)MPU6050_Data[6]) << 8) + MPU6050_Data[7];
	MPU6050_TEMP=36.53+((float)((int16)tempture)/340);
	GYR_X = (((uint16_t)MPU6050_Data[8]) << 8) + MPU6050_Data[9];
	GYR_Y = (((uint16_t)MPU6050_Data[10]) << 8) + MPU6050_Data[11];
	GYR_Z = (((uint16_t)MPU6050_Data[12]) << 8) + MPU6050_Data[13];
}