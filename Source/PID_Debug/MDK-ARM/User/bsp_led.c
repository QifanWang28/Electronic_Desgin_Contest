#include "bsp_led.h"

uint8_t led_counter = 0;
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();           	//??GPIOA/E??
		__HAL_RCC_GPIOB_CLK_ENABLE(); 
		__HAL_RCC_GPIOE_CLK_ENABLE();  	
	
    GPIO_Initure.Pin=GPIO_PIN_5; 				//PB5
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//????
    GPIO_Initure.Pull=GPIO_PULLUP;          	//??
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //??
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
		GPIO_Initure.Pin=GPIO_PIN_5; 				//PE5
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//????
    GPIO_Initure.Pull=GPIO_PULLUP;          	//??
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //??
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);

    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);	//PB5?0,????????
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_RESET);	//PE5?0,????????
}

void led_toggle(uint8_t red_or_green)
{
	
	if(red_or_green)
	{
		if(led_counter < 10) led_counter++;
		else{led_counter = 0; RED_TOGGLE;}
	}
	else	
	{
		if(led_counter < 10) led_counter++;
		else{led_counter = 0; GREEN_TOGGLE;}
	}
}
