/******************************Copyright(c)******************************
**                         
**
** File Name: bsp.h
** Author: 
** Date Last Update: 2019-11-02
** Description: bsp头文件集
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#ifndef _BSP_H_
#define _BSP_H_



//	#include "dev_led.h"
//	#include "dev_wdt.h"
#include "dev_gpi.h"
#include "dev_gpo.h"
//	
//	
//	
#if (SYS_RTC_EN > 0 || SYS_HRTC_EN > 0)
#include "dev_realtime.h"
#endif
//	
//	
//	
#if SYS_FILE_EN > 0
    #if SYS_FLASH_FRAM_EN > 0
    #include "dev_vfram.h"
    #endif
#include "dev_file.h"
#endif
//	
//	#include "dev_iap.h"
//	
//	#include "dev_dec.h"
#include "dev_uart2.h"
#include "dev_msgloop.h"
//	
//	
//	#include "frmcfg.h"
//	#include "dev_frm.h"

#if SYS_PWM_EN > 0
#include "dev_pwm.h"
#endif

#if SYS_E2_EN > 0
#include "dev_eeprom.h"
#endif

#include "dev_metering.h"

#include "dev_hall.h"

#if SYS_WIFIMODEM_EN > 0
#include "dev_wifi.h"
#endif


#if SYS_AD_EN > 0
#include "dev_ad.h"
#endif


#if SYS_LED_EN > 0
#include "dev_led.h"
#endif

#if SYS_WIFIMODEM_EN > 0
#include "dev_wifi.h"
#endif

#if SYS_LOWPWR_EN > 0
#include "dev_lowpwr.h"
#endif

#if SYS_LCD_EN > 0
#include "dev_disp.h"
#endif

#if SYS_PWRCHK_EN > 0
#include "dev_pwrchk.h"
#endif

#endif





