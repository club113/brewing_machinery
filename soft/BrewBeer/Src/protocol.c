
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


unsigned char upload_device_info(char* parameter)
{
	P_S_WifiFrame wifi_frame =NULL;	
	wifi_frame = (P_S_WifiFrame)parameter;
	CheckDeviceID(wifi_frame);
	
		
	S_WifiFrame msg;
	msg.data_length = 18;
	msg.format_control.fc.encrypt = 0;
	msg.format_control.fc.version = 0;
	msg.function = GETDEVICEINFO;	
	SetTimeStampIntoArrary(msg.arg+14);
	UploadDataByWifi(&msg);
	return 1;
}

unsigned char get_server_time(char* parameter)
{
	P_S_WifiFrame wifi_frame =NULL;	
	wifi_frame = (P_S_WifiFrame)parameter;
	CheckDeviceID(wifi_frame);
		
	S_WifiFrame msg;
	msg.data_length = 4;
	msg.format_control.fc.encrypt = 0;
	msg.format_control.fc.version = 0;
	msg.function = GETSERVERTIME;	
	SetTimeStampIntoArrary(msg.arg);
	UploadDataByWifi(&msg);
	return 1;
}


unsigned char get_device_id(char* parameter)
{
	P_S_WifiFrame wifi_frame =NULL;	
	wifi_frame = (P_S_WifiFrame)parameter;
	CheckDeviceID(wifi_frame);
		
	S_WifiFrame msg;
	msg.data_length = 4;
	msg.format_control.fc_value = 1;
	msg.function = CALLID;	
	SetTimeStampIntoArrary(msg.arg);
	UploadDataByWifi(&msg);
	return 1;
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
	
	data_length = *(data+2);// ȡ������֡�еı�ʾ���ݳ��ȵ���Ϣ
	data_length <<=8;
	data_length = *(data+3);

	if(data_length != recv_data_length)
		{
			return 2;//�յ������ݸ�ʽ����
		}

	
	unsigned char check_sum = 0;
	for(;loopx < (recv_data_length-1); loopx ++)
		{
			check_sum += *(data + loopx);
		}

	if(check_sum != *(data + (recv_data_length-1)))
		{
			return 3;//У��ʧ��
		}
	*res_data_length = data_length - 15;//��������������
	return 1;//�յ�һ���Ϸ�����
}


unsigned char WifiDeCode(unsigned char* data,unsigned short recv_data_length,P_S_WifiFrame wifi_frame)
{
	unsigned short data_length = 0;
	if(1 != CheckData(data,recv_data_length,&data_length))
		{
			return 2;//����֡����
		}

	wifi_frame->data_length = data_length;
	
	wifi_frame->format_control.fc_value = *(data+4);
	memcpy(wifi_frame->DeviceID,(data+5),8);
	wifi_frame->function = *(data+13);
	if(15 != data_length)//��������Ϊ��
		{
			memcpy(wifi_frame->arg,(data+14),(data_length));
		}
	return 1;
}

void SetTimeStampIntoArrary(unsigned char* insert_point)
{
	long int time = 0;
	
	time = GetDeviceTime();
	*(insert_point) = (time>>24)&0XFF;
	*(insert_point+1) = (time>>16)&0XFF;
	*(insert_point+2) = (time>>8)&0XFF;
	*(insert_point+3) = (time)&0XFF;
}

long int GetTimeStampFromArrary(unsigned char* draw_point)
{
	long int time = 0;
	time |= *(draw_point);
	time <<= 8;
	time |= *(draw_point+1);
	time <<= 8;
	time |= *(draw_point+2);
	time <<= 8;
	time |= *(draw_point+3);
	return time;
	
}

void SetDeviceIDToWifFrame(P_S_WifiFrame wifi_frame)
{
	memcpy(wifi_frame->DeviceID,MachineInfo.DeviceID,8);
}

HAL_StatusTypeDef UploadDataByWifi(P_S_WifiFrame wifi_frame)
{
	unsigned short frame_length = 15 + wifi_frame->data_length;
	
	SetDeviceIDToWifFrame(wifi_frame);
	unsigned short CheckLoop = 0;
	WifiOperatData.Tx_data[0] = 0X55;
	WifiOperatData.Tx_data[1] = 0XAA;
	WifiOperatData.Tx_data[2] = (frame_length >> 8)&0XFF;
	WifiOperatData.Tx_data[3] = frame_length&0XFF;
	WifiOperatData.Tx_data[4] = wifi_frame->format_control.fc_value;
	WifiOperatData.Tx_data[frame_length - 1] = 0;

	memcpy(&(WifiOperatData.Tx_data[5]),wifi_frame->DeviceID,8);

	WifiOperatData.Tx_data[13] = wifi_frame->function;
	memcpy(&(WifiOperatData.Tx_data[14]),wifi_frame->arg,(wifi_frame->data_length));

	
	for(;CheckLoop < (frame_length - 1); CheckLoop ++)
		{
			WifiOperatData.Tx_data[frame_length - 1] += WifiOperatData.Tx_data[CheckLoop];
		}
	return WifiSendData(WifiOperatData.Tx_data,frame_length);
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
{GETDEVICEINFO,upload_device_info,"upload device info"},//�豸�ϴ�״̬��Ϣ
{GETSERVERTIME,get_server_time,"get time from server"},//�豸���������ʱ��


{CALLID,get_device_id,"get device id"},//��������ȡ����ID
{SETDATE,set_device_date,"set device  date"},//�����豸ʱ��
{CTRLVALUE,get_device_id,"ctrl value"},//���Ƴ��Ʒ�
{SETPURCHASEVOLUME,set_device_date,"set device  date"},//���ù���Ƶ�����
{CTRLMACHINE,get_device_id,"get device id"},//������ƻ�
{GETFERMENTORID,set_device_date,"set device  date"},//��ȡ���͹�ID
{GETFERMENTORINFO,get_device_id,"get device id"},//��ȡָ�����͹޵���Ϣ
{SETUPLOADSETP,set_device_date,"set device  date"},//���ö�ʱ�ϴ�ʱ����
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

