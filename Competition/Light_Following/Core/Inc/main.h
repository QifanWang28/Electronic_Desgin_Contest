/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern uint8_t light_red_task;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USART2_Openmv_TX_Pin GPIO_PIN_2
#define USART2_Openmv_TX_GPIO_Port GPIOA
#define USART2_Openmv_RX_Pin GPIO_PIN_3
#define USART2_Openmv_RX_GPIO_Port GPIOA
#define KEY_COL0_Pin GPIO_PIN_4
#define KEY_COL0_GPIO_Port GPIOA
#define KEY_COL1_Pin GPIO_PIN_5
#define KEY_COL1_GPIO_Port GPIOA
#define KEY_COL2_Pin GPIO_PIN_6
#define KEY_COL2_GPIO_Port GPIOA
#define KEY_COL3_Pin GPIO_PIN_7
#define KEY_COL3_GPIO_Port GPIOA
#define KEY_ROW0_Pin GPIO_PIN_11
#define KEY_ROW0_GPIO_Port GPIOF
#define KEY_ROW1_Pin GPIO_PIN_12
#define KEY_ROW1_GPIO_Port GPIOF
#define KEY_ROW2_Pin GPIO_PIN_13
#define KEY_ROW2_GPIO_Port GPIOF
#define KEY_ROW3_Pin GPIO_PIN_14
#define KEY_ROW3_GPIO_Port GPIOF
#define Red_PWM_Up_Pin GPIO_PIN_9
#define Red_PWM_Up_GPIO_Port GPIOE
#define Red_PWM_Down_Pin GPIO_PIN_11
#define Red_PWM_Down_GPIO_Port GPIOE
#define Light_high_Pin GPIO_PIN_15
#define Light_high_GPIO_Port GPIOB
#define Debug_Tx_Pin GPIO_PIN_9
#define Debug_Tx_GPIO_Port GPIOA
#define Debug_usart1_Rx_Pin GPIO_PIN_10
#define Debug_usart1_Rx_GPIO_Port GPIOA
#define BEEP_Pin GPIO_PIN_8
#define BEEP_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
