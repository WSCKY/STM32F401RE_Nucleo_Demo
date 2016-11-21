/******************************************************************************
  * @file    stm32f4xx_nucleo_ExtRC522.c
  * @author  '^_^'
  * @version V0.0.0
  * @date    9-October-2014
  * @brief   This file includes the driver for MFRC522.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_ExtRC522.h"

/* Private Defines -----------------------------------------------------------*/

/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0x00)
/* Private variables ---------------------------------------------------------*/
__IO uint32_t  MFRC522Timeout = MFRC522_FLAG_TIMEOUT;
static SPI_HandleTypeDef hMFRC522_Spi;
/* Private FunctionPrototypes ------------------------------------------------*/
static void       SPIx_Init(void);
static void       SPIx_Error(void);
static void       SPIx_MspInit(SPI_HandleTypeDef *hspi);
static uint8_t    MFRC522_SendByte(uint8_t Value);
static void       SetBitMask(uint8_t Reg, uint8_t Mask);
static void       ClearBitMask(uint8_t Reg, uint8_t Mask);
static void       CalulateCRC(uint8_t *pIndata, uint8_t Length, uint8_t *pOutData);
static void       RC_Delay(uint8_t Delay);

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
  MFRC522_SPI_SCK_GPIO_CLK_ENABLE();
  MFRC522_SPI_MISO_GPIO_CLK_ENABLE();
  
  /* Configure SPI SCK */
  GPIO_InitStruct.Pin = MFRC522_SPI_SCK_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = MFRC522_SPI_SCK_AF;
  HAL_GPIO_Init(MFRC522_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);

  /* Configure SPI MISO and MOSI */ 
  GPIO_InitStruct.Pin = MFRC522_SPI_MOSI_PIN;
  GPIO_InitStruct.Alternate = MFRC522_SPI_MOSI_AF;
  GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
  HAL_GPIO_Init(MFRC522_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = MFRC522_SPI_MISO_PIN;
  HAL_GPIO_Init(MFRC522_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);

  /*** Configure the SPI peripheral ***/ 
  /* Enable SPI clock */
  MFRC522_SPI_CLK_ENABLE();
}

/**
  * @brief  Initializes SPI HAL.
  * @param  None
  * @retval None
  */
static void SPIx_Init(void)
{
  if(HAL_SPI_GetState(&hMFRC522_Spi) == HAL_SPI_STATE_RESET)
  {
    /* SPI Config */
    hMFRC522_Spi.Instance = MFRC522_SPI;
      /* SPI baudrate is set to 12,5 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 100/8 = 12,5 MHz) 
       to verify these constraints:
          - SD card SPI interface max baudrate is 25MHz for write/read
          - PCLK2 max frequency is 100 MHz 
       */ 
    hMFRC522_Spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
    hMFRC522_Spi.Init.Direction = SPI_DIRECTION_2LINES;
    hMFRC522_Spi.Init.CLKPhase = SPI_PHASE_1EDGE;
    hMFRC522_Spi.Init.CLKPolarity = SPI_POLARITY_LOW;
    hMFRC522_Spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    hMFRC522_Spi.Init.CRCPolynomial = 7;
    hMFRC522_Spi.Init.DataSize = SPI_DATASIZE_8BIT;
    hMFRC522_Spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hMFRC522_Spi.Init.NSS = SPI_NSS_SOFT;
    hMFRC522_Spi.Init.TIMode = SPI_TIMODE_DISABLED;
    hMFRC522_Spi.Init.Mode = SPI_MODE_MASTER;

    SPIx_MspInit(&hMFRC522_Spi);
    HAL_SPI_Init(&hMFRC522_Spi);
  }
}

/**
  * @brief  SPI error treatment function.
  * @param  None
  * @retval None
  */
static void SPIx_Error(void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&hMFRC522_Spi);
  
  /* Re-Initiaize the SPI communication BUS */
  SPIx_Init();
}

/******************************************************************************************/

/**
  * @brief  Set MFRC522 Initialization.
  * @param  MFRC522_Config_Struct: pointer to a MFRC522_Config_TypeDef structure.
  *         that contains the configuration setting for the MFRC522.
  * @retval None
  */
void MFRC522_Init(MFRC522_InitTypeDef *MFRC522_InitStruct)
{
	uint16_t Temp;
	GPIO_InitTypeDef  GPIO_InitStruct;
	MFRC522_SPI_RST_GPIO_CLK_ENABLE();
	MFRC522_SPI_NSS_GPIO_CLK_ENABLE();
#ifdef USE_IRQ_PIN
	MFRC522_SPI_IRQ_GPIO_CLK_ENABLE();
#endif

	GPIO_InitStruct.Pin = MFRC522_SPI_RST_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(MFRC522_SPI_RST_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = MFRC522_SPI_NSS_PIN;
	HAL_GPIO_Init(MFRC522_SPI_NSS_GPIO_PORT, &GPIO_InitStruct);
#ifdef USE_IRQ_PIN	
	GPIO_InitStruct.Pin = MFRC522_SPI_IRQ_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(MFRC522_SPI_IRQ_GPIO_PORT, &GPIO_InitStruct);
#endif

	SPIx_Init();
	MFRC522_Reset();

	Temp = MFRC522_InitStruct->TPrescaler;
	MFRC522_SPI_RW_Reg(TModeReg, MFRC522_InitStruct->TAutoMode | (Temp >> 8));
	Temp = MFRC522_InitStruct->TPrescaler;
	MFRC522_SPI_RW_Reg(TPrescalerReg, (uint8_t)Temp);
	Temp = MFRC522_InitStruct->TReload;
	MFRC522_SPI_RW_Reg(TReloadRegL, (uint8_t)Temp);
	Temp = MFRC522_InitStruct->TReload;
	MFRC522_SPI_RW_Reg(TReloadRegH, (Temp >> 8));
	MFRC522_SPI_RW_Reg(TxASKReg, MFRC522_InitStruct->ASKRequi);
	MFRC522_SPI_RW_Reg(ModeReg, MFRC522_InitStruct->CRCPreset | 0x3C);
	/* start */
	MFRC522_SPI_RW_Reg(CommandReg, 0x00);
	Set_Antenna_On();
}

/**
  * @brief  Set the MFRC522 reset.
  * @param  None
  * @retval None
  */
void MFRC522_Reset(void)
{
	MFRC522_RST_HIGH();
	RC_Delay(100);
	MFRC522_RST_LOW();
	RC_Delay(100);
	MFRC522_RST_HIGH();
	RC_Delay(100);
	/* reset by software */
	MFRC522_SPI_RW_Reg(CommandReg, SoftReset);
}

/**
  * @brief  Open the antenna.
  * @param  None
  * @retval None
  */
void Set_Antenna_On(void)
{
	uint8_t Temp;
	
	Temp = MFRC522_SPI_Read_Reg(TxControlReg);
	if((Temp & 0x03) == 0)
	{
		SetBitMask(TxControlReg, 0x03);
	}
}

/**
  * @brief  Close the antenna.
  * @param  None
  * @retval None
  */
void Set_Antenna_Off(void)
{
	ClearBitMask(TxControlReg, 0x03);
}

/**
  * @brief  Find cards.
  * @param  ReqMode : Card search mode.
  * @param  TagType : Points to an array of memory card types.
  *						0x4400 = Mifare_UltraLight
  *						0x0400 = Mifare_One(S50)
  *						0x0200 = Mifare_One(S70)
  *						0x0800 = Mifare_Pro(X)
  *						0x4403 = Mifare_DESFire
  * @retval Request status.
  */
uint8_t MFRC522_RequestCard(uint8_t ReqMode, uint8_t *TagType)
{
	uint8_t Resualt;
	uint16_t ReceiBits;

	ClearBitMask(Status2Reg,0x08);

	MFRC522_SPI_RW_Reg(BitFramingReg, 0x07);

	SetBitMask(TxControlReg, 0x03);

	*TagType = ReqMode;/* IC Command */
	Resualt = CommunicateWithIC(Transceive, TagType, 1, TagType, &ReceiBits);
	if((Resualt != MI_OK) || (ReceiBits != 0x10))
		Resualt = MI_ERROR;
	return Resualt;
}

/**
  * @brief  Anti collision detection.
  * @param  *pSerNum : return IC Card's Serial number.
  * @retval operation resualt.
  */
uint8_t MFRC522_Anti_Coll_Detec(uint8_t *pSerNum)
{
	uint8_t Resualt, SerNumCheck = 0;
	uint8_t Index;
	uint16_t ReceiBits;

	ClearBitMask(Status2Reg, 0x08);
	ClearBitMask(CollReg,0x80);
	MFRC522_SPI_RW_Reg(BitFramingReg, 0x00);
	*pSerNum = PICC_ANTICOLL1;/* IC Command */
	pSerNum[1] = 0x20;/* IC Command ? */
	Resualt = CommunicateWithIC(Transceive, pSerNum, 2, pSerNum, &ReceiBits);
	
	if(Resualt == MI_OK)
	{
		/* Check Serial Number */
		for(Index = 0; Index < 4; Index ++)
		{
			SerNumCheck ^= *pSerNum;
			pSerNum ++;
		}
		if(SerNumCheck != *pSerNum)
			Resualt = MI_ERROR;
	}
	SetBitMask(CollReg, 0x80);
	return Resualt;
}

/**
  * @brief  Select Tag.
  * @param  *pSerNum : IC Card's SerNum.
  * @retval Card capacity.
  */
uint8_t MFRC522_SelectTag(uint8_t *pSerNum)
{
	uint8_t Index;
	uint8_t Resualt, Capacity;
	uint16_t ReceiBits;
	uint8_t Buffer[9];

	Buffer[0] = PICC_ANTICOLL1;
	Buffer[1] = 0x70;
	Buffer[6] = 0x00;
	for (Index = 0; Index < 4; Index ++)
	{
		Buffer[Index + 2] = *(pSerNum + Index);
		Buffer[6]  ^=	*(pSerNum + Index);
	}

	CalulateCRC(Buffer, 7, &Buffer[7]);
	ClearBitMask(Status2Reg,0x08);
	Resualt = CommunicateWithIC(Transceive, Buffer, 9, Buffer, &ReceiBits);

	if ((Resualt == MI_OK) && (ReceiBits == 0x18))
		Capacity = Buffer[0];
	else
		Capacity = 0;

	return Capacity;
}

/**
  * @brief  The IC card password verification.
  * @param  AuthMode : Authentication mode.
  *					0x60 : Key A; 0x61 : Key B.
  * @param  BlockAddr : Block Address.
  * @param  *SectorKey : Sector pass word.
  * @param  *pSerNum : IC Card's Serial number.
  * @retval The verification results.
  */
uint8_t MFRC522_Auth(uint8_t AuthMode, uint8_t BlockAddr, uint8_t *SectorKey, uint8_t *pSerNum)
{
	uint8_t Index, Resualt;
	uint16_t ReceiBits;
	uint8_t Buffer[12];    
/* Authentication mode + Block Address + Sector pass word + Serial number*/
	Buffer[0] = AuthMode;
	Buffer[1] = BlockAddr;
	for (Index = 0; Index < 6; Index ++)
		Buffer[Index + 2] = *(SectorKey + Index);

	for (Index = 0; Index < 4; Index ++)
		Buffer[Index + 8] = *(pSerNum + Index);

	Resualt = CommunicateWithIC(MFAuthent, Buffer, 12, Buffer, &ReceiBits);

	if ((Resualt != MI_OK) || (!(MFRC522_SPI_Read_Reg(Status2Reg) & 0x08)))
		Resualt = MI_ERROR;

	return Resualt;
}

/**
  * @brief  Transmit a block of data to IC Card by MFRC522.
  * @param  BlockAddr : the address of block.
  * @param  *pBuffer : pointer to the buffer where containing the data to transmit.
  * @retval The results of transmission.
  */
uint8_t MFRC522_TX_Packet(uint8_t BlockAddr, uint8_t *pBuffer)
{
	uint8_t Index, Resualt;
	uint16_t ReceiBits;
	uint8_t Buffer[18];

	Buffer[0] = PICC_WRITE;
	Buffer[1] = BlockAddr;
	CalulateCRC(Buffer, 2, &Buffer[2]);
	Resualt = CommunicateWithIC(Transceive, Buffer, 4, Buffer, &ReceiBits);

	if ((Resualt != MI_OK) || (ReceiBits != 4) || ((Buffer[0] & 0x0F) != 0x0A))
		Resualt = MI_ERROR;

	if (Resualt == MI_OK)
	{         
		for (Index = 0; Index < 16; Index ++)
			Buffer[Index] = *(pBuffer + Index);

		CalulateCRC(Buffer, 16, &Buffer[16]);         
		Resualt = CommunicateWithIC(Transceive, Buffer, 18, Buffer, &ReceiBits);           
		if ((Resualt != MI_OK) || (ReceiBits != 4) || ((Buffer[0] & 0x0F) != 0x0A))               
			Resualt = MI_ERROR;         
	}          
	return Resualt;
}

/**
  * @brief  Receive a block of data from IC Card by MFRC522.
  * @param  BlockAddr : the address of block.
  * @param  *pBuffer : pointer to the buffer that receives the data read from the IC Card.
  * @retval The results of receive.
  */
uint8_t MFRC522_RX_Packet(uint8_t BlockAddr, uint8_t *pBuffer)
{
	uint8_t Resualt;
	uint16_t ReceiBits;

	pBuffer[0] = PICC_READ;
	pBuffer[1] = BlockAddr;
	CalulateCRC(pBuffer, 2, &pBuffer[2]);
	Resualt = CommunicateWithIC(Transceive, pBuffer, 4, pBuffer, &ReceiBits);

	if ((Resualt != MI_OK) || (ReceiBits != 0x90))
		Resualt = MI_ERROR;
	return Resualt;
}

/**
  * @brief  Make the card enter to halt mode.
  * @param  None
  * @retval None
  */
void MFRC522_EnterHalt(void)
{
	uint16_t ReceiBits;
	uint8_t Buffer[4];

	Buffer[0] = PICC_HALT;
	Buffer[1] = 0;
	CalulateCRC(Buffer, 2, &Buffer[2]);
	CommunicateWithIC(Transceive, Buffer, 4, Buffer, &ReceiBits);
}

/**
  * @brief  Try to communicate with IC card.
  * @param  Command : MFRC522 command.
  * @param  *pTransData : pointer to the buffer where containing the data to transmit.
  * @param  TransLen : the length of data.
  * @param  *pReceiData : pointer to the buffer that receives the data read from the IC Card.
  * @param  ReceiLen : the length of received data.
  * @retavl Communication results.
  */
uint8_t CommunicateWithIC(uint8_t Command, uint8_t *pTransData, uint8_t TransLen, uint8_t *pReceiData, uint16_t *ReceiLen)
{
	uint8_t IRqEn = 0x00, WaitIRq = 0x00;
	uint8_t Index = 0, Temp, Resualt;
	uint8_t LastBits = 0;
	uint16_t WaitTime = COMMUNI_WAIT_TIME;

	switch(Command)
	{
		case MFAuthent:
			IRqEn = 0x12;
			WaitIRq = 0x10;
		break;
		case Transceive:
			IRqEn = 0x77;
			WaitIRq = 0x30;
		break;
		default :
			Resualt = MI_NOTAGREE;
			return Resualt;
	}
	MFRC522_SPI_RW_Reg(CommIEnReg, IRqEn | 0x80);
	ClearBitMask(CommIRqReg, 0x80);
	SetBitMask(FIFOLevelReg, 0x80);
	MFRC522_SPI_RW_Reg(CommandReg, IDLE);/* make MFRC522 idle */
	/* Write data to FIFO */
	for(Index = 0; Index < TransLen; Index ++)
	{
		MFRC522_SPI_RW_Reg(FIFODataReg, *pTransData);
		pTransData ++;
	}
	/* Execute user commands */
	MFRC522_SPI_RW_Reg(CommandReg, Command);
	/* Use antenna to transmit the data */
	if(Command == Transceive)
		SetBitMask(BitFramingReg, 0x80);/* transmission of data starts */
	do/* Wait for up to 25ms(MAX) */
	{
		Temp = MFRC522_SPI_Read_Reg(CommIRqReg);
		WaitTime --;
	}while(WaitTime && !(Temp & 0x01) && !(Temp & WaitIRq));
	if(Command == Transceive)
		ClearBitMask(BitFramingReg, 0x80);/* transmission of data stops */
	if(WaitTime != 0)
	{
		if(!(MFRC522_SPI_Read_Reg(ErrorReg) & 0x1B))
		{
			if(Temp & IRqEn & 0x01)
				Resualt = MI_NOTAGREE;/* useful??? */
			if(Command == Transceive)
			{
				Temp = MFRC522_SPI_Read_Reg(FIFOLevelReg);
				LastBits = MFRC522_SPI_Read_Reg(ControlReg) & 0x07;
				if(LastBits)
					*ReceiLen = (Temp - 1) * 8 + LastBits;
				else
					*ReceiLen = Temp * 8;
				if(!Temp) Temp = 1;
				if(Temp > MAX_LEN) Temp = MAX_LEN;
				for(Index = 0; Index < Temp; Index ++)
				{
					*pReceiData = MFRC522_SPI_Read_Reg(FIFODataReg);
					pReceiData ++;
				}
			}
			Resualt = MI_OK;
		}
		else
			Resualt = MI_ERROR;
	}
	SetBitMask(ControlReg, 0x80);
	MFRC522_SPI_RW_Reg(CommandReg, IDLE);
	return Resualt;
}

/**
  * @brief  Writes one byte to the MFRC522.
  * @param  Reg : MFRC522's internal address to write to.
  * @param  the data to be written to the MFRC522.
  * @retval The register status.
  */
uint8_t MFRC522_SPI_RW_Reg(uint8_t Reg, uint8_t Value)
{
	uint8_t status;

	/* Set chip select Low at the start of the transmission */
	MFRC522_NSS_LOW();
	/* select register */
	status = MFRC522_SendByte((Reg << 1) & 0x7E);
	/* ..and write value to it.. */ 
	MFRC522_SendByte(Value);
	/* Set chip select High at the end of the transmission */ 
	MFRC522_NSS_HIGH();

	return(status);
}

/**
  * @brief  Reads one byte from the MFRC522.
  * @param  Reg : MFRC522's internal address to read from.
  * @retval The data read from the MFRC522.
  */
uint8_t MFRC522_SPI_Read_Reg(uint8_t Reg)
{
	uint8_t reg_val;

	/* NSS low, initialize SPI communication... */
	MFRC522_NSS_LOW();
	/* Select register to read from.. */
	MFRC522_SendByte(((Reg << 1) & 0x7E) | 0x80);
	/* ..then read registervalue */
	reg_val = MFRC522_SendByte(DUMMY_BYTE);
	/* NSS high, terminate SPI communication */
	MFRC522_NSS_HIGH();

	/* return register value */
	return(reg_val);
}

/**
  * @brief  Calculation of CRC using MFRC522.
  * @param  *pIndata : Input data.
  * @param  Length : Data length.
  * @param  *pOutData : Output data.
  * @retval None
  */
static void CalulateCRC(uint8_t *pIndata, uint8_t Length, uint8_t *pOutData)
{     
	uint16_t Index;
	uint8_t Temp;

	ClearBitMask(DivIRqReg, 0x04);
	SetBitMask(FIFOLevelReg, 0x80);
	MFRC522_SPI_RW_Reg(CommandReg, IDLE);
	/* Write data to FIFO */
	for (Index = 0; Index < Length; Index ++)
		MFRC522_SPI_RW_Reg(FIFODataReg, *(pIndata + Index));
	/* Start calulate CRC */
	MFRC522_SPI_RW_Reg(CommandReg, CalcCRC);
	Index = CAL_CRC_WAIT_TIME;
	do
	{
		Temp = MFRC522_SPI_Read_Reg(DivIRqReg);
		Index --;
	}while ((Index != 0) && !(Temp & 0x04));
	/* Read the results */
	*pOutData = MFRC522_SPI_Read_Reg(CRCResultRegL);
	*(pOutData + 1) = MFRC522_SPI_Read_Reg(CRCResultRegH);
	MFRC522_SPI_RW_Reg(CommandReg, IDLE);
}

/**
  * @brief  Set register bit.
  * @param  Reg : MFRC522's internal address to write to.
  * @param  Mask : the register bit to be set.
  * @retval None
  */
static void SetBitMask(uint8_t Reg, uint8_t Mask)
{
	uint8_t Temp;
	
	Temp = MFRC522_SPI_Read_Reg(Reg);
	MFRC522_SPI_RW_Reg(Reg, Temp | Mask);
}

/**
  * @brief  Clear register bit.
  * @param  Reg : MFRC522's internal address to write to.
  * @param  Mask : the register to be clear.
  * @retval None
  */
static void ClearBitMask(uint8_t Reg, uint8_t Mask)
{
	uint8_t Temp;
	
	Temp = MFRC522_SPI_Read_Reg(Reg);
	MFRC522_SPI_RW_Reg(Reg, Temp & (~Mask));
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received
  *         from the SPI bus.
  * @param  Byte : Byte send.
  * @retval The received byte value.
  */
static uint8_t MFRC522_SendByte(uint8_t Value)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t Retval;
  status = HAL_SPI_TransmitReceive(&hMFRC522_Spi, (uint8_t *)&Value, (uint8_t *)&Retval, 1, MFRC522Timeout);
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPIx_Error();
  }
  return Retval;
}

/**
  * @brief  Insert a short delay time.
  * @param  Delay: delay time.
  * @retval None
  */
static void RC_Delay(uint8_t Delay)
{
	while(Delay --);
}

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t MFRC522_TIMEOUT_UserCallback(void)
{
	/* Block communication and all processes */
	while(1);
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

/*************************************** END OF FILE ***************************************/
