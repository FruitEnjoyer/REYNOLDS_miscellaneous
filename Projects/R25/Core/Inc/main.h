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
#define STARTER_SPEED_A_Pin GPIO_PIN_2
#define STARTER_SPEED_A_GPIO_Port GPIOE
#define STARTER_SPEED_C_Pin GPIO_PIN_4
#define STARTER_SPEED_C_GPIO_Port GPIOE
#define VALVEMAIN_PWM_Pin GPIO_PIN_9
#define VALVEMAIN_PWM_GPIO_Port GPIOF
#define VALVESTART_PWM_Pin GPIO_PIN_10
#define VALVESTART_PWM_GPIO_Port GPIOF
#define SPI_CLK_Pin GPIO_PIN_5
#define SPI_CLK_GPIO_Port GPIOA
#define SPI_MISO_Pin GPIO_PIN_6
#define SPI_MISO_GPIO_Port GPIOA
#define SPI_MOSI_Pin GPIO_PIN_7
#define SPI_MOSI_GPIO_Port GPIOA
#define AD7689_CS_Pin GPIO_PIN_4
#define AD7689_CS_GPIO_Port GPIOC
#define FLASH_CS_Pin GPIO_PIN_5
#define FLASH_CS_GPIO_Port GPIOC
#define FLASH_RESET_Pin GPIO_PIN_0
#define FLASH_RESET_GPIO_Port GPIOB
#define PUMP_SPEED_B_Pin GPIO_PIN_2
#define PUMP_SPEED_B_GPIO_Port GPIOB
#define PUMP_PWMN_A_Pin GPIO_PIN_8
#define PUMP_PWMN_A_GPIO_Port GPIOE
#define PUMP_PWM_A_Pin GPIO_PIN_9
#define PUMP_PWM_A_GPIO_Port GPIOE
#define PUMP_PWMN_B_Pin GPIO_PIN_10
#define PUMP_PWMN_B_GPIO_Port GPIOE
#define PUMP_PWM_B_Pin GPIO_PIN_11
#define PUMP_PWM_B_GPIO_Port GPIOE
#define PUMP_PWMN_C_Pin GPIO_PIN_12
#define PUMP_PWMN_C_GPIO_Port GPIOE
#define PUMP_PWM_C_Pin GPIO_PIN_13
#define PUMP_PWM_C_GPIO_Port GPIOE
#define LED2_Pin GPIO_PIN_12
#define LED2_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOB
#define RS485_DE_Pin GPIO_PIN_14
#define RS485_DE_GPIO_Port GPIOB
#define RS485_DI_Pin GPIO_PIN_8
#define RS485_DI_GPIO_Port GPIOD
#define RS485_RO_Pin GPIO_PIN_9
#define RS485_RO_GPIO_Port GPIOD
#define FLASH_WP_Pin GPIO_PIN_13
#define FLASH_WP_GPIO_Port GPIOD
#define STABILIZER_Pin GPIO_PIN_14
#define STABILIZER_GPIO_Port GPIOD
#define CAN_STB_Pin GPIO_PIN_15
#define CAN_STB_GPIO_Port GPIOD
#define STARTER_PWM_A_Pin GPIO_PIN_6
#define STARTER_PWM_A_GPIO_Port GPIOC
#define STARTER_PWM_B_Pin GPIO_PIN_7
#define STARTER_PWM_B_GPIO_Port GPIOC
#define STARTER_PWM_C_Pin GPIO_PIN_8
#define STARTER_PWM_C_GPIO_Port GPIOC
#define RS232_TX_Pin GPIO_PIN_9
#define RS232_TX_GPIO_Port GPIOA
#define RS232_RX_Pin GPIO_PIN_10
#define RS232_RX_GPIO_Port GPIOA
#define CAN_RX_Pin GPIO_PIN_11
#define CAN_RX_GPIO_Port GPIOA
#define CAN_TX_Pin GPIO_PIN_12
#define CAN_TX_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define STARTER_PWMN_A_Pin GPIO_PIN_10
#define STARTER_PWMN_A_GPIO_Port GPIOC
#define STARTER_PWMN_B_Pin GPIO_PIN_11
#define STARTER_PWMN_B_GPIO_Port GPIOC
#define STARTER_PWMN_C_Pin GPIO_PIN_12
#define STARTER_PWMN_C_GPIO_Port GPIOC
#define PUMP_SPEED_C_Pin GPIO_PIN_6
#define PUMP_SPEED_C_GPIO_Port GPIOD
#define PUMP_SPEED_A_Pin GPIO_PIN_3
#define PUMP_SPEED_A_GPIO_Port GPIOB
#define SPARKPLUG_PWM_Pin GPIO_PIN_5
#define SPARKPLUG_PWM_GPIO_Port GPIOB
#define STARTER_SPEED_B_Pin GPIO_PIN_6
#define STARTER_SPEED_B_GPIO_Port GPIOB
#define SPARKPLUG_PWMN_Pin GPIO_PIN_7
#define SPARKPLUG_PWMN_GPIO_Port GPIOB
#define STARTER_SPEED_HALL_Pin GPIO_PIN_9
#define STARTER_SPEED_HALL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
