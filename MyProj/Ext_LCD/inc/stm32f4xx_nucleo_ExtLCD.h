/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtLCD.h
  * @author  '^_^'
  * @version V0.0.1
  * @date    24-September-2014
  * @modify  5-October-2014
  * @brief   Header file for stm32f4xx_nucleo_ExtLCD.c
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXTLCD_H
#define __STM32F4XX_NUCLEO_EXTLCD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "Common/fonts.h"
#include "stm32f4xx_nucleo_ExtLCDConfig.h"

#define DIR_HORIZONTAL				0x00
#define DIR_VERTICAL				0x01

/**
  * @brief LCD default font
  */
#define LCD_DEFAULT_FONT			Font8x16

#define _delay_     delay

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

#if (PICDECODE == 1)

/* Bitmap File Header */
typedef struct tagBITMAPFILEHEADER { 
	unsigned short bfType;              ///* (2) */
	unsigned long  bfSize;              ///* (4) */
	unsigned short bfReserved1;         ///* (2) */
	unsigned short bfReserved2;         ///* (2) */
	unsigned long  bfOffBits;           ///* (4) */
} BITMAPFILEHEADER, tagBITMAPFILEHEADER;
/* Bitmap Information Header */
typedef struct tagBITMAPINFOHEADER
{
	unsigned long  biSize;              ///* (4) */		 												4
	unsigned long  biWidth;             ///* (4) */											 												4
	unsigned long  biHeight;            ///* (4) */											 												4
	unsigned short biPlanes;            ///* (2) */								 													2
	unsigned short biBitCount;          ///* (2) */
	unsigned long  biCompression;       ///* (4) */
	unsigned long  biSizeImage;         ///* (4) */														4
	unsigned long  biXPelsPerMeter;     ///* (4) */																					4
	unsigned long  biYPelsPerMeter;     ///* (4) */																					4
	unsigned long  biClrUsed;           ///* (4) */
	unsigned long  biClrImportant;      ///* (4) */
} BITMAPINFOHEADER,tagBITMAPINFOHEADER;

#endif

/**
  * @brief  LCD Lines depending on the chosen fonts.
  */
#define LCD_LINE_0               LINE(0)
#define LCD_LINE_1               LINE(1)
#define LCD_LINE_2               LINE(2)
#define LCD_LINE_3               LINE(3)
#define LCD_LINE_4               LINE(4)
#define LCD_LINE_5               LINE(5)
#define LCD_LINE_6               LINE(6)
#define LCD_LINE_7               LINE(7)
#define LCD_LINE_8               LINE(8)
#define LCD_LINE_9               LINE(9)
#define LCD_LINE_10              LINE(10)
#define LCD_LINE_11              LINE(11)
#define LCD_LINE_12              LINE(12)
#define LCD_LINE_13              LINE(13)
#define LCD_LINE_14              LINE(14)
#define LCD_LINE_15              LINE(15)
#define LCD_LINE_16              LINE(16)
#define LCD_LINE_17              LINE(17)
#define LCD_LINE_18              LINE(18)
#define LCD_LINE_19              LINE(19)
#define LCD_LINE_20              LINE(20)

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
#define SDA_Value()              HAL_GPIO_ReadPin(SDA, SDA_PIN)
//#define SDA_Value()              ((SDA->IDR & SDA_PIN)? GPIO_PIN_SET : GPIO_PIN_RESET)

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
void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor);
void LCD_GetColors(__IO uint16_t *_TextColor, __IO uint16_t *_BackColor);
void LCD_SetTextColor(__IO uint16_t Color);
void LCD_SetBackColor(__IO uint16_t Color);
void LCD_SetFont(sFONT *fonts);
sFONT *LCD_GetFont(void);
void LCD_ClearLine(uint16_t Line);
void LCD_Clear(uint16_t color);
void LCD_SetRamAddr(uint8_t xStart, uint8_t xEnd, uint8_t yStart, uint8_t yEnd);
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c);
void LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii);
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr);
void LCD_Showphrase(uint16_t refcolumn, uint16_t Line, uint8_t *ptr);
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

#if (SUPPORTCHINESE == 1)

#if (FROMFAT == 0)
	#include "stm32f4xx_nucleo_Ext25Q16.h"
#else
	#include "ff.h"
#endif

void GetHzCode(unsigned char* pBuffer,const unsigned char * c);

void LCD_Write_Char(uint8_t x, uint8_t y, const uint8_t *str, uint16_t tcolor, uint16_t bcolor);
void LCD_Write_Char_On(uint8_t x, uint8_t y, const uint8_t *str, uint16_t tcolor);

void LCD_CHINESE_STR(uint8_t x, uint8_t y, const uint8_t *str, uint16_t tcolor, uint16_t bcolor);
void LCD_CHINESE_STR_ON(uint8_t x, uint8_t y, const uint8_t *str, uint16_t tcolor);

void LCD_STRING(uint8_t x, uint8_t y, const uint8_t *str, uint16_t tcolor, uint16_t bcolor);
void LCD_STRING_ON(uint8_t x, uint8_t y, const uint8_t *str, uint16_t tcolor);

void DrawCharacter(uint8_t x, uint8_t y, uint8_t ascii, uint16_t tcolor, uint16_t bcolor);
void DrawCharacter_On(uint8_t x, uint8_t y, uint8_t ascii, uint16_t tcolor);

#endif /* (SUPPORTCHINESE == 1) */

#if (PICDECODE == 1)

#include "ff.h"
//#include "ucos_ii.h"

BITMAPFILEHEADER LoadBmpHeader(uint8_t *data);
BITMAPINFOHEADER LoadBmpInfor(uint8_t *data);
uint8_t Show_BMP(uint8_t x, uint8_t y, char *path);

#endif /* (PICDECODE == 1) */

#ifdef __cplusplus
}
#endif

#endif

/*************************************** END OF FILE ***************************************/

