/**
  ******************************************************************************
  * @file    main.c 
  * @author  '^_^'
  * @version V0.0.0
  * @date    26-September-2014
  * @brief   Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef UartHandle;
/* Buffer used for transmission (must < 32) */
uint8_t aTxBuffer[TXBUFFERSIZE] = "ZYXWVUTSRQPONMLKJIHGFEDCBA012345";

/* Buffer used for reception */
uint8_t aRxBuffer[RXBUFFERSIZE];
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
	BSP_LED_Init(LED2);
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);

	LCD_Init();
	LCD_Clear(LCD_COLOR_BLACK);
	LCD_SetDisplayDirect(Dir_4);
	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"   BlueTooth Test   ");
	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"W25Q16 Init...      ");
	W25Q16_Init(Init_Y);
	while(SPI_Flash_ReadID() != W25Q16)
	{
		LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"ID Error!           ");
		HAL_Delay(1000);
		LCD_DisplayStringLine(LCD_LINE_2, (uint8_t *)"                    ");
		HAL_Delay(1000);
	}
	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t *)"W25Q16 Init OK!     ");
	
	if(BlueTooth_Init(&UartHandle, 115200))//波特率必须与蓝牙一致！
	{
		LCD_STRING_ON(0, LCD_LINE_1, (uint8_t *)"蓝牙配置失败！      ", LCD_COLOR_RED);
		Error_Handler();
	}
	LCD_STRING(0, LCD_LINE_1, (uint8_t *)"外设配置完成！      ", LCD_COLOR_WHITE, LCD_COLOR_BLACK);
//	//该操作必须是在蓝牙未建立连接的时候,可以取消该步骤
//	if(BlueToothDetected(&UartHandle))
//	{
//		LCD_STRING_ON(0, LCD_LINE_2, (uint8_t *)"蓝牙检测失败！      ", LCD_COLOR_WHITE);
//	}
//该操作必须在蓝牙未建立连接的时候，并且初始化时的波特率必须保证与蓝牙上电的波特率一致！
//建议不要在程序中修改波特率。
//	HAL_Delay(2000);
//	while(SetBlueToothBaudRate(&UartHandle, BaudRate_115200))
//	{
//		LCD_STRING_ON(0, LCD_LINE_2, (uint8_t *)"波特率设置失败！    ", LCD_COLOR_WHITE);
//		HAL_Delay(1000);
//	}
//	LCD_STRING_ON(0, LCD_LINE_2, (uint8_t *)"波特率设置成功！    ", LCD_COLOR_WHITE);
	LCD_STRING_ON(0, LCD_LINE_2, (uint8_t *)"波特率为\"115200\"  ", LCD_COLOR_WHITE);
	if(BlueToothStartRecv(&UartHandle, (uint8_t *)aRxBuffer) != HAL_OK)
	{
		LCD_STRING_ON(0, LCD_LINE_2, (uint8_t *)"启动接收失败！      ", LCD_COLOR_WHITE);
		Error_Handler();
	}
	LCD_STRING_ON(0, LCD_LINE_3, (uint8_t *)"接收启动！接收内容：", LCD_COLOR_WHITE);

	/* Infinite loop */
	while(1)
	{
		BSP_LED_Toggle(LED4);
		HAL_Delay(50);
	}
}

void BlueToothRecvedHandle(UART_HandleTypeDef *UartHandle)
{
	BSP_LED_Toggle(LED5);
	LCD_DrawFullRect(0, LCD_LINE_4, LCD_PIXEL_WIDTH, 80, LCD_COLOR_BLACK);
	LCD_STRING_ON(0, LCD_LINE_4, (uint8_t *)aRxBuffer, LCD_COLOR_WHITE);
	//send response.
	if(BlueToothTransmit(UartHandle, (uint8_t *)aTxBuffer) != HAL_OK)
	{
		LCD_STRING(0, LCD_LINE_3, (uint8_t *)"发送失败！          ", LCD_COLOR_WHITE, LCD_COLOR_BLACK);
		Error_Handler();
	}
	//restart receive.
	if(BlueToothStartRecv(UartHandle, (uint8_t *)aRxBuffer) != HAL_OK)
	{
		LCD_STRING(0, LCD_LINE_3, (uint8_t *)"启动接收失败！      ", LCD_COLOR_WHITE, LCD_COLOR_BLACK);
		Error_Handler();
	}
}

void BlueToothTansedHandle(UART_HandleTypeDef *UartHandle)
{
	BSP_LED_Toggle(LED6);
}

void BlueToothErrorHandle(UART_HandleTypeDef *UartHandle)
{
	
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
