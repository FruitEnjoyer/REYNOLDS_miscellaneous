/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "fdcan.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pump.h"
#include "starter.h"
#include <stdlib.h>
#include "system.h"
#include "../../../../Thermocouple/thermocouple.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
struct TIM tim;
#pragma pack(0)
struct ADC_1 adc_1;
#pragma pack(0)
struct CONFIG config;
struct CONFIG *p_config;
#pragma pack(0)
struct GET_CONFIG get_config;
struct SET_CONFIG set_config;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//static inline void BLDC_Update();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern uint16_t var;
extern int16_t tg;
int16_t hall_ccr = 0;
float hall_speed = 0;
uint8_t no_capture = 1;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FDCAN1_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM15_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM17_Init();
  MX_ADC1_Init();
  MX_TIM7_Init();
  MX_TIM16_Init();
  MX_CRC_Init();
  MX_TIM20_Init();
  /* USER CODE BEGIN 2 */
    HAL_UART_Abort(rs485_puart);
    HAL_UARTEx_ReceiveToIdle_DMA(rs485_puart, (uint8_t*)(rs.rs485_rx_buff), sizeof(rs.rs485_rx_buff));
    __HAL_UART_ENABLE_IT(rs485_puart, UART_IT_IDLE);

    mtm.crc32 = HAL_CRC_Calculate(&hcrc, (uint32_t*)0x08000000, 131072);
    mtm.crc16 = (mtm.crc32 >> 16) ^ (mtm.crc32 & 0xFF);

    // Disable AD7689 chip select
    HAL_GPIO_WritePin(AD7689_CS_GPIO_Port, AD7689_CS_Pin, GPIO_PIN_SET);
    FLASH_CS_UNSELECT;

    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)(&(systemvars.adc)), 3);

    pump.targetspeed = 0;
    pump.idle.run_flag = 1;
    starter.targetspeed = 0;
    starter.idle.run_flag = 1;
    HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_3);
    HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_4);

    // Клапаны
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);

    // Свеча или искра
    HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&htim17, TIM_CHANNEL_1);
    HAL_TIM_Base_Start_IT(&htim16);
    HAL_TIM_Base_Start_IT(&htim20);
    HAL_Delay(5);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while(1)
    {
        if(tim.flag_1000Hz)
        {
            tim.flag_1000Hz = 0;
            Pump_Update();
            //var = (uint16_t)pump.filtspeed;
            Starter_Update();
        }
        if(tim.flag_10Hz)
        {
            tim.flag_10Hz = 0;
            LED1_GPIO_Port->BSRR = ((LED1_GPIO_Port->ODR & LED1_Pin) << 16u) | (~LED1_GPIO_Port->ODR & LED1_Pin);

            starter.speed = 0.5 * starter.speed + 0.5 * starter.speed_cnt / STARTER_MAGPAIRS / 6 * 1000 / 100 * 60;
            starter.intspeed = (uint32_t)starter.speed;
            starter.speed_cnt = 0;

            pump.speed = 0.5 * pump.speed + 0.5 * pump.speed_cnt / PUMP_MAGPAIRS / 6 * 1000 / 100 * 60;
            pump.intspeed = (uint32_t)pump.speed;
            pump.speed_cnt = 0;

            if(systemvars.adc_ready_flag)
            {
                systemvars.adc_ready_flag = 0;
                systemvars.vref = __HAL_ADC_CALC_VREFANALOG_VOLTAGE(systemvars.adc[0], ADC_RESOLUTION_12B);
                systemvars.mcu_temp = __HAL_ADC_CALC_TEMPERATURE(systemvars.vref, systemvars.adc[1], ADC_RESOLUTION_12B);
                systemvars.thermocouple_volts = __HAL_ADC_CALC_DATA_TO_VOLTAGE(systemvars.vref, systemvars.adc[2], ADC_RESOLUTION_12B);
                TC_Volts2Temp((float)systemvars.thermocouple_volts / 52.f - 14.9551345962f, 30.f, &(systemvars.thermocouple_temp));
                tg = (int16_t)systemvars.thermocouple_temp;
                hall_speed = 60.f / ((STARTER_IC_PSC + 1) * (float)hall_ccr / STARTER_TIM_FREQ * 2. * STARTER_MAGPAIRS);

                HAL_ADC_Start_DMA(&hadc1, (uint32_t*)systemvars.adc, 3);
            }

            if(flash.tm_on_flag)
            {
                TM_updater(tim.timer_ms);
            }

            rs.rs485_active_count++;
            if ((rs.rs485_active_count > 50) && (flash.tm_upload_flag == 0))
            {
                rs.rs485_active_count = 0;
                HAL_UART_Abort(rs485_puart);
                HAL_UARTEx_ReceiveToIdle_DMA(rs485_puart, (uint8_t*)rs.rs485_rx_buff, sizeof(rs.rs485_rx_buff));
                 __HAL_UART_ENABLE_IT(rs485_puart, UART_IT_IDLE);
            }
        }

        State_machine_flash();

        //обработка rs-485------------------------------------------------------------------------------------------------
        if(rs.rs485_rx_flag)
          {
              rs.rs485_rx_flag = 0;
              Frame_Test();
          }
        if(rs.rs485_tx_flag && (flash.fat_upload_flag == 0) && (flash.tm_upload_flag == 0))
        {
            rs.rs485_tx_flag = 0;
            if(rs.config_updating)
            {
                HAL_UARTEx_ReceiveToIdle_DMA(rs485_puart, (uint8_t*)(&config), sizeof(config) * 2);
            }
            else
            {
                HAL_UARTEx_ReceiveToIdle_DMA(rs485_puart, (uint8_t*)rs.rs485_rx_buff, sizeof(rs.rs485_rx_buff));
            }
        }

        LED2_GPIO_Port->BSRR = ((LED2_GPIO_Port->ODR & LED2_Pin) << 16u) | (~LED2_GPIO_Port->ODR & LED2_Pin);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV5;
  RCC_OscInitStruct.PLL.PLLN = 64;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint8_t nocap_cnt = 0;
#if 0
    if(htim == &htim6)
    {

        if(pump.control_mode_t != CLOSELOOP)
        {
            pump.field_state = (pump.field_state + 1) % 6;
            BLDC_SetPWM(&pump);
        }
    }
#endif
    if(htim == &htim7) // Starter
    {
        if(starter.control_mode_t != CLOSELOOP)
        {
            starter.speed_cnt += 1;
            starter.field_state = (starter.field_state + 1) % 6;
            BLDC_SetPWM(&starter);
        }
    }
    else if(htim == &htim5) // Pump
    {
        pump.field_state = (pump.field_state + 1) % 6;
        BLDC_SetPWM(&pump);
    }
#if 0
    else if(htim == &htim4) // Starter
    {
        if(1)//no_capture)
        {
            starter.field_state = (starter.field_state + 1) % 6;
            BLDC_SetPWM(&starter);
        }
        no_capture = 1;
    }
#endif
    if(htim == &htim16)
    {
        tim.flag_1000Hz = 1;
    }
    else if(htim == &htim20)
    {
        tim.flag_10Hz = 1;
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    int32_t pump_new_ccr, starter_new_ccr;

    if(htim == &htim2 && pump.control_mode_t == CLOSELOOP)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if(pump.field_state == STATE_3)
            {
                __HAL_TIM_SET_COUNTER(&htim5, 0);
                pump.speed_cnt += 1;
                pump.field_state = STATE_4;
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            if(pump.field_state == STATE_6)
            {
                pump.speed_cnt += 1;
                pump.field_state = STATE_1;
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        {
            if(pump.field_state == STATE_1)
            {
                pump.speed_cnt += 1;
                pump.field_state = STATE_2;
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
        {
            if(pump.field_state == STATE_4)
            {
                pump.speed_cnt += 1;
                pump.field_state = STATE_5;
            }
        }
        BLDC_SetPWM(&pump);
    } else if(htim == &htim5 && pump.control_mode_t == CLOSELOOP)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if(pump.field_state == STATE_2)
            {
                pump.speed_cnt += 1;
                pump.field_state = STATE_3;
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            if(pump.field_state == STATE_5)
            {
                pump.speed_cnt += 1;
                __HAL_TIM_SET_COUNTER(&htim2, 0);
                pump_new_ccr = ((__HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1) - __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_2)) +
                        (__HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_4) - __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_3)));
                if(pump_new_ccr > 0)
                {
                    pump.last_ccr = 0.95 * pump.last_ccr + 0.05 * pump_new_ccr / 6;
                }
                else{
                    pump.last_ccr = 0.95 * pump.last_ccr - 0.05 * pump_new_ccr / 6;
                }
                pump.field_state = STATE_6;
            }
        }
        BLDC_SetPWM(&pump);
    }

    else if(htim == &htim3 && starter.control_mode_t == CLOSELOOP)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if(starter.field_state == STATE_3)
            {
                starter.speed_cnt += 1;
                starter.field_state = STATE_4;
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            if(starter.field_state == STATE_6)
            {
                starter.speed_cnt += 1;
                starter.field_state = STATE_1;
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        {
            if(starter.field_state == STATE_1)
            {
                starter.speed_cnt += 1;
                starter.field_state = STATE_2;
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
        {
            if(starter.field_state == STATE_4)
            {
                starter.speed_cnt += 1;
                starter.field_state = STATE_5;
            }
        }
        BLDC_SetPWM(&starter);
    }
    else if(htim == &htim4 && starter.control_mode_t == CLOSELOOP)
    {

        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if(starter.field_state == STATE_2)
            {
                starter.speed_cnt += 1;
                starter.field_state = STATE_3;
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            if(starter.field_state == STATE_5)
            {
                //no_capture = 0;
                //__HAL_TIM_SET_COUNTER(&htim4, 0);
                starter.speed_cnt += 1;
                __HAL_TIM_SET_COUNTER(&htim3, 0);
                starter_new_ccr = ((__HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_1) - __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_2)) +
                        (__HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_4) - __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_3)));
                if(starter_new_ccr > 0)
                {
                    starter.last_ccr = 0.95 * starter.last_ccr + 0.05 * starter_new_ccr / 6;
                }
                else{
                    starter.last_ccr = 0.95 * starter.last_ccr - 0.05 * starter_new_ccr / 6;
                }
                starter.field_state = STATE_6;
            }
        }
        else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
        {
            __HAL_TIM_SET_COUNTER(&htim4, 0);
            hall_ccr = __HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_4) - __HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_3);
            //hall_speed = 60.f * 160000000 / 1600 / 6 / STARTER_MAGPAIRS / (__HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_4) - __HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_3));
        }
        BLDC_SetPWM(&starter);
    }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart == &huart1)
        {
            rs_232.rx_frame_size = Size;
            rs_232.rx_flag = 1;
        }
    if(huart == rs485_puart)
    {
        rs.receive_package_size = Size;
        rs.rs485_rx_flag = 1;
        rs.rs485_active_count = 0;
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart1)
    {
        rs_232.tx_flag = 0;
    }
    if(huart == rs485_puart)
    {
        rs.rs485_tx_flag = 1;
    }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
     if(hspi == &hspi1)
     {
         FLASH_CS_UNSELECT;
         flash.rx_tx_flag = 1;
        // spi_dma_flag=1;
     }
}
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while(1)
    {
    }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
