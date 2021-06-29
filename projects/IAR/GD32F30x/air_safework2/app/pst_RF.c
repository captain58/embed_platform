/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: pst_RF.c
**创   建   人: LGY
**最后修改日期: 2015年11月23日
**描        述: 微功率无线RF模块___规约处理文件包括主动通讯和被动通讯
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
**创建人: LGY
**版  本: v1.0
**日　期: 2015年11月23日
**描　述: 原始版本
********************************************************************************************************/
#define EXT_PST_RF
#include "protocol.h"
#include "pst_RF.h"

//	/********************************************************** 
//	//函数表原型
//	**********************************************************/
//	typedef struct _RF_CMD_TABLE_ {
//		uint16          cmd; 		//!< 命令名
//		void*			func;	    //!< 函数功能
//	} RF_CMD_TABLE_t;
//	
//	typedef uint8 (*FW_COMMAD_t)(const RF_CMD_TABLE_t* tbl, RF_Frame* frm);

/************************************************************************
 * @function: fRFCMD_FR
 * @描述: 读HXRF的频率基准点
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fRFCMD_FR(const CMD_TABLE_t* tbl, RF_Frame* frm)
{   
    if(frm->len == 4)
    {
        frm->send[4]=0;
//	        
//	        ParaFR = HxDevRF_FR();
//	        IntToHByte(frm->send+5,ParaFR);  //高字节在前
        frm->len=7;
    }
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
  
    ES_OK();
}

/************************************************************************
 * @function: fRFCMD_ID
 * @描述: 修改或读HXRF的表地址
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fRFCMD_ID(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         
    if(frm->len == 4)
    {
        frm->send[4]=0;
        MoveBuffer(nDeviceMacAddr, frm->send+5, 6); 
        InvBuffer(frm->send+5,6);
        frm->len=11;
    }
    else if(frm->len > 9)
    {
        InvBuffer(frm->recv+4,6);
        if(CmpBuffer(frm->recv+4, nDeviceMacAddr, 6)!=0)
        {
            MoveBuffer(frm->recv+4,nDeviceMacAddr,6); 
            uint8 addrLen = 6;
            I2cWrite(0xA0, &addrLen, FM_DEVICE_ADDR_LEN, 1);
            
			I2cWrite(0xA0, frm->recv+4, FM_DEVICE_ADDR , 6);
			I2cRead(0xA0, nDeviceMacAddr, FM_DEVICE_ADDR, 6); 
        }        
        frm->send[4]=0;

        frm->len=5;
        //return ES_ERR_NOREPLY;              //不需要应答
    }
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
  
    ES_OK();
}
/************************************************************************
 * @function: fRFCMD_CH
 * @描述: 修改或读HXRF的通道
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fRFCMD_CH(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         
    if(frm->len == 4)
    {
        frm->send[4] = 0;
        I2cRead(0xA0, &frm->send[5], FM_NETPARA_ADDR, 1);

        frm->len=6;
    }
    else if(frm->len > 4)
    {
        if(frm->recv[4] > 0 && frm->recv[4] < getChannelNum())
        {
            I2cWrite(0xA0, &frm->recv[4], FM_NETPARA_ADDR, 1);
            rfpara.rf_channel = frm->recv[4];
            uint8 tmp[4];
            I2cRead(0xA0, &tmp[0], FM_FREQ_DEFINE+8, 4); //存设置频点定义参数***
            int32 order = tmp[0]*0x1000000 + tmp[1]*0x10000 + tmp[2]*0x100 + tmp[3];

            Radio->Init(order, rfpara.rf_channel, rfpara.rf_power);
            //SX1276LoRaInit( );
            Radio->StartRx( );
            frm->send[4]=0;

        }

//	         gsPara.CH = frm->recv[4];
//	         if(ParamSave_CH()==ES_ERR_OK)
//	         {
//	           frm->send[4]=0;
//	           ClrFlag(HRS_LORA_CONF,gl_SysRunStt); //重新配置通道 
//	           ClrFlag(HRS_RF_FINDCH,gl_SysRunStt); //配置通道 默认是正确通道
//	         }
        else
        {
            frm->send[4]=1;
        }
        frm->len=5;
    }
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
  
    ES_OK();
}
/************************************************************************
 * @function: fRFCMD_PL
 * @描述: 修改或读HXRF的频率基准点
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fRFCMD_PL(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         
    if(frm->len == 4)
    {
        frm->send[4] = 0;
        frm->send[5] = rfpara.rf_power;       
        frm->len=6;
    }
    else if(frm->len > 4)
    {
        frm->send[4]=1;
        if(frm->recv[4]<8)
        {
//	            gsPara.PL = frm->recv[4];
//	            if(ParamSave_PL()==ES_ERR_OK)
//	            {
//	                frm->send[4]=0;
//	                ClrFlag(HRS_LORA_CONF,gl_SysRunStt); //重新配置RF
//	            }

            uint8 rf_power = frm->recv[4];
            if(rf_power != rfpara.rf_power && rf_power < getPwrLevelNum())
            {
                rfpara.rf_power = rf_power;
                I2cWrite(0xA0, (uint8*)&rfpara.rf_power, FM_NETPARA_ADDR+1, 1); //存网络参数***	
                //LoRaSettings.Power = powerArray[rfpara.rf_power];
                //SX1276LoRaSetRFPower( LoRaSettings.Power );
                setRfPwr(rfpara.rf_power);
            }

            frm->send[4]=0;
        }        
        frm->len=5;
    }
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
    ES_OK();
}
/************************************************************************
 * @function: fRFCMD_SV
 * @描述: 读HXRF的读取版本号
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fRFCMD_SV(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         
    uint8 m = 0;
    
    if(frm->len == 4)
    {
        uint8 * buffer = frm->send+5;
        frm->send[4] = 0;

        memcpy(buffer + m, (uint8 *)ucModeVer, 20);

        buffer[m + 8] = (sVersiong.swCode[0] / 0x10) + '0';
        buffer[m + 9] = (sVersiong.swCode[0] % 0x10) + '0';

        buffer[m + 11] = (sVersiong.yy / 0x10) + '0';
        buffer[m + 12] = (sVersiong.yy % 0x10) + '0';
        buffer[m + 13] = (sVersiong.mm / 0x10) + '0';
        buffer[m + 14] = (sVersiong.mm % 0x10) + '0';
        buffer[m + 15] = (sVersiong.dd / 0x10) + '0';
        buffer[m + 16] = (sVersiong.dd % 0x10) + '0';

        m+=20;
        //MoveBuffer((uint8*)gc_HxNkVer,frm->send+5,sizeof(gc_HxNkVer));          
        frm->len=m+5;
    }    
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
  
    ES_OK();
}
/************************************************************************
 * @function: fRFCMD_RS
 * @描述: 接收到的最新信号强度
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fRFCMD_RS(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         

    ES_OK();
}
/************************************************************************
 * @function: fRFCMD_RS
 * @描述: 工厂模式
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fRFCMD_FT(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         

    g_ucfactory_mod = 0x55;
    frm->send[4] = 0;
    frm->len=5;
    ES_OK();
}
/************************************************************************
 * @function: fRFCMD_RS
 * @描述: 接收到的最新信号强度
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fRFCMD_HT(const CMD_TABLE_t* tbl, RF_Frame* frm)
{       
    uint8 temp = 0;
    frm->send[4] = 0;

    
    uint8 ret = FlashTest(256);
    temp |= ret << HD_TEST_FLASH;
    ret = EEpromTest(256);
    temp |= ret << HD_TEST_EEPROM;

    if(!(SPI_Check()))
    {
        temp |= 1 << HD_TEST_RF;
    }
    //buffer[m++] = temp;
    frm->send[5] = temp;       
    frm->len=6;

    ES_OK();
}
/************************************************************************
 * @function: fRFCMD_RS
 * @描述: 接收到的最新信号强度
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fRFCMD_CT(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         
    extern COLLECTOR_SHADOW cltor_shadow[MAX_SUP_SS_NUM + 1];//子节点信息数组            
    if(g_ucfactory_mod == 0x55)
    {
		recvuppkt.deadtime = MAX_DATA_TIME_LIVE;
		StaAck = PKT_TYPE_HD_TEST;	
        EZ_TX_PKT * ezPkt = getIdelSendCacheByPri(CON_SEND_PRIORITY_NORMAL);

    	if(NULL != ezPkt)
		//if(0 == EzDataTxPkt.bValid)
		{
			PKT* pTxPkt 	= 	&(ezPkt->TxPkt);
			
			pTxPkt->ttl = MAX_DATA_TIME_LIVE;//(uint8)recvuppkt.deadtime;
                    
            memset((uint8 *)&pTxPkt->head.apdu, 0, sizeof(STAPDU));
			pTxPkt->head.apdu.ctrl.dir = 0;
            pTxPkt->head.apdu.ctrl.prm = 1;
            pTxPkt->head.apdu.ctrl.evtmode = 1;
            pTxPkt->head.apdu.ctrl.ftd = 1;
            pTxPkt->head.apdu.seq = cltor_shadow[0].sendseq;
            pTxPkt->head.apdu.fn = 49;//自适应
            pTxPkt->head.apdu.stInfo.stDown.bit1.routeFlg = 0;//pkt->rarea->stDown.bit1.routeFlg;
            pTxPkt->head.apdu.stInfo.stDown.bit1.nodeFlg = 0;//pkt->rarea->stDown.bit1.nodeFlg;
            pTxPkt->head.apdu.stInfo.stDown.bit1.addrFlg = 0;
            pTxPkt->head.apdu.stInfo.stDown.bit1.conflict = 1;//pkt->rarea->stDown.bit1.conflict;
            pTxPkt->head.apdu.stInfo.stDown.bit1.routeNum = 0;//pkt->rarea->stDown.bit1.routeNum;
            pTxPkt->head.apdu.stInfo.stDown.bit2.channelFlg = 0;//pkt->rarea->stDown.bit2.channelFlg;
            pTxPkt->head.apdu.stInfo.stDown.bit2.errEncodeFlg = 0;//pkt->rarea->stDown.bit2.errEncodeFlg & 0x03;
            pTxPkt->head.apdu.addrlen = 12;//pkt->afnoffset -  pkt->AddrOffSet;                        
            pTxPkt->head.apdu.addr = pTxPkt->data + PKT_HEAD_LEN;
            extern uint8 nDeviceMacAddr[6];
            extern const uint8 sBroadAddrFD[6];
            memcpy(pTxPkt->head.apdu.addr, nDeviceMacAddr, UNIQUE_MAC_ADDR_LEN);//节点唯一标识(队列)	
            memcpy(pTxPkt->head.apdu.addr + 6, sBroadAddrFD, UNIQUE_MAC_ADDR_LEN);//节点唯一标识(队列)	

    		
            pTxPkt->head.apdu.len = 0;


            //recvuppkt.isneedack = 1; // 需要 ack

            ezPkt->bValid = 1;//业务缓存有效
            ezPkt->nBackOffSlot = 0;   //马上发送
            cltor_shadow[0].sendseq++; //每发送一条加1
            //negapkt.ttl = 0xff;

        }
    }
    SA_NOREPLY();
}
/************************************************************************
 * @function: fRFCMD_RS
 * @描述: 接收到的最新信号强度
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fRFCMD_NS(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         
    if(frm->len == 4)
    {
        frm->send[4] = 0;
        //frm->send[5] = gsPara.PL; 
        I2cRead(0xA0, &frm->send[5], FM_FREQ_DEFINE+8, 4); //存设置频点定义参数***	
        
        frm->len=9;
    }
    else if(frm->len > 4)
    {
        uint8 tmp[4];
        //uint8 orderFreq = 0;
        uint32 oldOrder = 0, newOrder = 0;
        I2cRead(0xA0, &tmp[0], FM_FREQ_DEFINE+8, 4); //存设置频点定义参数***	
        oldOrder = tmp[0]*0x1000000 + tmp[1]*0x10000 + tmp[2]*0x100 + tmp[3];
        newOrder = frm->recv[4]*0x1000000 + frm->recv[5]*0x10000 + frm->recv[6]*0x100 + frm->recv[7];
        //orderFreq = frm->recv[6] & 0xF0;
        if(oldOrder != newOrder)
        {

//	#if (SX127X_FREQ == FREQ_433M)
//	            if(orderFreq == 0x40)
//	#elif  (SX127X_FREQ == FREQ_915M)
//	            if(orderFreq == 0x90)
//	#elif  (SX127X_FREQ == FREQ_470M)
//	            if(orderFreq == 0x70)
//	#elif  (SX127X_FREQ == FREQ_868M)
//	            if(orderFreq == 0x80)                            
//	#endif
            {
                I2cWrite(0xA0, &frm->recv[4], FM_FREQ_DEFINE+8, 4); //存设置频点定义参数***	
                //SX1276LoRaInit( );
                uint8 tmp[4];
                I2cRead(0xA0, &tmp[0], FM_FREQ_DEFINE+8, 4); //存设置频点定义参数***
                int32 order = tmp[0]*0x1000000 + tmp[1]*0x10000 + tmp[2]*0x100 + tmp[3];
               
                Radio->Init(order, rfpara.rf_channel, rfpara.rf_power);
                Radio->StartRx( );
                
                frm->send[4]=0;
                frm->len=5;        

                ES_OK();
            }
//            else
//            {
//                frm->send[4]=1;
//                SA_ERR();
//            }        
        }
        else
        {
            frm->send[4]=0;
            frm->len=5;        
        }

    }
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
    ES_OK();
}
/************************************************************************
 * @function: fRFCMD_LG
 * @描述: 日志开关
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fRFCMD_LG(const CMD_TABLE_t* tbl, RF_Frame* frm)
{         
    if(frm->len == 4)
    {
        frm->send[4] = 0;
        frm->send[5] = ES_LOG_OCStt(); 
        frm->send[6] = ES_LOG_LevelStt();
        frm->len=7;
    }
    else if(frm->len > 4)
    {
        
        if(frm->recv[4]<9)
        {
            uint8 en = frm->recv[4];
            uint8 level = frm->recv[5];
            if(ES_ERR_OK == ES_LOG_OC(en, level))
            {
                frm->send[4]=0;
            }
            else
            {
                frm->send[4]=1;
            }
        }        
        frm->len=5;
    }
    else
    {
        frm->send[4]=1;
        frm->len=5;
    }
    ES_OK();
}
/**********************************************************
测试硬件函数的数组. 按照命令号大小排列，因为使用中值法查表
**********************************************************/
const CMD_TABLE_t stRFCMDTbl[] = {
    { RF_CMD_CH,		(void*)fRFCMD_CH,},
    { RF_CMD_CT,		(void*)fRFCMD_CT,},    
	{ RF_CMD_FR,		(void*)fRFCMD_FR,},
    { RF_CMD_FT,		(void*)fRFCMD_FT,},     
    { RF_CMD_HT,		(void*)fRFCMD_HT,},
	{ RF_CMD_ID,		(void*)fRFCMD_ID,},  
    { RF_CMD_LG,		(void*)fRFCMD_LG,},
    { RF_CMD_NS,		(void*)fRFCMD_NS,},      
    { RF_CMD_PL,		(void*)fRFCMD_PL,},
    { RF_CMD_RS,		(void*)fRFCMD_RS,},
    { RF_CMD_SV,		(void*)fRFCMD_SV,},
    { NULL,			NULL,	},
};
/************************************************************************
 * @function: PST_RF_Decode
 * @描述: 判断当前缓存里面是否为合法帧,如果是的话则解析成为被动通讯结构体
 * 
 * @参数: 
 * @param: receive 接收到数据的缓存
 * @param: send 需要发送的数据的缓存
 * @param: sbuflen 发送缓存长度
 * @param: frame 数据解析成的结构体
 * 
 * @返回: 
 * @return: uint8  ES_ERR_OK/ES_ERR_FT
 * @说明: 
 * @作者: xugl (2013/7/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_RF_Decode(uint8* receive, uint8* send, uint16 sbuflen, PST_Frame* frame)
{
    RF_Frame* frm = &frame->_rf;
    
    frm->len = ByteToHInt(receive + 1 ); //长度
    frm->FrameType = receive[3];        //控制域类型
    frm->FrameID = receive[4];          //控制域
    frm->cmd = ByteToHInt(receive + 5 ); //命令字
    if(frm->cmd > 0x6100)               //小写字母改大写
    {
      frm->cmd-=0x2020;
    }
    
    MoveBuffer(receive,send,7);         //为应答处理做准备
    frm->recv = receive + 3;            //接收到的数据
    frm->send = send + 3;               //发送数据缓存的起始地址
    
    if((frm->FrameType != 0x08) &&
       (frm->FrameID != 0x01))
    {
        return ES_ERR_FT;
    }   
    
     ES_OK();                            //让数据通过
}



/************************************************************************
 * @function: PST_RF_Proc
 * @描述: 
 * 
 * @参数: 
 * @param: frm 数据解析成的结构体
 * 
 * @返回: 
 * @return: uint8  ES_ERR_OK/ES_ERR_FT
 * @说明: 
 * @作者: xugl (2014/5/27)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_RF_Proc(PST_Frame* frame)
{
    RF_Frame* frm = &frame->_rf;
    uint16 index;
    uint16 length;
    const CMD_TABLE_t* tbl;
    uint8 err;
    
    index = ADVMidWaySearch(
        (uint8*)stRFCMDTbl,
        sizeof(stRFCMDTbl)/sizeof(CMD_TABLE_t),
        sizeof(CMD_TABLE_t),
        frm->cmd
        );
    
    if(index == 0xFFFF)                 //没有搜索到则返回数据标识错误
    {
        frm->len = 5;
        frm->send[4]=1;                 //设置失败
        err = ES_ERR_DI;                //无效命令ID  
    }
    else
    {
    
        tbl = stRFCMDTbl + index;
        if(tbl->func == __NULL)
        {           
            frm->len = 5;
            frm->send[4]=1;             //设置失败
            err = ES_ERR_FT;           //如果数据标识不支持读,直接回复错误帧
        }
        else
        {                               //执行读操作
            err = ((FW_COMMAD_CONF_t)tbl->func)(tbl, frm);
        }
        
    } 

    if(err == ES_ERR_NOREPLY)               //不需要回复数据帧,直接退出
    {        
        return ES_ERR_NOREPLY;
    }

    frm->send-=3;
    PST_RF_Pack(frame, err, &length);     //打包数据帧

    length+=1;
    gfs_PSTChnSend[frame->chninfo & PST_FRM_CHNNO](frm->send, length);

    ES_OK();
}


/************************************************************************
 * @function: PST_RF_Pack
 * @描述: 对智源帧进行打包,被动通讯
 * 
 * @参数: 
 * @param: frame 打包的结构体
 * @param: err 
 * @param: length 输入为打包缓存的最大长度,输出为打包好之后的长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: lgy (2015/11/23)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PST_RF_Pack(PST_Frame* frame, uint8 err, uint16* length)
{
    RF_Frame* frm = &frame->_rf;
    
    frm->send[1] = frm->len>>8;         //长度
    frm->send[2] = frm->len ;           //长度
    frm->send[3] = frm->FrameType + 0x80;//命令字
                                        //打包操作
    *length = PST_DoneFrame(PST_FRM_CONF_NO, frm->send);

    if(*length > 0)                     //打包长度>0
    {
        ES_OK();
    }
    return ES_ERR_FT;                  //返回发生致命错误
}

/************************************************************************
 * @function: PstRF_Reply
 * @描述: RF协议应答处理
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: void  
 * @说明: 
 * @作者: lgy (2015/12/16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void PstRF_Reply(uint16 cmd, uint8 value)
{
    uint8 send[16];
    uint16 length;
    
    send[1] = 0;         //长度
    send[2] = 5 ;        //长度
    send[3] = 0x88;      //命令字
    send[4] = 0x01;    
    send[5] = cmd>>8;  
    send[6] = cmd ;
    if(value == TRUE)
    {
        send[7] = 0;
    }
    else
    {
        send[7] = 1;
    }
    length = PST_DoneFrame(PST_FRM_CONF_NO, send);
    gfs_PSTChnSend[PST_CHN_SET](send, length);
}
