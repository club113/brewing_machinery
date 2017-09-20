#ifndef __MACHINE_INFO_H__
#define __MACHINE_INFO_H__
#include "include.h"
#define CHIP_ID_ADDRESS	0X1FFFF7E8

#define MAX_FERMENTOR_NUMBER 20

typedef struct
{
	unsigned char Mac[8];
	unsigned char IsAlive;
	unsigned char IsSpaceUsed;
	unsigned short FermentorID;
	unsigned short Capacity;//容量
	unsigned short UsedVolume;//已使用容量
	unsigned short LeftVolume;//剩余容量
	unsigned short Temperature;//温度
	unsigned short PressValue;//发酵罐压力
	long int StartFermentTime;
}S_FermentorInfo,*P_S_FermentorInfo;

typedef struct
{
	unsigned char DeviceID[8];
	unsigned char DeviceAddress;
	unsigned char ValueStatus;//出酒阀状态
	unsigned char ScreenStatus;
	unsigned char RuningStage;//工作阶段
	unsigned char AutoUploadTimerStatus;//软件定时器状态0 未开启   1开启
	unsigned char AutoUploadTimer;//定时上传使用的计时器
	unsigned int UploadStepTime;//定时上传时间间隔
	unsigned short Temperature1;//温度	
	unsigned short Temperature2;//温度
	unsigned short FermentorNum;
	unsigned short AliveNum;
	S_FermentorInfo Fermentor[MAX_FERMENTOR_NUMBER];
}S_MachineInfo,*P_S_MachineInfo;
extern S_MachineInfo MachineInfo;

void AutoUpload(void* arg);
void GetDeviceID(unsigned char* buf);
void SetAutoUpload(unsigned int msec);

#endif
