/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtLCDForGL.h
  * @author  '^_^'
  * @version V0.0.1
  * @date    24-September-2014
  * @modify  5-October-2014
  * @brief   Header file for stm32f4xx_nucleo_ExtLCDForGL.c
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXTLCDFORGL_H
#define __STM32F4XX_NUCLEO_EXTLCDFORGL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#define DIR_HORIZONTAL				0x00
#define DIR_VERTICAL				0x01

/**
  * @brief  LCD Size (Width and Height)
  */
extern uint8_t LCD_PIXEL_WIDTH;
extern uint8_t LCD_PIXEL_HEIGHT;

typedef enum
{
	Dir_1 = 0,
	Dir_2,
	Dir_3,
	Dir_4,
	Dir_1r,
	Dir_2r,
	Dir_3r,
	Dir_4r
}LCD_DirTypeDef;

/** @defgroup MiniLCD_Exported_Macros
  * @{
  */
#define ABS(X)                   ((X) > 0 ? (X) : -(X))
#define ASSEMBLE_RGB(R ,G, B)    (((R) << 11) | ((G) << 5) | (B))

/**
  * @brief  LCD color
  */
#define LCD_COLOR_WHITE          0xFFFF
#define LCD_COLOR_BLACK          0x0000
#define LCD_COLOR_GREY           0xF7DE
#define LCD_COLOR_BLUE           0x001F
#define LCD_COLOR_BLUE2          0x051F
#define LCD_COLOR_RED            0xF800
#define LCD_COLOR_MAGENTA        0xF81F
#define LCD_COLOR_GREEN          0x07E0
#define LCD_COLOR_CYAN           0x7FFF
#define LCD_COLOR_YELLOW         0xFFE0
#define LCD_COLOR_GBLUE          0X07FF/* GREEN + BLUE */
#define LCD_COLOR_BROWN          0XBC40/* BROWN */
#define LCD_COLOR_BRRED          0XFC07/* BROWN + RED */
#define LCD_COLOR_GRAY           0X8430/* BROWN */
#define LCD_COLOR_DARKBLUE       0X01CF/* DARKBLUE */
#define LCD_COLOR_LIGHTBLUE      0X7D7C/* LIGHTBLUE */  
#define LCD_COLOR_GRAYBLUE       0X5458/* GRAY + BLUE */

/**
  * Pins Configuration
  */
#define SDA                      GPIOC
#define SCL                      GPIOB
#define CSX                      GPIOB
#define DCX                      GPIOC
#define BK_LED                   GPIOA

#define SDA_PIN                  GPIO_PIN_2
#define SCL_PIN                  GPIO_PIN_10
#define CSX_PIN                  GPIO_PIN_6
#define DCX_PIN                  GPIO_PIN_3
#define BK_LED_PIN               GPIO_PIN_15

#define SDA_MODE_OUT()           {SDA->MODER &= ~(GPIO_MODER_MODER0 << 4);\
                                  SDA->MODER |= ((GPIO_MODE_OUTPUT_PP) << 4);}
#define SDA_MODE_IN()            {SDA->MODER &= ~(GPIO_MODER_MODER0 << 4);\
                                  SDA->MODER |= ((GPIO_MODE_INPUT) << 4);}

#define SDA_Value()              ((SDA->IDR & SDA_PIN)? GPIO_PIN_SET : GPIO_PIN_RESET)

#define SDA_HIGH()               SDA->BSRRL |= SDA_PIN
#define SDA_LOW()                SDA->BSRRH |= SDA_PIN

#define SCL_HIGH()               SCL->BSRRL |= SCL_PIN
#define SCL_LOW()                SCL->BSRRH |= SCL_PIN

#define CSX_HIGH()               CSX->BSRRL |= CSX_PIN
#define CSX_LOW()                CSX->BSRRH |= CSX_PIN

#define DCX_HIGH()               DCX->BSRRL |= DCX_PIN
#define DCX_LOW()                DCX->BSRRH |= DCX_PIN

#define BK_LED_ON()              BK_LED->BSRRL |= BK_LED_PIN
#define BK_LED_OFF()             BK_LED->BSRRH |= BK_LED_PIN
#define BK_LED_TOG()             BK_LED->ODR ^= BK_LED_PIN

#define LCD_CLK_ENABLE()         {__GPIOA_CLK_ENABLE();\
                                  __GPIOB_CLK_ENABLE();\
                                  __GPIOC_CLK_ENABLE();}

/**
  * @brief LOW LEVEL Exported Functions
  */
void LCD_Init(void);
void LCD_SetDisplayDirect(LCD_DirTypeDef dir);
LCD_DirTypeDef LCD_GetDisplayDirect(void);
void LCD_Clear(uint16_t color);
void LCD_SetRamAddr(uint8_t xStart, uint8_t xEnd, uint8_t yStart, uint8_t yEnd);
void DrawPiont(uint8_t x, uint8_t y, uint16_t color);
uint16_t ReadPoint(uint8_t x, uint8_t y);
void LCD_DrawCircle(uint8_t Xpos, uint8_t Ypos, uint8_t Radius, uint16_t color);
void LCD_DrawLine(uint8_t Xpos, uint8_t Ypos, uint8_t Length, uint8_t Direction,uint16_t color);
void LCD_DrawRect(uint8_t Xpos, uint8_t Ypos, uint8_t Height, uint8_t Width, uint16_t color);
void LCD_DrawFullRect(uint8_t Xpos, uint8_t Ypos, uint8_t Width, uint8_t Height, uint16_t color);
void LCD_DrawFullCircle(uint8_t Xpos, uint8_t Ypos, uint8_t Radius, uint16_t color);
void LCD_DrawUniLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint16_t color);
void ShowPicture(uint8_t x, uint8_t y, uint8_t high, uint8_t width, uint8_t *PIC);
void ShowBMP(uint8_t x, uint8_t y, uint8_t high, uint8_t width,
			 uint8_t *BMP, uint16_t Fcolor, uint16_t Bcolor);

uint32_t ReadRAMDAT(void);
void WriteDAT(uint8_t value);
void WriteCOM(uint8_t value);
void WriteColor(uint16_t color);
void LCD_CtrlLinesConfig(void);

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ***************************************/

