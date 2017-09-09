#include "protocol.h"

unsigned char CheckDeviceID(P_S_WifiFrame wifi_frame)
{
	if(NULL == wifi_frame)
		{
			return 0;
		}
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
	
		
	S_WifiFrame msg={0};
	msg.data_length = 18;
	msg.format_control.fc.encrypt = 0;
	msg.format_control.fc.version = 0;
	msg.function = GETDEVICEINFO;	
	msg.arg[0] = ((MachineInfo.Temperature1) >> 8)&0XFF;
	msg.arg[1] = ((MachineInfo.Temperature1) >> 0)&0XFF;

	
	msg.arg[3] = ((MachineInfo.Fermentor[0].FermentorID) >> 8)&0XFF;
	msg.arg[4] = ((MachineInfo.Fermentor[0].FermentorID) >> 0)&0XFF;

	
	msg.arg[8] = ((MachineInfo.Fermentor[0].LeftVolume) >> 8)&0XFF;
	msg.arg[9] = ((MachineInfo.Fermentor[0].LeftVolume) >> 0)&0XFF;
	
	SetTimeStampIntoArrary(msg.arg+14);
	UploadDataByWifi(&msg);
	return 1;
}

unsigned char get_server_time(char* parameter)
{
	P_S_WifiFrame wifi_frame =NULL;	
	wifi_frame = (P_S_WifiFrame)parameter;
	CheckDeviceID(wifi_frame);
		
	S_WifiFrame msg={0};
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
		
	S_WifiFrame msg={0};
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
	return 0;
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
	S_WifiFrame msg={0};//�ظ���Ϣ
	long int time_from_server = 0;
	
	wifi_frame = (P_S_WifiFrame)parameter;
	if(0 == CheckDeviceID(wifi_frame)&&(4 != wifi_frame->data_length))
		{
			return 0;//����֡����
		}
	time_from_server = GetTimeStampFromArrary(wifi_frame->arg);
	SetDeviceTime(time_from_server);

	msg.data_length = 4;
	msg.format_control.fc.encrypt = 0;
	msg.format_control.fc.version= 0;
	msg.function = SETDATE;	
	SetTimeStampIntoArrary(msg.arg);
	UploadDataByWifi(&msg);
	return 1;
}

unsigned char ctrl_device_value(char* parameter)
{
	P_S_WifiFrame wifi_frame =NULL;
	S_WifiFrame msg={0};//�ظ���Ϣ
	wifi_frame = (P_S_WifiFrame)parameter;

	S_PLCSendFrame PLCMsg = {0};
	if(0 == CheckDeviceID(wifi_frame)&&(1 != wifi_frame->data_length))
		{
			return 0;//����֡����
		}

	if(0 == wifi_frame->arg[0])
		{
			//�رճ��Ʒ�
		}

	if(1 == wifi_frame->arg[0])
		{
			//�򿪳��Ʒ�
		}

	if(0X11 == wifi_frame->arg[0])
		{
			//����Ļ
			PLCMsg.DeviceAddress = MachineInfo.Fermentor[0].FermentorID;
			PLCMsg.ScreenCtrl = 1;
			CtrlPLC(&PLCMsg);
		}

	if(0X10 == wifi_frame->arg[0])
		{
			//�ر���Ļ
			PLCMsg.DeviceAddress = MachineInfo.Fermentor[0].FermentorID;
			PLCMsg.ScreenCtrl = 0;
			CtrlPLC(&PLCMsg);
		}

	msg.data_length = 5;
	msg.format_control.fc.encrypt = 0;
	msg.format_control.fc.version= 0;
	msg.function = CTRLVALUE;	
	msg.arg[0] = 1;//���͹޳��Ʒ�״̬
	SetTimeStampIntoArrary(msg.arg+1);
	UploadDataByWifi(&msg);
	return 1;
}


unsigned char set_purchase_volume(char* parameter)
{
	P_S_WifiFrame wifi_frame =NULL;
	S_WifiFrame msg={0};//�ظ���Ϣ
	unsigned short purchase_volume = 0;//������������Ԥ������
	
	wifi_frame = (P_S_WifiFrame)parameter;
	if(0 == CheckDeviceID(wifi_frame)&&(2 != wifi_frame->data_length))
		{
			return 0;//����֡����
		}

	purchase_volume |= wifi_frame->arg[0];
	purchase_volume <<= 8;
	purchase_volume |= wifi_frame->arg[1];

	//set_volume(purchase_volume);

	msg.data_length = 4;
	msg.format_control.fc.encrypt = 0;
	msg.format_control.fc.version= 0;
	msg.function = SETPURCHASEVOLUME;
	SetTimeStampIntoArrary(msg.arg);
	UploadDataByWifi(&msg);
	return 1;	
}

unsigned char ctrl_machine(char* parameter)
{
	P_S_WifiFrame wifi_frame =NULL;
	S_WifiFrame msg={0};//�ظ���Ϣ
	
	wifi_frame = (P_S_WifiFrame)parameter;
	if(0 == CheckDeviceID(wifi_frame)&&(3 != wifi_frame->data_length))
		{
			return 0;//����֡����
		}	

	//set_machine_status(wifi_frame->arg[0]);
	//set_automod(wifi_frame->arg[1]);
	//set_formula(wifi_frame->arg[2]);

	msg.data_length = 4;
	msg.format_control.fc.encrypt = 0;
	msg.format_control.fc.version= 0;
	msg.function = CTRLMACHINE;
	SetTimeStampIntoArrary(msg.arg);
	UploadDataByWifi(&msg);
	return 1;	
}

unsigned char get_fermentor_ID(char* parameter)
{
	P_S_WifiFrame wifi_frame =NULL;
	S_WifiFrame msg={0};//�ظ���Ϣ
	
	wifi_frame = (P_S_WifiFrame)parameter;
	if(0 == CheckDeviceID(wifi_frame)&&(0 != wifi_frame->data_length))
		{
			return 0;//����֡����
		}	

	msg.data_length = 4;
	msg.format_control.fc.encrypt = 0;
	msg.format_control.fc.version= 0;
	msg.function = GETFERMENTORID;
	SetTimeStampIntoArrary(msg.arg);
	UploadDataByWifi(&msg);
	return 1;	
}

unsigned char get_fermentor_INFO(char* parameter)
{
	P_S_WifiFrame wifi_frame =NULL;	
	wifi_frame = (P_S_WifiFrame)parameter;
	unsigned short fermentor_ID;
	if(0 == CheckDeviceID(wifi_frame)&&(2 != wifi_frame->data_length))
		{
			return 0;
		}
	fermentor_ID |= wifi_frame->arg[0];
	fermentor_ID <<= 8;
	fermentor_ID |= wifi_frame->arg[1];

	//upload_fermentor_info(fermentor_ID);
		
	return 1;	
}

unsigned char set_autoupload_step(char* parameter)
{
	P_S_WifiFrame wifi_frame =NULL;
	S_WifiFrame msg={0};//�ظ���Ϣ
	unsigned int upload_step = 0;
	
	wifi_frame = (P_S_WifiFrame)parameter;
	if(0 == CheckDeviceID(wifi_frame)&&(2 != wifi_frame->data_length))
		{
			return 0;//����֡����
		}	
	upload_step |= wifi_frame->arg[0];
	upload_step <<= 8;
	upload_step |= wifi_frame->arg[1];

	//set_uoload_step(upload_step);
	SetAutoUpload(upload_step*1000);

	msg.data_length = 4;
	msg.format_control.fc.encrypt = 0;
	msg.format_control.fc.version= 0;
	msg.function = SETUPLOADSETP;
	SetTimeStampIntoArrary(msg.arg);
	UploadDataByWifi(&msg);
	return 1;	
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
{CTRLVALUE,ctrl_device_value,"ctrl value"},//���Ƴ��Ʒ�
{SETPURCHASEVOLUME,set_purchase_volume,"set purchase  volume"},//���ù���Ƶ�����
{CTRLMACHINE,ctrl_machine,"get device id"},//������ƻ�
{GETFERMENTORID,get_fermentor_ID,"set device  date"},//��ȡ���͹�ID
{GETFERMENTORINFO,get_fermentor_INFO,"get device id"},//��ȡָ�����͹޵���Ϣ
{SETUPLOADSETP,set_autoupload_step,"set device  date"},//���ö�ʱ�ϴ�ʱ����
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

	if( 0 == VerifyCRC8Sub(WifiOperatData.Rx_data,(WifiOperatData.recv_data_length - 1)))
		{
			return;
		}
	
	if((WifiOperatData.Rx_data[7]==0XF0)||(WifiOperatData.Rx_data[7]==0XF1)||(WifiOperatData.Rx_data[7]==0XF2)||(WifiOperatData.Rx_data[7]==0XF8))//wifi Զ������
		{
			wifi_iap_operater();
		}
}
/***********************************************��SERVER��**********************************************************/

/***********************************************��PLC��**********************************************************/





unsigned char PLCDecode(unsigned char* data,unsigned short recv_data_length,void* plc_frame)
{
	if(CURFRAMELENGTH != recv_data_length)	
		{
			return 0;
		}
	memcpy(plc_frame,data,CURFRAMELENGTH);
	return 1;
}

unsigned char GetFirstEmptyFermentorSpace(void)
{
	unsigned char loopx = 0;
	for(; loopx < MAX_FERMENTOR_NUMBER; loopx++)
		{
			if(0 == MachineInfo.Fermentor[loopx].IsSpaceUsed)
				{
					return loopx;
				}
		}
	return 0XFF;
}

unsigned char GetFermentorID(unsigned short FermentorID)
{

	unsigned char loopx = 0;
	for(;loopx < MachineInfo.FermentorNum; loopx ++)
		{
			if(FermentorID == MachineInfo.Fermentor[loopx].FermentorID)
				{
					return loopx;
				}
		}
	return 0XFF;
}

unsigned char AddFermentorID(unsigned short FermentorID)
{
	unsigned char AddPoint = 0;
	if(0XFF == GetFermentorID(FermentorID))
		{
			AddPoint = GetFirstEmptyFermentorSpace();
			if(0XFF == AddPoint)
				{
					return 0XFF;
				}			
			MachineInfo.Fermentor[AddPoint].FermentorID = FermentorID;
			MachineInfo.Fermentor[AddPoint].IsSpaceUsed = 1;
			MachineInfo.FermentorNum ++;
		}
	return 0;
}

void UpdataMachineInfo(unsigned char fermentor_num,void* plc_frame)
{
	if(fermentor_num > MachineInfo.FermentorNum)
		{
			return;//���Ϸ����͹ޱ����ַ
		}
	//MachineInfo.RuningStage = (P_CURPLCFRAME)plc_frame->DeviceStatus;
	MachineInfo.Fermentor[fermentor_num].LeftVolume = (((POINTCURPLCFRAME)plc_frame)->LeftVloume)*10;
	MachineInfo.Temperature1 = (((POINTCURPLCFRAME)plc_frame)->Temperture)*10;
	//MachineInfo.Fermentor[fermentor_num].UsedVolume = (P_CURPLCFRAME)plc_frame->UsedVloume;
	//MachineInfo.Fermentor[fermentor_num].PressValue = (P_CURPLCFRAME)plc_frame->PressValue;
	//MachineInfo.Fermentor[fermentor_num].Temperature = (P_CURPLCFRAME)plc_frame->Temperture;
}


HAL_StatusTypeDef CtrlPLC(P_S_PLCSendFrame sendframe)
{
	HAL_StatusTypeDef res;
	res = Rs4851SendData((unsigned char*)sendframe,RS485SENDFRAMELENGTH);	
	return res;
}

void DealPLCData(void)
{
	CURPLCFRAME plc_frame = {0};
	//unsigned short default_fermentorID = 0;
	if(1 == PLCDecode(Rs485_1OperatData.Rx_data,Rs485_1OperatData.recv_data_length,(void*)&plc_frame))
		{
			AddFermentorID(Rs485_1OperatData.Rx_data[0]);
			UpdataMachineInfo(GetFermentorID(Rs485_1OperatData.Rx_data[0]),(void*)&plc_frame);
		}	
}

