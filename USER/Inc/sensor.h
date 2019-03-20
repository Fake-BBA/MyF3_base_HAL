#ifndef _SENSOR_H__
#define _SENSOR_H__

#include "MyTypedef.h"
#include "stm32f3xx_hal.h"

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

#define SENSORS_NBR_OF_BIAS_SAMPLES		1024	/* ���㷽��Ĳ����������� */
#define GYRO_VARIANCE_BASE				4000	/* ��������ƫ������ֵ */
#define SENSORS_ACC_SCALE_SAMPLES  		200		/* ���ټƲ������� */

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

//ƫ��ֵ����
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