/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtLCDForGL.c
  * @author  '^_^'
  * @version V0.0.1
  * @date    24-September-2014
  * @modify  5-October-2014
  * @brief   This file includes the LCD driver for LCD_ST7735S Liquid Crystal Display Modules.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_ExtLCDForGL.h"

/** @defgroup ExtLCD_Private_Variables
  * @{
  */
uint8_t LCD_PIXEL_WIDTH = 128;
uint8_t LCD_PIXEL_HEIGHT = 160;

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
	delay(10);
	WriteCOM(0x11);	    //sleep out/power on(SLPOUT)
	WriteDAT(0x11);
	delay(5);
	WriteCOM(0x20);		//display inversion off
	WriteDAT(0x20);
	delay(5);
	WriteCOM(0x38);	    //ID mode off (IDMOFF)
	WriteDAT(0x38);
	delay(5);
	WriteCOM(0x13);	    //Normal display mode on (Partial mode off)
	WriteDAT(0x13);
	delay(5);
	WriteCOM(0x3a);	    //color mode Interface pixel format  (COLMOD)
	WriteDAT(0x05);		//16-bit/pixel , 1-times data transfer
	
	WriteCOM(0xc0);		//power control 1      (PWCTR1)
	WriteDAT(0x03);		//default value  LCM=0 set the GVDD voltage=4.65
	
	WriteCOM(0xc5);	   
	WriteDAT(0xc8);	   //VCOMH voltage set 4.300V
	
	WriteCOM(0xc6);
	WriteDAT(0x1f);	   //VCOMAC voltage set 5.550V;
	delay(1);
	
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

/*************************************** END OF FILE ***************************************/
