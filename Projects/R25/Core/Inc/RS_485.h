#ifndef _RS_485
#define _RS_485



#include "main.h"
#include "usart.h"

extern TIM_HandleTypeDef htim20;

uint8_t RS_SU_Pack(uint8_t *data,uint16_t size,uint8_t screen_byte);
uint8_t RS_SU_RePack(uint8_t *data, uint16_t size, uint8_t screen_byte);

uint8_t Frame_Test();

struct RS
{
	uint8_t rs485_rx_flag;
	uint8_t rs485_tx_flag;

	uint16_t receive_package_size;

	uint8_t xor;
	uint8_t i;

	uint8_t config_complate_flag;
	uint8_t config_updating;

	uint8_t address;
	uint8_t group_address;

	uint8_t rs485_rx_buff[256];
	uint8_t rs485_tx_buff[256];

	uint8_t req_count;
	uint8_t bsu_count;
	uint16_t bsu_count_new;
	uint16_t bsu_count_old;

	uint8_t rud_count;

    uint8_t start_count;
    uint8_t start1_count;
    uint8_t start2_count;

    uint8_t stop_count;
    uint8_t stop1_count;
    uint8_t stop2_count;

    uint8_t fly_start_flag;

    uint8_t used_flag;//используется для перевода сау в стендовый режим, если проходит первая команда загрузка конфига, то переход в стендовый

    uint8_t rs485_active_count;
/*

	uint8_t ModbusRX[300];
	uint8_t ModbusTX[20]={0xFE,0xFE,0xC1,0,1,2,3,4,5,6,7,8,9,0x00,0xFF,0xFF};
	uint8_t i=0;
	uint32_t test=0;
	uint8_t count_start_byte=0;//количество байт в заголовке пакета
	uint8_t count_stop_byte=0;//количество байт в конце пакета
	=0;

	uint8_t xor=0;
	uint8_t arr_size_tx=0;
	uint8_t usb_rx_flag=0;


	//uint8_t advence_tm_buff[77]={0xFE,0xFE,ADDRESS};
	uint32_t time=0;

	uint8_t standard_response[STANDARD_ANSWER_SIZE+2]={0xFE,0xFE,0xA1,0x00,0x00,0xFF,0xFF};//расширенный размер на случай добавления экранирования
	//uint8_t tm_response[TM_ANSWER_SIZE+30]={0xFE,0xFE,ADDRESS,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0xFF,0xFF};
	//	uint8_t tm_response_new[TM_ANSWER_SIZE]={0xFE,0xFE,ADDRESS,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0xFF,0xFF};
	uint8_t tm_update_flag=1;



	uint8_t screen_numb=0;//количество экранированных байт


	uint8_t rs_control_state=0;//для контроля приходов пакетов

	uint8_t rs_req_count=0;
	uint8_t rs_rud_count=0;
	uint8_t rs_bsu_count=0;

	uint16_t rs_old_bsu_count=0;
	uint16_t rs_new_bsu_count=0;

	uint16_t sau_to_bsu_count=0;



	uint8_t rs_start_stop_err_count=0;
	uint8_t rs_start_count=0;
	uint8_t rs_start1_count=0;
	uint8_t rs_start2_count=0;

	uint8_t rs_stop_count=0;
	uint8_t rs_stop1_count=0;
	uint8_t rs_stop2_count=0;

	uint8_t rs_flag_ok=0;
	uint8_t rs_state_count=0;

	uint8_t rs_com1_state=0;
	uint8_t rs_com2_state=0;
	uint8_t rs_com3_state=0;
	uint8_t rs_com4_state=0;


	uint8_t rs_com1_count=0;
	uint8_t rs_com2_count=0;
	uint8_t rs_com3_count=0;
	uint8_t rs_com4_count=0;
	uint8_t flag_combat_start=0;
	*/
};


#pragma pack(1)
struct RS_Standart_TM_Frame
{
	uint16_t start_frame;

	uint8_t req_address;
	uint8_t req_err;

	uint16_t n1;
	int16_t tg;
	uint16_t state;
	uint16_t flag;
	uint16_t rud;
	uint16_t bus_volt;
	uint16_t starter_pwm;
	uint16_t pump_pwm;
	uint16_t pump_fb;
	uint16_t counter;

	uint8_t xor;
	uint8_t screen_byte;

	uint16_t end_frame;
};
#pragma pack(0)

#pragma pack(1)
struct STANDART_REQ
{
	uint16_t start_frame;

	uint8_t req_address;
	uint8_t req_command;

	uint8_t xor;

	uint8_t screen_byte;

	uint16_t end_frame;
};
#pragma pack(0)


#pragma pack(1)
struct RS_Service_TM_Frame
{
    uint16_t start_frame;

    uint8_t req_address;
    uint8_t req_err;

    uint8_t n1_h;
    uint8_t n1_l;

    int16_t tg;


    uint8_t state_h;
    uint8_t state_l;

    uint8_t flag_h;
    uint8_t flag_l;

    uint8_t rud_h;
    uint8_t rud_l;

    uint8_t bus_volt_h;
    uint8_t bus_volt_l;

    uint8_t starter_pwm_h;
    uint8_t starter_pwm_l;

    uint8_t pump_pwm_h;
    uint8_t pump_pwm_l;

    uint8_t pump_fb_h;
    uint8_t pump_fb_l;

    uint8_t pump_cur_h;
    uint8_t pump_cur_l;

    uint8_t valve1_cur_h;
    uint8_t valve1_cur_l;

    uint8_t valve2_cur_h;
    uint8_t valve2_cur_l;

    uint8_t plug_cur_h;
    uint8_t plug_cur_l;

    uint8_t valve1_pwm_h;
    uint8_t valve1_pwm_l;

    uint8_t valve2_pwm_h;
    uint8_t valve2_pwm_l;

    uint8_t plug_pwm_h;
    uint8_t plug_pwm_l;

    uint8_t t_gen_h;
    uint8_t t_gen_l;

    int16_t t_diod;

    uint8_t t1_h;
    uint8_t t1_l;

    uint8_t t2_h;
    uint8_t t2_l;

    uint8_t t3_h;
    uint8_t t3_l;

    uint8_t u1_h;
    uint8_t u1_l;

    uint8_t u2_h;
    uint8_t u2_l;

    uint8_t s1_h;
    uint8_t s1_l;

    uint8_t s2_h;
    uint8_t s2_l;

    uint8_t s3_h;
    uint8_t s3_l;

    uint8_t s4_h;
    uint8_t s4_l;

    uint8_t s5_h;
    uint8_t s5_l;

    uint8_t s6_h;
    uint8_t s6_l;

    uint8_t s7_h;
    uint8_t s7_l;

    uint8_t s8_h;
    uint8_t s8_l;

    uint8_t s9_h;
    uint8_t s9_l;

    uint8_t s10_h;
    uint8_t s10_l;

    uint8_t tim_3byte;
    uint8_t tim_2byte;
    uint8_t tim_1byte;
    uint8_t tim_0byte;

    uint8_t xor;
    uint8_t screen_byte;

    uint16_t end_frame;
};
#pragma pack(0)


#pragma pack(1)
struct CRC_Frame
{
    uint16_t start_frame;

    uint8_t req_address;
    uint8_t req_err;

    uint32_t chip_crc;

    uint8_t frame_crc;
    uint16_t end_frame;
};
#pragma pack(0)

struct RS_Service_Direct_Frame
{
    uint16_t starter_pwm;
    uint16_t pump_pwm;
    uint16_t valve1_pwm;
    uint16_t valve2_pwm;
    uint16_t plug_pwm;

    uint8_t com_check;

    uint8_t com1;
    uint8_t com2;
    uint8_t com3;

    uint8_t var_flag;//поднимается при ручном запросе, опускается при обычном запрос
};

extern struct RS rs;


//RS485-------------------------------------------------------------------------------------------------------------------------
#define PACKAGE_START 0xFEFE //НАЧАЛО ПАКЕТА
#define PACKAGE_STOP 0xFFFF  //КОНЕЦ ПАКЕТА

#define DIRECT_FRAME_SIZE 						7
#define DIRECT_SCREEN_FRAME_SIZE 				8

#define RUD_COUNT_FRAME_SIZE 					9
#define RUD_COUNT_SCREEN_FRAME_SIZE			 	10

#define FAT_SERVISE_FRAME_SIZE					15

#define SERVICE_FRAME_SIZE 						21
#define SERVICE_SCREEN_FRAME_SIZE			 	22

#define CONFIG_FRAME_SIZE						256

#define DEFAULT_ADDRESS							0xA1

#define RX_PACKAGE_SIZE 250


#define DATA_REQUEST 0X95    //ЗАПРОС ДАННЫХ
#define SERVICE_DATA_REQUEST 0xB1
#define CRC_REQUEST 		0xB2
#define CHECK 0x26           //ПРОВЕРКА
#define AUTO_PUMP 0x25		 //АВТО ПРОКАЧКА
#define PUMP 0x20		     //ПРОКАЧКА
#define START 0x21		     //СТАРТ
#define START1 0x30	         //БОЕВОЙ СТАРТ 1
#define START2 0x31	         //БОЕВОЙ СТАРТ 2
#define SET_RUD 0x84         //УСТАВКА РУД
#define BSU_COUNTER 0X50	 //ПЕРЕДАЧА СЧЁТЧ�?КА БСУ
#define STOP 0x22			 //СТОП
#define STOP1 0x40			 //БОЕВОЙ СТОП 1
#define STOP2 0x41			 //БОЕВОЙ СТОП 2
#define VENT 0x23			 //ВЕНТ
#define SEND_CINFIG 0x96	 //ОТПРАВ�?ТЬ КОНФ�?Г
#define LOAD_CONFIG1 0x97	 //ГОТОВНОСТЬ К ЗАГРУЗКЕ КОНФ�?ГА
#define LOAD_CONFIG2 0x98	 //ЗАГРУЗКА КОНФ�?Г
#define RESET 0x24           //сброс контроллеров

#define SEND_FAT  		0xC1//выгрузить фат таблицу
#define SEND_TM_FILE 	 0xC2//выгрузить запрошенный файл

#define SEND_OPER_COUNT  0xC3//выгрузить счетчик моточасов
#define RES_OPER_COUNT   0xC4//сброс счётчика моточасов
#endif
