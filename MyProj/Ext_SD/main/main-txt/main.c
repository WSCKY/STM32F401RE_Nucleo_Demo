/**
  ******************************************************************************
  * @file    main.c 
  * @author  '^_^'
  * @version V0.0.1
  * @date    2-July-2014
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
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL MyFile;     /* File object */
char SDPath[4]; /* SD card logical drive path */

uint8_t Info[16];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	FRESULT res;                                          /* FatFs function common result code */
	uint32_t byteswritten, bytesread;                     /* File write/read counts */
	uint8_t wtext[] = "Write a few paragraphs.........."; /* File write buffer */
	uint8_t rtext[100];                                   /* File read buffer */
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
	LCD_Init();
	LCD_Clear(LCD_COLOR_BLACK);

	/*##-1- Link the micro SD disk I/O driver ##################################*/
	if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
	{
		/* Initialize the SD mounted on ExtBoard */
		BSP_SD_Init();
		/*##-2- Register the file system object to the FatFs module ##############*/
		if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
		{
			LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"f_mount over!");
			/* FatFs Initialization Error */
			Error_Handler();
		}
		else
		{
			LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"f_mount ok!");
//			/*##-3- Create a FAT file system (format) on the logical drive #########*/
//			/* WARNING: Formatting the uSD card will delete all content on the device */
//			if(f_mkfs((TCHAR const*)SDPath, 0, 0) != FR_OK)
//			{
//				/* FatFs Format Error */
//				Error_Handler();
//			}
//			else
//			{
				/*##-4- Create and Open a new text file object with write access #####*/
				if(f_open(&MyFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
				{
					LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"f_open over!");
					/* 'STM32.TXT' file Open for write Error */
					Error_Handler();
				}
				else
				{
					LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"f_open ok!");
					/*##-5- Write data to the text file ################################*/
					res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);

					if((byteswritten == 0) || (res != FR_OK))
					{
						LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"f_write over!");
						sprintf((char *)Info, "%d + %d", res, byteswritten);
						LCD_DisplayStringLine(LCD_LINE_3, Info);
						/* 'STM32.TXT' file Write or EOF Error */
						Error_Handler();
					}
					else
					{
						LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"f_write ok!");
						/*##-6- Close the open text file #################################*/
						f_close(&MyFile);
						/*##-7- Open the text file object with read access ###############*/
						if(f_open(&MyFile, "STM32.TXT", FA_READ) != FR_OK)
						{
							LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)"f_open over!");
							/* 'STM32.TXT' file Open for read Error */
							Error_Handler();
						}
						else
						{
							LCD_DisplayStringLine(LCD_LINE_3, (uint8_t *)"f_open ok!");
							/*##-8- Read data from the text file ###########################*/
							res = f_read(&MyFile, rtext, sizeof(rtext), (UINT*)&bytesread);

							if((bytesread == 0) || (res != FR_OK))
							{
								LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)"f_read over!");
								/* 'STM32.TXT' file Read or EOF Error */
								Error_Handler();
							}
							else
							{
								LCD_DisplayStringLine(LCD_LINE_4, (uint8_t *)"f_read ok!");
								/*##-9- Close the open text file #############################*/
								f_close(&MyFile);
LCD_DisplayStringLine(LCD_LINE_5, rtext);
								/*##-10- Compare read data with the expected data ############*/
								if((bytesread != byteswritten))
								{                
									/* Read data is different from the expected data */
									Error_Handler();
								}
							}
						}
					}
				}
//			}
		}
	}

	/*##-11- Unlink the RAM disk I/O driver ####################################*/
	FATFS_UnLinkDriver(SDPath);
	/* Success of the demo: no error occurrence */
	/* Infinite loop */
	while (1)
	{
		BSP_LED_Toggle(LED3);
		/* Insert delay 500 ms */
		HAL_Delay(500);
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
//	RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
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
//	LCD_DisplayChar(LCD_LINE_1, 0, errorNum + '0');
	/* User may add here some code to deal with this error */
	while(1)
	{
		BSP_LED_Toggle(LED3);
		/* Insert delay 50 ms */
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
