#include "machine_info.h"
S_MachineInfo MachineInfo;

void GetDeviceID(unsigned char* buf)//���ܻ�ʹ����������ȷ��buf����8���ֽ�
{
	unsigned char* ID = (unsigned char*)CHIP_ID_ADDRESS;
	memcpy(buf,ID,8);
}




