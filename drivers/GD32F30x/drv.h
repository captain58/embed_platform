/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: bsp.h
**创   建   人: yzy
**最后修改日期: 2011年1月12日
**描        述: BSP配置头文件
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2011年1月12日
** 描　述: 原始版本
******************************************************************************/
#ifndef _BSP_H_
#define _BSP_H_


#include "board.h"
//	#include "dev_system.h"
#include "dev_led.h"
//	#include "dev_wdt.h"
#include "dev_gpi.h"
#include "dev_gpo.h"
#include "dev_ad.h"
//	
//	
//	
#include "dev_realtime.h"
//	
//	
//	
//	#if SYS_FILE_EN > 0
//	    #if SYS_FLASH_FRAM_EN > 0
//	    #include "dev_vfram.h"
//	    #endif
#include "dev_file.h"
//#include "dev_lcd.h"

//	#endif
//	
//	#include "dev_iap.h"
//	
//	#include "dev_dec.h"
#include "dev_uart2.h"
#include "dev_msgloop.h"
//	
//	
#include "frmcfg.h"
#include "dev_frm.h"

//#include "dev_valve.h"

#include "dev_lowpwr.h"

#include "dev_hwtimer.h"

#include "dev_ble.h"

#endif





