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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL MyFile;     /* File object */
char SDPath[4]; /* SD card logical drive path */

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/
uint8_t play_music(char * file);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	VS1003_InitTypeDef VS1003_InitStructure;/* VS1003 initial structure */
	
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
	BSP_LED_Init(LED6);

	VS1003_InitStructure.Bass = 6;
	VS1003_InitStructure.Bass_Freq = 15;
	VS1003_InitStructure.Left_Chan_VOL = 200;
	VS1003_InitStructure.Right_Chan_VOL = 200;
	VS1003_InitStructure.Sample_Rate = 48000;
	VS1003_InitStructure.Stereo = STEREO_ON;
	VS1003_InitStructure.Treble = 10;
	VS1003_InitStructure.Treble_Freq = 15;

	VS1003_Init(&VS1003_InitStructure, REQ_MODE_GPIO);
	BSP_LED_Toggle(LED3);

	/*##-1- Link the micro SD disk I/O driver ##################################*/
	if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
	{
		/*##-2- Register the file system object to the FatFs module ##############*/
		if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
		{
			/* FatFs Initialization Error */
			Error_Handler();
		}
		else
		{
			while(1)
			{
				VS1003_Sine_Test(0x24, 1000);
				VS1003_Sine_Test(0x45, 1000);
				VS1003_SW_RESET();
				if(play_music("m1.mp3"))
					Error_Handler();
				if(play_music("m2.mp3"))
					Error_Handler();
				if(play_music("m3.mp3"))
					Error_Handler();
				if(play_music("m4.mp3"))
					Error_Handler();
				if(play_music("m5.mp3"))
					Error_Handler();
				if(play_music("m6.mp3"))
					Error_Handler();
				if(play_music("m7.mp3"))
					Error_Handler();
			}
		}
	}

	/*##-11- Unlink the RAM disk I/O driver ####################################*/
	FATFS_UnLinkDriver(SDPath);
	/* Success of the demo: no error occurrence */
	/* Infinite loop */
	while (1)
	{
		BSP_LED_Toggle(LED4);
		/* Insert delay 500 ms */
		HAL_Delay(500);
	}
}

uint8_t play_music(char * file)
{
	FRESULT res;                            /* FatFs function common result code */
	uint32_t bytesread;                     /* File read counts */
	uint8_t rtext[512];                     /* File read buffer */
	uint32_t Index = 0;

	VS1003_Play_Prepare();
	VS1003_Reset_DecodeTime();
	if(f_open(&MyFile, file, FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
		BSP_LED_On(LED4);//play start.
		while(1)
		{
			Index = 0;
			res = f_read(&MyFile, rtext, sizeof(rtext), (UINT*)&bytesread);
			do
			{
				if(!VS1003_Send_MusicData(rtext + Index))
				{
					Index += 32;
					BSP_LED_Toggle(LED6);
				}
			}
			while(Index < 512);
			if((bytesread != 512) || (res != FR_OK))
				break;
		}
		f_close(&MyFile);
		BSP_LED_Off(LED6);
		BSP_LED_Off(LED4);//play over.
		return 0;
	}
	return 1;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
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
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
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
		BSP_LED_Toggle(LED5);
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
