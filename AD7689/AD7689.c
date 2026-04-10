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
    chip->circular_counter = 0;

    // Do a dummy call
    chip->nss_port->BRR = chip->nss_pin;
    HAL_SPI_TransmitReceive(chip->spi, chip->txbuff, chip->rxbuff, 1, 1);
    chip->nss_port->BSRR = chip->nss_pin;
    for(int i = 0; i < 50; ++i) __NOP();

    // Do a dummy call
    chip->nss_port->BRR = chip->nss_pin;
    status = HAL_SPI_TransmitReceive(chip->spi, chip->txbuff, chip->rxbuff, 1, 1);
    chip->nss_port->BSRR = chip->nss_pin;
    for(int i = 0; i < 50; ++i) __NOP();

    // Clear garbage values
    chip->rxbuff[0] = 0;
    chip->rxbuff[1] = 0;

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

    chip->nss_port->BRR = chip->nss_pin;
    HAL_SPI_TransmitReceive(chip->spi, chip->txbuff, chip->rxbuff, 1, 1);
    chip->nss_port->BSRR = chip->nss_pin;
    for(int i = 0; i < 50; ++i) __NOP();

    chip->nss_port->BRR = chip->nss_pin;
    HAL_SPI_TransmitReceive(chip->spi, chip->txbuff, chip->rxbuff, 1, 1);
    chip->nss_port->BSRR = chip->nss_pin;
    for(int i = 0; i < 50; ++i) __NOP();

    chip->nss_port->BRR = chip->nss_pin;
    status = HAL_SPI_TransmitReceive(chip->spi, chip->txbuff, chip->rxbuff, 1, 1);
    chip->nss_port->BSRR = chip->nss_pin;

    if(status == HAL_OK)
    {
        chip->results_int[channel] = (uint16_t)(256 * chip->rxbuff[1]) + chip->rxbuff[0];
        chip->results_float[channel] = AD7689_VREF * chip->results_int[channel] / 65536;
    }
    return status;
}

HAL_StatusTypeDef AD7689_ReadCircular(AD7689_t *chip)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint16_t config;

    switch(chip->circular_counter)
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

    chip->nss_port->BRR = chip->nss_pin;
    status = HAL_SPI_TransmitReceive(chip->spi, chip->txbuff, chip->rxbuff, 1, 1);
    chip->nss_port->BSRR = chip->nss_pin;

    if(status == HAL_OK)
    {
        chip->results_int[(chip->circular_counter + 6) % 8] = (uint16_t)(256 * chip->rxbuff[1]) + chip->rxbuff[0];
        chip->results_float[(chip->circular_counter + 6) % 8] = AD7689_VREF * chip->results_int[(chip->circular_counter + 6) % 8] / 65536;
    }
    chip->circular_counter = (chip->circular_counter + 1) % 8;

    return status;
}
