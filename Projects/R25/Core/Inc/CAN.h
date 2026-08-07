/*
 * CAN.h
 *
 *  Created on: 25 июн. 2025 г.
 *      Author: user
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_
#include "main.h"

extern struct CAN can;
extern FDCAN_HandleTypeDef hfdcan1;
//include in main before while this code
/*
  TxHeader_tm.BitRateSwitch=FDCAN_BRS_OFF;
  TxHeader_tm.DataLength=FDCAN_DLC_BYTES_8;
  TxHeader_tm.ErrorStateIndicator=FDCAN_ESI_ACTIVE;
  TxHeader_tm.FDFormat=FDCAN_FD_CAN;
  TxHeader_tm.IdType=FDCAN_STANDARD_ID;
  TxHeader_tm.Identifier=REQ_TM_FRAME_ID;
  TxHeader_tm.TxFrameType=FDCAN_DATA_FRAME;

  TxHeader_ver.BitRateSwitch=FDCAN_BRS_OFF;
  TxHeader_ver.DataLength=FDCAN_DLC_BYTES_8;
  TxHeader_ver.ErrorStateIndicator=FDCAN_ESI_ACTIVE;
  TxHeader_ver.FDFormat=FDCAN_FD_CAN;
  TxHeader_ver.IdType=FDCAN_STANDARD_ID;
  TxHeader_ver.Identifier=REQ_VER_FRAME_ID;
  TxHeader_ver.TxFrameType=FDCAN_DATA_FRAME;


	  can.tx_buff_ver[0]=DUm>>8;
	  can.tx_buff_ver[1]=DUm;
	  can.tx_buff_ver[2]=DUg>>8;
	  can.tx_buff_ver[3]=DUg&0xFF;
	  can.tx_buff_ver[4]=DUver>>8;
	  can.tx_buff_ver[5]=DUver;

HAL_FDCAN_Start(&hfdcan1);
HAL_FDCAN_ActivateNotification(&hfdcan1,FDCAN_IT_LIST_RX_FIFO0|FDCAN_IT_LIST_RX_FIFO1
								|FDCAN_IT_LIST_SMSG|FDCAN_IT_LIST_TX_FIFO_ERROR,8);
 */
//CAN--------------------------------------------

#define NUMBER_SLOT_FRAME_ID 		0x08F
#define DIRECTION_FRAME_ID 			0x097
#define SEND_VERSION_FRAME_ID	 	0x089
#define NUMBER_DEV_VERSION   		70

#define REQ_TM_FRAME_ID 			0x0CF
#define REQ_TM2_FRAME_ID 			0x0D7

#define REQ_VER_FRAME_ID 			0x4E7

#define DUm							12//месяц компиляции ПО ДУ, диапазон от 1 до 12,ц.м.р. = 1
#define DUg							0x7E9//год компиляции ПО ДУ, диапазон от 2000 до 65535,ц.м.р. = 1
#define DUver						4//– версия ПО ДУ (определяется разработчиком блока)
struct CAN
{
    uint8_t N;//номер тайм-слота, диапазон от 1 до 5;
    uint8_t dS;//чет периодов информационного обмена, диапазон от 0 до 199, цена младшего разряда (далее – ц.м.р.) = 5 мс

    uint8_t SU;
    uint8_t Rud;
    uint8_t flag_dir;//флаг прихода команды

    uint8_t flag_ver_req;//отправить кадр с версией по

    uint8_t ss;

    int16_t temp;//
    uint8_t trot;//0-100%
    uint16_t rotor_freq;//*10
    uint16_t pump_freq;

    int16_t bar;
    int8_t t_gen;
    uint8_t state;
    uint8_t starter_pwm;
    uint8_t pump_pwm;
    uint16_t err;

    uint8_t slow_flag; //выставляется при аварийном останове

    uint8_t flag_check_ok;//флаг прохождения начального самотестирования
    uint8_t check_count;//счётчик времени самоконтроля
    uint16_t engine_flag;

    uint8_t rx_buff_info[10];
    uint8_t rx_buff_dir[10];
    uint8_t tx_buff_tm[10];
    uint8_t tx_buff2_tm[10];
    uint8_t tx_buff_ver[10];

    uint8_t tx_tm_buff_numb;//номер кудра для отправки

    uint8_t flag_tm_req;//флаг запроса тм-отправить кадр тм
    uint8_t flag_tm2_req;//лаг запроса 2го кадра тм

    uint8_t req_count;//счетчик проеба кан запросов

    uint8_t flag_ex_ok;//флаг нормального обмена при 0 отправлять кадры тм по таймеру 1 сек

    uint8_t tx_flag;
    uint8_t rx_flag;

    uint16_t otlad1;
    uint16_t otlad2;
    uint16_t otlad3;

    uint16_t RK4;

    uint8_t shod_count;
    uint8_t start_count;

    uint8_t active_flag;
    uint8_t vsk_counter_sec;

    uint8_t up_state_flag;

    uint8_t err_flag;

    uint8_t used_flag;

    uint8_t activate_prs_flag;

    uint16_t counter_msek;

    uint8_t ss_complate_state;
};

void CAN_RX_Frame_Test();
void CAN_Timer_1_Hz();
void CAN_TM_Complate();
void CAN_SS_Complate();

#endif /* INC_CAN_H_ */
