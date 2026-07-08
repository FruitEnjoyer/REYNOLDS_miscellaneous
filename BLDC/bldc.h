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

//#define BLDC_STARTER

#ifdef BLDC_STARTER
#define BLDC_MAGPAIRS              2
#define BLDC_KV                    3300  // [rpm / V]
#define BLDC_ALIGN_DELAY           500   // [ms]
#define BLDC_PRESTARTUP_DELAY      1500  // [ms]
#define BLDC_SPEEDUP_ACCELERATION  200   // [rpm / sec]
#define BLDC_SPEEDUP_MINSPEED      60    // [rpm]
#define BLDC_SPEEDUP_MAXSPEED      3300  // [rpm]
#define BLDC_STARTUP_MINDUTY       120
#define BLDC_CLOSELOOP_LOADDUTY    50
#else // PUMP
#define BLDC_MAGPAIRS              7
#define BLDC_KV                    2300  // [rpm / V]
#define BLDC_ALIGN_DELAY           0     // [ms]
#define BLDC_PRESTARTUP_DELAY      0     // [ms]
#define BLDC_SPEEDUP_ACCELERATION  2700  // [rpm / sec]
#define BLDC_SPEEDUP_MINSPEED      60    // [rpm]
#define BLDC_SPEEDUP_MAXSPEED      2700  // [rpm]
#define BLDC_STARTUP_MINDUTY       150
#define BLDC_CLOSELOOP_LOADDUTY    45
#define BLDC_CLOSELOOP_WRONGCCR_MIN 350  // to detect fault
#define BLDC_CLOSELOOP_WRONGCCR_MAX 500  // to detect fault
#define BLDC_ARR_INITTARGET         5800
#endif

#define BLDC_DIRECTION 5
#define BLDC_SPEEDUP_INTER_NUM 42
#define TIM_FREQ 160000000
#define TIM_BASE_INIT_ARR 5999
#define TIM_PWM_ARR 1000
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
    float speed, filtspeed, speedbyarr, targetspeed;
    volatile uint32_t duty, fakeduty;
    const uint32_t pwm_CCER_ch1, pwm_CCER_ch2, pwm_CCER_ch3;
    uint32_t ic_freq;
    enum state_dir{
        FORWARD,
        REVERSE
    } state_dir_t;
    enum control_mode{
        IDLE = 0,
        ALIGN,
		PRESTARTUP,
        STARTUP,
		CLOSELOOP
    } control_mode_t;
    uint32_t manual_ticksdelta, ctrl_ticksdelta;
    //float ctrl; // Magnitude of PWM-ON state (from -1 to 1)
    uint8_t usearr;

    struct{
        uint8_t disabletim_flag, run_flag;
    } idle;
    struct{
        uint16_t cnt;
    } align;
    struct{
        float speed, finalspeed, tmax, t;
        uint16_t cnt;
        uint32_t last_psc;
        uint16_t minduty;
    } startup;
    struct{
        uint32_t arr, load_duty;
        float target, fduty;
        float kp, ki, err, interr, out;
        uint8_t needrestart_flag;
    } closeloop;
} bldc_t;

void BLDC_Configure(bldc_t* bldc);

void BLDC_Start(bldc_t* bldc);

uint16_t DutyByTargetPump(float target);
void BLDC_SetPWM(bldc_t* bldc);
void BLDC_SetCtrl(bldc_t* bldc, float ctrl);

void BLDC_CalcSpeed(bldc_t* bldc);

// Functions for main()
void BLDC_Restart(bldc_t* bldc);

float BLDC_Speedup(float t);

#ifdef __cplusplus
}
#endif

#endif
