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
#include "cordic.h"
#include "fdcan.h"
#include "fmac.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CAN_protocol/can_protocol.h"
#include "BLDC/bldc.h"
#include "LowPassFilter/lowpassfilter.h"
#include "uart_debug/uart_debug.h"
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
extern lpfilter_t pumpfilter;
bldc_t pump = {
        .pwmtim = &htim8,
        .ictim = &htim5,
        .pole_number = 7,
        .pwm_CCER_ch1 = (TIM_CCER_CC1E | TIM_CCER_CC1NE),
        .pwm_CCER_ch2 = (TIM_CCER_CC2E | TIM_CCER_CC2NE),
        .pwm_CCER_ch3 = (TIM_CCER_CC3E | TIM_CCER_CC3NE)
};

uint8_t rx_buff[4] = {0,};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HeartbeatLED_Update();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
MessageData_t can_data = {
        .recv_id = 0,
        .pumpdriver.pwm_pump = 0,
        .pumpdriver.pwm_heat = 0,
        .pumpdriver.pump_speed = 0,
        .pumpdriver.pump_current = 0
};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
    uint32_t can_tickcounter = 0;
    uint32_t ctrl_tick = 0;
    float speed_target = 1300.f, ctrl_value = BLDC_DEFAULTCTRL;
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
  MX_FDCAN1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM8_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_CORDIC_Init();
  MX_FMAC_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */
    HAL_FDCAN_Start(&hfdcan1);
    HAL_FDCAN_ActivateNotification(&hfdcan1,
            FDCAN_IT_LIST_RX_FIFO0 | FDCAN_IT_LIST_SMSG,
            FDCAN_TX_BUFFER0 | FDCAN_TX_BUFFER1 | FDCAN_TX_BUFFER2);
    HAL_GPIO_WritePin(CAN_STB_GPIO_Port, CAN_STB_Pin, GPIO_PIN_RESET);

    LPF_init(&pumpfilter, BLACKMAN);
    BLDC_Configure(&pump);
    BLDC_Start(&pump);

    //HAL_UART_Receive_IT(&huart1, rx_buff, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while(1)
    {
        HAL_IWDG_Refresh(&hiwdg);
#if 1
        BLDC_Restart(&pump);
#endif
#if 1
        if(ctrl_tick + 10 < HAL_GetTick() && pump.control_mode_t == INTERRUPT)
        {
            if(pump.speed < speed_target - 5.f)
            {
                ctrl_value += 0.0001;
                BLDC_SetCtrl(&pump, ctrl_value);
            }
            else if(pump.speed > speed_target + 5.f)
            {
                ctrl_value -= 0.0001;
                BLDC_SetCtrl(&pump, ctrl_value);
            }
            ctrl_tick += 10;
        }
#endif
        DBG_SendInfo_BLDC(&pump);
#if 0
        if(can_tickcounter + 1000 < HAL_GetTick())
        {
            SendMessage(&hfdcan1, COMMAND_PUMPDRIVER, &can_data);
            can_data.pumpdriver.pump_current += 1;
            can_data.pumpdriver.pump_speed += 2;
            can_data.pumpdriver.pwm_heat += 3;
            can_data.pumpdriver.pwm_pump += 4;
            can_tickcounter += 1000;
        }
#endif
        HeartbeatLED_Update();
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 20;
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
void HeartbeatLED_Update()
{
    static uint32_t heartbeat_ticks = 0;
    static uint32_t heartbear_delta = 500;

    if (heartbeat_ticks + heartbear_delta < HAL_GetTick())
    {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
        heartbeat_ticks += 500;
    }
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
