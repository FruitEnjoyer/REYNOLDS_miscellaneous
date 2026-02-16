/**
 * @file lowpassfilter.c
 * @author ruslan
 * @brief Low-pass digital filter via window function
 * @date 16.02.2026
 */

#include "lowpassfilter.h"
#include "stddef.h"
#include "stdio.h"
#include "math.h"


void LPF_init(lpfilter_t* filter, filter_type_t type)
{
    filter->type = type;
    for(size_t i = 0; i < FILTERSIZE; ++i)
    {
        filter->buffer[i] = 0.0;
    }
    filter->index = 0;
    switch(type)
    {
        case RECTANGULAR:
            for(size_t i = 0; i < FILTERSIZE; ++i)
            {
                filter->coefs[i] = 1.0 / FILTERSIZE;
            }
            break;
        case BLACKMAN:
            for(size_t i = 0; i < FILTERSIZE; ++i)
            {
                filter->coefs[i] = 0.42 - \
                           0.5 * cos(2 * M_PI * i / FILTERSIZE) + \
                           0.08 * cos(4 * M_PI * i / FILTERSIZE);
            }
            break;
        default: // Rectangular by default
            for(size_t i = 0; i < FILTERSIZE; ++i)
            {
                filter->coefs[i] = 1.0 / FILTERSIZE;
            }
            break;
    }
}

real LPF_filter(lpfilter_t* filter, real a)
{
    real total = 0.0;
    real res = 0.0;

    filter->buffer[filter->index] = a;
    filter->index = (filter->index + 1) % FILTERSIZE;

    for(size_t i = 0; i < FILTERSIZE; ++i)
    {
        res += filter->buffer[(i + filter->index) % FILTERSIZE] * filter->coefs[i];
        total += filter->coefs[i];
    }
    return res / total;
}