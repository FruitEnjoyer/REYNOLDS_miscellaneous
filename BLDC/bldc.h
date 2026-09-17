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

#endif

#pragma pack(0)
typedef struct bldc{
    // Motor characteristics
    //const uint8_t pole_number; // Number of rotor magnetic poles
    //const float KV; // Back-EMF constant
    //float pwm_freq; //

    // Power variables
    enum {
        STATE_OFF = -1,
        STATE_1 = 0, STATE_2,
        STATE_3, STATE_4,
        STATE_5, STATE_6
    } field_state; // Stator field state
    TIM_HandleTypeDef* pwmtim;
    TIM_HandleTypeDef* ictim;
    volatile uint32_t last_ccr, fakelast_ccr;
    float speed, speedbyarr;
    int32_t targetspeed;
    uint32_t intspeed;
    volatile uint32_t duty;
    const uint32_t pwm_CCER_ch1, pwm_CCER_ch2, pwm_CCER_ch3;
    //uint32_t ic_freq;
    enum control_mode{
        IDLE = 0,
        ALIGN,
        PRESTARTUP,
        STARTUP,
        CLOSELOOP
    } control_mode_t;
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
    } startup;
    struct{
        uint16_t cnt;
        uint32_t arr, load_duty;
        int32_t target;
        float fduty;
        float kp, ki, kd, err, preverr, prev2err, interr, differr, out;
        uint8_t needrestart_flag;
    } closeloop;
    uint32_t speed_cnt, speed_hall_cnt;
} bldc_t;
#pragma pack(1)


void BLDC_SetPWM(bldc_t* bldc);

#ifdef __cplusplus
}
#endif

#endif
