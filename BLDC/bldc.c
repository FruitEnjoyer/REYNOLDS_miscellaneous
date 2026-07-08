/**
 * @file bldc.c
 * @author ruslan
 * @brief 
 * @date 25.02.2026
 */

#include "bldc.h"

float speedup_inter[BLDC_SPEEDUP_INTER_NUM] = {
/*0.0024726231566347743,
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
 0.8587416999834824, 0.8807970779778823 */
0.0017729545947921428, 0.002154131104081375, 0.0026170438357934036, 0.003179117421165398, 0.0038614425580054414, 0.00468952429246947, 0.005694172066397154,
        0.006912553101095241, 0.00838943129708398, 0.0101786128289911, 0.0123446159700346, 0.01496457490818598, 0.018130373354652174, 0.021950980827615037,
        0.02655492906573788, 0.032092813790665836, 0.0387396332731304, 0.04669667552437347, 0.0561925381024402, 0.0674827109955109, 0.08084698553037946,
        0.09658379758306813, 0.11500052008558426, 0.13639876291493977, 0.16105401695954877, 0.18918960025591283, 0.2209459025257257, 0.2563473588421691,
        0.29527121929156586, 0.33742360216461964, 0.38232892018305953, 0.42933794056115027, 0.477657165722823, 0.5263981951626329, 0.5746412513676964,
        0.621503557088486, 0.6662020005138252, 0.7081009948172341, 0.7467400711819927, 0.7818402365299438, 0.8132920616370185, 0.8411308951190849 };

static int16_t pump_targets[26] = {
        3900, 3000, 2400, 1950, 1650,
        1475, 1400, 1200, 950, 900,
        840, 630, 590, 490, 400,
        290, 200, 140, 100, 10,
        -50, -70, -100, -190, -250,
        -300
};
static uint16_t pump_duties[26] = {
        150, 190, 230, 270, 310,
        350, 390, 400, 430, 450,
        470, 490, 500, 520, 540,
        560, 580, 600, 620, 640,
        660, 680, 700, 740, 780,
        800
};

uint16_t DutyByTargetPump(float target)
{
    if(target >= pump_targets[0])
    {
        return pump_duties[0];
    }
    else if(target <= pump_targets[25])
    {
        return pump_duties[25];
    }
    else
    {
        size_t i = 0;
        while(target < pump_targets[i])
        {
            i += 1;
        }

        // w = a * t + b
        float a = (pump_duties[i] - pump_duties[i - 1]) / (float)(pump_targets[i] - pump_targets[i - 1]);
        float b = (pump_duties[i - 1] * pump_targets[i] - pump_duties[i] * pump_targets[i - 1]) / (float)(pump_targets[i] - pump_targets[i - 1]);

        return (uint16_t)(a * target + b);
    }
}


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

void BLDC_CalcSpeed(bldc_t *bldc)
{
    bldc->speed = (float)(HAL_RCC_GetPCLK1Freq() * 60.0f) / (bldc->last_ccr * 6.f * bldc->pole_number * (bldc->ictim->Instance->PSC + 1.f));
    //pump.speed = 60. * 1 / ((float)pump.last_ccr / TIM_FREQ * 6. * BLDC_MAGPAIRS);
    if(bldc->state_dir_t == REVERSE)
    {
        bldc->speed *= -1;
    }
}

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

float BLDC_Speedup(float t)
{
    float res, delta = 1.f / (BLDC_SPEEDUP_INTER_NUM - 1);

    if(t <= 0)
    {
        res = speedup_inter[0];
    } else if(t >= 1)
    {
        res = speedup_inter[BLDC_SPEEDUP_INTER_NUM - 1];
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
