/*
 * starter.c
 *
 *  Created on: Jul 9, 2026
 *      Author: user
 */

#include "starter.h"
#include "tim.h"

float starter_speedup_inter[STARTER_SPEEDUP_INTER_NUM] = {
 0.0024726231566347743,
 0.0030037723162584648, 0.0036486013693163262, 0.004431242607258863,
 0.005380857228733976, 0.006532638796200481, 0.007928995754895156,
 0.009620934704788116, 0.011669663373616702, 0.014148425855294211,
 0.017144570773967492, 0.020761833060024427, 0.02512277865861266,
 0.03037131463680641, 0.03667510015791109, 0.044227602075155,
 0.0532494193146028, 0.06398835348420844, 0.07671753773785574,
 0.09173077357808615, 0.10933410756773208, 0.12983267390701375,
 0.15351202760257815, 0.18061370401278665, 0.21130565374336038,
 0.24564953176134968, 0.28356842975218577, 0.3248201803118195,
 0.368982270103351, 0.4154540400088802, 0.4634797867662746,
 0.5121927042988746, 0.5606751187484297, 0.6080265379340203,
 0.6534290371166286, 0.696200191934337, 0.7358268831179157,
 0.7719776934366042, 0.8044958413852503, 0.8333775210250712,
 0.8587416999834824, 0.8807970779778823
};

bldc_t starter = {
        .pwmtim = &htim8,
        .pwm_CCER_ch1 = (TIM_CCER_CC1E | TIM_CCER_CC1NE),
        .pwm_CCER_ch2 = (TIM_CCER_CC2E | TIM_CCER_CC2NE),
        .pwm_CCER_ch3 =(TIM_CCER_CC3E | TIM_CCER_CC3NE),
        .field_state = STATE_1,
        .control_mode_t = IDLE,
        .duty = STARTER_STARTUP_MINDUTY,
        .align.cnt = 0,
        .startup.cnt = 0,
        .idle.disabletim_flag = 1,
        .idle.run_flag = 0,
        .startup.finalspeed = STARTER_SPEEDUP_MAXSPEED,
        .closeloop.kp = STARTER_KP,
        .closeloop.ki = STARTER_KI,
        .closeloop.kd = STARTER_KD,
        .closeloop.target = STARTER_ARR_INITTARGET,
        .closeloop.cnt = 0,
        .filtspeed = 3000,
        .targetspeed = 5000
};

uint16_t aligndelay = STARTER_ALIGN_DELAY;
uint16_t prestartupdelay = STARTER_PRESTARTUP_DELAY;
uint16_t duty = STARTER_STARTUP_MINDUTY;

void Starter_Update()
{
    static uint32_t starter_ticks = 0;
    const static uint32_t starter_delta = 1;

    if(starter_ticks + starter_delta < HAL_GetTick())
    {
        starter_ticks += starter_delta;

        switch(starter.control_mode_t)
        {
        case IDLE:
            if(starter.idle.disabletim_flag)
            {
                HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_1);
                HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_2);
                HAL_TIM_IC_Stop_IT(&htim4, TIM_CHANNEL_1);
                HAL_TIM_IC_Stop_IT(&htim4, TIM_CHANNEL_2);
                HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_3);
                HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_4);

                HAL_TIM_PWM_Stop(starter.pwmtim, TIM_CHANNEL_1);
                HAL_TIMEx_PWMN_Stop(starter.pwmtim, TIM_CHANNEL_1);
                HAL_TIM_PWM_Stop(starter.pwmtim, TIM_CHANNEL_2);
                HAL_TIMEx_PWMN_Stop(starter.pwmtim, TIM_CHANNEL_2);
                HAL_TIM_PWM_Stop(starter.pwmtim, TIM_CHANNEL_3);
                HAL_TIMEx_PWMN_Stop(starter.pwmtim, TIM_CHANNEL_3);
                HAL_TIM_Base_Stop_IT(&htim7);
                starter.idle.disabletim_flag = 0;
            }
            if(starter.idle.run_flag)
            {
                HAL_TIM_PWM_Start(starter.pwmtim, TIM_CHANNEL_1);
                HAL_TIMEx_PWMN_Start(starter.pwmtim, TIM_CHANNEL_1);
                HAL_TIM_PWM_Start(starter.pwmtim, TIM_CHANNEL_2);
                HAL_TIMEx_PWMN_Start(starter.pwmtim, TIM_CHANNEL_2);
                HAL_TIM_PWM_Start(starter.pwmtim, TIM_CHANNEL_3);
                HAL_TIMEx_PWMN_Start(starter.pwmtim, TIM_CHANNEL_3);
                htim7.Instance->PSC = 5999;
                htim7.Instance->ARR = 5999;
                htim7.Instance->CNT = 0;
                starter.control_mode_t = ALIGN;
                starter.idle.disabletim_flag = 1;
                starter.field_state = STATE_1;
                starter.last_ccr = 7300;
                //starter.duty = STARTER_STARTUP_MINDUTY;
                starter.filtspeed = 3000;
                starter.targetspeed = 8000;
                starter.closeloop.needrestart_flag = 0;
                starter.speed = 0;
                starter.startup.cnt = 0;
                starter.closeloop.interr = 0;
                starter.closeloop.preverr = 0;
                starter.closeloop.prev2err = 0;
                starter.closeloop.err = 0;
                starter.closeloop.out = 0;
                starter.closeloop.cnt = 0;
                starter.usearr = 0;
                starter.closeloop.arr = STARTER_ARR_INITTARGET;
                starter.closeloop.target = STARTER_ARR_INITTARGET;
            }
            break;
        case ALIGN:
            if(!starter.idle.run_flag)
            {
                starter.control_mode_t = IDLE;
            }
            if(starter.align.cnt == 0)
            {
                starter.field_state = (starter.field_state + 1) % 6;
                starter.duty = duty - 30;
                BLDC_SetPWM(&starter);
            }
            if(starter.align.cnt == (uint16_t)(aligndelay / (float)starter_delta * 0.7))
            {
                starter.field_state = (starter.field_state + 1) % 6;
                starter.duty = duty;
                BLDC_SetPWM(&starter);
            }
            if(starter.align.cnt == (uint16_t)(aligndelay / (float)starter_delta * 0.9))
            {
                starter.field_state = (starter.field_state + 1) % 6;
                starter.duty = duty;
                BLDC_SetPWM(&starter);
            }
            if(starter.align.cnt == (uint16_t)(aligndelay / (float)starter_delta))
            {
                starter.field_state = (starter.field_state + 1) % 6;
                starter.duty = duty;
                BLDC_SetPWM(&starter);
            }
            BLDC_SetPWM(&starter);
            starter.align.cnt++;
            if(starter.align.cnt > aligndelay / (float)starter_delta)
            {
                starter.align.cnt = 0;
                starter.startup.speed = STARTER_SPEEDUP_MINSPEED;
                starter.startup.tmax = (STARTER_SPEEDUP_MAXSPEED - STARTER_SPEEDUP_MINSPEED) / (float)STARTER_SPEEDUP_ACCELERATION;
                starter.startup.t = 0;
                STARTER_SPEEDUP_SET_PSC(starter.startup.speed);
                starter.control_mode_t = PRESTARTUP;
                HAL_TIM_Base_Start_IT(&htim7);
            }
            break;
        case PRESTARTUP:
            if(!starter.idle.run_flag)
            {
                starter.control_mode_t = IDLE;
            }
            starter.align.cnt++;
            if(starter.align.cnt > prestartupdelay / (float)starter_delta)
            {
                starter.align.cnt = 0;
                starter.control_mode_t = STARTUP;
            }
            break;

        case STARTUP:
            if(!starter.idle.run_flag)
            {
                starter.control_mode_t = IDLE;
            }
            if(starter.startup.t < starter.startup.tmax)
            {
                //starter.startup.speed = (STARTER_SPEEDUP_MAXSPEED - STARTER_SPEEDUP_MINSPEED) * Starter_Speedup(starter.startup.t / starter.startup.tmax) + STARTER_SPEEDUP_MINSPEED;
                starter.startup.speed = STARTER_SPEEDUP_MINSPEED + STARTER_SPEEDUP_ACCELERATION * starter.startup.t;
                starter.startup.t += starter_delta / 1000.f;
                STARTER_SPEEDUP_SET_PSC(starter.startup.speed);
                starter.startup.cnt = 0;
            }
            else if(starter.startup.cnt > 5)
            {
                starter.startup.cnt = 0;
                HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
                HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
                HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
                HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
                HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3);
                HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);
                starter.control_mode_t = CLOSELOOP;
                starter.duty = 150;
            }
            starter.startup.cnt += 1;
            break;

        case CLOSELOOP:
            if((!starter.idle.run_flag) || starter.closeloop.needrestart_flag)
            {
                starter.closeloop.needrestart_flag = 0;
                starter.field_state = STATE_OFF;
                BLDC_SetPWM(&starter);
                starter.control_mode_t = IDLE;
            }

            starter.speed = 60.f / ((STARTER_IC_PSC + 1) * (float)starter.last_ccr / STARTER_TIM_FREQ * 6. * STARTER_MAGPAIRS);
            starter.filtspeed = 0.999 * starter.filtspeed + 0.001 * starter.speed;
            //starter.closeloop.arr = (uint32_t)(STARTER_TIM_FREQ / 6. / STARTER_MAGPAIRS / (htim7.Instance->PSC + 1) / starter.speed * 60. - 1);
#if 0
            if(pump.closeloop.cnt >= 100)
            {
                pump.usearr = 1;
                pump.closeloop.prev2err = pump.closeloop.preverr;
                pump.closeloop.preverr = pump.closeloop.err;
                pump.closeloop.err = pump.targetspeed - pump.filtspeed;
                pump.closeloop.interr += pump.closeloop.err * 100 * starter_delta / 1000.;
                pump.closeloop.differr = (pump.closeloop.err - 2 * pump.closeloop.preverr + pump.closeloop.prev2err) / (100 * starter_delta / 1000.);
                if(pump.closeloop.interr > 100) pump.closeloop.interr = 100;
                if(pump.closeloop.interr < -100) pump.closeloop.interr = -100;
                pump.closeloop.out = pump.closeloop.kp * pump.closeloop.err +
                                     pump.closeloop.ki * pump.closeloop.interr +
                                     pump.closeloop.kd * pump.closeloop.differr;
                pump.duty = DutyByTargetPump(pump.closeloop.target + pump.closeloop.out);
                pump.closeloop.target += pump.closeloop.out;
                pump.closeloop.cnt = 0;
            }
            pump.closeloop.cnt += 1;
#endif
            break;
        default:
            starter.control_mode_t = IDLE;
            break;
        }
    }
}

float Starter_Speedup(float t)
{
    {
        float res, delta = 1.f / (STARTER_SPEEDUP_INTER_NUM - 1);

        if(t <= 0)
        {
            res = starter_speedup_inter[0];
        } else if(t >= 1)
        {
            res = starter_speedup_inter[STARTER_SPEEDUP_INTER_NUM - 1];
        }

        else
        {
            size_t i = 0;
            while(t > i * delta)
            {
                i += 1;
            }

            // w = a * t + b
            float a = (starter_speedup_inter[i] - starter_speedup_inter[i - 1]) / delta;
            float b = (starter_speedup_inter[i - 1] * i * delta - starter_speedup_inter[i] * (i - 1) * delta) / delta;

            res = a * t + b;
        }
        return res;
    }
}
