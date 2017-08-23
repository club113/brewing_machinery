#include "serial.h"

S_SerialRecvData WifiRecvData;
S_SerialRecvData Rs485_1RecvData;
S_SerialRecvData Rs485_2RecvData;




void UsartReceive_IDLE(UART_HandleTypeDef *huart)  
{  
	if(huart == &WIFICOM)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
				__HAL_UART_CLEAR_IDLEFLAG(&WIFICOM);  
				WifiRecvData.DmaCNDTR =  WIFICOM.hdmarx->Instance->CNDTR;
				HAL_TIM_Base_Start_IT(&WIFITIMER);
		    }  
	  	}

	if(huart == &RS485COM1)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
				__HAL_UART_CLEAR_IDLEFLAG(&RS485COM1);  
				Rs485_1RecvData.DmaCNDTR =  RS485COM1.hdmarx->Instance->CNDTR;
				HAL_TIM_Base_Start_IT(&RS485TIMER1);
		    }  
	  	}

	if(huart == &RS485COM2)
	  	{
		    if((__HAL_UART_GET_FLAG(huart,UART_FLAG_IDLE) != RESET))  
		    {   
				__HAL_UART_CLEAR_IDLEFLAG(&RS485COM2);  
				Rs485_2RecvData.DmaCNDTR =  RS485COM2.hdmarx->Instance->CNDTR;
				HAL_TIM_Base_Start_IT(&RS485TIMER2);
		    }  
	  	}
}


void DealWifiRecvData(void)
{

}

void DealRs485_1RecvData(void)
{

}

void DealRs485_2RecvData(void)
{

}

