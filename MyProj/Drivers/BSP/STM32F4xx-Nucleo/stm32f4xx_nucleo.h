/** 
  ******************************************************************************
  * @file    stm32f4xx_nucleo.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-February-2014
  * @brief   This file contains definitions for STM32F4xx-Nucleo Kit's Leds and 
  *          push-button hardware resources.
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
#ifndef __STM32F4XX_NUCLEO_H
#define __STM32F4XX_NUCLEO_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
   
/** @addtogroup BSP
  * @{
  */
  
/** @addtogroup STM32F4XX_NUCLEO
  * @{
  */
      
/** @addtogroup STM32F4XX_NUCLEO_LOW_LEVEL
  * @{
  */ 

/** @defgroup STM32F4XX_NUCLEO_LOW_LEVEL_Exported_Types STM32F4XX_NUCLEO_LOW_LEVEL_Exported_Types
  * @{
  */
typedef enum 
{
  LED2 = 0,
  LED3 = 1,
  LED4 = 2,
  LED5 = 3,
  LED6 = 4
} Led_TypeDef;

typedef enum 
{  
  BUTTON_KEY = 0,
  BUTTON1 = 1,
  BUTTON2 = 2,
  BUTTON3 = 3,
  BUTTON4 = 4
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;     

/**
  * @}
  */ 

/** @defgroup STM32F4XX_NUCLEO_LOW_LEVEL_Exported_Constants STM32F4XX_NUCLEO_LOW_LEVEL_Exported_Constants
  * @{
  */ 

/** 
* @brief	Define for STM32F4XX_NUCLEO board  
*/ 
#if !defined (USE_STM32F4XX_NUCLEO)
 #define USE_STM32F4XX_NUCLEO
#endif

/** @addtogroup STM32F4XX_NUCLEO_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             5

#define LED2_PIN                         GPIO_PIN_5
#define LED2_GPIO_PORT                   GPIOA
#define LED2_GPIO_CLK_ENABLE()           __GPIOA_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          __GPIOA_CLK_DISABLE()

#define LED3_PIN                         GPIO_PIN_8
#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK_ENABLE()           __GPIOC_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          __GPIOC_CLK_DISABLE()

#define LED4_PIN                         GPIO_PIN_9
#define LED4_GPIO_PORT                   GPIOC
#define LED4_GPIO_CLK_ENABLE()           __GPIOC_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          __GPIOC_CLK_DISABLE()

#define LED5_PIN                         GPIO_PIN_6
#define LED5_GPIO_PORT                   GPIOC
#define LED5_GPIO_CLK_ENABLE()           __GPIOC_CLK_ENABLE()
#define LED5_GPIO_CLK_DISABLE()          __GPIOC_CLK_DISABLE()

#define LED6_PIN                         GPIO_PIN_7
#define LED6_GPIO_PORT                   GPIOC
#define LED6_GPIO_CLK_ENABLE()           __GPIOC_CLK_ENABLE()
#define LED6_GPIO_CLK_DISABLE()          __GPIOC_CLK_DISABLE()

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)   (((__INDEX__) == 0) ? LED2_GPIO_CLK_ENABLE() : LED6_GPIO_CLK_ENABLE())
#define LEDx_GPIO_CLK_DISABLE(__INDEX__)  (((__INDEX__) == 0) ? LED2_GPIO_CLK_DISABLE() : LED6_GPIO_CLK_DISABLE())
/**
  * @}
  */ 
  
/** @addtogroup STM32F4XX_NUCLEO_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                          5

/**
 * @brief Wakeup push-button
 */
#define KEY_BUTTON_PIN                       GPIO_PIN_13
#define KEY_BUTTON_GPIO_PORT                 GPIOC
#define KEY_BUTTON_GPIO_CLK_ENABLE()         __GPIOC_CLK_ENABLE()
#define KEY_BUTTON_GPIO_CLK_DISABLE()        __GPIOC_CLK_DISABLE()
#define KEY_BUTTON_EXTI_IRQn                 EXTI15_10_IRQn

#define BUTTON1_PIN                          GPIO_PIN_5
#define BUTTON1_GPIO_PORT                    GPIOC
#define BUTTON1_GPIO_CLK_ENABLE()            __GPIOC_CLK_ENABLE()
#define BUTTON1_GPIO_CLK_DISABLE()           __GPIOC_CLK_DISABLE()
#define BUTTON1_EXTI_IRQn                    EXTI9_5_IRQn

#define BUTTON2_PIN                          GPIO_PIN_4
#define BUTTON2_GPIO_PORT                    GPIOC
#define BUTTON2_GPIO_CLK_ENABLE()            __GPIOC_CLK_ENABLE()
#define BUTTON2_GPIO_CLK_DISABLE()           __GPIOC_CLK_DISABLE()
#define BUTTON2_EXTI_IRQn                    EXTI4_IRQn

#define BUTTON3_PIN                          GPIO_PIN_9
#define BUTTON3_GPIO_PORT                    GPIOA
#define BUTTON3_GPIO_CLK_ENABLE()            __GPIOA_CLK_ENABLE()
#define BUTTON3_GPIO_CLK_DISABLE()           __GPIOA_CLK_DISABLE()
#define BUTTON3_EXTI_IRQn                    EXTI9_5_IRQn

#define BUTTON4_PIN                          GPIO_PIN_10
#define BUTTON4_GPIO_PORT                    GPIOA
#define BUTTON4_GPIO_CLK_ENABLE()            __GPIOA_CLK_ENABLE()
#define BUTTON4_GPIO_CLK_DISABLE()           __GPIOA_CLK_DISABLE()
#define BUTTON4_EXTI_IRQn                    EXTI15_10_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)    (((__INDEX__) < BUTTON3) ? KEY_BUTTON_GPIO_CLK_ENABLE() : \
                                                                            BUTTON4_GPIO_CLK_ENABLE())
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__)   (((__INDEX__) < BUTTON3) ? KEY_BUTTON_GPIO_CLK_DISABLE() : \
                                                                            BUTTON4_GPIO_CLK_DISABLE())
/**
  * @}
  */ 

/** @addtogroup STM32F4XX_NUCLEO_LOW_LEVEL_BUS
  * @{
  */  
/*############################### SPI2 #######################################*/
#define NUCLEO_SPIx                                     SPI2
#define NUCLEO_SPIx_CLK_ENABLE()                        __SPI2_CLK_ENABLE()

#define NUCLEO_SPIx_SCK_AF                              GPIO_AF5_SPI2
#define NUCLEO_SPIx_SCK_GPIO_PORT                       GPIOB
#define NUCLEO_SPIx_SCK_PIN                             GPIO_PIN_13
#define NUCLEO_SPIx_SCK_GPIO_CLK_ENABLE()               __GPIOB_CLK_ENABLE()
#define NUCLEO_SPIx_SCK_GPIO_CLK_DISABLE()              __GPIOB_CLK_DISABLE()

#define NUCLEO_SPIx_MISO_MOSI_AF                        GPIO_AF5_SPI2
#define NUCLEO_SPIx_MISO_MOSI_GPIO_PORT                 GPIOB
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_ENABLE()         __GPIOB_CLK_ENABLE()
#define NUCLEO_SPIx_MISO_MOSI_GPIO_CLK_DISABLE()        __GPIOB_CLK_DISABLE()
#define NUCLEO_SPIx_MISO_PIN                            GPIO_PIN_14
#define NUCLEO_SPIx_MOSI_PIN                            GPIO_PIN_15
/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define NUCLEO_SPIx_TIMEOUT_MAX                   1000

/**
  * @brief  SD Control Lines management
  */
#define SD_CS_LOW()       HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_RESET)
#define SD_CS_HIGH()      HAL_GPIO_WritePin(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  SD Control Interface pins
  */
#define SD_CS_PIN                                 GPIO_PIN_6
#define SD_CS_GPIO_PORT                           GPIOA
#define SD_CS_GPIO_CLK_ENABLE()                 __GPIOA_CLK_ENABLE()
#define SD_CS_GPIO_CLK_DISABLE()                __GPIOA_CLK_DISABLE()

/**
  * @brief  W25Q16 Control Lines management
  */
#define W25Q16_CS_LOW()  HAL_GPIO_WritePin(W25Q16_CS_GPIO_PORT, W25Q16_CS_PIN, GPIO_PIN_RESET)
#define W25Q16_CS_HIGH() HAL_GPIO_WritePin(W25Q16_CS_GPIO_PORT, W25Q16_CS_PIN, GPIO_PIN_SET)

/**
  * @brief  W25Q16 Control Interface pins
  */
#define W25Q16_CS_PIN                             GPIO_PIN_7
#define W25Q16_CS_GPIO_PORT                       GPIOA
#define W25Q16_CS_GPIO_CLK_ENABLE()             __GPIOA_CLK_ENABLE()
#define W25Q16_CS_GPIO_CLK_DISABLE()            __GPIOA_CLK_DISABLE()

/**
  * @}
  */

/** @defgroup STM32F4XX_NUCLEO_LOW_LEVEL_Exported_Macros STM32F4XX_NUCLEO_LOW_LEVEL_Exported_Macros
  * @{
  */  
/**
  * @}
  */ 


/** @defgroup STM32F4XX_NUCLEO_LOW_LEVEL_Exported_Functions STM32F4XX_NUCLEO_LOW_LEVEL_Exported_Functions
  * @{
  */
uint32_t  BSP_GetVersion(void);  
void      BSP_LED_Init(Led_TypeDef Led);
void      BSP_LED_On(Led_TypeDef Led);
void      BSP_LED_Off(Led_TypeDef Led);
void      BSP_LED_Toggle(Led_TypeDef Led);
void      BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode);
uint32_t  BSP_PB_GetState(Button_TypeDef Button);

/* W25Q16 IO functions */
void W25Q16_IO_Init(void);
void W25Q16_SPI_Init(void);
void W25Q16_IO_WriteByte(uint8_t Data);
uint8_t W25Q16_IO_ReadByte(void);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32F4XX_NUCLEO_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

