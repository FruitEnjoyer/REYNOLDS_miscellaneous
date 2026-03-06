/**
 * @file callbacks.c
 * @author ruslan
 * @brief 
 * @date 25.02.2026
 */

#include "main.h"
#include "BLDC/bldc.h"

extern TIM_HandleTypeDef htim8;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM5)
    {

    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM5)
    {

    }
    if(htim->Instance == TIM6)
    {

    }
    // TODO: перезапустить таймер?
}
