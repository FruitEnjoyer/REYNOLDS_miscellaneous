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




typedef struct bldc{
    uint8_t poles_number; // number of rotor poles
    float speed;
    struct{
        uint64_t ccr; // last captured ticks
        uint64_t overflow; // timer overflow counter
        enum{
            FORWARD,
            REVERSE
        } direction;
        HAL_TIM_ActiveChannel channels[3];
    } speedtracking;
    struct{
        uint8_t state;
        float magnitude; // stator field magnitude
        uint32_t channels[3];
    } pwm;
} bldc_t;


void BLDC_getspeed(bldc_t* bldc, TIM_HandleTypeDef *htim);

void BLDC_IC_speedtracking(bldc_t* bldc, TIM_HandleTypeDef *htim);
void BLDC_IC_setPWM(bldc_t* bldc, TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif

#endif
