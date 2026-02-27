/**
 * @file callbacks.c
 * @author ruslan
 * @brief 
 * @date 25.02.2026
 */

#include "main.h"
#include "BLDC/bldc.h"

extern bldc_t pump;
extern TIM_HandleTypeDef htim8;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM5)
    {
        // Catch ticks for speedtracking
        BLDC_IC_speedtracking(&pump, htim);
#if 0
        // Set PWMs for stator magnetic field
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {
            BLDC_IC_setPWM(&pump, &htim8);
            pump.pwm.state = (pump.pwm.state + 1) % 6;
        }
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
        {
            BLDC_IC_setPWM(&pump, &htim8);
            pump.pwm.state = (pump.pwm.state + 1) % 6;
        }
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
        {
            BLDC_IC_setPWM(&pump, &htim8);
            pump.pwm.state = (pump.pwm.state + 1) % 6;
        }
#endif
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM5)
    {
        // Increase number of timer overflows by 1
        pump.speedtracking.overflow += 1;
    }
    // TODO: перезапустить таймер?
}
