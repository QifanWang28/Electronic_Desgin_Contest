#include "openmv.h"

uint8_t loc_x_int,loc_x_flt = 0;
uint8_t loc_y_int,loc_y_flt = 0;
float loc_x = 0;
float loc_y = 0;
float fps = 0;
float speed_x = 0;
float speed_y = 0;
float loc_x_r = 0;
float loc_y_r = 0;

uint8_t ball_flag = 0;
uint8_t start_flag = 0;
uint32_t count_fps = 0;

//uint8_t fps_int = 0;
//uint8_t fps_flt = 0;
//	
void Openmv_Receive_Data(int16_t com_data)
{
	
	uint8_t i;
	static uint8_t RxCounter1 = 0;//计数
	static uint8_t RxBuffer1[10] = { 0 };
	static uint8_t RxState = 0;
	static uint8_t RxFlag1 = 0;
	
	if (RxState == 0 && com_data == 0x2C)  //0x2c帧头
	{
		RxState = 1;
		RxBuffer1[RxCounter1++] = com_data;	
	}

	else if (RxState == 1 && com_data == 0x12)  //0x12帧头
	{
		RxState = 2;
		RxBuffer1[RxCounter1++] = com_data;
	}
	else if (RxState == 2)
	{
		RxBuffer1[RxCounter1++] = com_data;
		if (RxCounter1 >= 10 || com_data == 0x5B)       //RxBuffer1接受满了,接收数据结束
		{
			RxState = 3;
			loc_x_int = RxBuffer1[RxCounter1 - 5];
			loc_x_flt = RxBuffer1[RxCounter1 - 4];
			loc_y_int = RxBuffer1[RxCounter1 - 3];
			loc_y_flt = RxBuffer1[RxCounter1 - 2];
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
			for (i = 0; i < 10; i++)
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
		for (i = 0; i < 10; i++)
		{
			RxBuffer1[i] = 0x00;      //将存放数据数组清零
		}
	}
}
u8 x_r = 0;
u8 y_r = 0;

void trans_loc()
{		
		
		loc_x_r = loc_x;
		loc_y_r = loc_y;
		
		if(x_r - loc_x_int < 10 && x_r - loc_x_int > -10)
		{
				loc_x = loc_x_int + (float)loc_x_flt/100;
		}
		
		if(y_r - loc_y_int < 10 && y_r - loc_y_int > -10)
		{
				loc_y = loc_y_int + (float)loc_y_flt/100;
		}
		
		if(loc_x - loc_x_r < 5 && loc_x_r > -5)	speed_x = (loc_x - loc_x_r) * 48.45;
		if(loc_y - loc_y_r < 5 && loc_y_r > -5) speed_y = (loc_y - loc_y_r) * 48.45;
		
//		if(count2 < 5)count2++;
//		else{count2 = 0; printf("%f %f %f\n",speed_x, loc_x, loc_x_r);}
		
		x_r = loc_x_int;
		y_r = loc_y_int;
}
