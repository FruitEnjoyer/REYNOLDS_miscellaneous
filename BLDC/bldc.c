/**
 * @file bldc.c
 * @author ruslan
 * @brief 
 * @date 25.02.2026
 */

#include "bldc.h"

void BLDC_Configure(bldc_t* bldc)
{
    // TODO: перенастроить делители на указанную частоту шим
    // TODO: сконфигурировать таймеры если нужно

    bldc->field_state = STATE_OFF;
    bldc->duty1 = 0;
    bldc->duty2 = 0;

    BLDC_SetPWM(bldc);
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
        bldc->PWM_TIM->CCER &= ~(bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch2 | bldc->pwm_CCER_ch3);
        break;
    case STATE_1:
        bldc->PWM_TIM->CCER |= (bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch2);
        bldc->PWM_TIM->CCER &= ~(bldc->pwm_CCER_ch3);
        bldc->PWM_TIM->CCR1 = bldc->duty1;
        bldc->PWM_TIM->CCR2 = bldc->duty2;
        break;
    case STATE_2:
        bldc->PWM_TIM->CCER |= (bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch3);
        bldc->PWM_TIM->CCER &= ~(bldc->pwm_CCER_ch2);
        bldc->PWM_TIM->CCR1 = bldc->duty1;
        bldc->PWM_TIM->CCR3 = bldc->duty2;
        break;
    case STATE_3:
        bldc->PWM_TIM->CCER |= (bldc->pwm_CCER_ch2 | bldc->pwm_CCER_ch3);
        bldc->PWM_TIM->CCER &= ~(bldc->pwm_CCER_ch1);
        bldc->PWM_TIM->CCR2 = bldc->duty1;
        bldc->PWM_TIM->CCR3 = bldc->duty2;
        break;
    case STATE_4:
        bldc->PWM_TIM->CCER |= (bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch2);
        bldc->PWM_TIM->CCER &= ~(bldc->pwm_CCER_ch3);
        bldc->PWM_TIM->CCR1 = bldc->duty2;
        bldc->PWM_TIM->CCR2 = bldc->duty1;
        break;
    case STATE_5:
        bldc->PWM_TIM->CCER |= (bldc->pwm_CCER_ch1 | bldc->pwm_CCER_ch3);
        bldc->PWM_TIM->CCER &= ~(bldc->pwm_CCER_ch2);
        bldc->PWM_TIM->CCR1 = bldc->duty2;
        bldc->PWM_TIM->CCR3 = bldc->duty1;
        break;
    case STATE_6:
        bldc->PWM_TIM->CCER |= (bldc->pwm_CCER_ch2 | bldc->pwm_CCER_ch3);
        bldc->PWM_TIM->CCER &= ~(bldc->pwm_CCER_ch1);
        bldc->PWM_TIM->CCR2 = bldc->duty2;
        bldc->PWM_TIM->CCR3 = bldc->duty1;
        break;
    default:
        break;
    }
}

void BLDC_SetCtrl(bldc_t* bldc, float ctrl)
{
    //uint16_t duty = (bldc->ctrl_magnitude)(bldc->PWM_TIM->ARR + 1);

    if(ctrl >= 0)
    {

    }
    else
    {

    }
}
