/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_Ext25Q16.h
  * @author  '^_^'
  * @version V0.0.0
  * @date    25-September-2014
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32f4xx_nucleo_Ext25Q16.c driver.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXT25Q16_H
#define __STM32F4XX_NUCLEO_EXT25Q16_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"

#define Delay HAL_Delay

typedef enum {Init_N = 0, Init_Y = !Init_N} InitStat;

/**
  * @brief  W25Q16 Type Definition
  */
#define W25Q16                    0XEF14

/**
  * @brief  W25Q16 Register Definition
  */
#define W25Q16_WriteEnable        0x06 
#define W25Q16_WriteDisable       0x04 
#define W25Q16_ReadStatusReg      0x05 
#define W25Q16_WriteStatusReg     0x01 
#define W25Q16_ReadData           0x03 
#define W25Q16_FastReadData       0x0B 
#define W25Q16_FastReadDual       0x3B 
#define W25Q16_PageProgram        0x02 
#define W25Q16_BlockErase         0xD8 
#define W25Q16_SectorErase        0x20 
#define W25Q16_ChipErase          0xC7 
#define W25Q16_PowerDown          0xB9 
#define W25Q16_ReleasePowerDown   0xAB 
#define W25Q16_DeviceID           0xAB 
#define W25Q16_ManufactDeviceID   0x90 
#define W25Q16_JedecDeviceID      0x9F

void W25Q16_Init(InitStat s);
uint8_t SPI_Flash_ReadSR(void);
void SPI_FLASH_Write_SR(uint8_t SR);
void SPI_FLASH_Write_Enable(void);
void SPI_FLASH_Write_Disable(void);
uint16_t SPI_Flash_ReadID(void);
void SPI_Flash_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void SPI_Flash_Write_Page(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_Flash_Write_NoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_Flash_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_Flash_Erase_Chip(void);
void SPI_Flash_Erase_Sector(uint32_t Dst_Addr);
void SPI_Flash_Wait_Busy(void);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);

#endif /* __STM32F4XX_NUCLEO_EXT25Q16_H */

/*************************************** END OF FILE ***************************************/
