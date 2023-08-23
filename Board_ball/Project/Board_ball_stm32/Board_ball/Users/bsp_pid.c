#include "bsp_pid.h"

//定义全局变量   
_pid pid_speed, pid_speed2;
_pid pid_location, pid_location2;

//#define  PID_ASSISTANT_EN
/**
  * @brief  PID参数初始化
	*	@note 	无
  * @retval 无
  */
void PID_param_init()
{
		/* 位置相关初始化参数 */
    pid_location.target_val=100;				
    pid_location.actual_val=0.0;
    pid_location.err=0.0;
    pid_location.err_last=0.0;
    pid_location.integral=0.0;
		pid_location.diff = 0;
  
		pid_location.Kp = 1.5;//0.8
		pid_location.Ki = 0;
		pid_location.Kd = 5;
		
		// x轴的速度值
		pid_speed.target_val=0;				
    pid_speed.actual_val=0.0;
    pid_speed.err=0.0;
    pid_speed.err_last=0.0;
    pid_speed.integral=0.0;
  
		pid_speed.Kp = 5;
		pid_speed.Ki = 0;
		pid_speed.Kd = 3;
		
		/* 位置相关初始化参数 */
    pid_location2.target_val=104;				
    pid_location2.actual_val=0.0;
    pid_location2.err=0.0;
    pid_location2.err_last=0.0;
    pid_location2.integral=0.0;
  
		pid_location2.Kp = 3.5;//0.8
		pid_location2.Ki = 0;
		pid_location2.Kd = 150;
		
		// y轴的速度值
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
  * @brief  设置目标值
  * @param  val		目标值
	*	@note 	无
  * @retval 无
  */
void set_pid_target(_pid *pid, float temp_val)
{
  pid->target_val = temp_val;    // 设置当前的目标值
}

/**
  * @brief  获取目标值
  * @param  无
	*	@note 	无
  * @retval 目标值
  */
float get_pid_target(_pid *pid)
{
  return pid->target_val;    // 设置当前的目标值
}

void set_p_i_d(_pid *pid, float p, float i, float d)
{
  	pid->Kp = p;    // 设置比例系数 P
		pid->Ki = i;    // 设置积分系数 I
		pid->Kd = d;    // 设置微分系数 D;
}

/**
  * @brief  位置PID算法实现
  * @param  actual_val:实际值
	*	@note 	无
  * @retval 通过PID计算后的输出
  */

uint8_t diff_count = 0;
uint8_t re_count = 0;
uint8_t stable_flag = 0;


float location_pid_realize(_pid *pid, float actual_val)  //位置环光个Kp好像也可以
{
		/*计算目标值与实际值的误差*/
    pid->err=pid->target_val-actual_val;
		
		if((pid->err <= 0.5f ) && (pid->err >= -0.5f))
		{
      pid->err = 0.f;
		}
		
		pid->diff = pid->err - pid->err_last;
		
		pid->integral += pid->err;
		
		pid->actual_val = pid->Kp	*	pid->err + pid->Ki * pid->integral + pid->Kd * pid->diff;
		
		pid->err_last = pid->err;

		/*PID算法实现*/
		/*返回当前实际值*/
    return pid->actual_val;
}


float speed_pid_realize(_pid *pid, float actual_val)
{
		/*计算目标值与实际值的误差*/
    pid->err=pid->target_val-actual_val;

    if((pid->err < 0.5f ) && (pid->err > -0.5f))  //差1这么多可以吗？运行1分钟，位置差为1个轮子的周长 
		{
      pid->err = 0.0f;
		}
		
		pid->diff = pid->err - pid->err_last;
		
    pid->integral += pid->err;    // 误差累积
	
	  /*积分限幅*/
	  if (pid->integral >= 1000) {pid->integral =1000;}
    else if (pid->integral < -1000)  {pid->integral = -1000;}

		/*PID算法实现*/
    pid->actual_val = pid->Kp*pid->err
		                  +pid->Ki*pid->integral
		                   +pid->Kd*pid->diff;
		
		
		
		/*误差传递*/
    pid->err_last=pid->err;
    
		/*返回当前实际值*/
    return pid->actual_val;
}
