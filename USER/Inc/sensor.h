#ifndef _SENSOR_H__
#define _SENSOR_H__

#include "MyTypedef.h"

struct Accelerometer{
    union X
    {
        uint8 H_L[2];
        uint16 data;
    }x;
    union Y
    {
        uint8 H_L[2];
        uint16 data;
    }y;
    union Z
    {
        uint8 H_L[2];
        uint16 data;
    }z;
};
struct Gyroscope{
    union X
    {
        uint8 H_L[2];
        uint16 data;
    }x;
    union Y
    {
        uint8 H_L[2];
        uint16 data;
    }y;
    union Z
    {
        uint8 H_L[2];
        uint16 data;
    }z;
};

struct Sensor
{
    struct Accelerometer acc;
    struct Gyroscope gyro; 
    
};

#endif