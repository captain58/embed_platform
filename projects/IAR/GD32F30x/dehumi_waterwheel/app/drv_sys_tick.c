#define MAIN_VAR
#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "sys.h"
#include "bsp.h"
#include "public.h"
#include "user_func.h"
#include "A7139reg.h"
#include "wirelessSendCache.h"
#include "paradef.h"
//extern uint32 curslottime; //目前的总时隙个数



extern uint8 Change_MR0 ;//是否操纵FLASH标识位

uint32 UartDeadCont;
extern uint32 netcontime;
//extern uint8 Uart_Status;
extern uint32 EzMacDeadCont;
//extern RF_PARAMETER rfpara;//设置无线通信参数
extern EZMACPROREG EZMacProReg;

extern EZ_TEST ez_test;//发射功率测试
extern LISN_TEST ls_test;//灵敏度测试

extern uint32 RsResetTime;
//extern UPPKT recvuppkt;
//extern UPPKT senduppkt;
//extern UPPKT SendMeterpkt;
extern QUEUE rxdopktq;
//extern uint8 bJcDisallowFalg;
//extern COLLECTOR cltor[MAX_SUP_SS_NUM + 1];//子节点信息数组
//extern COLLECTOR_SHADOW cltor_shadow[MAX_SUP_SS_NUM + 1];//子节点信息数组

//	extern uint8 SystemStatus;
extern uint8 BusyLBT;
extern uint8 NetAvalidFreq[8]; //use to record the avalible channel in total network
extern uint32 CurScanFreq; // 
//	extern uint8 MacAvalidChan[MAX_AVAILBLE_FRQ];
extern uint8 AvalidChanNum;

extern uint8  broad_fhc,data_fhc;//补偿电容的值 ;//广播实际偏移、业务实际偏移
extern CHANNEL_PARA channel_para;//信道参数
extern uint8  broad_fhc,data_fhc;//补偿电容的值 ;//广播实际偏移、业务实际偏移
extern uint32 NodeUpdateTime; // avoid the first time clean
extern ADVER adpkt; // 广播包结构体
//extern uint8 localid; // 本地ID
extern uint8 MacFrqList[MAX_AVAILBLE_FRQ];
//extern volatile uint32 MSR;
extern uint8 ChannelIsFree;
volatile unsigned int MSR = 0;
extern uint8 netcid ;
extern uint32 curtimecycle; // 当前总时帧
//灵敏度接收完成后标志
extern uint8 EzTestRevDone;
extern uint8 EZMacPro_PacketType;
//extern uint8 IapAction;

//extern uint32 HashUpdateSectorAdd;
//extern uint32 updata_code_cache[64];//256byte缓存
//extern uint8 Write_Sucess;//写FLASH数据正确标志位
//extern uint32 HashUpdateSectorAdd;

extern uint16 pkt_seq; //包顺序号
//extern EZ_TX_PKT EzDataTxPkt;//业务包缓存
//extern EZ_TX_PKT EzNetTxPkt;//网络包缓存
//extern EZ_TX_PKT EzHHUTxPkt;//网络包缓存

extern EZ_Cur_Tx_Type EzCurTxType; 
extern uint8 BackOffSlot;//避退时间
extern uint32 nCurTimeMin;

//	extern DEBUG_UPPKT DebugPacket[DEBUG_PACKET_LEN];
extern uint8 MeterEventSendStatus;	

//	extern CTRPKT AckPkt;
//	extern CTRPKT CtsPkt;
//	extern CTRPKT RtsPkt;
//extern UPDATE_PKT upd_pkt;//升级包结构体
extern ADVER adpkt; // 广播包结构体

extern uint8 adv_subtype;
extern uint8 localhop;
extern uint8 nBroadSendData;
extern uint8 bBroadStart;
extern tRadioDriver *Radio;
//extern uint8 nDeviceMacAddr[6];
uint8 nMeterEventSendTime = 0;
uint8 EZMacProLBT_Retrys = 0;
uint32 topo_deadtime = 0;
uint32 g_ulUartIdelCount;
//extern uint32 ExtCtrlDeadCont;
#if SEND_FREQ_CTRL
extern uint8 g_ucSendCount[MAX_AVAILBLE_FRQ];
#endif
tRadioDriver *Radio = NULL;
//extern uint8 g_ucPktRssiValue[256];

uint8 g_ucMacBSN = 0;
extern uint8_t guc_netStat;
extern uint8_t guc_RegisterStat;
uint8 Flash_Proc(void);
unsigned char getMSR(void);
void setMSR(unsigned char cmsr);
void WDTFeed( void )
{}

//extern uint8 g_ucUpgradeFlgForPush;
//extern uint32 g_ucUpgradeCount;
//extern uint16 g_sUartByteCount[UART_MAX_CHANNEL];
//extern uint8 g_sUartRxDone[UART_MAX_CHANNEL];
static double g_rfLR_rssi_lmit = RFLR_RSSI_VALUE_LIMT;
uint32 g_ucUpgradeCount = 0;
extern uint8 g_ucUpgradeFlgForPush;

#define Get_Current_Behave2(x, y) Get_Current_Behave(x, y)

void Switch_Channel(unsigned char channel)
{
        // 切换到相应频段
    //SpiWriteRegister(SI4432_FREQUENCY_HOPPING_CHANNEL_SELECT, channel);
    return ;
}

/*************************************************************************
 * Function Name: CheckSum
 * Parameters:  buf and length of pkt 
 * Return:  checksum value
 * Description: calculate the cs
 *
 *************************************************************************/
//uint8 Check_SUM(uint8 *data, int len)
//{
//        unsigned char cksum = 0;
//        int i;
//
//        for (i = 0; i < len; i++)
//        {
//                cksum += data[i];
//        }
//
//        return (cksum);
//}
#define CON_LOGIN_TIME_OUT 400
#define CON_LOGIN_INTERVAL 160
uint32 sysSlotTimeCycle = 50;
uint32_t gn_loginTO = 0;
void Slot_Time_IRQ(void * arg)
{
	//uint16 slot;
    uint16 slotNum = MAX_SLOT_NUM;
    uint16 curSlot = sysSlotTime ;//% slotNum;
	BEHAVIOR behave;
	//uint16 temp8;
    behave = Get_Current_Behave2(curSlot, sysSlotTimeCycle);//获得当前行为
    uint8 msr = getMSR();
    uint32 tcnt1,tcnt2;

	tcnt1 = 0;//T1TC; //记录IAP起动时间

    switch (behave.behave)
    {	              
        case BEHAVIOR_BROADCAST://本地发广播
        {

//	            if (MSR == EZMAC_PRO_IDLE)           //空闲状态                     
//	            {
//	                Switch_Channel(behave.freq);//切换频点
//	                EZMacPRO_Transmit_Adv(0, NULL, 0);										
//	            }
//	            else
//	            {
//	                //
//	            }
//            EZMacPRO_Transmit_Beacon(1, behave.rebirth);
            if(guc_RegisterStat == NODE_STATUS_LOGIN)
            {
                if(guc_netStat == NODE_STATUS_LOGIN)
                {
                    EZMacPRO_Transmit_Adv(3, NULL, 0);	
                    sysSlotTimeCycle = CON_LOGIN_TIME_OUT;
                }
                else
                {
                    EZMacPRO_Transmit_Adv(5, NULL, 0);
                    sysSlotTimeCycle = CON_LOGIN_INTERVAL;
                }
            }
            else
            {
                sysSlotTimeCycle = CON_LOGIN_TIME_OUT;
            }

            break;

        }
        case BEHAVIOR_BROADALLOWLOGIN:
            EZMacPRO_Transmit_Adv(4, NULL, 0);	
            break;
        case BEHAVIOR_RECV_ADV://收广播
        {

            if ((msr & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
            {
                
            }

            break;
        }
        case BEHAVIOR_SHARE_TX:   //业务时隙                     
        {		
		//对业务与网络维护的缓存做TTL的老化

            decTTL(msr);
        
            wireless_send();
            //无线状态机
            wireless_proc();


            break;
        }
        default:
        {
            //do nothing
        }
    }

    sysSlotTime++;

		//取最大有效SS存储空间
		for (int Loop = 1; Loop <= rfpara.rf_slotnum; Loop++)
	{
    	COLLECTOR *pCltor = &(cltor[Loop]);
        COLLECTOR_SHADOW *pCltor_shadow = &(cltor_shadow[Loop]);
    	if(pCltor->devAddr[0] < 0xFF)
  		{
  			//老化入网回复包与心跳包
	   		if(pCltor_shadow->nodestatus.bNetAck || pCltor_shadow->nodestatus.bDataAck)
	   		{
	   			if(pCltor_shadow->FrameDeadTime > 1)
					pCltor_shadow->FrameDeadTime --;	
				else
                {            
	   				pCltor_shadow->nodestatus.bDataAck = 0;
                    pCltor_shadow->nodestatus.bNetAck = 0;
                }
	   		}
            else if(pCltor->nodestatus.bNeedDelete)
            {
                if(pCltor_shadow->FrameDeadTime > 0)
					pCltor_shadow->FrameDeadTime --;	
            }
  		}
		else
		{
			//老化SS空间的消亡时间
//					if(pCltor->DeadTime > 0)
//						pCltor->DeadTime --;
		}
    }
 

//	tcnt2 = T1TC; //记录IAP终止时间
//	if(tcnt2>tcnt1)
//		tcnt2 -= tcnt1;
//	else
//	{
//		tcnt2+=(0-tcnt1);
//	}
//
//	tcnt2+= 1;//SYS_TIMER_SLOT_INTERVAL_PRE_TICKS;		//保护时间
//	sysSlotTime+=(tcnt2/SYS_TIMER_SLOT_INTERVAL_PRE_TICKS);
//    sysSlotTimeCycle = sysSlotTime / (slotNum);//时帧计数
//    
//	TIMER1_MR1_UPDATE((tcnt2/SYS_TIMER_SLOT_INTERVAL_PRE_TICKS) * SYS_TIMER_SLOT_INTERVAL_PRE_TICKS);  // 正常心跳

}
/*************************************************************************
 * Function Name: SetSysTickFlag
 * Parameters: void
 * Return: void
 *
 * Description: 每一个时隙的中断调用函数，网络时隙计时；根据
当前的时隙号和时帧号来确定此次的行为；发送广播，接收
广播及发送业务。
 *		
 *************************************************************************/
void Set_System_Tick_Flag(void *arg) //每隔25ms运行一次
{
	uint16 slot;
	//uint16 temp8;
	BEHAVIOR behave;
	uint16 Loop;
//    if(curslottime % 4 == 0)
//    {
//        LED_Server(NULL);
//    }
	slot = curslottime % (rfpara.rf_slotnum * 2);//当前时隙在实帧中的第几个时隙
	uint8 bSSFound;
	
	Change_MR0=0;//恢复正常定时
	
    if (UartDeadCont > 0)
        UartDeadCont--;
    else
        UartDeadCont = 0;

	if(EzMacDeadCont > 0)
		EzMacDeadCont --;
	else
	{
		EzMacDeadCont = 0;
		if(bJcDisallowFalg)
			bJcDisallowFalg = 0;
	}

    if(ExtCtrlDeadCont > 0)
    {
        ExtCtrlDeadCont--;
    }
    else
    {
        ExtCtrlDeadCont = 0;
    }

    g_ulUartIdelCount++;

    if(g_stMuchframeindex_port.ttl > 0)
    {
        g_stMuchframeindex_port.ttl--;
    }
    else
    {
        g_stMuchframeindex_port.ttl = 0;
    }
    
    if ((curslottime % 40) == 0)
    {
        //if (Check_Network())
//        {
//            // 如果网络中有节点存在，则喂狗，否则超时
//            // 正常情况下，10s时间会喂一次狗，超过100s左右的时间，系统会重启
//            WDTFeed();
//        }

    }

//    for (Loop = 0; Loop <= UART_MAX_CHANNEL; Loop++)//遍历采集器状态
//    {                
//        if (g_sUartRxDone[Loop] == 0 && g_sUartByteCount[Loop] > 0)// 只要有节点在线，则返回正常
//        {
//            g_sUartByteCount[Loop]--;
//            if(g_sUartByteCount[Loop] == 0)
//            {
//                g_sUartRxDone[Loop] = 1;
//            }
//        }
//    }		
    
    if ((curslottime % UPLINK_TIMEOUT) == 0)
    {
        bSSFound = 0;
        if((ez_test.msr==EZ_TEST_BIT) || (ls_test.msr==EZ_TEST_BIT))
        {
            //当处于发射与接收的测试条件下，禁止重启
            RsResetTime = RS_RESET_TIME;
        }
        else 
        {
            for (Loop = 2; Loop <= rfpara.rf_slotnum; Loop++)//遍历采集器状态
            {                
                if (cltor[Loop].nodestatus.NetStatus == NODE_STATUS_LOGIN)// 只要有节点在线，则返回正常
                {
                    bSSFound = 1;
                    break;
                }
            }		
            if(0 == bSSFound)
            {
                if(RsResetTime > 0)
                    RsResetTime --;
                
//                LED_BlinkSet1(LED_NET, 1, 10, 10, 0);
            }
            else
            {
                RsResetTime = RS_RESET_TIME;
                
//                LED_BlinkSet1(LED_NET, 1, 1, 1, 0);
            }
        }
	
    	if(0 == RsResetTime)
    	{
    		//重启
//    		Reset();
    	}

        
    }

    ChannelIsFree = CHANNEL_CLEAR_STATE;

    behave = Get_Current_Behave(slot, curtimecycle & 0xFF);//获得当前行为

#if DISTANCE_TEST
    ls_test.msr=EZ_TEST_BIT;  //拉距测试or 发射测试
    ls_test.netid=2;
    ls_test.frq=0x20;

#endif 

#if SEND_TEST_CARRIER
    ez_test.msr=EZ_TEST_BIT;  //拉距测试or 发射测试
    ez_test.netid=2;
    ez_test.frq=broad_fhc;//behave.freq;
#endif    
    
#if SEND_TEST_JUMP
    ez_test.msr=EZ_TEST_BIT;  //拉距测试or 发射测试
    ez_test.netid=2;
    ez_test.frq=behave.freq;
#endif   

	if(ez_test.msr==EZ_TEST_BIT)//如果是测试发射模式
    {
#if CW_TEST
        CW_generator(0x1f);
        WDTFeed();
#else 	
        if (MSR == EZMAC_PRO_IDLE)           //空闲状态                     
        {
            Switch_Channel(ez_test.frq);//切换频点
            ez_test.cnt++;
            EZMacPRO_Transmit_Test();	//发送		
        }
#endif			
    }
	else if(ls_test.msr==EZ_TEST_BIT)	//如果是测灵敏度接收模式
    {
#if 1   
        if(curslottime==ls_test.deadtime)
        {			  
            ls_test.msr=0;//退出接收模式
            EzTestRevDone = 1; 
        }
#endif
        WDTFeed();
        Switch_Channel(ls_test.frq);//切换频点
        EZMacPro_PacketType = PACKET_TYPE_BORADCAST;//接收的包类型为广播包
        netcid=ls_test.netid;//网络号
//	        EZMacPRO_Receive(); //监听数据包
    }
	else
    {           
//        switch (behave.behave)
//        {	              
//            case BEHAVIOR_BROADCAST://本地发广播
//            {
//
//                if (MSR == EZMAC_PRO_IDLE)           //空闲状态                     
//                {
//                    Switch_Channel(behave.freq);//切换频点
//                    EZMacPRO_Transmit_Adv(0, NULL, 0);										
//                }
//                else
//                {
//                    //
//                }
//                break;
//
//            }
//            case BEHAVIOR_RECV_ADV://收广播
//            {
//
//                if ((MSR & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
//                {
//                    
//                }
//
//                break;
//            }
//            case BEHAVIOR_SHARE_TX:   //业务时隙                     
//            {		
//            //对业务与网络维护的缓存做TTL的老化
//                uint8 TTL  = 0;
//                uint8 MsgType;
//                if(EzDataTxPkt.bValid)//业务缓存有效
//                {
//                    TTL = EzDataTxPkt.TxPkt.ttl;//data[0];
//                    if(TTL >= 2)
//                    {
//                        TTL--;
//                        EzDataTxPkt.TxPkt.ttl = TTL;//data[0]
//                    }
//                    else
//                    {
//                        if((MSR & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
//                        {
//                            EzDataTxPkt.bValid = 0;
//                            EzDataTxPkt.nBackOffSlot = 0;
//                            MsgType = EzNetTxPkt.TxPkt.head.apdu.ctrl.ftd;//.pkthead.ctrl.bits.msgtype;
//                            if(MsgType == MSG_TYPE_CONFIRM)
//                            {
//                                temp8 = EzNetTxPkt.TxPkt.head.apdu.index;//.data[3];
//                                cltor_shadow[temp8].nod.bNeedAck = 0;	//本地已回ACK							
//                            }                            
//                        }
//                    }
//                }
//                if(EzNetTxPkt.bValid)//网络报文缓存有效
//                {
//                    TTL = EzNetTxPkt.TxPkt.ttl;//data[0];
//                    if(TTL >= 2)
//                    {
//                        TTL--;
//                        EzNetTxPkt.TxPkt.ttl = TTL;//data[0]						
//                    }
//                    else
//                    {
//                        if((MSR & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
//                        {
//                            EzNetTxPkt.bValid = 0;
//                            EzNetTxPkt.nBackOffSlot = 0;
//                            MsgType = EzNetTxPkt.TxPkt.head.apdu.ctrl.ftd;//.pkthead.ctrl.bits.msgtype;
//                            if(MsgType == MSG_TYPE_ROUTE_REQ)
//                            {
//                                temp8 = EzNetTxPkt.TxPkt.head.apdu.index;//.data[3];
//                                cltor_shadow[temp8].nod.needack= 0;	//本地已回ACK							
//                            }
//                        }
//                    }
//                }
//                if(EzHHUTxPkt.bValid)//业务缓存有效
//                {
//                    TTL = EzHHUTxPkt.TxPkt.ttl;//data[0];
//                    if(TTL >= 2)
//                    {
//                        TTL--;
//                        EzHHUTxPkt.TxPkt.ttl = TTL;//data[0]
//                    }
//                    else
//                    {
//                        if((MSR & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
//                        {
//                            EzHHUTxPkt.bValid = 0;
//                            EzHHUTxPkt.nBackOffSlot = 0;
//                        }
//                    }
//                }
//                
//                //对业务与网络维护的避退时间做更新
//                if(EzDataTxPkt.nBackOffSlot > 0)
//                    EzDataTxPkt.nBackOffSlot --;
//                if(EzNetTxPkt.nBackOffSlot > 0)
//                    EzNetTxPkt.nBackOffSlot --;
//#if SEND_FREQ_CTRL
//                if(behave.freq >= data_fhc)//安全判断，
//                {//业务时隙是否可以发送数据
//                    if(0 != g_ucSendCount[behave.freq - data_fhc])	
//                    {
//                        g_ucSendCount[behave.freq - data_fhc]++;
//                        if(g_ucSendCount[behave.freq - data_fhc] > 3)
//                        {
//                            g_ucSendCount[behave.freq - data_fhc] = 0;
//                        }
//                    }         
//                }
//#endif            
//
////	                wireless_send();
////	                //无线状态机
////	                wireless_proc();
//
//
//                break;
//            }
//            default:
//            {
//                //do nothing
//            }
//
//        }
	}
//end: 

//    Flash_Proc();

    
//	TIMER1_MR0_UPDATE(SYS_TIMER_COUNT_PRE_TICKS);  // 正常心跳
	
	if(Change_MR0)
	{
		curslottime += Change_MR0;
//		TIMER1_MR0_UPDATE(SYS_TIMER_COUNT_PRE_TICKS*Change_MR0);
		Change_MR0 =0;
	}
		
    curslottime++;//时隙总计数
    curtimecycle = curslottime / (rfpara.rf_slotnum*2);//时帧计数
	nCurTimeMin = curslottime / 2400;

	if(1 == (slot % 2) )	//业务时隙做串口报文的老化处理
	{
		if(recvuppkt.deadtime > 1)
			recvuppkt.deadtime --;
		else
			recvuppkt.deadtime = 0;

//	if(bShareTX)	//业务时隙做老化处理
//	{
		//取最大有效SS存储空间
		for (Loop = 1; Loop <= rfpara.rf_slotnum; Loop++)
		{
        	COLLECTOR *pCltor = &(cltor[Loop]);
            COLLECTOR_SHADOW *pCltor_shadow = &(cltor_shadow[Loop]);
        	if(pCltor->devAddr[0] < 0xFF)
      		{
      			//老化入网回复包与心跳包
    	   		if(pCltor_shadow->nodestatus.bNetAck || pCltor_shadow->nodestatus.bDataAck)
    	   		{
    	   			if(pCltor_shadow->FrameDeadTime > 1)
    					pCltor_shadow->FrameDeadTime --;	
    				else
                    {            
    	   				pCltor_shadow->nodestatus.bDataAck = 0;
                        pCltor_shadow->nodestatus.bNetAck = 0;
                    }
    	   		}
                else if(pCltor->nodestatus.bNeedDelete)
                {
                    if(pCltor_shadow->FrameDeadTime > 0)
    					pCltor_shadow->FrameDeadTime --;	
                }
      		}
			else
			{
				//老化SS空间的消亡时间
//					if(pCltor->DeadTime > 0)
//						pCltor->DeadTime --;
			}
	    }
	 
		//增加对ID为0的入网包的回复包消亡时间
		if(cltor[0].devAddr[0] < 0xFF)	
		{
            if(cltor_shadow[0].nodestatus.bNetAck)
            {
                if(cltor_shadow[0].FrameDeadTime > 1)
                    cltor_shadow[0].FrameDeadTime --;			
                else
                    cltor_shadow[0].nodestatus.bNetAck = 0;	
			}
		}
	}

	//上报电表状态报文的老化时间
	//路灯项目的事件上报不需要回复
//	if(USER_DEFINE != USER_LuDeng)
//	{
//        if(1 == MeterEventSendStatus)
//        {
//            if(nMeterEventSendTime > 0)
//                nMeterEventSendTime--;
//            else
//                MeterEventSendStatus = 0;
//        }
//	}

    if(g_ucUpgradeFlgForPush == 0xAA)
    {
        g_ucUpgradeCount++;
        if(g_ucUpgradeCount > 12000)
        {
            g_ucUpgradeCount = 0;
            g_ucUpgradeFlgForPush = 0xFF;

        }

    }
}
#if 0
/*************************************************************************
 * Function Name: SetSysTickFlag
 * Parameters: void
 * Return: void
 *
 * Description: 每一个时隙的中断调用函数，网络时隙计时；根据
当前的时隙号和时帧号来确定此次的行为；发送广播，接收
广播及发送业务。
 *		
 *************************************************************************/
void Set_System_Tick_Flag(void *arg) //每隔25ms运行一次
{
	uint16 slot;
	uint8 temp8;
	BEHAVIOR behave;
	//uint32 err;
	uint16 Loop;
//		LED_RX1_OFF();
//		LED_TX1_OFF();	 
//		LED_RX2_OFF();
//		LED_TX2_OFF();
//	    LED_NET_OFF();
    if(curslottime % 4 == 0)
    {
//        LED_Server(NULL);
    }
	slot = curslottime % (rfpara.rf_slotnum * 2);//当前时隙在实帧中的第几个时隙
	uint8 bSSFound;
	
	Change_MR0=0;//恢复正常定时
	
    if (UartDeadCont > 0)
        UartDeadCont--;
    else
        UartDeadCont = 0;

	if(EzMacDeadCont > 0)
		EzMacDeadCont --;
	else
	{
		EzMacDeadCont = 0;
		if(bJcDisallowFalg)
			bJcDisallowFalg = 0;
	}

    if(ExtCtrlDeadCont > 0)
    {
        ExtCtrlDeadCont--;
    }
    else
    {
        ExtCtrlDeadCont = 0;
    }

    g_ulUartIdelCount++;

    if(g_stMuchframeindex_port.ttl > 0)
    {
        g_stMuchframeindex_port.ttl--;
    }
    else
    {
        g_stMuchframeindex_port.ttl = 0;
    }
    
    if ((curslottime % 40) == 0)
    {
        //if (Check_Network())
        {
            // 如果网络中有节点存在，则喂狗，否则超时
            // 正常情况下，10s时间会喂一次狗，超过100s左右的时间，系统会重启
            WDTFeed();
        }

    }

//    for (Loop = 0; Loop <= UART_MAX_CHANNEL; Loop++)//遍历采集器状态
//    {                
//        if (g_sUartRxDone[Loop] == 0 && g_sUartByteCount[Loop] > 0)// 只要有节点在线，则返回正常
//        {
//            g_sUartByteCount[Loop]--;
//            if(g_sUartByteCount[Loop] == 0)
//            {
//                g_sUartRxDone[Loop] = 1;
//            }
//        }
//    }		
    
    if ((curslottime % UPLINK_TIMEOUT) == 0)
    {
        
    //温度补偿:	6S执行一次
#ifdef EN_TEMPERAUREDEGC
        temp8 = SpiReadRegister(SI4432_ADC_VALUE);					//读出温度值
        if(EZMacProReg.Temper_ADC_Value != temp8)
        {
            EZMacProReg.Temper_ADC_Value = temp8;
            Set_Crystal_Value(EZMacProReg.Temper_ADC_Value);
        }
        SpiWriteRegister(SI4432_ADC_CONFIGURATION, 0x80);				//重新启动ADC
#endif
	
	
        bSSFound = 0;
        if((ez_test.msr==EZ_TEST_BIT) || (ls_test.msr==EZ_TEST_BIT))
        {
            //当处于发射与接收的测试条件下，禁止重启
            RsResetTime = RS_RESET_TIME;
        }
        else 
        {
            for (Loop = 2; Loop <= rfpara.rf_slotnum; Loop++)//遍历采集器状态
            {                
                if (cltor[Loop].nodestatus.NetStatus == NODE_STATUS_LOGIN)// 只要有节点在线，则返回正常
                {
                    bSSFound = 1;
                    break;
                }
            }		
            if(0 == bSSFound)
            {
                if(RsResetTime > 0)
                    RsResetTime --;
                
                LED_BlinkSet1(LED_NET, 1, 10, 10, 0);
            }
            else
            {
                RsResetTime = RS_RESET_TIME;
                
                LED_BlinkSet1(LED_NET, 1, 1, 1, 0);
            }
        }
	
    	if(0 == RsResetTime)
    	{
    		//重启
    		Reset();
    	}

        
    }



    ChannelIsFree = CHANNEL_CLEAR_STATE;

    behave = Get_Current_Behave(slot, curtimecycle & 0xFF);//获得当前行为

#if DISTANCE_TEST
    ls_test.msr=EZ_TEST_BIT;  //拉距测试or 发射测试
    ls_test.netid=2;
    ls_test.frq=0x20;

#endif 

#if SEND_TEST_CARRIER
    ez_test.msr=EZ_TEST_BIT;  //拉距测试or 发射测试
    ez_test.netid=2;
    ez_test.frq=broad_fhc;//behave.freq;
#endif    
    
#if SEND_TEST_JUMP
    ez_test.msr=EZ_TEST_BIT;  //拉距测试or 发射测试
    ez_test.netid=2;
    ez_test.frq=behave.freq;
#endif   

	if(ez_test.msr==EZ_TEST_BIT)//如果是测试发射模式
    {
#if CW_TEST
        CW_generator(0x1f);
        WDTFeed();
#else 	
        if (MSR == EZMAC_PRO_IDLE)           //空闲状态                     
        {
            Switch_Channel(ez_test.frq);//切换频点
            ez_test.cnt++;
            EZMacPRO_Transmit_Test();	//发送		
        }
#endif			
    }
	else if(ls_test.msr==EZ_TEST_BIT)	//如果是测灵敏度接收模式
    {
#if 1   
        if(curslottime==ls_test.deadtime)
        {			  
            ls_test.msr=0;//退出接收模式
            EzTestRevDone = 1; 
        }
#endif
        WDTFeed();
        Switch_Channel(ls_test.frq);//切换频点
        EZMacPro_PacketType = PACKET_TYPE_BORADCAST;//接收的包类型为广播包
        netcid=ls_test.netid;//网络号
//	        EZMacPRO_Receive(); //监听数据包
    }
	else
    {           
	    switch (behave.behave)
        {	              
            case BEHAVIOR_BROADCAST://本地发广播
            {

                if (MSR == EZMAC_PRO_IDLE)           //空闲状态                     
                {
                    Switch_Channel(behave.freq);//切换频点
                    EZMacPRO_Transmit_Adv(0, NULL, 0);										
                }
                else
                {
                    //
                }
                break;

            }
            case BEHAVIOR_RECV_ADV://收广播
            {

                if ((MSR & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
                {
                    
                }

                break;
            }
            case BEHAVIOR_SHARE_TX:   //业务时隙                     
            {		
			//对业务与网络维护的缓存做TTL的老化
        		uint8 TTL  = 0;
        		uint8 MsgType;
    			if(EzDataTxPkt.bValid)//业务缓存有效
    			{
    				TTL = EzDataTxPkt.TxPkt.ttl;//data[0];
    				if(TTL >= 2)
    				{
    					TTL--;
    					EzDataTxPkt.TxPkt.ttl = TTL;//data[0]
    				}
    				else
    				{
    					if((MSR & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
    					{
    						EzDataTxPkt.bValid = 0;
    						EzDataTxPkt.nBackOffSlot = 0;
                            MsgType = EzNetTxPkt.TxPkt.head.apdu.ctrl.ftd;//.pkthead.ctrl.bits.msgtype;
    						if(MsgType == MSG_TYPE_CONFIRM)
    						{
                                temp8 = EzNetTxPkt.TxPkt.head.apdu.index;//.data[3];
                                cltor_shadow[temp8].nod.bNeedAck = 0;	//本地已回ACK							
    						}                            
    					}
    				}
    			}
			    if(EzNetTxPkt.bValid)//网络报文缓存有效
    			{
    				TTL = EzNetTxPkt.TxPkt.ttl;//data[0];
    				if(TTL >= 2)
    				{
    					TTL--;
    					EzNetTxPkt.TxPkt.ttl = TTL;//data[0]						
    				}
    				else
    				{
    					if((MSR & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
    					{
    						EzNetTxPkt.bValid = 0;
    						EzNetTxPkt.nBackOffSlot = 0;
    						MsgType = EzNetTxPkt.TxPkt.head.apdu.ctrl.ftd;//.pkthead.ctrl.bits.msgtype;
    						if(MsgType == MSG_TYPE_ROUTE_REQ)
    						{
                                temp8 = EzNetTxPkt.TxPkt.head.apdu.index;//.data[3];
                                cltor_shadow[temp8].nod.needack= 0;	//本地已回ACK							
    						}
    					}
    				}
    			}
    			if(EzHHUTxPkt.bValid)//业务缓存有效
    			{
    				TTL = EzHHUTxPkt.TxPkt.ttl;//data[0];
    				if(TTL >= 2)
    				{
    					TTL--;
    					EzHHUTxPkt.TxPkt.ttl = TTL;//data[0]
    				}
    				else
    				{
    					if((MSR & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
    					{
    						EzHHUTxPkt.bValid = 0;
    						EzHHUTxPkt.nBackOffSlot = 0;
    					}
    				}
    			}
                
    			//对业务与网络维护的避退时间做更新
    			if(EzDataTxPkt.nBackOffSlot > 0)
    				EzDataTxPkt.nBackOffSlot --;
    			if(EzNetTxPkt.nBackOffSlot > 0)
    				EzNetTxPkt.nBackOffSlot --;
#if SEND_FREQ_CTRL
                if(behave.freq >= data_fhc)//安全判断，
                {//业务时隙是否可以发送数据
    			    if(0 != g_ucSendCount[behave.freq - data_fhc])	
                    {
                        g_ucSendCount[behave.freq - data_fhc]++;
                        if(g_ucSendCount[behave.freq - data_fhc] > 3)
                        {
                            g_ucSendCount[behave.freq - data_fhc] = 0;
                        }
                    }         
                }
#endif            

                wireless_send();
                //无线状态机
                wireless_proc();


                break;
            }
            default:
            {
                //do nothing
            }

        }
	}
//end: 
    
    Flash_Proc();
	
	TIMER1_MR0_UPDATE(SYS_TIMER_COUNT_PRE_TICKS);  // 正常心跳
	
	if(Change_MR0)
	{
		curslottime += Change_MR0;
		TIMER1_MR0_UPDATE(SYS_TIMER_COUNT_PRE_TICKS*Change_MR0);
		Change_MR0 =0;
	}
		
    curslottime++;//时隙总计数
    curtimecycle = curslottime / (rfpara.rf_slotnum*2);//时帧计数
	nCurTimeMin = curslottime / 2400;

	if(1 == (slot % 2) )	//业务时隙做串口报文的老化处理
	{
		if(recvuppkt.deadtime > 1)
			recvuppkt.deadtime --;
		else
			recvuppkt.deadtime = 0;

//	if(bShareTX)	//业务时隙做老化处理
//	{
		//取最大有效SS存储空间
		for (Loop = 1; Loop <= rfpara.rf_slotnum; Loop++)
		{
        	COLLECTOR *pCltor = &(cltor[Loop]);
            COLLECTOR_SHADOW *pCltor_shadow = &(cltor_shadow[Loop]);
        	if(pCltor->devAddr[0] <= 0x99)
      		{
      			//老化入网回复包与心跳包
    	   		if(pCltor_shadow->nod.needack || pCltor_shadow->nod.bNeedAck)
    	   		{
    	   			if(pCltor_shadow->FrameDeadTime > 1)
    					pCltor_shadow->FrameDeadTime --;	
    				else
                    {            
    	   				pCltor_shadow->nod.needack = 0;
                        pCltor_shadow->nod.bNeedAck = 0;
                    }
    	   		}
                else if(pCltor_shadow->nod.bNeedDelete)
                {
                    if(pCltor_shadow->FrameDeadTime > 0)
    					pCltor_shadow->FrameDeadTime --;	
                }
      		}
			else
			{
				//老化SS空间的消亡时间
//					if(pCltor->DeadTime > 0)
//						pCltor->DeadTime --;
			}
	    }
	 
		//增加对ID为0的入网包的回复包消亡时间
		if(cltor[0].devAddr[0] <= 0x99)	
		{
            if(cltor_shadow[0].nod.needack)
            {
                if(cltor_shadow[0].FrameDeadTime > 1)
                    cltor_shadow[0].FrameDeadTime --;			
                else
                    cltor_shadow[0].nod.needack = 0;	
			}
		}
	}

	//上报电表状态报文的老化时间
	//路灯项目的事件上报不需要回复
	if(USER_DEFINE != USER_LuDeng)
	{
        if(1 == MeterEventSendStatus)
        {
            if(nMeterEventSendTime > 0)
                nMeterEventSendTime--;
            else
                MeterEventSendStatus = 0;
        }
	}

    if(g_ucUpgradeFlgForPush == 0xAA)
    {
        g_ucUpgradeCount++;
        if(g_ucUpgradeCount > 12000)
        {
            g_ucUpgradeCount = 0;
            g_ucUpgradeFlgForPush = 0xFF;

        }

    }
}
#endif
/*************************************************************************
 * Function Name: CheckNetwork
 * Parameters:  none
 * Return:   1 表示有节点存在，0 表示无接点存在
 * Description: 检查是否有节点存在在网络中
 *
 *************************************************************************/

uint8 Check_Network()
{
    uint32 i;        
    if (UartDeadCont == 0)
    {              
        return 0;  // 串口收不到任何数据
    }

	for (i = 2; i <= rfpara.rf_slotnum; i++)//遍历采集器状态
    {                
        if (cltor[i].nodestatus.NetStatus == NODE_STATUS_LOGIN)// 只要有节点在线，则返回正常
        {
            return 1;
        }
    }
    return 0;
//        return 1;
}


/*************************************************************************
 * Function Name: EZMacPRO_Transmit
 * Parameters: EZmac发送数据调用函数，发送
 * Return: MAC_OK
 *
 * Description: 
 *		
 *************************************************************************/


uint8 EZMacPRO_Transmit(void)/*STAPDU * stPdu*/
{   
        //DISABLE_MAC_EXT_INTERRUPT(); //禁止响应无线中断
	EZ_TX_PKT* pEzTxPkt = NULL;
    double pktRssi = 0;

    uint8 sendLevel = 0;
    pEzTxPkt = getReadySendCache(&sendLevel);
    if(pEzTxPkt == NULL)
    {
        return 0;
    }
    g_stSendCacheIndex.level = sendLevel;
    g_stSendCacheIndex.ezPkt = pEzTxPkt;
//		if(EzCurTxType == EZ_TX_TYPE_DATA)
//			pEzTxPkt = &EzDataTxPkt;
//		else if(EzCurTxType == EZ_TX_TYPE_NET)
//			pEzTxPkt = &EzNetTxPkt;
//	    else
//	        pEzTxPkt = &EzHHUTxPkt;
//	#ifdef RF_MESH_LEVEL2
//	
//	    if(pEzTxPkt->TxPkt.head.mhr.frame_ctrl.bit1.dir == 1 && pEzTxPkt->TxPkt.head.mhr.index >= SUP_SS_INDEX_START && pEzTxPkt->TxPkt.head.mhr.index <= MAX_SUP_SS_NUM)
//	    {
//	        pktRssi = -cltor_shadow[pEzTxPkt->TxPkt.head.apdu.index].rRssi;
//	    }
//	
//	#else

    if(pEzTxPkt->TxPkt.protocol == PST_FRM_NO)
    {
        pEzTxPkt->TxPkt.protocol = 0;
        if(pEzTxPkt->TxPkt.head.apdu.ctrl.prm == 0 && 
            pEzTxPkt->TxPkt.index >= SUP_SS_INDEX_START && 
            pEzTxPkt->TxPkt.index <= MAX_SUP_SS_NUM)
        {
            pktRssi = -cltor_shadow[pEzTxPkt->TxPkt.index].rRssi;
        }
        LOG_INFO( DBGFMT"CH[%d] ID[%d] rf send ftd = %d, fn = %d seq = %d \n",DBGARG, 
                rfpara.rf_channel, pEzTxPkt->TxPkt.index, pEzTxPkt->TxPkt.head.apdu.ctrl.ftd, pEzTxPkt->TxPkt.head.apdu.fn, pEzTxPkt->TxPkt.head.apdu.seq);
    }
//    else if(pEzTxPkt->TxPkt.protocol == PST_FRM_802_R_NO)
//    {
//        pEzTxPkt->TxPkt.protocol = 0;
//    
//        if(pEzTxPkt->TxPkt.head.mhr.frame_ctrl.bit1.dir == 1 && 
//            pEzTxPkt->TxPkt.index >= SUP_SS_INDEX_START && 
//            pEzTxPkt->TxPkt.index <= MAX_SUP_SS_NUM)
//        {
//            pktRssi = -cltor_shadow[pEzTxPkt->TxPkt.index].rRssi;
//        }
//        LOG_DEBUG( DBGFMT"CH[%d] ID[%d] rf send ftd = %d, fn = %d seq = %d \n",DBGARG, 
//                rfpara.rf_channel, pEzTxPkt->TxPkt.index, 
//                pEzTxPkt->TxPkt.head.mhr.frame_ctrl.bit1.ftd, 
//                pEzTxPkt->TxPkt.head.mhr.fn, pEzTxPkt->TxPkt.head.mhr.seq);
//    }
    else
    {
    	uint16 m = 0;

        if(pEzTxPkt->TxPkt.head.apdu.broadCastFlg != 0xAA)
        {
    //FCD
            pEzTxPkt->TxPkt.data[m++] = *(uint8 *)&pEzTxPkt->TxPkt.head.apdu.ctrl;
            //pEzTxPkt->TxPkt.head.apdu.dir << 7 | pEzTxPkt->TxPkt.head.apdu.prm << 6 | 0x20 |
            //                         pEzTxPkt->TxPkt.head.apdu.FTD;
            m+=2;
        //SEQ
            pEzTxPkt->TxPkt.data[m++] = pEzTxPkt->TxPkt.head.apdu.seq;
        //fn
            pEzTxPkt->TxPkt.data[m++] = pEzTxPkt->TxPkt.head.apdu.fn;
        //固定
            if(pEzTxPkt->TxPkt.head.apdu.ctrl.dir)
            {
                pEzTxPkt->TxPkt.head.apdu.stInfo.stUp.bit1.conflict = 1;
                //pEzTxPkt->TxPkt.head.apdu.stInfo.stUp.bitNumAns = 128;
                pEzTxPkt->TxPkt.head.apdu.stInfo.stUp.bit2.channelFlg =  rfpara.rf_channel & 0x0F;
                pEzTxPkt->TxPkt.head.apdu.stInfo.stUp.len = pEzTxPkt->TxPkt.head.apdu.len;
                pEzTxPkt->TxPkt.head.apdu.stInfo.stUp.rssi = pEzTxPkt->TxPkt.head.apdu.srssi;//g_ucPktRssiValue[pEzTxPkt->TxPkt.head.apdu.seq];
            
            }
            else
            {
                pEzTxPkt->TxPkt.head.apdu.stInfo.stDown.bit1.conflict = 1;
                pEzTxPkt->TxPkt.head.apdu.stInfo.stDown.bitNumAns = 128;
                pEzTxPkt->TxPkt.head.apdu.stInfo.stDown.bit2.channelFlg =  rfpara.rf_channel & 0x0F;
                pEzTxPkt->TxPkt.head.apdu.stInfo.stDown.rssi = pEzTxPkt->TxPkt.head.apdu.srssi;//g_ucPktRssiValue[pEzTxPkt->TxPkt.head.apdu.seq];
            }
            if(pEzTxPkt->TxPkt.head.apdu.ctrl.prm == 0 && pEzTxPkt->TxPkt.index >= SUP_SS_INDEX_START && pEzTxPkt->TxPkt.index <= MAX_SUP_SS_NUM)
            {
                pktRssi = -cltor_shadow[pEzTxPkt->TxPkt.index].rRssi;
            }
            
            memcpy(pEzTxPkt->TxPkt.data + m, &pEzTxPkt->TxPkt.head.apdu.stInfo, sizeof(STINFOWIRE));
            m+=sizeof(STINFOWIRE);
            
            memcpy(pEzTxPkt->TxPkt.data + m, &pEzTxPkt->TxPkt.head.apdu.pandId, 2);
            m+=2;
            if(pEzTxPkt->TxPkt.head.apdu.broadCastFlg == 0x55)
            {
                memcpy(pEzTxPkt->TxPkt.data + m, nDeviceMacAddr, 6);
                m+=6;
                memset(pEzTxPkt->TxPkt.data + m, 0x99, 6);
                m+=6;
            }
            else if(pEzTxPkt->TxPkt.head.apdu.broadCastFlg == 0x66)
            {
                memcpy(pEzTxPkt->TxPkt.data + m, (uint8_t *)sBroadAddrFE, 6);
                m+=6;
                memset(pEzTxPkt->TxPkt.data + m, 0x99, 6);
                m+=6;
            }
            else  if(pEzTxPkt->TxPkt.head.apdu.broadCastFlg == 0x77)
            {
                memcpy(pEzTxPkt->TxPkt.data + m, nDeviceMacAddr, 6);
                m+=6;
                memcpy(pEzTxPkt->TxPkt.data + m, nParentMacAddr, 6);
                m+=6;
            }
            else
            {
                memcpy(pEzTxPkt->TxPkt.data + m, pEzTxPkt->TxPkt.head.apdu.addr, pEzTxPkt->TxPkt.head.apdu.addrlen);
                m+=pEzTxPkt->TxPkt.head.apdu.addrlen;
            }
            memcpy(pEzTxPkt->TxPkt.data + m, pEzTxPkt->TxPkt.head.apdu.data, pEzTxPkt->TxPkt.head.apdu.len);
            m+=pEzTxPkt->TxPkt.head.apdu.len;
            pEzTxPkt->TxPkt.len = m+1;
            pEzTxPkt->TxPkt.data[1] = pEzTxPkt->TxPkt.len & 0xFF;
            pEzTxPkt->TxPkt.data[2] = pEzTxPkt->TxPkt.len >> 8;
            pEzTxPkt->TxPkt.data[m] = Check_SUM(pEzTxPkt->TxPkt.data, m);
            m++;
            if(pEzTxPkt->TxPkt.head.apdu.addr != NULL)
            {
                LOG_INFO( DBGFMT"CH[%d] ID[%d] rf send ftd = %d, fn = %d seq = %d dest addr %02x%02x%02x%02x%02x%02x \n",DBGARG, 
                    rfpara.rf_channel, pEzTxPkt->TxPkt.index, pEzTxPkt->TxPkt.head.apdu.ctrl.ftd, pEzTxPkt->TxPkt.head.apdu.fn, pEzTxPkt->TxPkt.head.apdu.seq, 
                    pEzTxPkt->TxPkt.head.apdu.addr[pEzTxPkt->TxPkt.head.apdu.addrlen - 1], pEzTxPkt->TxPkt.head.apdu.addr[pEzTxPkt->TxPkt.head.apdu.addrlen - 2],
                    pEzTxPkt->TxPkt.head.apdu.addr[pEzTxPkt->TxPkt.head.apdu.addrlen - 3], pEzTxPkt->TxPkt.head.apdu.addr[pEzTxPkt->TxPkt.head.apdu.addrlen - 4],
                    pEzTxPkt->TxPkt.head.apdu.addr[pEzTxPkt->TxPkt.head.apdu.addrlen - 5],pEzTxPkt->TxPkt.head.apdu.addr[pEzTxPkt->TxPkt.head.apdu.addrlen - 6]);
            }
        }
        else //完成组包，直接发送
        {
            m=pEzTxPkt->TxPkt.len - 1;
            pEzTxPkt->TxPkt.data[m] = Check_SUM(pEzTxPkt->TxPkt.data, m);
        }
    }
//	#endif    

    if(g_rfLR_rssi_lmit > pktRssi)
    {
        g_rfLR_rssi_lmit = pktRssi;
    }
    if(g_rfLR_rssi_lmit < RFLR_RSSI_VALUE_LIMT_MIN)
    {
        g_rfLR_rssi_lmit = RFLR_RSSI_VALUE_LIMT_MIN;
    }

//fire
    uint32 timeout = 3;
    //extern tLoRaSettings LoRaSettings;
//    timeout = getWirelessBaudRate(getSF(), getBW(), getErrorCoding());
    if(timeout != 0)
    {
        timeout = (pEzTxPkt->TxPkt.len * 10) / timeout;
        timeout = (timeout == 0) ? 1: timeout;
    }
    else
    {
        timeout = 5;
    }

    //LOG_DEBUG( DBGFMT" TX timeout = %d\n",DBGARG, timeout);
//    SX1276LoRaSetTxPacketTimeout(timeout * 1000 * 2);

    Radio->SetTxPacket( pEzTxPkt->TxPkt.data, pEzTxPkt->TxPkt.len );
    //MSR = TX_STATE_BIT | TX_STATE_LBT_LISTEN;//设状态机   
    LOG_INFO( DBGFMT" rf send type =%d timeout = %d\n",DBGARG, g_stSendCacheIndex.level, timeout);
    LOG_DUMPHEX(LOG_LEVEL_DEBUG, "HD : \n", pEzTxPkt->TxPkt.data, pEzTxPkt->TxPkt.len);
    
    return MAC_OK;
}

/*************************************************************************
 * Function Name: EZMacPRO_Transmit
 * Parameters: EZmac发送数据调用函数，发送
 * Return: MAC_OK
 *
 * Description: 
 *		
 *************************************************************************/
uint8 EZMacPRO_Transmit_WithPKT(EZ_TX_PKT* pEzTxPkt)/*STAPDU * stPdu*/
{   
    double pktRssi = 0;

//	#ifdef RF_MESH_LEVEL2
//	
//	    if(pEzTxPkt->TxPkt.head.mhr.frame_ctrl.bit1.dir == 1 && pEzTxPkt->TxPkt.head.mhr.index >= SUP_SS_INDEX_START && pEzTxPkt->TxPkt.head.mhr.index <= MAX_SUP_SS_NUM)
//	    {
//	        pktRssi = -cltor_shadow[pEzTxPkt->TxPkt.head.apdu.index].rRssi;
//	    }
//	
//	#else
    LOG_INFO( DBGFMT"EZMacPRO_Transmit_WithPKT protocol = %d \n",DBGARG, pEzTxPkt->TxPkt.protocol);

    if(pEzTxPkt->TxPkt.protocol == PST_FRM_NO)
    {
        pEzTxPkt->TxPkt.protocol = 0;
        if(pEzTxPkt->TxPkt.head.apdu.ctrl.prm == 0 && 
            pEzTxPkt->TxPkt.index >= SUP_SS_INDEX_START && 
            pEzTxPkt->TxPkt.index <= MAX_SUP_SS_NUM)
        {
            pktRssi = -cltor_shadow[pEzTxPkt->TxPkt.index].rRssi;
        }
        LOG_INFO( DBGFMT"CH[%d] ID[%d] rf send ftd = %d, fn = %d seq = %d \n",DBGARG, 
                rfpara.rf_channel, pEzTxPkt->TxPkt.index, pEzTxPkt->TxPkt.head.apdu.ctrl.ftd, pEzTxPkt->TxPkt.head.apdu.fn, pEzTxPkt->TxPkt.head.apdu.seq);
    }
//    else if(pEzTxPkt->TxPkt.protocol == PST_FRM_802_R_NO)
//    {
//        pEzTxPkt->TxPkt.protocol = 0;
//    
//        if(pEzTxPkt->TxPkt.head.mhr.frame_ctrl.bit1.dir == 1 && 
//            pEzTxPkt->TxPkt.index >= SUP_SS_INDEX_START && 
//            pEzTxPkt->TxPkt.index <= MAX_SUP_SS_NUM)
//        {
//            pktRssi = -cltor_shadow[pEzTxPkt->TxPkt.index].rRssi;
//        }
//        LOG_DEBUG( DBGFMT"CH[%d] ID[%d] rf send ftd = %d, fn = %d seq = %d \n",DBGARG, 
//                rfpara.rf_channel, pEzTxPkt->TxPkt.index, 
//                pEzTxPkt->TxPkt.head.mhr.frame_ctrl.bit1.ftd, 
//                pEzTxPkt->TxPkt.head.mhr.fn, pEzTxPkt->TxPkt.head.mhr.seq);
//    }
    else
    {
    	uint16 m = 0;

        if(pEzTxPkt->TxPkt.head.apdu.broadCastFlg != 0xAA)
        {
    //FCD
            pEzTxPkt->TxPkt.data[m++] = *(uint8 *)&pEzTxPkt->TxPkt.head.apdu.ctrl;
            //pEzTxPkt->TxPkt.head.apdu.dir << 7 | pEzTxPkt->TxPkt.head.apdu.prm << 6 | 0x20 |
            //                         pEzTxPkt->TxPkt.head.apdu.FTD;
            m+=2;
        //SEQ
            pEzTxPkt->TxPkt.data[m++] = pEzTxPkt->TxPkt.head.apdu.seq;
        //fn
            pEzTxPkt->TxPkt.data[m++] = pEzTxPkt->TxPkt.head.apdu.fn;
        //固定
            if(pEzTxPkt->TxPkt.head.apdu.ctrl.dir)
            {
                pEzTxPkt->TxPkt.head.apdu.stInfo.stUp.bit1.conflict = 1;
                //pEzTxPkt->TxPkt.head.apdu.stInfo.stUp.bitNumAns = 128;
                pEzTxPkt->TxPkt.head.apdu.stInfo.stUp.bit2.channelFlg =  rfpara.rf_channel & 0x0F;
                pEzTxPkt->TxPkt.head.apdu.stInfo.stUp.len = pEzTxPkt->TxPkt.head.apdu.len;
                pEzTxPkt->TxPkt.head.apdu.stInfo.stUp.rssi = pEzTxPkt->TxPkt.head.apdu.srssi;//g_ucPktRssiValue[pEzTxPkt->TxPkt.head.apdu.seq];
            
            }
            else
            {
                pEzTxPkt->TxPkt.head.apdu.stInfo.stDown.bit1.conflict = 1;
                pEzTxPkt->TxPkt.head.apdu.stInfo.stDown.bitNumAns = 128;
                pEzTxPkt->TxPkt.head.apdu.stInfo.stDown.bit2.channelFlg =  rfpara.rf_channel & 0x0F;
                pEzTxPkt->TxPkt.head.apdu.stInfo.stDown.rssi = pEzTxPkt->TxPkt.head.apdu.srssi;//g_ucPktRssiValue[pEzTxPkt->TxPkt.head.apdu.seq];
            }
            if(pEzTxPkt->TxPkt.head.apdu.ctrl.prm == 0 && pEzTxPkt->TxPkt.index >= SUP_SS_INDEX_START && pEzTxPkt->TxPkt.index <= MAX_SUP_SS_NUM)
            {
                pktRssi = -cltor_shadow[pEzTxPkt->TxPkt.index].rRssi;
            }
            
            memcpy(pEzTxPkt->TxPkt.data + m, &pEzTxPkt->TxPkt.head.apdu.stInfo, sizeof(STINFOWIRE));
            m+=sizeof(STINFOWIRE);
            
            memcpy(pEzTxPkt->TxPkt.data + m, &pEzTxPkt->TxPkt.head.apdu.pandId, 2);
            m+=2;
            if(pEzTxPkt->TxPkt.head.apdu.broadCastFlg == 0x55)
            {
                memcpy(pEzTxPkt->TxPkt.data + m, nDeviceMacAddr, 6);
                m+=6;
                memset(pEzTxPkt->TxPkt.data + m, 0x99, 6);
                m+=6;
            }
            else
            {
                memcpy(pEzTxPkt->TxPkt.data + m, pEzTxPkt->TxPkt.head.apdu.addr, pEzTxPkt->TxPkt.head.apdu.addrlen);
                m+=pEzTxPkt->TxPkt.head.apdu.addrlen;
            }
            memcpy(pEzTxPkt->TxPkt.data + m, pEzTxPkt->TxPkt.head.apdu.data, pEzTxPkt->TxPkt.head.apdu.len);
            m+=pEzTxPkt->TxPkt.head.apdu.len;
            pEzTxPkt->TxPkt.len = m+1;
            pEzTxPkt->TxPkt.data[1] = pEzTxPkt->TxPkt.len & 0xFF;
            pEzTxPkt->TxPkt.data[2] = pEzTxPkt->TxPkt.len >> 8;
            pEzTxPkt->TxPkt.data[m] = Check_SUM(pEzTxPkt->TxPkt.data, m);
            m++;
            if(pEzTxPkt->TxPkt.head.apdu.addr != NULL)
            {
                LOG_INFO( DBGFMT"CH[%d] ID[%d] rf send ftd = %d, fn = %d seq = %d dest addr %02x%02x%02x%02x%02x%02x \n",DBGARG, 
                    rfpara.rf_channel, pEzTxPkt->TxPkt.index, pEzTxPkt->TxPkt.head.apdu.ctrl.ftd, pEzTxPkt->TxPkt.head.apdu.fn, pEzTxPkt->TxPkt.head.apdu.seq, 
                    pEzTxPkt->TxPkt.head.apdu.addr[pEzTxPkt->TxPkt.head.apdu.addrlen - 1], pEzTxPkt->TxPkt.head.apdu.addr[pEzTxPkt->TxPkt.head.apdu.addrlen - 2],
                    pEzTxPkt->TxPkt.head.apdu.addr[pEzTxPkt->TxPkt.head.apdu.addrlen - 3], pEzTxPkt->TxPkt.head.apdu.addr[pEzTxPkt->TxPkt.head.apdu.addrlen - 4],
                    pEzTxPkt->TxPkt.head.apdu.addr[pEzTxPkt->TxPkt.head.apdu.addrlen - 5],pEzTxPkt->TxPkt.head.apdu.addr[pEzTxPkt->TxPkt.head.apdu.addrlen - 6]);
            }

        }
        else //完成组包，直接发送
        {
            m=pEzTxPkt->TxPkt.len - 1;
            pEzTxPkt->TxPkt.data[m] = Check_SUM(pEzTxPkt->TxPkt.data, m);
        }
    }
//	#endif    

    if(g_rfLR_rssi_lmit > pktRssi)
    {
        g_rfLR_rssi_lmit = pktRssi;
    }
    if(g_rfLR_rssi_lmit < RFLR_RSSI_VALUE_LIMT_MIN)
    {
        g_rfLR_rssi_lmit = RFLR_RSSI_VALUE_LIMT_MIN;
    }

//fire
    uint32 timeout = 3;
    //extern tLoRaSettings LoRaSettings;
//    timeout = getWirelessBaudRate(getSF(), getBW(), getErrorCoding());
    if(timeout != 0)
    {
        timeout = (pEzTxPkt->TxPkt.len * 10) / timeout;
        timeout = (timeout == 0) ? 1: timeout;
    }
    else
    {
        timeout = 5;
    }

    //LOG_DEBUG( DBGFMT" TX timeout = %d\n",DBGARG, timeout);
//    SX1276LoRaSetTxPacketTimeout(timeout * 1000 * 2);

    Radio->SetTxPacket( pEzTxPkt->TxPkt.data, pEzTxPkt->TxPkt.len );
    //MSR = TX_STATE_BIT | TX_STATE_LBT_LISTEN;//设状态机   
    LOG_INFO( DBGFMT" rf send type =%d timeout = %d\n",DBGARG, g_stSendCacheIndex.level, timeout);
    LOG_DUMPHEX(LOG_LEVEL_DEBUG, "HD : \n", pEzTxPkt->TxPkt.data, pEzTxPkt->TxPkt.len);
//	    g_stSendCacheIndex.level = CON_SEND_PRIORITY_LOW;
//	    g_stSendCacheIndex.ezPkt = pEzTxPkt;
    pEzTxPkt->bValid = 0;
    return MAC_OK;
}
static uint8 g_sucDirectSendFlg = FALSE;

uint8 EZMacPRO_Transmit_Direct(EZ_TX_PKT* pEzTxPkt)/*STAPDU * stPdu*/
{
    uint8 ret = 0;
    uint8 msr = getMSR();
    if ((msr & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)    //空闲状态                            
    {
        if(0 == CheckConflict())
        {
            g_sucDirectSendFlg = TRUE;
            EZMacPRO_Transmit_WithPKT(pEzTxPkt);//发送RTS	
            g_sucDirectSendFlg = FALSE;
            ret = 1;
        }
    }
    return ret;
}

uint8 Flash_Proc(void)
{
  	uint32 err;
//    switch (IapAction)
//    {
//        case IAP_ACTION_ERASE_HASH1:
//        {
//            Change_MR0 = Do_Iap_EraseSector(FLASH_MAP_SECTOR, FLASH_MAP_SECTOR);// 选择扇区20
//            IapAction = IAP_ACTION_IDLE;
//            break;
//        }
//        case IAP_ACTION_ERASE_HASH2:
//        {
//            Change_MR0 = Do_Iap_EraseSector(FLASH_MAP_SECTOR2, FLASH_MAP_SECTOR2);// 选择扇区21                
//            IapAction = IAP_ACTION_IDLE;
//            break;
//        }                   
//        case IAP_ACTION_TRANSFER_HASH1:
//        {                                                   
//            SelSector(FLASH_MAP_SECTOR, FLASH_MAP_SECTOR); // 选择扇区22
//            RamToFlash(HashUpdateSectorAdd, (uint32)updata_code_cache, 256); // 写数据到扇区23*** 
//            err= Compare( HashUpdateSectorAdd,  (uint32)updata_code_cache,  256);  //比较写入是否正确
//
//            if(err!=0)//未正确写入
//                Write_Sucess=ERR;
//            else
//                Write_Sucess=SUCCESS;
//
//
//            IapAction = IAP_ACTION_IDLE;
//            break;
//        }       
//        case IAP_ACTION_TRANSFER_HASH2:
//        {
//            SelSector(FLASH_MAP_SECTOR2, FLASH_MAP_SECTOR2); // 选择扇区23
//            RamToFlash(HashUpdateSectorAdd, (uint32)updata_code_cache, 256); // 写数据到扇区1*** 
//            err= Compare( HashUpdateSectorAdd,  (uint32)updata_code_cache,  256);  //比较写入是否正确
//
//            if(err!=0)//未正确写入  
//                Write_Sucess=ERR;
//            else
//                Write_Sucess=SUCCESS;
//            IapAction = IAP_ACTION_IDLE;
//            break;
//        }
//#if (PROG_FLASH_512)                        
//        case IAP_ACTION_ERASE_SS_CODE://擦除
//        {
//            Change_MR0 = Do_Iap_EraseSector(IAP_SS_FLASH_PAGE_START, IAP_SS_FLASH_PAGE_END); // 选择扇区8
//            IapAction = IAP_ACTION_IDLE;
//            break;
//        }
//        case IAP_ACTION_ERASE_BS_CODE://擦除
//        {
//            Change_MR0 = Do_Iap_EraseSector(IAP_BS_FLASH_PAGE_START, IAP_BS_FLASH_PAGE_END); // 选择扇区15-19
//            IapAction = IAP_ACTION_IDLE;
//            break;
//        }
//        case IAP_ACTION_WRITE_SS_CODE:   //往FLASH里填升级程序的代码                                             
//        {
//            SelSector(IAP_SS_FLASH_PAGE_START, IAP_SS_FLASH_PAGE_END); // 选择扇区8
//
//            RamToFlash(IAP_SS_CODE_START + 256 * pkt_seq, (uint32)updata_code_cache, 256); // 写数据到FLASH*** 
//            IapAction = IAP_ACTION_IDLE;
//
//            if (!memcmp(updata_code_cache,&UpdateSource[ 256 * pkt_seq],256))//比较写入的数据数否正确
//                Write_Sucess=1;
//            else
//                Write_Sucess=0; 
//            break;
//        }
//        case IAP_ACTION_ERASE_I_SS_CODE:                            
//        {
//            Change_MR0 = Do_Iap_EraseSector(IAP_I_SS_FLASH_PAGE_START, IAP_I_SS_FLASH_PAGE_END); // 选择扇区8
//            IapAction = IAP_ACTION_IDLE;                            
//            break;
//        }
//        case IAP_ACTION_WRITE_I_SS_CODE:
//        {
//            SelSector(IAP_I_SS_FLASH_PAGE_START, IAP_I_SS_FLASH_PAGE_END); // 选择
//
//            RamToFlash(IAP_I_SS_CODE_START + 256 * pkt_seq, (uint32)updata_code_cache, 256); // 写数据到FLASH*** 
//            IapAction = IAP_ACTION_IDLE;
//
//            if (!memcmp(updata_code_cache,&Update_I_SS_Source[ 256 * pkt_seq],256))//比较写入的数据数否正确
//                Write_Sucess=1;
//            else
//                Write_Sucess=0;                             
//            break;
//        }
//        case IAP_ACTION_WRITE_BS_CODE:  //往FLASH里填本地升级程序1的代码                                              
//        {
//            SelSector(IAP_BS_FLASH_PAGE_START, IAP_BS_FLASH_PAGE_END); // 选择扇区
//
//            RamToFlash(IAP_BS_CODE_START + 256 * pkt_seq, (uint32)updata_code_cache, 256); // 写数据到FLASH*** 
//            IapAction = IAP_ACTION_IDLE;
//
//            if (!memcmp(updata_code_cache,&Update_Bs_Source[ 256 * pkt_seq],256))//比较写入的数据数否正确
//                Write_Sucess=1;
//            else
//                Write_Sucess=0; 
//            break;
//        }   
//#endif                        
////	        case IAP_ACTION_ERASE_WRITE_SS_HASH1:       //擦除白名单1
////	        {
////	            Change_MR0 = Do_Iap_EraseSector(FLASH_SS_WHITE_MAP_SECTOR1, FLASH_SS_WHITE_MAP_SECTOR1);// 选择扇区22
////	            IapAction = IAP_ACTION_IDLE;                            
////	            break;
////	        }
////	        case  IAP_ACTION_ERASE_WRITE_SS_HASH2:      //擦除白名单2
////	        {
////	            Change_MR0 = Do_Iap_EraseSector(FLASH_SS_WHITE_MAP_SECTOR2, FLASH_SS_WHITE_MAP_SECTOR2);// 选择扇区24
////	            IapAction = IAP_ACTION_IDLE;                                        
////	            break;
////	        }
////	        case IAP_ACTION_ERASE_BLACK_SS_HASH1:       //擦除黑名单1
////	        {
////	            Change_MR0 = Do_Iap_EraseSector(FLASH_SS_BLACK_MAP_SECTOR1, FLASH_SS_BLACK_MAP_SECTOR1);// 选择扇区23
////	            IapAction = IAP_ACTION_IDLE;                                
////	            break;
////	        }
////	        case IAP_ACTION_ERASE_BLACK_SS_HASH2:       //擦除黑名单2
////	        {
////	            Change_MR0 = Do_Iap_EraseSector(FLASH_SS_BLACK_MAP_SECTOR2, FLASH_SS_BLACK_MAP_SECTOR2);// 选择扇区25
////	            IapAction = IAP_ACTION_IDLE;                                        
////	            break;
////	        }
////	        case IAP_ACTION_TRANSFER_WRITE_SS_HASH1:
////	        {
////	            SelSector(FLASH_SS_WHITE_MAP_SECTOR1, FLASH_SS_WHITE_MAP_SECTOR1); // 选择扇区22
////	            RamToFlash(HashUpdateSectorAdd, (uint32)updata_code_cache, 256); // 写数据到扇区22*** 
////	            err= Compare( HashUpdateSectorAdd,  (uint32)updata_code_cache,  256);  //比较写入是否正确
////	
////	            if(err!=0)//未正确写入
////	                Write_Sucess=ERR;
////	            else
////	                Write_Sucess=SUCCESS;
////	            IapAction = IAP_ACTION_IDLE;                            
////	            break;
////	        }
////	        case IAP_ACTION_TRANSFER_BLACK_SS_HASH1:
////	        {
////	            SelSector(FLASH_SS_BLACK_MAP_SECTOR1, FLASH_SS_BLACK_MAP_SECTOR1); // 选择扇区23
////	            RamToFlash(HashUpdateSectorAdd, (uint32)updata_code_cache, 256); // 写数据到扇区23*** 
////	            err= Compare( HashUpdateSectorAdd,  (uint32)updata_code_cache,  256);  //比较写入是否正确
////	
////	            if(err!=0)//未正确写入
////	                Write_Sucess=ERR;
////	            else
////	                Write_Sucess=SUCCESS;
////	            IapAction = IAP_ACTION_IDLE;                            
////	            break;
////	        }
////	        case IAP_ACTION_TRANSFER_WRITE_SS_HASH2:
////	        {
////	            SelSector(FLASH_SS_WHITE_MAP_SECTOR2, FLASH_SS_WHITE_MAP_SECTOR2); // 选择扇区24
////	            RamToFlash(HashUpdateSectorAdd, (uint32)updata_code_cache, 256); // 写数据到扇区24*** 
////	            err= Compare( HashUpdateSectorAdd,  (uint32)updata_code_cache,  256);  //比较写入是否正确
////	
////	            if(err!=0)//未正确写入
////	                Write_Sucess=ERR;
////	            else
////	                Write_Sucess=SUCCESS;
////	            IapAction = IAP_ACTION_IDLE;                                    
////	            break;
////	        }                       
////	        case IAP_ACTION_TRANSFER_BLACK_SS_HASH2:
////	        {
////	            SelSector(FLASH_SS_BLACK_MAP_SECTOR2, FLASH_SS_BLACK_MAP_SECTOR2); // 选择扇区25
////	            RamToFlash(HashUpdateSectorAdd, (uint32)updata_code_cache, 256); // 写数据到扇区25*** 
////	            err= Compare( HashUpdateSectorAdd,  (uint32)updata_code_cache,  256);  //比较写入是否正确
////	
////	            if(err!=0)//未正确写入
////	                Write_Sucess=ERR;
////	            else
////	                Write_Sucess=SUCCESS;
////	            IapAction = IAP_ACTION_IDLE;                                        
////	            break;
////	        }
//        default:
//        {
//            IapAction = IAP_ACTION_IDLE;
//            break;
//        }
//    }
    return 0;
}
/*************************************************************************
 * Function Name: EZMacPRO_Transmit_Adv
 * Parameters: EZmac发送数据调用函数，发送RTS请求
 * Return: MAC_OK
 *
 * Description: 
 *		
 *************************************************************************/
 uint8 nSendData[10];
uint8 TXbuffer[30]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
uint8 Rxbuffer[256];

uint8 EZMacPRO_Transmit_Reset(void)//发送重启广播命令
{   


    
	return MAC_OK;		
}
static uint8 sOffNetworkMeterMngBit = 0;
extern ST_WATER_STT gst_water_stt;
uint8 EZMacPRO_Transmit_Adv(uint8 type, uint8 * data, uint8 len)//发送重启广播命令
{   
    uint16 k = 0;
    uint16 m = 0, n = 0;
    uint32 stt = 0;
    PKT *pkt;

    EZ_TX_PKT * ezPkt = getIdelSendCache(CON_SEND_PRIORITY_NORMAL);
    if(ezPkt == NULL)//网络报文缓存有效
    {
        return 0;
    }
	pkt = &(ezPkt->TxPkt);
    pkt->ttl = MAX_TIME_LIVE;   //生存周期by ben 29
    pkt->index = k;
    
    memset((uint8 *)&pkt->head.apdu, 0, sizeof(STAPDU));

    switch(type)
    {
        case 0://广播离网
            if(sOffNetworkMeterMngBit == 0)
            {
//                sOffNetworkMeterMngBit = checkMeterModuleProtocol();
            }
            if((sOffNetworkMeterMngBit & 0x01) == 0x01)
            {
                sOffNetworkMeterMngBit &= (~0x01);
                pkt->head.apdu.ctrl.dir = 0;
                pkt->head.apdu.ctrl.prm = 1;
                pkt->head.apdu.ctrl.evtmode = 1;
                pkt->head.apdu.ctrl.ftd = 7;
                pkt->head.apdu.seq = cltor_shadow[k].sendseq;
                pkt->head.apdu.broadCastFlg = 0x55;

                pkt->head.apdu.fn = 6;//剔除

                pkt->head.apdu.stInfo.stDown.bit1.routeFlg = 0;
                pkt->head.apdu.stInfo.stDown.bit1.nodeFlg = 0;
                pkt->head.apdu.stInfo.stDown.bit1.addrFlg = 0;
                pkt->head.apdu.stInfo.stDown.bit1.conflict = 1;
                pkt->head.apdu.stInfo.stDown.bit1.routeNum = 0;;//pkt->rarea->stDown.bit1.routeNum;
                pkt->head.apdu.stInfo.stDown.bit2.channelFlg = rfpara.rf_channel;//Cal_Hash_Value(nDeviceMacAddr) % LORA_CHANNEL_NUM;//pkt->rarea->stDown.bit2.channelFlg;
                pkt->head.apdu.stInfo.stDown.bit2.errEncodeFlg = 0;
                pkt->head.apdu.addrlen = 12;
                pkt->head.apdu.addr = pkt->data + PKT_HEAD_LEN;
                pkt->head.apdu.len = 0;
            }
//	            else if((sOffNetworkMeterMngBit & 0x02) == 0x02)
//	            {
//	                sOffNetworkMeterMngBit &= (~0x02);
//	
//	                uint8 m = 0;
//	                memset((uint8 *)&pkt->head.mhr, 0, sizeof(STIEEE));
//	
//	//	        LOG_DEBUG( DBGFMT"Create_JRep ID[%x] rssi[%d] level[%d]\n",DBGARG, k, cltor_shadow[k].rRssi,
//	//	                getRssiQualityByRssi(cltor_shadow[k].rRssi));                        
//	
//	                
//	                pkt->head.mhr.seg_ctrl.ctrl_net.bit3.OC = 0;
//	                pkt->head.mhr.seg_ctrl.ctrl_net.bit2.signalQ = 0x0F;//getRssiQualityByRssi(cltor_shadow[k].rRssi);
//	                pkt->head.mhr.seg_ctrl.ctrl_net.bit2.pwr = rfpara.rf_power;
//	                
//	                pkt->head.mhr.frame_ctrl.bit2.prm = 0;
//	                pkt->head.mhr.frame_ctrl.bit1.ftd = 3;
//	                pkt->head.mhr.frame_ctrl.bit1.panid = 1;
//	                pkt->head.mhr.frame_ctrl.bit1.dir = 0;
//	                
//	                pkt->head.mhr.seq = cltor_shadow[k].recvseq;
//	                pkt->head.mhr.srssi = cltor_shadow[k].rRssi;
//	            
//	                pkt->head.mhr.app = pkt->data;
//	
//	                pkt->head.mhr.fn = 0x03;//离网 
//	                pkt->head.mhr.frame_ctrl.bit2.dest = 0;
//	                pkt->head.mhr.frame_ctrl.bit2.src = 1;
//	                pkt->head.mhr.app[m++] = pkt->head.mhr.fn;
//	                pkt->head.mhr.app[m++] = 0x01;//协调器希望设备断开PAN
//	                pkt->head.mhr.len = m;
//	                memset(cltor[1].devAddr, 0xAA, CON_DEV_ADDR_LEN_8);
//	                IE_Compose_RD(1, ezPkt, &pkt->head.mhr.seg_ctrl, &pkt->head.mhr.frame_ctrl, 
//	                                   pkt->head.mhr.seq, pkt->head.mhr.app, pkt->head.mhr.len);
//	            }
            else
            {
                ezPkt->nBackOffSlot = FUNC_DELAY_MS(500);
                break;
            }
                
            cltor_shadow[k].sendseq++;
    		ezPkt->bValid = 1;//业务缓存有效
    		ezPkt->nBackOffSlot = FUNC_DELAY_MS(500);
        break;
        case 1://电源切换
        {
//            extern EZ_TX_PKT EzBroadCastPkt;//广播包缓存
            pkt->head.apdu.ctrl.dir = 0;
            pkt->head.apdu.ctrl.prm = 1;
            pkt->head.apdu.ctrl.evtmode = 1;
            pkt->head.apdu.ctrl.ftd = 4;
            pkt->data[0] = *(uint8*)&pkt->head.apdu.ctrl;
            memcpy(pkt->data+1, EzBroadCastPkt.TxPkt.data+1, EzBroadCastPkt.TxPkt.len);
            pkt->len = EzBroadCastPkt.TxPkt.len;
            pkt->head.apdu.broadCastFlg = 0xAA;
            ezPkt->bValid = 1;//业务缓存有效
    		ezPkt->nBackOffSlot = FUNC_DELAY_MS(500);
            break;
        }
        case 2://从节点控制字
            
            pkt->head.apdu.ctrl.dir = 0;
            pkt->head.apdu.ctrl.prm = 1;
            pkt->head.apdu.ctrl.evtmode = 1;
            pkt->head.apdu.ctrl.ftd = 1;
            pkt->head.apdu.seq = cltor_shadow[k].sendseq;
            pkt->head.apdu.broadCastFlg = 0x55;

            pkt->head.apdu.fn = 14;//剔除

            pkt->head.apdu.stInfo.stDown.bit1.routeFlg = 0;
            pkt->head.apdu.stInfo.stDown.bit1.nodeFlg = 0;
            pkt->head.apdu.stInfo.stDown.bit1.addrFlg = 0;
            pkt->head.apdu.stInfo.stDown.bit1.conflict = 1;
            pkt->head.apdu.stInfo.stDown.bit1.routeNum = 0;;//pkt->rarea->stDown.bit1.routeNum;
            pkt->head.apdu.stInfo.stDown.bit2.channelFlg = rfpara.rf_channel;//Cal_Hash_Value(nDeviceMacAddr) % LORA_CHANNEL_NUM;//pkt->rarea->stDown.bit2.channelFlg;
            pkt->head.apdu.stInfo.stDown.bit2.errEncodeFlg = 0;
            pkt->head.apdu.addrlen = 12;
            pkt->head.apdu.addr = pkt->data + PKT_HEAD_LEN;
            pkt->head.apdu.data = pkt->data + PKT_HEAD_LEN + pkt->head.apdu.addrlen;
            pkt->head.apdu.len = len;
            memcpy(pkt->head.apdu.data, data, len);
            pkt->head.apdu.broadCastFlg = 0x55;//地址广播


            cltor_shadow[k].sendseq++;
    		ezPkt->bValid = 1;//业务缓存有效
    		ezPkt->nBackOffSlot = FUNC_DELAY_MS(500);
        break;
#ifndef MASTER_NODE        
        case 3:
        {
            pkt->head.apdu.ctrl.dir = 1;//服务器  发广播
            pkt->head.apdu.ctrl.prm = 1;
            pkt->head.apdu.ctrl.evtmode = 1;
            pkt->head.apdu.ctrl.ftd = 3;
            pkt->head.apdu.seq = cltor_shadow[k].sendseq;
            pkt->head.apdu.broadCastFlg = 0x77;

            pkt->head.apdu.fn = 20;//上告

            pkt->head.apdu.stInfo.stDown.bit1.routeFlg = 0;
            pkt->head.apdu.stInfo.stDown.bit1.nodeFlg = 0;
            pkt->head.apdu.stInfo.stDown.bit1.addrFlg = 0;
            pkt->head.apdu.stInfo.stDown.bit1.conflict = 1;
            pkt->head.apdu.stInfo.stDown.bit1.routeNum = 0;;//pkt->rarea->stDown.bit1.routeNum;
            pkt->head.apdu.stInfo.stDown.bit2.channelFlg = rfpara.rf_channel;//Cal_Hash_Value(nDeviceMacAddr) % LORA_CHANNEL_NUM;//pkt->rarea->stDown.bit2.channelFlg;
            pkt->head.apdu.stInfo.stDown.bit2.errEncodeFlg = 0;
            pkt->head.apdu.addrlen = 12;
            pkt->head.apdu.addr = pkt->data + PKT_HEAD_LEN;
            pkt->head.apdu.data = pkt->data + PKT_HEAD_LEN + pkt->head.apdu.addrlen;
//	            n = PKT_HEAD_LEN + pkt->head.apdu.addrlen;
            m+=4;
//	            pkt->head.apdu.len = len;
//	            memcpy(pkt->head.apdu.data, data, len);
//	            pkt->head.apdu.broadCastFlg = 0x55;//地址广播


            stt |= 1<<CON_STT_WATER_LEVEL;
            //pkt->head.apdu.data[m++] = guc_SwitchNorErr;//;guc_SwitchOnOff;//
            memcpy(pkt->head.apdu.data+m, (uint8_t *)&gst_water_stt.st_sensor, 4);
            m+=4;
            //TRFIDModemState strfstt = (TRFIDModemState)0;//HAL_RFID_Status();
//	            stt |= 1<<CON_STT_CARD_OFFSET;
//	            pkt->head.apdu.data[m++] = 0;//strfstt.bit.linked;

//            if(strfstt.bit.linked)
//            {
//                stt |= 1<<CON_STT_CARD_ID_OFFSET;
//                pkt->head.apdu.data[m++] = strfstt.bit.linked;
//                m += 0;//HAL_RFID_GetCardID(pkt->head.apdu.data  + m);
//            }
            memcpy(pkt->head.apdu.data, &stt, 4);
            pkt->head.apdu.len = m;

            cltor_shadow[k].sendseq++;
    		ezPkt->bValid = 1;//业务缓存有效
    		ezPkt->nBackOffSlot = FUNC_DELAY_MS(500);
            break;
        }
        case 5://入网帧
        {
            pkt->head.apdu.ctrl.dir = 1;//服务器  发广播
            pkt->head.apdu.ctrl.prm = 1;
            pkt->head.apdu.ctrl.evtmode = 1;
            pkt->head.apdu.ctrl.ftd = 7;
            pkt->head.apdu.seq = cltor_shadow[k].sendseq;
            pkt->head.apdu.broadCastFlg = 0x77;

            pkt->head.apdu.fn = 3;//入网

            pkt->head.apdu.stInfo.stDown.bit1.routeFlg = 0;
            pkt->head.apdu.stInfo.stDown.bit1.nodeFlg = 0;
            pkt->head.apdu.stInfo.stDown.bit1.addrFlg = 0;
            pkt->head.apdu.stInfo.stDown.bit1.conflict = 1;
            pkt->head.apdu.stInfo.stDown.bit1.routeNum = 0;;//pkt->rarea->stDown.bit1.routeNum;
            pkt->head.apdu.stInfo.stDown.bit2.channelFlg = rfpara.rf_channel;//Cal_Hash_Value(nDeviceMacAddr) % LORA_CHANNEL_NUM;//pkt->rarea->stDown.bit2.channelFlg;
            pkt->head.apdu.stInfo.stDown.bit2.errEncodeFlg = 0;
            pkt->head.apdu.addrlen = 12;
            pkt->head.apdu.addr = pkt->data + PKT_HEAD_LEN;
            pkt->head.apdu.data = pkt->data + PKT_HEAD_LEN + pkt->head.apdu.addrlen;
//	            m = n = PKT_HEAD_LEN + pkt->head.apdu.addrlen;


            pkt->head.apdu.data[m++] = 0;
            pkt->head.apdu.data[m++] = 0;
            //GIS
            memset(pkt->head.apdu.data+m,0,8);
            m+=8;
            //心跳周期（分）
            pkt->head.apdu.data[m++] = 5;
            //精简模块软件版本号
            pkt->head.apdu.data[m++] = 0;
            pkt->head.apdu.data[m++] = 1;
            //精简模块硬件版本号
            pkt->head.apdu.data[m++] = 1;
            //本地时间
            memcpy(pkt->head.apdu.data+m,(uint8_t *)GetTime(),6);
            m+=6;



            pkt->head.apdu.len = m;

            cltor_shadow[k].sendseq++;
            ezPkt->bValid = 1;//业务缓存有效
            ezPkt->nBackOffSlot = FUNC_DELAY_MS(500);
            break;
        }
#else        
       
        case 4:
            pkt->head.apdu.ctrl.dir = 0;//服务器  发广播
            pkt->head.apdu.ctrl.prm = 1;
            pkt->head.apdu.ctrl.evtmode = 1;
            pkt->head.apdu.ctrl.ftd = 4;
            pkt->head.apdu.seq = cltor_shadow[k].sendseq;
            pkt->head.apdu.broadCastFlg = 0x55;

            pkt->head.apdu.fn = 99;//广播入网

            pkt->head.apdu.stInfo.stDown.bit1.routeFlg = 0;
            pkt->head.apdu.stInfo.stDown.bit1.nodeFlg = 0;
            pkt->head.apdu.stInfo.stDown.bit1.addrFlg = 0;
            pkt->head.apdu.stInfo.stDown.bit1.conflict = 1;
            pkt->head.apdu.stInfo.stDown.bit1.routeNum = 0;;//pkt->rarea->stDown.bit1.routeNum;
            pkt->head.apdu.stInfo.stDown.bit2.channelFlg = rfpara.rf_channel;//Cal_Hash_Value(nDeviceMacAddr) % LORA_CHANNEL_NUM;//pkt->rarea->stDown.bit2.channelFlg;
            pkt->head.apdu.stInfo.stDown.bit2.errEncodeFlg = 0;
            pkt->head.apdu.addrlen = 12;
            pkt->head.apdu.addr = pkt->data + PKT_HEAD_LEN;
            pkt->head.apdu.data = pkt->data + PKT_HEAD_LEN + pkt->head.apdu.addrlen;
            pkt->head.apdu.len = len;
            memcpy(pkt->head.apdu.data, data, len);
//            pkt->head.apdu.broadCastFlg = 0x55;//地址广播

            cltor_shadow[k].sendseq++;
    		ezPkt->bValid = 1;//业务缓存有效
    		ezPkt->nBackOffSlot = FUNC_DELAY_MS(500);
            break; 
#endif             
        default:
            break;
    }
	return 1;		
}

/*************************************************************************
 * Function Name: EZMacPRO_Transmit_Beacon
 * Parameters: EZmac发送数据调用函数，发送Beacon
 * Return: MAC_OK
 * Para : type 0:复位;1入网；2升级
 * Description: 
 *		
 *************************************************************************/
uint8 EZMacPRO_Transmit_Beacon(uint8 type, uint8 rebirth)//发送Beacon
{
    uint8 option = 0;

    option = CON_BEACON_CTRL_BIT_BEACON | CON_BEACON_CTRL_BIT_ROUTE | CON_BEACON_CTRL_BIT_UP;

    switch(type)
    {
    case 0:
        option |= CON_BEACON_CTRL_BIT_RESET;
        break;
    case 1:
        option |= CON_BEACON_CTRL_BIT_NETIN;
        break;
    case 2:
        option |= CON_BEACON_CTRL_BIT_NETIN | CON_BEACON_CTRL_BIT_UPGRADE;
        break;  
    default:
        break;
    }

//	    HX_Beacon1(g_ucMacBSN++, option, rebirth);

    return EZMacPRO_Transmit_WithPKT(&EzBeaconTxPkt);
}

/*************************************************************************
 * Function Name: EZMacPRO_Transmit_Adv
 * Parameters: EZmac发送数据调用函数，发送RTS请求
 * Return: MAC_OK
 *
 * Description: 
 *		
 *************************************************************************/
uint8 EZMacPRO_Transmit_RTS(void)//发送RTS请求
{   
//	        DISABLE_MAC_EXT_INTERRUPT(); //禁止响应无线中断
//		
//		EZ_TX_PKT* pEzTxPkt;
//		if(EzCurTxType == EZ_TX_TYPE_DATA)
//			pEzTxPkt = &EzDataTxPkt;
//		else
//			pEzTxPkt = &EzNetTxPkt;
//		
//	        //生成RTS包
//	    //    RtsPkt.pkthead.did = txdopktq.rxpkt[txdopktq.head].pkthead.did;//接收者地址
//		RtsPkt.pkthead.did = pEzTxPkt->TxPkt.pkthead.did;//接收者地址
//		RtsPkt.pkthead.ctrl.bits.DataFlag = pEzTxPkt->TxPkt.pkthead.ctrl.bits.DataFlag;
//		RtsPkt.pkthead.ctrl.bits.UpFlag = pEzTxPkt->TxPkt.pkthead.ctrl.bits.UpFlag;
//	        
//							
//	    if ((pEzTxPkt->TxPkt.pkthead.ctrl.bits.msgtype == MSG_TYPE_JREPON) 
//	    	&& ((pEzTxPkt->TxPkt.data[1] & 0x0f) ==0) 
//	    	&& (cltor[(pEzTxPkt->TxPkt.data[10] )].nod.ans_pkt_type == 1))  //判断入网包是否有中继(判断跳数)
//	    {                
//	        RtsPkt.len=0x7;//长度为7(无中继)     
//	    }
//	    else
//	    {               
//	       RtsPkt.len=0x1;//普通包
//	       RtsPkt.Payload= 0x1;//有中继
//	            
//	    }
//	
//	    Send_Dlink_PKT(&RtsPkt, DELAY_SEND);//向FIFO里填数据
//	    
//	    SpiWriteRegister(SI4432_RSSI_THRESHOLD, EZMacProReg.RSSILR);//设置门限
//	    SpiReadRegister(SI4432_INTERRUPT_STATUS_1);//清除中断状态
//	    SpiReadRegister(SI4432_INTERRUPT_STATUS_2);
//	
//		CLEAR_MAC_EXT_INTERRUPT();//清除无线中断标识位    
//	    SpiWriteRegister(SI4432_INTERRUPT_ENABLE_1, 0x00);//禁止中断
//	    
//	    SpiWriteRegister(SI4432_INTERRUPT_ENABLE_2, SI4432_ENRSSI);//使能侦听RSSI中断
//	
//	    EZMacPro_PacketType = PACKET_TYPE_2STEP | PACKET_TYPE_DATA;
//	    EZMacProLBT_Retrys = 0;
//	    BusyLBT = 0; //初始化标识位
//	   
//	    MSR = TX_STATE_BIT | TX_STATE_LBT_LISTEN;//设状态机   
//	    timeOutOffset0(TIME_FIRE_ON); //设4 ms超时时间
//		
//		RF_SWITCH_RX();// 切到收模式
//		SpiWriteRegister(SI4432_OPERATING_AND_FUNCTION_CONTROL_2, 0x20); // rx 	
//		SpiWriteRegister(SI4432_OPERATING_AND_FUNCTION_CONTROL_1, SI4432_RXON | SI4432_XTON);//点火
//	
//	    ENABLE_MAC_EXT_INTERRUPT();// 使能RF中断
    return MAC_OK;
}
/*************************************************************************
 * Function Name: EZMacPRO_Transmit_Test
 * Parameters: EZMAC发送测试数据
 * Return: MAC_OK
 *
 * Description: EZMAC发送测试数据
 *		
 *************************************************************************/
uint8 EZMacPRO_Transmit_Test(void)
{      
//	    WDTFeed();//喂狗
//	    //LED_TX2_ON();
//	    LED_BlinkSet(LED_FAR_TX, 1, 5, 2);
//	    DISABLE_MAC_EXT_INTERRUPT();  //禁止响应无线中断
//	
//	
//	    SpiReadRegister(SI4432_INTERRUPT_STATUS_1);//清除中断状态
//	    SpiReadRegister(SI4432_INTERRUPT_STATUS_2);
//	
//		CLEAR_MAC_EXT_INTERRUPT(); //清除无线中断标识位
//		
//		adpkt.CurTime = curtimecycle;//当前总时帧数
//		adpkt.subtype.Type = adv_subtype; // 记录当前附属消息类型
//	
//	
//		netcid=ez_test.netid;//网络号
//		
//		Copy_EZ_TEST_PKT_To_FIFO(adpkt);//要发送的内容
//	
//	      
//	    SpiWriteRegister(SI4432_INTERRUPT_ENABLE_1, 0x00);//禁止中断     
//	    SpiWriteRegister(SI4432_INTERRUPT_ENABLE_2, SI4432_ENRSSI);//使能侦听RSSI中断
//	
//	    EZMacPro_PacketType = PACKET_TYPE_BORADCAST;//
//	    EZMacProLBT_Retrys = 0;
//	    BusyLBT = 0;//初始化标识位
//	    MSR = TX_STATE_BIT | TX_STATE_LBT_LISTEN;//置标识位
//	
//		 timeOutOffset0(TIME_FIRE_ON);
//		 
//	    RF_SWITCH_RX();//切至收模式
//	    SpiWriteRegister(SI4432_OPERATING_AND_FUNCTION_CONTROL_2, 0x20); // rx 	
//	    SpiWriteRegister(SI4432_OPERATING_AND_FUNCTION_CONTROL_1, SI4432_RXON | SI4432_XTON);//点火
//	    ENABLE_MAC_EXT_INTERRUPT();// 使能RF中断
    return MAC_OK;
}


/*************************************************************************
 * Function Name: EZMacPRO_Transmit_Adv
 * Parameters: EZMAC发送广播数据
 * Return: MAC_OK
 *
 * Description: EZMAC发送广播数据
 *		
//	 *************************************************************************/
//	uint8 EZMacPRO_Transmit_Adv(void)
//	{
//	#ifdef DEBUG_PRINTF
//	
//	    //printf("EZMacPRO_Transmit_Adv \n");
//	#endif
//	//	    LED_TX2_ON();//测试
//	//	    DISABLE_MAC_EXT_INTERRUPT();  //禁止响应无线中断
//	//	
//	//	    SpiReadRegister(SI4432_INTERRUPT_STATUS_1);//清除中断状态
//	//	    SpiReadRegister(SI4432_INTERRUPT_STATUS_2);
//	//	
//	//		CLEAR_MAC_EXT_INTERRUPT();  //清除无线中断标识位
//	//	
//	//	    adpkt.CurTime = curtimecycle;//当前时隙
//	//	    adpkt.subtype.Type = adv_subtype; // 记录当前附属消息类型
//	//	    if(topo_deadtime>curslottime)//如果正在查询拓扑
//	//	        adpkt.subtype.NeedTopo = 1;
//	//	    else
//	//	        adpkt.subtype.NeedTopo = 0;
//	//	
//	//		adpkt.subtype.StartBoardMeterData = bBroadStart;
//	//		
//	//		Copy_ADPKT_To_FIFO(adpkt);//向FIFO中填写广播数据
//	//		 
//	//		SpiWriteRegister(SI4432_INTERRUPT_ENABLE_1, 0x00);//禁止中断
//	//		 
//	//		SpiWriteRegister(SI4432_INTERRUPT_ENABLE_2, SI4432_ENRSSI);//使能侦听RSSI中断
//	//	
//	//	    EZMacPro_PacketType = PACKET_TYPE_BORADCAST;
//	//	    EZMacProLBT_Retrys = 0;
//	//	    BusyLBT = 0;//
//	//	    MSR = TX_STATE_BIT | TX_STATE_LBT_LISTEN;//置标识位
//	//	
//	//		timeOutOffset0(TIME_FIRE_ON);
//	//	
//	//		RF_SWITCH_RX();//切到收模式
//	//		SpiWriteRegister(SI4432_OPERATING_AND_FUNCTION_CONTROL_2, 0x20); // rx 	
//	//		SpiWriteRegister(SI4432_OPERATING_AND_FUNCTION_CONTROL_1, SI4432_RXON | SI4432_XTON);//点火
//	//		ENABLE_MAC_EXT_INTERRUPT();// 使能RF中断
//	
//	    return MAC_OK;
//	}

/*************************************************************************
 * Function Name: EZMacPRO_Receive
 * Parameters: EZMAC进入接收状态
 * Return: MAC_OK
 * Description: EZMAC进入接收状态
 *		
 *************************************************************************/


uint8 EZMacPRO_Receive(void)
{
//	    uint8 temp8;
//	
//	    DISABLE_MAC_EXT_INTERRUPT(); // 关闭RF中断      
//	 
//	    SpiWriteRegister(SI4432_RSSI_THRESHOLD, EZMacProReg.RSSILR);//设置门限  
//	    SpiWriteRegister(SI4432_INTERRUPT_ENABLE_1, 0x00);//禁止中断
//	    SpiWriteRegister(SI4432_INTERRUPT_ENABLE_2, SI4432_ENSWDET);      
//	    SpiReadRegister(SI4432_INTERRUPT_STATUS_1);//清除中断状态
//	    SpiReadRegister(SI4432_INTERRUPT_STATUS_2);
//	
//		CLEAR_MAC_EXT_INTERRUPT();//清除无线中断标识位
//	
//	    temp8 = SpiReadRegister(SI4432_OPERATING_AND_FUNCTION_CONTROL_2);//清RX FIFO
//	    temp8 |= SI4432_FFCLRRX;
//	    SpiWriteRegister(SI4432_OPERATING_AND_FUNCTION_CONTROL_2, temp8);
//	    temp8 &= ~SI4432_FFCLRRX;
//	    SpiWriteRegister(SI4432_OPERATING_AND_FUNCTION_CONTROL_2, temp8);
//	        
//		RF_SWITCH_RX();//切到收模式
//		SpiWriteRegister(SI4432_OPERATING_AND_FUNCTION_CONTROL_2, 0x20); // rx 	
//		SpiWriteRegister(SI4432_OPERATING_AND_FUNCTION_CONTROL_1, SI4432_RXON | SI4432_XTON);//点火
//		       
//	    if (SystemStatus > SYSTEM_STATUS_WAIT_FOR_SYNC)
//	    {
//	        if( ls_test.msr!=EZ_TEST_BIT)
//	        	timeOutOffset0(SYNC_RX_FIXED_TIME); // start timer with RSSI timeout
//	    }
//	    MSR = RX_STATE_BIT | RX_STATE_WAIT_FOR_SYNC;
//	    ENABLE_MAC_EXT_INTERRUPT(); // 使能RF中断
    
    Radio->StartRx( );
    return MAC_OK;
}



/*************************************************************************
 * Function Name: GetLocalBehave
 * Parameters: slot : 时隙位置；timeframe ,时帧计数的低8位
 * Return: 当前的行为
 *
 * Description: channel is clear
 *		
 *************************************************************************/
BEHAVIOR Get_Current_Behave(uint32_t slot, uint32_t timeframe) //获取要操作的行为
{
	BEHAVIOR ret;
	//uint16 index;
    uint32 temp16;	
    uint32 temp32;
#ifndef MASTER_NODE    
    if(timeframe < 10) timeframe = 10;
    if ((slot % timeframe) == (localid - 1))    //本地发广播    
    {
        ret.behave = BEHAVIOR_BROADCAST;
        ret.freq = broad_fhc;
    }
    else if ((slot & 1) == 0)  //其他点他广播      
    {              
        ret.behave = BEHAVIOR_RECV_ADV;
        ret.freq = broad_fhc; //广播频率
    }
    else
#else
    extern uint8 guc_AllowLogin;
    if (guc_AllowLogin && (slot % 100) == (localid - 1))    //本地发广播
    {
        ret.behave = BEHAVIOR_BROADALLOWLOGIN;
        ret.freq = broad_fhc;
    }
    else
#endif      
    {      // 业务时隙          
        ret.behave = BEHAVIOR_SHARE_TX;
//	        temp16 = rfpara.rf_slotnum * 2 * timeframe;//跳频计算公式	
//	        temp32 = (temp16 + slot) >> 1;
        //index = (temp32 % channel_para.data_freq_num);
       
        //dex =1;//测试、设成固定频点
//	        ret.freq = MacFrqList[index]; //业务频点表格
#ifdef DEBUG_PRINTF
        //printf("Get_Current_Behave timeframe = %d temp16 = %d temp32 = %d index = %d freq = %d\n", timeframe, temp16, temp32, index, ret.freq);
#endif           
    }
#ifndef MASTER_NODE     
    gn_loginTO ++;
    if(gn_loginTO > 3*CON_LOGIN_TIME_OUT)
    {
        guc_netStat = NODE_STATUS_OUT;
        gn_loginTO = 0;
    }
#endif    
    return ret;
}



//Lora新时序程序
//BEHAVIOR Get_Current_Behave2(uint16 slot, uint8 timeframe) //获取要操作的行为
//{
//	BEHAVIOR ret;
//	//uint16 index;
//    uint8 littleSlot = slot % (LITTLE_SLOT_NUM * 8);
//    uint8 littleSlotNum = slot / (LITTLE_SLOT_NUM * 8);
//	//uint16 temp16;	
//	//uint32 temp32;
//    uint8 level = 0;
//    if (slot == (localid - 1))    //本地发广播    
//    {
//        ret.behave = BEHAVIOR_BROADCAST;
//        ret.freq = broad_fhc;
//        level = getNetworkStatus();
//        if(level > 1)
//        {
//            ret.rebirth = 1; 
//        }
//    }
//    else if (littleSlot < 8)  //竞争时隙    
//    {   
//        if(littleSlot == 0)
//        {
//            level = getNetworkStatus();
//
//            switch(level)
//            {
//            case 1:
//                ret.behave = BEHAVIOR_BROADCAST;  
//                ret.rebirth = 0;
//                break;
//            case 2:
//                ret.behave = BEHAVIOR_BROADCAST;  
//                ret.rebirth = littleSlotNum % 2;
//                break;
//            case 3:
//                ret.behave = BEHAVIOR_SHARE_TX;  
//                ret.rebirth = 0;
//                break;
//            default:
//                ret.behave = BEHAVIOR_SHARE_TX;  
//                ret.rebirth = 0;                
//                break;
//            }
//        }
//        else
//        {
//            ret.behave = BEHAVIOR_SHARE_TX;
//        }
////	        temp16 = rfpara.rf_slotnum * 2 * timeframe;//跳频计算公式	
////	        temp32 = (temp16 + slot) >> 1;
////	        index = (temp32 % channel_para.data_freq_num);
////	       
////	        //dex =1;//测试、设成固定频点
////	        ret.freq = MacFrqList[index]; //业务频点表格    
//    }
//    else
//    {
//        
//        littleSlot = slot % (LITTLE_SLOT_NUM);
//        littleSlotNum = slot / (LITTLE_SLOT_NUM);
//
//        // 业务时隙   
//        if (littleSlot < 8)
//        {
//            ret.behave = BEHAVIOR_SHARE_TX;
//        }
//        else
//        {
//            ret.behave = BEHAVIOR_RECV;
//            ret.freq = broad_fhc; //广播频率
//        }
//    }
//
//    if(g_sucDirectSendFlg == TRUE && ret.behave == BEHAVIOR_SHARE_TX)
//    {
//        ret.behave = BEHAVIOR_RECV;//碰到正在直接发送数据，跳过此次发送
//    }
//    return ret;
//}
unsigned char getMSR(void)
{
    return MSR;
}

void setMSR(unsigned char cmsr)
{
    MSR = cmsr;
}

//====================================================
//void Create_JRep(uint16 k, EZ_TX_PKT * ezPkt)//组入网应答包
//{
//	PKT *pkt;
//	uint8 /*recvid , */bNotSend = 0;
//	//JOINRSP *joinresp;
//	//EXTDATA* pExtData;
//	//uint8 bTypeExtData = 0;
////	uint8 bNeedDelete = 0;
//	//ROUTE temprt;
//    uint8 m = 0;
//    uint16 whiteMeterNum = 0;
//	if(cltor[k].nodestatus.bNeedDelete)
//	{
////		bNeedDelete = 1;
//        cltor_shadow[k].FrameDeadTime = MAX_DATA_TIME_LIVE;
//		//cltor[k].nod.ans_pkt_type = 3;
//	}
//	pkt = &(ezPkt->TxPkt);
//	pkt->ttl = MAX_DATA_TIME_LIVE;	//生存周期by ben 29
//    pkt->index = k;
//    pkt->protocol = cltor[k].nodestatus.protocol;
//    
//    if(cltor[k].nodestatus.protocol == PST_FRM_WL_1_NO)
//	{
//        //LOG_DEBUG( DBGFMT"Create_JRep[%d] FrameDeadTime = %d\n",DBGARG, k, cltor[k].FrameDeadTime);
//
//        memset((uint8 *)&pkt->head.apdu, 0, sizeof(STAPDU));
//        
//		pkt->head.apdu.ctrl.dir = 0;
//        pkt->head.apdu.ctrl.prm = 0;
//        pkt->head.apdu.ctrl.evtmode = 1;
//        pkt->head.apdu.ctrl.ftd = 7;
//        pkt->head.apdu.seq = cltor_shadow[k].recvseq;
//        pkt->head.apdu.srssi = cltor_shadow[k].rRssi;
//        if(cltor_shadow[k].nodestatus.ans_pkt_type == 0x01)
//        {
//            pkt->head.apdu.fn = 3;//登陆
//            if(cltor_shadow[k].nodestatus.result == 0x07)
//            {
//                pkt->head.apdu.fn = 7;
//                cltor_shadow[k].nodestatus.result = 0;
//            }
//            else if(cltor_shadow[k].nodestatus.result == 0x06)
//            {
//                pkt->head.apdu.fn = 7;
//                cltor_shadow[k].nodestatus.result = 2;
//            }
//                
//        }
//        else if(cltor_shadow[k].nodestatus.ans_pkt_type == 0x00)
//        {
//            pkt->head.apdu.fn = 5;//心跳
//        }
//        else if(cltor_shadow[k].nodestatus.ans_pkt_type == 0x03)
//        {
//            pkt->head.apdu.fn = 6;//剔除
//            
//            pkt->head.apdu.ctrl.prm = 1;
//            cltor_shadow[k].sendseq++;
//            pkt->head.apdu.seq = cltor_shadow[k].sendseq;
//        }
//        else
//        {
//            pkt->head.apdu.fn = 4;//登出
//        }
//            
//        pkt->head.apdu.stInfo.stDown.bit1.routeFlg = 0;
//        pkt->head.apdu.stInfo.stDown.bit1.nodeFlg = 0;
//        pkt->head.apdu.stInfo.stDown.bit1.addrFlg = 0;
//        pkt->head.apdu.stInfo.stDown.bit1.conflict = 1;
//        pkt->head.apdu.stInfo.stDown.bit1.routeNum = cltor[k].hop - 2;;//pkt->rarea->stDown.bit1.routeNum;
//        pkt->head.apdu.stInfo.stDown.bit2.channelFlg = rfpara.rf_channel;//Cal_Hash_Value(nDeviceMacAddr) % LORA_CHANNEL_NUM;//pkt->rarea->stDown.bit2.channelFlg;
//        pkt->head.apdu.stInfo.stDown.bit2.errEncodeFlg = 0;
//        pkt->head.apdu.addrlen = 0;
//        pkt->head.apdu.addr = pkt->data + PKT_HEAD_LEN;
//
//        memcpy(pkt->head.apdu.addr, nDeviceMacAddr, UNIQUE_MAC_ADDR_LEN);//节点唯一标识(队列)	
//		ROUTE temprt = Fill_Route(pkt->head.apdu.addr + 6,  MASTER_ROUTE_PATH, k); //接收者地址、路由
//		if(temprt.rinfo.rtype != ERROR_ROUTE_PATH)
//		{
//			//pExtData->route.rdepth = temprt.rinfo.rdepth;
//			pkt->head.apdu.stInfo.stDown.bit1.routeNum = temprt.rinfo.rdepth;
//			//recvid = temprt.did;
//			//pkt->data[7] = 0;
//			
//            memcpy(pkt->head.apdu.addr + (temprt.rinfo.rdepth + 1) * 6, cltor[k].devAddr, UNIQUE_MAC_ADDR_LEN);//节点唯一标识(队列)
//
//            pkt->head.apdu.addrlen = (temprt.rinfo.rdepth + 2) * 6;
//
//            pkt->head.apdu.data = pkt->data + PKT_HEAD_LEN + pkt->head.apdu.addrlen;
//            
//
//            switch(pkt->head.apdu.fn)
//            {
//            case 3:
//            case 7:
//                pkt->head.apdu.data[m++] = cltor_shadow[k].nodestatus.result;
//                pkt->head.apdu.data[m++] = 0x00;
//                pkt->head.apdu.data[m++] = 0x00;
//                whiteMeterNum = Record_Num();
//                memcpy(pkt->head.apdu.data + m, (uint8 *)&whiteMeterNum, 2);//网络规模
//                m+=2;
//                
//                memcpy(pkt->head.apdu.data + m, (uint8 *)&k, 2);//短地址
//                m+=2;
//
//                memset(pkt->head.apdu.data + m, 0, 12);//时间
//                m+=12;
//                pkt->head.apdu.data[m++] = 0x00;
//                pkt->head.apdu.data[m++] = 0x00;
//                break;
//            case 4:
//            case 5:
//                pkt->head.apdu.data[m++] = cltor_shadow[k].nodestatus.result;
//                break;
//            case 6:
//                break;
//            default:    
//                break;
//            }
//            pkt->head.apdu.len = m;
//		}
//		else
//        {      
//			bNotSend = 1;
//        }
//	}
//    else if(cltor[k].nodestatus.protocol == PST_FRM_802_R_NO)
//    {
//        uint8 m = 0;
//        memset((uint8 *)&pkt->head.mhr, 0, sizeof(STIEEE));
//
////	        LOG_DEBUG( DBGFMT"Create_JRep ID[%x] rssi[%d] level[%d]\n",DBGARG, k, cltor_shadow[k].rRssi,
////	                getRssiQualityByRssi(cltor_shadow[k].rRssi));                        
//
//        
//        pkt->head.mhr.seg_ctrl.ctrl_net.bit3.OC = 0;
//        pkt->head.mhr.seg_ctrl.ctrl_net.bit2.signalQ = getRssiQualityByRssi(cltor_shadow[k].rRssi);
//        pkt->head.mhr.seg_ctrl.ctrl_net.bit2.pwr = rfpara.rf_power;
//        
//        pkt->head.mhr.frame_ctrl.bit2.prm = 0;
//        pkt->head.mhr.frame_ctrl.bit1.ftd = 3;
//        pkt->head.mhr.frame_ctrl.bit1.panid = 1;
//        pkt->head.mhr.frame_ctrl.bit1.dir = 0;
//        
//        pkt->head.mhr.seq = cltor_shadow[k].recvseq;
//        pkt->head.mhr.srssi = cltor_shadow[k].rRssi;
//    
//        pkt->head.mhr.app = pkt->data;
//
//        
//        if(cltor_shadow[k].nodestatus.ans_pkt_type == CON_NODE_UPDATE_LOGIN 
//            || cltor_shadow[k].nodestatus.ans_pkt_type == CON_NODE_UPDATE_REGISTER)
//        {
//            if(cltor_shadow[k].nodestatus.ans_pkt_type == CON_NODE_UPDATE_LOGIN)
//            {
//                pkt->head.mhr.fn = 0x02;//入网应答
//            }
//            else if(cltor_shadow[k].nodestatus.ans_pkt_type == CON_NODE_UPDATE_REGISTER)
//            {
//                pkt->head.mhr.fn = 0x0E;//注册应答
//            }
//            
//            if(cltor[k].addrLen == CON_DEV_ADDR_LEN_8)
//            {
//                pkt->head.mhr.frame_ctrl.bit2.dest = 3;
//            }
//            else
//            {
//                pkt->head.mhr.frame_ctrl.bit2.dest = 1;
//            }
//            
//            if(nDeviceMacAddrLen == CON_DEV_ADDR_LEN_8)
//            {
//                pkt->head.mhr.frame_ctrl.bit2.src = 3;
//            }
//            else
//            {
//                pkt->head.mhr.frame_ctrl.bit2.src = 1;
//            }
//            pkt->head.mhr.app[m++] = pkt->head.mhr.fn;
//            memcpy(pkt->head.mhr.app + m, (uint8 *)&rfpara.shortID, 2);
//            m+=2;
//            memcpy(pkt->head.mhr.app + m, (uint8 *)&k, 2);
//            m+=2;
//            pkt->head.mhr.app[m++] = 0x18 | cltor_shadow[k].nodestatus.result;//信标允许，设备类型ffd
//
//            
//        }
//        else if(cltor_shadow[k].nodestatus.ans_pkt_type == CON_NODE_UPDATE_HEARTBEAT)
//        {
//            pkt->head.mhr.fn = 0x0F;//心跳 
//            pkt->head.mhr.frame_ctrl.bit2.dest = 2;
//            pkt->head.mhr.frame_ctrl.bit2.src = 2;
//            pkt->head.mhr.app[m++] = pkt->head.mhr.fn;
//            pkt->head.mhr.app[m++] = 0x18 | cltor_shadow[k].nodestatus.result;//信标允许，设备类型ffd
//
//            //pkt->head.apdu.fn = 5;//心跳
//        }
//        else if(cltor_shadow[k].nodestatus.ans_pkt_type == 0x03)
//        {
//            pkt->head.mhr.fn = 0x03;//离网 
//            pkt->head.mhr.frame_ctrl.bit2.dest = 2;
//            pkt->head.mhr.frame_ctrl.bit2.src = 2;
//            pkt->head.mhr.app[m++] = pkt->head.mhr.fn;
//            pkt->head.mhr.app[m++] = 0x01;//协调器希望设备断开PAN
//
////	            pkt->head.apdu.fn = 6;//剔除
////	            
////	            pkt->head.apdu.ctrl.prm = 1;
////	            cltor_shadow[k].sendseq++;
////	            pkt->head.apdu.seq = cltor_shadow[k].sendseq;
//        }
//        else
//        {
////	            pkt->head.apdu.fn = 4;//登出
//        }  
//        pkt->head.mhr.len = m;
//        
//        uint8 err = IE_Compose_RD(k, ezPkt, &pkt->head.mhr.seg_ctrl, &pkt->head.mhr.frame_ctrl, 
//                               pkt->head.mhr.seq, pkt->head.mhr.app, pkt->head.mhr.len);
//    }
//	else
//    {      
//		bNotSend = 1;
//    }
//
//		//*********************************************************物理层
//	if(0 == bNotSend)
//	{
//		//cltor[k].sendseq++;
//	
//		ezPkt->bValid = 1;//业务缓存有效
//		ezPkt->nBackOffSlot = 0;
//        
////	        LOG_DEBUG( DBGFMT"[%d] wireless send net data ftd = %d, fn = %d seq = %d dest addr %02x%02x%02x%02x%02x%02x\n",DBGARG, 
////	            k, pkt->apdu.ctrl.ftd, pkt->apdu.fn, pkt->apdu.seq, pkt->apdu.addr[11], pkt->apdu.addr[10],pkt->apdu.addr[9],pkt->apdu.addr[8],pkt->apdu.addr[7],pkt->apdu.addr[6]);
//        //LOG_DUMPHEX(LOG_LEVEL_DEBUG, "HexData : \n", pkt->data, pkt->len);
//
//        
//	}
//	else
//	{
//		//路由错误时的处理
//		cltor_shadow[k].nodestatus.bNetAck = 0;
//	}
//}



