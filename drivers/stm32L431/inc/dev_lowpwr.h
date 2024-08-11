/******************************Copyright(c)******************************
**                          
**
** File Name: dev_lowpwr.h
** Author: 
** Date Last Update: 2019-11-02
** Description: 低功耗
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#ifndef _DEV_LOWPWR_H_
#define _DEV_LOWPWR_H_

#ifndef EXT_DEV_LOWPWR
#define EXT_DEV_LOWPWR extern
#endif 

uint8_t SYS_Pwr_ChkUnSleep();
#ifndef __MODULE__

void SYS_Pwr_Sleep(void);
#endif

uint8_t SYS_Pwr_SleepCfg();
uint8_t SYS_Pwr_WakeUpCfg();

#endif

