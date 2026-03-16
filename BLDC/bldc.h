/**
 * @file bldc.h
 * @author ruslan
 * @brief 
 * @date 25.02.2026
 */

#ifndef BLDC_H_
#define BLDC_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "main.h"
#include <stdint.h>

#define BLDC_DEFAULTCTRL    (0.1105f)
#define BLDC_SPEEDTHRESHOLD (10) // threshold between manual & interrupt control modes [revolutions per second]

typedef struct bldc{
    // Motor characteristics
    const uint8_t pole_number; // Number of rotor magnetic poles
    const float KV; // Back-EMF constant
    float pwm_freq; //

    // Power variables
    enum {
        STATE_OFF = -1,
        STATE_1 = 0, STATE_2,
        STATE_3, STATE_4,
        STATE_5, STATE_6
    } field_state; // Stator field state
    TIM_HandleTypeDef* pwmtim;
    TIM_HandleTypeDef* ictim;
    volatile uint32_t last_ccr;
    float speed;
    volatile uint32_t duty1, duty2;
    const uint32_t pwm_CCER_ch1, pwm_CCER_ch2, pwm_CCER_ch3;
    uint32_t ic_freq;
    enum state_dir{
        FORWARD,
        REVERSE
    } state_dir_t;
    enum control_mode{
        INTERRUPT = 0,
        MANUAL
    } control_mode_t;
    uint32_t manual_ticksdelta, ctrl_ticksdelta;
    //float ctrl; // Magnitude of PWM-ON state (from -1 to 1)

    // BEMF variables
    uint64_t ccr, overflow; // last captured ticks & IC overflow
    enum fir{
        BEMF_UNDEF,
        BEMF_FORWARD,
        BEMF_REVERSE
    } dir_t; // Rotor spinning direction based on back-EMF detection
} bldc_t;

void BLDC_Configure(bldc_t* bldc);

void BLDC_Start(bldc_t* bldc);

void BLDC_SetPWM(bldc_t* bldc);
void BLDC_SetCtrl(bldc_t* bldc, float ctrl);

void BLDC_CalcSpeed(bldc_t* bldc);

// Functions for main()
void BLDC_Restart(bldc_t* bldc);


#ifdef __cplusplus
}
#endif

#endif
