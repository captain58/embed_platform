/******************************Copyright(c)******************************
**                         
**
** File Name: dev_iap.h
** Author: 
** Date Last Update: 2019-11-02
** Description: 
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/

#ifndef _EXT_DEV_IAP_
#define _EXT_DEV_IAP_

#ifndef EXT_DEV_IAP
#define EXT_DEV_IAP
#endif


/************************************************************************
 * @function: SYS_IAP_Ready
 * @描述: 
 * 
 * @参数: 
 * @param: address 升级起始地址
 * @param: length 用户程序需要升级的程序的长度
 * @param: isreset 升级完之后是否需要复位用户程序
 * 
 * @返回: 
 * @return: bool  true成功/false失败
 * @说明: 
 * @作者: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
bool SYS_IAP_Ready(uint32 address, uint32 length, bool isreset);



#endif


