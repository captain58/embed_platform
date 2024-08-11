/******************************************************************************
FileName		: Dev_disp.c
Description		: LCD显示驱动程序
Version			: Ver1.0
Author			: Leayud
Created Date	: 2019-11-12 15:28:12
MCU				: ARM M0+
Modified		: Leayud
Modified Date	: 2019-11-18 14:39:25

Copyright By CHINT Instrument Co.,Ltd.
E-mail		: 26885251@qq.com
******************************************************************************/

//#include <stdint.h>
////#include "chip.h"
//#include "define.h"
////#include "disp_api.h"
//#include "dev_disp.h"
#ifdef __MODULE__
#include "WOSsys.h"
#endif
#include "sys.h"
#include "hal.h"
#include "bsp.h"
// 液晶显示真值表，BIT0:A, ..., BIT6:G
const uint8_t LCD_DISP_NUM_TABLE[] = 
{
	// 0 ~ 7
	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 
	// 8 ~ F
	0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 
	// 
	0x00,	// ' '
	0x40,	// '-'
};
const uint8_t LCD_DISP_CHAR_TABLE[] = 
{
	// 0x20
	//' '
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	// 0x28
	//                             '-'
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 
	// 0x30 0 ~ 9
	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 
	0x7F, 0x6F, 
	// 0x3A 
	//':'               '='
	0x48, 0x00, 0x00, 0x48, 0x00, 0x00, 0x00, 
	// 0x41 A ~ G
	0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D, 
	// 0x48 H ~ N 'K' 'M'
	0x76, 0x30, 0x1E, 0x49, 0x38, 0x49, 0x37, 
	// 0x4F O ~ T
	0x3F, 0x73, 0x67, 0x50, 0x6D, 0x78, 
	// 0x55 U ~ Z 'W' 'X'
	0x3E, 0x1C, 0x49, 0x49, 0x6E, 0x5B, 
	// 0x5B
	//                       '_'
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 
	// 0x61 a ~ g
	0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x3D, 
	// 0x48 h ~ n 'k' 'm'
	0x76, 0x30, 0x1E, 0x49, 0x38, 0x49, 0x37, 
	// 0x4F o ~ t
	0x3F, 0x73, 0x67, 0x50, 0x6D, 0x78, 
	// 0x55 u ~ z 'w' 'x'
	0x3E, 0x1C, 0x49, 0x49, 0x6E, 0x5B, 
	// 0x7B
	0x00, 0x00, 0x00, 0x00, 0x00, 
};

void HAL_LcdInit(void)
{
	LCD->DF = 0x52;
	LCD->LCDSET = 0x10;
	LCD->LCDDRV = 0x87;
	LCD->LCDBIAS = 0x0E;
	// COM 0~3
	LCD->COM_EN = 0x0F;
	// SEG 0~16, 40~43
	LCD->SEG_EN0 = 0x0001FFFF;
	LCD->SEG_EN1 = 0x00000F00;
	LCD->DISPCTRL = 0xC0;
	//
	HAL_LcdSetValue(LCD_DATARAM_CLEAR);
}

void HAL_LcdDeInit(void)
{
	LCD->DISPCTRL = 0x00;
}

void HAL_LcdSetValue(uint32_t val)
{
	uint32_t *p = (uint32_t *)&LCD->DISPDATA0;
	
	for (uint8_t i=0; i<LCD_CHIP_RAMSIZE; i++)
	{
		p[i] = val;
	}
}

void HAL_LcdOn(void)
{
	LCD->DISPCTRL |= 0x40;
}

void HAL_LcdOff(void)
{
	LCD->DISPCTRL &= ~0x40;
}

void HAL_LcdWrite(uint8_t index, uint8_t val)
{
	uint32_t segmentMsk;
	
	index <<= 1;
	
	// 设置段码显示内容，对应段号A~G
	if (index < 12)
	{
		segmentMsk = ~(0x1800>>index);
		// 清除段码显示内容
		LCD->DISPDATA0 &= segmentMsk;
		LCD->DISPDATA1 &= segmentMsk;
		LCD->DISPDATA2 &= segmentMsk;
		LCD->DISPDATA3 &= segmentMsk;
		
		if (val & 0x01)
		{
			LCD->DISPDATA3 |= (0x1000>>index);
		}
		if (val & 0x02)
		{
			LCD->DISPDATA2 |= (0x0800>>index);
		}
		if (val & 0x04)
		{
			LCD->DISPDATA0 |= (0x0800>>index);
		}
		if (val & 0x08)
		{
			LCD->DISPDATA0 |= (0x1000>>index);
		}
		if (val & 0x10)
		{
			LCD->DISPDATA1 |= (0x1000>>index);
		}
		if (val & 0x20)
		{
			LCD->DISPDATA2 |= (0x1000>>index);
		}
		if (val & 0x40)
		{
			LCD->DISPDATA1 |= (0x0800>>index);
		}
	}
	else if (index == 12)
	{
		segmentMsk = ~(0x01);
		// 清除段码显示内容
		LCD->DISPDATA0 &= segmentMsk;
		LCD->DISPDATA1 &= segmentMsk;
		LCD->DISPDATA2 &= segmentMsk;
		LCD->DISPDATA3 &= segmentMsk;
		LCD->DISPDATA4 &= ~(0x00200200);
		LCD->DISPDATA5 &= ~(0x02);
		
		if (val & 0x01)
		{
			LCD->DISPDATA3 |= 0x01;
		}
		if (val & 0x02)
		{
			LCD->DISPDATA5 |= 0x02;
		}
		if (val & 0x04)
		{
			LCD->DISPDATA4 |= 0x0200;
		}
		if (val & 0x08)
		{
			LCD->DISPDATA0 |= 0x01;
		}
		if (val & 0x10)
		{
			LCD->DISPDATA1 |= 0x01;
		}
		if (val & 0x20)
		{
			LCD->DISPDATA2 |= 0x01;
		}
		if (val & 0x40)
		{
			LCD->DISPDATA4 |= 0x00200000;
		}
	}
	else if (index == 14)
	{
		// 清除段码显示内容
		LCD->DISPDATA4 &= ~(0x00900900);
		LCD->DISPDATA5 &= ~(0x1009);
		
		if (val & 0x01)
		{
			LCD->DISPDATA5 |= 0x1000;
		}
		if (val & 0x02)
		{
			LCD->DISPDATA5 |= 0x08;
		}
		if (val & 0x04)
		{
			LCD->DISPDATA4 |= 0x0800;
		}
		if (val & 0x08)
		{
			LCD->DISPDATA4 |= 0x0100;
		}
		if (val & 0x10)
		{
			LCD->DISPDATA4 |= 0x00100000;
		}
		if (val & 0x20)
		{
			LCD->DISPDATA5 |= 0x01;
		}
		if (val & 0x40)
		{
			LCD->DISPDATA4 |= 0x00800000;
		}
	}
}

void HAL_LcdSetSegment(uint16_t mark)
{
	uint32_t *p = (uint32_t *)&LCD->DISPDATA0;
	
	p[mark>>8] |= (1<<(mark&0x1F));
}

void HAL_LcdSetDots(uint8_t dots)
{
	switch(dots)
	{
		case 1:
			HAL_LcdSetSegment(LCD_SEGMENT_DOTS_BIT1);
			break;
		case 2:
			HAL_LcdSetSegment(LCD_SEGMENT_DOTS_BIT2);
			break;
		case 3:
			HAL_LcdSetSegment(LCD_SEGMENT_DOTS_BIT3);
			break;
		default:
			break;
	}
}

void HAL_LcdSetUnitStBits(EN_DISP_UNITS unit, uint32_t stBits)
{
	// 显示单位
	switch(unit)
	{
		case DISP_UNITS_VOLUME:
			HAL_LcdSetSegment(LCD_SEGMENT_UNIT_M3);
			break;
		case DISP_UNITS_AMOUNT:
			HAL_LcdSetSegment(LCD_SEGMENT_UNIT_RMB);
			break;
		case DISP_UNITS_PRICE:
			HAL_LcdSetSegment(LCD_SEGMENT_UNIT_RMB);
			HAL_LcdSetSegment(LCD_SEGMENT_UNIT_PRICE);
			HAL_LcdSetSegment(LCD_SEGMENT_UNIT_M3);
			break;
		case DISP_UNITS_TEMPERATURE:
			HAL_LcdSetSegment(LCD_SEGMENT_UNIT_TEMP);
			break;
		case DISP_UNITS_PRESSURE:
			HAL_LcdSetSegment(LCD_SEGMENT_UNIT_KPA);
			break;
		case DISP_UNITS_PERCENTAGE:
			HAL_LcdSetSegment(LCD_SEGMENT_UNIT_RATE);
			break;
		default:
			break;
	}
	
	// 显示符号位
	if (stBits & DISP_ST_BIT_LOGO)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_LOGO);
	}
	if (stBits & DISP_ST_BIT_YEAR)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_YEAR);
	}
	if (stBits & DISP_ST_BIT_MONTH)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_MONTH);
	}
	if (stBits & DISP_ST_BIT_TOTAL)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_TOTAL);
	}
	if (stBits & DISP_ST_BIT_REMAIN)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_REMAIN);
	}
	if (stBits & DISP_ST_BIT_PURCHASE)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_PURCHASE);
	}
	if (stBits & DISP_ST_BIT_USED)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_ST_USED);
	}
	if (stBits & DISP_ST_BIT_TEST)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_ST_TEST);
	}
	if (stBits & DISP_ST_BIT_ERROR)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_ST_ERROR);
	}
	if (stBits & DISP_ST_BIT_PROTECT)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_ST_PROTECT);
	}
	if (stBits & DISP_ST_BIT_CLOSE)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_ST_CLOSE);
	}
	if (stBits & DISP_ST_BIT_OPEN)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_ST_OPEN);
	}
	if (stBits & DISP_ST_BIT_BATT_LOW)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_BATT_LOW);
	}
	if (stBits & DISP_ST_BIT_ALARM)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_ALARM);
	}
	if (stBits & DISP_ST_BIT_BATT_CHARGE)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_BATT_CHARGE);
	}
	if (stBits & DISP_ST_BIT_RSSI_S1)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_RSSI_S1);
	}
	if (stBits & DISP_ST_BIT_RSSI_S2)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_RSSI_S1);
		HAL_LcdSetSegment(LCD_SEGMENT_RSSI_S2);
	}
	if (stBits & DISP_ST_BIT_RSSI_S3)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_RSSI_S1);
		HAL_LcdSetSegment(LCD_SEGMENT_RSSI_S2);
		HAL_LcdSetSegment(LCD_SEGMENT_RSSI_S3);
	}
	if (stBits & DISP_ST_BIT_RSSI_S4)
	{
		HAL_LcdSetSegment(LCD_SEGMENT_RSSI_S1);
		HAL_LcdSetSegment(LCD_SEGMENT_RSSI_S2);
		HAL_LcdSetSegment(LCD_SEGMENT_RSSI_S3);
		HAL_LcdSetSegment(LCD_SEGMENT_RSSI_S4);
	}
}

void HAL_LcdWriteHex(uint32_t val, uint8_t num, EN_DISP_UNITS unit, uint32_t stBits)
{
	uint8_t i;
	uint8_t data;
	
	HAL_LcdSetValue(LCD_DATARAM_CLEAR);
	if (num > 8)
	{
		num = 8;
	}
	for (i=0; i<num; i++)
	{
		data = val&0x0F;
		val >>= 4;
		HAL_LcdWrite(7-i, LCD_DISP_NUM_TABLE[data]);
	}
	HAL_LcdSetUnitStBits(unit, stBits);
	
	/*// Test
	HAL_LcdSetSegment(LCD_SEGMENT_LOGO);
	HAL_LcdSetSegment(LCD_SEGMENT_TOTAL);
	HAL_LcdSetSegment(LCD_SEGMENT_UNIT_M3);
	HAL_LcdSetSegment(LCD_SEGMENT_DOTS_BIT1);
	HAL_LcdSetSegment(LCD_SEGMENT_DOTS_BIT2);
	HAL_LcdSetSegment(LCD_SEGMENT_DOTS_BIT3);
	HAL_LcdSetSegment(LCD_SEGMENT_UNIT_RMB);
	HAL_LcdSetSegment(LCD_SEGMENT_UNIT_PRICE);
	HAL_LcdSetSegment(LCD_SEGMENT_UNIT_TEMP);
	HAL_LcdSetSegment(LCD_SEGMENT_UNIT_KPA);
	HAL_LcdSetSegment(LCD_SEGMENT_UNIT_RATE);
	HAL_LcdSetSegment(LCD_SEGMENT_YEAR);
	HAL_LcdSetSegment(LCD_SEGMENT_MONTH);
	HAL_LcdSetSegment(LCD_SEGMENT_REMAIN);
	HAL_LcdSetSegment(LCD_SEGMENT_PURCHASE);
	HAL_LcdSetSegment(LCD_SEGMENT_ST_USED);
	HAL_LcdSetSegment(LCD_SEGMENT_ST_TEST);
	HAL_LcdSetSegment(LCD_SEGMENT_ST_ERROR);
	HAL_LcdSetSegment(LCD_SEGMENT_ST_PROTECT);
	HAL_LcdSetSegment(LCD_SEGMENT_ST_CLOSE);
	HAL_LcdSetSegment(LCD_SEGMENT_ST_OPEN);
	HAL_LcdSetSegment(LCD_SEGMENT_BATT_LOW);
	HAL_LcdSetSegment(LCD_SEGMENT_ALARM);
	HAL_LcdSetSegment(LCD_SEGMENT_BATT_CHARGE);
	HAL_LcdSetSegment(LCD_SEGMENT_RSSI_S1);
	HAL_LcdSetSegment(LCD_SEGMENT_RSSI_S2);
	HAL_LcdSetSegment(LCD_SEGMENT_RSSI_S3);
	HAL_LcdSetSegment(LCD_SEGMENT_RSSI_S4);*/
}

void HAL_LcdWriteDec(int32_t val, uint8_t dots, EN_DISP_UNITS unit, uint32_t stBits)
{
	uint8_t i;
	uint8_t data;
	uint8_t flag = 0;
	
	if (val>99999999 || val<-9999999)
	{
		return ;
	}
	HAL_LcdSetValue(LCD_DATARAM_CLEAR);
	if (val < 0)
	{
		flag = 1;
		val *= -1;
	}
	for (i=0; i<8; i++)
	{
		data = val%10;
		val /= 10;
		HAL_LcdWrite(7-i, LCD_DISP_NUM_TABLE[data]);
		if (val==0 && i>=dots)
		{
			if (flag != 0)
			{
				HAL_LcdWrite(6-i, LCD_DISP_NUM_TABLE[0x11]);
			}
			break;
		}
	}
	HAL_LcdSetDots(dots);
	HAL_LcdSetUnitStBits(unit, stBits);
}

void HAL_LcdWriteStr(char *str, EN_DISP_UNITS unit, uint32_t stBits)
{
	uint8_t i;
	
	HAL_LcdSetValue(LCD_DATARAM_CLEAR);
	for (i=0; i<8&&str[i]!='\0'; i++)
	{
		HAL_LcdWrite(i, LCD_DISP_CHAR_TABLE[(str[i]&0x7F)-' ']);
	}
	HAL_LcdSetUnitStBits(unit, stBits);
}

// API接口定义
void ApiDispInit(void)
{
	HAL_LcdInit();
}

void ApiDispDeInit(void)
{
	HAL_LcdDeInit();
}

void ApiDispOn(void)
{
	HAL_LcdOn();
}

void ApiDispOff(void)
{
	HAL_LcdOff();
}

void ApiDispClear(void)
{
	HAL_LcdSetValue(LCD_DATARAM_CLEAR);
}

void ApiDispTest(void)
{
	HAL_LcdSetValue(LCD_DATARAM_TEST);
}

void ApiDispWriteHex(uint32_t para)
{
	STAPI_DISPWRITEHEX_PARA *stPara = (STAPI_DISPWRITEHEX_PARA *)para;
	HAL_LcdWriteHex(stPara->buf, stPara->num, stPara->unit, stPara->stBits);
}

void ApiDispWriteDec(uint32_t para)
{
	STAPI_DISPWRITEDEC_PARA *stPara = (STAPI_DISPWRITEDEC_PARA *)para;
	HAL_LcdWriteDec(stPara->buf, stPara->dots, stPara->unit, stPara->stBits);
}

void ApiDispWriteStr(uint32_t para)
{
	STAPI_DISPWRITESTR_PARA *stPara = (STAPI_DISPWRITESTR_PARA *)para;
	HAL_LcdWriteStr(stPara->str, stPara->unit, stPara->stBits);
}
