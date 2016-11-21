/**
  ******************************************************************************
  * @file    main.c
  * @author  '^_^'
  * @version V0.0.0
  * @date    4-October-2014
  * @brief   Main program body.
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
#include <stdlib.h>
#include "LcdHal.h"
#include "cursor.h"
#include "graphicObject.h"
//#include "pictures.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//TIM_HandleTypeDef TimHandle;
uint32_t Cur = CUR_RELEASE;
GL_Page_TypeDef *pageStart;

//int16_t MyDataPoints[12] = {13,28,1,20,68,5,16,7,10,8,0,11};//,97,56,43,78,35,57,92,30,47,38,88,99};
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/
void LED3_Toggle(void);
void LED4_Toggle(void);
void LED5_Toggle(void);
void LED6_Toggle(void);
void ChangeBack(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
//	GL_PageControls_TypeDef* PresentationBtn1;
//	GL_PageControls_TypeDef* PresentationCkb1;
//	GL_PageControls_TypeDef* PresentationSwt1;
//	GL_PageControls_TypeDef* pageLabel_0a;
//	GL_PageControls_TypeDef* pageLabel_0b;
//	GL_PageControls_TypeDef* pageLabel_0c;
//	GL_PageControls_TypeDef* LogoIcon;
	GL_PageControls_TypeDef* Slidebar_LV;
	GL_PageControls_TypeDef* Slidebar_RV;
	GL_PageControls_TypeDef* Slidebar_H;
//	GL_PageControls_TypeDef* MyHistogram;
//	GL_PageControls_TypeDef* MyGraphChart;
//	GL_PageControls_TypeDef* ComboBoxGrp;
	GL_RadioButtonGrp_TypeDef* RadioBtn;
	GL_PageControls_TypeDef* RadioPage1;
	GL_PageControls_TypeDef* RadioPage2;
	GL_PageControls_TypeDef* RadioPage3;
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
	BSP_PB_Init(BUTTON1, BUTTON_MODE_GPIO);
	BSP_PB_Init(BUTTON2, BUTTON_MODE_GPIO);
	BSP_PB_Init(BUTTON3, BUTTON_MODE_GPIO);
	BSP_PB_Init(BUTTON4, BUTTON_MODE_GPIO);
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

	Set_LCD_Resolution( 160, 128 );
	/* Initialize the LCD */
	GL_LCD_Init();
	GL_Clear(White);
	LCD_SetDisplayDirect(Dir_4);
//	if(BK_LED_TIMx_Config(&TimHandle))
//	{
//		Error_Handler();
//	}
//	BK_LED_Set(&TimHandle, 30);
//	GL_SetFont(GL_FONT_SMALL);
//	GL_DisplayAdjStringLine(0, 20, (uint8_t *)"ABCDEFGHIJK", GL_TRUE);

	pageStart = malloc(sizeof(GL_Page_TypeDef));
	Create_PageObj( pageStart );

	RadioBtn = NewRadioButtonGrp(2);
	// at most 3 !!!
	RadioPage1 = AddRadioOption(RadioBtn, (uint8_t *)"LD3_Tgl", LED3_Toggle);
	RadioPage2 = AddRadioOption(RadioBtn, (uint8_t *)"LD4_Tgl", LED4_Toggle);
	RadioPage3 = AddRadioOption(RadioBtn, (uint8_t *)"LD5_Tgl", LED5_Toggle);
	AddPageControlObj(20, 50, RadioPage1, pageStart);
	AddPageControlObj(20, 75, RadioPage2, pageStart);
	AddPageControlObj(20, 100, RadioPage3, pageStart);
	
//pass
//	ComboBoxGrp = NewComboBoxGrp(4);
//	AddComboOption(ComboBoxGrp->objPTR, (uint8_t *)"LD4_Tgle", LED4_Toggle);
//	AddComboOption(ComboBoxGrp->objPTR, (uint8_t *)"LD5_Tgle", LED5_Toggle);
//	AddComboOption(ComboBoxGrp->objPTR, (uint8_t *)"LD6_Tgle", LED6_Toggle);
//	AddPageControlObj(0, 40, ComboBoxGrp, pageStart);
	
//pass
//	pageLabel_0a = NewLabel(0, (uint8_t*)"MyGraphChart Test.", GL_HORIZONTAL, GL_FONT_SMALL, GL_Red);
//	AddPageControlObj(12, 0, pageLabel_0a, pageStart);
//	MyGraphChart = NewGraphChart(0, (uint8_t *)"Hours", (uint8_t *)"Wh",
//                                   MyDataPoints, 12, GL_TRUE);
//	AddPageControlObj(21, 16, MyGraphChart, pageStart);
//pass
//	pageLabel_0a = NewLabel(0, (uint8_t*)"MyHistogram Test.", GL_HORIZONTAL, GL_FONT_SMALL, GL_Red);
//	AddPageControlObj(16, 0, pageLabel_0a, pageStart);
//	MyHistogram = NewHistogram(0, (uint8_t *)"Hours", (uint8_t *)"Wh",
//                               MyDataPoints, 24);
//	AddPageControlObj(21, 20, MyHistogram, pageStart);
//pass		
	Slidebar_H = NewSlidebar(5, (uint8_t *)"backlight ADJ", GL_HORIZONTAL, ChangeBack);
	AddPageControlObj(30, 3, Slidebar_H, pageStart);
	Slidebar_LV = NewSlidebar(6, (uint8_t *)"Slidebar2", GL_LEFT_VERTICAL, NullFunc);
	AddPageControlObj(1, 1, Slidebar_LV, pageStart);
	Slidebar_RV = NewSlidebar(7, (uint8_t *)"Slidebar3", GL_RIGHT_VERTICAL, NullFunc);
	AddPageControlObj(140, 1, Slidebar_RV, pageStart);
//pass	
//	pageLabel_0a = NewLabel(0, (uint8_t*)"Graphic Library", GL_HORIZONTAL, GL_FONT_SMALL, GL_Red);
//	AddPageControlObj(16, 0, pageLabel_0a, pageStart);
//	PresentationCkb1 = NewCheckbox(1, (uint8_t*)"LED3_Toggle", LED3_Toggle);
//	AddPageControlObj(25, 65, PresentationCkb1, pageStart);
//	PresentationBtn1 = NewButton(2, (uint8_t*)"LD3_Tgl", LED3_Toggle);
//	AddPageControlObj(84, 102, PresentationBtn1, pageStart);
//	PresentationSwt1 = NewSwitch(3, (uint8_t*)"LD3_Tgl1", (uint8_t*)"LD3_Tgl2", LED3_Toggle);
//	AddPageControlObj(2, 102, PresentationSwt1, pageStart);
//	pageLabel_0b = NewLabel(4, (uint8_t*)"Graphic Library", GL_LEFT_VERTICAL, GL_FONT_SMALL, GL_Blue);
//	AddPageControlObj(10, 20, pageLabel_0b, pageStart);
//	pageLabel_0c = NewLabel(5, (uint8_t*)"Graphic Library", GL_RIGHT_VERTICAL, GL_FONT_SMALL, GL_Blue);
//	AddPageControlObj(20, 10, pageLabel_0c, pageStart);
//	LogoIcon = NewIcon(6, (uint8_t *)STM32Logo, 50, 50, NullFunc);
//	AddPageControlObj(109, 77, LogoIcon, pageStart);
//	GL_Clear(Red);
	GL_SetTextColor(GL_Blue);
	pageStart->ShowPage(pageStart, GL_TRUE);
//pass
//	GL_SetTextColor(GL_Blue);
//	GL_DisplayAdjStringLine(100, 0, (uint8_t *)"disp str..", GL_FALSE);
//	GL_SetFont(GL_FONT_SMALL);
//	LCD_Change_Direction(_90_degree);
//	GL_DisplayAdjStringLine(15, 0, (uint8_t *)"print line1", GL_FALSE);
//	LCD_Change_Direction(_180_degree);
//	GL_DisplayAdjStringLine(15, 0, (uint8_t *)"print line2", GL_FALSE);
//	LCD_Change_Direction(_270_degree);
//	GL_DisplayAdjStringLine(30, 0, (uint8_t *)"print line3", GL_FALSE);
//	LCD_Change_Direction(_0_degree);
//	GL_DisplayAdjStringLine(30, 0, (uint8_t *)"print line3", GL_FALSE);
//pass	
//	GL_SetFont(GL_FONT_SMALL);
//	LCD_Change_Direction(_90_degree);
//	LCD_PrintStringLine(0, 0, (uint8_t *)"print string");
//		LCD_Change_Direction(_180_degree);
//	LCD_PrintStringLine(0, 0, (uint8_t *)"print string");
//		LCD_Change_Direction(_270_degree);
//	LCD_PrintStringLine(0, 0, (uint8_t *)"print string");
//		LCD_Change_Direction(_0_degree);
//	LCD_PrintStringLine(0, 0, (uint8_t *)"print string");
//		GL_SetFont(GL_FONT_BIG);
//	LCD_Change_Direction(_90_degree);
//	LCD_PrintStringLine(15, 0, (uint8_t *)"print1");
//		LCD_Change_Direction(_180_degree);
//	LCD_PrintStringLine(15, 0, (uint8_t *)"print2");
//		LCD_Change_Direction(_270_degree);
//	LCD_PrintStringLine(15, 0, (uint8_t *)"print3");
//		LCD_Change_Direction(_0_degree);
//	LCD_PrintStringLine(15, 0, (uint8_t *)"print4");

	/*Initialize cursor*/
	CursorInit(GL_NULL);
	CursorShow(25, 25);
	while(1)
	{
		ProcessInputData();//pass
		/* Time out calculate for power saving mode */
		TimeOutCalculate();//pass

		Cur = CUR_RELEASE;
		if(BSP_PB_GetState(BUTTON1) == 0)
		{
//			while(BSP_PB_GetState(BUTTON1) == 0);
			Cur = CUR_UP;
		}
		if(BSP_PB_GetState(BUTTON2) == 0)
		{
//			while(BSP_PB_GetState(BUTTON2) == 0);
			Cur = CUR_DOWN;
		}
		if(BSP_PB_GetState(BUTTON3) == 0)
		{
//			while(BSP_PB_GetState(BUTTON3) == 0);
			Cur = CUR_LEFT;
		}
		if(BSP_PB_GetState(BUTTON4) == 0)
		{
//			while(BSP_PB_GetState(BUTTON4) == 0);
			Cur = CUR_RIGHT;
		}
		if(BSP_PB_GetState(BUTTON_KEY) == 0)
		{
//			while(BSP_PB_GetState(BUTTON_KEY) == 0);
			Cur = CUR_PRESS;
		}
		CursorReadBtnstick(Cur);
//		BSP_LED_Toggle(LED4);
//		HAL_Delay(500);
	}
}

void LED3_Toggle(void)
{
	BSP_LED_Toggle(LED3);
}

void LED4_Toggle(void)
{
	BSP_LED_Toggle(LED4);
}
void LED5_Toggle(void)
{
	BSP_LED_Toggle(LED5);
}
void LED6_Toggle(void)
{
	BSP_LED_Toggle(LED6);
}

void ChangeBack(void)
{
//	BK_LED_Set(&TimHandle, Get_SlidebarValue(pageStart, 5));
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
