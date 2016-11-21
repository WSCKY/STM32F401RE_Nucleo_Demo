/**
  ******************************************************************************
  * @file    NRF24L01.c
  * @author  '^_^'
  * @version V1.1.0
  * @date    25-November-2013
  * @brief   This file provides a set of functions needed to manage the NRF24L01.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "NRF24L01.h"

/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)

/* Private variables ---------------------------------------------------------*/
__IO uint32_t  NRF24L01Timeout = NRF24L01_FLAG_TIMEOUT;
/* SPI handler declaration */
SPI_HandleTypeDef SpiHandle;

/** @defgroup NRF24L01_Private_FunctionPrototypes
  * @{
  */
static uint8_t NRF24L01_LowLevel_Init(IRQMode_TypeDef IRQ_Mode);
static uint8_t NRF24L01_SendByte(uint8_t byte);
static uint8_t Check_Pipe_Adr(uint8_t Pipe);
static uint8_t Check_Pipe_Wid(uint8_t Pipe);

/**
  * @brief  Set NRF24L01 Initialization.
  * @param  NRF24L01_Config_Struct: pointer to a NRF24L01_Config_TypeDef structure.
  *         that contains the configuration setting for the NRF24L01.
  * @retval None
  */
void NRF24L01_Init(NRF24L01_InitTypeDef *NRF24L01_InitStruct, IRQMode_TypeDef IRQ_Mode)
{
	NRF24L01_LowLevel_Init(IRQ_Mode);

	NRF24L01_CE_LOW();

	NRF24L01_Write(WRITE_REG_NRF + TX_ADDR, NRF24L01_InitStruct->TX_ADD, ADR_WIDTH);
	NRF24L01_Write(WRITE_REG_NRF + Check_Pipe_Adr(NRF24L01_InitStruct->RX_PIPE), \
									NRF24L01_InitStruct->RX_ADD, ADR_WIDTH);

	NRF24L01_SPI_RW_Reg(WRITE_REG_NRF + EN_AA, NRF24L01_InitStruct->AUTO_ACK);
	NRF24L01_SPI_RW_Reg(WRITE_REG_NRF + EN_RXADDR, NRF24L01_InitStruct->RX_PIPE);
	NRF24L01_SPI_RW_Reg(WRITE_REG_NRF + RF_CH, NRF24L01_InitStruct->COM_FREQ);
	NRF24L01_SPI_RW_Reg(WRITE_REG_NRF + RF_SETUP, NRF24L01_InitStruct->RF_SETUP_VALUE);
	NRF24L01_SPI_RW_Reg(WRITE_REG_NRF + CONFIG, NRF24L01_InitStruct->TRANS_MODE);
	NRF24L01_SPI_RW_Reg(WRITE_REG_NRF + Check_Pipe_Wid(NRF24L01_InitStruct->RX_PIPE), \
									NRF24L01_InitStruct->PLOAD_WIDTH);
	NRF24L01_SPI_RW_Reg(WRITE_REG_NRF + SETUP_RETR, NRF24L01_InitStruct->REPEATS);

	NRF24L01_CE_HIGH();
}

/**
  * @brief  set the nRF24L01 to receive mode.
  * @param  None
  * @retval None
  */
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE_LOW();
	NRF24L01_SPI_RW_Reg(WRITE_REG_NRF + CONFIG, 0x0f);
	NRF24L01_CE_HIGH();
}

/**
  * @brief  set the nRF24L01 to transmit mode.
  * @param  None
  * @retval None
  */
void NRF24L01_TX_Mode(void)
{
	NRF24L01_CE_LOW();
	NRF24L01_SPI_RW_Reg(WRITE_REG_NRF + CONFIG, 0x0e);
	NRF24L01_CE_HIGH();
}

/**
  * @brief  Transmit a block of data by nRF24L01.
  * @param  pointer to the buffer  containing the data to be transmit.
  * @retval status.
  */
uint8_t NRF24L01_TX_Packet(uint8_t* pBuffer)
{
	uint8_t status;
	NRF24L01_CE_LOW();
	NRF24L01_Write(WR_TX_PLOAD, pBuffer, PLOAD_WIDTH_32);
	NRF24L01_CE_HIGH();
	while(NRF24L01_IRQ_STATUS());
	status = NRF24L01_SPI_Read_Reg(STATUS);
	NRF24L01_SPI_RW_Reg(WRITE_REG_NRF + STATUS, status);
	if(status & MAX_TX)
	{
		NRF24L01_SPI_RW_Reg(FLUSH_TX, DUMMY_BYTE);
		return MAX_TX;
	}
	if(status & TX_OK)
		return TX_OK;
	return 0xff;
}

/**
  * @brief  Receive a block of data from nRF24L01.
  * @param  pointer to the buffer that receives the data read from the NRF24L01.
  * @retval status.
  */
uint8_t NRF24L01_RX_Packet(uint8_t* pBuffer)
{
	uint8_t status;		    							   
	status = NRF24L01_SPI_Read_Reg(STATUS);
	NRF24L01_SPI_RW_Reg(WRITE_REG_NRF + STATUS, status);
	if(status & RX_OK)
	{
		NRF24L01_Read(RD_RX_PLOAD, pBuffer, PLOAD_WIDTH_32);
		NRF24L01_SPI_RW_Reg(FLUSH_RX, DUMMY_BYTE);
		return 0; 
	}	   
	return 1;/* None */
}

/**
  * @brief  Writes one byte to the NRF24L01.
  * @param  Reg : NRF24L01's internal address to write to.
  * @param  the data to be written to the NRF24L01.
  * @retval The register status.
  */
uint8_t NRF24L01_SPI_RW_Reg(uint8_t Reg, uint8_t Value)
{
	uint8_t status;

	/* Set chip select Low at the start of the transmission */
	NRF24L01_CSN_LOW();
	/* select register */
	status = NRF24L01_SendByte(Reg);
	/* ..and write value to it.. */ 
	NRF24L01_SendByte(Value);
	/* Set chip select High at the end of the transmission */ 
	NRF24L01_CSN_HIGH();

	return(status);
}

/**
  * @brief  Reads one byte from the NRF24L01.
  * @param  Reg : NRF24L01's internal address to read from.
  * @retval The data read from the NRF24L01.
  */
uint8_t NRF24L01_SPI_Read_Reg(uint8_t Reg)
{
	uint8_t reg_val;

	/* CSN low, initialize SPI communication... */
	NRF24L01_CSN_LOW();
	/* Select register to read from.. */
	NRF24L01_SendByte(Reg);
	/* ..then read registervalue */
	reg_val = NRF24L01_SendByte(DUMMY_BYTE);
	/* CSN high, terminate SPI communication */
	NRF24L01_CSN_HIGH();

	/* return register value */
	return(reg_val);
}

/**
  * @brief  Writes a block of data to the NRF24L01.
  * @param  WriteAddr : NRF24L01's internal address to write to.
  * @param  pBuffer : pointer to the buffer containing the data to be written to the NRF24L01.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval None
  */
void NRF24L01_Write(uint8_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite)
{
	/* Set chip select Low at the start of the transmission */
	NRF24L01_CSN_LOW();

	/* Send the Address of the indexed register */
	NRF24L01_SendByte(WriteAddr);
	/* Send the data that will be written into the device (MSB First) */
	while(NumByteToWrite >= 0x01)
	{
		NRF24L01_SendByte(*pBuffer);
		NumByteToWrite--;
		pBuffer++;
	}

	/* Set chip select High at the end of the transmission */ 
	NRF24L01_CSN_HIGH();
}

/**
  * @brief  Reads a block of data from the NRF24L01.
  * @param  ReadAddr : NRF24L01's internal address to read from.
  * @param  pBuffer : pointer to the buffer that receives the data read from the NRF24L01.
  * @param  NumByteToRead : number of bytes to read from the NRF24L01.
  * @retval None
  */
void NRF24L01_Read(uint8_t ReadAddr, uint8_t* pBuffer, uint16_t NumByteToRead)
{  
	/* Set chip select Low at the start of the transmission */
	NRF24L01_CSN_LOW();

	/* Send the Address of the indexed register */
	NRF24L01_SendByte(ReadAddr);

	/* Receive the data that will be read from the device (MSB First) */
	while(NumByteToRead > 0x00)
	{
		/* Send dummy byte (0x00) to generate the SPI clock to NRF24L01 (Slave device) */
		*pBuffer = NRF24L01_SendByte(DUMMY_BYTE);
		NumByteToRead--;
		pBuffer++;
	}

	/* Set chip select High at the end of the transmission */
	NRF24L01_CSN_HIGH();
}

/**
  * @brief SPI MSP Initialization 
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX/RX clock */
	NRF24L01_SPI_SCK_GPIO_CLK_ENABLE();
	NRF24L01_SPI_MISO_GPIO_CLK_ENABLE();
	NRF24L01_SPI_MOSI_GPIO_CLK_ENABLE();
	/* Enable SPI clock */
	NRF24L01_SPI_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* SPI SCK GPIO pin configuration  */
	GPIO_InitStruct.Pin       = NRF24L01_SPI_SCK_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
	GPIO_InitStruct.Alternate = NRF24L01_SPI_SCK_AF;

	HAL_GPIO_Init(NRF24L01_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);

	/* SPI MISO GPIO pin configuration  */
	GPIO_InitStruct.Pin = NRF24L01_SPI_MISO_PIN;
	GPIO_InitStruct.Alternate = NRF24L01_SPI_MISO_AF;

	HAL_GPIO_Init(NRF24L01_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);

	/* SPI MOSI GPIO pin configuration  */
	GPIO_InitStruct.Pin = NRF24L01_SPI_MOSI_PIN;
	GPIO_InitStruct.Alternate = NRF24L01_SPI_MOSI_AF;

	HAL_GPIO_Init(NRF24L01_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief SPI MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to its default state
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
	/*##-1- Reset peripherals ##################################################*/
	NRF24L01_SPI_FORCE_RESET();
	NRF24L01_SPI_RELEASE_RESET();

	/*##-2- Disable peripherals and GPIO Clocks ################################*/
	/* Configure SPI SCK as alternate function  */
	HAL_GPIO_DeInit(NRF24L01_SPI_SCK_GPIO_PORT, NRF24L01_SPI_SCK_PIN);
	/* Configure SPI MISO as alternate function  */
	HAL_GPIO_DeInit(NRF24L01_SPI_MISO_GPIO_PORT, NRF24L01_SPI_MISO_PIN);
	/* Configure SPI MOSI as alternate function  */
	HAL_GPIO_DeInit(NRF24L01_SPI_MOSI_GPIO_PORT, NRF24L01_SPI_MOSI_PIN);
}

/**
  * @brief  Initializes the low level interface used to drive the NRF24L01
  * @param  IRQ_Mode: Specifies interrupt pin mode.
  * @retval Init status.
  */
static uint8_t NRF24L01_LowLevel_Init(IRQMode_TypeDef IRQ_Mode)
{

	GPIO_InitTypeDef GPIO_InitStruct;

	/* Configure the SPI peripheral */
	/* Set the SPI parameters */
	SpiHandle.Instance					= NRF24L01_SPI;
	SpiHandle.Init.BaudRatePrescaler	= SPI_BAUDRATEPRESCALER_8;
	SpiHandle.Init.Direction			= SPI_DIRECTION_2LINES;
	SpiHandle.Init.CLKPhase				= SPI_PHASE_1EDGE;
	SpiHandle.Init.CLKPolarity			= SPI_POLARITY_LOW;
	SpiHandle.Init.CRCCalculation		= SPI_CRCCALCULATION_DISABLED;
	SpiHandle.Init.CRCPolynomial		= 7;
	SpiHandle.Init.DataSize				= SPI_DATASIZE_8BIT;
	SpiHandle.Init.FirstBit				= SPI_FIRSTBIT_MSB;
	SpiHandle.Init.NSS					= SPI_NSS_SOFT;
	SpiHandle.Init.TIMode				= SPI_TIMODE_DISABLED;
	SpiHandle.Init.Mode					= SPI_MODE_MASTER;
	if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
		return 1;

	/* Configure the other pins */
	/* Enable the GPIO_CE Clock */
	NRF24L01_CE_GPIO_CLK_ENABLE();
	/* Enable the GPIO_CSN Clock */
	NRF24L01_CSN_GPIO_CLK_ENABLE();
	/* Enable the GPIO_IRQ Clock */
	NRF24L01_IRQ_GPIO_CLK_ENABLE();
	__SYSCFG_CLK_ENABLE();
	
		/* Configure the GPIO_CE pin */
	GPIO_InitStruct.Pin = NRF24L01_CE_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(NRF24L01_CE_GPIO_PORT, &GPIO_InitStruct);
	
	NRF24L01_CE_LOW();
	
	/* Configure the GPIO_CSN pin */
	GPIO_InitStruct.Pin = NRF24L01_CSN_PIN;
	HAL_GPIO_Init(NRF24L01_CSN_GPIO_PORT, &GPIO_InitStruct);
	
	NRF24L01_CSN_HIGH();

	/* Configure the GPIO_DREQ pin */
	GPIO_InitStruct.Pin = NRF24L01_IRQ_PIN;
	if(IRQ_Mode == IRQ_MODE_GPIO)
	{
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(NRF24L01_IRQ_GPIO_PORT, &GPIO_InitStruct);
	}
	else
	{
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		HAL_GPIO_Init(NRF24L01_IRQ_GPIO_PORT, &GPIO_InitStruct);
		/* Enable and set Data request EXTI Interrupt to the lowest priority */
		HAL_NVIC_SetPriority((IRQn_Type)NRF24L01_IRQ_EXIT_IRQn, 0x0F, 0x00);
		HAL_NVIC_EnableIRQ((IRQn_Type)NRF24L01_IRQ_EXIT_IRQn);
	}
	return 0;
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received
  *         from the SPI bus.
  * @param  Byte : Byte send.
  * @retval The received byte value.
  */
static uint8_t NRF24L01_SendByte(uint8_t byte)
{
	uint8_t ReceByte;
	if(HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t *)&byte, (uint8_t *)&ReceByte, 1, 5000) != HAL_OK)
		return NRF24L01_TIMEOUT_UserCallback();
	return ReceByte;
}

/**
  * @brief  Check the parameters.
  * @param  Pipe : pipe.
  * @retval pipe address.
  */
static uint8_t Check_Pipe_Adr(uint8_t Pipe)
{
	switch(Pipe)
	{
		case EN_RXADDR_P0: return RX_ADDR_P0;
		case EN_RXADDR_P1: return RX_ADDR_P1;
		case EN_RXADDR_P2: return RX_ADDR_P2;
		case EN_RXADDR_P3: return RX_ADDR_P3;
		case EN_RXADDR_P4: return RX_ADDR_P4;
		case EN_RXADDR_P5: return RX_ADDR_P5;
		default :return RX_ADDR_P0;
	}
}

/**
  * @brief  Check the parameters
  * @param  Pipe : pipe
  * @retval pipe width
  */
static uint8_t Check_Pipe_Wid(uint8_t Pipe)
{
	switch(Pipe)
	{
		case EN_RXADDR_P0: return RX_PW_P0;
		case EN_RXADDR_P1: return RX_PW_P1;
		case EN_RXADDR_P2: return RX_PW_P2;
		case EN_RXADDR_P3: return RX_PW_P3;
		case EN_RXADDR_P4: return RX_PW_P4;
		case EN_RXADDR_P5: return RX_PW_P5;
		default :return RX_PW_P0;
	}
}

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t NRF24L01_TIMEOUT_UserCallback(void)
{
	/* Block communication and all processes */
	while(1);
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
