/*
 * system.c
 *
 *  Created on: Jun 29, 2026
 *      Author: user
 */

#include "system.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"

volatile systemvars_t systemvars = { 0, };

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
        //HAL_ADC_Start_IT(&hadc1);
        ad7689_ticks += ad7689_delta;
    }
}

void ValveStart_SetDuty(uint16_t duty)
{
    if(duty < 0)
    {
        duty = 0;
    }
    else if(duty > 920)
    {
        duty = 920;
    }
    htim15.Instance->CCR2 = duty;
}

void ValveMain_SetDuty(uint16_t duty)
{
    if(duty < 0)
    {
        duty = 0;
    }
    else if(duty > 920)
    {
        duty = 920;
    }
    htim15.Instance->CCR1 = duty;
}

void Ignition_SetDuty(uint16_t duty)
{
#ifdef IGNITION_SPARK
    if(duty < 0)
    {
        duty = 0;
    }
    else if(duty > 90)
    {
        duty = 90;
    }
    htim17.Instance->CCR1 = duty;
#else
#endif
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if(hadc == &hadc1)
    {
        systemvars.adc_ready_flag = 1;
        mtm.adc1_complate_flag = 1;
    }
    //systemvars.mcu_temp = __HAL_ADC_CALC_TEMPERATURE(3300, HAL_ADC_GetValue(hadc), ADC_RESOLUTION_12B);
}
