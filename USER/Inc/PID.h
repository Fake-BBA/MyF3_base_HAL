#ifndef _PID_H__
#define _PID_H__

typedef struct{
	float P_Coefficient;
	float I_Coefficient;
	float D_Coefficient;
	float result;	//����һ��PID�Ľ��
	float outPut;	//�ܵ�PID����ۼ����
}PID_Controller;
void PID_Thread();

#endif