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
#include "REGULATION.h"
#include "PID.h"
#include "TM.h"
#include "RS_485.h"
#include "RS_232.h"
// TODO: #include "mcp3008.h"
#include "math.h"
#include "CAN.h"
#include "stdlib.h"
// TODO: #include "HP206.h"
// TODO: #include "LPS22HB.h"
#include "../../../../Thermocouple/thermocouple.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/*
40 Гц
*/
struct TIM
{
uint8_t flag_1Hz;
uint8_t flag_10Hz;
uint8_t flag_100Hz;
uint8_t flag_1000Hz;
uint8_t flag_40Hz;

uint32_t timer_ms;

uint8_t counter_ms;

uint8_t config_complate_flag;
uint8_t init_flag;

uint8_t tc_buff[4];
};

#pragma pack(1)
struct ADC_1
{
    uint16_t tr1;
    uint16_t tr2;
    uint16_t tr3;
    uint16_t t_cristal;
    uint16_t ref_v;
};

#pragma pack(1)
struct CONFIG
{
    uint16_t start_frame;

    uint8_t req_address;
    uint8_t req_command;

    uint8_t first_simbol_name;
    uint8_t second_simbol_name;
    uint16_t number_name;

    uint16_t starter_min;
    uint16_t starter_max;
    uint16_t pump_test_pwm;
    uint16_t starter_rate;
    uint16_t rate_up;
    uint16_t rate_down;
    uint16_t n1_ignition;
    uint16_t n1_overspeed;
    uint16_t n1_operation;
    uint16_t n1_min;
    uint16_t n1_max;
    uint16_t t_ignition;
    uint16_t t_vent;
    uint16_t n1_vent;
    uint8_t time_vent;
    uint16_t t_start_max;
    uint16_t t_oper_max;

    uint8_t fuel_curve_y[15];
    uint16_t fuel_curve_x[15];

    uint8_t curve_1_y[15];
    uint16_t curve_1_x[15];

    uint8_t curve_2_y[15];
    uint16_t curve_2_x[15];

    float starter_k;
    float starter_b;
    float starter_c;

    float starter_P;
    float starter_I;
    float starter_D;

    float main_P;
    float main_I;
    float main_D;


    float t_lim_P;
    float t_lim_I;
    float t_lim_D;

    uint16_t t_PID_lim;
    uint8_t variation;

    uint8_t engine_qualiti;

    int16_t tg_correct_k;
    uint16_t trv_restart_pump_pwm_corr;
    uint16_t u_pump_test_crit;//k3;
    uint16_t pump1_rate;
    uint16_t pump2_rate;
    uint16_t hot_pwm_correct;
    uint16_t bus_u_correct;
    uint16_t t_flameout;
    uint16_t n1_starter_off;

    uint8_t engine_type;

    uint8_t address;
    uint8_t group_address;

    uint8_t day;
    uint8_t mounth;
    uint8_t year;
    uint8_t numb_ver;

    uint8_t xor;

    uint16_t end_frame;
};


#pragma pack(1)
struct GET_CONFIG
{
    uint8_t dum1;
    uint8_t dum2;
    uint8_t dum3;
    uint8_t dum4;
    uint8_t dum5;
    uint8_t dum6;


    struct CONFIG config;

};
#pragma pack(0)

#pragma pack(1)
struct SET_CONFIG
{
    uint8_t dum1;
    uint8_t dum2;
    uint8_t dum3;
    uint8_t dum4;
    uint8_t dum5;


    struct CONFIG config;
};
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern struct TIM tim;
extern struct CONFIG config;
extern struct CONFIG *p_config;
extern struct GET_CONFIG get_config;
extern struct SET_CONFIG set_config;
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
#define RE_OFF // TODO: HAL_GPIO_WritePin(RE_GPIO_Port,RE_Pin,GPIO_PIN_SET);
#define RE_ON // TODO: HAL_GPIO_WritePin(RE_GPIO_Port,RE_Pin,GPIO_PIN_RESET);

#define INVALID_OPERATION_CODE 0x01          // неверный (несуществующий, неподдерживаемый) код операции
#define CHECKSUM_MISMATCH 0x02               // несовпадение контрольной суммы при приеме сообщения
#define INCORRECT_COMMAND_LENGTH 0x03        // неправильная длина команды
#define INCORRECT_PARAMETERS 0x05            // в принятой команде (кадре) обнаружены некорректные параметры
#define EXCEEDING_ARRAY_LIMITS 0x07          // предел массива при формировании параметров
#define GENERAL_FAILURE 0x10                 // общий сбой
#define FRAME_NOT_PROCESSED 0x0A             // не обработан кадр
#define TIME_REQUEST 0x0F                    // ответ на команду запроса текущего времени

#define MAX_PACKAGE_SIZE 250 //МАКС�?МАЛЬНЫЙ РАЗМЕР ПАКЕТА �?СПОЛЬЗУЕТСЯ В ОЦЕНКЕ КОРРЕКТНОСТ�? ДЛ�?НЫ ПР�?ШЕДШЕГО ПАКЕТА
#define MIN_PACKAGE_SIZE 7    //М�?Н�?МАЛЬНЫЙ-//-

#define STANDARD_ANSWER_SIZE 7//РАЗМЕР СТАНДАРТНОГО ОТВЕТА
#define TM_ANSWER_SIZE 27// 25//РАЗМЕР СТАНДАРТНОГО ОТВЕТА телеметрии
#define  ERROR 3


#define VER         10
#define DAY         3
#define MOUNTH      7
#define YEAR        26
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
