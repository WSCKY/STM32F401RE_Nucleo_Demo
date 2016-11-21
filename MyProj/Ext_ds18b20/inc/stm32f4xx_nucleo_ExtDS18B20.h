/******************************************************************************
  * @file    stm32f4xx_nucleo_ExtDS18B20.h
  * @author  '^_^'
  * @version V0.0.0
  * @date    30-September-2014
  * @brief   The header file for stm32f4xx_nucleo_ExtDS18B20.c
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXTDS18B20_H
#define __STM32F4XX_NUCLEO_EXTDS18B20_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_ExtTIMDelay.h"

/**
  * @brief  DS18B20 Interface pins definition
  */
#define DS18B20_PIN                  GPIO_PIN_2
#define DS18B20_GPIO_PORT            GPIOB
#define DS18B20_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()
#define DS18B20_GPIO_CLK_DISABLE() __GPIOB_CLK_DISABLE()

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define DS18B20_GPIO_MODE_OUT()     {DS18B20_GPIO_PORT->MODER &= ~(GPIO_MODER_MODER0 << 4);\
                                     DS18B20_GPIO_PORT->MODER |= ((GPIO_MODE_OUTPUT_PP) << 4);}
#define DS18B20_GPIO_MODE_IN()      {DS18B20_GPIO_PORT->MODER &= ~(GPIO_MODER_MODER0 << 4);\
                                     DS18B20_GPIO_PORT->MODER |= ((GPIO_MODE_INPUT) << 4);}

#define DS18B20_ReadPin()           (DS18B20_GPIO_PORT->IDR & DS18B20_PIN)
									 
#define DS18B20_LOW()               (DS18B20_GPIO_PORT->BSRRH |= DS18B20_PIN)
#define DS18B20_HIGH()              (DS18B20_GPIO_PORT->BSRRL |= DS18B20_PIN)
/* Exported functions ------------------------------------------------------- */
uint8_t DS18B20_Init(void);
float DS18B20_Get_Temp(void);

#endif

/*************************************** END OF FILE ***************************************/
