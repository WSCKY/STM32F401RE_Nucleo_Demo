/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtLCD.c
  * @author  '^_^'
  * @version V0.0.1
  * @date    24-September-2014
  * @modify  5-October-2014
  * @brief   This file includes the LCD driver for LCD_ST7735S Liquid Crystal Display Modules.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_ExtLCD.h"
#include "Common/fonts.c"

/** @defgroup ExtLCD_Private_Variables
  * @{
  */
uint8_t LCD_PIXEL_WIDTH = 128;
uint8_t LCD_PIXEL_HEIGHT = 160;

static sFONT *LCD_Currentfonts;
static LCD_DirTypeDef LCD_CurrentDir = Dir_1;
/* Global variables to set the written text color */
static __IO uint16_t TextColor = 0xFFFF, BackColor = 0x0000;

static void delay(__IO uint32_t nCount);

/**********************************************************
						LCD initializations
**********************************************************/
void LCD_Init(void)
{
	/* Configure the LCD Control pins --------------------------------------------*/
	LCD_CtrlLinesConfig();

	WriteCOM(0x01);	    //software reset
	WriteDAT(0x01);
	_delay_(10);
	WriteCOM(0x11);	    //sleep out/power on(SLPOUT)
	WriteDAT(0x11);
	_delay_(5);
	WriteCOM(0x20);		//display inversion off
	WriteDAT(0x20);
	_delay_(5);
	WriteCOM(0x38);	    //ID mode off (IDMOFF)
	WriteDAT(0x38);
	_delay_(5);
	WriteCOM(0x13);	    //Normal display mode on (Partial mode off)
	WriteDAT(0x13);
	_delay_(5);
	WriteCOM(0x3a);	    //color mode Interface pixel format  (COLMOD)
	WriteDAT(0x05);		//16-bit/pixel , 1-times data transfer
	
	WriteCOM(0xc0);		//power control 1      (PWCTR1)
	WriteDAT(0x03);		//default value  LCM=0 set the GVDD voltage=4.65
	
	WriteCOM(0xc5);	   
	WriteDAT(0xc8);	   //VCOMH voltage set 4.300V
	
	WriteCOM(0xc6);
	WriteDAT(0x1f);	   //VCOMAC voltage set 5.550V;
	_delay_(1);
	
	WriteCOM(0xfe);
	WriteDAT(0x00);
	WriteDAT(0x04);
	
	WriteCOM(0x26);	   //gamma sel
	WriteDAT(0x04);
	
	
	if(!(LCD_CurrentDir & 0x01))
	{
		WriteCOM(0x2a);	   //column address set (CASET)
		WriteDAT(0x00);	   //XS15-XS8
		WriteDAT(0x00);	   //XS7-XS0    (X start address)
		WriteDAT(0x00);	   //XE15-XE8
		WriteDAT(0x7f);	   //XE7-XE0    (X end address)

		WriteCOM(0x2b);	   //row address set (RASET)
		WriteDAT(0x00);	   //YS15-YS8
		WriteDAT(0x00);	   //YS7-YS0    (Y start address)
		WriteDAT(0x00);	   //YE15-YE8
		WriteDAT(0x9f);	   //YE7-YE0    (Y end address)

		LCD_PIXEL_WIDTH = 128;
		LCD_PIXEL_HEIGHT = 160;
	}
	else
	{
		WriteCOM(0x2a);	   //column address set (CASET)
		WriteDAT(0x00);	   //XS15-XS8
		WriteDAT(0x00);	   //XS7-XS0    (X start address)
		WriteDAT(0x00);	   //XE15-XE8
		WriteDAT(0x9f);	   //XE7-XE0    (X end address)

		WriteCOM(0x2b);	   //row address set (RASET)
		WriteDAT(0x00);	   //YS15-YS8
		WriteDAT(0x00);	   //YS7-YS0    (Y start address)
		WriteDAT(0x00);	   //YE15-YE8
		WriteDAT(0x7f);	   //YE7-YE0    (Y end address)
		LCD_PIXEL_WIDTH = 160;
		LCD_PIXEL_HEIGHT = 128;
	}

	WriteCOM(0x36);	   //memory data access control  (MADCTR)
	switch(LCD_CurrentDir)
	{
		case Dir_1: WriteDAT(0xC0);	 //→↓  MY=1;MX=1;MV=0;ML=0; RGB=0;MH=0;--;--;Update at H
		break;
		case Dir_2: WriteDAT(0xA4);	 //↓←  MY=1;MX=0;MV=1;ML=0; RGB=0;MH=1;--;--;Update at V
		break;
		case Dir_3: WriteDAT(0x14);	 //←↑  MY=0;MX=0;MV=0;ML=1; RGB=0;MH=1;--;--;Update at H
		break;
		case Dir_4: WriteDAT(0x64);	 //↑→  MY=0;MX=1;MV=1;ML=0; RGB=0;MH=1;--;--;Update at V
		break;

		case Dir_1r: WriteDAT(0x40); //→↑  MY=0;MX=1;MV=0;ML=0; RGB=0;MH=0;--;--;Update at H
		break;
		case Dir_2r: WriteDAT(0xE4); //↓→  MY=1;MX=1;MV=1;ML=0; RGB=0;MH=1;--;--;Update at V
		break;
		case Dir_3r: WriteDAT(0x94); //←↓  MY=1;MX=0;MV=0;ML=1; RGB=0;MH=1;--;--;Update at H
		break;
		case Dir_4r: WriteDAT(0x24); //↑←  MY=0;MX=0;MV=1;ML=0; RGB=0;MH=1;--;--;Update at V
		break;
	}
	WriteCOM(0x29);	  // display on
	LCD_SetFont(&LCD_DEFAULT_FONT);
}

/**
  * @brief  Set LCD Display Direction.
  * @param  dir: Display Direction.
  * @retval None
  */
void LCD_SetDisplayDirect(LCD_DirTypeDef dir)
{
	if((dir & 0x01) != (LCD_CurrentDir & 0x01))
	{
		if(!(dir & 0x01))
		{
			WriteCOM(0x2a);	   //column address set (CASET)
			WriteDAT(0x00);	   //XS15-XS8
			WriteDAT(0x00);	   //XS7-XS0    (X start address)
			WriteDAT(0x00);	   //XE15-XE8
			WriteDAT(0x7f);	   //XE7-XE0    (X end address)

			WriteCOM(0x2b);	   //row address set (RASET)
			WriteDAT(0x00);	   //YS15-YS8
			WriteDAT(0x00);	   //YS7-YS0    (Y start address)
			WriteDAT(0x00);	   //YE15-YE8
			WriteDAT(0x9f);	   //YE7-YE0    (Y end address)

			LCD_PIXEL_WIDTH = 128;
			LCD_PIXEL_HEIGHT = 160;
		}
		else
		{
			WriteCOM(0x2a);	   //column address set (CASET)
			WriteDAT(0x00);	   //XS15-XS8
			WriteDAT(0x00);	   //XS7-XS0    (X start address)
			WriteDAT(0x00);	   //XE15-XE8
			WriteDAT(0x9f);	   //XE7-XE0    (X end address)

			WriteCOM(0x2b);	   //row address set (RASET)
			WriteDAT(0x00);	   //YS15-YS8
			WriteDAT(0x00);	   //YS7-YS0    (Y start address)
			WriteDAT(0x00);	   //YE15-YE8
			WriteDAT(0x7f);	   //YE7-YE0    (Y end address)
			LCD_PIXEL_WIDTH = 160;
			LCD_PIXEL_HEIGHT = 128;
		}
	}
	LCD_CurrentDir = dir;
	WriteCOM(0x36);	   //memory data access control  (MADCTR)
	switch(LCD_CurrentDir)
	{
		case Dir_1: WriteDAT(0xC0);	 //→↓  MY=1;MX=1;MV=0;ML=0; RGB=0;MH=0;--;--;Update at H
					break;
		case Dir_2: WriteDAT(0xA4);	 //↓←  MY=1;MX=0;MV=1;ML=0; RGB=0;MH=1;--;--;Update at V
					break;
		case Dir_3: WriteDAT(0x14);	 //←↑  MY=0;MX=0;MV=0;ML=1; RGB=0;MH=1;--;--;Update at H
					break;
		case Dir_4: WriteDAT(0x64);	 //↑→  MY=0;MX=1;MV=1;ML=0; RGB=0;MH=1;--;--;Update at V
					break;
		case Dir_1r: WriteDAT(0x40); //→↑  MY=0;MX=1;MV=0;ML=0; RGB=0;MH=0;--;--;Update at H
					break;
		case Dir_2r: WriteDAT(0xE4); //↓→  MY=1;MX=1;MV=1;ML=0; RGB=0;MH=1;--;--;Update at V
					break;
		case Dir_3r: WriteDAT(0x94); //←↓  MY=1;MX=0;MV=0;ML=1; RGB=0;MH=1;--;--;Update at H
					break;
		case Dir_4r: WriteDAT(0x24); //↑←  MY=0;MX=0;MV=1;ML=0; RGB=0;MH=1;--;--;Update at V
					break;
	}
}

/**
  * @brief  Get LCD Display Direction.
  * @param  None
  * @retval Display Direction.
  */
LCD_DirTypeDef LCD_GetDisplayDirect(void)
{
	return LCD_CurrentDir;
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
		WriteColor(color);
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
				WriteColor(BackColor);
			}
			else
			{
				WriteColor(TextColor);
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

	WriteColor(color);
}

/**********************************************************
					   read Point
**********************************************************/
uint16_t ReadPoint(uint8_t x, uint8_t y)
{
	uint32_t RamDat;
	uint8_t R, G, B;
	WriteCOM(0x2a);
	WriteDAT(0);	/* XS15-XS8 */
	WriteDAT(x);	/* XS7-XS0    (X start address) */
	WriteDAT(0);	/* XE15-XE8 */
	WriteDAT(x);	/* XE7-XE0    (X end address) */

	WriteCOM(0x2b);	/* row address set (RASET) */
	WriteDAT(0);	/* YS15-YS8 */
	WriteDAT(y);	/* YS7-YS0    (Y start address) */
	WriteDAT(0);	/* YE15-YE8 */
	WriteDAT(y);	/* YE7-YE0    (Y end address) */

	RamDat = ReadRAMDAT();
	R = (RamDat & 0x003E0000) >> 17;
	G = (RamDat & 0x00003F00) >> 8;
	B = (RamDat & 0x0000003E) >> 1;
	return ASSEMBLE_RGB(R, G, B);
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
		LCD_SetRamAddr(Xpos, Xpos + Length - 1, Ypos, Ypos);
	    for(i = 0; i < Length; i++)
			WriteColor(color);
	}
	else
	{
		LCD_SetRamAddr(Xpos, Xpos, Ypos, Ypos + Length - 1);
		for(i = 0; i < Length; i++)
			WriteColor(color);
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
	uint16_t Index;
	LCD_SetRamAddr(Xpos, Xpos + Width - 1, Ypos, Ypos + Height - 1);
	for(Index = (Width * Height); Index > 0; Index --)
		WriteColor(color);
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
				WriteColor(Fcolor);
			}
			else
			{
				WriteColor(Bcolor);
			}
		}
		BMP ++;
	}
}
/**********************************************************
					   read RAM data
**********************************************************/
uint32_t ReadRAMDAT(void)
{
	uint8_t Index;
	uint32_t value = 0x2E;
	CSX_LOW();
	DCX_LOW();
	for(Index = 0; Index < 8; Index ++)
	{
		if(value & 0x80)
			SDA_HIGH();
		else
			SDA_LOW();
		value <<= 1;
		/* SDA is sampled at the rising edge of SCL */
		SCL_LOW();
		SCL_HIGH();
	}
	DCX_HIGH();
	SDA_MODE_IN();
	value = 0;
	//Read Eight Dummy Bytes...
	for(Index = 0; Index < 8; Index ++)
	{
		/* shifts SDA (output data) at the falling edge of SCL */
		SCL_LOW();
		/* samples the SDA (input data) at rising edge of SCL */
		SCL_HIGH();
	}

	for(Index = 0; Index < 24; Index ++)
	{
		value <<= 1;
		if(SDA_Value())
			value |= 0x000001;
		/* shifts SDA (output data) at the falling edge of SCL */
		SCL_LOW();
		/* samples the SDA (input data) at rising edge of SCL */
		SCL_HIGH();
	}

	SDA_MODE_OUT();
	CSX_HIGH();
	return value;
}

/**********************************************************
				   write color data
**********************************************************/
void WriteColor(uint16_t color)
{
	uint8_t Index;
	CSX_LOW();
	DCX_HIGH();

	for(Index = 0; Index < 16; Index ++)
	{
		if(color & 0x8000)
			SDA_HIGH();
		else
			SDA_LOW();
		color <<= 1;
		/* SDA is sampled at the rising edge of SCL */
		SCL_LOW();
		SCL_HIGH();
	}
	CSX_HIGH();
}

/**********************************************************
						write data
**********************************************************/
void WriteDAT(uint8_t value)
{
	uint8_t Index;
	CSX_LOW();
	DCX_HIGH();

	for(Index = 0; Index < 8; Index ++)
	{
		if(value & 0x80)
			SDA_HIGH();
		else
			SDA_LOW();
		value <<= 1;
		SCL_LOW();
		/* SDA is sampled at the rising edge of SCL */
		SCL_HIGH();
	}
	CSX_HIGH();
}

/**********************************************************
						write command
**********************************************************/
void WriteCOM(uint8_t value)
{
	uint8_t Index;
	CSX_LOW();
	DCX_LOW();

	for(Index = 0; Index < 8; Index ++)
	{
		if(value & 0x80)
			SDA_HIGH();
		else
			SDA_LOW();
		value <<= 1;
		/* SDA is sampled at the rising edge of SCL */
		SCL_LOW();
		SCL_HIGH();
	}
	CSX_HIGH();
}

/* Pin Map:
LED-  RST   A0(D/CX) SDA SCL  CS
PA15  RESET PC3      PC2 PB10 PB6
*/
/**
  * @brief  Configures LCD Control lines in geneal mode.
  * @param  None
  * @retval None
  */
void LCD_CtrlLinesConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	LCD_CLK_ENABLE();
/*-- GPIO Configuration ------------------------------------------------------*/
	GPIO_InitStructure.Pin = SCL_PIN | CSX_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(SCL, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = SDA_PIN | DCX_PIN;
	HAL_GPIO_Init(SDA, &GPIO_InitStructure);
	GPIO_InitStructure.Pin = BK_LED_PIN;
	HAL_GPIO_Init(BK_LED, &GPIO_InitStructure);
	BK_LED_ON();
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

#if (SUPPORTCHINESE == 1)

#if (FROMFAT == 1)
FRESULT fres;
UINT countbr;
FIL file;
#endif

/**
  * @简介  从存储介质中获取汉字编码
  * @参数  *pBuffer:用于存储汉字编码的数组
  *		   *c:汉字机内码,例如"储"
  * @返回  无
  */
void GetHzCode(unsigned char *pBuffer, const unsigned char *c)
{
	unsigned char High8bit,Low8bit;
	unsigned long pos;
	High8bit = *c;     /* 取高8位数据 */
	Low8bit = *(c+1);  /* 取低8位数据 */

	if(Low8bit < 0x7f)
		Low8bit -= 0x40;
	else
		Low8bit -= 0x41;
	High8bit -= 0x81;

	pos = ((unsigned long)190 * High8bit + Low8bit) * 32;

	#if (FROMFAT == 1)
		fres = f_open(&file , FilePath, FA_OPEN_EXISTING | FA_READ);
		if ( fres == FR_OK )
	    {
			f_lseek (&file, pos);
			fres = f_read( &file, pBuffer, 32, &countbr );
			f_close(&file);
	    }
	#else
		SPI_Flash_Read(pBuffer, FontADDRESS + pos, 32);
	#endif
}

/**
  *	@简介  根据汉字点阵数据数组显示汉字
  *	@参数  x:起始横坐标
  *		   y:起始纵坐标
  *        *str:汉字点阵数据数组
  *		   tcolor:字体颜色
  *		   bcolor:字体背景色
  *	@返回  无
  */
void LCD_Write_Char(uint8_t x, uint8_t y, const uint8_t *str, uint16_t tcolor, uint16_t bcolor)
{
	uint8_t table[32];
	uint8_t i, j;
	uint16_t tmp_char = 0;
	GetHzCode(table, str);
	LCD_SetRamAddr(x, x + 15, y, y + 15);
	for(i = 0; i < 16; i ++)
	{
		tmp_char=table[i*2];
		tmp_char=(tmp_char<<8);
		tmp_char|=table[2*i+1];
		for(j = 0; j < 16; j++)
		{
			if( (tmp_char>>15-j) & 0x01 == 0x01)
			{
				WriteColor(tcolor);
			}
			else
			{
				WriteColor(bcolor);
			}
		}
		y++;
	}
}

/**
  *	@简介  根据汉字点阵数据数组"悬浮"显示汉字
  *	@参数  x:起始横坐标
  *		   y:起始纵坐标
  *        *str:汉字点阵数据数组
  *		   tcolor:字体颜色
  *	@返回  无
  */
void LCD_Write_Char_On(uint8_t x, uint8_t y,const uint8_t *str, uint16_t tcolor)
{
	uint8_t table[32];
	uint8_t i, j;
	uint16_t tmp_char = 0;
	GetHzCode(table, str);
	for(i = 0; i < 16; i ++)
	{
		tmp_char=table[i*2];
		tmp_char=(tmp_char<<8);
		tmp_char|=table[2*i+1];
		for(j = 0; j < 16; j++)
		{
			if((tmp_char>>15-j) & 0x01 == 0x01)
			{
				DrawPiont(x + j, y + i, tcolor);
			}
		}
	}
}

/**
  *	@简介  显示纯汉字字符串(支持自动换行)
  * @参数  x:起始横坐标
  *		   y:起始纵坐标
  *		   *str:字符串数组
  *		   tcolor:字符串颜色
  *		   bcolor:字符串背景色
  *	@返回  无
  */
void LCD_CHINESE_STR(uint8_t x, uint8_t y, const uint8_t *str, uint16_t tcolor, uint16_t bcolor)
{
	while(*str != '\0')
	{
		LCD_Write_Char(x, y, str, tcolor, bcolor);
		str += 2;
		x += 16;
		if(x >= LCD_PIXEL_WIDTH)
		{
			y += 16;
			x = 0;
		}
	}
}

/**
  *	@简介  显示纯汉字字符串(支持自动换行)
  * @参数  x:起始横坐标
  *		   y:起始纵坐标
  *		   *str:字符串数组
  *		   tcolor:字符串颜色
  *	@返回  无
  */
void LCD_CHINESE_STR_ON(uint8_t x, uint8_t y, const uint8_t *str, uint16_t tcolor)
{
	while(*str != '\0')
	{
		LCD_Write_Char_On(x, y, str, tcolor);
		str += 2;
		x += 16;
		if(x >= LCD_PIXEL_WIDTH)
		{
			y += 16;
			x = 0;
		}
	}
}

/**
  * @简介  显示任意字符串
  * @参数  x:起始横坐标
  *		   y:起始纵坐标
  *		   *str:字符串数组
  *		   tcolor:字符串颜色
  *		   bcolor:字符串背景色
  * @返回  无
  */
void LCD_STRING(uint8_t x, uint8_t y, const uint8_t *str, uint16_t tcolor, uint16_t bcolor)
{
	while(*str != 0)
	{
		if(*str > 0x80)
		{
			if(x + 16 > LCD_PIXEL_WIDTH)
			{
				y += 16;
				x = 0;
			}
			LCD_Write_Char(x,y,str,tcolor,bcolor);
			str += 2;
			x += 16;
		}
		else
		{	
			if((x + 8 > LCD_PIXEL_WIDTH) || (*str == '\n') || (*str == '\r'))
			{
				y += 16;
				x = 0;
			}
			if((*str != '\n') && (*str != '\r'))
			{
				DrawCharacter(x, y, *str, tcolor, bcolor);
				x += 8;
			}
			str ++;
		}
	}
}

/**
  * @简介  显示任意"悬浮"字符串
  * @参数  x:起始横坐标
  *		   y:起始纵坐标
  *		   *str:字符串数组
  *		   tcolor:字符串颜色
  * @返回  无
  */
void LCD_STRING_ON(uint8_t x, uint8_t y,const uint8_t *str, uint16_t tcolor)
{
	while(*str != 0)
	{
		if(*str > 0x80)
		{	
			if(x + 16 > LCD_PIXEL_WIDTH)
			{
				y += 16;
				x = 0;
			}
			LCD_Write_Char_On(x,y,str,tcolor);
			str += 2;
			x += 16;
		}
		else
		{	
			if((x + 8 > LCD_PIXEL_WIDTH) || (*str == '\n') || (*str == '\r'))
			{
				y += 16;
				x = 0;
			}
			if((*str != '\n') && (*str != '\r'))
			{
				DrawCharacter_On(x, y, *str, tcolor);
				x += 8;
			}
			str ++;
		}
	}
}

/**
  * @简介  显示8*16的ASCII字符
  * @参数  x:起始横坐标
  *		   y:起始纵坐标
  *		   ascii:要显示的ASCII字符
  *		   tcolor:字符串颜色
  *		   bcolor:字符串背景色
  * @返回  无
  */
void DrawCharacter(uint8_t x, uint8_t y, uint8_t ascii, uint16_t tcolor, uint16_t bcolor)
{
	uint8_t i,j;
	uint16_t adr;
	ascii -= 32;
	adr = ascii * 16;
	LCD_SetRamAddr(x, x + 7, y, y + 15);
	for(i = 0; i < 16; i ++)
	{
		for(j = 0; j < 8; j++)
		{
			if(((ASCII8x16_Table[adr]>>(7-j)) & 0x01) == 0x01)
			{
				WriteColor(tcolor);
			}
			else
			{
				WriteColor(bcolor);
			}
		}
		y ++;
		adr ++;
	}
}

/**
  * @简介  显示8*16的"悬浮"ASCII字符
  * @参数  x:起始横坐标
  *		   y:起始纵坐标
  *		   ascii:要显示的ASCII字符
  *		   tcolor:字符串颜色
  * @返回  无
  */
void DrawCharacter_On(uint8_t x, uint8_t y, uint8_t ascii, uint16_t tcolor)
{
	uint8_t i,j;
	uint16_t adr;
	ascii -= 32;
	adr = ascii * 16;
	for(i=0;i<16;i++)
	{
		for(j = 0; j < 8; j++)
		{
			if(((ASCII8x16_Table[adr]>>(7-j)) & 0x01) == 0x01)
			{
				DrawPiont(x + j, y + i, tcolor);
			}
		}
		adr ++;
	}
}

#endif

/**	
  * @brief  支持图片解码
  */
#if (PICDECODE == 1)

	FRESULT bmpres;
	FIL bmpfile;
	uint16_t nubr;

#define GETRGB_R(RGB) ((unsigned char)(( ((unsigned short int )RGB) >>11)<<3))
#define GETRGB_G(RGB) ((unsigned char)(( ((unsigned short int )(RGB & 0x7ff)) >>5)<<2))
#define GETRGB_B(RGB) ((unsigned char)(( ((unsigned short int )(RGB & 0x1f))<<3)))
#define RGB24TO16(R,G,B) ((unsigned short int)((((R)>>3)<<11) | (((G)>>2)<<5) | ((B)>>3) ))
/* 对于24位真彩色 每一行的像素宽度必须为4的倍数 否则以0补齐 */
#define WIDTHBYTES(bits) (((bits)+31)/32*4)

BITMAPFILEHEADER LoadBmpHeader(uint8_t *data)
{
	BITMAPFILEHEADER bmpheader;
	bmpheader.bfType		 = 	(data[0]<<8) | data[1];
	bmpheader.bfSize 		 = 	(data[5]<<24) | (data[4]<<16) | (data[3]<<8) | data[2];
	bmpheader.bfOffBits 	 = 	(data[13]<<24) | (data[12]<<16) | (data[11]<<8) | data[10];
	return bmpheader ;
}

BITMAPINFOHEADER LoadBmpInfor(uint8_t *data)
{
	BITMAPINFOHEADER bmpinfor;
	bmpinfor.biSize		 	 = 	(data[17]<<24) | (data[16]<<16) | (data[15]<<8) | data[14];
	bmpinfor.biWidth	 	 = 	(data[21]<<24) | (data[20]<<16) | (data[19]<<8) | data[18];
	bmpinfor.biHeight	 	 = 	(data[25]<<24) | (data[24]<<16) | (data[23]<<8) | data[22];
	bmpinfor.biPlanes 	 	 = 	(data[27]<<8) | data[26];
	bmpinfor.biBitCount	 	 = 	(data[29]<<8) | data[28];
	bmpinfor.biCompression 	 = 	(data[33]<<24) | (data[32]<<16) | (data[31]<<8) | data[30];
	bmpinfor.biSizeImage 	 = 	(data[37]<<24) | (data[36]<<16) | (data[35]<<8) | data[34];
	bmpinfor.biXPelsPerMeter = 	(data[41]<<24) | (data[40]<<16) | (data[39]<<8) | data[38];
	bmpinfor.biYPelsPerMeter = 	(data[45]<<24) | (data[44]<<16) | (data[43]<<8) | data[42];
	bmpinfor.biClrUsed 		 = 	(data[49]<<24) | (data[48]<<16) | (data[47]<<8) | data[46];
	bmpinfor.biClrImportant  = 	(data[53]<<24) | (data[52]<<16) | (data[51]<<8) | data[50];
	return bmpinfor;
}	

unsigned char linebuffer[4 * 320];
uint8_t data[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];

/**
  * @brief  Display BMP File
  * @param  (x,y):position
  * @param  path
  * @retval Display state
  */
uint8_t Show_BMP(uint8_t x, uint8_t y, char *path)
{
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfor;
	unsigned long indexx, indexy, width, height;
	unsigned int numdatwidth;
	/* open file */
	bmpres = f_open(&bmpfile, (char *)path, FA_OPEN_EXISTING | FA_READ);
	if(bmpres != FR_OK)
	{
        LCD_SetTextColor(LCD_COLOR_RED);
		LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"File Error!     ");
		LCD_SetTextColor(LCD_COLOR_BLACK);
		f_close(&bmpfile);
		return 1;
	}
	/* read file header */
	bmpres = f_read(&bmpfile, data, (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)), (void *)&nubr);
	if(bmpres != FR_OK)
	{
        LCD_SetTextColor(LCD_COLOR_RED);
		LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"Header Error!   ");
		LCD_SetTextColor(LCD_COLOR_BLACK);
		f_close(&bmpfile);
		return 1;
	}
	/* analytial data */
	bmpheader = LoadBmpHeader(data);
	bmpinfor = LoadBmpInfor(data);
	if(bmpheader.bfType != 0x424d)
	{
		LCD_SetTextColor(LCD_COLOR_RED);
		LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"Type Error!     ");
		LCD_SetTextColor(LCD_COLOR_BLACK);
		f_close(&bmpfile);
		return 1;
	}
	width = bmpinfor.biWidth;
	height = bmpinfor.biHeight;
	if((width > LCD_PIXEL_WIDTH) || (height > LCD_PIXEL_HEIGHT))
	{
		LCD_SetTextColor(LCD_COLOR_RED);
		LCD_DisplayStringLine(LCD_LINE_0, (uint8_t *)"Size Error!     ");
		LCD_SetTextColor(LCD_COLOR_BLACK);
		f_close(&bmpfile);
		return 1;
	}
	numdatwidth = WIDTHBYTES(width * (bmpinfor.biBitCount));
	/* fixed position */
	f_lseek(&bmpfile, bmpheader.bfOffBits);
	
	WriteCOM(0x36);	   //memory data access control  (MADCTR)
	switch(LCD_CurrentDir)
	{
		case Dir_1: WriteDAT(0x40); //→↑  MY=0;MX=1;MV=0;ML=0; RGB=0;MH=0;--;--;Update at H
					break;
		case Dir_2: WriteDAT(0xE4); //↓→  MY=1;MX=1;MV=1;ML=0; RGB=0;MH=1;--;--;Update at V
					break;
		case Dir_3: WriteDAT(0x94); //←↓  MY=1;MX=0;MV=0;ML=1; RGB=0;MH=1;--;--;Update at H
					break;
		case Dir_4: WriteDAT(0x24); //↑←  MY=0;MX=0;MV=1;ML=0; RGB=0;MH=1;--;--;Update at V
					break;
		default : break;
	}
	LCD_SetRamAddr(x, x + width - 1, LCD_PIXEL_HEIGHT - y - height, LCD_PIXEL_HEIGHT - y - 1);
	for(indexy = 0; indexy < height; indexy ++)
	{
		f_read(&bmpfile, linebuffer, numdatwidth, (void *)&nubr);
		for(indexx = 0; indexx < width; indexx ++)
		{
			WriteColor(RGB24TO16(linebuffer[indexx * 3 + 2], \
				linebuffer[indexx * 3 + 1], linebuffer[indexx * 3]));
		}
	}

	WriteCOM(0x36);	   //memory data access control  (MADCTR)
	switch(LCD_CurrentDir)
	{
		case Dir_1: WriteDAT(0xC0);	 //→↓  MY=1;MX=1;MV=0;ML=0; RGB=0;MH=0;--;--;Update at H
					break;
		case Dir_2: WriteDAT(0xA4);	 //↓←  MY=1;MX=0;MV=1;ML=0; RGB=0;MH=1;--;--;Update at V
					break;
		case Dir_3: WriteDAT(0x14);	 //←↑  MY=0;MX=0;MV=0;ML=1; RGB=0;MH=1;--;--;Update at H
					break;
		case Dir_4: WriteDAT(0x64);	 //↑→  MY=0;MX=1;MV=1;ML=0; RGB=0;MH=1;--;--;Update at V
					break;
		default: break;
	}

	f_close(&bmpfile);
	return 0;
}

#endif

/*************************************** END OF FILE ***************************************/
