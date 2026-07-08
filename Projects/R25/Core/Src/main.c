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
#include "fdcan.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../../../BLDC/bldc.h"
#include "../../../../LowPassFilter/lowpassfilter.h"
#include <stdlib.h>
#include "system.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SPEEDUP_SET_PSC(speed) __HAL_TIM_SET_PRESCALER(&htim6, (uint32_t)(TIM_FREQ / 6. / (TIM_BASE_INIT_ARR + 1) / BLDC_MAGPAIRS / speed * 60 - 1)); // speed = [rpm]
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
bldc_t pump = { .pwmtim = &htim1, .pwm_CCER_ch1 = (TIM_CCER_CC1E | TIM_CCER_CC1NE), .pwm_CCER_ch2 = (TIM_CCER_CC2E | TIM_CCER_CC2NE), .pwm_CCER_ch3 =
        (TIM_CCER_CC3E | TIM_CCER_CC3NE), .field_state = STATE_1, .state_dir_t = FORWARD, .control_mode_t = IDLE, .duty = BLDC_STARTUP_MINDUTY, .align.cnt = 0,
        .startup.cnt = 0, .idle.disabletim_flag = 1, .idle.run_flag = 0, .startup.finalspeed =
        BLDC_SPEEDUP_MAXSPEED, .closeloop.kp = -0.03, .filtspeed = 3000, .targetspeed = 8000};

int32_t target = BLDC_ARR_INITTARGET;
uint32_t wrong_ccr = 0;
uint32_t x = 0;

lpfilter_t filt1;
uint16_t closeloopcnt = 0;

extern AD7689_t extADC;
extern float speedup_inter[BLDC_SPEEDUP_INTER_NUM];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static inline void BLDC_Update();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t arr = 0;
uint32_t temp = 0;
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
  /* USER CODE BEGIN 2 */
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    AD7689_Init(&extADC);
    pump.idle.run_flag = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while(1)
    {
        BLDC_Update();
        HeartbeatLED_Update();
        AD7689_Update();
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
    if(htim == &htim6)
    {

        if(pump.control_mode_t != CLOSELOOP)
        {
            pump.field_state = (pump.field_state + 1) % 6;
        } else
        {
        }
        BLDC_SetPWM(&pump);
        HAL_TIM_Base_Start_IT(htim);
    } else if(htim == &htim2 || htim == &htim5)
    {
        __HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);
    }
}

static inline void BLDC_Update()
{
    static uint32_t bldc_ticks = 0;
    const static uint32_t bldc_delta = 1;

    if(bldc_ticks + bldc_delta < HAL_GetTick())
    {
        bldc_ticks += bldc_delta;

        switch(pump.control_mode_t)
        {
        case IDLE:
            if(pump.idle.disabletim_flag)
            {
                HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);
                HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_2);
                HAL_TIM_IC_Stop_IT(&htim5, TIM_CHANNEL_1);
                HAL_TIM_IC_Stop_IT(&htim5, TIM_CHANNEL_2);
                HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_3);
                HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_4);

                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
                HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
                HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
                HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
                HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
                HAL_TIM_Base_Stop_IT(&htim6);
                pump.idle.disabletim_flag = 0;
            }
            if(pump.idle.run_flag)
            {
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
                HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
                HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
                HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
                HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
                htim6.Instance->PSC = 5999;
                htim6.Instance->ARR = 5999;
                htim6.Instance->CNT = 0;
                pump.control_mode_t = ALIGN;
                pump.idle.disabletim_flag = 1;
                pump.field_state = STATE_1;
                pump.last_ccr = 100000;
                pump.duty = BLDC_STARTUP_MINDUTY;
                pump.filtspeed = 3000;
                pump.targetspeed = 8000;
                pump.closeloop.needrestart_flag = 0;
                pump.speed = 0;
                pump.closeloop.interr = 0;
                pump.closeloop.out = 0;
                closeloopcnt = 0;
                pump.usearr = 0;
                arr = BLDC_ARR_INITTARGET;
                target = BLDC_ARR_INITTARGET;
            }
            break;
        case ALIGN:
            if(!pump.idle.run_flag)
            {
                pump.control_mode_t = IDLE;
            }
            BLDC_SetPWM(&pump);
            pump.align.cnt++;
            if(pump.align.cnt > BLDC_ALIGN_DELAY / (float)bldc_delta)
            {
                pump.align.cnt = 0;
                pump.startup.speed = BLDC_SPEEDUP_MINSPEED;
                pump.startup.tmax = (BLDC_SPEEDUP_MAXSPEED - BLDC_SPEEDUP_MINSPEED) / (float)BLDC_SPEEDUP_ACCELERATION;
                pump.startup.t = 0;
                SPEEDUP_SET_PSC(pump.startup.speed);
                pump.control_mode_t = PRESTARTUP;
                HAL_TIM_Base_Start_IT(&htim6);
            }
            break;
        case PRESTARTUP:
            if(!pump.idle.run_flag)
            {
                pump.control_mode_t = IDLE;
            }
            pump.align.cnt++;
            if(pump.align.cnt > BLDC_PRESTARTUP_DELAY / (float)bldc_delta)
            {
                pump.align.cnt = 0;
                pump.control_mode_t = STARTUP;
            }
            break;

        case STARTUP:
            if(!pump.idle.run_flag)
            {
                pump.control_mode_t = IDLE;
            }

            if(pump.startup.t < pump.startup.tmax)
            {
                pump.startup.speed = (BLDC_SPEEDUP_MAXSPEED - BLDC_SPEEDUP_MINSPEED) * BLDC_Speedup(pump.startup.t / pump.startup.tmax) + BLDC_SPEEDUP_MINSPEED;
                pump.startup.t += bldc_delta / 1000.f;
                SPEEDUP_SET_PSC(pump.startup.speed);
                pump.startup.cnt = 0;
            } else if(pump.startup.cnt > 500)
            {
                pump.control_mode_t = CLOSELOOP;
                pump.startup.cnt = 0;
                HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
                HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
                HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
                HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_2);
                HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
                HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);
            }
            pump.startup.cnt += 1;
            break;

        case CLOSELOOP:
            if(!pump.idle.run_flag || pump.closeloop.needrestart_flag)
            {
                pump.closeloop.needrestart_flag = 0;
                pump.control_mode_t = IDLE;
            }

            pump.speed = 60. * 1 / ((float)pump.last_ccr / TIM_FREQ * 6. * BLDC_MAGPAIRS);
            pump.filtspeed = 0.99 * pump.filtspeed + 0.01 * pump.speed;
            arr = (uint32_t)(TIM_FREQ / 6. / BLDC_MAGPAIRS / (htim6.Instance->PSC + 1) / pump.speed * 60. - 1);
            if(closeloopcnt >= 100)
            {
                pump.usearr = 1;
                pump.closeloop.err = pump.targetspeed - pump.filtspeed;
                pump.closeloop.interr += pump.closeloop.err * 100 * bldc_delta / 1000.;
                if(pump.closeloop.interr > 100) pump.closeloop.interr = 100;
                if(pump.closeloop.interr < -100) pump.closeloop.interr = -100;
                pump.closeloop.out = pump.closeloop.kp * pump.closeloop.err + pump.closeloop.ki * pump.closeloop.interr;
                pump.duty = DutyByTargetPump(target + pump.closeloop.out);
                target += pump.closeloop.out;
                closeloopcnt = 0;
            }
            closeloopcnt += 1;
            break;

        default:
            pump.control_mode_t = IDLE;
            break;
        }
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    //__HAL_TIM_SET_COUNTER(&htim2, 0);
    static uint64_t iccounter = 0;
    int32_t new_ccr;

    if(htim == &htim2 && pump.control_mode_t == CLOSELOOP)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if(pump.field_state == STATE_3)
            {
                pump.field_state = STATE_4;
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            if(pump.field_state == STATE_6)
            {
                pump.field_state = STATE_1;
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        {
            if(pump.field_state == STATE_1)
            {
                pump.field_state = STATE_2;
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
        {
            if(pump.field_state == STATE_4)
            {
                pump.field_state = STATE_5;
            }
        }
    } else if(htim == &htim5 && pump.control_mode_t == CLOSELOOP)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if(pump.field_state == STATE_2)
            {
                __HAL_TIM_SET_COUNTER(&htim5, 0);
                new_ccr = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1) - __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_2);
                if(new_ccr > 0)
                {
                    pump.last_ccr = 0.95 * pump.last_ccr + 0.05 * new_ccr / 3;
                }
                else{
                    pump.last_ccr = 0.95 * pump.last_ccr - 0.05 * new_ccr / 3;
                }
                pump.field_state = STATE_3;
            }
        } else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            if(pump.field_state == STATE_5)
            {
                pump.field_state = STATE_6;
            }
        }
    }
    if(pump.usearr && pump.control_mode_t == CLOSELOOP)
    {
        __HAL_TIM_SET_AUTORELOAD(&htim6, (uint32_t)(arr * 0.4 + target * 0.6));
    }
    else if(pump.control_mode_t == CLOSELOOP)
    {
        __HAL_TIM_SET_AUTORELOAD(&htim6, (uint32_t)(htim6.Instance->ARR * 0.90 + arr * 0.04 + target * 0.06));
    }

    __HAL_TIM_SET_COUNTER(&htim6, 0);
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
