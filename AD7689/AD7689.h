/*
 * AD7689.h
 *
 *  Created on: Apr 6, 2026
 *      Author: user
 */

#ifndef _AD7689_H_
#define _AD7689_H_


#include "main.h"

typedef struct AD7689{
	SPI_HandleTypeDef* spi;
	GPIO_TypeDef* nss_port;
	uint16_t nss_pin;
} AD7689_t;

#endif /* _AD7689_H_ */
