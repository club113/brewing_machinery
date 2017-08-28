#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <string.h>

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "dma.h"
#include "iwdg.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stm32f1xx_hal_usart.h"
#include "FreeRTOS.h"
#include "task.h"

#include "machine_info.h"
#include "serial.h"
#include "wifi.h"

#include "led.h"
#include "crc.h"
#include "cpu.h"
#include "key.h"
#include "protocol.h"
#include "timer_operat.h"
#include "rtc_time.h"


extern osMessageQId SerialQueueHandle;
extern osMessageQId KeyQueueHandle;
extern osMessageQId SysQueueHandle;

#endif

