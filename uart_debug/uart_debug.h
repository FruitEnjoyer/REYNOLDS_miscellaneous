/**
 * @file uart_debug.h
 * @author ruslan
 * @brief Debug via UART interface
 * @date 10.03.2026
 */

#ifndef UART_DEBUG_H_
#define UART_DEBUG_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "main.h"

void DBG_BLDC_Detection(uint32_t ch);


#ifdef __cplusplus
}
#endif

#endif /* UART_DEBUG_H_ */
