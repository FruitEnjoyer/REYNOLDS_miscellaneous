/*
 * starter.h
 *
 *  Created on: Jul 9, 2026
 *      Author: user
 */

#ifndef INC_STARTER_H_
#define INC_STARTER_H_

#include "main.h"
#include "../../../../BLDC/bldc.h"
#if 1
#define STARTER_MAGPAIRS              2
#define STARTER_ALIGN_DELAY           1900     // [ms]
#define STARTER_PRESTARTUP_DELAY      0     // [ms]
#define STARTER_SPEEDUP_ACCELERATION  400  // [rpm / sec]
#define STARTER_SPEEDUP_MINSPEED      60    // [rpm]
#define STARTER_SPEEDUP_MAXSPEED      3600  // [rpm]
#define STARTER_STARTUP_MINDUTY       170
#define STARTER_ARR_INITTARGET         3000
#define STARTER_SPEEDUP_INTER_NUM 42
#define STARTER_TIM_FREQ 160000000
#define STARTER_TIM_INIT_ARR 5999
#define STARTER_IC_PSC    49
#define STARTER_PWM_ARR 1000
#define STARTER_KP    (-0.025f)
#define STARTER_KI    (-0.01f)
#define STARTER_KD    (-0.001f)

#define STARTER_SPEEDUP_SET_PSC(speed) __HAL_TIM_SET_PRESCALER(&htim7, (uint32_t)(STARTER_TIM_FREQ / 6. / (STARTER_TIM_INIT_ARR + 1) / STARTER_MAGPAIRS / speed * 60 - 1)); // speed = [rpm]


void Starter_Update();
uint16_t DutyByTargetStarter(float target);
float Starter_Speedup(float t);
#endif
#endif /* INC_STARTER_H_ */
