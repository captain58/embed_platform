/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: log.h
**创   建   人: 
**最后修改日期: 2007年2月2日
**描        述: 系统监视进程处理函数集和相关接口定义的头文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 
** 版  本: v1.0
** 日　期: 2007年2月2日
** 描　述: 原始版本
*****************************************************************************************************/

#ifndef _LOG_H_
#define _LOG_H_

#ifndef EXT_LOG
#define EXT_LOG extern
#endif

#include "logcfg.h"

#define DBGFMT			"%s[%d]:"
#define DBGARG			__FUNCTION__,__LINE__

#define	DBGFMT_A		"%s[%d]:"
#define	DBGARG_A		__FUNCTION__,__LINE__

typedef int (*log_fun_ptr)(unsigned char * buffer, unsigned short len);

unsigned char SYS_LOG_OC(unsigned char en, unsigned char level);
unsigned char SYS_LOG_OCStt(void);
unsigned char SYS_LOG_LevelStt(void);

//日志
#define  LOG_LEVEL_NONE		0		
#define  LOG_LEVEL_ERROR	1
#define  LOG_LEVEL_WARN		2
#define  LOG_LEVEL_INFO		3
#define  LOG_LEVEL_DEBUG	4

#if defined(CHIP_R7F0C00X)
unsigned char LogInit(unsigned char level, unsigned short bufflen, log_fun_ptr __far * ptr );
#else
unsigned char LogInit(unsigned char level, unsigned short bufflen, log_fun_ptr * ptr );
#endif

#if (SYS_PRINT_EN > 0)
#include <stdarg.h>
#include <stdio.h>
#include <string.h>




//	#ifndef SYS_PST_PORT
//	#define SYS_PST_PORT 0
//	#endif

//	#ifndef PRINTPORT_NO
//	#define PRINTPORT_NO UART_CHANNEL_DEBUG
//	#endif


//	#define SYS_UART_LOG_MUX 0  //打印串口是否独立 0复用 1独立

#if defined ( _PLATFORM_ALIOS )
EXT_LOG ksem_t  gs_logRes;  
#endif

#if defined(CHIP_R7F0C00X)//#ifdef (CHIP_R7F0C00X)
EXT_LOG log_fun_ptr __far * g_send_func_ptr;
#else
EXT_LOG log_fun_ptr * g_send_func_ptr;
#endif

//可配置预编译
#define  CONFIG_OPTIONS         0

#define LOG_BUFF_BASE_LEN 256

unsigned char LogDumpHex(unsigned char ucLevel, const char *strTitle, const unsigned char *strData, int nLen);
unsigned char LogPrintf(unsigned char ucLevel, const char *strFormat, ...);
//unsigned char LogInit(unsigned char module, unsigned char level, unsigned short bufflen);


#define LOG_DEBUG(format, args...) 	        LogPrintf(LOG_LEVEL_DEBUG, format, ## args)
#define LOG_INFO(format, args...) 	        LogPrintf(LOG_LEVEL_INFO, format, ## args)
#define LOG_WARN(format, args...) 	        LogPrintf(LOG_LEVEL_WARN, format, ## args)
#define LOG_ERROR(format, args...) 	        LogPrintf(LOG_LEVEL_ERROR, format, ## args)

#define LOG_DUMPHEX(ucLevel, strTitle, strData, nLen)	LogDumpHex(ucLevel, strTitle, strData, nLen)

//LOG_DEBUG( DBGFMT" Tcp_Process = %d\n",DBGARG, (*Tcp_Rec).length);
//LOG_DUMPHEX(0, "HexData dddddddddddddddddddddddddddd : \n", (unsigned char*)&gs_NetPara->Tcp_Packed.Tcp, (*Tcp_Rec).length);


#else
#define LOG_DEBUG(format, args...)
#define LOG_INFO(format, args...)
#define LOG_WARN(format, args...)
#define LOG_ERROR(format, args...)
	
#define LOG_DUMPHEX(ucLevel, strTitle, strData, nLen)
	
#endif

#endif
