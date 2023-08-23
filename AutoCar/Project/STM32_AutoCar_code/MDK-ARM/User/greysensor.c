#include "greysensor.h"

uint8_t L2 = 0;
uint8_t L1 = 0;
uint8_t Center = 0;
uint8_t R1 = 0;
uint8_t R2 = 0;
int8_t error = 0;

void get_grey_status(void)
{
	L2 = HAL_GPIO_ReadPin(GPIOB,L2_Pin);
	L1 = HAL_GPIO_ReadPin(GPIOB,L1_Pin);
	Center = HAL_GPIO_ReadPin(GPIOB,Center_Pin);
	R1 = HAL_GPIO_ReadPin(GPIOB,R1_Pin);
	R2 = HAL_GPIO_ReadPin(GPIOD,R2_Pin);
//	printf("%d %d %d %d %d\n", L2, L1, Center, R1, R2);
}

void get_error(void)
{
	get_grey_status();
	if(L2 == 1 && L1 == 0 && Center == 0 && R1 == 0 && R2 == 0)
	{
		error = -9;
	}
	else if(L2 == 0 && L1 == 1 && Center == 0 && R1 == 0 && R2 == 0)
	{
		error = -3;
	}
	else if(L2 == 0 && L1 == 0 && Center == 1 && R1 == 0 && R2 == 0)
	{
		error = 0;
	}
	else if(L2 == 0 && L1 == 0 && Center == 0 && R1 == 1 && R2 == 0)
	{
		error = 3;
	}
	else if(L2 == 0 && L1 == 0 && Center == 0 && R1 == 0 && R2 == 1)
	{
		error = 9;
	}
	else if(L2 == 1 && L1 == 1 && Center == 0 && R1 == 0 && R2 == 0)
	{
		error = -5;
	}
	else if(L2 == 0 && L1 == 1 && Center == 1 && R1 == 0 && R2 == 0)
	{
		error = -1;
	}
	else if(L2 == 0 && L1 == 0 && Center == 1 && R1 == 1 && R2 == 0)
	{
		error = 1;
	}
	else if(L2 == 0 && L1 == 0 && Center == 0 && R1 == 1 && R2 == 1)
	{
		error = 5;
	}
	else if(L2 == 1 && L1 == 1 && Center == 1 && R1 == 0 && R2 == 0)
	{
		error = -4;
	}
	else if(L2 == 0 && L1 == 1 && Center == 1 && R1 == 1 && R2 == 0)
	{
		error = 0;
	}
	else if(L2 == 0 && L1 == 0 && Center == 1 && R1 == 1 && R2 == 1)
	{
		error = 4;
	}
}
