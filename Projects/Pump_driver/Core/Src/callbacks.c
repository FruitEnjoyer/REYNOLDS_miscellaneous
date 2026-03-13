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
extern uint8_t rx_buff[4];
extern bldc_t pump;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    /*
     * @brief Catch BEMF, calibrate TIM->ARR, calculate rotor speed
     */
    if(htim->Instance == pump.ictim->Instance)
    {
        if(pump.ictim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            if(pump.state_dir == FORWARD)
            {
                if(pump.field_state == STATE_3)
                    pump.field_state = STATE_4;
                else if(pump.field_state == STATE_6)
                    pump.field_state = STATE_1;
            }
            else
            {
                if(pump.field_state == STATE_3)
                    pump.field_state = STATE_2;
                else if(pump.field_state == STATE_6)
                    pump.field_state = STATE_5;
            }
            pump.last_ccr = pump.ictim->Instance->CCR1;
        }
        else if(pump.ictim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            if(pump.state_dir == FORWARD)
            {
                if(pump.field_state == STATE_2)
                    pump.field_state = STATE_3;
                else if(pump.field_state == STATE_5)
                    pump.field_state = STATE_6;
            }
            else
            {
                if(pump.field_state == STATE_2)
                    pump.field_state = STATE_1;
                else if(pump.field_state == STATE_5)
                    pump.field_state = STATE_4;
            }
            pump.last_ccr = pump.ictim->Instance->CCR2;
        }
        else if(pump.ictim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        {
            if(pump.state_dir == FORWARD)
            {
                if(pump.field_state == STATE_1)
                    pump.field_state = STATE_2;
                else if(pump.field_state == STATE_4)
                    pump.field_state = STATE_5;
            }
            else
            {
                if(pump.field_state == STATE_1)
                    pump.field_state = STATE_6;
                else if(pump.field_state == STATE_4)
                    pump.field_state = STATE_3;
            }
            pump.last_ccr = pump.ictim->Instance->CCR3;
        }
        pump.last_ccr = (uint32_t)LPF_filter(&pumpfilter, pump.last_ccr);;
        __HAL_TIM_SET_COUNTER(pump.ictim, 0);
        BLDC_SetPWM(&pump);
        pump.control_mode = INTERRUPT;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == pump.ictim->Instance)
    {
        pump.control_mode = MANUAL;
        __HAL_TIM_ENABLE_IT(pump.ictim, TIM_IT_UPDATE);
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
