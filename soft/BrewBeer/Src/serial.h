#ifndef __SERIAL_H__
#define __SERIAL_H__
#include "include.h"

#define WIFICOM		huart2
#define RS485COM1	huart1
#define RS485COM2	huart3

#define RS485NETCOM	RS485COM1

#define ENABLE_RS485_1_TX		HAL_GPIO_WritePin(RS485_1_TX_EN_GPIO_Port,RS485_1_TX_EN_Pin,GPIO_PIN_RESET)
#define DISABLE_RS485_1_TX		HAL_GPIO_WritePin(RS485_1_TX_EN_GPIO_Port,RS485_1_TX_EN_Pin,GPIO_PIN_SET)
#define ENABLE_RS485_2_TX		HAL_GPIO_WritePin(RS485_2_TX_EN_GPIO_Port,RS485_2_TX_EN_Pin,GPIO_PIN_RESET)
#define DISABLE_RS485_2_TX		HAL_GPIO_WritePin(RS485_2_TX_EN_GPIO_Port,RS485_2_TX_EN_Pin,GPIO_PIN_SET)

#define SERIAL_RXBUFF_SIZE	300
#define SERIAL_TXBUFF_SIZE	100

typedef struct
{
	unsigned short recv_data_length;
	unsigned char Rx_data[SERIAL_RXBUFF_SIZE];
	unsigned char Tx_data[SERIAL_TXBUFF_SIZE];
}S_SerialRecvData,*P_S_SerialRecvData;


#endif

