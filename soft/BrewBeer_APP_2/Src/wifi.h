#ifndef __WIFI_H__
#define __WIFI_H__
#include "Include.h"

#define OK 1
#define FULL  2
#define FAIL 3
#define  	A_T_FIELD_START	 0X38
#define 	A_T_FIELD_STOP	 0X00
#define WARNING		0XEE

//#define READ_NREADY HAL_GPIO_ReadPin(NREADY_GPIO_Port,NREADY_Pin)
//#define READ_NLINK HAL_GPIO_ReadPin(NLINK_GPIO_Port,NLINK_Pin)

#define PRESS_SMART_LINK HAL_GPIO_WritePin(WIFI_RELOAD_GPIO_Port,WIFI_RELOAD_Pin,GPIO_PIN_RESET)
#define RELEASE_SMART_LINK HAL_GPIO_WritePin(WIFI_RELOAD_GPIO_Port,WIFI_RELOAD_Pin,GPIO_PIN_SET)
#define SET_SMART_LINK {PRESS_SMART_LINK;HAL_Delay(1000);RELEASE_SMART_LINK;}
#define RESET_SMART_LINK {PRESS_SMART_LINK;HAL_Delay(5000);RELEASE_SMART_LINK;}

#define PRESS_WIFI_RESET HAL_GPIO_WritePin(WIFI_RESET_GPIO_Port,WIFI_RESET_Pin,GPIO_PIN_RESET)
#define RELEASE_WIFI_RESET HAL_GPIO_WritePin(WIFI_RESET_GPIO_Port,WIFI_RESET_Pin,GPIO_PIN_SET)
#define RESET_WIFI {PRESS_WIFI_RESET;HAL_Delay(20);RELEASE_WIFI_RESET;}


void A_T_Command(char* command);
void Quit_A_T_Field(void);
unsigned char A_T_Set_SockA_Server(unsigned char mode,unsigned char ip1,unsigned char ip2,unsigned char ip3,unsigned char ip4,unsigned int port);
unsigned char A_T_Set_SockB_Server(unsigned char mode,unsigned char ip1,unsigned char ip2,unsigned char ip3,unsigned char ip4,unsigned int port);
void wifi_set_server(void);


#endif


