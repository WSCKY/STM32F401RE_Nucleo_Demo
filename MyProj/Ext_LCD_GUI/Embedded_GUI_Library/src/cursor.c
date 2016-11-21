/**
  ******************************************************************************
  * @file    cursor.c
  * @author  MCD Application Team
  * @modify  '^_^'
  * @version V2.0.0
  * @date    8-October-2014
  * @brief   Application - cursor routines
  ******************************************************************************
  */

/* Includes -----------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include "LcdHal.h"
#include "cursor.h"

/** @addtogroup Embedded_GUI_Library
  * @{
  */

/** @defgroup Cursor 
  * @brief Cursor main functions
  * @{
  */ 

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define TIMEOUT                         2000000
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Set to 0 to disable The Automatic Backlight Switch Off */
__IO uint8_t vu8_gPowerSaveOption = 1;
__IO uint8_t vu8_gSleepState = 0;
__IO uint32_t vu32_gTimeOutCount = 0;

/**
  * @brief  Cursor main structure
  */
Cursor_TypeDef* Cursor;

/**
  * @brief  Header and bitmap image of cursor's symbol
  */
static uint8_t CursorPointer[] =
{
  0x08,   /*Height of cursor symbol*/
  0x0B,   /*Width of cursor symbol*/
  0x35,   /*Count of pixels of cursor symbol*/
  0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xF8, 0xD8
};

/**
  * @brief  touch or cursor coordinate
  */
__IO uint32_t u32_TSXCoordinate;
__IO uint32_t u32_TSYCoordinate;



/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/** @defgroup Cursor_Private_Functions
  * @{
  */

/**
  * @brief  Shift cursor with defined step (CUR_STEP).
  * @param  CursorEvent: specifies direction of shift.
  *   This parameter can be one of the following values:
  *     @arg  CUR_DOWN: shift cursor down by step.
  *     @arg  CUR_UP: shift cursor up by step.
  *     @arg  CUR_RIGHT: shift cursor right by step.
  *     @arg  CUR_LEFT: shift cursor left by step.
  * @retval None
  */
void CursorStepMove(uint8_t CursorEvent)
{
  uint16_t tmpY, tmpX;

  /*Load current cursor position*/
  tmpY = Cursor->Y;
  tmpX = Cursor->X;

  /*Step cursor position with regard to desired direction.*/
  switch (CursorEvent)
  {
    case CUR_DOWN:
      tmpY += (LCD_Height - tmpY - CursorPointer[1] >= CUR_STEP) ? (CUR_STEP) : (LCD_Height - tmpY - CursorPointer[1]);
      break;
    case CUR_RIGHT:
      tmpX += (LCD_Width - tmpX - CursorPointer[0] >= CUR_STEP) ? (CUR_STEP) : (LCD_Width - tmpX - CursorPointer[0]);
      break;
    case CUR_LEFT:
      tmpX -= (tmpX >= CUR_STEP) ? (CUR_STEP) : (tmpX);
      break;
    case CUR_UP:
      tmpY -= (tmpY >= CUR_STEP) ? (CUR_STEP) : (tmpY);
      break;
    default:
      break;
  }

  /*Pass just calculated position to call what handle this change*/
  CursorPosition(tmpX, tmpY);
  GL_Delay(3);
}

/**
  * @brief  Handle cursor position and cursor events. Display cursor at new position and create message.
  * @param  Xpos: specifies cursor X position.
  * @param  Ypos: specifies cursor Y position.
  * @retval None
  */
void CursorPosition(uint16_t Xpos, uint16_t Ypos)
{
  CursorDraw(Xpos, Ypos, CUR_DRAW_BEH);
  CursorDraw(Xpos, Ypos, CUR_READ_DRAW_CUR);
}

/**
  * @brief  Read state of Button. Pressed key is announced to CursorStepMove
  *     to eventually calculate move of cursor.
  * @param  mode: IOEXP_MODE or POLLING_MODE
  * @retval None
  */
void CursorReadBtnstick(uint32_t joystick_move)
{
  switch (joystick_move)
  {
      /* "Up" key is pressed */
    case CUR_UP:
      u32_TSXCoordinate = Cursor->X;
      u32_TSYCoordinate = Cursor->Y;
      CursorStepMove(CUR_UP);
      break;

      /* "Down" key is pressed */
    case CUR_DOWN:
      u32_TSXCoordinate = Cursor->X;
      u32_TSYCoordinate = Cursor->Y;
      CursorStepMove(CUR_DOWN);
      break;

      /* "Left" key is pressed */
    case CUR_LEFT:
      u32_TSXCoordinate = Cursor->X;
      u32_TSYCoordinate = Cursor->Y;
      CursorStepMove(CUR_LEFT);
      break;

      /* "Right" key is pressed */
    case CUR_RIGHT:
      u32_TSXCoordinate = Cursor->X;
      u32_TSYCoordinate = Cursor->Y;
      CursorStepMove(CUR_RIGHT);
      break;

      /* "Sel" key is pressed */
    case CUR_PRESS:
      /*Load current cursor position*/
      u32_TSXCoordinate = Cursor->X;
      u32_TSYCoordinate = Cursor->Y;
      btn_done = 1; /* Run Touch Event Routine */
      break;

    case CUR_RELEASE:
      break;

    default:
      break;
  }
  if(joystick_move != CUR_RELEASE)
  {
    if(vu8_gSleepState == 1)
    {
      GL_BackLightSwitch(GL_ON);
      vu8_gSleepState = 0;
    }
	else
	  vu32_gTimeOutCount = 0;
  }
}

/**
  * @brief  Initiate the cursor including allocating memory for structure and behind array.
  * @param  PointerMark
  * @retval Status of initializing: Return ERROR if allocating failed.
  */
ErrorStatus CursorInit(uint8_t *PointerMark)
{
  /*Allocate memory for cursor structure*/
  Cursor = (Cursor_TypeDef *)malloc(sizeof(Cursor_TypeDef));

  if (Cursor == (Cursor_TypeDef*)GL_NULL)
  {
    return ERROR;
  }

  /*Allocate memory for cursor behind array. Actual behind of cursor is stored here*/
  Cursor->BehindCursor = (uint16_t *)malloc(CursorPointer[0] * CursorPointer[1] * sizeof(uint16_t));

  if (Cursor->BehindCursor == (uint16_t*)GL_NULL)
  {
    return ERROR;
  }

  /*Fill cursor structure by default values*/
  Cursor->CursorPointer =  ((PointerMark == (uint8_t*)GL_NULL) ? (CursorPointer) : (PointerMark));
  Cursor->Color = GL_Black;
  Cursor->PressState = CUR_RELEASE;
  /*Display cursor at specified position*/
  /* CursorDraw(Xpos, Ypos, CUR_READ_DRAW_CUR); */

  return SUCCESS;
}

/**
  * @brief  Show the cursor at the specified position.
  * @param  Xpos: specifies cursor initial X position.
  * @param  Ypos: specifies cursor initial Y position.
  * @retval None
  */
void CursorShow(uint16_t Xpos, uint16_t Ypos)
{
  /*Display cursor at specified position*/
  CursorDraw(Xpos, Ypos, CUR_READ_DRAW_CUR);
}

/**
  * @brief  Display a cursor at new position. In depence to DrawPhase parameter
  *         one of the following phases is performed:
  *  - Draw last cursors behind at last position - hide cursor.
  *  - Read new cursors behind at specified position.
  *  - Draw cursor at new position - display cursor.
  * @param  Xpos: specifies cursor X position.
  * @param  Ypos: specifies cursor Y position.
  * @param  DrawPhase: drawing phase, possible values are:
  *     @arg  CUR_DRAW_ALL: entire process.
  *     @arg  CUR_DRAW_BEH: draw behind at last position.
  *     @arg  CUR_READ_BEH: read behind at new position.
  *     @arg  CUR_READ_DRAW_CUR: read behind and draw cursor at new position.
  * @retval None
  */
void CursorDraw(uint16_t X, uint16_t Y, uint8_t DrawPhase)
{
  uint8_t tmpX, tmpY = 0x00;
  uint8_t Mask, Temp = 0x00;
  CursorHeader_Typedef *CursorHeader = (CursorHeader_Typedef *)Cursor->CursorPointer;
  uint8_t* CursorImagePointer;
  uint16_t* CursorBehindPointer;

  /*Draw last cursors behind at last position - hide cursor*/
  if (DrawPhase & CUR_DRAW_BEH_MASK)
  {
    /*Initiate pointers on Cursor bit map image and on array of cursors behind to draw*/
    CursorImagePointer = (uint8_t *)(Cursor->CursorPointer) + sizeof(CursorHeader_Typedef);
    CursorBehindPointer = Cursor->BehindCursor;

    /*Repeat for all lines of cursor symbol*/
    for (tmpY = 0x00 ; tmpY < CursorHeader->Height ; tmpY++)
    {
      Mask = 0x00;
      /*Repeat for all columns of cursor symbol*/
      for (tmpX = 0x00 ; tmpX < CursorHeader->Width; tmpX++)
      {
        if (Mask == 0)
        {
          /*Read next byte of bit map image and initiate mask*/
          Temp = *CursorImagePointer;
          CursorImagePointer++;
          Mask = 0x80;
        }

        /*Mask actual byte of cursor bitmap image*/
        if (Temp & Mask)
        {
          /*Draw pixel of cursor behind*/
          LCD_PutPixel(Cursor->X + tmpX, Cursor->Y + tmpY, *CursorBehindPointer++, SinglePixel);
        }
        /*Shift a mask right*/
        Mask >>= 1;
      }
    }
  }

  /*Read new cursors behind at specified position.*/
  if (DrawPhase & CUR_READ_BEH_MASK)
  {
    /*Initiate pointers on Cursor bit map image and on array of cursors behind to draw*/
    CursorImagePointer = (uint8_t *)(Cursor->CursorPointer) + sizeof(CursorHeader_Typedef);
    CursorBehindPointer = Cursor->BehindCursor;

    /*Repeat for all lines of cursor symbol*/
    for (tmpY = 0x00 ; tmpY < CursorHeader->Height ; tmpY++)
    {
      Mask = 0x00;
      /*Repeat for all columns of cursor symbol*/
      for (tmpX = 0x00 ; tmpX < CursorHeader->Width; tmpX++)
      {
        if (Mask == 0)
        {
          /*Read next byte of bitmap image and initiate mask*/
          Temp = *CursorImagePointer;
          CursorImagePointer++;
          Mask = 0x80;
        }
        /*Mask actual byte of cursor bitmap image*/
        if (Temp & Mask)
        {
          /*Read pixel which is going to be behind cursor*/
          *CursorBehindPointer = LCD_GetPixel(X + tmpX, Y + tmpY);
          CursorBehindPointer++;
        }
        /*Shift a mask right*/
        Mask >>= 1;
      }
    }
  }

  /*Draw cursor at new position - display cursor*/
  if (DrawPhase & CUR_DRAW_CUR_MASK || DrawPhase == CUR_DRAW_CUR)
  {
    CursorImagePointer = (uint8_t *)(Cursor->CursorPointer) + sizeof(CursorHeader_Typedef);

    /*Repeat for all lines of cursor symbol*/
    for (tmpY = 0x00 ; tmpY < CursorHeader->Height ; tmpY++)
    {
      Mask = 0x00;
      /*Repeat for all columns of cursor symbol*/
      for (tmpX = 0x00 ; tmpX < CursorHeader->Width; tmpX++)
      {
        if (Mask == 0)
        {
          /*Read next byte of bit map image and initiate mask*/
          Temp = *CursorImagePointer;
          CursorImagePointer++;
          Mask = 0x80;
        }
        /*Mask actual byte of cursor bitmap image*/
        if (Temp & Mask)
        {
          /*Draw pixel of cursor*/
          LCD_PutPixel(X + tmpX, Y + tmpY, Cursor->Color, SinglePixel);
        }
        /*Shift a mask right*/
        Mask >>= 1;
      }
    }
  }

  /*Save new position for future use as last position*/
  Cursor->X = X;
  Cursor->Y = Y;
}

/**
  * @brief  Switches the backlightOFF when power save mode is
  *         enabled and no event is detected for long time.
  * @param  None
  * @retval None
  */
void TimeOutCalculate(void)
{
  if(vu8_gSleepState == 0)
  {
    if(vu8_gPowerSaveOption == 1)
    {
      vu32_gTimeOutCount++;
      if (vu32_gTimeOutCount > TIMEOUT)
      {
        GL_BackLightSwitch(GL_OFF);
        vu8_gSleepState = 1;
        vu32_gTimeOutCount = 0;
      }
    }
  }
}

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
