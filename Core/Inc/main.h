/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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
#define DRES_Pin GPIO_PIN_15
#define DRES_GPIO_Port GPIOC
#define LEDR_Pin GPIO_PIN_0
#define LEDR_GPIO_Port GPIOA
#define LEDB_Pin GPIO_PIN_2
#define LEDB_GPIO_Port GPIOA
#define LEDG_Pin GPIO_PIN_3
#define LEDG_GPIO_Port GPIOA
#define BUT2_Pin GPIO_PIN_4
#define BUT2_GPIO_Port GPIOA
#define A_BATT_Pin GPIO_PIN_5
#define A_BATT_GPIO_Port GPIOA
#define OPTINT_Pin GPIO_PIN_6
#define OPTINT_GPIO_Port GPIOA
#define IMUINT1_Pin GPIO_PIN_8
#define IMUINT1_GPIO_Port GPIOA
#define NCHG_Pin GPIO_PIN_9
#define NCHG_GPIO_Port GPIOA
#define BUT1_Pin GPIO_PIN_10
#define BUT1_GPIO_Port GPIOA
#define IMUINT2_Pin GPIO_PIN_3
#define IMUINT2_GPIO_Port GPIOB
#define V36EN_Pin GPIO_PIN_5
#define V36EN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
