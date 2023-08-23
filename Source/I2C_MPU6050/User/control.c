#include "control.h"
#include "bsp_i2c.h"
#include "tim.h"

float angle_z = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) // ������sysTick��������ʱ�������ж�Ҫ���ûص�����ʱ����Ҫ�����������������жϺ���ִ����Ӧ����
{
	if(htim == &htim7)
	{	
		MPU6050_Get();
		//printf("%f \n",(float)((float)gz/ (0xffff/500)));
		angle_z += (float)((float)gz/ (0xffff/500)) * 0.02;
	}
}
