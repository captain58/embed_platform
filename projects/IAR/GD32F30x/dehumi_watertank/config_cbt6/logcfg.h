/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: ext.h
**创   建   人: yzy
**最后修改日期: 2018年2月6日
**描        述: 对系统应用的头文件定义
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2018年4月11日
** 描　述: 原始版本
******************************************************************************/


#ifndef _LOGCFG_H_
#define _LOGCFG_H_

#define SYS_PRINT_EN     1       //打印


#ifndef PRINT_TERMINALIO
#define PRINT_TERMINALIO    1   //在terminal io 输出
#endif

#ifndef PRINT_UART
#define PRINT_UART          2   //在UART输出
#endif

#ifndef PRINT_TYPE
#define PRINT_TYPE      PRINT_UART
#endif

#endif

