/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtBlueTooth.h
  * @author  '^_^'
  * @version V0.0.0
  * @date    27-September-2014
  * @brief   header file for stm32f4xx_nucleo_ExtBlueTooth.c
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXTBLUETOOTH_H
#define __STM32F4XX_NUCLEO_EXTBLUETOOTH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "UART_ConfigDMA.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported Definitions ------------------------------------------------------*/
#define BaudRate_1200                   0x1
#define BaudRate_2400                   0x2
#define BaudRate_4800                   0x3
#define BaudRate_9600                   0x4
#define BaudRate_19200                  0x5
#define BaudRate_38400                  0x6

#define BaudRate_57600                  0x7
#define BaudRate_115200                 0x8
#define BaudRate_230400                 0x9
#define BaudRate_460800                 0xA
#define BaudRate_921600                 0xB

#define BaudRate_1382400                0xC

#define BlueToothRecvedHandle           HAL_UART_RxCpltCallback
#define BlueToothTansedHandle           HAL_UART_TxCpltCallback
#define BlueToothErrorHandle            HAL_UART_ErrorCallback
/* Exported constants --------------------------------------------------------*/
/* Size of Transmission buffer */
#define TXBUFFERSIZE                    (uint32_t)0x20
/* Size of Reception buffer */
#define RXBUFFERSIZE                    TXBUFFERSIZE
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t BlueTooth_Init(UART_HandleTypeDef *UartHandle, uint32_t BaudRate);
uint8_t BlueToothStartRecv(UART_HandleTypeDef *huart, uint8_t *pData);
uint8_t BlueToothTransmit(UART_HandleTypeDef *huart, uint8_t *pData);
uint8_t BlueToothDetected(UART_HandleTypeDef *huart);
uint8_t SetBlueToothBaudRate(UART_HandleTypeDef *huart, uint8_t BaudRate);

#endif /* __STM32F4XX_NUCLEO_EXTBLUETOOTH_H */

/*************************************** END OF FILE ***************************************/
