/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2003
 *
 *    File name   : user_func.c
 *    Description : User functions module
 *
 *    History :
 *    1. Date        : Feb 12, 2016
 *       Author      : yzy
 *       Description : Create
 *
 **************************************************************************/
#define USER_VAR
#define UPGRADE_VAR
#include "sys.h"
#include "public.h"
#include "App_Public.h"
#include "user_func.h"
#include "protocol.h"
#include "user_func.h"
#include "A7139reg.h"
#include "LinkMng.h"
volatile uint32 TickCounter = 0;
volatile uint32 SecCounter = 0;


USER_VAR volatile int TickSysFlag = 0;

uint32 RsResetTime = RS_RESET_TIME;

EZ_Cur_Tx_Type EzCurTxType; 
USER_VAR COLLECTOR cltor[MAX_SUP_SS_NUM + 1]@"AHB_RAM_MEMORY";//子节点信息数组
STAREPORT StaReport;
EZMACPROREG EZMacProReg;
RF_FREQ rf_define;//设置频点定义
//	USER_VAR RF_PARAMETER rfpara;//设置无线通信参数
CHANNEL_PARA channel_para;//信道参数
EZ_TEST ez_test;//发射功率测试
LISN_TEST ls_test;//灵敏度测试
USER_VAR HASH_INFO shadow_space[MAX_HASH_TABLE_FLASH_SIZE]@"AHB_RAM_MEMORY";//影子空间


USER_VAR uint32 updata_code_cache[128];//256byte缓存
uint8 NetAvalidFreq[8]; //use to record the avalible channel in total network
uint32 StaTxtime; // 统计包的发送时间
uint32 StaRxtime; // 接收到节点反馈的统计包的接收 时间
uint32 NodeUpdateTime = 80000; // avoid the first time clean
uint32 CurScanFreq = 0x0; // 
USER_VAR uint32 Uppktacked = 0;
uint32 curtimecycle = 0; // 当前总时帧

uint32 nCurTimeMin = 0;
uint32 nLastCurTimeMin = 0;

USER_VAR uint32 curslottime = 0; //
USER_VAR uint32 sysSlotTime = 0; //目前的总时隙个数


uint16 pkt_seq; //包顺序号
uint16 updata_pkt_no = 0; //用于判断升级程序包的奇偶性

uint8 ChannelIsFree = CHANNEL_CLEAR_STATE;
USER_VAR uint16 localid = 1; // 本地ID
uint8 netcid ;
uint8 localhop = 1;
uint8 updata_pkt = 0; //FLASH块数
uint8 netslotnum; //net's the number of slots of one cycle
USER_VAR uint8 IapAction = IAP_ACTION_IDLE;
uint8 ccl_addr[6]; // collector address
uint8 AvalidChanNum = 0;
//	uint8 MacAvalidChan[MAX_AVAILBLE_FRQ];
//	uint8 MacFrqList[MAX_AVAILBLE_FRQ];
uint8 BackOffSlot = 0;//避退时间
USER_VAR uint8 Write_Sucess = 1;//写FLASH数据正确标志位
uint8 spierr = 0;
uint8 EZMacPro_PacketType = PACKET_TYPE_BORADCAST;

uint8 BusyLBT = 0;
//	uint8 SystemStatus = 0;
uint8 rf_clk;//射频校准中断标识位


USER_VAR STMUCHFRAMEINDEX g_stMuchframeindex_port@"AHB_RAM_MEMORY";
USER_VAR uint8 g_ucfactory_mod = 0;
extern HASHT * htable1; //哈希表(2K)
extern HASHT * htable2; //哈希表(2K)
//	USER_VAR uint8 	CurWhiteSSMap;		// 0: 白名单1;     1: 白名单2
//	USER_VAR uint8 	CurBlackSSMap;		// 0: 黑名单1;     1: 黑名单2  
//uint8 	SSMapShadowSpace[MAX_VALIDATE_SS_NUM];

/****广播包相关定义**************************************************/
uint32 adv_updatetime = 0;
uint8 adv_sublen = ADV_SUB_LEN;
uint8 adv_subtype = 0;

//adv_pkt:普通广播包的字节定义: 
//0 功率等级 1:底座版本号 2: I型采集器版本 3: I型采集器版本块数
//	uint8 adv_pkt[16];

//上一次底座版本
uint8 nBaseSSVersion = 0;

//广播透明转发的内容
uint8 adv_Trans_pkt[128];

uint8 subtype_flag = 0;//规约类型
uint8 EzMacSubTypeFlag = 0;

//	USER_VAR CLTP cltparm; // 采集器相关参数


USER_VAR uint8 StaAck = 0; // 统计包的回应
uint8 EZMacStaAck = 0; // 统计包的回应

uint8 EZMacProRandomNumber = 0;
USER_VAR uint32 HashUpdateSectorAdd = FLASH_MAP_ADD;
uint8 ReqMeterID = 0; //上层请求的ID 号
uint8 EzMacReqMeterID = 0; //上层请求的ID 号

uint8 Change_MR0=0 ;//是否操纵FLASH标识位
uint8  broad_fhc,data_fhc;//补偿电容的值 ;//广播实际偏移、业务实际偏移
uint32 timer0_cnt;
uint8 mainflag=0;//校准出中断标识位

uint8 bBroadStart;
uint8 bBroadStartTriger;
uint8 bHaveJCDataFlag = 0;
uint8 nJCDataFlag[4];
uint32 u32BroadStartTrigerTime;

USER_VAR uint8 bMeterSearchStart = 0;
USER_VAR uint8 bMeterDeadStart = 0;
USER_VAR uint8 bCltorSearchStart = 0;

uint8 bWaitForSendMeterAck;
uint8 bSendMeterSearchSeq;
USER_VAR METER_STATUS MeterStatus[MAX_HASH_TABLE_FLASH_SIZE]@"AHB_RAM_MEMORY";

USER_VAR uint8 bNeedDeleteMeter = 0;
USER_VAR uint8 bNeedMeterDeleteAck = 0;

//	uint8 MeterSourceID[MAX_HASH_TABLE_FLASH_SIZE];
USER_VAR uint8 MeterDeadCount;
uint8 ReSendCount = 0;

uint8 bSendNeedWait = 0;
uint32 u32TxSlot = 0;

uint8 bNeedAckMeter;
uint8 nMeterAckType = 0;
uint8 NeedAckMeterAddr[6];
uint8 NeedAckSourceId;

//遍历采集器时，保存的上一次的采集器老化时间间隔
uint8 bSendTickToSS = 0;
//	uint32 u32SendTickToSSDeadTime = 0;
uint32 u32LastFoundSlot = 0;

//对于单个采集器进行查表的标志
uint8 bSingleSearchSSSendMeter = 0;
uint8 bSingleSearchSSSendMeterWaitAck = 0;
uint32 u32TimeOfSingleSearchSSSendMeter = 0;
uint8 bSingleSearchSSMeterSended = 0;

uint8 bEzMacSingleSearchSSFlag = 0;
uint8 nEzMacSourceId = 0;

//拓扑显示的显示内容来源
uint8 u8TuopuDisplaySource = 0;

//重启广播命令标志
USER_VAR uint8 bSendResetCmd;

//采集器认证使能参数
USER_VAR uint8 bSSValidateEnable;

//采集器升级请求回复处理过程中的记录的上一次发送的SS节点TTL
uint32 LastUpdataPktDeadTime = 0;

//采集器升级请求回复处理过程中的记录的上一次发送的SS节点TTL
uint32 LastSSBaseUpdataPktDeadTime = 0;

//灵敏度接收完成后标志
uint8 EzTestRevDone;

//测量点同步使能参数
uint8 bMeterInfoSynEnable = 0;

//频段号         0:433-470    1: 470-480   2:500-510  3:491-500
uint8 nFreqChannel = SX127X_FREQ;


uint8 u8NeedSetSSID;

//升级标记下次需要的包序号
//uint16 u16NextNeedID = 0;


//广播复位时帧内容
USER_VAR uint8 bNeedChangeChannel = 0;
USER_VAR uint8 nFreqChannelNum = 0;
USER_VAR uint32 nChangeChannelTime = 0;
uint8 nDefaultJumpChannel = 0;
uint8 bNeedRelayBroadCast = 0;
uint8 nChannelChangeNum = 0;

uint8 nBroadSendData = 0;

//	USER_VAR uint8 nDeviceMacAddr[6];

//回复调试报文标志
uint8 bDebugSend = 0;

//无线调试报文的标志
uint8 bEZMACDebugType = 0;
DEBUG_UPPKT* pEZMACRecvPacket = NULL;

//信道配置模式
uint8 bChannelSetManualMode = 0;	//0为自动模式    1为手动模式

//无线调试报文的缓冲区

uint8 nDebugPacketRear = 0;
uint8 nDebugPacketFront = 0;

//无线调试报文处理的状态机
uint8 EZMAC_Debug_Status = EZMAC_DEBUG_STATUS_IDLE;

//无线调试收到回复标志
uint8 bEZMacRecFlag = 0;


//集中器抄表允许标志	0: 允许  1:禁止
USER_VAR uint8 bJcDisallowFalg = 0;
uint32 EzMacDeadCont = EZMAC_DEAD_CNT;

//从节点主动注册标志
USER_VAR uint8 bBroadMeterEnable = 0;
//从节点主动注册持续时间
USER_VAR uint16 nBroadMeterTime = 0;
//需要开启认证使能参数
uint8 bNeedOpenSSValidateEnable = 0;

#ifdef _Debug
uint8 Test[100];
#endif


uint8 SendNum = 0;
uint32 MeterDataSendSlot = 0;
uint8 bMesterDataWaitAck = FALSE;
uint16 MeterID = 0;

uint16  gab=0;//频点偏移值


USER_VAR const uint8 sBroadAddr99[8] = {0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99};
USER_VAR const uint8 sBroadAddrAA[8] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
USER_VAR const uint8 sBroadAddrFC[8] = {0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
USER_VAR const uint8 sBroadAddrFD[8] = {0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
USER_VAR const uint8 sBroadAddrFE[8] = {0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
USER_VAR const uint8 sBroadAddrFF[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

USER_VAR ST_PENDING_ADDR_LIST g_stPendingAddrList;
USER_VAR uint32 g_nModuleResetTime = 0;

extern uint8 informpkt[];
extern uint8 informpktForDebug[];
extern uint8 negativepktForDebog[];
//extern STMUCHFRAMEINDEX g_stMuchframeindex_port;
#if SEND_FREQ_CTRL
uint8 g_ucSendCount[MAX_AVAILBLE_FRQ];
#endif
USER_VAR uint8 g_bResetCmd;
//-----------------------------------------------------------------------------
#define POLY 0x1021
uint16 UpdateCRC (uint16 CRC_acc, uint8 CRC_input)
{
	uint8  i; 
	CRC_acc = CRC_acc ^ (CRC_input << 8);
	for (i = 0; i < 8; i++)
	{
		if ((CRC_acc & 0x8000) == 0x8000)
		{
			CRC_acc = CRC_acc << 1;
			CRC_acc ^= POLY;
		}
		else
		{
			CRC_acc = CRC_acc << 1;
		}
	}
	return CRC_acc;
}
//--------------------------------------------------------------------------------
uint16 CRC_16BIT_Check1(uint8 *pentry,uint32 len)//FLASH数据校验
{
	uint16 CRC_acc=0xffff;
	uint32 temp;
	if(len==0xffffffff)
	    return CRC_acc=0;
	for(temp=0;temp<len;temp++)
		CRC_acc=UpdateCRC (CRC_acc, pentry [temp]);
	return CRC_acc;
}

//--------------------------------------------------------------------------------
//uint16 CRC_16BIT_Check2(uint16 CRC, uint8 *pentry,uint32 len)//FLASH数据校验
uint16 CRC_16BIT_Check2(uint16 CRC_acc, uint8 *pentry,uint32 len)
{
//	uint16 CRC_acc=CRC;
	uint32 temp;
	if(len==0xffffffff)
	    return CRC_acc=0;
	for(temp=0;temp<len;temp++)
		CRC_acc=UpdateCRC (CRC_acc, pentry [temp]);
	return CRC_acc;
}

#if (PROG_FLASH_512) 

//--------------------------------------------------------------------------------
uint16 CRC_16BIT_Check(uint16 len)//获取采集器升级代码校验值
{
	uint16 CRC_acc=0xffff;
	uint16 temp;
	for(temp=0;temp<len;temp++)
		CRC_acc=UpdateCRC (CRC_acc, UpdateSource[32+temp]);
	return CRC_acc;
}


//--------------------------------------------------------------------------------
uint8 FlashCRCCheck(uint8 blocknum)//采集器升级代码校验
{
	uint16 temp16;
	uint16 code_len;
	uint16 code_crc;
	
//	temp16=adpkt.SoftBlockNum*200;
	temp16=blocknum*200;
	code_len=(UpdateSource[32+temp16-4]<<8)+UpdateSource[32+temp16-3];
	code_crc =(UpdateSource[32+temp16-2]<<8)+UpdateSource[32+temp16-1];
	temp16=CRC_16BIT_Check(code_len);
	if(temp16==code_crc) 	return 1;
	else 			return 0;
}
#endif
/*************************************************************************
 * MAC层状态轮转及底层函数
 *************************************************************************/
uint8 Do_Iap_EraseSector(uint8 sec1, uint8 sec2)//扇区擦除
{
//    uint32 tcnt1,tcnt2;
//
//	tcnt1 = T1TC; //记录IAP起动时间
//	SelSector(sec1, sec2); // 选择扇区
//	EraseSector(sec1, sec2); // 擦除扇区
//	BlankCHK(sec1, sec2); // 查空扇区
//	tcnt2 = T1TC; //记录IAP终止时间
//	if(tcnt2>tcnt1)
//		tcnt2 -= tcnt1;
//	else
//	{
//		tcnt2+=(0-tcnt1);
//	}
//
//	tcnt2+= SYS_TIMER_COUNT_PRE_TICKS;		//保护时间
	return 0;//(tcnt2/SYS_TIMER_COUNT_PRE_TICKS);
}

const uint8 Crystal_Value_Table[52]=
{
	0x8f,
	0x8f,
	0x8f,
	0x8f,
	0x8f,
	0x8f,
	0x8c,
	0x87,
	0x83,
	0x00,

	0x02,
	0x04,
	0x05,
	0x06,
	0x06,
	0x05,
	0x04,
	0x02,
	0x01,
	0x00,

	0x81,
	0x82,
	0x84,
	0x85,
	0x86,
	0x86,
	0x85,
	0x84,
	0x83,
	0x82,

	0x81,
	0x01,
	0x03,
	0x05,
	0x0a,
	0x0a,
	0x0a,
	0x0a,
	0x0a,
	0x0a,

	0x0a,
	0x0a,
	0x0a,
	0x0a,
	0x0a,
	0x0a,
	0x0a,
	0x0a,
	0x0a,
	0x0a,

	0x0a,
	0x0a
};	//5度一档
	
void Set_Crystal_Value(uint8 tmp_adc)
{
	int16 temp16;
	uint8 temp8; 

	temp16 = EZMacProReg.CrystalValue;
	temp8=Crystal_Value_Table[tmp_adc/5];
	
	if(temp8 & 0x80) 
	{
		temp16 -=(temp8&0x7f);
		if(temp16 < 0) 
			temp16=0;
	}
	else
	{
		temp16 += temp8;
		if(temp16 > 0xff) 
			temp16 = 0xff;
	}	
	
	if(temp16 != EZMacProReg.Cur_Crystal_Value)
	{
		EZMacProReg.Cur_Crystal_Value = (temp16 & 0xff);
//		SpiWriteRegister(SI4432_CRYSTAL_OSCILLATOR_LOAD_CAPACITANCE, EZMacProReg.Cur_Crystal_Value);
	}
}


/*************************************************************************
 * Function Name: DisplayNeigh
 * Parameters:  none 
 * Return: none
 * Description: display the rssi of neighbor
 *
 *************************************************************************/

/*
        void Display_Neighbor_RSSI( uint32 i)//显示网络拓扑
        {
               
                uint8 dis[31], end[2],neigh[16]={0},temp[24]={0},j,h;           
            
                        if ((cltor[i].hop > 1) && (cltor[i].nod.NetStatus == NODE_STATUS_LOGIN))//如果跳数大于1并入网
                        {
				// 在线的节点                               
				dis[0] = START_FLAG;
				dis[1] = 55; // 设定拓扑包的长度  
				dis[2] = 0x00;

				dis[3] = 0x41; //控制域 ;// 节点号
				dis[4] = 0x00; //[6字节信息域]
				dis[5] = 0x00;

				dis[6] = 0x50;
				dis[7] = 0x00;
				dis[8] = 0x00;
				dis[9] = 0x00;

				dis[10] = 0xF0; //AFN
				dis[11] = 0x01; //数据单元标识[2字节]
				dis[12] = 0x00;

				dis[13] = cltor[i].devAddr[5];//SN
				dis[14] = cltor[i].devAddr[4];
				dis[15] = cltor[i].devAddr[3];
				dis[16] = cltor[i].devAddr[2];
				dis[17] = cltor[i].devAddr[1];
				dis[18] = cltor[i].devAddr[0];
				
				dis[19] = i; // 节点号(16位)
				dis[20] = 0x0; // 节点号
				
				dis[21] = cltor[i].softver; //软件版本号
				
				dis[22] = cltor[i].father; // 主路由
				dis[23] =0x0;// 主路由节点(16位)	
				dis[24] = cltor[i].frssi; // 主路由的能量值
				
				dis[25] = cltor[i].mother; // 从路由
				dis[26] =0x0;// 从路由节点(16位)				
				dis[27] = cltor[i].mrssi; // 从路由的能量值

				dis[28] = 8; //校验1
				dis[29] = Check_SUM(dis + 3, 26); //校验1
				
				I2cRead(0xA0, neigh, FM_SS_INFO + i*22,16);     //从铁电中 提取16个字节邻居表内容，在这里要有一个16字节数组	
				 
				for(h=0;h<8;h++)
				{				
				j=h*3;
				memcpy(&temp[j],&neigh[h*2],1);	
				memcpy(&temp[j+2],&neigh[h*2+1],1);	
				}
				
				dis[30] = Check_SUM( temp, cltor[i].neighnum *3); //校验2					  

				end[0] = dis[29] + dis[30]; //校验总和
				end[1] = END_FLAG;
				UART_PutArray(UART0, dis, 29);
				UART_PutArray(UART0, temp, cltor[i].neighnum *3);//邻居号及能量值(24)	                       
				UART_PutArray(UART0, end, 2);
                        }
  
               else//如果节点离网
               	{
				dis[0] = START_FLAG;
				dis[1] = 55; // 设定拓扑包的长度  
				dis[2] = 0x00;

				dis[3] = 0x41; //控制域 ;// 节点号
				dis[4] = 0x0; //[6字节信息域]
				dis[5] = 0x0;
				dis[6] = 0x50;
				dis[7] = 0x0;
				dis[8] = 0x0;
				dis[9] = 0x0;

				dis[10] = 0xF0; //AFN
				dis[11] = 0x01; //数据单元标识[2字节]
				dis[12] = 0x0;
			

				memcpy(&dis[13],cltor[i].devAddr,6);//	
				dis[19] = i; // 节点号(16位低位)	
				dis[20] = 0x0; // 节点号			
				dis[21] =0x0; //软件版本号
				dis[22] =0x0;// 主路由节点(16位)
				dis[23] =0x0;// 主路由
				dis[24] =0x0; // 主路由的能量值
				dis[25] =0x0; // 从路由节点(16位)
				dis[26] =0x0; // 从路由
				dis[27] =0x0; // 从路由的能量值

				dis[28] = 8; //校验1
				dis[29] = Check_SUM(dis + 3, 26); //校验1
				dis[30] = 0x00; //校验2

				end[0] = dis[29] + dis[30]; //校验总和
				end[1] = END_FLAG;
				UART_PutArray(UART0, dis, 29);

				for(i=0;i<24;i++)
				{
				UART_PutCharByPolling(UART0, 0x00);	//发送24个0x00
				}
				UART_PutArray(UART0, end, 2);
			   }
        }
*/
/*************************************************************************
 * Function Name: DisplayTopo
 * Parameters:  none 
 * Return: none
 * Description: display the topo of network
 *
 *************************************************************************/

void Display_Topo()
{
    uint32 i;
    TOPOINFO tmp;

// 取最大有效SS存储空间

    for (i = 0; i <= rfpara.rf_slotnum; i++)
    {
        if (cltor[i].hop > 1)
        {
            tmp.nodeid = i;
            tmp.hop = cltor[i].hop;
            tmp.father = cltor[i].father;
            tmp.mother = cltor[i].mother;

//            UART_PutArray(UART0, (uint8*) &tmp, 4);
        }
    }
}

/********************************************************************************************************
 ** 函数名称：DelayNS()
 ** 函数功能：长软件延时
 ** 入口参数：dly		延时参数，值越大，延时越久
 ** 出口参数：无
 ********************************************************************************************************/
void DelayNS(uint32_t dly)
{
        uint32 i;

        for (; dly > 0; dly--)
                for (i = 0; i < 50000; i++)
                        ;
}

/*************************************************************************
 * Function Name: FreqTrans
 * Parameters:  none 
 * Return: none
 * Description: switch the si4430 to receive mode , enable power amplifer and receive interrupts
 *
 *************************************************************************/
void Freq_Trans(uint8 methord, uint8 *frqlist, uint8 *MacFrqlist, uint8 frqnum)
{
    uint8 i, index;
#ifdef DEBUG_PRINTF
    //printf("************Freq_Trans methord = %d frqnum = %d ***********\n", methord, frqnum);
#endif           
    for (i = 0; i < frqnum; i++)
    {
        index = (i * methord) % frqnum;
        MacFrqlist[i] = frqlist[index];
#ifdef DEBUG_PRINTF
        //printf("i *methord = %d frqlist[%2d] = %4d \t MacFrqlist[%2d] = %4d \n", (i * methord), index, frqlist[index], i, MacFrqlist[i]);
#endif           
    }
    return ;
}

void Check_Freq(uint8 methord, uint8 *MacFrqlist, uint8 frqnum)
{
    uint8 i, j,frq,frq2;
    for (i = 0; i < frqnum; i++)
    {
        frq = MacFrqlist[i];
        for (j = 0; j < frqnum; j++)
        {
            frq2 = MacFrqlist[j];
            if(i != j)
            {
                if(frq2 == frq)
                {
#ifdef DEBUG_PRINTF
                    //printf("XXXXXXXXXXXXXXXXXX calculate err methord = %d MacFrqlist[%d] == MacFrqlist[%d] = %d\n", methord, i, j, frq);
#endif  
                    break;
                }
            }
        }
    }
}
//====================================================
void Timer_INT_TimeOut(uint32 longTime)
{

//        T0TCR = 0x00; // stop timer if already running
//        T0TC = 0; // clear start time                   
//
//
//        T0MR0 = longTime;
//
//        T0TCR = 0x01; // run timer

}

/*************************************************************************
 * Function Name: CheckPkt
 * Parameters:  buf and length of pkt 
 * Return: 1 for ok , 0 for error
 * Description: check the uplink pkt is avilable
 *
 *************************************************************************/
uint8 Check_PKT(uint8 *buf, int limit, UPPKT *pkt)
{
    uint16 len;
    uint8 recvcs;
    uint8 afnoffset, AddrOffSet;
    if (*(buf) != START_FLAG)
        return 0;
    len = *(uint16*)(buf + OFFSET_UP_LEN);
    if (len > limit)
        return 0;
    if (*(buf + len - 1) != END_FLAG)
        return 0;
    recvcs = *(buf + len - 2);
    if (Check_SUM(buf + 3, len - 5) != recvcs)
        return 0;

    pkt->ctrl = *(buf + OFFSET_UP_CTRL);
    pkt->rarea = (STINFO3762*)(buf + OFFSET_UP_RAREA); //信息域
    if (pkt->rarea->stDown.bit1.moduleFlg)//对电表操作
    {
            
        afnoffset = OFFSET_UP_AFN_BASIC + 6 *(pkt->rarea->stDown.bit1.routeNum + 2); 
        AddrOffSet = OFFSET_UP_AFN_BASIC;
    }
    else//对模块操作
    {
        afnoffset = OFFSET_UP_AFN_BASIC;
        AddrOffSet = 0;
    }
    if (afnoffset > len)
        return 0;
    pkt->afnoffset = afnoffset;

    pkt->AddrOffSet = AddrOffSet;
    pkt->afn = *(buf + afnoffset); //AFN      
    pkt->dataflag = *(buf + afnoffset + 1)+(*(buf + afnoffset + 2)<<8);//数据单元标识***

    pkt->len = len;


    return len;
}

/*************************************************************************
 * Function Name: CheckSum
 * Parameters:  buf and length of pkt 
 * Return:  checksum value
 * Description: calculate the cs
 *
 *************************************************************************/
uint8 Check_SUM(uint8 *data, int len)
{
        unsigned char cksum = 0;
        int i;

        for (i = 0; i < len; i++)
        {
                cksum += data[i];
        }

        return (cksum);
}

/*************************************************************************
 * Function Name: HandleRxDoPkt
 * Parameters:  downlink packet
 * Return:  none
 * Description: handle the downlink packet
 *
 *************************************************************************/

uint8 Check_Meter_Type(uint8 *buf)
{
	uint8 temp8;
	if((buf[8]==0x03)||(buf[8]==0x07))
		return 2;
	temp8=buf[8]&0x1f;							//检查功能码
	if((temp8==0x00)||(temp8==0x10)||(temp8==0x08))	//保留+广播校时+最大需量清零	状态不能确定
		return 2;									//不确定
	temp8=buf[8]&0x10;
	if(temp8==0)
		return 1;									//dl-97
	else
		return 2;									//dl-07
}



void Handle_I_SS_Update_Request(PKT *pkt)
{
#if 0 
    //	uint8 recvid;
    uint8 id;
    uint16 blocknum;
    uint8 softver;
//	DATAPKT* spkt = (DATAPKT *)pkt->data;
    
//	id = pkt->data[DATAPKT_HEAD_LEN]; // 源节点号
//	softver = pkt->data[DATAPKT_HEAD_LEN + 1]; // 软件版本号
//	blocknum = pkt->data[DATAPKT_HEAD_LEN + 2]; // 请求的软件块号

//		id = pkt->pkthead.sid; 				// 源节点号
    softver = pkt->data[DATAPKT_HEAD_LEN]; // 软件版本号
    memcpy(&blocknum, pkt->data + DATAPKT_HEAD_LEN + 1, 2); // 请求的软件块号

//	uint8* pData = pkt->data + DATAPKT_PAYLOAD_OFFSET + pkt->data[DATAPKT_PAYLOAD_OFFSET - 1];
//	UpdataRouteInfo(id, pData, spkt->route.rdepth);		
  

    //置标志位
    COLLECTOR* pCltor = &(cltor[id]);
//		if((pCltor->devAddr[0] <= 0x99) && (softver == adpkt.SoftVersion) )
    if((pCltor->devAddr[0] <= 0x99) && (softver == adv_pkt[I_SS_BASE_VERSION]) && (softver > 0) && (blocknum < 510))
    {
        if(pCltor->nSSBaseUpdataBlockNum == MAX_I_SS_UPDATA_BLOCK_NUM)
        {
            pCltor->nSSBaseUpdataBlockNum = blocknum;
            pCltor->nSSBASEUpdataPktTime = curslottime;

            #ifdef _Debug
            Test[0] = 0x22;
            Test[1] = id;
            Test[2] = softver;			
            Test[3] = blocknum;
            UART_PutArray(UART1, Test, 4);	
            #endif
        }
        else
        {
            #ifdef _Debug
            Test[0] = 0x11;
            Test[1] = id;
            Test[2] = softver;			
            Test[3] = blocknum;
            UART_PutArray(UART1, Test, 4);					
            #endif
        }
    }
    else
        pCltor->nSSBaseUpdataBlockNum = MAX_I_SS_UPDATA_BLOCK_NUM;
    
    if(EzNetTxPkt.bValid)
        return;
    
    DATAPKT *reply = (DATAPKT*)(EzNetTxPkt.TxPkt.data);	
    
    //组程序更新应答包
    reply->ttl = MAX_TIME_LIVE;
    reply->destid = id;
    reply->route.rtype = MASTER_ROUTE_PATH;
    reply->route.downlink = ROUTE_DOWN_LINK; //下行包
    reply->dataseq = cltor[id].sendseq;
    reply->subtype = DATA_SUBTYPE_UPDATE_RESPONSE;
    
    EzNetTxPkt.TxPkt.data[DATAPKT_HEAD_LEN] = softver;
    EzNetTxPkt.TxPkt.data[DATAPKT_HEAD_LEN + 1] = blocknum;
    reply->paylen = 2+Get_Flash_Pro(EzNetTxPkt.TxPkt.data + DATAPKT_HEAD_LEN + 2, blocknum);//从FLASH取出数据
    
    ROUTE temprt = Fill_Route(EzNetTxPkt.TxPkt.data + DATAPKT_HEAD_LEN + reply->paylen,  MASTER_ROUTE_PATH, id); //接收者地址、路由	
    if (temprt.rinfo.rtype != ERROR_ROUTE_PATH) 
    {
        reply->route.rdepth = temprt.rinfo.rdepth;
        recvid = temprt.did;

        EzNetTxPkt.TxPkt.len = DATAPKT_HEAD_LEN + (reply->route.rdepth) + reply->paylen;
        EzNetTxPkt.TxPkt.pkthead.ctrl.bits.msgtype = MSG_TYPE_DATA;
        //	EzNetTxPkt.TxPkt.pkthead.ctrl.bits.seq = cltor[id].sendseq;
        EzNetTxPkt.TxPkt.pkthead.ctrl.bits.DataFlag = 0;
        EzNetTxPkt.TxPkt.pkthead.ctrl.bits.UpFlag = 0; 
        EzNetTxPkt.TxPkt.pkthead.did = recvid;
        EzNetTxPkt.TxPkt.pkthead.sid = localid;
        EzNetTxPkt.TxPkt.pkthead.cid.all = (netcid << 4) | localhop;
        EzNetTxPkt.bValid = 1;
        EzNetTxPkt.nBackOffSlot = 0;
        cltor[id].sendseq++;
    }	
#endif 
}

/******************************************************************************************************
*存储主动上报的电表地址
*
*
*******************************************************************************************************/
void Handle_Meter_Address(PKT *pkt)
{
	HASHT elem;
	uint8 insertok = 0xff;
	DATAPKT *reply = NULL;

	uint8 u8_Source_ID = 0;
	uint8 * u8_Meter_Address_Data = NULL;

	uint8 u8_Meter_NUM = 0;
	uint8 u8_Counter = 0;
	int return_hash;	
        
	uint8* pStartAddr;

	uint8* pSn;
	int ReturnHash = 0;
	memset(&elem, 0xFF, sizeof(HASHT));
	
	u8_Source_ID = pkt->data[DATAPKT_HEAD_LEN]; // 源节点号
	u8_Meter_NUM = pkt->data[DATAPKT_HEAD_LEN + 1]; //收到的电表个数
	u8_Meter_Address_Data = &pkt->data[DATAPKT_HEAD_LEN + 2]; //电表地址区首地址


	pSn = cltor[u8_Source_ID].devAddr;
	if(pSn[0] == 0xFF)
        return;
	if((0 == pSn[0]) && (0 == pSn[1]) && (0 == pSn[2])&&(0 == pSn[3])&&(0 == pSn[4])&&(0 == pSn[5]))
        return;

	uint8 bError = 0;
	if((0xee == u8_Meter_Address_Data[0]) && (0xee == u8_Meter_Address_Data[1]) && (0xee == u8_Meter_Address_Data[2]) 
		&& (0xee == u8_Meter_Address_Data[3]) && (0xee == u8_Meter_Address_Data[4]))
		bError = 1;

	if(0 == bError)
	{
//			memcpy(elem.falsh_sn, pSn, 6);
    	for (u8_Counter= 0; u8_Counter< u8_Meter_NUM; u8_Counter++)//把读到的电表地址插入到哈希表中
		{
			#ifdef PROJECT_PINGSHUI
				pStartAddr	=  u8_Meter_Address_Data + u8_Counter * (METER_ADDRESS_LENGTH);
				//elem.SubType	=  0；		
			#else
				pStartAddr	=  u8_Meter_Address_Data + u8_Counter * (METER_ADDRESS_LENGTH + 1);
				elem.SubType	=*(pStartAddr + METER_ADDRESS_LENGTH); //规约类型
			#endif
			
			memcpy(elem.addr, pStartAddr, METER_ADDRESS_LENGTH);
									
			insertok = 1;
			return_hash=Hash_Table_Insert(&elem, &ReturnHash);			
			if (return_hash < 0)        	//添加哈希表                                        
			{                                                        
				insertok = 0; 			//如果添加失败，置标志位
			}

			if (insertok)
			{  
				if (return_hash==MAX_HASH_TABLE_FLASH_SIZE)
				{
					HASH_Table_Used++;
					HASH_Table_Used=(HASH_Table_Used&1);   //1表示之前用Table1,之后用Table2;    0相反
					I2cWrite(0xA0,&HASH_Table_Used, FM_HASH_USED, 1);			
					Hash_Transfer();//表满,转移哈希表
					Hash_Table_Insert(&elem, &ReturnHash);//再次插入上次未添加成功的
				}
				else //如果添加成功
				;
			}
			else
			{   
			 	//重复添加
			 	shadow_space[ReturnHash].times = MeterDeadCount;//复位老化计数
				I2cWrite(0xA0,(uint8*)&shadow_space[ReturnHash], ReturnHash, 1);	
			}

			if((ReturnHash < MAX_HASH_TABLE_FLASH_SIZE) && (ReturnHash >= 0))
			{
				if(bMeterSearchStart)
					MeterStatus[ReturnHash].bReceive = 1;		
				else
				{
					if(bSingleSearchSSSendMeter)
						MeterStatus[ReturnHash].bSingleSSSearchMeterReceive = 1;	
				}
			}
		}
	}
				
	//设置上送报文回复包的标志
//		if(0 != u8_Meter_NUM)
//			MeterAddrRecMark[u8_Source_ID].bReceived = 1;

	uint8* pData = pkt->data + DATAPKT_PAYLOAD_OFFSET + pkt->data[DATAPKT_PAYLOAD_OFFSET - 1];
	reply = (DATAPKT* )pkt->data;
	UpdataRouteInfo(u8_Source_ID, pData, reply->route.rdepth);	
#if 0 	
//	temp = txdopktq.tail;
	//生成回复包
	 reply = (DATAPKT*)(txdopktq.rxpkt[temp].data);	
	reply->destid = u8_Source_ID;
	reply->ttl = MAX_TIME_LIVE;
 /*       if (cltor[u8_Source_ID].father_update > cltor[u8_Source_ID].mother_update)
                reply->route.rtype = MASTER_ROUTE_PATH;
        else
                reply->route.rtype = BACKUP_ROUTE_PATH;    
                */
	reply->route.rtype = MASTER_ROUTE_PATH;  
 
//	reply->route.rdepth = cltor[u8_Source_ID].hop - 2;

	reply->route.downlink = ROUTE_DOWN_LINK; //下行包
        reply->dataseq = cltor[u8_Source_ID].sendseq;//包顺序号
        
	reply->destid = u8_Source_ID;
        reply->subtype = DATA_SUBTYPE_METER_ADDR_RESPONSE;	//回复的给采集器的业务数据包的类型
	reply->paylen = (1+u8_Meter_NUM);//Payload部分长度

	txdopktq.rxpkt[temp].data[DATAPKT_HEAD_LEN] = u8_Meter_NUM;
	for (u8_Counter= 0; u8_Counter< u8_Meter_NUM; u8_Counter++)
	{
		txdopktq.rxpkt[temp].data[DATAPKT_HEAD_LEN + 1+ u8_Counter] = *(u8_Meter_Address_Data_Temp+u8_Counter*METER_ADDRESS_LENGTH);
	}
	
/*	if (reply->route.rdepth == 0)
	        recvid = reply->destid;
	else
	        recvid = Fill_Route_INFO(txdopktq.rxpkt[temp].data + DATAPKT_HEAD_LEN + reply->paylen, reply->route, u8_Source_ID);
*/
	
	ROUTE temprt = Fill_Route(txdopktq.rxpkt[temp].data + DATAPKT_HEAD_LEN + reply->paylen, reply->route.rtype, u8_Source_ID);
	if(temprt.rinfo.rtype != ERROR_ROUTE_PATH)
	{
		reply->route.rdepth = temprt.rinfo.rdepth;
		recvid = temprt.did;
		
	txdopktq.rxpkt[temp].len = DATAPKT_HEAD_LEN + reply->route.rdepth + reply->paylen;//长度
	txdopktq.rxpkt[temp].pkthead.ctrl.bits.msgtype = MSG_TYPE_DATA;//包类型
	txdopktq.rxpkt[temp].pkthead.ctrl.bits.seq = cltor[u8_Source_ID].sendseq;
	txdopktq.rxpkt[temp].pkthead.did = recvid;//接收者地址
		txdopktq.rxpkt[temp].pkthead.sid = localid;
		txdopktq.rxpkt[temp].pkthead.cid.all = (netcid << 4) | localhop;
		
		cltor[u8_Source_ID].sendseq++;
		txdopktq.tail = (txdopktq.tail + 1) &(QUEUE_LEN_MASK);
		txdopktq.len = (txdopktq.len + 1);
	}
#endif
}

/*************************************************************************
 * Function Name: HandleStaReport
 * Parameters:  downlink packet
 * Return:  the receiver id 
 * Description: handle the downlink packet
 *
 *************************************************************************/
void Handle_Sta_Report(STAREPORT *sp)
{
        // todo : add code here



}



#if 0
/*************************************************************************
 * Function Name: CreateJoinResp
 * Parameters:  downlink packet
 * Return:  none
 * Description: handle the downlink packet
 *
 *************************************************************************/

void Create_Join_RESP(JOINREQ *joinreq)
{

        PKT *pkt;
        JOINRSP *joinresp;

        //LOCK(txdopktq);

        //tail = txdopktq.tail ;
        pkt = &(txdopktq.rxpkt[txdopktq.tail]); //附地址
        //组反馈包
        pkt->len = JOINRSP_ROUTE_OFFSET + 3+(joinreq->route.rdepth);
        // fill the CTRL Byte
        pkt->pkthead.ctrl.bits.msgtype = MSG_TYPE_JREPON;
        pkt->pkthead.ctrl.bits.seq = cltor[joinreq->sourid].sendseq;
        pkt->pkthead.sid = localid;
        pkt->pkthead.cid.all = (netcid << 4) | (localhop);



        joinresp = (JOINRSP*)(pkt->data); //附地址

        joinresp->ttl = MAX_TIME_LIVE;
        joinresp->dataseq = cltor[joinreq->sourid].sendseq;
        joinresp->route.rtype = joinreq->route.rtype;
        joinresp->route.rdepth = joinreq->route.rdepth;
        joinresp->route.downlink = ROUTE_DOWN_LINK;
        joinresp->destid = joinreq->sourid;

        //set the unique mac for every node
        memcpy(pkt->data + JOINRSP_SN_OFFSET, (uint8*)joinreq + JOINREQ_SN_OFFSET, UNIQUE_MAC_ADDR_LEN);
        // 设置采集器相关参数
        memcpy(pkt->data + JOINRSP_ROUTE_OFFSET, (uint8*) &cltparm, 3);
        //set the route path
        memcpy(pkt->data + JOINRSP_ROUTE_OFFSET + 3, (uint8*)joinreq + JOINREQ_ROUTE_OFFSET + 2+joinreq->neignum *2, joinreq->route.rdepth);



        // fill the DID Byte
        if (joinreq->route.rdepth == 0)
                pkt->pkthead.did = joinreq->sourid;
        else
                pkt->pkthead.did = *(pkt->data + JOINRSP_ROUTE_OFFSET + joinreq->route.rdepth - 1);
        // increase  the send sequence num
        cltor[joinreq->sourid].sendseq++;

        txdopktq.tail = (txdopktq.tail + 1) &(QUEUE_LEN_MASK); //尾部加1，并判断翻转
        txdopktq.len = (txdopktq.len + 1);
        //UNLOCK(txdopktq);

}

#endif


/*
uint8 Sn_Search_Id_10(uint8* sn_addr)//根据SN查询ID
{
	uint8 i ;
	for(i=2;i<255;i++)//遍历
	{
		if((!memcmp(cltor[i].devAddr,sn_addr, 5))&&(i==cltor[i].old_id))//对比一样
			return i;			
	}								 
	return 0;//查询不到 	
}
*/

//=============================================
uint8 Check_GetID(uint8 get_id,uint8* get_sn)//判断实际的SN与表中ID对应的SN是否一样
{
	uint8 RES;	
	
	if (!memcmp(cltor[get_id].devAddr,get_sn,6))//对比一样
			RES=1;		
	else
 	  	RES=0;
	 	
	 return RES;
}


/*************************************************************************
 * Function Name: GetFlashPro
 * Parameters:  destdata ， 目标地址；block ，程序的块数
 * Return:  拷贝的长度
 * Description:从Flash  拷贝第几块程序到相应的地址
 *
 *************************************************************************/
uint8 Get_Flash_Pro(uint8 *destdata, uint8 block)
{
#if (PROG_FLASH_512)  
    memcpy(destdata, UpdateSource +32+ block * PROGRAM_BLOCK_SIZE, PROGRAM_BLOCK_SIZE);
#else
//    BFLS_Read(destdata, EXT_FLASH_SS_CODE_START+32+block * PROGRAM_BLOCK_SIZE, PROGRAM_BLOCK_SIZE);//
#endif    
    return PROGRAM_BLOCK_SIZE;
}


/*************************************************************************
 * Function Name: FillStaPkt
 * Parameters:  none
 * Return:  none
 * Description: 
 *
 *************************************************************************/
#if 0
void Fill_Sta_PKT()
{
        // this function will be called with the txdoqueue locked
        uint8 id;
        uint8 tail;
        uint8 recvid;
        STAREQUEST *spkt;

        for (id = 0; id < QUEUE_MAX_LEN; id++)
        {
                tail = txdopktq.tail;
                if (cltor[id].hop > 1)
                //if((cltor[id].hop >1) &&( id == 12))
                {
                        //the node has been join in this net , create the statistic request

                        spkt = (STAREQUEST*)(txdopktq.rxpkt[tail].data);
                        spkt->ttl = MAX_TIME_LIVE;
                        spkt->destid = id;
                        spkt->route.rtype = MASTER_ROUTE_PATH;
                        spkt->route.downlink = ROUTE_DOWN_LINK;
                        spkt->route.rdepth = cltor[id].hop - 2;
                        spkt->staseq = StaReport.curstaseq;

                        //fill the hop data area 
                        if (spkt->route.rdepth == 0)
                        {
                                recvid = spkt->destid;
                        }
                        else
                        {
                                recvid = Fill_Route_INFO(txdopktq.rxpkt[tail].data + STATPKT_HEAD_LEN, spkt->route, id);
                        }
                        //Fill the CTRL and DID Byte
                        txdopktq.rxpkt[tail].len = STATPKT_HEAD_LEN + (spkt->route.rdepth);
                        txdopktq.rxpkt[tail].pkthead.ctrl.bits.msgtype = MSG_TYPE_STATC_REQUEST;
                        txdopktq.rxpkt[tail].pkthead.ctrl.bits.seq = cltor[id].sendseq;
                        txdopktq.rxpkt[tail].pkthead.did = recvid;
                        //Push it to txdopktq
                        txdopktq.tail = (txdopktq.tail + 1) &(QUEUE_LEN_MASK);
                        txdopktq.len = (txdopktq.len + 1);
                }
        }
}
#endif



/*************************************************************************
 * A special ultra-optimized versions that knows they are hashing exactly
 * 3, 2 or 1 word(s).
 *
 * NOTE: In partilar the "c += length; __jhash_mix(a,b,c);" normally
 *       done at the end is not done here.
 *************************************************************************/
static inline uint32 Jhash_3words(uint32 a, uint32 b, uint32 c, uint32 initval)
{
    a += JHASH_GOLDEN_RATIO;
    b += JHASH_GOLDEN_RATIO;
    c += initval;

    __jhash_mix(a, b, c);

    return c;
}

/*************************************************************************
 * Function Name: SendAdver
 * Parameters:  advertisement 
 * Return: none
 * Description: send the advertise
 *
 *************************************************************************/
uint16 Cal_Hash_Value(uint8 *meteraddr) //电表计算哈希值
{
    uint32 key;
    key = (meteraddr[3] << 24) | (meteraddr[2] << 16) | (meteraddr[1] << 8) | (meteraddr[0]);
    return (Jhash_3words(key, 0, 0, 0) %(MAX_HASH_TABLE_FLASH_SIZE));
}

/********************************************************************
*标记已处理的电表地址
*
********************************************************************/
void Record_Handled_Addr(int hash)
{
    shadow_space[hash].handled=HANDLED;
    I2cWrite(0xA0, (uint8*)&shadow_space[hash], hash,1);//写入E2PR0M
}
/*************************************************************************
 * Function Name: htable1_search
 * Parameters:  ammeter 's address 
 * Return: none
 * Description: 根据电表地址查找ID
 *
 *************************************************************************/

int Hash_Table_Search(uint8 *addr)
{
    uint16 hash;
    uint16 index;
    uint8 metertable[6];
    uint8 j;
    for (j = 0; j < 6; j++)
    {
        metertable[j] = addr[j];
        /*            
        if (metertable[j] == 0xAA)
        {
        metertable[j] = 0x0;
        }*/
    }
    hash = Cal_Hash_Value(metertable); //计算得到哈希值
    if(HASH_Table_Used==0)
    {
        if ((htable1[hash].status== OCCUPIED) && (!(memcmp(metertable, htable1[hash].addr, 6))))
        {
            return hash; //返回哈希索引
        }
        index = (hash + 1)  % (MAX_HASH_TABLE_FLASH_SIZE); //如果不是它，则加1往下对比
        while (index != hash)
        {
            if ((htable1[index].status== OCCUPIED) && (!(memcmp(metertable, htable1[index].addr, 6)))) //如果哈希表里有其值
            {
                return index; //返回哈希索引
            }
            index = (index + 1)  % (MAX_HASH_TABLE_FLASH_SIZE); //如果不是它，则加1往下对比
        }
        return ( - 1); //如果找不到，返回-1
    }   
    else
    {
        if ((htable2[hash].status== OCCUPIED) && (!(memcmp(metertable, htable2[hash].addr, 6))))    //如果哈希表里有其值	      
        {
            return hash; //返回哈希索引
        }
        index = (hash + 1)  % (MAX_HASH_TABLE_FLASH_SIZE); //如果不是它，则加1往下对比
        while (index != hash)
        {	
            if ((htable2[index].status== OCCUPIED) && (!(memcmp(metertable, htable2[index].addr, 6))))    //如果哈希表里有其值
            {
                return index; //返回哈希索引
            }
            index = (index + 1)  % (MAX_HASH_TABLE_FLASH_SIZE); //如果不是它，则加1往下对比
        }
        return ( - 1); //如果找不到，返回-1
    }   
}

//通过黑白名单, 检查输入的表地址地址
uint8 Meter_Check(uint8* addr)	
{
	uint8 rc = FALSE;
    int ret;

    ret = Hash_Table_Search(addr);

    if(ret != (-1))
    	rc = TRUE;
	return rc; 
}

uint16 Meter_Num(void)
{
    uint16 m=0,i;
    
    HASHT* pHasht;
//	if(HASH_Table_Used==0)//当前使用table1
// 		pHasht = htable1;
//	else
//		pHasht = htable2;
//
//
//    for(i = 0; i < MAX_HASH_TABLE_FLASH_SIZE; i++)
//    {
//        if((pHasht[i].status == OCCUPIED && pHasht[i].devType != 2))
//        {
//            m++;
//        }
//    }
    return m;
}
uint16 Record_Num(void)
{
    uint16 m=0,i;
    
//    HASHT* pHasht;
//	if(HASH_Table_Used==0)//当前使用table1
// 		pHasht = htable1;
//	else
//		pHasht = htable2;
//
//
//    for(i = 0; i < MAX_HASH_TABLE_FLASH_SIZE; i++)
//    {
//        if((pHasht[i].status == OCCUPIED))
//        {
//            m++;
//        }
//    }
    return m;
}
//获取最大路由级别
uint8 Get_MaxHop(void)
{
    uint8 maxhop = 2;
    uint16 i = 0;
    for(i = 2; i < MAX_SUP_SS_NUM+1; i++)
    {
        if(cltor[i].devAddr[0] < 0xFF && cltor[i].hop > maxhop)
        {
            maxhop = cltor[i].hop;
        }
    }
    return maxhop;
}

//获取广播延时
uint16 Get_BroadCast_Delay(uint8 type)
{
    uint8 max = Get_MaxHop();
    uint16 delay = MAX_DATA_TIME_LIVE;
    if(max <= 2)
    {
        return delay;
    }
    delay = MAX_DATA_TIME_LIVE * 2;
    switch(type)
    {
    case 0:
        delay = (0x0001 << (max - 2)) * delay;
        break;
    case 1:
        delay = (0x0001 << (max - 2)) * delay * 2;
        break;
    case 2:
    case 3:
    case 4:
    case 5:        
        delay = (0x0001 << (max - 2)) * delay * 2 * 2;
        break;
    case 0x99: 
        delay = MAX_DATA_TIME_LIVE;
        delay = (0x0001 << (max - 2)) * delay;
        break;
    case 0x98: 
        delay = MAX_DATA_TIME_LIVE;
        break;
    default:
        
        break;
    }
    return delay;
}



/*************************************************************************
 * Function Name: memcmp
 * Parameters:  
 * Return: 1  内存内容不同，0 内存内容相同
 * Description: 内存比较
 *
 *************************************************************************/
uint8 LPC_Memory_CMP(uint8 *dest, uint8 *sour, uint32 len)
{
    uint32 i;
    if ((dest == NULL) || (sour == NULL))
        return 1;
    for (i = 0; i < len; i++)
    {
        if (dest[i] != sour[i])
            return 1;
    }
    return 0;
}



/*************************************************************************
 * Function Name: UserParamInit
 * Parameters: void
 * Return: void
 *
 * Description: Initialize the parameters and variables 
 *		
 *************************************************************************/

void Queue_Init(QUEUE *q)
{
        if (!q)
                return ;
        q->len = 0;
        q->head = 0;
        q->tail = 0;
        memset(q->rxpkt, 0, sizeof(PKT) *QUEUE_MAX_LEN);//清空队列中的数据
}

/*************************************************************************
 * Function Name: CtlPktInit
 * Parameters: void
 * Return: void
 *
 * Description: Initialize the parameters and variables 
 *		
 *************************************************************************/
//	void Ctl_PKT_Init()
//	
//	{
//	        AckPkt.pkthead.cid.bits.cid = netcid;
//	        AckPkt.pkthead.cid.bits.hop = localhop;
//	        AckPkt.pkthead.ctrl.bits.msgtype = MSG_TYPE_ACK;
//	//        AckPkt.pkthead.ctrl.bits.seq = 0;
//	        AckPkt.pkthead.ctrl.bits.DataFlag = 0;
//	        AckPkt.pkthead.ctrl.bits.UpFlag = 0;   
//	                
//	        AckPkt.pkthead.sid = localid;
//	        AckPkt.len = 1;
//	
//	
//	        RtsPkt.pkthead.cid.bits.cid = netcid;
//	        RtsPkt.pkthead.cid.bits.hop = localhop;
//	        RtsPkt.pkthead.ctrl.bits.msgtype = MSG_TYPE_RTS;
//	//        RtsPkt.pkthead.ctrl.bits.seq = 0;
//	        RtsPkt.pkthead.ctrl.bits.DataFlag = 0;
//	        RtsPkt.pkthead.ctrl.bits.UpFlag = 0;          
//	        RtsPkt.pkthead.sid = localid;
//	        RtsPkt.len = 1;
//	
//	        CtsPkt.pkthead.cid.bits.cid = netcid;
//	        CtsPkt.pkthead.cid.bits.hop = localhop;
//	        CtsPkt.pkthead.ctrl.bits.msgtype = MSG_TYPE_CTS;
//	//        CtsPkt.pkthead.ctrl.bits.seq = 0;
//	        CtsPkt.pkthead.ctrl.bits.DataFlag = 0;
//	        CtsPkt.pkthead.ctrl.bits.UpFlag = 0;   
//	        CtsPkt.pkthead.sid = localid;
//	        CtsPkt.len = 1;		
//	}



/*************************************************************************
 * Function Name: UserParamInit
 * Parameters: void
 * Return: void
 *
 * Description: Initialize the parameters and variables 
 *		
 *************************************************************************/

void Si_Pameter_Scan(){

}

/*************************************************************************
 * Function Name: timerIntRandom
 * Parameters: void
 * Return: Get random num ,and backoff the random offset slot
 *
 * Description: Get random num
 *		
 *************************************************************************/

uint8 Timer_INT_Random(void)
{
        uint8 temp8;
        // 61 is largest prime less than 256/4
        EZMacProRandomNumber *= 245; // 4 * 61 + 1 = 245
        EZMacProRandomNumber += 251; // 251 is the largest prime < 256


        temp8 = EZMacProRandomNumber >> 4;
        if (temp8 == 0)
                return 1;
        else
                return temp8;
}

/*************************************************************************
 * Function Name: StaRepInit
 * Parameters: void
 * Return: void
 *
 * Description: Initialize the parameters and variables 
 *		
 *************************************************************************/
void Sta_Rep_Init()
{
        StaReport.stamodeon = 0; // off
        StaReport.nextcycletime = 0;
        StaReport.curstaseq = 0;

}

/*************************************************************************
 * Function Name: UserParamInit
 * Parameters: void
 * Return: void
 *
 * Description: Initialize the parameters and variables 
 *		
 *************************************************************************/
void User_Parameter_Init(void)
{
//    int i = 0;

    //localid = 1;
    
    netslotnum = rfpara.rf_slotnum;//
    netcid = rfpara.rf_net_id;//网络号

    curslottime = 0;
    curtimecycle = 0;

    localhop = 1;


    cltor[localid].hop = 1;

//	    Ctl_PKT_Init(); //初始化ACK.RTS.CTS包
    Queue_Init(&rxdopktq);

    Sta_Rep_Init();
    Ez_MAC_Init();
    Set_Self_ID();
	Cltor_init();
    //资产初始化
//    RecoverCltorPara();
//    memset(g_ucPktRssiValue, 0, 256);
    negapkt.len = 0;
    negapkt.data[0] = START_FLAG;

	bBroadStart = 0;
	bBroadStartTriger = 0;
	//bMeterSearchStart = 0;
	//bMeterDeadStart = 0;
	bCltorSearchStart = 0;

	bWaitForSendMeterAck = 0;
	memset(MeterStatus, 0, MAX_HASH_TABLE_FLASH_SIZE * sizeof(METER_STATUS));

	bSendNeedWait = 0;

	bNeedAckMeter = 0;
	memset(NeedAckMeterAddr, 0, sizeof(NeedAckMeterAddr));
	NeedAckSourceId = 0;
	u32LastFoundSlot = 0;
	bSendTickToSS = 0;
	bNeedMeterDeleteAck = 0;

//		memset(&EzDataTxPkt,            0,          sizeof(EzDataTxPkt));
//		memset(&EzNetTxPkt,             0,          sizeof(EzNetTxPkt));	
//	    memset(&EzHHUTxPkt,             0,          sizeof(EzHHUTxPkt));	
    memset(&EzBeaconTxPkt,          0,          sizeof(EzBeaconTxPkt));	
    memset(&g_stPendingAddrList,    0,          sizeof(g_stPendingAddrList));	
    
//	memset(MeterAddrRecMark, 0,sizeof(MeterAddrRecMark));

	//memset(SSMapShadowSpace, 0, sizeof(SSMapShadowSpace));

	//bSendResetCmd = 0;	//启动时不需要发生重启广播命令
			
	LastUpdataPktDeadTime = 0;

	LastSSBaseUpdataPktDeadTime = 0;
	EzTestRevDone = 0;
	nBaseSSVersion = 0;
	
	memset(ccl_addr, 0, 6);

	bNeedChangeChannel = 0;
    bNeedRelayBroadCast = 0;
	nChannelChangeNum = 0;
	bJcDisallowFalg = 0;
	bEZMacRecFlag = 0;
#if SEND_FREQ_CTRL    
    memset(g_ucSendCount, 0, MAX_AVAILBLE_FRQ);
#endif
    memset(NetAvalidFreq, 0, 8);

    bMeterInfoSynEnable = 1;
    memset((uint8 *)&g_stMuchframeindex_port, 0, sizeof(STMUCHFRAMEINDEX));

    DeleteProcInit();
//    CompressAddrInit(CON_DEV_ADDR_LEN_8, 5, 0);
    sendCacheInit();
}

/**********************************************************************
 *EzMacInit ,设定EzMac的一些参数
 *
 **********************************************************************/

void Ez_MAC_Init()
{
//	    SystemStatus = SYSTEM_STATUS_STARTUP;
    EZMacProReg.RSSILR = 0x78; // 设置RSSI的门限值
//	    MSR = EZMAC_PRO_IDLE;
    setMSR(EZMAC_PRO_IDLE);
}


/**********************************************************************
 *从E2P中获取网络参数
 *
 **********************************************************************/
const uint8 FH_SEQ[16]={5,7,9,11,13,14,15,16,17,18,19,20,21,23,25,27};

void GetFreqChannel()
{
	//频段号
	uint8 temp;
	I2cRead(0xA0,  &temp, FM_FREQ_CHANNEL_NUM, 1);          
	if(temp >= FREQ_NUM)
	{
		temp = SX127X_FREQ;
		I2cWrite(0xA0,  &temp, FM_FREQ_CHANNEL_NUM, 1);         
	}
	nFreqChannel = temp;	
}


//extern UPDATE_PKT upd_pkt;//升级包结构体
//extern STUPGRADE g_stUpgrade;
void Get_Net_Parameter(void)
{ 
	uint8  temp,ad;
//	uint8 Temp1[9];
//	uint8  P[10]={0};
    UPDATE_PKT tmpPkt;
    uint8 bNeedSave = 0;
//	    I2cRead(0xA0,  (uint8 *)&tmpPkt, EEPROM_UPDATE_PARA, sizeof(UPDATE_PKT));  
//	
//	    if(tmpPkt.crc == CRC_16BIT_Check1((uint8 *)&tmpPkt, sizeof(UPDATE_PKT) - 2))
//	    {
//	        memcpy((uint8 *)&upd_pkt,(uint8 *)&tmpPkt,sizeof(UPDATE_PKT));
//	        if(upd_pkt.nextSeq !=0) upd_pkt.nextSeq++;
//	    }
//	    else
//	    {
//	        memset((uint8 *)&upd_pkt,0,sizeof(UPDATE_PKT));
//	    }
//	    
//	    I2cRead(0xA0,  (uint8 *)&g_stUpgrade, DP_DOWNLOAD_PROC_PARA_ADDR, sizeof(STUPGRADE));  
//	
//	    if(g_stUpgrade.crc == CRC_16BIT_Check1((uint8 *)&g_stUpgrade, sizeof(STUPGRADE) - 2))
//	    {
//	        g_stUpgrade.timeCount = GET_TICK_COUNT( );
//	    }
//	    else
//	    {
//	        //memset((uint8 *)&g_stUpgrade,0,sizeof(STUPGRADE));
//	    }
//	    
//		I2cRead(0xA0, (uint8*) &cltparm, FM_CLTP_ADDR, sizeof(CLTP)); //获取路由参数         
//		if (cltparm.maxttl == 0xff)// 如果I2C读出来都是零，则默认值
//		{
//			cltparm.routeuptime = 60;//心跳      
//			cltparm.maxttl = 120;//生存周期
//			cltparm.routeseltime = 30;//选父节点时间    
//			I2cWrite(0xA0, (uint8*) &cltparm, FM_CLTP_ADDR, sizeof(CLTP)); //写入路由参数         
//		}
//		I2cRead(0xA0,  &temp, FM_SS_CODEUPDATA_ENABLE, 1); //获取路由参数         
//		if(0xff == temp)
//		{
//			temp = 0;
//			I2cWrite(0xA0,  &temp, FM_SS_CODEUPDATA_ENABLE, 1);         
//		}
//	//	    adpkt.subtype.SSCodeUpdataEnable = temp;
//		
//		//载波主节点地址
//		memset(nDeviceMacAddr, 0, 10);
//		I2cRead(0xA0, &nDeviceMacAddrLen, FM_DEVICE_ADDR_LEN, 1); //载波主节点地址
//	    if(nDeviceMacAddrLen > 10)
//	    {
//	        nDeviceMacAddrLen = 10;
//	    }
//		I2cRead(0xA0, nDeviceMacAddr, FM_DEVICE_ADDR, nDeviceMacAddrLen); //载波主节点地址
//		
//		//I型采集器底座的升级使能
//	//		I2cRead(0xA0,  &temp, FM_I_SS_CODEUPDATA_ENABLE, 1); 
//	//		if(0xff == temp)
//	//		{
//	//			temp = 0;
//	//			I2cWrite(0xA0,  &temp, FM_I_SS_CODEUPDATA_ENABLE, 1);         
//	//		}
//	//	    adpkt.subtype.I_SSCodeUpdataEnable = temp;
//			
//		//电表老化次数
//		I2cRead(0xA0,  &temp, FM_METER_DEAD_COUNT, 1); 
//		if((0xff == temp) || (0 == temp))
//		{
//			temp = METER_DEAD_COUNT;
//			I2cWrite(0xA0,  &temp, FM_METER_DEAD_COUNT, 1);         
//		}
//	    MeterDeadCount = temp;
//		//拓扑显示的显示内容来源
//		I2cRead(0xA0,  &temp, FM_TUOPU_SOURCE, 1); 
//		if((0xff == temp) || (temp >= 2))
//		{
//			temp = 0;
//			I2cWrite(0xA0,  &temp, FM_TUOPU_SOURCE, 1);         
//		}
//	    u8TuopuDisplaySource = temp;
//			
//	    I2cRead(0xA0, (uint8*) &HASH_Table_Used, FM_HASH_USED, 1); //获取有效哈希位
//	    if(HASH_Table_Used==0xff)
//	    {
//	        HASH_Table_Used=0x0; 
//	        I2cWrite(0xA0, (uint8*) &HASH_Table_Used, FM_HASH_USED, 1);       
//	    }
//		
//	    I2cRead(0xA0, (uint8*) &CurWhiteSSMap, FM_WRITE_HASH_USED, 1); //获取有效哈希位
//	    if(CurWhiteSSMap==0xff)
//	    {
//	        CurWhiteSSMap=0x0; 
//	        I2cWrite(0xA0, (uint8*) &CurWhiteSSMap, FM_WRITE_HASH_USED, 1);       
//	    }
//	
//	    I2cRead(0xA0, (uint8*) &CurBlackSSMap, FM_BLACK_HASH_USED, 1); //获取有效哈希位
//	    if(CurBlackSSMap==0xff)
//	    {
//	        CurBlackSSMap=0x0; 
//	        I2cWrite(0xA0, (uint8*) &CurBlackSSMap, FM_BLACK_HASH_USED, 1);       
//	    }
//	
//	    I2cRead(0xA0, (uint8*) &bSSValidateEnable, FM_SS_VALIDATE_ENABLE, 1); //采集器认证使能参数
//	    if(bSSValidateEnable==0xff)
//	    {
//	        bSSValidateEnable = 0x0; 
//	        I2cWrite(0xA0, (uint8*) &bSSValidateEnable, FM_SS_VALIDATE_ENABLE, 1);       
//	    }
//	
//		//需要开启认证使能的参数，在激活主动注册时触发
//		bNeedOpenSSValidateEnable = 0;
//		I2cRead(0xA0, (uint8*)&bNeedOpenSSValidateEnable, FM_NEED_OPEN_SS_VALIDATE , 1); 
//		if(0xff == bNeedOpenSSValidateEnable)	
//		{
//			bNeedOpenSSValidateEnable = 0;
//			I2cWrite(0xA0, &bNeedOpenSSValidateEnable, FM_NEED_OPEN_SS_VALIDATE , 1); 		
//		}
//		if(bNeedOpenSSValidateEnable)
//		{
//			if(0 == bSSValidateEnable)
//			{
//				bSSValidateEnable = 1;
//				I2cWrite(0xA0, &bSSValidateEnable, FM_SS_VALIDATE_ENABLE , 1); 		
//			}
//			bNeedOpenSSValidateEnable = 0;
//			I2cWrite(0xA0, &bNeedOpenSSValidateEnable, FM_NEED_OPEN_SS_VALIDATE , 1); 		
//		}
//		
//		 I2cRead(0xA0, (uint8*) &bMeterInfoSynEnable, FM_METER_INFO_SYN_ENABLE, 1); //采集器认证使能参数
//		 if(bMeterInfoSynEnable==0xff)
//		 {
//			bMeterInfoSynEnable = 0x1; 
//			I2cWrite(0xA0, (uint8*) &bMeterInfoSynEnable, FM_METER_INFO_SYN_ENABLE, 1);       
//		 }
//		
//		EZMacProReg.Temper_ADC_Value = 0;
//		EZMacProReg.Cur_Crystal_Value = 0;	 
//		I2cRead(0xA0, (uint8*) &EZMacProReg.CrystalValue, FM_CAP_MEND, 1); //获取有校准值         
//		if(EZMacProReg.CrystalValue == 0xff)
//		{
//			EZMacProReg.CrystalValue = 0xC7;
//			//by peter 校准值不能随便改动!!!
//			//I2cWrite(0xA0, (uint8*) &CrystalValue, FM_CAP_MEND, 1);       
//		}
//		Set_Crastyle(EZMacProReg.CrystalValue); // 设置晶振电容补偿值
//		
//	#ifdef EN_TEMPERAUREDEGC
//		uint8 temp8;
//		macSpiWriteReg(SI4432_ADC_CONFIGURATION, 0x80);
//		while(1)
//		{
//			temp8 = macSpiReadReg(SI4432_ADC_CONFIGURATION);
//			if(temp8 & 0x80)
//				break;
//		}
//		EZMacProReg.Temper_ADC_Value = macSpiReadReg(SI4432_ADC_VALUE);
//		Set_Crystal_Value(EZMacProReg.Temper_ADC_Value);
//	#endif
//		
//		I2cRead(0xA0, (uint8*) &rf_define, FM_FREQ_DEFINE , 7); //获取频点定义
//		if(rf_define.startChanne[0]==0xff&&rf_define.frq_num==0xff)
//	    {
//			if(USER_DEFINE == USER_JuHua_433m)
//			{
//				rf_define.startChanne[0]=0x04;//
//				rf_define.startChanne[1]=0x33;
//				rf_define.startChanne[2]=0x0;
//				rf_define.startChanne[3]=0x0;
//				rf_define.channel_Wideth=0xc8;//广播步长
//				rf_define.frq_num=0x64;//频点个数	
//			}
//			else if(USER_DEFINE == USER_LuDeng)
//			{
//				rf_define.startChanne[0]=0x04;//
//				rf_define.startChanne[1]=0x70;
//				rf_define.startChanne[2]=0x0;
//				rf_define.startChanne[3]=0x0;
//				rf_define.channel_Wideth=0xc8;//广播步长
//				rf_define.frq_num=0x64;//频点个数				
//			}
//			else
//			{
//				if(nFreqChannel == FREQ_470M)
//				{
//					rf_define.startChanne[0]=0x04;//
//					rf_define.startChanne[1]=0x70;
//					rf_define.startChanne[2]=0x0;
//					rf_define.startChanne[3]=0x0;
//					rf_define.channel_Wideth=0xc8;//广播步长
//					rf_define.frq_num=0x64;//频点个数				
//				}
//				else if(nFreqChannel == FREQ_433M)
//				{
//					rf_define.startChanne[0]=0x04;//
//					rf_define.startChanne[1]=0x33;
//					rf_define.startChanne[2]=0x0;
//					rf_define.startChanne[3]=0x0;
//					rf_define.channel_Wideth=0xc8;//广播步长
//					rf_define.frq_num=0x64;//频点个数	
//				}	
//				else if(nFreqChannel == FREQ_868M)
//				{
//					rf_define.startChanne[0]=0x08;//
//					rf_define.startChanne[1]=0x68;
//					rf_define.startChanne[2]=0x0;
//					rf_define.startChanne[3]=0x0;
//					rf_define.channel_Wideth=0xc8;//广播步长
//					rf_define.frq_num=0x64;//频点个数					
//				}
//				else if(nFreqChannel == FREQ_915M)
//				{
//					rf_define.startChanne[0]=0x09;//
//					rf_define.startChanne[1]=0x15;
//					rf_define.startChanne[2]=0x0;
//					rf_define.startChanne[3]=0x0;
//					rf_define.channel_Wideth=0xc8;//广播步长
//					rf_define.frq_num=0x64;//频点个数					
//				}
//	            else if(nFreqChannel == FREQ_928M)
//				{
//					rf_define.startChanne[0]=0x09;//
//					rf_define.startChanne[1]=0x28;
//					rf_define.startChanne[2]=0x0;
//					rf_define.startChanne[3]=0x0;
//					rf_define.channel_Wideth=0xc8;//广播步长
//					rf_define.frq_num=0x64;//频点个数					
//				}
//			}
//	    }       
//	
//	    gab=(rf_define.startChanne[0]&0xf)*100+(rf_define.startChanne[1]&0xf)+(rf_define.startChanne[1]>>4)*10;//输入的频点
//	
//	    uint16 usBase = 0;
//	    
//	    
//		if(USER_DEFINE == USER_JuHua_433m)
//	    {   
//			usBase = 433;//gab = ((gab-433)>>1)*10;//偏移值
//	    }
//		else if(USER_DEFINE == USER_LuDeng)
//	    {   
//			usBase = 470;//gab = ((gab-470)>>1)*10;//偏移值
//	    }
//		else
//		{
//		    switch(nFreqChannel)
//	        {
//	        case FREQ_470M:
//	            usBase = 470;
//	            break;
//	        case FREQ_433M:
//	            usBase = 433;
//	            break;
//	        case FREQ_868M:
//	            usBase = 868;
//	            break;
//	        case FREQ_915M:
//	            usBase = 915;
//	            break;
//	        case FREQ_928M:
//	            usBase = 928;
//	            break;            
//	        default:
//	            break;
//	
//	        }   
//	    }
//	        
//	    if(gab >= usBase )
//	    {
//	        gab = ((gab-usBase)>>1)*10;//偏移值
//	    }
//	    else
//	    {
//	        gab = 0;
//	    }
//	
//	    gab=(gab + (rf_define.startChanne[2] >> 5));//偏移值
//	    	
//		I2cRead(0xA0, (uint8*)&rfpara, FM_NETPARA_ADDR, sizeof(RF_PARAMETER)); //获取网络参数	
//		//I2cRead(0xA0, P, FM_NETPARA_ADDR+4, 5); //获取网络参数	
//		
//	//		rfpara.rf_net_id=*(P+1) <<8|*(P);
//	//		rfpara.rf_route = *(P+2);//路由方式
//	//		rfpara.rf_jump_chaneel=*(P+3);//调频序列号
//	//		rfpara.rf_slotnum=MAX_SUP_SS_NUM;//*(P+4);//时帧长度 
//	
//	    if (rfpara.rf_channel== 0xff && rfpara.rf_boad== 0xff && rfpara.rf_power== 0xff && rfpara.rf_route== 0xff)
    {
//	        uint8 channelId;
//	        if(nDeviceMacAddr[0] != 0xFF)
//	        {
//	            channelId = Cal_Hash_Value(nDeviceMacAddr) % LORA_CHANNEL_NUM;
//	            if(channelId == 0)
//	            {
//	                channelId = 1;
//	            }                  
//	
//	        }
//	        else
//	        {
//	            channelId = 0;
//	        }
//        uint32_t addr = Cal_Hash_Value(nDeviceMacAddr);
//        memcpy(nDeviceMacAddr+1, &addr, 4);
//	        sys_time_t ul_TkTick = krhino_sys_tick_get();
//	        memcpy(nDeviceMacAddr, &addr, 4);
//        nDeviceMacAddr[5] = krhino_sys_tick_get();
        rfpara.rf_channel = 0;// channelId; //信道号
        rfpara.rf_power = 0;//发射功率
        rfpara.rf_boad = 0x1;//空中波特率
#ifdef RF_MESH_LEVEL2
        rfpara.rf_limit = 80;//rssi门限    
#else
        rfpara.rf_limit = 157;//rssi门限    
#endif        
        rfpara.rf_net_id = 0x01;//网络标识
        rfpara.rf_route = 0x01;//路由方式
        rfpara.rf_jump_chaneel=0x2;//调频序列号
//                rfpara.rf_slotnum=0xfb;//时帧长度
        rfpara.rf_slotnum=MAX_SUP_SS_NUM;//时帧长度
        memcpy((uint8 *)&rfpara.panid, nDeviceMacAddr, 2);
        //rfpara.panid = 0xFFFF;
        rfpara.shortID = localid;
        bNeedSave = 1;
    }

//	    if(rfpara.rf_channel < 1 || rfpara.rf_channel > 6)
//	    {
//	        //extern uint8 nDeviceMacAddr[6];
//	        uint8 channelId = Cal_Hash_Value(nDeviceMacAddr) % LORA_CHANNEL_NUM;
//	        if(channelId == 0)
//	        {
//	            channelId = 1;
//	        }                  
//	    
//	        rfpara.rf_channel = channelId; //信道号
//	        bNeedSave = 1;
//	    }
//	    if(rfpara.rf_power > 4)
//	    {
//	        rfpara.rf_power = 0;//发射功率
//	        bNeedSave = 1;
//	    }
//	    if(rfpara.rf_slotnum > MAX_SUP_SS_NUM)
//	    {
//	        rfpara.rf_slotnum = MAX_SUP_SS_NUM;
//	        bNeedSave = 1;
//	    }
//	    
//	    if(rfpara.shortID == 0xFFFF || rfpara.shortID == 0xEEEE || rfpara.shortID == 0)
//	    {
//	        rfpara.shortID = localid;
//	        bNeedSave = 1;
//	    }
//	    
//	    if(rfpara.panid == 0xFFFF || rfpara.panid == 0xEEEE || rfpara.panid == 0)
//	    {
//	        memcpy((uint8 *)&rfpara.panid, nDeviceMacAddr, 2);
//	        bNeedSave = 1;
//	    }
//	
//	    if(rfpara.rf_limit > 157)
//	    {
//	#ifdef RF_MESH_LEVEL2
//	        rfpara.rf_limit = 80;//rssi门限    
//	#else
//	        rfpara.rf_limit = 157;//rssi门限    
//	#endif        
//	        bNeedSave = 1;
//	    }
//	    if(bNeedSave)
//	    {
//		    I2cWrite(0xA0, (uint8*)&rfpara, FM_NETPARA_ADDR, sizeof(RF_PARAMETER)); //获取网络参数	
//	    }
//		nDefaultJumpChannel = rfpara.rf_jump_chaneel;
//	
//		//信道配置模式
//		I2cRead(0xA0, &temp, FM_CHANNEL_SET_MODE, 1); //信道配置模式
//		if(0xff == temp)
//		{
//			temp = 0;
//			I2cWrite(0xA0,  &temp, FM_CHANNEL_SET_MODE, 1);         		
//		}
//		bChannelSetManualMode = temp;
//	//		if((0 == bChannelSetManualMode) && (USER_DEFINE == USER_DianLi))
//	//		{
//	//			temp = nDeviceMacAddr[2];
//	//			temp = (temp / 16) * 10 + temp % 16;
//	//			temp = temp % 16 + 1;
//	//			if(temp != rfpara.rf_channel)
//	//			{
//	//				rfpara.rf_channel = temp;	    
//	//				rfpara.rf_net_id = temp;
//	//	
//	//				memcpy(Temp1, (uint8*)&rfpara, 3);
//	//				Temp1[3] = rfpara.rf_net_id;
//	//				Temp1[4] = 0;
//	//				Temp1[5] = rfpara.rf_route;
//	//				Temp1[6] = nDefaultJumpChannel;
//	//				Temp1[7] = rfpara.rf_slotnum;
//	//				Temp1[8] = 0;
//	//				I2cWrite(0xA0, Temp1, FM_NETPARA_ADDR, 8); //存网络参数***			
//	//			}
//	//		}
//		
//		//add by peter
//		 rfpara.rf_net_id = rfpara.rf_net_id-1;//实际使用值比显示值小1  
//		 rfpara.rf_jump_chaneel=FH_SEQ[ rfpara.rf_channel-1];//调频序列号
//		 
//		 
//	//	    temp=  rfpara.rf_power &0x7;
//	//	    SpiWriteRegister(SI4432_TX_POWER, (temp|0x18)); // 发射功率0 ~ 7
//	
//	         // rfpra.rf_boad、 以后要设置
//		 // I2cWrite(0xA0, (uint8*) &P, FM_NETPARA_ADDR, 8); //获取网络参数
//		I2cRead(0xA0,  (uint8*) &channel_para, (FM_CHANNEL_PARA+((rfpara.rf_channel-1)<<2)), 4);//获取信道参数
//		 
//		if(channel_para.channel_num==0xff)
//		{
//			if(USER_DEFINE == USER_JuHua_433m)
//			{
//				channel_para.channel_num=1;
//				channel_para.data_start=2;
//				channel_para.data_end=7;
//				channel_para.data_freq_num=6;
//			}
//			else
//			{
//				channel_para.channel_num=1;
//				channel_para.data_start=16;
//				channel_para.data_end=46;
//				channel_para.data_freq_num = MAX_AVAILBLE_FRQ - 1;
//			}
//		}
//		
//		if(USER_DEFINE == USER_JuHua_433m)
//			channel_para.data_freq_num = 6;
//		else
//			channel_para.data_freq_num = MAX_AVAILBLE_FRQ - 1;
//		
//		broad_fhc= (rfpara.rf_channel - 1 + gab);//广播实际偏移值
//		data_fhc=(channel_para.data_start + gab);//业务实际偏移值
//	
//		 
//		I2cRead(0xA0,  &ad,  FM_RESET_MESAGE, 1);//获取复位信息值
//		if(ad==0xff)
//		{
//			ad=0;
//		}
//		ad=ad+1;//
//		I2cWrite(0xA0,  &ad,  FM_RESET_MESAGE, 1);//获取复位信息值
//	//		adv_pkt[POWER]=(ad&0xf)<<4|temp;//放入广播包的对应位置
//	
//		bSendResetCmd = 1;	//启动时需要发生重启广播命令
//		uint8 bNotNeedSendResetCmd = 0;
//		I2cRead(0xA0,  &bNotNeedSendResetCmd, FM_NEED_RESET_CMD, 1); 
//		if(bNotNeedSendResetCmd)
//		{
//			if(1 == bNotNeedSendResetCmd)
//				bSendResetCmd = 0;
//			bNotNeedSendResetCmd = 0;
//			I2cWrite(0xA0, &bNotNeedSendResetCmd, FM_NEED_RESET_CMD, 1);        		
//		}
//	
//		bBroadMeterEnable = 0;
//	    I2cRead(0xA0,  &bBroadMeterEnable, FM_BROAD_METER_ENABLE, 1); 
//	    if(0x01 != bBroadMeterEnable)
//	    {
//	    	bBroadMeterEnable = 0;
//	    }
//	    else
//	    {
//	        bMeterSearchStart = 1;
//	    }
//			
//		nBroadMeterTime = 0;
//	    
//	//    temp = SpiReadRegister(SI4432_TX_POWER);
//	    I2cRead(0xA0, (uint8*)&nBroadMeterTime, FM_BROAD_METER_TIME , 2); 
//	    if(0xffff == nBroadMeterTime)	
//	    	nBroadMeterTime = 0;
}


/**********************************************************************
 *软复位函数
 *
 **********************************************************************/

void Set_Self_ID()
{
//	    SpiWriteRegister(SI4432_TRANSMIT_HEADER_2, (netcid << 4) | localhop); // set CID and hop num
//	    SpiWriteRegister(SI4432_TRANSMIT_HEADER_1, localid); // set SID
}


//	EZ_TX_PKT * Get_Idle_Send_Pkt(void)
//	{
//	    if(!EzHHUTxPkt.bValid)
//	    {
//	        return (&EzHHUTxPkt);
//	    }
//	    else if(!EzNetTxPkt.bValid)
//	    {
//	        return (&EzNetTxPkt);
//	    }
//	    else if(!EzDataTxPkt.bValid)
//	    {
//	        return (&EzDataTxPkt);
//	    }
//	    else
//	    {
//	        return NULL;
//	    }
//	}


//====================================================
void Create_JRep(uint16 k, EZ_TX_PKT * ezPkt)//组入网应答包
{
	PKT *pkt;
	uint8 /*recvid , */bNotSend = 0;
	//JOINRSP *joinresp;
	//EXTDATA* pExtData;
	//uint8 bTypeExtData = 0;
//	uint8 bNeedDelete = 0;
	//ROUTE temprt;
    uint8 m = 0;
    uint16 whiteMeterNum = 0;
	if(cltor[k].nodestatus.bNeedDelete)
	{
//		bNeedDelete = 1;
        cltor_shadow[k].FrameDeadTime = MAX_DATA_TIME_LIVE;
		//cltor[k].nod.ans_pkt_type = 3;
	}
	pkt = &(ezPkt->TxPkt);
	pkt->ttl = MAX_DATA_TIME_LIVE;	//生存周期by ben 29
    pkt->index = k;
    pkt->protocol = cltor[k].nodestatus.protocol;
    
    if(cltor[k].nodestatus.protocol == PST_FRM_WL_1_NO)
	{
        //LOG_DEBUG( DBGFMT"Create_JRep[%d] FrameDeadTime = %d\n",DBGARG, k, cltor[k].FrameDeadTime);

        memset((uint8 *)&pkt->head.apdu, 0, sizeof(STAPDU));
        
		pkt->head.apdu.ctrl.dir = 0;
        pkt->head.apdu.ctrl.prm = 0;
        pkt->head.apdu.ctrl.evtmode = 1;
        pkt->head.apdu.ctrl.ftd = 7;
        pkt->head.apdu.seq = cltor_shadow[k].recvseq;
        pkt->head.apdu.srssi = cltor_shadow[k].rRssi;
        if(cltor_shadow[k].nodestatus.ans_pkt_type == 0x01)
        {
            pkt->head.apdu.fn = 3;//登陆
            if(cltor_shadow[k].nodestatus.result == 0x07)
            {
                pkt->head.apdu.fn = 7;
                cltor_shadow[k].nodestatus.result = 0;
            }
            else if(cltor_shadow[k].nodestatus.result == 0x06)
            {
                pkt->head.apdu.fn = 7;
                cltor_shadow[k].nodestatus.result = 2;
            }
                
        }
        else if(cltor_shadow[k].nodestatus.ans_pkt_type == 0x00)
        {
            pkt->head.apdu.fn = 5;//心跳
        }
        else if(cltor_shadow[k].nodestatus.ans_pkt_type == 0x03)
        {
            pkt->head.apdu.fn = 6;//剔除
            
            pkt->head.apdu.ctrl.prm = 1;
            cltor_shadow[k].sendseq++;
            pkt->head.apdu.seq = cltor_shadow[k].sendseq;
        }
        else
        {
            pkt->head.apdu.fn = 4;//登出
        }
            
        pkt->head.apdu.stInfo.stDown.bit1.routeFlg = 0;
        pkt->head.apdu.stInfo.stDown.bit1.nodeFlg = 0;
        pkt->head.apdu.stInfo.stDown.bit1.addrFlg = 0;
        pkt->head.apdu.stInfo.stDown.bit1.conflict = 1;
        pkt->head.apdu.stInfo.stDown.bit1.routeNum = cltor[k].hop - 2;;//pkt->rarea->stDown.bit1.routeNum;
        pkt->head.apdu.stInfo.stDown.bit2.channelFlg = rfpara.rf_channel;//Cal_Hash_Value(nDeviceMacAddr) % LORA_CHANNEL_NUM;//pkt->rarea->stDown.bit2.channelFlg;
        pkt->head.apdu.stInfo.stDown.bit2.errEncodeFlg = 0;
        pkt->head.apdu.addrlen = 0;
        pkt->head.apdu.addr = pkt->data + PKT_HEAD_LEN;

        memcpy(pkt->head.apdu.addr, nDeviceMacAddr, UNIQUE_MAC_ADDR_LEN);//节点唯一标识(队列)	
		ROUTE temprt = Fill_Route(pkt->head.apdu.addr + 6,  MASTER_ROUTE_PATH, k); //接收者地址、路由
		if(temprt.rinfo.rtype != ERROR_ROUTE_PATH)
		{
			//pExtData->route.rdepth = temprt.rinfo.rdepth;
			pkt->head.apdu.stInfo.stDown.bit1.routeNum = temprt.rinfo.rdepth;
			//recvid = temprt.did;
			//pkt->data[7] = 0;
			
            memcpy(pkt->head.apdu.addr + (temprt.rinfo.rdepth + 1) * 6, cltor[k].devAddr, UNIQUE_MAC_ADDR_LEN);//节点唯一标识(队列)

            pkt->head.apdu.addrlen = (temprt.rinfo.rdepth + 2) * 6;

            pkt->head.apdu.data = pkt->data + PKT_HEAD_LEN + pkt->head.apdu.addrlen;
            

            switch(pkt->head.apdu.fn)
            {
            case 3:
            case 7:
                pkt->head.apdu.data[m++] = cltor_shadow[k].nodestatus.result;
                pkt->head.apdu.data[m++] = 0x00;
                pkt->head.apdu.data[m++] = 0x00;
                whiteMeterNum = Record_Num();
                memcpy(pkt->head.apdu.data + m, (uint8 *)&whiteMeterNum, 2);//网络规模
                m+=2;
                
                memcpy(pkt->head.apdu.data + m, (uint8 *)&k, 2);//短地址
                m+=2;

                memset(pkt->head.apdu.data + m, 0, 12);//时间
                m+=12;
                pkt->head.apdu.data[m++] = 0x00;
                pkt->head.apdu.data[m++] = 0x00;
                break;
            case 4:
            case 5:
                pkt->head.apdu.data[m++] = cltor_shadow[k].nodestatus.result;
                break;
            case 6:
                break;
            default:    
                break;
            }
            pkt->head.apdu.len = m;
		}
		else
        {      
			bNotSend = 1;
        }
	}
//	    else if(cltor[k].nodestatus.protocol == PST_FRM_802_R_NO)
//	    {
//	        uint8 m = 0;
//	        memset((uint8 *)&pkt->head.mhr, 0, sizeof(STIEEE));
//	
//	//	        LOG_DEBUG( DBGFMT"Create_JRep ID[%x] rssi[%d] level[%d]\n",DBGARG, k, cltor_shadow[k].rRssi,
//	//	                getRssiQualityByRssi(cltor_shadow[k].rRssi));                        
//	
//	        
//	        pkt->head.mhr.seg_ctrl.ctrl_net.bit3.OC = 0;
//	        pkt->head.mhr.seg_ctrl.ctrl_net.bit2.signalQ = getRssiQualityByRssi(cltor_shadow[k].rRssi);
//	        pkt->head.mhr.seg_ctrl.ctrl_net.bit2.pwr = rfpara.rf_power;
//	        
//	        pkt->head.mhr.frame_ctrl.bit2.prm = 0;
//	        pkt->head.mhr.frame_ctrl.bit1.ftd = 3;
//	        pkt->head.mhr.frame_ctrl.bit1.panid = 1;
//	        pkt->head.mhr.frame_ctrl.bit1.dir = 0;
//	        
//	        pkt->head.mhr.seq = cltor_shadow[k].recvseq;
//	        pkt->head.mhr.srssi = cltor_shadow[k].rRssi;
//	    
//	        pkt->head.mhr.app = pkt->data;
//	
//	        
//	        if(cltor_shadow[k].nodestatus.ans_pkt_type == CON_NODE_UPDATE_LOGIN 
//	            || cltor_shadow[k].nodestatus.ans_pkt_type == CON_NODE_UPDATE_REGISTER)
//	        {
//	            if(cltor_shadow[k].nodestatus.ans_pkt_type == CON_NODE_UPDATE_LOGIN)
//	            {
//	                pkt->head.mhr.fn = 0x02;//入网应答
//	            }
//	            else if(cltor_shadow[k].nodestatus.ans_pkt_type == CON_NODE_UPDATE_REGISTER)
//	            {
//	                pkt->head.mhr.fn = 0x0E;//注册应答
//	            }
//	            
//	            if(cltor[k].addrLen == CON_DEV_ADDR_LEN_8)
//	            {
//	                pkt->head.mhr.frame_ctrl.bit2.dest = 3;
//	            }
//	            else
//	            {
//	                pkt->head.mhr.frame_ctrl.bit2.dest = 1;
//	            }
//	            
//	            if(nDeviceMacAddrLen == CON_DEV_ADDR_LEN_8)
//	            {
//	                pkt->head.mhr.frame_ctrl.bit2.src = 3;
//	            }
//	            else
//	            {
//	                pkt->head.mhr.frame_ctrl.bit2.src = 1;
//	            }
//	            pkt->head.mhr.app[m++] = pkt->head.mhr.fn;
//	            memcpy(pkt->head.mhr.app + m, (uint8 *)&rfpara.shortID, 2);
//	            m+=2;
//	            memcpy(pkt->head.mhr.app + m, (uint8 *)&k, 2);
//	            m+=2;
//	            pkt->head.mhr.app[m++] = 0x18 | cltor_shadow[k].nodestatus.result;//信标允许，设备类型ffd
//	
//	            
//	        }
//	        else if(cltor_shadow[k].nodestatus.ans_pkt_type == CON_NODE_UPDATE_HEARTBEAT)
//	        {
//	            pkt->head.mhr.fn = 0x0F;//心跳 
//	            pkt->head.mhr.frame_ctrl.bit2.dest = 2;
//	            pkt->head.mhr.frame_ctrl.bit2.src = 2;
//	            pkt->head.mhr.app[m++] = pkt->head.mhr.fn;
//	            pkt->head.mhr.app[m++] = 0x18 | cltor_shadow[k].nodestatus.result;//信标允许，设备类型ffd
//	
//	            //pkt->head.apdu.fn = 5;//心跳
//	        }
//	        else if(cltor_shadow[k].nodestatus.ans_pkt_type == 0x03)
//	        {
//	            pkt->head.mhr.fn = 0x03;//离网 
//	            pkt->head.mhr.frame_ctrl.bit2.dest = 2;
//	            pkt->head.mhr.frame_ctrl.bit2.src = 2;
//	            pkt->head.mhr.app[m++] = pkt->head.mhr.fn;
//	            pkt->head.mhr.app[m++] = 0x01;//协调器希望设备断开PAN
//	
//	//	            pkt->head.apdu.fn = 6;//剔除
//	//	            
//	//	            pkt->head.apdu.ctrl.prm = 1;
//	//	            cltor_shadow[k].sendseq++;
//	//	            pkt->head.apdu.seq = cltor_shadow[k].sendseq;
//	        }
//	        else
//	        {
//	//	            pkt->head.apdu.fn = 4;//登出
//	        }  
//	        pkt->head.mhr.len = m;
//	        
//	        uint8 err = IE_Compose_RD(k, ezPkt, &pkt->head.mhr.seg_ctrl, &pkt->head.mhr.frame_ctrl, 
//	                               pkt->head.mhr.seq, pkt->head.mhr.app, pkt->head.mhr.len);
//	    }
	else
    {      
		bNotSend = 1;
    }

		//*********************************************************物理层
	if(0 == bNotSend)
	{
		//cltor[k].sendseq++;
	
		ezPkt->bValid = 1;//业务缓存有效
		ezPkt->nBackOffSlot = 0;
        
//	        LOG_DEBUG( DBGFMT"[%d] wireless send net data ftd = %d, fn = %d seq = %d dest addr %02x%02x%02x%02x%02x%02x\n",DBGARG, 
//	            k, pkt->apdu.ctrl.ftd, pkt->apdu.fn, pkt->apdu.seq, pkt->apdu.addr[11], pkt->apdu.addr[10],pkt->apdu.addr[9],pkt->apdu.addr[8],pkt->apdu.addr[7],pkt->apdu.addr[6]);
        //LOG_DUMPHEX(LOG_LEVEL_DEBUG, "HexData : \n", pkt->data, pkt->len);

        
	}
	else
	{
		//路由错误时的处理
		cltor_shadow[k].nodestatus.bNetAck = 0;
	}
}
//====================================================
void Create_Affirm(uint16 k, EZ_TX_PKT * ezPkt)//推送数据应答包
{
	PKT *pkt;
	uint8 bNotSend = 0;
    uint8 m = 0;

    //LOG_DEBUG( DBGFMT" id = %d Affirm the push data !\n",DBGARG, k);

	pkt = &(ezPkt->TxPkt);
	pkt->ttl = MAX_DATA_TIME_LIVE;//cltor[k].FrameDeadTime;	//生存周期by ben 29
    
    if(cltor[k].nodestatus.protocol == PST_FRM_WL_1_NO)
	{
        //LOG_DEBUG( DBGFMT"Create_Affirm[%d] FrameDeadTime = %d\n",DBGARG, k, cltor[k].FrameDeadTime);

        memset((uint8 *)&pkt->head.apdu, 0, sizeof(STAPDU));
        //传递数据
        pkt->index = k;
        
		pkt->head.apdu.ctrl.dir = 0;
        pkt->head.apdu.ctrl.prm = 0;
        pkt->head.apdu.ctrl.evtmode = 1;
        pkt->head.apdu.ctrl.ftd = 0;
        pkt->head.apdu.seq = cltor_shadow[k].recvseq;
        pkt->head.apdu.srssi = cltor_shadow[k].rRssi;
        
        if(cltor_shadow[k].nodestatus.errCode == 0x00)
        {
            pkt->head.apdu.fn = 1;//确认
        }
        else 
        {
            pkt->head.apdu.fn = 2;//否认
            
        }
        pkt->head.apdu.stInfo.stDown.bit1.routeFlg = 0;
        pkt->head.apdu.stInfo.stDown.bit1.nodeFlg = 0;
        pkt->head.apdu.stInfo.stDown.bit1.addrFlg = 0;
        pkt->head.apdu.stInfo.stDown.bit1.conflict = 1;
        pkt->head.apdu.stInfo.stDown.bit1.routeNum = cltor[k].hop - 2;//pkt->rarea->stDown.bit1.routeNum;
        pkt->head.apdu.stInfo.stDown.bit2.channelFlg = rfpara.rf_channel;//Cal_Hash_Value(nDeviceMacAddr) % LORA_CHANNEL_NUM;//pkt->rarea->stDown.bit2.channelFlg;
        pkt->head.apdu.stInfo.stDown.bit2.errEncodeFlg = 0;
        pkt->head.apdu.addrlen = 0;
        pkt->head.apdu.addr = pkt->data + PKT_HEAD_LEN;

        memcpy(pkt->head.apdu.addr, nDeviceMacAddr, UNIQUE_MAC_ADDR_LEN);//节点唯一标识(队列)	
		ROUTE temprt = Fill_Route(pkt->head.apdu.addr + 6,  MASTER_ROUTE_PATH, k); //接收者地址、路由
		if(temprt.rinfo.rtype != ERROR_ROUTE_PATH)
		{
			//pExtData->route.rdepth = temprt.rinfo.rdepth;
			pkt->head.apdu.stInfo.stDown.bit1.routeNum = temprt.rinfo.rdepth;
			//recvid = temprt.did;
			//pkt->data[7] = 0;
			
            memcpy(pkt->head.apdu.addr + (temprt.rinfo.rdepth + 1) * 6, cltor[k].devAddr, UNIQUE_MAC_ADDR_LEN);//节点唯一标识(队列)

            pkt->head.apdu.addrlen = (temprt.rinfo.rdepth + 2) * 6;

            pkt->head.apdu.data = pkt->data + PKT_HEAD_LEN + pkt->head.apdu.addrlen;
            

            switch(pkt->head.apdu.fn)
            {
            case 0x01:
                pkt->head.apdu.data[m++] = 0xFF;
                pkt->head.apdu.data[m++] = 0xFF;
                
                pkt->head.apdu.data[m++] = 0x00;
                pkt->head.apdu.data[m++] = 0x00;
                break;
            default:   
                pkt->head.apdu.data[m++] = cltor_shadow[k].nodestatus.errCode;
                break;
            }
            pkt->head.apdu.len = m;


		}
		else
			bNotSend = 1;
	}
//	    else if(cltor[k].nodestatus.protocol == PST_FRM_802_R_NO)
//	    {
//	        uint8 m = 0;
//	        memset((uint8 *)&pkt->head.mhr, 0, sizeof(STIEEE));
//	        pkt->head.mhr.seg_ctrl.ctrl_ack.bit3.OC = 0;
//	        pkt->head.mhr.seg_ctrl.ctrl_ack.bit2.rssiQ = getSignalQuality(cltor_shadow[k].rRssi, RSSI_OFFSET_MF, RSSI_OFFSET_HF);
//	
//	        pkt->head.mhr.frame_ctrl.bit2.prm = 0;
//	        pkt->head.mhr.frame_ctrl.bit1.ftd = 2;
//	        pkt->head.mhr.frame_ctrl.bit1.panid = 1;
//	        pkt->head.mhr.frame_ctrl.bit1.dir = 0;
//	        pkt->head.mhr.frame_ctrl.bit2.version = 2;
//	        
//	        pkt->head.mhr.seq = cltor_shadow[k].recvseq;
//	        pkt->head.mhr.srssi = cltor_shadow[k].rRssi;
//	    
//	        pkt->head.mhr.app = pkt->data;
//	        pkt->head.mhr.afn = 0;
//	        if(cltor_shadow[k].nodestatus.errCode == 0x00)
//	        {
//	            pkt->head.mhr.fn = 0x01;//确认
//	        }
//	        else
//	        {
//	            pkt->head.mhr.fn = 0x02;//否认
//	        }
//	        
//	
//	        pkt->head.mhr.frame_ctrl.bit2.dest = 2;
//	        pkt->head.mhr.frame_ctrl.bit2.src = 2;
//	        
//	        pkt->head.mhr.app[m++] = (pkt->head.mhr.afn << 4) | (pkt->head.mhr.fn);
//	
//	        switch(pkt->head.mhr.fn)
//	        {
//	        case 0x01:
//	            break;
//	        default:   
//	            pkt->head.mhr.app[m++] = cltor_shadow[k].nodestatus.errCode;//信标允许，设备类型ffd
//	            break;
//	        }
//	        //pkt->head.mhr.app[m++] = 0x18 | cltor[k].nodestatus.result;//信标允许，设备类型ffd
//	        pkt->head.mhr.len = m;
//	        
//	        uint8 err = IE_Compose_RD(k, ezPkt, &pkt->head.mhr.seg_ctrl, &pkt->head.mhr.frame_ctrl, 
//	                               pkt->head.mhr.seq, pkt->head.mhr.app, pkt->head.mhr.len);
//	
//	        if(err != SYS_ERR_OK)
//	        {
//	            bNotSend = 1;
//	        }
//	    }
	else
    {   
	    bNotSend = 1;
    }

		//*********************************************************物理层
	if(0 == bNotSend)
	{
		//cltor[k].sendseq++;
	
		ezPkt->bValid = 1;//业务缓存有效
		ezPkt->nBackOffSlot = 0;
	}
	else
	{
		//路由错误时的处理
		cltor_shadow[k].nodestatus.bDataAck = 0;
	}
}


//	//=======================================================
//	void Erase_FM_SN(void)//擦除铁电中的SN记录
//	{
//	
//	
//	     uint8 p=0xff;
//	     uint16 j ;
//		for(j=0;j<100;j++)
//		{
//		I2cWrite( 0xA0,&p, (0x1e00+j), 1);//E2PROM参数清零(第一次上电是擦写)	
//		}
//		
//	}
//=======================================================
#if 0
#define RF_CLOCK_CORRECT_COUNT    0xF4240//0x7A1200   //8Mhz校准
void Correct_RF_Clock(uint8 set_para,uint8 accyracy)//射频时钟校准
{
	uint8 gap;
	uint16 cnt=0;
	uint8 para;
	
	Set_Crastyle(set_para); // 设置晶振电容补偿值
       
	VIC_DisableInt(1 << VIC_TIMER0); // 关T0中断

        rf_clk=1;
        // Clear interrupts flags
    T0IR=0xFF;
    // Disable counting
    T0TCR=0;
    // Clear timer counter
    T0TC=0;
    // PR = Prescaler - 1
    T0PR= 0;
    // Clear prescaler timer counter
    T0PC=0;
    // Reset Compare modules
    T0MCR=0;
    T0MR0=0;
    T0MR1=0;
    T0MR2=0;
    T0MR3=0;
    // Reset Capture modules
    T0CCR=0;
    // Reset External Compare module
	T0EMR=0;
	VIC_DisableInt(1 << VIC_TIMER1); // 关闭定时器中断
	VIC_DisableInt(1 << VIC_EINT3); // 关闭RF中断
	VIC_DisableInt(1 << VIC_UART0); // 关闭串口中断
	VIC_DisableInt(1 << VIC_I2C1); // 关闭I2C中断
    T1TCR=0x0;//禁能
	T0TCR=0x0;//禁能
/*	PINSEL1_bit.P0_30 = 0x0;//p0.30置成IO口
	FIO0DIR &= (~(1<<30)); //p0.30设置为输入
	
	PINSEL1_bit.P0_29 = 0x2;// CAP0.0 TIMER0  外部校准源
	T0CTCR=0xd;//计数器模式、上升沿　CAP0.0 
	T0PR=0;//  15mhz时钟源
	T0TC=0;//计数清零
	T0MR0=0xF4240;//1mhz 计数
	T0MCR=0x1;//匹配时产生中断
	
	PINSEL0_bit.P0_10 = 0x2;//CAP1.0 TIMER1   射频时钟
	T1CTCR=0x1;//计数器模式、上升沿　CAP1.0 
	T1PR=0;//  15mhz时钟源
	T1TC=0;//计数清零	
	T1MCR=0x0;//匹配时不产生中断
*/
	PINSEL3_bit.P1_25 = 0x0;//p1.25置成IO口
	FIO1DIR &= (~(1<<25)); //p1.25设置为输入
	
	PINSEL3_bit.P1_26 = 0x3;// CAP0.0 TIMER0  外部校准源
	T0CTCR=0x1;//计数器模式、上升沿　CAP0.0 
	T0PR=0;//  8mhz时钟源,分频后1Mhz
	T0TC=0;//计数清零
	T0MR0=0x7A1200;//RF_CLOCK_CORRECT_COUNT;//8mhz 计数
	T0MCR=0x1;//匹配时产生中断
	
	PINSEL3_bit.P1_18 = 0x3;//CAP1.0 TIMER1   射频时钟
	T1CTCR=0x1;//计数器模式、上升沿　CAP1.0 
	T1PR=0;//  15mhz时钟源
	T1TC=0;//计数清零	
	T1MCR=0x0;//匹配时不产生中断

	para = SpiReadRegister(SI4432_MICROCONTROLLER_OUTPUT_CLOCK);
	 VIC_EnableInt(1 << VIC_TIMER0); // 开T0中断

	T0TCR=0x1;//使能
	T1TCR=0x1;//使能
	
	while(!mainflag);
	while((timer0_cnt - RF_CLOCK_CORRECT_COUNT)>accyracy||((RF_CLOCK_CORRECT_COUNT - timer0_cnt)>accyracy))//如果不满足精度要求，一直处理
	{
		WDTFeed();
		if(timer0_cnt > (RF_CLOCK_CORRECT_COUNT + 1))
		{
			gap=timer0_cnt - RF_CLOCK_CORRECT_COUNT;
			cnt=set_para+gap;
			if(cnt>0xff)
			{
#ifdef DEBUG_PRINTF
                //printf("Correct_RF_Clock failed  timer0_cnt = %d set_para = %d gap = %d\n", timer0_cnt, set_para, gap);
#endif			
				set_para=0xff;
				Set_Crastyle(set_para); // 设置晶振电容补偿值				
				I2cWrite(0xA0, (uint8*) &set_para, FM_CAP_MEND, 1); //写入校准值
				UART_PutArray(UART0, informpkt, 0x13); //返回校准值
				//延时200MS
				Delay_ms(200);			
				Reset();
			}
			else
				set_para=set_para+gap;//补偿			
#ifdef DEBUG_PRINTF
            //printf("Correct_RF_Clock ingggg timer0_cnt = %d set_para = %d gap = %d\n", timer0_cnt, set_para, gap);
#endif					
			Set_Crastyle(set_para); // 设置晶振电容补偿值
			Delay_us(3000);//延时3ms
			T1TCR=0x1;//使能
			T0TCR=0x1;//使能	
			mainflag=0;
			while(!mainflag);
		}	

		if(timer0_cnt < (RF_CLOCK_CORRECT_COUNT-1))
		{	
			gap=RF_CLOCK_CORRECT_COUNT-timer0_cnt;
			set_para=set_para-gap;//补偿		
			Set_Crastyle(set_para); // 设置晶振电容补偿值
			Delay_us(3000);//延时3ms
			T1TCR=0x1;//使能
			T0TCR=0x1;//使能
		 
			mainflag=0;
			while(!mainflag);
		}
		
		if(((timer0_cnt-RF_CLOCK_CORRECT_COUNT) <= accyracy) || ((RF_CLOCK_CORRECT_COUNT - timer0_cnt) <= accyracy))//如果满足精度要求
		{
			rf_clk=0;

			para = set_para;
#ifdef EN_TEMPERAUREDEGC
			uint8 temp8;
			I16 temp16;
			temp8=Crystal_Value_Table[macSpiReadReg(SI4432_ADC_VALUE)/5];
			temp16=para;
			if(temp8&0x80) 
			{
				temp16+=(temp8&0x7f);
				if(temp16>0xff)  	
					temp16=0xff;
			}
			else
			{
				temp16-=temp8;
				if(temp16<0) 		
					temp16=0x00;
			}
			para=(temp16&0xff);
#endif

			I2cWrite(0xA0, (uint8*) &para, FM_CAP_MEND, 1); //写入校准值
			informpkt[14]=para;
			informpkt[17]=Check_SUM(informpkt+3, 14);
			UART_PutArray(UART0, informpkt, 0x13); //确认回答
			//延时200MS
			Delay_ms(200);		
			Reset();	 
		}
	}
	
	 VIC_DisableInt(1 << VIC_TIMER0); // 关T0中断
}
#endif
//=================================================================
void Delay_us(uint16 us)
{   
        uint16 i;

	for(i=0;i<(5*us);)
	{i++;}
}
//=================================================================
void Delay_ms(uint16 ms)
{  
	uint16 i;

	for(i=0;i<ms;i++)
	{
	Delay_us(1000);
	}
}
//=================================================================

//=================================================================
void Cltor_init(void)//初始化SN
{
	uint16 i;
	uint8* pData = (uint8 *)shadow_space;

	memset(cltor, 0, sizeof(cltor));
	for(i=0; i<MAX_SUP_SS_NUM + 1; i++)	
	{
//			cltor[i].sn.dw[0] = 0xffff;
//			cltor[i].sn.dw[1] = 0xffff;
//	        cltor[i].sn.dw[2] = 0xffff;   
        cltor[i].addrLen = 0;
        memset(cltor[i].devAddr, 0xff, CON_DEV_ADDR_LEN_10);
        cltor_shadow[i].father_update = 0;
//	        cltor_shadow[i].mother_update = 0;

		//cltor[i].nUpdataBlockNum = MAX_SS_UPDATA_BLOCK_NUM;
		//cltor[i].nSSBaseUpdataBlockNum = MAX_I_SS_UPDATA_BLOCK_NUM;
			
/*		cltor[i].nod.ans_pkt_type=0;
		cltor[i].nod.fixed=0;
		cltor[i].nod.handled=0;
		cltor[i].nod.needack=0;	*/
	} 
	for(i=0;i<32;i++)
	{            
		I2cRead(0xA0, &(pData[i*32]), i*32, 32); //从E2PROM中获取电表状态信息
	}

}


/******************************************************************************
*遍历电表
*
******************************************************************************/ 
void Search_Addr(uint8 flag)
{
	uint16 i,un_handle=0;//未处理电表数量
	uint16 exist_addr=0;//当前有效电表数量
	uint8 up_pkt[21]={0};
					
	  
//	if(HASH_Table_Used==0)
//	{
//		for (i =0 ; i <MAX_HASH_TABLE_FLASH_SIZE; i++)
//        {
//            if ((htable1[i].status == OCCUPIED) &&(htable1[i].addr[0]!= 0xff)) //如果该哈希元素的值有效              
//            {
//                exist_addr++;//记录有效电表数
//                if(shadow_space[i].handled==UN_HANDLED)	
//                    un_handle++;//记录未处理的数量			                    
//            }			      			
//        }	
//	}
//	else
//	{
//		for (i =0 ; i <MAX_HASH_TABLE_FLASH_SIZE; i++)
//	        {
//                if ((htable2[i].status == OCCUPIED) &&(htable2[i].addr[0]!= 0xff) )//如果该哈希元素的值有效              
//                {
//    				exist_addr++;//记录有效电表数
//    				if(shadow_space[i].handled==UN_HANDLED)
//    				un_handle++;//记录未处理的数量		                    
//                }		      			
//	        }	
//	  	 
//	}
//
//	up_pkt[0]=0x68;
//	up_pkt[1]=19;
//	up_pkt[2]=0;
//	up_pkt[3]=0x8a;
//	up_pkt[4]=0x1;	
//	up_pkt[5]=0;
//	up_pkt[6]=0x50;
//	up_pkt[7]=0;
//	up_pkt[8]=0;
//	up_pkt[9]=0;
//	if((bDebugSend) && (0 == bEZMACDebugType))
//	{
//		up_pkt[10]=0xF0;
//		up_pkt[12]=0x03;
//		if(flag==0)
//			up_pkt[11]=0x08;
//		else
//			up_pkt[11]=0x80;			
//	}
//	else
//	{
//		up_pkt[10]=0x3;
//		up_pkt[12]=0x1;
//		if(flag==0)
//			up_pkt[11]=0x04;
//		else
//			up_pkt[11]=0x40;		
//	}
//	up_pkt[13]=un_handle;
//	up_pkt[14]=un_handle>>8;//未处理电表数量
//	up_pkt[15]=exist_addr;
//	up_pkt[16]=exist_addr>>8;//当前有效电表数量	 
//	up_pkt[17]= Check_SUM(up_pkt + 3, 14); //校验位
//	up_pkt[18]=0x16;
//	if(bEZMACDebugType)
//		SendPacketToEzmacProc(pEZMACRecvPacket, up_pkt, 19, 0);
//	else					
//		UART_PutArray(UART0, up_pkt, 19); // 回答
}
#if 0
/******************************************************************
*上报未处理电表地址
*
*********************************************************************/ 
void Report_Unhandled_Addr(uint8 *pkt)
{
	uint8 h,len,handled=0;//request_no=0;
	int hash=0;
	uint8 report_no=0;
	uint8 addr[60]={0};
	uint8 up_pkt[200]={0};
	uint16 i;

	len=*(pkt+1);//报文长度
//----------------------标记已处理的电表--------------------
	if(len>0x10)//有已处理电表
	{
		handled=(len-0x10)/6;//已处理电表数量
		for(h=0;h<handled;h++)//标记已处理的电表
		{
			memcpy(addr,pkt+13+(6*h),6);//已处理电表地址					 
//			hash = Hash_Table_Search(addr); //计算得到哈希值

			while(hash != ( - 1))//判断 有效性
			{
				if(HASH_Table_Used==0)
				{
					if((htable1[hash].status==OCCUPIED)&&(shadow_space[hash].handled==UN_HANDLED))//有效且未处理
					{
						Record_Handled_Addr( hash) ;//将电表状态改为已处理
						break;
					}
					else
						hash++;
					if(hash==MAX_HASH_TABLE_FLASH_SIZE)
						hash=-1;
				}
				else
				{
					if((htable2[hash].status==OCCUPIED)&&(shadow_space[hash].handled==UN_HANDLED))//有效且未处理
					{
						Record_Handled_Addr( hash) ;
						break;
					}
					else
						hash++;
					if(hash==MAX_HASH_TABLE_FLASH_SIZE)
						hash=-1;
				}				
			}			       
		}
	}
// request_no=*(pkt+len-3);//请求电表个数

 
//-------------------上报未处理的电表----------------------------
//遍历直至找到有未处理的电表
	if(HASH_Table_Used==0)
	{
		for(i=0;i<MAX_HASH_TABLE_FLASH_SIZE;i++)
		{	
			if((htable1[i].status==OCCUPIED)&&(shadow_space[i].handled==UN_HANDLED))//找到未处理的有效电表			 		
			{
				//组包
				memcpy(up_pkt+14+(report_no*20),htable1[i].addr,6);//电表地址
//					memcpy(up_pkt+20+(report_no*20),htable1[i].falsh_sn,6);//SN地址
				//	*(up_pkt+20+(report_no*20) + 5) = 0;

				memcpy(up_pkt+26+(report_no*20),addr,6);//局号地址(无)
				up_pkt[32+(report_no*20)]=htable1[i].SubType;//规约类型(暂时写死)
				up_pkt[33+(report_no*20)]=0x1;//添加电表
				report_no++;				
				if(report_no==9)
				{
					i=MAX_HASH_TABLE_FLASH_SIZE;							 
				}
			}	 
		}
	}
	else
	{
		for(i=0;i<MAX_HASH_TABLE_FLASH_SIZE;i++)
		{
			if((htable2[i].status==OCCUPIED)&&(shadow_space[i].handled==UN_HANDLED))//找到未处理的有效电表			
			{
				//组包
				memcpy(up_pkt+14+(report_no*20),htable2[i].addr,6);//电表地址
//					memcpy(up_pkt+20+(report_no*20),htable2[i].falsh_sn,6);//SN地址
				//	*(up_pkt+20+(report_no*20) + 5) = 0;

				memcpy(up_pkt+26+(report_no*20),addr,6);//局号地址(无)
				up_pkt[32+(report_no*20)]=htable2[i].SubType;//规约类型(暂时写死)
				up_pkt[33+(report_no*20)]=0x1;//添加电表
				report_no++;
				if(report_no==9)
				{
					i=MAX_HASH_TABLE_FLASH_SIZE;							 
				}
			}   
		}
	}
//-------------------组上行报文------------------------
	up_pkt[0]=0x68;
	up_pkt[1]=16+(report_no*20);
	up_pkt[2]=0;
	up_pkt[3]=0x8a;
	up_pkt[4]=0x1;
	up_pkt[5]=0;
	up_pkt[6]=0x50;
	up_pkt[7]=0;
	up_pkt[8]=0;
	up_pkt[9]=0;
	
	if((bDebugSend) && (0 == bEZMACDebugType))
	{
		up_pkt[10]=0xF0;
		up_pkt[11]=0x10;
		up_pkt[12]=0x03;
	}
	else
	{
		up_pkt[10]=0x3;
		up_pkt[11]=0x8;
		up_pkt[12]=0x1;
	}
	up_pkt[13]=report_no;//电表数量
	up_pkt[14+(report_no*20)]= Check_SUM(up_pkt + 3, 11+(report_no*20)); //校验位
	up_pkt[15+(report_no*20)]=0x16;

	if(bEZMACDebugType)
		SendPacketToEzmacProc(pEZMACRecvPacket, up_pkt, 16+(report_no*20), 0);
	else		
		UART_PutArray(UART0, up_pkt, 16+(report_no*20)); // 回答
}

/******************************************************************
*查询采集器信息
*
******************************************************************/ 
void Search_SS(uint8 *data)
{
	uint8 len;
    uint16 i,j,id;
	uint8 handled_no;//已处理采集数量
	uint8 request_no;//请求数量
	uint8 report_no=0;//上报数量
	uint8 up_pkt[200]={0};
	len=*(data+1);//包长度
	request_no=*(data+len-3);
	if(*(data+13)==1)//清除已处理标识位
	{
		for(i=SUP_SS_INDEX_START;i<MAX_SUP_SS_NUM+1;i++)
		cltor[i].nodestatus.handled= UN_HANDLED;
	}
	
	if(len>16)//标记已处理采集器
	{
		handled_no=(len-16)/6;
		for(i=0;i<handled_no;i++)
		{
			for(id=SUP_SS_INDEX_START;id<MAX_SUP_SS_NUM+1;id++)
			{
			//	if(!LPC_Memory_CMP(cltor[id].devAddr,data+14+i*6,5))
				if(!LPC_Memory_CMP(cltor[id].devAddr, data+14+i*6, 6))
				{
					cltor[id].nodestatus.handled=HANDLED;
				}
			}
		
		}
	}
	
	
		for(j=SUP_SS_INDEX_START;j<MAX_SUP_SS_NUM+1;j++)
		{
			if((cltor[j].nodestatus.handled==UN_HANDLED)&&( cltor[j].devAddr[0]<=0x99))//如果未处理且该SN被占用
			{
			//	 if((cltor[j].nod.NetStatus== NODE_STATUS_LOGIN)&& (cltor[j].hop >= 2))//如果入网，则添加
				{
				memcpy(up_pkt+14+(report_no*10),cltor[j].devAddr,6);//采集器地址
				//去掉SN的最高字节
			//	memcpy(up_pkt+14+(report_no*10),cltor[j].devAddr,5);//采集器地址	
			//	*(up_pkt+14+(report_no*10 + 5)) = 0;
    			STMETERPARAFLASH stMeter;
                GetCltorPara(j, &stMeter);                        

				*(up_pkt+20+(report_no*10))=j;//短号
				*(up_pkt+21+(report_no*10))=0;//短号
				*(up_pkt+22+(report_no*10))=1;//添加
				*(up_pkt+23+(report_no*10))=stMeter.softver;//版本号
					 
				report_no++; 
				}
			}

			 if(report_no==request_no)
			 break;
			 
		}

	up_pkt[0]=0x68;
	up_pkt[1]=16+(report_no*10);
	up_pkt[2]=0;
	up_pkt[3]=0x8a;
	up_pkt[4]=0x1;
	up_pkt[5]=0;
	up_pkt[6]=0x50;
	up_pkt[7]=0;
	up_pkt[8]=0;
	up_pkt[9]=0;

	if((bDebugSend) && (0 == bEZMACDebugType))
	{
		up_pkt[10]=0xF0;
		up_pkt[11]=0x20;
		up_pkt[12]=0x03;
	}
	else
	{
		up_pkt[10]=0x3;
		up_pkt[11]=0x10;
		up_pkt[12]=0x1;
	}
	up_pkt[13]=report_no;//采集器数量

	up_pkt[14+(report_no*10)]= Check_SUM(up_pkt + 3, 11+(report_no*10)); //校验位
	up_pkt[15+(report_no*10)]=0x16;
	if(bEZMACDebugType)
		SendPacketToEzmacProc(pEZMACRecvPacket, up_pkt, 16+(report_no*10), 0);
	else			
		UART_PutArray(UART0, up_pkt, 16+(report_no*10)); //回答
}

/********************************************************************
*上报32字节采集器状态
*
********************************************************************/
void Check_SS_Status(void)
{
	uint8 x=0,y=0;
	uint8 up_pkt[47]={0};
    uint16 i;
	for(i=SUP_SS_INDEX_START;i<MAX_SUP_SS_NUM+1;i++)
	{
		x = 32 - (i / 8);
		y = i % 8;
		 
		if((cltor[i].nodestatus.NetStatus == NODE_STATUS_LOGIN)&& (cltor[i].hop >= 2))//如果入网

		{		
		    up_pkt[12+x]=up_pkt[12+x]|(1<<y);//该位置1
		}
		else//离网
		{
		    up_pkt[12+x]=up_pkt[12+x]&(~(1<<y));//该位置0
		}
		
	}
	up_pkt[0]=0x68;
	up_pkt[1]=47;
	up_pkt[2]=0;
	up_pkt[3]=0x8a;
	up_pkt[4]=0x1;
	up_pkt[5]=0;
	up_pkt[6]=0x50;
	up_pkt[7]=0;
	up_pkt[8]=0;
	up_pkt[9]=0;
	if((bDebugSend) && (0 == bEZMACDebugType))
	{
		up_pkt[10]=0xF0;
		up_pkt[11]=0x40;
		up_pkt[12]=0x03;
	}
	else
	{
		up_pkt[10]=0x3;
		up_pkt[11]=0x20;
		up_pkt[12]=0x1;
	}
	up_pkt[45]= Check_SUM(up_pkt + 3, 42); //校验位
	up_pkt[46]=0x16;

	if(bEZMACDebugType)
		SendPacketToEzmacProc(pEZMACRecvPacket, up_pkt, 47, 0);
	else			
		UART_PutArray(UART0, up_pkt, 47); //肯定回答
}
/****************************************************************
* 获取已处理电表  _交互指令
*
*****************************************************************/
void Report_Haneled_Addr(uint16 handled_no)
{
	uint8 *p;
	uint8 report_no=0;
	uint16 inside_handled=0,i;
	p=(uint8 *)updata_code_cache;
	if(HASH_Table_Used==0)
	{
		for(i=0;i<MAX_HASH_TABLE_FLASH_SIZE;i++)
		{
			if((htable1[i].status==OCCUPIED)&&(shadow_space[i].handled==HANDLED))//找到已处理的有效电表	
			 
			
				{
					inside_handled++;
					
					if(inside_handled>=handled_no)
					{
						memcpy(p+14+(report_no*20),htable1[i].addr,6);//电表地址
//							memcpy(p+20+(report_no*20),htable1[i].falsh_sn,6);//SN地址
					//	*(p+20+(report_no*20) + 5) = 0;
						
					 	memset(p+26+(report_no*20),0xff,6);//局号地址(无)
					 	p[32+(report_no*20)]=htable1[i].SubType;//规约类型(暂时写死)
					 	p[33+(report_no*20)]=0x1;//添加电表				
						report_no++;//上报个数
						if(report_no==10)
							break;			 
					}
				
				
				 
			}   

		}
	}
	else
	{
		for(i=0;i<MAX_HASH_TABLE_FLASH_SIZE;i++)
		{
			
			if((htable2[i].status==OCCUPIED)&&(shadow_space[i].handled==HANDLED))//找到已处理的有效电表	
				{
					inside_handled++;
					
					if(inside_handled>=handled_no)
					{
					memcpy(p+14+(report_no*20),htable2[i].addr,6);//电表地址
//						memcpy(p+20+(report_no*20),htable2[i].falsh_sn,6);//SN地址
				//	*(p+20+(report_no*20) + 5) = 0;
					
				 	memset(p+26+(report_no*20),0xff,6);//局号地址(无)
				 	p[32+(report_no*20)]=htable2[i].SubType;//规约类型(暂时写死)
				 	p[33+(report_no*20)]=0x1;//添加电表				
					
					report_no++;//上报个数
					if(report_no==10)
						break;	
					 
					}	
			}   

		}
	}
	 
	p[0]=0x68;
	p[1]=16+(report_no*20);
	p[2]=0;
	p[3]=0x8a;
	p[4]=0x1;
	p[5]=0;
	p[6]=0x50;
	p[7]=0;
	p[8]=0;
	p[9]=0;

	if((bDebugSend) && (0 == bEZMACDebugType))
	{
		p[10]=0xF0;
		p[12]=0x04;
		p[11]=0x01;
	}
	else
	{
		p[10]=0x3;
		p[11]=0x80;
		p[12]=0x1;
	}
	p[13]=report_no;//上报电表数量
	p[14+(report_no*20)]= Check_SUM(p + 3, 11+(report_no*20)); //校验位
	p[15+(report_no*20)]=0x16;

	if(bEZMACDebugType)
		SendPacketToEzmacProc(pEZMACRecvPacket, p, 16+(report_no*20), 0);
	else			
		UART_PutArray(UART0, p, 16+(report_no*20)); // 回答

}
#endif
void MeterSearchMng()
{
//	uint16 loop;
//	uint16 temp =0;
//	HASHT* pHasht; 
//	uint8* pData;
//	uint16 SendNum = 0;
//	uint8* pMeterData ;
//    uint16 m = 0;
//    uint8 ml;
//    uint16 id = 0;
//	COLLECTOR* pCltor;
//		if(0 == HASH_Table_Used)
//			pHasht = htable1;
//		else
//			pHasht = htable2;
//		
//		pData = SendMeterpkt.data;							
//		if(bMeterSearchStart)
//		{
//			if(0 == bWaitForSendMeterAck)
//			{
//				SendNum = 0;
//				for(loop = 0; loop < MAX_HASH_TABLE_FLASH_SIZE; loop++)
//				{
//				    ml = 0;
//					if((MeterStatus[loop].bReceive) && (0 == MeterStatus[loop].bWaitAck))
//					{
//						if(bBroadMeterEnable)
//						{
//							pMeterData = pData + 14 + m;
//							memcpy(pMeterData + ml, pHasht[loop].addr, 6);	
//	                        ml+=6;
//							pMeterData[ml++] = pHasht[loop].SubType;
//	                        id = Sn_Search_Id(pHasht[loop].addr);
//							memcpy(pMeterData + 7, (uint8 *)&id, 2);
//	                        ml+=2;
//	                        pMeterData[ml++] = pHasht[loop].devType;//电表
//	                        pMeterData[ml++] = 0x0;//从节点下接从节点数量
//	                        pMeterData[ml++] = 0x0;//本报文传输的从节点m
//						}
//	//						else
//	//						{
//	//							pMeterData = pData + 14 + SendNum * 13;
//	//							memcpy(pMeterData, pHasht[loop].addr, 6);				
//	//							pMeterData[6] = pHasht[loop].SubType;
//	//							memcpy(pMeterData + 7, pHasht[loop].falsh_sn, 6);
//	//							
//	//							//采集器最高位不上送
//	//						//	*(pMeterData + 7 + 5) = 0;
//	//						}
//	                    m+=ml;
//						MeterStatus[loop].bWaitAck = 1;
//						SendNum ++;							
//						if(SendNum > 15)
//							break;
//					}
//				}
//	
//				if(SendNum > 0)
//				{
//				    
//	                SendMeterpkt.len = 16 + m;
//	//					if(bBroadMeterEnable)
//	//					{
//	//						SendMeterpkt.len = 16 + m;
//	//						pData[11] = 0x01;
//	//					}
//	//					else
//	//					{
//	//						SendMeterpkt.len = 16 + m;
//	//						pData[11] = 0x04; //数据单元标识F3上报电表信息
//	//					}				
//					pData[0] = START_FLAG;
//					pData[1] = SendMeterpkt.len; //长度
//					pData[2] = 0x00;
//					pData[3] = 0xCA; //控制域
//					pData[4] = 0x00; //信息域
//					pData[5] = 0x00;
//					pData[6] = 0x50;
//					pData[7] = 0x00;
//					pData[8] = 0x00;
//					pData[9] = bSendMeterSearchSeq = cltor_shadow[1].sendseq++;
//					pData[10] = 0x06; //AFN
//					pData[11] = 0x08;
//					pData[12] = 0x00;
//					pData[13] = SendNum;
//					
//					pData[SendMeterpkt.len  - 2] = Check_SUM(SendMeterpkt.data + 3, SendMeterpkt.len - 5); //校验位
//					pData[SendMeterpkt.len  - 1] = END_FLAG;
//					UART_PutArray(UART0, pData, SendMeterpkt.len); 
//					SendMeterpkt.deadtime = curslottime + UART_PKT_TIMEOUT;
//					ReSendCount = 0;
//					bWaitForSendMeterAck = 1;
//				}
//			}
//			else
//			{
//				if(SendMeterpkt.deadtime < curslottime)
//				{
//					if(ReSendCount > 5)
//					{
//						//重发次数大于6次的处理
//						bWaitForSendMeterAck = 0;
//					}
//					else
//					{
//						UART_PutArray(UART0, SendMeterpkt.data, SendMeterpkt.len); 
//						SendMeterpkt.deadtime =  curslottime + UART_PKT_TIMEOUT;
//						ReSendCount ++;				
//					}
//				}
//			}
//		}
//		else
//		{
//			if(bMeterDeadStart)
//			{
//				if(bNeedDeleteMeter) 
//				{
//					if(0 == bNeedMeterDeleteAck)
//					{
//						SendNum = 0;
//						for(loop = 0; loop < MAX_HASH_TABLE_FLASH_SIZE; loop++)
//						{
//							if((MeterStatus[loop].bNeedDelete) && (0 == MeterStatus[loop].bDeleteWaitAck))
//							{	
//								pMeterData = pData + 14 + SendNum * 13;
//								memcpy(pMeterData, pHasht[loop].addr, 6);
//								pMeterData[6] = pHasht[loop].SubType;
//								memset(pMeterData + 7, 0, 6);
//	
//								MeterStatus[loop].bDeleteWaitAck = 1;
//								SendNum++;
//								if(SendNum > 15)
//									break;					
//							}
//						}
//					
//						if(SendNum > 0)
//						{
//							SendMeterpkt.len = 16 + SendNum * 13;
//							pData[0] = START_FLAG;
//							pData[1] = SendMeterpkt.len; //长度
//							pData[2] = 0x00;
//							pData[3] = 0xCA; //控制域
//							pData[4] = 0x00; //信息域
//							pData[5] = 0x00;
//							pData[6] = 0x50;
//							pData[7] = 0x00;
//							pData[8] = 0x00;
//							pData[9] = 0x00;
//							pData[10] = 0x06; //AFN
//							pData[11] = 0x04; //数据单元标识F3上报电表信息
//							pData[12] = 0x00;
//							pData[13] = SendNum;
//							
//							pData[SendMeterpkt.len  - 2] = Check_SUM(SendMeterpkt.data + 3, SendMeterpkt.len - 5); //校验位
//							pData[SendMeterpkt.len  - 1] = END_FLAG;
//							UART_PutArray(UART0, pData, SendMeterpkt.len); 
//							SendMeterpkt.deadtime = curslottime + UART_PKT_TIMEOUT;
//							ReSendCount = 0;
//							bNeedMeterDeleteAck = 1;
//						}
//						else
//						{
//							bNeedDeleteMeter = 0;
//							bNeedMeterDeleteAck = 0;
//						}
//					}
//					else
//					{
//						if(SendMeterpkt.deadtime < curslottime)
//						{
//							if(ReSendCount > 5)
//							{
//								//重发次数大于6次的处理
//								bNeedMeterDeleteAck = 0;
//								for(loop = 0; loop < MAX_HASH_TABLE_FLASH_SIZE; loop++)
//								{
//									if((MeterStatus[loop].bNeedDelete) && (MeterStatus[loop].bDeleteWaitAck))
//									{						
//										Updata_Hash_Table(loop, NULL);    //删除电表
//										MeterStatus[loop].bNeedDelete = 0;
//										MeterStatus[loop].bDeleteWaitAck = 0;
//									}
//								}						
//							}	
//							else
//							{
//								UART_PutArray(UART0, SendMeterpkt.data, SendMeterpkt.len); 
//								SendMeterpkt.deadtime =  curslottime + UART_PKT_TIMEOUT;
//								ReSendCount ++;							
//							}
//						}
//					}
//				}
//	//				else
//	//				{
//	//					if(bCltorSearchStart)
//	//					{					
//	//						uint8 bFound;
//	//						for(temp = 2; temp <= rfpara.rf_slotnum; temp++)
//	//						{
//	//							pCltor = &(cltor[temp]);
//	//							pCltor->nodestatus.NoMeterSendStatus = 0;
//	//							bFound = 0;
//	//							if((pCltor->nodestatus.NetStatus == NODE_STATUS_LOGIN) && (pCltor->devAddr[0] < 0x99))
//	//							{
//	//								for(loop = 0; loop < MAX_HASH_TABLE_FLASH_SIZE; loop++)
//	//								{
//	//									HASHT *pMeter = &(pHasht[loop]);	
//	//									if (pMeter->status == OCCUPIED && (!LPC_Memory_CMP(pMeter->falsh_sn, pCltor->devAddr, 6)))
//	//									{
//	//										bFound = 1;
//	//										break;
//	//									}
//	//								}
//	//								if(0 == bFound)
//	//									pCltor->nodestatus.NoMeterSendStatus = 1;
//	//							}
//	//						}
//	//						bCltorSearchStart = 0;
//	//					}
//	//					
//	//					if(0 == bNeedMeterDeleteAck)
//	//					{
//	//						SendNum = 0;
//	//						for(temp = 2; temp <= rfpara.rf_slotnum; temp++)
//	//						{
//	//							pCltor = &(cltor[temp]);
//	//							if(1 == pCltor->nodestatus.NoMeterSendStatus)
//	//							{	
//	//								pMeterData = pData + 14 + SendNum * 13;
//	//								memset(pMeterData, 0xee, 6);
//	//								pMeterData[6] = 0;
//	//								memcpy(pMeterData + 7, pCltor->devAddr, 6);
//	//	
//	//								//采集器最高位不上送
//	//							//	*(pMeterData + 7 + 5) = 0;
//	//						
//	//								pCltor->nodestatus.NoMeterSendStatus = 0;
//	//								SendNum++;
//	//								if(SendNum > 15)
//	//									break;					
//	//							}
//	//						}
//	//					
//	//						if(SendNum > 0)
//	//						{
//	//							SendMeterpkt.len = 16 + SendNum * 13;
//	//							pData[0] = START_FLAG;
//	//							pData[1] = SendMeterpkt.len; //长度
//	//							pData[2] = 0x00;
//	//							pData[3] = 0xCA; //控制域
//	//							pData[4] = 0x00; //信息域
//	//							pData[5] = 0x00;
//	//							pData[6] = 0x50;
//	//							pData[7] = 0x00;
//	//							pData[8] = 0x00;
//	//							pData[9] = 0x00;
//	//							pData[10] = 0x06; //AFN
//	//							pData[11] = 0x04; //数据单元标识F3上报电表信息
//	//							pData[12] = 0x00;
//	//							pData[13] = SendNum;
//	//							
//	//							pData[SendMeterpkt.len  - 2] = Check_SUM(SendMeterpkt.data + 3, SendMeterpkt.len - 5); //校验位
//	//							pData[SendMeterpkt.len  - 1] = END_FLAG;
//	//							UART_PutArray(UART0, pData, SendMeterpkt.len); 
//	//							SendMeterpkt.deadtime = curslottime + UART_PKT_TIMEOUT;
//	//							ReSendCount = 0;
//	//							bNeedMeterDeleteAck = 1;
//	//						}
//	//						else
//	//						{
//	//							bCltorSearchStart = 0;
//	//							bMeterDeadStart = 0;
//	//							bNeedMeterDeleteAck = 0;
//	//						}
//	//					}
//	//					else
//	//					{
//	//						if(SendMeterpkt.deadtime < curslottime)
//	//						{						
//	//							if(ReSendCount > 2)
//	//							{
//	//								//重发次数大于3次的处理
//	//								bNeedMeterDeleteAck = 0;				
//	//							}
//	//							else
//	//							{
//	//								UART_PutArray(UART0, SendMeterpkt.data, SendMeterpkt.len); 
//	//								SendMeterpkt.deadtime =  curslottime + UART_PKT_TIMEOUT;
//	//								ReSendCount ++;						
//	//							}
//	//						}
//	//					}
//	//				}			
//			}
//		}
}


//------------------------------------------------------

void Send_Broad()
{
    uint8 msr = getMSR();
    if(ExtCtrlDeadCont != 0)
    {
        return ;
    }

	if(g_bResetCmd)
    {
		if(curslottime > (nChangeChannelTime))
        {
			//延时200MS
			Reset();//复位
        }
	}
	else if(bSendResetCmd)
	{
        if(curslottime < 800)//(MAX_SUP_SS_NUM * 2))  10秒
        {
            if((msr & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
            {
//	                behave.behave = BEHAVIOR_BROADCAST;
//	                behave.freq = broad_fhc;
//	                Switch_Channel(behave.freq);//切换频点
//	                nBroadSendData = 0;
//	                EZMacPRO_Transmit_Reset();
#ifdef RF_MESH_LEVEL2
                EZMacPRO_Transmit_Beacon(0, 0);
                ExtCtrlDeadCont = Get_BroadCast_Delay(0x98);
#endif
            }
        }
        else if(curslottime > (800 + 10))
        {
            curslottime = 0;
            sysSlotTime = 0;
            msr = EZMAC_PRO_IDLE;
            setMSR(msr);
            bSendResetCmd = 0;
        }
	}
    
	else if(bNeedChangeChannel)
	{
		if(curslottime < nChangeChannelTime)
		{
			if((msr & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
			{
				EZMacPRO_Transmit_Adv(0, NULL, 0);
                ExtCtrlDeadCont = Get_BroadCast_Delay(0x99);

			}
		}
		else if(curslottime > (nChangeChannelTime + MAX_SLOT_TIME * 5))
		{
			//修改频段号
			if((nFreqChannel != nFreqChannelNum) && (nFreqChannelNum < FREQ_NUM))
            {         
				I2cWrite(0xA0,  &nFreqChannelNum, FM_FREQ_CHANNEL_NUM, 1);  	
            }

			Reset(); //重启
		}
	}
    else if(bNeedRelayBroadCast)
    {
		if(curslottime < nChangeChannelTime)
		{
			if((msr & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
			{
				EZMacPRO_Transmit_Adv(1, NULL, 0);
                ExtCtrlDeadCont = Get_BroadCast_Delay(0x99);
                
			}
		}
		else if(curslottime > (nChangeChannelTime + MAX_SLOT_TIME * 3))
		{
			bNeedRelayBroadCast = 0;
            EzBroadCastPkt.bValid = 0;
		}
    }

    if( ( GET_TICK_COUNT( ) - g_nModuleResetTime ) > RF_RESET_INTERVAL )
    {
        //uint8 RFLRState = msr & 0x0F;

        if((msr & EZMAC_PRO_IDLE) == EZMAC_PRO_IDLE)
        {
            g_nModuleResetTime = GET_TICK_COUNT( );
            LOG_DEBUG( DBGFMT"+++++++ SX1276ReInit +++++++\n",DBGARG);
            uint8 tmp[4];
            I2cRead(0xA0, &tmp[0], FM_FREQ_DEFINE+8, 4); //存设置频点定义参数***
            int32 order = tmp[0]*0x1000000 + tmp[1]*0x10000 + tmp[2]*0x100 + tmp[3];
            
            Radio->Init(order, rfpara.rf_channel, rfpara.rf_power);
            Radio->StartRx( 0);
            //SX1276ReInit();
            //打印串口初始化一下
            Delay_ms(300);
//            UART_Init(UART1);
            
        }
        
    }

    
}


#if 0
void Add_Validate_W_B(uint8* pAddr, uint8 type)
{
	uint16 localssloop; 
	uint16 ssloop;
	WB_SPACE SSTemp;
	uint8 bNeedAdd;
	int return_hash;
	uint8 ValidateSSType;
	uint8 bAddSuccess;
	
	WB_SPACE* pWriteSSSpace;
	WB_SPACE* pBlackSSSpace;

	//取出当前黑白名单的存放位置
	if(0 == CurWhiteSSMap)
		pWriteSSSpace = WhiteSSMap1;
	else
		pWriteSSSpace = WhiteSSMap2;
	if(0 == CurBlackSSMap)
		pBlackSSSpace = BlackSSMap1;
	else
		pBlackSSSpace = BlackSSMap2;		

	//取出要添加的采集器地址与信息
	memcpy(SSTemp.addr, pAddr, 6);
//	SSTemp.addr[5] = 0;
	
	ValidateSSType = type;
	if(ValidateSSType > 1)
		ValidateSSType = SS_VALIDATE_TYPE_WHITE;

	//遍历白名单，查找是否有同地址的采集器
	bNeedAdd = 1;
	for(ssloop = 0; ssloop < MAX_SS_MAP_SIZE; ssloop++)
	{
	//	if((pWriteSSSpace[ssloop].status == OCCUPIED) && (!LPC_Memory_CMP(SSTemp.addr, pWriteSSSpace[ssloop].addr, 5)))	
		if((pWriteSSSpace[ssloop].status == OCCUPIED) && (!LPC_Memory_CMP(SSTemp.addr, pWriteSSSpace[ssloop].addr, 6)))
		{
			//名单中已有同地址的采集器
			if(ValidateSSType == SS_VALIDATE_TYPE_BLACK)
			{
				//当添加的采集器信息为黑名单时，需要删除白名单
				Updata_SS_Hash_Table(ssloop, NULL, SS_VALIDATE_TYPE_WHITE);
			}
			else if(ValidateSSType == SS_VALIDATE_TYPE_WHITE)
			{
				//当添加的采集器信息为白名单时，无需添加，直接返回
				bNeedAdd = 0;
				break;
			}
		}
	}
		
	if(bNeedAdd)
	{
		//遍历黑名单，判断是否有同地址的采集器
		for(ssloop = 0; ssloop < MAX_SS_MAP_SIZE; ssloop++)
		{
		//	if((pBlackSSSpace[ssloop].status == OCCUPIED) && (!LPC_Memory_CMP(SSTemp.addr, pBlackSSSpace[ssloop].addr, 5)))		
			if((pBlackSSSpace[ssloop].status == OCCUPIED) && (!LPC_Memory_CMP(SSTemp.addr, pBlackSSSpace[ssloop].addr, 6)))
			{
				//有同地址时，无需添加，直接返回
				bNeedAdd = 0;
				break;
			}
		}
		if(bNeedAdd)
		{
			//需要添加此采集器信息
			return_hash = SS_Hash_Table_Insert(&SSTemp, ValidateSSType);
			bAddSuccess = FALSE;
			if(return_hash >= 0)
			{  
				if (return_hash == MAX_SS_MAP_SIZE)
				{
					//当FLASH存储满时，需要转化存储区域
					if(ValidateSSType == SS_VALIDATE_TYPE_BLACK)
					{
						CurBlackSSMap =  (CurBlackSSMap + 1) % 2;
						I2cWrite(0xA0, &CurBlackSSMap, FM_BLACK_HASH_USED, 1);
					}
					else
					{
						CurWhiteSSMap = (CurWhiteSSMap + 1) % 2;
						I2cWrite(0xA0, &CurWhiteSSMap, FM_WRITE_HASH_USED, 1);					
					}
					SS_Hash_Transfer(ValidateSSType); 	//表满,转移哈希表
					return_hash = SS_Hash_Table_Insert(&SSTemp, ValidateSSType);	//再次插入上次未添加成功的
					if((return_hash >= 0) && (return_hash != MAX_SS_MAP_SIZE))
					{
						bAddSuccess = TRUE;
					}
				}
				else //如果添加成功
				{
					bAddSuccess = TRUE;
				}
			}		
			else
			{
				//有相同的采集器存在
			}
			
			//当添加黑名单成功时，需要离网本地相对应的采集器
			if((bSSValidateEnable) && (bAddSuccess) && (ValidateSSType == SS_VALIDATE_TYPE_BLACK))
			{
				//需要离网相对应的采集器
				for(localssloop = 2; localssloop <= rfpara.rf_slotnum; localssloop++)
				{
					if(cltor[localssloop].devAddr[0] <= 0x99)
					{
						if(!LPC_Memory_CMP(SSTemp.addr, cltor[localssloop].devAddr, 6))
						{
							cltor[localssloop].nod.bNeedDelete = TRUE;
							break;
						}
					}
				}					
			}
		}
	}
	
}

void Add_Validate_W_B_List(uint8* pData)
{
	uint8 AddNum = pData[13];
	uint8 SSType, loop, StartAddr;
	for(loop = 0; loop < AddNum; loop++)
	{
		//取出要添加的采集器地址与信息
		StartAddr = loop * 7 + 13 + 1;
		SSType = pData[StartAddr + 6];

		Add_Validate_W_B(pData + StartAddr, SSType);
	}
	//SendReturnpkt(0);
}

void Del_Validate_W_B(uint8* pData)				
{
	uint16 ssloop, localssloop;
	WB_SPACE SSTemp;
		
	WB_SPACE* pWriteSSSpace;
	WB_SPACE* pBlackSSSpace;
	uint8 bFound = 0;

	//取出当前黑白名单的存放位置
	if(0 == CurWhiteSSMap)
		pWriteSSSpace = WhiteSSMap1;
	else
		pWriteSSSpace = WhiteSSMap2;
	if(0 == CurBlackSSMap)
		pBlackSSSpace = BlackSSMap1;
	else
		pBlackSSSpace = BlackSSMap2;		

	//取出采集器地址
	memcpy(SSTemp.addr, pData, 6);
	
	//遍历白名单，查找采集器
	for(ssloop = 0; ssloop < MAX_SS_MAP_SIZE; ssloop++)
	{
		if((pWriteSSSpace[ssloop].status == OCCUPIED) 
			&& (!LPC_Memory_CMP(pWriteSSSpace[ssloop].addr, SSTemp.addr, 6)))
		{
			//需要离网相对应的采集器
			if(bSSValidateEnable)
			{
				for(localssloop = 2; localssloop <= rfpara.rf_slotnum; localssloop++)
				{
					if(cltor[localssloop].devAddr[0] <= 0x99)
					{
						if(!LPC_Memory_CMP(pWriteSSSpace[ssloop].addr, cltor[localssloop].devAddr, 6))
						{
							cltor[localssloop].nod.bNeedDelete = TRUE;
							break;
						}
					}
				}
			}	
			//删除采集器
			Updata_SS_Hash_Table(ssloop, NULL, SS_VALIDATE_TYPE_WHITE);					
			bFound = 1;
			break;
		}
	}
	
	if(0 == bFound)
	{
		//遍历黑名单，查找采集器
		for(ssloop = 0; ssloop < MAX_SS_MAP_SIZE; ssloop++)
		{
			if((pBlackSSSpace[ssloop].status == OCCUPIED) 
				&& (!LPC_Memory_CMP(pBlackSSSpace[ssloop].addr, SSTemp.addr, 6)))
			{
				//删除采集器
				Updata_SS_Hash_Table(ssloop, NULL, SS_VALIDATE_TYPE_BLACK);
				break;
			}
		}
	}
}

void Del_Validate_W_B_List(uint8* pData)
{
	uint8 DelNum = pData[13];
	uint8  loop, StartAddr;
	for(loop = 0; loop < DelNum; loop++)
	{
		//取出要添加的采集器地址与信息
		StartAddr = loop * 6 + 13 + 1;
		
		Del_Validate_W_B(pData + StartAddr);
	}
}
#endif


//---------------------------------------------
//获取温度
int8 getTemperaureDegC(void)
{
  uint8 ret = 0;
  
//	  SpiWriteRegister(SI4432_ADC_CONFIGURATION, 0x00);
//	  DelayNS(1);
//	  SpiWriteRegister(SI4432_TEMPERATURE_SENSOR_CONTROL, 0x60);
//	  SpiWriteRegister(SI4432_ADC_CONFIGURATION, 0x80);
//	  while(1)
//	  {
//		ret = SpiReadRegister(SI4432_ADC_CONFIGURATION);
//		if(ret & 0x80)
//			break;
//	  }
//	  ret = SpiReadRegister(SI4432_ADC_VALUE);
  return ret; 
}
//---------------------------------------------------------------------------------
//发送单频点波形
void CW_generator(uint8 pwr)
{
//	    RF_SWITCH_TX();
//	    //set the output power
//	    macSpiWriteReg(SI4432_TX_POWER, pwr);
//	    //enable CW mode
//	    macSpiWriteReg(SI4432_MODULATION_MODE_CONTROL_2, 0x00);
//	    //disable packet handler & CRC16
//	    macSpiWriteReg(SI4432_DATA_ACCESS_CONTROL, 0x00);
//	    //enable transmitter
//	    macSpiWriteReg(SI4432_OPERATING_AND_FUNCTION_CONTROL_1, 0x08);
//	    //disable all the ITs
//	    macSpiWriteReg(SI4432_INTERRUPT_ENABLE_1, 0x00);
//	    macSpiWriteReg(SI4432_INTERRUPT_ENABLE_2, 0x00);
//	    LED_TX2_ON(); 
 //  while(1);
}

void CloseBoardSearchMeter(uint8 flag)
{
	uint16 loop;
	bMeterSearchStart = 0;
	
	for(loop = 0; loop < MAX_HASH_TABLE_FLASH_SIZE; loop++)
	{
		MeterStatus[loop].bAck = 0;
		MeterStatus[loop].bReceive = 0;
		MeterStatus[loop].bWaitAck = 0;
	}
	
	bMeterDeadStart = 0;
	bCltorSearchStart = 0;
	bNeedMeterDeleteAck = 0;
	bWaitForSendMeterAck = 0;

	//记录白名单
/*	for(loop = 2; loop < MAX_SUP_SS_NUM+1; loop++)
	{
		if(cltor[loop].devAddr[0]<=0x99)
			Add_Validate_W_B(cltor[loop].devAddr, SS_VALIDATE_TYPE_WHITE);
	}*/
	
	//开启认证使能
	if(bNeedOpenSSValidateEnable)
	{
		bSSValidateEnable = 1;
		I2cWrite(0xA0, &bSSValidateEnable, FM_SS_VALIDATE_ENABLE , 1); 		

		bNeedOpenSSValidateEnable = 0;
		I2cWrite(0xA0, &bNeedOpenSSValidateEnable, FM_NEED_OPEN_SS_VALIDATE , 1); 		
	}
 
	bBroadMeterEnable = 0;
	I2cWrite(0xA0, &bBroadMeterEnable, FM_BROAD_METER_ENABLE , 1); 		
	
	adv_subtype = ADV_TYPE_EMPTY;//普通包
	adv_sublen = ADV_SUB_LEN;   		

//		if(flag)
//		{
//			SendReturnpkt(0);
//			DelayNS(100);
//		}
	//重启
//		Reset();
}

uint8 getRssiByRssiQuality(uint8 rssiQ)
{
    uint8 ret = 0;
    switch(rssiQ)
    {
    case 0:
        ret = 140;
        break;
    case 1:
        ret = 136;
        break;
    case 2:
        ret = 132;
        break;
    case 3:
        ret = 128;
        break;
    case 4:
        ret = 124;
        break;
    case 5:
        ret = 120;
        break;
    case 6:
        ret = 116;
        break;
    case 7:
        ret = 112;
        break;
    case 8:
        ret = 108;
        break;
    case 9:
        ret = 104;
        break;
    case 10:
        ret = 100;
        break;
    case 11:
        ret = 96;
        break;
    case 12:
        ret = 92;
        break;
    case 13:
        ret = 88;
        break;
    case 14:
        ret = 84;
        break;
    case 15:
        ret = 80;
        break;
            
    }
    return ret;
}
const uint8 aRssiQ[] = {140,136,132,128,124,120,116,112,108,104,100,96,92,88,84};
uint8 getRssiQualityByRssi(uint8 rssi)
{
    uint8 ret = 0;
    uint8 i = 0;
    for(i = 0; i < sizeof(aRssiQ); i++)
    {
        if(rssi >= aRssiQ[i])
        {
            break;
        }
    }

    ret = i;

    return ret;
}

uint8 paraRecorvery(uint8 type)
{


	uint8 dis[32] ;
    uint16 i = 0;
	//uint8 bError = 0;

	uint8 bValue = 0;
	uint8 Temp[16];
	//bError = 0;
    uint8 channelId;
    RF_PARAMETER * p;
    uint8 addrLen;
    switch(type)
    {

    case 1:
		memset(dis,0xff,32);
		for(i=0;i<32;i++)
		{	                                   
			I2cWrite(0xA0, dis, 0+(i*32), 32); //复位影子空间***
			memset(shadow_space+(i*32),0xff,32);						 
		}
        Init_Hash_Table();                    

		//初始化路由参数
		cltparm.routeuptime = 60;//心跳      
		cltparm.maxttl = 120;//生存周期
		cltparm.routeseltime = 30;//选父节点时间    
		I2cWrite(0xA0, (uint8*) &cltparm, FM_CLTP_ADDR, sizeof(CLTP)); //写入路由参数         

		//初始化网络参数
        //extern uint8 nDeviceMacAddr[6];
        channelId = Cal_Hash_Value(nDeviceMacAddr) % getChannelNum();
        if(channelId == 0)
        {
            channelId = 1;
        }                  
		memset(Temp, 0, sizeof(RF_PARAMETER));

        p = (RF_PARAMETER *)Temp;

        p->rf_channel = channelId; //信道号
        p->rf_power = 0;//发射功率
        p->rf_boad = 0x1;//空中波特率
        p->rf_limit = 157;//rssi门限        
        p->rf_net_id = 0x01;//网络标识
        p->rf_route = 0x01;//路由方式
        p->rf_jump_chaneel=0x2;//调频序列号
        p->rf_slotnum = MAX_SUP_SS_NUM;//时帧长度
        //p->panid = 0xFFFF;
        memcpy((uint8 *)&p->panid, nDeviceMacAddr, 2);
        //rfpara.panid = 0xFFFF;
        p->shortID = localid;
        
		I2cWrite(0xA0, Temp, FM_NETPARA_ADDR, sizeof(RF_PARAMETER)); //存网络参数***

		memset(Temp, 0, 9);
        addrLen = 6;
        I2cWrite(0xA0, &addrLen, FM_DEVICE_ADDR_LEN, 1);
        
		I2cWrite(0xA0, Temp, FM_DEVICE_ADDR , 6);
	//	I2cWrite(0xA0, Temp, FM_BROAD_METER_TIME , 2); 

		bValue = 0;
		I2cWrite(0xA0, &bValue, FM_BROAD_METER_ENABLE , 1); 
		I2cWrite(0xA0, &bValue, FM_SS_CODEUPDATA_ENABLE , 1); 
		I2cWrite(0xA0,  &bValue, FM_I_SS_CODEUPDATA_ENABLE, 1);   
		I2cWrite(0xA0,  &bValue, FM_CHANNEL_SET_MODE, 1);         						

		bValue = METER_DEAD_COUNT;
		I2cWrite(0xA0,  &bValue, FM_METER_DEAD_COUNT, 1);     

		bValue = 0;		 
		I2cWrite(0xA0, &bValue, FM_SS_VALIDATE_ENABLE , 1); 
		I2cWrite(0xA0, &bValue, FM_METER_INFO_SYN_ENABLE, 1);	

        nChangeChannelTime = curslottime + MAX_SLOT_TIME * 10;
        bNeedChangeChannel = TRUE;

        for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM+1; i++)  //遍历
        {
            if(cltor[i].devAddr[0] < 0xFF)//
            {
                Delete_SS_Node(i);          
            }
        }
        break;
    default:
        break;
    }
    return 1;    
}

uint8 getNetSts(void)
{
    uint8 sts = 0;
    uint16 nMeterNum;
    uint16 nSSNum; 
    nMeterNum = Meter_Num();
    if(bSendResetCmd)
    {
        sts = 0x02;//信标阶段
    }
    else
    {
        nSSNum = 0;
        for (uint16 i = 2; i <= rfpara.rf_slotnum; i++)
        {  
            if ((cltor[i].devAddr[0] < 0xFF) && (cltor[i].nodestatus.NetStatus == NODE_STATUS_LOGIN))
                nSSNum ++;
        }
        
        //memcpy(updata_pkt_back, pkt->data, 13);                                           
        //updata_pkt_back[1] = 31;  //包长度       
        //updata_pkt_back[3] = 0x8a;  //控制字  
        //m = 13;
    //////////////////////////////////路由稳定期，表越多，等待时间越长；一半表计入网以后路由稳定
        int x;
        if(nMeterNum > 1000)
        {
            x=8;
        }
        else if(nMeterNum > 800)
        {
            x=7;
        }
        else if(nMeterNum > 600)
        {
            x=6;
        }
        else if(nMeterNum > 400)
        {
            x=5;
        }
        else if(nMeterNum > 200)
        {
            x=4;
        }
        else if(nMeterNum > 100)
        {
            x=3;
        }
        else
        {
            x=2;
        }

        if((2*nSSNum) >= nMeterNum || (curslottime > (nMeterNum * x * 1600)))// 2 * 40s * x值表
        {
            sts = 0x01;//正在工作，路由学习完成
        }
        else
        {
            sts = 0x00;//等待组网
        } 
    }
    return sts;
}
//检测表记模块类型
uint8 checkMeterModuleProtocol(void)
{
    uint16 i = 0;
    uint8 ret = 0;
    for(i = SUP_SS_INDEX_START; i < rfpara.rf_slotnum; i++)
    {
        if(cltor[i].nodestatus.protocol == PST_FRM_WL_1_NO)
        {
            ret |= 0x01;
            break;
        }
    }
    for(i = SUP_SS_INDEX_START; i < rfpara.rf_slotnum; i++)
    {
        if(cltor[i].nodestatus.protocol == PST_FRM_802_R_NO)
        {
            ret |= 0x02;
            break;
        }
    }
    if(ret == 0)
    {
        ret = 0x01 | 0x02;
    }
    return ret;
}

void I2cWrite(uint8 Addr, uint8 *Data, uint16 inside_addr, uint16 NByte)
{

//    WriteDataToEeprom(EEPROM_PIECE1, Data, inside_addr, NByte);
}
#define SIGNAL_QUALITY_LEVEL    15
/*************************************************************************
 * Function Name: getSignalQuality
 * Parameters:  获取信号质量  1-15
 *              rssi :收到的信号质量
 *              max  最好的信号
 *              low  最低灵敏度
 * Return:  none
 *
 *************************************************************************/

uint8 getSignalQuality(int rssi, int max, int low)
{
    float x;
    uint8 ret = 0;
//	    x = (float)(max - low) / SIGNAL_QUALITY_LEVEL;
//	
//	    ret = (uint8)((float)(-(rssi - max)) / x);

    return ret;
}

#ifdef MASTER_NODE

/*************************************************************************
 * Function Name: UpdataHashTable
 * Parameters:  insert a record to hashtable1 
 * Return: the location in the hashtable1 if successed ,-1 for negative
 * Description: 
 *
 *************************************************************************/
void Updata_Hash_Table(uint16 hash, HASHT *elem) //更新哈希表
{
    uint32 i;
    uint32 sector;
    uint32 location;
    HASHT *p;
    
    TDataBlock db;
    p=( HASHT *)updata_code_cache; 
    sector = hash / MAX_HASH_TABLE_SECTOR_SIZE;
    location = sector * MAX_HASH_TABLE_SECTOR_SIZE;

//	    if(IapAction == IAP_ACTION_IDLE)//内存保护
    {
        switch(HASH_Table_Used)
        {
        case 0://当前使用table1
        
            for (i = 0; i < MAX_HASH_TABLE_SECTOR_SIZE; i++)
            {
                if(htable1[location + i].status==OCCUPIED)//如果该哈希元素的值有效     
                {
                    p[i] = htable1[location + i]; //把原来哈希表里的数据保存在缓存中
                }
                else if(htable1[location + i].status==EMPTY)
                {   // 如果该哈希元素的值无效                     
                    memset(&p[i], 0xFF, sizeof(HASHT)); //缓冲中的数都置成0X0***
                }
                else if(htable1[location + i].status==DEAD)
                {   // 如果该哈希元素的值无效                     
                    memset(&p[i], 0x0, sizeof(HASHT)); //缓冲中的数都置成0X0***
                }

            }	
            break;
        case 1://当前使用table2
            for (i = 0; i < MAX_HASH_TABLE_SECTOR_SIZE; i++)
            {

                if(htable2[location + i].status==OCCUPIED)//如果该哈希元素的值有效     
                {
                    p[i] = htable2[location + i]; //把原来哈希表里的数据保存在缓存中
                }
                else if(htable2[location + i].status==EMPTY)
                {   // 如果该哈希元素的值无效                     
                    memset(&p[i], 0xFF, sizeof(HASHT)); //缓冲中的数都置成0X0***
                }
                else if(htable2[location + i].status==DEAD)
                {   // 如果该哈希元素的值无效                     
                    memset(&p[i], 0x0, sizeof(HASHT)); //缓冲中的数都置成0X0***
                }
            }	
            break; 
        }
        

        if (elem != NULL)//如果有新添加的哈希元素        
        {
                // 更新哈希缓存
    		memcpy((uint8 *)&p[hash - location], (uint8 *)elem, sizeof(HASHT)); //把新添加的哈希表元素放到哈希缓存表
//	    		memcpy(p[hash - location].falsh_sn, elem->falsh_sn,6);
    		p[hash - location].status	= OCCUPIED;	
//	    		p[hash - location].SubType = elem->SubType;
//	    		p[hash - location].src = elem->src;
//	            p[hash - location].devType = elem->devType;
            
    		shadow_space[hash].handled=UN_HANDLED;              
    		shadow_space[hash].times= MeterDeadCount;
    		I2cWrite(0xA0, (uint8*)&shadow_space[hash], hash,1);//写入E2PR0M		
        }
        else
        {           
        	//p[hash - location].status	= DEAD;	
        	memset(&p[hash - location],0x0,sizeof(HASHT));
        }
	

                // 写入flash
		if(HASH_Table_Used==0)
		{
		
            SYS_FILE_DB_Open(DB_HASH1, &db, TDB_MODE_RW);
            SYS_IFILE_DB_WriteFrom(&db, updata_code_cache, MAX_HASH_TABLE_CACHE_SIZE*MAX_HASH_TABLE_SECTOR_SIZE,0);
            SYS_FILE_DB_Close(&db);
//				IapAction = IAP_ACTION_TRANSFER_HASH1;//写table1
//			    HashUpdateSectorAdd = FLASH_MAP_ADD + sector * MAX_HASH_TABLE_SECTOR_SIZE * MAX_HASH_TABLE_CACHE_SIZE ;
		}
		else
		{
            SYS_FILE_DB_Open(DB_HASH2, &db, TDB_MODE_RW);
            SYS_IFILE_DB_WriteFrom(&db, updata_code_cache, MAX_HASH_TABLE_CACHE_SIZE*MAX_HASH_TABLE_SECTOR_SIZE,0);
            SYS_FILE_DB_Close(&db);
		
//				IapAction = IAP_ACTION_TRANSFER_HASH2;//写table2
//			    HashUpdateSectorAdd = FLASH_MAP_ADD2 + sector * MAX_HASH_TABLE_SECTOR_SIZE * MAX_HASH_TABLE_CACHE_SIZE;
		}
//			while (IapAction != IAP_ACTION_IDLE);   // hold on
//			
//			if(Write_Sucess==ERR)//如果成功失败
//			{
//			    Hash_Correct(HashUpdateSectorAdd,(uint8*)&updata_code_cache);		
//			}
    }
}


/*************************************************************************
 * Function Name: htable1_insert
 * Parameters:  insert a record to hashtable1 
 * Return: the location in the hashtable1 if successed ,-1 for negative
 * Description: 
 *
 *************************************************************************/

int Hash_Table_Insert(HASHT *elem, int* pReturnHash) //添加哈希表
{
    uint16 hash;
    uint16 index;
    hash = Cal_Hash_Value(elem->addr); //计算得到哈希索引	
    HASHT* pHasht;
    
    TDataBlock db;
	if(HASH_Table_Used==0)//当前使用table1
    {   
    
        SYS_FILE_DB_Open(DB_HASH1, &db, TDB_MODE_RW);
 		pHasht = (HASHT *)(0x08000000 + db.start);
        SYS_FILE_DB_Close(&db);
    }
	else
    {   
        SYS_FILE_DB_Open(DB_HASH2, &db, TDB_MODE_RW);
 		pHasht = (HASHT *)(0x08000000 + db.start);
        SYS_FILE_DB_Close(&db);
    }
		
	if(pHasht[hash].status==EMPTY)//空
	{                        
        Updata_Hash_Table(hash, elem); //更新哈希表
        *pReturnHash =  hash;
        return hash; //返回索引值并退出
	}
	 else if (pHasht[hash].status==OCCUPIED && (!LPC_Memory_CMP(pHasht[hash].addr, elem->addr, 6)))//该位被相同电表地址占用
	{
        if (/*(LPC_Memory_CMP(pHasht[hash].falsh_sn, elem->falsh_sn ,6))
            || */(pHasht[hash].SubType != elem->SubType)) 			//如果器件sn不一样                 
        {
            Updata_Hash_Table(hash, NULL);    //置原先的记录无效，再继续插入		                  
        }
        else
        {   // 器件ID一样，不再插入                             
			*pReturnHash =  hash;
            return hash;//( - 1);
        }
	}

	index = (hash + 1) % (MAX_HASH_TABLE_FLASH_SIZE); //如果被别的数占据，则索引加1再判断
	while (hash != index)
	{
        if (pHasht[index].status==EMPTY)    //如果索引对应的位置为空
        {
            Updata_Hash_Table(index, elem); //把哈希所以加1后添加进哈希表
            *pReturnHash =  index;
            return index; //返回索引值并退出循环
        }
        else if ((pHasht[index].status==OCCUPIED) && (!LPC_Memory_CMP(pHasht[index].addr, elem->addr, 6)))//该位被相同电表地址占用
        {
        	 if (/*(LPC_Memory_CMP(pHasht[index].falsh_sn, elem->falsh_sn ,6))
			 || */(pHasht[index].SubType != elem->SubType)) 	//如果器件sn不一样                           
            {
                Updata_Hash_Table(index, NULL);//置原先的记录无效
            }
            else // 器件ID一样，不再插入            
            {                                  
    			*pReturnHash =  index;    		
                 return index;//( - 1);
            }
        }
        index = (index + 1) %(MAX_HASH_TABLE_FLASH_SIZE);//加1、翻转
	}
	*pReturnHash =  MAX_HASH_TABLE_FLASH_SIZE;    
	return ( MAX_HASH_TABLE_FLASH_SIZE);//哈希表已满
}
#else

int Hash_Table_Insert(HASHT *elem, int* pReturnHash) //添加哈希表
{
    return 0;
}

#endif

void Hash_Table_Init(void)
{
    
    TDataBlock db;
    SYS_FILE_DB_Open(DB_HASH1, &db, TDB_MODE_RW);
    htable1 = (HASHT *)(0x08000000 + db.start);
    SYS_FILE_DB_Close(&db);
    
    SYS_FILE_DB_Open(DB_HASH2, &db, TDB_MODE_RW);
    htable2 = (HASHT *)(0x08000000 + db.start);
    SYS_FILE_DB_Close(&db);
}

void Hash_Transfer(void)
{
}
void Init_Hash_Table(void)
{}
uint8  I2cRead(uint8 Addr, uint8 *Data, uint16 inside_addr, uint16 NByte)
{return 0;}



