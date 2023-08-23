#include "control.h"

int PWM_Up = 1500;
int PWM_Down = 1200;

u8 light = 1;		//激光笔
u8 count_sys = 0;	// 系统打拍器件
u8 count_div = 0;	// 细分计时器

u8 count_start = 0;	// 启动计时器
u8 run_flag = 0;	// 是否启动

float y_err = 0.f;
float x_err = 0.f;

float angle_y =-10;
float angle_x =-15;

int count_light = 0;

u8 stop = 1;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // 好像处理sysTick，其他定时器产生中断要调用回调函数时，都要进到这个函数里进行判断后在执行相应操作
{
	if(htim == &htim7)
	{	
		if(stop == 0){
			// 任务三， 任务四控制
			angle_y += location_pid_control_up();
			angle_x += location_pid_control_down();
			
			PWM_Up = trans_angle_up(angle_y);
			PWM_Down = trans_angle_down(angle_x);
			
			SET_UP_PWM(PWM_Up);
			SET_DOWN_PWM(PWM_Down);
			//printf("%f %f",x_err, y_err);
		
		}
			if(x_err < 15 && x_err >-15 && y_err < 15 && y_err > -15 && x_err != 0 && y_err != 0)
			{
				HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
				if(count_light < 4) count_light++;
				else
				{
					count_light = 0;
					HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);
				}
			}
			else
			{
				HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_3);
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);
			}
//		
//		
//		// 控制激光亮暗
////		if(light == 0)
////		{
////			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
////		}
////		else
////		{
//			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
////		}
//		
//		// 系统启动和计时器
//		if(count_sys < 25) count_sys++;
//		if(count_start < 25) count_start++;
//		printf("%d %d %f %f\n", PWM_Up, PWM_Down, angle_y, angle_x);
	}
}


float location_pid_control_up(void)
{
	float cont_val = 0.f;
	cont_val = location_pid_realize(&pid_location_up, y_err);
	return cont_val;
}


float location_pid_control_down(void)
{
	float cont_val = 0.f;
	cont_val = location_pid_realize(&pid_location_down, x_err);
//	printf("%f\n", x_err);
	return cont_val;
}

int trans_angle_up(float angle)
{
	if(angle > 80) angle = 80;
	else if(angle < -80) angle = -80;
	
	float real_PWM = 0;
	real_PWM = (-angle * 2000)/ALL_ANGLE + 1500;
	return (int)real_PWM;
}

int trans_angle_down(float angle)
{
	if(angle > 80) angle = 80;
	else if(angle < -80) angle = -80;
	
	float real_PWM = 0;
	real_PWM = (-angle * 2000)/ALL_ANGLE + 1500;
	return (int)real_PWM;
}

