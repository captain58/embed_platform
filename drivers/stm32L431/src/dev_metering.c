/******************************************************************************
FileName		: dev_metering.c
Description		: Ë«Âö³å¼ÆÁ¿
Version			: Ver1.0
Author			: Leayud
Created Date	: 2019-01-07 13:33
MCU				: FM33A048
Modified		: Leayud
Modified Date	: 2019-11-20 13:45:04

Copyright By CHINT Instrument Co.,Ltd.
E-mail		: 26885251@qq.com
******************************************************************************/

#include "sys.h"
#include "hal.h"
#include "bsp.h"
//#include "define.h"
//#include "WOSsys.h"
#include "define.h"

uint32_t g_hTotalPos = 0;

void DEV_MeteringGpioInit(void)
{
	HAL_GPIO_PinConfig(GPIOC, 4, 0, 0, 0);
	HAL_GPIO_EInt_Cfg(METER_SENSOR_CH_A, GPIOC, 4, 1, DEV_Metering_CHA_IRQ, NULL);
	HAL_GPIO_PinConfig(GPIOC, 5, 0, 0, 0);
	HAL_GPIO_EInt_Cfg(METER_SENSOR_CH_B, GPIOC, 5, 1, DEV_Metering_CHB_IRQ, NULL);
	// Led Debug
	HAL_GPIO_SetPinState(GPIOC, 7, 1);
	HAL_GPIO_SetPinState(GPIOC, 6, 1);
	HAL_GPIO_PinConfig(GPIOC, 7, 0, 0, 2);
	HAL_GPIO_PinConfig(GPIOC, 6, 0, 0, 2);
}

void DEV_Metering_CHA_IRQ(void * para)
{
	if ((g_hTotalPos&0x01) == 0)
	{
		g_hTotalPos |= 0x01;
	}
	HAL_GPIO_SetPinState(GPIOC, 7, 1);
	HAL_GPIO_SetPinState(GPIOC, 6, 0);
}

void DEV_Metering_CHB_IRQ(void * para)
{
	if ((g_hTotalPos&0x01) != 0)
	{
		g_hTotalPos ++;
		//SysSndMsg(D_ENY_MDL_NO, 0);
		SysSndMsg(9, 0);
	}
	HAL_GPIO_SetPinState(GPIOC, 6, 1);
	HAL_GPIO_SetPinState(GPIOC, 7, 0);
}
