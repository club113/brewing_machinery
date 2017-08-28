#include "machine_info.h"
S_MachineInfo MachineInfo;

void GetDeviceID(unsigned char* buf)//可能会使程序崩溃务必确认buf大于8个字节
{
	unsigned char* ID = (unsigned char*)CHIP_ID_ADDRESS;
	memcpy(buf,ID,8);
}




