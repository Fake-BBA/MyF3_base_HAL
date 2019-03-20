#include "MyConfiguration.h"

#ifndef _DRI_MPU6050_H__
#define _DRI_MPU6050_H__

#include "Har_I2C.h"

#define ADDR_MPU6050_Write 0xD0	//MPU6050–¥µÿ÷∑
#define ADDR_MPU6050_Read 0xD1	//MPU6050∂¡µÿ÷∑

#define MPU6500_DEG_PER_LSB_250  (float)((2 * 250.0) / 65536.0)
#define MPU6500_DEG_PER_LSB_500  (float)((2 * 500.0) / 65536.0)
#define MPU6500_DEG_PER_LSB_1000 (float)((2 * 1000.0) / 65536.0)
#define MPU6500_DEG_PER_LSB_2000 (float)((2 * 2000.0) / 65536.0)
#define SENSORS_DEG_PER_LSB_CFG   MPU6500_DEG_PER_LSB_2000	

#define MPU6500_G_PER_LSB_2      (float)((2 * 2) / 65536.0)
#define MPU6500_G_PER_LSB_4      (float)((2 * 4) / 65536.0)
#define MPU6500_G_PER_LSB_8      (float)((2 * 8) / 65536.0)
#define MPU6500_G_PER_LSB_16     (float)((2 * 16) / 65536.0)
#define SENSORS_G_PER_LSB_CFG     MPU6500_G_PER_LSB_16

#endif