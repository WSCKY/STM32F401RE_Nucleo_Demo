/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtTIMIC.c 
  * @author  '^_^'
  * @version V0.0.0
  * @date    1-October-2014
  * @brief   This example shows how to use the TIM peripheral to measure the
  *          frequency and duty cycle of an external signal.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_ExtTIMIC.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef        IC_TimHandle;

/* Timer Input Capture Configuration Structure declaration */
TIM_IC_InitTypeDef       sConfig;

/* Slave configuration structure */
TIM_SlaveConfigTypeDef   sSlaveConfig;
/* Captured Value */
__IO uint32_t            uwIC1Value = 0;
/* Duty Cycle Value */
__IO uint32_t            uwDutyCycle = 0;
/* Frequency Value */
__IO uint32_t            uwFrequency = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Timer Input Capture Initialize.
  * @param  None
  * @retval Init status.
  */
uint8_t TIM_IC_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIMx Peripheral clock enable */
	IC_TIMx_CLK_ENABLE();

	/* Enable GPIO channels Clock */
	IC_TIMx_CHANNEL_GPIO_PORT();

	/* Configure  (TIMx_Channel) in Alternate function, push-pull and 100MHz speed */
	GPIO_InitStruct.Pin = IC_GPIO_PIN_CHANNEL1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = IC_GPIO_AF_TIMx;
	HAL_GPIO_Init(IC_GPIO_PORT, &GPIO_InitStruct);

	/*##-2- Configure the NVIC for TIMx #########################################*/
	/* Sets the priority grouping field */
	HAL_NVIC_SetPriority(IC_TIMx_IRQn, 0, 1);

	/* Enable the TIM5 global Interrupt */
	HAL_NVIC_EnableIRQ(IC_TIMx_IRQn);
	/*##-1- Configure the TIM peripheral #######################################*/ 
	/* Set TIMx instance */
	IC_TimHandle.Instance = IC_TIMx;

	/* Initialize TIMx peripheral as follow:
		+ Period = 0xFFFFFFFF(32 bit)
		+ Prescaler = 0
		+ ClockDivision = 0
		+ Counter direction = Up
	*/
	IC_TimHandle.Init.Period = 0xFFFFFFFF;
	IC_TimHandle.Init.Prescaler = 0;
	IC_TimHandle.Init.ClockDivision = 0;
	IC_TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_IC_Init(&IC_TimHandle) != HAL_OK)
		return 1;

	/*##-2- Configure the Input Capture channels ###############################*/
	/* Common configuration */
	sConfig.ICPrescaler = TIM_ICPSC_DIV1;
	sConfig.ICFilter = 0x0;

	/* Configure the Input Capture of channel 1 */
	sConfig.ICPolarity = TIM_ICPOLARITY_RISING;
	sConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	if(HAL_TIM_IC_ConfigChannel(&IC_TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
		return 1;

	/* Configure the Input Capture of channel 2 */
	sConfig.ICPolarity = TIM_ICPOLARITY_FALLING;
	sConfig.ICSelection = TIM_ICSELECTION_INDIRECTTI;
	if(HAL_TIM_IC_ConfigChannel(&IC_TimHandle, &sConfig, TIM_CHANNEL_2) != HAL_OK)
		return 1;

	/*##-3- Configure the slave mode ###########################################*/
	/* Select the slave Mode: Reset Mode */
	sSlaveConfig.SlaveMode     = TIM_SLAVEMODE_RESET;
	sSlaveConfig.InputTrigger  = TIM_TS_TI1FP1;
	if(HAL_TIM_SlaveConfigSynchronization(&IC_TimHandle, &sSlaveConfig) != HAL_OK)
		return 1;

	/*##-4- Start the Input Capture in interrupt mode ##########################*/
	if(HAL_TIM_IC_Start_IT(&IC_TimHandle, TIM_CHANNEL_1) != HAL_OK)
		return 1;

	/*##-5- Start the Input Capture in interrupt mode ##########################*/
	if(HAL_TIM_IC_Start_IT(&IC_TimHandle, TIM_CHANNEL_2) != HAL_OK)
		return 1;
	return 0;
}

void GetPWMParam(__IO uint32_t *DutyCycle, __IO uint32_t *Frequency)
{
	*DutyCycle = uwDutyCycle;
	*Frequency = uwFrequency;
}

void IC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&IC_TimHandle);
}

/**
  * @brief  Input Capture callback in non blocking mode 
  * @param  htim: TIM IC handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		/* Get the Input Capture value */
		uwIC1Value = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1) + 2;

		if (uwIC1Value != 0)
		{
			/* Duty cycle computation */
			uwDutyCycle = ((HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2) + 3) * 100) / uwIC1Value;
			/* uwFrequency computation
			TIM5 counter clock = (RCC_Clocks.HCLK_Frequency) */      
			uwFrequency = (HAL_RCC_GetHCLKFreq()) / uwIC1Value;
		}
		else
		{
			uwDutyCycle = 0;
			uwFrequency = 0;
		}
	}
}

/*************************************** END OF FILE ***************************************/
