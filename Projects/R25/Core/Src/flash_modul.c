#include "flash_modul.h"
#include "main.h"

struct flash_t flash;

void State_machine_flash()
{
    switch(flash.state)
    {
///////////////////////////////////////////////////////////
    case (FLASH_IDLE_STATE):
    {

        if(flash.flag_update_state)
        {
            flash.flag_update_state = 0;
        }

        if(flash.init_status == 0)
        {

            flash.flag_update_state = 1;
            flash.state = INIT_MASTER_STATE;
        }

        else if(flash.need_erse_flag) //стереть сектор
        {
            flash.need_erse_flag = 0;
            flash.flag_update_state = 1;
            flash.state = ERSE_SECTOR;
        } else if(flash.send_fat_flag)
        {

            flash.flag_update_state = 1;
            flash.state = UPLOAD_FAT;
        }

        else if(flash.send_tm_flag)
        {
            flash.flag_update_state = 1;
            flash.state = UPLOAD_TM;
        } else if(flash.config_update_flag)
        {
            flash.config_update_flag = 0;
            flash.flag_update_state = 1;
            flash.state = WRITE_CONFIG;

        }

        else if(flash.flag_reset_next_bit_oper_count)
        {
            flash.flag_reset_next_bit_oper_count = 0;
            flash.flag_update_state = 1;
            flash.state = WRITE_COUNT_OPER;
        } else
        {
            if(flash.tm_on_flag)
            {
                if(flash.flag_close_open)
                {
                    flash.flag_close_open = 0;
                    flash.flag_update_state = 1;
                    flash.state = CLOSE_TM;
                } else
                {
                    if(flash.init_status & (1 << 3))
                    {
                        if(flash.tm_buff_update_flag)
                        {
                            //flash.tm_buff_update_flag=0;
                            flash.flag_update_state = 1;
                            flash.state = TM_WRITE;
                        }
                    }

                    else
                    {
                        flash.flag_update_state = 1;
                        flash.state = TM_START;
                    }
                }
            } else if(flash.init_status & (1 << 3))
            {
                flash.flag_update_state = 1;
                flash.state = CLOSE_TM;
            }
        }

    }
        break;

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case (CHECK_STATUS_REG1):
    {
        if(flash.flag_update_state)
        {
            flash.flag_update_state = 0;
            flash.nano_state = 0;
        }

        switch(flash.nano_state)
        {
        case (0):
        {
            flash.rx_tx_flag = 0;
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 1);
            flash.nano_state++;
        }
            break;
        case (1):
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];
                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    flash.flag_update_state = 1;
                    flash.state = FLASH_IDLE_STATE;
                }
            }
        }
            break;
        }
    }
        break;

        //////////////////////////////////////////////////////////////////////////
    case (INIT_MASTER_STATE):
    {
        if(flash.flag_update_state)
        {
            flash.flag_update_state = 0;
            flash.nano_state = 0;
        }

        switch(flash.nano_state)
        {
        case (0): //анализ статуса
        {
            flash.rx_tx_flag = 0;
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;
        }
            break;

        case (1): //чтение айди
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    flash.nano_state++;

                    flash.rx_tx_flag = 0;
                    FLASH_CS_SELECT;

                    flash.tx_buff[0] = FLASH_GET_JEDEC_ID;

                    HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 4);
                }
            }
        }
            break;

        case (2): //анализ айди
        {
            if(flash.rx_tx_flag)
            {
                if((flash.rx_buff[2] == FLASH_ID_BYTE2) && (flash.rx_buff[3] == FLASH_ID_BYTE3))
                {

                    flash.init_status = flash.init_status & (~(1 << 7));
                    flash.init_status = flash.init_status | (1 << 0);
                }
                else
                {
                    flash.init_status = flash.init_status & (~(1 << 0));
                    flash.init_status = flash.init_status | (1 << 7);
                }

                flash.nano_state++;
            }

        }
            break;

        case (3): //анализ состояния
        {
            flash.rx_tx_flag = 0;
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;
        }
            break;

        case (4): //запрос конфига
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    flash.nano_state++;
                    flash.rx_tx_flag = 0;
                    flash.oper_address = CONFIG_ADRESS;

                    FLASH_CS_SELECT;

                    flash.tx_buff[0] = COM_FLASH_READ_PAGE;

                    flash.tx_buff[1] = ((flash.oper_address & 0xFF000000) >> 24);
                    flash.tx_buff[2] = ((flash.oper_address & 0xFF0000) >> 16);
                    flash.tx_buff[3] = ((flash.oper_address & 0xFF00) >> 8);
                    flash.tx_buff[4] = ((flash.oper_address & 0xFF));

                    HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, (uint8_t*)&get_config, 262);
                }
            }
        }
            break;

        case (5): //анализ конфига
        {
            if(flash.rx_tx_flag)
            {
                *p_config = get_config.config;

                if(RS_SU_RePack((uint8_t*)&config, 256, 0))
                {
                    flash.engine_name[0] = config.first_simbol_name;
                    flash.engine_name[1] = config.second_simbol_name;
                    flash.engine_name[2] = config.number_name >> 8;
                    flash.engine_name[3] = config.number_name;

                    flash.init_status = flash.init_status & (~(1 << 6));
                    flash.init_status = flash.init_status & (~(1 << 5));
                    flash.init_status = flash.init_status | (1 << 1);

                    rs.config_complate_flag = 1;

                    mtm.tg_correct_k = config.tg_correct_k;
                    mtm.engine_state = 0;
                    mtm.update_state_flag = 1;
                } else //нет конфига
                {
                    flash.init_status = flash.init_status & (~(1 << 1));
                    flash.init_status = flash.init_status | (1 << 6);
                }
                flash.nano_state++;
                flash.fat_count = 0;
                flash.clean_fat_flag = 1;
                flash.file_name = 0;
                flash.count_fast = 0;
                flash.fast_var = 0;

                flash.operation_count = 0;
                flash.address_operation_count = 0;
                flash.byte_numb_oper_count = 0;
                flash.bit_numb_oper_count = 0;
            }
        }
            break;
        case (6): //запрос состояния
        {
            flash.rx_tx_flag = 0;
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;
        }
            break;

        case (7): //запрос очередной записи cчетчика
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    flash.nano_state++;
                    flash.rx_tx_flag = 0;
                    flash.oper_address = OPERATION_COUNTER_BEGIN_ADRESS + (256 * flash.fat_count);

                    FLASH_CS_SELECT;

                    flash.tx_buff[0] = COM_FLASH_READ_PAGE;

                    flash.tx_buff[1] = ((flash.oper_address & 0xFF000000) >> 24);
                    flash.tx_buff[2] = ((flash.oper_address & 0xFF0000) >> 16);
                    flash.tx_buff[3] = ((flash.oper_address & 0xFF00) >> 8);
                    flash.tx_buff[4] = ((flash.oper_address & 0xFF));

                    HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 262);

                }

            }

        }
            break;

        case (8): //анализ очередной записи счетчика
        {
            if(flash.rx_tx_flag)
            {
                flash.count_fast = 6;
                while(flash.count_fast < 262)
                {
                    if(flash.rx_buff[flash.count_fast] == 0xFF)
                    {
                        flash.count_fast = 263;
                    } else
                    {
                        flash.address_operation_count = flash.fat_count;
                        flash.byte_numb_oper_count = (flash.count_fast - 6);
                        flash.fast_var = 0;

                        while(flash.fast_var < 8)
                        {
                            if((flash.rx_buff[flash.count_fast] & (1 << flash.fast_var)) == 0)
                            {
                                flash.bit_numb_oper_count = flash.fast_var;
                                flash.fast_var++;
                                flash.operation_count++;
                            }

                            else
                            {
                                flash.fast_var = 8;
                            }
                        }

                        flash.count_fast++;

                    }
                }

                flash.fat_count++;

                if(flash.fat_count < 16)
                {
                    flash.nano_state = flash.nano_state - 2;
                }

                else
                {
                    if(flash.operation_count != 0)
                    {
                        flash.bit_numb_oper_count++;

                        if(flash.bit_numb_oper_count > 7)
                        {
                            flash.bit_numb_oper_count = 0;

                            flash.byte_numb_oper_count++;

                            if(flash.byte_numb_oper_count > 255)
                            {
                                flash.byte_numb_oper_count = 0;

                                flash.address_operation_count++;

                                if((OPERATION_COUNTER_BEGIN_ADRESS + (256 * flash.address_operation_count)) > OPERATION_COUNTER_END_ADRESS)
                                {
                                    flash.address_operation_count = 0;
                                    flash.flag_limit_oper = 1;
                                }

                            }
                        }
                    }

                    flash.nano_state++;
                    flash.fat_count = 0;
                    flash.fast_var = 0;
                    flash.clean_fat_flag = 1;
                    flash.file_name = 0;

                }

            }
        }
            break;
        case (9): //запрос состояния
        {
            flash.rx_tx_flag = 0;
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;
        }
            break;

        case (10): //запрос очередной записи фат
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    flash.nano_state++;
                    flash.rx_tx_flag = 0;
                    flash.oper_address = FAT_BEGIN_ADRESS + (256 * flash.fat_count);

                    FLASH_CS_SELECT;

                    flash.tx_buff[0] = COM_FLASH_READ_PAGE;

                    flash.tx_buff[1] = ((flash.oper_address & 0xFF000000) >> 24);
                    flash.tx_buff[2] = ((flash.oper_address & 0xFF0000) >> 16);
                    flash.tx_buff[3] = ((flash.oper_address & 0xFF00) >> 8);
                    flash.tx_buff[4] = ((flash.oper_address & 0xFF));

                    HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 262);

                }

            }

        }
            break;

        case (11): //анализ очередной записи фат
        {
            if(flash.rx_tx_flag)
            {
                if((flash.rx_buff[260] != 0xFF))
                {
                    flash.count_fast = 0;
                    while(flash.count_fast < 25)
                    {
                        flash.fast_var = ((flash.rx_buff[(6 + (10 * flash.count_fast))]) << 8) + flash.rx_buff[(7 + (10 * flash.count_fast))];

                        if((flash.fast_var != 0xFFFF) && (((flash.fast_var) == flash.file_name + 1)))
                        {
                            flash.file_name++;
                            flash.actul_page_numb = flash.fat_count;
                            flash.actual_numb_note = flash.count_fast;
                            flash.clean_fat_flag = 0;

                            flash.file_address = (((flash.rx_buff[(8 + (10 * flash.count_fast))]) << 24)
                                    + ((flash.rx_buff[(9 + (10 * flash.count_fast))]) << 16) + ((flash.rx_buff[(10 + (10 * flash.count_fast))]) << 8)
                                    + (flash.rx_buff[(11 + (10 * flash.count_fast))]));

                            flash.file_end_address = (((flash.rx_buff[(12 + (10 * flash.count_fast))]) << 24)
                                    + ((flash.rx_buff[(13 + (10 * flash.count_fast))]) << 16) + ((flash.rx_buff[(14 + (10 * flash.count_fast))]) << 8)
                                    + (flash.rx_buff[(15 + (10 * flash.count_fast))]));
                        }
                        flash.count_fast++;
                    }

                }

                flash.fat_count++;
                if(flash.fat_count < 64)
                {
                    flash.nano_state = flash.nano_state - 2;
                }

                else
                {
                    flash.nano_state++;
                    flash.fat_count = 0;
                    flash.fast_var = 0;

                }
            }

        }
            break;

        case (12): //запрос состояния
        {
            if((flash.clean_fat_flag == 0) && (flash.file_end_address == 0xFFFFFFFF))
            {
                FLASH_CS_SELECT;
                flash.rx_tx_flag = 0;

                flash.tx_buff[0] = FLASH_READ_STATUS_1;
                flash.tx_buff[1] = DUMMY_BYTE;

                HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            }
            flash.nano_state++;
        }
            break;

        case (13): //запрос очередной записи логов
        {
            if((flash.clean_fat_flag == 0) && (flash.file_end_address == 0xFFFFFFFF))
            {
                if(flash.rx_tx_flag)
                {
                    flash.StatusRegister1 = flash.rx_buff[1];

                    if(flash.StatusRegister1 & 1)
                    {
                        flash.nano_state--;
                    } else
                    {
                        flash.nano_state++;
                        flash.rx_tx_flag = 0;
                        flash.oper_address = flash.file_address + (256 * flash.fat_count);
                        //flash.fat_count++;

                        if(flash.oper_address > TM_END_ADRESS)
                        {
                            flash.oper_address = flash.oper_address - SIZE_TM;
                        }

                        FLASH_CS_SELECT;

                        flash.tx_buff[0] = COM_FLASH_READ_PAGE;

                        flash.tx_buff[1] = ((flash.oper_address & 0xFF000000) >> 24);
                        flash.tx_buff[2] = ((flash.oper_address & 0xFF0000) >> 16);
                        flash.tx_buff[3] = ((flash.oper_address & 0xFF00) >> 8);
                        flash.tx_buff[4] = ((flash.oper_address & 0xFF));

                        HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 262);

                    }

                }
            } else
            {
                flash.nano_state++;
            }
        }
            break;

        case (14): //анализ очередной записи логов
        {
            if((flash.clean_fat_flag == 0) && (flash.file_end_address == 0xFFFFFFFF))
            {
                if(flash.rx_tx_flag)
                {
                    flash.file_page_numb = (flash.rx_buff[257] << 24) + (flash.rx_buff[258] << 16) + (flash.rx_buff[259] << 8) + (flash.rx_buff[260]);

                    if(flash.file_page_numb == flash.fat_count)
                    {
                        flash.nano_state = flash.nano_state - 2;

                        flash.fat_count++;

                        flash.fast_var = flash.oper_address;

                    }

                    else
                    {

                        if(flash.fat_count == 0)
                        {
                            flash.fast_var = (flash.file_address);
                        }
                        flash.nano_state++;
                    }

                }
            } else
            {
                flash.nano_state++;
            }
        }
            break;

        case (15): //запрос состояния
        {
            if((flash.clean_fat_flag == 0) && (flash.file_end_address == 0xFFFFFFFF))
            {
                flash.rx_tx_flag = 0;

                FLASH_CS_SELECT;

                flash.tx_buff[0] = FLASH_READ_STATUS_1;
                flash.tx_buff[1] = DUMMY_BYTE;

                HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);

            }
            flash.nano_state++;
        }
            break;

        case (16): //включаем запись
        {

            if((flash.clean_fat_flag == 0) && (flash.file_end_address == 0xFFFFFFFF))
            {
                if(flash.rx_tx_flag)
                {
                    flash.StatusRegister1 = flash.rx_buff[1];

                    if(flash.StatusRegister1 & 1)
                    {
                        flash.nano_state--;
                    } else
                    {
                        if(flash.StatusRegister1 & 1 << 1)
                        {
                            flash.nano_state++;
                            flash.rx_tx_flag = 0;
                        }

                        else
                        {
                            FLASH_CS_SELECT;

                            flash.tx_buff[0] = FLASH_WRITE_ENABLE;

                            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 1);
                            flash.nano_state--;
                        }

                    }

                }
            } else
            {
                flash.nano_state++;
            }

        }
            break;

        case (17): //записываем смещение в соответствующую запись фат
        {

            if((flash.clean_fat_flag == 0) && (flash.file_end_address == 0xFFFFFFFF))
            {
                flash.file_end_address = flash.fast_var;
                for(uint16_t i = 0; i < 256; i++)
                {
                    flash.tx_buff[i + 5] = 0xFF;
                }

                FLASH_CS_SELECT;
                flash.tx_buff[0] = FLASH_PAGE_PROGRAMM;

                flash.oper_address = (FAT_BEGIN_ADRESS + (256 * flash.actul_page_numb));

                flash.tx_buff[1] = ((flash.oper_address & 0xFF000000) >> 24);
                flash.tx_buff[2] = ((flash.oper_address & 0xFF0000) >> 16);
                flash.tx_buff[3] = ((flash.oper_address & 0xFF00) >> 8);
                flash.tx_buff[4] = ((flash.oper_address & 0xFF));

                flash.tx_buff[(11 + (10 * flash.actual_numb_note))] = (flash.file_end_address >> 24);
                flash.tx_buff[(12 + (10 * flash.actual_numb_note))] = (flash.file_end_address >> 16);
                flash.tx_buff[(13 + (10 * flash.actual_numb_note))] = (flash.file_end_address >> 8);
                flash.tx_buff[(14 + (10 * flash.actual_numb_note))] = (flash.file_end_address);
                flash.rx_tx_flag = 0;
                HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 261);

            }

            if(flash.clean_fat_flag)
            {
                flash.actual_numb_note = 0;
                flash.actul_page_numb = 0;
                flash.file_address = TM_BEGIN_ADRESS;
                flash.file_offset = 0;
                flash.file_name = 1;
            }

            else
            {
                flash.actual_numb_note++;

                if(flash.actual_numb_note > 24)
                {
                    flash.actual_numb_note = 0;

                    flash.actul_page_numb++;

                    if(flash.actul_page_numb > 63)
                    {
                        flash.actul_page_numb = 0;
                    }

                    if(flash.actul_page_numb == 63)
                    {
                        flash.erse_sektor_address = FAT_BEGIN_ADRESS;
                        flash.need_erse_flag = 1;
                    } else if(flash.actul_page_numb == 15)
                    {
                        flash.erse_sektor_address = FAT_BEGIN_ADRESS + 0x1000;
                        flash.need_erse_flag = 1;
                    }

                    else if(flash.actul_page_numb == 31)
                    {
                        flash.erse_sektor_address = FAT_BEGIN_ADRESS + 0x2000;
                        flash.need_erse_flag = 1;
                    }

                    else if(flash.actul_page_numb == 47)
                    {
                        flash.erse_sektor_address = FAT_BEGIN_ADRESS + 0x3000;
                        flash.need_erse_flag = 1;
                    }
                }

                flash.file_name++;
                if(flash.file_name == 0xFFFF)
                {
                    flash.file_name = 1; //
                }

                flash.file_address = flash.file_end_address + 256;

                if(flash.file_address > TM_END_ADRESS)
                {
                    flash.file_address = TM_BEGIN_ADRESS;
                }
                flash.tm_slot_numb = 0;
            }

            flash.nano_state++;
        }
            break;

        case (18):
        {
            if(flash.rx_tx_flag)
            {
                flash.state = CHECK_STATUS_REG1;
                flash.flag_update_state = 1;

                flash.init_status = flash.init_status & (~(1 << 4));
                flash.init_status = flash.init_status | (1 << 2);
            }
        }
            break;
        }
    }
        break;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    case (ERSE_SECTOR):
    {
        if(flash.flag_update_state)
        {
            flash.flag_update_state = 0;
            flash.nano_state = 0;
        }

        switch(flash.nano_state)
        {
        case (0): //анализ статуса
        {
            FLASH_CS_SELECT;
            flash.rx_tx_flag = 0;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;
        }
            break;

        case (1): //включаем запись
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    if(flash.StatusRegister1 & (1 << 1))
                    {
                        flash.nano_state++;
                        flash.rx_tx_flag = 0;
                    }

                    else
                    {
                        FLASH_CS_SELECT;

                        flash.tx_buff[0] = FLASH_WRITE_ENABLE;

                        HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 1);
                        flash.nano_state--;
                    }

                }

            }

        }
            break;

        case (2): //трем сектор по прерыванию возврат в идл
        {

            FLASH_CS_SELECT;
            flash.tx_buff[0] = FLASH_SECTOR_ERASE;

            flash.tx_buff[1] = ((flash.erse_sektor_address & 0xFF000000) >> 24);
            flash.tx_buff[2] = ((flash.erse_sektor_address & 0xFF0000) >> 16);
            flash.tx_buff[3] = ((flash.erse_sektor_address & 0xFF00) >> 8);
            flash.tx_buff[4] = ((flash.erse_sektor_address & 0xFF));

            flash.rx_tx_flag = 0;
            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 5);
            flash.nano_state++;

        }
            break;

        case (3):
        {
            if(flash.rx_tx_flag)
            {
                flash.state = CHECK_STATUS_REG1;
                flash.flag_update_state = 1;

            }
        }
            break;
        }

    }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    case (WRITE_CONFIG):

    {

        if(flash.flag_update_state)
        {
            flash.flag_update_state = 0;
            flash.nano_state = 0;

        }

        switch(flash.nano_state)
        {
        case (0): //анализ статуса
        {
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            flash.rx_tx_flag = 0;
            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;

        }
            break;

        case (1): //включаем запись
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    if(flash.StatusRegister1 & 1 << 1)
                    {
                        flash.nano_state++;
                        flash.rx_tx_flag = 0;
                    }

                    else
                    {

                        FLASH_CS_SELECT;

                        flash.tx_buff[0] = FLASH_WRITE_ENABLE;

                        HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 1);
                        flash.nano_state--;
                    }

                }

            }

        }
            break;

        case (2): //пишем конфиг, по выполнению сбрасываем инит флаг для инициализации
        {

            flash.oper_address = CONFIG_ADRESS;
            //for(uint16_t i=0;i<flash.config_buff_lenth;i++)
            {
                //	flash.tx_buff[i+5]=flash.config_buff[i];
            }

            //flash.tx_buff[8]=flash.config_buff_lenth;

            set_config.config = *p_config;

            FLASH_CS_SELECT;

            //flash.tx_buff[0]=FLASH_PAGE_PROGRAMM;

            //flash.tx_buff[1]=((flash.oper_address & 0xFF000000) >> 24);
            //flash.tx_buff[2]=((flash.oper_address & 0xFF0000) >> 16);
            //flash.tx_buff[3]=((flash.oper_address & 0xFF00) >> 8);
            //flash.tx_buff[4]=((flash.oper_address & 0xFF));
            set_config.dum1 = FLASH_PAGE_PROGRAMM;
            set_config.dum2 = ((flash.oper_address & 0xFF000000) >> 24);
            set_config.dum3 = ((flash.oper_address & 0xFF0000) >> 16);
            set_config.dum4 = ((flash.oper_address & 0xFF00) >> 8);
            set_config.dum5 = ((flash.oper_address & 0xFF));

            flash.rx_tx_flag = 0;
            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, (uint8_t*)&set_config, flash.rx_buff, 261);

            flash.nano_state++;

        }
            break;

        case (3):
        {
            if(flash.rx_tx_flag)
            {
                flash.state = CHECK_STATUS_REG1;
                flash.flag_update_state = 1;
                flash.init_status = 0;
            }
        }
            break;
        }

    }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    case (TM_START):

    {

        if(flash.flag_update_state)
        {
            flash.flag_update_state = 0;
            flash.nano_state = 0;

        }

        switch(flash.nano_state)
        {
        case (0):					//анализ статуса
        {
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            flash.rx_tx_flag = 0;
            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;

        }
            break;

        case (1):					//включаем запись
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    if(flash.StatusRegister1 & 1 << 1)
                    {
                        flash.nano_state++;
                        flash.rx_tx_flag = 0;
                    }

                    else
                    {

                        FLASH_CS_SELECT;

                        flash.tx_buff[0] = FLASH_WRITE_ENABLE;

                        HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 1);
                        flash.nano_state--;
                    }

                }

            }

        }
            break;

        case (2):					//пишем запись в таблицу фат, по выполнению сбрасываем инит флаг для инициализации
        {

            for(uint16_t i = 0; i < 256; i++)
            {
                flash.tx_buff[i + 5] = 0xFF;
            }
            FLASH_CS_SELECT;
            flash.tx_buff[0] = FLASH_PAGE_PROGRAMM;

            flash.oper_address = FAT_BEGIN_ADRESS + (256 * flash.actul_page_numb);

            flash.tx_buff[(5 + (10 * flash.actual_numb_note))] = (flash.file_name >> 8);
            flash.tx_buff[(6 + (10 * flash.actual_numb_note))] = (flash.file_name);

            flash.tx_buff[(7 + (10 * flash.actual_numb_note))] = (flash.file_address >> 24);
            flash.tx_buff[(8 + (10 * flash.actual_numb_note))] = (flash.file_address >> 16);
            flash.tx_buff[(9 + (10 * flash.actual_numb_note))] = (flash.file_address >> 8);
            flash.tx_buff[(10 + (10 * flash.actual_numb_note))] = (flash.file_address);

            flash.tx_buff[1] = ((flash.oper_address & 0xFF000000) >> 24);
            flash.tx_buff[2] = ((flash.oper_address & 0xFF0000) >> 16);
            flash.tx_buff[3] = ((flash.oper_address & 0xFF00) >> 8);
            flash.tx_buff[4] = ((flash.oper_address & 0xFF));

            flash.tx_buff[259] = flash.actul_page_numb;

            flash.rx_tx_flag = 0;

            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 261);

            flash.tm_slot_numb = 0;

            flash.tm_page_number = 0;

            flash.init_status = flash.init_status | (1 << 3);
            flash.nano_state++;

        }
            break;

        case (3):
        {
            if(flash.rx_tx_flag)
            {
                flash.state = CHECK_STATUS_REG1;
                flash.flag_update_state = 1;
                flash.file_offset = 0;
            }
        }
            break;
        }
    }
        break;

        /////////////////////////////////////////////////////////////////////////////////////////

    case (CLOSE_TM):

    {
        if(flash.flag_update_state)
        {
            flash.flag_update_state = 0;
            flash.nano_state = 0;
        }

        switch(flash.nano_state)
        {
        case (0):					//анализ статуса
        {
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            flash.rx_tx_flag = 0;
            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;
        }
            break;

        case (1):					//включаем запись
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    if(flash.StatusRegister1 & 1 << 1)
                    {
                        flash.nano_state++;
                        flash.rx_tx_flag = 0;
                    }

                    else
                    {
                        FLASH_CS_SELECT;

                        flash.tx_buff[0] = FLASH_WRITE_ENABLE;

                        HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 1);
                        flash.nano_state--;
                    }
                }
            }
        }
            break;

        case (2):					//пишем cмещение в таблицу
        {
            for(uint16_t i = 0; i < 256; i++)
            {
                flash.tx_buff[i + 5] = 0xFF;
            }

            FLASH_CS_SELECT;
            flash.tx_buff[0] = FLASH_PAGE_PROGRAMM;

            flash.oper_address = (FAT_BEGIN_ADRESS + (256 * flash.actul_page_numb));

            flash.tx_buff[1] = ((flash.oper_address & 0xFF000000) >> 24);
            flash.tx_buff[2] = ((flash.oper_address & 0xFF0000) >> 16);
            flash.tx_buff[3] = ((flash.oper_address & 0xFF00) >> 8);
            flash.tx_buff[4] = ((flash.oper_address & 0xFF));

            flash.tx_buff[(11 + (10 * flash.actual_numb_note))] = (flash.file_end_address >> 24);
            flash.tx_buff[(12 + (10 * flash.actual_numb_note))] = (flash.file_end_address >> 16);
            flash.tx_buff[(13 + (10 * flash.actual_numb_note))] = (flash.file_end_address >> 8);
            flash.tx_buff[(14 + (10 * flash.actual_numb_note))] = (flash.file_end_address);
            flash.rx_tx_flag = 0;

            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 261);

            flash.actual_numb_note++;

            if(flash.actual_numb_note > 24)
            {
                flash.actual_numb_note = 0;

                flash.actul_page_numb++;

                if(flash.actul_page_numb > 63)
                {
                    flash.actul_page_numb = 0;
                }

                if(flash.actul_page_numb == 63)
                {
                    flash.erse_sektor_address = FAT_BEGIN_ADRESS;
                    flash.need_erse_flag = 1;
                } else if(flash.actul_page_numb == 15)
                {
                    flash.erse_sektor_address = FAT_BEGIN_ADRESS + 0x1000;
                    flash.need_erse_flag = 1;
                }

                else if(flash.actul_page_numb == 31)
                {
                    flash.erse_sektor_address = FAT_BEGIN_ADRESS + 0x2000;
                    flash.need_erse_flag = 1;
                }

                else if(flash.actul_page_numb == 47)
                {
                    flash.erse_sektor_address = FAT_BEGIN_ADRESS + 0x3000;
                    flash.need_erse_flag = 1;
                }

            }

            flash.file_name++;
            if(flash.file_name == 0xFFFF)
            {
                flash.file_name = 1;					//
            }

            flash.file_address = flash.file_end_address + 256;

            if(flash.file_address > TM_END_ADRESS)
            {
                flash.file_address = TM_BEGIN_ADRESS;
            }
            flash.tm_slot_numb = 0;

            flash.file_offset = 0;

            flash.nano_state++;
        }
            break;

        case (3):
        {
            if(flash.rx_tx_flag)
            {
                flash.state = CHECK_STATUS_REG1;
                flash.flag_update_state = 1;
                flash.init_status = flash.init_status & (~(1 << 3));
            }
        }
            break;
        }

    }
        break;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case (TM_WRITE):

    {

        if(flash.flag_update_state)
        {
            flash.flag_update_state = 0;
            flash.nano_state = 0;

        }

        switch(flash.nano_state)
        {
        case (0):					//анализ статуса
        {
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            flash.rx_tx_flag = 0;
            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;

        }
            break;

        case (1):					//включаем запись
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    if(flash.StatusRegister1 & 1 << 1)
                    {
                        flash.nano_state++;
                        flash.rx_tx_flag = 0;
                    }

                    else
                    {

                        FLASH_CS_SELECT;

                        flash.tx_buff[0] = FLASH_WRITE_ENABLE;

                        HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 1);
                        flash.nano_state--;
                    }

                }

            }

        }
            break;

        case (2):					//запись следующей страницы телеметрии+ сброс флага +усди конец сектора стераем следующий
        {

            flash.oper_address = flash.file_offset + flash.file_address;

            flash.file_offset = flash.file_offset + 256;

            if((flash.file_offset > MAX_OFFSET) || ((flash.oper_address + 256) > TM_END_ADRESS))
            {
                flash.flag_close_open = 1;
            }

            /*
             if (flash.oper_address>TM_END_ADRESS)
             {
             flash.oper_address=flash.oper_address-SIZE_TM;
             }
             */
            flash.file_end_address = flash.oper_address;

            if(((flash.file_end_address + 256) % 0x1000) == 0)
            {

                flash.erse_sektor_address = flash.file_end_address + 0x100;
                if(flash.erse_sektor_address > TM_END_ADRESS)
                {
                    flash.erse_sektor_address = TM_BEGIN_ADRESS;
                }

                flash.need_erse_flag = 1;
            }

            //flash.file_address=flash.file_address+(0x100*flash.file_offset);
            flash.tm_page_number++;

            for(uint16_t i = 0; i < 256; i++)
            {
                flash.tx_buff[i + 5] = flash.tm_buff[i];					//flash.tm_buff[i];
            }

            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_PAGE_PROGRAMM;

            flash.tx_buff[1] = ((flash.oper_address & 0xFF000000) >> 24);
            flash.tx_buff[2] = ((flash.oper_address & 0xFF0000) >> 16);
            flash.tx_buff[3] = ((flash.oper_address & 0xFF00) >> 8);
            flash.tx_buff[4] = ((flash.oper_address & 0xFF));

            flash.rx_tx_flag = 0;
            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 261);
            flash.nano_state++;
            flash.tm_buff_update_flag = 0;

        }
            break;

        case (3):
        {
            if(flash.rx_tx_flag)
            {
                flash.state = CHECK_STATUS_REG1;
                flash.flag_update_state = 1;

            }
        }
            break;
        }

    }
        break;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    case (ERSE_CHIP):

    {
        if(flash.flag_update_state)
        {
            flash.flag_update_state = 0;
            flash.nano_state = 0;

        }

        switch(flash.nano_state)
        {
        case (0):					//анализ статуса
        {
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            flash.rx_tx_flag = 0;
            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;

        }
            break;

        case (1):					//включаем запись
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    if(flash.StatusRegister1 & 1 << 1)
                    {
                        flash.nano_state++;
                        flash.rx_tx_flag = 0;
                    }

                    else
                    {
                        FLASH_CS_SELECT;

                        flash.tx_buff[0] = FLASH_WRITE_ENABLE;

                        HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 1);
                        flash.nano_state--;
                    }
                }
            }
        }
            break;
        case (2):					//стереть сектор
        {
            flash.rx_tx_flag = 0;
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_CHIP_ERASE;

            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 1);

            flash.nano_state++;
        }
            break;

        case (3):
        {
            if(flash.rx_tx_flag)
            {
                flash.flag_update_state = 1;
                flash.state = CHECK_STATUS_REG1;
                flash.init_status = 0;
            }
        }
            break;
        }
    }
        break;
//////////////////////////////////////////////////////////////////////////////
    case (UPLOAD_FAT):

    {
        if(flash.flag_update_state)
        {
            flash.flag_update_state = 0;

            flash.fat_count = 0;

            if(flash.init_status & (1 << 3))
            {
                flash.nano_state = 0;
            }

            else
            {
                flash.nano_state = 4;
            }
        }

        switch(flash.nano_state)
        {
        case (0):					//анализ статуса
        {
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            flash.rx_tx_flag = 0;
            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;
        }
            break;

        case (1):					//
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    if(flash.StatusRegister1 & 1 << 1)
                    {
                        flash.nano_state++;
                        flash.rx_tx_flag = 0;
                    }

                    else
                    {

                        FLASH_CS_SELECT;

                        flash.tx_buff[0] = FLASH_WRITE_ENABLE;

                        HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 1);
                        flash.nano_state--;
                    }
                }
            }
        }
            break;

        case (2):					//
        {
            for(uint16_t i = 0; i < 256; i++)
            {
                flash.tx_buff[i + 5] = 0xFF;
            }

            FLASH_CS_SELECT;
            flash.tx_buff[0] = FLASH_PAGE_PROGRAMM;

            flash.oper_address = (FAT_BEGIN_ADRESS + (256 * flash.actul_page_numb));

            flash.tx_buff[1] = ((flash.oper_address & 0xFF000000) >> 24);
            flash.tx_buff[2] = ((flash.oper_address & 0xFF0000) >> 16);
            flash.tx_buff[3] = ((flash.oper_address & 0xFF00) >> 8);
            flash.tx_buff[4] = ((flash.oper_address & 0xFF));

            flash.tx_buff[(11 + (10 * flash.actual_numb_note))] = (flash.file_end_address >> 24);
            flash.tx_buff[(12 + (10 * flash.actual_numb_note))] = (flash.file_end_address >> 16);
            flash.tx_buff[(13 + (10 * flash.actual_numb_note))] = (flash.file_end_address >> 8);
            flash.tx_buff[(14 + (10 * flash.actual_numb_note))] = (flash.file_end_address);
            flash.rx_tx_flag = 0;

            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 261);

            flash.actual_numb_note++;

            if(flash.actual_numb_note > 24)
            {
                flash.actual_numb_note = 0;

                flash.actul_page_numb++;

                if(flash.actul_page_numb > 63)
                {
                    flash.actul_page_numb = 0;
                }

                if(flash.actul_page_numb == 63)
                {
                    flash.erse_sektor_address = FAT_BEGIN_ADRESS;
                    flash.need_erse_flag = 1;
                } else if(flash.actul_page_numb == 15)
                {
                    flash.erse_sektor_address = FAT_BEGIN_ADRESS + 0x1000;
                    flash.need_erse_flag = 1;
                }

                else if(flash.actul_page_numb == 31)
                {
                    flash.erse_sektor_address = FAT_BEGIN_ADRESS + 0x2000;
                    flash.need_erse_flag = 1;
                }

                else if(flash.actul_page_numb == 47)
                {
                    flash.erse_sektor_address = FAT_BEGIN_ADRESS + 0x3000;
                    flash.need_erse_flag = 1;
                }

            }

            flash.file_name++;
            if(flash.file_name == 0xFFFF)
            {
                flash.file_name = 0;					//
            }

            flash.file_address = flash.file_end_address + 256;

            if(flash.file_address > TM_END_ADRESS)
            {
                flash.file_address = TM_BEGIN_ADRESS;
            }
            flash.tm_slot_numb = 0;

            flash.file_offset = 0;

            flash.nano_state++;
        }
            break;

        case (3):
        {
            if(flash.rx_tx_flag)
            {
                flash.nano_state++;
                flash.init_status = (flash.init_status & (~(1 << 3)));
            }
        }
            break;

            HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)flash.tm_buff, 256);

        case (4):					//запрос состояния
        {
            flash.rx_tx_flag = 0;
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;
        }
            break;

        case (5):					//запрос очередной записи фат
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    flash.nano_state++;
                    flash.rx_tx_flag = 0;
                    flash.oper_address = FAT_BEGIN_ADRESS + (256 * flash.fat_count);
                    flash.fat_count++;
                    /*
                     if(flash.fat_count>31)
                     {
                     flash.fat_count=0;
                     }

                     flash.count_fast++;
                     */
                    FLASH_CS_SELECT;

                    flash.tx_buff[0] = COM_FLASH_READ_PAGE;

                    flash.tx_buff[1] = ((flash.oper_address & 0xFF000000) >> 24);
                    flash.tx_buff[2] = ((flash.oper_address & 0xFF0000) >> 16);
                    flash.tx_buff[3] = ((flash.oper_address & 0xFF00) >> 8);
                    flash.tx_buff[4] = ((flash.oper_address & 0xFF));

                    HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 262);
                }
            }
        }
            break;

        case (6):					//отправка очередной записи фат
        {
            if((flash.rx_tx_flag) && ((flash.fat_upload_flag == 0) || (rs.rs485_tx_flag)))
            {
                flash.fat_upload_flag = 1;
                rs.rs485_tx_flag = 0;
                {
                    flash.xor = 0;

                    for(uint16_t i = 0; i < 256; i++)
                    {
                        flash.tm_buff[i] = flash.rx_buff[i + 6];
                        if(i < 255)
                        {
                            flash.xor = flash.xor ^ flash.tm_buff[i];
                        }
                    }

                    flash.tm_buff[255] = flash.xor;

                    HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)flash.tm_buff, 256);

                    if(flash.fat_count < 64)
                    {
                        flash.nano_state = flash.nano_state - 2;
                    }
                    else
                    {
                        flash.fat_upload_flag = 0;
                        flash.state = CHECK_STATUS_REG1;
                        flash.flag_update_state = 1;
                        flash.send_fat_flag = 0;
                    }
                }
            }
        }
            break;
        }
    }
        break;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    case (UPLOAD_TM):
    {
        if(flash.flag_update_state)
        {
            flash.flag_update_state = 0;
            flash.fat_count = 0;
            if(flash.init_status & (1 << 3))
            {
                flash.nano_state = 0;
            }
            else
            {
                flash.nano_state = 4;
            }
        }
        switch(flash.nano_state)
        {
        case (0):					//анализ статуса
        {
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            flash.rx_tx_flag = 0;
            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;
        }
            break;
        case (1):					//включаем запись
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    if(flash.StatusRegister1 & 1 << 1)
                    {
                        flash.nano_state++;
                        flash.rx_tx_flag = 0;
                    }
                    else
                    {
                        FLASH_CS_SELECT;

                        flash.tx_buff[0] = FLASH_WRITE_ENABLE;

                        HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 1);
                        flash.nano_state--;
                    }
                }
            }
        }
            break;
        case (2):					//пишем cмещение в таблицу
        {
            for(uint16_t i = 0; i < 256; i++)
            {
                flash.tx_buff[i + 5] = 0xFF;
            }

            FLASH_CS_SELECT;
            flash.tx_buff[0] = FLASH_PAGE_PROGRAMM;

            flash.oper_address = (FAT_BEGIN_ADRESS + (256 * flash.actul_page_numb));

            flash.tx_buff[1] = ((flash.oper_address & 0xFF000000) >> 24);
            flash.tx_buff[2] = ((flash.oper_address & 0xFF0000) >> 16);
            flash.tx_buff[3] = ((flash.oper_address & 0xFF00) >> 8);
            flash.tx_buff[4] = ((flash.oper_address & 0xFF));

            flash.tx_buff[(11 + (10 * flash.actual_numb_note))] = (flash.file_end_address >> 24);
            flash.tx_buff[(12 + (10 * flash.actual_numb_note))] = (flash.file_end_address >> 16);
            flash.tx_buff[(13 + (10 * flash.actual_numb_note))] = (flash.file_end_address >> 8);
            flash.tx_buff[(14 + (10 * flash.actual_numb_note))] = (flash.file_end_address);
            flash.rx_tx_flag = 0;

            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 261);

            flash.actual_numb_note++;

            if(flash.actual_numb_note > 24)
            {
                flash.actual_numb_note = 0;

                flash.actul_page_numb++;

                if(flash.actul_page_numb > 63)
                {
                    flash.actul_page_numb = 0;
                }

                if(flash.actul_page_numb == 63)
                {
                    flash.erse_sektor_address = FAT_BEGIN_ADRESS;
                    flash.need_erse_flag = 1;
                } else if(flash.actul_page_numb == 15)
                {
                    flash.erse_sektor_address = FAT_BEGIN_ADRESS + 0x1000;
                    flash.need_erse_flag = 1;
                }

                else if(flash.actul_page_numb == 31)
                {
                    flash.erse_sektor_address = FAT_BEGIN_ADRESS + 0x2000;
                    flash.need_erse_flag = 1;
                }

                else if(flash.actul_page_numb == 47)
                {
                    flash.erse_sektor_address = FAT_BEGIN_ADRESS + 0x3000;
                    flash.need_erse_flag = 1;
                }
            }

            flash.file_name++;
            if(flash.file_name == 0xFFFF)
            {
                flash.file_name = 0;
            }

            flash.file_address = flash.file_end_address + 256;

            if(flash.file_address > TM_END_ADRESS)
            {
                flash.file_address = TM_BEGIN_ADRESS;
            }
            flash.tm_slot_numb = 0;

            flash.file_offset = 0;

            flash.nano_state++;
        }
            break;

        case (3):
        {
            if(flash.rx_tx_flag)
            {
                flash.nano_state++;
                flash.init_status = (flash.init_status & (~(1 << 3)));
            }
        }
            break;

        case (4):					//запрос состояния
        {
            flash.rx_tx_flag = 0;
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;
        }
            break;

        case (5):                   //запрос очередной записи фат
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    flash.nano_state++;
                    flash.rx_tx_flag = 0;
                    flash.oper_address = flash.uploading_file_address_begin + (256 * flash.fat_count);
                    flash.fat_count++;

                    FLASH_CS_SELECT;

                    flash.tx_buff[0] = COM_FLASH_READ_PAGE;

                    flash.tx_buff[1] = ((flash.oper_address & 0xFF000000) >> 24);
                    flash.tx_buff[2] = ((flash.oper_address & 0xFF0000) >> 16);
                    flash.tx_buff[3] = ((flash.oper_address & 0xFF00) >> 8);
                    flash.tx_buff[4] = ((flash.oper_address & 0xFF));

                    HAL_Delay(10);

                    HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 262);
                }
            }
        }
            break;
        case (6):					//отправка очередной записи фат
        {
            if((flash.rx_tx_flag) && ((flash.tm_upload_flag == 0) || (rs.rs485_tx_flag)))
            {
                flash.tm_upload_flag = 1;
                rs.rs485_tx_flag = 0;
                {
                    //rs_tx_flag=0;
                    flash.xor = 0;

                    for(uint16_t i = 0; i < 256; i++)
                    {
                        flash.tm_buff[i] = flash.rx_buff[i + 6];
                    }

                    //rs_tx_flag=0;
                    HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)flash.tm_buff, 256);

                    if((flash.oper_address + 256) < flash.uploading_file_address_end)					//(flash.fat_count<64)
                    {
                        flash.nano_state = flash.nano_state - 2;
                    }

                    else
                    {
                        flash.tm_upload_flag = 0;
                        flash.state = CHECK_STATUS_REG1;
                        flash.flag_update_state = 1;
                        flash.send_tm_flag = 0;
                    }
                }
            }
        }
            break;
        }
    }
        break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**/
    case (WRITE_COUNT_OPER):
    {
        if(flash.flag_update_state)
        {
            flash.flag_update_state = 0;
            flash.nano_state = 0;
        }

        switch(flash.nano_state)
        {
        case (0):					//анализ статуса
        {
            FLASH_CS_SELECT;

            flash.tx_buff[0] = FLASH_READ_STATUS_1;
            flash.tx_buff[1] = DUMMY_BYTE;

            flash.rx_tx_flag = 0;
            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 2);
            flash.nano_state++;
        }
            break;

        case (1):					//включаем запись
        {
            if(flash.rx_tx_flag)
            {
                flash.StatusRegister1 = flash.rx_buff[1];

                if(flash.StatusRegister1 & 1)
                {
                    flash.nano_state--;
                } else
                {
                    if(flash.StatusRegister1 & 1 << 1)
                    {
                        flash.nano_state++;
                        flash.rx_tx_flag = 0;
                    }

                    else
                    {
                        FLASH_CS_SELECT;

                        flash.tx_buff[0] = FLASH_WRITE_ENABLE;

                        HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 1);
                        flash.nano_state--;
                    }
                }
            }
        }
            break;

        case (2):					// ресет след бита счетчика
        {
            for(uint16_t i = 0; i < 256; i++)
            {
                flash.tx_buff[i + 5] = 0xFF;
            }

            flash.tx_buff[flash.byte_numb_oper_count + 5] = 0xFF & ~(1 << flash.bit_numb_oper_count);

            FLASH_CS_SELECT;
            flash.tx_buff[0] = FLASH_PAGE_PROGRAMM;

            flash.oper_address = (OPERATION_COUNTER_BEGIN_ADRESS + (256 * flash.address_operation_count));

            flash.tx_buff[1] = ((flash.oper_address & 0xFF000000) >> 24);
            flash.tx_buff[2] = ((flash.oper_address & 0xFF0000) >> 16);
            flash.tx_buff[3] = ((flash.oper_address & 0xFF00) >> 8);
            flash.tx_buff[4] = ((flash.oper_address & 0xFF));

            flash.rx_tx_flag = 0;

            HAL_SPI_TransmitReceive_DMA(FLASH_SPI_PTR, flash.tx_buff, flash.rx_buff, 261);

            flash.bit_numb_oper_count++;

            if(flash.bit_numb_oper_count > 7)
            {
                flash.bit_numb_oper_count = 0;

                flash.byte_numb_oper_count++;

                if(flash.byte_numb_oper_count > 255)
                {
                    flash.byte_numb_oper_count = 0;

                    flash.address_operation_count++;

                    if((OPERATION_COUNTER_BEGIN_ADRESS + (256 * flash.address_operation_count)) > OPERATION_COUNTER_END_ADRESS)
                    {
                        flash.address_operation_count = 0;
                        flash.flag_limit_oper = 1;
                    }
                }
            }
            flash.operation_count++;
            flash.nano_state++;
        }
            break;

        case (3):
        {
            if(flash.rx_tx_flag)
            {
                flash.state = CHECK_STATUS_REG1;
                flash.flag_update_state = 1;
            }
        }
            break;
        }
    }
        break;
////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
}

void TM_updater(uint16_t time)
{
    if(flash.tm_buff_update_flag == 0)
    {
        //время

        //time=time/100;
        flash.tm_buff[0 + (50 * flash.tm_slot_numb)] = time >> 8;
        flash.tm_buff[1 + (50 * flash.tm_slot_numb)] = time;

        //n1
        flash.tm_buff[2 + (50 * flash.tm_slot_numb)] = ((mtm.rotor_speed / 10) >> 8);
        flash.tm_buff[3 + (50 * flash.tm_slot_numb)] = (((mtm.rotor_speed) / 10) & 0xFF);

        //tg
        flash.tm_buff[4 + (50 * flash.tm_slot_numb)] = ((int16_t)mtm.t_real) >> 8;
        flash.tm_buff[5 + (50 * flash.tm_slot_numb)] = ((int16_t)mtm.t_real & 0xFF);

        //rud
        flash.tm_buff[6 + (50 * flash.tm_slot_numb)] = mtm.trotle >> 8;
        flash.tm_buff[7 + (50 * flash.tm_slot_numb)] = mtm.trotle & 0xFF;

        //state
        flash.tm_buff[8 + (50 * flash.tm_slot_numb)] = mtm.engine_state >> 8;
        flash.tm_buff[9 + (50 * flash.tm_slot_numb)] = mtm.engine_state & 0xFF;

        //flag
        flash.tm_buff[10 + (50 * flash.tm_slot_numb)] = mtm.engine_flag >> 8;
        flash.tm_buff[11 + (50 * flash.tm_slot_numb)] = mtm.engine_flag & 0xFF;

        //starterPWM
        flash.tm_buff[12 + (50 * flash.tm_slot_numb)] = STARTER_PWM >> 8;
        flash.tm_buff[13 + (50 * flash.tm_slot_numb)] = STARTER_PWM & 0xFF;

        //pumpPWM
        flash.tm_buff[14 + (50 * flash.tm_slot_numb)] = PUMP_PWM >> 8;
        flash.tm_buff[15 + (50 * flash.tm_slot_numb)] = PUMP_PWM & 0xFF;

        //PumpFreq
        flash.tm_buff[16 + (50 * flash.tm_slot_numb)] = mtm.pump_speed >> 8;
        flash.tm_buff[17 + (50 * flash.tm_slot_numb)] = mtm.pump_speed & 0xFF;

        //Pump_cur
        flash.tm_buff[18 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.pump_current * 100)) >> 8);
        flash.tm_buff[19 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.pump_current * 100)) & 0xFF);

        //Valve1_cur
        flash.tm_buff[20 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.valve1_cur * 100)) >> 8);
        flash.tm_buff[21 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.valve1_cur * 100)) & 0xFF);

        //Vflve2_cur
        flash.tm_buff[22 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.valve2_cur * 100)) >> 8);
        flash.tm_buff[23 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.valve2_cur * 100)) & 0xFF);

        //Plug_cur
        flash.tm_buff[24 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.plug_current * 100)) >> 8);
        flash.tm_buff[25 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.plug_current * 100)) & 0xFF);

        //bus_volt
        flash.tm_buff[26 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.bus_volt * 10)) >> 8);
        flash.tm_buff[27 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.bus_volt * 10)) & 0xFF);

        //DC_volt
        flash.tm_buff[28 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.gen_volt * 10)) >> 8);
        flash.tm_buff[29 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.gen_volt * 10)) & 0xFF);

        //t_diod
        flash.tm_buff[30 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.t_diod * 10)) >> 8);
        flash.tm_buff[31 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.t_diod * 10)) & 0xFF);

        //t_gen
        flash.tm_buff[32 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.t_gen * 10)) >> 8);
        flash.tm_buff[33 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.t_gen * 10)) & 0xFF);

        //p_206
        flash.tm_buff[34 + (50 * flash.tm_slot_numb)] = 0x45; // TODO: (hc206s.p>>8);
        flash.tm_buff[35 + (50 * flash.tm_slot_numb)] = 0x45; // TODO: (hc206s.p);

        //a_206
        flash.tm_buff[36 + (50 * flash.tm_slot_numb)] = 0x45; // TODO: (uint8_t)(((uint16_t)lps22hb.p*10)>>8);
        flash.tm_buff[37 + (50 * flash.tm_slot_numb)] = 0x45; // TODO: (uint8_t)(lps22hb.p*10);;

        //T_a
        flash.tm_buff[38 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.t_cristal)) >> 8);
        flash.tm_buff[39 + (50 * flash.tm_slot_numb)] = (((uint16_t)(mtm.t_cristal)) & 0xFF);

        //T_b
        //flash.tm_buff[40+(50*flash.tm_slot_numb)]=(((uint16_t)(fst.t_b*10))>>8);
        //flash.tm_buff[41+(50*flash.tm_slot_numb)]=(((uint16_t)(fst.t_b*10))&0xFF);

        //T_int_TC
        //flash.tm_buff[42+(50*flash.tm_slot_numb)]=0;
        //flash.tm_buff[43+(50*flash.tm_slot_numb)]=20;

        //RS_status
        flash.tm_buff[44 + (50 * flash.tm_slot_numb)] = ((uint16_t)mtm.hot_cap_tc) >> 8;
        flash.tm_buff[45 + (50 * flash.tm_slot_numb)] = (uint8_t)mtm.hot_cap_tc;

        //servis1
        flash.tm_buff[46 + (50 * flash.tm_slot_numb)] = mtm.pump2_speed >> 8;
        flash.tm_buff[47 + (50 * flash.tm_slot_numb)] = mtm.pump2_speed;

        //servis2
        flash.tm_buff[48 + (50 * flash.tm_slot_numb)] = flash.operation_count >> 8;
        flash.tm_buff[49 + (50 * flash.tm_slot_numb)] = flash.operation_count;

        flash.tm_slot_numb++;
    }

    if(flash.tm_slot_numb > 4)
    {
        flash.tm_slot_numb = 0;

        flash.tm_buff[251] = flash.tm_page_number >> 24;
        flash.tm_buff[252] = flash.tm_page_number >> 16;
        flash.tm_buff[253] = flash.tm_page_number >> 8;
        flash.tm_buff[254] = flash.tm_page_number;

        flash.xor = 0;
        for(uint16_t i = 0; i < 255; i++)
        {
            flash.xor = flash.xor ^ flash.tm_buff[i];
        }

        flash.tm_buff[255] = flash.xor;

        flash.tm_buff_update_flag = 1;
    }
}

