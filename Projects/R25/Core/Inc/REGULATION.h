#ifndef _REG
#define _REG



#include "main.h"




uint8_t Regulation();
uint32_t Get_next_setpoint ( uint32_t setpoint, uint32_t parameter,
								uint32_t rate_up, uint32_t rate_down);

uint32_t Starter_Curve(uint32_t X);
uint16_t Valve_Curve( float X);
uint16_t Start_Valve_Curve( float X);
float Pump_Curve( float X);
uint8_t Regulation_init();
float derivative(float value);

uint16_t Starter_PWM_Correct(uint16_t n,float bus_volt);
uint16_t Pump_PWM_Correct (uint16_t pwm, float bus_volt);
uint16_t Pump_PWM_Correct_BIDIR (uint16_t pwm, float bus_volt);

uint16_t Plug_Curve( float X);

void Overheating_Control_Start(uint8_t time);
void Overheating_Control_Operation(uint8_t time);

//стэйты:
#define INITIAL_STAGE 		0          //начальный стэйт

#define START_1		  		1          //
#define START_2		  		2  				//
#define START_3		  		3 					//стэйты наземного пуска
#define START_4		  		4 				//
#define START_5		  		5          //

#define OPERATION     		6			 //наземная работа

#define RESTART      		7			 //наземный рестарт

#define CONFIG_ERR      	9  			 //ошибка загрузки конфига

#define PRE_PUMP      		10		     //прокачка перед стартом

#define FLY_START_1		 	11          //
#define FLY_START_2		 	12  			//
#define FLY_START_3		 	13 					//стэйты наземного пуска
#define FLY_START_4		 	14 				//
#define FLY_START_5		 	15          //

#define FLY_OPERATION      	16			 //боевая работа

#define FLY_RESTART         17			 //боевой рестарт

#define SLOWDOWN     		20			 //подготовка к венту
#define STAGE_VENT     		21       	 //вент


#define PUMP_ST	            22			 //ручная прокачка
#define PUMP_U_ST           23			 //автопрокачка

#define SHECK_VAL           24			 //проверка клапанов
#define SHECK_PUMP   		25			 //проверка насоса
#define SHECK_IGN    		26			 //проверка свечи
#define SHECK_STARTER  	    27			 //проверка стартера


#define SHECK_PRS_1			28			//проверка пиросвечи 1
#define SHECK_PRS_2			29			//проверка пиросвечи 2

#define HALL_ERR_STATE		30			//работа при отказе дх
//--------------------------------------------------------------------

//error flags
#define NO_RX		1//отказ связи 232
#define C_CONTROL 	2//нет термопары
#define A_CONTROL	4//не прошел расширенный контроль
#define STOP_RK		8//стоп
#define PLUG_NO		16//нет фидбека на расширенном
#define START_NO	32//не раскрутился либо нет фидбека на расширенном
#define IGN_FAIL	64//нерозжиг на 2 стэйте
#define CC_HOT		128//пергрев на старте
#define FLAMEOUT 	256//срыв горения либо зависание
#define FLPM_HGH	512//овер шим насоса
#define PUMP_NO		1024//нет фидбека на расширенном
#define TG_HIGH		2048//перегрев в работе
#define TB_HIGH		4096//перегрев катушек
#define TG_DI		8192//отсоединена термопара
#define VALVE_NO	16384//нет фидбека на расширенном
#define N1_HIGH		32768//превышение максимальных оборотов

//уставки по умолчанию
#define PIN				1234

#define ZERO_SETPOINT 800			   //сторожевой нулевой шим драйверов
#define BIDIR_ZERO_SETPOINT	1500       //сорожевой шим для насоса с двунаправленной прошивкой
#define ZERO_POINT_STARTER_WORK 1000

#define ZERO_POINT_PUMP 	900
#define ZERO_POINT_PUMP2 	1500

#define ZERO_POINT_PUMP_WORK 1000
#define DELTA_PUMP_PWM 10

#define T_VENT         100      //пороговое значение температуры для вента

#define REGULATION_FREQ 10.00 				//частота управления используется при вычислении рэйта и кэфов пидов

#define N1_MAX_DY 10000

#define R40 			0
#define R500			1
#define R500_PRS_KBM	2
#define R500_PRS_TRV	3
#define R1				4


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//каналы управления

#define STARTER_PWM			TIM1->CCR1
#define STARTER2_PWM		TIM1->CCR2
#define PUMP_PWM			TIM3->CCR1
#define PUMP2_PWM			TIM3->CCR2
#define START_VALVE_PWM		TIM4->CCR1
#define MAIN_VALVE_PWM		TIM4->CCR2

#define HOT_PWM				TIM8->CCR1



#define PLUG_ON // TODO: HAL_GPIO_WritePin(PRS_ON_GPIO_Port,PRS_ON_Pin,GPIO_PIN_SET);
#define PLUG_OFF // TODO: HAL_GPIO_WritePin(PRS_ON_GPIO_Port,PRS_ON_Pin,GPIO_PIN_RESET);

#define GEN_ON // TODO: HAL_GPIO_WritePin(DC_ON_GPIO_Port,DC_ON_Pin,GPIO_PIN_RESET);
#define GEN_OFF // TODO: HAL_GPIO_WritePin(DC_ON_GPIO_Port,DC_ON_Pin,GPIO_PIN_SET);

#define ADG_ON // TODO: HAL_GPIO_WritePin(ADG_ON_GPIO_Port,ADG_ON_Pin,GPIO_PIN_SET);
#define ADG_OFF // TODO: HAL_GPIO_WritePin(ADG_ON_GPIO_Port,ADG_ON_Pin,GPIO_PIN_RESET);

#define START_VALVE_ON  START_VALVE_PWM = 1000;
#define START_VALVE_OFF START_VALVE_PWM = 0;

#define MAIN_VALVE_ON  	MAIN_VALVE_PWM = 1000;
#define MAIN_VALVE_OFF  MAIN_VALVE_PWM=0;



#define KBM_PREPUMP_TIME            10
#define UZGA_PREPUMP_TIME			25
//
#endif
