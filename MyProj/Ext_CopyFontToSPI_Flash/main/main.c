/**
  ******************************************************************************
  * @file    main.c 
  * @author  '^_^'
  * @version V0.0.0
  * @date    24-September-2014
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define GBK_FILE "0:/GBK12.DZK"
#define Write_Addr FontADDRESS

#define DataSize ((uint16_t)512 * 8)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SD_FatFs;  /* File system object for SD card logical drive */
char SD_Path[4]; /* SD card logical drive path */
FRESULT fres;
UINT countbr;
FIL file;
uint8_t Info[20];
uint32_t NumOfPackage = 0;
uint32_t temp;
static uint8_t pBuffer[DataSize]=
{
	0x00
};
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/
static void SDCard_Config(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
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
	BSP_LED_Init(LED5);
	BSP_PB_Init(BUTTON1, BUTTON_MODE_GPIO);

	LCD_Init();
	LCD_Clear(LCD_COLOR_BLACK);
	LCD_SetDisplayDirect(Dir_4);
	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"Copy Font To Flash..");
/*-- SD FATFS --*/
	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"FATFS Initialization");
	SDCard_Config();
	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"FATFS Init Success! ");
/*-- W25Q16 SPI Flash --*/
	LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"Initialize SPI Flash");
	W25Q16_Init(Init_N);
	while(SPI_Flash_ReadID() != W25Q16)
	{
		LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"Flash ID Error!     ");
		HAL_Delay(1000);
		LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"                    ");
		HAL_Delay(1000);
	}
	LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"SPI Flash Init OK!  ");

	LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)"Press B1 to start...");
	/* Wait for BUTTON1 is pressed */
	while (BSP_PB_GetState(BUTTON1) != 0);
	/* Wait for BUTTON1 is released */
	while (BSP_PB_GetState(BUTTON1) == 0);
	
	sprintf((char *)Info, "Open %s...", GBK_FILE);
	LCD_DisplayStringLine(LCD_LINE_3, Info);
	fres = f_open(&file , GBK_FILE, FA_OPEN_EXISTING | FA_READ);
	if ( fres != FR_OK )
	{
		LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)"File Open Failed!   ");
		f_close(&file);
		Error_Handler();
	}
	LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)"File Open Success!  ");
	LCD_DisplayStringLine(LCD_LINE_5, (uint8_t *)"File Copy Starting..");
	while(1)
	{
		fres = f_read( &file, pBuffer, DataSize, &countbr );
		temp = countbr;
		SPI_Flash_Write(pBuffer, Write_Addr + NumOfPackage * DataSize, temp);
		BSP_LED_Toggle(LED5);
		if (temp < DataSize) break;
		NumOfPackage ++;
	}
	f_close(&file);
	LCD_DisplayStringLine(LCD_LINE_5, (uint8_t *)"File Copy Comelete! ");
	sprintf((char *)Info, "Copy:%ld ", NumOfPackage * DataSize + temp);
	LCD_DisplayStringLine(LCD_LINE_6, Info);
	LCD_DisplayStringLine(LCD_LINE_7, (uint8_t *)"Press B1 to Test... ");
	/* Wait for BUTTON1 is pressed */
	while (BSP_PB_GetState(BUTTON1) != 0);
	/* Wait for BUTTON1 is released */
	while (BSP_PB_GetState(BUTTON1) == 0);
	LCD_Clear(LCD_COLOR_BLACK);

	LCD_STRING(0, LCD_LINE_0, (uint8_t *)"    何以飘零去，\n"
	                                     "    何以少团栾。\n"
	                                     "    何以别离久，\n"
	                                     "    何以不得安？",
	           LCD_COLOR_WHITE, LCD_COLOR_BLACK);
	LCD_STRING_ON(0, LCD_LINE_4, (uint8_t *)"    床前明月光，\n"
	                                        "    疑是地上霜。\n"
	                                        "    举头望明月，\n"
	                                        "    低头思故乡。", LCD_COLOR_WHITE);

	/* Unlink the RAM disk I/O driver */
	FATFS_UnLinkDriver(SD_Path);
	/* Infinite loop */
	while(1)
	{
		BSP_LED_Toggle(LED4);
		HAL_Delay(200);
	}
}

static void SDCard_Config(void)
{
	if(FATFS_LinkDriver(&SD_Driver, SD_Path) == 0)
	{
		/* Initialize the SD mounted on adafruit 1.8" TFT shield */
		BSP_SD_Init();

		/* Check the mounted device */
		if(f_mount(&SD_FatFs, (TCHAR const*)"/", 0) != FR_OK)
		{
			LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"Device Mount Error! ");
			Error_Handler();
		}
	}
	else
	{
		LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"Driver Link Error!  ");
		Error_Handler();
	}
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
