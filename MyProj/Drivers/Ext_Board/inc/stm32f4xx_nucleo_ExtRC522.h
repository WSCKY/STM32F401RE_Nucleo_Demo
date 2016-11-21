/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtRC522.h
  * @author  '^_^'
  * @version V0.0.0
  * @date    9-October-2014
  * @brief   This file contains all the functions prototypes for the
  *            stm32f4xx_nucleo_ExtRC522.c firmware driver.
  ******************************************************************************  
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4XX_NUCLEO_EXTRC522_H
#define __STM32F4XX_NUCLEO_EXTRC522_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

//#define USE_IRQ_PIN
#define USE_DEFAULT_TIMEOUT_CALLBACK
#define MFRC522_FLAG_TIMEOUT			((uint32_t)0x1000)
#define COMMUNI_WAIT_TIME				((uint16_t)10000)
#define CAL_CRC_WAIT_TIME				((uint16_t)1000)
#define MAX_LEN							18
	 
/* Exported macro ------------------------------------------------------------*/

/**
  * @brief MFRC522 Struct
  */
typedef struct
{
	uint8_t TAutoMode;
	uint16_t TPrescaler;
	uint16_t TReload;
	uint8_t ASKRequi;
	uint8_t CRCPreset;
}MFRC522_InitTypeDef;

/**
  * @brief  MFRC522 SPI Interface pins
  */
#define MFRC522_SPI								SPI3
#define MFRC522_SPI_CLK_ENABLE()              __SPI3_CLK_ENABLE()

#define MFRC522_SPI_SCK_PIN						GPIO_PIN_10
#define MFRC522_SPI_SCK_GPIO_PORT				GPIOC
#define MFRC522_SPI_SCK_AF						GPIO_AF6_SPI3
#define MFRC522_SPI_SCK_GPIO_CLK_ENABLE()     __GPIOC_CLK_ENABLE()
#define MFRC522_SPI_SCK_GPIO_CLK_DISABLE()    __GPIOC_CLK_DISABLE()

#define MFRC522_SPI_MISO_PIN                    GPIO_PIN_11
#define MFRC522_SPI_MISO_GPIO_PORT				GPIOC
#define MFRC522_SPI_MISO_AF						GPIO_AF6_SPI3
#define MFRC522_SPI_MISO_GPIO_CLK_ENABLE()    __GPIOC_CLK_ENABLE()
#define MFRC522_SPI_MISO_GPIO_CLK_DISABLE()   __GPIOC_CLK_DISABLE()

#define MFRC522_SPI_MOSI_PIN                    GPIO_PIN_12
#define MFRC522_SPI_MOSI_GPIO_PORT				GPIOC
#define MFRC522_SPI_MOSI_AF						GPIO_AF6_SPI3
#define MFRC522_SPI_MOSI_GPIO_CLK_ENABLE()    __GPIOC_CLK_ENABLE()
#define MFRC522_SPI_MOSI_GPIO_CLK_DISABLE()   __GPIOC_CLK_DISABLE()

#define MFRC522_SPI_RST_PIN                     GPIO_PIN_4
#define MFRC522_SPI_RST_GPIO_PORT               GPIOA
#define MFRC522_SPI_RST_GPIO_CLK_ENABLE()     __GPIOA_CLK_ENABLE()
#define MFRC522_SPI_RST_GPIO_CLK_DISABLE()    __GPIOA_CLK_DISABLE()

#define MFRC522_SPI_NSS_PIN                     GPIO_PIN_0
#define MFRC522_SPI_NSS_GPIO_PORT               GPIOB
#define MFRC522_SPI_NSS_GPIO_CLK_ENABLE()     __GPIOB_CLK_ENABLE()
#define MFRC522_SPI_NSS_GPIO_CLK_DISABLE()    __GPIOB_CLK_DISABLE()

#define MFRC522_RST_HIGH()		HAL_GPIO_WritePin(MFRC522_SPI_RST_GPIO_PORT, MFRC522_SPI_RST_PIN, GPIO_PIN_SET);
#define MFRC522_RST_LOW()		HAL_GPIO_WritePin(MFRC522_SPI_RST_GPIO_PORT, MFRC522_SPI_RST_PIN, GPIO_PIN_RESET);

#define MFRC522_NSS_HIGH()		HAL_GPIO_WritePin(MFRC522_SPI_NSS_GPIO_PORT, MFRC522_SPI_NSS_PIN, GPIO_PIN_SET);
#define MFRC522_NSS_LOW()		HAL_GPIO_WritePin(MFRC522_SPI_NSS_GPIO_PORT, MFRC522_SPI_NSS_PIN, GPIO_PIN_RESET);

#ifdef USE_IRQ_PIN

#define MFRC522_SPI_IRQ_PIN                     GPIO_PIN_5
#define MFRC522_SPI_IRQ_GPIO_PORT               GPIOA
#define MFRC522_SPI_IRQ_GPIO_CLK_ENABLE()     __GPIOA_CLK_ENABLE()
#define MFRC522_SPI_IRQ_GPIO_CLK_DISABLE()    __GPIOA_CLK_DISBALE()

#endif

/******************************** Error Code *********************************/
#define MI_OK								0x00
#define MI_NOTAGREE							0x01
#define MI_ERROR							0x02

/**************************** Mifare Card Command ****************************/
#define PICC_REQIDL							0x26
#define PICC_REQALL							0x52
#define PICC_ANTICOLL1						0x93
#define PICC_ANTICOLL2						0x95
#define PICC_AUTHENT1A						0x60
#define PICC_AUTHENT1B						0x61
#define PICC_READ							0x30
#define PICC_WRITE							0xA0
#define PICC_DECREMENT						0xC0
#define PICC_INCREMENT						0xC1
#define PICC_RESTORE						0xC2
#define PICC_TRANSFER						0xB0
#define PICC_HALT							0x50

/*************************** SPI(MFRC522) commands ***************************/
/**
  * @brief
  * No action; cancels current command execution.
  */
#define IDLE								0x00
/**
  * @brief
  * Stores 25 byte into the internal buffer.
  */
#define MEM									0x01
/**
  * @brief
  * Generates a 10 byte random ID number.
  */
#define GenerateRandomID					0x02
/**
  * @brief
  * Activates the CRC co-processor or performs a selftest.
  */
#define CalcCRC								0x03
/**
  * @brief
  * Transmits data from the FIFO buffer.
  */
#define TRANSMIT							0x04
/**
  * @brief
  * No command change. this command can be used to modify different bits in the command
  * register without touching the command. E.g. Power-down.
  */
#define NoCmdChange							0x07
/**
  * @brief
  * Activates the receiver circuitry.
  */
#define RECEIVER							0x08
/**
  * @brief
  * Transmits data from FIFO buffer to the antenna and activates automatically the receiver
  * after transmission.
  */
#define Transceive							0x0c
/**
  * @brief
  * Reserved for further use. ^Useless^
  */
#define RESERVED							0x0d
/**
  * @brief
  * Performs the MIFARE(R) standard authentication as a reader.
  */
#define MFAuthent							0x0e
/**
  * @brief
  * Resets the MFRC522.
  */
#define SoftReset							0x0f
/* MFRC522 Command Description (In the 73 page) */

/*******************************************************************************/
/********************** SPI(MFRC522) registers(addresses) **********************/
/*******************************************************************************/

/*------------------------ Page0 : Command and Status -------------------------*/
/* Reserved for future use ----------------------------------------------------*/
#define RFU00								0x00
/* Starts and stops command execution -----------------------------------------*/
#define CommandReg							0x01
/*
								Description of CommandReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|7 to 6	|    -     |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
|	5	|  RcvOff  |Set to logic 1, the analog part of the receiver is switched off.		|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, Soft Power-down mode is entered.						|
|		|		   |Set to logic 0, the MFRC522 starts the wake up procedure. During this	|
|	4	|PowerDown |procedure this bit still shows a logic 1. A logic 0 indicates that the	|
|		|		   |MFRC522 is ready for operations; see Section 16.2 Soft "Power-down".	|
|		|		   |Remark: The bit PowerDown cannot be set, when the command SoftReset		|
|		|		   |		has been activated.												|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Activates a command according to the Command Code. Reading this	register|
|3 to 0	| Command  |shows which command is actually executed (see Section 18.3 "MFRC522		|
|		|		   |Commands Overview").													|
+-------+----------+------------------------------------------------------------------------+
*/
/* Control bits to enable and disable the passing of interrupt requests -------*/
#define CommIEnReg							0x02
/*
								Description of CommIEnReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, the signal on pin IRQ is inverted with respect to bit	|
|		|		   |IRq in the register Status1Reg.											|
|	7	|  IRqInv  |Set to logic 0, the signal on pin IRQ is equal to bit IRq. In			|
|		|		   |combination with bit IRqPushPull in register DivIEnReg, the default		|
|		|		   |value of logic 1 ensures, that the output level on pin IRQ is tristate.	|
+-------+----------+------------------------------------------------------------------------+
|	6	|  TxIEn   |Allows the transmitter interrupt request (indicated by bit TxIRq) to be	|
|		|		   |propagated to pin IRQ.													|
+-------+----------+------------------------------------------------------------------------+
|	5	|  RxIEn   |Allows the receiver interrupt request (indicated by bit RxIRq) to be	|
|		|		   |propagated to pin IRQ.													|
+-------+----------+------------------------------------------------------------------------+
|	4	| IdleIEn  |Allows the idle interrupt request (indicated by bit IdleIRq) to be		|
|		|		   |propagated to pin IRQ.													|
+-------+----------+------------------------------------------------------------------------+
|	3	|HiAlertIEn|Allows the high alert interrupt request (indicated by bit HiAlertIRq)	|
|		|		   |to be propagated to pin IRQ.											|
+-------+----------+------------------------------------------------------------------------+
|	2	|LoAlertIEn|Allows the low alert interrupt request (indicated by bit LoAlertIRq)	|
|		|		   |to be propagated to pin IRQ.											|
+-------+----------+------------------------------------------------------------------------+
|	1	|  ErrIEn  |Allows the error interrupt request (indicated by bit ErrIRq) to be		|
|		|		   |propagated to pin IRQ.													|
+-------+----------+------------------------------------------------------------------------+
|	0	| TimerIEn |Allows the timer interrupt request (indicated by bit TimerIRq) to be	|
|		|		   |propagated to pin IRQ.													|
+-------+----------+------------------------------------------------------------------------+
*/
/* Control bits to enable and disable the passing of interrupt requests -------*/
#define DivIEnReg							0x03
/*
								 Description of DivIEnReg bits
+-------+-----------+------------------------------------------------------------------------+
|  Bit  |  Symbol   |								Description								 |
+-------+-----------+------------------------------------------------------------------------+
|	7	|IRQPushPull|Set to logic 1, the pin IRQ works as standard CMOS output pad.			 |
|		|			|Set to logic 0, the pin IRQ works as open drain output pad.			 |
+-------+-----------+------------------------------------------------------------------------+
|6 to 5	|	  -  	|Reserved for future use.												 |
+-------+-----------+------------------------------------------------------------------------+
|	4	|MfinActIEn	|Allows the MFIN active interrupt request to be propagated to pin IRQ.	 |
+-------+-----------+------------------------------------------------------------------------+
|	3	|	  -  	|Reserved for future use.												 |
+-------+-----------+------------------------------------------------------------------------+
|	2	|	CRCIEn	|Allows the CRC interrupt request (indicated by bit CRCIRq) to be		 |
|		|			|propagated to pin IRQ.													 |
+-------+-----------+------------------------------------------------------------------------+
|1 to 0	|	  -  	|Reserved for future use.												 |
+-------+-----------+------------------------------------------------------------------------+
*/
/* Contains Interrupt Request bits --------------------------------------------*/
#define CommIRqReg							0x04
/*
								Description of CommIRqReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, Set1 defines that the marked bits in the register		|
|	7	|	Set1   |CommIRqReg are set.														|
|		|		   |Set to logic 0, Set1 defines, that the marked bits in the register		|
|		|		   |CommIRqReg are cleared.													|
+-------+----------+------------------------------------------------------------------------+
|	6	|	TxIRq  |Set to logic 1 immediately after the last bit of the transmitted data	|
|		|		   |was sent out.															|
+-------+----------+------------------------------------------------------------------------+
|		|	RxIRq  |Set to logic 1 when the receiver detects the end of a valid data stream.|
|	5	|		   |If the bit RxNoErr in register RxModeReg is set to logic 1, Bit RxIRq	|
|		|		   |is only set to logic 1 when data bytes are available in the FIFO.		|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, when a command terminates by itself e.g. when the		|
|		|		   |CommandReg changes its value from any command to the Idle Command.		|
|	4	|  IdleIRq |If an unknown command is started, the CommandReg changes its content to	|
|		|		   |the idle state and the bit IdleIRq is set. Starting the Idle Command by	|
|		|		   |the u-Controller does not set bit IdleIRq.								|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, when bit HiAlert in register Status1Reg is set. In		|
|	3	|HiAlertIRq|opposition to HiAlert, HiAlertIRq stores this event and can only be		|
|		|		   |reset as indicated by bit Set1.											|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, when bit LoAlert in register Status1Reg is set. In		|
|	2	|LoAlertIRq|opposition to LoAlert, LoAlertIRq stores this event and can only be		|
|		|		   |reset as indicated by bit Set1.											|
+-------+----------+------------------------------------------------------------------------+
|	1	|  ErrIRq  |Set to logic 1 if any error bit in the ErrorReg Register is set.		|
+-------+----------+------------------------------------------------------------------------+
|	0	| TimerIRq |Set to logic 1 when the timer decrements the TimerValue Register to		|
|		|		   |zero.																	|
+-------+----------+------------------------------------------------------------------------+
*/
/* Contains Interrupt Request bits --------------------------------------------*/
#define DivIRqReg							0x05
/*
								Description of DivIRqReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, Set2 defines that the marked bits in the register		|
|	7	|	Set2   |DivIRqReg are set.														|
|		|		   |Set to logic 0, Set2 defines, that the marked bits in the register		|
|		|		   |DivIRqReg are cleared.													|
+-------+----------+------------------------------------------------------------------------+
|6 to 5 |		   |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
|	4	|MfinActIRq|Set to logic 1, when MFIN is active. This interrupt is set when either a|
|		|		   |rising or falling signal edge is detected.								|
+-------+----------+------------------------------------------------------------------------+
|	3	|	 -     |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
|	2	|  CRCIRq  |Set to logic 1, when the CRC command is active and all data are			|
|		|		   |processed.																|
+-------+----------+------------------------------------------------------------------------+
|1 to 0	|	 -	   |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
*/
/* Error bits showing the error status of the last command executed -----------*/
#define ErrorReg							0x06
/* Contains status bits for communication -------------------------------------*/
#define Status1Reg							0x07
/*
								Description of Status1Reg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|	7	|	 -	   |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, if the CRC result is zero. For data transmission and	|
|		|		   |reception the bit CRCOk is undefined (use CRCErr in register			|
|	6	|  CRCOk   |ErrorReg). CRCOk indicates the status of the CRC co-processor,			|
|		|		   |during calculation the value changes to logic 0, when the calculation is|
|		|		   |done correctly, the value changes to logic 1.							|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, when the CRC calculation has finished. This bit is only	|
|	5	| CRCReady |valid for the CRC co-processor calculation using the command			|
|		|		   |CalcCRC.																|
+-------+----------+------------------------------------------------------------------------+
|		|		   |This bit shows, if any interrupt source requests attention (with respect|
|	4	|	IRq    |to the setting of the interrupt enable bits, see register  CommIEnReg	|
|		|		   |and DivIEnReg).															|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, if the MFRC522's timer unit is running, e.g. the timer	|
|		|		   |will decrement the TCounterValReg with the next timer clock.			|
|	3	| TRunning |Remark: In the gated mode the bit TRunning is set to logic 1, when		|
|		|		   |		the timer is enabled by the register bits. This bit is not		|
|		|		   |		influenced by the gated signal.									|
+-------+----------+------------------------------------------------------------------------+
|	2	|	 -	   |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, when the number of bytes stored in the FIFO buffer		|
|		|		   |fulfils the following equation:											|
|	1	| HiAlert  |HiAlert = (64 - FIFOLength  ) ¡Ü WaterLevel								|
|		|		   |Example:																|
|		|		   |	FIFOLength = 60, WaterLevel = 4 ¡ú HiAlert = 1						|
|		|		   |	FIFOLength = 59, WaterLevel = 4 ¡ú HiAlert = 0						|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, when the number of bytes stored in the FIFO buffer		|
|		|		   |fulfils the following equation:											|
|	0	| LoAlert  |LoAlert = FIFOLength ¡Ü WaterLevel										|
|		|		   |Example:																|
|		|		   |	FIFOLength = 4, WaterLevel = 4 ¡ú LoAlert = 1						|
|		|		   |	FIFOLength = 5, WaterLevel = 4 ¡ú LoAlert = 0						|
+-------+----------+------------------------------------------------------------------------+
*/
/* Contains status bits of the receiver and transmitter -----------------------*/
#define Status2Reg							0x08
/*
								Description of Status2Reg bits
+-------+-------------+---------------------------------------------------------------------+
|  Bit  |    Symbol   |								Description								|
+-------+-------------+---------------------------------------------------------------------+
|	7	|TempSensClear|Set to logic 1, this bit clears the temperature error, if the		|
|		|			  |temperature is below the alarm limit of 125 'C.						|
+-------+-------------+---------------------------------------------------------------------+
|		|			  |I2C input filter settings.											|
|	6	| I2CForceHS  |Set to logic 1, the I2C input filter is set to the High-speed mode	|
|		|			  |independent of the I2C protocol.										|
|		|			  |Set to logic 0, the I2C input filter is set to the used I2C protocol.|
+-------+-------------+---------------------------------------------------------------------+
|5 to 4	|	   -	  |Reserved for future use.												|
+-------+-------------+---------------------------------------------------------------------+
|		|			  |This bit indicates that the MIFARE Crypto1 unit is switched on and	|
|		|			  |therefore all data communication with the card is encrypted.			|
|	3	| MFCrypto1On |This bit can only be set to logic 1 by a successful execution of the	|
|		|			  |MFAuthent Command. This bit is only valid in Reader/Writer mode		|
|		|			  |for MIFARE Standard cards. This bit shall be cleared by software.	|
+-------+-------------+---------------------------------------------------------------------+
|2 to 0	| Modem State |ModemState shows the state of the transmitter and receiver state		|
|		|			  |machines.															|
+-------+-------------+-------------------+-------------------------------------------------+
|		|			  |		  Status	  |					 Description					|
+-------+-------------+-------------------+-------------------------------------------------+
|		|			  |			000	   	  | 					IDLE						|
+-------+-------------+-------------------+-------------------------------------------------+
|		|			  |			001	   	  |Wait for bit StartSend in register  BitFramingReg|
+-------+-------------+-------------------+-------------------------------------------------+
|		|			  |			 	  	  |TxWait: Wait until Rf field is present, if the	|
|		|			  |			010	 	  |bit TxWaitRF is set to logic 1. The minimum time	|
|		|			  |				 	  |for TxWait is defined by the TxWaitReg  register.|
+-------+-------------+-------------------+-------------------------------------------------+
|		|			  |			011	   	  | 				Transmitting					|
+-------+-------------+-------------------+-------------------------------------------------+
|		|			  |				 	  |RxWait: Wait until RF field is present, if the	|
|		|			  |			100		  |bit RxWaitRF  is set to logic 1. The minimum time|
|		|			  |				 	  |for RxWait is defined by the RxWaitReg  register.|
+-------+-------------+-------------------+-------------------------------------------------+
|		|			  |			101	   	  | 				Wait for data					|
+-------+-------------+-------------------+-------------------------------------------------+
|		|			  |			110	   	  | 					Receiving					|
+-------+-------------+-------------------+-------------------------------------------------+
*/
/* In- and output of 64 byte FIFO buffer --------------------------------------*/
#define FIFODataReg							0x09
/*
								Description ofFIFODataReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Data input and output port for the internal 64 byte FIFO buffer. The	|
|7 to 0 | FIFOData |FIFO buffer acts as parallel in/parallel out converter for all serial	|
|		|		   |data stream in- and outputs.											|
+-------+----------+------------------------------------------------------------------------+
*/
/* Indicates the number of bytes stored in the FIFO ---------------------------*/
#define FIFOLevelReg						0x0A
/*
								Description of FIFOLevelReg bits
+-------+-----------+------------------------------------------------------------------------+
|  Bit  |   Symbol  |								Description								 |
+-------+-----------+------------------------------------------------------------------------+
|		|			|Set to logic 1, this bit clears the internal FIFO-buffer's read- and	 |
|	7	|FlushBuffer|write-pointer and the bit BufferOvfl in the register ErrReg immediately.|
|		|			|Reading this bit will always return 0.									 |
+-------+-----------+------------------------------------------------------------------------+
|6 to 0	| FIFOLevel |Indicates the number of bytes stored in the FIFO buffer. Writing to the |
|		|			|FIFODataReg increments, reading decrements the FIFOLevel.				 |
+-------+-----------+------------------------------------------------------------------------+
*/
/* Defines the level for FIFO under- and overflow warning ---------------------*/
#define WaterLevelReg						0x0B
/* Contains miscellaneous Control Registers -----------------------------------*/
#define ControlReg							0x0C
/*
								Description of ControlReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|	7	| TStopNow |Set to logic 1, the timer stops immediately.							|
|		|		   |Reading this bit will always return 0.									|
+-------+----------+------------------------------------------------------------------------+
|	6	|TStartNow |Set to logic 1 starts the timer immediately.							|
|		|		   |Reading this bit will always return 0.									|
+-------+----------+------------------------------------------------------------------------+
|5 to 3 |	 -	   |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
|2 to 0 |RxLastBits|Shows the number of valid bits in the last received byte. If 0, the		|
|		|		   |whole byte is valid.													|
+-------+----------+------------------------------------------------------------------------+
*/
/* Adjustments for bit oriented frames ----------------------------------------*/
#define BitFramingReg						0x0D
/*
								Description of BitFramingReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|	7	|StartSend |Set to logic 1, the transmission of data starts.						|
|		|		   |This bit is only valid in combination with the Transceive command.		|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Used for reception of bit oriented frames: RxAlign defines the bit		|
|6 to 4 | RxAlign  |position for the first bit received to be stored in the FIFO. Further	|
|		|		   |received bits are stored at the following bit positions.				|
+-------+----------+------------------------------------------------------------------------+
|				   |Example:																|
|				   |RxAlign = 0:  the LSB of the received bit is stored at bit 0, the second|
|				   |received bit is stored at bit position 1.								|
|				   |RxAlign = 1:  the LSB of the received bit is stored at bit 1, the second|
|				   |received bit is stored at bit position 2.								|
|				   |RxAlign = 7:  the LSB of the received bit is stored at bit 7, the second|
|				   |received bit is stored in the following byte at bit position 0.			|
|				   +------------------------------------------------------------------------+
|				   |This bits shall only be used for bitwise anticollision at 106 kbit/s.	|
|				   |In all other modes it shall be set to 0.								|
+-------+----------+------------------------------------------------------------------------+
|	3	|	 -	   |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Used for transmission of bit oriented frames: TxLastBits defines the	|
|2 to 0 |TxLastBits|number of bits of the last byte that shall be transmitted. A 000b		|
|		|		   |indicates that all bits of the last byte shall be transmitted.			|
+-------+----------+------------------------------------------------------------------------+
*/
/* Bit position of the first bit collision detected on the RF-interface -------*/
#define CollReg								0x0E
/* Reserved for future use ----------------------------------------------------*/
#define RFU0F								0x0F

/*------------------------------ Page1 : Command ------------------------------*/
/* Reserved for future use ----------------------------------------------------*/
#define RFU10								0x10
/* Defines general modes for transmitting and receiving -----------------------*/
#define ModeReg								0x11
/*
								Description of ModeReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, the CRC co-processor calculates the CRC with MSB		|
|	7	| MSBFirst |first and the CRCResultMSB and the CRCResultLSB in the					|
|		|		   |CRCResultReg register are bit reversed.									|
|		|		   |Remark:  During RF communication this bit is ignored.					|
+-------+----------+------------------------------------------------------------------------+
|	6	|	 -     |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
|	5	| TxWaitRF |Set to logic 1 the transmitter can only be started, if an RF field is	|
|		|		   |generated.																|
+-------+----------+------------------------------------------------------------------------+
|	4	|	 -     |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
|		|		   |PolMFin defines the polarity of the MFIN pin. Set to logic 1, the		|
|		|		   |polarity of MFIN pin is active high. Set to logic 0 the polarity of		|
|	3	| PolMFin  |MFIN pin is active low.													|
|		|		   |Remark: The internal envelope signal is coded active low.				|
|		|		   |		Changing this bit will generate a MFinActIRq event.				|
+-------+----------+------------------------------------------------------------------------+
|	2	|	 -     |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Defines the preset value for the CRC co-processor for the command		|
|		|		   |CalCRC.																	|
|1 to 0 |CRCPreset |Remark: During any communication, the preset values is selected			|
|		|		   |		automatically according to the definition in the bits in		|
|		|		   |		RxModeReg and TxModeReg.										|
+-------+----------+----------------------------+-------------------------------------------+
|				   |			Value			|				Description					|
+------------------+----------------------------+-------------------------------------------+
|				   |			 00				|					0000					|
|				   |			 01				|					6363					|
|				   |			 10				|					A671					|
|				   |			 11				|					FFFF					|
+------------------+----------------------------+-------------------------------------------+
*/
/* Defines the transmission data rate and framing -----------------------------*/
#define TxModeReg							0x12
/*
								Description of TxModeReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, this bit enables the CRC generation during data			|
|	7	| TxCRCEn  |transmission.															|
|		|		   |Remark:  This bit shall only be set to logic 0 at 106 kbit/s.			|
+-------+----------+------------------------------------------------------------------------+
|6 to 4 | TxSpeed  |Defines the bit rate while data transmission. The MFRC522 handels		|
|		|		   |transfer speeds up to 848 kbit/s.										|
+-------+----------+----------------------------+-------------------------------------------+
|				   |			Value			|				Description					|
+------------------+----------------------------+-------------------------------------------+
|				   |			 000			|				106 kbit/s					|
|				   |			 001			|				212 kbit/s					|
|				   |			 010			|				424 kbit/s					|
|				   |			 011			|				848 kbit/s					|
|				   |			 100			|				 Reserved					|
|				   |			 101			|				 Reserved					|
|				   |			 110			|				 Reserved					|
|				   |			 111			|				 Reserved					|
+-------+----------+----------------------------+-------------------------------------------+
|	3	|  InvMod  |Set to logic 1, the modulation for transmitting data is inverted.		|
+-------+----------+------------------------------------------------------------------------+
|2 to 0 |	 -	   |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
*/
/* Defines the receive data rate and framing ----------------------------------*/
#define RxModeReg							0x13
/*
								Description of RxModeReg bits
+-------+----------+-------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								 |
+-------+----------+-------------------------------------------------------------------------+
|	7	| RxCRCEn  |Set to logic 1, this bit enables the CRC calculation during reception.	 |
|		|		   |Remark:  This bit shall only be set to logic 0 at 106 kbit/s.			 |
+-------+----------+-------------------------------------------------------------------------+
|6 to 4 | RxSpeed  |Defines the bit rate while data receiving. The MFRC522 handles			 |
|		|		   |transfer speeds up to 848kbit/s.										 |
+-------+----------+----------------------------+--------------------------------------------+
|				   |			Value			|				Description					 |
+------------------+----------------------------+--------------------------------------------+
|				   |			 000			|				106 kbit/s					 |
|				   |			 001			|				212 kbit/s					 |
|				   |			 010			|				424 kbit/s					 |
|				   |			 011			|				848 kbit/s					 |
|				   |			 100			|				 Reserved					 |
|				   |			 101			|				 Reserved					 |
|				   |			 110			|				 Reserved					 |
|				   |			 111			|				 Reserved					 |
+-------+----------+----------------------------+--------------------------------------------+
|	3	| RxNoErr  |If set to logic 1, a not valid received data stream (less than 4 bits	 |
|		|		   |received) will be ignored. The receiver will remain active.				 |
+-------+----------+-------------------------------------------------------------------------+
|		|		   |Set to logic 0, the receiver is deactivated after receiving a data frame.|
|		|		   |Set to logic 1, it is possible to receive more than one data frame. This |
|		|		   |bit is only valid for data rates above 106 kbit/s to handle the Polling	 |
|		|		   |command. Having set this bit, the receive and transceive commands will	 |
|	2	|RxMultiple|not terminate automatically. In this case the multiple receiving can only|
|		|		   |be deactivated by writing any command (except the Receive command)		 |
|		|		   |to the CommandReg register or by clearing the bit by the host.			 |
|		|		   |If set to logic 1, at the end of a received data stream an error byte is |
|		|		   |added to the FIFO. The error byte is a copy of the ErrorReg  register.	 |
+-------+----------+-------------------------------------------------------------------------+
|1 to 0 |	 -     |Reserved for future use.												 |
+-------+----------+-------------------------------------------------------------------------+
*/
/* Controls the logical behavior of the antenna driver pins TX1 and TX2 -------*/
#define TxControlReg						0x14
/*
								Description of TxControlReg bits
+-------+-----------+------------------------------------------------------------------------+
|  Bit  |   Symbol  |								Description								 |
+-------+-----------+------------------------------------------------------------------------+
|	7	|InvTx2RFOn	|Set to logic 1, the output signal at pin TX2 will be inverted, if driver|
|		|			|TX2 is enabled.														 |
+-------+-----------+------------------------------------------------------------------------+
|	6	|InvTx1RFOn	|Set to logic 1, the output signal at pin TX1 will be inverted, if driver|
|		|			|TX1 is enabled.														 |
+-------+-----------+------------------------------------------------------------------------+
|	5	|InvTx2RFOff|Set to logic 1, the output signal at pin TX2 will be inverted, if driver|
|		|			|TX2 is disabled.														 |
+-------+-----------+------------------------------------------------------------------------+
|	4	|InvTx1RFOff|Set to logic 1, the output signal at pin TX1 will be inverted, if driver|
|		|			|TX1 is disabled.														 |
+-------+-----------+------------------------------------------------------------------------+
|		|			|Set to logic 1, the output signal on pin TX2 will deliver continuously	 |
|	3	|	Tx2CW	|the un-modulated 13.56 MHz energy carrier.								 |
|		|			|Set to logic 0, Tx2CW is enabled to modulate the 13.56 MHz energy		 |
|		|			|carrier.																 |
+-------+-----------+------------------------------------------------------------------------+
|	2	|	  - 	|Reserved for future use.												 |
+-------+-----------+------------------------------------------------------------------------+
|	1	|  Tx2RFEn  |Set to logic 1, the output signal on pin TX2 will deliver the 13.56 MHz |
|		|			|energy carrier modulated by the transmission data.						 |
+-------+-----------+------------------------------------------------------------------------+
|	0	|  Tx1RFEn  |Set to logic 1, the output signal on pin TX1 will deliver the 13.56 MHz |
|		|			|energy carrier modulated by the transmission data.						 |
+-------+-----------+------------------------------------------------------------------------+
*/
/* Controls the setting of the TX modulation ----------------------------------*/
#define TxASKReg							0x15
/*
								Description of TxAutoReg bits
+-------+-----------+------------------------------------------------------------------------+
|  Bit  |   Symbol  |								Description								 |
+-------+-----------+------------------------------------------------------------------------+
|	7	|	  - 	|Reserved for future use.												 |
+-------+-----------+------------------------------------------------------------------------+
|	6	|Force100ASK|Set to logic 1, Force100ASK forces a 100% ASK modulation				 |
|		|			|independent of the setting in register ModGsPReg.						 |
+-------+-----------+------------------------------------------------------------------------+
|5 to 0 |	  -		|Reserved for future use.												 |
+-------+-----------+------------------------------------------------------------------------+
*/
/* Selects the internal sources for the antenna driver ------------------------*/
#define TxSelReg							0x16
/*
								Description of TxSelReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|7 to 6 |	 -	   |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
|5 to 4 |DriverSel |Selects the input of driver Tx1 and Tx2.								|
+-------+----------+----------------------------+-------------------------------------------+
|				   |			Value			|				 Description 				|
+------------------+----------------------------+-------------------------------------------+
|				   |			 00				|				  Tristate					|
|				   |							|Remark:  In soft power-down the drivers are|
|				   |							|only in tristate mode if DriverSel is set	|
|				   |							|to tristate mode.							|
|				   +----------------------------+-------------------------------------------+
|				   |			 01				|Modulation signal (envelope) from the		|
|				   |							|internal coder, Miller Pulse Coded.		|
|				   +----------------------------+-------------------------------------------+
|				   |			 10				|Modulation signal (envelope) from MIFIN	|
|				   +----------------------------+-------------------------------------------+
|				   |			 11				|					High					|
|				   |							|Remark:  The High level depends on the		|
|				   |							|setting of InvTx1RFOn/ InvTx1RFOff and		|
|				   |							|InvTx2RFOn/ InvTx2RFOff.					|
+-------+----------+----------------------------+-------------------------------------------+
|3 to 0 | MFOutSel |Selects the input for the MFOUT Pin.									|
+-------+----------+----------------------------+-------------------------------------------+
|				   |			Value			|				 Description 				|
+------------------+----------------------------+-------------------------------------------+
|				   |			0000			|				  Tristate  				|
|				   |			0001			|					 Low					|
|				   |			0010			|					High					|
|				   |			0011			|TestBus signal as defined by bit			|
|				   |							|TestBusBitSel in register TestSel1Reg.		|
|				   |			0100			|Modulation signal (envelope) from the		|
|				   |							|internal coder, Miller Puls Coded			|
|				   |			0101			|Serial data stream to be transmitted, data	|
|				   |							|stream before Miller Coder					|
|				   |			0110			|				  Reserved					|
|				   |			0111			|Serial data stream received, data stream	|
|				   |							|after Manchester Decoder					|
|				   |		 1000-1111			|				  Reserved					|
+------------------+----------------------------+-------------------------------------------+
*/
/* Selects internal receiver settings -----------------------------------------*/
#define RxSelReg							0x17
/*
								Description of RxSelReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|7 to 6 | UartSel  |Selects the input of the contactless UART								|
+-------+----------+----------------------------+-------------------------------------------+
|				   |			Value			|				 Description 				|
|				   +----------------------------+-------------------------------------------+
|				   |			  00  			|Constant Low								|
|				   +----------------------------+-------------------------------------------+
|				   |			  01  			|Manchester with sub-carrier from MFIN pin	|
|				   +----------------------------+-------------------------------------------+
|				   |			  10  			|Modulation signal from the internal analog	|
|				   |							|part, default								|
|				   +----------------------------+-------------------------------------------+
|				   |			  11  			|NRZ coding without sub-carrier from MFIN	|
|				   |							|pin. Only valid for transfer speeds above	|
|				   |							|106 kbit/s.								|
+-------+----------+----------------------------+-------------------------------------------+
|		|		   |After data transmission, the activation of the receiver is delayed for	|
|		|		   |RxWait bit-clocks. During this 'frame guard time' any signal at pin Rx	|
|5 to 0 |  RxWait  |is ignored.This parameter is ignored by the receive command.			|
|		|		   |All other commands (e.g. Transceive, MFAuthent) use this parameter.		|
|		|		   |The counter starts immediately after the external RF field is switched	|
|		|		   |on.																		|
+-------+----------+------------------------------------------------------------------------+
*/
/* Selects thresholds for the bit decoder -------------------------------------*/
#define RxThresholdReg						0x18
/*
								Description of RxThresholdReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Defines the minimum signal strength at the decoder input that shall be	|
|7 to 4 | MinLevel |accepted. If the signal strength is below this level, it is not			|
|		|		   |evaluated.																|
+-------+----------+------------------------------------------------------------------------+
|	3	|	 -	   |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Defines the minimum signal strength at the decoder input that has to be	|
|2 to 0 |CollLevel |reached by the weaker half-bit of the Manchester-coded signal to		|
|		|		   |generate a bit-collision relatively to the amplitude of the stronger	|
|		|		   |half-bit.																|
+-------+----------+------------------------------------------------------------------------+
*/
/* Defines demodulator settings -----------------------------------------------*/
#define DemodReg							0x19
/* Reserved for future use ----------------------------------------------------*/
#define RFU1A								0x1A
/* Reserved for future use ----------------------------------------------------*/
#define RFU1B								0x1B
/* Controls some MIFARE communication transmit parameters ---------------------*/
#define MfTxReg								0x1C
/*
								Description of MifNFCReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|7 to 2 |	 -	   |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
|1 to 0 |  TxWait  |These bits define the additional response time. Per default 7 bits are	|
|		|		   |added to the value of the register bit.									|
+-------+----------+------------------------------------------------------------------------+
*/
/* Controls some MIFARE communication receive parameters ----------------------*/
#define MfRxReg								0x1D
/*
								Description of ManualRCVReg bits
+-------+--------------+--------------------------------------------------------------------+
|  Bit  |    Symbol    |							Description								|
+-------+--------------+--------------------------------------------------------------------+
|7 to 5 |		-	   |Reserved for future use.											|
+-------+--------------+--------------------------------------------------------------------+
|		|			   |If this bit is set to logic 1, the generation of the Parity bit for	|
|	4	|Parity Disable|transmission and the Parity-Check for receiving is switched off. The|
|		|			   |received Parity bit is handled like a data bit.						|
+-------+--------------+--------------------------------------------------------------------+
|3 to 0 |		-	   |Reserved for future use.											|
+-------+--------------+--------------------------------------------------------------------+
*/
/* Reserved for future use ----------------------------------------------------*/
#define RFU1E								0x1E
/* Selects the speed of the serial UART interface -----------------------------*/
#define SerialSpeedReg						0x1F
/*
								Description of SerialSpeedReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|7 to 5 |  BR_T0   |Factor BR_T0 to adjust the transfer speed, for description see  Section	|
|		|		   |10.3.2 "Selection of the transfer speeds".								|
+-------+----------+------------------------------------------------------------------------+
|4 to 0 |  BR_T1   |Factor BR_T1 to adjust the transfer speed, for description see  Section	|
|		|		   |10.3.2 "Selection of the transfer speeds".								|
+-------+----------+------------------------------------------------------------------------+
*/

/*-------------------------------- Page2 : CFG --------------------------------*/
/* Reserved for future use ----------------------------------------------------*/
#define RFU20								0x20
/* Shows the actual MSB and LSB values of the CRC calculation -----------------*/
#define CRCResultRegH						0x21
/*
							Description of CRCResultReg higher bits
+-------+------------+-----------------------------------------------------------------------+
|  Bit  |   Symbol   |								Description								 |
+-------+------------+-----------------------------------------------------------------------+
|		|			 |This register shows the actual value of the most significant byte of	 |
|7 to 0 |CRCResultMSB|the CRCResultReg register. It is valid only if bit CRCReady  in		 |
|		|			 |register Status1Reg is set to logic 1.								 |
+-------+------------+-----------------------------------------------------------------------+
*/
/* Shows the actual MSB and LSB values of the CRC calculation -----------------*/
#define CRCResultRegL						0x22
/*
							Description of CRCResultReg lower bits
+-------+------------+-----------------------------------------------------------------------+
|  Bit  |   Symbol   |								Description								 |
+-------+------------+-----------------------------------------------------------------------+
|		|			 |This register shows the actual value of the least significant byte of	 |
|7 to 0 |CRCResultLSB|the CRCResult register. It is valid only if bit CRCReady in register	 |
|		|			 |Status1Reg is set to logic 1.											 |
+-------+------------+-----------------------------------------------------------------------+
*/
/* Reserved for future use ----------------------------------------------------*/
#define RFU23								0x23
/* Controls the setting of the ModWidth ---------------------------------------*/
#define ModWidthReg							0x24
/*
								Description of ModWidthReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|		|		   |These bits define the width of the Miller modulation as multiples of the|
|7 to 0 | ModWidth |carrier frequency (ModWidth +1/fc). The maximum value is half the bit	|
|		|		   |period.																	|
+-------+----------+------------------------------------------------------------------------+
*/
/* Reserved for future use ----------------------------------------------------*/
#define RFU25								0x25
/* Configures the receiver gain -----------------------------------------------*/
#define RFCfgReg							0x26
/*
								Description of RFCfgReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|	7	|	 -     |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
|6 to 4 |  RxGain  |This register defines the receivers signal voltage gain factor:			|
+-------+----------+----------------------------+-------------------------------------------+
|				   |			Value			|				  Description  				|
+------------------+----------------------------+-------------------------------------------+
|				   |			 000			|					 18 dB					|
|				   |			 001			|					 23 dB					|
|				   |			 010			|					 18 dB					|
|				   |			 011			|					 23 dB					|
|				   |			 100			|					 33 dB					|
|				   |			 101			|					 38 dB					|
|				   |			 110			|					 43 dB					|
|				   |			 111			|					 48 dB					|
+-------+----------+----------------------------+-------------------------------------------+
|3 to 0 |	 -	   |Reserved for future use.												|
+-------+----------+------------------------------------------------------------------------+
*/
/* Selects the conductance of the antenna driver pins TX1 and TX2 for modulation */
#define GsNReg								0x27
/* ----------------------------------------------------------------------------*/
#define CWGsPReg							0x28
/* ----------------------------------------------------------------------------*/
#define ModGsPReg							0x29
/* Defines settings for the internal timer ------------------------------------*/
#define TModeReg							0x2A
/*
								Description of TModeReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|		|		   |Set to logic 1, the timer starts automatically at the end of the		|
|		|		   |transmission in all communication modes at all speeds. The timer stops	|
|		|		   |immediately after receiving the first data bit if the bit RxMultiple in	|
|		|		   |the register RxModeReg is not set.										|
|	7	|	TAuto  |If RxMultiple is set to logic 1, the timer never stops. In this case the|
|		|		   |timer can be stopped by setting the bit TStopNow in register  ControlReg|
|		|		   |to logic 1.																|
|		|		   |Set to logic 0 indicates, that the timer is not influenced by the		|
|		|		   |protocol.																|
+-------+----------+------------------------------------------------------------------------+
|		|		   |The internal timer is running in gated mode.							|
|6 to 5 |  TGated  |Remark: In the gated mode, the bit TRunning is logic 1 when the timer	|
|		|		   |		is enabled by the register bits. This bit does not influence the|
|		|		   |		gating signal.													|
+-------+----------+----------------------------+-------------------------------------------+
|				   |			Value			|				  Description  				|
+------------------+----------------------------+-------------------------------------------+
|				   |			 00				|				Non gated mode				|
|				   |			 01				|				 Gated by MFIN				|
|				   |			 10				|				 Gated by AUX1				|
|				   |			 11				|				  Gated by A3				|
+-------+----------+-+--------------------------+-------------------------------------------+
|		|			 |Set to logic 1, the timer automatically restart its count-down from	|
|	4	|TAutoRestart|TReloadValue, instead of counting down to zero.						|
|		|			 |Set to logic 0 the timer decrements to 0 and the bit TimerIRq is set	|
|		|			 |to logic 1.															|
+-------+------------++---------------------------------------------------------------------+
|		|			  |Defines higher 4 bits for TPrescaler.								|
|3 to 0 |TPrescaler_Hi|The following formula is used to calculate fTimer:					|
|		|			  |		fTimer = 6.78 MHz/TPreScaler.									|
|		|			  |For detailed description see Section 13 "Timer Unit".				|
+-------+-------------+---------------------------------------------------------------------+
*/
/* ----------------------------------------------------------------------------*/
#define TPrescalerReg						0x2B
/*
								Description of TPrescalerReg bits
+-------+-------------+---------------------------------------------------------------------+
|  Bit  |   Symbol    |								Description								|
+-------+-------------+---------------------------------------------------------------------+
|		|			  |Defines lower 8 bits for TPrescaler.									|
|7 to 0 |TPrescaler_Lo|The following formula is used to calculate fTimer:					|
|		|			  |		fTimer = 6.78 MHz/TPreScaler.									|
|		|			  |For detailed description see Section 13 "Timer Unit".				|
+-------+-------------+---------------------------------------------------------------------+
*/
/* Describes the 16 bit timer reload value ------------------------------------*/
#define TReloadRegH							0x2C
/*
								Description of higher TReloadReg bits
+-------+-------------+---------------------------------------------------------------------+
|  Bit  |   Symbol    |								Description								|
+-------+-------------+---------------------------------------------------------------------+
|		|			  |Defines the higher 8 bits for the TReloadReg.						|
|7 to 0 |TReloadVal_Hi|With a start event the timer loads the TReloadVal. Changing this		|
|		|			  |register affects the timer only at the next start event.				|
+-------+-------------+---------------------------------------------------------------------+
*/
/* Describes the 16 bit timer reload value ------------------------------------*/
#define TReloadRegL							0x2D
/*
								Description of lower TReloadReg bits
+-------+-------------+---------------------------------------------------------------------+
|  Bit  |   Symbol    |								Description								|
+-------+-------------+---------------------------------------------------------------------+
|		|			  |Defines the lower 8 bits for the TReloadReg.							|
|7 to 0 |TReloadVal_Lo|With a start event the timer loads the TReloadVal. Changing this		|
|		|			  |register affects the timer only at the next start event.				|
+-------+-------------+---------------------------------------------------------------------+
*/
/* Shows the 16 bit actual timer value ----------------------------------------*/
#define TCounterValRegH						0x2E
/*
								Description of higher TCounterValReg bits
+-------+--------------+--------------------------------------------------------------------+
|  Bit  |    Symbol    |							Description								|
+-------+--------------+--------------------------------------------------------------------+
|7 to 0 |TCounterVal_Hi|Current value of the timer, higher 8 bits.							|
+-------+--------------+--------------------------------------------------------------------+
*/
/* Shows the 16 bit actual timer value ----------------------------------------*/
#define TCounterValRegL						0x2F
/*
								Description of lower TCounterValReg bits
+-------+--------------+--------------------------------------------------------------------+
|  Bit  |    Symbol    |							Description								|
+-------+--------------+--------------------------------------------------------------------+
|7 to 0 |TCounterVal_Lo|Current value of the timer, lower 8 bits.							|
+-------+--------------+--------------------------------------------------------------------+
*/

/*--------------------------- Page3 : TestRegister ----------------------------*/
/* Reserved for future use ----------------------------------------------------*/
#define RFU30								0x30
/* General test signal configuration ------------------------------------------*/
#define TestSel1Reg							0x31
/* General test signal configuration and PRBS control -------------------------*/
#define TestSel2Reg							0x32
/* Enables pin output driver on D1 - D7 ---------------------------------------*/
#define TestPinEnReg						0x33
/* Defines the values for D1 - D7 when it is used as I/O bus ------------------*/
#define TestPinValueReg						0x34
/* Shows the status of the internal testbus -----------------------------------*/
#define TestBusReg							0x35
/* Controls the digital selftest ----------------------------------------------*/
#define AutoTestReg							0x36
/* Shows the version ----------------------------------------------------------*/
#define VersionReg							0x37
/*
								Description of VersionReg bits
+-------+----------+------------------------------------------------------------------------+
|  Bit  |  Symbol  |								Description								|
+-------+----------+------------------------------------------------------------------------+
|7 to 0 | Version  |Indicates current version.												|
|		|		   |Remark: The current version for MFRC522 is 90h or 91h.					|
+-------+----------+------------------------------------------------------------------------+
*/
/* Controls the pins AUX1 and AUX2 --------------------------------------------*/
#define AnalogTestReg						0x38
/* Defines the test value for the TestDAC1 ------------------------------------*/
#define TestDAC1Reg							0x39
/* Defines the test value for the TestDAC2 ------------------------------------*/
#define TestDAC2Reg							0x3A
/* Shows the actual value of ADC I and Q --------------------------------------*/
#define TestADCReg							0x3B
/* Reserved for future use ----------------------------------------------------*/
#define RFU3C								0x3C
/* Reserved for future use ----------------------------------------------------*/
#define RFU3D								0x3D
/* Reserved for future use ----------------------------------------------------*/
#define RFU3E								0x3E
/* Reserved for future use ----------------------------------------------------*/
#define RFU3F								0x3F

/*******************************************************************************/
/**************************** END REGISTER MAPPING *****************************/
/*******************************************************************************/

/* TModeReg->TAuto */
#define TAutoMode_On						(1<<7)
#define TAutoMode_Off						(0<<7)

/* TxASKReg->Force100ASK */
#define Force100ASK							(1<<6)
#define NoForce								(0<<6)

/* ModeReg->CRCPreset */
#define CRCPreset_0000						(0x00)
#define CRCPreset_6363						(0x01)
#define CRCPreset_A671						(0x02)
#define CRCPreset_FFFF						(0x03)

/* Exported FunctionPrototypes -----------------------------------------------*/
void MFRC522_Init(MFRC522_InitTypeDef *MFRC522_InitStruct);
void MFRC522_Reset(void);
void Set_Antenna_On(void);
void Set_Antenna_Off(void);
uint8_t MFRC522_RequestCard(uint8_t ReqMode, uint8_t *TagType);
uint8_t MFRC522_Anti_Coll_Detec(uint8_t *pSerNum);
uint8_t MFRC522_SelectTag(uint8_t *pSerNum);
uint8_t MFRC522_Auth(uint8_t AuthMode, uint8_t BlockAddr, uint8_t *SectorKey, uint8_t *pSerNum);
uint8_t MFRC522_TX_Packet(uint8_t BlockAddr, uint8_t *pBuffer);
uint8_t MFRC522_RX_Packet(uint8_t BlockAddr, uint8_t *pBuffer);
void MFRC522_EnterHalt(void);
uint8_t CommunicateWithIC(uint8_t Command, uint8_t *pTransData, uint8_t TransLen, uint8_t *pReceiData, uint16_t *ReceiLen);
uint8_t MFRC522_SPI_RW_Reg(uint8_t Reg, uint8_t Value);
uint8_t MFRC522_SPI_Read_Reg(uint8_t Reg);
#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
uint32_t MFRC522_TIMEOUT_UserCallback(void);
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

#ifdef __cplusplus
 }
#endif

#endif /* __STM32F4XX_NUCLEO_EXTRC522_H */

/*************************************** END OF FILE ***************************************/
