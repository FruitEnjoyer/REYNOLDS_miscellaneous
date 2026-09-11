/*
 * system.h
 *
 *  Created on: Jun 29, 2026
 *      Author: user
 */

#ifndef INC_SYSTEM_H_
#define INC_SYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "../../../../AD7689/AD7689.h"
#include "../../../../Thermocouple/thermocouple.h"

//#define IGNITION_HEAT
#ifndef IGNITION_HEAT
#define IGNITION_SPARK
#endif

#pragma pack(0)
typedef struct
{
    uint16_t adc[4];
    uint8_t adc_ready_flag;

    int32_t thermocouple_volts;
    int32_t vref;
    int32_t mcu_temp; // Celsius
    float thermocouple_temp; // Celsius
} systemvars_t;
#pragma pack(1)

volatile extern systemvars_t systemvars;
extern AD7689_t extADC;

void HeartbeatLED_Update();
void AD7689_Update();
void ValveStart_SetDuty(uint16_t duty);
void ValveMain_SetDuty(uint16_t duty);
void Ignition_SetDuty(uint16_t duty);
//float Thermocouple_CalcTemp();

#ifdef __cplusplus
}
#endif

#endif /* INC_SYSTEM_H_ */
