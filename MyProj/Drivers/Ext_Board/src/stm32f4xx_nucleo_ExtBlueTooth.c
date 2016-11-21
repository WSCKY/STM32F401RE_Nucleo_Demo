/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtBlueTooth.c
  * @author  '^_^'
  * @version V0.0.0
  * @date    27-September-2014
  * @brief   This file includes the BlueTooth driver for ExtBoard BlueTooth Modules.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_ExtBlueTooth.h"

/* Private functions ---------------------------------------------------------*/
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/**
  * @brief  Config the BlueTooth.
  * @param  *UartHandle: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
    * @param  BlueToothType: BluoTooth working mode.
  * @retval Init status.
  */
uint8_t BlueTooth_Init(UART_HandleTypeDef *UartHandle, uint32_t BaudRate)
{
	if(UART_Config(UartHandle, BaudRate))
		return 1;
	return 0;
}

/**
  * @brief  Start BlueTooth Receive.
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @param  pData: Pointer to data buffer.
  * @retval HAL status.
  */
uint8_t BlueToothStartRecv(UART_HandleTypeDef *huart, uint8_t *pData)
{
	if(HAL_UART_Receive_DMA(huart, pData, TXBUFFERSIZE) != HAL_OK)
		return 1;
	return 0;
}

/**
  * @brief  Transmit any data by BlueTooth.
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @param  pData: Pointer to data buffer
  * @retval HAL status.
  */
uint8_t BlueToothTransmit(UART_HandleTypeDef *huart, uint8_t *pData)
{
	if(HAL_UART_Transmit_DMA(huart, pData, TXBUFFERSIZE)!= HAL_OK)
		return 1;
	return 0;
}

/**
  * @brief  check up the BlueTooth.(should be in connection is not established!)
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval Detecte resualt.
  */
uint8_t BlueToothDetected(UART_HandleTypeDef *huart)
{
	uint8_t Recv[4];
	if(HAL_UART_Transmit(huart, (uint8_t *)"AT\r\n", 4, 5000))
		return 1;
	if(HAL_UART_Receive(huart, (uint8_t *)Recv, 4, 5000))
		return 1;
	if(Buffercmp(Recv, (uint8_t *)"OK\r\n", 4))
		return 1;
	return 0;
}

/**
  * @brief  Set BlueTooth Communicate Baudrate.
  * @param  huart: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @param  BaudRate: UART Communication BaudRate.
  *           this parameter should be:
  *                BaudRate_xxx.
  * @retval operate Ans.
  */
uint8_t SetBlueToothBaudRate(UART_HandleTypeDef *huart, uint8_t BaudRate)
{
	uint8_t Com[13];
	uint8_t Recv[13];
	uint32_t BdRt;
	sprintf((char *)Com, "AT+BAUD%x\r\n   ", BaudRate);
	if(HAL_UART_Transmit(huart, Com, 10, 5000))
		return 1;
	if(HAL_UART_Receive(huart, Recv, 13, 5000))
		return 1;
	sprintf((char *)Com, "+BAUD=%x\r\nOK\r\n", BaudRate);
	if(Buffercmp(Recv, Com, 13))
		return 1;
	if(BaudRate < 0x7)
		BdRt = (600 << BaudRate);
	else if(BaudRate < 0xC)
		BdRt = (57600 << (BaudRate - 0x7));
	else BdRt = 1382400;
	
	huart->Init.BaudRate = BdRt;
	/* Check the Over Sampling */
	if(huart->Init.OverSampling == UART_OVERSAMPLING_8)
	{
		/*-------------------------- USART BRR Configuration ---------------------*/
		huart->Instance->BRR = __UART_BRR_SAMPLING8(HAL_RCC_GetPCLK1Freq(), huart->Init.BaudRate);
	}
	else
	{
		/*-------------------------- USART BRR Configuration ---------------------*/
		huart->Instance->BRR = __UART_BRR_SAMPLING16(HAL_RCC_GetPCLK1Freq(), huart->Init.BaudRate);
	}
	return 0;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
	while (BufferLength--)
	{
		if ((*pBuffer1) != *pBuffer2)
		{
			return BufferLength;
		}
		pBuffer1++;
		pBuffer2++;
	}
	return 0;
}

/*************************************** END OF FILE ***************************************/
