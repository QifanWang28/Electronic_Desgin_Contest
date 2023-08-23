#ifndef __PID__
#define __PID__

typedef struct
{
    float target_val;           //Ŀ��ֵ
    float actual_val;        		//ʵ��ֵ
    float err;             			//����ƫ��ֵ
    float err_last;          		//������һ��ƫ��ֵ
    float Kp,Ki,Kd;          		//������������֡�΢��ϵ��
    float integral;          		//�������ֵ
}_pid;

extern _pid pid_location_up, pid_location_down;

float location_pid_realize(_pid *pid, float actual_val);
void PID_param_init();
#endif