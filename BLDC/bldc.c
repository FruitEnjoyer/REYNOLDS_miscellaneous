/**
 * @file bldc.c
 * @author ruslan
 * @brief 
 * @date 25.02.2026
 */

#include "bldc.h"

static int8_t sign[12][3] = {
        {1, -1, -1},
        {1, -1, 0},
        {1, -1, 1},
        {0, -1, 1},
        {-1, -1, 1},
        {-1, 0, 1},
        {-1, 1, 1},
        {-1, 1, 0},
        {-1, 1, -1},
        {0, 1, -1},
        {1, 1, -1},
        {1, 0, -1}
#if 0
        {0, 1, -1},
        {-1, 1, 0},
        {-1, 0, 1},
        {0, -1, 1},
        {1, -1, 0},
        {1, 0, -1}
#endif
};



void BLDC_getspeed(bldc_t* bldc, TIM_HandleTypeDef *htim)
{
    // tick time [sec]
    float Ttick = (float)(htim->Instance->PSC + 1) / HAL_RCC_GetHCLKFreq();

    // time between 2 period elapsed callback calls [sec]
    float Toverflow = Ttick * (htim->Instance->ARR + 1);
    UNUSED(Toverflow);

    // time since last capture observed [sec]
    float t = Ttick * bldc->speedtracking.ccr;
    
    // rotation frequency [rotation / sec]
    bldc->speed = 1 / t / bldc->poles_number;
}


void BLDC_IC_speedtracking(bldc_t* bldc, TIM_HandleTypeDef *htim)
{
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
    {
        /*
         * Save ticks counted by timer.
         * Take into account possible timer overflow.
         */
        bldc->speedtracking.ccr = (uint64_t)(htim->Instance->CCR1) +
                bldc->speedtracking.overflow * (htim->Instance->ARR + 1);
        // Set timer counter to 0
        htim->Instance->CNT = 0;
        // Reset overflow counter
        bldc->speedtracking.overflow = 0;
    }
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
        // TODO: implement rotation direction handling
    }
    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
    {
        // TODO: implement rotation direction handling
    }
}

void BLDC_IC_setPWM(bldc_t* bldc, TIM_HandleTypeDef *htim_pwm)
{
    uint32_t max = (htim_pwm->Instance->ARR + 1);
    uint8_t state = bldc->pwm.state % 12;
    float ctrl = bldc->pwm.magnitude;

    htim_pwm->Instance->CCR1 = (uint32_t)((1 + sign[state][0] * ctrl) * max / 2);
    htim_pwm->Instance->CCR2 = (uint32_t)((1 + sign[state][1] * ctrl) * max / 2);
    htim_pwm->Instance->CCR3 = (uint32_t)((1 + sign[state][2] * ctrl) * max / 2);
}


