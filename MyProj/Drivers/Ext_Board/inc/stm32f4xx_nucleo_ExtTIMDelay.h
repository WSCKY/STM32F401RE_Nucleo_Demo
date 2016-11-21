/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtTIMDelay.h 
  * @author  '^_^'
  * @version V0.0.0
  * @date    29-September-2014
  * @brief   Header for stm32f4xx_nucleo_ExtTIMDelay.c module.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXTTIMDELAY_H
#define __STM32F4XX_NUCLEO_EXTTIMDELAY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* User can use this section to tailor TIMx instance used and associated 
   resources */
/* Definition for TIMx clock resources */
#define DELAY_TIMx                           TIM4
#define DELAY_TIMx_CLK_ENABLE()            __TIM4_CLK_ENABLE()

/* Definition for TIMx's NVIC */
#define DELAY_TIMx_IRQn                      TIM4_IRQn
#define DELAY_TIMx_IRQHandler                TIM4_IRQHandler

/* Exported functions ------------------------------------------------------- */
uint8_t Delay_TIM_Init(void);
void TIM_Delay_10us(__IO uint32_t Delay);
void Delay_TIM_IRQHandler(void);


#endif /* __STM32F4XX_NUCLEO_EXTTIMDELAY_H */

/*************************************** END OF FILE ***************************************/
