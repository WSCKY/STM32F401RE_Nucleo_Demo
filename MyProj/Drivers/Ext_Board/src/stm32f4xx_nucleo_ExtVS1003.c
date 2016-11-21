/**
  ******************************************************************************
  * @file    stm32f4xx_nucleo_ExtVS1003.c
  * @author  '^_^'
  * @version V0.0.0
  * @date    9-October-2014
  * @brief   This file provides driver of VS1003.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_ExtVS1003.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE                 ((uint8_t)0xFF)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t VS1003Timeout = VS1003_TIMEOUT;
/* SPI handler declaration */
static SPI_HandleTypeDef hVS1003_Spi;
uint32_t VSSpixTimeout = VS1003_TIMEOUT; /*<! Value of Timeout when SPI communication fails */

uint8_t StartSineTest[8] = {0x53, 0xef, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t StopSineTest[8] = {0x45, 0x78, 0x69, 0x74, 0x00, 0x00, 0x00, 0x00};
uint8_t RAMTestBuff[8] = {0x4d, 0xea, 0x6d, 0x54, 0x00, 0x00, 0x00, 0x00};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void         SPIx_Init(void);
static void         SPIx_Error(void);
static void         SPIx_MspInit(SPI_HandleTypeDef *hspi);
static uint8_t      VS1003_SendByte(uint8_t byte);

/**
  * @brief  Set VS1003 Initialization.
  * @param  *VS1003_InitStruct: pointer to a VS1003_Config_TypeDef structure 
  *         that contains the configuration setting for the VS1003.
  * @param  REQ_Mode: Specifies the Data Request Pin's mode.
  * @retval None
  */
void VS1003_Init(VS1003_InitTypeDef *VS1003_InitStruct, REQMode_TypeDef REQ_Mode)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Configure the other pins */
	/* Enable the GPIO_XCS Clock */
	VS1003_XCS_GPIO_CLK_ENABLE();
	/* Enable the GPIO_XDCS Clock */
	VS1003_XDCS_GPIO_CLK_ENABLE();
	/* Enable the GPIO_RESET Clock */
	VS1003_RESET_GPIO_CLK_ENABLE();
	/* Enable the GPIO_DREQ Clock */
	VS1003_DREQ_GPIO_CLK_ENABLE();
	__SYSCFG_CLK_ENABLE();

	/* Configure the GPIO_XCS pin */
	GPIO_InitStruct.Pin = VS1003_XCS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(VS1003_XCS_GPIO_PORT, &GPIO_InitStruct);
	/* Configure the GPIO_XDCS pin */
	GPIO_InitStruct.Pin = VS1003_XDCS_PIN;
	HAL_GPIO_Init(VS1003_XDCS_GPIO_PORT, &GPIO_InitStruct);
	/* Configure the GPIO_RESET pin */
	GPIO_InitStruct.Pin = VS1003_RESET_PIN;
	HAL_GPIO_Init(VS1003_RESET_GPIO_PORT, &GPIO_InitStruct);
	/* Configure the GPIO_DREQ pin */
	GPIO_InitStruct.Pin = VS1003_DREQ_PIN;
	if(REQ_Mode == REQ_MODE_GPIO)
	{
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(VS1003_DREQ_GPIO_PORT, &GPIO_InitStruct);
	}
	else
	{
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		HAL_GPIO_Init(VS1003_DREQ_GPIO_PORT, &GPIO_InitStruct);
		/* Enable and set Data request EXTI Interrupt to the lowest priority */
		HAL_NVIC_SetPriority((IRQn_Type)VS1003_DREQ_EXIT_IRQn, 0x0F, 0x00);
		HAL_NVIC_EnableIRQ((IRQn_Type)VS1003_DREQ_EXIT_IRQn);
	}

	SPIx_Init();

	VS1003_HW_RESET();
	VS1003_SW_RESET();
	VS1003_Set_Volume(VS1003_InitStruct->Left_Chan_VOL,
						VS1003_InitStruct->Right_Chan_VOL);
	VS1003_Set_Bass(VS1003_InitStruct->Bass_Freq,
					VS1003_InitStruct->Bass,
					VS1003_InitStruct->Treble_Freq,
					VS1003_InitStruct->Treble);
	VS1003_Set_SamRat_Stereo(VS1003_InitStruct->Sample_Rate,
								VS1003_InitStruct->Stereo);
	while(!VS1003_DREQ_STATUS());
}

/**
  * @brief  set volume
  * @param  LeftVol: volume value for the left channel.
  *          This parameter should be: 0..254.
  * @param  RightVol: volume value for the right channel.
  *          This parameter should be: 0..254.
  * @retval None
  */
void VS1003_Set_Volume(uint8_t LeftVol, uint8_t RightVol)
{
	uint16_t Volume;
	Volume = 254 - LeftVol;
	Volume <<= 8;
	Volume += (254 - RightVol);
	SPI_WriteDeviceRegister(SCI_VOL, Volume);
}

/**
  * @brief  Set Built-in bass/treble enhancer.
  * @param  Bass_Freq: Lower limit frequency in 10 Hz steps (2..15)
  *          This parameter should be: 2..15.(uint: 10 Hz)
  * @param  Bass: Bass Enhancement in 1 dB steps (0..15, 0 = off).
  *          This parameter should be: 0..15.(uint: 1 dB)
  * @param  Treble_Freq: Lower limit frequency in 1000 Hz steps (0..15).
  *          This parameter should be: 1..15.(uint: KHz)
  * @param  Treble: Treble Control in 1.5 dB steps (-8..7, 0 = off).
  *          This parameter should be: 0..15.(uint: 1.5 dB)
  * @retval None
  */
void VS1003_Set_Bass(uint8_t Bass_Freq, uint8_t Bass, uint8_t Treble_Freq, uint8_t Treble)
{
    uint16_t bass_set = 0;
    int8_t temp = 0;   	 
	if(Treble == 0)
		temp = 0;
	else if(Treble > 8)
		temp = Treble - 8;
 	else
		temp = Treble - 9;  
	bass_set = temp & 0X0f;
	bass_set <<= 4;
	bass_set += Treble_Freq & 0xf;
	bass_set <<= 4;
	bass_set += Bass & 0xf;
	bass_set <<= 4;
	bass_set += Bass_Freq & 0xf;
	SPI_WriteDeviceRegister(SCI_BASS, bass_set);
}

/**
  * @brief  set current sample rate and stereo.
  * @param  Sample_Rate: set the current sample rate.
  * @param  Stereo: ON for mono data and OFF for stereo.
  * @retval None
  */
void VS1003_Set_SamRat_Stereo(uint16_t Sample_Rate, STEREO_SwitchDef Stereo)
{
	SPI_WriteDeviceRegister(SCI_AUDATA, Sample_Rate + Stereo);
}

/**
  * @brief  send music data to vs1003.
  * @param  *DataBuffer: pointer to buffer contains data to send.
  * @retval send resualt.
  */
uint8_t VS1003_Send_MusicData(uint8_t *DataBuffer)
{
	if(VS1003_DREQ_STATUS())
	{
		VS1003_XDCS_LOW();

		if(HAL_SPI_Transmit(&hVS1003_Spi, (uint8_t *)DataBuffer, 32, 5000) != HAL_OK)
		{
			return VS1003_ERROR_UserCallback();
		}

		VS1003_XDCS_HIGH();
		return 0;
	}
	return 1;
}

/**
  * @brief  play music prepare.
  * @param  None
  * @retval None
  */
void VS1003_Play_Prepare(void)
{
	uint16_t temp, temp0, temp1;
	uint8_t temp_buf[32];
	for(temp = 0; temp < 32; temp ++) temp_buf[temp] = 0;
	SPI_ReadDeviceRegister(SCI_MODE, &temp);
	temp |= (1<<3);
	SPI_WriteDeviceRegister(SCI_MODE, temp);
	for(temp = 0; temp < 2048; )
	{
		if(!VS1003_Send_MusicData(temp_buf))
			temp += 32;
	}
	SPI_ReadDeviceRegister(SCI_HDAT0, &temp0);
	SPI_ReadDeviceRegister(SCI_HDAT1, &temp1);
	if(temp0 + temp1)
	{
		VS1003_HW_RESET();
		VS1003_SW_RESET();
	}
}

/**
  * @brief  Reset VS1003 Decode time.
  * @param  None
  * @retval None
  */
void VS1003_Reset_DecodeTime(void)
{
	/* operate twice */
	SPI_WriteDeviceRegister(SCI_DECODE_TIME, 0x0000);
	SPI_WriteDeviceRegister(SCI_DECODE_TIME, 0x0000);
}

/**
  * @brief  Get decode time.
  * @param  None
  * @retval decode time.
  */
uint16_t VS1003_Get_DecodeTime(void)
{
	uint16_t decode_time;
	SPI_ReadDeviceRegister(SCI_DECODE_TIME, &decode_time);
	return decode_time;
}

/**
  * @brief  Sine Test.
  * @param  TestValue: sine test parameter.
  *          The frequency of the sine to be output can now be calculated from F=Fs*S/128.
  * @param  TestTime: test time.
  * @retval None
  */
uint8_t VS1003_Sine_Test(uint8_t TestValue, uint32_t TestTime)
{
	VS1003_HW_RESET();
	/* set volume */
	SPI_WriteDeviceRegister(SCI_VOL, 0x2020);
	/* test mode */
	SPI_WriteDeviceRegister(SCI_MODE, SDI_TEST_Allow|SDINEW_YES|SOFT_SET|OUTOFWAV_NO);
	VS1003Timeout = VS1003_TIMEOUT;
	while(!VS1003_DREQ_STATUS())
	{
		if((VS1003Timeout --) == 0) return VS1003_ERROR_UserCallback();
	}
	VS1003_XDCS_LOW();
	StartSineTest[3] = TestValue;
	HAL_SPI_Transmit(&hVS1003_Spi, (uint8_t *)StartSineTest, 8, 5000);
	HAL_Delay(TestTime);
	VS1003_XDCS_HIGH();
	VS1003_XDCS_LOW();
	HAL_SPI_Transmit(&hVS1003_Spi, (uint8_t *)StopSineTest, 8, 5000);
	HAL_Delay(20);
	VS1003_XDCS_HIGH();
	return 0;
}

/**
  * @brief  RAM Test.
  * @param  None
  * @retval Test resualt.
  */
uint8_t VS1003_RAM_Test(void)
{
	uint16_t regit = 0;
	VS1003_HW_RESET();
	/* test mode on */
	SPI_WriteDeviceRegister(SCI_MODE, SDI_TEST_Allow|SDINEW_YES|SOFT_SET|OUTOFWAV_NO);
	VS1003Timeout = VS1003_TIMEOUT;
	while(!VS1003_DREQ_STATUS())
	{
		if((VS1003Timeout --) == 0) return VS1003_ERROR_UserCallback();
	}
	VS1003_XDCS_LOW();
	HAL_SPI_Transmit(&hVS1003_Spi, (uint8_t *)RAMTestBuff, 8, 5000);
	HAL_Delay(50);
	VS1003_XDCS_HIGH();
	SPI_ReadDeviceRegister(SCI_HDAT0, &regit);
	if(regit == 0x807f)
		return 0;
	return 1;
}

/**
  * @brief  reset VS1003 by hardware.
  * @param  None
  * @retval reset status.
  */
uint8_t VS1003_HW_RESET(void)
{
	VS1003_RESET_LOW();
	/* Insert delay 15 ms */
	HAL_Delay(15);
	VS1003_XDCS_HIGH();
	VS1003_XCS_HIGH();
	VS1003_RESET_HIGH();
	/* Insert delay 15 ms */
	HAL_Delay(15);
	VS1003Timeout = VS1003_TIMEOUT;
	while(!VS1003_DREQ_STATUS())
	{
		if((VS1003Timeout --) == 0) return VS1003_ERROR_UserCallback();
	}
	return 0;
}

/**
  * @brief  reset VS1003 by software.
  * @param  None
  * @retval reset status.
  */
uint8_t VS1003_SW_RESET(void)
{
	VS1003_SendByte(DUMMY_BYTE);
	SPI_WriteDeviceRegister(SCI_MODE, 0x0800);
	VS1003Timeout = VS1003_TIMEOUT;
	while(!VS1003_DREQ_STATUS())
	{
		if((VS1003Timeout --) == 0) return VS1003_ERROR_UserCallback();
	}
	SPI_WriteDeviceRegister(SCI_CLOCKF, 0x9800);
	return 0;
}

/**
  * @brief  Writes a value in a register of the device through SPI.
  * @param  RegisterAddr: The target register address.
  * @param  RegisterValue: The target register value to be written.
  * @retval operation status.
  */
uint8_t SPI_WriteDeviceRegister(uint8_t RegisterAddr, uint16_t RegisterValue)
{
//	VS1003Timeout = VS1003_TIMEOUT;
//	while(!VS1003_DREQ_STATUS())
//	{
//		if((VS1003Timeout --) == 0) return VS1003_ERROR_UserCallback();
//	}
	VS1003_XDCS_HIGH();
	VS1003_XCS_LOW();
	VS1003_SendByte(VS1003_WRITE_COMMAND);
	VS1003_SendByte(RegisterAddr);
	VS1003_SendByte(RegisterValue >> 8);
	VS1003_SendByte(RegisterValue);
	VS1003_XCS_HIGH();
	return 0;
}

/**
  * @brief  Write a byte of data to VS1003's RAM.
  * @param  Data: the value to be written.
  * @retval None
  */
void SPI_WriteDeviceData(uint8_t Data)
{
	VS1003_XDCS_LOW();
	VS1003_SendByte(Data);
	VS1003_XDCS_HIGH();
}

/**
  * @brief  Reads a register of the device through SPI.
  * @param  RegisterAddr: The target register address.
  * @param  *RegisterValue: the value read from target register.
  * @retval read status.
  */
uint8_t SPI_ReadDeviceRegister(uint8_t RegisterAddr, uint16_t *RegisterValue)
{
	uint16_t reg_val;
//	VS1003Timeout = VS1003_TIMEOUT;
//	while(!VS1003_DREQ_STATUS())
//	{
//		if((VS1003Timeout --) == 0) return VS1003_ERROR_UserCallback();
//	}
	VS1003_XDCS_HIGH();
	VS1003_XCS_LOW();
	VS1003_SendByte(VS1003_READ_COMMAND);
	VS1003_SendByte(RegisterAddr);
	reg_val = VS1003_SendByte(DUMMY_BYTE);
	reg_val <<= 8;
	reg_val += VS1003_SendByte(DUMMY_BYTE);
	VS1003_XCS_HIGH();
	*RegisterValue = reg_val;
	return 0;
}

/**
  * @brief  Read a halfword of data from vs1003's ram.
  * @param  RamAddr: RAM's address.
  * @retval data read from vs1003's RAM.
  */
uint16_t SPI_ReadDeviceData(uint16_t RamAddr)
{
	uint16_t RamValue;
	SPI_WriteDeviceRegister(SCI_WRAMADDR, RamAddr);
	SPI_ReadDeviceRegister(SCI_WRAM, &RamValue);
	return RamValue;
}

/**
  * @brief  Initializes SPI MSP.
  * @param  None
  * @retval None
  */
static void SPIx_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef  GPIO_InitStruct;  

	/*** Configure the GPIOs ***/  
	/* Enable GPIO clock */
	VS1003_SPI_SCK_GPIO_CLK_ENABLE();
	VS1003_SPI_MISO_MOSI_GPIO_CLK_ENABLE();

	/* Configure SPI SCK */
	GPIO_InitStruct.Pin = VS1003_SPI_SCK_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = VS1003_SPI_SCK_AF;
	HAL_GPIO_Init(VS1003_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);

	/* Configure SPI MISO and MOSI */ 
	GPIO_InitStruct.Pin = VS1003_SPI_MOSI_PIN;
	GPIO_InitStruct.Alternate = VS1003_SPI_MOSI_AF;
	GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
	HAL_GPIO_Init(VS1003_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = VS1003_SPI_MISO_PIN;
	HAL_GPIO_Init(VS1003_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);

	/*** Configure the SPI peripheral ***/ 
	/* Enable SPI clock */
	VS1003_SPI_CLK_ENABLE();
}

/**
  * @brief  Initializes SPI HAL.
  * @param  None
  * @retval None
  */
static void SPIx_Init(void)
{
	if(HAL_SPI_GetState(&hVS1003_Spi) == HAL_SPI_STATE_RESET)
	{
		/* SPI Config */
		hVS1003_Spi.Instance = VS1003_SPI;
		/* SPI baudrate is set to 12,5 MHz maximum (PCLK2/SPI_BaudRatePrescaler = 100/8 = 12,5 MHz) 
			to verify these constraints:
			- SD card SPI interface max baudrate is 25MHz for write/read
			- PCLK2 max frequency is 100 MHz 
		*/ 
		hVS1003_Spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
		hVS1003_Spi.Init.Direction = SPI_DIRECTION_2LINES;
		hVS1003_Spi.Init.CLKPhase = SPI_PHASE_1EDGE;
		hVS1003_Spi.Init.CLKPolarity = SPI_POLARITY_LOW;
		hVS1003_Spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
		hVS1003_Spi.Init.CRCPolynomial = 7;
		hVS1003_Spi.Init.DataSize = SPI_DATASIZE_8BIT;
		hVS1003_Spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
		hVS1003_Spi.Init.NSS = SPI_NSS_SOFT;
		hVS1003_Spi.Init.TIMode = SPI_TIMODE_DISABLED;
		hVS1003_Spi.Init.Mode = SPI_MODE_MASTER;

		SPIx_MspInit(&hVS1003_Spi);
		HAL_SPI_Init(&hVS1003_Spi);
	}
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received from the SPI bus.
  * @param  byte : Byte send.
  * @retval The received byte value.
  */
/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received from the SPI bus.
  * @param  byte : Byte send.
  * @retval The received byte value.
  */
static uint8_t VS1003_SendByte(uint8_t byte)
{
	uint8_t ReceByte;
	if(HAL_SPI_TransmitReceive(&hVS1003_Spi, (uint8_t *)&byte, (uint8_t *)&ReceByte, 1, 5000) != HAL_OK)
	{
		SPIx_Error();
	}
	return ReceByte;
}

/**
  * @brief  SPI error treatment function.
  * @param  None
  * @retval None
  */
static void SPIx_Error(void)
{
	/* De-initialize the SPI communication BUS */
	HAL_SPI_DeInit(&hVS1003_Spi);

	/* Re-Initiaize the SPI communication BUS */
	SPIx_Init();
}

/**
  * @brief  Basic management of the error situation
  * @param  None
  * @retval None
  */
__weak uint8_t VS1003_ERROR_UserCallback(void)
{
	/* Block communication and all processes */
	while(1);
}

/*************************************** END OF FILE ***************************************/
