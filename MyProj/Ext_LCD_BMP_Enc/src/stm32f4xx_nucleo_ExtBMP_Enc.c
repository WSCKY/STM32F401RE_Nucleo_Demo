/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtLCD.c
  * @author  '^_^'
  * @version V0.0.0
  * @date    9-October-2014
  * @brief   BMP Encode.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_ExtBMP_Enc.h"

/* Private variables ---------------------------------------------------------*/
UINT bw;
FIL f_bmp;

/**
  * @brief  BMP Encode.
  * @param  *filename: file name.
  * @param  x, y, width, height: specified screenshots area.
  * @retval None
  */
uint8_t BMP_ENCODE(uint8_t *filename, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	uint16_t bmpheadsize;
 	BITMAPINFO hbmp;
	uint8_t res = 0;
	uint16_t tx, ty;
	uint16_t *databuf;
	uint16_t pixcnt;
	uint16_t bi4width;
	/* Check Window */
	if(width == 0 || height == 0)return 1;
	if((x + width - 1) > LCD_PIXEL_WIDTH) return 1;
	if((y + height - 1) > LCD_PIXEL_HEIGHT) return 1;

	databuf = (uint16_t *)malloc(256);

	bmpheadsize = sizeof(hbmp);/* Get BMP File Header Size */
	hbmp.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	hbmp.bmiHeader.biWidth = width;
	hbmp.bmiHeader.biHeight = height;
	hbmp.bmiHeader.biPlanes = 1;
	hbmp.bmiHeader.biBitCount = 16;
	hbmp.bmiHeader.biCompression = BI_BITFIELDS;
 	hbmp.bmiHeader.biSizeImage = hbmp.bmiHeader.biHeight * hbmp.bmiHeader.biWidth * \
									hbmp.bmiHeader.biBitCount / 8;
 				   
	hbmp.bmfHeader.bfType = ((uint16_t)'M'<<8) + 'B';
	hbmp.bmfHeader.bfSize = bmpheadsize + hbmp.bmiHeader.biSizeImage;
   	hbmp.bmfHeader.bfOffBits = bmpheadsize;

	hbmp.RGB_MASK[0] = 0X00F800;
	hbmp.RGB_MASK[1] = 0X0007E0;
	hbmp.RGB_MASK[2] = 0X00001F;

	if((hbmp.bmiHeader.biWidth * 2) % 4)
		bi4width = ((hbmp.bmiHeader.biWidth * 2) / 4 + 1) * 4;
	else
		bi4width = hbmp.bmiHeader.biWidth * 2;
	/* Creat a New File */
	res = f_open(&f_bmp, (char *)filename, FA_CREATE_NEW | FA_WRITE);	   
 	f_close(&f_bmp);

 	res = f_open(&f_bmp, (char *)filename, FA_OPEN_EXISTING | FA_WRITE);
	if(res == FR_OK)
	{
		res = f_write(&f_bmp, (uint8_t *)&hbmp, bmpheadsize, &bw);
		for(ty = y + height - 1; hbmp.bmiHeader.biHeight; ty --)
		{
			pixcnt = 0;
 			for(tx = x; pixcnt != (bi4width / 2); )
			{
				if(pixcnt < hbmp.bmiHeader.biWidth)
					databuf[pixcnt] = ReadPoint(tx, ty);
				else
					databuf[pixcnt] = 0xFFFF;
				pixcnt ++;
				tx ++;
			}
			hbmp.bmiHeader.biHeight --;
			res = f_write(&f_bmp, (uint8_t *)databuf, bi4width, &bw);
		}
		f_close(&f_bmp);
	}
	free(databuf);
	return res;
}

/*************************************** END OF FILE ***************************************/
