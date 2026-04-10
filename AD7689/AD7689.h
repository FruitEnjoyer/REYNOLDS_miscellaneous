/*
 * AD7689.h
 *
 *  Created on: Apr 6, 2026
 *      Author: user
 */

#ifndef _AD7689_H_
#define _AD7689_H_

#include "main.h"

/*
 * Tested SPI settings
 *
 * Motorola
 * 16 bits
 * MSB First
 *
 * Baud Rate 5.0 MBits/s
 * CPOL Low
 * CPHA 1 Edge
 *
 * CRC Calculation: Disabled
 * NSSP Mode: Enabled
 * NSS Signal Type: Output Hardware
 *
 * ENABLE INTERRUPTS
 */

#define AD7689_VREF        (4.096f)

#define AD7689_CONFIG_CH0  ((uint16_t)(0b1111000111100100))
#define AD7689_CONFIG_CH1  ((uint16_t)(0b1111001111100100))
#define AD7689_CONFIG_CH2  ((uint16_t)(0b1111010111100100))
#define AD7689_CONFIG_CH3  ((uint16_t)(0b1111011111100100))
#define AD7689_CONFIG_CH4  ((uint16_t)(0b1111100111100100))
#define AD7689_CONFIG_CH5  ((uint16_t)(0b1111101111100100))
#define AD7689_CONFIG_CH6  ((uint16_t)(0b1111110111100100))
#define AD7689_CONFIG_CH7  ((uint16_t)(0b1111111111100100))

typedef struct AD7689{
    SPI_HandleTypeDef* spi;
    GPIO_TypeDef* nss_port;
    uint32_t nss_pin;
    uint8_t circular_counter;

    uint8_t txbuff[2], rxbuff[2];

    uint16_t results_int[8]; // [0-65535]
    float results_float[8];  // [Volts]
} AD7689_t;

HAL_StatusTypeDef AD7689_Init(AD7689_t *chip);
HAL_StatusTypeDef AD7689_ReadSingle(AD7689_t *chip, uint8_t channel);
HAL_StatusTypeDef AD7689_ReadCircular(AD7689_t *chip);


#endif /* _AD7689_H_ */
