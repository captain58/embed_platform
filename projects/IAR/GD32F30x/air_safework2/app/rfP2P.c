#define EXT_FARP
#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"
#include "app.h"
#include "pst.h"
//#include "hlv.h"
//#include "farp.h"
//#include "netp.h"
#include "task.h"
#include "farp.h"

#include "A7139reg.h"
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
static char         g_RFMng_buf_recv[BUFQUEUE_MSG_MAX+4];

size_t       g_RFMng_recv_size;
ktimer_t     g_RFMngTimer;

cpu_stack_t  gs_RFMngStack[TASK_RFMNG_STKL];
ktask_t      gs_RFMngHandle;
kbuf_queue_t gs_RFMngQueue;
char         gc_RFMngbuf[MSG_BUFF_LEN];

void Farp_SendTestData(uint8 ch);
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
    memset((uint8*)&gs_FarpVar, 0, sizeof(FarpVar));
    memset((uint8_t *)&gs_FtpPara,0,sizeof(S_FTPPARA));
    
//    LoadSystemParam(PARA_TYPE_FARP);

//    gui_FarpWmRcSec = 10;                   //初始化完成10s后进行远程连接
//    gui_FarpSerRcSec = 10;
//    //gui_FarpEthRcSec = 10;
//    
//    guc_FarpWmHKUnRpyCnt = 0;               //心跳无回复次数计数器清零
//    //guc_FarpEthHKUnRpyCnt = 0;
//    
//    gui_FarpWmSafeCnt = 0;
//    gui_FarpSerSafeCnt = 0;
//    //gui_FarpEthSafeCnt = 0;
//    
//    guc_FarpWmRcCnt = 0;                   //联网失败后重连次数清零
//    guc_FarpSerRcCnt = 0;
//    //guc_FarpEthRcCnt = 0;
//    
//    gui_FarpHdRstCnt = 0;                   //模块复位计数器清零
//    guc_FarpNVHours = 0;                    //无IP数据时间清零
//    guc_FarpPstTryCnt = 0;                   //被动激活模式重拨次数计数器清零
//    
//    gui_FarpBkTmCnt = 0;                    //被动激活无数据休眠计数器清零
//    guc_FarpWmRcFailCnt = 0;
//    guc_FarpWmRcFailNorFtpCnt = 0;
//    //guc_FarpEthRcFailCnt = 0;
//    
//    gui_FarpServerNoDataCnt = 0;
//    guc_FarpFromClnCloseCnt = 0;
//    
//    guc_FarpLoginConfirmErr = 0;
//    gul_FarpLoginDeadTime = 0xFFFFFFFF;
//    
//    gs_FarpVar.wmsv = 0x88000000;
    //gs_SysVar.mLPstt |= HLV_LPTASK_MDCK;

}
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
//    if(g_ulTcp_fd >=0 )
//    {                                       //发送IP数据
////	        if(!SYS_MODM_SendIP(buffer, len))
//        LOG_DUMPHEX(LOG_LEVEL_DEBUG, "send:\n", buffer, len);
//
//        if(HAL_TCP_Write(g_ulTcp_fd, buffer, len,3000) >= 0)
//        {
////	            gs_GPIO.LED_BlinkSet(LED_FAR_T, 0, 0, 0);
//            SYS_OK();                        //发送成功
//        }
//        else
//        {
//            uint8 msg = MSG_FARP_DISGPRS;
//            krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//            
//            gui_FarpWmRcSec = 10;
//        }
//    }
    return SYS_ERR_FT;                      //IP数据发生失败
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
void Farp_SendTestData(uint8 ch)
{
    if(gs_FarpVar.login)
    {
        gs_FarpVar.hklen = RF_BuildLinkFrm(0x09, 0, gs_FarpVar.hkd);
    }
    else
    {
        gs_FarpVar.hklen = RF_BuildLinkFrm(0x05, 0xA619, gs_FarpVar.hkd);
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

///************************************************************************
// * @function: Farp_WMSecondProc
// * @描述: 无线猫的秒处理
// * @参数: 
// * @返回: 
// * @说明: 
// * @作者: yzy (2014/1/14)
// *-----------------------------------------------------------------------
// * @修改人: 
// ************************************************************************/
void Farp_WMSecondProc(void)
{
    uint8 uc_closeclnflag = 0;
//    uint8 uc_closeserflag = 0;
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
    TRFModemState modemstt = SYS_RF_Status();//SYS_MODM_Status();
	
    if(gui_FarpWmSafeCnt > 0)               //数据通讯状态断线重连安全时间倒计时
    {
        gui_FarpWmSafeCnt--;
    }
    
    if(gui_FarpSerSafeCnt > 0)              //TCP Server 无数据通讯状态断线重连安全时间倒计时
    {
        gui_FarpSerSafeCnt--;
    }
    
//    if(!(modemstt.bit.typeChecked))         //模块还未识别
//    {
//        return;
//    }
    
    if(!(modemstt.bit.chnrdy))              //是否完成AT配置
    {
        if(++guc_FastWmSttReqCnt >= 5)      //每5秒钟
        {
            guc_FastWmSttReqCnt = 0;
            //SYS_MODM_Step(Modem_Init);
//            SYS_MODM_Reinit();
//	            HAL_WIFI_Init(TASK_FARP_TKID,"tw14",gs_FarpVar.ap , &g_ulTcp_fd, 4);
//            gs_GPIO.LED_BlinkSet(GPIO_LED_GPRS, 0xFF, 0, 0);
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
//	            HAL_WIFI_BeginCheck();
        }
        return;
    }
	
    gs_FarpVar.wmst |= WMS_STT_GSM;
    gs_FarpVar.mcsq = modemstt.bit.signl;
//    SYS_LED_BlinkSet1(GPIO_LED_GPRS, 1, 10, 10, 0);  //注册GSM网络成功，1HZ闪烁
    
    if(!(modemstt.bit.pppoe))       //是否完成PPP链接
    {
        gs_FarpVar.wmst &= ~WMS_STT_PPP;    //数据通道不可用
        if(++guc_FastWmSttReqCnt >= 30)     //每10秒钟
        {
//	            SYS_Message_Send(MSG_FARP_CNTPPP, TASK_FARP_TKID);
            uint8 msg = MSG_FARP_CNTPPP;
            krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
            
            guc_FastWmSttReqCnt = 0;
        }
//	        
//        if(guc_FastWmSttReqCnt % 5 == 0)     //每3秒钟
//        {
//            HAL_WIFI_BeginCheck();
//        }
        return;
    }
    gs_FarpVar.wmst |= WMS_STT_PPP;
    
    
    


//	    if(gs_FarpVar.clnen == 1)           //客户端模式
    {
        if(gs_FarpVar.login == 0)    //TCP 不在线
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
                        krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
                        
                    }
//	                    if(gui_FarpWmRcSec % 15 == 0)
//	                    {
//	                        int fd = -1;
//	                        fd = HAL_WIFI_TCP_Lnk_Chk();
//	                        if(fd < 0)
//	                        {
//	                            SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
//	                        }
//	                        else
//	                        {
//	                            if(g_ulTcp_fd < 0)
//	                            {
//	                                g_ulTcp_fd = fd;
//	                                guc_FarpLoginConfirmErr = 0;
//	                                gul_FarpLoginDeadTime = gul_SysRunSecs + 10;
//	                                                                //
//	//                                gs_FarpVar.wmst |= WMS_STT_CLN;
//	                                guc_FarpPstTryCnt = 0;          //被动模式重连次数清零
//	                                Farp_SendLoginData(0);          //发送登录帧
//	                                //Farp_SendHKData(0);             //发送心跳帧
//	                                                                //初始化心跳周期
//	                                gui_FarpWmHBCYCnt = 5;//gs_FarpVar.hbcy;
//	                                                                //实时在线模式无有效数据计数器初始化
//	                                gui_FarpWmSafeCnt = gs_FarpVar.hbcy;
//	                                guc_FarpWmRcFailCnt = 0;        //重连失败次数清零
//	                            }
//	                        }
//	                    }
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
                    krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
                    
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
                        krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
                        

                        uc_closeclnflag = 1;
                        guc_FarpWmHKUnRpyCnt = 0; //心跳无回复次数清零
                    }
                    else
                    {
                        //Farp_SendHKData(0);      //发送心跳数据
                        Farp_SendTestData(0);
                        guc_FarpWmHKUnRpyCnt++;
                        gui_FarpWmHBCYCnt = gs_FarpVar.hbcy;
//                        g_ulTcp_fd = HAL_WIFI_TCP_Lnk_Chk();
//	                        int fd = -1;
//	                        fd = HAL_WIFI_TCP_Lnk_Chk();
//	                        if(fd < 0)
//	                        {
//	                            SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
//	                        }
//	                        else
//	                        {
//	                            if(g_ulTcp_fd < 0)
//	                            {
//	                                g_ulTcp_fd = fd;
//	                                guc_FarpLoginConfirmErr = 0;
//	                                gul_FarpLoginDeadTime = gul_SysRunSecs + 10;
//	                                                                //
//	//                                gs_FarpVar.wmst |= WMS_STT_CLN;
//	                                guc_FarpPstTryCnt = 0;          //被动模式重连次数清零
//	//                                Farp_SendLoginData(0);          //发送登录帧
//	                                //Farp_SendHKData(0);             //发送心跳帧
//	                                                                //初始化心跳周期
//	                                gui_FarpWmHBCYCnt = 5;//gs_FarpVar.hbcy;
//	                                                                //实时在线模式无有效数据计数器初始化
//	                                gui_FarpWmSafeCnt = gs_FarpVar.hbcy;
//	                                guc_FarpWmRcFailCnt = 0;        //重连失败次数清零
//	                            }
//	                        }                    
                    }
                }
            }
            else
            {
                gui_FarpWmHBCYCnt = gs_FarpVar.hbcy;
            }
            gui_FarpWmRcSec = 0;                //重试次数清零
                                                // 
//	            if(gs_FarpVar.wkmd == 0)            //实时在线模式
//	            {
//	                //....
//	            }
//	            else if(gs_FarpVar.wkmd == 1)       //被动激活模式
//	            {
//	                gui_FarpBkTmCnt++;              //无数据休眠计数器累加
//	                                                //无数据通讯时间超过定值或需要断开连接
//	                if(gui_FarpBkTmCnt >= gs_FarpVar.slep)
//	                {                               //置断开连接消息
//	                    SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
//	//	                    uint8 msg = MSG_FARP_DISGPRS;
//	//	                    krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	                    
//	                    uc_closeclnflag = 1;
//	                }
//	            }
//	            else                                //短信模式直接断开连接
//	            {
//	                SYS_Message_Send(MSG_FARP_DISGPRS, TASK_FARP_TKID);
//	                
//	//	                uint8 msg = MSG_FARP_DISGPRS;
//	//	                krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
//	                uc_closeclnflag = 1;
//	            }
        }
    }
    

    if((uc_closeclnflag == 1) && (gs_FarpVar.wkmd == 0))
    {
//	        SYS_Message_Send(MSG_FARP_CNTGPRS, TASK_FARP_TKID);
        uint8 msg = MSG_FARP_CNTGPRS;
        krhino_buf_queue_send(&gs_RFMngQueue, &msg, 1);
        
    }
}


//void wireless_send()
//{
//    uint8 msr = getMSR();
//    if ((msr & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)    //空闲状态                            
//    {                                       	
//		//Switch_Channel(behave.freq); //在业务信道进行通信 
//		// 在靠近广播时隙的地方，不能够发送业务数据
//		//if(slot < ((rfpara.rf_slotnum * 2) - 3))
//		{
//			if(checkSendCache() )
//			{	
//
//                if(CheckConflict())
//                {
//                    goto recv;
//                }
//                else
//                {
//                    EZMacPRO_Transmit();//发送RTS	
//                }
//			}
//			else
//            {
//				goto recv;
//            }
//		}
//        return ;
//recv: 
//		{
//			//采集器老化过程中的点名包处理				
//			//EZMacPro_PacketType = PACKET_TYPE_2STEP;
//			EZMacPRO_Receive(); //监听数据包
////			if (BackOffSlot > 0)
////			    BackOffSlot--;						
//		}										
//        
//    }
//
//    
//}
//extern tRadioDriver *Radio;
extern uint8 g_ucUpgradeFlgForPush;
extern uint32 g_ucUpgradeCount;

//=======================================================
//void wireless_mng(void)//状态机处理
//{
//    switch( Radio->Process( ) )
//    {
//    case RF_RX_TIMEOUT:
//
//        break;
//    case RF_RX_DONE:
//    {
//        PKT *pkt;
//        uint8 tail;
//        SYS_Dev_HalfBlinkSet(LED_FAR_RX, 0, 0, 0);
//        
//        if(g_ucUpgradeFlgForPush != 0xAA)
//        {
//            //uint16 frameLen = 0;
//            tail = rxdopktq.tail;
//            pkt = &(rxdopktq.rxpkt[tail]); //接收数据包地址                        
//            //LOG_DEBUG( DBGFMT"======================RF_RX_DONE ========================= = \n",DBGARG);    
//
//            pkt->len = Radio->GetRxPacket( pkt->data);//, ( uint16* )&pkt->len );
//
//            CHN_ENUM channel = PST_CHN_WL;
//            uint8 protocal;   
//            if(gs_FRM.Channel(channel, pkt->data, pkt->len, &protocal) != PST_ERR_OK)
//            {
//                LOG_DEBUG( DBGFMT"rssi[%4.1f] crc check err %02x%02x%02x%02x%02x%02x\n",DBGARG, SX1276LoRaGetPacketRssi(),
//                    pkt->data[18], pkt->data[17],pkt->data[16],pkt->data[15],pkt->data[14],pkt->data[13]);     
//
//                LOG_DUMPHEX(LOG_LEVEL_DEBUG, "err data : \n", pkt->data, pkt->len);
//                
//                SYS_Dev_HalfBlinkSet(LED_FAR_RX, 1, 5, 2);
//                SYS_Dev_HalfBlinkSet(LED_FAR_TX, 1, 5, 2);        
//                break;
//            }
//
//            //frm.chninfo = channel & PST_FRM_CHNNO;  //通道号
//            pkt->protocol = protocal;
//    
//            rxdopktq.rxpkt[rxdopktq.tail].sentoffset = 0; // 清除offset值
//            
//            //保存收到的抄读电表数据
//            rxdopktq.tail = (rxdopktq.tail + 1) & QUEUE_LEN_MASK;
//            rxdopktq.len = (rxdopktq.len + 1);                                          
//        }
//                //MSR = RX_STATE_BIT | RX_STATE_WAIT_FOR_SEND_ACK;
//        break;
//    }
//                
//    case RF_TX_DONE:
//        SYS_Dev_HalfBlinkSet(LED_FAR_TX, 0, 0, 0);
//
//        Radio->StartRx( );
//        if(NULL != g_stSendCacheIndex.ezPkt)
//        {
//            uint16 temp8 = 0;
//            uint8 errCode = 0;
//            g_stSendCacheIndex.ezPkt->bValid = 0;
//            g_stSendCacheIndex.ezPkt->nBackOffSlot = 0;
//            temp8 = g_stSendCacheIndex.ezPkt->TxPkt.index;
//            
//            if(g_stSendCacheIndex.level == CON_SEND_PRIORITY_NORMAL)
//            {
////	                temp8 = g_stSendCacheIndex.ezPk->TxPkt.index;
//                if(cltor_shadow[temp8].nodestatus.bDataAck != 0)
//                {
//                    cltor_shadow[temp8].nodestatus.bDataAck = 0;
//                }
////	                g_stSendCacheIndex.ezPk->bValid = 0;
////	                g_stSendCacheIndex.ezPk->nBackOffSlot = 0;
//                errCode = cltor_shadow[temp8].nodestatus.errCode;
//                
////	                g_ucPktRssiValue[EzDataTxPkt.TxPkt.apdu.seq] = 0;
//            }
//            else if(g_stSendCacheIndex.level == CON_SEND_PRIORITY_HIGH)
//            {
//
//                cltor_shadow[temp8].nodestatus.bNetAck = 0;//对方收到ACK    
//                
//                errCode = cltor_shadow[temp8].nodestatus.result;
//                //cltor[EzNetTxPkt.TxPkt.apdu.index].nod.needack = 0;//对方收到ACK    
//                
////	                g_ucPktRssiValue[EzNetTxPkt.TxPkt.apdu.seq] = 0;
//            }
////	            else
////	            {
////	                EzHHUTxPkt.bValid = 0;
////	                EzHHUTxPkt.nBackOffSlot = 0;
////	                temp8 = 0;
////	            }//	            if(EzCurTxType == EZ_TX_TYPE_DATA)
////	            {
////	                temp8 = EzDataTxPkt.TxPkt.index;
////	                if(cltor[temp8].nodestatus.bDataAck != 0)
////	                {
////	                    cltor[temp8].nodestatus.bDataAck = 0;
////	                }
////	                EzDataTxPkt.bValid = 0;
////	                EzDataTxPkt.nBackOffSlot = 0;
////	                errCode = cltor[temp8].nodestatus.errCode;
////	                
////	//	                g_ucPktRssiValue[EzDataTxPkt.TxPkt.apdu.seq] = 0;
////	            }
////	            else if(EzCurTxType == EZ_TX_TYPE_NET)
////	            {
////	                EzNetTxPkt.bValid = 0;
////	                EzNetTxPkt.nBackOffSlot = 0;
////	                temp8 = EzNetTxPkt.TxPkt.index;
////	                cltor[temp8].nodestatus.bNetAck = 0;//对方收到ACK    
////	                
////	                errCode = cltor[temp8].nodestatus.result;
////	                //cltor[EzNetTxPkt.TxPkt.apdu.index].nod.needack = 0;//对方收到ACK    
////	                
////	//	                g_ucPktRssiValue[EzNetTxPkt.TxPkt.apdu.seq] = 0;
////	            }
////	            else
////	            {
////	                EzHHUTxPkt.bValid = 0;
////	                EzHHUTxPkt.nBackOffSlot = 0;
////	                temp8 = 0;
////	            }
//            
//            if ((temp8 != 0) && (cltor[temp8].devAddr[0] < 0xFF)
//                && (cltor[temp8].nodestatus.NetStatus == NODE_STATUS_OUT
//                && errCode == 0)
//                /*&& (cltor[temp8].nod.ans_pkt_type == 0)*/)
//            {
//                cltor[temp8].nodestatus.NetStatus = NODE_STATUS_LOGIN;
//            }
//        }
//        break;
//    case RF_TX_TIMEOUT:
//        
////	        if(EzCurTxType == EZ_TX_TYPE_DATA)//1秒后发
////	        {
////	            EzDataTxPkt.nBackOffSlot = 40;
////	        }
////	        else
////	        {
////	            EzNetTxPkt.nBackOffSlot = 40;
////	        }
//
//        g_stSendCacheIndex.ezPkt->nBackOffSlot = FUNC_DELAY_MS(1000);
//        //Radio->StartRx( );
//        break;
//    case RF_IDLE:
//        Radio->StartRx( );
//        break;
////	    case RF_RX_CRCERR:
////	        LED_BlinkSet(LED_FAR_RX, 1, 5, 2);
////	        break;
//    default:
//        break;
//    }
//
//}
////=======================================================
//void wireless_proc(void)//
//{
//    wireless_mng();
//}

extern __IO uint32_t uwTick;
ktimer_t     g_rf_tick_timer;
void SYS_RFMng_Task(void * arg)
{
    TIME time;
    kstat_t ret = 0;
                                            //链表消息
    //uint8 nmsg;                             //数字消息
    
    krhino_buf_queue_create(&gs_RFMngQueue, "rf_queue",
                         gc_RFMngbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);
    localid = 1; 

    Farp_PreInit();
    Radio = RadioDriverInit( );
    SYS_RF_Init(0,0,0);
    Radio->Tick((uint32 *)&uwTick);
//    static uint8_t ble_name[14] = {'V','S'};
//    ByteArrayBcdToHexString(gs_PstPara.Addr, ble_name+2, 6, 0);
    
//    g_ucPutcharEn = 0;
//
//    SYS_MAIN_Init();
                                            //申请获取时间消息
    SYS_MSG_Apply(TASK_RFMNG_TKID, MSG_CLS_TM);//*初始化监控机制
//    SYS_MSG_Apply(TASK_MAIN_TKID, MSG_CLS_GPIO);
//    SYS_MSG_ApplyExt(TASK_MAIN_TKID, MSG_CLS_UART, UART_CHANNEL_DEBUG);

//    LOG_DEBUG("\nVS Project %s  Softver[%x] Hardver[%x]!!!\n", gucs_PrjCode, gul_UsrFuncVer, gul_UsrHardcVer);
    //Flash_Test();
    krhino_timer_create(&g_rf_tick_timer, "rf_tick_timer", Slot_Time_IRQ,
                        krhino_ms_to_ticks(SYS_TIMER_COUNT_MS), krhino_ms_to_ticks(SYS_TIMER_COUNT_MS), 0, 1);
    for(;;)
    {   
        ret = krhino_buf_queue_recv(&gs_RFMngQueue, 1/*RHINO_WAIT_FOREVER*/, g_RFMng_buf_recv,
                          &g_RFMng_recv_size);
        if(ret == RHINO_SUCCESS)
        {
            switch(g_RFMng_buf_recv[0])
            {
                case MSG_SEC:
                    Farp_WMSecondProc();
                    break;
                case MSG_FARP_RECVDATA:
                {
                    PKT *pkt;
            		while (rxdopktq.len > 0)  //如果接收队列里有数据  	      
            		{
            			pkt = &(rxdopktq.rxpkt[rxdopktq.head]); //无线接收队列的数据包地址***
            			uint8_t Return = PST_RX_Down_PKT(pkt); //处理无线数据包
            			if(0 == Return)
            			{
            				Clean_PKT(pkt); // 清除rxdopktq的已处理数据
            				rxdopktq.len = (rxdopktq.len - 1); //队列长度减1
            				rxdopktq.head = (rxdopktq.head + 1) & QUEUE_LEN_MASK; //判断翻转
            			}
            			else
                        {         
                            break;
                        }
            		}
                    break;
                }
                case MSG_LIVE:                  //回复保活消息
                    HB_RetLive(TASK_RFMNG_TKID);
                    break; 
                default:
                    
                    break;
            }
        }
        else
        {
//#ifdef MASTER_NODE          
//	            SYS_A7139_Proc(1);
//#else
//            SYS_A7139_Proc(0);
//#endif            
        }
    }
}

/******************************************************************************
**RF进程块常量部分定义
******************************************************************************/
const KTaskConst gs_TkLP2PCmm = 
{
    &gs_RFMngHandle,
    SYS_RFMng_Task,
    gs_RFMngStack,
    TASK_RFMNG_TKID,                           //进程编号
    true,                                    //消息循环机制进程
    TASK_RFMNG_STKL,
    "rfmng",
    &gs_RFMngQueue,
};
