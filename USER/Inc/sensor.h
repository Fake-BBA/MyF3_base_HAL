#ifndef _SENSOR_H__
#define _SENSOR_H__

#include "MyTypedef.h"

union Union_DByte{	//double byte��˫�ֽ�������
	uint8 H_L[2];
	int16 data;
};
//���ٶȼ�
struct Accelerometer{
	//������ԭʼ����
	union Union_DByte x;
    union Union_DByte y;
    union Union_DByte z;
	
	//��ԭʼ����ת��Ϊ���ʵ�λ�Ľ��
	float x_g;	//gravity unit
	float y_g;	//gravity unit
	float z_g;	//gravity unit
};
//������
struct Gyroscope{
    union Union_DByte x;
    union Union_DByte y;
    union Union_DByte z;
	
	//��ԭʼ����ת��Ϊ���ʵ�λ�Ľ��
	float x_degree;	//�Ƕ�
	float y_degree;	//�Ƕ�
	float z_degree;	//�Ƕ�
};

//�¶ȼ�
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