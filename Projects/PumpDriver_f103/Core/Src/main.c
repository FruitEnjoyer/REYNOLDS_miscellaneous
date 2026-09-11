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
#include "can.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pump.h"
#include "stm32f1xx_ll_adc.h"
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
volatile uint16_t adc[5] = {0,};
int64_t vcpu, temp, vbus, currheat, currpump;
volatile uint8_t adc_flag = 0;

volatile uint8_t can_flag = 0;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8] = {0,};
uint8_t RxData[8] = {0,};
uint32_t TxMailbox = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void HeartbeatLED_Update();
static void Heat_Update();
static void ADC_Update();
static void CAN_Update();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t can_send_flag = 0;
uint32_t can_id = 0x1234;
uint16_t pumpduty = 0, heatduty = 0;
uint16_t pumptarget = 0;
uint16_t heattarget = 0;
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
  MX_ADC1_Init();
  MX_CAN_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc, 5);
  HAL_GPIO_WritePin(HEAT_EN_GPIO_Port, HEAT_EN_Pin, GPIO_PIN_SET); // Enable IR2184
  HAL_GPIO_WritePin(TJA1042_STB_GPIO_Port, TJA1042_STB_Pin, GPIO_PIN_RESET); // Enable TJA1042
  HAL_CAN_Start(&hcan);
  HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_ERROR | CAN_IT_BUSOFF | CAN_IT_LAST_ERROR_CODE);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, 0);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
  pump.targetspeed = 0;
  pump.idle.run_flag = 1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      ADC_Update();
      CAN_Update();
      Heat_Update();
      HeartbeatLED_Update();
      Pump_Update();
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
static void HeartbeatLED_Update()
{
    static uint32_t heartbeat_ticks = 0;
    static const uint32_t heartbeat_delta = 125;

    if (heartbeat_ticks + heartbeat_delta < HAL_GetTick())
    {
        LED_GPIO_Port->BSRR = ((LED_GPIO_Port->ODR & LED_Pin) << 16u) | (~LED_GPIO_Port->ODR & LED_Pin);
        heartbeat_ticks += heartbeat_delta;
    }
}

static void CAN_Update()
{
    static uint32_t can_ticks = 0;
    static const uint32_t can_delta = 50; // 20 Hz

    if(can_flag) // Command has been received
    {
        HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData);
        pumptarget = (RxData[1] << 8) | (RxData[0]);
        heattarget = (RxData[3] << 8) | (RxData[2]);
        Pump_SetDuty(pumptarget);
        can_flag = 0;
    }

    if (can_ticks + can_delta < HAL_GetTick())
    {
        pump.speed = 0.5 * pump.speed + 0.5 * pump.speed_cnt / PUMP_MAGPAIRS / 6 * 1000 / can_delta * 60;
        pump.intspeed = (uint32_t)pump.speed;
        pump.speed_cnt = 0;
        if(HAL_CAN_GetTxMailboxesFreeLevel(&hcan) > 0)// && can_send_flag)
        {
            // Send message here
            TxHeader.StdId = CAN_RESPONSE_ID;
            TxHeader.ExtId = 0;
            TxHeader.RTR = CAN_RTR_DATA; //CAN_RTR_REMOTE
            TxHeader.IDE = CAN_ID_STD;   // CAN_ID_EXT
            TxHeader.DLC = 8;
            TxHeader.TransmitGlobalTime = 0;
            //TxData[0] = (uint8_t)(pumpduty);
            //TxData[1] = (uint8_t)(pumpduty >> 8);
            //TxData[2] = (uint8_t)(heatduty);
            //TxData[3] = (uint8_t)(heatduty >> 8);
            TxData[0] = (uint8_t)(pump.intspeed);
            TxData[1] = (uint8_t)(pump.intspeed >> 8);
            TxData[2] = (uint8_t)(currheat);
            TxData[3] = (uint8_t)(currheat >> 8);
            TxData[4] = (uint8_t)(currpump);
            TxData[5] = (uint8_t)(currpump >> 8);
            TxData[6] = (uint8_t)(vbus);
            TxData[7] = (uint8_t)(vbus >> 8);
            HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);
            can_send_flag = 0;
        }
        can_ticks += can_delta;
    }
}

static void Heat_Update()
{
    static uint32_t heat_ticks = 0;
    static const uint32_t heat_delta = 10; // 100 Hz

    if (heat_ticks + heat_delta < HAL_GetTick())
    {
        if(heattarget > 600) heattarget = 600;
        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, heattarget);
        heat_ticks += heat_delta;
    }
}

static void ADC_Update()
{
    static uint32_t adc_ticks = 0;
    static const uint32_t adc_delta = 100;

    if (adc_ticks + adc_delta < HAL_GetTick())
    {
        if(adc_flag) // Conversion completed
        {
            vcpu = 1200 * 4096 / adc[0];
            temp = (1430 - adc[1] * vcpu / 4096) * 1000 / 4300 + 25;
            vbus = adc[2] * vcpu * 11 / 4096;
            currheat = adc[3] * vcpu * 5000 * 200 / 160000 / 4096;
            currpump = adc[4] * vcpu * 5000 * 200 / 160000 / 4096;
            adc_flag = 0;
            HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc, 5);
        }
        adc_ticks += adc_delta;
    }
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
    {

        if(RxHeader.StdId == CAN_COMMAND_ID)
        {
            can_flag = 1;
        }
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc == &hadc1)
    {
        adc_flag = 1;
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    int32_t new_ccr;

    if(htim == &htim2 && pump.control_mode_t == CLOSELOOP)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) // fallA -> riseA
        {
            if(pump.field_state == STATE_6) // 3 -> 6
            {
                __HAL_TIM_SET_COUNTER(&htim4, 0);
                pump.speed_cnt += 1;
                pump.field_state = STATE_1; // 4 -> 1
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) // riseA -> fallA
        {
            if(pump.field_state == STATE_3) // 6 -> 3
            {
                pump.speed_cnt += 1;
                pump.field_state = STATE_4; // 1 -> 4
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3) // fallC -> riseB
        {
            if(pump.field_state == STATE_2) // 1 -> 2
            {
                pump.speed_cnt += 1;
                pump.field_state = STATE_3; // 2 -> 3
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) // riseC -> fallB
        {
            if(pump.field_state == STATE_5) // 4 -> 5
            {
                pump.speed_cnt += 1;
                pump.field_state = STATE_6; // 5 -> 6
            }
        }
        BLDC_SetPWM(&pump);
    }
    else if(htim == &htim4 && pump.control_mode_t == CLOSELOOP)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) // riseB -> riseC
        {
            if(pump.field_state == STATE_4) // 2 -> 4
            {
                pump.speed_cnt += 1;
                pump.field_state = STATE_5; // 3 -> 5
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) // fallB ->fallC
        {
            if(pump.field_state == STATE_1) // 5 -> 1
            {
                pump.speed_cnt += 1;
                __HAL_TIM_SET_COUNTER(&htim2, 0);
                new_ccr = ((__HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1) - __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_2)) +
                        (__HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_4) - __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_3)));
                if(new_ccr > 0)
                {
                    pump.last_ccr = 0.95 * pump.last_ccr + 0.05 * new_ccr / 6;
                }
                else{
                    pump.last_ccr = 0.95 * pump.last_ccr - 0.05 * new_ccr / 6;
                }
                pump.field_state = STATE_2; // 6 -> 2
            }
        }
        BLDC_SetPWM(&pump);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim4)
    {
        pump.field_state = (pump.field_state + 1) % 6;
        BLDC_SetPWM(&pump);
    }
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
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
  while (1)
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
