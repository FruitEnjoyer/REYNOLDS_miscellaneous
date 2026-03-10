/**
 * @file callbacks.c
 * @author ruslan
 * @brief 
 * @date 25.02.2026
 */

#include "main.h"
#include "BLDC/bldc.h"
#include "uart_debug/uart_debug.h"

extern TIM_HandleTypeDef htim8;
extern uint8_t rx_buff[4];
extern bldc_t pump;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    /*
     * @brief Catch BEMF, calibrate TIM->ARR, calculate rotor speed
     */
    if(htim->Instance == TIM5)
    {

    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /**
     * @brief Switching PWM states on BLDC phases
     */
    if(htim->Instance == TIM5)
    {
        switch(pump.state_dir)
        {
        case FORWARD:
            pump.field_state = (pump.field_state + 1) % 6;
            break;
        case REVERSE:
            pump.field_state = (pump.field_state + 6 - 1) % 6;
            break;
        default:
            break;
        }
        BLDC_SetPWM(&pump);
        pump.IC_TIM->DIER |= (TIM_DIER_UIE | TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE);
    }

    // TODO: перезапустить таймер?
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        switch(rx_buff[0])
        {
        case '1':
            break;
        case '2':
            break;
        default:
            break;
        }
        HAL_UART_Receive_IT(huart, rx_buff, 1);
    }
}
