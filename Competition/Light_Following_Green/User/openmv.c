#include "openmv.h"

void Openmv_Receive_Data(u8 com_data)
{
	
	uint8_t i;
	static uint8_t RxCounter1 = 0;//计数
	static uint8_t RxBuffer1[10] = { 0 };
	static uint8_t RxState = 0;
//	static uint8_t RxFlag1 = 0;
	
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
			x_err = (float)RxBuffer1[RxCounter1 - 4] * ((RxBuffer1[RxCounter1 - 5] == 1)?1:-1);
			y_err = (float)RxBuffer1[RxCounter1 - 2] * ((RxBuffer1[RxCounter1 - 3] == 1)?1:-1);
			//printf("%f %f\n", x_err, y_err);
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


