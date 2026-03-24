/**
 * @file bldc.c
 * @author ruslan
 * @brief 
 * @date 25.02.2026
 */

#include "bldc.h"
#include <math.h>
#include "LowPassFilter/lowpassfilter.h"
#include "PID/PID.h"

static lpfilter_t speedfilter;


void BLDC_Configure(bldc_t* bldc)
{
    LPF_init(&speedfilter, BLACKMAN);

    bldc->duty = 0;
    bldc->field_state = STATE_OFF;
    bldc->last_ccr = 0xFFFFFFFF;
    bldc->control_mode = OPENLOOP;
    bldc->state_dir = FORWARD;
    bldc->catched_interrupt = NO_INTERRUPT;
}

void BLDC_Start(bldc_t* bldc)
{
    // Set default duties & enable/disable channels
    BLDC_SetCtrl(bldc, BLDC_OPENLOOPCTRL);
    BLDC_SetPWM(bldc);

    HAL_TIM_PWM_Start(bldc->pwmtim, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(bldc->pwmtim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(bldc->pwmtim, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(bldc->pwmtim, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(bldc->pwmtim, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(bldc->pwmtim, TIM_CHANNEL_3);

    __HAL_TIM_SET_COUNTER(bldc->statetim, 0);
    HAL_TIM_Base_Start_IT(bldc->statetim);
#if 1
    HAL_TIM_IC_Start_IT(bldc->ictim1, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(bldc->ictim1, TIM_CHANNEL_2);
    HAL_TIM_IC_Start_IT(bldc->ictim1, TIM_CHANNEL_3);
    HAL_TIM_IC_Start_IT(bldc->ictim1, TIM_CHANNEL_4);
    HAL_TIM_IC_Start_IT(bldc->ictim2, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(bldc->ictim2, TIM_CHANNEL_2);
    __HAL_TIM_ENABLE_IT(bldc->ictim1, TIM_IT_UPDATE);
    __HAL_TIM_ENABLE_IT(bldc->ictim2, TIM_IT_UPDATE);
#endif
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
        bldc->pwmtim->Instance->CCR1 = bldc->duty;
        bldc->pwmtim->Instance->CCR2 = 0;
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

void BLDC_SetCtrl(bldc_t* bldc, float ctrl)
{
    if(ctrl > 0.5)
        ctrl = 0.5;
    if(ctrl < 0)
        ctrl = 0;

    uint16_t new_duty = (uint16_t)(fabsf(ctrl) * (bldc->pwmtim->Instance->ARR + 1));

    bldc->duty = new_duty;
}

void BLDC_CalcSpeed(bldc_t* bldc)
{
    bldc->speed = (float)(2 * HAL_RCC_GetPCLK1Freq() * 60.0f) / (bldc->last_ccr * 6.f * bldc->pole_number * (bldc->ictim1->Instance->PSC + 1.f));
}

void BLDC_Execute(bldc_t* bldc)
{
    switch(bldc->control_mode)
    {
    case IDLE:
        BLDC_IdleExecute(bldc);
        break;
    case START:
        BLDC_StartExecute(bldc);
    case OPENLOOP:
        BLDC_OpenLoopExecute(bldc);
        break;
    case CLOSELOOP:
        BLDC_CloseLoopExecute(bldc);
        break;
    }
}

void BLDC_IdleExecute(bldc_t* bldc)
{
    BLDC_SetCtrl(bldc, 0);

    HAL_TIM_PWM_Stop(bldc->pwmtim, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Stop(bldc->pwmtim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(bldc->pwmtim, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Stop(bldc->pwmtim, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(bldc->pwmtim, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Stop(bldc->pwmtim, TIM_CHANNEL_3);

    HAL_TIM_IC_Stop_IT(bldc->ictim1, TIM_CHANNEL_1);
    HAL_TIM_IC_Stop_IT(bldc->ictim1, TIM_CHANNEL_2);
    HAL_TIM_IC_Stop_IT(bldc->ictim1, TIM_CHANNEL_3);
    HAL_TIM_IC_Stop_IT(bldc->ictim1, TIM_CHANNEL_4);
    HAL_TIM_IC_Stop_IT(bldc->ictim2, TIM_CHANNEL_1);
    HAL_TIM_IC_Stop_IT(bldc->ictim2, TIM_CHANNEL_2);
    __HAL_TIM_DISABLE_IT(bldc->ictim1, TIM_IT_UPDATE);
    __HAL_TIM_DISABLE_IT(bldc->ictim2, TIM_IT_UPDATE);
}

void BLDC_StartExecute(bldc_t* bldc)
{
    static uint32_t counter = 0, limit = 50000;

    bldc->field_state = STATE_1;
    bldc->catched_interrupt = NO_INTERRUPT;

    // Set default duties & enable/disable channels
    BLDC_SetCtrl(bldc, BLDC_OPENLOOPCTRL);
    BLDC_SetPWM(bldc);

    HAL_TIM_PWM_Start(bldc->pwmtim, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(bldc->pwmtim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(bldc->pwmtim, TIM_CHANNEL_2);
    HAL_TIMEx_PWMN_Start(bldc->pwmtim, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(bldc->pwmtim, TIM_CHANNEL_3);
    HAL_TIMEx_PWMN_Start(bldc->pwmtim, TIM_CHANNEL_3);

    // Wait some time to keep rotor in start position
    if(counter < limit)
    {
        __NOP();
        counter += 1;
    }
    else // Start rotation
    {
        counter = 0;
        bldc->control_mode = OPENLOOP;
    }
}

void BLDC_OpenLoopExecute(bldc_t* bldc)
{
    static uint32_t counter = 0, limit = 6000;

    if(counter < limit)
    {
        __NOP();
        counter += 1;
    }
    else
    {
        bldc->field_state = (bldc->field_state + 1) % 6;
        BLDC_SetPWM(bldc);
        counter = 0;
        limit -= 20;
    }
    if(limit < 4000)
    {
        HAL_TIM_IC_Start_IT(bldc->ictim1, TIM_CHANNEL_1);
        HAL_TIM_IC_Start_IT(bldc->ictim1, TIM_CHANNEL_2);
        HAL_TIM_IC_Start_IT(bldc->ictim1, TIM_CHANNEL_3);
        HAL_TIM_IC_Start_IT(bldc->ictim1, TIM_CHANNEL_4);
        HAL_TIM_IC_Start_IT(bldc->ictim2, TIM_CHANNEL_1);
        HAL_TIM_IC_Start_IT(bldc->ictim2, TIM_CHANNEL_2);
        __HAL_TIM_ENABLE_IT(bldc->ictim1, TIM_IT_UPDATE);
        __HAL_TIM_ENABLE_IT(bldc->ictim2, TIM_IT_UPDATE);
    }
    if(limit < 2500)
    {
        limit = 6000;
        bldc->control_mode = CLOSELOOP;
    }
}
void BLDC_CloseLoopExecute(bldc_t* bldc)
{
    static float ctrl = BLDC_OPENLOOPCTRL;
    static PID_controller_t pid = {
            .kp = 0.00001,
            .ki = 0,
            .kd = 0,
            .preverr = 0,
            .integral = 0,
            .out = 0
    };

    if(bldc->catched_interrupt == NO_INTERRUPT)
    {
        ctrl = BLDC_OPENLOOPCTRL;
        HAL_TIM_IC_Stop_IT(bldc->ictim1, TIM_CHANNEL_1);
        HAL_TIM_IC_Stop_IT(bldc->ictim1, TIM_CHANNEL_2);
        HAL_TIM_IC_Stop_IT(bldc->ictim1, TIM_CHANNEL_3);
        HAL_TIM_IC_Stop_IT(bldc->ictim1, TIM_CHANNEL_4);
        HAL_TIM_IC_Stop_IT(bldc->ictim2, TIM_CHANNEL_1);
        HAL_TIM_IC_Stop_IT(bldc->ictim2, TIM_CHANNEL_2);
        __HAL_TIM_DISABLE_IT(bldc->ictim1, TIM_IT_UPDATE);
        __HAL_TIM_DISABLE_IT(bldc->ictim2, TIM_IT_UPDATE);
        bldc->control_mode = START;
    }
    else
    {

    }
}
