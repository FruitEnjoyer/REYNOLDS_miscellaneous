/**
 * @file bldc.c
 * @author ruslan
 * @brief 
 * @date 25.02.2026
 */

#include "bldc.h"

void BLDC_Configure(bldc_t* bldc)
{
    bldc->duty1 = 0;
    bldc->duty2 = 0;
    bldc->field_state = STATE_OFF;
    bldc->last_ccr = 0xFFFFFFFF;
    bldc->control_mode = MANUAL;
    bldc->state_dir = REVERSE;
}

void BLDC_Start(bldc_t* bldc)
{
    // Set default duties & enable/disable channels
    BLDC_SetCtrl(bldc, BLDC_DEFAULTCTRL);
    BLDC_SetPWM(bldc);

    HAL_TIM_PWM_Start(bldc->pwmtim, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(bldc->pwmtim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(bldc->pwmtim, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(bldc->pwmtim, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(bldc->pwmtim, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(bldc->pwmtim, TIM_CHANNEL_3);

    HAL_TIM_IC_Start_IT(bldc->ictim, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(bldc->ictim, TIM_CHANNEL_2);
    HAL_TIM_IC_Start_IT(bldc->ictim, TIM_CHANNEL_3);
    __HAL_TIM_ENABLE_IT(bldc->ictim, TIM_IT_UPDATE);
}



void BLDC_SetPWM(bldc_t* bldc)
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
        bldc->pwmtim->Instance->CCER &= ~(bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch2 | bldc->pwm_CCER_ch3);
        break;
    case STATE_1:
        bldc->pwmtim->Instance->CCER |= (bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch2);
        bldc->pwmtim->Instance->CCER &= ~(bldc->pwm_CCER_ch3);
        bldc->pwmtim->Instance->CCR1 = bldc->duty1;
        bldc->pwmtim->Instance->CCR2 = bldc->duty2;
        //bldc->IC_TIM->
        break;
    case STATE_2:
        bldc->pwmtim->Instance->CCER |= (bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch3);
        bldc->pwmtim->Instance->CCER &= ~(bldc->pwm_CCER_ch2);
        bldc->pwmtim->Instance->CCR1 = bldc->duty1;
        bldc->pwmtim->Instance->CCR3 = bldc->duty2;
        break;
    case STATE_3:
        bldc->pwmtim->Instance->CCER |= (bldc->pwm_CCER_ch2 | bldc->pwm_CCER_ch3);
        bldc->pwmtim->Instance->CCER &= ~(bldc->pwm_CCER_ch1);
        bldc->pwmtim->Instance->CCR2 = bldc->duty1;
        bldc->pwmtim->Instance->CCR3 = bldc->duty2;
        break;
    case STATE_4:
        bldc->pwmtim->Instance->CCER |= (bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch2);
        bldc->pwmtim->Instance->CCER &= ~(bldc->pwm_CCER_ch3);
        bldc->pwmtim->Instance->CCR1 = bldc->duty2;
        bldc->pwmtim->Instance->CCR2 = bldc->duty1;
        break;
    case STATE_5:
        bldc->pwmtim->Instance->CCER |= (bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch3);
        bldc->pwmtim->Instance->CCER &= ~(bldc->pwm_CCER_ch2);
        bldc->pwmtim->Instance->CCR1 = bldc->duty2;
        bldc->pwmtim->Instance->CCR3 = bldc->duty1;
        break;
    case STATE_6:
        bldc->pwmtim->Instance->CCER |= (bldc->pwm_CCER_ch2 | bldc->pwm_CCER_ch3);
        bldc->pwmtim->Instance->CCER &= ~(bldc->pwm_CCER_ch1);
        bldc->pwmtim->Instance->CCR2 = bldc->duty2;
        bldc->pwmtim->Instance->CCR3 = bldc->duty1;
        break;
    default:
        break;
    }
}

void BLDC_SetCtrl(bldc_t* bldc, float ctrl)
{
    uint16_t duty = (uint16_t)(ctrl * (bldc->pwmtim->Instance->ARR + 1));

    if(bldc->state_dir == FORWARD)
    {
        bldc->duty1 = duty;
        bldc->duty2 = 0;
    }
    else
    {
        bldc->duty1 = 0;
        bldc->duty2 = duty;
    }
}

void BLDC_CalcSpeed(bldc_t* bldc)
{
    bldc->speed = (float)(HAL_RCC_GetPCLK1Freq() * 60.0f) / (bldc->last_ccr * 6.f * bldc->pole_number * (bldc->ictim->Instance->PSC + 1.f));
}
