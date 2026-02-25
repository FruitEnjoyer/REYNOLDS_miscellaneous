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
    float speed;
    volatile uint32_t a;
} bldc_t;


void BLDC_getspeed(bldc_t* bldc);

#ifdef __cplusplus
}
#endif

#endif
