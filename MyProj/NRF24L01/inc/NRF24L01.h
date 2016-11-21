/**
  ******************************************************************************
  * @file    NRF24L01.h
  * @author  '^_^'
  * @version V1.1.0
  * @date    25-November-2013
  * @brief   This file contains all the functions prototypes for the NRF24L01.c firmware driver.
  ******************************************************************************  
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __NRF24L01_H
#define __NRF24L01_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#define USE_DEFAULT_TIMEOUT_CALLBACK
#define NRF24L01_FLAG_TIMEOUT			((uint32_t)0x1000)

#define ADR_WIDTH						5

/* NRF24L01 struct */
typedef struct
{
	uint8_t *TX_ADD;
	uint8_t *RX_ADD;
	uint8_t AUTO_ACK;
	uint8_t RX_PIPE;
	uint8_t COM_FREQ;
	uint8_t RF_SETUP_VALUE;
	uint8_t TRANS_MODE;
	uint8_t PLOAD_WIDTH;
	uint8_t REPEATS;
}NRF24L01_InitTypeDef;

typedef enum 
{  
	IRQ_MODE_GPIO = 0,
	IRQ_MODE_EXTI = 1
}IRQMode_TypeDef;

/**
  * @brief  NRF24L01 SPI Interface pins
  */
/* Definition for NRF24L01_SPI clock resources */
#define NRF24L01_SPI								SPI2
#define NRF24L01_SPI_CLK_ENABLE()					__SPI2_CLK_ENABLE()
#define NRF24L01_SPI_SCK_GPIO_CLK_ENABLE()			__GPIOB_CLK_ENABLE()
#define NRF24L01_SPI_MISO_GPIO_CLK_ENABLE()			__GPIOB_CLK_ENABLE()
#define NRF24L01_SPI_MOSI_GPIO_CLK_ENABLE()			__GPIOB_CLK_ENABLE()

#define NRF24L01_SPI_FORCE_RESET()					__SPI2_FORCE_RESET()
#define NRF24L01_SPI_RELEASE_RESET()				__SPI2_RELEASE_RESET()

/* Definition for NRF24L01_SPI Pins */
#define NRF24L01_SPI_SCK_PIN						GPIO_PIN_13
#define NRF24L01_SPI_SCK_GPIO_PORT					GPIOB
#define NRF24L01_SPI_SCK_AF							GPIO_AF5_SPI2
#define NRF24L01_SPI_MISO_PIN						GPIO_PIN_14
#define NRF24L01_SPI_MISO_GPIO_PORT					GPIOB
#define NRF24L01_SPI_MISO_AF						GPIO_AF5_SPI2
#define NRF24L01_SPI_MOSI_PIN						GPIO_PIN_15
#define NRF24L01_SPI_MOSI_GPIO_PORT					GPIOB
#define NRF24L01_SPI_MOSI_AF						GPIO_AF5_SPI2

/* Definition for NRF24L01's other pins */
#define NRF24L01_CE_PIN								GPIO_PIN_12
#define NRF24L01_CE_GPIO_PORT						GPIOB
#define NRF24L01_CE_GPIO_CLK_ENABLE()				__GPIOB_CLK_ENABLE()

#define NRF24L01_CSN_PIN							GPIO_PIN_2
#define NRF24L01_CSN_GPIO_PORT						GPIOA
#define NRF24L01_CSN_GPIO_CLK_ENABLE()				__GPIOA_CLK_ENABLE()

#define NRF24L01_IRQ_PIN							GPIO_PIN_8
#define NRF24L01_IRQ_GPIO_PORT						GPIOD
#define NRF24L01_IRQ_GPIO_CLK_ENABLE()				__GPIOD_CLK_ENABLE()
#define NRF24L01_IRQ_EXIT_IRQn						EXTI9_5_IRQn

/*************************** SPI(nRF24L01) commands ***************************/
/* Define read command to register */
#define READ_REG_NRF						0x00
/* Define write command to register */
#define WRITE_REG_NRF						0x20
/* Define RX payload register address */
#define RD_RX_PLOAD							0x61
/* Define TX payload register address */
#define WR_TX_PLOAD							0xA0
/* Define flush TX register command */
#define FLUSH_TX							0xE1
/* Define flush RX register command */
#define FLUSH_RX							0xE2
/* Define reuse TX payload register command */
#define REUSE_TX_PL							0xE3
/* Define No Operation, might be used to read status register */
#define NOP									0xFF

/*******************************************************************************/
/********************* SPI(nRF24L01) registers(addresses) **********************/
/*******************************************************************************/

/* 'Config' register address */
#define CONFIG								0x00
/*
+------------------------------------------------------------------------------------------+
|Bit|Mnemonic	|Reset Value|Type|Description											   |
+---+-----------+-----------+----+---------------------------------------------------------+
| 7	|reserved	|     0		|R/W |Only '0' allowed.										   |
+---+-----------+-----------+----+---------------------------------------------------------+
| 6	|MASK_RX_DR	|     0		|R/W |Mask interrupt caused by RX_DR.						   |
|  	|			|	   		|	 |1: Interrupt not reflected on the IRQ pin.			   |
|	|			|			|	 |0: Reflect RX_DR as active low interrupt on the IRQ pin. |
+---+-----------+-----------+----+---------------------------------------------------------+
| 5	|MASK_TX_DS	|     0		|R/W |Mask interrupt caused by TX_DS.						   |
|  	|			|	   		|	 |1: Interrupt not reflected on the IRQ pin.			   |
|	|			|			|	 |0: Reflect TX_DS as active low interrupt on the IRQ pin. |
+---+-----------+-----------+----+---------------------------------------------------------+
| 4	|MASK_MAX_RT|     0		|R/W |Mask interrupt caused by MAX_RT.						   |
|  	|			|	   		|	 |1: Interrupt not reflected on the IRQ pin.			   |
|	|			|			|	 |0: Reflect MAX_RT as active low interrupt on the IRQ pin.|
+---+-----------+-----------+----+---------------------------------------------------------+
| 3	|EN_CRC		|     1		|R/W |Enable CRC.											   |
|	|			|			|	 |	Forced high if one of the bits in the EN_AA is high.   |
+---+-----------+-----------+----+---------------------------------------------------------+
|	|			|     		|	 |CRC encoding scheme.									   |
| 2 |CRCO		|	  0		|R/W |					'0' - 1 byte.						   |
|	|			|			|	 |					'1' ¨C 2 bytesigh.					   |
+---+-----------+-----------+----+---------------------------------------------------------+
| 1	|PWR_UP		|     0		|R/W |1: POWER UP, 0:POWER DOWN.							   |
+---+-----------+-----------+----+---------------------------------------------------------+
| 0	|PRIM_RX	|     0		|R/W |RX/TX control.										   |
|	|			|			|	 |					1: PRX, 0: PTX.						   |
+------------------------------------------------------------------------------------------+
*/
/* 'Enable Auto Acknowledgment' register address */
#define EN_AA								0x01
/* 'Enabled RX addresses' register address */
#define EN_RXADDR							0x02
/* 'Setup address width' register address */
#define SETUP_AW							0x03
/* 'Setup Auto. Retrans' register address */
#define SETUP_RETR							0x04
/* 'RF channel' register address */
#define RF_CH								0x05
/* 'RF setup' register address */
#define RF_SETUP							0x06
/* 'Status' register address */
#define STATUS								0x07
/* 'Observe TX' register address */
#define OBSERVE_TX							0x08
/* 'Carrier Detect' register address */
#define CD									0x09
/* 'RX address pipe0' register address */          
#define RX_ADDR_P0							0x0A
/* 'RX address pipe1' register address */
#define RX_ADDR_P1							0x0B
/* 'RX address pipe2' register address */
#define RX_ADDR_P2							0x0C
/* 'RX address pipe3' register address */
#define RX_ADDR_P3							0x0D
/* 'RX address pipe4' register address */
#define RX_ADDR_P4							0x0E
/* 'RX address pipe5' register address */
#define RX_ADDR_P5							0x0F
/* 'TX address' register address */
#define TX_ADDR								0x10
/* 'RX payload width, pipe0' register address */
#define RX_PW_P0							0x11
/* 'RX payload width, pipe1' register address */
#define RX_PW_P1							0x12
/* 'RX payload width, pipe2' register address */
#define RX_PW_P2							0x13
/* 'RX payload width, pipe3' register address */
#define RX_PW_P3							0x14
/* 'RX payload width, pipe4' register address */
#define RX_PW_P4							0x15
/* 'RX payload width, pipe5' register address */
#define RX_PW_P5							0x16
/* 'FIFO Status Register' register address */
#define FIFO_STATUS							0x17

/***********************************************************************************************/
/************************************* END REGISTER MAPPING ************************************/
/***********************************************************************************************/

/***************************** Configuration Register *****************************/
/* Mask interrupt caused by RX_DR */
#define MASK_RX_DR_Y						(0<<6)
#define MASK_RX_DR_N						(1<<6)
/* Mask interrupt caused by TX_DS */
#define MASK_TX_DS_Y						(0<<5)
#define MASK_TX_DS_N						(1<<5)
/* Mask interrupt caused by MAX_RT */
#define MASK_MAX_RT_Y						(0<<4)
#define MASK_MAX_RT_N						(1<<4)
/* Enable CRC. Forced high if one of the bits in the EN_AA is high */
#define EN_CRC_ENABLE						(1<<3)
#define EN_CRC_DISABLE						(0<<3)
/* CRC encoding scheme */
#define CRCO_1Byte							(0<<2)
#define CRCO_2Bytes							(1<<2)
/* power control */
#define POWER_UP							(1<<1)
#define POWER_DOWN							(0<<1)
/* RX/TX control */
#define PRIM_RX_PRX							1
#define PRIM_RX_PTX							0

/* Transmission mode */
#define RX_MODE								0x0f
#define TX_MODE								0x0e

/******************************** Auto Acknowledgment ********************************/
#define AUTO_ACK_ENAA_P0					0x01
#define AUTO_ACK_ENAA_P1					0x02
#define AUTO_ACK_ENAA_P2					0x04
#define AUTO_ACK_ENAA_P3					0x08
#define AUTO_ACK_ENAA_P4					0x10
#define AUTO_ACK_ENAA_P5					0x20

/******************************** Enabled RX Addresses *******************************/
#define EN_RXADDR_P0						0x01
#define EN_RXADDR_P1						0x02
#define EN_RXADDR_P2						0x04
#define EN_RXADDR_P3						0x08
#define EN_RXADDR_P4						0x10
#define EN_RXADDR_P5						0x20

/****************************** Setup of Address Widths ******************************/
#define SETUP_AW_ILLEGAL					0x00
#define SETUP_AW_3Bytes						0x01
#define SETUP_AW_4Bytes						0x02
#define SETUP_AW_5Bytes						0x03/* LSByte is used if address width is below 5 bytes */

/************************* Setup of Automatic Retransmission *************************/
/* Auto Retransmit Delay */
#define SETUP_RETR_ARD_250us				(0x00<<4)
#define SETUP_RETR_ARD_500us				(0x01<<4)
#define SETUP_RETR_ARD_750us				(0x02<<4)
#define SETUP_PETR_ARD_1000us				(0x03<<4)
#define SETUP_PETR_ARD_1250us				(0x04<<4)
#define SETUP_PETR_ARD_1500us				(0x05<<4)
#define SETUP_PETR_ARD_1750us				(0x06<<4)
#define SETUP_PETR_ARD_2000us				(0x07<<4)
#define SETUP_PETR_ARD_2250us				(0x08<<4)
#define SETUP_PETR_ARD_2500us				(0x09<<4)
#define SETUP_PETR_ARD_2750us				(0x0a<<4)
#define SETUP_PETR_ARD_3000us				(0x0b<<4)
#define SETUP_PETR_ARD_3250us				(0x0c<<4)
#define SETUP_PETR_ARD_3500us				(0x0d<<4)
#define SETUP_PETR_ARD_3750us				(0x0e<<4)
#define SETUP_PETR_ARD_4000us				(0x0f<<4)

/* Auto Retransmit Count */
#define SETUP_PETR_ARC_0					0x00
#define SETUP_PETR_ARC_1					0x01
#define SETUP_PETR_ARC_2					0x02
#define SETUP_PETR_ARC_3					0x03
#define SETUP_PETR_ARC_4					0x04
#define SETUP_PETR_ARC_5					0x05
#define SETUP_PETR_ARC_6					0x06
#define SETUP_PETR_ARC_7					0x07
#define SETUP_PETR_ARC_8					0x08
#define SETUP_PETR_ARC_9					0x09
#define SETUP_PETR_ARC_10					0x0a
#define SETUP_PETR_ARC_11					0x0b
#define SETUP_PETR_ARC_12					0x0c
#define SETUP_PETR_ARC_13					0x0d
#define SETUP_PETR_ARC_14					0x0e
#define SETUP_PETR_ARC_15					0x0f

/* RF Channel ??? */
#define RF_Channel_2_4G						0

/********************************* RF Setup Register *********************************/
/* Force PLL lock signal. Only used in test */
#define PLL_LOCK_ALLOW						(1<<4)
#define PLL_LOCK_FORBID						(0<<4)

/* Air Data Rate */
#define RF_DR_1Mbps							(0<<3)
#define RF_DR_2Mbps							(1<<3)

/* Set RF output power in TX mode */
#define RF_PWR_L18dBm						(0x00<<1)
#define RF_PWR_L12dBm						(0x01<<1)
#define RF_PWR_L6dBm						(0x02<<1)
#define RF_PWR_L0dBm						(0x03<<1)

/* Setup LNA gain */
#define LNA_HCURR_Y							0
#define LAN_HCURR_N							1

/* RF SETUP VALUE */
#define RF_DEFAULT							0x0f

/*************************** Number of bytes in RX payload ***************************/
/* RX_PW_P0 */
#define RX_PW_P0_ILLEGAL					0x00
#define RX_PW_P0_1							0x01
#define RX_PW_P0_2							0x02
#define RX_PW_P0_3							0x03
#define RX_PW_P0_4							0x04
#define RX_PW_P0_5							0x05
#define RX_PW_P0_6							0x06
#define RX_PW_P0_7							0x07
#define RX_PW_P0_8							0x08
#define RX_PW_P0_9							0x09
#define RX_PW_P0_10							0x0a
#define RX_PW_P0_11							0x0b
#define RX_PW_P0_12							0x0c
#define RX_PW_P0_13							0x0d
#define RX_PW_P0_14							0x0e
#define RX_PW_P0_15							0x0f
#define RX_PW_P0_16							0x10
#define RX_PW_P0_17							0x11
#define RX_PW_P0_18							0x12
#define RX_PW_P0_19							0x13
#define RX_PW_P0_20							0x14
#define RX_PW_P0_21							0x15
#define RX_PW_P0_22							0x16
#define RX_PW_P0_23							0x17
#define RX_PW_P0_24							0x18
#define RX_PW_P0_25							0x19
#define RX_PW_P0_26							0x1a
#define RX_PW_P0_27							0x1b
#define RX_PW_P0_28							0x1c
#define RX_PW_P0_29							0x1d
#define RX_PW_P0_30							0x1e
#define RX_PW_P0_31							0x1f
#define RX_PW_P0_32							0x20
/* RX_PW_P1 */
#define RX_PW_P1_ILLEGAL					0x00
#define RX_PW_P1_1							0x01
#define RX_PW_P1_2							0x02
#define RX_PW_P1_3							0x03
#define RX_PW_P1_4							0x04
#define RX_PW_P1_5							0x05
#define RX_PW_P1_6							0x06
#define RX_PW_P1_7							0x07
#define RX_PW_P1_8							0x08
#define RX_PW_P1_9							0x09
#define RX_PW_P1_10							0x0a
#define RX_PW_P1_11							0x0b
#define RX_PW_P1_12							0x0c
#define RX_PW_P1_13							0x0d
#define RX_PW_P1_14							0x0e
#define RX_PW_P1_15							0x0f
#define RX_PW_P1_16							0x10
#define RX_PW_P1_17							0x11
#define RX_PW_P1_18							0x12
#define RX_PW_P1_19							0x13
#define RX_PW_P1_20							0x14
#define RX_PW_P1_21							0x15
#define RX_PW_P1_22							0x16
#define RX_PW_P1_23							0x17
#define RX_PW_P1_24							0x18
#define RX_PW_P1_25							0x19
#define RX_PW_P1_26							0x1a
#define RX_PW_P1_27							0x1b
#define RX_PW_P1_28							0x1c
#define RX_PW_P1_29							0x1d
#define RX_PW_P1_30							0x1e
#define RX_PW_P1_31							0x1f
#define RX_PW_P1_32							0x20
/* RX_PW_P2 */
#define RX_PW_P2_ILLEGAL					0x00
#define RX_PW_P2_1							0x01
#define RX_PW_P2_2							0x02
#define RX_PW_P2_3							0x03
#define RX_PW_P2_4							0x04
#define RX_PW_P2_5							0x05
#define RX_PW_P2_6							0x06
#define RX_PW_P2_7							0x07
#define RX_PW_P2_8							0x08
#define RX_PW_P2_9							0x09
#define RX_PW_P2_10							0x0a
#define RX_PW_P2_11							0x0b
#define RX_PW_P2_12							0x0c
#define RX_PW_P2_13							0x0d
#define RX_PW_P2_14							0x0e
#define RX_PW_P2_15							0x0f
#define RX_PW_P2_16							0x10
#define RX_PW_P2_17							0x11
#define RX_PW_P2_18							0x12
#define RX_PW_P2_19							0x13
#define RX_PW_P2_20							0x14
#define RX_PW_P2_21							0x15
#define RX_PW_P2_22							0x16
#define RX_PW_P2_23							0x17
#define RX_PW_P2_24							0x18
#define RX_PW_P2_25							0x19
#define RX_PW_P2_26							0x1a
#define RX_PW_P2_27							0x1b
#define RX_PW_P2_28							0x1c
#define RX_PW_P2_29							0x1d
#define RX_PW_P2_30							0x1e
#define RX_PW_P2_31							0x1f
#define RX_PW_P2_32							0x20
/* RX_PW_P3 */
#define RX_PW_P3_ILLEGAL					0x00
#define RX_PW_P3_1							0x01
#define RX_PW_P3_2							0x02
#define RX_PW_P3_3							0x03
#define RX_PW_P3_4							0x04
#define RX_PW_P3_5							0x05
#define RX_PW_P3_6							0x06
#define RX_PW_P3_7							0x07
#define RX_PW_P3_8							0x08
#define RX_PW_P3_9							0x09
#define RX_PW_P3_10							0x0a
#define RX_PW_P3_11							0x0b
#define RX_PW_P3_12							0x0c
#define RX_PW_P3_13							0x0d
#define RX_PW_P3_14							0x0e
#define RX_PW_P3_15							0x0f
#define RX_PW_P3_16							0x10
#define RX_PW_P3_17							0x11
#define RX_PW_P3_18							0x12
#define RX_PW_P3_19							0x13
#define RX_PW_P3_20							0x14
#define RX_PW_P3_21							0x15
#define RX_PW_P3_22							0x16
#define RX_PW_P3_23							0x17
#define RX_PW_P3_24							0x18
#define RX_PW_P3_25							0x19
#define RX_PW_P3_26							0x1a
#define RX_PW_P3_27							0x1b
#define RX_PW_P3_28							0x1c
#define RX_PW_P3_29							0x1d
#define RX_PW_P3_30							0x1e
#define RX_PW_P3_31							0x1f
#define RX_PW_P3_32							0x20
/* RX_PW_P4 */
#define RX_PW_P4_ILLEGAL					0x00
#define RX_PW_P4_1							0x01
#define RX_PW_P4_2							0x02
#define RX_PW_P4_3							0x03
#define RX_PW_P4_4							0x04
#define RX_PW_P4_5							0x05
#define RX_PW_P4_6							0x06
#define RX_PW_P4_7							0x07
#define RX_PW_P4_8							0x08
#define RX_PW_P4_9							0x09
#define RX_PW_P4_10							0x0a
#define RX_PW_P4_11							0x0b
#define RX_PW_P4_12							0x0c
#define RX_PW_P4_13							0x0d
#define RX_PW_P4_14							0x0e
#define RX_PW_P4_15							0x0f
#define RX_PW_P4_16							0x10
#define RX_PW_P4_17							0x11
#define RX_PW_P4_18							0x12
#define RX_PW_P4_19							0x13
#define RX_PW_P4_20							0x14
#define RX_PW_P4_21							0x15
#define RX_PW_P4_22							0x16
#define RX_PW_P4_23							0x17
#define RX_PW_P4_24							0x18
#define RX_PW_P4_25							0x19
#define RX_PW_P4_26							0x1a
#define RX_PW_P4_27							0x1b
#define RX_PW_P4_28							0x1c
#define RX_PW_P4_29							0x1d
#define RX_PW_P4_30							0x1e
#define RX_PW_P4_31							0x1f
#define RX_PW_P4_32							0x20
/* RX_PW_P5 */
#define RX_PW_P5_ILLEGAL					0x00
#define RX_PW_P5_1							0x01
#define RX_PW_P5_2							0x02
#define RX_PW_P5_3							0x03
#define RX_PW_P5_4							0x04
#define RX_PW_P5_5							0x05
#define RX_PW_P5_6							0x06
#define RX_PW_P5_7							0x07
#define RX_PW_P5_8							0x08
#define RX_PW_P5_9							0x09
#define RX_PW_P5_10							0x0a
#define RX_PW_P5_11							0x0b
#define RX_PW_P5_12							0x0c
#define RX_PW_P5_13							0x0d
#define RX_PW_P5_14							0x0e
#define RX_PW_P5_15							0x0f
#define RX_PW_P5_16							0x10
#define RX_PW_P5_17							0x11
#define RX_PW_P5_18							0x12
#define RX_PW_P5_19							0x13
#define RX_PW_P5_20							0x14
#define RX_PW_P5_21							0x15
#define RX_PW_P5_22							0x16
#define RX_PW_P5_23							0x17
#define RX_PW_P5_24							0x18
#define RX_PW_P5_25							0x19
#define RX_PW_P5_26							0x1a
#define RX_PW_P5_27							0x1b
#define RX_PW_P5_28							0x1c
#define RX_PW_P5_29							0x1d
#define RX_PW_P5_30							0x1e
#define RX_PW_P5_31							0x1f
#define RX_PW_P5_32							0x20
/* Data Width */
#define PLOAD_WIDTH_32						32

/************************************** STATUS **************************************/
#define MAX_TX  		0x10
#define TX_OK   		0x20
#define RX_OK   		0x40

/** @defgroup NRF24L01_Exported_Macros
  */
#define NRF24L01_CSN_LOW()		NRF24L01_CSN_GPIO_PORT->BSRRH = NRF24L01_CSN_PIN
#define NRF24L01_CSN_HIGH()		NRF24L01_CSN_GPIO_PORT->BSRRL = NRF24L01_CSN_PIN

#define NRF24L01_CE_LOW()		NRF24L01_CE_GPIO_PORT->BSRRH = NRF24L01_CE_PIN
#define NRF24L01_CE_HIGH()		NRF24L01_CE_GPIO_PORT->BSRRL = NRF24L01_CE_PIN

#define NRF24L01_IRQ_STATUS()	HAL_GPIO_ReadPin(NRF24L01_IRQ_GPIO_PORT, NRF24L01_IRQ_PIN)

void NRF24L01_Init(NRF24L01_InitTypeDef *NRF24L01_InitStruct, IRQMode_TypeDef IRQ_Mode);
void NRF24L01_RX_Mode(void);
void NRF24L01_TX_Mode(void);
uint8_t NRF24L01_TX_Packet(uint8_t* pBuffer);
uint8_t NRF24L01_RX_Packet(uint8_t* pBuffer);
uint8_t NRF24L01_SPI_RW_Reg(uint8_t Reg, uint8_t Value);
uint8_t NRF24L01_SPI_Read_Reg(uint8_t Reg);
void NRF24L01_Write(uint8_t WriteAddr, uint8_t* pBuffer, uint16_t NumByteToWrite);
void NRF24L01_Read(uint8_t ReadAddr, uint8_t* pBuffer, uint16_t NumByteToRead);

uint32_t NRF24L01_TIMEOUT_UserCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __NRF24L01_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
