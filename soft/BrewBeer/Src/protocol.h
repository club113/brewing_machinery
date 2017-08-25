#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include "include.h"

#define CALLID	0X01
#define GETTEMP	0X01
#define GETINFO	0X01
#define SETDATE	0X01


#define ARGMAXLENGTH	20

typedef struct
{
	unsigned char function_code;
	unsigned char (*function)(char* parameter);
	char* help;
}S_Cmd,*P_S_Cmd;


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



void DealWifiData(void);


#endif

