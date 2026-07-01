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

typedef struct
{
    // MCU die temperature (degree Celsius)
    int32_t mcu_temp;
} systemvars_t;

void HeartbeatLED_Update();
void AD7689_Update();

#ifdef __cplusplus
}
#endif

#endif /* INC_SYSTEM_H_ */
