

#include "../Inc/REGULATION.h"

extern struct Master_TM mtm;
extern struct RS rs;

extern struct CAN can;

extern struct PIDController pid_starter;
extern struct PIDController pid_pump;
extern struct PIDController pid_t_lim;
extern struct PIDController pid_t4_start;
extern struct PIDController pid_t4_hall_err;

extern struct CONFIG config;

uint8_t Regulation_init()
{
	//uint8_t flag_time_threshold=0;


	// PIDController_Init (&pid_starter);
	// PIDController_Init (&pid_pump);
	 //PIDController_Init (&pid_t_lim);

	 return 0;
}



uint8_t Regulation()
{
	switch (mtm.engine_state)
	{
		case(INITIAL_STAGE)://базовое состояние

			switch(config.engine_type)
			{
				case(R40):
				{

				}
				break;
				case(R500):
					{
						if (mtm.update_state_flag)
						{
							mtm.update_state_flag=0;

							STARTER_PWM = ZERO_SETPOINT;
							PUMP_PWM = ZERO_POINT_PUMP;

							PLUG_OFF
							mtm.prs_state=0;

							ADG_OFF
							mtm.adg_state=0;



							mtm.hot_state=0;

							START_VALVE_OFF
							MAIN_VALVE_OFF




							PIDController_Reset (&pid_starter);
							PIDController_Reset (&pid_pump);
							PIDController_Reset (&pid_t_lim);
							PIDController_Reset (&pid_t4_start);
							PIDController_Reset (&pid_t4_hall_err);

							PIDController_REInit(&pid_starter,config.starter_P,config.starter_I,config.starter_D);
							PIDController_REInit(&pid_pump,config.main_P,config.main_I,config.main_D);
							PIDController_REInit(&pid_t_lim,config.t_lim_P,config.t_lim_I,config.t_lim_D);
							PIDController_REInit(&pid_t4_start,0.002,0.0002,0.00);
							PIDController_REInit(&pid_t4_hall_err,0.004,0.0004,0.00);

							mtm.starter_restart_flag=0;
							mtm.starter_move_count=0;
							mtm.relevant_setpoint=0;
							mtm.next_setpoint=0;



						}

					}
				break;
				case(R500_PRS_KBM):
					{
						if (mtm.update_state_flag)
						{
							mtm.update_state_flag=0;

							STARTER_PWM = ZERO_SETPOINT;
							PUMP_PWM = ZERO_POINT_PUMP;

							PLUG_OFF
							mtm.prs_state=0;

							ADG_OFF
							mtm.adg_state=0;



							mtm.hot_state=0;

							START_VALVE_OFF
							MAIN_VALVE_OFF




							PIDController_Reset (&pid_starter);
							PIDController_Reset (&pid_pump);
							PIDController_Reset (&pid_t_lim);

							PIDController_REInit(&pid_starter,config.starter_P,config.starter_I,config.starter_D);
							PIDController_REInit(&pid_pump,config.main_P,config.main_I,config.main_D);
							PIDController_REInit(&pid_t_lim,config.t_lim_P,config.t_lim_I,config.t_lim_D);

							mtm.starter_restart_flag=0;
							mtm.starter_move_count=0;
							mtm.relevant_setpoint=0;
							mtm.next_setpoint=0;



						}
					}
				break;
				case(R500_PRS_TRV):
					{
						if (mtm.update_state_flag)
						{
							mtm.update_state_flag=0;

							STARTER_PWM = ZERO_SETPOINT;
							PUMP_PWM = ZERO_POINT_PUMP;

							PLUG_OFF
							mtm.prs_state=0;

							ADG_OFF
							mtm.adg_state=0;



							mtm.hot_state=0;

							START_VALVE_OFF
							MAIN_VALVE_OFF





							PIDController_Reset (&pid_starter);
							PIDController_Reset (&pid_pump);
							PIDController_Reset (&pid_t_lim);
							PIDController_Reset (&pid_t4_start);
							PIDController_Reset (&pid_t4_hall_err);

							PIDController_REInit(&pid_starter,config.starter_P,config.starter_I,config.starter_D);
							PIDController_REInit(&pid_pump,config.main_P,config.main_I,config.main_D);
							PIDController_REInit(&pid_t_lim,config.t_lim_P,config.t_lim_I,config.t_lim_D);
							PIDController_REInit(&pid_t4_start,0.002,0.0002,0.00);
							PIDController_REInit(&pid_t4_hall_err,0.004,0.0004,0.00);

							mtm.starter_restart_flag=0;
							mtm.starter_move_count=0;
							mtm.relevant_setpoint=0;
							mtm.next_setpoint=0;



						}
					}
				break;
				case(R1):
					{
						if (mtm.update_state_flag)
						{
							mtm.update_state_flag=0;

							STARTER_PWM = ZERO_SETPOINT;
							PUMP_PWM = ZERO_POINT_PUMP;
							PUMP2_PWM=ZERO_POINT_PUMP2;

							PLUG_OFF
							mtm.prs_state=0;

							ADG_OFF
							mtm.adg_state=0;



							mtm.hot_state=0;

							START_VALVE_OFF
							MAIN_VALVE_OFF




							PIDController_Reset (&pid_starter);
							PIDController_Reset (&pid_pump);
							PIDController_Reset (&pid_t_lim);

							PIDController_REInit(&pid_starter,config.starter_P,config.starter_I,config.starter_D);
							PIDController_REInit(&pid_pump,config.main_P,config.main_I,config.main_D);
							PIDController_REInit(&pid_t_lim,config.t_lim_P,config.t_lim_I,config.t_lim_D);

							mtm.starter_restart_flag=0;
							mtm.starter_move_count=0;
							mtm.relevant_setpoint=0;
							mtm.next_setpoint=0;



						}

					}
				break;
			}

			if   ((mtm.t_real>config.t_vent) && (mtm.rotor_speed<5000)&&(rs.config_complate_flag))
			{
				if(mtm.work_count>10)
				{
					  mtm.engine_state=STAGE_VENT;
					  mtm.update_state_flag=1;
				}
				else
				{
					mtm.work_count++;
				}

			}
			else
			{
				mtm.work_count=0;
			}


		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(SLOWDOWN)://готовность к охлаждению
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;
				case(R500):
					{
						if (mtm.update_state_flag)
						{

							mtm.update_state_flag=0;



							STARTER_PWM = 0;
							PUMP_PWM = ZERO_POINT_PUMP;

							PLUG_OFF
							mtm.prs_state=0;

							ADG_OFF
							mtm.adg_state=0;



							mtm.hot_state=0;

							mtm.work_count=0;

							START_VALVE_OFF
							MAIN_VALVE_OFF

							PIDController_Reset (&pid_starter);
							PIDController_Reset (&pid_pump);
							PIDController_Reset (&pid_t_lim);

							mtm.starter_restart_flag=0;
							mtm.starter_move_count=0;
							mtm.relevant_setpoint=0;
							mtm.next_setpoint=0;
							mtm.work_count=0;

						}


						if (mtm.work_count>10)
						{
							STARTER_PWM = ZERO_SETPOINT;
						}
						else
						{
							mtm.work_count++;
						}

						if   (mtm.rotor_speed<5000)
						{
						  mtm.engine_state=INITIAL_STAGE;
						  mtm.update_state_flag=1;

						  mtm.engine_flag=0;
						}



					}
				break;
				case(R500_PRS_KBM):
					{
						if (mtm.update_state_flag)
						{

							mtm.update_state_flag=0;


							mtm.engine_flag=0;

							STARTER_PWM = 0;
							PUMP_PWM = ZERO_POINT_PUMP;

							PLUG_OFF
							mtm.prs_state=0;

							ADG_OFF
							mtm.adg_state=0;



							mtm.hot_state=0;

							mtm.work_count=0;

							START_VALVE_OFF
							MAIN_VALVE_OFF

							PIDController_Reset (&pid_starter);
							PIDController_Reset (&pid_pump);
							PIDController_Reset (&pid_t_lim);

							mtm.starter_restart_flag=0;
							mtm.starter_move_count=0;
							mtm.relevant_setpoint=0;
							mtm.next_setpoint=0;
							mtm.work_count=0;

						}


						if (mtm.work_count>10)
						{
							STARTER_PWM = ZERO_SETPOINT;
						}
						else
						{
							mtm.work_count++;
						}

						if   (mtm.rotor_speed<5000)
						{
						  mtm.engine_state=INITIAL_STAGE;
						  mtm.update_state_flag=1;

						  mtm.engine_flag=0;
						}

					}
				break;
				case(R500_PRS_TRV):
					{
						if (mtm.update_state_flag)
						{

							mtm.update_state_flag=0;

							mtm.engine_flag=0;

							STARTER_PWM = 0;
							PUMP_PWM = ZERO_POINT_PUMP;

							PLUG_OFF
							mtm.prs_state=0;

							ADG_OFF
							mtm.adg_state=0;



							mtm.hot_state=0;

							mtm.work_count=0;

							START_VALVE_OFF
							MAIN_VALVE_OFF

							PIDController_Reset (&pid_starter);
							PIDController_Reset (&pid_pump);
							PIDController_Reset (&pid_t_lim);

							mtm.starter_restart_flag=0;
							mtm.starter_move_count=0;
							mtm.relevant_setpoint=0;
							mtm.next_setpoint=0;
							mtm.work_count=0;

						}


						if (mtm.work_count>10)
						{
							STARTER_PWM = ZERO_SETPOINT;
						}
						else
						{
							mtm.work_count++;
						}

						if   (mtm.rotor_speed<5000)
						{
						  mtm.engine_state=INITIAL_STAGE;
						  mtm.update_state_flag=1;

						  mtm.engine_flag=0;
						}

					}
				break;
				case(R1):
					{
						if (mtm.update_state_flag)
						{

							mtm.update_state_flag=0;

							mtm.engine_flag=0;

							STARTER_PWM = 0;
							PUMP_PWM = ZERO_POINT_PUMP;
							PUMP2_PWM=ZERO_POINT_PUMP2;

							PLUG_OFF
							mtm.prs_state=0;

							ADG_OFF
							mtm.adg_state=0;



							mtm.hot_state=0;

							mtm.work_count=0;

							START_VALVE_OFF
							MAIN_VALVE_OFF

							PIDController_Reset (&pid_starter);
							PIDController_Reset (&pid_pump);
							PIDController_Reset (&pid_t_lim);

							mtm.starter_restart_flag=0;
							mtm.starter_move_count=0;
							mtm.relevant_setpoint=0;
							mtm.next_setpoint=0;
							mtm.work_count=0;

						}


						if (mtm.work_count>10)
						{
							STARTER_PWM = ZERO_SETPOINT;
						}
						else
						{
							mtm.work_count++;
						}

						if   (mtm.rotor_speed<5000)
						{
						  mtm.engine_state=INITIAL_STAGE;
						  mtm.update_state_flag=1;

						  mtm.engine_flag=0;
						}

					}
				break;




			}
			Overheating_Control_Start(30);
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case (START_1)://1 этап
			switch(config.engine_type)
			{
				case(R40):
				{

				}
				break;
				case(R500):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.next_setpoint=(config.n1_ignition*10)+1000;

						mtm.relevant_setpoint=1200;

						mtm.work_count=0;

						mtm.threshold_count=0;

						mtm.starter_move_count=0;

						pid_starter.integrator=(((float)config.starter_min)/1000.00);


						mtm.hot_state=1;

						GEN_OFF
						mtm.gen_state=0;

						mtm.n1_starter_off_correct=config.n1_starter_off*10*(mtm.bus_volt/27.00);

						// MAIN_VALVE_ON;

						// PUMP_PWM=ZERO_POINT_PUMP+config.pump_test_pwm;

						mtm.pre_pump_counter=0;

						STARTER_PWM=ZERO_POINT_STARTER_WORK;


					}

					if(mtm.pre_pump_counter>UZGA_PREPUMP_TIME)
					{
					  //PUMP_PWM = ZERO_POINT_PUMP;
					  //MAIN_VALVE_OFF;
					}

					mtm.pre_pump_counter++;

					if((mtm.work_count>40)&&(mtm.starter_move_count==0))//ход на перезапуск при нераскрутке ротора
					{
						mtm.work_count=0;

						STARTER_PWM=ZERO_POINT_STARTER_WORK;

						mtm.starter_restart_flag=1;
					}

					mtm.work_count++;

					if(mtm.rotor_speed>(config.n1_ignition*10))//переход в след. стэйт  счетчик =0,1 секунды
					{
						mtm.threshold_count++;//дописать процедуру сбора данных после активации защелки
						if(mtm.threshold_count>1)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=START_2;
						}
					}
					else
					{
						mtm.threshold_count=0;
					}

					if(mtm.engine_flag&C_CONTROL)//если отьебнула термопара
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}

					if ((mtm.rotor_speed>2000)&&(mtm.starter_move_count<10))//условие перехода на пид
					{
						mtm.engine_flag=mtm.engine_flag&(~START_NO);
						mtm.starter_move_count++;
					}

					if(mtm.starter_move_count>2)// если крутится переход на пид
					{
						mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
																  config.starter_rate,config.starter_rate);

						STARTER_PWM=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,mtm.rotor_speed,0,1000);
					}
					else
					{
						if(mtm.starter_restart_flag)//если перезапуск то плавное приращение уставки
						{
							if((STARTER_PWM+20)<=((config.starter_min)+1000))
							{
								STARTER_PWM= STARTER_PWM+20;
							}
							else
							{
								STARTER_PWM= ((config.starter_min)+1000);
							}
						}
						else//если первый заход то фиксированная уставка от конфига
						{
							STARTER_PWM=(config.starter_min)+1000;
						}
					}


					//MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					//START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

				}
				break;
				case(R500_PRS_KBM):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.next_setpoint=(config.n1_ignition*10)+1000;

						mtm.relevant_setpoint=1200;

						mtm.work_count=0;

						mtm.threshold_count=0;

						mtm.starter_move_count=0;

						pid_starter.integrator=(((float)config.starter_min)/1000.00);


						mtm.hot_state=1;

						GEN_OFF
						mtm.gen_state=0;


						MAIN_VALVE_ON;

						PUMP_PWM=ZERO_POINT_PUMP+config.pump_test_pwm;

						mtm.pre_pump_counter=0;

						STARTER_PWM=ZERO_POINT_STARTER_WORK;

					}

					if(mtm.pre_pump_counter>KBM_PREPUMP_TIME)
					{
						PUMP_PWM = ZERO_POINT_PUMP;
						MAIN_VALVE_OFF;
					}
					mtm.pre_pump_counter++;


					if((mtm.work_count>40)&&(mtm.starter_move_count==0))//ход на перезапуск при нераскрутке ротора
					{
						mtm.work_count=0;
						STARTER_PWM=ZERO_POINT_STARTER_WORK;

						mtm.starter_restart_flag=1;
					}
					mtm.work_count++;


					if(mtm.rotor_speed>(config.n1_ignition*10))//переход в след. стэйт  счетчик =0,1 секунды
					{
						mtm.threshold_count++;//дописать процедуру сбора данных после активации защелки
						if(mtm.threshold_count>1)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=START_2;
						}
					}
					else
					{
						mtm.threshold_count=0;
					}

					if(mtm.engine_flag&C_CONTROL)//если отьебнула термопара
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}

					if ((mtm.rotor_speed>2000)&&(mtm.starter_move_count<10))//условие перехода на пид
					{
						mtm.engine_flag=mtm.engine_flag&(~START_NO);
						mtm.starter_move_count++;
					}

					if(mtm.starter_move_count>2)// если крутится переход на пид
					{
						mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
																config.starter_rate,config.starter_rate);

						STARTER_PWM=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,mtm.rotor_speed,0,1000);
					}
					else
					{
						if(mtm.starter_restart_flag)//если перезапуск то плавное приращение уставки
						{
							if((STARTER_PWM+20)<=((config.starter_min)+1000))
							{
								STARTER_PWM= STARTER_PWM+20;
							}
							else
							{
								STARTER_PWM= ((config.starter_min)+1000);
							}
						}
						else//если первый заход то фиксированная уставка от конфига
						{
						STARTER_PWM=(config.starter_min)+1000;
						}

					}

					//MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					//START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);
				}
				break;
				case(R500_PRS_TRV):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.next_setpoint=(config.n1_ignition*10)+1000;

						mtm.relevant_setpoint=1200;

						mtm.work_count=0;

						mtm.threshold_count=0;

						mtm.starter_move_count=0;

						pid_starter.integrator=(((float)config.starter_min)/1000.00);


						mtm.hot_state=1;

						GEN_OFF
						mtm.gen_state=0;


						MAIN_VALVE_ON;

						PUMP_PWM=ZERO_POINT_PUMP+config.pump_test_pwm;

						mtm.pre_pump_counter=0;

						STARTER_PWM=ZERO_POINT_STARTER_WORK;
					}

					if(mtm.pre_pump_counter>KBM_PREPUMP_TIME)
					{
						PUMP_PWM = ZERO_POINT_PUMP;
						MAIN_VALVE_OFF;
					}
					mtm.pre_pump_counter++;

					if((mtm.work_count>40)&&(mtm.starter_move_count==0))//ход на перезапуск при нераскрутке ротора
					{
						mtm.work_count=0;
						STARTER_PWM=ZERO_POINT_STARTER_WORK;
						mtm.starter_restart_flag=1;
					}
					mtm.work_count++;

					if(mtm.rotor_speed>(config.n1_ignition*10))//переход в след. стэйт  счетчик =0,1 секунды
					{
						mtm.threshold_count++;//дописать процедуру сбора данных после активации защелки
						if(mtm.threshold_count>1)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=START_2;
						}
					}
					else
					{
						mtm.threshold_count=0;
					}

					if(mtm.engine_flag&C_CONTROL)//если отьебнула термопара
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}

					if ((mtm.rotor_speed>2000)&&(mtm.starter_move_count<10))//условие перехода на пид
					{
						mtm.engine_flag=mtm.engine_flag&(~START_NO);
						mtm.starter_move_count++;
					}

					if(mtm.starter_move_count>2)// если крутится переход на пид
					{
						mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
																config.starter_rate,config.starter_rate);

						STARTER_PWM=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,mtm.rotor_speed,0,1000);
					}
					else
					{
						if(mtm.starter_restart_flag)//если перезапуск то плавное приращение уставки
						{
							if((STARTER_PWM+20)<=((config.starter_min)+1000))
							{
								STARTER_PWM= STARTER_PWM+20;
							}
							else
							{
								STARTER_PWM= ((config.starter_min)+1000);
							}
						}
						else//если первый заход то фиксированная уставка от конфига
						{
							STARTER_PWM=(config.starter_min)+1000;
						}

					}

					//MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					//START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

				}
				break;
				case(R1):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.next_setpoint=6000;

						mtm.relevant_setpoint=1200;

						mtm.work_count=0;

						mtm.threshold_count=0;

						mtm.starter_move_count=0;

						pid_starter.integrator=(((float)config.starter_min)/1000.00);


						GEN_OFF
						mtm.gen_state=0;

						STARTER_PWM=ZERO_POINT_STARTER_WORK;
					}

					if((mtm.work_count>50)&&(mtm.starter_move_count==0))//ход на перезапуск при нераскрутке ротора
					{
						mtm.work_count=0;
						STARTER_PWM=ZERO_POINT_STARTER_WORK;

						mtm.starter_restart_flag=1;
					}
					mtm.work_count++;

					if(mtm.rotor_speed>(config.n1_ignition*10))//переход в след. стэйт  счетчик =0,1 секунды
					{
						ADG_ON;
						mtm.threshold_count++;//дописать процедуру сбора данных после активации защелки

						mtm.actuator_r1_pump=Pump_Curve(mtm.rotor_speed);

						PUMP_PWM=Get_next_setpoint((Pump_PWM_Correct (1250,mtm.bus_volt)),
												  PUMP_PWM,config.pump1_rate/10,config.pump1_rate/10);

						PUMP2_PWM=Get_next_setpoint((Pump_PWM_Correct_BIDIR (1200,mtm.bus_volt)),
												  PUMP2_PWM,config.pump2_rate/10,config.pump2_rate/10);

						if(mtm.threshold_count>1)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=START_2;
						}
					}

					else
					{
						mtm.threshold_count=0;
					}

					if(mtm.engine_flag&C_CONTROL)//если отьебнула термопара
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}

					if ((mtm.rotor_speed>2000)&&(mtm.starter_move_count<10))//условие перехода на пид
					{
						mtm.engine_flag=mtm.engine_flag&(~START_NO);
						mtm.starter_move_count++;
					}

					if(mtm.starter_move_count>2)// если крутится переход на пид
					{
						mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
															  config.starter_rate,config.starter_rate);

						STARTER_PWM=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,mtm.rotor_speed,0,1000);
					}
					else
					{
						if(mtm.starter_restart_flag)//если перезапуск то плавное приращение уставки
						{
							if((STARTER_PWM+20)<=((config.starter_min)+1000))
							{
								STARTER_PWM= STARTER_PWM+20;
							}
							else
							{
								STARTER_PWM= ((config.starter_min)+1000);
							}
						}
					  else//если первый заход то фиксированная уставка от конфига
					  {
						  STARTER_PWM=(config.starter_min)+1000;
					  }
					}
				}
				break;

			}
			Overheating_Control_Start(30);
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case (START_2)://2 этап
			switch(config.engine_type)
			{
				case(R40):
				{

				}
				break;
				case(R500):
					{
						if (mtm.update_state_flag)
						{
							mtm.update_state_flag=0;

							PLUG_ON
							mtm.prs_state=1;

							mtm.work_count=0;
							mtm.threshold_count=0;
						}

						if(mtm.t_real>=config.t_ignition)//переход в след. стэйт  счетчик =0,1 секунды
						{
							mtm.threshold_count++;
							if(mtm.threshold_count>20)
							{
								mtm.update_state_flag=1;
								mtm.engine_state=START_3;
							}
						}
						else
						{
							mtm.threshold_count=0;
						}
						if(derivative(mtm.t_real)<5)
						{
							if(mtm.work_count>250)//если 25 секунд нет розжига то переходим в перезапуск
							{
								mtm.update_state_flag=1;
								mtm.engine_state=SLOWDOWN;
								mtm.engine_flag=mtm.engine_flag|(IGN_FAIL);
							}
						}

						mtm.work_count++;

						mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
																	 config.starter_rate,config.starter_rate);

						mtm.ust_starter_pid=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,
															mtm.rotor_speed,0,1000);
						mtm.ust_starter_curve=Starter_Curve(mtm.rotor_speed);
						STARTER_PWM=(mtm.ust_starter_curve<=mtm.ust_starter_pid)?mtm.ust_starter_curve:mtm.ust_starter_pid;

						MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

						PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
														ZERO_POINT_PUMP_WORK),mtm.bus_volt));

						if(mtm.engine_flag&CC_HOT)//если превышение температуры падаем в слоу
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}

						if(mtm.engine_flag&C_CONTROL)//если отьебнула термомара
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}
					}
				break;
				case(R500_PRS_KBM):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						PLUG_ON
						mtm.prs_state=1;

						mtm.work_count=0;
						mtm.threshold_count=0;
					}

					if(mtm.t_real>=config.t_ignition)//переход в след. стэйт  счетчик =0,1 секунды
					{
						mtm.threshold_count++;
						if(mtm.threshold_count>1)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=START_3;
						}
					}
					else
					{
						mtm.threshold_count=0;
					}

					if(mtm.work_count>120)//если 10 секунд нет розжига то переходим в перезапуск
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
						mtm.engine_flag=mtm.engine_flag|(IGN_FAIL);
					}
					mtm.work_count++;


					mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
															 config.starter_rate,config.starter_rate);

					mtm.ust_starter_pid=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,
														mtm.rotor_speed,0,1000);
					mtm.ust_starter_curve=Starter_Curve(mtm.rotor_speed);
					STARTER_PWM=(mtm.ust_starter_curve<=mtm.ust_starter_pid)?mtm.ust_starter_curve:mtm.ust_starter_pid;

					MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

					PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
												ZERO_POINT_PUMP_WORK),mtm.bus_volt));

					if(mtm.engine_flag&CC_HOT)//если превышение температуры падаем в слоу
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}

					if(mtm.engine_flag&C_CONTROL)//если отьебнула термомара
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}

				}
				break;
				case(R500_PRS_TRV):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						PLUG_ON
						mtm.prs_state=1;

						mtm.work_count=0;
						mtm.threshold_count=0;
					}

					if(mtm.t_real>=config.t_ignition)//переход в след. стэйт  счетчик =0,1 секунды
					{
						mtm.threshold_count++;
						if(mtm.threshold_count>1)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=START_3;
						}
					}
					else
					{
						mtm.threshold_count=0;
					}

					if(mtm.work_count>120)//если 10 секунд нет розжига то переходим в перезапуск
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
						mtm.engine_flag=mtm.engine_flag|(IGN_FAIL);
					}
					mtm.work_count++;

					mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
															 config.starter_rate,config.starter_rate);

					mtm.ust_starter_pid=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,
														mtm.rotor_speed,0,1000);
					mtm.ust_starter_curve=Starter_Curve(mtm.rotor_speed);
					STARTER_PWM=(mtm.ust_starter_curve<=mtm.ust_starter_pid)?mtm.ust_starter_curve:mtm.ust_starter_pid;

					MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

					PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
												ZERO_POINT_PUMP_WORK),mtm.bus_volt));

					if(mtm.engine_flag&CC_HOT)//если превышение температуры падаем в слоу
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}

					if(mtm.engine_flag&C_CONTROL)//если отьебнула термомара
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}
				}
				break;
				case(R1):
				{

				}
				break;
			}
			Overheating_Control_Start(30);
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case (START_3)://3 этап
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;


				case(R500):
					{
					 if (mtm.update_state_flag)
					  {
						 mtm.update_state_flag=0;

						 mtm.next_setpoint=25000;

						 mtm.work_count=0;
						 mtm.flameout_work_count=0;


						 mtm.threshold_count=0;
					  }



					  if(mtm.rotor_speed>=18000)//переход в след. стэйт  счетчик =0,1 секунды
					  {
						  mtm.threshold_count++;
						  if(mtm.threshold_count>1)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=START_4;
						  }
					  }

					  else
					  {
						  mtm.threshold_count=0;
					  }


					  if(mtm.work_count>600)//рестарт по зависанию 60 сек
					  {
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
						mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
					  }
					  mtm.work_count++;


					  if(mtm.t_real<config.t_flameout)//незапуск по низкой температуре
					  {
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=SLOWDOWN;
							  mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  }

					  else
					  {
						  mtm.flameout_work_count=0;
					  }






					  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

					  STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);

					  PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
													ZERO_POINT_PUMP_WORK),mtm.bus_volt));


					  if( (mtm.engine_flag&CC_HOT))//перегрев
						  {
							   mtm.update_state_flag=1;
							   mtm.engine_state=SLOWDOWN;
						  }
						if(mtm.engine_flag&C_CONTROL)//отказ термопары
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=SLOWDOWN;
						  }

					}
				break;


				case(R500_PRS_KBM):
					{
						 if (mtm.update_state_flag)
						  {
							 mtm.update_state_flag=0;

							 mtm.next_setpoint=25000;

							 mtm.work_count=0;
							 mtm.flameout_work_count=0;


							 mtm.threshold_count=0;
						  }



						  if(mtm.rotor_speed>=18000)//переход в след. стэйт  счетчик =0,1 секунды
						  {
							  mtm.threshold_count++;
							  if(mtm.threshold_count>1)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=START_4;
							  }
						  }

						  else
						  {
							  mtm.threshold_count=0;
						  }


						  if(mtm.work_count>600)//рестарт по зависанию 60 сек
						  {
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
						  mtm.work_count++;


						  if(mtm.t_real<config.t_flameout)//незапуск по низкой температуре
						  {
							  mtm.flameout_work_count++;
							  if(mtm.flameout_work_count>30)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=SLOWDOWN;
								  mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
							  }
						  }

						  else
						  {
							  mtm.flameout_work_count=0;
						  }






						  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

						  STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);

						  PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
														ZERO_POINT_PUMP_WORK),mtm.bus_volt));


					  if( (mtm.engine_flag&CC_HOT))//перегрев
						  {
							   mtm.update_state_flag=1;
							   mtm.engine_state=SLOWDOWN;
						  }
						if(mtm.engine_flag&C_CONTROL)//отказ термопары
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=SLOWDOWN;
						  }

					}
				break;

				case(R500_PRS_TRV):
					{
					 if (mtm.update_state_flag)
					  {
						 mtm.update_state_flag=0;

						 mtm.next_setpoint=25000;

						 mtm.work_count=0;
						 mtm.flameout_work_count=0;


						 mtm.threshold_count=0;
					  }



					  if(mtm.rotor_speed>=18000)//переход в след. стэйт  счетчик =0,1 секунды
					  {
						  mtm.threshold_count++;
						  if(mtm.threshold_count>1)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=START_4;
						  }
					  }

					  else
					  {
						  mtm.threshold_count=0;
					  }


					  if(mtm.work_count>600)//рестарт по зависанию 60 сек
					  {
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
						mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
					  }
					  mtm.work_count++;


					  if(mtm.t_real<config.t_flameout)//незапуск по низкой температуре
					  {
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=SLOWDOWN;
							  mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  }

					  else
					  {
						  mtm.flameout_work_count=0;
					  }






					  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

					  STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);

					  PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
													ZERO_POINT_PUMP_WORK),mtm.bus_volt));


					  if( (mtm.engine_flag&CC_HOT))//перегрев
						  {
							   mtm.update_state_flag=1;
							   mtm.engine_state=SLOWDOWN;
						  }
						if(mtm.engine_flag&C_CONTROL)//отказ термопары
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=SLOWDOWN;
						  }

					}
				break;


				case(R1):
					{

					 if (mtm.update_state_flag)
					  {
						 mtm.update_state_flag=0;

						 mtm.next_setpoint=25000;

						 mtm.work_count=0;
						 mtm.flameout_work_count=0;


						 mtm.threshold_count=0;
						PLUG_OFF;
					  }



					  if(mtm.rotor_speed>=18000)//переход в след. стэйт  счетчик =0,1 секунды
					  {
						  mtm.threshold_count++;
						  if(mtm.threshold_count>1)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=START_4;
						  }
					  }

					  else
					  {
						  mtm.threshold_count=0;
					  }


					  if(mtm.work_count>600)//рестарт по зависанию 60 сек
					  {
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
						mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
					  }
					  mtm.work_count++;


					  if(mtm.t_real<config.t_flameout)//незапуск по низкой температуре
					  {
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=SLOWDOWN;
							  mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  }

					  else
					  {
						  mtm.flameout_work_count=0;
					  }







					  STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);

					  mtm.actuator_r1_pump=Pump_Curve(mtm.rotor_speed);

					  PUMP_PWM=Get_next_setpoint((Pump_PWM_Correct (((Valve_Curve(mtm.actuator_r1_pump))+
												  ZERO_POINT_PUMP_WORK),mtm.bus_volt)),
												  PUMP_PWM,config.pump1_rate/10,config.pump1_rate/10);

					  PUMP2_PWM=Get_next_setpoint((Pump_PWM_Correct_BIDIR (((Start_Valve_Curve(mtm.actuator_r1_pump))+
												  ZERO_POINT_PUMP_WORK),mtm.bus_volt)),
												  PUMP2_PWM,config.pump2_rate/10,config.pump2_rate/10);


					  if( (mtm.engine_flag&CC_HOT))//перегрев
						  {
							   mtm.update_state_flag=1;
							   mtm.engine_state=SLOWDOWN;
						  }
						if(mtm.engine_flag&C_CONTROL)//отказ термопары
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=SLOWDOWN;
						  }




					}
				break;


			}
			Overheating_Control_Start(30);
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case (START_4)://4этап
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;


				case(R500):
					{
					if (mtm.update_state_flag)
						{
						mtm.update_state_flag=0;

						mtm.flameout_work_count=0;
						mtm.threshold_count=0;

						}
						//if(mtm.rotor_speed>=( mtm.n1_starter_off_correct))//переход в след. стэйт  счетчик =0,1 секунды
						if(mtm.rotor_speed>=(config.n1_starter_off*10))//переход в след. стэйт  счетчик =0,1 секунды
						{
						  mtm.threshold_count++;
						  if(mtm.threshold_count>1)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=START_5;
						  }
						}

						else
						{
						  mtm.threshold_count=0;
						}


						if(mtm.work_count>600)
						{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
						mtm.engine_flag=mtm.engine_state|(FLAMEOUT);
						}
						mtm.work_count++;


						if(mtm.t_real<config.t_flameout)
						{
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
						  {
								mtm.update_state_flag=1;
								mtm.engine_state=SLOWDOWN;
								mtm.engine_flag=mtm.engine_state|(FLAMEOUT);
						  }
						}

						else
						{
						  mtm.flameout_work_count=0;
						}



						MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);


						STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);
						PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
													  ZERO_POINT_PUMP_WORK),mtm.bus_volt));

						if( (mtm.engine_flag&CC_HOT))//перегрев
						{
						  mtm.update_state_flag=1;
						  mtm.engine_state=SLOWDOWN;
						}

						if(mtm.engine_flag&C_CONTROL)//отказ термопары
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}

					}
				break;


				case(R500_PRS_KBM):
					{
					if (mtm.update_state_flag)
						{
						mtm.update_state_flag=0;

						mtm.flameout_work_count=0;
						mtm.threshold_count=0;

						}

						if(mtm.rotor_speed>=(config.n1_starter_off*10))//переход в след. стэйт  счетчик =0,1 секунды
						{
						  mtm.threshold_count++;
						  if(mtm.threshold_count>1)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=START_5;
						  }
						}

						else
						{
						  mtm.threshold_count=0;
						}


						if(mtm.work_count>600)
						{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
						mtm.engine_flag=mtm.engine_state|(FLAMEOUT);
						}
						mtm.work_count++;


						if(mtm.t_real<config.t_flameout)
						{
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
						  {
								mtm.update_state_flag=1;
								mtm.engine_state=SLOWDOWN;
								mtm.engine_flag=mtm.engine_state|(FLAMEOUT);
						  }
						}

						else
						{
						  mtm.flameout_work_count=0;
						}



						MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);


						STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);
						PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
													  ZERO_POINT_PUMP_WORK),mtm.bus_volt));

						if( (mtm.engine_flag&CC_HOT))//перегрев
						{
						  mtm.update_state_flag=1;
						  mtm.engine_state=SLOWDOWN;
						}

						if(mtm.engine_flag&C_CONTROL)//отказ термопары
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}

					}
				break;

				case(R500_PRS_TRV):
					{
					if (mtm.update_state_flag)
						{
						mtm.update_state_flag=0;

						mtm.flameout_work_count=0;
						mtm.threshold_count=0;

						}

						if(mtm.rotor_speed>=(config.n1_starter_off*10))//переход в след. стэйт  счетчик =0,1 секунды
						{
						  mtm.threshold_count++;
						  if(mtm.threshold_count>1)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=START_5;
						  }
						}

						else
						{
						  mtm.threshold_count=0;
						}


						if(mtm.work_count>600)
						{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
						mtm.engine_flag=mtm.engine_state|(FLAMEOUT);
						}
						mtm.work_count++;


						if(mtm.t_real<config.t_flameout)
						{
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
						  {
								mtm.update_state_flag=1;
								mtm.engine_state=SLOWDOWN;
								mtm.engine_flag=mtm.engine_state|(FLAMEOUT);
						  }
						}

						else
						{
						  mtm.flameout_work_count=0;
						}



						MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);


						STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);
						PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
													  ZERO_POINT_PUMP_WORK),mtm.bus_volt));

						if( (mtm.engine_flag&CC_HOT))//перегрев
						{
						  mtm.update_state_flag=1;
						  mtm.engine_state=SLOWDOWN;
						}

						if(mtm.engine_flag&C_CONTROL)//отказ термопары
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}

					}
				break;


				case(R1):
					{
					if (mtm.update_state_flag)
						{
						mtm.update_state_flag=0;

						mtm.flameout_work_count=0;
						mtm.threshold_count=0;

						}

						if(mtm.rotor_speed>=(config.n1_starter_off*10))//переход в след. стэйт  счетчик =0,1 секунды
						{
						  mtm.threshold_count++;
						  if(mtm.threshold_count>1)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=START_5;
						  }
						}

						else
						{
						  mtm.threshold_count=0;
						}


						if(mtm.work_count>600)
						{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
						mtm.engine_flag=mtm.engine_state|(FLAMEOUT);
						}
						mtm.work_count++;


						if(mtm.t_real<config.t_flameout)
						{
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
						  {
								mtm.update_state_flag=1;
								mtm.engine_state=SLOWDOWN;
								mtm.engine_flag=mtm.engine_state|(FLAMEOUT);
						  }
						}

						else
						{
						  mtm.flameout_work_count=0;
						}





						STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);


						mtm.actuator_r1_pump=Pump_Curve(mtm.rotor_speed);

						  PUMP_PWM=Get_next_setpoint((Pump_PWM_Correct (((Valve_Curve(mtm.actuator_r1_pump))+
													  ZERO_POINT_PUMP_WORK),mtm.bus_volt)),
													  PUMP_PWM,config.pump1_rate/10,config.pump1_rate/10);

						  PUMP2_PWM=Get_next_setpoint((Pump_PWM_Correct_BIDIR (((Start_Valve_Curve(mtm.actuator_r1_pump))+
													  ZERO_POINT_PUMP_WORK),mtm.bus_volt)),
													  PUMP2_PWM,config.pump2_rate/10,config.pump2_rate/10);


						if( (mtm.engine_flag&CC_HOT))//перегрев
						{
						  mtm.update_state_flag=1;
						  mtm.engine_state=SLOWDOWN;
						}

						if(mtm.engine_flag&C_CONTROL)//отказ термопары
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}
					}
				break;


			}
			Overheating_Control_Start(30);
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case (START_5)://5 этап
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;
				case(R500):
					{
					if (mtm.update_state_flag)
						  {
							mtm.update_state_flag=0;

							STARTER_PWM=0;

							PLUG_OFF;
							mtm.prs_state=0;

							mtm.flameout_work_count=0;
							mtm.threshold_count=0;
						  }

					if(mtm.rotor_speed>=(config.n1_operation*10))//переход в след. стэйт  счетчик =0,1 секунды
						  {
							  mtm.threshold_count++;
							  if(mtm.threshold_count>1)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=OPERATION;
							  }
						  }

					 else
						  {
							  mtm.threshold_count=0;
						  }


					 if(mtm.work_count>600)
						  {
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  mtm.work_count++;


					  if(mtm.t_real<config.t_flameout)
					  {
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=SLOWDOWN;
							  mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  }

					  else
					  {
						  mtm.flameout_work_count=0;
					  }




					  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);


					  uint16_t ust=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
								  ZERO_POINT_PUMP_WORK),mtm.bus_volt))+
								((PIDController_Update(&pid_t4_start,mtm.t_real, 700,0,0))*(-1));
					  if(ust<2000)
					  {
						  PUMP_PWM=ust;
					  }

					  else
					  {
						  PUMP_PWM=2000;
					  }



						if( (mtm.engine_flag&CC_HOT))//перегрев
						{
						  mtm.update_state_flag=1;
						  mtm.engine_state=SLOWDOWN;
						}

						if(mtm.engine_flag&C_CONTROL)//отказ термопары
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}



					}
				break;


				case(R500_PRS_KBM):
					{
					if (mtm.update_state_flag)
						  {
							mtm.update_state_flag=0;

							STARTER_PWM=0;

							PLUG_OFF;
							mtm.prs_state=0;

							mtm.flameout_work_count=0;
							mtm.threshold_count=0;
						  }

					if(mtm.rotor_speed>=(config.n1_operation*10))//переход в след. стэйт  счетчик =0,1 секунды
						  {
							  mtm.threshold_count++;
							  if(mtm.threshold_count>1)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=OPERATION;
							  }
						  }

					 else
						  {
							  mtm.threshold_count=0;
						  }


					 if(mtm.work_count>600)
						  {
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  mtm.work_count++;


					  if(mtm.t_real<config.t_flameout)
					  {
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=SLOWDOWN;
							  mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  }

					  else
					  {
						  mtm.flameout_work_count=0;
					  }




					  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);



					  uint16_t ust=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
													  ZERO_POINT_PUMP_WORK),mtm.bus_volt))+
													((PIDController_Update(&pid_t4_start,mtm.t_real, 700,0,0))*(-1));
					  if(ust<2000)
					  {
						  PUMP_PWM=ust;
					  }

					  else
					  {
						  PUMP_PWM=2000;
					  }



						if( (mtm.engine_flag&CC_HOT))//перегрев
						{
						  mtm.update_state_flag=1;
						  mtm.engine_state=SLOWDOWN;
						}

						if(mtm.engine_flag&C_CONTROL)//отказ термопары
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}

					}
				break;

				case(R500_PRS_TRV):
					{
					if (mtm.update_state_flag)
						  {
							mtm.update_state_flag=0;

							STARTER_PWM=0;

							PLUG_OFF;
							mtm.prs_state=0;

							mtm.flameout_work_count=0;
							mtm.threshold_count=0;
						  }

					if(mtm.rotor_speed>=(config.n1_operation*10))//переход в след. стэйт  счетчик =0,1 секунды
						  {
							  mtm.threshold_count++;
							  if(mtm.threshold_count>1)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=OPERATION;
							  }
						  }

					 else
						  {
							  mtm.threshold_count=0;
						  }


					 if(mtm.work_count>600)
						  {
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  mtm.work_count++;


					  if(mtm.t_real<config.t_flameout)
					  {
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=SLOWDOWN;
							  mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  }

					  else
					  {
						  mtm.flameout_work_count=0;
					  }




					  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);




					  PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
													  ZERO_POINT_PUMP_WORK),mtm.bus_volt));


						if( (mtm.engine_flag&CC_HOT))//перегрев
						{
						  mtm.update_state_flag=1;
						  mtm.engine_state=SLOWDOWN;
						}

						if(mtm.engine_flag&C_CONTROL)//отказ термопары
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}



					}
				break;


				case(R1):
					{
					if (mtm.update_state_flag)
						  {
							mtm.update_state_flag=0;

							STARTER_PWM=0;


							mtm.prs_state=0;

							mtm.flameout_work_count=0;
							mtm.threshold_count=0;
						  }

					if(mtm.rotor_speed>=(config.n1_operation*10))//переход в след. стэйт  счетчик =0,1 секунды
						  {
							  mtm.threshold_count++;
							  if(mtm.threshold_count>1)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=OPERATION;
							  }
						  }

					 else
						  {
							  mtm.threshold_count=0;
						  }


					 if(mtm.work_count>600)
						  {
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  mtm.work_count++;


					  if(mtm.t_real<config.t_flameout)
					  {
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=SLOWDOWN;
							  mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  }

					  else
					  {
						  mtm.flameout_work_count=0;
					  }


						mtm.actuator_r1_pump=Pump_Curve(mtm.rotor_speed);

						  PUMP_PWM=Get_next_setpoint((Pump_PWM_Correct (((Valve_Curve(mtm.actuator_r1_pump))+
													  ZERO_POINT_PUMP_WORK),mtm.bus_volt)),
													  PUMP_PWM,config.pump1_rate/10,config.pump1_rate/10);

						  PUMP2_PWM=Get_next_setpoint((Pump_PWM_Correct_BIDIR (((Start_Valve_Curve(mtm.actuator_r1_pump))+
													  ZERO_POINT_PUMP_WORK),mtm.bus_volt)),
													  PUMP2_PWM,config.pump2_rate/10,config.pump2_rate/10);





						if( (mtm.engine_flag&CC_HOT))//перегрев
						{
						  mtm.update_state_flag=1;
						  mtm.engine_state=SLOWDOWN;
						}

						if(mtm.engine_flag&C_CONTROL)//отказ термопары
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}

					}
				break;


		}
			Overheating_Control_Start(30);
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(OPERATION)://работа
			switch(config.engine_type)
			{
				case(R40):
				{

				}
				break;
				case(R500):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.rud=config.n1_min*10;//////////////////////


						mtm.flameout_work_count=0;
						mtm.relevant_setpoint=config.n1_min*10;

						mtm.hot_state=0;

						pid_pump.integrator=((float)PUMP_PWM-1000)/pid_pump.delta;

						mtm.work_count=0;

						mtm.hall_err_flag=0;
					}

					if (mtm.hall_err_flag)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}
					else
					{
						if(mtm.work_count==30)
						{
							GEN_ON
							mtm.gen_state=1;
						}
						else if (mtm.work_count<32)
						{
							mtm.work_count++;
						}

						if(mtm.rotor_speed>BEGIN_T_LIM_PID)
						{
							mtm.t_lim_pid_val=PIDController_Update(&pid_t_lim,mtm.t_real, config.t_PID_lim,0,0);
						}
						else
						{
							mtm.t_lim_pid_val=0;
							PIDController_Reset(&pid_t_lim);
						}

						mtm.relevant_setpoint=Get_next_setpoint(mtm.rud,mtm.relevant_setpoint,
															   (config.rate_up*10),(config.rate_down*10));

						MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);


						PUMP_PWM= PIDController_Update(&pid_pump, (mtm.relevant_setpoint-mtm.t_lim_pid_val),
																 mtm.rotor_speed,0,ZERO_POINT_PUMP_WORK);

						if(mtm.rotor_speed>config.n1_overspeed*10)//превышение максимальных оборотов
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
							mtm.engine_flag=mtm.engine_flag|(N1_HIGH);
						}

						if((mtm.t_real<config.t_flameout)||(mtm.rotor_speed<30000))//погасла камера
						{
							mtm.flameout_work_count++;
							if(mtm.flameout_work_count>30)
							{
								mtm.update_state_flag=1;
								mtm.engine_state=SLOWDOWN;
								mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
							}
						}
						else
						{
							mtm.flameout_work_count=0;
						}

						if(mtm.engine_flag&(TG_HIGH))
						{

							 mtm.update_state_flag=1;
							 mtm.engine_state=SLOWDOWN;

						}

						if(mtm.engine_flag&C_CONTROL)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}
					}

				}
				break;
				case(R500_PRS_KBM):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.rud=config.n1_min*10;//////////////////////

						mtm.flameout_work_count=0;

						mtm.relevant_setpoint=config.n1_min*10;

						mtm.hot_state=0;

						pid_pump.integrator=((float)PUMP_PWM-1000)/pid_pump.delta;

						mtm.work_count=0;

						mtm.hall_err_flag=0;
					}

					if (mtm.hall_err_flag)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}

					else
					{

						if(mtm.work_count==30)
						{
							GEN_ON
							mtm.gen_state=1;
						}
						else if (mtm.work_count<32)
						{
							mtm.work_count++;
						}

						if(mtm.rotor_speed>BEGIN_T_LIM_PID)
						{
							mtm.t_lim_pid_val=PIDController_Update(&pid_t_lim,mtm.t_real, config.t_PID_lim,0,0);
						}
						else
						{
						  mtm.t_lim_pid_val=0;
						  PIDController_Reset(&pid_t_lim);
						}

						mtm.relevant_setpoint=Get_next_setpoint(mtm.rud,mtm.relevant_setpoint,
																 (config.rate_up*10),(config.rate_down*10));

						MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

						PUMP_PWM= PIDController_Update(&pid_pump, (mtm.relevant_setpoint-mtm.t_lim_pid_val),
																 mtm.rotor_speed,0,ZERO_POINT_PUMP_WORK);

						if(mtm.rotor_speed>config.n1_overspeed*10)//превышение максимальных оборотов
						{
						   mtm.update_state_flag=1;
						   mtm.engine_state=SLOWDOWN;
						   mtm.engine_flag=mtm.engine_flag|(N1_HIGH);

						}


						if((mtm.t_real<config.t_flameout)||(mtm.rotor_speed<30000))//погасла камера
						{
							mtm.flameout_work_count++;
							if(mtm.flameout_work_count>30)
							{
								mtm.update_state_flag=1;
								mtm.engine_state=SLOWDOWN;
								mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
							}
						}

						else
						{
							mtm.flameout_work_count=0;
						}

						if(mtm.engine_flag&(TG_HIGH))
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}

						if(mtm.engine_flag&C_CONTROL)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}
					}
				}
				break;
				case(R500_PRS_TRV):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.rud=config.n1_min*10;//////////////////////


						mtm.flameout_work_count=0;
						mtm.relevant_setpoint=config.n1_min*10;

						mtm.hot_state=0;

						pid_pump.integrator=((float)PUMP_PWM-1000)/pid_pump.delta;

						mtm.work_count=0;

						mtm.hall_err_flag=0;
					}

					if (mtm.hall_err_flag)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}
					else
					{
						if(mtm.work_count==30)
						{
							GEN_ON
							mtm.gen_state=1;
						}
						else if (mtm.work_count<32)
						{
							mtm.work_count++;
						}

						if(mtm.rotor_speed>BEGIN_T_LIM_PID)
						{
							mtm.t_lim_pid_val=PIDController_Update(&pid_t_lim,mtm.t_real, config.t_PID_lim,0,0);
						}
						else
						{
							mtm.t_lim_pid_val=0;
							PIDController_Reset(&pid_t_lim);
						}

						mtm.relevant_setpoint=Get_next_setpoint(mtm.rud,mtm.relevant_setpoint,
																 (config.rate_up*10),(config.rate_down*10));

						MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

						PUMP_PWM= PIDController_Update(&pid_pump, (mtm.relevant_setpoint-mtm.t_lim_pid_val),
																 mtm.rotor_speed,0,ZERO_POINT_PUMP_WORK);

						if(mtm.rotor_speed>config.n1_overspeed*10)//превышение максимальных оборотов
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
							mtm.engine_flag=mtm.engine_flag|(N1_HIGH);
						}

						if((mtm.t_real<config.t_flameout)||(mtm.rotor_speed<30000))//погасла камера
						{
							mtm.flameout_work_count++;
							if(mtm.flameout_work_count>30)
							{
								mtm.update_state_flag=1;
								mtm.engine_state=SLOWDOWN;
								mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
							}
						}
						else
						{
							mtm.flameout_work_count=0;
						}

						if(mtm.engine_flag&(TG_HIGH))
						{
							 mtm.update_state_flag=1;
							 mtm.engine_state=SLOWDOWN;
						}

						if(mtm.engine_flag&C_CONTROL)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}
					}
				}
				break;
				case(R1):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.rud=config.n1_min*10;//////////////////////


						mtm.flameout_work_count=0;

						mtm.relevant_setpoint=config.n1_min*10;

						mtm.hot_state=0;

						pid_pump.integrator=((float)mtm.actuator_r1_pump)/100;

						mtm.work_count=0;

						mtm.hall_err_flag=0;
					}

					if (mtm.hall_err_flag)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}
					else
					{
						if(mtm.rotor_speed>BEGIN_T_LIM_PID)
						{
							mtm.t_lim_pid_val=PIDController_Update(&pid_t_lim,mtm.t_real, config.t_PID_lim,0,0);
						}
						else
						{
							mtm.t_lim_pid_val=0;
							PIDController_Reset(&pid_t_lim);
						}

						mtm.relevant_setpoint=Get_next_setpoint(mtm.rud,mtm.relevant_setpoint,
														(config.rate_up*10),(config.rate_down*10));

						mtm.actuator_r1_pump=((PIDController_Update(&pid_pump, (mtm.relevant_setpoint-mtm.t_lim_pid_val),
												mtm.rotor_speed,0,ZERO_POINT_PUMP_WORK))-1000)/10;

						PUMP_PWM=Get_next_setpoint((Pump_PWM_Correct (((Valve_Curve(mtm.actuator_r1_pump))+
												  ZERO_POINT_PUMP_WORK),mtm.bus_volt)),
												  PUMP_PWM,config.pump1_rate/10,config.pump1_rate/10);

						PUMP2_PWM=Get_next_setpoint((Pump_PWM_Correct_BIDIR (((Start_Valve_Curve(mtm.actuator_r1_pump))+
												  ZERO_POINT_PUMP_WORK),mtm.bus_volt)),
												  PUMP2_PWM,config.pump2_rate/10,config.pump2_rate/10);


						if(mtm.rotor_speed>config.n1_overspeed*10)//превышение максимальных оборотов
						{
						   mtm.update_state_flag=1;
						   mtm.engine_state=SLOWDOWN;
						   mtm.engine_flag=mtm.engine_flag|(N1_HIGH);
						}


						if((mtm.t_real<config.t_flameout)||(mtm.rotor_speed<25000))//погасла камера
						{
							mtm.flameout_work_count++;
							if(mtm.flameout_work_count>30)
							{
								mtm.update_state_flag=1;
								mtm.engine_state=SLOWDOWN;
								mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
							}
						}
						else
						{
							mtm.flameout_work_count=0;
						}

						if(mtm.engine_flag&(TG_HIGH))
						{
							 mtm.update_state_flag=1;
							 mtm.engine_state=SLOWDOWN;
						}

						if(mtm.engine_flag&C_CONTROL)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=SLOWDOWN;
						}
					}
				}
			break;
			}
			Overheating_Control_Operation(30);
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(RESTART)://перезапуск
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;


				case(R500):
					{


					}
				break;


				case(R500_PRS_KBM):
					{

					}
				break;

				case(R500_PRS_TRV):
					{

					}
				break;


				case(R1):
					{

					}
				break;


			}

		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(PRE_PUMP)://боевой
		switch(config.engine_type)
		{
			case(R40):
				{

				}
			break;


			case(R500):
				{
				 if (mtm.update_state_flag)
					  {
					 	  mtm.update_state_flag=0;

						  mtm.work_count=0;


						  PUMP_PWM=ZERO_POINT_PUMP+(config.pump_test_pwm);
						  START_VALVE_ON
						  MAIN_VALVE_ON
					  }



					  if(mtm.work_count>10)
						  {

							  PUMP_PWM  = ZERO_POINT_PUMP;
							  START_VALVE_OFF
							  MAIN_VALVE_OFF
							  mtm.engine_state=FLY_START_1;
							  mtm.update_state_flag=1;

						  }
					  mtm.work_count++;


				}
			break;


			case(R500_PRS_KBM):
				{

				}
			break;

			case(R500_PRS_TRV):
				{

				}
			break;


			case(R1):
				{

				}
			break;


		}

		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case (FLY_START_1):
			switch(config.engine_type)
			{
				case(R40):
				{

				}
				break;
				case(R500):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.next_setpoint=(config.n1_ignition*10)+1000;

						mtm.relevant_setpoint=1200;

						mtm.work_count=0;

						mtm.threshold_count=0;

						mtm.starter_move_count=0;

						pid_starter.integrator=(((float)config.starter_min)/1000.00);

						mtm.hot_state=1;
						GEN_OFF
						mtm.gen_state=0;

						//MAIN_VALVE_ON;

						//PUMP_PWM=ZERO_POINT_PUMP+config.pump_test_pwm;

						mtm.pre_pump_counter=0;

						STARTER_PWM=ZERO_POINT_STARTER_WORK;
					}

					if(mtm.pre_pump_counter>UZGA_PREPUMP_TIME)
					{
						// PUMP_PWM = ZERO_POINT_PUMP;
						//MAIN_VALVE_OFF;
					}
					mtm.pre_pump_counter++;


					if((mtm.work_count>40)&&(mtm.starter_move_count==0))//ход на перезапуск при нераскрутке ротора
					{
						mtm.work_count=0;
						STARTER_PWM=ZERO_POINT_STARTER_WORK;

						mtm.starter_restart_flag=1;
					}
					mtm.work_count++;


					if(mtm.rotor_speed>(config.n1_ignition*10))//переход в след. стэйт  счетчик =0,1 секунды
					{
						mtm.threshold_count++;
						if(mtm.threshold_count>1)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=FLY_START_2;
						}
					}
					else
					{
						mtm.threshold_count=0;
					}

					if(mtm.engine_flag&C_CONTROL)//если отьебнула термопара
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}

					if ((mtm.rotor_speed>2000)&&(mtm.starter_move_count<10))//условие перехода на пид
					{
						mtm.engine_flag=mtm.engine_flag&(~START_NO);
						mtm.starter_move_count++;
					}

					if(mtm.starter_move_count>2)// если крутится переход на пид
					{
						mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
															  config.starter_rate,config.starter_rate);

						STARTER_PWM=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,mtm.rotor_speed,0,1000);
					}
					else
					{
						if(mtm.starter_restart_flag)//если перезапуск то плавное приращение уставки
						{
							if((STARTER_PWM+20)<=((config.starter_min)+1000))
							{
								STARTER_PWM= STARTER_PWM+20;
							}
							else
							{
								STARTER_PWM= ((config.starter_min)+1000);
							}
						}
					  else//если первый заход то фиксированная уставка от конфига
					  {
						  STARTER_PWM=(config.starter_min)+1000;
					  }
					}


					//MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					//START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);
				}
				break;
				case(R500_PRS_KBM):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.next_setpoint=(config.n1_ignition*10)+1000;

						mtm.relevant_setpoint=1200;

						mtm.work_count=0;

						mtm.threshold_count=0;

						mtm.starter_move_count=0;

						pid_starter.integrator=(((float)config.starter_min)/1000.00);

						mtm.hot_state=1;

						ADG_ON;

						GEN_OFF
						mtm.gen_state=0;


						MAIN_VALVE_ON;

						PUMP_PWM=ZERO_POINT_PUMP+config.pump_test_pwm;

						mtm.pre_pump_counter=0;

						STARTER_PWM=ZERO_POINT_STARTER_WORK;
					}

					if(mtm.pre_pump_counter>KBM_PREPUMP_TIME)
					{
						PUMP_PWM = ZERO_POINT_PUMP;
						MAIN_VALVE_OFF;
					}
					mtm.pre_pump_counter++;

					if((mtm.work_count>100)&&(mtm.starter_move_count==0))//ход на перезапуск при нераскрутке ротора
					{
						mtm.work_count=0;
						STARTER_PWM=ZERO_POINT_STARTER_WORK;
						mtm.starter_restart_flag=1;
					}
					mtm.work_count++;


					if((mtm.rotor_speed>(config.n1_ignition*10))&&(can.activate_prs_flag))//переход в след. стэйт  счетчик =0,1
																						//секунды и при поднятом флаге поднимается при приходе 5 раз команды на запуск двигла по кану
					{
						mtm.threshold_count++;
						if(mtm.threshold_count>1)
						{
						  mtm.update_state_flag=1;
						  mtm.engine_state=FLY_START_2;
						}
					}
					else
					{
						mtm.threshold_count=0;
					}

					if ((mtm.rotor_speed>2000)&&(mtm.starter_move_count<10))//условие перехода на пид
					{
						mtm.engine_flag=mtm.engine_flag&(~START_NO);
						mtm.starter_move_count++;
					}

					if(mtm.starter_move_count>2)// если крутится переход на пид
					{
						mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
																  config.starter_rate,config.starter_rate);

						STARTER_PWM=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,mtm.rotor_speed,0,1000);
					}
					else
					{
						if(mtm.starter_restart_flag)//если перезапуск то плавное приращение уставки
						{
							if((STARTER_PWM+20)<=((config.starter_min)+1000))
							{
								STARTER_PWM= STARTER_PWM+20;
							}
							else
							{
								STARTER_PWM= ((config.starter_min)+1000);
							}
						}
						else//если первый заход то фиксированная уставка от конфига
						{
							STARTER_PWM=(config.starter_min)+1000;
						}
					}


					  //MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					  //START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);


				}

				break;
				case(R500_PRS_TRV):
					{
						  if (mtm.update_state_flag)
						  {
							  mtm.update_state_flag=0;

							  mtm.next_setpoint=(config.n1_ignition*10)+1000;

							  mtm.relevant_setpoint=1200;

							  mtm.work_count=0;

							  mtm.threshold_count=0;

							  mtm.starter_move_count=0;

							  pid_starter.integrator=(((float)config.starter_min)/1000.00);

							  mtm.hot_state=1;
							  GEN_OFF
							  mtm.gen_state=0;

							  MAIN_VALVE_ON;

							  PUMP_PWM=ZERO_POINT_PUMP+config.pump_test_pwm;

							  mtm.pre_pump_counter=0;

							  STARTER_PWM=ZERO_POINT_STARTER_WORK;

						  }

						  if(mtm.pre_pump_counter>KBM_PREPUMP_TIME)
						  {
							  PUMP_PWM = ZERO_POINT_PUMP;
							  MAIN_VALVE_OFF;
						  }
						  mtm.pre_pump_counter++;



						  if((mtm.work_count>40)&&(mtm.starter_move_count==0))//ход на перезапуск при нераскрутке ротора
						  {
							  mtm.work_count=0;
							  STARTER_PWM=ZERO_POINT_STARTER_WORK;

							  mtm.starter_restart_flag=1;

						  }
						  mtm.work_count++;


						  if(mtm.rotor_speed>(config.n1_ignition*10))//переход в след. стэйт  счетчик =0,1 секунды
						  {
							  mtm.threshold_count++;
							  if(mtm.threshold_count>1)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_START_2;
							  }
						  }

						  else
						  {
							  mtm.threshold_count=0;
						  }



						  if(mtm.engine_flag&C_CONTROL)//если отьебнула термопара
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=SLOWDOWN;
						  }

						  if ((mtm.rotor_speed>2000)&&(mtm.starter_move_count<10))//условие перехода на пид
							 {
							  mtm.engine_flag=mtm.engine_flag&(~START_NO);
							  mtm.starter_move_count++;
							 }

						  if(mtm.starter_move_count>2)// если крутится переход на пид
						  {


							  mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
																		  config.starter_rate,config.starter_rate);

							  STARTER_PWM=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,mtm.rotor_speed,0,1000);

						  }
						  else
							{

								if(mtm.starter_restart_flag)//если перезапуск то плавное приращение уставки
								{
									if((STARTER_PWM+20)<=((config.starter_min)+1000))
									{
										STARTER_PWM= STARTER_PWM+20;
									}
									else
									{
										STARTER_PWM= ((config.starter_min)+1000);
									}
								}
								else//если первый заход то фиксированная уставка от конфига
								{
								  STARTER_PWM=(config.starter_min)+1000;
								}
							}


						  //MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						  //START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

					}
				break;
				case(R1):
				{

				}
				break;
			}
			Overheating_Control_Start(30);
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case (FLY_START_2):
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;
				case(R500):
					{
						 if (mtm.update_state_flag)
						  {
							 mtm.update_state_flag=0;


							 mtm.prs_state=1;

							 mtm.work_count=0;
							 mtm.threshold_count=0;



							 PLUG_ON

						  }

						  if(mtm.t_real>=config.t_ignition)//переход в след. стэйт  счетчик =0,1 секунды
						  {
							  mtm.threshold_count++;
							  if(mtm.threshold_count>20)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_START_3;
							  }
						  }

						  else
						  {
							  mtm.threshold_count=0;
						  }


						  if(mtm.work_count>250)//если 25 секунд нет розжига то переходим в перезапуск
						  {
							  if(mtm.engine_flag&C_CONTROL)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_START_3;
							  }

							  else
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_RESTART;
								  mtm.engine_flag=mtm.engine_flag|(IGN_FAIL);
							  }


						  }
						  mtm.work_count++;


						  mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
																	 config.starter_rate,config.starter_rate);

						  mtm.ust_starter_pid=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,
																mtm.rotor_speed,0,1000);
						  mtm.ust_starter_curve=Starter_Curve(mtm.rotor_speed);
						  STARTER_PWM=(mtm.ust_starter_curve<=mtm.ust_starter_pid)?mtm.ust_starter_curve:mtm.ust_starter_pid;

						  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);




						 PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
														ZERO_POINT_PUMP_WORK),mtm.bus_volt));



					}
				break;
				case(R500_PRS_KBM):
					{
					 if (mtm.update_state_flag)
					  {
						 mtm.update_state_flag=0;


						 mtm.prs_state=1;

						 mtm.work_count=0;
						 mtm.threshold_count=0;



						 PLUG_ON

					  }

					  if(mtm.t_real>=config.t_ignition)//переход в след. стэйт  счетчик =0,1 секунды
					  {
						  mtm.threshold_count++;
						  if(mtm.threshold_count>1)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=FLY_START_3;
						  }
					  }

					  else
					  {
						  mtm.threshold_count=0;
					  }



					  if(mtm.engine_flag&C_CONTROL)
					  {
						  mtm.update_state_flag=1;
						  mtm.engine_state=FLY_START_3;
					  }


					  mtm.work_count++;

	/*
					  mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
																 config.starter_rate,config.starter_rate);

					  mtm.ust_starter_pid=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,
															mtm.rotor_speed,0,1000);
					  mtm.ust_starter_curve=Starter_Curve(mtm.rotor_speed);
					  STARTER_PWM=(mtm.ust_starter_curve<=mtm.ust_starter_pid)?mtm.ust_starter_curve:mtm.ust_starter_pid;
	*/
					  STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);

					  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);




					 PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
													ZERO_POINT_PUMP_WORK),mtm.bus_volt));


					}
				break;
				case(R500_PRS_TRV):
					{
						 if (mtm.update_state_flag)
						  {
							 mtm.update_state_flag=0;



							 mtm.prs_state=1;

							 mtm.work_count=0;
							 mtm.threshold_count=0;

							 if(mtm.flag_restsrt)
							 {
								 ADG_ON
							 }
							 else
							 {
								 PLUG_ON
							 }
						  }

						  if(mtm.t_real>=config.t_ignition)//переход в след. стэйт  счетчик =0,1 секунды
						  {
							  mtm.threshold_count++;
							  if(mtm.threshold_count>1)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_START_3;
							  }
						  }

						  else
						  {
							  mtm.threshold_count=0;
						  }


						  if(mtm.work_count>100)//если 10 секунд нет розжига то переходим в перезапуск
						  {
							  if(mtm.engine_flag&C_CONTROL)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_START_3;
							  }

							  else
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_RESTART;
								  mtm.engine_flag=mtm.engine_flag|(IGN_FAIL);
							  }


						  }
						  mtm.work_count++;


						  mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
																	 config.starter_rate,config.starter_rate);

						  mtm.ust_starter_pid=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,
																mtm.rotor_speed,0,1000);
						  mtm.ust_starter_curve=Starter_Curve(mtm.rotor_speed);
						  STARTER_PWM=(mtm.ust_starter_curve<=mtm.ust_starter_pid)?mtm.ust_starter_curve:mtm.ust_starter_pid;

						  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);



						  if(mtm.flag_restsrt)
						  {
							  mtm.trv_restart_correction=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
														ZERO_POINT_PUMP_WORK),mtm.bus_volt))+config.trv_restart_pump_pwm_corr;
							  if(mtm.trv_restart_correction<=2000)
							  {
								  PUMP_PWM=mtm.trv_restart_correction;
							  }
							  else
							  {
								  PUMP_PWM=2000;
							  }
						  }
						  else
						  {
							  PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
														ZERO_POINT_PUMP_WORK),mtm.bus_volt));
						  }

					}
				break;
				case(R1):
					{

					}
				break;
			}
			Overheating_Control_Start(30);
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case (FLY_START_3):
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;
				case(R500):
					{
						 if (mtm.update_state_flag)
						  {
							 mtm.update_state_flag=0;

							 mtm.next_setpoint=25000;

							 mtm.work_count=0;
							 mtm.flameout_work_count=0;
							 mtm.threshold_count=0;
						  }



						  if(mtm.rotor_speed>=18000)//переход в след. стэйт  счетчик =0,1 секунды
						  {
							  mtm.threshold_count++;
							  if(mtm.threshold_count>1)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_START_4;
							  }
						  }

						  else
						  {
							  mtm.threshold_count=0;
						  }

						  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

						  STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);
						  PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
														ZERO_POINT_PUMP_WORK),mtm.bus_volt));


						if(mtm.work_count>600)
						  {
							mtm.update_state_flag=1;
							mtm.engine_state=FLY_RESTART;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
						mtm.work_count++;


						if((mtm.t_real<config.t_flameout)&&((mtm.engine_flag&C_CONTROL)==0))
						{
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_RESTART;
								  mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
							  }

						}

					}
				break;
				case(R500_PRS_KBM):
					{
						 if (mtm.update_state_flag)
						  {
							 mtm.update_state_flag=0;

							 PLUG_OFF
							 ADG_OFF

							 mtm.next_setpoint=25000;

							 mtm.work_count=0;
							 mtm.flameout_work_count=0;
							 mtm.threshold_count=0;
						  }



						  if(mtm.rotor_speed>=18000)//переход в след. стэйт  счетчик =0,1 секунды
						  {
							  mtm.threshold_count++;
							  if(mtm.threshold_count>1)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_START_4;
							  }
						  }

						  else
						  {
							  mtm.threshold_count=0;
						  }

						  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

						  STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);
						  PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
														ZERO_POINT_PUMP_WORK),mtm.bus_volt));







					}
				break;
				case(R500_PRS_TRV):
					{
						 if (mtm.update_state_flag)
						  {
							 mtm.update_state_flag=0;

							 mtm.next_setpoint=25000;

							 mtm.work_count=0;
							 mtm.flameout_work_count=0;
							 mtm.threshold_count=0;
						  }



						  if(mtm.rotor_speed>=18000)//переход в след. стэйт  счетчик =0,1 секунды
						  {
							  mtm.threshold_count++;
							  if(mtm.threshold_count>1)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_START_4;
							  }
						  }

						  else
						  {
							  mtm.threshold_count=0;
						  }

						  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

						  STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);


						  if(mtm.flag_restsrt)
						  {
							  mtm.trv_restart_correction=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
														ZERO_POINT_PUMP_WORK),mtm.bus_volt))+config.trv_restart_pump_pwm_corr;
							  if(mtm.trv_restart_correction<=2000)
							  {
								  PUMP_PWM=mtm.trv_restart_correction;
							  }
							  else
							  {
								  PUMP_PWM=2000;
							  }
						  }
						  else
						  {
							  PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
														ZERO_POINT_PUMP_WORK),mtm.bus_volt));
						  }


						if(mtm.work_count>600)
						  {
							mtm.update_state_flag=1;
							mtm.engine_state=FLY_RESTART;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
						mtm.work_count++;


						if((mtm.t_real<config.t_flameout)&&((mtm.engine_flag&C_CONTROL)==0))
						{
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_RESTART;
								  mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
							  }

						}

					}
				break;
				case(R1):
					{

					}
				break;
			}
			Overheating_Control_Start(30);
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case (FLY_START_4)://
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;
				case(R500):
					{
					if (mtm.update_state_flag)
						{
						mtm.update_state_flag=0;

						mtm.flameout_work_count=0;
						mtm.threshold_count=0;

						}

						if(mtm.rotor_speed>=config.n1_starter_off*10)//переход в след. стэйт  счетчик =0,1 секунды
						{
						  mtm.threshold_count++;
						  if(mtm.threshold_count>1)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=FLY_START_5;
						  }
						}

						else
						{
						  mtm.threshold_count=0;
						}



						mtm.work_count++;

						MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);


						STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);
						PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
													  ZERO_POINT_PUMP_WORK),mtm.bus_volt));

						  if(mtm.work_count>600)//рестарт по зависанию 60 сек
						  {
							mtm.update_state_flag=1;
							mtm.engine_state=FLY_RESTART;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
						  mtm.work_count++;


						  if((mtm.t_real<config.t_flameout)&&((mtm.engine_flag&C_CONTROL)==0))//незапуск по низкой температуре
						  {
							  mtm.flameout_work_count++;
							  if(mtm.flameout_work_count>30)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_RESTART;
								  mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
							  }
						  }

						  else
						  {
							  mtm.flameout_work_count=0;
						  }




					}
				break;
				case(R500_PRS_KBM):
					{
					if (mtm.update_state_flag)
						{
						mtm.update_state_flag=0;

						mtm.flameout_work_count=0;
						mtm.threshold_count=0;

						}

					if(mtm.rotor_speed>=config.n1_starter_off*10)//переход в след. стэйт  счетчик =0,1 секунды
					{
					  mtm.threshold_count++;
					  if(mtm.threshold_count>1)
					  {
						  mtm.update_state_flag=1;
						  mtm.engine_state=FLY_START_5;
					  }
					}

					else
					{
					  mtm.threshold_count=0;
					}



						mtm.work_count++;

						MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);


						STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);
						PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
													  ZERO_POINT_PUMP_WORK),mtm.bus_volt));


					}
				break;
				case(R500_PRS_TRV):
					{
					if (mtm.update_state_flag)
						{
						mtm.update_state_flag=0;

						mtm.flameout_work_count=0;
						mtm.threshold_count=0;

						}

						if(mtm.rotor_speed>=config.n1_starter_off*10)//переход в след. стэйт  счетчик =0,1 секунды
						{
						  mtm.threshold_count++;
						  if(mtm.threshold_count>1)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=FLY_START_5;
						  }
						}

						else
						{
						  mtm.threshold_count=0;
						}



						mtm.work_count++;

						MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);


						STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);

						if(mtm.flag_restsrt)
						{
						  mtm.trv_restart_correction=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
													ZERO_POINT_PUMP_WORK),mtm.bus_volt))+config.trv_restart_pump_pwm_corr;
						  if(mtm.trv_restart_correction<=2000)
						  {
							  PUMP_PWM=mtm.trv_restart_correction;
						  }
						  else
						  {
							  PUMP_PWM=2000;
						  }
						}
						else
						{
						  PUMP_PWM=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
													ZERO_POINT_PUMP_WORK),mtm.bus_volt));
						}



						  if(mtm.work_count>600)//рестарт по зависанию 60 сек
						  {
							mtm.update_state_flag=1;
							mtm.engine_state=FLY_RESTART;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
						  mtm.work_count++;


						  if((mtm.t_real<config.t_flameout)&&((mtm.engine_flag&C_CONTROL)==0))//незапуск по низкой температуре
						  {
							  mtm.flameout_work_count++;
							  if(mtm.flameout_work_count>30)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_RESTART;
								  mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
							  }
						  }

						  else
						  {
							  mtm.flameout_work_count=0;
						  }

					}
				break;
				case(R1):
					{

					}
				break;
			}
			Overheating_Control_Start(30);
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case (FLY_START_5)://
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;
				case(R500):
					{
					if (mtm.update_state_flag)
						  {
							mtm.update_state_flag=0;

							STARTER_PWM=0;

							PLUG_OFF;
							mtm.prs_state=0;

							mtm.flameout_work_count=0;
							mtm.threshold_count=0;
						  }

					if(mtm.rotor_speed>=(config.n1_operation*10))//переход в след. стэйт  счетчик =0,1 секунды
						  {
							  mtm.threshold_count++;
							  if(mtm.threshold_count>1)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_OPERATION;
							  }
						  }

					 else
						  {
							  mtm.threshold_count=0;
						  }


					 if(mtm.work_count>600)
						  {
							mtm.update_state_flag=1;
							mtm.engine_state=FLY_RESTART;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  mtm.work_count++;


					  if((mtm.t_real<config.t_flameout)&&((mtm.engine_flag&C_CONTROL)==0))
					  {
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=FLY_RESTART;
							  mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  }

					  else
					  {
						  mtm.flameout_work_count=0;
					  }




					  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);



					  uint16_t ust=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
								  ZERO_POINT_PUMP_WORK),mtm.bus_volt))+
								((PIDController_Update(&pid_t4_start,mtm.t_real, 700,0,0))*(-1));
					  if(ust<2000)
					  {
						  PUMP_PWM=ust;
					  }

					  else
					  {
						  PUMP_PWM=2000;
					  }




					}
				break;
				case(R500_PRS_KBM):
					{
					if (mtm.update_state_flag)
						  {
							mtm.update_state_flag=0;

							STARTER_PWM=0;


							mtm.prs_state=0;

							mtm.flameout_work_count=0;
							mtm.threshold_count=0;
						  }

					if(mtm.rotor_speed>=(config.n1_operation*10))//переход в след. стэйт  счетчик =0,1 секунды
						  {
							  mtm.threshold_count++;
							  if(mtm.threshold_count>1)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_OPERATION;
							  }
						  }

					 else
						  {
							  mtm.threshold_count=0;
						  }


					  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);



					  uint16_t ust=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
													  ZERO_POINT_PUMP_WORK),mtm.bus_volt))+
													((PIDController_Update(&pid_t4_start,mtm.t_real, 700,0,0))*(-1));
					  if(ust<2000)
						  {
							  PUMP_PWM=ust;
						  }

					  else
						  {
							  PUMP_PWM=2000;
						  }




					}
				break;
				case(R500_PRS_TRV):
					{
					if (mtm.update_state_flag)
						  {
							mtm.update_state_flag=0;

							STARTER_PWM=0;

							PLUG_OFF;
							mtm.prs_state=0;

							mtm.flameout_work_count=0;
							mtm.threshold_count=0;
						  }

					if(mtm.rotor_speed>=(config.n1_operation*10))//переход в след. стэйт  счетчик =0,1 секунды
						  {
							  mtm.threshold_count++;
							  if(mtm.threshold_count>1)
							  {
								  mtm.update_state_flag=1;
								  mtm.engine_state=FLY_OPERATION;
							  }
						  }

					 else
						  {
							  mtm.threshold_count=0;
						  }


					 if(mtm.work_count>600)
						  {
							mtm.update_state_flag=1;
							mtm.engine_state=FLY_RESTART;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  mtm.work_count++;


					  if((mtm.t_real<config.t_flameout)&&((mtm.engine_flag&C_CONTROL)==0))
					  {
						  mtm.flameout_work_count++;
						  if(mtm.flameout_work_count>30)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=FLY_RESTART;
							  mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						  }
					  }

					  else
					  {
						  mtm.flameout_work_count=0;
					  }




					  MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
					  START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);


					  uint16_t ust=(Pump_PWM_Correct (((Pump_Curve(mtm.rotor_speed)*DELTA_PUMP_PWM)+
								  ZERO_POINT_PUMP_WORK),mtm.bus_volt))+
								  ((PIDController_Update(&pid_t4_start,mtm.t_real, 700,0,0))*(-1))+
								  (config.trv_restart_pump_pwm_corr*mtm.flag_restsrt);
					  if(ust<2000)
					  {
						  PUMP_PWM=ust;
					  }

					  else
					  {
						  PUMP_PWM=2000;
					  }




					}
				break;
				case(R1):
					{

					}
				break;
			}
			Overheating_Control_Start(30);
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(FLY_OPERATION)://работа
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;
				case(R500):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;


						mtm.flameout_work_count=0;
						mtm.relevant_setpoint=config.n1_min*10;

						mtm.hot_state=0;

						pid_pump.integrator=((float)PUMP_PWM-1000)/pid_pump.delta;

						mtm.work_count=0;

						mtm.hall_err_flag=0;
					}

					if (mtm.hall_err_flag)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=HALL_ERR_STATE;
					}
					else
					{
						if(mtm.work_count==30)
						{
							GEN_ON
							mtm.gen_state=1;
						}
						else if (mtm.work_count<32)
						{
							mtm.work_count++;
						}

						if(mtm.rotor_speed>BEGIN_T_LIM_PID)
						{
						  mtm.t_lim_pid_val=PIDController_Update(&pid_t_lim,mtm.t_real, config.t_PID_lim,0,0);
						}
						else
						{
						  mtm.t_lim_pid_val=0;
						  PIDController_Reset(&pid_t_lim);
						}

						mtm.relevant_setpoint=Get_next_setpoint(mtm.rud,mtm.relevant_setpoint,
																(config.rate_up*10),(config.rate_down*10));

						MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

						PUMP_PWM= PIDController_Update(&pid_pump, (mtm.relevant_setpoint-mtm.t_lim_pid_val),
																 mtm.rotor_speed,0,ZERO_POINT_PUMP_WORK);

						if(mtm.rotor_speed>config.n1_overspeed*10)//превышение максимальных оборотов
						{
						   mtm.engine_flag=mtm.engine_flag|(N1_HIGH);
						}

						if(/*(mtm.t_real<config.t_flameout)||*/(mtm.rotor_speed<30000))//погасла камера
						{
							mtm.flameout_work_count++;
							if(mtm.flameout_work_count>30)
							{
								mtm.update_state_flag=1;
								mtm.engine_state=FLY_RESTART;
								mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
							}
						}
						else
						{
						  mtm.flameout_work_count=0;
						}
					}
				}
				break;
				case(R500_PRS_KBM):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;


						mtm.flameout_work_count=0;
						mtm.relevant_setpoint=config.n1_min*10;

						mtm.hot_state=0;

						pid_pump.integrator=((float)PUMP_PWM-1000)/pid_pump.delta;

						mtm.work_count=0;

						mtm.hall_err_flag=0;
					}

					if (mtm.hall_err_flag)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}
					else
					{
						if(mtm.work_count==30)
						{
							GEN_ON
							mtm.gen_state=1;
						}
						else if (mtm.work_count<32)
						{
							mtm.work_count++;
						}

						if(mtm.rotor_speed>BEGIN_T_LIM_PID)
						{
							mtm.t_lim_pid_val=PIDController_Update(&pid_t_lim,mtm.t_real, config.t_PID_lim,0,0);
						}
						else
						{
							mtm.t_lim_pid_val=0;
							PIDController_Reset(&pid_t_lim);
						}

						mtm.relevant_setpoint=Get_next_setpoint(mtm.rud,mtm.relevant_setpoint,
																(config.rate_up*10),(config.rate_down*10));

						MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

						PUMP_PWM= PIDController_Update(&pid_pump, (mtm.relevant_setpoint-mtm.t_lim_pid_val),
																 mtm.rotor_speed,0,ZERO_POINT_PUMP_WORK);

						if(mtm.rotor_speed>config.n1_overspeed*10)//превышение максимальных оборотов
						{
							mtm.engine_flag=mtm.engine_flag|(N1_HIGH);
						}
					}
				}
				break;
				case(R500_PRS_TRV):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;


						mtm.flameout_work_count=0;
						mtm.relevant_setpoint=config.n1_min*10;

						mtm.hot_state=0;

						pid_pump.integrator=((float)PUMP_PWM-1000)/pid_pump.delta;

						mtm.work_count=0;
						mtm.hall_err_flag=0;
					}

					if (mtm.hall_err_flag)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SLOWDOWN;
					}
					else
					{
						if(mtm.work_count==30)
						{
							GEN_ON
							mtm.gen_state=1;
						}
						else if (mtm.work_count<32)
						{
							mtm.work_count++;
						}

						if(mtm.rotor_speed>BEGIN_T_LIM_PID)
						{
							mtm.t_lim_pid_val=PIDController_Update(&pid_t_lim,mtm.t_real, config.t_PID_lim,0,0);
						}
						else
						{
							mtm.t_lim_pid_val=0;
							PIDController_Reset(&pid_t_lim);
						}

						mtm.relevant_setpoint=Get_next_setpoint(mtm.rud,mtm.relevant_setpoint,
																(config.rate_up*10),(config.rate_down*10));

						MAIN_VALVE_PWM=Valve_Curve(mtm.rotor_speed);
						START_VALVE_PWM=Start_Valve_Curve(mtm.rotor_speed);

						PUMP_PWM= PIDController_Update(&pid_pump, (mtm.relevant_setpoint-mtm.t_lim_pid_val),
																 mtm.rotor_speed,0,ZERO_POINT_PUMP_WORK);

						if(mtm.rotor_speed>config.n1_overspeed*10)//превышение максимальных оборотов
						{
						   mtm.engine_flag=mtm.engine_flag|(N1_HIGH);
						}


						if(/*(mtm.t_real<config.t_flameout)||*/(mtm.rotor_speed<30000))//погасла камера
						{
							mtm.flameout_work_count++;
							if(mtm.flameout_work_count>30)
							{
								mtm.update_state_flag=1;
								mtm.engine_state=FLY_RESTART;
								mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
							}
						}
						else
						{
							mtm.flameout_work_count=0;
						}
					}
				}
				break;
				case(R1):
					{

					}
				break;
			}
			Overheating_Control_Operation(30);
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(FLY_RESTART):
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;


				case(R500):
					{
						 if (mtm.update_state_flag)
						 {

							 mtm.update_state_flag=0;
							 mtm.work_count=0;




							 STARTER_PWM =0;
							 PUMP_PWM  = ZERO_POINT_PUMP;

							 PLUG_OFF;
							 mtm.prs_state=0;

							 ADG_OFF;
							 mtm.adg_state=0;
							 mtm.hot_state=0;



							 START_VALVE_OFF
							 MAIN_VALVE_OFF


							 PIDController_Reset (&pid_starter);
							 PIDController_Reset (&pid_pump);
							 PIDController_Reset (&pid_t_lim);

							 mtm.next_setpoint=5000;
							 mtm.relevant_setpoint=1200;

							 mtm.starter_move_count=0;



						 }

						  if(mtm.work_count>5)
						  {
							STARTER_PWM = ZERO_SETPOINT;
						  }

						 if(mtm.work_count>20)
						 {


							if(mtm.t_real<190)
							{

							mtm.update_state_flag=1;

							mtm.engine_state=FLY_START_1;


							}

							else if(mtm.rotor_speed<6000)

							{
								if(mtm.rotor_speed==0)
								{
									STARTER_PWM=(config.starter_min)+1000;
								}
								else
								{
									  STARTER_PWM=Starter_PWM_Correct(mtm.rotor_speed,mtm.bus_volt);
									/*
									mtm.relevant_setpoint=Get_next_setpoint(mtm.rud,mtm.relevant_setpoint,
																			 config.rate_up,config.rate_down);

									STARTER_PWM=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,mtm.rotor_speed,0,1000);
									*/

								}
							}



						 }

						 mtm.work_count++;


					}
				break;


				case(R500_PRS_KBM):
					{

					}
				break;

				case(R500_PRS_TRV):
					{
						 if (mtm.update_state_flag)
						 {

							 mtm.update_state_flag=0;
							 mtm.work_count=0;




							 STARTER_PWM =0;
							 PUMP_PWM  = ZERO_POINT_PUMP;

							 PLUG_OFF;
							 mtm.prs_state=0;

							 ADG_OFF;
							 mtm.adg_state=0;
							 mtm.hot_state=0;



							 START_VALVE_OFF
							 MAIN_VALVE_OFF


							 PIDController_Reset (&pid_starter);
							 PIDController_Reset (&pid_pump);
							 PIDController_Reset (&pid_t_lim);

							 mtm.next_setpoint=5000;
							 mtm.relevant_setpoint=1200;

							 mtm.starter_move_count=0;

							 mtm.flag_restsrt=1;
						 }

						  if(mtm.work_count>5)
						  {
							STARTER_PWM = ZERO_SETPOINT;
						  }

						 if(mtm.work_count>10)
						 {


							if(mtm.t_real<190)
							{

							mtm.update_state_flag=1;

							mtm.engine_state=FLY_START_1;


							}

							else if(mtm.rotor_speed<6000)

							{
								if(mtm.rotor_speed==0)
								{
									STARTER_PWM=(config.starter_min)+1000;
								}
								else
								{
									mtm.relevant_setpoint=Get_next_setpoint(mtm.rud,mtm.relevant_setpoint,
																			 config.rate_up,config.rate_down);

									STARTER_PWM=PIDController_Update(&pid_starter,mtm.relevant_setpoint ,mtm.rotor_speed,0,1000);

								}
							}



						 }

						 mtm.work_count++;

					}
				break;


				case(R1):
					{

					}
				break;


			}


		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(SHECK_VAL):
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;


				case(R500):
					{
					  if (mtm.update_state_flag)
					  {

						  mtm.update_state_flag=0;


						  START_VALVE_ON ;

						  mtm.work_count=0;

						  mtm.valve1_curve_control_flag=0;//set when ADC is complited
						  mtm.valve2_curve_control_flag=0;


					  }


					  if(mtm.work_count>5)
					  {
						  MAIN_VALVE_ON
					  }


					  if(mtm.work_count>10)
					  {
						  MAIN_VALVE_OFF
						  START_VALVE_OFF


						  mtm.engine_flag=mtm.engine_flag|(VALVE_NO);
						  mtm.engine_flag=mtm.engine_flag|(A_CONTROL);

						  mtm.update_state_flag=1;
						  mtm.engine_state=SHECK_PUMP;


					  }

					  else if(((mtm.valve1_cur>0.3)&&(mtm.valve2_cur>0.3))&&(mtm.work_count>5))
					  {
						  MAIN_VALVE_OFF ;
						  START_VALVE_OFF ;

						  mtm.update_state_flag=1;
						  mtm.engine_state=SHECK_PUMP;
					  }

					  mtm.work_count++;

					}
				break;


				case(R500_PRS_KBM):
					{
					  if (mtm.update_state_flag)
					  {

						  mtm.update_state_flag=0;


						  START_VALVE_ON ;

						  mtm.work_count=0;

						  mtm.valve1_curve_control_flag=0;//set when ADC is complited
						  mtm.valve2_curve_control_flag=0;


					  }
					  if(mtm.work_count>5)
					  {
						  MAIN_VALVE_ON
					  }


					  if(mtm.work_count>10)
					  {
						  MAIN_VALVE_OFF
						  START_VALVE_OFF


						  mtm.engine_flag=mtm.engine_flag|(VALVE_NO);
						  mtm.engine_flag=mtm.engine_flag|(A_CONTROL);

						  mtm.update_state_flag=1;
						  mtm.engine_state=SHECK_PUMP;


					  }

					  else if((mtm.valve1_cur>0.3)&&(mtm.valve2_cur>0.3))
					  {
						  MAIN_VALVE_OFF ;
						  START_VALVE_OFF ;

						  mtm.update_state_flag=1;
						  mtm.engine_state=SHECK_PUMP;
					  }

					  mtm.work_count++;
					}
				break;

				case(R500_PRS_TRV):
					{
						  if (mtm.update_state_flag)
						  {

							  mtm.update_state_flag=0;


							  START_VALVE_ON ;

							  mtm.work_count=0;

							  mtm.valve1_curve_control_flag=0;//set when ADC is complited
							  mtm.valve2_curve_control_flag=0;


						  }
						  if(mtm.work_count>5)
						  {
							  MAIN_VALVE_ON
						  }


						  if(mtm.work_count>10)
						  {
							  MAIN_VALVE_OFF
							  START_VALVE_OFF


							  mtm.engine_flag=mtm.engine_flag|(VALVE_NO);
							  mtm.engine_flag=mtm.engine_flag|(A_CONTROL);

							  mtm.update_state_flag=1;
							  mtm.engine_state=SHECK_PUMP;


						  }

						  else if((mtm.valve1_cur>0.3)&&(mtm.valve2_cur>0.3))
						  {
							  MAIN_VALVE_OFF ;
							  START_VALVE_OFF ;

							  mtm.update_state_flag=1;
							  mtm.engine_state=SHECK_PUMP;
						  }

						  mtm.work_count++;

					}
				break;


				case(R1):
					{

					}
				break;


			}






		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(SHECK_PUMP):
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;


				case(R500):
					{
					  if (mtm.update_state_flag)
					  {

						  mtm.update_state_flag=0;

						  mtm.work_count=0;

						  PUMP_PWM=ZERO_POINT_PUMP+config.pump_test_pwm;

					  }


					  if(mtm.work_count>20)
					  {

						  PUMP_PWM=ZERO_POINT_PUMP;

						  mtm.engine_flag=mtm.engine_flag|(PUMP_NO);
						  mtm.engine_flag=mtm.engine_flag|(A_CONTROL);

						  mtm.update_state_flag=1;
						  mtm.engine_state=SHECK_IGN;


					  }

					  else if((mtm.pump_speed>0)&&(mtm.work_count>5))
					  {
						  PUMP_PWM=ZERO_POINT_PUMP;


						  mtm.update_state_flag=1;
						  mtm.engine_state=SHECK_IGN;
					  }

					  mtm.work_count++;

					}
				break;


				case(R500_PRS_KBM):
					{
					  if (mtm.update_state_flag)
					  {

						  mtm.update_state_flag=0;


						  mtm.work_count=0;

						  PUMP_PWM=ZERO_POINT_PUMP+config.pump_test_pwm;

					  }


					  if(mtm.work_count>20)
					  {

						  PUMP_PWM=ZERO_POINT_PUMP;

						  mtm.engine_flag=mtm.engine_flag|(PUMP_NO);
						  mtm.engine_flag=mtm.engine_flag|(A_CONTROL);

						  mtm.update_state_flag=1;
						  mtm.engine_state=SHECK_IGN;


					  }

					  else if(mtm.pump_speed>0)
					  {
						  PUMP_PWM=ZERO_POINT_PUMP;


						  mtm.update_state_flag=1;
						  mtm.engine_state=SHECK_IGN;
					  }

					  mtm.work_count++;

					}
				break;

				case(R500_PRS_TRV):
					{
					  if (mtm.update_state_flag)
					  {

						  mtm.update_state_flag=0;


						  mtm.work_count=0;

						  PUMP_PWM=ZERO_POINT_PUMP+config.pump_test_pwm;

					  }


					  if(mtm.work_count>20)
					  {

						  PUMP_PWM=ZERO_POINT_PUMP;

						  mtm.engine_flag=mtm.engine_flag|(PUMP_NO);
						  mtm.engine_flag=mtm.engine_flag|(A_CONTROL);

						  mtm.update_state_flag=1;
						  mtm.engine_state=SHECK_IGN;


					  }

					  else if(mtm.pump_speed>0)
					  {
						  PUMP_PWM=ZERO_POINT_PUMP;


						  mtm.update_state_flag=1;
						  mtm.engine_state=SHECK_IGN;
					  }

					  mtm.work_count++;

					}
				break;


				case(R1):
					{

					}
				break;


			}
		  break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(SHECK_IGN):
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;


				case(R500):
					{
					  if (mtm.update_state_flag)
					  {

							mtm.update_state_flag=0;


							mtm.work_count=0;

							PLUG_ON
							mtm.prs_state=1;

					  }


					  if(mtm.work_count>30)
					  {

						  PLUG_OFF
						  mtm.prs_state=0;

						  mtm.engine_flag=mtm.engine_flag|(PLUG_NO);
						  mtm.engine_flag=mtm.engine_flag|(A_CONTROL);

						  mtm.update_state_flag=1;
						  mtm.engine_state=SHECK_STARTER;


					  }

					  else //if(mtm.plug_current>0)
					  {
						  PLUG_OFF
						  mtm.prs_state=0;


						  mtm.update_state_flag=1;
						  mtm.engine_state=SHECK_STARTER;
					  }

					  mtm.work_count++;

					}
				break;


				case(R500_PRS_KBM):
					{
						mtm.update_state_flag=1;
						 mtm.engine_state=SHECK_STARTER;
					}
				break;

				case(R500_PRS_TRV):
					{

						mtm.update_state_flag=1;
						mtm.engine_state=SHECK_STARTER;
					}
				break;


				case(R1):
					{

					}
				break;


			}



		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(SHECK_STARTER):
			switch(config.engine_type)
			{
				case(R40):
				{

				}
				break;
				case(R500):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.work_count=0;

						STARTER_PWM=ZERO_POINT_STARTER_WORK;
					}

					if((STARTER_PWM+20)<((config.starter_min)+1000))
					{
						STARTER_PWM= STARTER_PWM+20;
					}
					else if(STARTER_PWM<((config.starter_min)+1000))
					{
						STARTER_PWM= ((config.starter_min)+1000);
					}

					if(mtm.work_count>100)
					{
						STARTER_PWM = ZERO_SETPOINT;

						mtm.engine_flag=mtm.engine_flag|(START_NO);
						mtm.engine_flag=mtm.engine_flag|(A_CONTROL);

						mtm.update_state_flag=1;
						mtm.engine_state=INITIAL_STAGE;
					}
					else if(mtm.rotor_speed>500)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=INITIAL_STAGE;
					}

					mtm.work_count++;

				}
				break;
				case(R500_PRS_KBM):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.work_count=0;

						STARTER_PWM=ZERO_POINT_STARTER_WORK;
					}

					if((STARTER_PWM+20)<((config.starter_min)+1000))
					{
						STARTER_PWM= STARTER_PWM+20;
					}
					else if(STARTER_PWM<((config.starter_min)+1000))
					{
						STARTER_PWM= ((config.starter_min)+1000);
					}

					if(mtm.work_count>100)
					{
						STARTER_PWM = ZERO_SETPOINT;

						mtm.engine_flag=mtm.engine_flag|(START_NO);
						mtm.engine_flag=mtm.engine_flag|(A_CONTROL);

						mtm.update_state_flag=1;
						mtm.engine_state=INITIAL_STAGE;
					}
					else if(mtm.rotor_speed>500)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=INITIAL_STAGE;
					}

					mtm.work_count++;

				}
				break;
				case(R500_PRS_TRV):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.work_count=0;

						STARTER_PWM=ZERO_POINT_STARTER_WORK;
					}

					if((STARTER_PWM+20)<((config.starter_min)+1000))
					{
						STARTER_PWM= STARTER_PWM+20;
					}
					else if(STARTER_PWM<((config.starter_min)+1000))
					{
						STARTER_PWM= ((config.starter_min)+1000);
					}

					if(mtm.work_count>100)
					{
						STARTER_PWM = ZERO_SETPOINT;

						mtm.engine_flag=mtm.engine_flag|(START_NO);
						mtm.engine_flag=mtm.engine_flag|(A_CONTROL);

						mtm.update_state_flag=1;
						mtm.engine_state=INITIAL_STAGE;
					}
					else if(mtm.rotor_speed>500)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=INITIAL_STAGE;
					}

					mtm.work_count++;

				}
				break;
				case(R1):
				{

				}
				break;
			}

		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(PUMP_ST):
		switch(config.engine_type)
		{
			case(R40):
				{

				}
			break;


			case(R500):
				{
				  if (mtm.update_state_flag)
					  {

						  mtm.update_state_flag=0;


						  mtm.work_count=0;
						  START_VALVE_ON ;


						  PUMP_PWM=ZERO_POINT_PUMP+config.pump_test_pwm;

					  }


					  if(mtm.work_count>10)
					  {


						  mtm.update_state_flag=1;
						  mtm.engine_state=INITIAL_STAGE;
						  //START_VALVE_OFF;
						 // MAIN_VALVE_OFF;

					  }

					  mtm.work_count++;

				}
			break;


			case(R500_PRS_KBM):
				{
				  if (mtm.update_state_flag)
					  {

						  mtm.update_state_flag=0;


						  mtm.work_count=0;
						  START_VALVE_ON ;

						  PUMP_PWM=ZERO_POINT_PUMP+config.pump_test_pwm;

					  }


					  if(mtm.work_count>10)
					  {


						  mtm.update_state_flag=1;
						  mtm.engine_state=INITIAL_STAGE;
						  //START_VALVE_OFF;

					  }

					  mtm.work_count++;
				}
			break;

			case(R500_PRS_TRV):
				{
				  if (mtm.update_state_flag)
					  {

						  mtm.update_state_flag=0;

						  START_VALVE_ON ;
						  mtm.work_count=0;

						  PUMP_PWM=ZERO_POINT_PUMP+config.pump_test_pwm;

					  }


					  if(mtm.work_count>10)
					  {


						  mtm.update_state_flag=1;
						  mtm.engine_state=INITIAL_STAGE;
						  //START_VALVE_OFF ;

					  }

					  mtm.work_count++;

				}
			break;


			case(R1):
				{

				}
			break;


		}

	break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(PUMP_U_ST):
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;


				case(R500):
					{
					  if (mtm.update_state_flag)
						  {

							  mtm.update_state_flag=0;


							  mtm.work_count=0;
							  mtm.threshold_count=0;

							  PUMP_PWM=Pump_PWM_Correct(1450,mtm.bus_volt);//ZERO_POINT_PUMP+config.pump_test_pwm;
							  START_VALVE_ON ;
							  //MAIN_VALVE_ON;

							  if(config.u_pump_test_crit)
							  {
								  mtm.u_pump_test_crit=config.u_pump_test_crit;
							  }

							  else
							  {
								  mtm.u_pump_test_crit=5000;
							  }
						  }


						  if(mtm.work_count>40)
						  {


							  mtm.update_state_flag=1;
							  mtm.engine_state=INITIAL_STAGE;
							  //START_VALVE_OFF ;

						  }


						  mtm.work_count++;


						  if((mtm.pump_speed<mtm.u_pump_test_crit)&&(mtm.pump_speed>2500))
						  {
							  mtm.threshold_count++;
						  }

						  else
						  {
							  mtm.threshold_count=0;
						  }


						  if(mtm.threshold_count>2)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=INITIAL_STAGE;
							  //START_VALVE_OFF ;
							  //MAIN_VALVE_OFF;
						  }

					}
				break;


				case(R500_PRS_KBM):
					{
					  if (mtm.update_state_flag)
						  {

							  mtm.update_state_flag=0;


							  mtm.work_count=0;
							  mtm.threshold_count=0;

							  PUMP_PWM=Pump_PWM_Correct(1450,mtm.bus_volt);//ZERO_POINT_PUMP+config.pump_test_pwm;
							  //START_VALVE_ON ;
						  }


						  if(mtm.work_count>40)
						  {


							  mtm.update_state_flag=1;
							  mtm.engine_state=INITIAL_STAGE;
							  //START_VALVE_OFF ;

						  }


						  mtm.work_count++;


						  if((mtm.pump_speed<3500)&&(mtm.pump_speed>2500))
						  {
							  mtm.threshold_count++;
						  }

						  else
						  {
							  mtm.threshold_count=0;
						  }


						  if(mtm.threshold_count>2)
						  {
							  mtm.update_state_flag=1;
							  mtm.engine_state=INITIAL_STAGE;
							  //START_VALVE_OFF ;
						  }
					}
				break;

				case(R500_PRS_TRV):
					{
					  if (mtm.update_state_flag)
						  {

							  mtm.update_state_flag=0;


							  mtm.work_count=0;

							  PUMP_PWM=ZERO_POINT_PUMP+config.pump_test_pwm;
							  START_VALVE_ON ;
						  }


						  if(mtm.work_count>20)
						  {


							  mtm.update_state_flag=1;
							  mtm.engine_state=INITIAL_STAGE;
							  START_VALVE_OFF ;

						  }

						  mtm.work_count++;
					}
				break;


				case(R1):
					{
					  if (mtm.update_state_flag)
						  {

							  mtm.update_state_flag=0;


							  mtm.work_count=0;
							  mtm.threshold_count=0;

							  PUMP_PWM=Pump_PWM_Correct(1250,mtm.bus_volt);//ZERO_POINT_PUMP+config.pump_test_pwm;

							 PUMP2_PWM=Pump_PWM_Correct_BIDIR(1200,mtm.bus_volt);//ZERO_POINT_PUMP+config.pump_test_pwm;

							  ADG_ON ;
						  }


						  if(mtm.work_count>10)
						  {


							  mtm.update_state_flag=1;
							  mtm.engine_state=INITIAL_STAGE;
							PUMP_PWM = ZERO_POINT_PUMP;
							PUMP2_PWM=ZERO_POINT_PUMP2;
							  ADG_OFF ;

						  }


						  mtm.work_count++;


					}
				break;


			}

		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(STAGE_VENT)://вент
			switch(config.engine_type)
			{
				case(R40):
				{

				}
				break;
				case(R500):
				{
					if(mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.next_setpoint=config.n1_vent;

						mtm.relevant_setpoint=1200;
						STARTER_PWM=ZERO_POINT_STARTER_WORK;


						pid_starter.integrator=(((float)config.starter_min)/1000.00);

						mtm.work_count=0;
						mtm.starter_move_count=0;
						mtm.threshold_count=0;
					}

					if (mtm.work_count<10)
					{
					  mtm.work_count++;
					}
					else
					{
						if ((mtm.rotor_speed>2000)&&(mtm.starter_move_count<10))
						{
							mtm.starter_move_count++;
						}

						if(mtm.starter_move_count>5)
						{
							mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
																	 config.starter_rate,config.starter_rate);

							STARTER_PWM=PIDController_Update(&pid_starter,mtm.relevant_setpoint,mtm.rotor_speed,0,1000);
						}
						else
						{
							if   (STARTER_PWM<((config.starter_min)+1000))
							{
								if((STARTER_PWM+20)<=((config.starter_min)+1000))
								{
									STARTER_PWM= STARTER_PWM+20;
								}
								else
								{
									STARTER_PWM= ((config.starter_min)+1000);
								}
							}
						}

						if(mtm.t_real<config.t_vent)
						{
							if(mtm.threshold_count>config.time_vent*REGULATION_FREQ)
							 {
								 mtm.update_state_flag=1;
								 mtm.engine_state=INITIAL_STAGE;
							 }
							else
							{
								mtm.threshold_count++;
							}
						}
					}
				}
				break;
				case(R500_PRS_KBM):
				{
					if(mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.next_setpoint=config.n1_vent;

						mtm.relevant_setpoint=1200;
						STARTER_PWM=ZERO_POINT_STARTER_WORK;


						pid_starter.integrator=(((float)config.starter_min)/1000.00);

						mtm.work_count=0;
						mtm.starter_move_count=0;
						mtm.threshold_count=0;

					}

					if (mtm.work_count<10)
					{
					  mtm.work_count++;
					}
					else
					{
						if ((mtm.rotor_speed>2000)&&(mtm.starter_move_count<10))
						{
							mtm.starter_move_count++;
						}

						if(mtm.starter_move_count>5)
						{
							mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
															 config.starter_rate,config.starter_rate);

							STARTER_PWM=PIDController_Update(&pid_starter,mtm.relevant_setpoint,mtm.rotor_speed,0,1000);
						}
						else
						{
							if   (STARTER_PWM<((config.starter_min)+1000))
							{
								if((STARTER_PWM+20)<=((config.starter_min)+1000))
								{
									STARTER_PWM= STARTER_PWM+20;
								}
								else
								{
									STARTER_PWM= ((config.starter_min)+1000);
								}
							}
						}





						if(mtm.t_real<config.t_vent)
						{
							if(mtm.threshold_count>config.time_vent*REGULATION_FREQ)
							{
								mtm.update_state_flag=1;
								mtm.engine_state=INITIAL_STAGE;
							}
							else
							{
								mtm.threshold_count++;
							}
						}
					}

				}
				break;
				case(R500_PRS_TRV):
				{
					if(mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.next_setpoint=config.n1_vent;

						mtm.relevant_setpoint=1200;

						pid_starter.integrator=(((float)config.starter_min)/1000.00);

						mtm.work_count=0;
						mtm.starter_move_count=0;
						mtm.threshold_count=0;
						STARTER_PWM=ZERO_POINT_STARTER_WORK;
					}

					if (mtm.work_count<10)
					{
						mtm.work_count++;
					}
					else
					{
						if ((mtm.rotor_speed>2000)&&(mtm.starter_move_count<10))
						{
							mtm.starter_move_count++;
						}

						if(mtm.starter_move_count>5)
						{
							mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
																 config.starter_rate,config.starter_rate);

							STARTER_PWM=PIDController_Update(&pid_starter,mtm.relevant_setpoint,mtm.rotor_speed,0,1000);
						}
						else
						{
							if   (STARTER_PWM<((config.starter_min)+1000))
							{
								if((STARTER_PWM+20)<=((config.starter_min)+1000))
								{
									STARTER_PWM= STARTER_PWM+20;
								}
								else
								{
									STARTER_PWM= ((config.starter_min)+1000);
								}
							}
						}

						if(mtm.t_real<config.t_vent)
						{
							if(mtm.threshold_count>config.time_vent*REGULATION_FREQ)
							{
								mtm.update_state_flag=1;
								mtm.engine_state=INITIAL_STAGE;
							}
							else
							{
								mtm.threshold_count++;
							}
						}
					}
				}
				break;
				case(R1):
				{
					if(mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.next_setpoint=config.n1_vent;

						mtm.relevant_setpoint=1200;



						pid_starter.integrator=(((float)config.starter_min)/1000.00);

						mtm.work_count=0;
						mtm.starter_move_count=0;
						mtm.threshold_count=0;

						STARTER_PWM=ZERO_POINT_STARTER_WORK;

					}

					if (mtm.work_count<10)
					{
						mtm.work_count++;
					}
					else
					{
						if ((mtm.rotor_speed>2000)&&(mtm.starter_move_count<10))
						{
							mtm.starter_move_count++;
						}

						if(mtm.starter_move_count>5)
						{
							mtm.relevant_setpoint=Get_next_setpoint(mtm.next_setpoint,mtm.relevant_setpoint,
														 config.starter_rate,config.starter_rate);

							STARTER_PWM=PIDController_Update(&pid_starter,mtm.relevant_setpoint,mtm.rotor_speed,0,1000);
						}
						else
						{
							if   (STARTER_PWM<((config.starter_min)+1000))
							{
								if((STARTER_PWM+20)<=((config.starter_min)+1000))
								{
									STARTER_PWM= STARTER_PWM+20;
								}
								else
								{
									STARTER_PWM= ((config.starter_min)+1000);
								}
							}
						}





						if(mtm.t_real<config.t_vent)
						{
							if(mtm.threshold_count>config.time_vent*REGULATION_FREQ)
							{
								mtm.update_state_flag=1;
								mtm.engine_state=INITIAL_STAGE;
							}
							else
							{
								mtm.threshold_count++;
							}
						}
					}

				}
				break;

			}
		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(SHECK_PRS_1):
			switch(config.engine_type)
			{
				case(R40):
					{

					}
				break;


				case(R500):

				break;


				case(R500_PRS_KBM):

				break;

				case(R500_PRS_TRV):
					{
						  if (mtm.update_state_flag)
						  {

							  mtm.update_state_flag=0;

							  PLUG_ON ;

							  mtm.work_count=0;

						  }

						  if(mtm.work_count>10)
						  {
								 mtm.update_state_flag=1;
								 mtm.engine_state=INITIAL_STAGE;
						  }


						  mtm.work_count++;

					}
				break;



				case(R1):
					{

					}
				break;


			}

		break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(SHECK_PRS_2):
		switch(config.engine_type)
		{
			case(R40):
				{

				}
			break;


			case(R500):

			break;


			case(R500_PRS_KBM):

			break;

			case(R500_PRS_TRV):
				{
					  if (mtm.update_state_flag)
					  {

						  mtm.update_state_flag=0;

						  ADG_ON;

						  mtm.work_count=0;

					  }

					  if(mtm.work_count>10)
					  {
							 mtm.update_state_flag=1;
							 mtm.engine_state=INITIAL_STAGE;
					  }


	     			  mtm.work_count++;

				}
			break;



			case(R1):
				{

				}
			break;


		}

	break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(HALL_ERR_STATE)://отказ дх
			switch(config.engine_type)
			{
				case(R40):
				{

				}
				break;
				case(R500):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.flameout_work_count=0;

						pid_t4_hall_err.integrator=((float)PUMP_PWM-1000)/pid_t4_hall_err.delta;

						mtm.work_count=0;
					}

					PUMP_PWM= PIDController_Update(&pid_t4_hall_err, 500+(mtm.trotle/10), mtm.t_real,0,ZERO_POINT_PUMP_WORK);

					if((mtm.t_real<config.t_flameout)||(mtm.rotor_speed<30000))//погасла камера
					{
						mtm.flameout_work_count++;
						if(mtm.flameout_work_count>30)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=FLY_RESTART;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						}
					}
					else
					{
						mtm.flameout_work_count=0;
					}

				}
				break;
				case(R500_PRS_KBM):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.flameout_work_count=0;

						pid_t4_hall_err.integrator=((float)PUMP_PWM-1000)/pid_t4_hall_err.delta;

						mtm.work_count=0;
					}

					PUMP_PWM= PIDController_Update(&pid_t4_hall_err, 500+(mtm.trotle/10), mtm.t_real,0,ZERO_POINT_PUMP_WORK);

					if((mtm.t_real<config.t_flameout)||(mtm.rotor_speed<30000))//погасла камера
					{
						mtm.flameout_work_count++;
						if(mtm.flameout_work_count>30)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=FLY_RESTART;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						}
					}
					else
					{
						mtm.flameout_work_count=0;
					}

				}
				break;
				case(R500_PRS_TRV):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.flameout_work_count=0;

						pid_t4_hall_err.integrator=((float)PUMP_PWM-1000)/pid_t4_hall_err.delta;

						mtm.work_count=0;
					}

					PUMP_PWM= PIDController_Update(&pid_t4_hall_err, 500+(mtm.trotle/10), mtm.t_real,0,ZERO_POINT_PUMP_WORK);

					if((mtm.t_real<config.t_flameout)||(mtm.rotor_speed<30000))//погасла камера
					{
						mtm.flameout_work_count++;
						if(mtm.flameout_work_count>30)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=FLY_RESTART;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						}
					}
					else
					{
						mtm.flameout_work_count=0;
					}

				}
				break;
				case(R1):
				{
					if (mtm.update_state_flag)
					{
						mtm.update_state_flag=0;

						mtm.flameout_work_count=0;

						pid_t4_hall_err.integrator=((float)PUMP_PWM-1000)/pid_t4_hall_err.delta;

						mtm.work_count=0;
					}

					PUMP_PWM= PIDController_Update(&pid_t4_hall_err, 500+(mtm.trotle/10), mtm.t_real,0,ZERO_POINT_PUMP_WORK);

					if((mtm.t_real<config.t_flameout)||(mtm.rotor_speed<30000))//погасла камера
					{
						mtm.flameout_work_count++;
						if(mtm.flameout_work_count>30)
						{
							mtm.update_state_flag=1;
							mtm.engine_state=FLY_RESTART;
							mtm.engine_flag=mtm.engine_flag|(FLAMEOUT);
						}
					}
					else
					{
						mtm.flameout_work_count=0;
					}

				}
				break;
			}
			break;
//////////////////////////////////////////////////////////////////////////////////////////////
		case(CONFIG_ERR):


		break;
	}

return 0;
}






uint32_t Get_next_setpoint ( uint32_t setpoint, uint32_t parameter,
								uint32_t rate_up, uint32_t rate_down)
{
//при изменении частоты управления изменить делитель рэйтов в конфиге сейчас /10





	  if((setpoint>parameter)   &&  ((setpoint-parameter)>rate_up))
		  {
			  return (parameter+rate_up);

		  }

	  else if((setpoint<parameter)   &&  ((parameter-setpoint)>rate_down))
	 	  {
	 		  return (parameter-rate_down);
	 	  }

	  else
	  {
		  return setpoint;
	  }
}


uint32_t Starter_Curve(uint32_t X)
{
	float x1=0,   x2=20000;
	float y1=1250,y2=2000;
	float L=0;

	if(X>x2)
	{
		L=y2;
	}
	else
	{
	 L= (y1+(((y2-y1)/(x2-x1))*(X-x1)));


	}
	 return L;
}



uint16_t Valve_Curve( float X)
{
	X=X/10;

	// . за пределами точек?
	if (X <= (config.curve_1_x[0]))
	{
		return (config.curve_1_y[0]*10);

	}


	for (int i = 0; i < 14; ++i)
	{

		if (X==(config.curve_1_x[i]))
		{
			return (config.curve_1_y[i]*10);
		}

		if(config.curve_1_x[i+1]==0)
		{
			return (config.curve_1_y[i]*10);
		}


		if ((X > (config.curve_1_x[i] ))&& (X<(config.curve_1_x[i + 1])))	// . между точками?

		{
			if(config.curve_1_y[i+1]<config.curve_1_y[i])
			{
				 return ((config.curve_1_y[i]-(((float)(config.curve_1_y[i]-config.curve_1_y[i+1])/
						(float)(config.curve_1_x[i+1]-config.curve_1_x[i]))*(float)(X-config.curve_1_x[i])))*10);
			}
			else
			{
				return ((config.curve_1_y[i]+(((float)(config.curve_1_y[i+1]-config.curve_1_y[i])/
						(float)(config.curve_1_x[i+1]-config.curve_1_x[i]))*(float)(X-config.curve_1_x[i])))*10);
			}


		}
	}



	return (config.curve_1_y[14]*10);
}


uint16_t Start_Valve_Curve( float X)
{

		X=X/10;

		// . за пределами точек?
		if (X <= (config.curve_2_x[0]))
		{
			return (config.curve_2_y[0]*10);

		}


		for (int i = 0; i < 14; ++i)
		{

			if (X==(config.curve_2_x[i]))
			{
				return (config.curve_2_y[i]*10);
			}

			if(config.curve_2_x[i+1]==0)
			{
				return (config.curve_2_y[i]*10);
			}


			if (X > (config.curve_2_x[i]) && X<(config.curve_2_x[i + 1]))	// . между точками?

			{
				if(config.curve_2_y[i+1]<config.curve_2_y[i])
				{
					 return ((config.curve_2_y[i]-(((float)(config.curve_2_y[i]-config.curve_2_y[i+1])/
							(float)(config.curve_2_x[i+1]-config.curve_2_x[i]))*(float)(X-config.curve_2_x[i])))*10);
				}
				else
				{
					return ((config.curve_2_y[i]+(((float)(config.curve_2_y[i+1]-config.curve_2_y[i])/
							(float)(config.curve_2_x[i+1]-config.curve_2_x[i]))*(float)(X-config.curve_2_x[i])))*10);
				}


			}
		}



		return (config.curve_2_y[14]*10);


}
float Pump_Curve( float X)
{

	X=X/10;

	// . за пределами точек?
	if (X <= (config.fuel_curve_x[0]))
	{
		return (config.fuel_curve_y[0]);

	}


	for (int i = 0; i < 14; ++i)
	{

		if (X==config.fuel_curve_x[i])
		{
			return (config.fuel_curve_y[i]);
		}

		if(config.fuel_curve_x[i+1]==0)
		{
			return (config.fuel_curve_y[i]);
		}


		if (X > (config.fuel_curve_x[i] )&& (X<config.fuel_curve_x[i + 1]))	// . между точками?

		{
			if(config.fuel_curve_y[i+1]<config.fuel_curve_y[i])
			{
				 return (config.fuel_curve_y[i]-((((float)(config.fuel_curve_y[i]-config.fuel_curve_y[i+1]))/
						((float)(config.fuel_curve_x[i+1]-config.fuel_curve_x[i])))*((float)(X-config.fuel_curve_x[i]))));


			}
			else
			{
				return ((config.fuel_curve_y[i]+(((float)(config.fuel_curve_y[i+1]-config.fuel_curve_y[i])/
						(float)(config.fuel_curve_x[i+1]-config.fuel_curve_x[i]))*(float)(X-config.fuel_curve_x[i]))));
			}


		}
	}



	return (config.fuel_curve_y[14]);
}

uint16_t Starter_PWM_Correct(uint16_t n,float bus_volt)
{
//	float k=0.032;
//	float b=150;
//	float c=3;

	float res=0;

	//res=((config.starter_k*(float)n)+config.starter_b+1000+((27-bus_volt)*config.starter_c));

	res=((config.starter_k*n*(27/bus_volt))+config.starter_b+1000+((27-bus_volt)*config.starter_c));

	if (res<1000)
	{
		res=1000;
	}
	else if(res>2000)
	{
		res=2000;
	}

	return res;
}

uint16_t Pump_PWM_Correct (uint16_t pwm, float bus_volt)


{
	static float res=0;

	res=((27.00/bus_volt)*(pwm-930))+930;

	if (res<1000)
	{
		res=1000;
	}
	else if(res>2000)
	{
		res=2000;
	}

	return res;
}



uint16_t Pump_PWM_Correct_BIDIR (uint16_t pwm, float bus_volt)
{
	static float bi_res=0;

	bi_res=((27.00/bus_volt)*(pwm-1500))+1500;

	if (bi_res<1000)
	{
		bi_res=1000;
	}
	else if(bi_res>2000)
	{
		bi_res=2000;
	}

	return bi_res;
}
uint16_t Plug_Curve( float X)//ривая свечи подогрева
{

	  uint16_t L=0;

		 uint8_t n = 11;
	     //float y[11] = {  563,  444, 360 ,  298,   250,   213,  184,  160, 141, 125, 111};{  374,  295,  239,  199,   166,   142,  122,  106, 94,  84,  74}

		 float y[11] = {  299,  236,  191,  159,   133,   114,  98,  85, 75,  67,  59};//для мощной свечи
		 float x[11] = {   16,   18,   20,   22,    24,    26,   28,   30,  32,  34,  36};





			for (int i = 0; i < n-1; ++i)
			{
				if (X == x[i])// . в точках?
				{

						L=y[i];

				}
				if (X >= x[i] &&X <= x[i + 1])	// . между точками?

				{


						if(y[i+1]>y[i])

					    L= (y[i]+(((y[i+1]-y[i])/(x[i+1]-x[i]))*(X-x[i])));

						else

							  L= (y[i]-(((y[i]-y[i+1])/(x[i+1]-x[i]))*(X-x[i])));


				}
			}


			if (L>config.hot_pwm_correct)
			{
				L=L-config.hot_pwm_correct;
			}

		return L;

}






void Overheating_Control_Start(uint8_t time)
{
	if(mtm.t_real>config.t_start_max)
	{
		if (mtm.t_over_count>time)
		{
			mtm.engine_flag=mtm.engine_flag|CC_HOT;
		}
		mtm.t_over_count++;

	}
	else
	{
		mtm.t_over_count=0;
		mtm.engine_flag=mtm.engine_flag&(~CC_HOT);
	}
}
void Overheating_Control_Operation(uint8_t time)
{
	if (mtm.t_real>config.t_oper_max)
	{
		if (mtm.t_over_count>time)
		{
			mtm.engine_flag=mtm.engine_flag|TG_HIGH;
		}
		mtm.t_over_count++;

	}
	else
	{
		mtm.t_over_count=0;
		mtm.engine_flag=mtm.engine_flag&(~TG_HIGH);
	}
}

float derivative(float value)
{
	  static float old_value;
	  static int n;
	  static float m[10];
	  static float y;

	  static float DT;
	  DT=value-old_value;
	  old_value=value;

	  y=y+(DT-m[n]);
	  m[n]=DT;
	  n=(n+1)%10;
	  return y;
}
