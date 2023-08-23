#include "control.h"

int PWM_Up = 1500;
int PWM_Down = 1500;

u8 light = 1;		//激光笔
u8 count_sys = 0;	// 系统打拍器件
u8 count_div = 0;	// 细分计时器

u8 count_start = 0;	// 启动计时器
u8 run_flag = 0;	// 是否启动

float y_err = 0.f;
float x_err = 0.f;

float angle_y =0;
float angle_x =0;

float angle_y_4 =0;
float angle_x_4 =0;

u8 count_task_3 = 1;

u8 task_reg_openmv = 0;

uint8_t fresh_openmv = 0;

u8 task_reg_open = 0;

u8 up = 0;
u8 down = 0;
u8 left = 0;
u8 right = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // 好像处理sysTick，其他定时器产生中断要调用回调函数时，都要进到这个函数里进行判断后在执行相应操作
{
	if(htim == &htim7)
	{	
		// 任务三， 任务四控制
		if(light_red_task != 16)
		{
			if(light_red_task == 3)
			{
				count_task_3 = - count_task_3;
				if(count_task_3 == 1)
				{
					angle_y += location_pid_control_up();
					angle_x += location_pid_control_down();
					PWM_Up = trans_angle_up(angle_y);
					PWM_Down = trans_angle_down(angle_x);
				}
			}
			else
			{
				angle_y = 0;
				angle_x = 0;
			}
			if(light_red_task == 4)
			{
				angle_y_4 += up;
				angle_y_4 -= down;
				angle_x_4 += right;
				angle_x_4 -= left;
				up = 0;
				down = 0;
				left = 0;
				right = 0;	
				PWM_Up = trans_angle_up(angle_y_4);
				PWM_Down = trans_angle_down(angle_x_4);
			}
			else
			{
				angle_y_4 = 0;
				angle_x_4 = 0;
			}
			
			if((count_start > 20) || (run_flag == 1))
			{
				SET_UP_PWM(PWM_Up);
				SET_DOWN_PWM(PWM_Down);
				run_flag = 1;
			}
			
			// 控制激光亮暗
			if(light == 0)
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
			}
			
			if(task_reg_open != light_red_task)
			{
				task_reg_open = light_red_task;
				fresh_openmv = 1;
				HAL_UART_Transmit(&huart2, (uint8_t *)&fresh_openmv, 1, 1);
			}
			else
			{
				fresh_openmv = 0;
				HAL_UART_Transmit(&huart2, (uint8_t *)&fresh_openmv, 1, 1);
			}
			
			// 系统启动和计时器
			if(count_sys < TASK2_DIVIDE_cnt) count_sys++;
			if(count_start < 25) count_start++;
			
		}
	}
}

u8 sub_task = 0;
u8 divide_task = 0;

float div_x = 0;
float div_y = 0;

void light_red_task2(void)
{
		if(count_sys == TASK2_DIVIDE_cnt && sub_task == 0)
		{
				PWM_Up = trans_angle_up(LEFT_DOWN_Y);
				PWM_Down = trans_angle_down(LEFT_DOWN_X);
				count_sys = 0;
				sub_task++;
		}
		else if(count_sys == TASK2_DIVIDE_cnt && sub_task == 1)
		{
			div_x = (LEFT_UP_X - LEFT_DOWN_X)/TASK2_DIVIDE;
			div_y = (LEFT_UP_Y - LEFT_DOWN_Y)/TASK2_DIVIDE;
			
			if(divide_task < TASK2_DIVIDE)
			{
				PWM_Up = trans_angle_up(LEFT_DOWN_Y + div_y * divide_task);
				PWM_Down = trans_angle_down(LEFT_DOWN_X + div_x * divide_task);
				count_sys = 0;
				divide_task ++;
			}
			
			if(divide_task == TASK2_DIVIDE)
			{
				divide_task = 0;
				count_sys = 0;
				sub_task++;
			}
		}
		else if(count_sys == TASK2_DIVIDE_cnt && sub_task == 2)
		{
			div_x = (RIGHT_UP_X - LEFT_UP_X)/TASK2_DIVIDE;
			div_y = (RIGHT_UP_Y - LEFT_UP_Y)/TASK2_DIVIDE;
			
			if(divide_task < TASK2_DIVIDE)
			{
				PWM_Up = trans_angle_up(LEFT_UP_Y + div_y * divide_task);
				PWM_Down = trans_angle_down(LEFT_UP_X + div_x * divide_task);
				count_sys = 0;
				divide_task ++;
			}
			
			if(divide_task == TASK2_DIVIDE)
			{
				divide_task = 0;
				count_sys = 0;
				sub_task++;
			}
		}
		else if(count_sys == TASK2_DIVIDE_cnt && sub_task == 3)
		{
			div_x = (RIGHT_DOWN_X - RIGHT_UP_X)/TASK2_DIVIDE;
			div_y = (RIGHT_DOWN_Y - RIGHT_UP_Y)/TASK2_DIVIDE;
			
			if(divide_task < TASK2_DIVIDE)
			{
				PWM_Up = trans_angle_up(RIGHT_UP_Y + div_y * divide_task);
				PWM_Down = trans_angle_down(RIGHT_UP_X + div_x * divide_task);
				count_sys = 0;
				divide_task ++;
			}
			
			if(divide_task == TASK2_DIVIDE)
			{
				divide_task = 0;
				count_sys = 0;
				sub_task++;
			}
		}
		else if(count_sys == TASK2_DIVIDE_cnt && sub_task == 4)
		{
			div_x = (LEFT_DOWN_X - RIGHT_DOWN_X)/TASK2_DIVIDE;
			div_y = (LEFT_DOWN_Y - RIGHT_DOWN_Y)/TASK2_DIVIDE;
			
			if(divide_task < TASK2_DIVIDE)
			{
				PWM_Up = trans_angle_up(RIGHT_DOWN_Y + div_y * divide_task);
				PWM_Down = trans_angle_down(RIGHT_DOWN_X + div_x * divide_task);
				count_sys = 0;
				divide_task ++;
			}
			
			if(divide_task == TASK2_DIVIDE)
			{
				divide_task = 0;
				count_sys = 0;
				sub_task = 0;
			}
		}
}

float location_pid_control_up(void)
{
	float cont_val = 0.f;
	cont_val = location_pid_realize(&pid_location_up, y_err);
//	printf("%f\n", y_err);
	return cont_val;
}


float location_pid_control_down(void)
{
	float cont_val = 0.f;
	cont_val = location_pid_realize(&pid_location_down, x_err);
	return cont_val;
}

int trans_angle_up(float angle)
{
	if(angle > UP_MAX) angle = UP_MAX;
	else if(angle < UP_LOW) angle = UP_LOW;
	
	float real_PWM = 0;
	real_PWM = (angle * 2000)/ALL_ANGLE + 1500;
	return (int)real_PWM;
}

int trans_angle_down(float angle)
{
	if(angle > DOWN_MAX) angle = DOWN_MAX;
	else if(angle < DOWN_LOW) angle = DOWN_LOW;
	
	float real_PWM = 0;
	real_PWM = (-angle * 2000)/ALL_ANGLE + 1500;
	return (int)real_PWM;
}

