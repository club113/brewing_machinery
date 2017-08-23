#include "wifi.h"

/*void A_T_Field(void)
{
	data_from_wifi.at_ctrl_model= A_T_FIELD_START;
	unsigned char buf;
	buf = '+';
	HAL_UART_Transmit(&huart1,&buf,1,100);
	osDelay(150);
	HAL_UART_Transmit(&huart1,&buf,1,100);
	osDelay(150);
	HAL_UART_Transmit(&huart1,&buf,1,100);
	osDelay(800);
	buf = 'a';
	HAL_UART_Transmit(&huart1,&buf,1,100);
	
}


void Quit_A_T_Field(void)
{
	A_T_Command("AT+ENTM");
	clear_rx_buf();
	data_from_wifi.at_ctrl_model = A_T_FIELD_STOP;
}

void A_T_Command(char* command)
{
	unsigned loopx;
	unsigned short command_lenth = string_length(command);
	char** p_command;
	p_command = &command;
	for(loopx = 0 ; loopx < command_lenth; loopx++)
		{
			HAL_UART_Transmit(&huart1,(unsigned char*)((*p_command)++),1,100);
			osDelay(200);
		}
	clear_rx_buf();
	HAL_UART_Transmit_DMA(&huart1,"\r\n",2);
}

unsigned char A_T_Set_SockA_Server(unsigned char mode,unsigned char ip1,unsigned char ip2,unsigned char ip3,unsigned char ip4,unsigned int port)
{
	unsigned char times = 5;
	time.x_step_1_s = 0;
	char send_buf[50];
	A_T_Field();
	osDelay(1000);
	A_T_Command("AT+NETP");	
	osDelay(1000);
	if(mode == 1)
		{
			sprintf(send_buf,"AT+NETP=TCP,CLIENT,%d,%d.%d.%d.%d",port,ip1,ip2,ip3,ip4);
		}
	if(mode == 0)
		{
			sprintf(send_buf,"AT+NETP=UDP,CLIENT,%d,%d.%d.%d.%d",port,ip1,ip2,ip3,ip4);
		}
	A_T_Command(send_buf);
	
	osDelay(1000);
	while((data_from_wifi.data[4] != 0X6F)&&(times--))
		{
			HAL_UART_Transmit_DMA(&huart1,"\r\n",2);
			osDelay(200);
			A_T_Command(send_buf);
		}
	Quit_A_T_Field();
	
	if(times ==0)
		{
			return 0;
		}
	else
		{
			return 1;
		}
}

unsigned char A_T_Set_SockB_Server(unsigned char mode,unsigned char ip1,unsigned char ip2,unsigned char ip3,unsigned char ip4,unsigned int port)
{
	unsigned char times = 5;
	time.x_step_1_s = 0;
	char send_buf[50];
	A_T_Field();
	osDelay(1000);
	A_T_Command("AT+SOCKB");	
	osDelay(1000);
	if(mode == 1)
		{
			sprintf(send_buf,"AT+SOCKB=TCP,%d,%d.%d.%d.%d",port,ip1,ip2,ip3,ip4);
		}
	if(mode == 0)
		{
			sprintf(send_buf,"AT+SOCKB=UDP,%d,%d.%d.%d.%d",port,ip1,ip2,ip3,ip4);
		}
	A_T_Command(send_buf);
	osDelay(1000);
	while((data_from_wifi.data[4] != 0X6F)&&(times--))
		{
			HAL_UART_Transmit_DMA(&huart1,"\r\n",2);
			osDelay(200);
			A_T_Command(send_buf);
		}
	Quit_A_T_Field();
	if(times ==0)
		{
			return 0;
		}
	else
		{
			return 1;
		}
}

void upload_learn_data(unsigned char command,unsigned char* data,P_S_Save_Learned_Device_Data save_data)
{
	unsigned char loop8,loopx;
	loopx = 0;
	for(loop8 = 0;loop8 < 8;loop8 ++)
		{
			if(save_data ->device[loop8].tcm300_ID[4] == 0x55)
				{
					*(data + loopx *4+ 0) = save_data ->device[loop8].tcm300_ID[0];
					*(data + loopx *4+ 1) = save_data ->device[loop8].tcm300_ID[1];
					*(data + loopx *4+ 2) = save_data ->device[loop8].tcm300_ID[2];
					*(data + loopx *4+ 3) = save_data ->device[loop8].tcm300_ID[3];
					loopx ++;
				}
				
		}
	upload_data_by_wifi(command,data,loopx*4);
}




void wifi_set_server(void)
{
	unsigned int tmp_port = 0;
	tmp_port =data_from_wifi.data[13];
	tmp_port <<= 8;
	tmp_port |= data_from_wifi.data[14];
	
	switch(data_from_wifi.data[8])
		{
			case 1://socketA
			A_T_Set_SockA_Server(1,data_from_wifi.data[9],data_from_wifi.data[10],data_from_wifi.data[11],data_from_wifi.data[12],tmp_port);
			break;

			case 2://socketB
			A_T_Set_SockB_Server(1,data_from_wifi.data[9],data_from_wifi.data[10],data_from_wifi.data[11],data_from_wifi.data[12],tmp_port);
			break;

			default:
			break;
			
		}
}*/


