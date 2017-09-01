#include "include.h"

FLASH_R_W_BUFF flash_buff;
flash_message flash_message_1;
flash_message flash_message_2;
flash_message flash_message_3;
flash_message flash_message_4;
flash_message flash_message_5;
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


extern osMessageQId flash_queueHandle;

uint32_t Address = 0, PAGEError = 0;
uint32_t PageError = 0;
__IO TestStatus MemoryProgramStatus = PASSED;

static FLASH_EraseInitTypeDef EraseInitStruct;


unsigned char write_flash(unsigned address,char* data,unsigned short len)
{
	__disable_irq();
	erase_flash(address);
	MemoryProgramStatus = PASSED;
	unsigned short count = 0;
	char* data_tmp;
	data_tmp = data;
	HAL_FLASH_Unlock();
	Address = address;
	    while (count < len)
	    {
	      if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, Address, *(short*)data_tmp) == HAL_OK)
	      {
	        Address = Address + 2;
		data_tmp= data_tmp+ 2;
		count = count +2;
	      }
	      else
	      {
	        /* Error occurred while writing data in Flash memory. 
	           User can add here some code to deal with this error */	        
	      }
	    }
	
	__enable_irq();
	    /* Check the correctness of written data */
	    Address = address;
		data_tmp = data;
		count = 0;
		while (count < len)				
		    {
		      if((*(__IO uint16_t*) Address) != *(short*)data_tmp)
		      {
		        MemoryProgramStatus = FAILED;
		      }
		      Address += 2;			  
			data_tmp= data_tmp+ 2;
			count = count +2;
		    }
				
	  HAL_FLASH_Lock();
	   if (MemoryProgramStatus == PASSED)
	    	{
	    		return 1;
	    	}
	   else
	   	{
	   		return 0;
	   	}
 
} 

unsigned char read_flash(unsigned address,char* data,unsigned short len)
{
	  char* data_tmp;
	 data_tmp = data;
	 unsigned short count = 0;
	 MemoryProgramStatus = PASSED;
	 Address = address;
	 while (count < len)				
		    {
		      *data_tmp = (*(__IO uint8_t*) Address);
		      Address += 1;			  
			data_tmp= data_tmp+ 1;
			count = count +1;
		    }
	 
	    /* Check the correctness of read data */
	    Address = address;
		data_tmp = data;
		count = 0;
		while (count < len)				
		    {
		      if((*(__IO uint8_t*) Address) != *data_tmp)
		      {
		        MemoryProgramStatus = FAILED;
		      }
		      Address += 1;			  
			data_tmp= data_tmp+ 1;
			count = count +1;
		    }
		
	  if (MemoryProgramStatus == PASSED)
	    	{
	    		return 1;
	    	}
	   else
	   	{
	   		return 0;
	   	}
}

void erase_flash(unsigned int address)
{
	HAL_FLASH_Unlock();
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = address;
	EraseInitStruct.NbPages     = 1;
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	  {
	    /*
	      Error occurred while page erase.
	      User can add here some code to deal with this error.
	      PAGEError will contain the faulty page and then to know the code error on this page,
	      user can call function 'HAL_FLASH_GetError()'
	    */
	    /* Infinite loop */	   
	  }
	HAL_FLASH_Lock();
}

void memcopy(char* from_addr,char* to_addr,unsigned short length)
{
	char* tmp_from_addr;
	char* tmp_to_addr;
	tmp_from_addr = from_addr;
	tmp_to_addr = to_addr;
	while(length --)
		{
			*(tmp_to_addr ++) = *(tmp_from_addr ++);
		}
}


void flash_r_w_from_queue(unsigned int message_addr)
{
	flash_message* p_flash_message;
	p_flash_message = (flash_message*)message_addr;
	
	if(p_flash_message ->this_message_effect != MESSAGE_EFFECT)
		{
			return;
		}
	if(p_flash_message ->r_w_flag ==UPDATA_FLASH)
		{
			if(flash_buff.buff_info.is_buff_changed == 1)
				{
					write_flash((0X08000000 + flash_buff.buff_info.page_num *(0X800)),(char*)flash_buff.r_w_buff,0X800);
					flash_buff.buff_info.is_buff_changed = 0;
				}
			return;
		}
	if((p_flash_message ->page_num == flash_buff.buff_info.page_num)&&(flash_buff.buff_info.data_effect == DATA_EFFECT))
		{
			if(p_flash_message ->r_w_flag == WRITE_BUFF)
				{
					memcopy(p_flash_message ->from_addr,(char*)(flash_buff.r_w_buff + p_flash_message ->offset),p_flash_message ->length);
					flash_buff.buff_info.is_buff_changed = 1;
				}
			if(p_flash_message ->r_w_flag == READ_BUFF)
				{
					memcopy((char*)(flash_buff.r_w_buff + p_flash_message ->offset),p_flash_message ->to_addr,p_flash_message ->length);
				}
			return;
		}

	else
		{
			if((flash_buff.buff_info.data_effect == DATA_EFFECT) && (flash_buff.buff_info.is_buff_changed ==1))
				{
					write_flash((0X08000000 + flash_buff.buff_info.page_num *(0X800)),(char*)flash_buff.r_w_buff,0X800);
				}
			flash_buff.buff_info.page_num = p_flash_message ->page_num;
			
			read_flash((0X08000000 + flash_buff.buff_info.page_num *(0X800)),(char*)flash_buff.r_w_buff,0X800);
			flash_buff.buff_info.data_effect = DATA_EFFECT;
			
			if(p_flash_message ->r_w_flag == WRITE_BUFF)
				{
					memcopy(p_flash_message ->from_addr,(char*)(flash_buff.r_w_buff + p_flash_message ->offset),p_flash_message ->length);
					flash_buff.buff_info.is_buff_changed = 1;
				}
			if(p_flash_message ->r_w_flag == READ_BUFF)
				{
					memcopy((char*)(flash_buff.r_w_buff + p_flash_message ->offset),p_flash_message ->to_addr,p_flash_message ->length);
				}
			return;
		}
}

void flash_operat(FLASH_OPERATION OPEART,unsigned char*addr,unsigned char page_num,unsigned short offset,unsigned short length,unsigned char owner,flash_message* flash_messages)
{
	unsigned int send_to_queue;
	switch(OPEART)
		{
			case(WRITE_FLASH):
				flash_messages ->from_addr = (char*)addr;
				flash_messages ->length =length;
				flash_messages ->offset = offset;
				flash_messages ->owner = owner;
				flash_messages ->page_num = page_num;
				flash_messages ->r_w_flag = WRITE_BUFF;
				flash_messages ->this_message_effect = MESSAGE_EFFECT;
				send_to_queue = (unsigned int)flash_messages;
				xQueueSend(flash_queueHandle,&send_to_queue, 50);
				break;

			case(READ_FLASH):
				flash_messages ->to_addr = (char*)addr;
				flash_messages ->length =length;
				flash_messages ->offset = offset;
				flash_messages ->owner = owner;
				flash_messages ->page_num = page_num;
				flash_messages ->r_w_flag = READ_BUFF;
				flash_messages ->this_message_effect = MESSAGE_EFFECT;
				send_to_queue = (unsigned int)flash_messages;
				xQueueSend(flash_queueHandle,&send_to_queue, 50);
				break;

			case(UP_DATA):
				flash_messages ->r_w_flag = UPDATA_FLASH;
				flash_messages ->this_message_effect = MESSAGE_EFFECT;
				send_to_queue = (unsigned int)flash_messages;
				xQueueSend(flash_queueHandle,&send_to_queue, 50);
				break;
		}
}


//flash_operat(READ_FLASH,(unsigned char*)&time_table_info,26,sizeof(rtc_save_data),sizeof(Time_Table_Info),10,&flash_message_2);
