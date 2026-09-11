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
#define PUMP_CURR_Pin GPIO_PIN_0
#define PUMP_CURR_GPIO_Port GPIOA
#define HEAT_CURR_Pin GPIO_PIN_1
#define HEAT_CURR_GPIO_Port GPIOA
#define PUMP_SPEED_B_Pin GPIO_PIN_2
#define PUMP_SPEED_B_GPIO_Port GPIOA
#define VBUS_Pin GPIO_PIN_3
#define VBUS_GPIO_Port GPIOA
#define PUMP_CONTROL_Pin GPIO_PIN_6
#define PUMP_CONTROL_GPIO_Port GPIOA
#define HEAT_CONTROL_Pin GPIO_PIN_0
#define HEAT_CONTROL_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_12
#define LED_GPIO_Port GPIOB
#define PUMP_PWMN_A_Pin GPIO_PIN_13
#define PUMP_PWMN_A_GPIO_Port GPIOB
#define PUMP_PWMN_B_Pin GPIO_PIN_14
#define PUMP_PWMN_B_GPIO_Port GPIOB
#define PUMP_PWMN_C_Pin GPIO_PIN_15
#define PUMP_PWMN_C_GPIO_Port GPIOB
#define PUMP_PWM_A_Pin GPIO_PIN_8
#define PUMP_PWM_A_GPIO_Port GPIOA
#define PUMP_PWM_B_Pin GPIO_PIN_9
#define PUMP_PWM_B_GPIO_Port GPIOA
#define PUMP_PWM_C_Pin GPIO_PIN_10
#define PUMP_PWM_C_GPIO_Port GPIOA
#define HEAT_PWM_Pin GPIO_PIN_11
#define HEAT_PWM_GPIO_Port GPIOA
#define HEAT_EN_Pin GPIO_PIN_12
#define HEAT_EN_GPIO_Port GPIOA
#define PUMP_SPEED_A_Pin GPIO_PIN_15
#define PUMP_SPEED_A_GPIO_Port GPIOA
#define PUMP_SPEED_C_Pin GPIO_PIN_6
#define PUMP_SPEED_C_GPIO_Port GPIOB
#define TJA1042_STB_Pin GPIO_PIN_7
#define TJA1042_STB_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define CAN_COMMAND_ID 0x0200
#define CAN_RESPONSE_ID 0x0201
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
