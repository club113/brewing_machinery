/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "include.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId SystemHandle;
osThreadId SerialServerHandle;
osThreadId ReadKeyHandle;
osThreadId DealKeyHandle;
osThreadId WatcherHandle;
osMessageQId SerialQueueHandle;
osMessageQId KeyQueueHandle;
osMessageQId SysQueueHandle;

/* USER CODE BEGIN Variables */
osMessageQId flash_queueHandle;
/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void SystemTask(void const * argument);
void SerialServerTask(void const * argument);
void ReadKeyTask(void const * argument);
void DealKeyTask(void const * argument);
void WatcherTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationTickHook(void);

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/* USER CODE BEGIN 3 */

/* USER CODE END 3 */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of System */
  osThreadDef(System, SystemTask, osPriorityNormal, 0, 200);
  SystemHandle = osThreadCreate(osThread(System), NULL);

  /* definition and creation of SerialServer */
  osThreadDef(SerialServer, SerialServerTask, osPriorityHigh, 0, 300);
  SerialServerHandle = osThreadCreate(osThread(SerialServer), NULL);

  /* definition and creation of ReadKey */
  osThreadDef(ReadKey, ReadKeyTask, osPriorityNormal, 0, 128);
  ReadKeyHandle = osThreadCreate(osThread(ReadKey), NULL);

  /* definition and creation of DealKey */
  osThreadDef(DealKey, DealKeyTask, osPriorityAboveNormal, 0, 200);
  DealKeyHandle = osThreadCreate(osThread(DealKey), NULL);

  /* definition and creation of Watcher */
  osThreadDef(Watcher, WatcherTask, osPriorityNormal, 0, 400);
  WatcherHandle = osThreadCreate(osThread(Watcher), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of SerialQueue */
  osMessageQDef(SerialQueue, 5, uint32_t);
  SerialQueueHandle = osMessageCreate(osMessageQ(SerialQueue), NULL);

  /* definition and creation of KeyQueue */
  osMessageQDef(KeyQueue, 4, uint8_t);
  KeyQueueHandle = osMessageCreate(osMessageQ(KeyQueue), NULL);

  /* definition and creation of SysQueue */
  osMessageQDef(SysQueue, 8, uint32_t);
  SysQueueHandle = osMessageCreate(osMessageQ(SysQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  osMessageQDef(FashQueue, 8, uint32_t);
  flash_queueHandle = osMessageCreate(osMessageQ(FashQueue), NULL);
  /* USER CODE END RTOS_QUEUES */
}

/* SystemTask function */
void SystemTask(void const * argument)
{

  /* USER CODE BEGIN SystemTask */
	save_task_info();
	WifiComStartRecv();
	Rs485_1ComStartRecv();
	Rs485_2ComStartRecv();
	GetDeviceID(MachineInfo.DeviceID);
	RuningLedBlink(800,1);
	
	//COM2LedBlink(800,1);
	//COM1LedBlink(500,1);
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END SystemTask */
}

/* SerialServerTask function */
void SerialServerTask(void const * argument)
{
  /* USER CODE BEGIN SerialServerTask */
  	unsigned int RecvMsg = 0;
  	save_task_info();
	osDelay(1000);
  /* Infinite loop */
  for(;;)
  {
	xQueueReceive(SerialQueueHandle, &RecvMsg, portMAX_DELAY);
	switch(RecvMsg)
		{
			case WIFIMSG:
				DealWifiRecvData();
				break;
			case RS485_1MSG:
				DealRs485_1RecvData();
				break;

			case RS485_2MSG:
				DealRs485_2RecvData();
				break;

			default:
				break;
		}
  }
  /* USER CODE END SerialServerTask */
}

/* ReadKeyTask function */
void ReadKeyTask(void const * argument)
{
  /* USER CODE BEGIN ReadKeyTask */
  	save_task_info();
  /* Infinite loop */
  for(;;)
  {
	read_key();
	osDelay(35);
  }
  /* USER CODE END ReadKeyTask */
}

/* DealKeyTask function */
void DealKeyTask(void const * argument)
{
  /* USER CODE BEGIN DealKeyTask */
  	save_task_info();
	unsigned char recv_key_value;	
  /* Infinite loop */
  for(;;)
  {
	xQueueReceive(KeyQueueHandle, &recv_key_value, portMAX_DELAY);
	deal_key_value(recv_key_value);
  }
  /* USER CODE END DealKeyTask */
}

/* WatcherTask function */
void WatcherTask(void const * argument)
{
  /* USER CODE BEGIN WatcherTask */
	save_task_info();
  /* Infinite loop */
  for(;;)
  {
	osDelay(200);
	HAL_IWDG_Refresh(&hiwdg);

	//UploadDataByRs485_1(&msg);
	//UploadDataByRs485_2(&msg);
  }
  /* USER CODE END WatcherTask */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
