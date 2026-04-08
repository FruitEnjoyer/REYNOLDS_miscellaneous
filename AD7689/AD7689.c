/*
 * AD7689.c
 *
 *  Created on: Apr 6, 2026
 *      Author: user
 */


#include "AD7689.h"

HAL_StatusTypeDef AD7689_Init(AD7689_t *chip)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint16_t config = AD7689_CONFIG_CH0;

    chip->txbuff[0] = (uint8_t)config;
    chip->txbuff[1] = (uint8_t)(config >> 8);

    // Do a dummy calls
    status = HAL_SPI_TransmitReceive_IT(chip->spi, chip->txbuff, chip->rxbuff, 1);

    return status;
}

HAL_StatusTypeDef AD7689_ReadSingle(AD7689_t *chip, uint8_t channel)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint16_t config;

    switch(channel)
    {
    case 0:  config = AD7689_CONFIG_CH0; break;
    case 1:  config = AD7689_CONFIG_CH1; break;
    case 2:  config = AD7689_CONFIG_CH2; break;
    case 3:  config = AD7689_CONFIG_CH3; break;
    case 4:  config = AD7689_CONFIG_CH4; break;
    case 5:  config = AD7689_CONFIG_CH5; break;
    case 6:  config = AD7689_CONFIG_CH6; break;
    case 7:  config = AD7689_CONFIG_CH7; break;
    default: return HAL_ERROR;
    }

    chip->txbuff[0] = (uint8_t)config;
    chip->txbuff[1] = (uint8_t)(config >> 8);

    status = HAL_SPI_TransmitReceive_IT(chip->spi, chip->txbuff, chip->rxbuff, 1);

    if(status == HAL_OK)
    {
        chip->results[channel] = (uint16_t)(256 * chip->rxbuff[1]) + chip->rxbuff[0];
    }
    return status;
}
HAL_StatusTypeDef AD7689_ReadAll(AD7689_t *chip)
{

}
