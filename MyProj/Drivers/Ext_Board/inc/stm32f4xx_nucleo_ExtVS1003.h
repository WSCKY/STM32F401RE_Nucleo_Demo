/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtVS1003.h
  * @author  '^_^'
  * @version V0.0.0
  * @date    9-October-2014
  * @brief   This file contains the common defines and functions prototypes for
  *          the stm32f4xx_nucleo_ExtVS1003.c driver.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXTVS1003_H
#define __STM32F4XX_NUCLEO_EXTVS1003_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/

typedef enum
{
	REQ_MODE_GPIO = 0,
	REQ_MODE_EXIT = 1
}REQMode_TypeDef;

typedef enum
{
	STEREO_OFF = 0,
	STEREO_ON = 1
	
}STEREO_SwitchDef;

typedef struct
{
	uint8_t Left_Chan_VOL;
	uint8_t Right_Chan_VOL;
	uint8_t Bass_Freq;
	uint8_t Bass;
	uint8_t Treble_Freq;
	uint8_t Treble;
	uint16_t Sample_Rate;
	STEREO_SwitchDef Stereo;
}VS1003_InitTypeDef;

//#define USE_DEFAULT_ERROR_CALLBACK
/* Exported variables --------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Definition for VS1003_SPI clock resources */
#define VS1003_SPI								 SPI3
#define VS1003_SPI_CLK_ENABLE()                __SPI3_CLK_ENABLE()
#define VS1003_SPI_SCK_GPIO_CLK_ENABLE()       __GPIOC_CLK_ENABLE()
#define VS1003_SPI_MISO_MOSI_GPIO_CLK_ENABLE() __GPIOC_CLK_ENABLE()

#define VS1003_SPI_FORCE_RESET()               __SPI3_FORCE_RESET()
#define VS1003_SPI_RELEASE_RESET()             __SPI3_RELEASE_RESET()

/* Definition for VS1003_SPI Pins */
#define VS1003_SPI_SCK_PIN                       GPIO_PIN_10
#define VS1003_SPI_SCK_GPIO_PORT                 GPIOC
#define VS1003_SPI_SCK_AF                        GPIO_AF6_SPI3
#define VS1003_SPI_MISO_PIN                      GPIO_PIN_11
#define VS1003_SPI_MISO_GPIO_PORT                GPIOC
#define VS1003_SPI_MISO_AF                       GPIO_AF6_SPI3
#define VS1003_SPI_MOSI_PIN                      GPIO_PIN_12
#define VS1003_SPI_MOSI_GPIO_PORT                GPIOC
#define VS1003_SPI_MOSI_AF                       GPIO_AF6_SPI3

/* Definition for VS1003's other pins */
#define VS1003_XCS_PIN                           GPIO_PIN_12
#define VS1003_XCS_GPIO_PORT                     GPIOA
#define VS1003_XCS_GPIO_CLK_ENABLE()           __GPIOA_CLK_ENABLE()

#define VS1003_XDCS_PIN                          GPIO_PIN_11
#define VS1003_XDCS_GPIO_PORT                    GPIOA
#define VS1003_XDCS_GPIO_CLK_ENABLE()          __GPIOA_CLK_ENABLE()

#define VS1003_RESET_PIN                         GPIO_PIN_0
#define VS1003_RESET_GPIO_PORT                   GPIOC
#define VS1003_RESET_GPIO_CLK_ENABLE()         __GPIOC_CLK_ENABLE()

#define VS1003_DREQ_PIN                          GPIO_PIN_1
#define VS1003_DREQ_GPIO_PORT                    GPIOC
#define VS1003_DREQ_GPIO_CLK_ENABLE()          __GPIOC_CLK_ENABLE()
#define VS1003_DREQ_EXIT_IRQn                    EXTI1_IRQn

/* Set or reset pins */
#define VS1003_XCS_LOW()                         VS1003_XCS_GPIO_PORT->BSRRH = VS1003_XCS_PIN
#define VS1003_XCS_HIGH()                        VS1003_XCS_GPIO_PORT->BSRRL = VS1003_XCS_PIN

#define VS1003_XDCS_LOW()                        VS1003_XDCS_GPIO_PORT->BSRRH = VS1003_XDCS_PIN
#define VS1003_XDCS_HIGH()                       VS1003_XDCS_GPIO_PORT->BSRRL = VS1003_XDCS_PIN

#define VS1003_RESET_LOW()                       VS1003_RESET_GPIO_PORT->BSRRH = VS1003_RESET_PIN
#define VS1003_RESET_HIGH()                      VS1003_RESET_GPIO_PORT->BSRRL = VS1003_RESET_PIN

#define VS1003_DREQ_STATUS()                     (VS1003_DREQ_GPIO_PORT->IDR & VS1003_DREQ_PIN)

/* Exported macro ------------------------------------------------------------*/
#define VS1003_TIMEOUT                           ((uint32_t)0x5000)

#define VS1003_WRITE_COMMAND                     0x02
#define VS1003_READ_COMMAND                      0x03

/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/
/*
+------------------------------------------------------------------------------+
|                          SCI registers, prefix SCI_                          |
+-----+----+-----+----------+------------+-------------------------------------+
| Reg |Type|Reset|   Time   |Abbrev[bits]|             Description             |
+-----+----+-----+----------+------------+-------------------------------------+
| 0x0 | rw |0x800|  70CLKI  |    MODE    |Mode control                         |
+-----+----+-----+----------+------------+-------------------------------------+
| 0x1 | rw |0x3C |  40CLKI  |   STATUS   |Status of VS1003                     |
+-----+----+-----+----------+------------+-------------------------------------+
| 0x2 | rw |  0  | 2100CLKI |    BASS    |Built-in bass/treble enhancer        |
+-----+----+-----+----------+------------+-------------------------------------+
| 0x3 | rw |  0  |11000XTALI|   CLOCKF   |Clock freq + multiplier              |
+-----+----+-----+----------+------------+-------------------------------------+
| 0x4 | rw |  0  |  40CLKI  |DECODE_TIME |Decode time in seconds               |
+-----+----+-----+----------+------------+-------------------------------------+
| 0x5 | rw |  0  | 3200CLKI |   AUDATA   |Misc. audio data                     |
+-----+----+-----+----------+------------+-------------------------------------+
| 0x6 | rw |  0  |  80CLKI  |    WRAM    |RAM write/read                       |
+-----+----+-----+----------+------------+-------------------------------------+
| 0x7 | rw |  0  |  80CLKI  |  WRAMADDR  |Base address for RAM write/read      |
+-----+----+-----+----------+------------+-------------------------------------+
| 0x8 | ro |  0  |    -     |   HDAT0    |Stream header data 0                 |
+-----+----+-----+----------+------------+-------------------------------------+
| 0x9 | ro |  0  |    -     |   HDAT1    |Stream header data 1                 |
+-----+----+-----+----------+------------+-------------------------------------+
| 0xA | rw |  0  | 3200CLKI |   AIADDR   |Start address of application         |
+-----+----+-----+----------+------------+-------------------------------------+
| 0xB | rw |  0  | 2100CLKI |    VOL     |Volume control                       |
+-----+----+-----+----------+------------+-------------------------------------+
| 0xC | rw |  0  |  50CLKI  |  AICTRL0   |Application control register 0       |
+-----+----+-----+----------+------------+-------------------------------------+
| 0xD | rw |  0  |  50CLKI  |  AICTRL1   |Application control register 1       |
+-----+----+-----+----------+------------+-------------------------------------+
| 0xE | rw |  0  |  50CLKI  |  AICTRL2   |Application control register 2       |
+-----+----+-----+----------+------------+-------------------------------------+
| 0xF | rw |  0  |  50CLKI  |  AICTRL3   |Application control register 3       |
+-----+----+-----+----------+------------+-------------------------------------+
*/
#define SCI_MODE								0x00
#define SCI_STATUS								0x01
#define SCI_BASS								0x02
#define SCI_CLOCKF								0x03
#define SCI_DECODE_TIME							0x04
#define SCI_AUDATA								0x05
#define SCI_WRAM								0x06
#define SCI_WRAMADDR							0x07
#define SCI_HDAT0								0x08
#define SCI_HDAT1								0x09
#define SCI_AIADDR								0x0A
#define SCI_VOL									0x0B
#define SCI_AICTRL0								0x0C
#define SCI_AICTRL1								0x0D
#define SCI_AICTRL2								0x0E
#define SCI_AICTRL3								0x0F

/*
SCI_MODE is used to control the operation of VS1003 and defaults to 0x0800 (SM_SDINEW set).
+---+-------------+----------------------------------+-----------------------------------------------+
|Bit|    Name     |             Function             |Value&Description                              |
+---+-------------+----------------------------------+-----------------------------------------------+
| 0 |   SM_DIFF   | Differential                     |0,normal in-phase audio;1,left channel inverted|
+---+-------------+----------------------------------+-----------------------------------------------+
| 1 |SM_SETTOZERO | Set to zero                      |0,right;1,wrong                                |
+---+-------------+----------------------------------+-----------------------------------------------+
| 2 |  SM_RESET   | Soft reset                       |0,no reset;1,reset                             |
+---+-------------+----------------------------------+-----------------------------------------------+
| 3 | SM_OUTOFWAV | Jump out of WAV decoding         |0,no;1,yes                                     |
+---+-------------+----------------------------------+-----------------------------------------------+
| 4 |  SM_PDOWN   | Powerdown                        |0,power on;1,power down                        |
+---+-------------+----------------------------------+-----------------------------------------------+
| 5 |  SM_TESTS   | Allow SDI tests                  |0,not allowed;1,allowed                        |
+---+-------------+----------------------------------+-----------------------------------------------+
| 6 |  SM_STREAM  | Stream mode                      |0,no;1,yes                                     |
+---+-------------+----------------------------------+-----------------------------------------------+
| 7 |SM_SETTOZERO2| Set to zero                      |0,right;1,wrong                                |
+---+-------------+----------------------------------+-----------------------------------------------+
| 8 |   SM_DACT   | DCLK active edge                 |0,rising;1,falling                             |
+---+-------------+----------------------------------+-----------------------------------------------+
| 9 |  SM_SDIORD  | SDI bit order                    |0,MSb first;1,MSb last                         |
+---+-------------+----------------------------------+-----------------------------------------------+
|10 | SM_SDISHARE | Share SPI chip select            |0,no;1,yes                                     |
+---+-------------+----------------------------------+-----------------------------------------------+
|11 |  SM_SDINEW  | VS1002 native SPI modes          |0,no;1,yes                                     |
+---+-------------+----------------------------------+-----------------------------------------------+
|12 |  SM_ADPCM   | ADPCM recording active           |0,no;1,yes                                     |
+---+-------------+----------------------------------+-----------------------------------------------+
|13 | SM_ADPCM_HP | ADPCM high-pass filter active    |0,no;1,yes                                     |
+---+-------------+----------------------------------+-----------------------------------------------+
|14 | SM_LINE_IN  | ADPCM recording selector         |0,microphone;1,line in                         |
+---+-------------+----------------------------------+-----------------------------------------------+
*/
#define DIFF_NORMAL								(0x0000)
#define DIFF_INVERT								(0x0001)

#define SOFT_SET								(0x0000<<2)
#define SOFT_RESET								(0x0001<<2)

#define OUTOFWAV_NO								(0x0000<<3)
#define OUTOFWAV_YES							(0x0001<<3)

#define POWER_ON								(0x0000<<4)
#define POWER_DOWN								(0x0001<<4)

#define SDI_TEST_NotAllow						(0x0000<<5)
#define SDI_TEST_Allow							(0x0001<<5)

#define DACK_Rising								(0x0000<<8)
#define DACK_Falling							(0x0001<<8)

#define SDI_SHARE_NO							(0x0000<<10)
#define SDI_SHARE_YES							(0x0001<<10)

#define SDINEW_NO								(0x0000<<11)
#define SDINEW_YES								(0x0001<<11)
/* ADPCM recording */
#define ADPCM_RECORD_NO							(0x0000<<12)
#define ADPCM_RECORD_YES						(0x0001<<12)

#define ADPCM_Filter_NO							(0x0000<<13)
#define ADPCM_Filter_YES						(0x0001<<13)

#define ADPCM_Microphone						(0x0000<<14)
#define ADPCM_LineIn							(0x0001<<14)

/*
SCI STATUS contains information on the current status of VS1003 and lets the user shutdown
the chip without audio glitches.
+----------+----+--------------------------------------------------------------+
|   Name   |Bits|Description                                                   |
+----------+----+--------------------------------------------------------------+
|  SS_VER  |6:4 |Version                                                       |
+----------+----+--------------------------------------------------------------+
|SS_APDOWN2| 3  |Analog driver powerdown                                       |
+----------+----+--------------------------------------------------------------+
|SS_APDOWN1| 2  |Analog internal powerdown                                     |
+----------+----+--------------------------------------------------------------+
| SS_AVOL  |1:0 |Analog volume control                                         |
+----------+----+--------------------------------------------------------------+
*/

/*
The Bass Enhancer VSBE is a powerful bass boosting DSP algorithm, which tries
to take the most out of the users earphones without causing clipping.
+------------+--------+--------------------------------------------------------+
|    Name    |  Bits  |Description                                             |
+------------+--------+--------------------------------------------------------+
|ST_AMPLITUDE| 15:12  |Treble Control in 1.5 dB steps (-8..7, 0 = off)         |
+------------+--------+--------------------------------------------------------+
|ST_FREQLIMIT|  11:8  |Lower limit frequency in 1000 Hz steps (0..15)          |
+------------+--------+--------------------------------------------------------+
|SB_AMPLITUDE|  7:4   |Bass Enhancement in 1 dB steps (0..15, 0 = off)         |
+------------+--------+--------------------------------------------------------+
|SB_FREQLIMIT|  3:0   |Lower limit frequency in 10 Hz steps (2..15)            |
+------------+--------+--------------------------------------------------------+
*/

/*
When decoding correct data, current decoded time is shown in this register in full seconds.
The user may change the value of this register. In that case the new value
should be written twice.
SCI_DECODE_TIME is reset at every software reset and also when WAV (PCM or IMA ADPCM),
WMA, or MIDI decoding starts or ends.
*/

/*
When decoding correct data, the current sample rate and number of channels can be found in bits 15:1
and 0 of SCI_AUDATA, respectively. Bits 15:1 contain the sample rate divided by two, and bit 0 is 0 for
mono data and 1 for stereo. Writing to SCI_AUDATA will change the sample rate directly.
Note: due to a bug, an odd sample rate reverses the operation of the stereo bit in VS1003b.
*/

#define STEREO_YES								0x0001
#define STEREO_NO								0x0000

/*
SCI_VOL is a volume control for the player hardware. For each channel,
a value in the range of 0..254 may be defined to set its attenuation
from the maximum volume level (in 0.5 dB steps). The left channel value
is then multiplied by 256 and the values are added. Thus, maximum volume
is 0 and total silence is 0xFEFE.
Note: Setting SCI VOL to 0xFFFF will activate analog powerdown mode.
*/
/* Exported functions --------------------------------------------------------*/
void VS1003_Init(VS1003_InitTypeDef *VS1003_InitStruct, REQMode_TypeDef REQ_Mode);
void VS1003_Set_Volume(uint8_t LeftVol, uint8_t RightVol);
void VS1003_Set_Bass(uint8_t Bass_Freq, uint8_t Bass, uint8_t Treble_Freq, uint8_t Treble);
void VS1003_Set_SamRat_Stereo(uint16_t Sample_Rate, STEREO_SwitchDef Stereo);
uint8_t VS1003_Send_MusicData(uint8_t *DataBuffer);
void VS1003_Play_Prepare(void);
void VS1003_Reset_DecodeTime(void);
uint16_t VS1003_Get_DecodeTime(void);
uint8_t VS1003_Sine_Test(uint8_t TestValue, uint32_t TestTime);
uint8_t VS1003_RAM_Test(void);
uint8_t VS1003_HW_RESET(void);
uint8_t VS1003_SW_RESET(void);
uint8_t SPI_WriteDeviceRegister(uint8_t RegisterAddr, uint16_t RegisterValue);
void SPI_WriteDeviceData(uint8_t Data);
uint8_t SPI_ReadDeviceRegister(uint8_t RegisterAddr, uint16_t *RegisterValue);
uint16_t SPI_ReadDeviceData(uint16_t RamAddr);
uint8_t VS1003_ERROR_UserCallback(void);

#ifdef __cplusplus
 }
#endif

#endif

/*************************************** END OF FILE ***************************************/
