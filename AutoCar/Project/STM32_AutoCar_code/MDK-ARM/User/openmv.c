#include "bsp_sys.h"
#include "bsp_led.h"
#include "openmv.h"
#include <string.h>

uint8_t i, one_use_L = 0;
uint8_t target_1_L, target_2_L;
uint8_t Counter_L = 0;//计数
uint8_t count_num_L[9] = {0};

uint8_t one_use_R, target_1_R, target_2_R;
uint8_t Counter_R;//计数
uint8_t count_num_R[9];

int count3 = 0;
void K210_Receive_Data_L(int16_t com_data)
{
	if(Counter_L < 30){
		Counter_L++;
		count_num_L[com_data-1]++;
	}
	else if(Counter_L == 30)
	{
		for(i = 0;i < 8;i++)
		{
			if(count_num_L[i] > 7 && one_use_L == 0)
			{
				target_1_L = i + 1;
				one_use_L = 1;
			}
			else if(count_num_L[i]>7) 
			{
				target_2_L = i + 1;
			}
		}
	}
}

void K210_Receive_Data_R(int16_t com_data)
{
		target_1_R = com_data;
		//printf("%d\n",com_data);
}

void clean_K210(void)
{
		one_use_L = 0;
		target_1_L = 0;
		target_2_L = 0;
		Counter_L = 0;
		memset(count_num_R, 0, sizeof(count_num_R));
		one_use_R = 0;
		target_1_R = 0;
		target_2_R = 0;
		Counter_R = 0;
		memset(count_num_L, 0, sizeof(count_num_L));
}