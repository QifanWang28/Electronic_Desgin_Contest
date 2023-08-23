#include "control.h"
#include "tim.h"
#include "bsp_pid.h"
#include "openmv.h"
#include "protocol.h"
//#include "bsp_debug.h"

unsigned char location_control_count = 0;
float speed_Outval, location_Outval;
float speed2_Outval, location2_Outval;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // ������sysTick��������ʱ�������ж�Ҫ���ûص�����ʱ����Ҫ�����������������жϺ���ִ����Ӧ����
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

/*********************��PID**********************/
/****�ٶȻ�λ�û�����PID����*****/
uint8_t count2 = 0;
void Location_Speed_control(void)          //ԭ��ת�����ֱ�� �������
{
			//mpu_dmp_get_data�������Ӱ�쵽����������Ļ�ȡ��
//		location_control_count++;
//		if(location_control_count >= 2)
//		{
//			location_control_count = 0; 
		location_Outval = location_pid_control();
		location2_Outval = location2_pid_control();
//		}
//		if(count2 < 5) count2++;
//		else{printf("%f %f %f\n", pid_location.err, pid_location.actual_val, pid_location.diff);count2 = 0;}
		
		set_pid_target(&pid_speed, location_Outval); //ÿ�ζ�������λ�û���ֵ
		set_pid_target(&pid_speed2, location2_Outval); //ÿ�ζ�������λ�û���ֵ
	 
		speed_Outval = speed_pid_control();    //Ҫ�ǵ��ת�򲻷���Ԥ�ڣ�������������ȡ����ֵ
		speed2_Outval = speed2_pid_control();  	
}

float location_pid_control(void)
{
	float cont_val = 0.0;
	float actual_location;

	actual_location = loc_x; // 1Ȧ = 2550������  //����λ����Ȧ�����档

	cont_val = location_pid_realize(&pid_location, actual_location);
	
	// ��û�Ӵ���pID֮ǰ��λ�û���cont_val��ӦPWM�� �ĳɴ���PID��λ�û���cont_val��ӦĿ���ٶ�

	/* Ŀ���ٶ����޴��� */
	if (cont_val > SPEED_MAX)
	{
		cont_val = SPEED_MAX;
	}
	else if (cont_val < -SPEED_MAX)
	{
		cont_val = -SPEED_MAX;
	}

	int trans = (int)actual_location;
//  set_computer_value(SEND_FACT_CMD, CURVES_CH1, &trans, 1);                // ��ͨ�� 1 ����ʵ��ֵ
	
	return cont_val;
}

float location2_pid_control(void)
{
	float cont_val = 0.0;
	float actual_location;

	actual_location = loc_y; // 1Ȧ = 2550������  //����λ����Ȧ�����档

	cont_val = location_pid_realize(&pid_location2, actual_location);
	
	// ��û�Ӵ���pID֮ǰ��λ�û���cont_val��ӦPWM�� �ĳɴ���PID��λ�û���cont_val��ӦĿ���ٶ�

	/* Ŀ���ٶ����޴��� */
	if (cont_val > SPEED_MAX)
	{
		cont_val = SPEED_MAX;
	}
	else if (cont_val < -SPEED_MAX)
	{
		cont_val = -SPEED_MAX;
	}
	
	int trans = (int)actual_location;
	set_computer_value(SEND_FACT_CMD, CURVES_CH1, &trans, 1);                // ��ͨ�� 1 ����ʵ��ֵ
	
	return cont_val;
}

float speed_pid_control(void)  
{
   
    float cont_val = 0.0;                       // ��ǰ����ֵ
    float actual_speed;
	
	  actual_speed = speed_x;
	
    cont_val = speed_pid_realize(&pid_speed, actual_speed) + MID_PWM_1;    // ���� PID ����
   
		if (cont_val > TARGET_PWM_MAX_1)
		{	
			cont_val = TARGET_PWM_MAX_1;
		}
		else if (cont_val < TARGET_PWM_LOW_1)
		{
			cont_val = TARGET_PWM_LOW_1;
		}
		
	int trans = (int)actual_speed;
	
//	set_computer_value(SEND_FACT_CMD, CURVES_CH1, &trans, 1);  // ��ͨ�� 1 ����ʵ��ֵ
	
	return cont_val;
}

float speed2_pid_control(void)  
{
   
    float cont_val = 0.0;                       // ��ǰ����ֵ
    float actual_speed;
	
	  actual_speed = speed_y;
	
    cont_val = speed_pid_realize(&pid_speed2, actual_speed) + MID_PWM_2;    // ���� PID ����
		
		if (cont_val > TARGET_PWM_MAX_2)
		{	
		cont_val = TARGET_PWM_MAX_2;
		}
		else if (cont_val < TARGET_PWM_LOW_2)
		{
			cont_val = TARGET_PWM_LOW_2;
		}
		
		int trans = (int)actual_speed;
//		set_computer_value(SEND_FACT_CMD, CURVES_CH1, &trans, 1);                // ��ͨ�� 1 ����ʵ��ֵ
	
	return cont_val;
}