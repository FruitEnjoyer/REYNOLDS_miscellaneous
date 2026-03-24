/**
 * @file callbacks.c
 * @author ruslan
 * @brief 
 * @date 25.02.2026
 */

#include "main.h"
#include "BLDC/bldc.h"
#include "uart_debug/uart_debug.h"
#include "LowPassFilter/lowpassfilter.h"

lpfilter_t pumpfilter;

extern TIM_HandleTypeDef htim8;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim2;
extern uint8_t rx_buff[4];
extern bldc_t pump;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    /*
     * @brief Catch BEMF, calibrate TIM->ARR, calculate rotor speed
     */
    if(htim->Instance == pump.ictim1->Instance)
    {
        if(pump.ictim1->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if(pump.field_state == STATE_6)
            {
                pump.last_ccr = pump.ictim1->Instance->CCR1;
                pump.field_state = STATE_1;
                pump.catched_interrupt = INTERRUPT_1;
            }
        }
        else if(pump.ictim1->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            if(pump.field_state == STATE_3)
            {
                pump.last_ccr = pump.ictim1->Instance->CCR2;
                pump.field_state = STATE_4;
                pump.catched_interrupt = INTERRUPT_4;
            }
        }
        else if(pump.ictim1->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        {
            if(pump.field_state == STATE_4)
            {
                pump.last_ccr = pump.ictim1->Instance->CCR3;
                pump.field_state = STATE_5;
                pump.catched_interrupt = INTERRUPT_5;
            }
        }
        else if(pump.ictim1->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
        {
            if(pump.field_state == STATE_1)
            {
                pump.last_ccr = pump.ictim1->Instance->CCR4;
                pump.field_state = STATE_2;
                pump.catched_interrupt = INTERRUPT_2;
            }
        }
        pump.last_ccr = (uint32_t)LPF_filter(&pumpfilter, pump.last_ccr);
        __HAL_TIM_SET_COUNTER(pump.ictim1, 0);
        BLDC_SetPWM(&pump);
        pump.control_mode = CLOSELOOP;
    }
    if(htim->Instance == TIM2)
    {
        if(htim2.Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if(pump.field_state == STATE_5)
            {
                pump.last_ccr = TIM2->CCR1;
                pump.field_state = STATE_6;
                pump.catched_interrupt = INTERRUPT_6;
            }
        }
        else if(htim2.Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            if(pump.field_state == STATE_2)
            {
                pump.last_ccr = TIM2->CCR2;
                pump.field_state = STATE_3;
                pump.catched_interrupt = INTERRUPT_3;
            }
        }
        pump.last_ccr = (uint32_t)LPF_filter(&pumpfilter, pump.last_ccr);
        __HAL_TIM_SET_COUNTER(pump.ictim2, 0);
        BLDC_SetPWM(&pump);
        pump.control_mode = CLOSELOOP;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == pump.ictim1->Instance)
    {
        pump.catched_interrupt = NO_INTERRUPT;
        __HAL_TIM_ENABLE_IT(pump.ictim1, TIM_IT_UPDATE);
    }
    if(htim->Instance == pump.ictim2->Instance)
    {
        pump.catched_interrupt = NO_INTERRUPT;
        __HAL_TIM_ENABLE_IT(pump.ictim2, TIM_IT_UPDATE);
    }
    if(htim->Instance == pump.statetim->Instance)
    {
    }
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
