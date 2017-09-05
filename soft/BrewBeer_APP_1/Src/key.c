#include "key.h"
key_data_def rtos_key;
extern osMessageQId KeyQueueHandle;



unsigned char get_key_value(unsigned char key_number)
{
	unsigned char key_status;
	switch(key_number)
		{
			case 0:
				key_status = HAL_GPIO_ReadPin(KEY_1_GPIO_Port,KEY_1_Pin);
				break;
				
			case 1:
				key_status = HAL_GPIO_ReadPin(KEY_2_GPIO_Port,KEY_2_Pin);
				break;
			
			default:
				key_status = 0XFF;
				break;
		}
	return key_status;
}





void read_key(void)
{
unsigned char key_trigger_value;

unsigned char loopx = 0;
	for(loopx =0 ;loopx < KEY_NUMBER; loopx++)
	{
		if(GPIO_PIN_RESET == (GPIO_PinState)(get_key_value(loopx)))
			{
				rtos_key.press_time[loopx] = (rtos_key.press_time[loopx] >0XEE) ? 0XEE : rtos_key.press_time[loopx] ;
  				rtos_key.press_time[loopx] ++;
			}
	}
	
	for(rtos_key.key_point = 0; rtos_key.key_point <KEY_NUMBER ;rtos_key.key_point++)
		{
			if((rtos_key.press_time[rtos_key.key_point ]) > 2)
				{
					rtos_key.key_value[rtos_key.key_point ] = KEY_PRESS(rtos_key.key_point);	
						if((rtos_key.key_ok[rtos_key.key_point ]&0X01) == 0)
									{
										rtos_key.key_ok[rtos_key.key_point ] |= 0X01; 
										key_trigger_value =rtos_key.key_point;
										xQueueSend(KeyQueueHandle, &(key_trigger_value), 10);
									}
					if((rtos_key.press_time[rtos_key.key_point ]) > 0X40)
						{
							rtos_key.key_value[rtos_key.key_point ] = KEY_LONG_PRESS(rtos_key.key_point);
							if((rtos_key.key_ok[rtos_key.key_point ]&0X02) == 0)
								{
									rtos_key.key_ok[rtos_key.key_point ] |= 0X02; 
									key_trigger_value =KEY_LONG_TRIGGER(rtos_key.key_point);
									xQueueSend(KeyQueueHandle, &(key_trigger_value), 10);
								}
						}
					if((rtos_key.press_time[rtos_key.key_point ]) > 0XAF)
						{
							rtos_key.key_value[rtos_key.key_point ] = KEY_LONG_LONG_PRESS(rtos_key.key_point);
							if((rtos_key.key_ok[rtos_key.key_point ]&0X04) == 0)
								{
									rtos_key.key_ok[rtos_key.key_point ] |= 0X04; 
									key_trigger_value =KEY_LONG_LONG_TRIGGER(rtos_key.key_point);
									xQueueSend(KeyQueueHandle, &(key_trigger_value), 10);
								}
						}

				}
		}

	for(loopx = 0;loopx < KEY_NUMBER; loopx++)
	{
		if(GPIO_PIN_SET == (GPIO_PinState)(get_key_value(loopx)))
			{
				if(rtos_key.key_ok[loopx])
					{
						rtos_key.key_ok[loopx] = 0;
						xQueueSend(KeyQueueHandle, &(rtos_key.key_value[loopx]), 10);
					}
				rtos_key.press_time[loopx] = 0;
			}
	}
		
}




void deal_key_value(unsigned key_value)
{
	S_PLCSendFrame test;
	test.DeviceAddress = 1;
	test.PurchaseVloume = 2;
	test.EnablePurchase = 3;
	test.Formula = 4;
	test.StartSaccharify = 5;
	test.StartWash = 6;
	test.AutoShutDown = 7;
	test.BeerValueStatus = 8;
	switch(key_value)
  		{
  			case(KEY_1_TRIGGER):
				//LED_5_TOG;
				break;
			case(KEY_2_TRIGGER):
				//LED_5_TOG;
				break;
				
			case(KEY_1_LONG_TRIGGER):
				FaultLedBlink(50,0);
				
				break;

			case(KEY_2_LONG_TRIGGER):
				FaultLedBlink(50,0);
				break;

			case(KEY_1_LONG_LONG_TRIGGER):
				RUNING_LED_ON;
				RS485_1_LED_ON;
				RS485_2_LED_ON;
				FAULT_LED_ON;
				break;
			
			case(KEY_2_LONG_LONG_TRIGGER):
				break;
							
  			case(key1_press):
				COM1LedBlink(500,0);
				COM2LedBlink(400,0);
				RuningLedBlink(300,0);
				FaultLedBlink(200,0);
				upload_device_info(NULL);
				break;

			case(key1_long_press):
				//A_T_Set_SockB_Server(1,192,168,1,198,8888);
				break;

			case(key1_long_long_press):
				try_to_jump(APP_2);

				break;
				
			case(key2_press): 
				COM1LedBlink(200,0);
				COM2LedBlink(300,0);
				RuningLedBlink(400,0);
				FaultLedBlink(500,0);

				CtrlPLC(&test);

				break;

			case(key2_long_press):
				SET_SMART_LINK;
				
				break;
				
				default:
				break;
				
  		}
}


