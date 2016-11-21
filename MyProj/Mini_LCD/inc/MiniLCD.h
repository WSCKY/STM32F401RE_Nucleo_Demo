/**
  ******************************************************************************
  * @file    MiniLCD.h
  * @author  '^_^'
  * @version V0.0.1
  * @date    24-June-2014
  * @brief   header file for MiniLCD.c
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MINILCD_H
#define __MINILCD_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "Common/fonts.h"

#define DIR_HORIZONTAL				0x00
#define DIR_VERTICAL				0x01

/**
  * @brief LCD default font
  */
#define LCD_DEFAULT_FONT			Font8x16

#define _delay_     delay

#define LANDSCAPE					1					/* 1 for landscape, 0 for portrait    */

/**
  * @brief  LCD Size (Width and Height)
  */
#if (LANDSCAPE == 1)
	#define LCD_PIXEL_WIDTH			128
	#define LCD_PIXEL_HEIGHT		160
#else
	#define LCD_PIXEL_WIDTH			160
	#define LCD_PIXEL_HEIGHT		128
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
#define LCD_LINE_21              LINE(21)
#define LCD_LINE_22              LINE(22)
#define LCD_LINE_23              LINE(23)
#define LCD_LINE_24              LINE(24)
#define LCD_LINE_25              LINE(25)
#define LCD_LINE_26              LINE(26)
#define LCD_LINE_27              LINE(27)
#define LCD_LINE_28              LINE(28)
#define LCD_LINE_29              LINE(29)

/** @defgroup MiniLCD_Exported_Macros
  * @{
  */
#define ASSEMBLE_RGB(R ,G, B)	((((R)& 0xF8) << 8) | (((G) & 0xFC) << 3) | (((B) & 0xF8) >> 3))

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
#define WHITE				0xFFFF/*白色*/
#define BLACK				0x0000/*黑色*/	  
#define BLUE				0x001F/*蓝色*/  
#define BRED				0XF81F/*蓝+红*/
#define GBLUE				0X07FF/*绿+蓝*/
#define RED					0xF800/*红色*/
#define MAGENTA				0xF81F/*品红*/
#define GREEN				0x07E0/*绿色*/
#define CYAN				0x7FFF/* 青色 */
#define YELLOW				0xFFE0/*黄色,绿+红*/
#define BROWN				0XBC40/*棕色*/
#define BRRED				0XFC07/*棕红色*/
#define GRAY				0X8430/*灰色*/
#define DARKBLUE			0X01CF/*深蓝色*/
#define LIGHTBLUE			0X7D7C/*浅蓝色*/  
#define GRAYBLUE			0X5458/*灰蓝色*/

#define DATA_PORT GPIOC
#define CTRL_PORT GPIOB

#define DAT_PORT_VAL GPIOC->ODR

#define RST_HIGH()	GPIOB->BSRRL |= GPIO_PIN_0
#define RST_LOW()	GPIOB->BSRRH |= GPIO_PIN_0

#define CS_HIGH()	GPIOB->BSRRL |= GPIO_PIN_1
#define CS_LOW()	GPIOB->BSRRH |= GPIO_PIN_1

#define RS_HIGH()	GPIOB->BSRRL |= GPIO_PIN_2
#define RS_LOW()	GPIOB->BSRRH |= GPIO_PIN_2

#define WR_HIGH()	GPIOB->BSRRL |= GPIO_PIN_3
#define WR_LOW()	GPIOB->BSRRH |= GPIO_PIN_3

/**
  * @brief LOW LEVEL Exported Functions
  */
void LCD_Init(void);
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
void LCD_DrawCircle(uint8_t Xpos, uint8_t Ypos, uint8_t Radius, uint16_t color);
void LCD_DrawLine(uint8_t Xpos, uint8_t Ypos, uint8_t Length, uint8_t Direction,uint16_t color);
void LCD_DrawRect(uint8_t Xpos, uint8_t Ypos, uint8_t Height, uint8_t Width, uint16_t color);
void LCD_DrawFullRect(uint8_t Xpos, uint8_t Ypos, uint8_t Width, uint8_t Height, uint16_t color);
void LCD_DrawFullCircle(uint8_t Xpos, uint8_t Ypos, uint8_t Radius, uint16_t color);
void LCD_DrawUniLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint16_t color);
void ShowPicture(uint8_t x, uint8_t y, uint8_t high, uint8_t width, uint8_t *PIC);
void ShowBMP(uint8_t x, uint8_t y, uint8_t high, uint8_t width,
			 uint8_t *BMP, uint16_t Fcolor, uint16_t Bcolor);

void WriteDAT(uint8_t value);
void WriteCOM(uint8_t value);
void LCD_CtrlLinesConfig(void);

#ifdef __cplusplus
}
#endif

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
