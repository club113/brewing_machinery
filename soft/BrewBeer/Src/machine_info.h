#ifndef __MACHINE_INFO_H__
#define __MACHINE_INFO_H__
#include "include.h"
#define CHIP_ID_ADDRESS	0X1FFFF7E8

#define MAX_FERMENTOR_NUMBER 20

typedef struct
{
	unsigned char Mac[8];
	unsigned int FermentorID;
	float Temperature;//�¶�
	float Capacity;//����
}S_FermentorInfo,*P_S_FermentorInfo;

typedef struct
{
	unsigned char DeviceID[8];
	unsigned char RuningStage;//�����׶�
	unsigned short UploadStepTime;
	float Temperature1;//�¶�	
	float Temperature2;//�¶�
	S_FermentorInfo Fermentor[MAX_FERMENTOR_NUMBER];
}S_MachineInfo,*P_S_MachineInfo;
extern S_MachineInfo MachineInfo;

void GetDeviceID(unsigned char* buf);

#endif
