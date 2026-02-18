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

/**
 * @brief Thermocouple status enumeration
 * @details TC_STATUS_OK - thermocouple works properly
 *          TC_STATUS_FAIL - thermocouple damaged / not connected
 */
typedef enum TC_status{
    TC_STATUS_OK = 0, 
    TC_STATUS_FAIL
} TC_status_t;

TC_status_t TC_Volts2Temp(float V, float T, float* out);
TC_status_t TC_ERR_Control(float T);

#ifdef __cplusplus
}
#endif

#endif