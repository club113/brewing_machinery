#ifndef __LED_H__
#define __LED_H__
#include "include.h"

#define PH_POWER_ON		HAL_GPIO_WritePin(PH_POWER_GPIO_Port,PH_POWER_Pin,GPIO_PIN_SET)
#define PH_POWER_OFF		HAL_GPIO_WritePin(PH_POWER_GPIO_Port,PH_POWER_Pin,GPIO_PIN_RESET)

#define RUNING_LED_ON		HAL_GPIO_WritePin(RUN_IED_GPIO_Port,RUN_IED_Pin,GPIO_PIN_RESET)
#define RUNING_LED_OFF		HAL_GPIO_WritePin(RUN_IED_GPIO_Port,RUN_IED_Pin,GPIO_PIN_SET)
#define RUNING_LED_TOG		HAL_GPIO_TogglePin(RUN_IED_GPIO_Port,RUN_IED_Pin)

#define RS485_1_LED_ON		HAL_GPIO_WritePin(RS485_1_LED_GPIO_Port,RS485_1_LED_Pin,GPIO_PIN_RESET)	
#define RS485_1_LED_OFF	HAL_GPIO_WritePin(RS485_1_LED_GPIO_Port,RS485_1_LED_Pin,GPIO_PIN_SET)
#define RS485_1_LED_TOG	HAL_GPIO_TogglePin(RS485_1_LED_GPIO_Port,RS485_1_LED_Pin)

#define RS485_2_LED_ON		HAL_GPIO_WritePin(RS485_2_LED_GPIO_Port,RS485_2_LED_Pin,GPIO_PIN_RESET)
#define RS485_2_LED_OFF	HAL_GPIO_WritePin(RS485_2_LED_GPIO_Port,RS485_2_LED_Pin,GPIO_PIN_SET)
#define RS485_2_LED_TOG	HAL_GPIO_TogglePin(RS485_2_LED_GPIO_Port,RS485_2_LED_Pin)

#define FAULT_LED_ON		HAL_GPIO_WritePin(FAULT_LED_GPIO_Port,FAULT_LED_Pin,GPIO_PIN_RESET)
#define FAULT_LED_OFF		HAL_GPIO_WritePin(FAULT_LED_GPIO_Port,FAULT_LED_Pin,GPIO_PIN_SET)
#define FAULT_LED_TOG		HAL_GPIO_TogglePin(FAULT_LED_GPIO_Port,FAULT_LED_Pin)

void RuningLedBlink(unsigned int time);
void COM1LedBlink(unsigned int time);
void COM2LedBlink(unsigned int time);
void FaultLedBlink(unsigned int time);

#endif

