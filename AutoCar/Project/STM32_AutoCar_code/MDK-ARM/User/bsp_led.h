#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "stm32f1xx.h"
#include "gpio.h"
#include "main.h"

//#define RedSignal_Toggle HAL_GPIO_TogglePin(red_sig_GPIO_Port, red_sig_Pin)
//#define BlueSignal_Toggle HAL_GPIO_TogglePin(blue_sig_GPIO_Port, blue_sig_Pin)
//#define YellowSignal_Toggle HAL_GPIO_TogglePin(yellow_sig_GPIO_Port, yellow_sig_Pin)

//#define RedSignal_on  HAL_GPIO_WritePin(red_sig_GPIO_Port, red_sig_Pin,GPIO_PIN_SET)
//#define RedSignal_off HAL_GPIO_WritePin(red_sig_GPIO_Port, red_sig_Pin,GPIO_PIN_RESET)

//#define GreenSignal_on  HAL_GPIO_WritePin(blue_sig_GPIO_Port, blue_sig_Pin,GPIO_PIN_SET)
//#define GreenSignal_off  HAL_GPIO_WritePin(blue_sig_GPIO_Port, blue_sig_Pin,GPIO_PIN_RESET)

//#define YellowSignal_on  HAL_GPIO_WritePin(yellow_sig_GPIO_Port ,yellow_sig_Pin,GPIO_PIN_SET)
//#define YellowSignal_off  HAL_GPIO_WritePin(yellow_sig_GPIO_Port,yellow_sig_Pin,GPIO_PIN_RESET)

//#define greenLED_on  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET)
//#define greenLED_off  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET)

//#define greenLED_Toggle HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8)


//#define RedSignal_Toggle HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin)
//#define RedSignal_on  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET)
//#define RedSignal_off HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET)

//#define GreenSignal_Toggle HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin)
//#define GreenSignal_on  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)
//#define GreenSignal_off HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)

#define LED_RED_GPIO GPIOB
#define LED_RED_PIN GPIO_PIN_5
#define LED_GREEN_GPIO GPIOE
#define LED_GREEN_PIN GPIO_PIN_5

void LED_Init(void);
void led_toggle(uint8_t red_or_green);
#define RED_OFF		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET)
#define RED_ON		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET)
#define RED_TOGGLE HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5)

#define GREEN_OFF		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET)
#define GREEN_ON		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_5,GPIO_PIN_SET)
#define GREEN_TOGGLE HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_5)
#endif
