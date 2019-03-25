#ifndef _COMMUNICATION_H__
#define _COMMUNICATION_H__
#include "MyTypedef.h"

enum FlightFuntionWord
{
	VER = 0,
	STATUS=1,
	SENSER=2,
	RCDATA=3,
	GPSDATA=4,
	POWER=5,
	MOTO=6,
	SENSER2=7,
	LOOP_TIME=200,
};

struct RcData{
	int16 Throttle;
	int16 Yaw;
	int16 Roll;
	int16 Pitch; 
	int16 AUX1; 
	int16 AUX2; 
	int16 AUX3;
	int16 AUX4; 
	int16 AUX5; 
	int16 AUX6;
};

extern struct RcData rcData;

void SendStatus();
void SendSensor();
void SendLoopTime();

void RecvMessageThread();

#endif