/**
  ******************************************************************************
  * @file    MiniLCD.c
  * @author  '^_^'
  * @version V0.0.1
  * @date    24-June-2014
  * @brief   This file includes the LCD driver for LCD_ST7735 Liquid Crystal Display Modules.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "MiniLCD.h"
#include "Common/fonts.c"

/** @defgroup MiniLCD_Private_Variables
  * @{
  */
static sFONT *LCD_Currentfonts;
/* Global variables to set the written text color */
static __IO uint16_t TextColor = 0x0000, BackColor = 0xFFFF;

static void delay(__IO uint32_t nCount);

/**********************************************************
						LCD initializations
**********************************************************/
void LCD_Init(void)
{
	/* Configure the LCD Control pins --------------------------------------------*/
	LCD_CtrlLinesConfig();

//	RST_HIGH();
//	HAL_Delay(100);
	RST_LOW();
//	HAL_Delay(100);
	_delay_(10);
	RST_HIGH();
//	HAL_Delay(100);

	WriteCOM(0x01);	    //software reset
	WriteDAT(0x01);
//	HAL_Delay(100);
	_delay_(10);
	WriteCOM(0x11);	    //sleep out/power on(SLPOUT)
	WriteDAT(0x11);
//	HAL_Delay(50);
	_delay_(5);
	WriteCOM(0x20);		//display inversion off
	WriteDAT(0x20);
//	HAL_Delay(50);
	_delay_(5);
	WriteCOM(0x38);	    //ID mode off (IDMOFF)
	WriteDAT(0x38);
//	HAL_Delay(50);
	_delay_(5);
	WriteCOM(0x13);	    //Normal display mode on (Partial mode off)
	WriteDAT(0x13);
//	HAL_Delay(50);
	_delay_(5);
	WriteCOM(0x3a);	    //color mode Interface pixel format  (COLMOD)
	WriteDAT(0x05);		//16-bit/pixel , 1-times data transfer
	
	WriteCOM(0xc0);		//power control 1      (PWCTR1)
	WriteDAT(0x03);		//default value  LCM=0 set the GVDD voltage=4.65
	
	WriteCOM(0xc5);	   
	WriteDAT(0xc8);	   //VCOMH voltage set 4.300V
	
	WriteCOM(0xc6);
	WriteDAT(0x1f);	   //VCOMAC voltage set 5.550V ;
//	HAL_Delay(10);
	_delay_(1);
	
	WriteCOM(0xfe);
	WriteDAT(0x00);
	WriteDAT(0x04);
	
	WriteCOM(0x26);	   //gamma sel
	WriteDAT(0x04);
	
	WriteCOM(0x2a);	   //column address set (CASET)
	#if (LANDSCAPE == 1)
		WriteDAT(0x00);	   //XS15-XS8
		WriteDAT(0x00);	   //XS7-XS0    (X start address)
		WriteDAT(0x00);	   //XE15-XE8
		WriteDAT(0x7f);	   //XE7-XE0    (X end address)
	#else
		WriteDAT(0x00);	   //XS15-XS8
		WriteDAT(0x00);	   //XS7-XS0    (X start address)
		WriteDAT(0x00);	   //XE15-XE8
		WriteDAT(0x9f);	   //XE7-XE0    (X end address)
	#endif
	WriteCOM(0x2b);	   //row address set (RASET)
	#if (LANDSCAPE == 1)
		WriteDAT(0x00);	   //YS15-YS8
		WriteDAT(0x00);	   //YS7-YS0    (Y start address)
		WriteDAT(0x00);	   //YE15-YE8
		WriteDAT(0x9f);	   //YE7-YE0    (Y end address)
	#else
		WriteDAT(0x00);	   //YS15-YS8
		WriteDAT(0x00);	   //YS7-YS0    (Y start address)
		WriteDAT(0x00);	   //YE15-YE8
		WriteDAT(0x7f);	   //YE7-YE0    (Y end address)
	#endif
	WriteCOM(0x36);	   //memory data access control  (MADCTR)
	#if (LANDSCAPE == 1)
		WriteDAT(0xc0);	 //→↓(竖屏)  ML=0;MH=0;MV=0;MX=0;MY=0;(起点在左上角)
	#else
		WriteDAT(0x64);	 //↑→(横屏)	 ML=1;MH=1;MV=0;MX=1;MY=0;(起点在左下角)
	#endif
	WriteCOM(0x29);	  // display on
	LCD_SetFont(&LCD_DEFAULT_FONT);
}

/**
  * @brief  Sets the LCD Text and Background colors.
  * @param  _TextColor: specifies the Text Color.
  * @param  _BackColor: specifies the Background Color.
  * @retval None
  */
void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor)
{
	TextColor = _TextColor;
	BackColor = _BackColor;
}

/**
  * @brief  Gets the LCD Text and Background colors.
  * @param  _TextColor: pointer to the variable that will contain the Text
            Color.
  * @param  _BackColor: pointer to the variable that will contain the Background
            Color.
  * @retval None
  */
void LCD_GetColors(__IO uint16_t *_TextColor, __IO uint16_t *_BackColor)
{
	*_TextColor = TextColor; *_BackColor = BackColor;
}

/**
  * @brief  Sets the Text color.
  * @param  Color: specifies the Text color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetTextColor(__IO uint16_t Color)
{
	TextColor = Color;
}

/**
  * @brief  Sets the Background color.
  * @param  Color: specifies the Background color code RGB(5-6-5).
  * @retval None
  */
void LCD_SetBackColor(__IO uint16_t Color)
{
	BackColor = Color;
}

/**
  * @brief  Sets the Text Font.
  * @param  fonts: specifies the font to be used.
  * @retval None
  */
void LCD_SetFont(sFONT *fonts)
{
	LCD_Currentfonts = fonts;
}

/**
  * @brief  Gets the Text Font.
  * @param  None.
  * @retval the used font.
  */
sFONT *LCD_GetFont(void)
{
	return LCD_Currentfonts;
}

/**
  * @brief  Clears the selected line.
  * @param  Line: the Line to be cleared.
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..n
  * @retval None
  */
void LCD_ClearLine(uint16_t Line)
{
	uint16_t refcolumn = 0;
	/* Send the string character by character on lCD */
	while ((refcolumn + LCD_Currentfonts->Width) <= LCD_PIXEL_WIDTH)
	{
		/* Display one character on LCD */
		LCD_DisplayChar(Line, refcolumn, ' ');
		/* Decrement the column position by 16 */
		refcolumn += LCD_Currentfonts->Width;
	}
}

/**********************************************************
						clear screen
**********************************************************/
void  LCD_Clear(uint16_t color)
{
	unsigned int num;
	LCD_SetRamAddr(0, LCD_PIXEL_WIDTH - 1, 0, LCD_PIXEL_HEIGHT - 1);
	for(num = 20480; num > 0; num --)
	{
		WriteDAT(color>>8);/* High Byte */
		WriteDAT(color);/* Low Byte */
	}
}

/**********************************************************
						set display windows
**********************************************************/
void LCD_SetRamAddr(uint8_t xStart, uint8_t xEnd, uint8_t yStart, uint8_t yEnd)
{
	WriteCOM(0x2a);
	WriteDAT(xStart>>8);   /* XS15-XS8 */
	WriteDAT(xStart);	   /* XS7-XS0    (X start address) */
	WriteDAT(xEnd>>8);	   /* XE15-XE8 */
	WriteDAT(xEnd);		   /* XE7-XE0    (X end address) */
	
	WriteCOM(0x2b);		   /* row address set (RASET) */
	WriteDAT(yStart>>8);   /* YS15-YS8 */
	WriteDAT(yStart);	   /* YS7-YS0    (Y start address) */
	WriteDAT(yEnd>>8);	   /* YE15-YE8 */
	WriteDAT(yEnd);		   /* YE7-YE0    (Y end address) */
	
	WriteCOM(0x2c);	/* write prepare */
}

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: the Line where to display the character shape.
  * @param  Ypos: start column address.
  * @param  c: pointer to the character data.
  * @retval None
  */
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, const uint16_t *c)
{
	uint32_t index = 0, i = 0;
	LCD_SetRamAddr(Xpos, Xpos + LCD_Currentfonts->Width -1, Ypos, Ypos + LCD_Currentfonts->Height - 1);
	for(index = 0; index < LCD_Currentfonts->Height; index++)
	{
		for(i = 0; i < LCD_Currentfonts->Width; i++)
		{
			if((((c[index] & ((0x80 << ((LCD_Currentfonts->Width / 12 ) * 8 ) ) >> i)) == 0x00) &&(LCD_Currentfonts->Width <= 12))||
				(((c[index] & (0x1 << i)) == 0x00)&&(LCD_Currentfonts->Width > 12 )))
			{
				WriteDAT(BackColor >> 8);
				WriteDAT(BackColor);
			}
			else
			{
				WriteDAT(TextColor >> 8);
				WriteDAT(TextColor);
			}
		}
	}
}

/**
  * @brief  Displays one character (16dots width, 24dots height).
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  Column: start column address.
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E.
  * @retval None
  */
void LCD_DisplayChar(uint16_t Line, uint16_t Column, uint8_t Ascii)
{
	Ascii -= 32;
	LCD_DrawChar(Column, Line, &LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height]);
}

/**
  * @brief  Displays a maximum of 20 char on the LCD.
  * @param  Line: the Line where to display the character shape .
  *   This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  *ptr: pointer to string to display on LCD.
  * @retval None
  */
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr)
{
	uint16_t refcolumn = 0;
	/* Send the string character by character on lCD */
	while ((*ptr != 0) & ((refcolumn + LCD_Currentfonts->Width) <= LCD_PIXEL_WIDTH))
	{
		/* Display one character on LCD */
		LCD_DisplayChar(Line, refcolumn, *ptr);
		/* Decrement the column position by 16 */
		refcolumn += LCD_Currentfonts->Width;
		/* Point on the next character */
		ptr++;
	}
}

/**
  * @brief
  * @param	refcolumn: start column address.
  * @param  Line: the Line where to display the character shape .
  *	  This parameter can be one of the following values:
  *     @arg Linex: where x can be 0..9
  * @param  *ptr: pointer to string to display on LCD.
  * @retval None
  */
void LCD_Showphrase(uint16_t refcolumn, uint16_t Line, uint8_t *ptr)
{
	uint8_t enterflag=0;
	/* Send the string character by character on lCD */
	while ((*ptr != 0) )
	{	
		if((refcolumn + LCD_Currentfonts->Width) > LCD_PIXEL_WIDTH)
		{
			refcolumn = 0;
			Line += LCD_Currentfonts->Height;
			enterflag = 1;
		}
		if((*ptr=='\r')||(*ptr=='\n'))
		{
			if(!enterflag)
			{
				enterflag = 1;
				refcolumn = 0;
				Line += ((sFONT *)LCD_GetFont())->Height;
			}
		}
		else
		{
			if(enterflag)enterflag = 0;
			/* Display one character on LCD */
			LCD_DisplayChar(Line, refcolumn, *ptr);
			/* Decrement the column position by 16 */
			refcolumn += LCD_Currentfonts->Width;
		}
		/* Point on the next character */
		ptr++;
	}
}

/**********************************************************
						draw point
**********************************************************/
void DrawPiont(uint8_t x, uint8_t y, uint16_t color)
{
	WriteCOM(0x2a);
	WriteDAT(x>>8); /* XS15-XS8 */
	WriteDAT(x);	/* XS7-XS0    (X start address) */
	WriteDAT(x>>8);	/* XE15-XE8 */
	WriteDAT(x);	/* XE7-XE0    (X end address) */

	WriteCOM(0x2b);	/* row address set (RASET) */
	WriteDAT(y>>8); /* YS15-YS8 */
	WriteDAT(y);	/* YS7-YS0    (Y start address) */
	WriteDAT(y>>8);	/* YE15-YE8 */
	WriteDAT(y);	/* YE7-YE0    (Y end address) */

	WriteCOM(0x2c);	/* write prepare */

	WriteDAT(color>>8);/* High Byte */
	WriteDAT(color);/* Low Byte */
}

/**********************************************************
						draw circle
**********************************************************/
void LCD_DrawCircle(uint8_t Xpos, uint8_t Ypos, uint8_t Radius, uint16_t color)
{
	int D;/* Decision Variable */
	uint16_t CurX;/* Current X Value */
	uint16_t CurY;/* Current Y Value */
	D = 3 - (Radius << 1);
	CurX = 0;
	CurY = Radius;
	while (CurX <= CurY)
	{
		DrawPiont(Xpos + CurX, Ypos + CurY,color);
		DrawPiont(Xpos + CurX, Ypos - CurY,color);
		DrawPiont(Xpos - CurX, Ypos + CurY,color);
		DrawPiont(Xpos - CurX, Ypos - CurY,color);
		DrawPiont(Xpos + CurY, Ypos + CurX,color);
		DrawPiont(Xpos + CurY, Ypos - CurX,color);
		DrawPiont(Xpos - CurY, Ypos + CurX,color);
		DrawPiont(Xpos - CurY, Ypos - CurX,color);
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

/**********************************************************
						draw line
**********************************************************/
void LCD_DrawLine(uint8_t Xpos, uint8_t Ypos, uint8_t Length, uint8_t Direction,uint16_t color)
{
	uint8_t i = 0;
	if(Direction == DIR_HORIZONTAL)
	{
	    for(i = 0; i < Length; i++)
		{
			DrawPiont(Xpos, Ypos,color);
			Xpos++;
		}
	}
	else
	{
		for(i = 0; i < Length; i++)
		{
			DrawPiont(Xpos, Ypos,color);
			Ypos++;
		}
	}
}

/**********************************************************
						draw rectangle
**********************************************************/
void LCD_DrawRect(uint8_t Xpos, uint8_t Ypos, uint8_t Height, uint8_t Width, uint16_t color)
{
	LCD_DrawLine(Xpos, Ypos, Width, DIR_HORIZONTAL,color);
	LCD_DrawLine(Xpos,(Ypos+Height-1), Width, DIR_HORIZONTAL,color);
	LCD_DrawLine(Xpos, Ypos, Height, DIR_VERTICAL,color);
	LCD_DrawLine(Xpos+Width-1, Ypos, Height, DIR_VERTICAL,color);
}

/**********************************************************
						draw full rectangle
**********************************************************/
void LCD_DrawFullRect(uint8_t Xpos, uint8_t Ypos, uint8_t Width, uint8_t Height, uint16_t color)
{
	while(Height--)
	{
		LCD_DrawLine(Xpos, Ypos++, Width, DIR_HORIZONTAL,color);
	}
}

/**********************************************************
						draw full circle
**********************************************************/
void LCD_DrawFullCircle(uint8_t Xpos, uint8_t Ypos, uint8_t Radius, uint16_t color)
{
	int D;    /* Decision Variable */
	uint16_t CurX;/* Current X Value */
	uint16_t CurY;/* Current Y Value */
	D = 3 - (Radius << 1);
	CurX = 0;
	CurY = Radius;
	while (CurX <= CurY)
	{
		LCD_DrawRect(Xpos-CurX,Ypos-CurY,2*CurY,2*CurX,color);
		LCD_DrawRect(Xpos-CurY,Ypos-CurX,2*CurX,2*CurY,color);
		if (D < 0)
		{
			D += (CurX << 2) + 6;
		}
		else
		{
			D += ((CurX - CurY) << 2) + 10;
			CurY --;
		}
		CurX ++;
	}
}

/**********************************************************
						draw uniline
**********************************************************/
void LCD_DrawUniLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,uint16_t color)
{
	int16_t t; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
	delta_x = x2 - x1;
	delta_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 
	if(delta_x > 0) incx = 1;
	else if(delta_x == 0) incx = 0;
	else {incx = -1; delta_x = -delta_x;} 
	if(delta_y>0)incy = 1; 
	else if(delta_y == 0) incy = 0;
	else{incy = -1; delta_y = -delta_y;} 
	if( delta_x>delta_y)distance = delta_x;
	else distance = delta_y; 
	for(t = 0; t <= distance + 1; t ++ )
	{  
		DrawPiont(uRow, uCol, color);
		xerr += delta_x ; 
		yerr += delta_y ; 
		if(xerr > distance) 
		{ 
			xerr -= distance; 
			uRow += incx; 
		} 
		if(yerr > distance) 
		{ 
			yerr -= distance; 
			uCol += incy; 
		} 
	}
}

/**********************************************************
						draw picture
**********************************************************/
void ShowPicture(uint8_t x, uint8_t y, uint8_t high, uint8_t width, uint8_t *PIC)
{
	LCD_SetRamAddr(x, x + width - 1, y, y + high - 1);
	while(*PIC > 0)
	{
		WriteDAT(*(PIC + 1));
		WriteDAT(*PIC);
		PIC += 2;
	}
}

/**********************************************************
						show bitmap
**********************************************************/
void ShowBMP(uint8_t x, uint8_t y, uint8_t high, uint8_t width,
			 uint8_t *BMP, uint16_t Fcolor, uint16_t Bcolor)
{
	uint16_t COUN, size;
	LCD_SetRamAddr(x, x + width - 1, y, y + high - 1);
	for(size = 0; size < (high * width / 8); size ++)
	{
		for(COUN = 0; COUN < 8; COUN ++)
		{
			if(((*BMP) >> COUN) & 0x01)
			{
				WriteDAT(Fcolor >> 8);
				WriteDAT(Fcolor);
			}
			else
			{
				WriteDAT(Bcolor >> 8);
				WriteDAT(Bcolor);
			}
		}
		BMP ++;
	}
}

/**********************************************************
						write data
**********************************************************/
void WriteDAT(uint8_t value)
{
	uint16_t port_val;
	CS_LOW();
	RS_HIGH();
	port_val = DAT_PORT_VAL & 0xFF00;
	DAT_PORT_VAL = port_val | value;
//	DAT_PORT_VAL = value;
	WR_LOW();
	WR_HIGH();
	CS_HIGH();
}

/**********************************************************
						write command
**********************************************************/
void WriteCOM(uint8_t value)
{
	uint16_t port_val;
	CS_LOW();
	RS_LOW();
	port_val = DAT_PORT_VAL & 0xFF00;
	DAT_PORT_VAL = port_val | value;
//	DAT_PORT_VAL = value;
	WR_LOW();
	WR_HIGH();
	CS_HIGH();
}

/**
  * @brief  Configures LCD Control lines in geneal mode.
  * @param  None
  * @retval None
  */
void LCD_CtrlLinesConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	__GPIOC_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();

/*-- GPIO Configuration ------------------------------------------------------*/
	/* PCs -data port */
	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | 
									GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* PBs -control line */
	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void delay(__IO uint32_t nCount)
{
	__IO uint32_t index = 0;
	for(index = (50000 * nCount); index != 0; index--)
	{
	}
}

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
