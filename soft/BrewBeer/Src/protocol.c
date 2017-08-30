
#include "protocol.h"

unsigned char CheckDeviceID(P_S_WifiFrame wifi_frame)
{
	unsigned char loop8 = 0;
	for(;loop8 < 8; loop8++)
		{
			if((wifi_frame->DeviceID[loop8]) != (MachineInfo.DeviceID[loop8]))
				{
					return 0;
				}
		}
	return 1;
}


unsigned char get_device_id(char* parameter)
{
	P_S_WifiFrame wifi_frame =NULL;
	wifi_frame = (P_S_WifiFrame)parameter;
	CheckDeviceID(wifi_frame);
	return 0;
}

unsigned char get_temp(char* parameter)
{
	P_S_WifiFrame wifi_frame =NULL;
	wifi_frame = (P_S_WifiFrame)parameter;
	if(0 == CheckDeviceID(wifi_frame))
		{
			return 0;
		}
	return 1;
}


unsigned char get_device_info(char* parameter)
{
	P_S_WifiFrame wifi_frame =NULL;
	wifi_frame = (P_S_WifiFrame)parameter;
	if(0 == CheckDeviceID(wifi_frame))
		{
			return 0;
		}
	return 0;
}

unsigned char set_device_date(char* parameter)
{
	P_S_WifiFrame wifi_frame =NULL;
	wifi_frame = (P_S_WifiFrame)parameter;
	if(0 == CheckDeviceID(wifi_frame))
		{
			return 0;
		}
	return 0;
}

HAL_StatusTypeDef WifiSendData(unsigned char* pTxData, unsigned short Size)
{
	return HAL_UART_Transmit(&WIFICOM,pTxData,Size,200);
}

HAL_StatusTypeDef Rs4851SendData(unsigned char* pTxData, unsigned short Size)
{
	HAL_StatusTypeDef RES;
	ENABLE_RS485_1_TX;
	RES = HAL_UART_Transmit(&RS485COM1,pTxData,Size,200);
	ENABLE_RS485_1_RX;
	return RES;
}

HAL_StatusTypeDef Rs4852SendData(unsigned char* pTxData, unsigned short Size)
{
	HAL_StatusTypeDef RES;
	ENABLE_RS485_2_TX;
	RES = HAL_UART_Transmit(&RS485COM2,pTxData,Size,200);
	ENABLE_RS485_2_RX;
	return RES;
}

HAL_StatusTypeDef Rs485NetSendData(unsigned char* pTxData, unsigned short Size)
{
	return HAL_UART_Transmit(&RS485NETCOM,pTxData,Size,200);
}


unsigned char CheckData(unsigned char* data,unsigned short recv_data_length,unsigned short* res_data_length)
{
	unsigned short loopx = 0;
	unsigned short data_length = 0;
	
	data_length = *(data+2);// 取出数据帧中的表示数据长度的信息
	data_length <<=8;
	data_length = *(data+3);

	if(data_length != recv_data_length)
		{
			return 2;//收到的数据格式错误
		}

	
	unsigned char check_sum = 0;
	for(;loopx < (recv_data_length-1); loopx ++)
		{
			check_sum += *(data + loopx);
		}

	if(check_sum != *(data + (recv_data_length-1)))
		{
			return 3;//校验失败
		}
	*res_data_length = data_length;
	return 1;//收到一条合法数据
}


unsigned char WifiDeCode(unsigned char* data,unsigned short recv_data_length,P_S_WifiFrame wifi_frame)
{
	unsigned short data_length = 0;
	if(1 != CheckData(data,recv_data_length,&data_length))
		{
			return 2;//数据帧错误
		}

	wifi_frame->data_length = data_length;
	
	wifi_frame->format_control.fc_value = *(data+4);
	memcpy(wifi_frame->DeviceID,(data+5),8);
	wifi_frame->function = *(data+13);
	if(15 != data_length)//数据区不为空
		{
			memcpy(wifi_frame->arg,(data+14),(data_length -15));
		}
	return 1;
}



void SetDeviceIDToWifFrame(P_S_WifiFrame wifi_frame)
{
	memcpy(wifi_frame->DeviceID,MachineInfo.DeviceID,8);
}

HAL_StatusTypeDef UploadDataByWifi(P_S_WifiFrame wifi_frame)
{
	SetDeviceIDToWifFrame(wifi_frame);
	unsigned short CheckLoop = 0;
	WifiOperatData.Tx_data[0] = 0X55;
	WifiOperatData.Tx_data[1] = 0XAA;
	WifiOperatData.Tx_data[2] = (wifi_frame->data_length >> 8)&0XFF;
	WifiOperatData.Tx_data[3] = (wifi_frame->data_length)&0XFF;
	WifiOperatData.Tx_data[4] = wifi_frame->format_control.fc_value;
	WifiOperatData.Tx_data[wifi_frame->data_length - 1] = 0;

	memcpy(&(WifiOperatData.Tx_data[5]),wifi_frame->DeviceID,8);

	WifiOperatData.Tx_data[13] = wifi_frame->function;
	memcpy(&(WifiOperatData.Tx_data[14]),wifi_frame->arg,(wifi_frame->data_length - 15));
	for(;CheckLoop < (wifi_frame->data_length - 1); CheckLoop ++)
		{
			WifiOperatData.Tx_data[wifi_frame->data_length - 1] += WifiOperatData.Tx_data[CheckLoop];
		}
	return WifiSendData(WifiOperatData.Tx_data,wifi_frame->data_length);
}

HAL_StatusTypeDef UploadDataByRs485_1(P_S_WifiFrame wifi_frame)
{
	SetDeviceIDToWifFrame(wifi_frame);
	unsigned short CheckLoop = 0;
	WifiOperatData.Tx_data[0] = 0X55;
	WifiOperatData.Tx_data[1] = 0XAA;
	WifiOperatData.Tx_data[2] = (wifi_frame->data_length >> 8)&0XFF;
	WifiOperatData.Tx_data[3] = (wifi_frame->data_length)&0XFF;
	WifiOperatData.Tx_data[4] = wifi_frame->format_control.fc_value;
	WifiOperatData.Tx_data[wifi_frame->data_length - 1] = 0;

	memcpy(&(WifiOperatData.Tx_data[5]),wifi_frame->DeviceID,8);

	WifiOperatData.Tx_data[13] = wifi_frame->function;
	memcpy(&(WifiOperatData.Tx_data[14]),wifi_frame->arg,(wifi_frame->data_length - 15));
	for(;CheckLoop < (wifi_frame->data_length - 1); CheckLoop ++)
		{
			WifiOperatData.Tx_data[wifi_frame->data_length - 1] += WifiOperatData.Tx_data[CheckLoop];
		}
	return Rs4851SendData(WifiOperatData.Tx_data,wifi_frame->data_length);
}
HAL_StatusTypeDef UploadDataByRs485_2(P_S_WifiFrame wifi_frame)
{
	SetDeviceIDToWifFrame(wifi_frame);
	unsigned short CheckLoop = 0;
	WifiOperatData.Tx_data[0] = 0X55;
	WifiOperatData.Tx_data[1] = 0XAA;
	WifiOperatData.Tx_data[2] = (wifi_frame->data_length >> 8)&0XFF;
	WifiOperatData.Tx_data[3] = (wifi_frame->data_length)&0XFF;
	WifiOperatData.Tx_data[4] = wifi_frame->format_control.fc_value;
	WifiOperatData.Tx_data[wifi_frame->data_length - 1] = 0;

	memcpy(&(WifiOperatData.Tx_data[5]),wifi_frame->DeviceID,8);

	WifiOperatData.Tx_data[13] = wifi_frame->function;
	memcpy(&(WifiOperatData.Tx_data[14]),wifi_frame->arg,(wifi_frame->data_length - 15));
	for(;CheckLoop < (wifi_frame->data_length - 1); CheckLoop ++)
		{
			WifiOperatData.Tx_data[wifi_frame->data_length - 1] += WifiOperatData.Tx_data[CheckLoop];
		}
	return Rs4852SendData(WifiOperatData.Tx_data,wifi_frame->data_length);
}



S_Cmd cmd_list[] = 
{
{CALLID,get_device_id,"get device id"},
{GETTEMP,get_temp,"get device temp"},
{SETDATE,set_device_date,"set device  date"},
{GETINFO,get_device_info,"get device info"}
};


P_S_Cmd find_function(unsigned char function_code)
{
	unsigned char loopx = 0;
	unsigned char cmd_num = sizeof(cmd_list)/sizeof(cmd_list[0]);
	for(;loopx<cmd_num;loopx++)
		{
			if(function_code == cmd_list[loopx].function_code)
				{
					return &(cmd_list[loopx]);
				}
		}
	return (P_S_Cmd) 0;
}

void SendSuccessCode(unsigned char function)
{
	return;
}

void SendFailedCode(unsigned char function)
{
	return;
}

void ExecuteData(P_S_WifiFrame wifi_frame)
{
	P_S_Cmd ExecuteCode = NULL;
	unsigned char res = 0;
	ExecuteCode = find_function(wifi_frame->function);
	if(ExecuteCode)
		{
			res = ExecuteCode->function((char*)wifi_frame);
		}
	if(res)
		{
			SendSuccessCode(wifi_frame->function);
		}
	else
		{
			SendFailedCode(wifi_frame->function);
		}
}

void DealWifiData(void)
{
	S_WifiFrame WifiFrame = {0};
	if(1 == WifiDeCode(WifiOperatData.Rx_data,WifiOperatData.recv_data_length,&WifiFrame))
		{
			ExecuteData(&WifiFrame);
		}
}

