

#include "RS_485.h"
#include "pump.h"
#include "starter.h"
#include "system.h"
#include "tim.h"

extern struct Master_TM mtm;
extern struct CONFIG config;
extern struct TIM tim;

struct RS rs;
struct RS_Service_Direct_Frame rs_service_direction;
struct RS_Service_TM_Frame rs_service_tm;
struct RS_Standart_TM_Frame rs_standart_tm;
struct STANDART_REQ sr;

struct CRC_Frame crc_frame;

/*
 * #define DIRECT_FRAME_SIZE 						7
#define DIRECT_SCREEN_FRAME_SIZE 				8

#define RUD_COUNT_FRAME_SIZE 					9
#define RUD_COUNT_SCREEN_FRAME_SIZE			 	10



#define SERVICE_FRAME_SIZE 						21
#define SERVICE_SCREEN_FRAME_SIZE			 	22

#define CONFIG_FRAME_SIZE						256
 */


uint8_t Frame_Test()
{
    if(rs.config_updating)
    {
        rs.config_updating = 0;
        if(rs.receive_package_size == 256)
        {
            if(RS_SU_RePack((uint8_t*)(&config), rs.receive_package_size, 0))
            {
                flash.config_update_flag = 1;
                HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&sr),
                                      (sizeof(sr) - RS_SU_Pack((uint8_t*)(&sr), sizeof(sr), 1)));
            }
            else
            {
                HAL_UARTEx_ReceiveToIdle_DMA(rs485_puart, (uint8_t*)rs.rs485_rx_buff, sizeof(rs.rs485_rx_buff));
            }
        }
    }
    else
    {
        if((rs.receive_package_size == DIRECT_FRAME_SIZE) || (rs.receive_package_size == DIRECT_SCREEN_FRAME_SIZE) ||
           (rs.receive_package_size == SERVICE_FRAME_SIZE) || (rs.receive_package_size == SERVICE_SCREEN_FRAME_SIZE) ||
           (rs.receive_package_size == RUD_COUNT_FRAME_SIZE) || (rs.receive_package_size == RUD_COUNT_SCREEN_FRAME_SIZE) ||
           (rs.receive_package_size == FAT_SERVISE_FRAME_SIZE))
        {
            if(RS_SU_RePack(rs.rs485_rx_buff, rs.receive_package_size, 0))
            {
                if(((rs.rs485_rx_buff[2] == config.address) ||
                   ((rs.config_complate_flag == 0) && (rs.rs485_rx_buff[2] == DEFAULT_ADDRESS)) ||
                   rs.rs485_rx_buff[2] == config.group_address) || (rs.rs485_rx_buff[2] == 0xBB))
                {
                    mtm.su_485_counter = 0;
                    switch(rs.rs485_rx_buff[3])
                    {
                        case(DATA_REQUEST):
                        {
                            rs.used_flag = 1;
                            rs.req_count++;

                            rs_standart_tm.n1 = (uint16_t)(mtm.rotor_speed / 100);
                            rs_standart_tm.tg = (int16_t)(mtm.t_real);
                            rs_standart_tm.state = mtm.engine_state;
                            rs_standart_tm.flag = mtm.engine_flag;
                            rs_standart_tm.rud = mtm.trotle;
                            rs_standart_tm.bus_volt = (uint16_t)(mtm.bus_volt * 10);
                            //rs_standart_tm.starter_pwm = starter.duty;  //STARTER_PWM;
                            //rs_standart_tm.pump_pwm = pump.duty;  //PUMP_PWM;
                            //rs_standart_tm.pump_fb = (uint16_t)pump.filtspeed;  //mtm.pump_speed;
                            rs_standart_tm.counter++;

                            HAL_UART_Transmit_DMA(rs485_puart,(uint8_t*)(&rs_standart_tm),
                                                  (sizeof(rs_standart_tm) - RS_SU_Pack((uint8_t*)(&rs_standart_tm),
                                                  sizeof(rs_standart_tm), 1)));
                        }
                            break;
                        case(SERVICE_DATA_REQUEST):
                        {
                            rs.used_flag = 1;
                            if(rs_service_direction.var_flag)
                            {
                                if(rs_service_direction.starter_pwm != ((rs.rs485_rx_buff[4] << 8) + rs.rs485_rx_buff[5]))
                                {
                                    // TODO: STARTER_PWM =
                                    rs_service_direction.starter_pwm = ((rs.rs485_rx_buff[4] << 8) + rs.rs485_rx_buff[5]);
                                }
                                if(rs_service_direction.pump_pwm != ((rs.rs485_rx_buff[6] << 8) + rs.rs485_rx_buff[7]))
                                {
                                    // TODO: PUMP_PWM =
                                    rs_service_direction.pump_pwm = ((rs.rs485_rx_buff[6] << 8) + rs.rs485_rx_buff[7]);
                                    Pump_SetDuty(rs_service_direction.pump_pwm);
                                }
                                if(rs_service_direction.valve1_pwm != ((rs.rs485_rx_buff[8] << 8) + rs.rs485_rx_buff[9]))
                                {
                                    // TODO: START_VALVE_PWM =
                                    rs_service_direction.valve1_pwm = ((rs.rs485_rx_buff[8] << 8) + rs.rs485_rx_buff[9]);
                                    ValveStart_SetDuty(rs_service_direction.valve1_pwm);
                                }
                                if(rs_service_direction.valve2_pwm != ((rs.rs485_rx_buff[10] << 8) + rs.rs485_rx_buff[11]))
                                {
                                    // TODO: MAIN_VALVE_PWM =
                                    rs_service_direction.valve2_pwm = ((rs.rs485_rx_buff[10] << 8) + rs.rs485_rx_buff[11]);
                                    ValveMain_SetDuty(rs_service_direction.valve2_pwm);
                                    // TODO: PUMP2_PWM=1000+MAIN_VALVE_PWM;
                                }
                                if(rs_service_direction.plug_pwm != ((rs.rs485_rx_buff[12] << 8) + rs.rs485_rx_buff[13]))
                                {
                                    rs_service_direction.plug_pwm = ((rs.rs485_rx_buff[12] << 8) + rs.rs485_rx_buff[13]);
                                    if(rs_service_direction.plug_pwm)
                                    {
                                        //PLUG_ON
                                        mtm.prs_state = 1;
                                    }
                                    else
                                    {
                                        //PLUG_OFF
                                        mtm.prs_state = 0;
                                    }
                                    Ignition_SetDuty(rs_service_direction.plug_pwm);
                                }
                                if(rs_service_direction.com_check != rs.rs485_rx_buff[14])
                                {
                                    rs_service_direction.com_check = rs.rs485_rx_buff[14];

                                    if(rs_service_direction.com_check)
                                    {
                                    }
                                    else
                                    {
                                    }
                                }
                                if(rs_service_direction.com1 != rs.rs485_rx_buff[15])
                                {
                                    rs_service_direction.com1 = rs.rs485_rx_buff[15];

                                    if(rs_service_direction.com1)
                                    {
                                        mtm.hot_state = 1;
                                    }
                                    else
                                    {
                                        mtm.hot_state = 0;
                                    }
                                }
                                if(rs_service_direction.com2 != rs.rs485_rx_buff[16])
                                {
                                    rs_service_direction.com2 = rs.rs485_rx_buff[16];

                                    if(rs_service_direction.com2)
                                    {
                                        ADG_ON
                                        //flash.flag_update_state=1;
                                        // flash.state=ERSE_CHIP;
                                    }
                                    else
                                    {
                                        ADG_OFF
                                    }
                                }
                                if(rs_service_direction.com3 != rs.rs485_rx_buff[17])
                                {
                                    rs_service_direction.com3 = rs.rs485_rx_buff[17];
                                    if(rs_service_direction.com3)
                                    {
                                        GEN_ON
                                        // flash.tm_on_flag=1;
                                    }
                                    else
                                    {
                                        GEN_OFF
                                        // flash.tm_on_flag=0;
                                    }
                                }
                            }
                            else
                            {
                                rs_service_direction.var_flag = 1;

                                rs_service_direction.starter_pwm = ((rs.rs485_rx_buff[4] << 8) + rs.rs485_rx_buff[5]);
                                rs_service_direction.pump_pwm = ((rs.rs485_rx_buff[6] << 8) + rs.rs485_rx_buff[7]);
                                rs_service_direction.valve1_pwm = ((rs.rs485_rx_buff[8] << 8) + rs.rs485_rx_buff[9]);
                                rs_service_direction.valve2_pwm = ((rs.rs485_rx_buff[10] << 8) + rs.rs485_rx_buff[11]);
                                rs_service_direction.plug_pwm = ((rs.rs485_rx_buff[12] << 8) + rs.rs485_rx_buff[13]);

                                rs_service_direction.com_check = rs.rs485_rx_buff[14];

                                rs_service_direction.com1 = rs.rs485_rx_buff[15];
                                rs_service_direction.com2 = rs.rs485_rx_buff[16];
                                rs_service_direction.com3 = rs.rs485_rx_buff[17];
                            }

                            rs_service_tm.n1_h = (uint8_t)((mtm.rotor_speed / 10) >> 8);
                            rs_service_tm.n1_l = (uint8_t)((mtm.rotor_speed / 10));

                            rs_service_tm.tg = (mtm.t_real);


                            rs_service_tm.state_h = (uint8_t)(mtm.engine_state >> 8);
                            rs_service_tm.state_l = (uint8_t)(mtm.engine_state);

                            rs_service_tm.flag_h = (uint8_t)(mtm.engine_flag >> 8);
                            rs_service_tm.flag_l = (uint8_t)(mtm.engine_flag);

                            rs_service_tm.rud_h = (uint8_t)(mtm.trotle >> 8);
                            rs_service_tm.rud_l = (uint8_t)(mtm.trotle);

                            // TODO: rs_service_tm.bus_volt_h=(uint8_t)(((uint16_t)(exadc.ch_voltage[0]*110))>>8);
                            // TODO: rs_service_tm.bus_volt_l=(uint8_t)(exadc.ch_voltage[0]*110);

                            rs_service_tm.starter_pwm_h = (uint8_t)(starter.duty >> 8);  //(STARTER_PWM >> 8);
                            rs_service_tm.starter_pwm_l = (uint8_t)starter.duty;  //(STARTER_PWM);

                            rs_service_tm.pump_pwm_h = (uint8_t)(starter.duty >> 8);  //(PUMP_PWM >> 8);
                            rs_service_tm.pump_pwm_l = (uint8_t)(starter.duty);  //(PUMP_PWM);

                            rs_service_tm.pump_fb_h = (uint8_t)((uint16_t)(pump.filtspeed) >> 8);  //(mtm.pump_speed >> 8);
                            rs_service_tm.pump_fb_l = (uint8_t)((uint16_t)pump.filtspeed);  //(mtm.pump_speed);

                            // TODO: rs_service_tm.pump_cur_h=(uint8_t)(((uint16_t)(exadc.ch_voltage[CH_PUMP_CUR]))>>8);
                            // TODO: rs_service_tm.pump_cur_l=(uint8_t)((uint16_t)(exadc.ch_voltage[CH_PUMP_CUR]));

                            rs_service_tm.valve1_cur_h = (uint8_t)(((uint16_t)(mtm.valve1_cur * 100)) >> 8);
                            rs_service_tm.valve1_cur_l = (uint8_t)((uint16_t)(mtm.valve1_cur * 100));

                            rs_service_tm.valve2_cur_h = (uint8_t)(((uint16_t)(mtm.valve2_cur * 100)) >> 8);
                            rs_service_tm.valve2_cur_l = (uint8_t)((uint16_t)(mtm.valve2_cur * 100));

                            rs_service_tm.plug_cur_h = (uint8_t)(((uint16_t)(mtm.plug_current * 100)) >> 8);
                            rs_service_tm.plug_cur_l = (uint8_t)((uint16_t)(mtm.plug_current * 100));

                            rs_service_tm.valve1_pwm_h = (uint8_t)(htim15.Instance->CCR1 >> 8);  //(START_VALVE_PWM >> 8);
                            rs_service_tm.valve1_pwm_l = (uint8_t)(htim15.Instance->CCR1);  //(START_VALVE_PWM);

                            rs_service_tm.valve2_pwm_h = (uint8_t)(htim15.Instance->CCR2 >> 8);  //(MAIN_VALVE_PWM >> 8);
                            rs_service_tm.valve2_pwm_l = (uint8_t)(htim15.Instance->CCR2);  //(MAIN_VALVE_PWM);

                            rs_service_tm.plug_pwm_l = mtm.prs_state;

                            rs_service_tm.t_gen_h = (((uint16_t)(mtm.t_gen * 10)) >> 8);
                            rs_service_tm.t_gen_l = (((uint16_t)(mtm.t_gen * 10)) & 0xFF);

                            rs_service_tm.t_diod = (int16_t)mtm.t_real_int;

                            // TODO: rs_service_tm.t1_h=(uint8_t)(((uint16_t)(hc206s.p>>8)));
                            // TODO: rs_service_tm.t1_l=(uint8_t)(hc206s.p);

                            // TODO: rs_service_tm.t2_h=(uint8_t)(((uint16_t)(hc206s.a>>8)));
                            // TODO: rs_service_tm.t2_l=(uint8_t)(hc206s.a);

                            // TODO: rs_service_tm.t3_h=(uint8_t)(((uint16_t)(hc206s.t>>8)));
                            // TODO: rs_service_tm.t3_l=(uint8_t)(hc206s.t);

                            rs_service_tm.s1_h = ((mtm.crc32) >> 24);
                            rs_service_tm.s1_l = ((mtm.crc32) >> 16);

                            rs_service_tm.s2_h = ((mtm.crc32) >> 8);
                            rs_service_tm.s2_l = (mtm.crc32);

                            rs_service_tm.s3_h = (((uint16_t)(mtm.pump_current)) >> 8);
                            rs_service_tm.s3_l = (uint8_t)(mtm.pump_current);

                            // TODO: rs_service_tm.s4_h=(((uint16_t)(exadc.ch_voltage[CH_STARTER_CUR]))>>8);
                            // TODO: rs_service_tm.s4_l=(uint8_t)(exadc.ch_voltage[CH_STARTER_CUR]);

                            // TODO: rs_service_tm.s5_h=(((uint16_t)(exadc.ch_voltage[3]*100))>>8);
                            // TODO: rs_service_tm.s5_l=(uint8_t)(exadc.ch_voltage[3]*100);


                            rs_service_tm.s6_h = (((uint16_t)(flash.file_offset)) >> 8);
                            rs_service_tm.s6_l = (uint8_t)(flash.file_offset);

                            rs_service_tm.s7_h = (((uint16_t)(flash.oper_address)) >> 8);
                            rs_service_tm.s7_l = (uint8_t)(flash.oper_address);

                            rs_service_tm.s8_h = (((uint16_t)(flash.file_address)) >> 8);
                            rs_service_tm.s8_l = (uint8_t)(flash.file_address);

                            rs_service_tm.s9_h = (((uint16_t)(0)) >> 8);
                            // TODO: rs_service_tm.s9_l=(uint8_t)(rs_232.rx_frame_size);

                            // TODO: rs_service_tm.s10_h=(((uint16_t)(exadc.ch_voltage[7]*1000))>>8);
                            // TODO: rs_service_tm.s10_l=((uint8_t)exadc.ch_voltage[7]*1000);

                            rs_service_tm.tim_0byte = tim.timer_ms & 0xFF;
                            rs_service_tm.tim_1byte = (tim.timer_ms >> 8) & 0xFF;
                            rs_service_tm.tim_2byte = (tim.timer_ms >> 16) & 0xFF;
                            rs_service_tm.tim_3byte = (tim.timer_ms >> 24) & 0xFF;

                            HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&rs_service_tm),
                                                  (sizeof(rs_service_tm) - RS_SU_Pack((uint8_t*)(&rs_service_tm), sizeof(rs_service_tm), 1)));
                        }
                            break;
                        case(CHECK)://ПРОВЕРКА
                            rs.used_flag = 1;
                            if(mtm.engine_state == INITIAL_STAGE)
                            {
                                mtm.update_state_flag = 1;
                                mtm.engine_state = SHECK_VAL;
                                mtm.engine_flag = 0;
                            }
                            HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&sr),
                                                  (sizeof(sr) - RS_SU_Pack((uint8_t*)(&sr), sizeof(sr), 1)));
                            break;
                        case(AUTO_PUMP):
                            rs.used_flag = 1;
                            if(mtm.engine_state == INITIAL_STAGE)
                            {
                                mtm.update_state_flag = 1;
                                mtm.engine_state = PUMP_ST;
                            }
                            HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&sr),
                                                  (sizeof(sr) - RS_SU_Pack((uint8_t*)(&sr), sizeof(sr), 1)));
                            break;
                        case(PUMP):
                            rs.used_flag = 1;
                            if(mtm.engine_state == INITIAL_STAGE)
                            {
                                mtm.update_state_flag = 1;
                                mtm.engine_state = PUMP_U_ST;
                            }
                            HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&sr),
                                                  (sizeof(sr) - RS_SU_Pack((uint8_t*)(&sr), sizeof(sr), 1)));
                            break;
                        case(VENT):
                            rs.used_flag = 1;
                            if(mtm.engine_state == INITIAL_STAGE)
                            {
                                mtm.update_state_flag = 1;
                                mtm.engine_state = STAGE_VENT;
                            }
                            HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&sr),
                                                  (sizeof(sr) - RS_SU_Pack((uint8_t*)(&sr), sizeof(sr), 1)));
                            break;
                        case(START):
                            rs.used_flag = 1;
                            if((mtm.engine_state == INITIAL_STAGE) || (mtm.engine_state == SLOWDOWN) || (mtm.engine_state == STAGE_VENT))
                            {
                                mtm.update_state_flag = 1;
                                mtm.engine_state = START_1;
                                mtm.engine_flag = 0;
                                flash.tm_on_flag = 1;
                            }
                            rs.start_count++;
                            HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&sr),
                                                  (sizeof(sr) - RS_SU_Pack((uint8_t*)(&sr), sizeof(sr), 1)));
                            break;
                        case(START1):
                            rs.used_flag = 1;
                            if((mtm.engine_state == INITIAL_STAGE) || (mtm.engine_state == SLOWDOWN) || (mtm.engine_state == STAGE_VENT))
                            {
                                //flash.tm_on_flag=1;
                            }
                            rs.start1_count++;
                            rs.start2_count = 0;
                            HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&sr),
                                                  (sizeof(sr) - RS_SU_Pack((uint8_t*)(&sr), sizeof(sr), 1)));
                            break;
                        case(START2):
                            rs.used_flag = 1;
                            rs.start2_count++;
                            if((mtm.engine_state == INITIAL_STAGE) || (mtm.engine_state == SLOWDOWN) || (mtm.engine_state == STAGE_VENT))
                            {
                                flash.tm_on_flag = 1;
                                rs.stop1_count = 0;
                                rs.stop2_count = 0;

                                if(mtm.stend_flag && rs.start1_count && rs.start2_count)
                                {
                                    mtm.update_state_flag = 1;
                                    mtm.engine_state = FLY_START_1;
                                    rs.fly_start_flag = 1;
                                }
                                else if((rs.start1_count > 3) && (rs.start2_count > 3))
                                {
                                    mtm.update_state_flag = 1;
                                    mtm.engine_state = FLY_START_1;
                                    rs.fly_start_flag = 1;
                                }
                            }
                            HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&sr),
                                                  (sizeof(sr) - RS_SU_Pack((uint8_t*)(&sr), sizeof(sr), 1)));
                            break;
                        case(STOP):
                            rs.used_flag = 1;
                            if(rs.fly_start_flag == 0)
                            {
                                mtm.update_state_flag = 1;
                                mtm.engine_state = SLOWDOWN;
                                flash.tm_on_flag = 0;
                            }
                            rs.stop_count++;
                            HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&sr),
                                                  (sizeof(sr) - RS_SU_Pack((uint8_t*)(&sr), sizeof(sr), 1)));
                            break;
                        case(STOP1):
                            rs.used_flag = 1;
                            if(mtm.engine_state != STAGE_VENT)
                            {
                                flash.tm_on_flag = 0;
                            }
                            rs.stop1_count++;
                            rs.stop2_count = 0;
                            HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&sr),
                                                  (sizeof(sr) - RS_SU_Pack((uint8_t*)(&sr), sizeof(sr), 1)));
                            break;
                        case(STOP2):
                            rs.used_flag = 1;
                            rs.stop2_count++;
                            if((mtm.engine_state != INITIAL_STAGE) || (mtm.engine_state != SLOWDOWN) || (mtm.engine_state != STAGE_VENT))
                            {
                                flash.tm_on_flag = 0;
                                if(mtm.stend_flag && rs.stop1_count && rs.stop2_count)
                                {
                                    mtm.update_state_flag = 1;
                                    mtm.engine_state = SLOWDOWN;
                                    rs.fly_start_flag = 0;
                                }
                                if((rs.stop1_count > 3) && (rs.stop2_count > 3))
                                {
                                    mtm.update_state_flag = 1;
                                    mtm.engine_state = SLOWDOWN;
                                    rs.fly_start_flag = 0;
                                }
                            }
                            HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&sr),
                                                  (sizeof(sr) - RS_SU_Pack((uint8_t*)(&sr), sizeof(sr), 1)));
                            break;
                        case(SET_RUD):
                            rs.used_flag = 1;
                            if(((rs.rs485_rx_buff[5] << 8) + rs.rs485_rx_buff[4]) <= 1000)
                            {
                                if(rs.config_complate_flag && ((can.used_flag == 0) || (rs_232.used_flag == 0)))
                                {
                                    mtm.trotle = (rs.rs485_rx_buff[5] << 8) + rs.rs485_rx_buff[4];
                                    mtm.rud = config.n1_min * 10 + (config.n1_max - config.n1_min) * mtm.trotle / 100;
                                    rs.rud_count++;
                                }
                            }
                            HAL_UARTEx_ReceiveToIdle_DMA(rs485_puart, (uint8_t*)rs.rs485_rx_buff, sizeof(rs.rs485_rx_buff));
                            break;
                        case(BSU_COUNTER):
                            rs.used_flag = 1;
                            rs.bsu_count_new = (((rs.rs485_rx_buff[5] << 8) + rs.rs485_rx_buff[4]) <= 1000);
                            if(rs.bsu_count_new == rs.bsu_count_old + config.engine_qualiti)
                            {
                                rs.bsu_count++;
                            }
                            rs.bsu_count_old = rs.bsu_count_new;
                            HAL_UARTEx_ReceiveToIdle_DMA(rs485_puart, (uint8_t*)rs.rs485_rx_buff, sizeof(rs.rs485_rx_buff));
                            break;
                        case(SEND_CINFIG):
                            if(rs.used_flag == 0)
                            {
                                mtm.stend_flag = 1;
                            }
                            config.numb_ver = VER;
                            config.day = DAY;
                            config.mounth = MOUNTH;
                            config.year = YEAR;
                            HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&config),
                                                  (sizeof(config) - RS_SU_Pack((uint8_t*)(&config), sizeof(config), 0)));
                            break;
                        case(LOAD_CONFIG1):
                            rs.used_flag = 1;
                            rs.config_updating = 1;
                            flash.erse_sektor_address = CONFIG_ADRESS;
                            flash.need_erse_flag = 1;
                            rs.config_complate_flag = 0;
                            HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&sr),
                                                  (sizeof(sr) - RS_SU_Pack((uint8_t*)(&sr), sizeof(sr), 1)));
                            break;
                        case(RESET)://ресет
                            HAL_NVIC_SystemReset();
                            break;
                        case(SEND_FAT):
                            rs.used_flag = 1;
                            flash.send_fat_flag = 1;
                            break;
                        case(SEND_TM_FILE):
                            rs.used_flag = 1;
                            flash.send_tm_flag = 1;
                            flash.uploading_file_address_begin = ((rs.rs485_rx_buff[4] << 24) + (rs.rs485_rx_buff[5] << 16) +
                                                                 (rs.rs485_rx_buff[6] << 8) + rs.rs485_rx_buff[7]);
                            flash.uploading_file_address_end = ((rs.rs485_rx_buff[8] << 24) + (rs.rs485_rx_buff[9] << 16) +
                                                               (rs.rs485_rx_buff[10] << 8) + rs.rs485_rx_buff[11]);
                            break;
                        case(SEND_OPER_COUNT)://выгрузить счетчик моточасов
                            rs.used_flag = 1;
                            if(PIN == ((rs.rs485_rx_buff[4] << 8) + rs.rs485_rx_buff[5]))
                            {
                                flash.oper_count_buff[0] = 0xFE;
                                flash.oper_count_buff[1] = 0xFE;
                                flash.oper_count_buff[2] = config.address;
                                flash.oper_count_buff[3] = 0;

                                flash.operation_count_min = flash.operation_count / 6;

                                flash.oper_count_buff[4] = flash.operation_count_min << 8;
                                flash.oper_count_buff[5] = flash.operation_count_min;


                                flash.oper_count_buff[7] = 0xFF;
                                flash.oper_count_buff[8] = 0xFF;

                                HAL_UART_Transmit_DMA(rs485_puart, (uint8_t*)(&flash.oper_count_buff),
                                                      (sizeof(flash.oper_count_buff) - RS_SU_Pack((uint8_t*)(&flash.oper_count_buff),
                                                                                                  sizeof(flash.oper_count_buff), 0)));
                            }
                            else
                            {
                                HAL_UARTEx_ReceiveToIdle_DMA(rs485_puart, (uint8_t*)rs.rs485_rx_buff, sizeof(rs.rs485_rx_buff));
                            }
                            break;
                        case(RES_OPER_COUNT)://сбросить счетчик моточасов
                            rs.used_flag = 1;
                            if(PIN == ((rs.rs485_rx_buff[4] << 8) + rs.rs485_rx_buff[5]))
                            {
                                if(flash.need_erse_flag == 0)
                                {
                                    flash.need_erse_flag = 0;
                                    flash.erse_sektor_address = OPERATION_COUNTER_BEGIN_ADRESS;
                                    flash.operation_count = 0;
                                }
                                HAL_UART_Transmit_DMA(rs485_puart,(uint8_t*)(&sr),
                                                      (sizeof(sr) - RS_SU_Pack((uint8_t*)(&sr), sizeof(sr), 1)));
                            }
                            else if(((rs.rs485_rx_buff[4] << 8) + rs.rs485_rx_buff[5]) == 6666)
                            {
                                flash.flag_update_state = 1;
                                flash.state = ERSE_CHIP;
                            }
                            else
                            {
                                HAL_UARTEx_ReceiveToIdle_DMA(rs485_puart, (uint8_t*)rs.rs485_rx_buff, sizeof(rs.rs485_rx_buff));
                            }
                            break;
                        case (CRC_REQUEST):
                                break;
                        default:
                        {
                            HAL_UARTEx_ReceiveToIdle_DMA(rs485_puart, (uint8_t*)rs.rs485_rx_buff, sizeof(rs.rs485_rx_buff));
                        }
                    }
                }
                else
                {
                    HAL_UARTEx_ReceiveToIdle_DMA(rs485_puart, (uint8_t*)rs.rs485_rx_buff, sizeof(rs.rs485_rx_buff));
                }
            }
            else
            {
                HAL_UARTEx_ReceiveToIdle_DMA(rs485_puart, (uint8_t*)rs.rs485_rx_buff, sizeof(rs.rs485_rx_buff));
            }
        }
        else
        {
            HAL_UARTEx_ReceiveToIdle_DMA(rs485_puart, (uint8_t*)rs.rs485_rx_buff, sizeof(rs.rs485_rx_buff));
        }
    }
    return 0;
}


//считаем контрольную сумму, записываем и добавляем экраны где нужно---------------
uint8_t RS_SU_Pack(uint8_t *data, uint16_t size, uint8_t screen_byte)
{
    data[0] = 0xFE;
    data[1] = 0xFE;
    data[3] = 0;
    data[size - 1] = 0xFF;
    data[size - 2] = 0xFF;
    data[size - 3] = 0xFF;

    if(rs.config_complate_flag)
    {
        data[2] = config.address;
    }
    else
    {
        data[2] = DEFAULT_ADDRESS;
    }

    if(screen_byte)
    {
        for(rs.i = 2; rs.i < size - 4; rs.i++)
        {
        rs.xor = rs.xor ^ data[rs.i];
        }

        data[size - 4] = rs.xor;

        if(rs.xor == 0xFF)
        {
            data[size - 3] = 0;
            return 0;
        }
        return 1;
    }
    else
    {
        for(rs.i = 2; rs.i < size - 3; rs.i++)
        {
            rs.xor = rs.xor ^ data[rs.i];
        }
        data[size - 3] = rs.xor;
        return 0;
    }
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
uint8_t RS_SU_RePack(uint8_t *data, uint16_t size, uint8_t screen_byte)
{
    if((data[0] == 0xFE) && (data[1] == 0xFE) && (data[size - 1] == 0xFF) && (data[size - 2] == 0xFF))
    {
        rs.xor = 0;
        for(rs.i = 2; rs.i < (size - 2); rs.i++)
        {
            rs.xor = rs.xor ^ data[rs.i];
        }
        if(rs.xor == 0)
        {
            return 1;
        }
    }
    return 0;
}
