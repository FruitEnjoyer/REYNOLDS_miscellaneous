/**
 * @file uart_debug.c
 * @author ruslan
 * @brief 
 * @date 10.03.2026
 */

#include "uart_debug.h"
#include <stdio.h>
#include "usart.h"

#ifdef DEBUG

void DBG_SendInfo_BLDC(bldc_t* bldc)
{
    static uint32_t dbg_bldc_ticks = 0;
    static uint32_t dbg_bldc_delta = 100;
    static char buff[32] = {0,};

    if(dbg_bldc_ticks + dbg_bldc_delta < HAL_GetTick())
    {
        BLDC_CalcSpeed(bldc);
        HAL_UART_Transmit_IT(&huart1, (uint8_t*)buff, 
                            sprintf(buff, "%f  %i  %li\r\n", 
                                    bldc->speed, 
                                    bldc->control_mode_t, 
                                    bldc->duty1
                                )
                            );
        dbg_bldc_ticks += dbg_bldc_delta;
    }
}

#else
#warning uart_debug is not implemented in non-debug build!

void DBG_SendInfo_BLDC(bldc_t* bldc)
{
    UNUSED(bldc);
}

#endif
