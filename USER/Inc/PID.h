#ifndef _PID_H__
#define _PID_H__

typedef struct{
	float P_Coefficient;
	float I_Coefficient;
	float D_Coefficient;
	float result;	//最新一次PID的结果
	float outPut;	//总的PID结果累加输出
}PID_Controller;
void PID_Thread();

#endif