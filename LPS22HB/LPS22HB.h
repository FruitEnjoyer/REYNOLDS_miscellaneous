/**
 * @file LPS22HB.h
 * @brief Header for LPS22HB pressure sensor driver
 * @date 29.01.2026
 * @author Ruslan Valeev
 */

#ifndef LPS22HB_H_
#define LPS22HB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "LPS22HB_defines.h"

typedef struct LPS22HB_state
{
    uint8_t interrupt_cfg;
    uint8_t ths_p_l, ths_p_h;
    uint8_t ctrl_reg1, ctrl_reg2, ctrl_reg3;
    uint8_t fifo_ctrl;
    uint8_t ref_p_xl, ref_p_l, ref_p_h;
    uint8_t rpds_l, rpds_h;
    uint8_t res_conf;
} LPS22HB_state_t;

typedef struct LPS22HB_data
{
    float pressure, temp;
} LPS22HB_data_t;

HAL_StatusTypeDef LPS22HB_Init(I2C_HandleTypeDef* hi2c, LPS22HB_state_t* state);
HAL_StatusTypeDef LPS22HB_Configure(I2C_HandleTypeDef* hi2c, LPS22HB_state_t* newstate);
HAL_StatusTypeDef LPS22HB_GetState(I2C_HandleTypeDef* hi2c, LPS22HB_state_t* dest);
HAL_StatusTypeDef LPS22HB_GetData(I2C_HandleTypeDef* hi2c, LPS22HB_data_t* dest);


#ifdef __cplusplus
}
#endif
#endif /* LPS22HB_H_ */
