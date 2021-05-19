/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: log.h
**��   ��   ��: 
**����޸�����: 2007��2��2��
**��        ��: ϵͳ���ӽ��̴�����������ؽӿڶ����ͷ�ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: 
** ��  ��: v1.0
** �ա���: 2007��2��2��
** �衡��: ԭʼ�汾
*****************************************************************************************************/

#ifndef _LOG_H_
#define _LOG_H_

#ifndef EXT_LOG
#define EXT_LOG extern
#endif

#define DBGFMT			"%s[%d]:"
#define DBGARG			__FUNCTION__,__LINE__

#define	DBGFMT_A		"%s[%d]:"
#define	DBGARG_A		__FUNCTION__,__LINE__

#include <stdint.h>
#include "bdef.h"

uint8_t SYS_LOG_OC(uint8_t en, uint8_t level);
uint8_t SYS_LOG_OCStt(void);
uint8_t SYS_LOG_LevelStt(void);

#if (SYS_PRINT_EN > 0)
#include <stdarg.h>
#include <stdio.h>


#ifndef PRINT_TERMINALIO
#define PRINT_TERMINALIO    1   //��terminal io ���
#endif

#ifndef PRINT_UART
#define PRINT_UART          2   //��UART���
#endif

#ifndef PRINT_TYPE
#define PRINT_TYPE      PRINT_UART
#endif


#ifndef SYS_PST_PORT
#define SYS_PST_PORT 0
#endif

#ifndef PRINTPORT_NO
#define PRINTPORT_NO UART_CHANNEL_DEBUG
#endif


#define SYS_UART_LOG_MUX 0  //��ӡ�����Ƿ���� 0���� 1����

EXT_LOG ksem_t  gs_logRes;  

//������Ԥ����
#define  CONFIG_OPTIONS         0
//��־
#define  LOG_LEVEL_NONE		0		
#define  LOG_LEVEL_ERROR	1
#define  LOG_LEVEL_WARN		2
#define  LOG_LEVEL_INFO		3
#define  LOG_LEVEL_DEBUG	4

#define LOG_BUFF_BASE_LEN 256

uint8 LogDumpHex(uint8 ucLevel, const int8 *strTitle, const uint8 *strData, int32 nLen);
uint8 LogPrintf(uint8 ucLevel, const int8 *strFormat, ...);
uint8 LogInit(uint8 module, uint8 level, uint16 bufflen);



#define LOG_DEBUG(format, args...) 	        LogPrintf(LOG_LEVEL_DEBUG, format, ## args)
#define LOG_INFO(format, args...) 	        LogPrintf(LOG_LEVEL_INFO, format, ## args)
#define LOG_WARN(format, args...) 	        LogPrintf(LOG_LEVEL_WARN, format, ## args)
#define LOG_ERROR(format, args...) 	        LogPrintf(LOG_LEVEL_ERROR, format, ## args)

#define LOG_DUMPHEX(ucLevel, strTitle, strData, nLen)	LogDumpHex(ucLevel, strTitle, strData, nLen)

//LOG_DEBUG( DBGFMT" Tcp_Process = %d\n",DBGARG, (*Tcp_Rec).length);
//LOG_DUMPHEX(0, "HexData dddddddddddddddddddddddddddd : \n", (uint8*)&gs_NetPara->Tcp_Packed.Tcp, (*Tcp_Rec).length);


#else
#define LOG_DEBUG(format, args...)
#define LOG_INFO(format, args...)
#define LOG_WARN(format, args...)
#define LOG_ERROR(format, args...)

#define LOG_DUMPHEX(ucLevel, strTitle, strData, nLen)

#endif

#endif
