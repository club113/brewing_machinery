#ifndef __KEY_H__
#define __KEY_H__
#include "include.h"

#define Read_KEY_1 HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin)
#define Read_KEY_2 HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin)

#define KEY_PRESS(x)  (0X20 +3*((x)))
#define KEY_LONG_PRESS(x)  (0X21 +3*((x)))
#define KEY_LONG_LONG_PRESS(x)	(0X22+3*((x)))

#define KEY_1_TRIGGER 	0X00
#define KEY_2_TRIGGER 	0X01

#define KEY_LONG_BASE		0X70
#define KEY_LONG_TRIGGER(X)	(KEY_LONG_BASE+(X))	
#define KEY_1_LONG_TRIGGER 	(KEY_LONG_BASE+0)
#define KEY_2_LONG_TRIGGER 	(KEY_LONG_BASE+1)

#define KEY_LONG_LONG_BASE		0X90
#define KEY_LONG_LONG_TRIGGER(X)	(KEY_LONG_LONG_BASE+(X))
#define KEY_1_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+0)
#define KEY_2_LONG_LONG_TRIGGER 	(KEY_LONG_LONG_BASE+1)

#define key1_press  (0X20 +3*((1)-1))
#define key1_long_press (key1_press+1)
#define key1_long_long_press	(key1_press+2)

#define key2_press  (0X20 +3*((2)-1))
#define key2_long_press (key2_press+1)
#define key2_long_long_press	(key2_press+2)


#define KEY_NUMBER 2
typedef struct
{
	unsigned char key_point;
	unsigned char key_set_wifi;
	unsigned char set_wifi_delay;
	unsigned char key_ok[KEY_NUMBER];
	unsigned char key_value[KEY_NUMBER];
	unsigned char  press_time[KEY_NUMBER];
}key_data_def;
extern key_data_def rtos_key;

void read_key(void);
void deal_key_value(unsigned key_value);
#endif

