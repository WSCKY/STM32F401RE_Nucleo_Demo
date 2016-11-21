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
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t i, j = 0;
uint16_t color;
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

	LCD_Init();
	LCD_Clear(LCD_COLOR_BLACK);

//test1
//	LCD_Clear(LCD_COLOR_BLACK);
//	LCD_Clear(LCD_COLOR_WHITE);
//	LCD_Clear(LCD_COLOR_GREY);
//	LCD_Clear(LCD_COLOR_BLUE);
//	LCD_Clear(LCD_COLOR_RED);
//	LCD_Clear(LCD_COLOR_MAGENTA);
//	LCD_Clear(LCD_COLOR_GREEN);
//	LCD_Clear(LCD_COLOR_CYAN);
//	LCD_Clear(LCD_COLOR_YELLOW);
//	LCD_Clear(LCD_COLOR_GBLUE);

//	LCD_Clear(LCD_COLOR_BLACK);
//	LCD_Clear(LCD_COLOR_WHITE);
//	LCD_Clear(LCD_COLOR_GREY);
//	LCD_Clear(LCD_COLOR_BLUE);
//	LCD_Clear(LCD_COLOR_RED);
//	LCD_Clear(LCD_COLOR_MAGENTA);
//	LCD_Clear(LCD_COLOR_GREEN);
//	LCD_Clear(LCD_COLOR_CYAN);
//	LCD_Clear(LCD_COLOR_YELLOW);
//	LCD_Clear(LCD_COLOR_GBLUE);
//	
//	LCD_Clear(LCD_COLOR_BLACK);
//	LCD_Clear(LCD_COLOR_WHITE);
//	LCD_Clear(LCD_COLOR_GREY);
//	LCD_Clear(LCD_COLOR_BLUE);
//	LCD_Clear(LCD_COLOR_RED);
//	LCD_Clear(LCD_COLOR_MAGENTA);
//	LCD_Clear(LCD_COLOR_GREEN);
//	LCD_Clear(LCD_COLOR_CYAN);
//	LCD_Clear(LCD_COLOR_YELLOW);
//	LCD_Clear(LCD_COLOR_GBLUE);

//test2
//	LCD_DrawFullRect(0, 0, 64, 40, 0x5555);
//	LCD_DrawFullRect(64, 0, 64, 40, 0xAAAA);

//	LCD_DrawFullRect(0, 40, 64, 40, 0x1234);
//	LCD_DrawFullRect(64, 40, 64, 40, 0xABCD);

//	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)". Read RAM TEST!");

//	for(j = 0; j < 80; j ++)
//	{
//		for(i = 0; i < 128; i ++)
//		{
//			color = ReadPoint(i, j);
//			DrawPiont(i, (j + 80), color);
//		}
//	}

//test3
//	LCD_SetDisplayDirect(Dir_1);
//	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"LCD Set Dir Test");
//	LCD_DrawRect(20, 30, 5, 70, LCD_COLOR_RED);
//	LCD_SetDisplayDirect(Dir_2);
//	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"LCD Set Dir Test...");
//	LCD_DrawRect(20, 30, 5, 70, LCD_COLOR_RED);
//	LCD_SetDisplayDirect(Dir_3);
//	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"LCD Set Dir Test");
//	LCD_DrawRect(20, 30, 5, 70, LCD_COLOR_RED);
//	LCD_SetDisplayDirect(Dir_4);
//	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"LCD Set Dir Test...");
//	LCD_DrawRect(20, 30, 5, 70, LCD_COLOR_RED);

//test4
	LCD_SetDisplayDirect(Dir_1r);
	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"LCD Set Dir Test");
	LCD_DrawRect(20, 30, 5, 70, LCD_COLOR_RED);
	LCD_SetDisplayDirect(Dir_2r);
	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"LCD Set Dir Test...");
	LCD_DrawRect(20, 30, 5, 70, LCD_COLOR_RED);
	LCD_SetDisplayDirect(Dir_3r);
	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"LCD Set Dir Test");
	LCD_DrawRect(20, 30, 5, 70, LCD_COLOR_RED);
	LCD_SetDisplayDirect(Dir_4r);
	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"LCD Set Dir Test...");
	LCD_DrawRect(20, 30, 5, 70, LCD_COLOR_RED);

	/* Infinite loop */
	while (1)
	{
		/* Toggles the LED3 */
		BSP_LED_Toggle(LED3);
		/* Insert delay 250 ms */
		HAL_Delay(250);
		/* Toggles the LED4 */
		BSP_LED_Toggle(LED4);
		/* Insert delay 250 ms */
		HAL_Delay(250);
		/* Toggles the LED5 */
		BSP_LED_Toggle(LED5);
		/* Insert delay 250 ms */
		HAL_Delay(250);
		/* Toggles the LED6 */
		BSP_LED_Toggle(LED6);
		/* Insert delay 250 ms */
		HAL_Delay(250);
//		BK_LED_TOG();
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
	/* User may add here some code to deal with this error */
	while(1)
	{
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
