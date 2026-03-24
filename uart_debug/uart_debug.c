/**
 * @file uart_debug.c
 * @author ruslan
 * @brief 
 * @date 10.03.2026
 */

#include "uart_debug.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;

#ifdef DEBUG

void DBG_BLDC_Detection(uint32_t ch)
{
    static char buff[32] = {0,};

    HAL_UART_Transmit_IT(&huart1, (uint8_t*)buff,
            sprintf(buff, "%li\r\n", ch));
}

#else
#warning uart_debug is not implemented in non-debug build!



#endif
