/**
 * @file bldc.c
 * @author ruslan
 * @brief 
 * @date 25.02.2026
 */

#include "bldc.h"

void BLDC_Configure(bldc_t* bldc)
{
    // Disable IC timer
    bldc->IC_TIM->CR1 &= ~TIM_CR1_CEN;
    // Disable PWM timer
    bldc->PWM_TIM->CR1 &= ~TIM_CR1_CEN;

    // TODO: перенастроить делители на указанную частоту шим
    // TODO: сконфигурировать таймеры если нужно

    bldc->field_state = STATE_OFF;
    bldc->duty1 = 0;
    bldc->duty2 = 0;

    // Enable IC timer interrupts
    bldc->IC_TIM->DIER |= (TIM_DIER_UIE | TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_CC3IE);
    // Enable IC timer channels
    bldc->IC_TIM->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E);
    // Set IC timer counter to zero
    bldc->IC_TIM->CNT = 0;
    // Set IC timer ARR register to init state
    bldc->IC_TIM->ARR = 400000 - 1;

    // Set MOE bits
    bldc->PWM_TIM->BDTR |= TIM_BDTR_MOE;
}

void BLDC_Start(bldc_t* bldc)
{
    // Set default duties & enable/disable channels
    BLDC_SetCtrl(bldc, BLDC_DEFAULTCTRL);
    BLDC_SetPWM(bldc);

    // Enable IC timer
    bldc->IC_TIM->CR1 |= TIM_CR1_CEN;
    // Enable PWM timer
    bldc->PWM_TIM->CR1 |= TIM_CR1_CEN;
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
    uint16_t duty = (uint16_t)(ctrl * (bldc->PWM_TIM->ARR + 1));

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
