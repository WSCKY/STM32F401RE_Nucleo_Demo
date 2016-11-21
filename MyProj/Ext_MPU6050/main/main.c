/**
  ******************************************************************************
  * @file    main.c 
  * @author  '^_^'
  * @version V0.0.0
  * @date    10-October-2014
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
TIM_HandleTypeDef TimHandle;
uint8_t DebugInfo[20];
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
	MPU6050_InitTypeDef MPU6050_InitStruct;
	AcceDef AcceOffset;
	GyroDef GyroOffset;
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
	BSP_PB_Init(BUTTON1, BUTTON_MODE_GPIO);

	LCD_Init();
	LCD_Clear(LCD_COLOR_BLACK);
	LCD_SetDisplayDirect(Dir_4);
	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"MPU6050 Test!       ");

	if(BK_LED_TIMx_Config(&TimHandle))
	{
		LCD_STRING(0, LCD_LINE_1, (uint8_t *)"LCD BackLight Error!", 0xFFFF, 0x0000);
		Error_Handler();
	}

	W25Q16_Init(Init_Y);

	while(SPI_Flash_ReadID() != W25Q16)
	{
		LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"ID Error!           ");
		HAL_Delay(1000);
		LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"                    ");
		HAL_Delay(1000);
	}

	MPU6050_InitStruct.Sleep_Mode = MPU6050_PWR1_SLEEP_DISABLE;
	MPU6050_InitStruct.Clock_Mode = MPU6050_CLOCK_INTERNAL;
	MPU6050_InitStruct.Wake_FREQ = MPU6050_WAKE_FREQ_1P25;
	MPU6050_InitStruct.STDBY_Mode = MPU6050_PWR2_STBY_None;
	MPU6050_InitStruct.Gyro_FullScale = MPU6050_GYRO_FS_2000;
	MPU6050_InitStruct.Acce_FullScale = MPU6050_ACCEL_FS_8;
	MPU6050_InitStruct.HighPassFilter = MPU6050_DHPF_5;
	MPU6050_InitStruct.LowPassFilter = MPU6050_DLPF_BW_98;
	MPU6050_InitStruct.EXT_SYNC = MPU6050_EXT_SYNC_DISABLED;
	MPU6050_InitStruct.SMPLRT_DIV = 0x01;
	MPU6050_Init(&MPU6050_InitStruct);
//	LCD_STRING(0, LCD_LINE_1, (uint8_t *)"外设初始化完成！    ", 0xFFFF, 0x0000);

	sprintf((char *)DebugInfo, "MPU6050 ID = %x", MPU6050_GetID());
	LCD_DisplayStringLine(LCD_LINE_1, DebugInfo);

	MPU6050_Correct(&AcceOffset, &GyroOffset);
	/* Infinite loop */
	while(1)
	{
		LCD_ClearLine(LCD_LINE_2);
		LCD_ClearLine(LCD_LINE_3);
		sprintf((char *)DebugInfo, "GyroX=%2.2frad/s\nAcceX=%2.2fm/s^2", \
		(MPU6050_GetGyroX() - GyroOffset.GyroX) * 0.061 * 0.0174, \
		((MPU6050_GetAcceX() - AcceOffset.AcceX) * 0.0244) * 0.098);
		LCD_Showphrase(0, LCD_LINE_2, DebugInfo);

		LCD_ClearLine(LCD_LINE_4);
		LCD_ClearLine(LCD_LINE_5);
		sprintf((char *)DebugInfo, "GyroY=%2.2frad/s\nAcceY=%2.2fm/s^2", \
		(MPU6050_GetGyroY() - GyroOffset.GyroY) * 0.061 * 0.0174, \
		((MPU6050_GetAcceY() - AcceOffset.AcceY) * 0.0244) * 0.098);
		LCD_Showphrase(0, LCD_LINE_4, DebugInfo);

		LCD_ClearLine(LCD_LINE_6);
		LCD_ClearLine(LCD_LINE_7);
		sprintf((char *)DebugInfo, "GyroZ=%2.2frad/s\nAcceZ=%2.2fm/s^2", \
		(MPU6050_GetGyroZ() - GyroOffset.GyroZ) * 0.061 * 0.0174, \
		((MPU6050_GetAcceZ() - AcceOffset.AcceZ) * 0.0244) * 0.098);
		LCD_Showphrase(0, LCD_LINE_6, DebugInfo);

		BSP_LED_Toggle(LED4);
		HAL_Delay(300);
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
