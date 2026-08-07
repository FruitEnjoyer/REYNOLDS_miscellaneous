#include "CAN.h"

FDCAN_TxHeaderTypeDef TxHeader_tm;
FDCAN_TxHeaderTypeDef TxHeader_ver;

FDCAN_RxHeaderTypeDef RxHeader_info;
FDCAN_RxHeaderTypeDef RxHeader_direct;

struct CAN can;

void CAN_RX_Frame_Test()
{
    if(can.flag_tm_req && can.tx_flag == 0)//запрос первого кадра тм
    {
        can.flag_tm_req = 0;
        can.tx_flag = 1;
        CAN_TM_Complate();
        TxHeader_tm.Identifier = REQ_TM_FRAME_ID;
        HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader_tm, can.tx_buff_tm);
    }
    if(can.flag_tm2_req && can.tx_flag == 0)//запрос второго кадра тм
    {
        can.flag_tm2_req = 0;
        can.tx_flag = 1;
        CAN_TM_Complate();
        TxHeader_tm.Identifier = REQ_TM2_FRAME_ID;
        HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader_tm, can.tx_buff2_tm);
    }
    if(can.flag_ver_req && can.tx_flag == 0)//запрос версии ПО
    {
        can.flag_ver_req = 0;
        can.tx_flag = 1;
        HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader_ver, can.tx_buff_ver);
    }
    if(can.flag_dir)
    {
		  can.flag_dir=0;

		  can.active_flag=1;//флаг активации обмена служит для отключениея обновления руда по 485

		  if(can.shod_count>10)
			  {

			  ADG_ON;

			  if(mtm.engine_state==0)
			  {
				  mtm.update_state_flag=1;
				  mtm.engine_state=FLY_START_1;

				  mtm.engine_flag=0;
				  flash.tm_on_flag=1;

			  }

			  if (can.start_count>5)
				  {

				  	  can.activate_prs_flag=1;



				  }
			  else if ((can.SU&0b00000001))
				  {

				  	  can.start_count++;

				  }
			  }

		  else if(can.RK4&1)
		  	  {
			  	  can.shod_count++;
		  	  }
		  if ((can.Rud<=100)&&(rs.config_complate_flag))
			  {
			  	  mtm.trotle=(can.Rud*10);
			  	  mtm.rud=(config.n1_min*10)+(((config.n1_max-config.n1_min)*mtm.trotle)/100);
			  }

		  }

 }




void CAN_Timer_1_Hz()
{
    if(can.req_count > 5)
    {
        if(can.tx_tm_buff_numb == 0)
        {
            if(can.tx_flag == 0)
            {
                can.tx_tm_buff_numb++;
                CAN_TM_Complate();
                can.tx_flag = 1;
                TxHeader_tm.Identifier = REQ_TM_FRAME_ID;
                HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader_tm, can.tx_buff_tm);
            }
        }
        else
        {
            if(can.tx_flag == 0)
            {
                can.tx_tm_buff_numb = 0;
                CAN_TM_Complate();
                can.tx_flag = 1;
                TxHeader_tm.Identifier = REQ_TM2_FRAME_ID;
                HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader_tm, can.tx_buff2_tm);
            }
        }
    }
    else
    {
        can.req_count++;
    }
}



 void CAN_TM_Complate()
 {
	 can.rotor_freq = mtm.rotor_speed / 2;
	 can.temp=mtm.t_real;
	 can.trot=mtm.trotle/10;
	 can.pump_freq=mtm.pump_speed;
	 //can.bar=hc206s.p;
	 can.t_gen=mtm.t_gen;
	 can.state=mtm.engine_state;
	 can.pump_pwm=PUMP_PWM/10;
	 can.starter_pwm=STARTER_PWM/10;
	 can.err=mtm.engine_flag;

	 CAN_SS_Complate();



	  can.tx_buff_tm[0]=can.ss;

	  can.tx_buff_tm[1]=(can.temp&0xFF);
	  can.tx_buff_tm[2]=can.temp>>8;

	  can.tx_buff_tm[3]=can.trot;

	  can.tx_buff_tm[4]=(can.rotor_freq&0xFF);
	  can.tx_buff_tm[5]=(can.rotor_freq>>8);

	  can.tx_buff_tm[6]=(can.pump_freq&0xFF);
	  can.tx_buff_tm[7]=(can.pump_freq>>8);





	  can.tx_buff2_tm[0]=(can.bar&0xFF);
	  can.tx_buff2_tm[1]=(can.bar>>8);

	  can.tx_buff2_tm[2]=can.t_gen;

	  can.tx_buff2_tm[3]=can.state;

	  can.tx_buff2_tm[4]=(can.starter_pwm);

	  can.tx_buff2_tm[5]=(can.pump_pwm);

	  can.tx_buff2_tm[6]=(can.err&0xFF);
	  can.tx_buff2_tm[7]=(can.err>>8);

 }


 void CAN_SS_Complate()
 {

	switch (can.ss_complate_state)
	  {
		  case(0):
				if((can.vsk_counter_sec>20)&&((mtm.engine_state==9)||(can.err)))
				{
					can.err_flag=1;
					  can.ss=(can.ss|1<<4);
					  can.ss=(can.ss|1<<5);
				}

				else if((can.err==0)&&(can.state==0))
		        {
		        	can.up_state_flag=1;
		        	can.ss_complate_state=1;
		        }


		  break;

		  case(1)://ожидание


			 if(can.up_state_flag)
			 {
				 can.up_state_flag=0;

				  can.ss=(can.ss|1<<0);
				  can.ss=(can.ss|1<<1);
				  can.ss=(can.ss|1<<3);
			 }

		  	 if(can.err)
		  	 {
		  		 can.engine_flag=1;
		  		 can.ss=(can.ss&(~(1<<0)));
		  		 can.ss=(can.ss&(~(1<<1)));
		  		 can.ss=(can.ss&(~(1<<3)));
				  can.ss=(can.ss|1<<4);
				  can.ss=(can.ss|1<<5);

		  	 }
		  	 else
		  	 {
		  		can.engine_flag=0;
		  		can.ss=(can.ss|1<<0);
			    can.ss=(can.ss|1<<1);
			    can.ss=(can.ss|1<<3);
				 can.ss=(can.ss&(~(1<<4)));
				 can.ss=(can.ss&(~(1<<5)));

			    if(can.shod_count>10)
			    {
			    	can.up_state_flag=1;
			    	can.ss_complate_state=2;
			    }
		  	 }

		  break;

		  case(2)://готовность
			 if(can.up_state_flag)
			 {
				 can.start_count=0;
				 can.up_state_flag=0;
				  can.ss=(can.ss|1<<0);
				  can.ss=(can.ss|1<<1);
				  can.ss=(can.ss&(~(1<<3)));
				  can.ss=(can.ss|1<<4);

			 }
			 if(can.err)
			 {
				 can.engine_flag=1;
				 can.ss=(can.ss&(~(1<<0)));
				 can.ss=(can.ss&(~(1<<1)));


			 }
		  	 else
				  	 {
				  		can.engine_flag=0;
				  		can.ss=(can.ss|1<<0);
					    can.ss=(can.ss|1<<1);



				  	 }

			 if(can.start_count>5)
			 {
			    	can.up_state_flag=1;
			    	can.ss_complate_state=3;
			 }


         break;

		  case(3)://запуск
				 if(can.up_state_flag)
					 {
						 can.up_state_flag=0;
						  can.ss=(can.ss|1<<0);
						  can.ss=(can.ss|1<<1);
						  can.ss=(can.ss|1<<3);
						  can.ss=(can.ss|1<<4);


					 }
					 if(can.err)
					 {
						 can.engine_flag=1;
						 can.ss=(can.ss&(~(1<<0)));
						 can.ss=(can.ss&(~(1<<1)));

					 }
					 else if(can.engine_flag)
					 {
						 can.engine_flag=0;
					  		can.ss=(can.ss|1<<0);
							    can.ss=(can.ss|1<<1);
					 }

					 if(can.state==16)
					 {
						 can.up_state_flag=1;
						 can.ss_complate_state=4;
					 }


         break;

		  case(4)://работа
				if(can.up_state_flag)
				 {
					 can.up_state_flag=0;
					  can.ss=(can.ss|1<<0);
					  can.ss=(can.ss|1<<1);
					  can.ss=(can.ss&(~(1<<3)));
					  can.ss=(can.ss&(~(1<<4)));
					  can.ss=(can.ss|1<<5);


				 }

			 if(can.err)
			 {
				 can.engine_flag=1;
				 can.ss=(can.ss&(~(1<<0)));
				 can.ss=(can.ss&(~(1<<1)));

			 }
		  	 else
			 {
				can.engine_flag=0;
				can.ss=(can.ss|1<<0);
				can.ss=(can.ss|1<<1);



			 }

			 if(can.state!=16)
			 {
				 can.up_state_flag=1;
				 can.ss_complate_state=5;
			 }

         break;

		  case(6)://останов
			if(can.up_state_flag)
			 {
				 can.up_state_flag=0;
				  can.ss=(can.ss|1<<0);
				  can.ss=(can.ss|1<<1);
				  can.ss=(can.ss|1<<3);
				  can.ss=(can.ss&(~(1<<4)));
				  can.ss=(can.ss|1<<5);


			 }
			 if(can.err)
			 {
				 can.engine_flag=1;
				 can.ss=(can.ss&(~(1<<0)));
				 can.ss=(can.ss&(~(1<<1)));

			 }
         break;
	  }
 }
