/*
 * callbacks.c
 *
 *  Created on: Feb 25, 2026
 *      Author: user
 */

#include "main.h"


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM5)
    {
        if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
        {

        }
    }
}
