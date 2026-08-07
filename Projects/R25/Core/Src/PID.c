
#include "PID.h"

extern struct CONFIG config;

struct PIDController pid_starter;
struct PIDController pid_pump;
struct PIDController pid_t_lim;
struct PIDController pid_t4_start;
struct PIDController pid_t4_hall_err;

void PIDController_Init(struct PIDController *pid, float lim_min,float lim_max, float int_lim_min,float int_lim_max,
		               int16_t val_min, int16_t val_max) //инициализация
{

	pid->limMin=lim_min;
	pid->limMax=lim_max;

	pid->limMinInt=int_lim_min;
	pid->limMaxInt=int_lim_max;

	pid->minValue=val_min;
	pid->maxValue=val_max;

	pid->delta=pid->limMax-pid->limMin;
	pid->integrator = 0.0f;
	pid->prevError  = 0.0f;

	pid->differentiator  = 0.0f;
	pid->prevMeasurement = 0.0f;

	pid->out = 0.0f;
}

void PIDController_REInit(struct PIDController *pid,float p,float i,float d)//изменение кэфов
{
	pid->Kp_real=(p/FREQ_REGULATION);
	pid->Ki_real=(i/FREQ_REGULATION);
	pid->Kd_real=(d/FREQ_REGULATION);
}

void PIDController_Reset(struct PIDController *pid) //сброс всех  переменнных
{
	pid->delta=pid->limMax-pid->limMin;
	pid->integrator = 0.0f;
	pid->prevError  = 0.0f;

	pid->differentiator  = 0.0f;
	pid->prevMeasurement = 0.0f;

	pid->out = 0.0f;

	//pid->Kp_real=(pid->Kp/FREQ_REGULATION);
	//pid->Ki_real=(pid->Ki/FREQ_REGULATION);
	//pid->Kd_real=(pid->Kd/FREQ_REGULATION);

}


int PIDController_Update(struct PIDController *pid, float setpoint, float measurement, float table, int zero_point )//обновление пида
{
    float error = setpoint - measurement;//вычисление ошибки 500

    float proportional = pid->Kp_real * error;//пропорциональная составляющая 0.00001

    pid->integrator=pid->integrator+(pid->Ki_real*error);//расчет интегральной составляющей 0.000025

    if (pid->integrator > pid->limMaxInt)//контроль по выходу за пределы
    {
        pid->integrator = pid->limMaxInt;
    }
    else if (pid->integrator < pid->limMinInt)
    {
        pid->integrator = pid->limMinInt;
    }

    pid->differentiator =  pid->Kd_real * (measurement - pid->prevMeasurement);	//расчёт диф составлюящей

    pid->out =(pid->delta*((proportional + pid->integrator + pid->differentiator)+table))+zero_point;
 //  pid->out =(proportional + pid->integrator + pid->differentiator);
  //  pid->out = pid->out+pid->minValue;

    if (pid->out > pid->maxValue)
    {
        pid->out = pid->maxValue;
    }
    else if (pid->out < pid->minValue)
    {
        pid->out = pid->minValue;
    }

    pid->prevError       = error;
    pid->prevMeasurement = measurement;

    return pid->out;
}

void PID_setup()
{
	pid_starter.Kp=config.starter_P;
	pid_starter.Ki=config.starter_I;
	pid_starter.Kd=config.starter_D;

	pid_starter.limMinInt=STARTER_PID_LIM_MIN_INT;
	pid_starter.limMaxInt=STARTER_PID_LIM_MAX_INT;

	pid_starter.limMin=STARTER_PID_LIM_MIN;
	pid_starter.limMax=STARTER_PID_LIM_MAX;

	pid_starter.minValue=STARTER_MIN_VALUE;
	pid_starter.maxValue= STARTER_MAX_VALUE;

	//PIDController_Init(&pid_starter);


	pid_pump.Kp=config.main_P;
	pid_pump.Ki=config.main_I;
	pid_pump.Kd=config.main_D;

	pid_pump.limMinInt=PUMP_PID_LIM_MIN_INT;
	pid_pump.limMaxInt=PUMP_PID_LIM_MAX_INT;

	pid_pump.limMin=PUMP_PID_LIM_MIN;
	pid_pump.limMax=PUMP_PID_LIM_MAX;

	pid_pump.minValue=PUMP_MIN_VALUE;
	pid_pump.maxValue=PUMP_MAX_VALUE;

	//PIDController_Init(&pid_pump);



	pid_t_lim.Kp=config.t_lim_P;
	pid_t_lim.Ki=config.t_lim_I;
	pid_t_lim.Kd=config.t_lim_D;

	pid_t_lim.limMinInt=T_LIM_PID_LIM_MIN_INT;
	pid_t_lim.limMaxInt=T_LIM_PID_LIM_MAX_INT;

	pid_t_lim.limMin=T_LIM_PID_LIM_MIN;
	pid_t_lim.limMax=T_LIM_PID_LIM_MAX;

	pid_t_lim.minValue=T_LIM_MIN_VALUE;
	pid_t_lim.maxValue=T_LIM_MAX_VALUE;

	//PIDController_Init(&pid_t_lim);
}
