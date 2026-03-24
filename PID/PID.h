/**
 * @file PID.h
 * @author Yuriy Magaram
 * @brief Header for PID controller
 * @note Edited by ruslan
 */

#ifndef PID_H_
#define PID_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>

typedef struct PID_controller {
    float kp, ki, kd;
    float preverr, integral;
    float out;
} PID_controller_t;

void PID_Step(PID_controller_t* pid, float target, float measure, float dt);
void PID_Reset(PID_controller_t* pid);

#ifdef __cplusplus
}
#endif

#endif
