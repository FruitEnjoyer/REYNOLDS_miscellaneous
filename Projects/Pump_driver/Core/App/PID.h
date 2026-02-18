#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include "main.h"

#define FREQ_REGULATION 10

struct PIDController
{
    // PID coefficients
    float Kp; //
    float Ki; //<-кэфы ПИДА
    float Kd; //

    float limMin; //
    float limMax; //лимиты выхода в долях

    float limMinInt;
    float limMaxInt; //лимиты интегральной части

    float minValue;
    float maxValue; //лимиты выхода в реальных единицах

    float delta; //диапазон

    float integrator;
    float prevError;
    float differentiator;
    float prevMeasurement;

    float out;

    float Kp_real;
    float Ki_real;
    float Kd_real;

    uint8_t update_flag;
};

void PIDController_Init(struct PIDController *pid, float lim_min, float lim_max,
        float int_lim_min, float int_lim_max, int16_t val_min, int16_t val_max);

void PIDController_REInit(struct PIDController *pid, float p, float i, float d);
void PIDController_Reset(struct PIDController *pid);
int PIDController_Update(struct PIDController *pid, float setpoint,
        float measurement, float table, int zero_point);

void PID_setup();

struct PIDController pid_starter;
struct PIDController pid_pump;
struct PIDController pid_t_lim;
struct PIDController pid_t4_start;

struct PIDController pid_t4_hall_err;

//Настройки пид регулятора насоса//
////////////////////////////////////////////////////////////////////////////
#define PUMP_PID_KP  0.00001f
#define PUMP_PID_KI  0.000015f
#define PUMP_PID_KD  0.0000f

#define PUMP_PID_KP_WORK 0.000004//0.0000015f// 0.0000002
#define PUMP_PID_KI_WORK 0.0000012f//0.00000045f//0.00000006

#define PUMP_PID_LIM_MIN  0.0f
#define PUMP_PID_LIM_MAX  1100.0f

#define PUMP_PID_LIM_MIN_INT  -1.0f
#define PUMP_PID_LIM_MAX_INT  1.0f

#define PUMP_MIN_VALUE 1000.0f
#define PUMP_MAX_VALUE 2000.0f

/////////////////////////////////////////////////////////////////////////////

//Настройки пид регулятора ограничения оборотов по температуре стартера//
////////////////////////////////////////////////////////////////////////////
#define T_LIM_PID_KP  25.00f
#define T_LIM_PID_KI  5.00f
#define T_LIM_PID_KD  0.00f

#define T_LIM_PID_LIM_MIN  0.0f
#define T_LIM_PID_LIM_MAX  1.0f

#define T_LIM_PID_LIM_MIN_INT   0.0f
#define T_LIM_PID_LIM_MAX_INT   10000.0f

#define T_LIM_MIN_VALUE 00.0f
#define T_LIM_MAX_VALUE 10000.0f

#define UST_TEMP_LIM 700
#define BEGIN_T_LIM_PID 60000

//Настройки пид регулятора стартера//
////////////////////////////////////////////////////////////////////////////
#define STARTER_PID_KP  0.0001f
#define STARTER_PID_KI  0.00007f//15f
#define STARTER_PID_KD  0.0000f

#define STARTER_PID_LIM_MIN  0.0f
#define STARTER_PID_LIM_MAX  1900.0f

#define STARTER_PID_LIM_MIN_INT  -1.0f
#define STARTER_PID_LIM_MAX_INT   1.0f

#define STARTER_MIN_VALUE 1000.0f
#define STARTER_MAX_VALUE 1700.0f

/////////////////////////////////////////////////////////////////////////////
#endif
