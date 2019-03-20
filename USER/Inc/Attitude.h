#ifndef _ATTITUDE_H__
#define _ATTITUDE_H__

#include "MyConfiguration.h"

struct AccData{
	float x;
	float y;
	float z;
};

struct GyroData{
	float x;
	float y;
	float z;
};

struct Attitude{
	float pitch;
	float roll;
	float yaw;
};

struct FlightState{
	struct AccData acc;
	struct Attitude attitude;
};

void imuUpdate(struct AccData acc,struct GyroData gyro,struct FlightState *state, float dt);	/*�����ں� �����˲�*/

#define DEG2RAD		0.017453293f	/* ��ת���� ��/180 */
#define RAD2DEG		57.29578f		/* ����ת�� 180/�� */
#endif