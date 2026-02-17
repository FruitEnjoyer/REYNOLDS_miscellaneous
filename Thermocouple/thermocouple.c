/**
 * @file thermocouple.c
 * @author ruslan
 * @brief 
 * @date 17.02.2026
 */

#include "thermocouple.h"
#include <stddef.h>

/**
 * @brief Calibration voltage in millivolts
 */
static float volts[THERMOCOUPLE_POINTNUM] = {
    -6.458, -6.158, -5.354, -4.138, -2.587, 
    -0.778, 1.203, 3.267, 5.328, 7.34,
    9.343, 11.382, 13.457, 15.554, 17.667, 
    19.792, 21.924, 24.055, 26.179, 28.289,
    30.382, 32.453, 34.501, 36.524, 38.522, 
    40.494, 42.44, 44.359, 46.249, 48.105,
    49.926, 51.708, 53.451, 54.819
};

/**
 * @brief Calibration temperatures in Celsius
 */
static float temps[THERMOCOUPLE_POINTNUM] = {
    -270, -220, -170, -120, -70,
    -20, 30, 80, 130, 180,
    230, 280, 330, 380, 430,
    480, 530, 580, 630, 680,
    730, 780, 830, 880, 930,
    980, 1030, 1080, 1130, 1180,
    1230, 1280, 1330, 1370
};

/**
 * @brief Converts thermocouple voltage to temperature
 * 
 * @param volts ADC voltage
 * @param T temperature shift
 * @return float 
 */
float TC_Volts2Temp(float V, float T)
{
    float res = 0.0f;

    if(V <= THERMOCOUPLE_MINVOLT)
    { res = temps[0]; }
    else if(V >= THERMOCOUPLE_MAXVOLT)
    { res = temps[THERMOCOUPLE_POINTNUM - 1]; }
    else
    {
        size_t i = 0;
        while(V > volts[i])
        { i += 1; }
        
        // t = a * v + b
        // TODO: check possible division by zero
        float a = (temps[i] - temps[i - 1]) / (volts[i] - volts[i - 1]);
        float b = (temps[i - 1] * volts[i] - temps[i] * volts[i - 1]) / (volts[i] - volts[i - 1]);

        res = a * V + b;
    }

    return res - T;
}