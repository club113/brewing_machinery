
#include "protocol.h"

HAL_StatusTypeDef WifiSendData(unsigned char* pTxData, unsigned short Size)
{
	return HAL_UART_Transmit(&WIFICOM,pTxData,Size,200);
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
	*res_data_length = data_length;
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
			memcpy(wifi_frame->arg,(data+14),(data_length -15));
		}
	return 1;
}






