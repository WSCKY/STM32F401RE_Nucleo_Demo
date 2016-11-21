/**
  ******************************************************************************
  * @file    main.c 
  * @author  '^_^'
  * @version V0.0.0
  * @date    28-September-2014
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
//test1
//uint8_t TX_ADDRESS[ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};
//uint8_t RX_ADDRESS[ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};
//uint8_t txbuf[33] = "QWERTYUIOPASDFGHJKLZXCVBNM081204\0";
//uint8_t rxbuf[32] = {0};
//tes2
uint8_t TX_ADDRESS[ADR_WIDTH] = {0x01, 0xB6, 0xB5, 0xB4, 0xB3};
//uint8_t RX_ADDRESS[ADR_WIDTH] = {0x01, 0xB6, 0xB5, 0xB4, 0xB3};
uint8_t Addr1[ADR_WIDTH] = {0x01, 0xB6, 0xB5, 0xB4, 0xB3};
//uint8_t Addr2 = 0x3D; uint8_t Addr3 = 0x4D; uint8_t Addr4 = 0x2F; uint8_t Addr5 = 0xED;
uint8_t txbuf[33] = "QWERTYUIOPASDFGHJKLZXCVBNM081204\0";
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
	nRF24L01_InitTypeDef nRF24L01_InitStruct;
	nRF24L01_PipeAddr_InitTypedef nRF_Addr;
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
	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"nRF24L01 Com Test...");

	W25Q16_Init(Init_Y);

	while(SPI_Flash_ReadID() != W25Q16)
	{
		LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"ID Error!           ");
		HAL_Delay(1000);
		LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"                    ");
		HAL_Delay(1000);
	}
	
	nRF_Addr.LOCAL_TX_ADDR = TX_ADDRESS;
	nRF_Addr.RX_ADDR0 = TX_ADDRESS; nRF_Addr.RX_ADDR1 = Addr1;
//	nRF_Addr.RX_ADDR2 = &Addr2; nRF_Addr.RX_ADDR3 = &Addr3;
//	nRF_Addr.RX_ADDR4 = &Addr4; nRF_Addr.RX_ADDR5 = &Addr5;
	nRF24L01_InitStruct.Addr = &nRF_Addr;
	nRF24L01_InitStruct.AUTO_ACK = AUTO_ACK_ENAA_P0;
	nRF24L01_InitStruct.RX_PIPE = EN_RXADDR_P0;
	nRF24L01_InitStruct.COM_FREQ = RF_Channel_2_4G;
	nRF24L01_InitStruct.RF_SETUP_VALUE = RF_DEFAULT;
	nRF24L01_InitStruct.TRANS_MODE = TX_MODE;//RX_MODE;
	nRF24L01_InitStruct.PLOAD_WIDTH = RX_PW_P0_32;
	nRF24L01_InitStruct.REPEATS = SETUP_RETR_ARD_250us | SETUP_PETR_ARC_10;
	nRF24L01_Init(&nRF24L01_InitStruct);

	/* Infinite loop */
	while(1)
	{
//test1
//		if(BSP_PB_GetState(BUTTON1) == 0)
//		{
//			/* Wait for BUTTON1 is released */
//			while(BSP_PB_GetState(BUTTON1) == 0);
//			LCD_DrawFullRect(0, LCD_LINE_1, LCD_PIXEL_WIDTH, 112, 0x0000);
//			LCD_STRING(0, LCD_LINE_1, (uint8_t *)"数据发送启动！      ", 0xFFFF, 0x0000);
//			if(nRF24L01_TX_Packet(txbuf) == TX_OK)
//			{
//				LCD_STRING(0, LCD_LINE_1, (uint8_t *)"数据发送成功！      ", 0xFFFF, 0x0000);
//				LCD_STRING(0, LCD_LINE_2, (uint8_t *)"进入接收模式！      ", 0xFFFF, 0x0000);
//				nRF24L01_RX_Mode();
//				LCD_STRING(0, LCD_LINE_3, (uint8_t *)"等待数据...         ", 0xFFFF, 0x0000);
//				while(nRF24L01_RX_Packet(rxbuf) != 0)
//                {
//                    HAL_Delay(500);
//                }
//				LCD_STRING(0, LCD_LINE_3, (uint8_t *)rxbuf, 0xFFFF, 0x0000);
//				LCD_STRING(0, LCD_LINE_4, (uint8_t *)"退回发送模式！      ", 0xFFFF, 0x0000);
//				nRF24L01_TX_Mode();
//			}
//			else
//			{
//				LCD_STRING(0, LCD_LINE_1, (uint8_t *)"数据发送失败！      ", 0xFFFF, 0x0000);
//			}
//		}
//tes2
		if(BSP_PB_GetState(BUTTON1) == 0)
		{
			/* Wait for BUTTON1 is released */
			while(BSP_PB_GetState(BUTTON1) == 0);
			LCD_STRING(0, LCD_LINE_1, (uint8_t *)"数据发送启动！      ", 0xFFFF, 0x0000);
			if(nRF24L01_TX_Packet(txbuf) == TX_OK)
			{
				LCD_STRING(0, LCD_LINE_1, (uint8_t *)"数据发送成功！      ", 0xFFFF, 0x0000);
			}
			else
			{
				LCD_STRING(0, LCD_LINE_1, (uint8_t *)"数据发送失败！      ", 0xFFFF, 0x0000);
			}
		}
		BSP_LED_Toggle(LED4);
		HAL_Delay(50);
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
