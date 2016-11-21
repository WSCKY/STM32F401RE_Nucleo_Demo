/**
  ******************************************************************************
  * @file    main.h 
  * @author  '^_^'
  * @version V0.0.0
  * @date    29-September-2014
  * @brief   Header for stm32f4xx_nucleo_ExtLCDBK.c module
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXTLCDBK_H
#define __STM32F4XX_NUCLEO_EXTLCDBK_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define BK_LED_GPIO_PORT                    GPIOA
#define BK_LED_GPIO_PIN                     GPIO_PIN_15
#define BK_LED_GPIO_CLK_ENABLE()          __GPIOA_CLK_ENABLE()
#define BK_LED_GPIO_CLK_DISABLE()         __GPIOA_CLK_DISABLE()

/* Definition for TIMx clock resources */
#define TIMx                                TIM2
#define TIMx_CLK_ENABLE                   __TIM2_CLK_ENABLE
#define TIMx_LED_CHANNEL                    TIM_CHANNEL_1

#define  PERIOD_VALUE                       (100 - 1)  /* Period Value  */
#define  PULSE1_DEFAULT_VALUE               5          /* Capture Compare 1 Value(5%)  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t BK_LED_TIMx_Config(TIM_HandleTypeDef *TimHandle);
void BK_LED_Set(TIM_HandleTypeDef *TimHandle, uint8_t BKValue);

#endif /* __STM32F4XX_NUCLEO_EXTLCDBK_H */

/*************************************** END OF FILE ***************************************/
