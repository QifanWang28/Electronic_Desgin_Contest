#include "bsp_sys.h"
#include "bsp_led.h"
#include "openmv.h"
#include <string.h>
//uint16_t L3, L2, L1, Center, R1, R2, R3;
//float error = 0;
float speed_correction = 0;

uint8_t all_color = 0;
uint8_t no_color = 0;

uint8_t i, one_use_L = 0;
uint8_t target_1_L, target_2_L;
uint8_t Counter_L = 0;//计数
uint8_t count_num_L[9] = {0};

uint8_t one_use_R, target_1_R, target_2_R;
uint8_t Counter_R;//计数
uint8_t count_num_R[9];
uint8_t signal = 0;

int8_t correction_openmv_int = 0;
uint8_t correction_openmv_flt = 0;

int count_2 = 0;
void Light_GoStraight_control(void)   //灰度巡线直行, 需要有个判断需要直行多长距离的函数    //补偿要到中间的传感器测到回到线上为止
{
	//如果要更加精准的控制，只要有了巡线环，都需要对位置进行补偿。  可否直接将 g_lMotorPulseSigma  和g_lMotor2PulseSigma 进行数值加减？？
	//如果寻直线用的传感器可以读取模拟量的话，三个传感器都可以巡直线寻挺稳的
//	if(count_2 < 5) count_2++;
//	else{count_2 = 0; printf("L3 %d L2 %d L1 %d Center %d R1 %d R2 %d R3 %d\n", L3,L2,L1,Center,R1,R2,R3);}
	if(L3 == 1 && L2 == 1 && L1 == 1 && R3 == 1 && R2 == 1 && R1 == 1 && Center == 1)
	{
		error = 0;
		all_color = 1;
		no_color = 0;
	}
	else if(L3 == 0 && L2 == 0 && L1 == 0 && R3 == 0 && R2 == 0 && R1 == 0 && Center == 0)
	{
		error = 0;
		all_color = 0;
		no_color = 1;
	}
	else
	{
		if (L3 == 0 && L2 == 0 && L1 == 0 && Center == 1 && R1 == 0 && R2 == 0 && R3 == 0)
				error = 0; // Example value for Area 1

		// Check for Area 2 (left and center areas)
		else if (L3 == 1 && L2 == 0 && L1 == 0 && Center == 0 && R1 == 0 && R2 == 0 && R3 == 0)
				error = -3; // Example value for Area 2

		// Check for Area 3 (center area)
		else if (L3 == 0 && L2 == 1 && L1 == 0 && Center == 0 && R1 == 0 && R2 == 0 && R3 == 0)
				error = -2; // Example value for Area 3

		// Check for Area 4 (center and right areas)
		else if (L3 == 0 && L2 == 0 && L1 == 1 && Center == 0 && R1 == 0 && R2 == 0 && R3 == 0)
				error = -1; // Example value for Area 4

		// Check for Area 5 (rightmost area)
		else if (L3 == 0 && L2 == 0 && L1 == 0 && Center == 0 && R1 == 0 && R2 == 0 && R3 == 1)
				error = 3; // Example value for Area 5

		// Check for Area 6 (right and center areas)
		else if (L3 == 0 && L2 == 0 && L1 == 0 && Center == 0 && R1 == 1 && R2 == 0 && R3 == 0)
				error = 1; // Example value for Area 6

		// Check for Area 7 (right and center areas)
		else if (L3 == 0 && L2 == 0 && L1 == 0 && Center == 0 && R1 == 0 && R2 == 1 && R3 == 0)
				error = 2; // Example value for Area 7

		// Check for Area 8 (center and left areas)
		else if (L3 == 1 && L2 == 1 && L1 == 0 && Center == 0 && R1 == 0 && R2 == 0 && R3 == 0)
				error = -2.5; // Example value for Area 8

		// Check for Area 9 (center area)
		else if (L3 == 0 && L2 == 1 && L1 == 1 && Center == 0 && R1 == 0 && R2 == 0 && R3 == 0)
				error = -1.5; // Example value for Area 9

		// Check for Area 10 (leftmost area)
		else if (L3 == 0 && L2 == 0 && L1 == 1 && Center == 1 && R1 == 0 && R2 == 0 && R3 == 0)
				error = -0.5; // Example value for Area 10

		// Check for Area 11 (left and center areas)
		else if (L3 == 0 && L2 == 0 && L1 == 0 && Center == 1 && R1 == 1 && R2 == 0 && R3 == 0)
				error = 0.5; // Example value for Area 11

		// Check for Area 12 (center area)
		else if (L3 == 0 && L2 == 0 && L1 == 0 && Center == 0 && R1 == 1 && R2 == 1 && R3 == 0)
				error = 1.5; // Example value for Area 12

		// Check for Area 13 (center and right areas) 
		else if (L3 == 0 && L2 == 0 && L1 == 0 && Center == 0 && R1 == 0 && R2 == 1 && R3 == 1)
				error = 2.5; // Example value for Area 13

		// Check for Area 14 (rightmost area)
		else if (L3 == 1 && L2 == 1 && L1 == 1 && Center == 0 && R1 == 0 && R2 == 0 && R3 == 0)
				error = -4; // Example value for Area 14

		// Check for Area 15 (right and center areas)
		else if (L3 == 0 && L2 == 1 && L1 == 1 && Center == 1 && R1 == 0 && R2 == 0 && R3 == 0)
				error = -2.5; // Example value for Area 15

		// Check for Area 16 (center and left areas)
		else if (L3 == 0 && L2 == 0 && L1 == 0 && Center == 1 && R1 == 1 && R2 == 1 && R3 == 0)
				error = 2.5; // Example value for Area 16

		// Check for Area 17 (center area)
		else if (L3 == 0 && L2 == 0 && L1 == 0 && Center == 0 && R1 == 1 && R2 == 1 && R3 == 1)
				error = 4; // Example value for Area 17
		
		else if (L3 == 1 && L2 == 1 && L1 == 1 && Center == 1 && R1 == 0 && R2 == 0 && R3 == 0)
				error = -4.5; // Example value for Area 17
		
		else if (L3 == 0 && L2 == 0 && L1 == 1 && Center == 1 && R1 == 1 && R2 == 1 && R3 == 1)
				error = 4.5; // Example value for Area 17
		// If none of the above cases match, there may be some irregularity in the line following, handle it accordingly
		else
				error = 0; // Example value for an unknown error state
		
		all_color = 0;
		no_color = 0;
	}
}

void Openmv_Receive_Data(int16_t com_data)
{

	uint8_t i;
	static uint8_t RxCounter1 = 0;//计数
	static uint16_t RxBuffer1[13] = { 0 };
	static uint8_t RxState = 0;
	//static uint8_t RxFlag1 = 0;

	if (RxState == 0 && com_data == 0x2C)  //0x2c帧头
	{
		RxState = 1;
		RxBuffer1[RxCounter1++] = com_data;
		//RED_OFF;
	}

	else if (RxState == 1 && com_data == 0x12)  //0x12帧头
	{
		RxState = 2;
		RxBuffer1[RxCounter1++] = com_data;

	}
	else if (RxState == 2)
	{
		RxBuffer1[RxCounter1++] = com_data;
		if (RxCounter1 >= 13 || com_data == 0x5B)       //RxBuffer1接受满了,接收数据结束
		{
			RxState = 3;
			no_color = RxBuffer1[RxCounter1 - 5];
			correction_openmv_int = RxBuffer1[RxCounter1 - 4];
			correction_openmv_flt = RxBuffer1[RxCounter1 - 3];
			all_color = RxBuffer1[RxCounter1 - 2];
		}
	}

	else if (RxState == 3)		//检测是否接受到结束标志
	{
		if (RxBuffer1[RxCounter1 - 1] == 0x5B)
		{

			RxCounter1 = 0;
			RxState = 0;

		}
		else   //接收错误
		{
			RxState = 0;
			RxCounter1 = 0;
			for (i = 0; i < 13; i++)
			{
				RxBuffer1[i] = 0x00;      //将存放数据数组清零
			}
		}
		
		if(RxState == 0 && com_data == 0x2C)
		{
			RxState = 1;
			RxBuffer1[RxCounter1++] = com_data;
		}
	}

	else   //接收异常
	{
		RxState = 0;
		RxCounter1 = 0;
		for (i = 0; i < 13; i++)
		{
			RxBuffer1[i] = 0x00;      //将存放数据数组清零
		}
	}
}
int count3 = 0;
void K210_Receive_Data_L(int16_t com_data)
{
	if(com_data != 0) signal = 1;
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
	if(Counter_R < 30){
		Counter_R++;
		count_num_R[com_data-1]++;
	}
	else if(Counter_R == 30)
	{
		for(i = 0;i < 8;i++)
		{
			if(count_num_R[i]>7 && one_use_R == 0)
			{
				target_1_R = i + 1;
				one_use_R = 1;
			}
			else if(count_num_R[i]>7)
			{
				target_2_R = i + 1;
			}
		}
	}
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