#include "wifi.h"


unsigned char G_WifiCtrlFlag;

unsigned char string_length(char* string)
{
	char* tmp = string;
	unsigned char length = 0;
	while(*tmp++)
		{
			length ++; 
		}
	return length;
}



void A_T_Field(void)
{
	G_WifiCtrlFlag = A_T_FIELD_START;
	
	unsigned char buf;
	buf = '+';
	HAL_UART_Transmit(&WIFICOM,&buf,1,100);
	osDelay(150);
	HAL_UART_Transmit(&WIFICOM,&buf,1,100);
	osDelay(150);
	HAL_UART_Transmit(&WIFICOM,&buf,1,100);
	osDelay(800);
	buf = 'a';
	HAL_UART_Transmit(&WIFICOM,&buf,1,100);
	
}

void A_T_Command(char* command)
{
	unsigned loopx;
	unsigned short command_lenth = string_length(command);
	char** p_command;
	p_command = &command;
	for(loopx = 0 ; loopx < command_lenth; loopx++)
		{
			HAL_UART_Transmit(&WIFICOM,(unsigned char*)((*p_command)++),1,100);
			osDelay(200);
		}
	ClearRecvData(&WifiOperatData);
	HAL_UART_Transmit(&WIFICOM,"\r\n",2,100);
}

void Quit_A_T_Field(void)
{
	A_T_Command("AT+ENTM");
	ClearRecvData(&WifiOperatData);
	G_WifiCtrlFlag = A_T_FIELD_START;
}



unsigned char A_T_Set_SockA_Server(unsigned char mode,unsigned char ip1,unsigned char ip2,unsigned char ip3,unsigned char ip4,unsigned int port)
{
	unsigned char times = 5;
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
	while((WifiOperatData.Rx_data[4] != 0X6F)&&(times--))
		{
			HAL_UART_Transmit(&WIFICOM,"\r\n",2,100);
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
	osDelay(200);
	HAL_UART_Transmit(&WIFICOM,"\r\n",2,100);
	osDelay(1000);
	while((WifiOperatData.Rx_data[4] != 0X6F)&&(times--))
		{
			HAL_UART_Transmit(&WIFICOM,"\r\n",2,100);
			osDelay(200);
			A_T_Command(send_buf);
			osDelay(200);
			HAL_UART_Transmit(&WIFICOM,"\r\n",2,100);			
			osDelay(200);
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


void wifi_set_server(void)
{
	unsigned int tmp_port = 0;
	tmp_port =WifiOperatData.Rx_data[13];
	tmp_port <<= 8;
	tmp_port =WifiOperatData.Rx_data[14];
	
	switch(WifiOperatData.Rx_data[8])
		{
			case 1://socketA
			A_T_Set_SockA_Server(1,WifiOperatData.Rx_data[9],WifiOperatData.Rx_data[10],WifiOperatData.Rx_data[11],WifiOperatData.Rx_data[12],tmp_port);
			break;

			case 2://socketB
			A_T_Set_SockB_Server(1,WifiOperatData.Rx_data[9],WifiOperatData.Rx_data[10],WifiOperatData.Rx_data[11],WifiOperatData.Rx_data[12],tmp_port);
			break;

			default:
			break;
			
		}
}


