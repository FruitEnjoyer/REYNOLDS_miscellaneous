/**
 * @file LPS22HB.c
 * @brief LPS22HB pressure sensor driver implementation
 * @date 29.01.2026
 * @author Ruslan Valeev
 * @details Implemented basic get-set communication functions
 */

#include "LPS22HB.h"

/**
 * Preferred start mode structure
 */
static const LPS22HB_state_t custom_default = {
    .interrupt_cfg = LPS22HB_REGDEFAULT_INTERRUPT_CFG,
    .ths_p_l = LPS22HB_REGDEFAULT_THS_P_L,
    .ths_p_h = LPS22HB_REGDEFAULT_THS_P_H,
    .ctrl_reg1 = 0b01011010,
    .ctrl_reg2 = LPS22HB_REGDEFAULT_CTRL_REG2,
    .ctrl_reg3 = LPS22HB_REGDEFAULT_CTRL_REG3,
    .fifo_ctrl = LPS22HB_REGDEFAULT_FIFO_CTRL,
    .ref_p_xl = LPS22HB_REGDEFAULT_REF_P_XL,
    .ref_p_l = LPS22HB_REGDEFAULT_REF_P_L,
    .ref_p_h = LPS22HB_REGDEFAULT_REF_P_H,
    .rpds_l = LPS22HB_REGDEFAULT_RPDS_L,
    .rpds_h = LPS22HB_REGDEFAULT_RPDS_H,
    .res_conf = LPS22HB_REGDEFAULT_RES_CONF
};

static void StateToBuffer(LPS22HB_state_t* state, uint8_t* buffer);


HAL_StatusTypeDef LPS22HB_Init(I2C_HandleTypeDef* hi2c)
{
    HAL_StatusTypeDef res = HAL_OK;
    uint8_t rx_buff[8] = {0,};

    // Try to read WHO_AM_I register and validate its value
    res = HAL_I2C_Mem_Read(hi2c, (uint16_t)LPS22HB_I2CADDR_R,
                           (uint16_t)LPS22HB_REGADDR_WHO_AM_I,
                           1, rx_buff, 1, 30);
    if(res != HAL_OK || rx_buff[0] != LPS22HB_REGDEFAULT_WHO_AM_I)
    {  return res;  }

    res = LPS22HB_Configure(hi2c, (LPS22HB_state_t*)(&custom_default));

    return res;
}

HAL_StatusTypeDef LPS22HB_Configure(I2C_HandleTypeDef* hi2c,
                                    LPS22HB_state_t* newstate)
{
    HAL_StatusTypeDef res = HAL_OK;
    uint8_t config_buff[13] = {0,};
    StateToBuffer(newstate, config_buff);

    // Load config to sensor registers
    res = HAL_I2C_Mem_Write(hi2c, (uint16_t)LPS22HB_I2CADDR_W, (uint16_t)LPS22HB_REGADDR_INTERRUPT_CFG,
                      3, &config_buff[0], 3, 30);
    if(res != HAL_OK) return res;

    res = HAL_I2C_Mem_Write(hi2c, (uint16_t)LPS22HB_I2CADDR_W, (uint16_t)LPS22HB_REGADDR_CTRL_REG1,
                      3, &config_buff[3], 3, 30);
    if(res != HAL_OK) return res;

    res = HAL_I2C_Mem_Write(hi2c, (uint16_t)LPS22HB_I2CADDR_W, (uint16_t)LPS22HB_REGADDR_FIFO_CTRL,
                      7, &config_buff[6], 7, 30);
    return res;
}

HAL_StatusTypeDef LPS22HB_GetState(I2C_HandleTypeDef* hi2c,
                                   LPS22HB_state_t* dest)
{
    uint8_t rx_buff[13] = {0,};
    HAL_StatusTypeDef res = HAL_OK;

    res = HAL_I2C_Mem_Read(hi2c, (uint16_t)LPS22HB_I2CADDR_R, (uint16_t)LPS22HB_REGADDR_INTERRUPT_CFG,
                           3, &rx_buff[0], 3, 30);
    if(res != HAL_OK) return res;

    res = HAL_I2C_Mem_Read(hi2c, (uint16_t)LPS22HB_I2CADDR_R, (uint16_t)LPS22HB_REGADDR_CTRL_REG1,
                           3, &rx_buff[3], 3, 30);
    if(res != HAL_OK) return res;

    res = HAL_I2C_Mem_Read(hi2c, (uint16_t)LPS22HB_I2CADDR_R, (uint16_t)LPS22HB_REGADDR_CTRL_REG1,
                           7, &rx_buff[6], 7, 30);
    if(res != HAL_OK) return res;

    // TODO: перевести буффер принятых данных в структуру
    dest->interrupt_cfg = rx_buff[0];
    dest->ths_p_l       = rx_buff[1];
    dest->ths_p_h       = rx_buff[2];
    return res;
}

HAL_StatusTypeDef LPS22HB_GetData(I2C_HandleTypeDef* hi2c,
                                  LPS22HB_data_t* dest)
{
    uint8_t rx_buff[5] = {0,};
    HAL_StatusTypeDef res = HAL_OK;

    res = HAL_I2C_Mem_Read(hi2c, (uint16_t)LPS22HB_I2CADDR_R,
                           (uint16_t)LPS22HB_REGADDR_PRESS_OUT_XL,
                           5, rx_buff, 5, 30);
    if(res != HAL_OK) return res;

    // TODO: реализовать перевод байтов в давление и температуру
    uint32_t pressure_bits = ((uint32_t)rx_buff[2] << 16) | (rx_buff[1] << 8) | rx_buff[0];
    dest->pressure = (float)pressure_bits / 4096.0f;

    return res;
}


static void StateToBuffer(LPS22HB_state_t* state, uint8_t* buffer)
{
    buffer[0] = state->interrupt_cfg;
    buffer[1] = state->ths_p_l;
    buffer[2] = state->ths_p_h;
    buffer[3] = state->ctrl_reg1;
    buffer[4] = state->ctrl_reg2;
    buffer[5] = state->ctrl_reg3;
    buffer[6] = state->fifo_ctrl;
    buffer[7] = state->ref_p_xl;
    buffer[8] = state->ref_p_l;
    buffer[9] = state->ref_p_h;
    buffer[10] = state->rpds_l;
    buffer[11] = state->rpds_h;
    buffer[12] = state->res_conf;
}
