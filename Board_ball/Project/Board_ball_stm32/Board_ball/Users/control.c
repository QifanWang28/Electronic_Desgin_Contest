#include "control.h"
#include "tim.h"
#include "bsp_pid.h"
#include "openmv.h"
#include "protocol.h"
//#include "bsp_debug.h"

unsigned char location_control_count = 0;
float speed_Outval, location_Outval;
float speed2_Outval, location2_Outval;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // 好像处理sysTick，其他定时器产生中断要调用回调函数时，都要进到这个函数里进行判断后在执行相应操作
{
	if(htim == &htim7)
	{	
		trans_loc();
		Location_Speed_control();
//		speed_Outval = speed_pid_control();
//		speed2_Outval = speed2_pid_control();
		
		SET_COMPAER(speed_Outval);
		SET2_COMPAER(speed2_Outval);
		
		
		
//		printf("h\n");
//		printf("%f, %f\n",loc_x,loc_y);
//		if(count <= 25)	count++;
//		else{count = 0;printf("err = %f,actual_val = %f, min = %f, inte = %f, diff = %f\n",
//		pid_location.err,pid_location.actual_val,pid_location.Kp * pid_location.err,pid_location.Ki*pid_location.integral,pid_location.Kd * pid_location.diff);}
//		if(count <= 5)	count++;
//		else{count = 0;printf("err = %f, actual_val = %f, min = %f, inte = %f, diff = %f\n",
//		pid_location2.err,pid_location2.actual_val,pid_location2.err,pid_location2.integral,pid_location2.diff);}
		
//		if(count <= 5)	count++;
//		else{count = 0;printf("fps = %d count = %d\n",fps,count_fps);}

//		printf("%f\n",loc_y);
			
//		if(count <= 5)	count++;
//		else{count = 0;printf("loc_x = %f, loc_y = %f\n",loc_x,loc_y);}
	}
}

/*********************各PID**********************/
/****速度环位置环串级PID控制*****/
uint8_t count2 = 0;
void Location_Speed_control(void)          //原地转向可以直接 调用这个
{
			//mpu_dmp_get_data放在这会影响到编码器脉冲的获取。
//		location_control_count++;
//		if(location_control_count >= 2)
//		{
//			location_control_count = 0; 
		location_Outval = location_pid_control();
		location2_Outval = location2_pid_control();
//		}
//		if(count2 < 5) count2++;
//		else{printf("%f %f %f\n", pid_location.err, pid_location.actual_val, pid_location.diff);count2 = 0;}
		
		set_pid_target(&pid_speed, location_Outval); //每次都必须有位置环的值
		set_pid_target(&pid_speed2, location2_Outval); //每次都必须有位置环的值
	 
		speed_Outval = speed_pid_control();    //要是电机转向不符合预期，就在这两句里取反数值
		speed2_Outval = speed2_pid_control();  	
}

float location_pid_control(void)
{
	float cont_val = 0.0;
	float actual_location;

	actual_location = loc_x; // 1圈 = 2550个脉冲  //这里位置用圈数代替。

	cont_val = location_pid_realize(&pid_location, actual_location);
	
	// 还没加串级pID之前，位置环的cont_val对应PWM。 改成串级PID后，位置换的cont_val对应目标速度

	/* 目标速度上限处理 */
	if (cont_val > SPEED_MAX)
	{
		cont_val = SPEED_MAX;
	}
	else if (cont_val < -SPEED_MAX)
	{
		cont_val = -SPEED_MAX;
	}

	int trans = (int)actual_location;
//  set_computer_value(SEND_FACT_CMD, CURVES_CH1, &trans, 1);                // 给通道 1 发送实际值
	
	return cont_val;
}

float location2_pid_control(void)
{
	float cont_val = 0.0;
	float actual_location;

	actual_location = loc_y; // 1圈 = 2550个脉冲  //这里位置用圈数代替。

	cont_val = location_pid_realize(&pid_location2, actual_location);
	
	// 还没加串级pID之前，位置环的cont_val对应PWM。 改成串级PID后，位置换的cont_val对应目标速度

	/* 目标速度上限处理 */
	if (cont_val > SPEED_MAX)
	{
		cont_val = SPEED_MAX;
	}
	else if (cont_val < -SPEED_MAX)
	{
		cont_val = -SPEED_MAX;
	}
	
	int trans = (int)actual_location;
	set_computer_value(SEND_FACT_CMD, CURVES_CH1, &trans, 1);                // 给通道 1 发送实际值
	
	return cont_val;
}

float speed_pid_control(void)  
{
   
    float cont_val = 0.0;                       // 当前控制值
    float actual_speed;
	
	  actual_speed = speed_x;
	
    cont_val = speed_pid_realize(&pid_speed, actual_speed) + MID_PWM_1;    // 进行 PID 计算
   
		if (cont_val > TARGET_PWM_MAX_1)
		{	
			cont_val = TARGET_PWM_MAX_1;
		}
		else if (cont_val < TARGET_PWM_LOW_1)
		{
			cont_val = TARGET_PWM_LOW_1;
		}
		
	int trans = (int)actual_speed;
	
//	set_computer_value(SEND_FACT_CMD, CURVES_CH1, &trans, 1);  // 给通道 1 发送实际值
	
	return cont_val;
}

float speed2_pid_control(void)  
{
   
    float cont_val = 0.0;                       // 当前控制值
    float actual_speed;
	
	  actual_speed = speed_y;
	
    cont_val = speed_pid_realize(&pid_speed2, actual_speed) + MID_PWM_2;    // 进行 PID 计算
		
		if (cont_val > TARGET_PWM_MAX_2)
		{	
		cont_val = TARGET_PWM_MAX_2;
		}
		else if (cont_val < TARGET_PWM_LOW_2)
		{
			cont_val = TARGET_PWM_LOW_2;
		}
		
		int trans = (int)actual_speed;
//		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &trans, 1);                // 给通道 1 发送实际值
	
	return cont_val;
}