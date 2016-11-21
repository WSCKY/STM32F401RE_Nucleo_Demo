/**
  ******************************************************************************
  * @file    gl_fonts.h
  * @author  MCD Application Team
  * @modify  '^_^'
  * @version V2.0.1
  * @date    8-October-2014
  * @brief   Header for gl_fonts.c
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GL_FONTS_H
#define __GL_FONTS_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup Embedded_GUI_Library
  * @{
  */
   
/** @addtogroup GL_FONTS
  * @{
  */ 
  
/* Exported types ------------------------------------------------------------*/
/** @defgroup GL_FONTS_Exported_Types
  * @{
  */ 
typedef struct tFont
{
  const uint16_t *table;
  uint16_t Width;
  uint16_t Height;
}gl_sFONT;

/** 
  * @brief  exported fonts  
  */
extern gl_sFONT GL_Font16x24;
extern gl_sFONT GL_Font8x16;

/**
  * @}
  */
  
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/** @defgroup GL_FONTS_Exported_Macros
  * @{
  */
#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height)) 
/**
  * @}
  */
  
/* Exported functions ------------------------------------------------------- */ 

#ifdef __cplusplus
}
#endif

#endif /* __GL_FONTS_H */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
