#ifndef INC_RS_232_H_
#define INC_RS_232_H_

#include "main.h"

extern UART_HandleTypeDef huart1;
extern struct RS_232 rs_232;


struct RS_232
{
	uint8_t rx_buff[20];
	uint8_t rx_flag;
	uint8_t rx_frame_size;

	uint8_t tx_flag;

    uint8_t used_flag;
    uint8_t init_flag;//выставляется при инициализации порта
    uint16_t baud_rate;

    uint16_t frame_err_count;

    uint16_t rx_idle_counter;//для фиксации отсутствия пакетов по рх

    uint8_t trv_tx_state;//определяет номер пакета на отправку для трв

    uint8_t last_com;//последняя команда пришедшая по 232

    uint16_t counter;//счётчик номера пакета
    uint16_t rx_counter;//счетчик входящих пакетов

    uint8_t com_start_count;//счетчик прихода старта наземного
    uint8_t com_start1_count;//счетчик прихода старта 1
    uint8_t com_start2_count;//счетчик прихода старта 2

    uint8_t com_stop_count;//счетчик прихода стопа наземного
    uint8_t com_stop1_count;//счетчик прихода стопа 1
    uint8_t com_stop2_count;//счетчик прихода стопа 2

    uint8_t trv_prs1_count;//счетчик тестовой команды для пиросвечи 1
    uint8_t trv_prs2_count;//счетчик тестовой команды для пиросвечи 2


};

#pragma pack(1)
struct TM_RS_232_UZGA
{
	uint8_t start_byte;					//1
	uint8_t com_req;					//2
	uint16_t n1;						//3,4
	int16_t Tg;							//5,6
	uint16_t state;						//7,8
	uint16_t flag;						//9,10
	uint16_t trottle;					//11,12
	uint16_t bus_volt;					//13,14
	uint16_t starter_pwm;				//15,16
	uint16_t fuel_pwm;					//17,18
	uint16_t fuel_rpm;					//19,20
	uint16_t frame_err_counter;			//21,22
	uint16_t counter;					//23,24
	uint8_t ver;						//25
	uint8_t res1;						//26
	uint16_t checksum;				    //27,28
	uint16_t current;				    //29,30
	uint8_t crc;						//31
	uint8_t stop_byte;					//32

};
#pragma pack(0)

#pragma pack(1)
struct TM_RS_232_TRV_1ID
{
	uint8_t start_byte;
	uint8_t id;
	uint8_t com_req;
	uint16_t n1;
	int16_t Tg;
	uint16_t state;
	uint16_t counter;
	uint8_t crc;
	uint8_t stop_byte;
};
#pragma pack(0)

#pragma pack(1)
struct TM_RS_232_TRV_2ID
{
	uint8_t start_byte;
	uint8_t id;
	uint8_t com_req;
	uint16_t flag;
	uint16_t trottle;
	uint16_t bus_volt;
	uint16_t counter;
	uint8_t crc;
	uint8_t stop_byte;

};
#pragma pack(0)

#pragma pack(1)
struct TM_RS_232_TRV_3ID
{
	uint8_t start_byte;
	uint8_t id;
	uint8_t com_req;
	uint16_t starter_pwm;
	uint16_t fuel_pwm;
	uint16_t fuel_rpm;
	uint16_t counter;
	uint8_t crc;
	uint8_t stop_byte;

};
#pragma pack(0)

#pragma pack(1)
struct TM_RS_232_TRV_4ID
{
	uint8_t start_byte;
	uint8_t id;
	uint8_t com_req;
	uint16_t frame_err_counter;
	uint16_t res1;
	uint16_t res2;
	uint16_t counter;
	uint8_t crc;
	uint8_t stop_byte;
};
#pragma pack(0)

#pragma pack(1)
struct TEST
{
	uint8_t start_byte;
	uint8_t com;
	uint16_t rud;
	uint16_t counter;
	uint8_t crc;
	uint8_t stop_byte;
};
#pragma pack(0)

uint8_t Calc_CRC8(uint8_t *data, uint8_t lenght, uint8_t seed);

void RS_232_Frame_Processing();
void RS_232_Frame_err();
void RS_232_Send_tm();
void TRV_TX();
void UZGA_TX();
void TRV_RX();
void UZGA_RX();
void RS_232_Baud_Rate_CH();
void Transmite_232();
void Receive_232();
void Test(uint8_t com, uint16_t rud);


#define RS232_RX_FRAME_SIZE 		    4
#define RS232_RX_FRAME_START_BYTE		0xFF
#define RS232_RX_MAX_FRAME_SIZE  		8

#define RS232_TX_FRAME_SIZE  			7



#endif /* INC_RS_232_H_ */
