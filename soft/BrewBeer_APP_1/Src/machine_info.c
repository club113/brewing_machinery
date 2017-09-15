#include "machine_info.h"
S_MachineInfo MachineInfo = {.UploadStepTime = 30000,};

void GetDeviceID(unsigned char* buf)//可能会使程序崩溃务必确认buf大于8个字节
{
	unsigned char* ID = (unsigned char*)CHIP_ID_ADDRESS;
	memcpy(buf,ID,8);
}

void AutoUpload(void* arg)
{
	upload_device_info(NULL);
}

void SetAutoUpload(unsigned int msec)
{
	MachineInfo.UploadStepTime = msec;
	if(0 == MachineInfo.AutoUploadTimerStatus)//新建一个软件定时器
		{
			MachineInfo.AutoUploadTimer = register_value_into_timer(msec,1);
			if(0XFF == MachineInfo.AutoUploadTimer)
				{
					return;//创建失败
				}
			MachineInfo.AutoUploadTimerStatus = 1;	
			register_callback_function_into_timer(MachineInfo.AutoUploadTimer,AutoUpload);
		}
	if(1 == MachineInfo.AutoUploadTimerStatus)//修改一个软件定时器
		{
			timer_counter.timer[MachineInfo.AutoUploadTimer].target_value = msec;
		}
}


