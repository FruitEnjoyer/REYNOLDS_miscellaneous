/**
 * @file thermocouple.h
 * @author ruslan
 * @brief 
 * @date 17.02.2026
 * @details Thermocouple type K
 */

#ifndef __THERMOCOUPLE_H_
#define __THERMOCOUPLE_H_

#ifdef __cplusplus
extern "C" {
#endif


#define THERMOCOUPLE_POINTNUM  34
#define THERMOCOUPLE_MINVOLT   (-6.458f) // mV
#define THERMOCOUPLE_MAXVOLT   (54.819f) // mV

float TC_Volts2Temp(float V, float T);

#ifdef __cplusplus
}
#endif

#endif