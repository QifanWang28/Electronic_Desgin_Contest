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
#define RIGHT_HIGH_Pin GPIO_PIN_7
#define RIGHT_HIGH_GPIO_Port GPIOF
#define LEFT_HIGH_Pin GPIO_PIN_8
#define LEFT_HIGH_GPIO_Port GPIOF
#define DRIVE_HIGH_Pin GPIO_PIN_9
#define DRIVE_HIGH_GPIO_Port GPIOF
#define L2_Pin GPIO_PIN_12
#define L2_GPIO_Port GPIOB
#define L1_Pin GPIO_PIN_13
#define L1_GPIO_Port GPIOB
#define Center_Pin GPIO_PIN_14
#define Center_GPIO_Port GPIOB
#define R1_Pin GPIO_PIN_15
#define R1_GPIO_Port GPIOB
#define R2_Pin GPIO_PIN_8
#define R2_GPIO_Port GPIOD
#define RIGHT_LOW_Pin GPIO_PIN_3
#define RIGHT_LOW_GPIO_Port GPIOG
#define LEFT_LOW_Pin GPIO_PIN_4
#define LEFT_LOW_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
