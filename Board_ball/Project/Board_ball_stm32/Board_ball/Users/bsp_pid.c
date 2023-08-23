#include "bsp_pid.h"

//����ȫ�ֱ���   
_pid pid_speed, pid_speed2;
_pid pid_location, pid_location2;

//#define  PID_ASSISTANT_EN
/**
  * @brief  PID������ʼ��
	*	@note 	��
  * @retval ��
  */
void PID_param_init()
{
		/* λ����س�ʼ������ */
    pid_location.target_val=100;				
    pid_location.actual_val=0.0;
    pid_location.err=0.0;
    pid_location.err_last=0.0;
    pid_location.integral=0.0;
		pid_location.diff = 0;
  
		pid_location.Kp = 1.5;//0.8
		pid_location.Ki = 0;
		pid_location.Kd = 5;
		
		// x����ٶ�ֵ
		pid_speed.target_val=0;				
    pid_speed.actual_val=0.0;
    pid_speed.err=0.0;
    pid_speed.err_last=0.0;
    pid_speed.integral=0.0;
  
		pid_speed.Kp = 5;
		pid_speed.Ki = 0;
		pid_speed.Kd = 3;
		
		/* λ����س�ʼ������ */
    pid_location2.target_val=104;				
    pid_location2.actual_val=0.0;
    pid_location2.err=0.0;
    pid_location2.err_last=0.0;
    pid_location2.integral=0.0;
  
		pid_location2.Kp = 3.5;//0.8
		pid_location2.Ki = 0;
		pid_location2.Kd = 150;
		
		// y����ٶ�ֵ
    pid_speed2.target_val=0;				
    pid_speed2.actual_val=0.0;
    pid_speed2.err=0.0;
    pid_speed2.err_last=0.0;
    pid_speed2.integral=0.0;
  
		pid_speed2.Kp = 5;
		pid_speed2.Ki = 0;
		pid_speed2.Kd = 3;
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
		pid->Kd = d;    // ����΢��ϵ�� D;
}

/**
  * @brief  λ��PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
	*	@note 	��
  * @retval ͨ��PID���������
  */

uint8_t diff_count = 0;
uint8_t re_count = 0;
uint8_t stable_flag = 0;


float location_pid_realize(_pid *pid, float actual_val)  //λ�û����Kp����Ҳ����
{
		/*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid->err=pid->target_val-actual_val;
		
		if((pid->err <= 0.5f ) && (pid->err >= -0.5f))
		{
      pid->err = 0.f;
		}
		
		pid->diff = pid->err - pid->err_last;
		
		pid->integral += pid->err;
		
		pid->actual_val = pid->Kp	*	pid->err + pid->Ki * pid->integral + pid->Kd * pid->diff;
		
		pid->err_last = pid->err;

		/*PID�㷨ʵ��*/
		/*���ص�ǰʵ��ֵ*/
    return pid->actual_val;
}


float speed_pid_realize(_pid *pid, float actual_val)
{
		/*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid->err=pid->target_val-actual_val;

    if((pid->err < 0.5f ) && (pid->err > -0.5f))  //��1��ô�����������1���ӣ�λ�ò�Ϊ1�����ӵ��ܳ� 
		{
      pid->err = 0.0f;
		}
		
		pid->diff = pid->err - pid->err_last;
		
    pid->integral += pid->err;    // ����ۻ�
	
	  /*�����޷�*/
	  if (pid->integral >= 1000) {pid->integral =1000;}
    else if (pid->integral < -1000)  {pid->integral = -1000;}

		/*PID�㷨ʵ��*/
    pid->actual_val = pid->Kp*pid->err
		                  +pid->Ki*pid->integral
		                   +pid->Kd*pid->diff;
		
		
		
		/*����*/
    pid->err_last=pid->err;
    
		/*���ص�ǰʵ��ֵ*/
    return pid->actual_val;
}
