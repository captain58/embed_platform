/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: main.c
**��   ��   ��: 
**����޸�����: 2013��6��8��
**��        ��: Ӧ�ó���������
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
**������: 
**��  ��: v1.0
**�ա���: 2014��12��24��
**�衡��: ԭʼ�汾
**-------------------------------------------------------------------------------------------------------
** �޸���: 
** ��  ��: 
** ��  ��: 
********************************************************************************************************/

/*******************************************************************************
**�����������߽ӿ�ͷ�ļ�������������ע��һ������ֻ��Ҫ��һ��c
**�ļ��ж��������������
********************************************************************************/
#define EXT_LOG

#include "logTiny.h"

#if (SYS_PRINT_EN > 0)
//	#include "bsp.h"
//	#include "apis.h"

uint8 guc_LogLevel = LOG_LEVEL_DEBUG;
uint16 gui_buffLen = LOG_BUFF_BASE_LEN;
#if CONFIG_OPTIONS
uint8 * gucp_LogBuf;
#ifdef DEBUG
uint8 guc_LogEn = 0;
#else
uint8 guc_LogEn = 0;
#endif

#else

uint8 gucp_LogBuf[LOG_BUFF_BASE_LEN];
#ifdef DEBUG
uint8 guc_LogEn = 1;
#else
uint8 guc_LogEn = 1;
#endif

#endif

/*******************************************************************************
 * @function_name:  LogDumpHex
 * @function_file:  main.c
 * @����: 
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2014-11-28)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 LogDumpHex(uint8 ucLevel, const int8 *strTitle, const uint8 *strData, int32 nLen)
{
	int32 ret = 0, loop = 0, cloop = 0;
	int32 nOffset;

	if (strData == NULL || nLen <= 0)
    {   
		return SYS_ERR_VAR;
    }
	
	if (ucLevel < LOG_LEVEL_ERROR || ucLevel > LOG_LEVEL_DEBUG) 
    {
		return SYS_ERR_VAR;
	}
	
	if (ucLevel > guc_LogLevel || guc_LogEn == 0) return SYS_ERR_OK;	
	//д�����
	if (NULL != strTitle) 
    {
#if ((defined DEBUG) && (PRINT_TYPE == PRINT_TERMINALIO))
        if(krhino_sem_take(&gs_logRes, MS_TO_TICK(1000)) != RHINO_SUCCESS)
        {
            return SYS_ERR_NE;
        }

		printf("%s", strTitle);
        //gs_OS.SEM_Release(gs_logRes);
        krhino_sem_give(&gs_logRes);
        
#else
        SER_Open(PRINTPORT_NO, TDB_MODE_W);
//	        SYS_SER_WriteOption(PRINTPORT_NO, (uint8 *)strTitle, 
//	                           strlen((char const *)strTitle), 1);
        SER_SendData(PRINTPORT_NO, (uint8 *)strTitle, strlen((char const *)strTitle), 3000);
#if (SYS_UART_LOG_MUX > 0)
            SER_Close(PRINTPORT_NO);
#endif
#endif
	}
    if(krhino_sem_take(&gs_logRes, MS_TO_TICK(1000)) != RHINO_SUCCESS)    
//	    if(gs_OS.SEM_Wait(gs_logRes, 1000) != SYS_ERR_OK)
    {
        return SYS_ERR_NE;
    }
	//д������
	for (; loop < nLen; loop+=0x10)
	{
		nOffset = 0;
		for (cloop = 0; cloop < 0x10; cloop++)
		{
		    if((nOffset+3) >gui_buffLen )
            {
                break;//��ֹ���������
            }      
			if ((loop+cloop) >= nLen)
            {         
                //nOffset += sprintf((char *)gucp_LogBuf + nOffset, "   ");
                break;
            }
            else
            {         
                nOffset += sprintf((char *)gucp_LogBuf + nOffset, "%02X ", 
                                   strData[loop+cloop]);
            }
            
		}

		sprintf((char *)gucp_LogBuf + nOffset, "\n");

#if ((defined DEBUG) && (PRINT_TYPE == PRINT_TERMINALIO))	    
		printf("%s", gucp_LogBuf);
        //gs_OS.SEM_Release(gs_logRes);
        krhino_sem_give(&gs_logRes);

#else
        //gs_OS.SEM_Release(gs_logRes);
        krhino_sem_give(&gs_logRes);

        SER_Open(PRINTPORT_NO, TDB_MODE_W);
//	        ret = SYS_SER_WriteOption(PRINTPORT_NO, (uint8 *)gucp_LogBuf, 
//	                                 strlen((char const *)gucp_LogBuf), 1);
        SER_SendData(PRINTPORT_NO, (uint8 *)gucp_LogBuf, 
                                 strlen((char const *)gucp_LogBuf), 3000);

#if (SYS_UART_LOG_MUX > 0)
        SER_Close(PRINTPORT_NO);
#endif
#endif        
	}
	
	return ret;
}

/*==================================================================
//��������  LogPrintf
//���ߣ�    ������
//���ڣ�    2013-11-1
//���ܣ�    ��ָ���ĸ�ʽ�����־
//���������ucLevel:  ��־����
//          strFormat: ��־��ʽ�ַ�
//�������: ��
//����ֵ��  �ɹ�����0��ʧ�ܷ���1
//=================================================================*/
uint8 LogPrintf(uint8 ucLevel, const int8 *strFormat, ...)
{
	int32 ret = 0;
	va_list  vlist;

	if (strFormat == NULL) 
    {
        return SYS_ERR_VAR;
    }
	
	if (ucLevel < LOG_LEVEL_ERROR || ucLevel > LOG_LEVEL_DEBUG) 
    {
		return SYS_ERR_VAR;
	}

	if (ucLevel > guc_LogLevel || guc_LogEn == 0) 
    {
        return SYS_ERR_OK;
    }
    memset(gucp_LogBuf, 0, 256);
    if(krhino_sem_take(&gs_logRes, MS_TO_TICK(1000)) != RHINO_SUCCESS)
    {
        return SYS_ERR_NE;
    }
    sys_time_t ul_TkTick = krhino_sys_tick_get();
	sprintf((char *)gucp_LogBuf, "%ld.%02ld: ", (uint32_t)ul_TkTick/RHINO_CONFIG_TICKS_PER_SECOND, (uint32_t)ul_TkTick%RHINO_CONFIG_TICKS_PER_SECOND);
	
	va_start(vlist, strFormat);
	vsnprintf((char *)gucp_LogBuf + strlen((char const *)gucp_LogBuf), 
               sizeof(gucp_LogBuf) - strlen((char const *)gucp_LogBuf), 
               (char const *)strFormat, vlist);
	va_end(vlist);
    
	//snprintf((char *)gucp_LogBuf + strlen((char const *)gucp_LogBuf),4, "\r\n");
    
#if ((defined DEBUG) && (PRINT_TYPE == PRINT_TERMINALIO))	
	printf("%s", gucp_LogBuf);
    krhino_sem_give(&gs_logRes);
#else
    krhino_sem_give(&gs_logRes);
    SER_Open(PRINTPORT_NO, TDB_MODE_W);
//	    ret = SYS_SER_WriteOption(PRINTPORT_NO, (uint8 *)gucp_LogBuf, 
//	                             strlen((char const *)gucp_LogBuf), 0);
    SER_SendData(PRINTPORT_NO, (uint8 *)gucp_LogBuf, 
                                     strlen((char const *)gucp_LogBuf), 3000);
#if (SYS_UART_LOG_MUX > 0)
    SER_Close(PRINTPORT_NO);
#endif

#endif   

	return ret;
}
/************************************************************************
 * @Function: LogInit
 * @Description:  [����ӿ�]��ʼ��Logģ��
 *             1.ʹ��ģ��ǰ���ȳ�ʼ��.
 * @Arguments: 
 * @param: module Ĭ������򿪵�ģ��
 * @param: level Ĭ������򿪵ļ���
 * @param: bufflen �ڲ����泤��
 * @Auther: yzy
 * Date: 2015/5/14
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 LogInit(uint8 module, uint8 level, uint16 bufflen)
{
    
	if (level < LOG_LEVEL_ERROR || level > LOG_LEVEL_DEBUG) 
    {
        return SYS_ERR_VAR;
    }   
    guc_LogLevel = level;
    guc_LogEn = 1;
    //�����ź�������
    kstat_t err = krhino_sem_create(&gs_logRes, "sem_log", 1);
//	    gs_logRes = (TESRes *)gs_Mem.GetMEM(sizeof(TESRes));
    if(RHINO_SUCCESS != err)
    {
        return SYS_ERR_FT;
    }
#if CONFIG_OPTIONS    
   
    if(bufflen < LOG_BUFF_BASE_LEN)
    {
        bufflen = LOG_BUFF_BASE_LEN;
    }
    //��������
    gucp_LogBuf = (uint8 *)gs_Mem.GetMEM(bufflen);
    if(NULL == gucp_LogBuf)
    {
        return SYS_ERR_FT;
    }
    gui_buffLen = bufflen;
    
#endif    
    return SYS_ERR_OK;
}

#endif

/************************************************************************
 * @function: SYS_LOG_OC
 * @����: �򿪻��߹ر���־����
 * 
 * @����: 
 * @param: en   1    ����־����
 *              0   �ر���־����
 *           level ��־����
 * @����: 
 * @˵��: 
 * @����: yzy (2014/12/24)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/

uint8_t SYS_LOG_OC(uint8_t en, uint8_t level)
{
    uint8_t ret = SYS_ERR_EXIST;
#if (SYS_PRINT_EN > 0)

    if (level < LOG_LEVEL_ERROR || level > LOG_LEVEL_DEBUG)
    {
        return ret;
    }

    if(en == 1) 
    {
        guc_LogEn = 1;
    }
    else    
    {
        guc_LogEn = 0;
    }

    guc_LogLevel = level;

    ret = SYS_ERR_OK;
#endif
    return ret;
}
/************************************************************************
 * @function: SYS_LOG_OCStt
 * @����: ��ȡ��־ģ�鿪��״̬
 * 
 * @����: 
 * @param: uint8
 * @����: 
 * @˵��: 
 * @����: yzy (2013/11/10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/

uint8_t SYS_LOG_OCStt(void)
{
#if (SYS_PRINT_EN > 0)
    return guc_LogEn;
#else
    return 0;
#endif
}
/************************************************************************
 * @function: SYS_LOG_OCStt
 * @����: ��ȡ��־ģ���ӡ����״̬
 * 
 * @����: 
 * @param: uint8
 * @����: 
 * @˵��: 
 * @����: yzy (2013/11/10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/

uint8_t SYS_LOG_LevelStt(void)
{
#if (SYS_PRINT_EN > 0)
    return guc_LogLevel;
#else
    return 0;
#endif
}


