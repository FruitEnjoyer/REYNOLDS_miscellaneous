/**
 * @file i2c_scanner.h
 * @author ruslan
 * @brief Debug software for scanning devices on i2c bus
 * @date 29.01.2026
 * @details Implements function for i2c bus analyzing.
 *          Function scans all 256 addresses and saves
 *          those that have a response from slave devices.
 *          Allows verify slave devices accessability
 * @warning Works only for DEBUG build!
 *          Scanning takes some time
 */

#ifndef I2C_SCANNER_H_
#define I2C_SCANNER_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "i2c.h"
#include <stdint.h>

/**
 * @brief Scans i2c bus & saves results in static buffer
 * 
 * @param hi2c I2C handle Structure definition
 * @return uint8_t number of addresses with response
 */
uint8_t I2CScanner_InspectBus(I2C_HandleTypeDef* hi2c);

#ifdef __cplusplus
}
#endif
#endif /* I2C_SCANNER_H_ */
