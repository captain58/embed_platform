/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: log.c
**��   ��   ��: 
**����޸�����: 2018��4��11��
**��        ��: ��־CBB
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

********************************************************************************/
#define EXT_LOG

#if defined ( _PLATFORM_ALIOS )
#include "k_api.h"
#endif

#include "log.h"

#if (SYS_PRINT_EN > 0)

unsigned char guc_LogLevel = LOG_LEVEL_DEBUG;
unsigned short gui_buffLen = LOG_BUFF_BASE_LEN;
#if CONFIG_OPTIONS
unsigned char * gucp_LogBuf;
#ifdef DEBUG
unsigned char guc_LogEn = 0;
#else
unsigned char guc_LogEn = 0;
#endif

#else

unsigned char gucp_LogBuf[LOG_BUFF_BASE_LEN];
#ifdef DEBUG
unsigned char guc_LogEn = 1;
#else
unsigned char guc_LogEn = 1;
#endif

#endif
extern const uint32_t gul_UsrFuncVer;
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
unsigned char LogDumpHex(unsigned char ucLevel, const char *strTitle, const unsigned char *strData, int nLen)
{
	int ret = 0, loop = 0, cloop = 0;
	int nOffset;

	if (strData == NULL || nLen <= 0)
    {   
		return 11;
    }
	
	if (ucLevel < LOG_LEVEL_ERROR || ucLevel > LOG_LEVEL_DEBUG) 
    {
		return 11;
	}
	
	if (ucLevel > guc_LogLevel || guc_LogEn == 0) return 0;	
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
//	#if (SYS_UART_LOG_MUX > 0)
//	        SER_Open(PRINTPORT_NO, TDB_MODE_R | TDB_MODE_W);
//	#endif
//	        SYS_SER_WriteOption(PRINTPORT_NO, (unsigned char *)strTitle, 
//	                           strlen((char const *)strTitle), 1);
        //SER_SendData(PRINTPORT_NO, (unsigned char *)strTitle, strlen((char const *)strTitle), 3000);
        if(g_send_func_ptr != NULL)
        {
            ((log_fun_ptr)g_send_func_ptr)((unsigned char *)strTitle, strlen((char const *)strTitle));
        }

//	#if (SYS_UART_LOG_MUX > 0)
//	        SER_Close(PRINTPORT_NO);
//	#endif
#endif
	}
#if defined ( _PLATFORM_ALIOS )    
    if(krhino_sem_take(&gs_logRes, krhino_ms_to_ticks(1000)) != RHINO_SUCCESS)    
//	    if(gs_OS.SEM_Wait(gs_logRes, 1000) != SYS_ERR_OK)
    {
        return 6;//SYS_ERR_NE;
    }
#endif    
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
#if defined ( _PLATFORM_ALIOS )        
        krhino_sem_give(&gs_logRes);
#endif
//	#if (SYS_UART_LOG_MUX > 0)
//	        SER_Open(PRINTPORT_NO, TDB_MODE_R | TDB_MODE_W);
//	#endif
//	        ret = SYS_SER_WriteOption(PRINTPORT_NO, (unsigned char *)gucp_LogBuf, 
//	                                 strlen((char const *)gucp_LogBuf), 1);
        //SER_SendData(PRINTPORT_NO, (unsigned char *)gucp_LogBuf, 
        //                         strlen((char const *)gucp_LogBuf), 3000);
    if(g_send_func_ptr != NULL)
    {
        ((log_fun_ptr)g_send_func_ptr)((unsigned char *)gucp_LogBuf, 
                             strlen((char const *)gucp_LogBuf));
    }

//	#if (SYS_UART_LOG_MUX > 0)
//	        SER_Close(PRINTPORT_NO);
//	#endif
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
unsigned char LogPrintf(unsigned char ucLevel, const char *strFormat, ...)
{
	int ret = 0;
	va_list  vlist;

	if (strFormat == NULL) 
    {
        return 11;//SYS_ERR_VAR;
    }
	
	if (ucLevel < LOG_LEVEL_ERROR || ucLevel > LOG_LEVEL_DEBUG) 
    {
		return 11;//SYS_ERR_VAR;
	}

	if (ucLevel > guc_LogLevel || guc_LogEn == 0) 
    {
        return 0;//SYS_ERR_OK;
    }
    memset(gucp_LogBuf, 0, 256);
#if defined ( _PLATFORM_ALIOS )    
    if(krhino_sem_take(&gs_logRes, krhino_ms_to_ticks(1000)) != RHINO_SUCCESS)
    {
        return 6;//SYS_ERR_NE;
    }
    
    sys_time_t ul_TkTick = krhino_sys_tick_get();
#ifdef MASTER_NODE    
	sprintf((char *)gucp_LogBuf, "%ld.%02ld:[M%d] ", (unsigned long)ul_TkTick/RHINO_CONFIG_TICKS_PER_SECOND, (unsigned long)ul_TkTick%RHINO_CONFIG_TICKS_PER_SECOND, gul_UsrFuncVer%0x100);
#else
	sprintf((char *)gucp_LogBuf, "%ld.%02ld:[S%d] ", (unsigned long)ul_TkTick/RHINO_CONFIG_TICKS_PER_SECOND, (unsigned long)ul_TkTick%RHINO_CONFIG_TICKS_PER_SECOND, gul_UsrFuncVer%0x100);
#endif
#endif	
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
#if defined ( _PLATFORM_ALIOS ) 
    krhino_sem_give(&gs_logRes);
#endif    
//	#if (SYS_UART_LOG_MUX > 0)
//	    SER_Open(PRINTPORT_NO, TDB_MODE_R | TDB_MODE_W);
//	#endif
//	    ret = SYS_SER_WriteOption(PRINTPORT_NO, (unsigned char *)gucp_LogBuf, 
//	                             strlen((char const *)gucp_LogBuf), 0);
//	    SER_SendData(PRINTPORT_NO, (unsigned char *)gucp_LogBuf, 
//	                                     strlen((char const *)gucp_LogBuf), 1000);
    if(g_send_func_ptr != NULL)
    {
        ((log_fun_ptr)g_send_func_ptr)((unsigned char *)gucp_LogBuf, strlen((char const *)gucp_LogBuf));
    }

//	#if (SYS_UART_LOG_MUX > 0)
//	    SER_Close(PRINTPORT_NO);
//	#endif

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
#if defined(CHIP_R7F0C00X)
unsigned char LogInit(unsigned char level, unsigned short bufflen, log_fun_ptr __far * ptr )
#else
unsigned char LogInit(unsigned char level, unsigned short bufflen, log_fun_ptr * ptr )
#endif                        
{
    
	if (level < LOG_LEVEL_ERROR || level > LOG_LEVEL_DEBUG) 
    {
        return 11;//SYS_ERR_VAR;
    }   
    guc_LogLevel = level;
    guc_LogEn = 1;
    //�����ź�������
#if defined ( _PLATFORM_ALIOS )    
    kstat_t err = krhino_sem_create(&gs_logRes, "sem_log", 1);
//	    gs_logRes = (TESRes *)gs_Mem.GetMEM(sizeof(TESRes));
    if(RHINO_SUCCESS != err)
    {
        return 2;//SYS_ERR_FT;
    }
#endif    
#if CONFIG_OPTIONS    
   
    if(bufflen < LOG_BUFF_BASE_LEN)
    {
        bufflen = LOG_BUFF_BASE_LEN;
    }
    //��������
    gucp_LogBuf = (unsigned char *)gs_Mem.GetMEM(bufflen);
    if(NULL == gucp_LogBuf)
    {
        return SYS_ERR_FT;
    }
    gui_buffLen = bufflen;
    
#endif    

    g_send_func_ptr = ptr;
    return 0;//SYS_ERR_OK;
}
#else
#if defined(CHIP_R7F0C00X)
unsigned char LogInit(unsigned char level, unsigned short bufflen, log_fun_ptr __far * ptr )
#else
unsigned char LogInit(unsigned char level, unsigned short bufflen, log_fun_ptr * ptr )
#endif                        
{
    return 0;
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

unsigned char SYS_LOG_OC(unsigned char en, unsigned char level)
{
    unsigned char ret = 1;
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

    ret = 0;
#endif
    return ret;
}
/************************************************************************
 * @function: SYS_LOG_OCStt
 * @����: ��ȡ��־ģ�鿪��״̬
 * 
 * @����: 
 * @param: unsigned char
 * @����: 
 * @˵��: 
 * @����: yzy (2013/11/10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/

unsigned char SYS_LOG_OCStt(void)
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
 * @param: unsigned char
 * @����: 
 * @˵��: 
 * @����: yzy (2013/11/10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/

unsigned char SYS_LOG_LevelStt(void)
{
#if (SYS_PRINT_EN > 0)
    return guc_LogLevel;
#else
    return 0;
#endif
}


