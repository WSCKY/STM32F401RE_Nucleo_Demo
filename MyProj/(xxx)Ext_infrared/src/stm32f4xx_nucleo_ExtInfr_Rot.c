/******************************************************************************
  * @file    stm32f4xx_nucleo_ExtInfr_Rot.c
  * @author  '^_^'
  * @version V0.0.0
  * @date    3-October-2014
  * @brief   This file includes the driver for VS1838B Infrared signal receiver.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_ExtInfr_Rot.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
TIM_HandleTypeDef      VS_TimHandle;
/* Timer Input Capture Configuration Structure declaration */
TIM_IC_InitTypeDef     VS_ICConfig;

/* Captured Values */
uint32_t               uwIC4Value1    = 0;
uint32_t               uwIC4Value2    = 0;
uint32_t               uwDiffCapture  = 0;
/* Capture index */
uint16_t               uhCaptureIndex = 0;
uint16_t               SampleStart    = 0;

uint32_t               RemoteValue    = 0;
uint16_t               RemoteIndex    = 0;
uint16_t               RemoteGet      = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  VS1838B Driver Prepare.
  * @param  None
  * @retval Init status.
  */
uint8_t VS1838B_Prep(void)
{
	GPIO_InitTypeDef   GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* TIMx Peripheral clock enable */
	VS_TIMx_CLK_ENABLE();

	/* Enable GPIO channels Clock */
	VS1838B_GPIO_CLK_ENABLE();

	/* Configure  (TIMx_Channel) in Alternate function, push-pull and speed */
	GPIO_InitStruct.Pin = VS1838B_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = VS1838B_GPIO_AF_TIMx;
	HAL_GPIO_Init(VS1838B_GPIO_PORT, &GPIO_InitStruct);

	/*##-2- Configure the NVIC for TIMx #########################################*/
	/* Sets the priority grouping field */
	HAL_NVIC_SetPriority(VS_TIMx_IRQn, 0, 1);

	/* Enable the TIMx global Interrupt */
	HAL_NVIC_EnableIRQ(VS_TIMx_IRQn);

	/*##-3- Configure the TIM peripheral ########################################*/
	/* Set TIMx instance */
	VS_TimHandle.Instance = VS_TIMx;

	/* Initialize TIMx peripheral as follow:
		+ Period            = 0xFFFF
		+ Prescaler         = 41(2 MHz)
		+ ClockDivision     = 0
		+ Counter direction = Up
	*/
	VS_TimHandle.Init.Period           = 0xFFFF;
	VS_TimHandle.Init.Prescaler        = 41;
	VS_TimHandle.Init.ClockDivision    = 0;
	VS_TimHandle.Init.CounterMode      = TIM_COUNTERMODE_UP;
	if(HAL_TIM_IC_Init(&VS_TimHandle) != HAL_OK)
		return 1;

	/*##-2- Configure the Input Capture channels ###############################*/
	/* Configure the Input Capture of VS_TIMx_CHANNEL */
	VS_ICConfig.ICPolarity  = TIM_ICPOLARITY_FALLING;
	VS_ICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	VS_ICConfig.ICPrescaler = TIM_ICPSC_DIV1;
	VS_ICConfig.ICFilter    = 0;
	if(HAL_TIM_IC_ConfigChannel(&VS_TimHandle, &VS_ICConfig, VS_TIMx_CHANNEL) != HAL_OK)
		return 1;

	/*##-4- Start the Input Capture in interrupt mode ##########################*/
	if(HAL_TIM_IC_Start_IT(&VS_TimHandle, VS_TIMx_CHANNEL) != HAL_OK)
		return 1;

	return 0;
}

/**
  * @brief  Get Infrared parameter.
  * @param  *Value: remote control value.
  * @retval signal status.
  */
uint8_t Get_Infrared(uint32_t *Value)
{
	if(RemoteGet == 1)
	{
		*Value = RemoteValue;
		return 1;
	}
	return 0;
}

/**
  * @brief  Conversion complete callback in non blocking mode 
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == VS_TIMx_CHANNEL)
	{
		if(SampleStart == 1)
		{
			/* Get the last value */
			uwIC4Value1 = uwIC4Value2;
			uwIC4Value2 = HAL_TIM_ReadCapturedValue(htim, VS_TIMx_CHANNEL);
			/* Capture computation */
			if (uwIC4Value2 > uwIC4Value1)
				uwDiffCapture = (uwIC4Value2 - uwIC4Value1);
			else  /* (uwIC2Value2 <= uwIC2Value1) */
				uwDiffCapture = ((0xFFFF - uwIC4Value1) + uwIC4Value2);
			if((uwDiffCapture > 4280) && (uwDiffCapture < 4680)) //Permissible error 100us
			{
				RemoteValue <<= 1;
				RemoteValue |= 1;
				RemoteIndex ++;
			}
			else if((uwDiffCapture > 2040) && (uwDiffCapture < 2440)) //Permissible error 100us
			{
				RemoteValue <<= 1;
				RemoteIndex ++;
			}
			else
			{
				uhCaptureIndex = 0;
				SampleStart = 0;
				RemoteIndex = 0;
			}
			if(RemoteIndex == 32)
			{
				RemoteGet = 1;
				uhCaptureIndex = 0;
				SampleStart = 0;
				RemoteIndex = 0;
			}
		}
		else if((uhCaptureIndex == 1) && (SampleStart == 0))
		{
			/* Get the 2nd Input Capture value */
			uwIC4Value2 = HAL_TIM_ReadCapturedValue(htim, VS_TIMx_CHANNEL);

			/* Capture computation */
			if (uwIC4Value2 > uwIC4Value1)
				uwDiffCapture = (uwIC4Value2 - uwIC4Value1);
			else  /* (uwIC2Value2 <= uwIC2Value1) */
				uwDiffCapture = ((0xFFFF - uwIC4Value1) + uwIC4Value2);
			if((uwDiffCapture > 26500) && (uwDiffCapture < 27500)) //Permissible error 250us
			{
				SampleStart = 1;
				RemoteValue = 0;
				RemoteGet = 0;
			}
			else
				uhCaptureIndex = 0;
		}
		else if((uhCaptureIndex == 0) && (SampleStart == 0))
		{
			/* Get the 1st Input Capture value */
			uwIC4Value1 = HAL_TIM_ReadCapturedValue(htim, VS_TIMx_CHANNEL);
			uhCaptureIndex = 1;
		}
	}
}

/**
  * @brief  This function handles TIM3 global interrupt request.
  * @param  None
  * @retval None
  */
void VS_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&VS_TimHandle);
}

/*************************************** END OF FILE ***************************************/
