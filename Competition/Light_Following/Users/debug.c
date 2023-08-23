#include "debug.h"

// debug 走usart1
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}
 
int fgetc(FILE *f)
{
  uint8_t ch = 0;
  HAL_UART_Receive(&huart1, &ch, 1, 0xffff);
  return ch;
}

float angle_Up = 0;
float angle_Down = 0;

int dt_up = 0;
int dt_down = 0;

uint8_t i;
static uint8_t RxCounter1 = 0;//计数
static uint8_t RxBuffer1[10] = { 0 };
static uint8_t RxState = 0;

void Debug_Rec(u8 com_data)
{

	//printf("state: %d\n", com_data);
	
	if (RxState == 0 && com_data == 122 - '0')  //0x2c帧头
	{
		RxState = 1;
		RxBuffer1[RxCounter1++] = com_data;	
	}
	else if (RxState == 1)
	{
		RxBuffer1[RxCounter1++] = com_data;
		if (RxCounter1 >= 10 || com_data == 121 - '0')       //RxBuffer1接受满了,接收数据结束
		{
			RxState = 3;
			light_red_task = RxBuffer1[RxCounter1-2];
//			light = RxBuffer1[RxCounter1-8];
//			dt_up = (RxBuffer1[RxCounter1-7] == 'f'-'0')?-1:1;
//			dt_down = (RxBuffer1[RxCounter1-4] == 'f'-'0')?-1:1;
//			angle_Up = (float)(RxBuffer1[RxCounter1 - 6] * 10+RxBuffer1[RxCounter1 - 5]) * dt_up;
//			angle_Down =  (RxBuffer1[RxCounter1 - 3]*10 + RxBuffer1[RxCounter1 - 2]) * dt_down;
			
		}
	}

	else if (RxState == 3)		//检测是否接受到结束标志
	{
		if (RxBuffer1[RxCounter1 - 1] == 122 - '0')
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
		
		if(RxState == 0 && com_data == 122 - '0')
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
