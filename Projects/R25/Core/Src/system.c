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

void System_Init()
{
    HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    AD7689_Init(&extADC);
}

void HeartbeatLED_Update()
{
    static uint32_t heartbeat_ticks = 0;
    const static uint32_t heartbeat_delta = 125;

    if (heartbeat_ticks + heartbeat_delta < HAL_GetTick())
    {
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        heartbeat_ticks += heartbeat_delta;
    }
}

void ADC_Update()
{
    static uint32_t ad7689_ticks = 0;
    const static uint32_t ad7689_delta = 100;

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
