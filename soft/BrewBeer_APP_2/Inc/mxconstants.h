/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MXCONSTANT_H
#define __MXCONSTANT_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define WIFI_RESET_Pin GPIO_PIN_0
#define WIFI_RESET_GPIO_Port GPIOA
#define WIFI_RELOAD_Pin GPIO_PIN_1
#define WIFI_RELOAD_GPIO_Port GPIOA
#define PH_POWER_Pin GPIO_PIN_5
#define PH_POWER_GPIO_Port GPIOC
#define RS485_2_TX_EN_Pin GPIO_PIN_1
#define RS485_2_TX_EN_GPIO_Port GPIOB
#define RS485_1_TX_EN_Pin GPIO_PIN_11
#define RS485_1_TX_EN_GPIO_Port GPIOA
#define FAULT_LED_Pin GPIO_PIN_12
#define FAULT_LED_GPIO_Port GPIOA
#define RUN_IED_Pin GPIO_PIN_10
#define RUN_IED_GPIO_Port GPIOC
#define RS485_2_LED_Pin GPIO_PIN_11
#define RS485_2_LED_GPIO_Port GPIOC
#define RS485_1_LED_Pin GPIO_PIN_12
#define RS485_1_LED_GPIO_Port GPIOC
#define KEY_2_Pin GPIO_PIN_5
#define KEY_2_GPIO_Port GPIOB
#define KEY_1_Pin GPIO_PIN_6
#define KEY_1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MXCONSTANT_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
