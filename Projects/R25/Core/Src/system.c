/*
 * system.c
 *
 *  Created on: Jun 29, 2026
 *      Author: user
 */

#include "system.h"
#include "adc.h"
#include "spi.h"


systemvars_t systemvars = { 0, };

AD7689_t extADC = { .spi = &hspi1, .nss_mode = NSS_SOFTWARE, .nss_port =
        AD7689_CS_GPIO_Port, .nss_pin = AD7689_CS_Pin };

__attribute__((always_inline)) inline void HeartbeatLED_Update()
{
    static uint32_t heartbeat_ticks = 0;
    const static uint32_t heartbeat_delta = 125;

    if (heartbeat_ticks + heartbeat_delta < HAL_GetTick())
    {
        LED1_GPIO_Port->BSRR = ((LED1_GPIO_Port->ODR & LED1_Pin) << 16u) | (~LED1_GPIO_Port->ODR & LED1_Pin);
        heartbeat_ticks += heartbeat_delta;
    }
}

__attribute__((always_inline)) inline void AD7689_Update()
{
    static uint32_t ad7689_ticks = 0;
    const static uint32_t ad7689_delta = 10;

    if (ad7689_ticks + ad7689_delta < HAL_GetTick())
    {
        AD7689_ReadCircular(&extADC);
        HAL_ADC_Start_IT(&hadc1);
        ad7689_ticks += ad7689_delta;
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    systemvars.mcu_temp = __HAL_ADC_CALC_TEMPERATURE(3300, HAL_ADC_GetValue(hadc),
            ADC_RESOLUTION_12B);
}
