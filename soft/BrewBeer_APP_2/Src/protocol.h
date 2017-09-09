#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include "include.h"

#define GETDEVICEINFO	0XA0
#define GETSERVERTIME	0XA1

#define CALLID	0XB0
#define SETDATE	0XB1
#define CTRLVALUE	0XB2
#define SETPURCHASEVOLUME	0XB3
#define CTRLMACHINE	0XB4
#define GETFERMENTORID	0XB5
#define GETFERMENTORINFO	0XB6
#define SETUPLOADSETP	0XB7




#define ARGMAXLENGTH	40

typedef struct
{
	unsigned char function_code;
	unsigned char (*function)(char* parameter);
	char* help;
}S_Cmd,*P_S_Cmd;


typedef struct
{
	unsigned char encrypt:1;
	unsigned char version:7;
}S_FormatControl,*P_S_FormatControl;

typedef union
{
	S_FormatControl fc;
	unsigned char fc_value;
} U_FormatControl;

typedef struct
{
	unsigned short data_length;//����������
	U_FormatControl format_control;
	unsigned char DeviceID[8];	
	unsigned char function;
	unsigned char arg[ARGMAXLENGTH];
	long int time;
}S_WifiFrame,*P_S_WifiFrame;

//PLC�������ݵĳ���
#define RS485RECVFRAMELENGTH	7
#define RS485RECVFRAMELENGTH_2	3

#define CURPLCFRAME S_PLC_2_RecvFrame
#define POINTCURPLCFRAME P_S_PLC_2_RecvFrame
#define CURFRAMELENGTH		RS485RECVFRAMELENGTH_2

typedef struct
{
	unsigned char DeviceAddress;//�豸��ַ
	unsigned char UsedVloume;//��ʹ������
	unsigned char DeviceStatus;//�豸״̬
	unsigned char BeerValueStatus;//���Ʒ�״̬
	unsigned char LeftVloume;//��ǰҺλ ʣ������
	unsigned char PressValue;//���͹�ѹ��
	unsigned char Temperture;//���͹��¶�
}S_PLCRecvFrame,*P_S_PLCRecvFrame;

typedef struct
{
	unsigned char DeviceAddress;//�豸��ַ
	//unsigned char UsedVloume;//��ʹ������
	//unsigned char DeviceStatus;//�豸״̬
	//unsigned char BeerValueStatus;//���Ʒ�״̬
	unsigned char LeftVloume;//��ǰҺλ ʣ������
	//unsigned char PressValue;//���͹�ѹ��
	unsigned char Temperture;//���͹��¶�
}S_PLC_2_RecvFrame,*P_S_PLC_2_RecvFrame;


//PLC�������ݵĳ���
#define RS485SENDFRAMELENGTH	(sizeof(S_PLCSendFrame))
typedef struct
{
	unsigned char DeviceAddress;//�豸��ַ
	//unsigned char PurchaseVloume;//�����
	//unsigned char EnablePurchase;//ȷ�Ϲ���
	//unsigned char Formula;//�䷽
	//unsigned char StartSaccharify;//�����ǻ�
	//unsigned char StartWash;//������ϴ
	//unsigned char AutoShutDown;//�豸�Զ� �ػ�
	//unsigned char BeerValueStatus;//���Ʒ�״̬
	//unsigned char LcdStatus;//��ʾ��״̬
	//unsigned char RandomToken;//���������
	unsigned char ScreenCtrl;
}S_PLCSendFrame,*P_S_PLCSendFrame;

unsigned char upload_device_info(char* parameter);

HAL_StatusTypeDef UploadDataByWifi(P_S_WifiFrame wifi_frame);
HAL_StatusTypeDef UploadDataByRs485_1(P_S_WifiFrame wifi_frame);
HAL_StatusTypeDef UploadDataByRs485_2(P_S_WifiFrame wifi_frame);

void SetTimeStampIntoArrary(unsigned char* insert_point);
long int GetTimeStampFromArrary(unsigned char* draw_point);


void DealWifiData(void);
void DealPLCData(void);
HAL_StatusTypeDef CtrlPLC(P_S_PLCSendFrame sendframe);

HAL_StatusTypeDef WifiSendData(unsigned char* pTxData, unsigned short Size);

#endif

