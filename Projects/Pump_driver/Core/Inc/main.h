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
#include "stm32g4xx_hal.h"

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
#define PUMP_SPEED_1_Pin GPIO_PIN_0
#define PUMP_SPEED_1_GPIO_Port GPIOA
#define PUMP_SPEED_2_Pin GPIO_PIN_1
#define PUMP_SPEED_2_GPIO_Port GPIOA
#define PUMP_SPEED_3_Pin GPIO_PIN_2
#define PUMP_SPEED_3_GPIO_Port GPIOA
#define PUMP_CURR_1_Pin GPIO_PIN_3
#define PUMP_CURR_1_GPIO_Port GPIOA
#define CONTROLLER_PUMP_Pin GPIO_PIN_6
#define CONTROLLER_PUMP_GPIO_Port GPIOA
#define PUMP_CURR_2_Pin GPIO_PIN_7
#define PUMP_CURR_2_GPIO_Port GPIOA
#define PUMP_CURR_3_Pin GPIO_PIN_1
#define PUMP_CURR_3_GPIO_Port GPIOB
#define HEAT_PWMN_Pin GPIO_PIN_13
#define HEAT_PWMN_GPIO_Port GPIOB
#define CAN_STB_Pin GPIO_PIN_14
#define CAN_STB_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_15
#define LED_GPIO_Port GPIOB
#define HEAT_PWM_Pin GPIO_PIN_8
#define HEAT_PWM_GPIO_Port GPIOA
#define PUMP_PWM_1_Pin GPIO_PIN_6
#define PUMP_PWM_1_GPIO_Port GPIOB
#define CONTROLLER_HEAT_Pin GPIO_PIN_7
#define CONTROLLER_HEAT_GPIO_Port GPIOB
#define PUMP_PWM_2_Pin GPIO_PIN_8
#define PUMP_PWM_2_GPIO_Port GPIOB
#define PUMP_PWM_3_Pin GPIO_PIN_9
#define PUMP_PWM_3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
