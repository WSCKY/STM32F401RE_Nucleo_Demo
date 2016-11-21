/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtBMP_Enc.h
  * @author  '^_^'
  * @version V0.0.0
  * @date    9-October-2014
  * @brief   Header file for stm32f4xx_nucleo_ExtBMP_Enc.c
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXTBMP_ENC_H
#define __STM32F4XX_NUCLEO_EXTBMP_ENC_H

/* Includes ------------------------------------------------------------------*/
#include "ff.h"
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_ExtLCD.h"

/* Exported constants --------------------------------------------------------*/
#if (PICDECODE != 1)

/* Bitmap File Header */
typedef __packed struct tagBITMAPFILEHEADER { 
	unsigned short bfType;              ///* (2) */
	unsigned long  bfSize;              ///* (4) */
	unsigned short bfReserved1;         ///* (2) */
	unsigned short bfReserved2;         ///* (2) */
	unsigned long  bfOffBits;           ///* (4) */
} BITMAPFILEHEADER, tagBITMAPFILEHEADER;
/* Bitmap Information Header */
typedef __packed struct tagBITMAPINFOHEADER
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

#endif /* (PICDECODE != 1) */

/* color table */
typedef __packed struct 
{
    uint8_t rgbBlue;///* The blue intensity */
    uint8_t rgbGreen;///* The green intensity */
    uint8_t rgbRed;///* The red intensity */
    uint8_t rgbReserved ;///* reserved */
}RGBQUAD ;

typedef __packed struct
{
	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;  
	uint32_t RGB_MASK[3];//RGB Mask
}BITMAPINFO;
typedef RGBQUAD * LPRGBQUAD;

/* Exported macro ------------------------------------------------------------*/
/* Data compression type */
#define BI_RGB	 		0//No
#define BI_RLE8 		1
#define BI_RLE4 		2
#define BI_BITFIELDS 	3

/* Exported functions ------------------------------------------------------- */
uint8_t BMP_ENCODE
(uint8_t *filename, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

#endif

/*************************************** END OF FILE ***************************************/
