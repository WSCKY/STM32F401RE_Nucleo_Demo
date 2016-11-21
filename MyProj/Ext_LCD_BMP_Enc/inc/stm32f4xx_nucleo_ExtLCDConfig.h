/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtLCDConfig.h
  * @author  '^_^'
  * @version V0.0.0
  * @date    5-October-2014
  * @brief   LCD Extern Function Configuration.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXTLCDCONFIG_H
#define __STM32F4XX_NUCLEO_EXTLCDCONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#define SUPPORTCHINESE              1
#define PICDECODE                   1

#if (SUPPORTCHINESE == 1)

  #define FROMFAT                   0

  #if (FROMFAT == 0)
    #define FontADDRESS	            0x00
  #else
    #define FilePath                "0:/GBK12.DZK"
  #endif

#endif

#endif /* __STM32F4XX_NUCLEO_EXTLCDCONFIG_H */
