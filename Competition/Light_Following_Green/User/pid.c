#include "pid.h"

//����ȫ�ֱ���

_pid pid_speed, pid_speed2;    
_pid pid_location_up, pid_location_down;

_pid line_following;
//#define  PID_ASSISTANT_EN
/**
  * @brief  PID������ʼ��
	*	@note 	��
  * @retval ��
  */
void PID_param_init()
{
	/* λ����س�ʼ������ */
    pid_location_up.target_val=0;				
    pid_location_up.actual_val=0.0;
    pid_location_up.err=0.0;
    pid_location_up.err_last=0.0;
    pid_location_up.integral=0.0;
  
		pid_location_up.Kp = 0.008;//0.8
		pid_location_up.Ki = 0.0;
		pid_location_up.Kd = 0.015;
  
  	/* �ٶ���س�ʼ������ */
    pid_speed.target_val=60;				
    pid_speed.actual_val=0;
    pid_speed.err=0.0;
    pid_speed.err_last=0.0;
    pid_speed.integral=0.0;
  
		pid_speed.Kp = 2;
		pid_speed.Ki = 0.5;
		pid_speed.Kd = 0;
		
			/* λ����س�ʼ������ */
    pid_location_down.target_val=0;				
    pid_location_down.actual_val=0.0;
    pid_location_down.err=0.0;
    pid_location_down.err_last=0.0;
    pid_location_down.integral=0.0;
  
		pid_location_down.Kp = 0.008;
		pid_location_down.Ki = 0.0;
		pid_location_down.Kd = 0.015;
  
  	/* �ٶ���س�ʼ������ */
    pid_speed2.target_val=60;				
    pid_speed2.actual_val=0.0;
    pid_speed2.err=0.0;
    pid_speed2.err_last=0.0;
    pid_speed2.integral=0.0;
  
		pid_speed2.Kp = 2;
		pid_speed2.Ki = 0.5;
		pid_speed2.Kd = 0;
		
		line_following.target_val=0;				
    line_following.actual_val=0.0;
    line_following.err=0.0;
    line_following.err_last=0.0;
    line_following.integral=0.0;
  
		line_following.Kp = 15;
		line_following.Ki = 0;
		line_following.Kd = 0;
		
//#if defined(PID_ASSISTANT_EN)
//    float pid_temp[3] = {pid.Kp, pid.Ki, pid.Kd};
//    set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid_temp, 3);     // ��ͨ�� 1 ���� P I D ֵ
//#endif
}

/**
  * @brief  ����Ŀ��ֵ
  * @param  val		Ŀ��ֵ
	*	@note 	��
  * @retval ��
  */
void set_pid_target(_pid *pid, float temp_val)
{
  pid->target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
}

/**
  * @brief  ��ȡĿ��ֵ
  * @param  ��
	*	@note 	��
  * @retval Ŀ��ֵ
  */
float get_pid_target(_pid *pid)
{
  return pid->target_val;    // ���õ�ǰ��Ŀ��ֵ
}

void set_p_i_d(_pid *pid, float p, float i, float d)
{
  	pid->Kp = p;    // ���ñ���ϵ�� P
		pid->Ki = i;    // ���û���ϵ�� I
		pid->Kd = d;    // ����΢��ϵ�� D
}

/**
  * @brief  λ��PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
	*	@note 	��
  * @retval ͨ��PID���������
  */
float location_pid_realize(_pid *pid, float actual_val)  //λ�û����Kp����Ҳ����
{
		/*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid->err=-actual_val;
    
		if(pid->err < 8.f && pid->err > -8.f)
		{
			pid->err = 0;
		}
    pid->integral += pid->err;    // ����ۻ�
		
		if (pid->integral >= 1000) {pid->integral =1000;}
    else if (pid->integral < -1000)  {pid->integral = -1000;}
		
		/*PID�㷨ʵ��*/
    pid->actual_val = pid->Kp*pid->err
		                  +pid->Ki*pid->integral
		                  +pid->Kd*(pid->err-pid->err_last);
  
		/*����*/
    pid->err_last=pid->err;
    
		/*���ص�ǰʵ��ֵ*/
    return pid->actual_val;
}

/**
  * @brief  �ٶ�PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
	*	@note 	��
  * @retval ͨ��PID���������
  */
float speed_pid_realize(_pid *pid, float actual_val)
{
		/*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid->err=pid->target_val-actual_val;

    if((pid->err<0.5f ) && (pid->err>-0.5f))  //��1��ô�����������1���ӣ�λ�ò�Ϊ1�����ӵ��ܳ� 
		{
      pid->err = 0.0f;
		}
	
    pid->integral += pid->err;    // ����ۻ�

	  /*�����޷�*/
	   if (pid->integral >= 1000) {pid->integral =1000;}
     else if (pid->integral < -1000)  {pid->integral = -1000;}

		/*PID�㷨ʵ��*/
    pid->actual_val = pid->Kp*pid->err
		                  +pid->Ki*pid->integral
		                   +pid->Kd*(pid->err-pid->err_last);
  
		/*����*/
    pid->err_last=pid->err;
    
		/*���ص�ǰʵ��ֵ*/
    return pid->actual_val;
}


