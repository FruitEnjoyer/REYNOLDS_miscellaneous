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
        .filtspeed = 3000,
        .targetspeed = 0
};

static float speedup_inter[PUMP_SPEEDUP_INTER_NUM] = {
        0.0017729545947921428, 0.002154131104081375, 0.0026170438357934036, 0.003179117421165398, 0.0038614425580054414,
        0.00468952429246947, 0.005694172066397154, 0.006912553101095241, 0.00838943129708398, 0.0101786128289911,
        0.0123446159700346, 0.01496457490818598, 0.018130373354652174, 0.021950980827615037, 0.02655492906573788,
        0.032092813790665836, 0.0387396332731304, 0.04669667552437347, 0.0561925381024402, 0.0674827109955109,
        0.08084698553037946, 0.09658379758306813, 0.11500052008558426, 0.13639876291493977, 0.16105401695954877,
        0.18918960025591283, 0.2209459025257257, 0.2563473588421691, 0.29527121929156586, 0.33742360216461964,
        0.38232892018305953, 0.42933794056115027, 0.477657165722823, 0.5263981951626329, 0.5746412513676964,
        0.621503557088486, 0.6662020005138252, 0.7081009948172341, 0.7467400711819927, 0.7818402365299438,
        0.8132920616370185, 0.8411308951190849
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
        if(pump.targetspeed >= 150)
        {
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
            HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
            htim6.Instance->PSC = 5999;
            htim6.Instance->ARR = 5999;
            htim6.Instance->CNT = 0;
            pump.control_mode_t = ALIGN;
            pump.idle.disabletim_flag = 1;
            pump.field_state = STATE_1;
            pump.last_ccr = 100000;
            pump.duty = PUMP_STARTUP_MINDUTY;
            pump.filtspeed = 3000;
            //pump.targetspeed = 200;
            pump.closeloop.needrestart_flag = 0;
            pump.speed = 0;
            pump.startup.cnt = 0;
            pump.closeloop.interr = 0;
            pump.closeloop.preverr = 0;
            pump.closeloop.prev2err = 0;
            pump.closeloop.err = 0;
            pump.closeloop.out = 0;
            pump.closeloop.cnt = 0;
            pump.usearr = 0;
            pump.closeloop.arr = PUMP_ARR_INITTARGET;
            pump.closeloop.target = PUMP_ARR_INITTARGET;
        }
        break;
    case ALIGN:
        if(pump.targetspeed < 150)
        {
            pump.control_mode_t = IDLE;
        }
        BLDC_SetPWM(&pump);
        pump.align.cnt++;
        if(pump.align.cnt > PUMP_ALIGN_DELAY / (float)bldc_delta)
        {
            pump.align.cnt = 0;
            pump.startup.speed = PUMP_SPEEDUP_MINSPEED;
            pump.startup.tmax = (PUMP_SPEEDUP_MAXSPEED - PUMP_SPEEDUP_MINSPEED) / (float)PUMP_SPEEDUP_ACCELERATION;
            pump.startup.t = 0;
            PUMP_SPEEDUP_SET_PSC(pump.startup.speed);
            pump.control_mode_t = PRESTARTUP;
            HAL_TIM_Base_Start_IT(&htim6);
        }
        break;
    case PRESTARTUP:
        if(pump.targetspeed < 150)
        {
            pump.control_mode_t = IDLE;
        }
        pump.align.cnt++;
        if(pump.align.cnt > PUMP_PRESTARTUP_DELAY / (float)bldc_delta)
        {
            pump.align.cnt = 0;
            pump.control_mode_t = STARTUP;
        }
        break;

    case STARTUP:
        if(pump.targetspeed < 150)
        {
            pump.control_mode_t = IDLE;
        }

        if(pump.startup.t < pump.startup.tmax)
        {
            pump.startup.speed = (PUMP_SPEEDUP_MAXSPEED - PUMP_SPEEDUP_MINSPEED) * Pump_Speedup(pump.startup.t / pump.startup.tmax) + PUMP_SPEEDUP_MINSPEED;
            pump.startup.t += bldc_delta / 1000.f;
            PUMP_SPEEDUP_SET_PSC(pump.startup.speed);
            pump.startup.cnt = 0;
        } else if(pump.startup.cnt > 2)
        {
            HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
            HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
            HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
            HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_2);
            HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
            HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4);
            if(pump.startup.cnt > 10)
            {
                pump.control_mode_t = CLOSELOOP;
                pump.startup.cnt = 0;
                HAL_TIM_Base_Stop_IT(&htim6);
            }
        }
        pump.startup.cnt += 1;
        break;

    case CLOSELOOP:
        if(pump.closeloop.needrestart_flag || pump.targetspeed < 150)
        {
            pump.closeloop.needrestart_flag = 0;
            pump.field_state = STATE_OFF;
            BLDC_SetPWM(&pump);
            pump.control_mode_t = IDLE;
        }

        pump.speed = 60. * 1 / ((float)pump.last_ccr / PUMP_TIM_FREQ * 6. * PUMP_MAGPAIRS);
        pump.filtspeed = 0.99 * pump.filtspeed + 0.01 * pump.speed;

        if(pump.closeloop.cnt >= 10)
        {
            if(pump.targetspeed - pump.duty > 50)
            {
                pump.duty += 50;
            }
            else if(pump.targetspeed - pump.duty < -50)
            {
                if(pump.duty - 50 > 150) pump.duty -= 50;
                else pump.duty = 150;
            }
            else
            {
                pump.duty = pump.targetspeed;
            }
            pump.closeloop.cnt = 0;
        }
        pump.closeloop.cnt += 1;
        break;

    default:
        pump.control_mode_t = IDLE;
        break;
    }
}

float Pump_Speedup(float t)
{
    float res, delta = 1.f / (PUMP_SPEEDUP_INTER_NUM - 1);

    if(t <= 0)
    {
        res = speedup_inter[0];
    } else if(t >= 1)
    {
        res = speedup_inter[PUMP_SPEEDUP_INTER_NUM - 1];
    }

    else
    {
        size_t i = 0;
        while(t > i * delta)
        {
            i += 1;
        }

        // w = a * t + b
        float a = (speedup_inter[i] - speedup_inter[i - 1]) / delta;
        float b = (speedup_inter[i - 1] * i * delta - speedup_inter[i] * (i - 1) * delta) / delta;

        res = a * t + b;
    }
    return res;
}

void Pump_SetDuty(int32_t duty)
{
    if(duty > 800) duty = 800;
    else if(duty < 0) duty = 0;
    pump.targetspeed = duty;
}
