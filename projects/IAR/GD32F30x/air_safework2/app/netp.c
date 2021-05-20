/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: netp.c
**创   建   人: yzy
**最后修改日期: 2007年9月5日
**描        述: 配变程序组网通讯端口的通用函数
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2007年9月5日
** 描　述: 原始版本
********************************************************************************************************/
#define EXT_NETP
//	#include "es.h"
//	#include "hal.h"
//	#include "bsp.h"
//	#include "app.h"
//	#include "apis.h"
//	#include "extapi.h"


#include "public.h"
#include "lib_com/calclib_gw.h"

#include "netp.h"
#include "hlv.h"
//	#include "netp_ditbl.c"
#include "paradef.h"
#include "pst_zy.h"
#include "evt.h"
#include "netp_fm.h"
#include "task.h"
#define  NETPREADCNT     2              //单个标识的重读次数

cpu_stack_t  gs_TKNetpStack[TASK_NETP_STKL];
ktask_t      gs_TKNetpHandle;
kbuf_queue_t gs_TKNetpQueue;
char         gc_TKNetpQbuf[MSG_BUFF_LEN];
static char         g_TKNetpQ_buf_recv[MSG_BUFF_LEN];
static size_t       g_TKNetpQ_recv_size;

extern kbuf_queue_t gs_TKNetp2Queue;
extern kbuf_queue_t gs_TKNetp3Queue;
extern kbuf_queue_t gs_TKNetp4Queue;


//netp端口号与实际串口序号的对应关系
#if (MCB_PORT_NUM == 4)
const uint8 NETP_PHY_PORT[MCB_PORT_NUM] = {NETP_PORT_NO, NETP2_PORT_NO, NETP3_PORT_NO, NETP4_PORT_NO};
#elif (MCB_PORT_NUM == 3)
const uint8 NETP_PHY_PORT[MCB_PORT_NUM] = {NETP_PORT_NO, NETP2_PORT_NO, NETP3_PORT_NO};
#elif(MCB_PORT_NUM == 2)
const uint8 NETP_PHY_PORT[MCB_PORT_NUM] = {NETP_PORT_NO, NETP2_PORT_NO};
#elif(MCB_PORT_NUM == 1)
const uint8 NETP_PHY_PORT[MCB_PORT_NUM] = {NETP_PORT_NO};
#endif



/************************************************************************
 * @function: BG_PackRead
 * @描述: 打包数据读取帧
 * 
 * @参数: 
 * @param: buffer
 * @param: di 
 * @param: tstno 
 * 
 * @返回: 帧长度
 * @return: uint8  
 * @说明: 就是在BG_Pack()基础上简单打包下
 * @作者: yzy (2011-9-5)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint16 BG_PackRead(uint8* buffer, uint32 di, uint8 tstno)
//	{
//	    PST_Frame frame;
//	    DLT_Frame* frm = &(frame._dlt);
//	    uint16 len;
//	
//	    frm->cmd = 0x01;                                     //控制字
//	    frm->atr = 0;                                        //属性
//	    frm->send = buffer;                                  //发送起始地址
//	    frm->wd.word[0] = di;                                //数据标识
//	    frm->wd.word[1] = 0xFFFF;
//	    frm->len = 2;                                        //长度
//	                                                        //部规打包
//	//	    if(BG_Pack(frm, gss_CDATATst[tstno].ad, &len) != PST_ERR_OK)
//	//	    {
//	//	        return 0;
//	//	    }
//	    return len;
//	}



/************************************************************************
 * @function: XBG_PackRead
 * @描述: 打包数据读取帧
 * 
 * @参数: 
 * @param: buffer 
 * @param: di 
 * @param: tstno 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 就是在XBG_Pack()基础上简单打包下
 * @作者: yzy (2011-9-5)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint16 XBG_PackRead(uint8* buffer, uint32 di, uint8 tstno)
//	{
//	    PST_Frame frame;
//	    DLT_Frame* frm = &(frame._dlt);
//	    uint16 len;
//	
//	    frm->cmd = 0x11;                                     //控制字
//	    frm->atr = 0;                                        //属性
//	    frm->send = buffer;                                  //发送起始地址
//	    frm->wd.word[0] = di;                                //数据标识
//	    frm->wd.word[1] = di >> 16;
//	    frm->len = 4;                                        //数据区长度
//	                                                        //新部规打包
//	//	    if(XBG_Pack(frm, gss_CDATATst[tstno].ad, &len) != PST_ERR_OK)
//	//	    {
//	//	        return 0;
//	//	    }
//	    return len;
//	}






/************************************************************************
 * @function: BG_ExtractData
 * @描述: 提取有效数据内容
 * 
 * @参数: 
 * @param: buffer 接收数据缓存,传出有效数据内容
 * @param: length 传入为接收缓存长度,传出为有效数据内容长度
 * @param: di 标识(空表示不判标识)
 * 
 * @返回: 
 * @return: uint8 NETP_ERR_OK,NETP_ERR_NT,NETP_ERR_FT
 * @说明: 
 * @作者: yzy (2011-9-7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 BG_ExtractData(uint8* buffer, uint16* length, uint32 di)
//	{
//	    DLT_Frame frm;
//	    uint16 frmdi;
//	
//	    *length = PST_CheckFrame2(PST_FRM_BG_NO, buffer, *length);
//	    if(*length)
//	    {                                                   //解析收到的数据
//	//	        BG_Decode(buffer, &frm, __NULL);
//	
//	        if(frm.cmd & 0x40)                              //1.不支持该命令
//	        {
//	            (*length) = 0;
//	            return NETP_ERR_NT;
//	        }
//	                                                //2.判断数据标识
//	        if(di != 0)
//	        {
//	            frmdi = ByteToLong(frm.recv);
//	            if((uint32)frmdi != di)
//	            {
//	                return NETP_ERR_FT;
//	            }
//	        }
//	
//	        if(frm.len > (*length) + 2)                     //转移读取到的数据
//	        {
//	            MoveBuffer(frm.recv + 2, buffer, (*length));
//	        }
//	        else
//	        {
//	            MoveBuffer(frm.recv + 2, buffer, frm.len - 2);
//	            (*length) = frm.len - 2;
//	        }
//	
//	        SYS_OK();
//	    }
//	
//	    (*length) = 0;
//	    return NETP_ERR_FT;
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: XBG_ExtractData
//	 * @描述: 提取有效数据内容
//	 * 
//	 * @参数: 
//	 * @param: buffer 接收数据缓存,传出有效数据内容
//	 * @param: length 传入为接收缓存长度,传出为有效数据内容长度
//	 * @param: di 标识(空表示不判标识)
//	 * 
//	 * @返回: 
//	 * @return: uint8 NETP_ERR_OK,NETP_ERR_NT(有回复,但不支持),NETP_ERR_FT
//	 * @说明: 
//	 * @作者: yzy (2011-9-7)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint8 XBG_ExtractData(uint8* buffer, uint16* length, uint32 di)
//	{
//	    DLT_Frame frm;
//	    uint32 frmdi;
//	
//	    *length = PST_CheckFrame2(PST_FRM_BG_NO, buffer, *length);
//	    if(*length)
//	    {                                                   //解析收到的数据
//	//	        BG_Decode(buffer, &frm, __NULL);
//	        if(frm.cmd == 0x94)
//	        {
//	            return SYS_ERR_OK;
//	        }
//	        if(frm.cmd & 0x40)                              //不支持该命令
//	        {
//	            (*length) = 0;
//	            return NETP_ERR_NT;
//	        }
//	                                                //2.判断数据标识
//	        if(di != 0)
//	        {
//	            frmdi = ByteToLong(frm.recv);
//	            if(frmdi != di)
//	            {
//	                return NETP_ERR_FT;
//	            }
//	        }
//	
//	        if(frm.len > (*length) + 4)                     //转移读取到的数据
//	        {
//	            MoveBuffer(frm.recv + 4, buffer, (*length));
//	        }
//	        else
//	        {
//	            MoveBuffer(frm.recv + 4, buffer, frm.len - 4);
//	            (*length) = frm.len - 4;
//	        }
//	
//	        SYS_OK();
//	    }
//	
//	    (*length) = 0;
//	    return NETP_ERR_FT;
//	}
void Netp_ReadFinish(uint8 index);

/************************************************************************
 * @function: Netp_PentTran
 * @描述: 组网端口数据透明传输
 * 
 * @参数: 
 * @param: buffer 收发缓存
 * @param: length 收发数据内容长度
 * @param: ss 端口配置
 * @param: port 端口号(0-!)
 * @param: overtime 等待时间,单位100ms
 * @param: nfrm 通讯的规约编号,0xff表示为中继
 * 
 * @返回: 
 * @return: uint8  SYS_ERR_OK, SYS_ERR_FT
 * @说明: 
 * @作者: yzy (2011-9-8)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Netp_PentTran(uint8* buffer, uint16* length, uart_config_t * ss, uint16 overtime, uint8 port)
{
    uint8 phyport;
    uint16 ulen = 0, plen = 0;
    uint8_t protocol;
    if(port >= MCB_PORT_NUM)
    {
        return NETP_ERR_FT;
    }
    phyport = NETP_PHY_PORT[port];
//	    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);//LED_485_T
                                                        //打开串口
    
    //phyport = NETP_PORT_NO;
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);           //亮一下

//	    ss->baudrate = NETP_PORT_BAUD;
//	    ss->databits = DataBits_8bits;
//	    ss->parit = Parit_N;
//	    ss->stopbits = StopBits_1;
//	                                            //打开串口
//	    gs_GPIO.GPO_Out(GPO_485_PWR,true);
    Netp_PowerMng(1, NETP_PORT_PWR_TRANS);

    aos_msleep(200);
    gs_Uart.open(phyport, false);
    gs_Uart.Init(phyport, ss);
    gs_Uart.Clear(phyport);                            //清空串口缓存,并发送
    gs_Uart.Write(phyport, buffer, *length,1000);
    ulen = 0;                                        //接收长度为0
    while(overtime--)                                   //在规定的延时期间内进行有效帧的判断
    {
        aos_msleep(100);                             //延迟100ms          
        if(gs_Uart.ToBeRead(phyport) > 0)               //判断是否有数据收到
        {
            ulen += gs_Uart.Read(
                                     phyport, 
                                     buffer + ulen, 
                                     256 - (ulen & 0xFF),
                                     1000);

            ulen &= 0xFF;                          //数据长度不超过256
            gs_GPIO.BlinkSet(GPIO_LED_485, 1, 2, 5);
                                                        //判断帧的有效性                                         
//	            if(PST_Channel(port, buffer, ulen, &protocol) == PST_ERR_OK)
//	            {
//	                gs_GPIO.BlinkSet(GPIO_LED_485, 2, 0, 0);
//	                if(ulen > 0)
//	                {
//	                    gs_Uart.close(phyport);                             //释放串口
//	                    *length = ulen;
//	                    SYS_OK();
//	                }
//	            }
        }
        else
        {
            if(ulen > 0)//数据接收完毕，因为不确定是什么协议，所以没法判断帧协议
            {
                gs_Uart.Close(phyport);                             //释放串口
                gs_Uart.Clear(phyport);
                *length = ulen;
                gs_GPIO.BlinkSet(GPIO_LED_485, 3, 0, 0);
                
                Netp_PowerMng(0, NETP_PORT_PWR_TRANS);
                SYS_OK();
	
            }
        }
    }
    gs_Uart.Close(phyport);                             //释放串口
    *length = 0;
    gs_GPIO.BlinkSet(GPIO_LED_485, 3, 0, 0);
    Netp_PowerMng(0, NETP_PORT_PWR_TRANS);
    
    return NETP_ERR_FT;                             //超时无数据恢复
}

int Netp_PowerMng(uint8_t onoff, uint8_t flag)
{

    if(onoff)
    {
        gs_SysVar.netpTsk |= (1 << flag);
        gs_GPIO.GPO_Out(GPO_485_PWR, true);
    }
    else
    {
        gs_SysVar.netpTsk &= ~(1 << flag);
        if(gs_SysVar.netpTsk == 0)
        {
            if(gs_SysVar.mDGstt & HLV_STT_NENG)
                gs_GPIO.GPO_Out(GPO_485_PWR, false);
        }
    }


    return SYS_ERR_OK;
}

/************************************************************************
 * @function: Netp_Send
 * @描述: 组网端口透明传输(按测量点)
 * 
 * @参数: 
 * @param: phyport 端口
 * @param: buffer 收发缓存
 * @param: len 发送长度
 * @param: overtime 超时时间, 单位100ms
 * @param: ss 串口控制字
 * @param: fw 帧配置
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2011-11-2)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Netp_Send(uint8_t phyport, uint8* buffer, uint16* len, 
                    uint8 overtime, uart_config_t *ss, NetpFrameStr * frameStr)
{
    //scm_dlms_t* scm = &gs_scmDlms;
//	    SerialSets ss;
//	    uint8 phyport;
                                            //端口配置
   /* if(gss_CDATATst[tstno].pt >= MCB_PORT_NUM)
    {
        return NETP_ERR_FT;
    }*/
//	    CDATATst * pt = 
//	    phyport = NETP_PORT_NO;
    //uint8_t p[4] = {0xFE, 0xFE, 0xFE, 0xFE};
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);           //亮一下

//	    ss.baudrate = NETP_PORT_BAUD;
//	    ss.databits = DataBits_8bits;
//	    ss.parit = Parit_N;
//	    ss.stopbits = StopBits_1;
                                            //打开串口
//	    gs_GPIO.GPO_Out(GPO_MBUS_PWR,false);
    gs_Uart.open(phyport, false);
    gs_Uart.Init(phyport, ss);
    gs_Uart.Clear(phyport);
    aos_msleep(100);
    //gs_Uart.Write(phyport, p, 4, 100);
    gs_Uart.Write(phyport, buffer, *len, 1000);   //发送数据

    *len = 0;                                            //接收长度为0
    while(overtime--)                                   //在规定的延时期间内进行有效帧的判断
    {
        aos_msleep(10);                             //判断是否有数据收到
//	        if(gs_Uart.ToBeRead(phyport) > 0)
        {
           *len += gs_Uart.Read(
                           phyport, 
                           buffer + *len, 
                           256 - (*len & 0x3ff),
                           1000);
            *len &= 0x3ff;                                //数据在一个字节以内的
                                                        //判断帧的有效性 
            if(NETP_CheckFrame2(frameStr, buffer, len) == PST_ERR_OK)
            {    
                //关闭串口
                gs_GPIO.BlinkSet(GPIO_LED_485, 1, 2, 5);    //亮
                gs_Uart.Close(phyport);
                aos_msleep(50);
                
                SYS_OK();
            }
        }
    }
    gs_Uart.Close(phyport);                             //释放串口
    gs_GPIO.BlinkSet(GPIO_LED_485, 3, 0, 0);  //灭
    
    return NETP_ERR_FT;
}
#define CON_NETP_TST_LEN 16
#define CON_NETP_485_STT_BIT 28
uint8 Netp_Check(uint8_t port)
{
    uint8_t tstdat[CON_NETP_TST_LEN]={0x12,0x34,0x56,0x78,0x87,0x65,0x43,0x21,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
    uint8_t buff[CON_NETP_TST_LEN];
    uint8_t phyport = NETP_PORT_NO + port;
    uint8_t len = 0;
    uart_config_t ss;
    uint8_t overtime = 0xff;
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);           //亮一下
    PentCfgToSS(0x18, &ss);

    LOG_DEBUG("check 485-%d start !!!\n", port+1);
    aos_msleep(1500 + port*500);
    gs_Uart.open(phyport, false);
    gs_Uart.Init(phyport, &ss);
    gs_Uart.Clear(phyport);
                                            
    gs_Uart.Write(phyport, tstdat, sizeof(tstdat), 1000);   //发送数据
    memset(buff, 0, CON_NETP_TST_LEN);

    while(overtime--) //在规定的延时期间内进行有效帧的判断
    {
        aos_msleep(10);//判断是否有数据收到
        
        if(gs_Uart.ToBeRead(phyport) > 0)
        {
            len += gs_Uart.Read(
                       phyport, 
                       buff+len, 
                       CON_NETP_TST_LEN-len,
                       1500);
            
            LOG_DEBUG("check 485-%d data:\n", port+1);
            LOG_DUMPHEX(LOG_LEVEL_DEBUG, "\ncheck:\n", buff, CON_NETP_TST_LEN);
            if(len >= CON_NETP_TST_LEN && 0 == memcmp(buff, tstdat,CON_NETP_TST_LEN))
            {
                gs_SysVar.terstt.lword &= ~(1 << (CON_NETP_485_STT_BIT + port));
                gs_GPIO.BlinkSet(GPIO_LED_485, 1, 2, 5);//亮
                gs_Uart.Close(phyport);
                aos_msleep(50);
                
                LOG_DEBUG("check 485-%d  success!!!\n", port+1);
                SYS_OK();
            
            }
        }
    }
    gs_Uart.Close(phyport);//释放串口
    gs_GPIO.BlinkSet(GPIO_LED_485, 3, 0, 0);//灭
    gs_SysVar.terstt.lword |= (1 << (CON_NETP_485_STT_BIT + port));
    
    LOG_DEBUG("check 485-%d  failed???\n", port+1);
    return NETP_ERR_FT;
}


//	uint8 Netp_Trans2(uint8* buffer, uint16* len, uint8 overtime, ST_Netp_FW * fw)
//	{
//	
//	}
uint8 Netp_Trans(uint8* buffer, uint16* len, uint8 overtime, uint8 nfrm)
{
    //scm_dlms_t* scm = &gs_scmDlms;
    SerialSets ss;
    uint8 phyport;
                                            //端口配置
   /* if(gss_CDATATst[tstno].pt >= MCB_PORT_NUM)
    {
        return NETP_ERR_FT;
    }*/
//	    CDATATst * pt = 
    phyport = NETP_PORT_NO;
    //gs_GPIO.LED_BlinkSet(LED_485_T, 0, 0, 0);           //亮一下

    ss.baudrate = NETP_PORT_BAUD;
    ss.databits = DataBits_8bits;
    ss.parit = Parit_N;
    ss.stopbits = StopBits_1;
                                            //打开串口
//	    gs_GPIO.GPO_Out(GPO_MBUS_PWR,false);
    gs_Uart.open(phyport, false);
    gs_Uart.Init(phyport, &ss);
    gs_Uart.Clear(phyport);
                                            
    gs_Uart.Write(phyport, buffer, *len, 1000);   //发送数据

    *len = 0;                                            //接收长度为0
    while(overtime--)                                   //在规定的延时期间内进行有效帧的判断
    {
        aos_msleep(10);                             //判断是否有数据收到
//	        if(gs_Uart.ToBeRead(phyport) > 0)
        {
           *len += gs_Uart.Read(
                           phyport, 
                           buffer + *len, 
                           1024 - (*len & 0x3ff),
                           1000);
            *len &= 0x3ff;                                //数据在一个字节以内的
                                                        //判断帧的有效性 
            if(PST_CheckFrame3(nfrm, buffer, len) == PST_ERR_OK)
            {    
                //关闭串口
                //scm->recerr = 0;
                gs_GPIO.BlinkSet(GPIO_LED_485, 2, 0, 0);    //亮
                gs_Uart.Close(phyport);
                aos_msleep(50);
                
                SYS_OK();
            }
        }
    }
    gs_Uart.Close(phyport);                             //释放串口
//	    scm->recerr++;
//	    if(scm->recerr > 20)    //连续多次无法收到正确的数据帧，重新启动MBUS模块
//	    {
//	        scm->recerr = 0;
//	        gs_GPIO.GPO_Out(GPO_485_PWR,true);//关闭MBUS电源
//	        aos_msleep(350);
//	        gs_GPIO.GPO_Out(GPO_485_PWR,false);//打开MBUS电源
//	        aos_msleep(350);
//	    }
    gs_GPIO.BlinkSet(GPIO_LED_485, 3, 0, 0);  //灭
    
    return NETP_ERR_FT;
}







/************************************************************************
 * @function: Netp_ReadBG
 * @描述: 组网端口部规数据读取处理
 * 
 * @参数: 
 * @param: buffer 接收数据缓存
 * @param: di 部规标识
 * @param: length 接收数据的预计长度
 * @param: tstno 测量点号
 * 
 * @返回: 
 * @return: uint8  NETP_ERR_OK,NETP_ERR_NT(有回复,但不支持),NETP_ERR_FT
 * @说明: 
 * @作者: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 Netp_ReadBG(uint8* buffer, uint32 di, uint16 length, uint8 tstno)
//	{
//	    uint8 tmp[256];
//	    uint16 len;
//	    uint8 err;
//	                                            //打包
//	    len = BG_PackRead(tmp, di, tstno);
//	    if(len == 0)
//	    {
//	        return NETP_ERR_FT;
//	    }
//	                                            //数据传输通讯
//	    if(Netp_Trans(tmp, &len, 40, PST_FRM_BG_NO) != NETP_ERR_OK)
//	    {
//	        return NETP_ERR_FT;
//	    }
//	                                            //提取数据内容
//	    len = sizeof(tmp);
//	    err = BG_ExtractData(tmp, &len, di);
//	    if(err != NETP_ERR_OK)
//	    {
//	        return err;
//	    }
//	
//	    if(len > length)                                    //实际读取长度大于请求长度
//	    {
//	        MoveBuffer(tmp, buffer, length);
//	    }
//	    else
//	    {                                                   //转移读取到的数据
//	        memset(buffer, 0, length);
//	        MoveBuffer(tmp, buffer, len);
//	    }
//	
//	    SYS_OK();
//	}
//	


//	uint8 Netp_BGRead(uint8* buffer, uint32 di, uint16 length, uint8 tstno)
//	{
//	    uint8 err;
//	    for(uint8 uc_i = 0; uc_i < NETPREADCNT; uc_i++)
//	    {
//	        err = Netp_ReadBG(buffer, di, length, tstno);
//	        if((err == NETP_ERR_OK) || (err == NETP_ERR_NT))
//	        {
//	            break;
//	        }
//	    }
//	    return err;
//	}




/************************************************************************
 * @function: Netp_ReadXBG
 * @描述: 组网端口部规数据读取处理
 * 
 * @参数: 
 * @param: buffer 接收数据缓存
 * @param: di 数据标识
 * @param: length 接收数据的预计长度
 * @param: tstno 测量点号
 * 
 * @返回: 
 * @return: uint8  NETP_ERR_OK,NETP_ERR_NT(有回复,但不支持),NETP_ERR_FT
 * @说明: 
 * @作者: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 Netp_ReadXBG(uint8* buffer, uint32 di, uint16 length, uint8 tstno)
//	{
//	    uint8 tmp[256];
//	    uint16 len;
//	    uint8 err;
//	                                            //打包
//	    len = XBG_PackRead(tmp, di, tstno);
//	    if(len == 0)
//	    {
//	        return NETP_ERR_FT;
//	    }
//	                                            //数据传输通讯
//	    if(Netp_Trans(tmp, &len, 40, PST_FRM_BG_NO) != NETP_ERR_OK)
//	    {
//	        return NETP_ERR_FT;
//	    }
//	                                            //提取数据内容
//	    len = sizeof(tmp);
//	    err = XBG_ExtractData(tmp, &len, di);
//	    if(err != NETP_ERR_OK)
//	    {
//	        return err;
//	    }
//	
//	    if(len > length)                                    //实际读取长度大于请求长度
//	    {
//	        MoveBuffer(tmp, buffer, length);
//	    }
//	    else
//	    {                                                   //转移读取到的数据
//	        memset(buffer, 0, length);
//	        MoveBuffer(tmp, buffer, len);
//	    }
//	
//	    SYS_OK();
//	}


//	uint8 Netp_XBGRead(uint8* buffer, uint32 di, uint16 length, uint8 tstno)
//	{
//	    uint8 err;
//	    for(uint8 uc_i = 0; uc_i < NETPREADCNT; uc_i++)
//	    {
//	        err = Netp_ReadXBG(buffer, di, length, tstno);
//	        if((err == NETP_ERR_OK) || (err == NETP_ERR_NT))
//	        {
//	            break;
//	        }
//	    }
//	    return err;
//	}





/************************************************************************
 * @function: Netp_Read
 * @描述: 组网端口读数据处理
 * 
 * @参数: 
 * @param: buffer 接收数据缓存
 * @param: di 抄读的标识
 * @param: length 接收数据的长度
 * @param: tstno 测量点号
 * 
 * @返回: 
 * @return: uint8  NETP_ERR_OK,NETP_ERR_NT(有回复,但不支持),NETP_ERR_FT
 * @说明: 
 * @作者: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 Netp_Read(uint8* buffer, uint32 di, uint16 length, uint8 tstno)
//	{
//	     switch(gss_CDATATst[tstno].pf)          //根据通讯规约分别处理
//	    {
//	       case TST_PFID_DLT97:                          //部规1997
//	            return Netp_BGRead(buffer, di, length, tstno);
//	        case TST_PFID_DLT07:                          //部规2007
//	            return Netp_XBGRead(buffer, di, length, tstno);
//	        default:
//	            return NETP_ERR_FT;
//	    }
//	    //return Netp_DLMSRead(buffer, length, tstno, NETP_TYPE_GET, inbuf)
//	}








const uint8_t netp_check[4]={0xa5,0xa5,0xa5,0xa5};

/************************************************************************
 * @function: Netp_PreInit
 * @描述: 抄表端口初始化
 * 
 * @参数: 
 * @param: index 端口编号
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_PreInit(uint8 index)
{
    uint8_t temp[4];

    if(index >= MCB_PORT_NUM)
    {
        return;
    }
                                        //清除抄表状态
    ClearBuffer((uint8*)&gss_NetpPortStt[index], sizeof(NETPSTR));
                                        //清除抄表统计
    ClearBuffer((uint8*)gs_NetpReadStt, sizeof(gs_NetpReadStt));
    gs_GPIO.GPO_Out(GPO_485_PWR,false);
    gs_PstPara.cp_interval=300;

    extern const ST_Netp_FW* const gss_NetpFrameFw[];
    memset(temp,0,4);
    if(GD_Para_RW(NETP_CHECK, temp, 4, false) == SYS_ERR_OK)
    {
#ifndef DEBUG    
        if(memcmp(netp_check, temp, 4) != 0)
#endif            
        {
            for(int i = 0; i < Netp_Get_FW_num(); i++)
            {
                Netp_Register_Set(gss_NetpFrameFw[i]);
            }    
            GD_Para_RW(NETP_CHECK, netp_check, 4, true);
        }
    }
//	    Netp_MtUpdataParaInit();
}












/************************************************************************
 * @function: InitNetpRB
 * @描述: 启动抄表前的状态机复位操作
 * 
 * @参数: 
 * @param: rb 状态结构体
 * @返回: 
 * @说明: 
 * @作者: yzy (2011-3-14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void InitNetpRB(NETPSTR* rb)
//	{
//	    NETPRD* tbl;
//	    uint8 method;                           //标记是否需要抄读
//	
//	    rb->idx = 0;
//	    rb->prio = 1;                           //默认无必抄项
//	
//	    ClearBuffer(rb->bmp_m, SIZE_NETPBMP);
//	    ClearBuffer(rb->bmp_n, SIZE_NETPBMP);
//	    for(uint8 uc_i = 0; uc_i < rb->tbllen; uc_i++)
//	    {
//	        tbl = rb->tbl + uc_i;
//	        if(tbl->patr & NTRD_PATR_MUST)
//	        {
//	            STR_SetBit(rb->bmp_m, uc_i);
//	            rb->prio = 0;                   //存在必抄项
//	        }
//	        else
//	        {                                   //根据配置判断该标识是否需要抄读
//	            method = IsNetpDIRead(rb->tstno, tbl);
//	            if((method == 0) || (method == 2))
//	            {
//	                continue;
//	            }
//	                                            //将该标识在待抄读位图中加以标记
//	            if(tbl->zhtp == NTRD_ZHTP_MUFL)
//	            {
//	                if(tbl->zhid == 0xFF || tbl->zhid == 0)
//	                {                           //块抄/总
//	                    STR_SetBit(rb->bmp_n, uc_i);
//	                }
//	                else if(gss_CDATATst[rb->tstno].fl > 1)
//	                {                           //多费率
//	                    if(tbl->zhid <= gss_CDATATst[rb->tstno].fl)
//	                    {
//	                        STR_SetBit(rb->bmp_n, uc_i);
//	                    }
//	                }
//	            }
//	            else
//	            {
//	                STR_SetBit(rb->bmp_n, uc_i);
//	            }
//	        }
//	    }
//	}


/************************************************************************ 
**函数类型声明
*************************************************************************/
//	typedef void (*ftype_ntrd)(uint8*, NETPSTR*, uint8, uint8);


/************************************************************************
**函数名称: ReadDataToNetpRam
**描 述: 将通过集中器通道读取的数据放入内存数组
**
**输 入: nt 信息结构体 
**       buffer 读取的数据
**       cstrc 要放入的内存
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者:  yzy
**日 期:  11-01-25
*************************************************************************/
//	void ReadDataToNetpRam(uint8* buffer, NETPSTR* rb)
//	{
//	    uint32 val;
//	    uint32 mask;
//	    uint8* pbuf = buffer;
//	    NETPRD* nt = rb->tbl + rb->idx;
//	    uint8  issign;
//	                                            //循环处理读取的数据
//	    for(uint8 uc_j = 0; uc_j < nt->zhnum; uc_j++)
//	    {
//	        if(nt->datr & NTRD_DATR_SPEC)       //特殊处理
//	        {
//	            (*(ftype_ntrd)nt->func)(pbuf + nt->len * uc_j, rb, rb->tstno, uc_j);
//	            continue;
//	        }
//	
//	        if(nt->len == 1)                    //单字节数据
//	        {
//	            val = *(pbuf + uc_j);
//	        }
//	        else if(nt->len == 2)               //两字节数据
//	        {
//	            val = ByteToInt(pbuf + 2 * uc_j);
//	        }
//	        else if(nt->len == 3)               //三字节数据
//	        {
//	            val = ByteToInt24(pbuf + 3 * uc_j);
//	        }
//	        else if(nt->len == 4)               //四字节数据
//	        {
//	            val = ByteToLong(pbuf + 4 * uc_j);
//	        }
//	        else                                //直接转移并退出
//	        {
//	            MoveBuffer(pbuf, rb->data + nt->add, nt->len * nt->zhnum);
//	            break;
//	        }
//	                                            //带符号输入则先去除符号
//	        CLEAR_FLAG(issign);
//	        if(nt->datr & NTRD_DATR_INSG)
//	        {
//	            mask = 1 << ((nt->len << 3) - 1);
//	            if(val & mask)
//	            {
//	                val &= ~mask;
//	                SET_FLAG(issign);
//	            }
//	        }
//	                                            //需要装换成HEX
//	        if(nt->datr & NTRD_DATR_THEX)
//	        {
//	            if(IsBcd(val))
//	            {
//	                val = LongBcdToHex(val);
//	            }
//	            else
//	            {
//	                val = 0;
//	            }
//	        }
//	                                            //减少小数位
//	        if(nt->datr & NTRD_DATR_PDIV)
//	        {
//	            val /= guls_Do10Value[nt->dgt];
//	        }                                   //扩展小数位
//	        else if(nt->datr & NTRD_DATR_PMUL)
//	        {
//	            val *= guls_Do10Value[nt->dgt];
//	        }
//	                                            //带符号输出的要加上符号
//	        if(nt->datr & NTRD_DATR_OUSG)
//	        {
//	            if(issign)
//	            {
//	                val = -val;
//	            }
//	        }
//	
//	        if(nt->datr & NTRD_DATR_BEXT)
//	        {                                   //字节扩展成4字节,2->4,3->4
//	            LongToByte(rb->data + nt->add + 4 * uc_j, val);
//	        }
//	        else                                //否则直接转移
//	        {
//	            MoveBuffer(
//	                     (uint8*)&val, 
//	                     rb->data + nt->add + nt->len * uc_j,
//	                     nt->len
//	                     );
//	        }
//	    }
//	}






/************************************************************************
 * @function: Netp_GetTstData
 * @描述: 按照结构体信息抄读数据,解析数据.
 * 
 * @参数: 
 * @param: rb 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2011-3-18)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 Netp_GetTstData(NETPSTR* rb)
//	{
//	    uint8   buffer[256];
//	    NETPRD* nt;
//	    uint16  index;
//	    uint8   err;
//	    uint8   errcnt = 0;
//	    uint8   flag = 0;                       //标志性标识是否抄读成功
//	                                            //不支持终端测量点
//	    _IF_TRUE_RETURN_Z((rb->tstno == 0) || (rb->tstno >= CORE_TST_NO), NETP_ERR_FT);
//	
//	    InitNetpRB(rb);
//	    while(1)
//	    {                                       //获取待抄标识
//	        if(rb->prio == 0)
//	        {
//	            index = STR_GetThisNextSetBit(rb->bmp_m, SIZE_NETPBMP, rb->idx);
//	            if(index == 0xFFFF)
//	            {                               //必抄项失败,判断为抄表失败并退出
//	                _IF_TRUE_RETURN_Z(flag == 0, SYS_ERR_FT);
//	                rb->idx = 0;
//	                rb->prio++;
//	                continue;
//	            }
//	        }
//	        else
//	        {
//	            index = STR_GetThisNextSetBit(rb->bmp_n, SIZE_NETPBMP, rb->idx);
//	            if(index == 0xFFFF)
//	            {
//	                break;
//	            }
//	        }
//	
//	        rb->idx = index;
//	        nt = rb->tbl + rb->idx;
//	        STR_ClrBit(rb->bmp_m, rb->idx);
//	        STR_ClrBit(rb->bmp_n, rb->idx);
//	                                            //抄读并处理
//	        err = Netp_Read(buffer, nt->di, sizeof(buffer), rb->tstno);
//	        if(err == NETP_ERR_OK)              //正常回复
//	        {
//	            flag = 1;
//	            ReadDataToNetpRam(buffer, rb);
//	            for(uint8 uc_i = 0; uc_i < nt->zhnum; uc_i++)
//	            {
//	                STR_SetBit(rb->eft, nt->sbit + uc_i);
//	                if(nt->patr & NTRD_PATR_SKIP)//额外需要置的位
//	                {
//	                    STR_ClrBit(rb->bmp_m, rb->idx + 1 + uc_i);
//	                    STR_ClrBit(rb->bmp_n, rb->idx + 1 + uc_i);
//	                }
//	            }
//	            errcnt = 0;
//	        }
//	        else if(err == NETP_ERR_NT)          //回复拒绝帧
//	        {
//	            STR_ClrBit(rb->eft, nt->sbit);
//	            errcnt = 0;
//	        }
//	        else                                //通讯不通
//	        {
//	            STR_ClrBit(rb->eft, nt->sbit);
//	            if(errcnt++ > 5)                //连续多次抄表失败,判断为抄表失败并退出
//	            {
//	                return NETP_ERR_FT;
//	            }
//	        }
//	        rb->idx++;
//	    }
//	    SYS_OK();
//	
//	}


/************************************************************************
 * @function: gdw_Check97LastDayData
 * @描述: 确保97表内上日冻结数据的存在
 * 
 * @参数: 
 * @param: tstno 测量点号 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2011-10-7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 gdw_Check97LastDayData(uint8 tstno)
//	{
//	//	    S_DAYDATA_SEG0 daydata;
//	//	    bool flag;
//	//	                                                //97表,且抄表成功
//	//	    if((gss_CDATATst[tstno].pf != TST_PFID_DLT07) && (gss_ALRTStt[tstno].ew1.trn == 0))
//	//	    {                                           //读取上日冻结数据
//	//	        if(gdw_ReadDayData(gs_SysLastDayTime.YMDhms, tstno, 0, (uint8*)&daydata) != SYS_ERR_OK)
//	//	        {
//	//	            return SYS_ERR_FT;
//	//	        }
//	//	        
//	//	        flag = STR_IsBitSet(daydata.seg.eft, EFT_DAY_WP);
//	//	        if(!flag)                               //上日数据不存在
//	//	        {
//	//	            gdw_SaveTstDayData(tstno, 0);       //使用实时量冻结
//	//	        }
//	//	    }
//	    SYS_OK();
//	}
//	
//	#if (GDW_07DIN_EN > 0)
//	
//	
//	/************************************************************************
//	 * @function: gdw_Check07LastDayData
//	 * @描述: 使用07表内数据与上日冻结比较,若不一致已表内数据为准
//	 * 
//	 * @参数: 
//	 * @param: tstno 测量点号 
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 此处可更改为补抄近三日内数据.
//	 * @作者: yzy (2011-10-7)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	uint8 gdw_Check07LastDayData(uint8 tstno)
//	{
//	    NETPSTR netpstr;
//	    S_Tst07ExtraData tstdata;           //存放表计内历史日冻结数据
//	    S_DAYDATA_SEG0 daydata;             //存放终端内历史日冻结数据
//	    const S_DATASEGINFO* segstr;
//	    uint8 YMDhm[5];
//	    uint8 YMDhm_l[5];                   //上一天时标
//	    uint32 addr;
//	    uint16 bidx;
//	    uint8 flag = 0;                     //标志是否替换原数据
//	
//	    if((gss_CDATATst[tstno].pf == TST_PFID_DLT07) && (gss_ALRTStt[tstno].ew1.trn == 0))
//	    {
//	                                        //抄读前的准备操作
//	        ClearBuffer((uint8*)&tstdata, sizeof(tstdata));
//	        netpstr.data = (uint8*)&tstdata;
//	        netpstr.eft = tstdata.eft;
//	        netpstr.tstno = tstno;
//	        netpstr.tbl = (NETPRD*)gss_NetpXgExtra;
//	        netpstr.tbllen = LEN_XGEXTRA;
//	
//	        if(Netp_GetTstData(&netpstr) == SYS_ERR_OK)
//	        {
//	            tstdata.rtime = gs_SysTime;
//	                                        //表内冻结时间必须要有
//	            if(STR_IsBitClr(tstdata.eft, EFT_EXT_TSD))
//	            {
//	                return SYS_ERR_FT;
//	            }
//	                                        //顺序转换
//	            ByteArrayBcdToHex(tstdata.Tsd, YMDhm, 5);
//	            InvBuffer(YMDhm, 5);
//	            MoveBuffer(YMDhm, YMDhm_l, 5);
//	            StepDays(YMDhm_l, -1);      //提前到上一天.
//	                                        //根据表内冻结时间读取对应的终端冻结数据
//	            gdw_ReadDayData(YMDhm_l, tstno, 0, (uint8*)&daydata);
//	                                        //判断表计上日冻结时间是否为终端当日时间
//	            //if(CmpBuffer(YMDhm, gs_SysTime.YMDhms, 3))
//	            //{
//	            //    return SYS_ERR_FT;
//	            //}
//	                                       //准备结构体数据
//	            if(STR_IsBitSet(tstdata.eft, EFT_EXT_WPD))
//	            {
//	                if(CmpBuffer((uint8*)tstdata.Wpd, (uint8*)daydata.seg.Wp, sizeof(daydata.seg.Wp)) || STR_IsBitClr(daydata.seg.eft, EFT_DAY_WP))
//	                {
//	                    MoveBuffer((uint8*)tstdata.Wpd, (uint8*)daydata.seg.Wp, sizeof(daydata.seg.Wp));
//	                    STR_SetBit(daydata.seg.eft, EFT_DAY_WP);
//	                    flag = 1;
//	                }
//	            }
//	
//	            if(STR_IsBitSet(tstdata.eft, EFT_EXT_WND))
//	            {
//	                if(CmpBuffer((uint8*)tstdata.Wnd, (uint8*)daydata.seg.Wn, sizeof(daydata.seg.Wn)) || STR_IsBitClr(daydata.seg.eft, EFT_DAY_WN))
//	                {
//	                    MoveBuffer((uint8*)tstdata.Wnd, (uint8*)daydata.seg.Wn, sizeof(daydata.seg.Wn));
//	                    STR_SetBit(daydata.seg.eft, EFT_DAY_WN);
//	                    flag = 1;
//	                }
//	            }
//	                                        //保存替换数据后的结构体
//	            if(flag)
//	            {                           //ADDRESS(DIVIDED BY TST)
//	                BuildTimeLabel(TTYPE_YMDHMS, YMDhm, &daydata.seg.stime);
//	                daydata.seg.rtime = gs_SysTime;
//	                                        //保存到上一天日冻结
//	                addr = _DayDataAddr(YMDhm_l, tstno, 0, &segstr, &bidx);
//	                if(addr == 0xffffffff)
//	                {
//	                    return SYS_ERR_FT;
//	                }
//	                WriteFlashWithCRC(DB_DAY, (uint8*)&daydata, segstr->seglen, addr);
//	                STR_ClrFlashBit(DB_DAY, 0, bidx, 0xffff);//0有效,1无效
//	            }
//	        }
//	    }
//	    SYS_OK();
//	}
//	
//	#endif









/************************************************************************
 * @function: Netp_ReadTstRealData
 * @描述: 读取读取实时数据
 * 
 * @参数: 
 * @param: port 端口号(0-!)
 * @返回: 
 * @说明: 
 * @作者: yzy (2011-11-2)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Netp_ReadTstRealData(uint8 port)
//	{
//	    _IF_TRUE_RETURN_VOID(port >= MCB_PORT_NUM);
//	                                            //外部测量点循环读取处理
//	    for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++)
//	    {
//	        if(gs_InpExtracb[port].gbbreak)
//	        {
//	            return;
//	        }
//	                                            //要求:1.测量点有效,2.485表
//	        if((gss_CDATATst[uc_i].ef != 0x01) || (gss_CDATATst[uc_i].tf != 0x01) || (gss_CDATATst[uc_i].pt != port))
//	        {
//	            continue;
//	        }
//	
//	        //HB_RetLive();                  //抄表期间主动保活
//	                                            //赋值抄表内容结构体
//	        NETPSTR netpstr;
//	        S_TstRealData tstdata;
//	        ClearBuffer((uint8*)&tstdata, sizeof(tstdata));
//	        netpstr.data = (uint8*)&tstdata;
//	        netpstr.eft = tstdata.eft;
//	        netpstr.tstno = uc_i;
//	        if(gss_CDATATst[uc_i].pf == TST_PFID_DLT97)      //645-1997
//	        {
//	            netpstr.tbl = (NETPRD*)gss_NetpBgTstData;
//	            netpstr.tbllen = LEN_BGTSTDATA;
//	        }
//	        else if(gss_CDATATst[uc_i].pf == TST_PFID_DLT07) //645-2007
//	        {
//	            netpstr.tbl = (NETPRD*)gss_NetpXgTstData;
//	            netpstr.tbllen = LEN_XGTSTDATA;
//	        }
//	
//	        if(Netp_GetTstData(&netpstr) != SYS_ERR_OK)
//	        {
//	            _RefreshTrnFlag(uc_i, false);
//	
//	            //清除实时数据有效标记
//	            ClearBuffer(gss_TstRealData[uc_i].eft, sizeof(gss_TstRealData[uc_i].eft));
//	            if(gss_TstRealData[uc_i].errcnt < 0xFF)
//	            {
//	                gss_TstRealData[uc_i].errcnt++;
//	            }
//	            gss_TstRealData[uc_i].rtime = gs_SysTime;
//	        }
//	        else
//	        {
//	            _RefreshTrnFlag(uc_i, true);
//	
//	            //进行事件变更判断  
//	            //EVT_ERC12_Proc(&tstdata, uc_i); //时间超差判断
//	            //EVT_ERC27_28_29_30_Proc(&tstdata, uc_i);//电量异常判断
//	            
//	            //存储实时数据
//	            tstdata.errcnt = gss_TstRealData[uc_i].errcnt;
//	            MoveBuffer((uint8*)&tstdata, (uint8*)&gss_TstRealData[uc_i], sizeof(S_TstRealData));
//	            gss_TstRealData[uc_i].rtime = gs_SysTime;
//	
//	            //电压统计
//	            //STAT_CBVoltage(uc_i, gs_InpExtracb[port].netitv);
//	        }
//	    }
//	
//	    
//	}



/************************************************************************
 * @function: Netp_ReadTstDemandData
 * @描述:  更新FLASH需量数据(为了用到时不临时去抄读,比如日月冻结)
 * 
 * @参数: 
 * @param: port 
 * @返回: SYS_ERR_FT, SYS_ERR_OK
 * @说明: 
 * @作者: yzy (2014/3/4)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Netp_ReadTstDemandData(uint8 port)
//	{
//	    uint8 uc_i;
//	    S_TstDmdData* pdata = &gs_tstdmd[port];
//	
//	    _IF_TRUE_RETURN_VOID(port >= MCB_PORT_NUM);
//	    
//	    for(uc_i = 1; uc_i < CORE_TST_NO; uc_i++)
//	    {
//	        if(gs_InpExtracb[port].gbbreak)
//	        {
//	            return;
//	        }
//	                                            //要求:1.测量点有效,2.485表
//	//	        if((gss_CDATATst[uc_i].ef != 0x01) || (gss_CDATATst[uc_i].pt != port) || (gss_ALRTStt[uc_i].ew1.trn == 0x01))
//	//	        {
//	//	            continue;
//	//	        }
//	                                            //要求:2.485或交采测量点
//	        if((gss_CDATATst[uc_i].tf != 0x01) && (gss_CDATATst[uc_i].tf != 0x05))
//	        {
//	            continue;
//	        }
//	
//	        //HB_RetLive();                              //抄表期间主动保活
//	                                            //赋值抄表内容结构体
//	        NETPSTR netpstr;
//	        ClearBuffer((uint8*)pdata, sizeof(S_TstDmdData));
//	        netpstr.data = (uint8*)pdata;
//	        netpstr.eft = pdata->eft;
//	        netpstr.tstno = uc_i;
//	        if(gss_CDATATst[uc_i].pf == TST_PFID_DLT97)                  //645-1997
//	        {
//	            netpstr.tbl = (NETPRD*)gss_NetpBgTstDemand;
//	            netpstr.tbllen = LEN_BGTSTDEMAND;
//	        }
//	        else if(gss_CDATATst[uc_i].pf == TST_PFID_DLT07)             //645-2007
//	        {
//	            netpstr.tbl = (NETPRD*)gss_NetpXgTstDemand;
//	            netpstr.tbllen = LEN_XGTSTDEMAND;
//	        }
//	        else
//	        {
//	            continue;
//	        }
//	                                            //读取并保存
//	        if(Netp_GetTstData(&netpstr) == SYS_ERR_OK)
//	        {
//	            MoveBuffer(gs_SysTime.YMDhms, pdata->ymdhm, 5);  
//	            //SA_REALDATA_Write((uint8*)pdata, uc_i , 1);
//	        }
//	    }
//	}



/************************************************************************
 * @function: Netp_ReadEvtRealData
 * @描述:  抄读事件需要的实时数据(为了用到时不临时去抄读,比如日月冻结)
 * 
 * @参数: 
 * @param: 
 * @返回: SYS_ERR_FT, SYS_ERR_OK
 * @说明: 
 * @作者: zhenggb (2013-06-05)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Netp_ReadEvtRealData(uint8 port)
//	{
//	    uint8 uc_i;
//	
//	//	    S_EvtRealData* levtreal;                            //存放历史值
//	//	    uint8 buffer[SA_FLASH_REALDATA_ITMBABY_BYTE];       //存放历史值
//	//	    levtreal = (S_EvtRealData*)buffer;
//	//	    S_EvtRealData* pdata = &gs_TstEvtData[port];        //实时读取数据存放的缓存
//	//	
//	//	    _IF_TRUE_RETURN_VOID(port >= MCB_PORT_NUM);
//	//	    
//	//	
//	//	    for(uc_i = 1; uc_i < CORE_TST_NO; uc_i++)
//	//	    {
//	//	        if(gs_InpExtracb[port].gbbreak)
//	//	        {
//	//	            return;
//	//	        }
//	//	                                                    //要求:1.测量点有效,2.485表
//	//	        if((gss_CDATATst[uc_i].ef != 0x01) || (gss_CDATATst[uc_i].pt != port) || (gss_ALRTStt[uc_i].ew1.trn))
//	//	        {
//	//	            continue;
//	//	        }
//	//	                                                    //要求:2.485或交采测量点
//	//	        if((gss_CDATATst[uc_i].tf != 0x01) && (gss_CDATATst[uc_i].tf != 0x05))
//	//	        {
//	//	            continue;
//	//	        }
//	//	
//	//	        //HB_RetLive();                               //抄表期间主动保活
//	//	                                                        //赋值抄表内容结构体
//	//	        NETPSTR netpstr;
//	//	        ClearBuffer((uint8*)pdata, sizeof(S_EvtRealData));
//	//	        netpstr.data = (uint8*)pdata;
//	//	        netpstr.eft =  pdata->eft;
//	//	        netpstr.tstno = uc_i;
//	//	
//	//	        if(gss_CDATATst[uc_i].pf == TST_PFID_DLT97)                  //645-1997
//	//	        {
//	//	            netpstr.tbl = (NETPRD*)gss_NetpBgEvtData;
//	//	            netpstr.tbllen = LEN_BGEVTDATA;
//	//	        }
//	//	        else if(gss_CDATATst[uc_i].pf == TST_PFID_DLT07)             //645-2007
//	//	        {
//	//	            netpstr.tbl = (NETPRD*)gss_NetpXgEvtData;
//	//	            netpstr.tbllen = LEN_XGEVTDATA;
//	//	        }
//	//	        else
//	//	        {
//	//	            continue;
//	//	        }
//	//	                                            //读取并保存
//	//	        if(Netp_GetTstData(&netpstr) == SYS_ERR_OK)
//	//	        {
//	//	//	            if(SA_REALDATA_Read(buffer, uc_i, 0) == SYS_ERR_OK)
//	//	//	            {
//	//	//	                EVT_ERC8_Proc((uint8*)levtreal,     (uint8*)pdata, uc_i);    //各类事件判断
//	//	//	                EVT_ERC13_33_Proc((uint8*)levtreal, (uint8*)pdata, uc_i);
//	//	//	                EVT_ERC37_38_Proc((uint8*)levtreal, (uint8*)pdata, uc_i);
//	//	//	                EVT_ERC40_41_Proc((uint8*)levtreal, (uint8*)pdata, uc_i);
//	//	//	            }
//	//	//	            MoveBuffer(gs_SysTime.YMDhms, pdata->ymdhm , 5); 
//	//	//	            SA_REALDATA_Write((uint8*)pdata, uc_i, 0);    
//	//	        }
//	//	    }
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: Netp_ReadExDayData
//	 * @描述: 日历史数据冻结读取
//	 * @参数: 
//	 * @返回: 
//	 * @说明: 1.验证07表内数据作为日数据 2.再次判断上日日冻结数据存在,若不存在补存
//	 * @作者: yzy (2011-10-7)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void Netp_ReadExDayData(uint8 port)
//	{
//	    for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++)
//	    {
//	        if(gs_InpExtracb[port].gbbreak)
//	        {
//	            return;
//	        }
//	        
//	        if((gss_CDATATst[uc_i].ef == 0) || (gss_CDATATst[uc_i].pt != port))
//	        {
//	            continue;
//	        }
//	        
//	        //gdw_Check97LastDayData(uc_i);       //若上日数据不存在,则采用实时数据冻结
//	//#if (GDW_07DIN_EN > 0)               
//	//        gdw_Check07LastDayData(uc_i);       //时钟核对07表内日冻结数据
//	//#endif
//	    }
//	}

/************************************************************************
 * @function: Netp_InitData
 * @描述: 请求重新抄读
 * 
 * @参数: 
 * @param: index 抄表端口索引
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_InitData(uint8 index)
{
//	    if(gs_InpExtracb[index].gbbreak)
//	    {
//	        return;
//	    }
    if(gs_SysVar.mDGstt & HLV_STT_LENG)
    {
        gs_SysVar.mLPstt &= ~HLV_LPTASK_CP;
        return;
    }
    
    //LOG_DEBUG("~~~~~~~~~~~~~port[%d] cp wait start ~~~~~~~", index);
//	    if(gs_SysVar.mDGstt & HLV_STT_NENG)
//	    {
//	        aos_msleep(5000);
//	    }    
    //LOG_DEBUG("~~~~~~~~~~~~~port[%d] cp going start ~~~~~~~", index);

                                        //抄表前统计一次
    void Netp_StatisticProc(uint8 index);
    Netp_StatisticProc(index);
    
    gs_NetpReadStt[index].BSTOP = 0;
    gs_NetpReadStt[index].start = gs_SysTime;
    
    Netp_ReadCureData(index);          

//	    Netp_ReadTstRealData(index);        //读取实时数据
    //Netp_ReadExDayData(index);          //读取日冻结补抄数据
    //Netp_ReadTstDemandData(index);      //读取需量数据
    //Netp_ReadEvtRealData(index);        //读取统计数据


    Netp_ReadFinish(index);


    gs_NetpReadStt[index].end = gs_SysTime;
    gs_NetpReadStt[index].BSTOP = 1;
    //LOG_DEBUG("port[%d] CP Finish note slv to Report \n", index);
//	    uint8_t msg[2];
//	    msg[0] = MSG_NETP_FINISH;
//	    msg[1] = index;
//	    extern kbuf_queue_t gs_TKSlvQueue;
//	    kstat_t t =  gs_OS.Message_Send(&gs_TKSlvQueue, msg,2);
    JudgeTaskFlag(index);
    Netp_StatisticProc(index);          //抄完表再统计一次
}

/************************************************************************
 * @function: Netp_StatisticProc
 * @描述: 抄表端口状态统计
 * 
 * @参数: 
 * @param: index 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_StatisticProc(uint8 index)
{
    uint8 num_mt;
    uint8 num_fl;
    uint8 fl_idx;
    uint8 fl_list[NUM_TSTFAILREC];

    if(index >= MCB_PORT_NUM)
    {
        return;
    }
	
    //初始化
    num_mt = 0;
    num_fl = 0;
    fl_idx = 0;
    ClearBuffer((uint8*)fl_list, sizeof(fl_list));
	
    //统计
    for(uint8 uc_i = 0; uc_i < CORE_TST_NO; uc_i++)
    {
        if((gss_CDATATst[uc_i].pt == index))
        {
            num_mt++;                           //总数量累加
            if(gss_ALRTStt[uc_i].ew1.trn & 0x01)
            {
                num_fl++;                       //抄读失败数量累加
                                                //记录失败的测量点号
                if(fl_idx < NUM_TSTFAILREC)
                {
                    fl_list[fl_idx] = uc_i;
                    fl_idx++;
                }
//	                gs_SysVar.terstt.lword &= ~(0x1 << (16 + uc_i));
            }
            else
            {
//	                gs_SysVar.terstt.lword |= (0x1 << (16 + uc_i));
            }
        }
    }
	
    gs_NetpReadStt[index].num_mt = num_mt;
    gs_NetpReadStt[index].num_fl = num_fl;
    MoveBuffer((uint8*)fl_list, (uint8*)gs_NetpReadStt[index].fl_list, sizeof(fl_list));
}

//===========存储=========================










/************************************************************************
 * @function: Netp_StMonProc
 * @描述: 冻结月数据
 * 
 * @参数: 
 * @param: cbport 抄表端口号
 * @返回: 
 * @说明: 
 * @作者: yzy (2011-9-20)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_StMonProc(uint8 cbport)
{
    if((guc_MonSaveFlag & Bit_Map8[cbport]) == 0)
    {
        guc_MonSaveFlag |= Bit_Map8[cbport];
        //gdw_SaveMonthData(cbport);                      //冻结月数据

        //所有端口全部保存结束后,更新月统计(清零日统计前)
        //   if((guc_MonSaveFlag & Bit_Mask8[MCB_PORT_NUM]) == Bit_Mask8[MCB_PORT_NUM])
        //   {
        //    STAT_TMMonSave();
        //    if(GetTime()->dmon == 1)
        //    {                                   //过月,清零当月终端统计
        //        STAT_TMMonCrack(GetTime()->month);
        //       gdw_CrackMonthData(gs_SysTime.YMDhms);
        //     }
        //   }
        //
    }
}

/************************************************************************
**函数名称: void Netp_StDayProc(uint8 cbport)
**描 述: 冻结日数据
**
**输 入:  cbport 抄表端口号
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 2009-08-01
*************************************************************************/
void Netp_StDayProc(uint8 cbport)
{
    if((guc_DaySaveFlag & Bit_Map8[cbport]) == 0)
    {
        guc_DaySaveFlag |= Bit_Map8[cbport];

//	        gdw_SaveDayData(cbport);                        //冻结终端上一天日数据
    }
}

/************************************************************************
**函数名称: void Netp_StHourProc(uint8 cbport)
**描 述: 冻结小时数据
**
**输 入:  cbport 抄表端口号
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 2009-08-01
*************************************************************************/
void Netp_StHourProc(uint8 cbport)
{
    if((guc_HourSaveFlag & Bit_Map8[cbport]) == 0)
    {
        guc_HourSaveFlag |= Bit_Map8[cbport];
//	        gdw_SaveHourData(cbport);
    }
}


/************************************************************************
**函数名称: void Netp_StRecProc(uint8 cbport)
**描 述: 冻结抄表日数据
**
**输 入:  cbport 抄表端口号
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 2009-08-01
*************************************************************************/
void Netp_StRecProc(uint8 cbport)
{
    if((guc_RecSaveFlag & Bit_Map8[cbport]) == 0)
    {
        guc_RecSaveFlag |= Bit_Map8[cbport];
//	        gdw_SaveRecData(cbport, 0);
//	        gdw_SaveRecData(cbport, 2);
    }
}







/************************************************************************
 * @function: Netp_ReadFinish
 * @描述: 端口抄表完毕
 * 
 * @参数: 
 * @param: pdata 抄表端口号
 * @返回: 
 * @说明: 
 * @作者: yzy (2011-9-20)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_ReadFinish(uint8 index)
{                                           //获取抄表端口号
    if(index >= MCB_PORT_NUM)
    {
        return;
    }
                                            //数据冻结
//	    UpdateSysTimeStruct();                  //先更新下时标吧
//	    Netp_StMonProc(index);
//	    Netp_StDayProc(index);
//	    Netp_StHourProc(index);
//	    Netp_StRecProc(index);//抄表日数据
	
    //==================
    EVT_ERC21_31_Proc(index);               //抄表
}

/************************************************************************
 * @function: Netp_CheckTstTime
 * @描述: 对下挂电表进行广播校时
 * 
 * @参数: 
 * @param: index 抄表端口索引
 * @返回: 
 * @说明: 九七\零七规约:689999999999996808067A64454938455D16
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_CheckTstTime(uint8 index)
{
//	    SerialSets ss;
//	    DLT_Frame frm;
//	    uint8 tmp[32];
//	    uint16 len;
//	    const uint8 forchar[3] = {0xFE, 0xFE, 0xFE};
//	    uint8 port;
//	    port = NETP_PHY_PORT[index];
//	    for(uint8 i = 0; i < 2; i++)
//	    {
//	        frm.cmd = 0x08;                         //控制字
//	        frm.atr = 0;                            //属性
//	        frm.send = tmp;                         //发送起始地址
//	        frm.wd.lword = 0;                       //数据标识
//	        frm.len = 6;                            //长度
//	        gs_RealTime.ReadBCDDateTime((TIME*)(tmp + 10));
//	        BG_Pack(&frm, (uint8*)gucs_BGALLAddr[0], &len);
//	                                                //串口配置
//	        ss.baudrate = (i == 0) ? 1200 : 2400;
//	        ss.databits = DataBits_8bits;
//	        ss.parit = Parit_E;
//	        ss.stopbits = StopBits_1;
//	
//	        //gs_GPIO.LED_BlinkSet(LED_485_T, 0, 0, 0);//亮一下
//	                                                //打开串口
//	        gs_Uart.open(port, false);   
//	        gs_Uart.Init(port, &ss);
//	        gs_Uart.Clear(port);
//	        gs_Uart.Write(port, (uint8*)forchar, sizeof(forchar));    //前导符
//	        gs_Uart.Write(port, tmp, len);
//	        gs_Uart.close(port);
//	        aos_msleep(200);  
//	    }
//	    gs_InpExtracb[index].gbbreak = 0;
    //gs_OS.Message_Send(MSG_NETP_IDATA, TASK_NETP_TKID + index);
}


/************************************************************************
 * @function: Netp_RecvProc
 * @描述: 组网端口接收到数据之后的数据处理
 * 
 * @参数: 
 * @param: data 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_RecvProc(void)
{
    uint8 pdata[256];
    uint16 length;
    uint8 err;
    
    //here we should apply to occupy the resource
    //otherwise netp active read-received data will be readout here.
    gs_Uart.open(NETP_PORT_NO, false);

//	    while(gs_Uart.ToBeRead(NETP_PORT_NO) > 256)
//	    {
//	        length = gs_Uart.Read(NETP_PORT_NO, pdata, 256);
//	    }
    length = gs_Uart.Read(NETP_PORT_NO, pdata, 256, 5000);
    
    aos_msleep(2);
    gs_Uart.Close(NETP_PORT_NO);
    
    //标志
    //Netp_DiscoSetRecvFlag();
                                        //处理被动通道4的数据
    err = PstFrmProc(PST_CHN_NETP, pdata, length, pdata, sizeof(pdata));
    if(SYS_ERR_OK == err)
    {
        gul_noPlcReadSecs = 0;        //清零安全标志(***)
    }
    
}

/*******************************************************************************
 * @function_name:  Netp_SecProc
 * @function_file:  netp.c
 * @描述: 秒处理
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2011-04-25)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void Netp_SecProc(void)
{
//	    scm_dlms_t* scm = &gs_scmDlms;

    gul_noPlcReadSecs++;
                                        //异常安全机制的实现
    if((gul_noPlcReadSecs > 86400))
    {
        gul_noPlcReadSecs = 0;
//	        SYS_Reset();
    }

//	    if(!(scm->stt & SCM_STT_FIROK) &&
//	       ((scm->stt & SCM_STT_EXMASK) == SCM_STT_IDLE))
//	    {
//	        if((GetSysRunSecs() > 4)         //等待MBUS电源稳定
//	           )
//	        {                               //若上电从未启动搜表,则启动
//	            ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID);
//	        }
//	    }
                                        //执行搜表流程
//    Netp_FindSecProc();
//	    
//	    if(!(scm->stt & SCM_STT_FIROK) && 
//	       ((scm->stt & SCM_STT_EXMASK) == SCM_STT_IDLE))
//	    {
//	                                        //若上电从未启动搜表,则启动
//	        ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID);
//	    }
//	    
//	    Netp_MtUpdataPro();
   
}

/************************************************************************
 * @function: Netp_chktimeisok
 * @描述: 判断表计对时时间是否到
 * 
 * @参数: 
 * @param:  
 * @返回: 
 * @说明: 
 * @作者: zjc (2015/5/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_chktimeisok(void)  //
{
    
//	    UpdateSysTimeStruct();  //更新系统时标
    
    if(gs_NetpMtTime.period > 0 )//
    {
        uint32 syshour = CalcHourHex(gs_SysTime.YMDhms[0], gs_SysTime.YMDhms[1], gs_SysTime.YMDhms[2], 
                                        gs_SysTime.YMDhms[3]);
        
        if((syshour >= gs_NetpMtTime.hours) && ((syshour - gs_NetpMtTime.hours) % gs_NetpMtTime.period == 0)) //启动时间间隔到了
        {
            if((gs_SysTime.YMDhms[4] >= gs_NetpMtTime.datatime[4]) && (gs_SysTime.YMDhms[4] >= 1))   //启动时间 分钟到了
            {
                if(gs_NetpMtTime.stt == 0)  //保证只执行一次
                {
                    gs_NetpMtTime.stt = 1;
//                    ES_Message_Send(MSG_NETP_CKTIME, TASK_NETP_TKID);   //开始校时
                }
            }
            else    //分钟任务未到之前，将状态位准备好
            {
                gs_NetpMtTime.stt = 0;
            }
        }
        
    }
}

/************************************************************************
 * @function: Netp_MinProc
 * @描述: 分钟任务
 * 
 * @参数: 
 * @param:  
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/22)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_MinProc(void)  //
{
//	    scm_dlms_t* scm = &gs_scmDlms;
    uint8 bmaps[LEN_TST_BMPS];
    
//	    if(((scm->stt & SCM_STT_EXMASK) != SCM_STT_FIN))
//	    {
//	        gs_GPIO.BlinkSet(GPIO_LED_485, 1, 0, 0);
//	        return;
//	    }
    UpdateSysTimeStruct();  //更新系统时标
    
    if(gs_SysVar.mDGstt & HLV_STT_LENG)
    {
        gs_SysVar.mLPstt &= ~HLV_LPTASK_CP;
        return;
    }
//	    Netp_chktimeisok(); //判断给表计对时时间是否到了
    //uint8_t min_interval = (gs_PstPara.cp_interval / 60);
    //min_interval = (min_interval) ? min_interval:1;
    if((gs_SysTime.YMDhms[4] % gs_PstPara.cp_interval) == 0)  //n分钟后再去读表的曲线点
    {
        aos_msleep(100);
        GD_Para_RW(F10_PADDR, bmaps, LEN_TST_BMPS, false);   //读抄表档案位图
        if(STR_IsEmpty(bmaps, LEN_TST_BMPS))  //抄表档案位图为空，则再次搜表
        {
            return ;
        }
        else
        {
//	                uint8 msg = MSG_NETP_CURDATA;
        //          krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            Task_Clear();
//	            gs_GPIO.GPO_Out(GPO_485_PWR, true);
            
            Netp_PowerMng(1, NETP_PORT_PWR_485);
            gs_SysVar.mLPstt |= HLV_LPTASK_CP;
            
            if(gs_SysVar.mDGstt & HLV_STT_NENG)
            {
                HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/RHINO_CONFIG_TICKS_PER_SECOND);
                //LOG_DEBUG("~~~~~~~~~~~~~port cp wait start ~~~~~~~");
                aos_msleep(2500);
                //LOG_DEBUG("~~~~~~~~~~~~~port cp going start ~~~~~~~");   

            }    
            
            aos_msleep(100);
            
            uint8 YMDhms[6];   //时标
            uint8 ucmin;            
            MoveBuffer(gs_SysTime.YMDhms, YMDhms, 6);  //当前时间  
            ucmin = YMDhms[4] % gs_PstPara.cp_interval;
            YMDhms[4] = YMDhms[4] - ucmin;   //取上次 曲线保存分钟点
            YMDhms[5] = 0;
            memcpy(gss_TaskATimer.time.YMDhms, YMDhms, 6);

            gs_OS.Message_Send(&gs_TKNetpQueue, &msgidA[MSG_NETP_CURDATA], 1);//(MSG_NETP_CURDATA, TASK_NETP_TKID);  //读曲线数据
            gs_OS.Message_Send(&gs_TKNetp2Queue, &msgidA[MSG_NETP_CURDATA], 1);
            gs_OS.Message_Send(&gs_TKNetp3Queue, &msgidA[MSG_NETP_CURDATA], 1);
            gs_OS.Message_Send(&gs_TKNetp4Queue, &msgidA[MSG_NETP_CURDATA], 1);

        }
    }
    //gs_OS.Message_Send(MSG_NETP_EVNDATA, TASK_NETP_TKID);  //读事件
    
    if(dbflag.hour == 1)//小时任务启动
    {
        if(gs_SysTime.YMDhms[4] < NETP_READDAY_MIN)  //n分钟后再去读冻结数据
        {
            return ;
        }
        if((gs_SysTime.YMDhms[3] % NETP_REREADDAY_HOUR) == 0)   //
        {      
           //gs_OS.Message_Send(MSG_NETP_DAYDATA, TASK_NETP_TKID);  //读取日冻结数据
        }
        dbflag.hour = 0;    //关闭标志
    }
    if(dbflag.day == 1)  //日任务启动
    {
        if(gs_SysTime.YMDhms[4] < NETP_READMONTH_MIN)  //n分钟后再去读冻结数据
        {
            return ;
        }
        //gs_OS.Message_Send(MSG_NETP_MONDATA, TASK_NETP_TKID);  //读取月冻结数据
        dbflag.day = 0; //关闭标志
    }
}


/************************************************************************
 * @function: Netp_HourProc
 * @描述: 小时任务
 * 
 * @参数: 
 * @param:  
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/22)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_HourProc(void)  //
{ 
//	    scm_dlms_t* scm = &gs_scmDlms;
//	    
//	    if(((scm->stt & SCM_STT_EXMASK) != SCM_STT_FIN))
//	    {
//	        return;
//	    }
    
//	    UpdateSysTimeStruct();  //更新系统时标
    
    if( (gs_SysTime.YMDhms[3] % NETP_REREADCURE_HOUR) == 0 )  //N个小时查询一次，是否存在是否存在遗落
    {
//	        ES_Message_Send(MSG_NETP_BCURDATA, TASK_NETP_TKID);  //补曲线数据
    }
    dbflag.hour = 1;     //开启日冻结任务
    
    
}

/*******************************************************************************
 * @function_name:  Netp_DayProc
 * @function_file:  netp.c
 * @描述: 鈤处理
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2011-04-25)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void Netp_DayProc(void)
{                                       //复位MBUS
    //gs_GPIO.GPO_Out(GPO_485_PWR, false);
//	    aos_msleep(300);
//	    gs_GPIO.GPO_Out(GPO_485_PWR, true);
//	    aos_msleep(300);
    
    dbflag.day = 1;  //开启月冻结任务
}


extern const uint8 netp_hwtest_frm1[];



void NetpErrProb(void)
{
    SerialSets ss;
    uint8 phyport;
    uint8 overtime;
    uint8 buffer[50];
    uint16 len;
    
//	    ES_ENTER_SCRT();
//	
//	    for(uint8 xx = 0; xx < 20; xx++)
//	    {
//	        MoveBuffer((uint8*)netp_hwtest_frm1, buffer, 37);
//	        phyport = NETP_PORT_NO;
//	        ss.baudrate = NETP_PORT_BAUD;
//	        ss.databits = DataBits_8bits;
//	        ss.parit = Parit_N;
//	        ss.stopbits = StopBits_1;
//	                                                //打开串口
//	        gs_GPIO.GPO_Out(GPO_485_PWR,false);
//	        gs_Uart.open(phyport, false);
//	        gs_Uart.Init(phyport, &ss);
//	        gs_Uart.Clear(phyport);
//	                                                
//	        gs_Uart.Write(phyport, buffer, 37);   //发送数据
//	
//	        len = 0;                                            //接收长度为0
//	        overtime = 40;
//	        while(overtime--)                                   //在规定的延时期间内进行有效帧的判断
//	        {
//	            aos_msleep(10);                             //判断是否有数据收到
//	            if(gs_Uart.ToBeRead(phyport) > 0)
//	            {
//	               len += gs_Uart.Read(
//	                               phyport, 
//	                               buffer + len, 
//	                               256 - (len & 0xFF),
//	                               5000);
//	
//	                len &= 0xFF;                                //数据在一个字节以内的
//	                                                            //判断帧的有效性 
//	                if(__PST_CheckFrame(PST_FRM_DLMS_HDLC_NO, buffer, &len) == PST_ERR_OK)
//	                {    
//	                    //关闭串口
//	                    gs_GPIO.LED_BlinkSet(GPIO_LED_485, 2, 0, 0);    //亮
//	                    gs_Uart.close(phyport);
//	                    aos_msleep(50);
//	                    break;
//	                }
//	            }
//	        }
//	        gs_Uart.close(phyport);
//	    }
//	    
//	    ES_EXIT_SCRT();
}
//extern const ST_Netp_FW gs_netp_fw_tx13;
//extern const ST_Netp_FW gs_netp_fw_wxic;

/*******************************************************************************
 * @function_name:  SYS_Netp_Init
 * @function_file:  netp.c
 * @描述: 进程初始化
 * 
 * 
 * @参数: 
 * 
 * @返回: 
 * @作者: yzy (2011-03-03)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_Netp_Init(void)
{



//	    Netp_Register_Set(gss_NetpFrameFw[1]);
    Netp_Register_Init();
}

/************************************************************************
 * @function: SYS_NETP_Task
 * @描述: 抄表进程处理函数
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_NETP_Task(void)
{
    //数字消息
    krhino_buf_queue_create(&gs_TKNetpQueue, "netp_queue",
                         gc_TKNetpQbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);
    
    SYS_Netp_Init();

    SYS_MSG_Apply(TASK_NETP_TKID, MSG_CLS_TM);
//	    SYS_MSG_ApplyExt(TASK_NETP_TKID, MSG_CLS_UART, NETP_PORT_NO);
    aos_msleep(500);
//	    uint8_t msg[2];
//	    msg[0] = MSG_NETP_TASK_RUN_FLAG_PER_485;
//	    msg[1] = 0;
//	    extern kbuf_queue_t gs_TKSlvQueue;
//	    kstat_t t = gs_OS.Message_Send(&gs_TKSlvQueue, msg,2);
//	    LOG_DEBUG("port[0] CP flag note slv to sign [%d] \n", t);
    SetTaskRunFlag(0);
//	    gs_OS.Message_Send(&gs_TKNetpQueue, &msgidA[MSG_NETP_CHECK], 1);
    for(;;)
    {
        //获取进程消息
        krhino_buf_queue_recv(&gs_TKNetpQueue, RHINO_WAIT_FOREVER, g_TKNetpQ_buf_recv,
                                  &g_TKNetpQ_recv_size);

        switch(g_TKNetpQ_buf_recv[0])                        //根据消息分别处理
        {
            case MSG_SEC:
                
                Netp_SecProc();
           
                
                break;
            case MSG_MIN:
                gs_SysVar.mLPstt |= HLV_LPTASK_SMSG_NETP;
                
                Netp_MinProc();       //抄表
                gs_SysVar.mLPstt &= ~HLV_LPTASK_SMSG_NETP;
                
                break;
            case MSG_HOUR:
                Netp_HourProc();    
                break;
            case MSG_DAY:
                Netp_DayProc();         
                break;
            case MSG_NETP_IDATA:            //端口1数据抄读开
//	                Netp_ReReadCureData(0,24);        //补前24个点的曲线
//	                ReadDlmsLastDayData(1);          //读取日冻结电能
//	                ReadDlmsLastMonEnergy(1);        //读取月冻结电能
//	                ReadDlmsLastMonthDMData(1);      //读取月冻结需量
                break;
            case MSG_NETP_CURDATA:   //读曲线数据
                Netp_InitData(0);
                break;
            case MSG_NETP_BCURDATA:   //补曲线数据
//	                Netp_ReReadCureData(0,12);      //补前12个点的曲线     
                break;
            case MSG_NETP_DAYDATA:                //读日冻结数据
//	                ReadDlmsLastDayData(0);          //读取日冻结电能
                break;
            case MSG_NETP_MONDATA:                //读月冻结数据
//	                ReadDlmsLastMonEnergy(0);        //读取月冻结电能
//	                ReadDlmsLastMonthDMData(0);      //读取月冻结需量
//	                ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID);   //月冻结数据抄完后，再启动搜表 
                break;
            case MSG_NETP_EVNDATA:                //读事件
//	                Netp_ReadEvent(0);   //读DLMS事件
                break;
            case MSG_NETP:      //抄表口被动通信
                Netp_RecvProc();
                break;
            case MSG_NETP_CHECK:
                Netp_Check(0);
                break;
            case MSG_LIVE:
                HB_RetLive(TASK_NETP_TKID);
                break;
        }
    }
}

/******************************************************************************
**抄表进程块常量部分定义
******************************************************************************/
const KTaskConst gs_TkNetp = 
{
    &gs_TKNetpHandle,
    SYS_NETP_Task,                           //应用管理进程
    gs_TKNetpStack,
    TASK_NETP_TKID,                           //进程编号
    true,                                    //消息循环机制进程
    TASK_NETP_STKL,
    "netp",
    &gs_TKNetpQueue,
};



