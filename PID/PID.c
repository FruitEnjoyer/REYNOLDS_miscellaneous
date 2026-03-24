/**
 * @file PID.c
 * @author Yuriy Magaram
 * @brief PID regulator implementation
 */

#include "PID.h"



void PID_Step(PID_controller_t* pid, float target, float measure, float dt)
{
    float error = target - measure;
    pid->integral += error * dt;
    float derivative = (error - pid->preverr) / dt;
    pid->preverr = error;
    pid->out = (pid->kp * error) + (pid->ki * pid->integral) + (pid->kd * derivative);
}

void PID_Reset(PID_controller_t* pid)
{
    pid->preverr = 0;
    pid->integral = 0;
    pid->out = 0;
}
