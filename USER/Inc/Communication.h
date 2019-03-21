#ifndef _COMMUNICATION_H__
#define _COMMUNICATION_H__


void SendStatus(float pitch,float roll,float yaw);
void SendSensor();
void RecvMessageThread();

#endif