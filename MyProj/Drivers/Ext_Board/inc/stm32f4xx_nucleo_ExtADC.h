/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtADC.h
  * @author  '^_^'
  * @version V0.0.0
  * @date    28-September-2014
  * @brief   the header file for stm32f4xx_nucleo_ExtADC.c
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXTADC_H
#define __STM32F4XX_NUCLEO_EXTADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
typedef uint8_t ADCx_CHANNEL_Ax;
/* Exported definition -------------------------------------------------------*/
#define	ADCx_CHANNEL_A0                         (uint8_t)0x01
#define	ADCx_CHANNEL_A1                         (uint8_t)0x02
#define	ADCx_CHANNEL_A2                         (uint8_t)0x04
#define	ADCx_CHANNEL_A3                         (uint8_t)0x08
#define	ADCx_CHANNEL_A4                         (uint8_t)0x10
#define	ADCx_CHANNEL_A5                         (uint8_t)0x20
#define ADCx_CHANNEL_All                        (uint8_t)0x3F

#define ADCx_CHANNEL_MAX                        6
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor ADCx instance used and associated 
   resources */
/* Definition for ADCx clock resources */
#define ADCx                                    ADC1
#define ADCx_CLK_ENABLE()                       __ADC1_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                       __DMA2_CLK_ENABLE()

#define ADCx_FORCE_RESET()                      __ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()                    __ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_ExtA0_PIN                  GPIO_PIN_0
#define ADCx_CHANNEL_ExtA1_PIN                  GPIO_PIN_1
#define ADCx_CHANNEL_ExtA2_PIN                  GPIO_PIN_4
#define ADCx_CHANNEL_ExtA0_2_GPIO_PORT          GPIOA
#define ADCx_CHANNEL_ExtA0_2_GPIO_CLK_ENABLE()  __GPIOA_CLK_ENABLE()

#define ADCx_CHANNEL_ExtA3_PIN                  GPIO_PIN_0
#define ADCx_CHANNEL_ExtA3_GPIO_PORT            GPIOB
#define ADCx_CHANNEL_ExtA3_GPIO_CLK_ENABLE()    __GPIOB_CLK_ENABLE()

#define ADCx_CHANNEL_ExtA4_PIN                  GPIO_PIN_1
#define ADCx_CHANNEL_ExtA5_PIN                  GPIO_PIN_0
#define ADCx_CHANNEL_ExtA4_5_GPIO_PORT          GPIOC
#define ADCx_CHANNEL_ExtA4_5_GPIO_CLK_ENABLE()  __GPIOC_CLK_ENABLE()

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL_ExtA0                      ADC_CHANNEL_0
#define ADCx_CHANNEL_ExtA1                      ADC_CHANNEL_1
#define ADCx_CHANNEL_ExtA2                      ADC_CHANNEL_4
#define ADCx_CHANNEL_ExtA3                      ADC_CHANNEL_8
#define ADCx_CHANNEL_ExtA4                      ADC_CHANNEL_11
#define ADCx_CHANNEL_ExtA5                      ADC_CHANNEL_10

/* Definition for ADCx's DMA */
#define ADCx_DMA_CHANNEL                        DMA_CHANNEL_0
#define ADCx_DMA_STREAM                         DMA2_Stream0         

/* Definition for ADCx's NVIC */
#define ADCx_DMA_IRQn                           DMA2_Stream0_IRQn
#define ADCx_DMA_IRQHandler                     DMA2_Stream0_IRQHandler

/* Exported functions ------------------------------------------------------- */
uint8_t ADC_Ext_Init(ADC_HandleTypeDef* hadc, ADCx_CHANNEL_Ax Channel_Ax, uint32_t NbrOfConv);

#endif /* __STM32F4XX_NUCLEO_EXTADC_H */

/*************************************** END OF FILE ***************************************/
