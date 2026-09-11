/*
 * pump.h
 *
 *  Created on: Jul 29, 2026
 *      Author: user
 */

#ifndef INC_PUMP_H_
#define INC_PUMP_H_

#include "main.h"
#include "tim.h"
#include "../../../../BLDC/bldc.h"

#define PUMP_MAGPAIRS              7
#define PUMP_ALIGN_DELAY           0     // [ms]
#define PUMP_PRESTARTUP_DELAY      0     // [ms]
#define PUMP_SPEEDUP_ACCELERATION  2700  // [rpm / sec]
#define PUMP_SPEEDUP_MINSPEED      60    // [rpm]
#define PUMP_SPEEDUP_MAXSPEED      2200  // [rpm]
#define PUMP_STARTUP_MINDUTY       150
#define PUMP_CLOSELOOP_MINDUTY       0
#define PUMP_ARR_INITTARGET         5800
#define PUMP_SPEEDUP_INTER_NUM 42
#define PUMP_TIM_FREQ 160000000
#define PUMP_TIM_INIT_ARR 5999
#define PUMP_PWM_ARR 1000

//#define PUMP_SPEEDUP_SET_PSC(speed) __HAL_TIM_SET_PRESCALER(&htim6, (uint32_t)(PUMP_TIM_FREQ / 6. / (PUMP_TIM_INIT_ARR + 1) / PUMP_MAGPAIRS / speed * 60 - 1)); // speed = [rpm]

extern bldc_t pump;

void Pump_Update();
uint16_t DutyByTargetPump(float target);
void Pump_SetDuty(int32_t duty);

#endif /* INC_PUMP_H_ */
