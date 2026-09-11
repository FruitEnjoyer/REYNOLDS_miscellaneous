/**
 * @file bldc.c
 * @author ruslan
 * @brief 
 * @date 25.02.2026
 */

#include "bldc.h"


void BLDC_SetPWM(bldc_t *bldc)
{
    /*
     *            A     B     C
     * State1:   HIGH  LOW   OFF
     * State2:   HIGH  OFF   LOW
     * State3:   OFF   HIGH  LOW
     * State4:   LOW   HIGH  OFF
     * State5:   LOW   OFF   HIGH
     * State6:   OFF   LOW   HIGH
     *
     * StateOFF: OFF   OFF   OFF
     */
    switch(bldc->field_state)
    {
    case STATE_OFF:
        bldc->pwmtim->Instance->CCR1 = 0;
        bldc->pwmtim->Instance->CCR2 = 0;
        bldc->pwmtim->Instance->CCR3 = 0;
        bldc->pwmtim->Instance->CCER &= ~(bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch2 | bldc->pwm_CCER_ch3);
        break;
    case STATE_1:
        bldc->pwmtim->Instance->CCER |= (bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch2);
        bldc->pwmtim->Instance->CCER &= ~(bldc->pwm_CCER_ch3);
        bldc->pwmtim->Instance->CCR1 = bldc->duty;
        bldc->pwmtim->Instance->CCR2 = 0;
        //bldc->IC_TIM->
        break;
    case STATE_2:
        bldc->pwmtim->Instance->CCER |= (bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch3);
        bldc->pwmtim->Instance->CCER &= ~(bldc->pwm_CCER_ch2);
        bldc->pwmtim->Instance->CCR1 = bldc->duty;
        bldc->pwmtim->Instance->CCR3 = 0;
        break;
    case STATE_3:
        bldc->pwmtim->Instance->CCER |= (bldc->pwm_CCER_ch2 | bldc->pwm_CCER_ch3);
        bldc->pwmtim->Instance->CCER &= ~(bldc->pwm_CCER_ch1);
        bldc->pwmtim->Instance->CCR2 = bldc->duty;
        bldc->pwmtim->Instance->CCR3 = 0;
        break;
    case STATE_4:
        bldc->pwmtim->Instance->CCER |= (bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch2);
        bldc->pwmtim->Instance->CCER &= ~(bldc->pwm_CCER_ch3);
        bldc->pwmtim->Instance->CCR1 = 0;
        bldc->pwmtim->Instance->CCR2 = bldc->duty;
        break;
    case STATE_5:
        bldc->pwmtim->Instance->CCER |= (bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch3);
        bldc->pwmtim->Instance->CCER &= ~(bldc->pwm_CCER_ch2);
        bldc->pwmtim->Instance->CCR1 = 0;
        bldc->pwmtim->Instance->CCR3 = bldc->duty;
        break;
    case STATE_6:
        bldc->pwmtim->Instance->CCER |= (bldc->pwm_CCER_ch2 | bldc->pwm_CCER_ch3);
        bldc->pwmtim->Instance->CCER &= ~(bldc->pwm_CCER_ch1);
        bldc->pwmtim->Instance->CCR2 = 0;
        bldc->pwmtim->Instance->CCR3 = bldc->duty;
        break;
    default:
        break;
    }
}
#if 0
void BLDC_SetCtrl(bldc_t *bldc, float ctrl)
{
    uint16_t duty = (uint16_t)(ctrl * (bldc->pwmtim->Instance->ARR + 1));

    if(bldc->state_dir_t == FORWARD)
    {
        bldc->duty = duty;
    } else
    {
        bldc->duty = 0;
    }
}
#endif


#if 0
void BLDC_Restart(bldc_t *bldc)
{
    static uint32_t bldc_restart_ticks = 0;
    static uint32_t bldc_restart_delta = 4;

    if(bldc_restart_ticks + bldc_restart_delta < HAL_GetTick() && bldc->control_mode_t == ALIGN)
    {
        if(bldc->state_dir_t == FORWARD)
        {
            bldc->field_state = (bldc->field_state + 1) % 6;
        } else if(bldc->state_dir_t == REVERSE)
        {
            bldc->field_state = (bldc->field_state + 6 - 1) % 6;
        }
        BLDC_SetPWM(bldc);
        bldc_restart_ticks += bldc_restart_delta;
    }
}
#endif


