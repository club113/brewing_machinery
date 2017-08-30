#include "led.h"

void RuningLedOff(void* arg)
{
	RUNING_LED_OFF;
}

void RuningLedOn(void* arg)
{
	RUNING_LED_ON;
}


void COM1LedOff(void* arg)
{
	RS485_1_LED_OFF;
}

void COM1LedOn(void* arg)
{
	RS485_1_LED_ON;
}

void COM2LedOff(void* arg)
{
	RS485_2_LED_OFF;
}

void COM2LedOn(void* arg)
{
	RS485_2_LED_ON;
}

void FaultLedOff(void* arg)
{
	FAULT_LED_OFF;
}

void FaultLedOn(void* arg)
{
	FAULT_LED_ON;
}

void PHPowerOff(void* arg)
{
	PH_POWER_OFF;
}

void PHPowerOn(void* arg)
{
	PH_POWER_ON;
}


void RuningLedBlink(unsigned int time)
{
	RUNING_LED_ON;
	register_callback_function_into_timer(register_value_into_timer(time,0),RuningLedOff);
}

void COM1LedBlink(unsigned int time)
{
	RS485_1_LED_ON;
	register_callback_function_into_timer(register_value_into_timer(time,0),COM1LedOff);
}

void COM2LedBlink(unsigned int time)
{
	RS485_2_LED_ON;
	register_callback_function_into_timer(register_value_into_timer(time,0),COM2LedOff);
}

void FaultLedBlink(unsigned int time)
{
	FAULT_LED_ON;
	register_callback_function_into_timer(register_value_into_timer(time,0),FaultLedOff);
}



