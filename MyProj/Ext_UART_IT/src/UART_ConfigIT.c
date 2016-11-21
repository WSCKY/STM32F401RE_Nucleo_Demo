/**
  ******************************************************************************
  * @file    UART_Printf.c 
  * @author  '^_^'
  * @version V0.0.1
  * @date    26-September-2014
  * @brief   UART Com use printf.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "UART_ConfigIT.h"

/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void UART_MspInit(void);
static void UART_MspDeInit(void);

/**
  * @brief  UART MSP Initialization 
  *         This function configures the hardware resources used in this example: 
  *            - Peripheral's clock enable
  *            - Peripheral's GPIO Configuration  
  * @param  None
  * @retval None
  */
static void UART_MspInit(void)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();
  
  /* Enable USARTx clock */
  USARTx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;
  
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
    
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;
    
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
  /* NVIC for USARTx */
  HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
}

/**
  * @brief  UART MSP De-Initialization 
  *         This function frees the hardware resources used in this example:
  *           - Disable the Peripheral's clock
  *           - Revert GPIO and NVIC configuration to their default state
  * @param  None
  * @retval None
  */
static void UART_MspDeInit(void)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

  /*##-3- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(USARTx_IRQn);
}

/**
  * @brief  set UART Configuration.
  * @param  BaudRate: UART Communication BaudRate.
  * @param  *UartHandle: pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval Init status.
  */
uint8_t UART_Config(UART_HandleTypeDef *UartHandle, uint32_t BaudRate)
{
  UART_MspDeInit();
  UART_MspInit();
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART2 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = No parity
      - BaudRate = 'parameter' baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle->Instance        = USARTx;
  
  UartHandle->Init.BaudRate   = BaudRate;
  UartHandle->Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle->Init.StopBits   = UART_STOPBITS_1;
  UartHandle->Init.Parity     = UART_PARITY_NONE;
  UartHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle->Init.Mode       = UART_MODE_TX_RX;
  
  if(HAL_UART_Init(UartHandle) != HAL_OK)
  {
    return 1;
  }
  return 0;
}

/*************************************** END OF FILE ***************************************/
