#include "iap.h"


APP_INFORMATION App_info;
S_Wifi_Iap_Info wifi_iap_info;
IAP vIAP;


typedef  void (*pFunction)(void);
pFunction JumpToApplication;
uint32_t JumpAddress;

void test_jump(void)
{
	HAL_Init();	
	disable_all_irq();
	SystemClock_Config();
	if (((*(__IO uint32_t*)APP_2_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
		{
			/* Jump to user application */
			JumpAddress = *(__IO uint32_t*) (APP_2_ADDRESS + 4);
			JumpToApplication = (pFunction) JumpAddress;
			/* Initialize user application's Stack Pointer */
			__set_MSP(*(__IO uint32_t*) APP_2_ADDRESS);
			JumpToApplication();
		} 
}

void jump_to_app(unsigned int app_addr)
{
	//HAL_Init();
	//HAL_RCC_DeInit();
	//SystemClock_Config();	
	//disable_all_irq();
	if (((*(__IO uint32_t*)app_addr) & 0x2FFE0000 ) == 0x20000000)
		{
			/* Jump to user application */
			JumpAddress = *(__IO uint32_t*) (app_addr + 4);
			JumpToApplication = (pFunction) JumpAddress;
			/* Initialize user application's Stack Pointer */
			__set_MSP(*(__IO uint32_t*) app_addr);
			JumpToApplication();
		} 	
}


void disable_all_irq(void)
{
	unsigned char loopx;
	for(loopx = 0;loopx< 60;loopx ++)
		{			
			HAL_NVIC_DisableIRQ((IRQn_Type)loopx);
			NVIC_ClearPendingIRQ((IRQn_Type)loopx);
		}
}

void try_to_jump_to_next_app(void)
{
	try_to_jump(NEXT_APP);
}
	

void try_to_jump(unsigned char app)
{
	switch(app)
		{
			case APP_BASE:
			App_info.try_to_jump = APP_BASE_ADDRESS;
			App_info.app_offset_table[0] = 0;
			break;

			case APP_1:
			App_info.try_to_jump = APP_1_ADDRESS;	
			App_info.app_offset_table[1] = 0;
			break;

			case APP_2:
			App_info.try_to_jump = APP_2_ADDRESS;	
			App_info.app_offset_table[2] = 0;
			break;

			//case APP_3:
			//App_info.try_to_jump = APP_3_ADDRESS;
			//App_info.app_offset_table[3] = 0;
			//break;
			
			default :
			App_info.try_to_jump = APP_BASE_ADDRESS;
			App_info.app_offset_table[0] = 0;
			break;
		}
	App_info.tem_jump = try_jump_flag;
	write_app_info();
	NVIC_SystemReset();
}

void read_app_info(void)
{
	read_flash(APP_INFOR_SAVE_ADDR, (char *)(&App_info), sizeof(App_info));
}

void write_app_info(void)
{
	write_flash(APP_INFOR_SAVE_ADDR, (char *)(&App_info), sizeof(App_info));
}
void reset_iap_buff(void)
{
	unsigned short loop;
	for(loop = 0;loop<FLASH_PAGE_SIZE;loop++)
		{
			vIAP.Page_buffer[loop] = 0XFF;
		}
}
void choose_app(void)
{
	if((App_info.tem_jump == try_jump_flag)||(App_info.tem_jump == 0XFF))
		{
			App_info.tem_jump = no_jump;
			if(App_info.try_to_jump == THIS_APP_ADDR)
				{
					App_info.jump_to_app = App_info.try_to_jump;
					App_info.try_to_jump = 0X01234567;
					write_app_info();
					return;
				}
			else
				{					
					write_app_info();
					/////jump to try_to_jump app
					jump_to_app(App_info.try_to_jump);
				}
		}
	else
		{
			if((App_info.try_to_jump == APP_BASE_ADDRESS)||(App_info.try_to_jump == APP_1_ADDRESS)||(App_info.try_to_jump == APP_2_ADDRESS)||(App_info.try_to_jump == APP_3_ADDRESS))
				{
					if(App_info.try_to_jump == THIS_APP_ADDR)   //跳转成功
						{
							App_info.jump_to_app = App_info.try_to_jump;
							App_info.try_to_jump = 0X01234567;
							write_app_info();
							return;
						}
					else		//跳转失败
						{
							App_info.try_to_jump = 0X01234567;
							write_app_info();
							jump_to_app(App_info.jump_to_app);
						}
				}
			if(App_info.jump_to_app == THIS_APP_ADDR)
				{
					if(App_info.jump_to_app == APP_BASE_ADDRESS)
						{
							if(App_info.app_offset_table[0] != App_info.jump_to_app )
								{
									App_info.app_offset_table[0] = App_info.jump_to_app;
									write_app_info();
								}
						}
					if(App_info.jump_to_app == APP_1_ADDRESS)
						{
							if(App_info.app_offset_table[1] != App_info.jump_to_app )
								{
									App_info.app_offset_table[1] = App_info.jump_to_app;
									write_app_info();
								}
						}
					if(App_info.jump_to_app == APP_2_ADDRESS)
						{
							if(App_info.app_offset_table[2] != App_info.jump_to_app )
								{
									App_info.app_offset_table[2] = App_info.jump_to_app;
									write_app_info();
								}
						}
					if(App_info.jump_to_app == APP_3_ADDRESS)
						{
							if(App_info.app_offset_table[3] != App_info.jump_to_app )
								{
									App_info.app_offset_table[3] = App_info.jump_to_app;
									write_app_info();
								}
						}
					
					return;
				}
			if((App_info.jump_to_app != APP_BASE_ADDRESS)&&(App_info.jump_to_app != APP_1_ADDRESS)&&(App_info.jump_to_app != APP_2_ADDRESS)&&(App_info.jump_to_app != APP_3_ADDRESS)) //修改跳转bug

				{	
					App_info.jump_to_app = THIS_APP_ADDR;
					write_app_info();
					return;
				}
			jump_to_app(App_info.jump_to_app);
		}
	return;
}






void ClearRetryFlag(void)
{
	wifi_iap_info.need_retry = 0;
	wifi_iap_info.timer_set_flag = 0;
}

void WifiIapStopRetryFunction(void)
{
	if(1 == wifi_iap_info.start_retry)
		{
			wifi_iap_info.start_retry = 0;//关闭超时重传
			timer_free(wifi_iap_info.retry_timer);
		}
}



void RetryFunction(void* arg)
{
	if(1 == wifi_iap_info.timer_set_flag)
		{
			if(1 == wifi_iap_info.need_retry)
				{
					if(0 == wifi_iap_info.retry_times)
						{
							WifiIapStopRetryFunction();
							return;
						}
					wifi_iap_info.retry_times --;
					wifi_iap_request_page(wifi_iap_info.request_page);
				}
			wifi_iap_info.need_retry = 1;
		}
	wifi_iap_info.timer_set_flag = 1;
}

void WifiIapStartRetryFunction(unsigned short retry_interval,unsigned char retry_time)
{
	wifi_iap_info.retry_times = retry_time;
	wifi_iap_info.retry_timer = register_value_into_timer(retry_interval,1);
	if(0XFF != wifi_iap_info.retry_timer)
		{
			wifi_iap_info.start_retry = 1;//启动超时重传标志位
			register_callback_function_into_timer(wifi_iap_info.retry_timer,RetryFunction);
		}
}




void init_save_data(void)
{
	//reset_iap_buff();
	read_app_info();
	if(App_info.data_effect != 0X88)//0X88作为标志位,表示之前保存过数据,数据有效
		{	
			App_info.data_effect = 0X88;
			App_info.tem_jump= no_jump;
			App_info.jump_to_app = THIS_APP_ADDR;
			
			App_info.app_offset_table[0] = 0;
			App_info.app_offset_table[1] = 0;
			App_info.app_offset_table[2] = 0;
			App_info.app_offset_table[3] = 0;
			App_info.cur_app_offset= App_info.app_offset_table[App_info.app_point] ;
			write_app_info();
		}
	
}

void start_app(void)
{
	init_save_data();
	choose_app();
}


void upload_data_by_wifi(unsigned char comand,unsigned char *data,unsigned char length)
{
	unsigned char loopx;
	WifiOperatData.Tx_data[0] = 0XAA;
	WifiOperatData.Tx_data[1] = 0X55;
	WifiOperatData.Tx_data[2]= length +9;
	WifiOperatData.Tx_data[3] =  MachineInfo.DeviceID[0];
	WifiOperatData.Tx_data[4] =  MachineInfo.DeviceID[1];
	WifiOperatData.Tx_data[5] =  MachineInfo.DeviceID[2];
	WifiOperatData.Tx_data[6] =  MachineInfo.DeviceID[3];
	WifiOperatData.Tx_data[7] = 0X01;
	WifiOperatData.Tx_data[8] = length +2;
	WifiOperatData.Tx_data[9] = comand;
	for(loopx = 0;loopx < length;loopx ++)
		{
			WifiOperatData.Tx_data[10+loopx] = *(data+loopx);
		}
	
	SetCRC8Sub(&WifiOperatData.Tx_data[0],(10+length));
	WifiSendData(WifiOperatData.Tx_data, (11+length));
}

void wifi_iap_cmd_operat(P_S_Wifi_Iap_Cmd cmd_parameter)
{
	upload_data_by_wifi(0XF1,(unsigned char *)cmd_parameter,6);
}

void wifi_iap_request_page(unsigned int page)
{
	ClearRetryFlag();
	//unsigned char page_num[2] = {0};
	//page_num[1] = page&0X000000FF;
	//page_num[0] = (page>>8)&0X000000FF;
	//upload_data_by_wifi(0XF0,page_num,2);
	unsigned short tmp_page = page;
	WifiOperatData.Tx_data[0] = 0XA5;
	WifiOperatData.Tx_data[1] = 0X5A;
	WifiOperatData.Tx_data[2] = 0X00;
	WifiOperatData.Tx_data[3] = 0X0B;
	WifiOperatData.Tx_data[4] = 0X00;
	WifiOperatData.Tx_data[5] = 0X00;
	WifiOperatData.Tx_data[6] = 0X03;
	WifiOperatData.Tx_data[7] = 0XF2;
	WifiOperatData.Tx_data[8] = (tmp_page>>8)&0XFF;
	WifiOperatData.Tx_data[9] = (tmp_page)&0XFF;
	
	SetCRC8Sub(&WifiOperatData.Tx_data[0],10);
	
	WifiSendData(WifiOperatData.Tx_data, 11);
}

void wifi_iap_send_ready_code(void)
{
	//S_Wifi_Iap_Cmd send_cmd_parameter = {0};
	//send_cmd_parameter.cmd = WIFI_IAP_OPERAT_START_BACK;
	//send_cmd_parameter.parameter_1 = THIS_APP >> 4;
	//wifi_iap_cmd_operat(&send_cmd_parameter);

	WifiOperatData.Tx_data[0] = 0XA5;
	WifiOperatData.Tx_data[1] = 0X5A;
	WifiOperatData.Tx_data[2] = 0X00;
	WifiOperatData.Tx_data[3] = 0X0B;
	WifiOperatData.Tx_data[4] = 0X00;
	WifiOperatData.Tx_data[5] = 0X00;
	WifiOperatData.Tx_data[6] = 0X03;
	WifiOperatData.Tx_data[7] = 0XF0;
	WifiOperatData.Tx_data[8] = (DEVICE_SOFT_VER>>8)&0XFF;
	WifiOperatData.Tx_data[9] = (DEVICE_SOFT_VER)&0XFF;
	
	SetCRC8Sub(&WifiOperatData.Tx_data[0],10);
	
	WifiSendData(WifiOperatData.Tx_data, 11);
}

void wifi_iap_send_end_code(void)
{
	//S_Wifi_Iap_Cmd send_cmd_parameter = {0};
	//send_cmd_parameter.cmd = WIFI_IAP_OPERAT_END;
	//send_cmd_parameter.parameter_1 = THIS_APP >> 4;
	//wifi_iap_cmd_operat(&send_cmd_parameter);
	WifiOperatData.Tx_data[0] = 0XA5;
	WifiOperatData.Tx_data[1] = 0X5A;
	WifiOperatData.Tx_data[2] = 0X00;
	WifiOperatData.Tx_data[3] = 0X09;
	WifiOperatData.Tx_data[4] = 0X00;
	WifiOperatData.Tx_data[5] = 0X00;
	WifiOperatData.Tx_data[6] = 0X01;
	WifiOperatData.Tx_data[7] = 0XF3;
	
	SetCRC8Sub(&WifiOperatData.Tx_data[0],8);
	
	WifiSendData(WifiOperatData.Tx_data, 9);
}

void wifi_iap_operater(void)
{
	//unsigned char operat_type;
	
	/*if(WifiOperatData.Rx_data[7]==0XF1) //iap_cmd
		{
			switch(WifiOperatData.Rx_data[8])
				{
					case WIFI_IAP_OPERAT_START:
						wifi_iap_send_ready_code();
						break;
						
					case WIFI_IAP_OPERAT_BEGIN:
						wifi_iap_info.total_size = 0;
						wifi_iap_info.recved_page_sum = 0;
						wifi_iap_info.request_page = 0;
						wifi_iap_info.block = 0;
						wifi_iap_info.page = 0;
						
						wifi_iap_info.total_size |= WifiOperatData.Rx_data[9];
						wifi_iap_info.total_size <<= 8;
						wifi_iap_info.total_size |= WifiOperatData.Rx_data[10];
						
						wifi_iap_request_page(0);
						break;

					case WIFI_IAP_OPERAT_TRANSMISSION:
						break;

					case WIFI_IAP_OPERAT_END:
						break;
				}

		}*/

	if(WifiOperatData.Rx_data[7]==0XF0)//START
		{
			wifi_iap_send_ready_code();
		}	

	if(WifiOperatData.Rx_data[7]==0XF1)//READY
		{
			wifi_iap_info.total_size = 0;
			wifi_iap_info.recved_page_sum = 0;
			wifi_iap_info.request_page = 0;
			wifi_iap_info.block = 0;
			wifi_iap_info.page = 0;

			wifi_iap_info.target_version |= WifiOperatData.Rx_data[8];
			wifi_iap_info.target_version <<= 8;
			wifi_iap_info.target_version |= WifiOperatData.Rx_data[9];

			wifi_iap_info.total_size |= WifiOperatData.Rx_data[10];
			wifi_iap_info.total_size <<= 8;
			wifi_iap_info.total_size |= WifiOperatData.Rx_data[11];
			wifi_iap_info.total_size <<= 8;
			wifi_iap_info.total_size |= WifiOperatData.Rx_data[12];
			wifi_iap_info.total_size <<= 8;
			wifi_iap_info.total_size |= WifiOperatData.Rx_data[13];
			wifi_iap_info.total_page = (wifi_iap_info.total_size)/WIFI_IAP_PAGE_SIZE;
			if((wifi_iap_info.total_size)%WIFI_IAP_PAGE_SIZE)
				{
					wifi_iap_info.total_page ++;
				}

			wifi_iap_request_page(0);
			WifiIapStartRetryFunction(1000,5);
		}

	if(WifiOperatData.Rx_data[7]==0XF2)//transfer
		{
			wifi_iap_write_to_flash();
		}
	
	if(WifiOperatData.Rx_data[7]==0XF8)//jump
		{
			if(0 == WifiOperatData.Rx_data[8])
				{					
					try_to_jump(APP_BASE);
				}
			if(1 == WifiOperatData.Rx_data[8])
				{
					try_to_jump(APP_1);				
				}
			if(2 == WifiOperatData.Rx_data[8])
				{
					try_to_jump(APP_2);				
				}
		}

}


void wifi_iap_write_to_flash(void)
{
	vIAP.owner = OWNER_WIFI_IAP_BUFFER;

	wifi_iap_info.recved_page_sum++;
	wifi_iap_info.recved_page =0;
	wifi_iap_info.recved_page |= WifiOperatData.Rx_data[8];
	wifi_iap_info.recved_page <<= 8;
	wifi_iap_info.recved_page |= WifiOperatData.Rx_data[9];
	wifi_iap_info.request_page = wifi_iap_info.recved_page+1;
	wifi_iap_info.cur_page_size = WifiOperatData.recv_data_length- 11;

	wifi_iap_info.page = wifi_iap_info.recved_page % (FLASH_PAGE_SIZE / WIFI_IAP_PAGE_SIZE);
	wifi_iap_info.block = wifi_iap_info.recved_page /(FLASH_PAGE_SIZE / WIFI_IAP_PAGE_SIZE);

	wifi_iap_copy_to_flash();
	
	if(wifi_iap_info.recved_page == wifi_iap_info.total_page- 1)
		{
			if(wifi_iap_info.page)
				{
					//wifi_iap_info.block ++;
					memcopy((char*)&WifiOperatData.Rx_data[10],(char*)&vIAP.Page_buffer[wifi_iap_info.page *WIFI_IAP_PAGE_SIZE],(wifi_iap_info.cur_page_size));
					write_flash(WRITE_TO_ADDR+(wifi_iap_info.block * FLASH_PAGE_SIZE),(char*)vIAP.Page_buffer,FLASH_PAGE_SIZE);
					reset_iap_buff();
				}
			
			vIAP.owner = OWNER_ON_ONE;
			wifi_iap_send_end_code();
		}
	else
		{
			wifi_iap_request_page(wifi_iap_info.request_page);
		}
}

void wifi_iap_copy_to_flash(void)
{	
	if(wifi_iap_info.recved_page < wifi_iap_info.total_page -1)
		{
			if(wifi_iap_info.cur_page_size < WIFI_IAP_PAGE_SIZE)
				{
					wifi_iap_info.request_page = wifi_iap_info.recved_page;
					return;
				}
		}
	memcopy((char*)&WifiOperatData.Rx_data[10],(char*)&vIAP.Page_buffer[wifi_iap_info.page * WIFI_IAP_PAGE_SIZE],(wifi_iap_info.cur_page_size));
	if(wifi_iap_info.page == ((FLASH_PAGE_SIZE / WIFI_IAP_PAGE_SIZE)-1))
		{
			write_flash(WRITE_TO_ADDR+(wifi_iap_info.block * FLASH_PAGE_SIZE),(char*)vIAP.Page_buffer,FLASH_PAGE_SIZE);			
			reset_iap_buff();	
		}
	else
		{
			//memcopy((char*)&data_from_wifi.data[12],(char*)&vIAP.Page_buffer[wifi_iap_info.page *128],(wifi_iap_info.cur_page_size));
		}
	
}




