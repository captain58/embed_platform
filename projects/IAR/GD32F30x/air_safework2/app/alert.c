/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: GD_alert.c
**创   建   人: yzy
**最后修改日期: 2019-06-18
**描        述: 告警模块程序
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2019-06-18
** 描　述: 原始版本
********************************************************************************************************/
#define EXT_GD_ALERT


#include "public.h"
#include "extapi.h"
#include "task.h"
#include "evt.h"
#include "hlv.h"
#include "sal_import.h"
#include "atmodem.h"
//	uint8 EvtTmpBuf[200];                   //写入FLS事件缓存(须OSLOCK使用)

#define CON_EVENT_RECORD_MAX_LEN  0x80

#define ERR_TYPE_485        2
#define ERR_TYPE_CP         6
#define ERR_TYPE_LOSTVOL    5

#define ERR_CODE_485_NORMAL     0
#define ERR_CODE_485_INVALID    1
#define ERR_CODE_485_NORSP      2
#define ERR_CODE_485_CHECK      3
#define ERR_CODE_485_CMD        4
#define ERR_CODE_485_DATA       5

/************************************************************************
**函数名称: uint8 ALRT_WriteLog(uint8 evtno, uint8* pt_ex, uint8 len_ex)
**描 述: 往flash中写入事件记录
**
**输 入: evtno 写入的事件号(1-64)
         pt_ex 写入内容指针
         len_ex 写入内容的长度(<=200)

**输 出: true 成功 
         false 失败(不需记录或传入参数错误)

**备 注: 已做多进程保护
**--------------------------------------------------------
**作 者: yzy
**日 期: 
*************************************************************************/
uint8 ALRT_WriteLog(uint8 evtno, uint8* pt_ex, uint8 len_ex)
{
    TDataBlock db;
    uint32 addr;                        //flash数据块中偏移地址
    uint8* Pt_EvtCnt;
    uint8 erc = evtno - 1;
    int m = 0;
    uint8_t tmp[6];
    
                                        //判断是否已经载入参数
    uint8_t EvtTmpBuf[CON_EVENT_RECORD_MAX_LEN]; 
    memset(EvtTmpBuf,0,CON_EVENT_RECORD_MAX_LEN);
    
    if(!(gl_SysRunStt & HRS_PARALD))
    {
        return false;
    }
                                        //判断事件号是否有效
    if(erc > 63)
    {
        return false;
    }
                                        //再次判断是否需要记录
//	    if(!STR_GetBits(gs_InpVarData.evtbit, 1, erc))
//	    {
//	        return false;
//	    }
//	    
    //多进程调用.且用到全局事件指针
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();    
                                        
    if(gs_FramMisData.e1cnt == 255)
    {
        gs_FramMisData.e1cnt = 0;
    }

    addr = CON_EVENT_RECORD_MAX_LEN * gs_FramMisData.e1cnt;
    Pt_EvtCnt = &gs_FramMisData.e1cnt;
    
                                        //散转准备事件数据
    MoveBuffer(pt_ex, EvtTmpBuf+8, len_ex);

    RHINO_CPU_INTRPT_ENABLE();                   //进程解锁

    EvtTmpBuf[m++] = len_ex+7;
    gs_RealTime.ReadBCDDateTime((TIME*)tmp);
    MoveBuffer(tmp, EvtTmpBuf+m, 6);  
    InvBuffer(EvtTmpBuf+m, 6);
    m+=6;
    EvtTmpBuf[m++] = evtno;    
//	    EvtTmpBuf[1] = evtno;

    
    BuildBufferCRC(EvtTmpBuf, CON_EVENT_RECORD_MAX_LEN, true);
    
                                        //写入flash
//	    ES_FILE_DB_Open(DB_EVT, &db, TDB_MODE_RW);
//	    ES_FILE_DB_WriteFrom(&db, EvtTmpBuf, 0xC0, addr);
//	    ES_FILE_DB_Close(&db);
    CBB_FILE_Write_Event(DB_FLASH, EvtTmpBuf, CON_EVENT_RECORD_MAX_LEN);
    (*Pt_EvtCnt)++;                     //事件计数器增一

//	    SaveFramMisData(MIS_E1C);           //保存更新后事件指针到铁电
//	    SaveFramMisData(MIS_E2C);
    return true;

}







/************************************************************************
**函数名称: uint8 ALRT_ReadLog(uint8* pm, uint8* pn, uint8* data, uint8 type)
**描 述: 获取告警数据
**
**输 入: pm 起始指针 (0-255)
         pn 结束指针 (0-255)
         data 数据存放缓存 
         type 0 重要事件  1 一般事件
**输 出: 获取数据的长度,最大数据长度不超过200字节
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 08-11-26
*************************************************************************/
int ALRT_ReadLog(uint8* pm, uint8* pn, uint8* data, uint8 type)
{
    uint32 addr;
    uint8 tmp[200];                     //缓存
    uint16 len = 0;
    int buflen = 200;
    TDataBlock db;
    uint8_t exnum = 0;

    //SYS_FILE_DB_Open(DB_EVENT, &db, TDB_MODE_RW);

    for(; (*pm) != (*pn); (*pm)++)      //若pm = 0,pn = 1则读取首条记录. pm,pn不可相等
    {                                   //计算fls地址
//	        addr = 0x80 * (*pm);
//	        addr += (type ? 0xC000 : 0);
                                        //读取并校验
//	        SYS_FILE_DB_ReadFrom(&db, tmp, CON_EVENT_RECORD_MAX_LEN, addr);
        buflen = 200;
        memset(tmp,0,200);
        if(SYS_ERR_OK != CBB_FILE_Read_Event(DB_FLASH, *pm, 1, tmp, &buflen))
        {
            break;
        }
//	        CBB_FILE_Read_Event(DB_FLASH, *pm, 1, tmp, 128);

        if(CheckBufferCRC(tmp, CON_EVENT_RECORD_MAX_LEN, true) == false)
        {
            break;
        }
                                        //移入有效事件数据
        _IF_TRUE_DO(tmp[0] > 0x80, continue;)
        _IF_TRUE_DO(len + (tmp[0] + 2) > 480, break;)
//	        data[0] = tmp[0];
//	        len++;
        MoveBuffer(tmp+1, data + len, tmp[0]);
        len += tmp[0];
        exnum++;
#ifndef DEBUG        
        extern IWDG_HandleTypeDef hiwdg;
        HAL_IWDG_Refresh(&hiwdg); //喂狗
#endif        
    }

//	    SYS_FILE_DB_Close(&db);
    * pn = exnum;

    return len;
}









/************************************************************************
**函数名称: void ALRT_SendData(uint8 type)
**描 述: 发送首条未上报告警记录
**
**输 入: type 0 重要事件 1 一般事件
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-10-13
*************************************************************************/
void ALRT_SendData(uint8 type)
{
    TDataBlock db;
    PST_Frame pstFrame;
    ZY_Frame* frame = &pstFrame._zy;
    uint32 addr;
    uint8 data[256];
    uint8 tp[10];
    uint8 time[6];                      //当前时间(秒分时日月年)

    uint8 len;
    uint8 ept;
                                        //计算fls地址
//	    if(type == 0)
    {
        ept = gs_FramMisData.e1pt;
        addr = CON_EVENT_RECORD_MAX_LEN * ept;
    }

                                        //读取并校验
    SYS_FILE_DB_Open(DB_EVENT, &db, TDB_MODE_RW);
    SYS_FILE_DB_ReadFrom(&db, data, CON_EVENT_RECORD_MAX_LEN, addr);
    SYS_FILE_DB_Close(&db);

    if(CheckBufferCRC(data, CON_EVENT_RECORD_MAX_LEN, true) == false)
    {
        return;
    }
    len = data[0];            //事件数据长度
    MoveBuffer(data+1, data + 21, len);   //移到帧的相应位置

    frame->cmd = 0x05;          //seq域(单帧,不需确认)
    frame->send = data;          //发送缓冲区
    frame->len = len;
    
    frame->atr = GetMeterNum();
    frame->atr <<= 4;
    frame->atr |= 0x06;
    frame->seq = 0;
    ByteArrayHexToBcd(gss_TaskATimer.time.YMDhms, time, 6);
    
    frame->wd.lword = 0xA624;
    
    REP_CFG repcfg;
    repcfg.pseq = 2;
    repcfg.en = 0;
//	    frame->send += 21;
//	    data[14] = 0;                       //pn
//	    data[15] = 0;
//	    data[16] = type ? 0x02 : 0x01;      //Fn
//	    data[17] = 0;
//	                                        //数据内容
//	    data[18] = gs_FramMisData.e1cnt;
//	    data[19] = gs_FramMisData.e2cnt;
//	    data[20] = ept;
//	    data[21] = ept + 1;
//	    
//	    frame->atr = 0;                      //不含tp
//	    frame->send = data;                  //发送缓存
//	    frame->tbd = (CMD_DIR | CMD_PRM | 0x04);//控制域(上行|启动站|功能码4:用户数据)
//	    frame->cmd = 0x0E;                   //afn
//	    frame->len = len;                    //数据长度
//	    frame->send[11] = 0;                 //主站地址
//	    frame->tstno = SEQ_FIR_FIN;          //seq(单帧)
//	    frame->tstno |= guc_pseq & 0x0F;     //seq(帧序号)
                                        //发送
    /*
    if(gs_PstPara.AckServ & 0x04)
    {
        frame.tstno |= 0x10;            //seq(要求主站确认)
        len = GD_Pack(frame);
        REP_Enqueue(frame.send, len);
    }
    else
    {
        len = GD_Pack(frame);
        gfs_PSTChnSend[PST_CHN_TCP](frame.send, len);
    }
    */
    //frm->atr |= EXIST_TP;               //含TP(EXIST_TP)
//	    tp[0] = guc_pseq;
//	    gs_RealTime.ReadBCDDateTime((TIME*)(tp + 1));
//	    tp[5] = 0;
//	    frame->wd.lword = (uint32)tp;
//	    len = GD_Pack(frame);              //打包
    if(PST_ERR_OK == PST_ZY_Pack(&pstFrame,SYS_ERR_OK,&len))
    {
//	        if(gs_FarpVar.wmst & WMS_STT_CLN )
        {
            gfs_PSTChnSend[PST_CHN_TCP](frame->send, len);
        }
    }
//	    else
//	    {
//	        Farp_SendSMS(frame->send, len, 4, 1);
//	    }
                                        //更新启动计数器
    guc_pseq++;
}


uint8 guc_AlrtLinkWaitSec = 120;              //有事件上报时,重判网络状态倒计时

extern kbuf_queue_t gs_TKFarpQueue;
extern intptr_t g_ulTcp_fd;
extern int32_t HAL_TCP_Destroy(intptr_t fd);
/************************************************************************
 * @function: ALRT_ActiveReport
 * @描述: 告警记录的主动上报
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_ActiveReport(void)
{
  
                                        //是否允许主动上报
    /*if(!(gs_FramMisData.enable & 0x01))
    {
        return;
    }*/


                                        //是否有事件上报
    if(gs_FramMisData.e1pt != gs_FramMisData.e1cnt)
    {
        if(gs_SysVar.mDGstt & HLV_STT_NENG)
        {
            if((gs_SysVar.mLPstt & HLV_LPTASK_CLSCK) == HLV_LPTASK_CLSCK)
            {
                return ;
            }
            if((gs_SysVar.mLPstt & HLV_LPTASK_LOSTVOL) != HLV_LPTASK_LOSTVOL)
            {
                return ;
            }
            g_ulTcp_fd = Farp_WM_Connect_Once();
            if(g_ulTcp_fd >= 0)
            {
//	                frm.chninfo = PST_CHN_TCP;        //0设置通道;3远程通道
            }
            else
            {   SYS_MODM_Shutdown();
                gs_SysVar.mLPstt &= ~HLV_LPTASK_LOSTVOL; 
                //LOG_INFO("Farp_WM_Connect_Once failed modem shut down\n");    
                return ;
            }

            for(;gs_FramMisData.e1pt != gs_FramMisData.e1cnt;)
            {
                ALRT_SendData(0);
                gs_FramMisData.e1pt++;
            }
            
            if(gs_SysVar.mDGstt & HLV_STT_NENG)
            {
                if(g_ulTcp_fd >= 0)
                {
                    HAL_TCP_Destroy(g_ulTcp_fd);
                    g_ulTcp_fd = -1;
                    //Farp_Close_PPP();
                    //SYS_MODM_Shutdown();
                    LOG_INFO("report warn finish modem close socket\n");    
                }
            }
            gs_SysVar.mLPstt &= ~HLV_LPTASK_LOSTVOL;

        }
        else
        {
                                            //是否建立连接
            if(!(gs_FarpVar.wmst & (WMS_STT_CLN | WMS_ETH0STT)))
            {
                
                if(gs_FarpVar.wmst & WMS_STT_GSM)     //搜索到信号
                {
                    if(guc_AlrtLinkWaitSec > 0)     //正在等待网络重联
                    {
                        guc_AlrtLinkWaitSec--;
                        return ;
                    }
                                                    //等待超时后再次启动重联
                    if(gs_FramMisData.e1pt != gs_FramMisData.e1cnt)  
                    {
                        gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FARP_PRIOCNT], 1);
                        guc_AlrtLinkWaitSec = 60;
                        return;
                    }
                }
            
                return;
            }

        
//	            ALRT_SendData(0);
//	            gs_FramMisData.e1pt++;
            for(;gs_FramMisData.e1pt != gs_FramMisData.e1cnt;)
            {
                ALRT_SendData(0);
                gs_FramMisData.e1pt++;
                HB_RetLive(TASK_SLV_TKID);
            }

            gs_SysVar.mLPstt &= ~HLV_LPTASK_LOSTVOL;            

            
            if(gs_SysVar.mDGstt & HLV_STT_NENG)
            {
                if((gs_SysVar.mLPstt & (HLV_LPTASK_CP | HLV_LPTASK_RP | HLV_LPTASK_LOSTVOL))== 0)
                {
                    gs_SysVar.mLPstt |= HLV_LPTASK_CLSCK;
                    gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_ENTER_LP], 1);
                }
            }
        }
//	        SaveFramMisData(MIS_E1P);
    }
//	    else if(gs_FramMisData.e2pt != gs_FramMisData.e2cnt)
//	    {
//	        ALRT_SendData(1);
//	        gs_FramMisData.e2pt++;
//	        SaveFramMisData(MIS_E2P);
//	    }
}












/************************************************************************
 * @function: ALRT_ERC1
 * @描述: 数据初始化和版本变更记录
 * 
 * @参数: 
 * @param: pdata MSG_ALRT_ERC1 参数及数据初始化  /NULL 版本变更判断
 * @返回: 
 * @说明: 1.版本变化(状态驱动,每分钟判断)  2.参数及数据区初始化(消息驱动)
 * @作者: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_ERC1(uint8 pdata)
{
#if ERC1_EN == 1
//	    uint8 dat[32];
//	    if(pdata == MSG_ALRT_ERC1)
//	    {
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);                   //记录参数初始化事件
//	        MoveBuffer(dat + 1, dat, 5);
//	        dat[5] = 0x01;
//	        ByteArrayBcdToHexString((uint8*)&gul_UsrFuncVer, &dat[6], 2, false);
//	        ByteArrayBcdToHexString((uint8*)&gul_UsrFuncVer, &dat[10], 2, false);
//	        ALRT_WriteLog(1, dat, 14);
//	    }
//	    else
//	    {
//	        if(gul_UsrFuncVer != gs_FramMisData.ver)
//	        {
//	            gs_RealTime.ReadBCDDateTime((TIME*)dat);               //记录版本变化事件
//	            MoveBuffer(dat + 1, dat, 5);
//	            dat[5] = 0x02;
//	            ByteArrayBcdToHexString((uint8*)&gs_FramMisData.ver, &dat[6], 2, false);
//	            ByteArrayBcdToHexString((uint8*)&gul_UsrFuncVer, &dat[10], 2, false);
//	            ALRT_WriteLog(1, dat, 14);
//	                                                //更新备份版本号到fram
//	            gs_FramMisData.ver = gul_UsrFuncVer;
//	            SaveFramMisData(MIS_VER);
//	            ERC44_Collctor_Updata();
//	        }
//	    }
#endif
}



/************************************************************************
**函数名称: void ALRT_ERC4(void)
**描 述: 状态量变位记录
**
**输 入: 
**输 出: 
**
**备 注: 需要每秒钟处理
**--------------------------------------------------------
**作 者: yzy
**日 期: 2009-05-31
*************************************************************************/
//	void ALRT_ERC4(void)
//	{
//	#if ERC4_EN == 1
//	    uint8 chg;                          //变位标志,BS2
//	    uint8 dat[32];
//	
//	    chg = gs_MKStruct.mFall;
//	    chg |= gs_MKStruct.mRaise;
//	    gs_MKStruct.mFall = 0;              //清零结束标志
//	    gs_MKStruct.mRaise = 0;             //清零发生标志
//	
//	    if(chg)
//	    {
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);//mmHHDDMMYY
//	        dat[5] = chg;                   //变位标志
//	        dat[6] = gs_MKStruct.mState;    //变位后状态
//	                                        //写入事件
//	        ALRT_WriteLog(4, dat, 7);
//	    }
//	#endif
//	}




/************************************************************************
 * @function: ALRT_ParaChg
 * @描述: ERC3参数变更记录 
 * 
 * @参数: 
 * @param: host 主站地址
 * @param: pdi 参数变更DI表的指针
 * @param: di_no DI数量(最多20,大于20则记录前20条)
 * @返回: 
 * @说明: 由外部帧处理进程调用
 * @作者: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_ParaChg(uint8 host, uint8* pdi, uint8 di_no)
{
#if ERC3_EN == 1
    uint8 dat[90];
    gs_RealTime.ReadBCDDateTime((TIME*)dat);
    MoveBuffer(dat + 1, dat, 5);
                                                //主站地址
    dat[5] = host;
                                                //标识
    di_no = (di_no > 20) ? 20 : di_no;
    MoveBuffer(pdi, &dat[6], di_no << 2);
    ALRT_WriteLog(3, dat, 6 + (di_no << 2));
#endif
}



/************************************************************************
 * @function: ALRT_ERC8
 * @描述: 电能表参数变更
 * 
 * @参数: 
 * @param: tstno 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void ALRT_ERC8(uint8 tstno)
//	{
//	#if ERC8_EN == 1
//	    uint8 bit_chg;
//	    uint8 dat[32];
//	
//	    _IF_TRUE_DO(gss_CDATATst[tstno].ef != 0x01, return);
//	
//	    bit_chg = 0;                        //变化标志清零
//	    dat[7] = 0;
//	                                        //费率时段 bit0
//	    if(gss_ALRTStt[tstno].ewr1.gsd !=  0)
//	    {
//	        dat[7] |= 0x01;
//	        bit_chg = 1;
//	        gss_ALRTStt[tstno].ewr1.gsd = 0;
//	    }
//	                                        //编程时间 bit1
//	    if(gss_ALRTStt[tstno].ewr1.gsj != 0)
//	    {
//	        dat[7] |= 0x02;
//	        bit_chg = 1;
//	        gss_ALRTStt[tstno].ewr1.gsj = 0;
//	    }
//	                                        //抄表日 bit2
//	    if(gss_ALRTStt[tstno].ewr1.cbr != 0)
//	    {
//	        dat[7] |= 0x04;
//	        bit_chg = 1;
//	        gss_ALRTStt[tstno].ewr1.cbr = 0;
//	    }
//	                                        //脉冲常数 bit3
//	    if(gss_ALRTStt[tstno].ewr1.mcc != 0)
//	    {
//	        dat[7] |= 0x08;
//	        bit_chg = 1;
//	        gss_ALRTStt[tstno].ewr1.mcc = 0;
//	    }
//	                                        //互感器倍率 bit4
//	    if(gss_ALRTStt[tstno].ewr1.hgq != 0)
//	    {
//	        dat[7] |= 0x10;
//	        bit_chg = 1;
//	        gss_ALRTStt[tstno].ewr1.hgq = 0;
//	    }
//	                                        //需量清零时间 bit5
//	    if(gss_ALRTStt[tstno].ewr1.clr != 0)
//	    {
//	        dat[7] |= 0x20;
//	        bit_chg = 1;
//	        gss_ALRTStt[tstno].ewr1.clr = 0;
//	    }
//	
//	    if(bit_chg == 1)
//	    {
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	        IntToByte(dat + 5, tstno);
//	        ALRT_WriteLog(8, dat, 8);
//	    }
//	#endif
//	}




/************************************************************************
 * @function: ALRT_ERC12
 * @描述: 电能表时间超差
 * 
 * @参数: 
 * @param: tstno 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void ALRT_ERC12(uint8 tstno)
//	{
//	#if ERC12_EN == 1
//	    uint8 dat[32];
//	    _IF_TRUE_DO(gss_CDATATst[tstno].ef != 0x01, return);
//	                                            //若首次超差
//	    if(gss_ALRTStt[tstno].ewr1.tim == 1)
//	    {
//	        gss_ALRTStt[tstno].ewr1.tim = 0;
//	
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	        IntToByte(dat + 5, (0x8000 | tstno));
//	        ALRT_WriteLog(12, dat, 7);
//	    }
//	                                            //若首次停止超差
//	    if(gss_ALRTStt[tstno].ewf1.tim == 1)
//	    {
//	        gss_ALRTStt[tstno].ewf1.tim = 0;
//	
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	        IntToByte(dat + 5, tstno);
//	        ALRT_WriteLog(12, dat, 7);
//	    }
//	#endif
//	}
//	



/************************************************************************
 * @function: ALRT_ERC13
 * @描述: 电表故障信息
 * 
 * @参数: 
 * @param: tstno 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void ALRT_ERC13(uint8 tstno)
//	{
//	#if ERC13_EN == 1
//	    uint8 dat[32];
//	    uint8 iserr = 0;
//	    dat[7] = 0;
//	                                            //编程次数/需量清零次数变化
//	    if(gss_ALRTStt[tstno].ewr1.bcb || gss_ALRTStt[tstno].ewr1.xlb)
//	    {
//	        gss_ALRTStt[tstno].ewr1.bcb = 0;
//	        gss_ALRTStt[tstno].ewr1.xlb = 0;
//	        dat[7] |= 0x01;
//	        iserr = 1;
//	    }
//	                                            //断相次数变化
//	    if(gss_ALRTStt[tstno].ewr1.dxb)
//	    {
//	        gss_ALRTStt[tstno].ewr1.dxb = 0;
//	        dat[7] |= 0x02;
//	        iserr = 1;
//	    }
//	                                            //失压次数变化
//	    if(gss_ALRTStt[tstno].ewr1.syb)
//	    {
//	        gss_ALRTStt[tstno].ewr1.syb = 0;
//	        dat[7] |= 0x04;
//	        iserr = 1;
//	    }
//	                                            //停电次数变化
//	    if(gss_ALRTStt[tstno].ewr1.tdb)
//	    {
//	        gss_ALRTStt[tstno].ewr1.tdb = 0;
//	        dat[7] |= 0x08;
//	        iserr = 1;
//	    }
//	                                            //电池欠压
//	    if(gss_ALRTStt[tstno].ewr1.dcd)
//	    {
//	        gss_ALRTStt[tstno].ewr1.dcd = 0;
//	        dat[7] |= 0x10;
//	        iserr = 1;
//	    }
//	
//	    if(iserr)
//	    {
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	        IntToByte(dat + 5, tstno);
//	        ALRT_WriteLog(13, dat, 8);
//	    }
//	#endif
//	}



/************************************************************************
 * @function: ALRT_ERC14
 * @描述: 上电事件记录
 * 
 * @参数: 
 * @param: type 0上电. 1停电
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_ERC14(uint8 type)
{
#if ERC14_EN == 1
    uint8 buffer[32];
    uint8 tn[10];
    int m = 0;
//	    gs_RealTime.ReadBCDDateTime((TIME*)tn);
    
    if(type == 0)                       //上电事件
    {
//	        MoveBuffer(gs_FramMisData.tdt, buffer, 5);
//	        InvBuffer(buffer, 5);
//	        ByteArrayHexToBcd(buffer, buffer, 5);
//	        MoveBuffer((uint8*)&GetTime()->min, buffer + 5, 5);
//	        ByteArrayHexToBcd(buffer + 5, buffer + 5, 5);
        buffer[m++] = 0x00;

        buffer[m++] = SYS_AD_GetValue(AD_ID_CHECKVOL) / 100;  
        
    }
    else if(type == 1)                       //停电事件
    {
//	        MoveBuffer(gs_FramMisData.tbk, buffer, 5);
//	        InvBuffer(buffer, 5);
//	        ByteArrayHexToBcd(buffer, buffer, 5);
//	        memset(buffer + 5, 0xEE, 5);
        buffer[m++] = 0x01;
        buffer[m++] = SYS_AD_GetValue(AD_ID_CHECKVOL) / 100;        
    }
    else
    {
        return ;
    }
    
    memcpy(buffer + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
    m+=4;
    ALRT_WriteLog(ERR_TYPE_LOSTVOL, buffer, m);
#endif

}




/************************************************************************
 * @function: ALRT_ERC21
 * @描述: 终端故障记录
 * 
 * @参数: 
 * @param: port 抄表端口号
 * @返回: 
 * @说明: 支持项:单路485抄表故障.
 *        单块485表(非所有)抄不到时不应记录
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void ALRT_ERC21(uint8 port)
//	{
//	#if ERC21_EN == 1
//	    uint8 dat[10];
//	    uint8 k;
//	                                    
//	    gs_RealTime.ReadBCDDateTime((TIME*)dat);//ssmmHHDDMMYY
//	    MoveBuffer(dat + 1, dat, 5);
//	    dat[5] = 4;
//	    ALRT_WriteLog(21, dat, 6);
//	                                    //清除该路485上所有测量点的标志
//	    for(k = 0; k < CORE_TST_NO; k++)
//	    {
//	        if((gss_CDATATst[k].tf == 1) && (gss_CDATATst[k].pt == port))
//	        {
//	            gss_ALRTStt[k].ewr1.trn = 0;
//	            gss_ALRTStt[k].ewf1.trn = 0;
//	        }
//	    }
//	#endif
//	}



/************************************************************************
 * @function: ALRT_ERC27_28_29_30
 * @描述: 电能量异常(示度下降,超差,飞走,停走)
 * 
 * @参数: 
 * @param: nWp 
 * @param: tstno 
 * @返回: 
 * @说明: 1.不产生恢复事件. 2.总清电能示值后 可能导致 该事件发生
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void ALRT_ERC27_28_29_30(uint32 nWp, uint8 tstno)
//	{
//	    _IF_TRUE_DO(gss_CDATATst[tstno].ef != 0x01, return);
//	
//	#if ERC27_EN == 1
//	    uint8 dat[24];
//	    
//	    if(gss_ALRTStt[tstno].ewr1.szj)//ERC27示度下降发生
//	    {
//	        gss_ALRTStt[tstno].ewr1.szj = 0;
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, (0x8000 | tstno));
//	        Hex42ToGD14(gss_TstRealData[tstno].Wp[0], dat + 7);
//	        Hex42ToGD14(nWp, dat + 12);
//	
//	        ALRT_WriteLog(27, dat, 17);
//	    }
//	
//	    if(gss_ALRTStt[tstno].ewf1.szj)//ERC27示度下降恢复
//	    {
//	        gss_ALRTStt[tstno].ewf1.szj = 0;
//	        /*
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, tstno);
//	        Hex42ToGD14(gss_TstRealData[tstno].Wp[0], dat + 7);
//	        Hex42ToGD14(nWp, dat + 12);
//	
//	        ALRT_WriteLog(27, dat, 17);
//	        */
//	    }
//	#endif
//	
//	#if ERC28_EN == 1
//	    if(gss_ALRTStt[tstno].ewr1.fast)//ERC28超差发生
//	    {
//	        gss_ALRTStt[tstno].ewr1.fast = 0;
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, (0x8000 | tstno));
//	        Hex42ToGD14(gss_TstRealData[tstno].Wp[0] , dat + 7);
//	        Hex42ToGD14(nWp , dat + 12);
//	        dat[17] = ByteHexToBcd(gs_EVTComVar.wfast);
//	
//	        ALRT_WriteLog(28, dat, 18);
//	    }
//	
//	    if(gss_ALRTStt[tstno].ewf1.fast)//ERC28超差恢复
//	    {
//	        gss_ALRTStt[tstno].ewf1.fast = 0;
//	        /*
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, tstno);
//	        Hex42ToGD14(gss_TstRealData[tstno].Wp[0] , dat + 7);
//	        Hex42ToGD14(nWp , dat + 12);
//	        dat[17] = ByteHexToBcd(gs_EVTComVar.wfast);
//	
//	        ALRT_WriteLog(28, dat, 18);
//	        */
//	    }
//	#endif
//	
//	#if ERC29_EN == 1
//	    if(gss_ALRTStt[tstno].ewr1.fly)//ERC29飞走发生
//	    {
//	        gss_ALRTStt[tstno].ewr1.fly = 0;
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, (0x8000 | tstno));
//	        Hex42ToGD14(gss_TstRealData[tstno].Wp[0] , dat + 7);
//	        Hex42ToGD14(nWp, dat + 12);
//	        dat[17] = ByteHexToBcd(gs_EVTComVar.wfly);
//	
//	        ALRT_WriteLog(29, dat, 18);
//	    }
//	
//	    if(gss_ALRTStt[tstno].ewf1.fly)//ERC29飞走恢复
//	    {
//	        gss_ALRTStt[tstno].ewf1.fly = 0;
//	        /*
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, tstno);
//	        Hex42ToGD14(gss_TstRealData[tstno].Wp[0] , dat + 7);
//	        Hex42ToGD14(nWp, dat + 12);
//	        dat[17] = ByteHexToBcd(gs_EVTComVar.wfly);
//	
//	        ALRT_WriteLog(29, dat, 18);
//	        */
//	    }
//	#endif
//	
//	#if ERC30_EN == 1
//	    if(gss_ALRTStt[tstno].ewr1.bjt)//ERC30 停走发生
//	    {
//	        gss_ALRTStt[tstno].ewr1.bjt = 0;
//	
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, (0x8000 | tstno));
//	        Hex42ToGD14(nWp, dat + 7);
//	        dat[12] = gs_EVTComVar.stop;
//	
//	        ALRT_WriteLog(30, dat, 13);
//	    }
//	
//	    if(gss_ALRTStt[tstno].ewf1.bjt)//ERC30 停走恢复
//	    {
//	        gss_ALRTStt[tstno].ewf1.bjt = 0;
//	        /*
//	        gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	        MoveBuffer(dat + 1, dat, 5);
//	
//	        IntToByte(dat + 5, tstno);
//	        Hex42ToGD14(nWp, dat + 7);
//	        dat[12] = gs_EVTComVar.stop;
//	
//	        ALRT_WriteLog(30, dat, 13);
//	        */
//	    }
//	#endif
//	}

//	0x00：通信恢复正常
//	0x01：无效的指令
//	0x02：设备无回应
//	0x03：数据帧校验失败
//	0x04：指令返回错误
//	0x05：数据异常(压力<=0、流量等为负数)


/************************************************************************
 * @function: ALRT_ERC31
 * @描述: 485抄表失败事件
 * 
 * @参数: 
 * @param: tstno 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_ERC31(uint8 tstno, uint8_t type)
{
#if ERC31_EN == 1
    uint8 dat[77];
    int m = 0;
    memset(dat,0,70);
//	    void ALRT_ERC31_COMMONIN(uint8* dat, uint8 tstno);

    if(gs_SysVar.mDGstt & HLV_STT_NENG || tstno >= CORE_TST_NO)
    {
        return;
    }

#if 0  

    if(gss_ALRTStt[tstno].ewr1.trn & 0x01)
    {
        gss_ALRTStt[tstno].ewr1.trn = 0;
      
                                            //记录事件
        dat[m++] = ERR_CODE_485_NORSP;
        dat[m++] = tstno+1;
        dat[m++] = gss_CDATATst[tstno].info.main_addr;
        
        dat[m++] = gss_CDATATst[tstno].pf;
        m+=66;
//	        IntToByte(dat + 5, (0x8000 | tstno));
//	        ALRT_ERC31_COMMONIN(dat, tstno);
        ALRT_WriteLog(ERR_TYPE_485, dat, m);
    }

    if(gss_ALRTStt[tstno].ewf1.trn & 0x01)
    {
        gss_ALRTStt[tstno].ewf1.trn = 0;
                                            //记录事件
//	        IntToByte(dat + 5, tstno);
//	        ALRT_ERC31_COMMONIN(dat, tstno);
        dat[m++] = ERR_CODE_485_NORMAL;
        dat[m++] = tstno+1;
        dat[m++] = gss_CDATATst[tstno].info.main_addr;
        
        dat[m++] = gss_CDATATst[tstno].pf;
        m+=66;

        ALRT_WriteLog(ERR_TYPE_485, dat, m);
    }
#endif   
    if(gss_ALRTStt[tstno].ewr1.trn & 0x01)
    {
        gss_ALRTStt[tstno].ewr1.trn = 0;
      
                                            //记录事件
        dat[m++] = 0x01;
        dat[m++] = tstno+1;
//	        dat[m++] = gss_CDATATst[tstno].info.main_addr;
//	        
//	        dat[m++] = gss_CDATATst[tstno].pf;
//	        m+=66;
//          IntToByte(dat + 5, (0x8000 | tstno));
//          ALRT_ERC31_COMMONIN(dat, tstno);
//	        ALRT_WriteLog(ERR_TYPE_CP, dat, m);
        memcpy(dat + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
        m+=4;
        ALRT_WriteLog(ERR_TYPE_CP, dat, m);

    }

    if(gss_ALRTStt[tstno].ewf1.trn & 0x01)
    {
        m = 0;
        gss_ALRTStt[tstno].ewf1.trn = 0;
                                            //记录事件
//          IntToByte(dat + 5, tstno);
//          ALRT_ERC31_COMMONIN(dat, tstno);
        dat[m++] = 0;
        dat[m++] = tstno+1;
//	        dat[m++] = gss_CDATATst[tstno].info.main_addr;
//	        
//	        dat[m++] = gss_CDATATst[tstno].pf;
//	        m+=66;
        memcpy(dat + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
        m+=4;
        ALRT_WriteLog(ERR_TYPE_CP, dat, m);

    }

#endif
}


//	void ALRT_ERC31_COMMONIN(uint8* dat, uint8 tstno)
//	{
//	    uint8 tmp[10];
//	
//	    gs_RealTime.ReadBCDDateTime((TIME*)tmp);
//	    MoveBuffer(tmp + 1, dat, 5);
//	
//	    ByteArrayHexToBcd(gss_TstRealData[tstno].rtime.YMDhms, dat + 7, 5);
//	    InvBuffer(dat + 7, 5);
//	    Hex42ToGD14(gss_TstRealData[tstno].Wp[0], dat + 12);
//	    Hex42ToGD11(0, dat + 17);
//	}





/************************************************************************
**函数名称: void ALRT_ERC32(void)
**描 述: 通信流量超限事件
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-10-12
*************************************************************************/
//	void ALRT_ERC32(void)
//	{
//	#if ERC32_EN == 1
//	    uint32 GetMonthFlux(void);
//	    uint32 flux;
//	    uint8 dat[20];
//	    
//	    if(gs_EVTComVar.mflux != 0)
//	    {
//	        flux = GetMonthFlux();
//	        if(flux > gs_EVTComVar.mflux)
//	        {
//	            if(gss_ALRTStt[0].ew1.llc == 0)
//	            {
//	                gss_ALRTStt[0].ew1.llc = 1;
//	
//	                gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	                MoveBuffer(dat + 1, dat, 5);
//	                LongToByte(dat + 5, flux);
//	                LongToByte(dat + 9, gs_EVTComVar.mflux);
//	                ALRT_WriteLog(32, dat, 13);
//	            }
//	        }
//	        else
//	        {
//	            gss_ALRTStt[0].ew1.llc = 0;
//	        }
//	    }
//	#endif
//	}


/************************************************************************
 * @function: ALRT_ERC33
 * @描述: 电表状态字变位事件
 * 
 * @参数: 
 * @param: pnew 当前状态字 
 * @param: pold 之前状态字
 * @param: tstno 测量点
 * 
 * @返回: 
 * @return: uint8  true 成功 /false 失败(或不需记录)
 * @说明: 
 * @作者: yzy (2014/2/10)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 ALRT_ERC33(uint8* pnew, uint8* pold, uint8 tstno)
//	{
//	#if ERC33_EN == 1
//	    uint8 dat[40];
//	    gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	    MoveBuffer(dat + 1, dat, 5);
//	
//	    IntToByte(dat + 5, tstno);
//	    STR_Xor(pnew, pold, dat + 7, 14);
//	    MoveBuffer(pnew, dat + 21, 14);
//	
//	    return ALRT_WriteLog(33, dat, 35);
//	#else
//	    return false;
//	#endif
//	}



/************************************************************************
 * @function: ALRT_ERC37
 * @描述: 开表盖事件
 * 
 * @参数: 
 * @param: buffer 从发生时刻开始的详细信息.从表内获取
 * @param: cnt 开盖次数
 * @param: tstno 测量点号
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 ALRT_ERC37(uint8* buffer, uint32 cnt, uint8 tstno)
//	{
//	#if ERC37_EN == 1
//	    uint8 time[10];
//	    MoveBuffer(buffer, buffer + 10, 60);
//	    gs_RealTime.ReadBCDDateTime((TIME*)time);
//	    MoveBuffer(time + 1, buffer, 5);
//	    IntToByte(buffer + 5, tstno);
//	    HexToBcd(buffer + 7, cnt, 3, 4);
//	    return ALRT_WriteLog(37, buffer, 70);
//	#else
//	    return false;
//	#endif
//	}



/************************************************************************
 * @function: ALRT_ERC38
 * @描述: 开端钮盒事件
 * 
 * @参数: 
 * @param: buffer 从发生时刻开始的详细信息.从表内获取
 * @param: cnt 开端钮盒次数
 * @param: tstno 测量点
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 ALRT_ERC38(uint8* buffer, uint32 cnt, uint8 tstno)
//	{
//	#if ERC38_EN == 1
//	    uint8 time[10];
//	    MoveBuffer(buffer,buffer + 10, 60);
//	    gs_RealTime.ReadBCDDateTime((TIME*)time);
//	    MoveBuffer(time + 1 , buffer, 5);
//	    IntToByte(buffer + 5, tstno);
//	    HexToBcd(buffer + 7, cnt, 3, 4);
//	    return ALRT_WriteLog(38, buffer, 70);
//	#else
//	    return false;
//	#endif
//	}

/************************************************************************
 * @function: ALRT_ERC39_46
 * @描述: 发现新表/删除电表 事件记录
 * 
 * @参数: 
 * @param: tstno 
 * @param: tstpo 
 * @param: buf 
 * @param: ercid 
 * @返回: 
 * @return: uint8  
 * @说明: 塔吉克斯坦
 * @作者: ZJC (2014/8/22)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 ALRT_ERC39_46(uint16 tstno,uint16 tstpo, uint8 *buffer, uint8 ercid)
//	{
//	#if ERC39_DLMS_EN == 1 || ERC46_DLMS_EN == 1
//	    uint8 time[10];
//	    uint8 buf[38];
//	    
//	    IntToByte(buffer, tstno);    //
//	    IntToByte(buffer + 2, tstpo);    //
//	    
//	    gs_RealTime.ReadBCDDateTime((TIME*)time);
//	    MoveBuffer(time + 1 , buf , 5);
//	    //IntToByte(buf + 5, tstno);    //
//	    MoveBuffer(buffer, buf + 5, LEN_TST_CFG);
//	    //MoveBuffer(time ,buf + 5 + LEN_TST_CFG, 6);  //发生时刻
//	    return ALRT_WriteLog(ercid, buf, 32);
//	#endif
//	}
/************************************************************************
 * @function: ALRT_ERC40
 * @描述: 磁场异常事件记录
 * 
 * @参数: 
 * @param: tstno 
 * @param: type 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 ALRT_ERC40(uint8 tstno, uint8 type)
//	{
//	#if ERC40_EN == 1
//	    uint8 time[10];
//	    uint8 buffer[20];
//	    gs_RealTime.ReadBCDDateTime((TIME*)time);
//	    MoveBuffer(time + 1 , buffer , 5);
//	    IntToByte(buffer + 5, tstno);
//	    if((gss_CDATATst[tstno].pt == 0)||(gss_CDATATst[tstno].pt == 1))
//	    {
//	         buffer[7] = 0x02;     //电能表     
//	    }
//	    else
//	    {
//	         buffer[7] = 0x00;     //终端外接传感器
//	    }
//	    MoveBuffer(gss_CDATATst[tstno].ad ,buffer + 8 , 6);
//	    buffer[14] = type;
//	    return ALRT_WriteLog(40, buffer, 15);
//	    
//	#else
//	    return false;
//	#endif
//	}
/************************************************************************
 * @function: ALRT_DLMS_ERC40
 * @描述: 磁场异常事件记录
 * 
 * @参数: 
 * @param: tstno 
 * @param: type 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 塔吉克斯坦，表计标准事件
 * @作者: ZJC (2014/8/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 ALRT_DLMS_ERC40_41_42_43(uint8 tstno, uint8 type, uint8 *inbuf, uint8 inbuflen)
//	{
//	#if (ERC40_DLMS_EN == 1) || (ERC41_DLMS_EN == 1) || (ERC42_DLMS_EN == 1) || (ERC43_DLMS_EN == 1)
//	
//	    uint8 time[10];
//	    uint8 buffer[14];
//	
//	    gs_RealTime.ReadBCDDateTime((TIME*)time);
//	    MoveBuffer(time + 1 , buffer , 5);
//	    IntToByte(buffer + 5, tstno);
//	    //IntToByte(buffer + 7, tstno);
//	    MoveBuffer(inbuf ,buffer + 7 , inbuflen);
//	    type += 39;
//	    return ALRT_WriteLog(type, buffer, 14);
//	#else
//	    return false;
//	#endif
//	}


/************************************************************************
 * @function: ALRT_ERC41
 * @描述: 对时事件记录
 * 
 * @参数: 
 * @param: oldtime bcd格式
 * @param: newtime bcd格式
 * @param: tstno 
 * 
 * @返回: 
 * @return: uint8  true 成功 /false 失败(或不需记录)
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 ALRT_ERC41(uint8* oldtime, uint8* newtime, uint8 tstno)
//	{
//	#if ERC41_EN == 1
//	    uint8 buffer[20];
//	    MoveBuffer(oldtime, buffer + 2, 6);
//	    MoveBuffer(newtime, buffer + 8, 6);
//	    IntToByte(buffer, tstno); 
//	    return ALRT_WriteLog(41, buffer, 14);
//	#else
//	    return false;
//	#endif
//	}

/************************************************************************
 * @function: ALRT_DLMS_ERC44
 * @描述: 采集器升级事件
 * 
 * @参数: 
 * @param: tstno 
 * @param: type 置“1”：表计程序升级，置“2”：表计参数设置，置“3”：集中器程序升级，其他备用；
 * @param: flag 置“1”：成功，置“0”：失败；
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 塔吉克斯坦，
 * @作者: ZJC (2014/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 ALRT_DLMS_ERC44(uint8 tstno, uint8 type, uint8 flag, ST_ACT_INPBUF_S *stpinbuf)
//	{
//	#if ERC47_DLMS_EN == 1
//	    uint8 time[10];
//	    uint8 buffer[41];
//	    ClearBuffer(buffer, 41);
//	    
//	    gs_RealTime.ReadBCDDateTime((TIME*)time);
//	    MoveBuffer(time + 1 , buffer , 5);
//	    IntToByte(buffer + 5, tstno);
//	    buffer[7] = type;
//	    buffer[8] = flag;
//	    if(stpinbuf->len > 32)
//	    {
//	        stpinbuf->len = 32;
//	    }
//	    MoveBuffer(stpinbuf->buf, buffer + 9 , stpinbuf->len);
//	    
//	    return ALRT_WriteLog(44, buffer, 41);
//	#else
//	    return false;
//	#endif
//	}

/************************************************************************
 * @function: ALRT_DLMS_ERC47
 * @描述: 对表计校时失败事件
 * 
 * @参数: 
 * @param: tstno 
 * @param: type  事件代码（1：由于表计不支持对时操作，2：代表对时超出表计设置阀值，3：由于通信失败引起的对时失败操作）
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 塔吉克斯坦，
 * @作者: ZJC (2014/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 ALRT_DLMS_ERC47(uint8 tstno, uint8 type)
//	{
//	#if ERC47_DLMS_EN == 1
//	    uint8 time[10];
//	    uint8 buffer[14];
//	    
//	    gs_RealTime.ReadBCDDateTime((TIME*)time);
//	    MoveBuffer(time + 1 , buffer , 5);
//	    IntToByte(buffer + 5, tstno);
//	    buffer[7] = type;
//	    MoveBuffer(time, buffer + 8 , 6);
//	    return ALRT_WriteLog(47, buffer, 14);
//	#else
//	    return false;
//	#endif
//	}
//	

/***********************************************************
 * @function: ALRT_ERC60
 * @描述: 请求测试事件记录
 * 
 * @参数: 
 * @param: ext  
 * 
 * @返回: 
 * @return: uint8   
 * @作者: yzy (2010-3-30)
 *---------------------------------------------------------
 * @修改人: 
 **********************************************************/
//	uint8 ALRT_ERC60(uint32 ext)
//	{
//	#if ERC60_EN == 1
//	    uint8 dat[40];
//	    gs_RealTime.ReadBCDDateTime((TIME*)dat);
//	    MoveBuffer(dat + 1, dat, 5);
//	    LongToByte(dat + 5, ext);
//	
//	    return ALRT_WriteLog(60, dat, 9);
//	#endif
//	}



/************************************************************************
**函数名称: void ERC44_Collctor_Updata(void* pdata)
**描 述: 上电后判断采集器是否升级成功
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: zjc
**日 期: 09-09-09
*************************************************************************/
//	void ERC44_Collctor_Updata(void)
//	{   
//	    ST_ACT_INPBUF_S    inSbuf;
//	    GD_Para_RW(VER_HEXING, inSbuf.buf, 20, false);    //读取FLASH中的内测版本号
//	    if(inSbuf.buf[0] == 'F' && inSbuf.buf[1] == 'W')
//	    {
//	        extern const uint8 DefParaC1F21F22[20];
//	        inSbuf.len = 20;
//	        if(CmpBuffer((uint8 *)DefParaC1F21F22, inSbuf.buf, 20) == 0)   //版本完全相同，升级成功
//	        {
//	            ALRT_DLMS_ERC44(0 , 3 , 1, &inSbuf);
//	        }
//	        else
//	        {
//	            ALRT_DLMS_ERC44(0 , 3 , 0, &inSbuf);
//	        }
//	        memset(inSbuf.buf, 0xFF, 20);
//	        GD_Para_RW(VER_HEXING, inSbuf.buf, 20, true);    //清除FLASH中的内测版本号
//	    }
//	}


/************************************************************************
**函数名称: void ERC44_MT_Updata(void* pdata)
**描 述: 表计升级是否成功
**
 * @参数: tsno  测量点
 * @参数: okflag  1： 升级成功 ；0：升级失败
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: zjc
**日 期: 09-09-09
*************************************************************************/
//	void ERC44_MT_Updata(uint8 tsno, uint8 okflag)
//	{   
//	    ST_ACT_INPBUF_S    inSbuf;
//	    TDataBlock tdb;
//	    ES_FILE_DB_Open(DB_MT_UPDATE_PARA, &tdb, TDB_MODE_RW);
//	    ES_FILE_DB_ReadFrom(&tdb, (uint8 *)&g_Updata, sizeof(EXT_UPDATASTRUCT), 0);
//	    
//	    inSbuf.len = 20;
//	    MoveBuffer(g_Updata.mtVer, inSbuf.buf, 20);
//	
//	    if(okflag == 1)
//	    {
//	        g_Updata.FlagState[g_Updata.tstno] = METER_UPGRADE_OK;
//	    }
//	    else
//	    {
//	        g_Updata.FlagState[g_Updata.tstno] = METER_UPGRADE_BAD;
//	    }
//	
//	    ES_FILE_DB_WriteFrom(&tdb, (uint8 *)&g_Updata, sizeof(EXT_UPDATASTRUCT), 0);
//	    ES_FILE_DB_Close(&tdb);
//	    
//	    ALRT_DLMS_ERC44(tsno , 1 , okflag, &inSbuf);
//	}
//	


/************************************************************************
 * @function: ALRT_SecProc
 * @描述: 秒处理函数
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void ALRT_SecProc(void)
{
    ALRT_ERC1(0);                       //版本变更判断
//	    ALRT_ERC4();                        //状态量变位

    
    ALRT_ActiveReport();                //主动上报事件

}



/************************************************************************
**函数名称: void ALRT_MinProc(void* pdata)
**描 述: 分处理函数
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 
*************************************************************************/
//	void ALRT_MinProc(void)
//	{
//	    ALRT_ERC32();                               //流量超限
//	}






/************************************************************************
**函数名称: void ALRT_TstProc(void* pdata)
**描 述: 外部测量点事件处理
**
**输 入: 
**输 出: 
**
**备 注: 测量点统计结束后(每个抄表间隔)执行
**--------------------------------------------------------
**作 者: yzy
**日 期: 2008-11-29
*************************************************************************/
//	void ALRT_TstProc(void)
//	{
//	    for(uint8 uc_i = 0; uc_i < CORE_TST_NO; uc_i++)
//	    {                                       //测量点无效或测量点非485表
//	        if((gss_CDATATst[uc_i].ef != 0x01) || (gss_CDATATst[uc_i].tf != 0x01))
//	        {
//	            continue;
//	        }
//	
//	        ALRT_ERC12(uc_i);                   //时间超差
//	        ALRT_ERC13(uc_i);                   //电表故障
//	    }
//	}



/************************************************************************
**函数名称: void ALRT_CLR(void* pdata)
**描 述: 清除所有事件记录
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 09-12-30
*************************************************************************/
void ALRT_CLR(void)
{
    TDataBlock db;
//	    ES_FILE_DB_Open(DB_EVT, &db, TDB_MODE_RW);
//	    for(uint16 j = 0; j < 24; j++)
//	    {
//	        ES_FILE_DB_WriteFrom(&db, (void*)0, 4096, j * 4096);
//	    }
//	    ES_FILE_DB_Close(&db);
    CBB_FILE_Erase_Event(DB_FLASH);
}



/************************************************************************
**函数名称: void ALRT_Init(void* pdata)
**描 述: 告警进程初始化
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 09-09-09
*************************************************************************/
void ALRT_Init(void)
{
    //do nothing
    
}






/************************************************************************
 * @function: ALRT_DayProc
 * @描述: 日处理函数 
 * 
 * @参数: 
 * @param: pdata 
 * @返回: 
 * @说明: 
 * @作者: Zhenggb (2012-5-16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void ALRT_DayProc(void)
//	{                                   //1)更新当月月统计冻结
//	     STAT_TMMonSave();
//	     if(GetTime()->dmon == 1)
//	     {
//	         STAT_TMMonCrack();
//	     }
//	                                    //2)保存上日统计冻结
//	     STAT_TMDaySave();
//	     STAT_TMDayCrack();
//	                                    //3)清空当日统计实时数据
//	     STAT_Reset();
//	     
//	     gdw_SaveTstDayStat();             //电能表电压统计 日冻结 
//	     
//	}

/************************************************************************
 * @function: ALRT_DayProc
 * @描述: 日处理函数 
 * 
 * @参数: 
 * @param: pdata 
 * @返回: 
 * @说明: 
 * @作者: Zhenggb (2012-5-16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void ALRT_MonthProc(void)
//	{
//	     gdw_SaveTstMonthStat();
//	}





















/*******************************************************************************
 * @function_name:  ES_ALRT_Task
 * @function_file:  alert.c
 * @描述: 告警进程处理函数
 * 
 * 
 * @参数:  
 * 
 * @返回:  
 * @作者:  yzy (2011-1-24)
 *-----------------------------------------------------------------------------
 * @修改人:  
 * @修改说明: 
 ******************************************************************************/
//	void ES_ALRT_Task(void)
//	{
//	                                            //链表消息
//	    TESMsg* msg = (TESMsg*)gs_Mem.GetMEM(sizeof(TESMsg));
//	    uint8 nmsg;                             //数字消息
//	    
//	    ALRT_Init();
//	    
//	    ES_MSG_Apply(MSG_CLS_TM);
//	    
//	    for(;;)
//	    {
//	        nmsg = gs_OS.Message_Wait(&msg); //获取进程消息
//	        switch(nmsg)                         //根据消息分别处理
//	        {
//	            case MSG_SEC:                   //秒消息处理
//	                ALRT_SecProc();
//	                break;
//	                
//	            case MSG_MIN:                   //分消息处理
//	                ALRT_MinProc();
//	                break;
//	
//	            case MSG_DAY:                   //日消息处理
//	                ALRT_DayProc();
//	                break;
//	
//	            case MSG_MON:
//	                ALRT_MonthProc();
//	                break;
//	                
//	            case MSG_HLV_CLR:               //总清
//	                Inp_DataClear();
//	                break;
//	                
//	            case MSG_ALRT_MTSTAT:           //表计数据更新,统计完成
//	                ALRT_TstProc();
//	                break;
//	
//	            case MSG_ALRT_ERC1:             //参数及数据初始化
//	                ALRT_ERC1(MSG_ALRT_ERC1);
//	                break;
//	
//	            case MSG_ALRT_CLR:              //清除所有事件
//	                ALRT_CLR();
//	                break;
//	
//	            case MSG_ALRT_PARACKECK:        //启动搜表
//	                break;
//	                
//	            case MSG_LIVE:
//					HB_RetLive();
//	                break;
//	        }
//	        
//	    }
//	}
//	
//	/******************************************************************************
//	**应用程序主进程块常量部分定义
//	******************************************************************************/
//	const TESTcbConst gs_TkAlrt = 
//	{
//	    ES_ALRT_Task,
//	    TASK_ALRT_TKID >> 3,                    //应用管理进程
//	    TASK_ALRT_TKID,                         //进程编号
//	    TASK_ALRT_TKID & 7,                     //优先级内进程编号
//	    true,                                   //消息循环机制进程
//	};







