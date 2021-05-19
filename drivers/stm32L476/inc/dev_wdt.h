/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: dev_wdt.h
**创   建   人: yzy
**最后修改日期: 2018年4月25日
**描        述: WDT设备模块的头文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2018年4月25日
** 描　述: 原始版本
********************************************************************************************************/
#ifndef _DEV_WDT_H_
#define _DEV_WDT_H_


#ifndef EXT_DEV_WDT
#define EXT_DEV_WDT extern
#endif
 



/************************************************************************
**辅助喂狗计数器(看门狗喂狗时间<1s时)
*************************************************************************/
EXT_DEV_WDT uint8 guc_TickDogFeedCnt;       //单位100ms
EXT_DEV_WDT uint8 guc_TickDogFeedDIS;       //0xaa表示禁止扩展看门狗功能

/*******************************************************************************
 * @function_name:  WDT_Feed
 * @function_file:  dev_wdt.c
 * @描述: 喂狗
 * 
 * 
 * @参数: 
 * @param: wdttick.狗的寿命(单位100ms)
 * 
 * @返回: 
 * @return:  void   
 * @作者: yzy (2010-03-11)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_WDT_Feed(uint8 wdttick);
 
/*******************************************************************************
 * @function_name:  SYS_WDT_Init
 * @function_file:  
 * @描述: 看门狗初始化
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2010-03-11)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_WDT_Init(void);

/*******************************************************************************
 * @function_name:  SYS_WDT_DIS
 * @function_file:  dev_wdt.c
 * @描述: 停止喂狗
 * 
 * @参数: 仅限测试使用.注意,有外部看门狗的时候禁用内部看门狗.否则测试时无法有效测试外部看门狗
 * @返回: 
 * @作者: yzy (2010-03-11)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_WDT_DIS(void);


#endif







