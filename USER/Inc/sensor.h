#ifndef _SENSOR_H__
#define _SENSOR_H__

#include "MyTypedef.h"

union Union_DByte{	//double byte，双字节联合体
	uint8 H_L[2];
	uint16 data;
};
struct Accelerometer{
	union Union_DByte x;
    union Union_DByte y;
    union Union_DByte z;
};
struct Gyroscope{
    union Union_DByte x;
    union Union_DByte y;
    union Union_DByte z;
};

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