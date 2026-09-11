/*
 * pump.c
 *
 *  Created on: Jul 9, 2026
 *      Author: user
 */


#include "pump.h"
#include "tim.h"

bldc_t pump = {
        .pwmtim = &htim1,
        .pwm_CCER_ch1 = (TIM_CCER_CC1E | TIM_CCER_CC1NE),
        .pwm_CCER_ch2 = (TIM_CCER_CC2E | TIM_CCER_CC2NE),
        .pwm_CCER_ch3 =(TIM_CCER_CC3E | TIM_CCER_CC3NE),
        .field_state = STATE_1,
        .control_mode_t = IDLE,
        .duty = PUMP_STARTUP_MINDUTY,
        .align.cnt = 0,
        .startup.cnt = 0,
        .idle.disabletim_flag = 1,
        .idle.run_flag = 0,
        .startup.finalspeed = PUMP_SPEEDUP_MAXSPEED,
        .closeloop.target = PUMP_ARR_INITTARGET,
        .closeloop.cnt = 0,
        .targetspeed = 0
};


void Pump_Update()
{
    const static uint32_t bldc_delta = 1;

    switch(pump.control_mode_t)
    {
    case IDLE:
        if(pump.idle.disabletim_flag)
        {
            HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);
            HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_2);
            HAL_TIM_IC_Stop_IT(&htim5, TIM_CHANNEL_1);
            HAL_TIM_IC_Stop_IT(&htim5, TIM_CHANNEL_2);
            HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_3);
            HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_4);

            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
            HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_1);
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
            HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_2);
            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
            HAL_TIMEx_PWMN_Stop(&htim1, TIM_CHANNEL_3);
            HAL_TIM_Base_Stop_IT(&htim6);
            pump.idle.disabletim_flag = 0;
        }
        if(pump.idle.run_flag)
        {
            pump.idle.disabletim_flag = 1;
            pump.field_state = STATE_1;
            pump.last_ccr = 100000;
            pump.duty = pump.targetspeed;
            pump.speed = 0;
            pump.speed_cnt = 0;
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
            BLDC_SetPWM(&pump);
            HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
            HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
            HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
            HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_2);
            HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
            HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);
            __HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE);
            pump.control_mode_t = CLOSELOOP;
        }
        break;

    case ALIGN:
        break;
    case PRESTARTUP:
        break;

    case STARTUP:
        break;

    case CLOSELOOP:
        if(!pump.idle.run_flag)
        {
            pump.field_state = STATE_OFF;
            BLDC_SetPWM(&pump);
            pump.control_mode_t = IDLE;
        }

        //pump.speed = 60. * 1 / ((float)pump.last_ccr / PUMP_TIM_FREQ * 6. * PUMP_MAGPAIRS);
        //pump.filtspeed = 0.99 * pump.filtspeed + 0.01 * pump.speed;
        //pump.intspeed = (uint16_t)pump.filtspeed;

        if(pump.targetspeed - pump.duty > 5)
        {
            pump.duty += 5;
        }
        else if(pump.targetspeed - pump.duty < -5)
        {
            if(pump.duty > PUMP_CLOSELOOP_MINDUTY + 5) pump.duty -= 5;
            else pump.duty = PUMP_CLOSELOOP_MINDUTY;
        }
        else
        {
            pump.duty = pump.targetspeed;
        }
        break;

    default:
        pump.control_mode_t = IDLE;
        break;
    }
}

void Pump_SetDuty(int32_t duty)
{
    if(duty > 800) duty = 800;
    else if(duty < 0) duty = 0;
    pump.targetspeed = duty;
}
