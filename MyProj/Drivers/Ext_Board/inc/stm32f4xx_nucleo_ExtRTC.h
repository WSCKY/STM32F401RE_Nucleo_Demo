/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtRTC.h 
  * @author  '^_^'
  * @version V0.0.0
  * @date    4-October-2014
  * @brief   Header for stm32f4xx_nucleo_ExtRTC.c module
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXTRTC_H
#define __STM32F4XX_NUCLEO_EXTRTC_H

#include "stm32f4xx_hal.h"
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t RTC_Config(void);
void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate);
void RTC_GetTime(RTC_DateTypeDef *sDate, RTC_TimeTypeDef *sTime);
uint8_t RTC_TimeConfig(RTC_DateTypeDef *sDate, RTC_TimeTypeDef *sTime);

#endif

/*************************************** END OF FILE ***************************************/
