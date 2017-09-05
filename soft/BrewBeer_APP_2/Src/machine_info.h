#ifndef __MACHINE_INFO_H__
#define __MACHINE_INFO_H__
#include "include.h"
#define CHIP_ID_ADDRESS	0X1FFFF7E8

#define MAX_FERMENTOR_NUMBER 20

typedef struct
{
	unsigned char Mac[8];
	unsigned char IsAlive;
	unsigned short FermentorID;
	unsigned short Capacity;//����
	unsigned short UsedVolume;//��ʹ������
	unsigned short LeftVolume;//ʣ������
	unsigned short Temperature;//�¶�
	unsigned short PressValue;//���͹�ѹ��
	long int StartFermentTime;
}S_FermentorInfo,*P_S_FermentorInfo;

typedef struct
{
	unsigned char DeviceID[8];
	unsigned char DeviceAddress;
	unsigned char RuningStage;//�����׶�
	unsigned char AutoUploadTimerStatus;//������ʱ��״̬0 δ����   1����
	unsigned char AutoUploadTimer;//��ʱ�ϴ�ʹ�õļ�ʱ��
	unsigned int UploadStepTime;//��ʱ�ϴ�ʱ����
	float Temperature1;//�¶�	
	float Temperature2;//�¶�
	unsigned short FermentorNum;
	unsigned short AliveNum;
	S_FermentorInfo Fermentor[MAX_FERMENTOR_NUMBER];
}S_MachineInfo,*P_S_MachineInfo;
extern S_MachineInfo MachineInfo;

void GetDeviceID(unsigned char* buf);
void SetAutoUpload(unsigned int msec);

#endif