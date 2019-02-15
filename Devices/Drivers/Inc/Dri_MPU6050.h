#include "MyConfiguration.h"

#ifndef _DRI_MPU6050_H__
#define _DRI_MPU6050_H__

#include "Har_I2C.h"

#define ADDR_MPU6050_Write 0xD0
#define ADDR_MPU6050_Read 0xD1


extern int16_t ACC_X;
extern int16_t ACC_Y;
extern int16_t ACC_Z;
extern int16_t GYR_X;
extern int16_t GYR_Y;
extern int16_t GYR_Z;
extern float MPU6050_TEMP;

#endif