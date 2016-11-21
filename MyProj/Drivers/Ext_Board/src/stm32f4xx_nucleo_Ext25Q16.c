/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_Ext25Q16.c
  * @author  '^_^'
  * @version V0.0.0
  * @date    25-September-2014
  * @brief   This file provides a set of functions needed to manage the spi flash
  *          mounted on the ExtBoard 1.77" TFT LCD shield, that is used with the
  *          STM32 Nucleo board through SPI interface. It implements a high level
  *          communication layer for read and write from/to this memory.
  *          The needed STM32XXxx hardware resources (SPI and GPIO) are defined in
  *          stm32XXxx_nucleo.h file.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_Ext25Q16.h"

/**
  * @brief  Initializies the W25Q16.
  * @param  
  * @retval None
  */
void W25Q16_Init(InitStat s)
{
	W25Q16_IO_Init();
	if(s)
		W25Q16_SPI_Init();
}
/**
  * @brief  Read SPI_Flash Status Register
  * @param  None
  * @retval Register value.
  */
uint8_t SPI_Flash_ReadSR(void)
{
	uint8_t data = 0;

	/* W25Q16 chip select low */
	W25Q16_CS_LOW();

	W25Q16_IO_WriteByte(W25Q16_ReadStatusReg);
	data = W25Q16_IO_ReadByte();

	/* W25Q16 chip select high */
	W25Q16_CS_HIGH();
	return data;
}

/**
  * @brief  Write SPI_Flash Status Register.
  * @param  SR : Data to Write.
  * @retval None
  */
void SPI_FLASH_Write_SR(uint8_t SR)
{
	/* W25Q16 chip select low */
	W25Q16_CS_LOW();

	W25Q16_IO_WriteByte(W25Q16_WriteStatusReg);
	W25Q16_IO_WriteByte(SR);

	/* W25Q16 chip select high */
	W25Q16_CS_HIGH();
}

/**
  * @brief  SPI_Flash Write Enable.
  * @param  None
  * @retval None
  */
void SPI_FLASH_Write_Enable(void)
{
	/* W25Q16 chip select low */
	W25Q16_CS_LOW();

    W25Q16_IO_WriteByte(W25Q16_WriteEnable);

	/* W25Q16 chip select high */
	W25Q16_CS_HIGH();
}

/**
  * @brief  SPI_Flash Write Disable.
  * @param  None
  * @retval None
  */
void SPI_FLASH_Write_Disable(void)
{
	/* W25Q16 chip select low */
	W25Q16_CS_LOW();

    W25Q16_IO_WriteByte(W25Q16_WriteDisable);

	/* W25Q16 chip select high */
	W25Q16_CS_HIGH();
}

/**
  * @brief  Read SPI_Flash's ID.
  * @param  None
  * @retval SPI_Flash's ID.
  */
uint16_t SPI_Flash_ReadID(void)
{
	uint16_t Temp = 0;

	/* W25Q16 chip select low */
	W25Q16_CS_LOW();

	W25Q16_IO_WriteByte(0x90);
	W25Q16_IO_WriteByte(0x00);
	W25Q16_IO_WriteByte(0x00);
	W25Q16_IO_WriteByte(0x00);
	Temp = W25Q16_IO_ReadByte();
	Temp <<= 8;
	Temp |= W25Q16_IO_ReadByte();

	/* W25Q16 chip select high */
	W25Q16_CS_HIGH();
	return Temp;
}

/**
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: SPI_FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the SPI FLASH.
  * @retval None
  */
void SPI_Flash_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
 	/* W25Q16 chip select low */
	W25Q16_CS_LOW();

    W25Q16_IO_WriteByte(W25Q16_ReadData);
    W25Q16_IO_WriteByte((ReadAddr & 0xFF0000) >> 16);
    W25Q16_IO_WriteByte((ReadAddr& 0xFF00) >> 8);
    W25Q16_IO_WriteByte(ReadAddr & 0xFF);
	while(NumByteToRead --)/*!< while there is data to be read */
	{
		/*!< Read a byte from the SPI FLASH */
		*pBuffer = W25Q16_IO_ReadByte();
		/*!< Point to the next location where the byte read will be saved */
		pBuffer ++;
	}

	/* W25Q16 chip select high */
	W25Q16_CS_HIGH();
}

/**
  * @brief  Writes a block of data to SPI Flash at specified page.
  * @param  pBuffer: pointer to the buffer that receives the data read from the SPI FLASH.
  * @param  WriteAddr: FLASH's internal address to read from.(<! less than 256 >)
  * @param  NumByteToRead: number of bytes to read from the SPI FLASH.
  * @retval None
  */
void SPI_Flash_Write_Page(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    SPI_FLASH_Write_Enable();

	/* W25Q16 chip select low */
	W25Q16_CS_LOW();

    W25Q16_IO_WriteByte(W25Q16_PageProgram);
    W25Q16_IO_WriteByte((WriteAddr & 0xFF0000) >> 16);
    W25Q16_IO_WriteByte((WriteAddr & 0xFF00) >> 8);
    W25Q16_IO_WriteByte(WriteAddr & 0xFF);

    while(NumByteToWrite --)/*!< while there is data to be write */
	{
		W25Q16_IO_WriteByte(*pBuffer);
		pBuffer ++;
	}

	/* W25Q16 chip select high */
	W25Q16_CS_HIGH();

	SPI_Flash_Wait_Busy();
}

/**
  * @brief  Writes a block of data to SPI Flash(no checksum).
  * @param  pBuffer: pointer to the buffer that receives the data read from the SPI FLASH.
  * @param  WriteAddr: FLASH's internal address to read from.(<! less than 65535 >)
  * @param  NumByteToRead: number of bytes to read from the SPI FLASH.
  * @retval None
  */
void SPI_Flash_Write_NoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint16_t Page_Remain;
	Page_Remain = 256 - WriteAddr % 256;
	if(NumByteToWrite <= Page_Remain)
		Page_Remain = NumByteToWrite;
	while(1)
	{
		SPI_Flash_Write_Page(pBuffer, WriteAddr, Page_Remain);
		if(NumByteToWrite == Page_Remain)/* Write OK. */
			break;
	 	else
		{
			pBuffer += Page_Remain;
			WriteAddr += Page_Remain;
			NumByteToWrite -= Page_Remain;
			if(NumByteToWrite > 256)
				Page_Remain = 256;
			else
				Page_Remain = NumByteToWrite;
		}
	}
}

/**
  * @brief  Writes a block of data to SPI Flash(will erase sector).
  * @param  pBuffer: pointer to the buffer that receives the data read from the SPI FLASH.
  * @param  WriteAddr: FLASH's internal address to read from.(<! less than 65535 >)
  * @param  NumByteToRead: number of bytes to read from the SPI FLASH.
  * @retval None
  */
uint8_t SPI_FLASH_BUF[4096];
void SPI_Flash_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint32_t Sec_Pos;
	uint16_t Sec_Off;
	uint16_t Sec_Remain;
 	uint16_t i;

	Sec_Pos = WriteAddr / 4096;
	Sec_Off = WriteAddr % 4096;
	Sec_Remain = 4096 - Sec_Off;

	if(NumByteToWrite <= Sec_Remain)
		Sec_Remain = NumByteToWrite;
	while(1)
	{
		SPI_Flash_Read(SPI_FLASH_BUF, Sec_Pos * 4096,4096);
		for(i = 0; i < Sec_Remain; i ++)
		{
			if(SPI_FLASH_BUF[Sec_Off + i] != 0XFF)
				break;
		}
		if(i < Sec_Remain)
		{
			SPI_Flash_Erase_Sector(Sec_Pos);
			for(i = 0; i < Sec_Remain; i ++)
			{
				SPI_FLASH_BUF[i + Sec_Off] = pBuffer[i];
			}
			SPI_Flash_Write_NoCheck(SPI_FLASH_BUF, Sec_Pos * 4096, 4096);
		}
		else
			SPI_Flash_Write_NoCheck(pBuffer, WriteAddr, Sec_Remain);
		if(NumByteToWrite == Sec_Remain)
			break;
		else
		{
			Sec_Pos ++;
			Sec_Off = 0;

		   	pBuffer += Sec_Remain;
			WriteAddr += Sec_Remain;
		   	NumByteToWrite -= Sec_Remain;
			if(NumByteToWrite > 4096)
				Sec_Remain = 4096;
			else
				Sec_Remain = NumByteToWrite;
		}
	}
}

/**
  * @brief  Erases the entire FLASH.(approximately 25s ?)
  * @param  None.
  * @retval None.
  */
void SPI_Flash_Erase_Chip(void)
{
    SPI_FLASH_Write_Enable();
    SPI_Flash_Wait_Busy();

  	/* W25Q16 chip select low */
	W25Q16_CS_LOW();

    W25Q16_IO_WriteByte(W25Q16_ChipErase);

	/* W25Q16 chip select high */
	W25Q16_CS_HIGH();

	SPI_Flash_Wait_Busy();
}

/**
  * @brief  Erases the specified FLASH sector(4KB).
  * @param  SectorAddr: address of the sector to erase.(0 - 512)
  * @retval None
  */
void SPI_Flash_Erase_Sector(uint32_t Dst_Addr)
{
	Dst_Addr *= 4096;
    SPI_FLASH_Write_Enable();
    SPI_Flash_Wait_Busy();

  	/* W25Q16 chip select low */
	W25Q16_CS_LOW();

    W25Q16_IO_WriteByte(W25Q16_SectorErase);
    W25Q16_IO_WriteByte((Dst_Addr & 0xFF0000) >> 16);
    W25Q16_IO_WriteByte((Dst_Addr & 0xFF00) >> 8);
    W25Q16_IO_WriteByte(Dst_Addr & 0xFF);

	/* W25Q16 chip select high */
	W25Q16_CS_HIGH();

    SPI_Flash_Wait_Busy();
}

/**
  * @brief  Check SPI Flash Busy.
  * @param  None
  * @retval None
  */
void SPI_Flash_Wait_Busy(void)
{
	while((SPI_Flash_ReadSR() & 0x01) == 0x01);
}

/**
  * @brief  Make SPI Flash Power Down.
  * @param  None
  * @retval None
  */
void SPI_Flash_PowerDown(void)
{
  	/* W25Q16 chip select low */
	W25Q16_CS_LOW();

    W25Q16_IO_WriteByte(W25Q16_PowerDown);

	/* W25Q16 chip select high */
	W25Q16_CS_HIGH();
	/* Delay 1 ms */
    Delay(1);
}

/**
  * @brief  Make SPI Flash Wake Up.
  * @param  None
  * @retval None
  */
void SPI_Flash_WAKEUP(void)
{
	/* W25Q16 chip select low */
	W25Q16_CS_LOW();

	W25Q16_IO_WriteByte(W25Q16_ReleasePowerDown);

	/* W25Q16 chip select high */
	W25Q16_CS_HIGH();
	/* Delay 1 ms */
    Delay(1);
}

/*************************************** END OF FILE ***************************************/
