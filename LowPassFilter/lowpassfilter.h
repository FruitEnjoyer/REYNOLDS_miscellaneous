/**
 * @file lowpassfilter.h
 * @author ruslan
 * @brief lowpassfilter.c header
 * @date 16.02.2026
 */

#ifndef LOWPASSFILTER_H_
#define LOWPASSFILTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#define FILTERSIZE 31
#if FILTERSIZE < 1
#error FILTERSIZE must be positive integer.
#endif

typedef float real;
// typedef double real;

typedef enum filter_type{
    RECTANGULAR,
    BLACKMAN
} filter_type_t;

typedef struct lpfilter{
    filter_type_t type;
    real buffer[FILTERSIZE];
    real coefs[FILTERSIZE];
    int index;
} lpfilter_t;


void LPF_init(lpfilter_t* filter, filter_type_t type);
real LPF_filter(lpfilter_t* filter, real a);



#ifdef __cplusplus
}
#endif

#endif
