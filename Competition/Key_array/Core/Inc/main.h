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
#define KEY_COL0_Pin GPIO_PIN_7
#define KEY_COL0_GPIO_Port GPIOE
#define KEY_COL1_Pin GPIO_PIN_8
#define KEY_COL1_GPIO_Port GPIOE
#define KEY_COL2_Pin GPIO_PIN_9
#define KEY_COL2_GPIO_Port GPIOE
#define KEY_COL3_Pin GPIO_PIN_10
#define KEY_COL3_GPIO_Port GPIOE
#define KEY_ROW0_Pin GPIO_PIN_11
#define KEY_ROW0_GPIO_Port GPIOE
#define KEY_ROW1_Pin GPIO_PIN_12
#define KEY_ROW1_GPIO_Port GPIOE
#define KEY_ROW2_Pin GPIO_PIN_13
#define KEY_ROW2_GPIO_Port GPIOE
#define KEY_ROW3_Pin GPIO_PIN_14
#define KEY_ROW3_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
