/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: scan.c
**创   建   人: yzy
**最后修改日期: 2019年6月1日
**描        述: 低级扫描进程的函数文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2019年6月1日
** 描　述: 原始版本
********************************************************************************************************/

#define EXT_SCAN

#include "public.h"
#include "lib_com/calclib_gw.h"
#include "hlv.h"
#include "netp.h"
#include "task.h"
#include "netp_fm.h"
#include "alert.h"
#include <sal_import.h>

#include "atmodem.h"
#include "paradef.h"
cpu_stack_t  gs_TKSlvStack[TASK_SLV_STKL];
ktask_t      gs_TKSlvHandle;
kbuf_queue_t gs_TKSlvQueue;
char         gc_TKSlvQbuf[MSG_BUFF_LEN*2];
static char         g_TKSlvQ_buf_recv[BUFQUEUE_MSG_MAX+4];
static size_t       g_TKSlvQ_recv_size;

uint32_t g_ulFtp_Timeout = 0;
extern kbuf_queue_t gs_TKFarpQueue;

//外部flash相关宏定义
#define FLS_PROGRAM_ADDR         (4096 + 256)       //程序在外部flash中的位置(前32字节用来存放upatemark)
#define FLS_PROGRAM_MAXLEN       524288     //程序总空间大小512K字节


#define FLS_UPDATEMARK_ADDR      0          //定义程序升级标记在Flash中的位置

/*******************************************************************************
**这两组数据用来存放远程升级的标记,从而可以让升级时的意外停电
**在复位之后得到回复
********************************************************************************/
const uint32 gul_UpdataMarkA_APP[4] = 
{
    0x12345678, 0x87654321, 0x11223344, 0x55667788
};


const uint32 gul_UpdataMarkB_APP[4] = 
{
    0,0,0,0
};



/************************************************************************
 * @function: CheckFileHead
 * @描述: 校验dpg文件头的格式
 * 
 * @参数: 
 * @param: head  
 * 
 * @返回: 
 * @return: uint8 SYS_ERR_OK, SYS_ERR_FT
 * @作者: xugl (2010-4-23)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 CheckFileHead(uint8* head)
{
    uint8 tmp;
    tmp = DoSum(head, 31);
    if(head[31] != tmp)
    {
        return SYS_ERR_FT;
    }

    if(CmpBuffer(head, "DPG2", 4))
    {
        return SYS_ERR_FT;
    }
                                        //工程代码必须匹配或为全零,否则退出
    if(CmpBuffer(head + 4, (uint8*)gucs_PrjCode, 6) && 
       CmpBuffer(head + 4, "AAAAAA", 6))
    {
        return SYS_ERR_FT;
    }

    SYS_OK();
}
#define UPGRADE_TYPE_PROC   0
#define UPGRADE_TYPE_PARA   1
#define UPGRADE_TYPE_NETP   2
#ifdef UPGRADE_CHECK

extern const char g_programarray[];
#endif
//extern IWDG_HandleTypeDef hiwdg;
/***********************************************************
 * @function_name: ES_APP_IAP_Check
 * @function_file: iap_file.c
 * @描述: 检验Flash中升级程序的合法性         
 * @参数: 
 * @param:  startAddr  升级数据位于FLASH的起始偏移
 * @param； length 升级数据长度
 * @返回: 
 * @return: true  数据合法,将开始进行程序升级
           false  数据非法,将不能进行程序升级
 * @作者:  xugl (2010-11-11)
 *******************************************************/
bool IAP_CheckCRC(uint32 startAddr, uint32 length)
{
    uint8 uc_data[64];                  //缓存用来读取数据用
    uint16 crc = 0xFFFF;                //CRC校验值
    uint16 cmp;                         //CRC校验值的对比
    int i = 0;
    uint32 curAdress = startAddr;		//程序起始地址
    
    if((length & 0x03) != 0)            //判断长度的合法性
    {
        return false;                   //返回错误
    }
    
    SYS_FILE_Open();
    
    while(length > 0)                   //不断的读取数据然后进行校验
    {
        if(length > 64)                //大于64字节读64字节
        {
            
            SYS_FILE_Read(uc_data, curAdress, 64);
            crc = DoCrcRtuModbus(uc_data, 64, crc);
#ifdef UPGRADE_CHECK
            for(i = 0; i < 64; i ++)
            {
                if(uc_data[i] != g_programarray[curAdress- startAddr+ i])
                    break;
            }  
            if(i != 64)
            {
                break;
            }
#endif
            length -= 64;
            curAdress += 64;

            
        }
        else                            //否则读取剩余数据的长度
        {
            SYS_FILE_Read(uc_data, curAdress, length);
#ifdef UPGRADE_CHECK            
            for(i = 0; i < length; i ++)
            {
                if(uc_data[i] != g_programarray[curAdress - startAddr+ i])
                    break;
            }   
            if(i != length)
            {
                break;
            }
#endif
                                        //最后的4个字节是CRC校验
            crc = DoCrcRtuModbus(uc_data, length, crc);
                                        //获取CRC校验的对比值
            SYS_FILE_Read(&cmp, curAdress+length, 2);
            //cmp = ByteToInt(uc_data + length);
            length = 0;                 //已经读取完所有的数据
        }
#ifndef DEBUG        
        HAL_IWDG_Refresh(&hiwdg); //喂狗
#endif
    }
    
    SYS_FILE_Close();
    
    if(cmp != crc)                      //判断CRC的校验值是否相等
    {
        return false;
    }
    return true;                        //返回OK        
}

/***********************************************************
 * @function_name: ES_APP_IAP_Check
 * @function_file: iap_file.c
 * @描述: 检验Flash中升级程序的合法性         
 * @参数: 
 * @param:  db  数据块
 * @param； len 长度
 * @返回: 
 * @return: true  数据合法,将开始进行程序升级
           false  数据非法,将不能进行程序升级
 * @作者:  xugl (2010-11-11)
 *******************************************************/
bool IAP_Check(uint32 length)
{
	return IAP_CheckCRC(FLS_PROGRAM_ADDR, length);
}

/***********************************************************
 * @function_name: ES_IAP_Mark
 * @function_file: iap_file.c
 * @描述: 设置或者清除升级标记         
 * @参数: 
 * @param:   setorclr 设置或者清除升级标记 
 * @param:   ID       数据ID
 * @param:  length   数据长度
 * @param:  address  数据地址
 * @返回: 
 * @return:  
 * @作者:  xugl (2010-11-11)
 *******************************************************/
void IAP_Mark(bool setorclr, uint32 length, uint32 address)
{
    uint8 uc_data[28];   

    if(setorclr)                        //设置标记
    {
                                        //拷贝写入标记
        memcpy(uc_data, (void*)gul_UpdataMarkA_APP, sizeof(gul_UpdataMarkA_APP));
        uc_data[16] = 0;               //数据ID
                                        //长度
        LongToByte(uc_data + 18, length+4);
                                        //地址
        LongToByte(uc_data + 22, address);
                                        //计算CRC校验
        IntToByte(uc_data + 26, DoCrcRtuModbus(uc_data, 26, 0xFFFF));
    }
    else                                //仅仅拷贝数据就可以了
    {
        memcpy(uc_data, (void*)gul_UpdataMarkB_APP, sizeof(gul_UpdataMarkB_APP));
    }
    
    SYS_FILE_Open();
	SYS_FILE_Write(uc_data, FLS_UPDATEMARK_ADDR, 28);
    SYS_FILE_Close();
}

/************************************************************************
 * @function: ES_IAP_Ready
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
 * @作者: xugl (2013/7/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
bool IAP_Ready(uint32 address, uint32 length, bool isreset)
{
    if(length > 0x80000)                //不支持这么长的升级工作
    {
        return false;                   //返回错误
    }
                                        //确认升级数据的合法性,若主升级和备份部分CRC都不正确才认为不正确
    if(!IAP_Check(length))
    {
	    return false;                   //返回错误
    }
                                        //写入升级标志
    IAP_Mark(true, length, address);
                                        //进行系统复位
    

    return true;
}
int Upgrade_Netp_FW(uint32_t len)
{
    TDataBlock db;
//    ST_NETP_FW_BASE tmp;
    uint32_t addr = 256;
    
//    while (len >= sizeof(ST_NETP_FW_BASE))
//    {
//        
//        if(SYS_FILE_DB_Open(DB_UPDATE, &db, TDB_MODE_RW) != SYS_ERR_OK)
//        {
//            return -1;
//        }  
//        if(SYS_ERR_OK != SYS_FILE_DB_ReadFrom(&db, (uint8_t * )&tmp, sizeof(ST_NETP_FW_BASE), addr))
//        {
//            SYS_FILE_DB_Close(&db); 
//            return -1;
//        }
//        SYS_FILE_DB_Close(&db); 
//        if(SYS_ERR_OK != Netp_Register_Set(&tmp))
//        {
//            return -1;
//
//        }
//        len -= sizeof(ST_NETP_FW_BASE);
//        addr += sizeof(ST_NETP_FW_BASE);
//    }

    

    return 0;
}
/************************************************************************
 * @function: Farp_WM_FTP_Open
 * @描述: 连接GPRS处理
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
int Farp_WM_FTP_Open(void)
{
    if(SYS_ERR_OK != HAL_FTP_Check(gs_FtpPara.ftp_user, gs_FtpPara.ftp_pwd))
    {
        return -1;
    }
    if(SYS_ERR_OK != HAL_FTP_Start(gs_FtpPara.ftp_domain, gs_FtpPara.port, gs_FtpPara.ftp_fold))
    {
        return -1;
    }
    return 0;
}
#define CON_FTP_TRANS_LEN 1024
#ifdef UPGRADE_CHECK
uint8_t guc_ftpbufbak[CON_FTP_TRANS_LEN];
#endif
const uint8_t FW_KEY[16] ={'C','o','p','y','r','i','g','h','t',' ','H','R','C','o','m','m'};
int Farp_WM_FTP_Get(uint16_t * val)
{
//	    uint8_t outbuf[1000];
    uint8_t * guc_ftpbuf = NULL;

    uint16_t len = CON_FTP_TRANS_LEN;
    guc_ftpbuf = aos_malloc(len);
    if(guc_ftpbuf == NULL)
    {
        LOG_ERROR(DBGFMT_A"malloc failed! \n", DBGARG_A);
        return -1;
    }
    memset(guc_ftpbuf,0,CON_FTP_TRANS_LEN);
    TDataBlock db;
    int ret  = HAL_FTP_Get(gs_FtpPara.ftp_file, guc_ftpbuf, gs_FtpPara.getLen, &len);
    
//	    if(SYS_ERR_OK != HAL_FTP_Get(gs_FtpPara.ftp_file, outbuf, gs_FtpPara.getLen, &len))
//	    {
//	        return -1;
//	    }
    uint8_t * pxor = guc_ftpbuf;
    for(int loop=0;loop<len;loop++)
    {
        *pxor = (*pxor)^FW_KEY[loop&0xf];
        pxor++;
    }

    if(ret == SYS_ERR_OK)
    {
        if(gs_FtpPara.getLen == 0)
        {
            if(SYS_ERR_OK == CheckFileHead(guc_ftpbuf))
            {
            
                if(SYS_FILE_DB_Open(DB_UPDATE, &db, TDB_MODE_RW) != SYS_ERR_OK)
                {
                    aos_free(guc_ftpbuf);
                    return -1;
                }  
                if(SYS_ERR_OK != SYS_FILE_DB_WriteFrom(&db, guc_ftpbuf, len, gs_FtpPara.getLen))
                {
                    SYS_FILE_DB_Close(&db); 
                    aos_free(guc_ftpbuf);
                    return -1;
                }
                SYS_FILE_DB_Close(&db); 

            }
            else
            {
                aos_free(guc_ftpbuf);
                return 2;
            }
            
        }
        else
        {
            if(SYS_FILE_DB_Open(DB_UPDATE, &db, TDB_MODE_RW) != SYS_ERR_OK)
            {
                aos_free(guc_ftpbuf);
                return -1;
            }  
            if(SYS_ERR_OK != SYS_FILE_DB_WriteFrom(&db, guc_ftpbuf, len, gs_FtpPara.getLen))
            {
                SYS_FILE_DB_Close(&db); 
                aos_free(guc_ftpbuf);
                return -1;
            }
            SYS_FILE_DB_Close(&db); 

            

        }
#ifdef UPGRADE_CHECK

        if(SYS_FILE_DB_Open(DB_UPDATE, &db, TDB_MODE_RW) != SYS_ERR_OK)
        {
            aos_free(guc_ftpbuf);
            return -1;
        }  

        if(SYS_ERR_OK != SYS_FILE_DB_ReadFrom(&db, guc_ftpbufbak, len, gs_FtpPara.getLen))
        {
            SYS_FILE_DB_Close(&db); 
            aos_free(guc_ftpbuf);
            return -1;
        }

        
        SYS_FILE_DB_Close(&db);

        if(CmpBuffer(guc_ftpbufbak, guc_ftpbuf, len) != 0)
        {
            aos_free(guc_ftpbuf);
            return -100;
        }

#endif        
        *val = len;
        if(len != CON_FTP_TRANS_LEN)
        {
            aos_free(guc_ftpbuf);
            return 1;
        }
        else
        {
            aos_free(guc_ftpbuf);
            return 0;
        }
    }
    else if(ret == 1)
    {
        aos_free(guc_ftpbuf);
        return 3;
    }
    else
    {
        aos_free(guc_ftpbuf);
        return -1;
    }
}

uint8 guc_FtpLinkWaitSec;
extern uint16 gui_FarpWmRcSec;
extern uint8 RUN_Para_RW(uint32 addr, uint8* buff, uint16 len, bool rw);

int Task_Ftp(void)
{
    uint8_t outbuf[64];
    uint16_t framelen = 0;
    REP_CFG repcfg;
    TDataBlock db;
    int ret = 0;
    if(gs_SysVar.mDGstt & HLV_STT_NENG)
    {
        return 0;
    }
    
    if(gs_FarpVar.wmst & WMS_STT_GSM)//有信号就链接
    {
//	        frm.chninfo = PST_CHN_TCP;        //0设置通道;3远程通道
    }
    else
    {
        //if(gs_FarpVar.wmst & WMS_STT_GSM)     //搜索到信号
//	        {
//	            if(guc_FtpLinkWaitSec > 0)     //正在等待网络重联
//	            {
//	                guc_FtpLinkWaitSec--;
//	                return ;
//	            }
//	            if(gs_FtpPara.flag == FTP_STEP_OPEN || gs_FtpPara.flag == FTP_STEP_ING)
//	            {
//	                extern kbuf_queue_t gs_TKFarpQueue;
//	                gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FARP_PRIOCNT], 1);
//	                guc_FtpLinkWaitSec = 30;
//	                return ;
//	            }
//	        }

        return ret;
    }


    if(g_ulFtp_Timeout)
    {
        g_ulFtp_Timeout--;
        return ret;
    }
    if(gs_FtpPara.flag == FTP_STEP_WAIT || gs_FtpPara.flag == FTP_STEP_START)
    {
        uint32_t t1 = CalcSecs((uint8_t *)&(GetTime()->sec), false);
        uint32_t t2 = CalcSecs((uint8_t *)&gs_FtpPara.time.sec, false);
        if(t1 > t2)
        {
            gs_FtpPara.flag = FTP_STEP_OPEN;
            gs_FtpPara.fileLen = 0;
            gs_FtpPara.getLen = 0;
            ret = 1;
        }
        else
        {
            ret = 0;
        }
        gs_FtpPara.err_times = 0;
    }
    else if(gs_FtpPara.flag == FTP_STEP_OPEN)
    {
        
//	        gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FTP_OPEN], 1);
        if(0 == Farp_WM_FTP_Open())
        {
            gs_FarpVar.wmst |= WMS_STT_FTP;
//	            gs_OS.Message_Send(&gs_TKSlvQueue, &msgidA[MSG_FTP_OPEN_SUCC], 1);
            g_ulFtp_Timeout = 0;
            gs_FtpPara.flag = FTP_STEP_ING;
            gs_FtpPara.err_times = 0;

        }
        else
        {
            gs_FarpVar.wmst &= ~WMS_STT_FTP;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msgidA[MSG_FARP_DISGPRS], 1);
            gui_FarpWmRcSec = 10;
//	            gs_OS.Message_Send(&gs_TKSlvQueue, &msgidA[MSG_FTP_OPEN_FAILED], 1);
            gs_FtpPara.err_times++;
            g_ulFtp_Timeout = 60 * gs_FtpPara.err_times;//15秒超时

        }
        ret = 1;
    }
    else if(gs_FtpPara.flag == FTP_STEP_ING)
    {
//	        gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FTP_GET], 1);
//	        g_ulFtp_Timeout = 120;//15秒超时
        uint16_t glen = 0;
        uint8_t msg_buf[3];
        for(int i = 0; i < 180; i ++)
        {
            HB_RetLive(TASK_SLV_TKID);
            int ret = Farp_WM_FTP_Get(&glen);
            gs_SysVar.mDGstt |= HLV_STT_USBE;
            if(0 == ret)
            {
    //	            msg_buf[0] = MSG_FTP_GET_SUCC;
    //	            memcpy(msg_buf+1, (uint8_t *)&glen, 2);
    //	        
    //	            gs_OS.Message_Send(&gs_TKSlvQueue, msg_buf, 3);
    //	        uint16_t len = 0;
    //	        memcpy((uint8_t *)&len, &g_TKSlvQ_buf_recv[1], 2);
                gs_FtpPara.getLen += glen;
                gs_FtpPara.fileLen += glen;
                gs_FtpPara.err_times = 0;
                
                g_ulFtp_Timeout = 0;
                gs_FarpVar.wmst |= WMS_STT_FTP;
    //	                    uint8 msg = MSG_FARP_RECVVALIDIP;
                krhino_buf_queue_send(&gs_TKFarpQueue, &msgidA[MSG_FARP_RECVVALIDIP], 1);
                if((gs_FtpPara.getLen % 0x1000) == 0)
                    RUN_Para_RW(RUNPARA_FTP, &gs_FtpPara, RUNPARA_FTP_PLEN, true);

            }
            else if(ret == 1)
            {
    //	            msg_buf[0] = MSG_FTP_GET_FNS;
    //	            memcpy(msg_buf+1, (uint8_t *)&glen, 2);
    //	        
    //	            gs_OS.Message_Send(&gs_TKSlvQueue, msg_buf, 3);
                HAL_FTP_Close();
                gs_FarpVar.wmst &= ~WMS_STT_FTP;
    //	uint16_t len = 0;
    //	            memcpy((uint8_t *)&len, &g_TKSlvQ_buf_recv[1], 2);
                gs_FtpPara.getLen += glen;
                gs_FtpPara.fileLen += glen;
                gs_FtpPara.err_times = 0;
                
                g_ulFtp_Timeout = 0;
                gs_FtpPara.flag = FTP_STEP_FINISH;            
                krhino_buf_queue_send(&gs_TKFarpQueue, &msgidA[MSG_FARP_RECVVALIDIP], 1);
                break;
            }
            else if(ret == 2)
            {
    //	            gs_OS.Message_Send(&gs_TKSlvQueue, &msgidA[MSG_FTP_PROJ_FAILED], 1);
                g_ulFtp_Timeout = 2;
                gs_FtpPara.flag = FTP_STEP_FAILED;

                HAL_FTP_Close();
                gs_FarpVar.wmst &= ~WMS_STT_FTP;
                break;

            }
            else if(ret == 3)//CME err 625
            {
                gs_FtpPara.err_times++;
                g_ulFtp_Timeout = 15*gs_FtpPara.err_times;
                gs_FtpPara.flag = FTP_STEP_OPEN;
                HAL_FTP_Close();
                gs_FarpVar.wmst &= ~WMS_STT_FTP;   
                break;
            }
            else    
            {
    //	            gs_OS.Message_Send(&gs_TKSlvQueue, &msgidA[MSG_FTP_GET_FAILED], 1);
                gs_FtpPara.err_times++;
                if(gs_FtpPara.err_times > 2)
                {
                
                    g_ulFtp_Timeout = 15*gs_FtpPara.err_times;
                    gs_FtpPara.flag = FTP_STEP_OPEN;
                    HAL_FTP_Close();
                    gs_FarpVar.wmst &= ~WMS_STT_FTP;     
                }
                else
                {
                    g_ulFtp_Timeout = 5;
                }
                break;
            } 
        }
        ret = 1;

    }
    else if(gs_FtpPara.flag == FTP_STEP_FINISH)
    {
//	        gs_OS.Message_Send(&gs_TKFarpQueue, msgidA[MSG_FTP_GET], 1);
//	        g_ulFtp_Timeout = 15;//15秒超时
        //校验文件
        
        if(SYS_FILE_DB_Open( DB_UPDATE, &db, TDB_MODE_RW) != SYS_ERR_OK)
        {
            return 0;
        }  
        if(SYS_ERR_OK != SYS_FILE_DB_ReadFrom(&db, outbuf, 32, 0))
        {
            SYS_FILE_DB_Close(&db); 
            return 0;
        }
        SYS_FILE_DB_Close(&db); 
        
        if(CheckFileHead(outbuf) != SYS_ERR_OK)
        {
            return 0;
        }
        RUN_Para_RW(RUNPARA_FTP, &gs_FtpPara, RUNPARA_FTP_PLEN, true);
        
                                        //启动升级
        uint32_t faddr = ByteToLong(outbuf + 24);
        uint32_t flen = ByteToInt24(outbuf + 14);
//	        uint16_t framelen;
        uint8_t type = outbuf[28];
        repcfg.en = 0;
        repcfg.pseq = 3;
        
        if(type == 0)
        {
            
            if(true == IAP_Ready(faddr, flen, true))
            {
                memset(outbuf,0,64);
                gs_SysVar.mDGstt |= HLV_STT_USBP;
                framelen = VS_BuildReportFrm(0x05, 0x8353, 0, outbuf);
                REP_ChnSend(outbuf, framelen, PST_CHN_TCP, &repcfg);
                LOG_INFO("IAP ready,now sys will reboot !!!\n");
                aos_msleep(1000);
                aos_reboot();

//	                gs_FtpPara.flag = FTP_STEP_REBOOT;
                g_ulFtp_Timeout = 10;//15秒超时
            }
            else
            {
//	                memset(outbuf,0,64)
//	                    
//	                framelen = VS_BuildReportFrm(0x05, 0x8353, 0x21, outbuf);
//	                REP_ChnSend(outbuf, framelen, PST_CHN_TCP, &repcfg);
//	                gs_FtpPara.flag = FTP_STEP_IDEL;
                goto ERR_FTP;

            }
            
        }
        else if(type == 1)//参数
        {   
            gs_FtpPara.flag = FTP_STEP_IDEL;
        }
            
        else if(type == 2)//配型
        {   
            
            if(!IAP_Check(flen))
            {
                goto ERR_FTP;                   //返回错误
            }
//	            gs_FtpPara.flag = FTP_STEP_IDEL;
            if(SYS_ERR_OK != Upgrade_Netp_FW(flen))
            {
                goto ERR_FTP;                   //返回错误
            }
            memset(outbuf,0,64);
               
            framelen = VS_BuildReportFrm(0x05, 0x8353, 0, outbuf);
            REP_ChnSend(outbuf, framelen, PST_CHN_TCP, &repcfg);
        
            SYS_Netp_Init();
            
//	            aos_msleep(1000);
//	            aos_reboot();
        }
        else
        {
            goto ERR_FTP; 
        }
        ret = 1;
        
        gs_FtpPara.flag = FTP_STEP_IDEL;
        RUN_Para_RW(RUNPARA_FTP, &gs_FtpPara, RUNPARA_FTP_PLEN, true);
    }
    
    else if(gs_FtpPara.flag == FTP_STEP_FAILED)
    {
        ret = 1;
        goto ERR_FTP; 

    }
    else
    {
        //aos_msleep(1000); 
        ret = 0;
    }
//	    else if(gs_FtpPara.flag == FTP_STEP_REBOOT)
//	    {
//	
//	    }

    if(gs_FtpPara.err_times > 10)
    {
        //ftp 失败
//	        gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FTP_CLOSE], 1);
        HAL_FTP_Close();
        gs_FarpVar.wmst &= ~WMS_STT_FTP;

        g_ulFtp_Timeout = 15;//15秒超时
        goto ERR_FTP;
    }
    return ret;
ERR_FTP:
    memset(outbuf,0,64);
    gs_SysVar.mDGstt &= ~(HLV_STT_USBP | HLV_STT_USBE);
    framelen = VS_BuildReportFrm(0x05, 0x8353, 0x21, outbuf);
    REP_ChnSend(outbuf, framelen, PST_CHN_TCP, &repcfg);
    gs_FtpPara.flag = FTP_STEP_IDEL;
    gs_FtpPara.err_times = 0;
    RUN_Para_RW(RUNPARA_FTP, &gs_FtpPara, RUNPARA_FTP_PLEN, true);
    
    return ret;
    
}


/************************************************************************
**函数名称: void SLV_SecProc(void* pdata)
**描 述: 秒处理
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-09-09
*************************************************************************/
void SLV_SecProc(void)
{
    if(Task_Ftp())
    {
        return ;
    }
    Task_SecProc();
    

    ALRT_ActiveReport();
//	    Led_OpSignal();
}


/************************************************************************
**函数名称: void SLV_MinProc(void* pdata)
**描 述: 分处理
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-09-09
*************************************************************************/
void SLV_MinProc(void)
{
    Task_MinProc();
}


//---------------------------------------------------------------------






/************************************************************************
 * @function: SLV_InitProc
 * @描述: 低级进程的初始化函数
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/3/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SLV_InitProc(void)
{
    ;
}

int32_t guc_CardLen = 0;

#if (SYS_LOW_POWER > 0)

typedef struct _ST_AIR_SAFE_MNG_
{
    uint32_t delay;
    uint8_t interval;
    uint8_t switch_stt;
    int32_t card_stt;
    uint8_t stt;
}ST_AIR_SAFE_MNG;
ST_AIR_SAFE_MNG gst_asw_mng;


#else

#endif

#define CON_WIRELESS_WORK_DELAY 30 //1分钟
#define CON_WIRELESS_INIT_DELAY 30 //1分钟

uint8_t airsafe_mng_init()
{
#if (SYS_LOW_POWER > 0)

    memset((uint8_t *)&gst_asw_mng, 0, sizeof(ST_AIR_SAFE_MNG));
    gst_asw_mng.delay = CON_WIRELESS_INIT_DELAY;

#endif
}
#define CON_AIR_SAFE_STATUS_IEDL        0
#define CON_AIR_SAFE_STATUS_HOOKUP      0x01
#define CON_AIR_SAFE_STATUS_HOOKDOWN    0x02
#define CON_AIR_SAFE_STATUS_WAIT    0x04        //钩子拿开后继续保持60秒通讯
#define CON_AIR_SAFE_SLEEP_RUN_1M   0x08

uint8_t get_status(void)
{
    
    uint8_t ret = 0;
#if (SYS_LOW_POWER > 0)
    static uint32_t ulcount = 0;

    ulcount++;
    uint8_t ucswitch_stt = guc_SwitchOnOff;
    int32_t lcard_stt = guc_CardLen;

    if(gst_asw_mng.delay >0)
    {
        gst_asw_mng.delay --;
    }

    if(ucswitch_stt > 0)//勾子在线，正常通讯
    {
        gst_asw_mng.delay = CON_WIRELESS_WORK_DELAY;
        gst_asw_mng.switch_stt = ucswitch_stt;
        ret |= CON_AIR_SAFE_STATUS_HOOKUP;
    }
    else
    {
        if(gst_asw_mng.delay >0)//钩子掉线后，运行一段时间
        {
            ret |= CON_AIR_SAFE_STATUS_WAIT;
        }
        else
        {
//            if((ulcount % CON_WIRELESS_WORK_DELAY) == 0)
//            {
//                ret |= CON_AIR_SAFE_SLEEP_RUN_1M;
//            }
        }
    }

    if(gst_asw_mng.stt != ret)
    {//状态发生变化
        if(0 == ret)
        {
            extern kbuf_queue_t gs_RFMngQueue;
            krhino_buf_queue_send(&gs_RFMngQueue, &msgidA[MSG_ENTER_SLEEP], 1);
        }
        else
        {
            extern kbuf_queue_t gs_RFMngQueue;
            krhino_buf_queue_send(&gs_RFMngQueue, &msgidA[MSG_WAKE_UP], 1);

        }
        gst_asw_mng.stt = ret;
    }
    
#else
    ret |= CON_AIR_SAFE_STATUS_HOOKUP;
#endif
    return ret;

}
    volatile int g_i=0;
extern uint8_t guc_AllowLogin;
/*******************************************************************************
 * @function_name:  ES_SLV_Task
 * @function_file:  
 * @描述: 低级进程处理函数
 * 
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2011-1-24)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_SLV_Task(void)
{
    uint8_t stt;
    uint16_t count = 0;
    //链表消息
    krhino_buf_queue_create(&gs_TKSlvQueue, "slv_queue",
                         gc_TKSlvQbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);
    
    SYS_MSG_Apply(TASK_SLV_TKID, MSG_CLS_TM);

    //gs_OS.Message_Send(&gs_TKSlvQueue, &msgidA[MSG_FARP_CHECK], 1);
    aos_msleep(2000);
    
    airsafe_mng_init();
    
    for(;;)
    {
        
        //LOG_DEBUG("--------------------SLV wait msg-----------\n");
        krhino_buf_queue_recv(&gs_TKSlvQueue, RHINO_WAIT_FOREVER, g_TKSlvQ_buf_recv,
                                 &g_TKSlvQ_recv_size);
        //LOG_DEBUG("SLV recv msg[%d]\n", g_TKSlvQ_buf_recv[0]);
        //获取进程消息
        switch(g_TKSlvQ_buf_recv[0])        //根据消息分别处理
        {
            case MSG_SEC:                   //秒消息处理
            
//            if(count % 60)
            //SYS_AD_Scan(0, &gn_VBat);

            case MSG_EVENT_CHANGE:
            {
                stt = get_status();
#ifndef MASTER_NODE    
                if(stt > 0)
                {
                    if(guc_SwitchOnOff > 0)
                    {
                        if(!guc_SwitchNorErr)
                        {
                            for(g_i = 0; g_i < 1; g_i++)
                            {
                                int32_t CardLen = HAL_RFID_ReadCardID(nDeviceCardId, guc_CardLen);
                                if(CardLen > 0 /*&& CardLen != guc_CardLen*/)
                                {
                                    guc_CardLen = CardLen;
                                    extern kbuf_queue_t gs_RFMngQueue;
                                    krhino_buf_queue_send(&gs_RFMngQueue, &msgidA[MSG_SWITCH_CHANGE], 1);
                                    break;
                                    
                                }
                                msleep(10);
                            }
                            HAL_RFID_Sleep();
                            g_i = 0x11;
                        }

                        SYS_AD_Scan(0, &gn_VBat);
                        if(gn_VBat >= 3400)
                        {
                            SYS_Dev_OptBlinkSet(SYS_LED_BAT, 3, 0, 0, 0); 
                        }
                        else
                        {
                            SYS_Dev_OptBlinkSet(SYS_LED_BAT, 1, 100, 100, 0); 
                        }                        
                        
                    }
                    if(guc_CardLen > 0 && guc_SwitchOnOff > 0)
                    {
                        if(guc_BuzzerNorErr != 1)
                        {
                            SYS_Dev_OptBlinkSet(GPIO_BUZ_CARD, 2, 0, 0, 100); 
                            guc_BuzzerNorErr = 1;
                        }
                        if(!guc_SwitchNorErr)
                        {
                            extern kbuf_queue_t gs_RFMngQueue;
                            krhino_buf_queue_send(&gs_RFMngQueue, &msgidA[MSG_SWITCH_CHANGE], 1);
                        }                        
                        guc_SwitchNorErr = 1;
                        if(0 == guc_AllowLogin)
                        SYS_Dev_OptBlinkSet(SYS_LED_CARD, 2, 0, 0, 0); 
                        
                    }
                    else
                    {

                        if(guc_SwitchNorErr)
                        {
                            extern kbuf_queue_t gs_RFMngQueue;
                            krhino_buf_queue_send(&gs_RFMngQueue, &msgidA[MSG_SWITCH_CHANGE], 1);
                            SYS_Dev_OptBlinkSet(GPIO_BUZ_CARD, 2, 0, 0, 100); 
                        }                        
                        if(/*guc_CardLen <= 0 && */guc_SwitchOnOff == 0)
                        {
                            if(guc_SwitchNorErr)
                            {
                                extern kbuf_queue_t gs_RFMngQueue;
                                krhino_buf_queue_send(&gs_RFMngQueue, &msgidA[MSG_SWITCH_CHANGE], 1);

                                guc_CardLen = 0;
                            }

                        
                            guc_BuzzerNorErr = 0;
                            guc_SwitchNorErr = 0;
                            if(0 == guc_AllowLogin)
                                SYS_Dev_OptBlinkSet(SYS_LED_CARD, 3, 0, 0, 0); 
                        }
                    }
                }
#endif                  
                break;
            }
            case MSG_MIN: 
//                count ++;
//                SYS_AD_Scan(0, &gn_VBat);
//
//                if(gn_VBat >= 3600)
//                {
//                    SYS_Dev_OptBlinkSet(SYS_LED_BAT, 3, 0, 0, 0); 
//                }
//                else
//                {
//                    SYS_Dev_OptBlinkSet(SYS_LED_BAT, 1, 100, 100, 0); 
//                }
                
                break;
            case MSG_LIVE:
				HB_RetLive(TASK_SLV_TKID);
                break;
        }
    }
}



/******************************************************************************
**低级进程块常量部分定义
******************************************************************************/
//	const TESTcbConst gs_TkSlv = 
//	{
//	    ES_SLV_Task,
//	    TASK_SLV_TKID >> 3,                    //应用管理进程
//	    TASK_SLV_TKID,                         //进程编号
//	    TASK_SLV_TKID & 7,                     //优先级内进程编号
//	    true,                                      //消息循环机制进程
//	};

const KTaskConst gs_TkSLV = 
{
    &gs_TKSlvHandle,
    SYS_SLV_Task,                           //应用管理进程
    gs_TKSlvStack,
    TASK_SLV_TKID,                           //进程编号
    true,                                    //消息循环机制进程
    TASK_SLV_STKL,
    "Slv",
    &gs_TKSlvQueue,
};



