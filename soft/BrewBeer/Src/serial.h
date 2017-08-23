#ifndef __SERIAL_H__
#define __SERIAL_H__
#include "include.h"

#define WIFICOM		huart2
#define RS485COM1	huart1
#define RS485COM2	huart3

#define WIFITIMER	htim3
#define RS485TIMER1	htim4
#define RS485TIMER2	htim5

#define WIFIMSG		1
#define RS485_1MSG	2
#define RS485_2MSG	3

#define RS485NETCOM	RS485COM1


/*****************************************
串口采用DMA空闲中断加定时器接收数据
WIFICOM -> WIFITIMER
RS485COM1 -> RS485TIMER1
RS485COM2 -> RS485TIMER2

******************************************/


#define ENABLE_RS485_1_TX		HAL_GPIO_WritePin(RS485_1_TX_EN_GPIO_Port,RS485_1_TX_EN_Pin,GPIO_PIN_RESET)
#define DISABLE_RS485_1_TX		HAL_GPIO_WritePin(RS485_1_TX_EN_GPIO_Port,RS485_1_TX_EN_Pin,GPIO_PIN_SET)
#define ENABLE_RS485_2_TX		HAL_GPIO_WritePin(RS485_2_TX_EN_GPIO_Port,RS485_2_TX_EN_Pin,GPIO_PIN_RESET)
#define DISABLE_RS485_2_TX		HAL_GPIO_WritePin(RS485_2_TX_EN_GPIO_Port,RS485_2_TX_EN_Pin,GPIO_PIN_SET)

#define SERIAL_RXBUFF_SIZE	300
#define SERIAL_TXBUFF_SIZE	100

typedef struct
{
	unsigned short recv_data_length;
	unsigned short DmaCNDTR;
	unsigned char Rx_data[SERIAL_RXBUFF_SIZE];
	unsigned char Tx_data[SERIAL_TXBUFF_SIZE];
}S_SerialRecvData,*P_S_SerialRecvData;
extern S_SerialRecvData WifiRecvData;
extern S_SerialRecvData Rs485_1RecvData;
extern S_SerialRecvData Rs485_2RecvData;


void UsartReceive_IDLE(UART_HandleTypeDef *huart);

void DealWifiRecvData(void);
void DealRs485_1RecvData(void);
void DealRs485_2RecvData(void);

#endif

