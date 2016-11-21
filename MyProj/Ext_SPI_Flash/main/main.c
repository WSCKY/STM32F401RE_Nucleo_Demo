/**
  ******************************************************************************
  * @file    main.c 
  * @author  '^_^'
  * @version V0.0.0
  * @date    25-September-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/
#define  FLASH_WRITE_ADDRESS      0x000077
#define  FLASH_READ_ADDRESS       FLASH_WRITE_ADDRESS
#define  FLASH_SECTOR_TO_ERASE    ((FLASH_WRITE_ADDRESS / 4096) * 4096)
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))
#define BufferSize (countof(Tx_Buffer)-1)
/* Private variables ---------------------------------------------------------*/
uint8_t Tx_Buffer[] = 
"The register variable declaration can only be appied to automatic variables, and to the..\0";
uint8_t  Rx_Buffer[BufferSize];
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = PASSED;
uint8_t DebugInfo[120];
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	uint8_t Index = 0;
	/*	STM32F4xx HAL library initialization:
		- Configure the Flash prefetch, instruction and Data caches
		- Configure the Systick to generate an interrupt each 1 msec
		- Set NVIC Group Priority to 4
		- Low Level Initialization
	*/
	HAL_Init();

	/* Configure the System clock to have a frequency of 84 MHz */
	SystemClock_Config();

	/* Add your application code here*/
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);

	LCD_Init();
	LCD_Clear(LCD_COLOR_BLACK);
	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"W25Q16 Test     ");

	W25Q16_Init(Init_Y);//如果这里写不初始化，会在读ID错误一次后再次读的时候成功
	//原因在于SPIx调用失败时会自动重新初始化SPIx，见函数SPIx_Error()

	while(SPI_Flash_ReadID() != W25Q16)
	{
		LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"ID Error!       ");
		HAL_Delay(1000);
		LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"                ");
		HAL_Delay(1000);
	}

/************************************************************************************************/
	/* Perform a write in the Flash followed by a read of the written data */
	/* Erase SPI FLASH Sector to write on */
	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"ERASE SECTOR... ");
	SPI_Flash_Erase_Sector(FLASH_SECTOR_TO_ERASE);
	/* Write Tx_Buffer data to SPI FLASH memory */
	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"Write Buffer... ");
	SPI_Flash_Write(Tx_Buffer, FLASH_WRITE_ADDRESS, BufferSize);
	/* Read data from SPI FLASH memory */
	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"Read Buffer...  ");
	SPI_Flash_Read(Rx_Buffer, FLASH_READ_ADDRESS, BufferSize);
	/* Check the correctness of written data */
	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"Check Buffer... ");
	TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);
	/* TransferStatus1 = PASSED, if the transmitted and received data by SPI2
	are the same */
	/* TransferStatus1 = FAILED, if the transmitted and received data by SPI2
	are different */
	sprintf((char *)DebugInfo, "Rx_Buffer= %s", Rx_Buffer);
	LCD_Showphrase(0, LCD_LINE_2, DebugInfo);
	if(TransferStatus1 == FAILED)
		Error_Handler();
	HAL_Delay(2000);
	
	/* Perform an erase in the Flash followed by a read of the written data */
	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"ERASE SECTOR... ");
	SPI_Flash_Erase_Sector(FLASH_SECTOR_TO_ERASE);
//	SPI_Flash_Erase_Chip();
	/* Read data from SPI FLASH memory */
	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"Read Buffer...  ");
	SPI_Flash_Read(Rx_Buffer, FLASH_READ_ADDRESS, BufferSize);
	/* Check the correctness of erasing operation dada */
	for (Index = 0; Index < BufferSize; Index++)
	{
		if (Rx_Buffer[Index] != 0xFF)
		{
			TransferStatus2 = FAILED;
			LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"ERASE ERROR!    ");
			break;
		}
	}
	/* TransferStatus2 = PASSED, if the specified sector part is erased */
	/* TransferStatus2 = FAILED, if the specified sector part is not well erased */
	if(TransferStatus2 == FAILED)
		Error_Handler();
	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"ERASE SUCCESS...");

/************************************************************************************************/
	LCD_SetTextColor(LCD_COLOR_RED);
	LCD_DisplayStringLine(LCD_LINE_9, (uint8_t *)"TEST COMPELETE..");
	/* Infinite loop */
	while(1)
	{
		BSP_LED_Toggle(LED4);
		HAL_Delay(200);
	}
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length.
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
	while (BufferLength--)
	{
		if (*pBuffer1 != *pBuffer2)
		{
			return FAILED;
		}
		pBuffer1++;
		pBuffer2++;
	}
	return PASSED;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 84000000
  *            HCLK(Hz)                       = 84000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLL_M                          = 16
  *            PLL_N                          = 336
  *            PLL_P                          = 4
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale2 mode
  *            Flash Latency(WS)              = 2 
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is 
	clocked below the maximum system frequency, to update the voltage scaling value 
	regarding system frequency refer to product datasheet. */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

	/* Enable HSE Oscillator and activate PLL with HSI as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSICalibrationValue = 8;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}  
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
	/* User may add here some code to deal with this error */
	while(1)
	{
		BSP_LED_Toggle(LED3);
		HAL_Delay(50);
	}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
