/******************************************************************************
  * @file    stm32f4xx_nucleo_ExtInfr_Rot.h
  * @author  '^_^'
  * @version V0.0.0
  * @date    3-October-2014
  * @brief   The header file for stm32f4xx_nucleo_ExtInfr_Rot.c
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXTINFR_ROT_H
#define __STM32F4XX_NUCLEO_EXTINFR_ROT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/**
  * @brief  VS1838B Interface pins definition
  */
#define VS1838B_PIN                  GPIO_PIN_1
#define VS1838B_GPIO_PORT            GPIOB
#define VS1838B_GPIO_CLK_ENABLE()  __GPIOB_CLK_ENABLE()
#define VS1838B_GPIO_CLK_DISABLE() __GPIOB_CLK_DISABLE()
#define VS1838B_GPIO_AF_TIMx         GPIO_AF2_TIM3

/* Definition for TIMx clock resources */
#define VS_TIMx                      TIM3
#define VS_TIMx_CLK_ENABLE()       __TIM3_CLK_ENABLE()
#define VS_TIMx_CHANNEL              TIM_CHANNEL_4

/* Definition for TIMx's NVIC */
#define VS_TIMx_IRQn                 TIM3_IRQn
#define VS_TIMx_IRQHandler           TIM3_IRQHandler

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint8_t VS1838B_Prep(void);
uint8_t Get_Infrared(uint32_t *Value);
void VS_IRQHandler(void);

#endif /* __STM32F4XX_NUCLEO_EXTINFR_ROT_H */

/*************************************** END OF FILE ***************************************/
