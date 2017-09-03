#include "led.h"

void RuningLedOff(void* arg)
{
	RUNING_LED_OFF;
}

void RuningLedOn(void* arg)
{
	RUNING_LED_ON;
}

void RuningLedTog(void* arg)
{
	RUNING_LED_TOG;
}


void COM1LedOff(void* arg)
{
	RS485_1_LED_OFF;
}

void COM1LedOn(void* arg)
{
	RS485_1_LED_ON;
}

void COM1LedTog(void* arg)
{
	RS485_1_LED_TOG;
}

void COM2LedOff(void* arg)
{
	RS485_2_LED_OFF;
}

void COM2LedOn(void* arg)
{
	RS485_2_LED_ON;
}

void COM2LedTog(void* arg)
{
	RS485_2_LED_TOG;
}

void FaultLedOff(void* arg)
{
	FAULT_LED_OFF;
}

void FaultLedOn(void* arg)
{
	FAULT_LED_ON;
}

void FaultLedTog(void* arg)
{
	FAULT_LED_TOG;
}

void PHPowerOff(void* arg)
{
	PH_POWER_OFF;
}

void PHPowerOn(void* arg)
{
	PH_POWER_ON;
}


void RuningLedBlink(unsigned int time,unsigned char mod)//mod:0 只闪一次mod:1连续闪烁
{
	RUNING_LED_ON;
	if(0 == mod)
		{
			register_callback_function_into_timer(register_value_into_timer(time,mod),RuningLedOff);
		}
	if(1 == mod)
		{
			register_callback_function_into_timer(register_value_into_timer(time,mod),RuningLedTog);
		}
}

void COM1LedBlink(unsigned int time,unsigned char mod)
{
	RS485_1_LED_ON;
	if(0 == mod)
		{
			register_callback_function_into_timer(register_value_into_timer(time,mod),COM1LedOff);
		}
	if(1 == mod)
		{
			register_callback_function_into_timer(register_value_into_timer(time,mod),COM1LedTog);
		}
}

void COM2LedBlink(unsigned int time,unsigned char mod)
{
	RS485_2_LED_ON;
	if(0 == mod)
		{
			register_callback_function_into_timer(register_value_into_timer(time,mod),COM2LedOff);
		}
	if(1 == mod)
		{
			register_callback_function_into_timer(register_value_into_timer(time,mod),COM2LedTog);
		}
}

void FaultLedBlink(unsigned int time,unsigned char mod)
{
	FAULT_LED_ON;
	if(0 == mod)
		{
			register_callback_function_into_timer(register_value_into_timer(time,mod),FaultLedOff);
		}
	if(1 == mod)
		{
			register_callback_function_into_timer(register_value_into_timer(time,mod),FaultLedTog);
		}
}



