/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtADC.c
  * @author  '^_^'
  * @version V0.0.0
  * @date    28-September-2014
  * @brief   ADC RegularConversion by DMA.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_ExtADC.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void ADCx_MspInit(ADC_HandleTypeDef* hadc, ADCx_CHANNEL_Ax Channel_Ax);
//static void ADCx_MspDeInit(ADC_HandleTypeDef *hadc);

/**
  * @brief ADC MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param huart: ADC handle pointer
  * @param Channel_Ax: specific the channel to init.
  * @retval None
  */
static void ADCx_MspInit(ADC_HandleTypeDef* hadc, ADCx_CHANNEL_Ax Channel_Ax)
{
  GPIO_InitTypeDef          GPIO_InitStruct;
  static DMA_HandleTypeDef  hdma_adc;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO clock */
  if(Channel_Ax & 0x07)
    ADCx_CHANNEL_ExtA0_2_GPIO_CLK_ENABLE();
  if(Channel_Ax & 0x08)
    ADCx_CHANNEL_ExtA3_GPIO_CLK_ENABLE();
  if(Channel_Ax & 0x30)
    ADCx_CHANNEL_ExtA4_5_GPIO_CLK_ENABLE();
  /* ADC1 Periph clock enable */
  ADCx_CLK_ENABLE();
  /* Enable DMA2 clock */
  DMAx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/ 
  /* ADC1 Channelx GPIO pin configuration */
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Pin = 0;
  if(Channel_Ax & 0x01)
    GPIO_InitStruct.Pin |= ADCx_CHANNEL_ExtA0_PIN;
  if(Channel_Ax & 0x02)
	GPIO_InitStruct.Pin |= ADCx_CHANNEL_ExtA1_PIN;
  if(Channel_Ax & 0x04)
	GPIO_InitStruct.Pin |= ADCx_CHANNEL_ExtA2_PIN;
  if(Channel_Ax & 0x07)
  {
    HAL_GPIO_Init(ADCx_CHANNEL_ExtA0_2_GPIO_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = 0;
  }

  if(Channel_Ax & 0x08)
  {
	GPIO_InitStruct.Pin |= ADCx_CHANNEL_ExtA3_PIN;
	HAL_GPIO_Init(ADCx_CHANNEL_ExtA3_GPIO_PORT, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = 0;
  }

  if(Channel_Ax & 0x10)
	GPIO_InitStruct.Pin |= ADCx_CHANNEL_ExtA4_PIN;
  if(Channel_Ax & 0x20)
	GPIO_InitStruct.Pin |= ADCx_CHANNEL_ExtA5_PIN;
  if(Channel_Ax & 0x30)
  {
	HAL_GPIO_Init(ADCx_CHANNEL_ExtA4_5_GPIO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = 0;
  }
  
  /*##-3- Configure the DMA streams ##########################################*/
  /* Set the parameters to be configured */
  hdma_adc.Instance = ADCx_DMA_STREAM;
  
  hdma_adc.Init.Channel  = ADCx_DMA_CHANNEL;
  hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;//halfword!
  hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;//halfword!
  hdma_adc.Init.Mode = DMA_CIRCULAR;
  hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_adc.Init.FIFOMode = DMA_FIFOMODE_DISABLE;         
  hdma_adc.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
  hdma_adc.Init.MemBurst = DMA_MBURST_SINGLE;
  hdma_adc.Init.PeriphBurst = DMA_PBURST_SINGLE; 

  HAL_DMA_Init(&hdma_adc);
    
  /* Associate the initialized DMA handle to the the ADC handle */
  __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);

  /*##-4- Configure the NVIC for DMA #########################################*/
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 0, 0);   
  HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
}
  
///**
//  * @brief ADC MSP De-Initialization 
//  *        This function frees the hardware resources used in this example:
//  *          - Disable the Peripheral's clock
//  *          - Revert GPIO to their default state
//  * @param hadc: ADC handle pointer
//  * @retval None
//  */
//static void ADCx_MspDeInit(ADC_HandleTypeDef *hadc)
//{
//  static DMA_HandleTypeDef  hdma_adc;
//  
//  /*##-1- Reset peripherals ##################################################*/
//  ADCx_FORCE_RESET();
//  ADCx_RELEASE_RESET();

//  /*##-2- Disable peripherals and GPIO Clocks ################################*/
//  /* De-initialize the ADC1 Channelx GPIO pin */
//  HAL_GPIO_DeInit(ADCx_CHANNEL_ExtA0_2_GPIO_PORT, ADCx_CHANNEL_ExtA0_PIN | \
//	                                              ADCx_CHANNEL_ExtA1_PIN | \
//	                                              ADCx_CHANNEL_ExtA3_PIN);
//  HAL_GPIO_DeInit(ADCx_CHANNEL_ExtA3_GPIO_PORT, ADCx_CHANNEL_ExtA3_PIN);
//  HAL_GPIO_DeInit(ADCx_CHANNEL_ExtA4_5_GPIO_PORT, ADCx_CHANNEL_ExtA4_PIN | \
//	                                              ADCx_CHANNEL_ExtA5_PIN);
//  
//  /*##-3- Disable the DMA Streams ############################################*/
//  /* De-Initialize the DMA Stream associate to transmission process */
//  HAL_DMA_DeInit(&hdma_adc); 
//    
//  /*##-4- Disable the NVIC for DMA ###########################################*/
//  HAL_NVIC_DisableIRQ(ADCx_DMA_IRQn);
//}

/**
  * @brief  ADC Extboard Channel Init.
  * @param  huart: ADC handle pointer
  * @param  Channel_Ax: specific the channel to init.
  * @retval Init states
  */
uint8_t ADC_Ext_Init(ADC_HandleTypeDef* hadc, ADCx_CHANNEL_Ax Channel_Ax, uint32_t NbrOfConv)
{
	ADC_ChannelConfTypeDef sConfig;

	ADCx_MspInit(hadc, Channel_Ax);

	/*##-1- Configure the ADC peripheral #######################################*/
	hadc->Instance = ADCx;

	if(NbrOfConv == 1)
	{
		hadc->Init.ScanConvMode = DISABLE;
		hadc->Init.EOCSelection = DISABLE;
	}
	else
	{
		hadc->Init.ScanConvMode = ENABLE;
		hadc->Init.EOCSelection = EOC_SINGLE_CONV;
	}
	hadc->Init.ContinuousConvMode = ENABLE;
	hadc->Init.DiscontinuousConvMode = DISABLE;
	hadc->Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV8;
	hadc->Init.Resolution = ADC_RESOLUTION12b;
	hadc->Init.NbrOfDiscConversion = 0;
	hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc->Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc->Init.NbrOfConversion = NbrOfConv;
	hadc->Init.DMAContinuousRequests = ENABLE;

	if(HAL_ADC_Init(hadc) != HAL_OK)
		return 1;
  
	/*##-2- Configure ADC regular channel ######################################*/  
	sConfig.Rank = 0;
	sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
	sConfig.Offset = 0;
	if(Channel_Ax & 0x01)
	{
		sConfig.Channel = ADCx_CHANNEL_ExtA0;
		sConfig.Rank ++;
		if(HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
			return 1;
	}
	if(Channel_Ax & 0x02)
	{
		sConfig.Channel = ADCx_CHANNEL_ExtA1;
		sConfig.Rank ++;
		if(HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
			return 1;
	}
	if(Channel_Ax & 0x04)
	{
		sConfig.Channel = ADCx_CHANNEL_ExtA2;
		sConfig.Rank ++;
		if(HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
			return 1;
	}
	if(Channel_Ax & 0x08)
	{
		sConfig.Channel = ADCx_CHANNEL_ExtA3;
		sConfig.Rank ++;
		if(HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
			return 1;
	}
	if(Channel_Ax & 0x10)
	{
		sConfig.Channel = ADCx_CHANNEL_ExtA4;
		sConfig.Rank ++;
		if(HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
			return 1;
	}
	if(Channel_Ax & 0x20)
	{
		sConfig.Channel = ADCx_CHANNEL_ExtA5;
		sConfig.Rank ++;
		if(HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
			return 1;
	}
	return 0;
}

/*************************************** END OF FILE ***************************************/
