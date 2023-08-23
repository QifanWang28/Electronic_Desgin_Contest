#include "control.h"
#include "bsp_i2c.h"
#include "tim.h"

float angle_z = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // 好像处理sysTick，其他定时器产生中断要调用回调函数时，都要进到这个函数里进行判断后在执行相应操作
{
	if(htim == &htim7)
	{	
		MPU6050_Get();
		//printf("%f \n",(float)((float)gz/ (0xffff/500)));
		angle_z += (float)((float)gz/ (0xffff/500)) * 0.02;
	}
}
