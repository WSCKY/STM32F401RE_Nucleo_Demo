/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtRTC.c
  * @author  '^_^'
  * @version V0.0.0
  * @date    4-October-2014
  * @brief   This sample code shows how to use STM32F4xx RTC HAL API to configure 
  *          Time and Date.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_ExtRTC.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RtcHandle;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static uint8_t RTC_CalendarConfig(void);

/**
  * @brief  Configure the RTC peripheral
  * @param  None
  * @retval Init Status.
  */
uint8_t RTC_Config(void)
{
	RCC_OscInitTypeDef        RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

	/* To change the source clock of the RTC feature (LSE, LSI), You have to:
		- Enable the power clock using __PWR_CLK_ENABLE()
		- Enable write access using HAL_PWR_EnableBkUpAccess() function before to 
			configure the RTC clock source (to be done once after reset).
		- Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and 
			__HAL_RCC_BACKUPRESET_RELEASE().
		- Configure the needed RTc clock source */

	/*##-1- Configue LSE as RTC clock soucre ###################################*/ 
	RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
		return 1;

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		return 1;

	/*##-2- Enable RTC peripheral Clocks #######################################*/ 
	/* Enable RTC Clock */ 
	__HAL_RCC_RTC_ENABLE(); 

	/*##-1- Configure the RTC peripheral #######################################*/
	/* Configure RTC prescaler and RTC data registers */
	/* RTC configured as follow:
		- Hour Format    = Format 24
		- Asynch Prediv  = Value according to source clock
		- Synch Prediv   = Value according to source clock
		- OutPut         = Output Disable
		- OutPutPolarity = High Polarity
		- OutPutType     = Open Drain */
	RtcHandle.Instance = RTC; 
	RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
	RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
	RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
	RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
	RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
	if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
		return 1;
   
	/*##-2- Check if Data stored in BackUp register0: No Need to reconfigure RTC#*/
	/* Read the BackUp Register 0 Data */
	if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR0) != 0x32F2)
	{  
		/* Configure RTC Calendar */
		if(RTC_CalendarConfig())
			return 1;
	}
	return 0;
}

/**
  * @brief  Display the current time and date.
  * @param  showtime: pointer to buffer
  * @param  showdate: pointer to buffer
  * @retval None
  */
void RTC_CalendarShow(uint8_t *showtime, uint8_t *showdate)
{
	RTC_DateTypeDef sdatestructureget;
	RTC_TimeTypeDef stimestructureget;

	/* Get the RTC current Time */
	HAL_RTC_GetTime(&RtcHandle, &stimestructureget, FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, FORMAT_BIN);

	/* Display time Format: hh:mm:ss */
	sprintf((char*)showtime,"%02d:%02d:%02d",stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
	/* Display date Format: mm-dd-yy */
	sprintf((char*)showdate,"%02d-%02d-%02d",sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);
}

/**
  * @brief  RTC Get time.
  * @param  sDate: Pointer to date structure.
  * @param  sTime: Pointer to Time structure.
  * @retval None.
  */
void RTC_GetTime(RTC_DateTypeDef *sDate, RTC_TimeTypeDef *sTime)
{
	/* Get the RTC current Time */
	HAL_RTC_GetTime(&RtcHandle, sTime, FORMAT_BIN);
	/* Get the RTC current Date */
	HAL_RTC_GetDate(&RtcHandle, sDate, FORMAT_BIN);
}

/**
  * @brief  RTC Set time.
  * @param  sDate: Pointer to date structure.
  * @param  sTime: Pointer to Time structure.
  * @retval Set status.
  */
uint8_t RTC_TimeConfig(RTC_DateTypeDef *sDate, RTC_TimeTypeDef *sTime)
{
	if(HAL_RTC_SetDate(&RtcHandle, sDate, FORMAT_BIN) != HAL_OK)
		return 1;
	if(HAL_RTC_SetTime(&RtcHandle, sTime, FORMAT_BIN) != HAL_OK)
		return 1;
	return 0;
}

/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
static uint8_t RTC_CalendarConfig(void)
{
	RTC_DateTypeDef sdatestructure;
	RTC_TimeTypeDef stimestructure;

	/*##-1- Configure the Date #################################################*/
	/* Set Date: Saturday October 4th 2014 */
	sdatestructure.Year = 0x14;
	sdatestructure.Month = RTC_MONTH_OCTOBER;
	sdatestructure.Date = 0x04;
	sdatestructure.WeekDay = RTC_WEEKDAY_SATURDAY;

	if(HAL_RTC_SetDate(&RtcHandle, &sdatestructure, FORMAT_BCD) != HAL_OK)
		return 1;
  
	/*##-2- Configure the Time #################################################*/
	/* Set Time: 13:28:00 */
	stimestructure.Hours = 0x13;
	stimestructure.Minutes = 0x28;
	stimestructure.Seconds = 0x00;
	stimestructure.TimeFormat = RTC_HOURFORMAT12_PM;
	stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
	stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
	if(HAL_RTC_SetTime(&RtcHandle, &stimestructure, FORMAT_BCD) != HAL_OK)
		return 1;

	/*##-3- Writes a data in a RTC Backup data Register0 #######################*/
	HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR0, 0x32F2);  
	return 0;
}

/*************************************** END OF FILE ***************************************/
