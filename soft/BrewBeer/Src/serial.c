#include "serial.h"

S_SerialRecvData WifiOperatData;
S_SerialRecvData Rs485_1OperatData;
S_SerialRecvData Rs485_2OperatData;




void UsartReceive_IDLE(UART_HandleTypeDef *huart)  
{  
	if(huart == &WIFICOM)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
				__HAL_UART_CLEAR_IDLEFLAG(&WIFICOM);  
				WifiOperatData.DmaCNDTR =  WIFICOM.hdmarx->Instance->CNDTR;
				HAL_TIM_Base_Start_IT(&WIFITIMER);
		    }  
	  	}

	if(huart == &RS485COM1)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
				__HAL_UART_CLEAR_IDLEFLAG(&RS485COM1);  
				Rs485_1OperatData.DmaCNDTR =  RS485COM1.hdmarx->Instance->CNDTR;
				HAL_TIM_Base_Start_IT(&RS485TIMER1);
		    }  
	  	}

	if(huart == &RS485COM2)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
				__HAL_UART_CLEAR_IDLEFLAG(&RS485COM2);  
				Rs485_2OperatData.DmaCNDTR =  RS485COM2.hdmarx->Instance->CNDTR;
				HAL_TIM_Base_Start_IT(&RS485TIMER2);
		    }  
	  	}
}



void WifiComStartRecv(void)
{
	HAL_UART_Receive_DMA(&WIFICOM, WifiOperatData.Rx_data, SERIAL_RXBUFF_SIZE);  
	__HAL_UART_ENABLE_IT(&WIFICOM, UART_IT_IDLE);
}

void Rs485_1ComStartRecv(void)
{
	HAL_UART_Receive_DMA(&RS485COM1, Rs485_1OperatData.Rx_data, SERIAL_RXBUFF_SIZE);  
	__HAL_UART_ENABLE_IT(&RS485COM1, UART_IT_IDLE);
}

void Rs485_2ComStartRecv(void)
{
	HAL_UART_Receive_DMA(&RS485COM2, Rs485_2OperatData.Rx_data, SERIAL_RXBUFF_SIZE);  
	__HAL_UART_ENABLE_IT(&RS485COM2, UART_IT_IDLE);
}


void DealWifiRecvData(void)
{
	DealWifiData();
}

void DealRs485_1RecvData(void)
{

}

void DealRs485_2RecvData(void)
{

}

