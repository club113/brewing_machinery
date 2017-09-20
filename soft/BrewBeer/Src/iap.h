#ifndef __IAP_H__
#define __IAP_H__

#include "Include.h"


#define no_jump  		0X88
#define try_jump_flag	0X89

#define WIFI_IAP_PAGE_SIZE		256
/****************************
flash 分配情况 
按照页分配
0 - 35    APP_BASE
36 - 71    APP_1
72 - 107    APP_2
108 - 127    SAVE_DATA iap占用第108  页
*****************************/

#define DEVICE_SOFT_VER  (1001)

#define FLASH_START_ADDRESS	0X8000000

#define APP_BASE 		0X11
#define APP_BASE_START_PAGE	0
#define APP_BASE_PAGE_SIZE	36
#define APP_BASE_ADDRESS (FLASH_START_ADDRESS+(APP_BASE_START_PAGE*FLASH_PAGE_SIZE))


#define APP_1 		0X22
#define APP_1_START_PAGE	36
#define APP_1_PAGE_SIZE	36
#define APP_1_ADDRESS (FLASH_START_ADDRESS+(APP_1_START_PAGE*FLASH_PAGE_SIZE))


#define APP_2 		0X33
#define APP_2_START_PAGE	72
#define APP_2_PAGE_SIZE	36
#define APP_2_ADDRESS (FLASH_START_ADDRESS+(APP_2_START_PAGE*FLASH_PAGE_SIZE))



#define APP_3 		0X44
#define APP_3_START_PAGE	108
#define APP_3_PAGE_SIZE	36
#define APP_3_ADDRESS (FLASH_START_ADDRESS+(APP_3_START_PAGE*FLASH_PAGE_SIZE))



#define APP_INFOR_SAVE_ADDR 	(FLASH_START_ADDRESS+(108*FLASH_PAGE_SIZE))



///////////////////////////////////////////////////////////////////////需要根据情况更改
#define THIS_APP  APP_BASE
#define THIS_APP_ADDR  APP_BASE_ADDRESS


#define NEXT_APP		APP_1
#define WRITE_TO_ADDR APP_1_ADDRESS
///////////////////////////////////////////////////////////////////////

typedef struct
{
	unsigned char owner;
	unsigned char  BUFFflag; // 1; NEW  0 : old
	unsigned int Write_base_addr;
	unsigned int   Frame_Number;// 数据包号 
	unsigned int   Frame_All_Number;
	unsigned int   Frame_count;// 下位机帧计数器
	unsigned char  Page_Number;//存储页号
	unsigned char  Current_Frame_Len;
	//unsigned char   Frame_buffer [128];// 每一帧的数据缓存
	unsigned char   Page_buffer[FLASH_PAGE_SIZE];//一页的数据缓存可作为除了供iap以外的缓存使用
}IAP;

typedef struct
{
	unsigned int cur_app_offset;
	unsigned char app_num;
	unsigned char app_point;
	unsigned char data_effect;
	unsigned char tem_jump;
	unsigned int jump_to_app;
	unsigned int try_to_jump;
	unsigned int app_offset_table[6];	
}APP_INFORMATION;

typedef struct 
{
	unsigned char page:4;

	unsigned char start_retry;
	unsigned char timer_set_flag;
	unsigned char need_retry;
	unsigned char retry_times;
	unsigned char retry_timer;
	
	unsigned short block;
	unsigned short cur_page_size;
	unsigned short type;
	unsigned short target_version;
	unsigned int total_size;
	unsigned int total_page;
	unsigned int recved_page_sum;
	unsigned int recved_page;
	unsigned int request_page;
}S_Wifi_Iap_Info,*p_S_Wifi_Iap_Info;


typedef struct
{
	unsigned char cmd;
	unsigned char parameter_1;
	unsigned char parameter_2;
	unsigned char parameter_3;
	unsigned char parameter_4;
	unsigned char parameter_5;
}S_Wifi_Iap_Cmd,*P_S_Wifi_Iap_Cmd;

extern S_Wifi_Iap_Info wifi_iap_info;

//////////IAP.Page_buffer较大,设计可以作为公用缓存,一下宏指引当前使用缓存的地方
#define OWNER_ON_ONE			0X00
#define OWNER_IAP_BUFFER		0X01
#define OWNER_FLASH_READER 	0X02
#define OWNER_RAM_BUFFER		0X03
#define OWNER_WIFI_IAP_BUFFER	0X04


#define WIFI_IAP_OPERAT_START			0X01
#define WIFI_IAP_OPERAT_START_BACK		0X02
#define WIFI_IAP_OPERAT_BEGIN			0X03
#define WIFI_IAP_OPERAT_BEGIN_BACK		0X04

#define WIFI_IAP_OPERAT_END				0X05
#define WIFI_IAP_OPERAT_TRANSMISSION 	0X07

void my_gpio_init(void);
void test_jump(void);
void disable_all_irq(void);
void SystemClock_Config(void);

void jump_to_app(unsigned int app_addr);

void write_iap_buff(unsigned int length);

void try_to_jump(unsigned char app);
void reset_iap_buff(void);
void read_app_info(void);
void write_app_info(void);
void choose_app(void);
void init_save_data(void);


void wifi_iap_request_page(unsigned int page);
void wifi_iap_operater(void);
void wifi_iap_cmd_operat(P_S_Wifi_Iap_Cmd cmd_parameter);
void wifi_iap_send_ready_code(void);
void wifi_iap_write_to_flash(void);
void wifi_iap_send_end_code(void);
void wifi_iap_copy_to_flash(void);

void start_app(void);


#endif


