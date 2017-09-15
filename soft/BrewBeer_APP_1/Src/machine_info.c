#include "machine_info.h"
S_MachineInfo MachineInfo = {.UploadStepTime = 30000,};

void GetDeviceID(unsigned char* buf)//���ܻ�ʹ����������ȷ��buf����8���ֽ�
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
	if(0 == MachineInfo.AutoUploadTimerStatus)//�½�һ�������ʱ��
		{
			MachineInfo.AutoUploadTimer = register_value_into_timer(msec,1);
			if(0XFF == MachineInfo.AutoUploadTimer)
				{
					return;//����ʧ��
				}
			MachineInfo.AutoUploadTimerStatus = 1;	
			register_callback_function_into_timer(MachineInfo.AutoUploadTimer,AutoUpload);
		}
	if(1 == MachineInfo.AutoUploadTimerStatus)//�޸�һ�������ʱ��
		{
			timer_counter.timer[MachineInfo.AutoUploadTimer].target_value = msec;
		}
}


