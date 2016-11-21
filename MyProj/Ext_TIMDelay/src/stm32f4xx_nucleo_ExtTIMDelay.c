/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtTIMDelay.c
  * @author  '^_^'
  * @version V0.0.0
  * @date    29-September-2014
  * @brief   Delay function Implementation by the TIM4.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_ExtTIMDelay.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t IncTick;
TIM_HandleTypeDef DelayTimHandle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Delay Timer Init.
  * @param  None
  * @retval Init Status.
  */
uint8_t Delay_TIM_Init(void)
{
	uint32_t uwPrescalerValue = 0;
	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIMx Peripheral clock enable */
	DELAY_TIMx_CLK_ENABLE();
  
	/*##-2- Configure the NVIC for TIMx ########################################*/
	/* Set Interrupt Group Priority */ 
	HAL_NVIC_SetPriority(DELAY_TIMx_IRQn, 4, 0);

	/* Enable the TIMx global Interrupt */
	HAL_NVIC_EnableIRQ(DELAY_TIMx_IRQn);

	/*##-1- Configure the TIM peripheral #######################################*/ 
	/* -----------------------------------------------------------------------
	  In this example TIM4 input clock (TIM4CLK) is set to 2 * APB1 clock (PCLK1),
	   since APB1 prescaler is different from 1.
		  TIM4CLK = 2 * PCLK1
		  PCLK1 = HCLK / 2
		  => TIM4CLK = HCLK = SystemCoreClock
	  To get TIM4 counter clock at 1 MHz, the Prescaler is computed as following:
	   Prescaler = (TIM4CLK / TIM4 counter clock) - 1
	   Prescaler = ((SystemCoreClock) /1 MHz) - 1

	  Note:
	  SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
	   Each time the core clock (HCLK) changes, user had to update SystemCoreClock 
	   variable value. Otherwise, any configuration based on this variable will be incorrect.
	  This variable is updated in three ways:
	   1) by calling CMSIS function SystemCoreClockUpdate()
	   2) by calling HAL API function HAL_RCC_GetSysClockFreq()
	   3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency  
	----------------------------------------------------------------------- */  

	/* Compute the prescaler value to have TIM4 counter clock equal to 1 MHz */
	uwPrescalerValue = (uint32_t)((SystemCoreClock / 1000000) - 1);

	/* Set TIMx instance */
	DelayTimHandle.Instance = DELAY_TIMx;

	/* Initialize TIM4 peripheral as follow:
		+ Period = 10 - 1
		+ Prescaler = ((SystemCoreClock)/1000000) - 1
		+ ClockDivision = 0
		+ Counter direction = Up
	*/
	DelayTimHandle.Init.Period = 9;
	DelayTimHandle.Init.Prescaler = uwPrescalerValue;
	DelayTimHandle.Init.ClockDivision = 0;
	DelayTimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_Base_Init(&DelayTimHandle) != HAL_OK)
		return 1;
	return 0;
}

/**
  * @brief  This function provides accurate delay (in 10 microseconds) based 
  *           on variable incremented.
  * @param  Delay: specifies the delay time length, in 10 microseconds.
  * @retval None
  */
void TIM_Delay_10us(__IO uint32_t Delay)
{
	IncTick = Delay;
	/*##-2- Start the TIM Base generation in interrupt mode ####################*/
	HAL_TIM_Base_Start_IT(&DelayTimHandle);
	while(IncTick);
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(IncTick > 0)
		IncTick --;
	else
		HAL_TIM_Base_Stop_IT(htim);
}

/**
  * @brief  This function used be called to handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void Delay_TIM_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&DelayTimHandle);
}

/*************************************** END OF FILE ***************************************/
