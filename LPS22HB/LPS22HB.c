/**
 * @file LPS22HB.c
 * @brief LPS22HB pressure sensor driver implementation
 * @date 29.01.2026
 * @author Ruslan Valeev
 * @details Implemented basic get-set communication functions
 */

#include "LPS22HB.h"

#if 0
// TODO: определить желаемые настройки датчика
const LPS22HB_state_t lps22hb_custom_default = {
    .interrupt_cfg = LPS22HB_REGDEFAULT_INTERRUPT_CFG,

};
#endif

HAL_StatusTypeDef LPS22HB_Init(I2C_HandleTypeDef* hi2c,
                               LPS22HB_state_t* state)
{
    HAL_StatusTypeDef res = HAL_OK;
    uint8_t rx_buff[8] = {0,};

    // Try to read WHO_AM_I register and validate its value
    res = HAL_I2C_Mem_Read(hi2c, (uint16_t)LPS22HB_I2CADDR_R,
                           (uint16_t)LPS22HB_REGADDR_WHO_AM_I,
                           1, rx_buff, 1, 30);
    if(res != HAL_OK || rx_buff[0] != LPS22HB_REGDEFAULT_WHO_AM_I)
    {  return res;  }



    return res;
}

HAL_StatusTypeDef LPS22HB_Configure(I2C_HandleTypeDef* hi2c,
                                    LPS22HB_state_t* newstate)
{

}

HAL_StatusTypeDef LPS22HB_GetState(I2C_HandleTypeDef* hi2c,
                                   LPS22HB_state_t* dest)
{

}

HAL_StatusTypeDef LPS22HB_GetData(I2C_HandleTypeDef* hi2c,
                                  LPS22HB_data_t* dest)
{

}
