/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtE2PROM.h
  * @author  '^_^'
  * @version V0.0.0
  * @date    28-September-2014
  * @brief   This file contains all the functions prototypes for 
  *          the stm32f4xx_nucleo_ExtE2PROM.c firmware driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
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
#ifndef __STM32F4XX_NUCLEO_EXTE2PROM_H
#define __STM32F4XX_NUCLEO_EXTE2PROM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported constantIO -------------------------------------------------------*/
/* I2C clock speed configuration (in Hz) 
   WARNING: 
   Make sure that this define is not already declared in other files (ie. 
   stm32f4xx_nucleo.h file). It can be used in parallel by other modules. */
#ifndef I2C_SPEED
 #define I2C_SPEED                            100000
#endif /* I2C_SPEED */

/* For M24C32 devices, E0, E1 and E2 pins are all used for device 
  address selection (no need for additional address lines). According to the 
  Hardware connection on the board (on STM32f4xx-nucleo-Ext board E0 = E1 = E2 = 0) */
#define EEPROM_I2C_ADDRESS                    0xA0

/* User can use this section to tailor I2Cx/I2Cx instance used and associated 
   resources */
/* Definition for I2Cx clock resources */
#define Ext_I2Cx                            I2C1
#define Ext_I2Cx_CLK_ENABLE()               __I2C1_CLK_ENABLE()
#define Ext_DMAx_CLK_ENABLE()               __DMA1_CLK_ENABLE()
#define Ext_I2Cx_SCL_SDA_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()

#define Ext_I2Cx_FORCE_RESET()              __I2C1_FORCE_RESET()
#define Ext_I2Cx_RELEASE_RESET()            __I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define Ext_I2Cx_SCL_PIN                    GPIO_PIN_8
#define Ext_I2Cx_SCL_SDA_GPIO_PORT          GPIOB
#define Ext_I2Cx_SCL_SDA_AF                 GPIO_AF4_I2C1
#define Ext_I2Cx_SDA_PIN                    GPIO_PIN_9

/* I2C interrupt requests */                  
#define Ext_I2Cx_EV_IRQn                    I2C1_EV_IRQn
#define Ext_I2Cx_ER_IRQn                    I2C1_ER_IRQn

#define EEPROM_WP_PIN                       GPIO_PIN_7
#define EEPROM_WP_GPIO_PORT                 GPIOB
#define EEPROM_WP_CLK_ENABLE()            __GPIOB_CLK_ENABLE()
#define EEPROM_WP_CLK_DISABLE()           __GPIOB_CLK_DISABLE()

#define EEPROM_WriteProtect_On()            HAL_GPIO_WritePin(EEPROM_WP_GPIO_PORT, EEPROM_WP_PIN, GPIO_PIN_SET);
#define EEPROM_WriteProtect_Off()           HAL_GPIO_WritePin(EEPROM_WP_GPIO_PORT, EEPROM_WP_PIN, GPIO_PIN_RESET);

/* EEPROM hardware address and page size */ 
#define EEPROM_PAGESIZE             16
#define EEPROM_MAX_SIZE             0x1000 /* 32Kbit, 4K Byte */
/* Maximum Timeout values for flags and events waiting loops: This timeout is based 
   on systick set to 1ms.    
   Timeout for read based if read all the EEPROM: EEPROM_MAX_SIZE * I2C_SPEED (640ms) */
#define EEPROM_READ_TIMEOUT         ((uint32_t)(1000))
/* Timeout for write based on max write which is EEPROM_PAGESIZE bytes: EEPROM_PAGESIZE * I2C_SPEED (320us)*/
#define EEPROM_WRITE_TIMEOUT        ((uint32_t)(1000))

/* Maximum number of trials for EEPROM_WaitEepromStandbyState() function */
#define EEPROM_MAX_TRIALS           3000
      
#define EEPROM_OK                   0
#define EEPROM_FAIL                 1
#define EEPROM_TIMEOUT              2

uint32_t BSP_EEPROM_Init(void);
uint32_t BSP_EEPROM_ReadBuffer(uint8_t *pBuffer, uint16_t ReadAddr, uint16_t *NumByteToRead);
uint32_t BSP_EEPROM_WritePage(uint8_t *pBuffer, uint16_t WriteAddr, uint8_t *NumByteToWrite);
uint32_t BSP_EEPROM_WriteBuffer(uint8_t *pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint32_t BSP_EEPROM_WaitEepromStandbyState(void);

/* USER Callbacks: This function is declared as __weak in EEPROM driver and 
   should be implemented into user application.  
   BSP_EEPROM_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occure during communication (waiting on an event that doesn't occur, bus 
   errors, busy devices ...). */
void     BSP_EEPROM_TIMEOUT_UserCallback(void);

/* Link function for I2C EEPROM peripheral */
void                EEPROM_IO_Init(void);
HAL_StatusTypeDef   EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef   EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef   EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4XX_NUCLEO_EXTE2PROM_H */
                                
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
