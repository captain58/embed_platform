/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: farp.c
**创   建   人: yzy
**最后修改日期: 2017年9月1日
**描        述: 主通道处理函数
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
*******************************************************************************/

#define EXT_FARP

#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"
#include "app.h"
#include <sal_import.h>
#include "atmodem.h"
#include "farp.h"
#include "hlv.h"
//#include "iot_import.h"

#include <network/network.h>


/******************************************************************************
**数据通讯状态断线重连安全时间数器
******************************************************************************/
uint16 gui_FarpWmSafeCnt;                   //数据通讯状态断线重连安全时间倒计时(秒)
uint16 gui_FarpSerSafeCnt;                   //数据通讯状态断线重连安全时间倒计时(秒)



/******************************************************************************
**重连间隔倒计时
******************************************************************************/
uint16 gui_FarpWmRcSec;                     //GPRS通路重连间隔计数器(秒倒计时)
uint16 gui_FarpSerRcSec;                     //GPRS Tcp Server通路重连间隔计数器(秒倒计时)


/******************************************************************************
**连续重连失败次数
******************************************************************************/
uint8 guc_FarpWmRcFailCnt;                  //GPRS连续重连失败次数
uint8 guc_FarpSerRcFailCnt;                 //Tcp Server连续重连失败次数
uint16 guc_FarpWmRcFailNorFtpCnt;                  //GPRS连续重连但是FTP在线


/******************************************************************************
**重连次数
******************************************************************************/
uint8 guc_FarpWmRcCnt;                      //联网失败后的重连次数(yzy:可在发起联网前赋值)
uint8 guc_FarpSerRcCnt;                     //联网失败后的重连次数(yzy:可在发起联网前赋值)



/******************************************************************************
**心跳周期计数器
******************************************************************************/
uint16 gui_FarpWmHBCYCnt;                   //心跳周期计数器


uint8 guc_FarpWmHKUnRpyCnt;                 //心跳无回复次数计数器




/******************************************************************************
**其它计数器
******************************************************************************/
uint16 gui_FarpHdRstCnt;                    //无线猫初始化花费时间计数器(如未插卡时会出现初始化失败)
uint8 guc_FarpPstTryCnt;                    //GPRS通路被动激活重拨次数计数器
uint8 guc_FarpNVHours;                      //无数据通讯小时数
uint8 guc_FastWmSttReqCnt;                  //MD 操作步骤延时

uint16 gui_FarpServerNoDataCnt;             //TCP Server  在侦听的情况下，无数据计数
uint16 gui_FarpFromClnNoDataCnt;             //TCP Server  与客户端1链接的情况下，无数据计数
uint8  guc_FarpFromClnCloseCnt;             //TCP Server  与客户端1链接的情况下, 主动关闭客户端次数

uint8  guc_FarpReActiveFlag;                //被动激活模式，采集器重启后，需要先与主站建立链接



uint16 gui_smseq;

cpu_stack_t  gs_TKFarpStack[TASK_FARP_STKL];
ktask_t      gs_TKFarpHandle;
kbuf_queue_t gs_TKFarpQueue;
char         gc_TKFarpQbuf[MSG_BUFF_LEN];
static char         g_TKFarpQ_buf_recv[BUFQUEUE_MSG_MAX+4];
static size_t       g_TKFarpQ_recv_size;
ksem_t gs_FarpProc;   
intptr_t g_ulTcp_fd = -1;

extern kbuf_queue_t gs_TKSlvQueue;
extern intptr_t HAL_TCP_Establish(const char *host, uint16_t port);
extern int32_t HAL_TCP_Destroy(intptr_t fd);
extern int32_t HAL_TCP_Read(intptr_t fd, char *buf, uint32_t len, uint32_t timeout_ms);
extern int32_t HAL_TCP_Write(intptr_t fd, const char *buf, uint32_t len,uint32_t timeout_ms);
    
/************************************************************************
 * @function: Farp_SendIpData
 * @描述: 远程通道发送GPRS的IP数据
 * 
 * @参数: 
 * @param: buffer 发送缓存
 * @param: len 发送的长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Farp_SendIpData(uint8* buffer, uint16 len)
{
                                            //IP数据在通道可用的情况下发送
    if(g_ulTcp_fd >=0 )
    {                                       //发送IP数据
//	        if(!SYS_MODM_SendIP(buffer, len))
        LOG_DUMPHEX(LOG_LEVEL_DEBUG, "send:\n", buffer, len);

        if(HAL_TCP_Write(g_ulTcp_fd, buffer, len,3000) >= 0)
        {
//	            gs_GPIO.LED_BlinkSet(LED_FAR_T, 0, 0, 0);
            SYS_OK();                        //发送成功
        }
        else
        {
            uint8 msg = MSG_FARP_DISGPRS;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            
            gui_FarpWmRcSec = 10;
        }
    }
    return SYS_ERR_FT;                      //IP数据发生失败
}


const uint8_t cmdUpBuf[]=
                   {0x68, 0x33, 0x19, 0x27, 0x03, 0x18, 0x20, 0x68, 0x85, 0x4C, 
                    0x00 ,0x18 ,0x03 ,0x27 ,0x20 ,0x11 ,0x22 ,0x12 ,0x01 ,0xAA ,
                    0x00 ,0x1D ,0x01 ,0x18 ,0x03 ,0x27 ,0x20 ,0x11 ,0x22 ,0x00 ,
                    0x00 ,0x00 ,0x00 ,0x00 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,
                    0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xA0 ,0x00 ,
                    0x96 ,0x40 ,0x08 ,0x07 ,0x64 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,
                    0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,
                    0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF ,
                    0xFF ,0xFF ,0x00 ,0xFF ,0xFF ,0x89 ,0x01 ,0x9C ,0x16 };
/************************************************************************
 * @function: Farp_SendLoginData
 * @描述: 远程通道发送登录帧
 * 
 * @参数: 
 * @param: ch 0:GPRS,1:ETH
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_SendLoginData(uint8 ch)
{
    gs_FarpVar.ldlen = VS_BuildLinkFrm(0x08, 0, gs_FarpVar.ld);
    if(gs_FarpVar.ldlen > 0)
    {
        if(ch == 0)
        {
            Farp_SendIpData(gs_FarpVar.ld, gs_FarpVar.ldlen);
            //Farp_SendIpData((uint8 *)cmdUpBuf, sizeof(cmdUpBuf));
        }
        else
        {
            //Farp_SendEthData(gs_FarpVar.ld, gs_FarpVar.ldlen);
        }
    }
}


/************************************************************************
 * @function: Farp_SendLoginOutData
 * @描述: 远程通道发送退出登录帧
 * 
 * @参数: 
 * @param: ch 0:GPRS,1:ETH
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_SendLoginOutData(uint8 ch)
{
    //gs_FarpVar.lodlen = GD_BuildLinkFrm(2, gs_FarpVar.lod);
    if(gs_FarpVar.lodlen > 0)
    {
        if(ch == 0)
        {
            Farp_SendIpData(gs_FarpVar.lod, gs_FarpVar.lodlen);
        }
        else
        {
            //Farp_SendEthData(gs_FarpVar.lod, gs_FarpVar.lodlen);
        }
    }
}

uint8_t g_ucRtuLogin;

/************************************************************************
 * @function: Farp_SendTestData
 * @描述: 远程通道发送心跳帧
 * 
 * @参数: 
 * @param: ch 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_SendTestData(uint8 ch)
{
    if(gs_FarpVar.login)
    {
        gs_FarpVar.hklen = VS_BuildLinkFrm(0x09, 0, gs_FarpVar.hkd);
    }
    else
    {
        gs_FarpVar.hklen = VS_BuildLinkFrm(0x05, 0xA619, gs_FarpVar.hkd);
    }
    if(gs_FarpVar.hklen > 0)
    {
        if(ch == 0)
        {
//	            Farp_SendIpData((uint8 *)cmdUpBuf, sizeof(cmdUpBuf));
            Farp_SendIpData((uint8 *)gs_FarpVar.hkd, gs_FarpVar.hklen);

        }
        else
        {
            //Farp_SendEthData(gs_FarpVar.hkd, gs_FarpVar.hklen);
        }
    }
}

/************************************************************************
 * @function: Farp_SendTestData
 * @描述: 远程通道发送心跳帧
 * 
 * @参数: 
 * @param: ch 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_SendReportData(uint8 ch, uint8_t cmd, uint16_t err, uint16_t id)
{
    gs_FarpVar.hklen = VS_BuildReportFrm(cmd, id, err, gs_FarpVar.hkd);

    if(gs_FarpVar.hklen > 0)
    {
        if(ch == 0)
        {
//	            Farp_SendIpData((uint8 *)cmdUpBuf, sizeof(cmdUpBuf));
            Farp_SendIpData((uint8 *)gs_FarpVar.hkd, gs_FarpVar.hklen);

        }
        else
        {
            //Farp_SendEthData(gs_FarpVar.hkd, gs_FarpVar.hklen);
        }
    }
}


/************************************************************************
 * @function: Farp_PreInit
 * @描述: 远程参数初始化
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_PreInit(void)
{                                           //参数载入
    ClearBuffer((uint8*)&gs_FarpVar, sizeof(FarpVar));
    memset((uint8_t *)&gs_FtpPara,0,sizeof(S_FTPPARA));
    
    LoadSystemParam(PARA_TYPE_FARP);

    gui_FarpWmRcSec = 10;                   //初始化完成10s后进行远程连接
    gui_FarpSerRcSec = 10;
    //gui_FarpEthRcSec = 10;
    
    guc_FarpWmHKUnRpyCnt = 0;               //心跳无回复次数计数器清零
    //guc_FarpEthHKUnRpyCnt = 0;
    
    gui_FarpWmSafeCnt = 0;
    gui_FarpSerSafeCnt = 0;
    //gui_FarpEthSafeCnt = 0;
    
    guc_FarpWmRcCnt = 0;                   //联网失败后重连次数清零
    guc_FarpSerRcCnt = 0;
    //guc_FarpEthRcCnt = 0;
    
    gui_FarpHdRstCnt = 0;                   //模块复位计数器清零
    guc_FarpNVHours = 0;                    //无IP数据时间清零
    guc_FarpPstTryCnt = 0;                   //被动激活模式重拨次数计数器清零
    
    gui_FarpBkTmCnt = 0;                    //被动激活无数据休眠计数器清零
    guc_FarpWmRcFailCnt = 0;
    guc_FarpWmRcFailNorFtpCnt = 0;
    //guc_FarpEthRcFailCnt = 0;
    
    gui_FarpServerNoDataCnt = 0;
    guc_FarpFromClnCloseCnt = 0;
    
    guc_FarpLoginConfirmErr = 0;
    gul_FarpLoginDeadTime = 0xFFFFFFFF;
    
    gs_FarpVar.wmsv = 0x88000000;
    //gs_SysVar.mLPstt |= HLV_LPTASK_MDCK;

}

void Load_Para(void)
{
//	    gs_FarpVar.dcip = 0xC0A80101;                   //主站首选ip地址
//	    gs_FarpVar.dcpt = 8080;                   //主站首选端口号
//	    gs_FarpVar.dcib = 0xC0A80101;                   //主站备用ip地址
//	    gs_FarpVar.dcpb = 8080;                   //主站备用端口号
//	    gs_FarpVar.dcip = 0xDA197792;                   //主站首选ip地址
//	    gs_FarpVar.dcpt = 5555;                   //主站首选端口号
//	    gs_FarpVar.dcib = 0xDA197792;                   //主站备用ip地址
//	    gs_FarpVar.dcpb = 5555;                   //主站备用端口号
//	    devPara.Ip[0]=115;
//	    devPara.Ip[1]=236;
//	    devPara.Ip[2]=33;
//	    devPara.Ip[3]=164;
//	    devPara.NetPort=8013;
//	    gs_FarpVar.dcip = 0x73EC21A4;                   //主站首选ip地址

//	    gs_FarpVar.dcpt = 8012;                   //主站首选端口号
//	    gs_FarpVar.dcib = 0x73EC21A4;                   //主站备用ip地址
//	    gs_FarpVar.dcpb = 8012;                   //主站备用端口号
//	
//	    MoveBuffer("cmnet", gs_FarpVar.gapn, 6);
    
    gs_FarpVar.hbcy = 120;              //默认10min
    gs_FarpVar.login = 0;
    gs_FarpVar.ntpc = 0;                 //连接方式(00:TCP_01:UDP)
    gs_PstPara.wkms = 1;       //模块工作模式
    gs_PstPara.fpms = 1;       //客户端 工作模式
    gs_PstPara.rdct = 3;              //(被动模式)重拨次数
    gs_FarpVar.rttm = 60;  //重连间隔(秒)
    
    //MoveBuffer(buff + 5, gs_PstPara.wksd, 3);//(时段模式)在线时段
    //gs_FarpVar.clnen = 1;
    //gs_FarpVar.seren = 0;

    Farp_SetWkmd();
//	    memset((uint8_t *)&gs_FtpPara, 0, sizeof(gs_FtpPara));
//	    strcpy(gs_FtpPara.ftp_domain,"118.31.50.214");
//	    gs_FtpPara.port = 21;
//	    strcpy(gs_FtpPara.ftp_fold,"rtu");
//	    strcpy(gs_FtpPara.ftp_file,"RTU01.bin");
//	    strcpy(gs_FtpPara.ftp_user,"sgp");
//	    strcpy(gs_FtpPara.ftp_pwd,"viewshine@123");
    gs_FtpPara.err_times = 0;
//	    memset(gs_FarpVar.user,0,32);
//	    memset(gs_FarpVar.word,0,32);
//	    memset(gs_PstPara.device,0,21);
//	
//	    MoveBuffer("user", gs_FarpVar.user, strlen("user"));       //专网用户名
//	    MoveBuffer("pwd", gs_FarpVar.word, strlen("pwd"));  //专网密码
    MoveBuffer("12345678901234567890", gs_PstPara.device, 20);
//	    gs_PstPara.Addr[0] = 0x01;
//	    gs_PstPara.Addr[1] = 0x01;
//	    gs_PstPara.Addr[2] = 0x15;
//	    gs_PstPara.Addr[3] = 0x05;
//	    gs_PstPara.Addr[4] = 0x18;
//	    gs_PstPara.Addr[5] = 0x20;


}

/************************************************************************
 * @function: Farp_InitProc
 * @描述: 远程进程初始化函数
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_InitProc(void)
{ 
//	    SYS_MODM_Reinit();                       //唤醒ATMODEM进程
    SYS_MODM_Init(&gs_TKFarpQueue);
    guc_FarpReActiveFlag = 1;
}



/************************************************************************
 * @function: Farp_WMSecondProc
 * @描述: 无线猫的秒处理
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_WMSecondProc(void)
{
    uint8 uc_closeclnflag = 0;
    uint8 uc_closeserflag = 0;
    /*
    if(gs_FarpVar.wmst & WMS_ETH0STT)
    {
        return;
    }
    */
                                            //yzy 2014-04-09,国网检测要求以太网优先,只要检测到网线即禁用GPRS.
//	    if(gs_FarpVar.wmst & WMS_EHSTT)
//	    {
//	        if((gs_FarpVar.wmst & WMS_STT_CLN) || (gs_FarpVar.wmst & WMS_STT_SER))
//	        {
//	//	            SYS_Message_Send(MSG_FARP_DISPPP, TASK_FARP_TKID);
//	            uint8 msg = MSG_FARP_DISPPP;
//	            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	            
//	        }
//	        gs_FarpVar.wmst &= ~WMS_CHALLSTT;
//	        return;
//	    }                                       //yzy 2014-04-09
                                            //获取模块状态
    TModemState modemstt = SYS_MODM_Status();

    if(gui_FarpWmSafeCnt > 0)               //数据通讯状态断线重连安全时间倒计时
    {
        gui_FarpWmSafeCnt--;
    }
    
    if(gui_FarpSerSafeCnt > 0)              //TCP Server 无数据通讯状态断线重连安全时间倒计时
    {
        gui_FarpSerSafeCnt--;
    }
    
    if(!(modemstt.bit.typeChecked))         //模块还未识别
    {
        return;
    }
    
    if(!(modemstt.bit.chnrdy))              //是否完成AT配置
    {
        if(++guc_FastWmSttReqCnt >= 5)      //每5秒钟
        {
            guc_FastWmSttReqCnt = 0;
            //SYS_MODM_Step(Modem_Init);
            SYS_MODM_Reinit();
            
            gs_GPIO.BlinkSet(GPIO_LED_GPRS, 0xFF, 0, 0);
        }
        return;
    }

    if(!(modemstt.bit.regtt))               //是否完成网络注册
    {
        gs_FarpVar.wmst &= ~WMS_CHALLSTT;
        gs_FarpVar.mcsq = 0;
                                            //及时更新模块状态
        if(++guc_FastWmSttReqCnt >= 15)     //每15秒钟
        {
            guc_FastWmSttReqCnt = 0;
            SYS_MODM_BeginCheck();
        }
        return;
    }

    gs_FarpVar.wmst |= WMS_STT_GSM;
    gs_FarpVar.mcsq = modemstt.bit.signl;
    SYS_Dev_OptBlinkSet(GPIO_LED_GPRS, 1, 10, 10, 0);  //注册GSM网络成功，1HZ闪烁
    
    if(!(modemstt.bit.pppoe))       //是否完成PPP链接
    {
        gs_FarpVar.wmst &= ~WMS_STT_PPP;    //数据通道不可用
        if(++guc_FastWmSttReqCnt >= 10)     //每10秒钟
        {
//	            SYS_Message_Send(MSG_FARP_CNTPPP, TASK_FARP_TKID);
            uint8 msg = MSG_FARP_CNTPPP;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            
            guc_FastWmSttReqCnt = 0;
        }
//	        
//	        if(guc_FastWmSttReqCnt % 3 == 0)     //每3秒钟
//	        {
//	            SYS_MODM_BeginCheck();
//	        }
        return;
    }
    gs_FarpVar.wmst |= WMS_STT_PPP;
    
    
    


//	    if(gs_FarpVar.clnen == 1)           //客户端模式
    {
        if(g_ulTcp_fd < 0)    //TCP 不在线
        {
            gs_FarpVar.wmst &= ~WMS_STT_CLN;    //数据通道不可用
            if(gs_FarpVar.wkmd == 0x00)         //实时在线模式
            {
                if(gui_FarpWmRcSec == 0)        //永久在线模式断线后即开始重试倒计时
                {
                    gui_FarpWmRcSec = gs_FarpVar.rttm;
                }
                                                //数据通讯状态下掉线马上重连
                if(gui_FarpWmSafeCnt > 0)
                {
                    gui_FarpWmSafeCnt = 0;
                    
                    gui_FarpWmRcSec = 1;
                    guc_FarpWmRcCnt = 3;       //通讯状态下断线重连3次
                }
                if(gui_FarpWmRcSec > 0)
                {
                    gui_FarpWmRcSec--;         //计数器递减
                    if(gui_FarpWmRcSec == 0)   //重试间隔已到,进行重连
                    {
//	                        SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
                        uint8 msg = MSG_FARP_CNTGPRS;
                        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
                        
                    }
                }
            }
            else if(gs_FarpVar.wkmd == 0x01)    //被动激活模式
            {
                //
            }
        }
        else
        {
            gs_FarpVar.wmst |= WMS_STT_CLN;     //数据通道可用
            
            if(guc_FarpLoginConfirmErr)           //登录后n秒内未收到确认帧,执行重连操作
            {
                if(gul_SysRunSecs > gul_FarpLoginDeadTime)
                {
//	                    SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                    uint8 msg = MSG_FARP_DISGPRS;
                    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
                    
                    uc_closeclnflag = 1;
                    gul_FarpLoginDeadTime = 0xFFFFFFFF;
                }
            }
            
            if(gui_FarpWmHBCYCnt > 0)
            {
                gui_FarpWmHBCYCnt--;
                if(gui_FarpWmHBCYCnt == 0)      //心跳周期到发心跳帧
                {
                    if(guc_FarpWmHKUnRpyCnt >= 3) //心跳无回复次数超过3次
                    {
//	                        SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                        uint8 msg = MSG_FARP_DISGPRS;
                        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
                        
                        uc_closeclnflag = 1;
                        guc_FarpWmHKUnRpyCnt = 0; //心跳无回复次数清零
                    }
                    else
                    {
                        //Farp_SendHKData(0);      //发送心跳数据
                        Farp_SendTestData(0);
                        guc_FarpWmHKUnRpyCnt++;
                        gui_FarpWmHBCYCnt = gs_FarpVar.hbcy;
                    }
                }
            }
            else
            {
                gui_FarpWmHBCYCnt = gs_FarpVar.hbcy;
            }
            gui_FarpWmRcSec = 0;                //重试次数清零
                                                // 
            if(gs_FarpVar.wkmd == 0)            //实时在线模式
            {
                //....
            }
            else if(gs_FarpVar.wkmd == 1)       //被动激活模式
            {
                gui_FarpBkTmCnt++;              //无数据休眠计数器累加
                                                //无数据通讯时间超过定值或需要断开连接
                if(gui_FarpBkTmCnt >= gs_FarpVar.slep)
                {                               //置断开连接消息
//	                    SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                    uint8 msg = MSG_FARP_DISGPRS;
                    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
                    
                    uc_closeclnflag = 1;
                }
            }
            else                                //短信模式直接断开连接
            {
//	                SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                
                uint8 msg = MSG_FARP_DISGPRS;
                krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
                uc_closeclnflag = 1;
            }
        }
    }
    
//	    if((modemstt.bit.gsstt == 0)  && (uc_closeclnflag == 1))     //重启客户端时，发现服务器侦听也失败的，  则先重启PPP
//	    {
//	//	        SYS_Message_Send(MSG_FARP_DISPPP, TASK_FARP_TKID);                 
//	//	        SYS_Message_Send(MSG_FARP_CNTPPP, TASK_FARP_TKID);    
//	        uint8 msg = MSG_FARP_DISPPP;
//	        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	        msg = MSG_FARP_CNTPPP;
//	        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	    }
//	    if(uc_closeserflag == 1)
//	    {
//	//	        SYS_Message_Send(MSG_FARP_CNTSERVER, TASK_FARP_TKID);
//	        uint8 msg = MSG_FARP_CNTSERVER;
//	        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	        
//	    }
    if((uc_closeclnflag == 1) && (gs_FarpVar.wkmd == 0))
    {
//	        SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
        uint8 msg = MSG_FARP_CNTGPRS;
        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
        
    }
}






/************************************************************************
 * @function: Farp_SecondProc
 * @描述: 远程通道秒事件处理函数
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_SecondProc(void)
{
    
    if(gs_SysVar.mDGstt & HLV_STT_NENG)
    {
        return ;
    }

    Farp_WMSecondProc();
    
  
                                            //点灯服务
    if(gs_FarpVar.wmst & (WMS_STT_CLN | WMS_ETH0STT))
    {
         SYS_LED_BlinkSet(GPIO_LED_GPRS, 1, 2, 0);//亮灯
    }
    else if(gs_FarpVar.wmst & WMS_STT_GSM)      //GSM 在线，闪烁
    {
         SYS_LED_BlinkSet(GPIO_LED_GPRS, 1, 10, 0);    //亮灯
    }  
    else
    {
         SYS_LED_BlinkSet(GPIO_LED_GPRS, 0xFF, 0, 0);
    }
                                            //初始化异常纠正机制
    if((!SYS_MODM_Status().bit.regtt) || (!SYS_MODM_Status().bit.chnrdy))
    {
        gui_FarpHdRstCnt++;                 //计数器累加
    
        if(gui_FarpHdRstCnt >= 120)         //超过两分钟还未完成初始化,复位设备
        {
            gui_FarpHdRstCnt = 0;           //复位计数器清零
            SYS_MODM_Reset(1);               //复位设备
        }
        return;
    }
    else
    {
        gui_FarpHdRstCnt = 0;               //初始化完成,清计数器
    }
    
    if(guc_FarpReActiveFlag && gs_FarpVar.clnen)    //被动激活模式，采集器重启后，需要先与主站建立链接
    {
        if((gs_FarpVar.wmst & WMS_STT_GSM) && (gs_FarpVar.wmst & WMS_STT_PPP)) 
        {
            guc_FarpReActiveFlag = 0;
//	            SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
            uint8 msg = MSG_FARP_CNTGPRS;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            
        }
        
    }

}



/************************************************************************
 * @function: Farp_MinProc
 * @描述: 分处理
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_MinProc(void)
{
    if(gs_SysVar.mDGstt & HLV_STT_NENG)
    {
        return ;
    }

    if((gs_FarpVar.wmst & WMS_STT_FTP) == 0)
        SYS_MODM_BeginCheck();                   //更新无线模块状态字
    
    //--------------------------------------------------------------------
    
    if(gs_PstPara.fpms == 3)                //时段在线模式
    {                                       //实时更新工作状态
        void Farp_SetWkmd(void);
        Farp_SetWkmd();
    }
    
}





/************************************************************************
 * @function: Farp_HourProc
 * @描述: 小时处理函数
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_HourProc(void)
{
    guc_FarpNVHours++;                      //无数据通讯小时数增加

    if(guc_FarpNVHours >= 16)               //连续三天无数据通讯复位ARM系统
    {
        aos_msleep(30000);                  //等待30s,以使系统能够将重要数据保存完毕
        guc_FarpNVHours = 0;
                                            //复位系统
        //gs_OS.Message_Send(MSG_MAIN_RESET, TASK_MAIN_TKID);
    }
}


/************************************************************************
 * @function: Farp_DayProc
 * @描述: 天处理函数
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_DayProc(void)
{
//	    SYS_Message_Send(MSG_FARP_VAR, TASK_FARP_TKID);
}

#if (Modem_M72X_SMS_EN > 0)
/************************************************************************
 * @function: Farp_SmgReadProc
 * @描述: 读取短消息
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_SmgReadProc(void)
{
    uint8 uc_data[256];                     //临时缓存
    uint8 format;
    uint8 uc_len = 0;

    uc_len = SYS_MODM_ReadSMS(uc_data, guc_RecvSmgNo, &format);

    _IF_TRUE_RETURN_VOID(uc_len == 0);

//	    SYS_LED_BlinkSet(LED_FAR_R, 0, 0, 0);
                                            //转移目标号码
    MoveBuffer(guc_RecvSmgNo, gs_FarpVar.smfc, 17);
                                            //短信数据帧被动处理
//	    PstFrmProc(PST_CHN_SMS, uc_data, uc_len, uc_data, uc_len);

}
#endif



/************************************************************************
 * @function: Farp_IpDataReadProc
 * @描述: 读取IP数据并处理
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_IpDataReadProc(void)
{
    uint8 uc_data[LEN_OF_IPRECV];                    //临时缓存
    int32_t length;
    uint16 ui_len;
                                            //循环读取IP数据(超长帧需要读取多次)
    length = 0;
//	    for(uint8 uc_i = 0; uc_i < 10; uc_i++)
//	    {                                       //单次读取数据
//	        ui_len = SYS_MODM_ReadIP(uc_data + length, sizeof(uc_data) - length);
//	        length += ui_len;
//	                                            //读到数据,当数据长度过小,可以认为是超长帧接收结束.
//	        if((ui_len > 0) && (ui_len < 900))
//	        {
//	            break;
//	        }
//	                                            //超出缓存,退出.
//	        if(length >= sizeof(uc_data))
//	        {
//	            break;
//	        }
//	        aos_msleep(200); 
//	    }
    length = HAL_TCP_Read(g_ulTcp_fd, uc_data, LEN_OF_IPRECV, 0);
    

    _IF_TRUE_RETURN_VOID(length <= 0);      //读出的数据长度为0则返回不处理

//	    SYS_LED_BlinkSet(LED_FAR_R, 0, 0, 0);
    
    LOG_DUMPHEX(LOG_LEVEL_DEBUG, "tcp recv:\n", uc_data, length);
    PstFrmProc(PST_CHN_TCP, uc_data, length, uc_data, length);  //数据帧的被动处理

    guc_FarpWmHKUnRpyCnt = 0;               //收到IP数据,清心跳无回复次数计数器
    guc_FarpNVHours = 0;                    //无IP数据时间清零
}




/************************************************************************
 * @function: Farp_UpDataActiveIP
 * @描述: 被动激活模式下，主动上送数据，之前先建立客户端
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2015/03/27)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_UpDataActiveIP(void)
{
    if(gs_FarpVar.clnen == 1)           //客户端模式
    {
        if(g_ulTcp_fd < 0) 
        {             
//	            SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);   //置连接GPRS消息
            uint8 msg = MSG_FARP_CNTGPRS;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
        }
    }
    
    guc_FarpPstTryCnt = 0;                   //被动模式重试次数清零.
}


/************************************************************************
 * @function: Farp_RingProc
 * @描述: 振铃信号处理
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_RingProc(void)
{
                                            //置连接GPRS消息
    uint8 msg = MSG_FARP_CNTGPRS;
    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
                                            
//	    SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
    guc_FarpPstTryCnt = 0;                   //被动模式重试次数清零.
}


/************************************************************************
 * @function: Farp_Close_PPP
 * @描述: 注销PPP处理
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2015/03/23)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_Close_PPP(void)
{
    SYS_MODM_ClosePPP();                     //断开PPP连接
    
    gs_FarpVar.wmst &= ~WMS_STT_PPP;        //清零状态字
    gs_FarpVar.wmst &= ~WMS_STT_CLN;        //清零状态字
    gs_FarpVar.wmst &= ~WMS_STT_SER;        //清零状态字
    gs_FarpVar.wmst &= ~WMS_STT_SER_CNT1;
    gs_FarpVar.wmst &= ~WMS_STT_SER_CNT2;
}


/************************************************************************
 * @function: Farp_Creat_PPP
 * @描述: 连接PPP处理
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2015/03/23)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_Creat_PPP(void)
{
    if(!(gs_FarpVar.wmst & WMS_STT_GSM))      //
    {
        return;
    }
    if((gs_FarpVar.wmst & WMS_STT_PPP))
    {
        SYS_MODM_ClosePPP();                 //先关闭PPP连接
    }
    
    for(uint8 uc_i = 0; uc_i < 3; uc_i++)
    {
        aos_msleep(1000);
        if(!SYS_MODM_CreatePPP(
                (string)gs_FarpVar.gapn, 
                (string)gs_FarpVar.user, 
                (string)gs_FarpVar.word
                ))      
        {                                   
            gs_FarpVar.wmst |= WMS_STT_PPP;
            return;                     //建立成功直接退出
        }
    }
    SYS_MODM_ClosePPP();                 //TCP连接不成功,关闭PPP连接
return ;
}



/************************************************************************
 * @function: Farp_WM_Connect
 * @描述: 连接GPRS处理
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_WM_Connect(void)
{
    uint8 uc_i;                             //循环变量
    uint8_t ip[32];
    memset(ip,0,32);
    if(!(gs_FarpVar.wmst & WMS_STT_GSM) || !(gs_FarpVar.wmst & WMS_STT_PPP))      //
    {
        return;
    }
    
    if(!gs_FarpVar.clnen)   //客户端模式开启
    {
        return;
    }
    
    if((gs_FarpVar.wmst & WMS_STT_CLN))       //当前数据通道是否已经可用
    {                                       //置断开连接消息
//	        SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
        uint8 msg = MSG_FARP_DISGPRS;
        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
        
        gui_FarpWmRcSec = 1;                //断开连接后马上重连        
        guc_FarpPstTryCnt = 0;               //被动模式重连次数清零
        return;
    }

    if(gs_FarpVar.wkmd == 1)                //被动激活模式
    {                                       //重连次数超过最大次数则不进行重连
        if(guc_FarpPstTryCnt > gs_FarpVar.rtct)
        {
            return;
        }
        guc_FarpPstTryCnt++;                 //否则重连次数累加
    }

    guc_FarpWmRcFailCnt++;                  //重连失败次数累加
    if(guc_FarpWmRcFailCnt > 2)             //保护机制:连续3次重连失败,复位模块
    {
        if(gs_FarpVar.wmst & WMS_STT_FTP)
        {
            guc_FarpWmRcFailNorFtpCnt ++;
            if(guc_FarpWmRcFailNorFtpCnt > 1000)
            {
                SYS_MODM_Reset(1);                   //复位设备
                gs_FarpVar.wmst = 0;
                guc_FarpWmRcFailNorFtpCnt = 0;
            }
        }
        else
        {
            SYS_MODM_Reset(1);                   //复位设备
            gs_FarpVar.wmst = 0;
        }
        guc_FarpWmRcFailCnt = 0;

        return ;
    }
    
    gui_FarpWmRcSec = 0;                    //重试间隔计数器清零
    sprintf(ip,"%03d.%03d.%03d.%03d", (uint8_t)(gs_FarpVar.dcip >> 24), (uint8_t)(gs_FarpVar.dcip >> 16), (uint8_t)(gs_FarpVar.dcip >> 8), (uint8_t)(gs_FarpVar.dcip));
    for(uc_i = 0; uc_i < gs_FarpVar.rdct; uc_i++)
    {                                       //两次重试之间短暂延时
        aos_msleep(1000);
        
        if(!(gs_FarpVar.wmst & WMS_STT_CLN))
        {
                                                //连接首选主站
            if(!(guc_FarpLoginConfirmErr & 0x01))   //上次主用登陆确认无异常
            {
                
//	                if(!SYS_MODM_CreateIP(gs_FarpVar.ntpc, gs_FarpVar.dcip, gs_FarpVar.dcpt))
                g_ulTcp_fd = HAL_TCP_Establish(ip, gs_FarpVar.dcpt);
                if(g_ulTcp_fd >= 0)
                {
                    guc_FarpLoginConfirmErr = 0;
                    gul_FarpLoginDeadTime = gul_SysRunSecs + 10;
                                                    //
                    gs_FarpVar.wmst |= WMS_STT_CLN;
                    guc_FarpPstTryCnt = 0;          //被动模式重连次数清零
                    Farp_SendLoginData(0);          //发送登录帧
                    //Farp_SendHKData(0);             //发送心跳帧
                                                    //初始化心跳周期
                    gui_FarpWmHBCYCnt = 5;//gs_FarpVar.hbcy;
                                                    //实时在线模式无有效数据计数器初始化
                    gui_FarpWmSafeCnt = gs_FarpVar.hbcy;
                    guc_FarpWmRcFailCnt = 0;        //重连失败次数清零
                    
                    //SYS_MODM_Step(Modem_IPState);
                    return;
                }
                else
                {
                    //建立失败
                    //SYS_MODM_Reset(0);
                }                
            }
                                                    //连接备用主站
//	            if(!(guc_FarpLoginConfirmErr & 0x02))   //上次备用登陆确认无异常
//	            {
//	//	                if(!SYS_MODM_CreateIP(gs_FarpVar.ntpc, gs_FarpVar.dcib, gs_FarpVar.dcpb))
//	                g_ulTcp_fd = HAL_TCP_Establish(ip, gs_FarpVar.dcpt);
//	                if(g_ulTcp_fd >= 0)
//	                {
//	                    guc_FarpLoginConfirmErr = 0;
//	                    gul_FarpLoginDeadTime = gul_SysRunSecs + 10;
//	                                                    //
//	                    gs_FarpVar.wmst |= WMS_STT_CLN;
//	                    guc_FarpPstTryCnt = 0;          //被动模式重连次数清零
//	                    Farp_SendLoginData(0);          //发送登录帧
//	                    //Farp_SendHKData(0);             //发送心跳帧
//	                                                    //初始化心跳周期
//	                    gui_FarpWmHBCYCnt = 5;//gs_FarpVar.hbcy;
//	                    gui_FarpWmSafeCnt = gs_FarpVar.hbcy;
//	                    guc_FarpWmRcFailCnt = 0;        //重连失败次数清零
//	                    
//	                    //SYS_MODM_Step(Modem_IPState);
//	                    return;
//	                }
//	                else
//	                {
//	                    //建立失败
//	                    //SYS_MODM_Reset(0);
//	                }
//            }
            guc_FarpLoginConfirmErr = 0;
        }
        if((!(gs_FarpVar.wmst & WMS_STT_CLN)) && (!(gs_FarpVar.wmst & WMS_STT_SER)))
        {
//	            SYS_Message_Send(MSG_FARP_DISPPP, TASK_FARP_TKID);                 //TCP连接不成功,关闭PPP连接
            uint8 msg = MSG_FARP_DISPPP;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            
        }
    }
                                            //理论上此处不需要discnt
                                            //但是使用过程中发现某些模块CreateIP不成功后也需要DiscntIP
//	    SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
                                            
//	    SYS_Message_Send(MSG_FARPE_FCNT, TASK_FARP_TKID);
    uint8 msg = MSG_FARP_DISGPRS;
    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
    msg = MSG_FARPE_FCNT;//连接失败处理
    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);                                            
    if(guc_FarpWmRcCnt > 0)                //通讯状态下断线重连失败后5秒后重连
    {
        guc_FarpWmRcCnt--;
        gui_FarpWmRcSec = 10;
    }
}


/************************************************************************
 * @function: Farp_Creat_Server
 * @描述: 建立TCP服务端
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Farp_Creat_Server(void)
//	{
//	    uint8 uc_i;                             //循环变量
//	    
//	    if(!(gs_FarpVar.wmst & WMS_STT_GSM) || !(gs_FarpVar.wmst & WMS_STT_PPP))      //
//	    {
//	        return;
//	    }
//	    
//	    if(!gs_FarpVar.seren)   //客户端模式开启
//	    {
//	        return;
//	    }
//	    
//	    if((gs_FarpVar.wmst & WMS_STT_SER))       //当前数据通道是否已经可用
//	    {                                       //置断开连接消息
//	//	        SYS_Message_Send(MSG_FARP_DISSERVER, TASK_FARP_TKID);
//	        uint8 msg = MSG_FARP_DISSERVER;
//	        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	        
//	        gui_FarpSerRcSec = 1;                //断开连接后马上重连        
//	        return;
//	    }
//	
//	    guc_FarpSerRcFailCnt++;                  //重连失败次数累加
//	    if(guc_FarpSerRcFailCnt > 5)             //保护机制:连续5次重连失败,复位模块
//	    {
//	        SYS_MODM_Reset(1);                   //复位设备
//	        guc_FarpSerRcFailCnt = 0;
//	    }
//	    
//	    gui_FarpSerRcSec = 0;                    //重试间隔计数器清零
//	
//	    for(uc_i = 0; uc_i < gs_FarpVar.rdct; uc_i++)
//	    {                                       //两次重试之间短暂延时
//	        sleep_ms(1000);
//	        
//	        if(!(gs_FarpVar.wmst & WMS_STT_SER))
//	        {
//	            if(!SYS_MODM_CreatServer(0, gs_FarpVar.lispt))  //
//	            {
//	                gs_FarpVar.wmst |= WMS_STT_SER;
//	                gs_FarpVar.wmst &= ~WMS_STT_SER_CNT1;
//	                gs_FarpVar.wmst &= ~WMS_STT_SER_CNT2;
//	                
//	                gui_FarpFromClnNoDataCnt = gs_FarpVar.hbcy * 10;    //江苏规定为，10倍的心跳周期
//	                gui_FarpSerSafeCnt = gs_FarpVar.hbcy;
//	                guc_FarpSerRcFailCnt = 0;        //重连失败次数清零
//	                guc_FarpFromClnCloseCnt = 0;
//	                
//	                SYS_MODM_Step(Modem_IPState);
//	                return;
//	            }
//	        }
//	    }
//	    
//	    if((!(gs_FarpVar.wmst & WMS_STT_CLN)) && (!(gs_FarpVar.wmst & WMS_STT_SER)))
//	    {
//	//	        SYS_Message_Send(MSG_FARP_DISPPP, TASK_FARP_TKID);                 //TCP连接不成功,关闭PPP连接
//	        uint8 msg = MSG_FARP_DISPPP;
//	        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	        
//	    }
//	                                            //理论上此处不需要discnt
//	                                            //但是使用过程中发现某些模块CreateIP不成功后也需要DiscntIP
//	//	    SYS_Message_Send(MSG_FARP_DISSERVER, TASK_FARP_TKID);
//	//	                                            //连接失败处理
//	//	    SYS_Message_Send(MSG_FARPE_FCNT, TASK_FARP_TKID);
//	    uint8 msg = MSG_FARP_DISSERVER;
//	    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	    msg = MSG_FARPE_FCNT;
//	    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	    if(guc_FarpSerRcCnt > 0)                //通讯状态下断线重连失败后5秒后重连
//	    {
//	        guc_FarpSerRcCnt--;
//	        gui_FarpSerRcSec = 10;
//	    }
//	}

/************************************************************************
 * @function: Farp_PrioConnect
 * @描述: 按照优先级选择通道进行连接
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_PrioConnect(void)
{
    if(!(gs_FarpVar.wmst & WMS_STT_CLN))
//	    {
//	        SYS_Message_Send(MSG_FARP_CNTETH, TASK_FARP_TKID);
//	        uint8 msg = MSG_FARP_DISFROMCLN1;
//	        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	        
//	    }
//	    else
    {
//	        SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
        uint8 msg = MSG_FARP_CNTGPRS;
        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
        
    }
}

intptr_t Farp_WM_Connect_Once(void)
{
    uint8 uc_i;                             //循环变量
    uint8_t ip[32];
    memset(ip,0,32);
    intptr_t socket_id = -1;

    if(g_ulTcp_fd >= 0)
        return g_ulTcp_fd;
//	    gs_FarpVar.wmst &= ~WMS_STT_PPP;        //清零状态字
    if((!SYS_MODM_Status().bit.pppoe))
    {
        if(SYS_ERR_OK != SYS_MODM_Reset(0))
        {
            return socket_id;
        }
    }
    LOG_INFO( "%s %d WM Connect \r\n", __func__, __LINE__);
    sprintf(ip,"%03d.%03d.%03d.%03d", (uint8_t)(gs_FarpVar.dcip >> 24), (uint8_t)(gs_FarpVar.dcip >> 16), (uint8_t)(gs_FarpVar.dcip >> 8), (uint8_t)(gs_FarpVar.dcip));
    for(uc_i = 0; uc_i < 2; uc_i++)
    {                                       
       
        socket_id = HAL_TCP_Establish(ip, gs_FarpVar.dcpt);
        if(socket_id >= 0)
        {
            Farp_SendLoginData(0);          //发送登录帧
            return socket_id;
        }

    }
    return socket_id;
}
const uint32_t gl_constTestIp   = 0x73EC21A4;
const uint32_t gl_constTestPort = 8012;

int IP_Check(void)
{
    uint8_t ip[32];
    memset(ip,0,32);
    intptr_t g_ulTstTcp_fd = -1;

    sprintf(ip,"%03d.%03d.%03d.%03d", (uint8_t)(gl_constTestIp >> 24), (uint8_t)(gl_constTestIp >> 16), (uint8_t)(gl_constTestIp >> 8), (uint8_t)(gl_constTestIp));
    if((gs_FarpVar.wmst & WMS_STT_GSM))
    {
        for(int uc_i = 0; uc_i < 2; uc_i++)
        {   
            g_ulTstTcp_fd = HAL_TCP_Establish(ip, gl_constTestPort);
            if(g_ulTstTcp_fd >= 0)
            {
                break;
            }
        }
        if(g_ulTstTcp_fd < 0)
        {
            gs_SysVar.terstt.bit.ipcheck = 1;
            
            LOG_INFO(DBGFMT_A"IP check failed fd = %d\n",DBGARG_A, g_ulTstTcp_fd);
        }
        else
        {
            gs_SysVar.terstt.bit.ipcheck = 0;
            
            HAL_TCP_Destroy(g_ulTstTcp_fd);
            
            LOG_INFO(DBGFMT_A"IP check succ fd = %d\n",DBGARG_A, g_ulTstTcp_fd);
        }
    }
    else
    {
        aos_msleep(5000);
        gs_OS.Message_Send(&gs_TKSlvQueue, &msgidA[MSG_FARP_CHECK], 1);
        gs_SysVar.terstt.bit.ipcheck = 1;
        
        LOG_INFO(DBGFMT_A"IP check wait 5s\n",DBGARG_A);
    }
    return 0;
}



/************************************************************************
 * @function: Farp_Close_Server
 * @描述: 断开TCP Server 链接
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Farp_Close_Server(void)
//	{
//	    SYS_MODM_CloseTcpServer();                     //断开TCP连接
//	                                            //断开连接成功
//	    gs_FarpVar.wmst &= ~WMS_STT_SER;        //清零状态字
//	    gs_FarpVar.wmst &= ~WMS_STT_SER_CNT1;
//	    gs_FarpVar.wmst &= ~WMS_STT_SER_CNT2;
//	}



/************************************************************************
 * @function: Farp_WM_DisCnt
 * @描述: 断开GPRS连接
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_WM_DisCnt(void)
{
//	    Farp_SendLoginOutData(0);                //发送退出登陆帧
    aos_msleep(200);
//	    SYS_MODM_CloseIP();                     //断开TCP连接
    if(g_ulTcp_fd >= 0)
    {
        HAL_TCP_Destroy(g_ulTcp_fd);
        g_ulTcp_fd = -1;
    }
    gs_FarpVar.wmst &= ~WMS_STT_CLN;        //清零状态字
    gui_FarpBkTmCnt = 0;                    //被动激活模式无数据通讯休眠时间计数器清零
}




/************************************************************************
 * @function: Farp_Close_From_Client1
 * @描述: 断开收到的客户端链接1
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Farp_Close_From_Client1(void)
//	{
//	    SYS_MODM_CloseFromClient(0);                     //断开收到的客户端链接1
//	
//	    gs_FarpVar.wmst &= ~WMS_STT_SER_CNT1;
//	}


/************************************************************************
 * @function: Farp_Close_From_Client2
 * @描述: 断开收到的客户端链接2
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Farp_Close_From_Client2(void)
//	{
//	    SYS_MODM_CloseFromClient(1);                     //断开收到的客户端链接1
//	
//	    gs_FarpVar.wmst &= ~WMS_STT_SER_CNT2;
//	}



/************************************************************************
 * @function: Farp_RecvValidIpProc
 * @描述: 远程通道收到有效Ip数据后的处理
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_RecvValidIpProc(void)
{
    gui_FarpBkTmCnt = 0;                    //无数据休眠计数器清零
    
    gui_FarpWmSafeCnt = gs_FarpVar.hbcy;    //将无有效数据计数器复位
    //gui_FarpEthSafeCnt = gs_FarpVar.hbcy;
    
    gui_FarpWmHBCYCnt = gs_FarpVar.hbcy;
//#if SG_INSPECT
//    gui_FarpEthHBCYCnt = 60;//默认1分钟
//#else
//    gui_FarpEthHBCYCnt = gs_FarpVar.hbcy;
//#endif    
}

       
       
       
/************************************************************************
 * @function: Farp_RecvValidTcpSerProc
 * @描述: 远程通道收到有效TcpServer数据后的处理
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_RecvValidTcpSerProc(void)
{
    gui_FarpSerSafeCnt = gs_FarpVar.hbcy;
    gui_FarpFromClnNoDataCnt = gs_FarpVar.hbcy * 10;    //江苏规定为，10倍的心跳周期
    guc_FarpFromClnCloseCnt = 0;
}
       
/************************************************************************
**函数名称: void Farp_SetWkmd(void)
**描 述: 无线模块工作模式的设定
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-09-15
*************************************************************************/
void Farp_SetWkmd(void)
{
  
    uint8 ucClnen = gs_FarpVar.clnen;
    uint8 ucSeren = gs_FarpVar.seren;
                                        //设定终端工作模式
    if(gs_PstPara.wkms == 1)            //客户端模式
    {
        gs_FarpVar.clnen = 1;
        gs_FarpVar.seren = 0;
    }
    else if(gs_PstPara.wkms == 2)       //服务器模式
    {
        gs_FarpVar.clnen = 0;
        gs_FarpVar.seren = 1;
    }
    else                        // 混合模式
    {
        gs_FarpVar.clnen = 1;
        gs_FarpVar.seren = 1;
        gs_PstPara.fpms = 2;    //混合模式下，客户端为被动激活
    }
    
    if(gs_FarpVar.seren)
    {
        if(ucSeren == 0)    //从 非TCP server 状态  到 激活TCP server 状态
        {
//	            SYS_Message_Send(MSG_FARP_CNTSERVER, TASK_FARP_TKID);
            uint8 msg = MSG_FARP_CNTSERVER;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            
        }
    }
    
    if(gs_FarpVar.clnen)
    {
                                    //设定客户端工作模式
        uint8 mode = gs_FarpVar.wkmd;
        
                                            
        if(gs_PstPara.fpms == 2)                //被动激活模式
        {
            gs_FarpVar.wkmd = 1;
        }
        else if(gs_PstPara.fpms == 3)           //时段在线模式
        {
            if(STR_GetBits(gs_PstPara.wksd, 1, GetTime()->hour))
            {
                gs_FarpVar.wkmd = 0;
            }
            else
            {
                gs_FarpVar.wkmd = 1;
            }
        }
        else                                    //永久在线模式
        {
            gs_FarpVar.wkmd = 0;
        }
                                                //设定重拨重试次数
        if(gs_FarpVar.wkmd == 0)                //永久在线模式
        {
            gs_FarpVar.rdct = 2;
        }
        else if(gs_FarpVar.wkmd == 1)           //被动模式
        {
            gs_FarpVar.rdct = gs_PstPara.rdct;  //重拨次数
            if(gs_FarpVar.rdct == 0)            //被动激活模式至少重拨1次
            {
                gs_FarpVar.rdct = 1;
            }
            gs_FarpVar.rtct = 1;                //重试一次(每次按重拨次数连接)
        }
                                                //从被动切换到永久,需要重连
        if(((mode > gs_FarpVar.wkmd) && ((gs_FarpVar.wmst & WMS_STT_CLN) == 0)) || (ucClnen == 0))
        {
//	            SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
            uint8 msg = MSG_FARP_CNTGPRS;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            
        }
    }
}











/************************************************************************
**函数名称: void Farp_UpDateVar(void)
**描 述: 初始化远程通讯的参数
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: chenyj 
**日 期: 
**-----------------------------------------------------------------------
**修 改: yzy 
**日 期: 
*************************************************************************/
void Farp_UpdateVar(void)
{
    LoadSystemParam(PARA_TYPE_FARP);
}



/************************************************************************
 * @function: Farp_ParaChg
 * @描述: 重要连接参数更改,需要更新联网状态
 * 
 * @参数: 
 * @param: pdata 
 * @返回: 
 * @说明:
 * @作者: yzy (2011-2-24)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Farp_ParaChg(void)
{
    if(gs_FarpVar.wkmd == 0)            //实时在线模式
    {                                   //连接
//	        SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
        uint8 msg = MSG_FARP_CNTGPRS;
        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
        
    }
    else if(gs_FarpVar.wkmd == 1)       //被动激活模式
    {                                   //断开连接
//	        SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
        uint8 msg = MSG_FARP_DISGPRS;
        krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
        
    }
}


/*******************************************************************************
 * @function_name:  SYS_Farp_Task
 * @function_file:  farp.c
 * @描述: 远程通讯进程处理函数
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
void SYS_Farp_Task(void * arg)
{
                                            //数字消息
    krhino_buf_queue_create(&gs_TKFarpQueue, "farp_queue",
                         gc_TKFarpQbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);
//	    krhino_sem_create(&gs_FarpProc, "farp_proc_sem", 1);
    
    Farp_InitProc();
        
    Load_Para();

#ifdef WITH_SAL
    while(1)
    {
        gs_SysVar.mLPstt |= HLV_LPTASK_MDCK;
    
        if(0 == sal_init())
            break;
        gs_SysVar.mLPstt &= ~HLV_LPTASK_MDCK;
        aos_msleep(10000);
    }
    gs_SysVar.mLPstt &= ~HLV_LPTASK_MDCK;
    if((gs_SysVar.mLPstt & HLV_LPTASK_CLSCK) == HLV_LPTASK_CLSCK)
    {
        Farp_WM_DisCnt();
        Farp_Close_PPP();
        SYS_MODM_Shutdown();
        gs_SysVar.mLPstt &= ~HLV_LPTASK_CLSCK;
        
        LOG_INFO("modem check ok! but ext pwr losed,so modem should shut down\n");    
    }
#endif    
    
    SYS_MSG_Apply(TASK_FARP_TKID, MSG_CLS_TM);               //获取时间消息
    //SYS_MSG_Apply(TASK_FARP_TKID, MSG_CLS_MD);               //获取时间消息
    memcpy(gs_FarpVar.modemip, SYS_MODM_GetIP(), 4);   //获取本地IP
//	    krhino_buf_queue_flush(&gs_TKFarpQueue);
    for(;;)
    {
//获取进程消息
        krhino_buf_queue_recv(&gs_TKFarpQueue, RHINO_WAIT_FOREVER, g_TKFarpQ_buf_recv,
                                  &g_TKFarpQ_recv_size);

        switch(g_TKFarpQ_buf_recv[0])                        //根据消息分别处理
        {
            case MSG_SEC:
                gs_SysVar.mLPstt |= HLV_LPTASK_SMSG_FARP;
                
                Farp_SecondProc();
           
                gs_SysVar.mLPstt &= ~HLV_LPTASK_SMSG_FARP;
            
                break;

            case MSG_MIN:
                Farp_MinProc();
                break;

            case MSG_HOUR:
                Farp_HourProc();
                break;

            case MSG_DAY:
                Farp_DayProc();
                break;
#if (Modem_M72X_SMS_EN > 0)
            case MSG_SMRECV:                //短信数据
                Farp_SmgReadProc();
                break;
#endif                
            case MSG_RING:                  //振铃
                Farp_RingProc();
                break;
                
            case MSG_IP1DATA:               //IP数据
                Farp_IpDataReadProc();
                break;
                
            case MSG_FARP_DISPPP:           //关闭PPP链接
                Farp_Close_PPP();
                break;
                
            case MSG_FARP_CNTPPP:           //建立PPP链接
                Farp_Creat_PPP();
                break;
                
            case MSG_FARP_CNTGPRS:          //连接GPRS
                Farp_WM_Connect();
                break;

            case MSG_FARP_DISGPRS:          //断开GPRS
                Farp_WM_DisCnt();
                break;
                

//	            case MSG_FARP_CNTSERVER:           //建立一个TCP Server
//	                Farp_Creat_Server();
//	                break;
//	                
//	            case MSG_FARP_DISFROMCLN1:           //关闭 收到的 一个客户端链接1
//	                Farp_Close_From_Client1();
//	                break;
//	           
//	            case MSG_FARP_DISFROMCLN2:           //关闭 收到的 一个客户端链接2
//	                Farp_Close_From_Client2();
//	                break;

            case MSG_FARP_RECVVALIDIP:      //IP收到有效数据
                Farp_RecvValidIpProc();
                break;
                
            case MSG_FARP_RECVVALIDSER:     //TCP Server 收到有效数据
                Farp_RecvValidTcpSerProc(); 
                break;
                
            case MSG_FARP_PRIOCNT:          //按优先级连接
                Farp_PrioConnect();
                break;
            case MSG_FARP_VAR:              //参数更新
                Farp_UpdateVar();
                LOG_DEBUG("******MSG_FARP_VAR******** !!!!\n");
                
                break;

            case MSG_FARP_PARACHG:          //联网参数变更
            
                Farp_ParaChg();
                break;
            case MSG_ENTER_LP:
                Farp_WM_DisCnt();
                Farp_Close_PPP();
                SYS_MODM_Shutdown();
                
                LOG_INFO("HLV_LPTASK_CLSCK modem shut down\n");    
                gs_SysVar.mLPstt &= ~HLV_LPTASK_CLSCK;
                break;
            case MSG_LIVE:
				HB_RetLive(TASK_FARP_TKID);
                break;

            default:
                break;
        }
        memset(g_TKFarpQ_buf_recv,0,2);
    }
}

/******************************************************************************
**远程通信进程块常量部分定义
//	******************************************************************************/
//	const TESTcbConst gs_TkFarp = 
//	{
//	    SYS_Farp_Task,
//	    TASK_FARP_TKID >> 3,                    //应用管理进程
//	    TASK_FARP_TKID,                         //进程编号
//	    TASK_FARP_TKID & 7,                     //优先级内进程编号位
//	    true,                                      //消息循环机制进程
//	};



const KTaskConst gs_TkFarp = 
{
    &gs_TKFarpHandle,
    SYS_Farp_Task,                           //无线猫进程
    gs_TKFarpStack,
    TASK_FARP_TKID,                           //进程编号
    true,                                    //消息循环机制进程
    TASK_FARP_STKL,
    "modem",
    &gs_TKFarpQueue,
};


