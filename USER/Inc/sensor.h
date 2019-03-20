#ifndef _SENSOR_H__
#define _SENSOR_H__

#include "MyTypedef.h"

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
#endif