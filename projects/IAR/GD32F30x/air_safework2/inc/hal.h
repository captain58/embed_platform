/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: hal.h
**创   建   人: yzy
**最后修改日期: 2011年1月12日
**描        述: HAL配置头文件
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2011年1月12日
** 描　述: 原始版本
******************************************************************************/
#ifndef _HAL_H_
#define _HAL_H_


//#include "chip_r7f0c00x.h"
#include "stm32l476xx.h"


#include "hal_def.h"



//	#include "hal_wdt.h"
//	#include "hal_freq.h"
//	#include "hal_gpio.h"
//	#include "hal_hwtimer.h"
//	//	
//	#include "hal_mxflash.h"
//	//	#include "hal_fram.h"
//	//	
//	//	
//	#include "hal_rtc.h"
//	
//	#include "hal_spiio.h"
//	
//	#include "hal_iic_eeprom.h"
//	
//	#include "hal_uartx.h"

#include "hal_uart_stm32l4.h"
#include "stm32l4xx_hal_adc.h"

#include "gpio.h"
#include "drv_spiio.h"
#include "drv_flash.h"
#include "flsmap.h"

#endif







