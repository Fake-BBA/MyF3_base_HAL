#ifndef _SENSOR_H__
#define _SENSOR_H__

#include "MyTypedef.h"
#include "stm32f3xx_hal.h"

union Union_DByte{	//double byte，双字节联合体
	uint8 H_L[2];
	int16 data;
};
//加速度计
struct Accelerometer{
	//传感器原始数据
	union Union_DByte x;
    union Union_DByte y;
    union Union_DByte z;
	
	//将原始数据转化为国际单位的结果
	float x_g;	//gravity unit
	float y_g;	//gravity unit
	float z_g;	//gravity unit
};
//陀螺仪
struct Gyroscope{
    union Union_DByte x;
    union Union_DByte y;
    union Union_DByte z;
	
	//将原始数据转化为国际单位的结果
	float x_degree;	//角度
	float y_degree;	//角度
	float z_degree;	//角度
};

//温度计
struct Thermometer{
    union Union_DByte Temp;
};
struct MPU6050{
    struct Accelerometer acc;
    struct Gyroscope gyro; 
    struct Thermometer thermometer;
};
struct Sensor
{
    struct MPU6050 mpu6050;
    
};

extern struct Sensor sensor;

#define SENSORS_NBR_OF_BIAS_SAMPLES		1024	/* 计算方差的采样样本个数 */
#define GYRO_VARIANCE_BASE				4000	/* 陀螺仪零偏方差阈值 */
#define SENSORS_ACC_SCALE_SAMPLES  		200		/* 加速计采样个数 */

typedef union 
{
	struct 
	{
		float x;
		float y;
		float z;
	};
	float axis[3];
} Axis3f;

typedef union 
{
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
	};
	int16_t axis[3];
} Axis3i16;

//偏置值对象
typedef struct
{
	Axis3f     bias;
	bool       isBiasValueFound;
	bool       isBufferFilled;
	Axis3i16*  bufHead;
	Axis3i16   buffer[SENSORS_NBR_OF_BIAS_SAMPLES];
}BiasObj;	

uint8 SensorInit();
#endif