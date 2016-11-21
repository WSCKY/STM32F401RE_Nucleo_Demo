/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtTIMIC.h 
  * @author  '^_^'
  * @version V0.0.0
  * @date    1-October-2014
  * @brief   Header for stm32f4xx_nucleo_ExtTIMIC.c module
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXTTIMIC_H
#define __STM32F4XX_NUCLEO_EXTTIMIC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* User can use this section to tailor TIMx instance used and associated 
   resources */
/* Definition for TIMx clock resources */
#define IC_TIMx                             TIM5
#define IC_TIMx_CLK_ENABLE()              __TIM5_CLK_ENABLE()

/* Definition for TIMx Pins */
#define IC_TIMx_CHANNEL_GPIO_PORT()       __GPIOA_CLK_ENABLE()
#define IC_GPIO_PORT                        GPIOA
#define IC_GPIO_PIN_CHANNEL1                GPIO_PIN_0
#define IC_GPIO_AF_TIMx                     GPIO_AF2_TIM5

/* Definition for TIMx's NVIC */
#define IC_TIMx_IRQn                        TIM5_IRQn
#define IC_TIMx_IRQHandler                  TIM5_IRQHandler

/* Exported functions ------------------------------------------------------- */
uint8_t TIM_IC_Init(void);
void GetPWMParam(__IO uint32_t *DutyCycle, __IO uint32_t *Frequency);
void IC_IRQHandler(void);

#endif /* __STM32F4XX_NUCLEO_EXTTIMIC_H */

/*************************************** END OF FILE ***************************************/
