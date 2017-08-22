#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include "include.h"


#define WIFICOM		huart1
#define RS485COM1	huart2
#define RS485COM2	huart3

#define RS485NETCOM	RS485COM1
#define ARGMAXLENGTH	20

typedef struct
{
	unsigned char version:7;
	unsigned char encrypt:1;
}S_FormatControl,*P_S_FormatControl;

typedef union
{
	S_FormatControl fc;
	unsigned char fc_value;
} U_FormatControl;

typedef struct
{
	unsigned short data_length;
	U_FormatControl format_control;
	unsigned char DeviceID[8];	
	unsigned char function;
	unsigned char arg[ARGMAXLENGTH];
}S_WifiFrame,*P_S_WifiFrame;


#endif
