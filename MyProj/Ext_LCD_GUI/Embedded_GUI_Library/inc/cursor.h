/**
  ******************************************************************************
  * @file    cursor.h
  * @author  MCD Application Team
  * @modify  '^_^'
  * @version V2.0.0
  * @date    8-October-2014
  * @brief   Application - cursor routines
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __cursor_H
#define __cursor_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes --------------------------------------------------------------------*/
#include "graphicObject.h"
   
/** @addtogroup Embedded_GUI_Library
  * @{
  */

/** @addtogroup Cursor
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/   
/** @defgroup Cursor_Exported_Types
  * @{
  */
typedef struct{
uint8_t *   CursorPointer;              /*!< Pointer to Cursor bitmap image */
uint16_t    X;                          /*!< Cursor x position */
uint16_t    Y;                          /*!< Cursor y position */
uint8_t	    PressState;                 /*!< Cursor press state */
uint16_t *  BehindCursor;               /*!< pointer to data buffer to save 
                                             Cursor behind image */
uint16_t    Color;                      /*!< Cursor color */
}Cursor_TypeDef;

typedef struct{
uint8_t	Width;
uint8_t	Height;
uint8_t	NumOfPix;
}CursorHeader_Typedef;

/**
  * @}
  */
    
/* Exported constants --------------------------------------------------------*/   
/** @defgroup Cursor_Exported_Constants
  * @{
  */
/** 
  * @brief  Cursor states 
  */  
#define CUR_UP	  	        0x01
#define CUR_DOWN	        0x02
#define CUR_LEFT	        0x03
#define CUR_RIGHT	        0x04
#define CUR_PRESS 	        0x10
#define CUR_RELEASE	        0x20

/** 
  * @brief  Cursor Move steps 
  */ 
#define CUR_STEP	            0x04

/** 
  * @brief  Cursor Drawing Modes 
  */ 
#define	CUR_DRAW_ALL 	        0x07
#define	CUR_DRAW_BEH	        0x01
#define	CUR_READ_BEH	        0x02
#define	CUR_READ_DRAW_CUR     0x06
#define	CUR_DRAW_CUR          0x08

#define	CUR_DRAW_BEH_MASK     0x01
#define	CUR_READ_BEH_MASK     0x02
#define CUR_DRAW_CUR_MASK     0x04

/** 
  * @brief  Cursor Size 
  */ 
#define Cursor_Width          ((uint8_t)8)
#define Cursor_Height         ((uint8_t)14)

   
/**
  * @}
  */

/** @defgroup Cursor_Exported_variables
  * @{
  */
extern Cursor_TypeDef* Cursor;

/**
  * @}
  */

/** @defgroup Cursor_Exported_Macros
  * @{
  */

/**
  * @}
  */
/* Exported functions ------------------------------------------------------- */ 
/** @defgroup Cursor_Exported_Functions
  * @{
  */
ErrorStatus CursorInit(uint8_t *PointerMark);  
void CursorDraw(uint16_t X, uint16_t Y, uint8_t DrawPhase);
void CursorShow(uint16_t Xpos, uint16_t Ypos);
void CursorStepMove(uint8_t CursorEvent);
void CursorPosition(uint16_t Xpos, uint16_t Ypos);
void CursorReadBtnstick(uint32_t joystick_move);
void TimeOutCalculate(void);

#ifdef __cplusplus
}
#endif

#endif /* __cursor_H */

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
