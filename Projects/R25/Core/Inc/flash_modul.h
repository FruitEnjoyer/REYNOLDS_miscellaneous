#ifndef _FLASH___
#define _FLASH___



#include "usart.h"

extern SPI_HandleTypeDef hspi1;

#define FLASH_SPI_PTR           &hspi1

#define FLASH_CS_SELECT      HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET)
#define FLASH_CS_UNSELECT    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET)

#define DUMMY_BYTE              0x55

#define FLASH_WRITE_DISABLE     0x04
#define FLASH_WRITE_ENABLE      0x06
#define FLASH_CHIP_ERASE        0xC7 //0x60
#define FLASH_SECTOR_ERASE      0x21
#define FLASH_BLOCK_ERASE       0xD8
#define FLASH_FAST_READ         0x0B
#define FLASH_PAGE_PROGRAMM     0x12
#define FLASH_GET_JEDEC_ID      0x9F
#define FLASH_READ_STATUS_1     0x05
#define FLASH_READ_STATUS_2     0x35
#define FLASH_READ_STATUS_3     0x15
#define FLASH_WRITE_STATUS_1    0x01
#define FLASH_WRITE_STATUS_2    0x31
#define FLASH_WRITE_STATUS_3    0x11
#define FLASH_READ_UNIQUE_ID    0x4B
#define COM_FLASH_READ_PAGE         0x0C

#define OPERATION_COUNTER_BEGIN_ADRESS      0x1000
#define OPERATION_COUNTER_END_ADRESS        0x1F00


#define FAT_BEGIN_ADRESS 0x2000 //адрес  начала файловой таблицы
#define FAT_END_ADRESS   0x5F00//адрес конца файловой таблицы


#define TM_BEGIN_ADRESS 0x6000//адрес начала данных тм
#define TM_END_ADRESS   0x3FFFF00//адрес конца флэшки
#define SIZE_TM         0x3FF9FFF//размерадресации телеметрии
#define MAX_OFFSET      0x666600//величина максимального смещения

#define FLASH_ID_BYTE1   0x19
#define FLASH_ID_BYTE2   0x40
#define FLASH_ID_BYTE3   0x20

#define FLASH_IDLE_STATE        0
#define CHECK_STATUS_REG1       1
#define INIT_MASTER_STATE       2

#define WRITE_CONFIG            3

#define TM_START                4
#define CLOSE_TM                5
#define TM_WRITE                6

#define UPLOAD_FAT              7
#define UPLOAD_TM               8


#define ERSE_SECTOR             9
#define ERSE_CHIP               10//130 second

#define WRITE_COUNT_OPER        11


#define CONFIG_ADRESS           0x00000000
/*
typedef struct Fat_Fale_Info_Frame
{
    uint16_t file_name;
    uint32_t begin_adress;
    uint32_t end_adress;

}ffif[10];
*/


//extern struct CAN can;

struct flash_t
{
    uint8_t     ID;
    uint8_t     UniqID[8];

    uint8_t     StatusRegister1;
    uint8_t     StatusRegister2;
    uint8_t     StatusRegister3;

    uint8_t flag_update_state;
    uint8_t state;
    uint8_t nano_state;//стэйт внутри стэйта

    uint8_t init_status;//   7:ID_ERR|6:CONFIG_empty |5:CONFIG_INCORRECT|4:FAT_FAIL|3:TM degin|2:FAT OK|1:Config OK|0:ID ok

    uint8_t rx_tx_flag;

    uint8_t tm_status;//0-неюзанно 1-открыта 2-закрыта

    uint8_t config_buff[260];
    uint16_t config_buff_lenth;
    uint8_t config_update_flag;

    uint8_t engine_name[4];

    uint32_t oper_address;

    uint8_t flag_close_open;//выставляется внутри при достижении максимального смещения

    uint32_t fat_count;

    uint16_t file_name;
    uint16_t serch_file_name;
    uint16_t uploading_file_name;
    uint32_t file_page_numb;//порядковый номер в файле

    uint8_t actul_page_numb;//в таблице фат

    uint8_t uploading_actul_page_numb;

    uint8_t actual_numb_note;//текущий номер записи на странице

    uint32_t file_address;
    uint32_t file_end_address;

    uint32_t uploading_file_address_begin;
    uint32_t uploading_file_address_end;

    uint32_t file_offset;
    uint32_t uploading_file_offset;
    uint8_t clean_fat_flag;

    uint32_t erse_sektor_address;
    uint8_t need_erse_flag;

    uint32_t tm_page_number;

    uint8_t tm_on_flag;
    uint8_t tm_buff_update_flag;
    uint8_t tm_buff[300];
    uint8_t tm_buff_count;
    uint8_t tm_quantity;
    uint8_t tm_slot_numb;

    uint8_t send_fat_flag;
    uint8_t send_tm_flag;

    uint8_t xor;
    uint16_t count_fast;
    uint32_t fast_var;

    uint8_t rx_buff[300];
    uint8_t tx_buff[300];

    uint32_t operation_count; //счётчик моточасов цмр 10сек
    uint16_t operation_count_min;//счётчик в минутах
    uint32_t address_operation_count;//адрес счётчика моточасов
    uint16_t byte_numb_oper_count;
    uint8_t bit_numb_oper_count;
    uint8_t flag_limit_oper;
    uint8_t flag_reset_next_bit_oper_count;//записать бит
    uint8_t flag_oper_count_on;

    uint32_t operation_count_tim;

    uint8_t oper_count_buff[9];//буффер для реквеста моточасов

    uint16_t timer;
    uint8_t fat_upload_flag;
    uint8_t tm_upload_flag;
} flash;


void State_machine_flash();
void TM_updater(uint16_t time);

#endif
