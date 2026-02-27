/**
 * @file i2c_scanner.c
 * @author ruslan
 * @brief Debug software for scanning devices on i2c bus
 * @date 29.01.2026
 * @details Implements function for i2c bus analyzing.
 *          Function scans all 256 adresses and saves
 *          those that have a response from slave devices.
 *          Allows verify slave devices accessability
 * @warning Works only for DEBUG build!
 *          Scanning takes some time
 */

#include "i2c_scanner.h"

#ifdef DEBUG

static HAL_StatusTypeDef i2cscanner_results[256] = {0,};


uint8_t I2CScanner_InspectBus(I2C_HandleTypeDef* hi2c)
{
    uint8_t responces = 0;
    uint8_t buff[1] = {0,};
    for(uint16_t i = 0x00; i <= 0xFF; ++i)
    {
        i2cscanner_results[i] = HAL_I2C_Master_Receive(hi2c, i, buff, 1, 10);
        if(i2cscanner_results[i] == HAL_OK)
        {  responces += 1;  }
    }
    return responces;
}

#else

uint8_t I2CScanner_InspectBus(I2C_HandleTypeDef* hi2c)
{
    // Do nothing if build type is not DEBUG
    UNUSED(hi2c);
    return 0;
}

#endif
