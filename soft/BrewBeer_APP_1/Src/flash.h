#ifndef __FLASH_H__
#define __FLASH_H__
#include "Include.h"


#define READ_BUFF 	0X01
#define WRITE_BUFF 	0X02
#define UPDATA_FLASH	0X03
#define MESSAGE_EFFECT	0X99
#define DATA_EFFECT 	0X97

typedef enum
{ 
  WRITE_FLASH = 0,
  READ_FLASH,
  UP_DATA
}FLASH_OPERATION;

typedef struct
{
	unsigned char page_num;
	unsigned char r_w_flag;
	unsigned char owner;
	unsigned char this_message_effect;
	unsigned short offset;
	unsigned short length;
	char* from_addr;
	char* to_addr;
}flash_message;

extern flash_message flash_message_1;
extern flash_message flash_message_2;
extern flash_message flash_message_3;
extern flash_message flash_message_4;
extern flash_message flash_message_5;

typedef struct
{
	unsigned char page_num;
	unsigned char data_effect;
	unsigned char owner;
	unsigned char is_buff_changed;
}FLASH_BUFF_INFO;

typedef struct
{
	FLASH_BUFF_INFO buff_info;
	unsigned char r_w_buff[FLASH_PAGE_SIZE];
	
}FLASH_R_W_BUFF;

unsigned char write_flash(unsigned address,char* data,unsigned short len);
unsigned char read_flash(unsigned address,char* data,unsigned short len);
void erase_flash(unsigned int address);
void memcopy(char* from_addr,char* to_addr,unsigned short length);
void flash_r_w_from_queue(unsigned int message_addr);
void flash_operat(FLASH_OPERATION OPEART,unsigned char* addr,unsigned char page_num,unsigned short offset,unsigned short length,unsigned char owner,flash_message* flash_messages);

#endif

