#include "RS_232.h"
#include "flash_modul.h"

struct RS_232 rs_232;
struct TM_RS_232_UZGA tm_rs_232_uzga;
struct TM_RS_232_TRV_1ID tm_rs_232_trv_1id;
struct TM_RS_232_TRV_2ID tm_rs_232_trv_2id;
struct TM_RS_232_TRV_3ID tm_rs_232_trv_3id;
struct TM_RS_232_TRV_4ID tm_rs_232_trv_4id;
struct TEST test;

void TRV_RX()
{
    if((rs_232.rx_frame_size == RS232_RX_MAX_FRAME_SIZE) &&                         //проверка размера пакета
       (rs_232.rx_buff[0] == 0xFE) &&                                               //проверка старт байта
       (rs_232.rx_buff[7] == 0xFF) &&                                               //проверка стоп байта
       (rs_232.rx_buff[6] == Calc_CRC8((uint8_t*)(&rs_232.rx_buff), 6, 0)) &&       //проверка контрольной суммы
       ((((rs_232.rx_buff[5] << 8) + rs_232.rx_buff[4])) == rs_232.rx_counter + 1)) //проверка корректности счётчика
    {
        rs_232.rx_counter += 1;
        rs_232.rx_idle_counter = 0;

		if(rs.config_complate_flag)
		{
			if(((rs_232.rx_buff[3]<<8)+rs_232.rx_buff[2])<=1000)

			{
				mtm.trotle=((rs_232.rx_buff[3]<<8)+rs_232.rx_buff[2]);
				mtm.rud=(config.n1_min*10)+(((config.n1_max-config.n1_min)*mtm.trotle)/100);
			}

			switch (rs_232.rx_buff[1])
			{
			case(0x26)://Check
				if(rs_232.last_com!=rs_232.rx_buff[1])
				{
					rs_232.last_com=rs_232.rx_buff[1];
					if(mtm.engine_state==INITIAL_STAGE)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SHECK_VAL;
						mtm.engine_flag=0;
					}

				}


			break;

			case(0x25)://Pump_test_auto
				if(rs_232.last_com!=rs_232.rx_buff[1])
				{
					rs_232.last_com=rs_232.rx_buff[1];
					if(mtm.engine_state==INITIAL_STAGE)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=PUMP_U_ST;
					}
				}
			break;

			case(0x30)://Fly_start
				if(rs_232.last_com!=rs_232.rx_buff[1])
				{
					rs_232.last_com=rs_232.rx_buff[1];

					if((mtm.engine_state==INITIAL_STAGE)
						||(mtm.engine_state==SLOWDOWN)
						||(mtm.engine_state==STAGE_VENT))
					{

						flash.tm_on_flag=1;

						mtm.update_state_flag=1;
						mtm.engine_state=FLY_START_1;

						rs.fly_start_flag=1;


						mtm.engine_flag=0;

					}

				}
			break;

			case(0x41)://Fly_stop
				if(rs_232.last_com!=rs_232.rx_buff[1])
				{
					rs_232.last_com=rs_232.rx_buff[1];

					mtm.update_state_flag=1;
					mtm.engine_state=SLOWDOWN ;

					rs.fly_start_flag=0;
					flash.tm_on_flag=0;
				}
			break;

			case(0x23)://Vent
				if(rs_232.last_com!=rs_232.rx_buff[1])
				{
					rs_232.last_com=rs_232.rx_buff[1];

					if(mtm.engine_state==INITIAL_STAGE)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=STAGE_VENT;


					}
				}
			break;

			case(0x20)://Pump_test_mon
				if(rs_232.last_com!=rs_232.rx_buff[1])
				{
					rs_232.last_com=rs_232.rx_buff[1];

					if(mtm.engine_state==INITIAL_STAGE)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=PUMP_ST;
					}
				}
			break;

			case(0x27)://prs1 on
				rs_232.last_com=rs_232.rx_buff[1];

				rs_232.trv_prs1_count++;

				if(rs_232.trv_prs1_count>2)
				{
					rs_232.trv_prs1_count=0;

					mtm.update_state_flag=1;
					mtm.engine_state=SHECK_PRS_1;
				}


			break;

			case(0x28)://prs2 on

				rs_232.last_com=rs_232.rx_buff[1];

				rs_232.trv_prs2_count++;

				if(rs_232.trv_prs2_count>2)
				{
					rs_232.trv_prs2_count=0;

					mtm.update_state_flag=1;
					mtm.engine_state=SHECK_PRS_2;
				}


			break;

			default:
				{
					rs_232.last_com=0;
				}

			}
		}


	}

	else
	{
		RS_232_Frame_err();


	}

	 HAL_UARTEx_ReceiveToIdle_DMA(&huart1,(uint8_t*) rs_232.rx_buff,sizeof(rs_232.rx_buff));

	 __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
}
void UZGA_RX()
{

	if((rs_232.rx_frame_size==RS232_RX_MAX_FRAME_SIZE)							//проверка размера пакета
		 &&(rs_232.rx_buff[0]==0xFE)											//проверка старт байта
		 &&(rs_232.rx_buff[7]==0xFF)											//проверка стоп байта
		 &&(rs_232.rx_buff[6]==(Calc_CRC8((uint8_t*)&rs_232.rx_buff,6,0)))		//проверка контрольной суммы
		 &&((((rs_232.rx_buff[5]<<8)+rs_232.rx_buff[4]))==(rs_232.rx_counter+1))) //проверка корректности счётчика


	{
		rs_232.rx_counter++;
		rs_232.rx_idle_counter=0;

		if(rs.config_complate_flag)
		{
			if(((rs_232.rx_buff[3]<<8)+rs_232.rx_buff[2])<=1000)

			{
				mtm.trotle=((rs_232.rx_buff[3]<<8)+rs_232.rx_buff[2]);
				mtm.rud=(config.n1_min*10)+(((config.n1_max-config.n1_min)*mtm.trotle)/100);
			}

			switch (rs_232.rx_buff[1])
			{
			case(0x26)://Check
				if(rs_232.last_com!=rs_232.rx_buff[1])
				{
					rs_232.last_com=rs_232.rx_buff[1];
					if(mtm.engine_state==INITIAL_STAGE)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=SHECK_VAL;
						mtm.engine_flag=0;
					}

				}


			break;

			case(0x25)://Pump_test_auto
				if(rs_232.last_com!=rs_232.rx_buff[1])
				{
					rs_232.last_com=rs_232.rx_buff[1];
					if(mtm.engine_state==INITIAL_STAGE)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=PUMP_U_ST;
					}
				}
			break;

			case(0x21)://start


				rs_232.last_com=rs_232.rx_buff[1];

				if((mtm.engine_state==INITIAL_STAGE)
					||(mtm.engine_state==SLOWDOWN)
					||(mtm.engine_state==STAGE_VENT))
				{

					flash.tm_on_flag=1;

					mtm.update_state_flag=1;
					mtm.engine_state=START_1;



				}


			break;


			case(0x22)://stop

				rs_232.last_com=rs_232.rx_buff[1];


				flash.tm_on_flag=0;

				mtm.update_state_flag=1;
				mtm.engine_state=SLOWDOWN;


			break;


			case(0x30)://Fly_start1
				rs_232.last_com=rs_232.rx_buff[1];

				if((mtm.engine_state==INITIAL_STAGE)
					||(mtm.engine_state==SLOWDOWN)
					||(mtm.engine_state==STAGE_VENT))
				{

					flash.tm_on_flag=1;

					mtm.update_state_flag=1;
					mtm.engine_state=FLY_START_1;

					rs.fly_start_flag=1;

				}

		    break;



			case(0x40)://Fly_stop1

				rs_232.last_com=rs_232.rx_buff[1];

				if((mtm.engine_state!=INITIAL_STAGE)
					&&(mtm.engine_state!=SLOWDOWN)
					&&(mtm.engine_state!=STAGE_VENT))
				{
					mtm.update_state_flag=1;
					mtm.engine_state=SLOWDOWN ;

					rs.fly_start_flag=0;
					flash.tm_on_flag=0;
				}

			break;


			case(0x23)://Vent
				if(rs_232.last_com!=rs_232.rx_buff[1])
				{
					rs_232.last_com=rs_232.rx_buff[1];

					if(mtm.engine_state==INITIAL_STAGE)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=STAGE_VENT;


					}
				}
			break;

			case(0x20)://Pump_test_mon
				if(rs_232.last_com!=rs_232.rx_buff[1])
				{
					rs_232.last_com=rs_232.rx_buff[1];

					if(mtm.engine_state==INITIAL_STAGE)
					{
						mtm.update_state_flag=1;
						mtm.engine_state=PUMP_ST;
					}
				}
			break;

			default:
				{
					rs_232.last_com=0;
				}

			}
		}


	}

	else
	{
		//RS_232_Frame_err();


	}

	 HAL_UARTEx_ReceiveToIdle_DMA(&huart1,(uint8_t*) rs_232.rx_buff,sizeof(rs_232.rx_buff));

	 //__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
}
void TRV_TX()
{
	if(rs_232.rx_idle_counter>5)
	{
		mtm.engine_flag=mtm.engine_flag|NO_RX;
	}
	else
	{
		mtm.engine_flag=mtm.engine_flag&(~NO_RX);
		rs_232.rx_idle_counter++;
	}

	switch (rs_232.trv_tx_state)
	{
		case(0)://отправка 1 го кадра
			rs_232.trv_tx_state++;

			tm_rs_232_trv_1id.start_byte=0xFE;
			tm_rs_232_trv_1id.id=1;
			tm_rs_232_trv_1id.com_req=rs_232.last_com;
			tm_rs_232_trv_1id.n1=(mtm.rotor_speed/100);
			tm_rs_232_trv_1id.Tg=(int16_t)(mtm.t_real*10);
			tm_rs_232_trv_1id.state=mtm.engine_state;
			tm_rs_232_trv_1id.counter=rs_232.counter;
			tm_rs_232_trv_1id.crc=Calc_CRC8((uint8_t*)&tm_rs_232_trv_1id,11,0);
			tm_rs_232_trv_1id.stop_byte=0xFF;

		//	if(rs_232.tx_flag==0)
			{
				HAL_UART_Transmit_DMA(&huart1,(uint8_t*)&tm_rs_232_trv_1id,(sizeof tm_rs_232_trv_1id));
		//		rs_232.tx_flag++;
			}

		break;

		case(1)://отправка 2 го кадра
			rs_232.trv_tx_state++;

			tm_rs_232_trv_2id.start_byte=0xFE;
			tm_rs_232_trv_2id.id=2;
			tm_rs_232_trv_2id.com_req=rs_232.last_com;
			tm_rs_232_trv_2id.flag=mtm.engine_flag;
			tm_rs_232_trv_2id.trottle=mtm.trotle;
			tm_rs_232_trv_2id.bus_volt=(uint16_t)(mtm.bus_volt*10);
			tm_rs_232_trv_2id.counter=rs_232.counter;
			tm_rs_232_trv_2id.crc=Calc_CRC8((uint8_t*)&tm_rs_232_trv_2id,11,0);
			tm_rs_232_trv_2id.stop_byte=0xFF;

			//if(rs_232.tx_flag==0)
			{
				HAL_UART_Transmit_DMA(&huart1,(uint8_t*)&tm_rs_232_trv_2id,(sizeof tm_rs_232_trv_2id));
				//rs_232.tx_flag++;
			}


		break;

		case(2)://отправка 3 го кадра
			rs_232.trv_tx_state++;


			tm_rs_232_trv_3id.start_byte=0xFE;
			tm_rs_232_trv_3id.id=3;
			tm_rs_232_trv_3id.com_req=rs_232.last_com;
			tm_rs_232_trv_3id.starter_pwm=STARTER_PWM;
			tm_rs_232_trv_3id.fuel_pwm=PUMP_PWM;
			tm_rs_232_trv_3id.fuel_rpm=mtm.pump_speed;
			tm_rs_232_trv_3id.counter=rs_232.counter;
			tm_rs_232_trv_3id.crc=Calc_CRC8((uint8_t*)&tm_rs_232_trv_3id,11,0);
			tm_rs_232_trv_3id.stop_byte=0xFF;


			//if(rs_232.tx_flag==0)
			{
				HAL_UART_Transmit_DMA(&huart1,(uint8_t*)&tm_rs_232_trv_3id,(sizeof tm_rs_232_trv_3id));
			//	rs_232.tx_flag++;
			}

		break;

		case(3)://отправка 4 го кадра
			rs_232.trv_tx_state=0;

			tm_rs_232_trv_4id.start_byte=0xFE;
			tm_rs_232_trv_4id.id=4;
			tm_rs_232_trv_4id.com_req=rs_232.last_com;
			tm_rs_232_trv_4id.frame_err_counter=rs_232.frame_err_count;
			// TODO: tm_rs_232_trv_4id.res1=hc206s.p;
			tm_rs_232_trv_4id.res2=0;
			tm_rs_232_trv_4id.counter=rs_232.counter;
			tm_rs_232_trv_4id.crc=Calc_CRC8((uint8_t*)&tm_rs_232_trv_4id,11,0);
			tm_rs_232_trv_4id.stop_byte=0xFF;

			//if(rs_232.tx_flag==0)
			{
				HAL_UART_Transmit_DMA(&huart1,(uint8_t*)&tm_rs_232_trv_4id,(sizeof tm_rs_232_trv_4id));
			//	rs_232.tx_flag++;
			}


		break;

		default:
		{
			rs_232.trv_tx_state=0;
		}


	}
	rs_232.counter++;

}

void UZGA_TX()
{




	tm_rs_232_uzga.start_byte=0xFE;
	tm_rs_232_uzga.com_req=rs_232.last_com;
	tm_rs_232_uzga.n1=(mtm.rotor_speed/100);
	tm_rs_232_uzga.Tg=(int16_t)(mtm.t_real*10);
	tm_rs_232_uzga.state=mtm.engine_state;
	tm_rs_232_uzga.flag=mtm.engine_flag;
	if((mtm.engine_flag&C_CONTROL)||(mtm.engine_flag&A_CONTROL))
	{
		tm_rs_232_uzga.flag=tm_rs_232_uzga.flag&(~1);
	}
	else
	{

		tm_rs_232_uzga.flag=tm_rs_232_uzga.flag|1;
	}
	tm_rs_232_uzga.trottle=mtm.trotle;

	tm_rs_232_uzga.bus_volt=(uint16_t)(mtm.bus_volt*10);
	tm_rs_232_uzga.starter_pwm=STARTER_PWM;
	tm_rs_232_uzga.fuel_pwm=PUMP_PWM;
	tm_rs_232_uzga.fuel_rpm=mtm.pump_speed;
	tm_rs_232_uzga.frame_err_counter=rs_232.frame_err_count;
	tm_rs_232_uzga.counter=rs_232.counter;
	tm_rs_232_uzga.ver=VER;
	tm_rs_232_uzga.checksum=mtm.crc16;
	tm_rs_232_uzga.crc=Calc_CRC8((uint8_t*)&tm_rs_232_uzga,30,0);
	tm_rs_232_uzga.stop_byte=0xFF;

	HAL_UART_Transmit_DMA(&huart1,(uint8_t*)&tm_rs_232_uzga,(sizeof tm_rs_232_uzga));

	rs_232.counter++;
}

uint8_t Calc_CRC8(uint8_t *data, uint8_t lenght, uint8_t seed)
{
    uint8_t crc = seed;

    static uint8_t crc_arr[] = {
                0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83,
                0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41,
                0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e,
                0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc,
                0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0,
                0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
                0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d,
                0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff,
                0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5,
                0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07,
                0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58,
                0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
                0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6,
                0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24,
                0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
                0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9,
                0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f,
                0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
                0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92,
                0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50,
                0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c,
                0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee,
                0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1,
                0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
                0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49,
                0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b,
                0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4,
                0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16,
                0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a,
                0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
                0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7,
                0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35,
                };

    for(uint8_t i = 0; i < lenght; ++i)
    {
        crc = crc_arr[crc^data[i]];
    }
    return crc;
}



void RS_232_Send_tm()
{

}

void RS_232_Frame_err()
{
	//HAL_UARTEx_ReceiveToIdle_DMA(&huart1,(uint8_t*) rs_232.rx_buff,sizeof(rs_232.rx_buff));
	  rs_232.frame_err_count++;
	  rs_232.rx_counter=((rs_232.rx_buff[5]<<8)+rs_232.rx_buff[4]);
}
void RS_232_Baud_Rate_CH()
{




	  switch (config.variation)
	  {
		  case(5):
			  rs_232.baud_rate = 9600;

		  	  rs_232.init_flag=1;

		  break;

		  case(6):
			  rs_232.baud_rate = 19200;

		  	  rs_232.init_flag=1;

		  break;


		  default:
		  {

		  }
	  }


}


void Transmite_232()
{
	  switch (config.variation)
	  {
		  case(5)://трв
		  	  if(tim.counter_ms>=20)
		  	  {
		  		tim.counter_ms=0;
		  		TRV_TX();

			//Test(START_VALVE_PWM,MAIN_VALVE_PWM);
		  	  }

		  break;

		  case(6)://узга
			  if(tim.counter_ms>=40)
			  {
				tim.counter_ms=0;
				UZGA_TX();

				//Test(START_VALVE_PWM,MAIN_VALVE_PWM);
			  }
		  break;


		  default:
		  {

		  }
	  }
}



void Receive_232()
{
	  switch (config.variation)
	  {
		  case(5)://трв

		  	  {

		  		TRV_RX();
		  		rs_232.used_flag=1;
		  	  }

		  break;

		  case(6)://узга

			  {

				UZGA_RX();
				rs_232.used_flag=1;
			  }
		  break;


		  default:
		  {

		  }
	  }


}
void Test(uint8_t com, uint16_t rud)
{
	test.start_byte=0xFE;

	test.com=com;
	test.rud=rud;
	test.counter=rs_232.counter;
	test.crc=Calc_CRC8((uint8_t*)&test,6,0);
	test.stop_byte=0xFF;

	HAL_UART_Transmit_DMA(&huart1,(uint8_t*)&test,((sizeof test)));
	rs_232.counter++;
}
