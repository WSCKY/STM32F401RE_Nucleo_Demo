/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtnRF24L01.c
  * @author  '^_^'
  * @version V0.0.0
  * @date    28-September-2014
  * @brief   This file provides a set of functions needed to manage the nRF24L01.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_ExtnRF24L01.h"

/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)
/* Private variables ---------------------------------------------------------*/
uint32_t nRFSpixTimeout = nRF24L01_SPIx_TIMEOUT_MAX; /*<! Value of Timeout when SPI communication fails */
static SPI_HandleTypeDef hnRF24L01_Spi;
__IO uint32_t  nRF24L01Timeout = nRF24L01_FLAG_TIMEOUT;
/* Private function prototypes -----------------------------------------------*/
static void       SPIx_Init(void);
static void       SPIx_Write(uint8_t Value);
static uint32_t   SPIx_Read(void);
//static uint8_t    SPIx_ReadWrite(uint8_t Value);
static void       SPIx_Error(void);
static void       SPIx_MspInit(SPI_HandleTypeDef *hspi);
static void Set_nRF24L01_Addr(uint8_t Pipe, uint8_t Pload_Width, nRF24L01_PipeAddr_InitTypedef *Addr);

/**
  * @brief  Initializes SPI MSP.
  * @param  None
  * @retval None
  */
static void SPIx_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;  
  
  /*** Configure the GPIOs ***/  
  /* Enable GPIO clock */
  nRF24L01_SPIx_SCK_GPIO_CLK_ENABLE();
  nRF24L01_SPIx_MISO_MOSI_GPIO_CLK_ENABLE();
  
  /* Configure SPI SCK */
  GPIO_InitStruct.Pin = nRF24L01_SPIx_SCK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = nRF24L01_SPIx_SCK_AF;
  HAL_GPIO_Init(nRF24L01_SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

  /* Configure SPI MISO and MOSI */ 
  GPIO_InitStruct.Pin = nRF24L01_SPIx_MOSI_PIN;
  GPIO_InitStruct.Alternate = nRF24L01_SPIx_MISO_MOSI_AF;
  GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
  HAL_GPIO_Init(nRF24L01_SPIx_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = nRF24L01_SPIx_MISO_PIN;
  HAL_GPIO_Init(nRF24L01_SPIx_MISO_MOSI_GPIO_PORT, &GPIO_InitStruct);

  /*** Configure the SPI peripheral ***/ 
  /* Enable SPI clock */
  nRF24L01_SPIx_CLK_ENABLE();
}

/**
  * @brief  Initializes SPI HAL.
  * @param  None
  * @retval None
  */
static void SPIx_Init(void)
{
//  nRF24L01_SPIx_FORCE_RESET();
//  nRF24L01_SPIx_RELEASE_RESET();
  if(HAL_SPI_GetState(&hnRF24L01_Spi) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    hnRF24L01_Spi.Instance = nRF24L01_SPIx;
      /* SPI baudrate is set to 12,5 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 100/8 = 12,5 MHz) 
       to verify these constraints:
          - SD card SPI interface max baudrate is 25MHz for write/read
          - PCLK2 max frequency is 84 MHz 
       */ 
    hnRF24L01_Spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    hnRF24L01_Spi.Init.Direction = SPI_DIRECTION_2LINES;
    hnRF24L01_Spi.Init.CLKPhase = SPI_PHASE_1EDGE;
    hnRF24L01_Spi.Init.CLKPolarity = SPI_POLARITY_LOW;
    hnRF24L01_Spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    hnRF24L01_Spi.Init.CRCPolynomial = 7;
    hnRF24L01_Spi.Init.DataSize = SPI_DATASIZE_8BIT;
    hnRF24L01_Spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hnRF24L01_Spi.Init.NSS = SPI_NSS_SOFT;
    hnRF24L01_Spi.Init.TIMode = SPI_TIMODE_DISABLED;
    hnRF24L01_Spi.Init.Mode = SPI_MODE_MASTER;

    SPIx_MspInit(&hnRF24L01_Spi);
    HAL_SPI_Init(&hnRF24L01_Spi);
  }
}

/**
  * @brief  SPI Read 4 bytes from device.
  * @param  None
  * @retval Read data
*/
static uint32_t SPIx_Read(void)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t readvalue = 0;
  uint32_t writevalue = 0xFFFFFFFF;
  
  status = HAL_SPI_TransmitReceive(&hnRF24L01_Spi, (uint8_t*) &writevalue, (uint8_t*) &readvalue, 1, nRFSpixTimeout);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }

  return readvalue;
}

/**
  * @brief  SPI Write a byte to device.
  * @param  Value: value to be written
  * @retval None
  */
static void SPIx_Write(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_Transmit(&hnRF24L01_Spi, (uint8_t*) &Value, 1, nRFSpixTimeout);
    
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
}

//static uint8_t SPIx_ReadWrite(uint8_t Value)
//{
//  HAL_StatusTypeDef status = HAL_OK;
//  uint8_t Retval;
//  status = HAL_SPI_TransmitReceive(&hnRF24L01_Spi, (uint8_t *)&Value, (uint8_t *)&Retval, 1, nRFSpixTimeout);
//  /* Check the communication status */
//  if(status != HAL_OK)
//  {
//    /* Execute user timeout callback */
//    SPIx_Error();
//  }
//  return Retval;
//}

/**
  * @brief  SPI error treatment function.
  * @param  None
  * @retval None
  */
static void SPIx_Error(void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&hnRF24L01_Spi);
  
  /* Re-Initiaize the SPI communication BUS */
  SPIx_Init();
}

/******************************************************************************************/
/**
  * @brief  Set NRF24L01 Initialization.
  * @param  NRF24L01_Config_Struct: pointer to a NRF24L01_Config_TypeDef structure.
  *         that contains the configuration setting for the NRF24L01.
  * @retval None
  */
void nRF24L01_Init(nRF24L01_InitTypeDef *nRF24L01_InitStruct)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	nRF24L01_IRQ_GPIO_CLK_ENABLE();
	nRF24L01_CSN_GPIO_CLK_ENABLE();
	nRF24L01_CE_GPIO_CLK_ENABLE();

	GPIO_InitStruct.Pin = nRF24L01_CE_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(nRF24L01_CE_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = nRF24L01_CSN_PIN;
	HAL_GPIO_Init(nRF24L01_CSN_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = nRF24L01_IRQ_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(nRF24L01_IRQ_GPIO_PORT, &GPIO_InitStruct);

	SPIx_Init();

	nRF24L01_CE_LOW();

	nRF24L01_SPI_RW_Reg(WRITE_REG_NRF + EN_AA, nRF24L01_InitStruct->AUTO_ACK);
	nRF24L01_SPI_RW_Reg(WRITE_REG_NRF + EN_RXADDR, nRF24L01_InitStruct->RX_PIPE);
	nRF24L01_SPI_RW_Reg(WRITE_REG_NRF + RF_CH, nRF24L01_InitStruct->COM_FREQ);
	nRF24L01_SPI_RW_Reg(WRITE_REG_NRF + RF_SETUP, nRF24L01_InitStruct->RF_SETUP_VALUE);
	nRF24L01_SPI_RW_Reg(WRITE_REG_NRF + CONFIG, nRF24L01_InitStruct->TRANS_MODE);
	nRF24L01_SPI_RW_Reg(WRITE_REG_NRF + SETUP_RETR, nRF24L01_InitStruct->REPEATS);

	nRF24L01_Write(WRITE_REG_NRF + TX_ADDR, nRF24L01_InitStruct->Addr->LOCAL_TX_ADDR, ADR_WIDTH);
	Set_nRF24L01_Addr(nRF24L01_InitStruct->RX_PIPE, nRF24L01_InitStruct->PLOAD_WIDTH, \
	                  nRF24L01_InitStruct->Addr);

	nRF24L01_CE_HIGH();
}

/**
  * @brief  Get Pipe Number which received the data..
  * @param  None
  * @retval Pipe Number.
  */
uint8_t nRF24L01_GetPipeNum(void)
{
	uint8_t Sta;
	Sta = (nRF24L01_SPI_Read_Reg(STATUS) & 0x0E);
	Sta >>= 1;
	return Sta;
}

/**
  * @brief  set the nRF24L01 to receive mode.
  * @param  None
  * @retval None
  */
void nRF24L01_RX_Mode(void)
{
	nRF24L01_CE_LOW();
	nRF24L01_SPI_RW_Reg(WRITE_REG_NRF + CONFIG, RX_MODE);
	nRF24L01_CE_HIGH();
	HAL_Delay(1);
}

/**
  * @brief  set the nRF24L01 to transmit mode.
  * @param  None
  * @retval None
  */
void nRF24L01_TX_Mode(void)
{
	nRF24L01_CE_LOW();
	nRF24L01_SPI_RW_Reg(WRITE_REG_NRF + CONFIG, TX_MODE);
	nRF24L01_CE_HIGH();
	HAL_Delay(1);
}

/**
  * @brief  Transmit a block of data by nRF24L01.
  * @param  pointer to the buffer  containing the data to be transmit.
  * @retval status.
  */
uint8_t nRF24L01_TX_Packet(uint8_t* pBuffer)
{
	uint8_t status;
	nRF24L01_CE_LOW();
	nRF24L01_Write(WR_TX_PLOAD, pBuffer, PLOAD_WIDTH_32);
	nRF24L01_CE_HIGH();
	while(nRF24L01_IRQ_State());
	status = nRF24L01_SPI_Read_Reg(STATUS);
	nRF24L01_SPI_RW_Reg(WRITE_REG_NRF + STATUS, (status | 0x20));
	if(status & MAX_TX)
	{
		nRF24L01_SPI_RW_Reg(FLUSH_TX, DUMMY_BYTE);
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
uint8_t nRF24L01_RX_Packet(uint8_t* pBuffer)
{
	uint8_t status;		    							   
	status = nRF24L01_SPI_Read_Reg(STATUS);
	nRF24L01_SPI_RW_Reg(WRITE_REG_NRF + STATUS, status);
	if(status & RX_OK)
	{
		nRF24L01_Read(RD_RX_PLOAD, pBuffer, PLOAD_WIDTH_32);
		nRF24L01_SPI_RW_Reg(FLUSH_RX, DUMMY_BYTE);
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
void nRF24L01_SPI_RW_Reg(uint8_t Reg, uint8_t Value)
{
//	uint8_t status;

	/* Set chip select Low at the start of the transmission */
	nRF24L01_CSN_LOW();
	/* select register */
//	status = SPIx_ReadWrite(Reg);
	SPIx_Write(Reg);
	/* ..and write value to it.. */ 
	SPIx_Write(Value);
	/* Set chip select High at the end of the transmission */ 
	nRF24L01_CSN_HIGH();

//	return(status);
}

/**
  * @brief  Reads one byte from the NRF24L01.
  * @param  Reg : NRF24L01's internal address to read from.
  * @retval The data read from the NRF24L01.
  */
uint8_t nRF24L01_SPI_Read_Reg(uint8_t Reg)
{
	uint8_t reg_val;

	/* CSN low, initialize SPI communication... */
	nRF24L01_CSN_LOW();
	/* Select register to read from.. */
	SPIx_Write(Reg);
	/* ..then read registervalue */
	reg_val = SPIx_Read();
	/* CSN high, terminate SPI communication */
	nRF24L01_CSN_HIGH();

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
void nRF24L01_Write(uint8_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite)
{
	/* Set chip select Low at the start of the transmission */
	nRF24L01_CSN_LOW();

	/* Send the Address of the indexed register */
	SPIx_Write(WriteAddr);
	/* Send the data that will be written into the device (MSB First) */
	while(NumByteToWrite >= 0x01)
	{
		SPIx_Write(*pBuffer);
		NumByteToWrite--;
		pBuffer++;
	}

	/* Set chip select High at the end of the transmission */ 
	nRF24L01_CSN_HIGH();
}

/**
  * @brief  Reads a block of data from the NRF24L01.
  * @param  ReadAddr : NRF24L01's internal address to read from.
  * @param  pBuffer : pointer to the buffer that receives the data read from the NRF24L01.
  * @param  NumByteToRead : number of bytes to read from the NRF24L01.
  * @retval None
  */
void nRF24L01_Read(uint8_t ReadAddr, uint8_t* pBuffer, uint16_t NumByteToRead)
{  
	/* Set chip select Low at the start of the transmission */
	nRF24L01_CSN_LOW();

	/* Send the Address of the indexed register */
	SPIx_Write(ReadAddr);

	/* Receive the data that will be read from the device (MSB First) */
	while(NumByteToRead > 0x00)
	{
		/* Send dummy byte (0x00) to generate the SPI clock to NRF24L01 (Slave device) */
		*pBuffer = SPIx_Read();
		NumByteToRead--;
		pBuffer++;
	}

	/* Set chip select High at the end of the transmission */
	nRF24L01_CSN_HIGH();
}

/**
  * @brief  Set nRF24L01 Address.
  * @param  Pipe : pipe.
  * @param  Pload_Width : pload width.
  * @param  Addr : address structure.
  * @retval None
  */
static void Set_nRF24L01_Addr(uint8_t Pipe, uint8_t Pload_Width, nRF24L01_PipeAddr_InitTypedef *Addr)
{
	uint8_t Index = 0, *buf;
	
	for(Index = 0; Index < 6; Index ++)
	{
		if(Pipe & ((0x01) << Index))
		{
			switch(Index)
			{
				case 0: buf = Addr->RX_ADDR0; break;
				case 1: buf = Addr->RX_ADDR1; break;
				case 2: buf = Addr->RX_ADDR2; break;
				case 3: buf = Addr->RX_ADDR3; break;
				case 4: buf = Addr->RX_ADDR4; break;
				case 5: buf = Addr->RX_ADDR5; break;
			}
			nRF24L01_Write(WRITE_REG_NRF + RX_ADDR_P0 + Index, buf, Addr_Width(Index));
			nRF24L01_SPI_RW_Reg(WRITE_REG_NRF + RX_PW_P0 + Index, Pload_Width);
		}
	}
}

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t nRF24L01_TIMEOUT_UserCallback(void)
{
	/* Block communication and all processes */
	while(1);
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

/*************************************** END OF FILE ***************************************/
