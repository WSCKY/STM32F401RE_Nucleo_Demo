/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtLCDBK.c
  * @author  '^_^'
  * @version V0.0.0
  * @date    29-September-2014
  * @brief   LCD BackLight LED in TIM PWM OutPut mode.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_ExtLCDBK.h"

static void TIMx_PWM_MspInit(TIM_HandleTypeDef *htim);

/**
  * @brief TIM MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param htim: TIM handle pointer
  * @retval None
  */
static void TIMx_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct;
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  TIMx_CLK_ENABLE();
    
  /* Enable GPIO Channels Clock */
  BK_LED_GPIO_CLK_ENABLE();

  /* Common configuration for channel1 */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  
  GPIO_InitStruct.Pin = BK_LED_GPIO_PIN;
  HAL_GPIO_Init(BK_LED_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief  Configure the BackLight LED Timer.(default pulse: 50%)
  * @param  TimHandle: pointer to a TIM_HandleTypeDef structure that contains
  *                the configuration information for TIM module.
  * @retval Init status.
  */
uint8_t BK_LED_TIMx_Config(TIM_HandleTypeDef *TimHandle)
{
  /* Timer Output Compare Configuration Structure declaration */
  TIM_OC_InitTypeDef sConfig;
  /* Counter Prescaler value */
  uint32_t uhPrescalerValue = 0;
  /* Compute the prescaler value to have TIM2 counter clock equal to 100 KHz */
  uhPrescalerValue = (uint32_t) (SystemCoreClock / 100000) - 1;

  TIMx_PWM_MspInit(TimHandle);

  /*##-1- Configure the TIM peripheral #######################################*/ 
  /* -----------------------------------------------------------------------
  TIM2 Configuration: generate 1 PWM signals with 1 duty cycle.

    In this File TIM2 input clock (TIM2CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.
      TIM2CLK = 2 * PCLK1
      PCLK1 = HCLK / 2
      => TIM2CLK = HCLK = SystemCoreClock

    To get TIM2 counter clock at 100 KHz, the prescaler is computed as follows:
       Prescaler = (TIM2CLK / TIM2 counter clock) - 1
       Prescaler = ((SystemCoreClock) /100 KHz) - 1

    To get TIM2 output clock at 1 KHz, the period (ARR)) is computed as follows:
       ARR = (TIM2 counter clock / TIM2 output clock) - 1
           = 99

    TIM2 Channel1 duty cycle = (TIM2_CCR1/ TIM2_ARR)* 100 = 5%

    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock 
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency     
  ----------------------------------------------------------------------- */ 

  /* Initialize TIMx peripheral as follow:
       + Prescaler = (SystemCoreClock)/100000
       + Period = 99
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle->Instance = TIMx;

  TimHandle->Init.Prescaler = uhPrescalerValue;
  TimHandle->Init.Period = PERIOD_VALUE;
  TimHandle->Init.ClockDivision = 0;
  TimHandle->Init.CounterMode = TIM_COUNTERMODE_UP;
  if(HAL_TIM_PWM_Init(TimHandle) != HAL_OK)
	  return 1;

  /*##-2- Configure the PWM channels #########################################*/ 
  /* Common configuration for channel */
  sConfig.OCMode = TIM_OCMODE_PWM1;
  sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfig.OCFastMode = TIM_OCFAST_DISABLE;

  /* Set the pulse value for channel 1 */
  sConfig.Pulse = PULSE1_DEFAULT_VALUE;
  if(HAL_TIM_PWM_ConfigChannel(TimHandle, &sConfig, TIMx_LED_CHANNEL) != HAL_OK)
	  return 1;

  /*##-3- Start PWM signals generation #######################################*/ 
  /* Start channel 1 */
  if(HAL_TIM_PWM_Start(TimHandle, TIMx_LED_CHANNEL) != HAL_OK)
	  return 1;
  return 0;
}

/**
  * @brief  Set the backlight brightness.
  * @param  TimHandle: pointer to a TIM_HandleTypeDef structure that contains
  *                the configuration information for TIM module.
  * @param  BKValue: Specify the brightness
  *           Note: this value should be 0-100.
  * @retval None
  */
void BK_LED_Set(TIM_HandleTypeDef *TimHandle, uint8_t BKValue)
{
	TimHandle->Instance->CCR1 = BKValue;
}

/*************************************** END OF FILE ***************************************/
