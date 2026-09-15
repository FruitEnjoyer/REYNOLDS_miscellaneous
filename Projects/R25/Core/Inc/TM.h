#ifndef _TM
#define _TM


#include "main.h"


void TM_complate();

struct Master_TM
{
	uint8_t prs_state;
	uint8_t adg_state;
	uint8_t gen_state;

	uint16_t hot_PWM;
	uint8_t hot_state;

	uint32_t next_setpoint;
	uint32_t relevant_setpoint;

	float t_real;
	float t_real_int;


	uint16_t engine_state;
	uint16_t engine_flag;

	uint8_t update_state_flag;

	uint8_t starter_move_count;
	uint8_t starter_restart_flag;

	uint32_t rotor_speed;
	uint32_t rotor_speed_old;

	uint16_t n1_median_count;

	float rotor_it_count;
	uint8_t rotor_it_update_flag;



	uint16_t pump_speed;

	float pump_it_count;

	uint16_t pump2_speed;

	float pump2_it_count;

	uint16_t pre_pump_counter;

	uint16_t work_count;
	uint16_t threshold_count;
	uint16_t flameout_work_count;
	uint16_t t_over_count;

	uint8_t tc_complate_flag;

	uint32_t t_lim_pid_val;





	uint16_t trotle;
	uint32_t rud;

	uint8_t valve1_curve_control_flag;
	uint8_t valve2_curve_control_flag;




	uint8_t adc1_complate_flag;
	uint8_t adc5_complate_flag;

	uint8_t stend_flag;

	float vref;
	float vref_k;
	float t_cristal;
	float t_gen;
	float fire_cur;

	float t_diod;
	float gen_volt;
	float bus_volt;
    float vref_int_cal;
	float valve1_cur;
	float valve2_cur;
	float plug_current;
	float pump_current;
	float starter_current;
	float fire_current;

	uint16_t ust_starter_curve;
	uint16_t ust_starter_pid;


	uint8_t flag_restsrt;

	uint16_t actuator_r1_pump;

	uint32_t n1_starter_off_correct;

	uint16_t u_pump_test_crit;

	uint8_t hall_err_flag;

	uint16_t trv_restart_correction;

	float hot_cap_tc;

	int16_t tg_correct_k;


	uint16_t crc16;
	uint32_t crc32;

	uint16_t su_485_counter;
};

extern struct Master_TM mtm;

#endif
