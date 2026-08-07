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

//#define IGNITION_HEAT
#ifndef IGNITION_HEAT
#define IGNITION_SPARK
#endif

typedef struct
{
    // MCU die temperature (Celsius)
    int32_t mcu_temp;
} systemvars_t;

extern AD7689_t extADC;

void HeartbeatLED_Update();
void AD7689_Update();
void ValveStart_SetDuty(uint16_t duty);
void ValveMain_SetDuty(uint16_t duty);
void Ignition_SetDuty(uint16_t duty);

#ifdef __cplusplus
}
#endif

#endif /* INC_SYSTEM_H_ */
