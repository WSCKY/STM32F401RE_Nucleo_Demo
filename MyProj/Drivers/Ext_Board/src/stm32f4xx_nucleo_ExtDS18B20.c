/******************************************************************************
  * @file    stm32f4xx_nucleo_ExtDS18B20.c
  * @author  '^_^'
  * @version V0.0.0
  * @date    30-September-2014
  * @brief   This file includes the driver for ds18b20 temperature sensor.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_nucleo_ExtDS18B20.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void DS18B20_RST(void);
static void DS18B20_Start(void);
static uint8_t DS18B20_Check(void);
static uint8_t DS18B20_Read_Bit(void);
static uint8_t DS18B20_Read_Byte(void);
static void DS18B20_Write_Byte(uint8_t dat);
static void DS_Delay(uint8_t Delay);

/**
  * @brief  Init ds18b20 hardware.
  * @param  None.
  * @retval None.
  */
uint8_t DS18B20_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	DS18B20_GPIO_CLK_ENABLE();

	/* Configure the DS18B20 SDA pin */
	GPIO_InitStruct.Pin = DS18B20_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(DS18B20_GPIO_PORT, &GPIO_InitStruct);

	DS18B20_HIGH();
	DS18B20_RST();
	return DS18B20_Check();
}

/**
  * @brief  Reset ds18b20.
  * @param  None.
  * @retval None.
  */
static void DS18B20_RST(void)
{
	DS18B20_LOW();
	TIM_Delay_10us(50);
	DS18B20_HIGH();
	TIM_Delay_10us(2);
}

/**
  * @brief  Check the ds18b20.
  * @param  None.
  * @retval 0->have; 1->None.
  */
static uint8_t DS18B20_Check(void)
{
	uint8_t retry = 0;
	DS18B20_GPIO_MODE_IN();
	while(DS18B20_ReadPin() && retry < 20)
	{
		retry ++;
		TIM_Delay_10us(1);
	}
	if(retry >= 20)
	{
		DS18B20_GPIO_MODE_OUT();
		return 1;
	}
	else retry = 0;
	while(!DS18B20_ReadPin() && retry < 24)
	{
		retry ++;
		TIM_Delay_10us(1);
	}
	if(retry >= 24)
	{
		DS18B20_GPIO_MODE_OUT();
		return 1;
	}
	DS18B20_GPIO_MODE_OUT();
	return 0;
}

/**
  * @brief  Read a bit from ds18b20.
  * @param  None.
  * @retval 0->low; 1->high.
  */
static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t data = 0;
	DS18B20_LOW();
	DS_Delay(120);
	DS18B20_HIGH();
	TIM_Delay_10us(1);
	DS18B20_GPIO_MODE_IN();
	if(DS18B20_ReadPin()) data ++;
	DS18B20_GPIO_MODE_OUT();
	TIM_Delay_10us(5);
	return data;
}

/**
  * @brief  Read one byte from ds18b20.
  * @param  None.
  * @retval byte read from ds18b20.
  */
static uint8_t DS18B20_Read_Byte(void)
{
    uint8_t index, j, dat;
    dat = 0;
	for (index = 1; index <= 8; index ++)
	{
        j = DS18B20_Read_Bit();
        dat=(j << 7) | (dat >> 1);
    }
    return dat;
}

/**
  * @brief  write one byte to ds18b20.
  * @param  the byte to write to ds18b20.
  * @retval None.
  */
static void DS18B20_Write_Byte(uint8_t dat)
{
    uint8_t index;
    for (index = 1; index <= 8; index ++)
	{
        if(dat & 0x01)//write '1'
        {
            DS18B20_LOW();
            DS_Delay(200);
            DS18B20_HIGH();
            TIM_Delay_10us(6);
        }
        else//write '0'
        {
            DS18B20_LOW();
            TIM_Delay_10us(6);
            DS18B20_HIGH();
            DS_Delay(200);
        }
		dat >>= 1;
    }
}

/**
  * @brief  Start convert temperature.
  * @param  None.
  * @retval None.
  */
static void DS18B20_Start(void)
{
    DS18B20_RST();
	DS18B20_Check();
    DS18B20_Write_Byte(0xcc);
    DS18B20_Write_Byte(0x44);
}

/**
  * @bruef  Get temperature from the ds18b20.
  * @param  None.
  * @retval temperature.
  */
float DS18B20_Get_Temp(void)
{
    uint8_t temp, TL, TH;
	short tem;
	float temperature;
    DS18B20_Start();
    DS18B20_RST();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);
    DS18B20_Write_Byte(0xbe);
    TL = DS18B20_Read_Byte();
    TH = DS18B20_Read_Byte();
    if(TH > 7)
    {
        TH = ~TH;
        TL = ~TL;
        temp = 0;
    }else temp = 1;
    tem = TH;
    tem <<= 8;
    tem += TL;
    temperature = (float)(tem * 0.0625);
	if(temp) return temperature;
	else return -temperature;
}

/**
  * @brief  Insert a short delay time.
  * @param  specific the data to decrease.
  * @retval None
  */
static void DS_Delay(uint8_t Delay)
{
	while(Delay --);
}

/*************************************** END OF FILE ***************************************/
