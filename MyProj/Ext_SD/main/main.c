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
#define SD_CARD_NOT_FORMATTED                    0
#define SD_CARD_FILE_NOT_SUPPORTED               1
#define SD_CARD_OPEN_FAIL                        2
#define FATFS_NOT_MOUNTED                        3
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t BlinkSpeed = 0, str[20];
FATFS SD_FatFs;  /* File system object for SD card logical drive */
char SD_Path[4]; /* SD card logical drive path */
char* pDirectoryFiles[MAX_BMP_FILES];
__IO uint8_t ButtonValue = 0;

//uint8_t Info[16];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);

//static void LED5_Blink(void);
static void SDCard_Config(void);
static void TFT_DisplayErrorMessage(uint8_t message);
static void TFT_DisplayMenu(void);
static void TFT_DisplayImages(void);

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
	BSP_LED_Init(LED5);
	/* Configure the User Button in EXTI Mode */
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

	BSP_PB_Init(BUTTON1, BUTTON_MODE_GPIO);
	BSP_PB_Init(BUTTON2, BUTTON_MODE_GPIO);
	BSP_PB_Init(BUTTON3, BUTTON_MODE_GPIO);
	BSP_PB_Init(BUTTON4, BUTTON_MODE_GPIO);

	LCD_Init();
	LCD_Clear(LCD_COLOR_BLACK);
	LCD_SetColors(0x0000, 0xFFFF);

	SDCard_Config();

	/* Display on TFT Images existing on SD card */
    TFT_DisplayImages();

	/* Infinite loop */
	while (1)
	{

	}
}

/**
  * @brief  SD Card Configuration.
  * @param  None
  * @retval None
  */
static void SDCard_Config(void)
{
  uint32_t counter = 0;
  
  if(FATFS_LinkDriver(&SD_Driver, SD_Path) == 0)
  {
    /* Initialize the SD mounted on adafruit 1.8" TFT shield */
    BSP_SD_Init();
    
    /* Check the mounted device */
    if(f_mount(&SD_FatFs, (TCHAR const*)"/", 0) != FR_OK)
    {
      TFT_DisplayErrorMessage(FATFS_NOT_MOUNTED);
    }  
    else
    {
      /* Initialize the Directory Files pointers (heap) */
      for (counter = 0; counter < MAX_BMP_FILES; counter++)
      {
        pDirectoryFiles[counter] = (char *)malloc(11);
      }
    }
  }
}

/**
  * @brief  Displays demonstration menu.
  * @param  None
  * @retval None
  */
static void TFT_DisplayMenu(void)
{
  uint8_t tmp = 0;
  /* Set Text color */
  LCD_SetTextColor(LCD_COLOR_RED);
  /* Display message */
  LCD_DisplayStringLine(LCD_LINE_0, (uint8_t*)"  NUCLEO-F401RE ");
  LCD_DisplayStringLine(LCD_LINE_1, (uint8_t*)"      DEMO      ");
  
  /* Set Text color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  /* Display message */  
  LCD_DisplayStringLine(LCD_LINE_3, (uint8_t*)"Display images  ");
  LCD_DisplayStringLine(LCD_LINE_4, (uint8_t*)"stored under uSD");
  LCD_DisplayStringLine(LCD_LINE_5, (uint8_t*)"on TFT LCD      ");
  
  /* Set Text color */
  LCD_SetTextColor(LCD_COLOR_BLACK);
  /* Display message */ 
  LCD_DisplayStringLine(LCD_LINE_7, (uint8_t*)"Press BUTTON1   ");
  LCD_DisplayStringLine(LCD_LINE_8, (uint8_t*)"to continue...  ");
 
  /* Wait for BUTTON1 is pressed */
  while (BSP_PB_GetState(BUTTON1) != 0)
  {
  }
  /* Wait for BUTTON1 is released */
  while (BSP_PB_GetState(BUTTON1) == 0)
  {
  }
  
  /* Set Text color */
  LCD_SetTextColor(LCD_COLOR_BLACK);  
  /* Display message */ 
  LCD_DisplayStringLine(LCD_LINE_3, (uint8_t*)"                ");
  LCD_DisplayStringLine(LCD_LINE_4, (uint8_t*)"Press BUTTON1/2 ");
  
  /* Set Text color */
  LCD_SetTextColor(LCD_COLOR_BLUE);
  /* Display message */ 
  LCD_DisplayStringLine(LCD_LINE_5, (uint8_t*)"BUTTON1 for:    ");
  LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*)"Manual Mode     ");
  LCD_DisplayStringLine(LCD_LINE_7, (uint8_t*)"BUTTON2 for:    ");
  LCD_DisplayStringLine(LCD_LINE_8, (uint8_t*)"Automatic Mode  ");

  /* Wait for BUTTON1 or BUTTON2 is pressed */
  while ((BSP_PB_GetState(BUTTON1) != 0) && (BSP_PB_GetState(BUTTON2) != 0))
  {
  }
  if(BSP_PB_GetState(BUTTON1) == 0) tmp = 1;
  /* LCD Clear */
  LCD_Clear(LCD_COLOR_WHITE); 
  
  /* BUTTON1 is pressed: Display Manual mode menu #############################*/
  if(tmp == 1 )
  {
    /* Set Text color */
    LCD_SetTextColor(LCD_COLOR_RED);    
    /* Display message */ 
    LCD_DisplayStringLine(LCD_LINE_1, (uint8_t*)"   Manual Mode  ");
    LCD_DisplayStringLine(LCD_LINE_2, (uint8_t*)"    Selected    "); 
    
    /* Set Text color */
    LCD_SetTextColor(LCD_COLOR_BLUE);      
    /* Display message */               
    LCD_DisplayStringLine(LCD_LINE_5, (uint8_t*)"RIGHT:Next image"); 
    LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*)"LEFT :Previous  ");
    LCD_DisplayStringLine(LCD_LINE_7, (uint8_t*)"SEL  :Switch to ");
    LCD_DisplayStringLine(LCD_LINE_8, (uint8_t*)"automatic mode  ");    
    ButtonValue = 2;  
  }
  /* JOY_DOWN is pressed: Display Automatic mode menu ########################*/
  else if (tmp == 0)
  {
    /* Set Text color */
    LCD_SetTextColor(LCD_COLOR_RED);
    /* Display message */ 
    LCD_DisplayStringLine(LCD_LINE_1, (uint8_t*)" Automatic Mode ");
    LCD_DisplayStringLine(LCD_LINE_2, (uint8_t*)"    Selected    ");

    ButtonValue = 1;  
    HAL_Delay(200);
  }
}

/**
  * @brief  Displays on TFT Images or error messages when error occurred.
  * @param  None
  * @retval None
  */
static void TFT_DisplayImages(void)
{    
//  uint32_t bmplen = 0x00;
//  uint32_t checkstatus = 0x00;
  uint32_t filesnumbers = 0x00;
  uint32_t buttonstatus = 0;
  uint32_t bmpcounter = 0x00;
  DIR directory;
  FRESULT res;

  /* Welcome message */
  TFT_DisplayMenu(); 
  
  /* Open directory */
  res = f_opendir(&directory, "/");
  if((res != FR_OK))
  {
    if(res == FR_NO_FILESYSTEM)
    {
      /* Display message: SD card not FAT formated */
      TFT_DisplayErrorMessage(SD_CARD_NOT_FORMATTED);    
    }
    else
    {
      /* Display message: Fail to open directory */
      TFT_DisplayErrorMessage(SD_CARD_OPEN_FAIL);  
    }
  }
  
  /* Get number of bitmap files */
  filesnumbers = Storage_GetDirectoryBitmapFiles ("/", pDirectoryFiles);    
  /* Set bitmap counter to display first image */
  bmpcounter = 1;
  
  while (1)
  {     
	if(BSP_PB_GetState(BUTTON1) == 0)
	{
		buttonstatus = 1;
		/* Wait for BUTTON1 is released */
		while (BSP_PB_GetState(BUTTON1) == 0);
	}
	else if(BSP_PB_GetState(BUTTON2) == 0)
	{
		buttonstatus = 2;
		/* Wait for BUTTON2 is released */
		while (BSP_PB_GetState(BUTTON2) == 0);
	}
	else if(BSP_PB_GetState(BUTTON3) == 0)
	{
		ButtonValue = 1;
		/* Wait for BUTTON2 is released */
		while (BSP_PB_GetState(BUTTON3) == 0);
	}
	else
	{
		buttonstatus = 0;
	}
    
    if((buttonstatus != 0) && (ButtonValue == 1))
    {      
      ButtonValue++;
      buttonstatus = 0;
    }
    
    /*## Display BMP pictures in Automatic mode ##############################*/
    if(ButtonValue == 1) 
    {
      sprintf((char*)str, "%-11.11s", pDirectoryFiles[bmpcounter -1]);
LCD_DisplayStringLine(LCD_LINE_0, str);      
//      checkstatus = Storage_CheckBitmapFile((const char*)str, &bmplen);
//      
//      if(checkstatus == 0)
//      {
        /* Format the string */
        Storage_OpenReadFile(127, 159, (const char*)str); 
//      }
//      else if (checkstatus == 1)
//      {
//        /* Display message: File not supported */
//        TFT_DisplayErrorMessage(SD_CARD_FILE_NOT_SUPPORTED);
//      }

      bmpcounter++;
      if(bmpcounter > filesnumbers)
      {
        bmpcounter = 1;
      }
    }   
    
    /*## Display BMP pictures in Manual mode #################################*/
    if(ButtonValue == 2)
    {
      if(buttonstatus == 2)
      {
		bmpcounter++;
        if(bmpcounter == filesnumbers)
        {
          bmpcounter = 0;
        }
        sprintf ((char*)str, "%-11.11s", pDirectoryFiles[bmpcounter]);
LCD_DisplayStringLine(LCD_LINE_0, str);          
//        checkstatus = Storage_CheckBitmapFile((const char*)str, &bmplen);
//        
//        if(checkstatus == 0)
//        {
          /* Format the string */
          Storage_OpenReadFile(127, 159, (const char*)str);
//        }
//        else
//        {
//          /* Display message: File not supported */
//          TFT_DisplayErrorMessage(SD_CARD_FILE_NOT_SUPPORTED);
//        }
        buttonstatus = 0;
        ButtonValue = 2;
      }
      else if(buttonstatus == 1)
      {
        if(bmpcounter == 0)
        {
          bmpcounter = filesnumbers;
        }
		bmpcounter--;
        sprintf ((char*)str, "%-11.11s", pDirectoryFiles[bmpcounter]);
LCD_DisplayStringLine(LCD_LINE_0, str);
//        checkstatus = Storage_CheckBitmapFile((const char*)str, &bmplen);
        
//        if(checkstatus == 0)
//        {
          /* Format the string */
          Storage_OpenReadFile(127, 159, (const char*)str); 
//        }
//        else if (checkstatus == 1)
//        {
//          /* Display message: File not supported */
//          TFT_DisplayErrorMessage(SD_CARD_FILE_NOT_SUPPORTED);
//        }
        buttonstatus = 0;
        ButtonValue = 2;
      }
    }
  }  
}

/**
  * @brief  Displays adequate message on TFT available on adafruit 1.8" TFT shield  
  * @param  message: Error message to be displayed on the LCD.
  *   This parameter can be one of following values:   
  *     @arg SD_CARD_NOT_FORMATTED: SD CARD is not FAT formatted
  *     @arg SD_CARD_FILE_NOT_SUPPORTED: File is not supported
  *     @arg SD_CARD_OPEN_FAIL: Failure to open directory
  *     @arg FATFS_NOT_MOUNTED: FatFs is not mounted
  * @retval None
  */
static void TFT_DisplayErrorMessage(uint8_t message)
{
  /* LCD Clear */
  LCD_Clear(LCD_COLOR_WHITE);
  /* Set Text and Back colors */
  LCD_SetBackColor(LCD_COLOR_GREY); 
  LCD_SetTextColor(LCD_COLOR_RED);

  if(message == SD_CARD_NOT_FORMATTED)
  {
    /* Display message */
    LCD_DisplayStringLine(LCD_LINE_5, (uint8_t*)"SD Card is not  ");
    LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*)"FAT formatted.  ");  
    LCD_DisplayStringLine(LCD_LINE_7, (uint8_t*)"Please Format   ");
    LCD_DisplayStringLine(LCD_LINE_8, (uint8_t*)"the microSD card");
    while (1)
    {
    }    
  }
  if(message == SD_CARD_FILE_NOT_SUPPORTED)
  {
    /* Display message */
    LCD_DisplayStringLine(LCD_LINE_5, (uint8_t*)"                ");
    LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*)"File type is not");
    LCD_DisplayStringLine(LCD_LINE_7, (uint8_t*)"supported.      ");
    LCD_DisplayStringLine(LCD_LINE_8, (uint8_t*)"                ");
    while(1)
    {
    }    
  }
  if(message == SD_CARD_OPEN_FAIL)
  {
    /* Display message */
    LCD_DisplayStringLine(LCD_LINE_5, (uint8_t*)"                ");
    LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*)"Open directory  ");
    LCD_DisplayStringLine(LCD_LINE_7, (uint8_t*)"fails.          ");
    LCD_DisplayStringLine(LCD_LINE_8, (uint8_t*)"                ");
    while(1)
    {
    }     
  }
  if(message == FATFS_NOT_MOUNTED)
  {
    /* Display message */
    LCD_DisplayStringLine(LCD_LINE_5, (uint8_t*)"                ");
    LCD_DisplayStringLine(LCD_LINE_6, (uint8_t*)"Cannot mount    ");
    LCD_DisplayStringLine(LCD_LINE_7, (uint8_t*)"FatFs on Drive. "); 
    LCD_DisplayStringLine(LCD_LINE_8, (uint8_t*)"                ");
    while (1)
    {
    }    
  }
}

///**
//  * @brief  Blinks LED2 with two frequencies depending on User press button.
//  * @param  None
//  * @retval None
//  */
//static void LED5_Blink(void)
//{
//  /* Initiate BlinkSpeed variable */ 
//  BlinkSpeed = 0;  
//  
//  /* Infinite loop */
//  while(1)
//  {
//    /* Test on blink speed */
//    if(BlinkSpeed == 0)
//    {
//      BSP_LED_Toggle(LED5);
//      /* Wait for 500ms */      
//      HAL_Delay(500);      
//    }      
//    else if(BlinkSpeed == 1)
//    {
//      BSP_LED_Toggle(LED5);
//      /* Wait for 100ms */
//      HAL_Delay(100); 
//    }
//    else if(BlinkSpeed == 2)
//    {
//      BSP_LED_Toggle(LED5);    
//      /* Wait for 50ms */
//      HAL_Delay(50);  
//    }
//  }
//}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(BlinkSpeed == 2)
  {
    BlinkSpeed = 0;
  }
  else
  {
    BlinkSpeed ++;
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
