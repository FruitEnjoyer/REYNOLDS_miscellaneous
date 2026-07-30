/*
 * pump.c
 *
 *  Created on: Jul 29, 2026
 *      Author: user
 */


#include "pump.h"

bldc_t pump = {
        .pwmtim = &htim1
};

void Pump_Update()
{
    static uint32_t pump_ticks = 0;
    const static uint32_t pump_delta = 1;

    if(pump_ticks + pump_delta < HAL_GetTick())
    {

    }
}
