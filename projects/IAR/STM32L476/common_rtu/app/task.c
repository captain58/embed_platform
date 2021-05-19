/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: task.c
**创   建   人: yzy
**最后修改日期: 2019年6月1日
**描        述: 任务函数文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2019年6月1日
** 描　述: 原始版本
********************************************************************************************************/

#define EXT_GD_TASK

#include "public.h"
#include "task.h"
#include "pst_zy.h"
#include "hlv.h"
#include "sal_import.h"
#include "atmodem.h"
#include "farp.h"
/**************************************************************
**进行任务分处理的标志
***************************************************************/
uint8 guc_TaskMinFlag;




/************************************************************************
**函数名称: uint8 REP_ChnSend(uint8* buffer, uint16 length, CHN_ENUM chninfo, REP_CFG* cfg)
**描 述: 发送需确认上报帧.
**
**输 入: 
**输 出: SA_ERR_OK / others
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-09-09
*************************************************************************/
uint8 REP_ChnSend(uint8* buffer, uint16 length, uint8_t chninfo, REP_CFG* cfg)
{
    uint8 waitsec = gs_PstPara.repwait;
    uint8 trycnt = gs_PstPara.reptry + 1;       //发送执行次数为重发次数+1(即至少发送一次)
    
    if(cfg->en == 0)                            //若无需等待回复帧,直接发送
    {
        return gfs_PSTChnSend[chninfo](buffer, length);
    }
    else
    {                                           //首先出该序号的确认回复帧接收标志
        STR_ClrBit(bmap_AckRecv, cfg->pseq & 0x0F);
        
        for(uint8 uc_i = 0; uc_i < trycnt; uc_i++)
        {
            if(gfs_PSTChnSend[chninfo](buffer, length) == PST_ERR_OK)
            {                                   //等待接收确认回复帧,每隔100毫秒判断是否接收标志
                for(uint8 uc_j = 0; uc_j < waitsec; uc_j++)
                {
                    gs_OS.TK_Sleep(100);
                    if(STR_IsBitSet(bmap_AckRecv, cfg->pseq & 0x0F))
                    {
                        STR_ClrBit(bmap_AckRecv, cfg->pseq & 0x0F);
                        SYS_OK();
                    }
                }
            }
            else
            {
                aos_msleep(5000);
            }
        }
        
        return SYS_ERR_FT;
    }
    
}





//==============================================================











/************************************************************************
 * @function: _JudgeTaskFlag
 * @描述: 置执行标志位.依赖分消息.必须放在实时性进程中
 * 
 * @参数: 
 * @param:  
 * 
 * @返回: 
 * @return:  
 * @说明: 
 * @作者: yzy (2011-5-13)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void JudgeTaskFlag(uint8_t port)
{

    gss_TaskATimer.flag |= 1 << port;

    LOG_DEBUG("485 port[%d] set TaskFlag ********[ %02x ]******!!!!\n",port, gss_TaskATimer.flag);

    if((gss_TaskATimer.flag & gss_TaskATimer.runflag) == gss_TaskATimer.runflag)
    {
        gss_TaskATimer.atr |= TKT_ATR_FLAG; 
        
        gs_SysVar.mLPstt &= ~HLV_LPTASK_CP;
        gs_SysVar.mLPstt |= HLV_LPTASK_RP;
        
        gss_TaskATimer.flag = 0;
        gs_GPIO.GPO_Out(GPO_485_PWR, false);

    }


}




uint8 guc_TaskLinkWaitSec = 120;              //有任务上报时,重判网络状态倒计时


/************************************************************************
 * @function: Task_Proc
 * @描述: 普通任务处理函数
 * @参数: 
 * 
 * @返回: 
 * @return: bool  true 任务全部上报完成. false 未完成(将每秒再次进入)
 * @说明: 每分钟调用
 * @作者: yzy (2011-5-16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
ST_Netp_FW gs_report_fw;
extern intptr_t g_ulTcp_fd;
extern int32_t HAL_TCP_Destroy(intptr_t fd);
extern kbuf_queue_t gs_TKFarpQueue;

bool Task_Proc(void)
{
    //uint8 para[TASK_PARA_LEN];        //任务参数缓存
    uint8* para = gucs_TaskParaBuffer;  //任务参数缓存
    uint8 time[6];                      //当前时间(秒分时日月年)
    uint8 lasttime[6];                  //历史上报时间(秒分时日月年)
    uint8 index;
    
    PST_Frame frm;
    ZY_Frame* frame = &frm._zy;

    if(gs_SysVar.mDGstt & HLV_STT_LENG)
    {
        gs_SysVar.mLPstt &= ~HLV_LPTASK_RP;
        return false;
    }
    if(gss_TaskATimer.atr & TKT_ATR_FLAG)
    {
        if(gs_SysVar.mDGstt & HLV_STT_NENG)
        {
            if((gs_SysVar.mLPstt & HLV_LPTASK_CLSCK) == HLV_LPTASK_CLSCK)
            {
                return false;
            }
            g_ulTcp_fd = Farp_WM_Connect_Once();
            if(g_ulTcp_fd >= 0)
            {
                frm.chninfo = PST_CHN_TCP;        //0设置通道;3远程通道
            }
            else
            {   SYS_MODM_Shutdown();
                //LOG_INFO("Farp_WM_Connect_Once failed modem shut down\n");    
                return false;
            }
            frame->recv = para;                  //任务参数
            frame->send = gucs_PstSbuf;          //发送缓冲区
            frame->cmd = 0x05;          //seq域(单帧,不需确认)
            frame->atr = GetMeterNum();
            frame->atr <<= 4;
            frame->atr |= 0x06;
            frame->seq = 0;
            ByteArrayHexToBcd(gss_TaskATimer.time.YMDhms, time, 6);
            memcpy(frame->recv, time,6);
            frame->len = 6;

            frame->wd.lword = 0xA621;
            frame->send += 21;
            Task_Execute(&frm, lasttime, time, true);

            
            frame->wd.lword = 0xA622;
            frame->send += 21;
            Task_Execute(&frm, lasttime, time, true);

            gss_TaskATimer.atr &= ~TKT_ATR_FLAG;
            gss_TaskATimer.flag = 0;
            
    //	        aos_msleep(300);
            

            if(gs_SysVar.mDGstt & HLV_STT_NENG)
            {
                if(g_ulTcp_fd >= 0)
                {
                    HAL_TCP_Destroy(g_ulTcp_fd);
                    g_ulTcp_fd = -1;
                    Farp_Close_PPP();
                    SYS_MODM_Shutdown();
                    LOG_INFO("report finish modem shut down\n");    
                }
            }
            gs_SysVar.mLPstt &= ~HLV_LPTASK_RP;
            
        }
        else
        {
            if(gs_FarpVar.wmst & WMS_STT_CLN)
            {
                frm.chninfo = PST_CHN_TCP;        //0设置通道;3远程通道
            }
            else
            {
                if(gs_FarpVar.wmst & WMS_STT_GSM)     //搜索到信号
                {
                    if(guc_TaskLinkWaitSec > 0)     //正在等待网络重联
                    {
                        guc_TaskLinkWaitSec--;
                        return false;
                    }
                    if((gss_TaskATimer.flag & gss_TaskATimer.runflag) == gss_TaskATimer.runflag)
                    {
                        gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FARP_PRIOCNT], 1);
                        guc_TaskLinkWaitSec = 60;
                        return false;
                    }
                }

                return true;
            }
            frame->recv = para;                  //任务参数
            frame->send = gucs_PstSbuf;          //发送缓冲区
            frame->cmd = 0x05;          //seq域(单帧,不需确认)
            frame->atr = GetMeterNum();
            frame->atr <<= 4;
            frame->atr |= 0x06;
            frame->seq = 0;
            ByteArrayHexToBcd(gss_TaskATimer.time.YMDhms, time, 6);
            memcpy(frame->recv, time,6);
            frame->len = 6;

            frame->wd.lword = 0xA621;
            frame->send += 21;
            Task_Execute(&frm, lasttime, time, true);

            
            frame->wd.lword = 0xA622;
            frame->send += 21;
            Task_Execute(&frm, lasttime, time, true);

            gss_TaskATimer.atr &= ~TKT_ATR_FLAG;

            if(gs_SysVar.mDGstt & HLV_STT_NENG)
            {
                if((gs_SysVar.mLPstt & (HLV_LPTASK_CP | HLV_LPTASK_RP))== 0)
                {
                    gs_SysVar.mLPstt |= HLV_LPTASK_CLSCK;
                    gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_ENTER_LP], 1);
                }
            }
            gs_SysVar.mLPstt &= ~HLV_LPTASK_RP;
        }
    }

    return true;

}






/************************************************************************
**函数名称: void Task_PreInit(void* pdata)
**描 述: 任务进程初始化
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-09-09
*************************************************************************/
void Task_PreInit(void)
{
    guc_TaskLinkWaitSec = 0;
    guc_TaskMinFlag = 0;
                                        //初始化主动上报状态机
    ClearBuffer(bmap_AckRecv, sizeof(bmap_AckRecv));
                                        //载入参数
    LoadSystemParam(PARA_TYPE_TASK);
    
//	    ClearBuffer((uint8*)&gss_TaskATimer, sizeof(gss_TaskATimer));
//	    Task_SetTimerCfg(true);             //设置任务允许执行标志
//	    Task_Clear();
    ClearBuffer((uint8*)&gss_TaskATimer, sizeof(gss_TaskATimer));
}

void Task_Clear()
{
    gss_TaskATimer.atr = 0;
    gss_TaskATimer.flag = 0;
//	    ClearBuffer((uint8*)&gss_TaskATimer, sizeof(gss_TaskATimer));
}

/************************************************************************
**函数名称: void Task_SecProc(void* pdata)
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
void Task_SecProc(void)
{
    //REP_Proc();                       //须主站确认帧的上报
    //if(guc_TaskMinFlag == 1)
    {
        //if(GetTime()->sec > 10)         //为了避免过0秒上报时2类数据未冻结
        {
            if(Task_Proc() == true)
            {
                guc_TaskMinFlag = 0;
            }
        }
    }
}


/************************************************************************
**函数名称: void Task_MinProc(void* pdata)
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
void Task_MinProc(void)
{
    guc_TaskMinFlag = 1;
}



