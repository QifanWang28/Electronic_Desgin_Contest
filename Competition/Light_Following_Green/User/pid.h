#ifndef __PID__
#define __PID__

typedef struct
{
    float target_val;           //目标值
    float actual_val;        		//实际值
    float err;             			//定义偏差值
    float err_last;          		//定义上一个偏差值
    float Kp,Ki,Kd;          		//定义比例、积分、微分系数
    float integral;          		//定义积分值
}_pid;

extern _pid pid_location_up, pid_location_down;

float location_pid_realize(_pid *pid, float actual_val);
void PID_param_init();
#endif