/**
  ******************************************************************************
  * @file    LcdHal.c
  * @author  MCD Application Team
  * @modify  '^_^'
  * @version V2.0.0
  * @date    8-October-2014
  * @brief   This file contains all the LCD functions whose
  *          implementation depends on the LCD Type used in your Application.
  *          You only need to change these functions implementations
  *          in order to reuse this code with other LCD
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "LcdHal.h"
#include "gl_fonts.h"


/** @addtogroup Embedded_GUI_Library
  * @{
  */

/** @defgroup LcdHal 
  * @brief LcdHal main functions
  * @{
  */ 

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* WARNING: If LcdHal functions are used by more Graphic Applications, remember that the
   following static variables will be shared among the several Graphic Objects applications */
/* Global variables to handle the right font */
__IO uint8_t GL_Font = GL_FONT_BIG;
__IO uint8_t GL_FontWidth = GL_FONT_BIG_WIDTH;
__IO uint8_t GL_FontHeight = GL_FONT_BIG_HEIGHT;

uint16_t LCD_Height = 128;
uint16_t LCD_Width  = 160;
LCD_Direction_TypeDef LCD_Direction = _0_degree;
  
__IO uint16_t          GL_TextColor;
__IO uint16_t          GL_BackColor;

/* Private function prototypes -----------------------------------------------*/
static void BmpBuffer32BitRead(uint32_t* ptr32BitBuffer, uint8_t* ptrBitmap);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Sets the Text color.
  * @param  Color: specifies the Text color code RGB(5-6-5).
  * @param  GL_TextColor: Text color global variable used by GL_DrawChar
  *         and GL_DrawPicture functions.
  * @retval None
  */
void GL_SetTextColor(__IO uint16_t GL_NewTextColor)
{
  GL_TextColor = GL_NewTextColor;
}

/**
  * @brief  Sets the Background color.
  * @param  Color: specifies the Background color code RGB(5-6-5).
  * @param  GL_BackColor: Background color global variable used by
  *         GL_DrawChar and GL_DrawPicture functions.
  * @retval None
  */
void GL_SetBackColor(__IO uint16_t GL_NewBackColor)
{
  GL_BackColor = GL_NewBackColor;
}

/**
  * @brief  Clears the whole LCD.
  * @param  Color: specifies the Background color code RGB(5-6-5) for the Display.
  * @retval None
  */
void GL_Clear(uint16_t Color)
{
  LCD_Clear(Color);
}

/**
  * @brief  Draws a character on LCD without background.
  * @param  Xpos: the Line where to display the character shape.
  *         This parameter can be one of the following values:
  * @param  Ypos: start column address.
  * @param  c: pointer to the character data.
  * @retval None
  */
void GL_LCD_DrawCharTransparent(uint16_t Xpos, uint16_t Ypos, const uint16_t *c) /* 16bit char */
{
  uint32_t line_index = 0, pixel_index = 0;
  uint8_t Xaddress = 0;
  uint16_t Yaddress = 0;

  Xaddress = Ypos;
  Yaddress = Xpos;

  for (line_index = 0; line_index < GL_FontHeight; line_index++)
  {
    for (pixel_index = 0; pixel_index < GL_FontWidth; pixel_index++)
    {
      /* SmallFonts have bytes in reverse order */
      if (( GL_Font == GL_FONT_BIG   && (((const uint16_t*)c)[line_index] & (1 << pixel_index)) == 0x00) ||
          ( GL_Font == GL_FONT_SMALL && (((const uint16_t*)c)[line_index] & (0x80 >> pixel_index)) == 0x00))
      {
        Xaddress ++;
      }
      else
      {
        LCD_PutPixel(Xaddress ++, Yaddress, GL_TextColor, SinglePixel);
      }
    }
    Yaddress ++;
    Xaddress = Ypos;
  }
}

/**
  * @brief  Displays a maximum of 20 char on the LCD.
  * @param  Line: the Line where to display the character shape.
  * @param  *ptr: pointer to the string to display on LCD.
  * @param  Transparent_Flag: if TRUE the character is printed without changing the background
  * @retval None
  */
void GL_DisplayAdjStringLine(uint16_t Line, uint16_t Column, uint8_t *ptr, GL_bool Transparent_Flag)
{
  uint32_t index = 0;

  /* Send the string character by character on lCD */
  while (*ptr != 0)
  { /* Display one character on LCD */
    GL_LCD_DisplayChar(Line, Column, *ptr, Transparent_Flag);
    /* Decrement the column position by GL_FontWidth */
    if ( *ptr == 'A' || *ptr == 'G' || *ptr == 'M' || *ptr == 'O' || *ptr == 'Q' || *ptr == 'X' || *ptr == 'm')
      Column += (GL_FontWidth);
    else
      Column += (GL_FontWidth - 1);
    /* Point on the next character */
    ptr++;
    /* Increment the character counter */
    index++;
  }
}

/**
  * @brief  Displays one character (16dots width, 24dots height).
  * @param  Line: the Line where to display the character shape.
  *         This parameter can be one of the following values:
  *     @arg  - Linex: where x can be 0..9
  * @param  Column: start column address.
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
  * @param  GL_bool Trasparent_Flag, if TRUE it does not print the GL_BackColor
  * @retval None
  */
void GL_LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii, GL_bool Transparent_Flag)
{
  Ascii -= 32;
  switch (GL_Font)
  {
    case GL_FONT_BIG:
      if (Transparent_Flag == GL_TRUE)
        GL_LCD_DrawCharTransparent(Line, Column, &GL_Font16x24.table[Ascii * GL_FontHeight] );
      else
        GL_LCD_DrawChar(Line, Column, (&GL_Font16x24.table[Ascii * GL_FontHeight]));
      break;

    case GL_FONT_SMALL:
      if (Transparent_Flag == GL_TRUE)
        GL_LCD_DrawCharTransparent(Line, Column, &GL_Font8x16.table[Ascii * GL_FontHeight] );
      else
        GL_LCD_DrawChar(Line, Column, &GL_Font8x16.table[Ascii * GL_FontHeight]);
      break;

    default:
      break;
  }
}

/**
  * @brief  Sets a display window
  * @param  Xpos: specifies the X bottom left position.
  * @param  Ypos: specifies the Y bottom left position.
  * @param  Height: display window height.
  * @param  Width: display window width.
  * @retval None
  */
void GL_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Height, uint16_t Width)
{
  LCD_SetRamAddr((Xpos + 1) - Height, Xpos, (Ypos + 1) - Width, Ypos);
}

/**
  * @brief  Displays a line.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Length: line length.
  * @param  Direction: line direction.
  *         This parameter can be one of the following values:
  *     @arg  Vertical
  *     @arg  Horizontal
  * @retval None
  */
void GL_DrawLine(uint16_t Ypos, uint16_t Xpos, uint16_t Length, uint8_t Direction)
{
  LCD_DrawLine(Xpos, Ypos, Length, Direction, GL_TextColor);
}

/**
  * @brief  Displays a rectangle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Height: display rectangle height.
  * @param  Width: display rectangle width.
  * @retval None
  */
void GL_LCD_DrawRect(uint8_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
  GL_DrawLine(Xpos, Ypos, Width, Horizontal);
  GL_DrawLine((Xpos + Height), Ypos, Width, Horizontal);

  GL_DrawLine(Xpos, Ypos, Height, Vertical);
  GL_DrawLine(Xpos, (Ypos - Width + 1), Height, Vertical);
}

/**
  * @brief  Displays a circle.
  * @param  Xpos: specifies the X position.
  * @param  Ypos: specifies the Y position.
  * @param  Radius: the radius size of the circle
  * @retval None
  */
void GL_LCD_DrawCircle(uint8_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;/* Decision Variable */
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */

  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;

  while (CurX <= CurY)
  {
    DrawPiont(Xpos + CurX, Ypos + CurY, GL_TextColor);

    DrawPiont(Xpos + CurX, Ypos - CurY, GL_TextColor);

    DrawPiont(Xpos - CurX, Ypos + CurY, GL_TextColor);

    DrawPiont(Xpos - CurX, Ypos - CurY, GL_TextColor);

    DrawPiont(Xpos + CurY, Ypos + CurX, GL_TextColor);

    DrawPiont(Xpos + CurY, Ypos - CurX, GL_TextColor);

    DrawPiont(Xpos - CurY, Ypos + CurX, GL_TextColor);

    DrawPiont(Xpos - CurY, Ypos - CurX, GL_TextColor);

    if (D < 0)
    {
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
}

/**
  * @brief  Displays a bitmap picture
  * @param  BmpAddress: Bmp picture address
  * @retval None
  */
void GL_DrawBMP(uint16_t maxX, uint16_t minX, uint16_t maxY, uint16_t minY, uint8_t* ptrBitmap)
{
  uint32_t uDataAddr = 0, uBmpSize = 0;
  uint16_t uBmpData;

  BmpBuffer32BitRead(&uBmpSize, ptrBitmap + 2);
  BmpBuffer32BitRead(&uDataAddr, ptrBitmap + 10);

  switch(LCD_Direction)
  {
	  case _0_degree:   LCD_SetDisplayDirect(Dir_4r);break;
	  case _90_degree:  LCD_SetDisplayDirect(Dir_1r);break;
	  case _180_degree: LCD_SetDisplayDirect(Dir_2r);break;
	  case _270_degree: LCD_SetDisplayDirect(Dir_3r);break;
	  default :         break;
  }
  
  LCD_SetRamAddr(minX, maxX - 1, LCD_PIXEL_HEIGHT - maxY, LCD_PIXEL_HEIGHT - minY);
	/* Read bitmap data and write them to LCD */
  for (; uDataAddr < uBmpSize; uDataAddr += 2)
  {
    uBmpData = (uint16_t)(*(ptrBitmap + uDataAddr)) + (uint16_t)((*(ptrBitmap + uDataAddr + 1)) << 8);
    WriteColor( uBmpData );
  }
  LCD_SetDisplayDirect(LCD_Default_Degree);
  GL_SetDisplayWindow(LCD_Height - 1, LCD_Width - 1, LCD_Height, LCD_Width);
}

/**
  * @brief  Sets the Font (Big or Small).
  * @param  uFont: specifies the Font (GL_FONT_BIG or GL_FONT_SMALL).
  * @retval None
  */
void GL_SetFont(uint8_t uFont)
{
  GL_Font = uFont;

  switch (uFont)
  {
    case GL_FONT_BIG:
      GL_FontWidth  = GL_FONT_BIG_WIDTH;
      GL_FontHeight = GL_FONT_BIG_HEIGHT;
      break;

    case GL_FONT_SMALL:
      GL_FontWidth  = GL_FONT_SMALL_WIDTH;
      GL_FontHeight = GL_FONT_SMALL_HEIGHT;
      break;

    default:
      break;
  } /* End switch */
}

/**
  * @brief  Switches the backlight either ON or OFF
  * @param  state. This parameter can be one of the following values:
  *     @arg   GL_ON
  *     @arg   GL_OFF
  * @retval None
  */
void GL_BackLightSwitch(uint8_t u8_State)
{
  if (u8_State == GL_OFF)
  { /* Turning OFF the LCD Backlight */
    BK_LED_OFF();
//	  BK_LED_Set(&TimHandle, 0);
  }
  else if (u8_State == GL_ON)
  { /* Turning ON the LCD Backlight */
    BK_LED_ON();
//	  BK_LED_Set(&TimHandle, 50);
  }
}

/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval None
  */
void GL_LCD_Init(void)
{
	LCD_Init();
}

/**
  * @brief  Draw one pixel at position given by Xpos, Ypos of color Color.
  *     Three different modes are specified by PixelSpec in order to save time:
  *     1.FirstPixel, MiddlePixel and LasPixel are used for fastened block-writing
  *       to GRAM (for instance: drawing rectangle, horizontal line etc.)
  *     2.SinglePixel is used for drawing stand-alone pixel. (for instance:
  *       drawing circle, character etc.).
  * @param  Xpos: specifies X position
  * @param  Ypos: specifies Y position
  * @param  Color: RGB color of point
  * @param  PixelSpec: specifies Mode of putting pixel.
  *         This parameter can be one of the following values:
  *     @arg  - FirstPixel:  Starting pixel of block-writing sequence.
  *     @arg  - MiddlePixel: Middle-located pixel of block-writing sequence.
  *     @arg  - LasPixel:    Ending pixel of block-writing sequence
  *     @arg  - SinglePixel: Separated pixel.
  * @retval None
  */
void LCD_PutPixel(uint16_t Xpos, uint16_t Ypos, uint16_t Color, uint8_t PixelSpec)
{
  /*Start part of put pixel for first pixel of block and for single one.
  It consists of set the cursor's position and GRAM prepare sequence.*/
  if ((PixelSpec & (FirstPixel | SinglePixel)) != GL_RESET)
  {
     WriteCOM(0x2a);
     WriteDAT(Xpos>>8); /* XS15-XS8 */
     WriteDAT(Xpos);    /* XS7-XS0    (X start address) */
     WriteDAT((LCD_Width - 1)>>8);	/* XE15-XE8 */
     WriteDAT((LCD_Width - 1));    /* XE7-XE0    (X end address) */

     WriteCOM(0x2b);    /* row address set (RASET) */
     WriteDAT(Ypos>>8); /* YS15-YS8 */
     WriteDAT(Ypos);    /* YS7-YS0    (Y start address) */
     WriteDAT(Ypos>>8);//((LCD_Height - 1)>>8); /* YE15-YE8 */
     WriteDAT(Ypos);//((LCD_Height - 1));    /* YE7-YE0    (Y end address) */

     WriteCOM(0x2c);    /* write prepare */
  }

  /*Write pixel's color to GRAM. Obligatory for all modes of PutPixel call.*/
  WriteColor(Color);
}

/**
  * @brief  Get color of pixel located at appropriate position
  * @param  Xpos: specifies X position
  * @param  Ypos: specifies Y position
  * @retval uint16_t - RGB color of required pixel.
  */
uint16_t LCD_GetPixel(uint16_t Xpos, uint16_t Ypos)
{
  return ReadPoint(Xpos, Ypos);
}

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: the Line where to display the character shape.
  *         This parameter can be one of the following values:
  *     @arg  - Linex: where x can be 0..9
  * @param  Ypos: start column address.
  * @param  c: pointer to the character data.
  * @retval None
  */
void GL_LCD_DrawChar(uint8_t Xpos, uint16_t Ypos, const uint16_t *c) /* 16bit char */
{
  uint32_t line_index = 0, pixel_index = 0;

  LCD_SetRamAddr(Ypos, Ypos + GL_FontWidth - 1, Xpos, Xpos + GL_FontHeight - 1);
  for (line_index = 0; line_index < GL_FontHeight; line_index++)
  {
    for (pixel_index = 0; pixel_index < GL_FontWidth; pixel_index++)
    {
      /* SmallFonts have bytes in reverse order */
      if (( GL_Font == GL_FONT_BIG   && (((const uint16_t*)c)[line_index] & (1 << pixel_index)) == 0x00) ||
          ( GL_Font == GL_FONT_SMALL && (((const uint16_t*)c)[line_index] & (0x80 >> pixel_index)) == 0x00))
      {
        WriteColor(GL_BackColor);
      }
      else
      {
        WriteColor(GL_TextColor);
      }
    }
  }
}

/**
  * @brief  Copy 4 bytes from bitmap array to 32Bit buffer
  * @param  ptrBitmap - Bitmap pointer
  * @param  ptr32BitBuffer - 32Bit buffer to fill
  * @retval None
  */
void BmpBuffer32BitRead(uint32_t* ptr32BitBuffer, uint8_t* ptrBitmap)
{
  *ptr32BitBuffer = 0;
  *ptr32BitBuffer = (*ptrBitmap);
  *ptr32BitBuffer += (*(ptrBitmap + 1)) << 8;
  *ptr32BitBuffer += (*(ptrBitmap + 2)) << 16;
  *ptr32BitBuffer += (*(ptrBitmap + 3)) << 24;
}

/**
  * @brief  LCD_Change_Direction
  * @param  RCC_APBPeriph: specifies the APB peripheral to gates its clock.
  * @param  Direction: The Drawing Direction
  *         This parameter can be one of the following values:
  *     @arg  _0_degree
  *     @arg  _90_degree
  *     @arg  _180_degree
  *     @arg  _270_degree
  * @retval None
  */
void LCD_Change_Direction(LCD_Direction_TypeDef Direction)
{
  LCD_Direction = Direction;

  if (LCD_Direction == _0_degree)
  {
    LCD_SetDisplayDirect(Dir_4);
  }
  else if (LCD_Direction == _90_degree)
  {
    LCD_SetDisplayDirect(Dir_1);
  }
  else if (LCD_Direction == _180_degree)
  {
    LCD_SetDisplayDirect(Dir_2);
  }
  else if (LCD_Direction == _270_degree)
  {
    LCD_SetDisplayDirect(Dir_3);
  }
}

/**
  * @brief  LCD_WriteChar
  * @param  Xpos: The X axis position
  * @param  Ypos: The Y axis position
  * @param  *c:   The Character pointer
  * @retval None
  */
void LCD_WriteChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  uint32_t index = 0, counter = 0;

  LCD_SetRamAddr(Xpos, Xpos + GL_FontWidth - 1, Ypos, Ypos + GL_FontHeight - 1);
  for (index = 0; index < GL_FontHeight; index++)
  {
    for (counter = 0; counter < GL_FontWidth; counter++)
    {
      /* SmallFonts have bytes in reverse order */
      if (( GL_Font == GL_FONT_BIG   && (((const uint16_t*)c)[index] & (1 << counter)) == 0x00) ||
          ( GL_Font == GL_FONT_SMALL && (c[index] & (0x80 >> counter)) == 0x00))
      {
        WriteColor(GL_BackColor);
      }
      else
      {
        WriteColor(GL_TextColor);
      }
    }
  }
}

/**
  * @brief  LCD_PrintChar
  * @param  Line:   The X axis position
  * @param  Column: The Y axis position
  * @param  Ascii:  The Character pointer
  * @retval None
  */
void LCD_PrintChar(uint16_t Line, uint16_t Column, uint8_t Ascii)
{
  Ascii -= 32;
  switch (GL_Font)
  {
    case GL_FONT_BIG:
      LCD_WriteChar( Line, Column, &GL_Font16x24.table[Ascii * GL_FontHeight] );
      break;

    case GL_FONT_SMALL:
      LCD_WriteChar( Line, Column, &GL_Font8x16.table[Ascii * GL_FontHeight] );
      break;

    default:
      break;
  }
}

/**
  * @brief  LCD_PrintStringLine
  * @param  Line:   The X axis position
  * @param  Column: The Y axis position
  * @param  *ptr:  The String of characters to be printed
  * @retval None
  */
void LCD_PrintStringLine(uint16_t Line, uint16_t Column, uint8_t *ptr)
{
  uint32_t counter = 0;
  uint8_t max_char_line = 0;

  if((LCD_Direction == _0_degree) || (LCD_Direction == _180_degree))
  {
    max_char_line = (LCD_Width / GL_FontWidth);
  }
  else if((LCD_Direction == _90_degree) || (LCD_Direction == _270_degree))
  {
    max_char_line = (LCD_Height / GL_FontWidth);
  }

  /* Send the string character by character on LCD */
  while ((*ptr != 0) & (counter < max_char_line))
  {
    /* Display one character on LCD */
    LCD_PrintChar(Column, Line, *ptr);
    /* Increment the column position by 16 */
    Column += GL_FontWidth;

    /* Point on the next character */
    ptr++;
    /* Increment the character counter */
    counter++;
  }
}

/**
  * @brief  LCD_DrawColorBMP
  * @param  *ptrBitmap:   The pointer to the image
  * @param  Xpos_Init: The X axis position
  * @param  Ypos_Init: The Y axis position
  * @param  Height:    The Height of the image
  * @param  Width:     The Width of the image
  * @retval None
  */
void LCD_DrawColorBMP(uint8_t* ptrBitmap, uint16_t Xpos_Init, uint16_t Ypos_Init, uint16_t Height, uint16_t Width)
{
  uint32_t uDataAddr = 0, uBmpSize = 0;
  uint16_t uBmpData = 0;

  BmpBuffer32BitRead(&uBmpSize, ptrBitmap + 2);  
  BmpBuffer32BitRead(&uDataAddr, ptrBitmap + 10);  

  if (LCD_Direction == _0_degree)
  {
    LCD_SetDisplayDirect(Dir_4r);
    GL_SetDisplayWindow(LCD_Width - 1 - Xpos_Init, Ypos_Init, Height, Width);
  }
  else if (LCD_Direction == _90_degree)
  {
    LCD_SetDisplayDirect(Dir_1r);
	GL_SetDisplayWindow(Xpos_Init + Width - 1, LCD_Height - 1 - Ypos_Init, Width, Height);
  }
  else if (LCD_Direction == _180_degree)
  {
    LCD_SetDisplayDirect(Dir_2r);
	GL_SetDisplayWindow(LCD_Width - Xpos_Init - Height, Ypos_Init + Width - 1, Height, Width);
  }
  else if (LCD_Direction == _270_degree)
  {
    LCD_SetDisplayDirect(Dir_3r);
	GL_SetDisplayWindow(Xpos_Init, LCD_Height - Ypos_Init - Height, Width, Height);
  }
  LCD_SetRamAddr(Xpos_Init, Xpos_Init + Width - 1, 
                 LCD_PIXEL_HEIGHT - (Ypos_Init - Height - 1), LCD_PIXEL_HEIGHT - Ypos_Init);
  /* Read bitmap data and write them to LCD */
  for (; uDataAddr < uBmpSize; uDataAddr += 2)
  {
    uBmpData = (uint16_t)(*(ptrBitmap + uDataAddr)) + (uint16_t)((*(ptrBitmap + uDataAddr + 1)) << 8);
    WriteColor( uBmpData );
  }

  LCD_SetDisplayDirect(LCD_Default_Degree);
  GL_SetDisplayWindow(LCD_Height - 1, LCD_Width - 1, LCD_Height, LCD_Width);
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
