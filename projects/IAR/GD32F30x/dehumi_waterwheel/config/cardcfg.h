/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: cardcfg.h.h
**创   建   人: yzy
**最后修改日期: 2018年2月6日
**描        述: 对卡系统应用的头文件定义
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2018年4月11日
** 描　述: 原始版本
******************************************************************************/


#ifndef _CARDCFG_H_
#define _CARDCFG_H_

#define CBB_CARDCFG_EN     1       //打印

#define CBB_ESAM_UART_PORT  UART_CHANNEL_ESAM   //esam通信串口
#define CBB_CARD_UART_PORT  UART_CHANNEL_CARD   //card通信串口

#define CBB_ESAM_GPO_PWR GPO_ESAM_PWR           //esam电源开关定义
#define CBB_CARD_GPO_PWR GPO_CARD_PWR           //card电源开关定义



#endif


