/****************************************Copyright (c)**************************************************
**                               	杭州xx有限公司
**                                     		开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: M72X.c
**创   建   人: yzy
**最后修改日期: 2018年2月1日
**描        述: GSM模块的驱动函数集
**注        意：
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2007年1月18日
** 描　述: 原始版本
********************************************************************************************************/
#define EXT_M72D

#include "public.h"
#include "k_types.h"
#include "k_api.h"
//	#include "lib_app.h"
#include "hal.h"


#include "atmodem.h"

#include "modemlib.h"
#include "atstd.h"



/*******************************************************************************
**M72X模块调试过程中的特殊现象说明:
1)tcpcnt成功,发送时提示error.
出现背景:目的ip端口正常,但目的IP内网中映射端口的前置机未开启.
2)...
********************************************************************************/

uint8 guc_M72XRingCount;

/*******************************************************************************
**通道号,socketid号,定义
********************************************************************************/
typedef enum
{
    NET_TYPE_TCPCLIENT = 0,
    NET_TYPE_TCPSERVER,
    NET_TYPE_UDP,
}e_MODM_NET_TYPE;

typedef enum
{
    CHN_DEFAULT = 0,
}e_MODM_CHN;



//M72X:Valid socket numbers - 0,1,2,3 and 4
typedef enum
{
    SOCKET_TCP_CLIENT = 0,
    SOCKET_UDP_CLIENT = 1,
//	    SOCKET_SERVER_1 = 3,
//	    SOCKET_SERVER_2 = 4,
    SOCKET_MAX = 2,
}e_MODM_SOCKET;

#define SOCKET_TCP_STR "TCP"
#define SOCKET_UDP_STR "UDP"
//	unsigned char guc_modembuff[MODEM_STRING_BUFF_LEN];

/************************************************************************
 * @function: SYS_M72X_ModemOn
 * @描述: 开机开机(执行后将占用串口)
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_M72X_ModemOn(void)
{    
    SerialSets ss;
#ifdef FARP_PORT_BAUD
    ss.baudrate = FARP_PORT_BAUD;
#else
    ss.baudrate = 19200;
#endif
    ss.databits = DataBits_8bits;
    ss.parit = Parit_N;
    ss.stopbits = StopBits_1;
//	    SYS_SER_Init(SYS_GPRS_PORT, &ss);
    SER_Init(SYS_GPRS_PORT, &ss);
    SER_Clear(SYS_GPRS_PORT);

    _ATMODEM_VIN();
  
  //初始值
    _ATMODEM_ONOFF_0();
                                            //复位(或重新上电)
    //_ATMODEM_PORTCLOSE();                   //关闭串口的txd,rxd脚,防止灌电流供电
    //_ATMODEM_PWROFF();
    //_ATMODEM_PWRWAIT();
    //sleep_ms(1200);
    _ATMODEM_PWRON();
    //_ATMODEM_PORTOPEN();
                                            //开机时序
    sleep_ms(1000);
    _ATMODEM_ONOFF_1();
    sleep_ms(2000);
    _ATMODEM_ONOFF_0();
    sleep_ms(100);
    _ATMODEM_PORTOPEN();
                                            //重新配置串口参数
//	    SER_Open(UART_CHANNEL_DEBUG, TDB_MODE_R | TDB_MODE_W);

    guc_M72XRingCount = 0;
}


/************************************************************************
 * @function: SYS_M72X_ModemOff
 * @描述: 关机操作
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_M72X_ModemOff(void)
{
    _ATMODEM_PORTCLOSE();
    _ATMODEM_PWROFF();
}

/************************************************************************
 * @function: SYS_M72X_Check
 * @描述: 查询模块标识是否匹配
 * @参数: 
 * 
 * @返回: 
 * @return: bool true成功,false失败(成功后不关闭串口,将以识别到的串口配置长期占用)
 * @说明: 
 * @作者: yzy (2014/11/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
bool SYS_M72X_Check(void)
{
	SerialSets ss;
    uint8 overtime;
    uint8 uc_data[60];
    uint8 len;
    uint8 flag;
                                            //配置串口参数
#ifdef FARP_PORT_BAUD
    ss.baudrate = FARP_PORT_BAUD;
#else
    ss.baudrate = 19200;
#endif
    ss.databits = DataBits_8bits;
    ss.parit = Parit_N;
    ss.stopbits = StopBits_1;
    SER_Init(SYS_GPRS_PORT, &ss);
//	    SYS_SER_Init(SYS_GPRS_PORT, &ss);
    SER_Clear(SYS_GPRS_PORT);
                                            //打开串口,若检测成功就不关闭了
    SER_Open(SYS_GPRS_PORT, TDB_MODE_R | TDB_MODE_W);
    sleep_ms(200);
				                            //发送ATE0,多次发送是为了让模块自动识别波特率.
    for(uint8 trycnt = 0; trycnt < 5; trycnt++)
    {
        ClearBuffer(uc_data, sizeof(uc_data));
        //
        SER_SendData(SYS_GPRS_PORT, "ATE0\r", 5, MODEM_SER_TIMEOUT);//ATE0
        //sleep_ms(100);
        flag = 0;
        len = SER_RecvData(SYS_GPRS_PORT, uc_data, sizeof(uc_data), MODEM_SER_TIMEOUT/5);
        for(uint8 uc_i = 0; uc_i < len; uc_i++)
        {
            if(!(CmpBuffer(uc_data + uc_i, "OK", 2)))
            {
                flag = 1;
                break;
            }
        }
        
        if(flag)
        {
            break;
        }
    }
    
    if(flag == 0)
    {
        SER_Close(SYS_GPRS_PORT);
        return false;
    }
	//

    ClearBuffer(uc_data, sizeof(uc_data));
    	                                    //发送读取模块标识指令
    SER_SendData(SYS_GPRS_PORT, "AT+GMM\r", 7, MODEM_SER_TIMEOUT);
    overtime = 100;
    flag = 0;
    while(overtime--)
    {
        sleep_ms(60);
        
        len = SER_LineLen(SYS_GPRS_PORT);
        if((len > 0) && (len < 255))
        {
            SER_Line(SYS_GPRS_PORT, uc_data, sizeof(uc_data));
											//比较读取的数据是否为对应的模块标识
            for(uint8 uc_i = 0; uc_i < 32; uc_i++)
            {
                if((!(CmpBuffer(uc_data + uc_i, "Quectel_M72", 11))))
                {
                    flag = 1;
                    SER_Clear(SYS_GPRS_PORT);//回复帧最后还有个"OK"需要删除
                    break;
                }
            }
            
            if(flag)
            {
                break;
            }
        }
    }
    
    if(flag == 0)
    {
        SER_Close(SYS_GPRS_PORT);
        return false;
    }
    
    return true;                            //返回成功
}




/************************************************************************
 * @function: DecodeRecvIPClose
 * @描述: 
 * 
 * @参数: 
 * @param: mrfb 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvIPClose(ModemRecvFeedBack* mrfb)
{
    /*第一种回复
     *      AT+MIPCLOSE?
     *      +MIPCLOSE: 0    //No opened sockets(0在M72X指令中是不合法的socketid)
     *      OK
     *      或者
     *      +MIPCLOSE: 1,2  //Sockets 1 and 2 are opened*
     *      OK
     *第二种回复
     *      AT+MIPCLOSE=1
     *      OK
     *      +MIPCLOSE: 1,0  //Socket 1关闭成功,对方回应了ACK*
     *      或者
     *      +MIPCLOSE: 1,1  //Socket 1关闭超时,对方无应答*
     *      或者
     *      +MIPCLOSE: 1,1024,2 //Socket 1关闭的其他情况:1024 bytes were acked,对方不回复RST.
     *      //指令说明+MIPCLOSE: <Socket_ID>[,<number_of_acknowledged_bytes>],<close_type>
     * 
     *注意:带*号的第一种回复和第二种回复需要根据语境进行解析. 
     */
	
    uint16 byte;
    uint16 num[4];
    uint8 n = 0;                        //解析出的数值个数
                                        //初始化为0xffff
    memset(num, 0xff, sizeof(num));
                                        //循环读取最大5个数值,直到该行读完
//	    for(uint8 uc_i = 0; uc_i < 4; uc_i++)
//	    {
    num[0] = GetOneChar(&byte);	//获取第N个数字
//	        if(num[uc_i] != 0xffff)
//	        {
//	            n++;
//	        }
//	        
//	        if(byte == 0x0D || byte == 0x0A)//这一行读完则退出
//	        {
//	            break;
//	        }
//	    }

    //if(gs_ModemStep == Modem_IPState)   //查询状态(num1-num4为opened socketid)
    {                                   //tcp链接是否还存在
        if((num[0] == SOCKET_TCP_CLIENT))
        {
            DevModem->stt.bit.tcpcc = 0;
        }
                                        //udp链接是否还存在
        if((num[0] == SOCKET_UDP_CLIENT))
        {
            DevModem->stt.bit.udpcc = 0;
        }
                                        //根据tcp和udp链接状态确定ip状态
        if(!DevModem->stt.bit.tcpcc && !DevModem->stt.bit.udpcc)
        {
            DevModem->stt.bit.ipcc = 0;
        }
    }
    
    {
        DecodeRecvDataNone(mrfb);		//....
    }
}

/************************************************************************
 * @function: DecodeRecvIPFail
 * @描述: 
 * 
 * @参数: 
 * @param: mrfb 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvIPFail(ModemRecvFeedBack* mrfb)
{
    /*第一种回复
     *      0, CONNECT FAIL  //socket 0 链接失败
     *      
     */
	
    uint16 num=0xff;
    uint16 byte;
                                        //初始化为0xffff
                                        //循环读取最大5个数值,直到该行读完
    num = GetOneChar(&byte);	//获取第N个数字
        
    SYS_MODM_EndCnt();      //结束超时计数器的计时  
    SYS_MODM_Error();       //收到ERROR之后的处理

    if((num == SOCKET_TCP_CLIENT))
    {
        SYS_MODM_Step(Modem_TcpDiscnt);
    }
                                    //udp链接是否还存在
    if((num == SOCKET_UDP_CLIENT))
    {
        SYS_MODM_Step(Modem_UdpDiscnt);
    }


    DecodeRecvDataNone(mrfb);		//....
}

/************************************************************************
 * @function: DecodeRecvTCPData
 * @描述: 解析收到的IP数据,并存放到相应的FIFO中去
 * 
 * @参数: 
 * @param: mrfb 处理后的信息结构
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvTCPData(ModemRecvFeedBack* mrfb)
{
    //+MIPRTCP: <socket_ID>,<Left>,<Data>
    //+QIRD:116.228.146.250:7070,TCP,36<CR><LF>
    TModemIPData* iprev = DevModem->ipRev;  //IP数据接收缓存指针
    uint16 socket;
  	uint16 byte;
    uint16 len = 0;
    uint16 ctr;
    
    socket = GetOneChar(&byte);             //读出socket
//	    if(SOCKET_TCP_CLIENT != socket)
//	    {
//	        return;
//	    }
                                            //
    len = GetOneChar(&byte);
    if(0xFFFF == len)
    {
        return;
    }
    DecodeRecvDataNone(mrfb);//去掉符号 0x0A

    sleep_ms(5);
    ctr = len + 100;
	while(len > 0)                          //不停的读取数据
	{
		byte = ReadNextByte();
        if(byte == 0x100)                   //如果数据还没有接收完
        {
            if(ctr == 0)                    //不可能超过110ms的
            {
                sleep_ms(1);             //睡眠10ms
                return;
            }
            sleep_ms(1);                 //睡眠10ms
            ctr--;
            continue;
        }

        len--;

        if(iprev->len < LEN_OF_IPRECV)      //数据转移到数据缓存
        {
            iprev->buffer[iprev->rp++] = byte;
            if(iprev->rp >= LEN_OF_IPRECV)  //卷绕处理
            {
                iprev->rp = 0;
            }
            iprev->len++;                   //接收到的数据长度累加
        }
	}
    
    DecodeRecvDataNone(mrfb);
}




/************************************************************************
 * @function: DecodeRecvUDPData
 * @描述: 解析收到的IP数据,并存放到相应的FIFO中去
 * 
 * @参数: 
 * @param: mrfb 处理后的信息结构
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvUDPData(ModemRecvFeedBack* mrfb)
{
    TModemIPData* iprev = DevModem->ipRev;  //IP数据接收缓存指针
    uint16 byte;
    uint16 len = 0;
    uint16 ctr;
    uint8 result[32];
                                            //读出数据IP,端口号,
    uint8 commas = 0;
    uint8 lenmax = 24;
    uint8 uc_i;
    for(uc_i = 0; uc_i < lenmax; uc_i++)
    {
        result[uc_i] = ReadNextByte();
        if(',' == result[uc_i])
        {
            commas++;
            if(commas == 2)
            {
                break;
            }
        }
    }
    
    if(uc_i == lenmax)
    {
        return;
    }
    
    byte = ReadNextByte();                  //再读出socket
    if('1' != byte)				            //socket是1
    {
        return;
    }
    byte = ReadNextByte();                  //socket之后有个“，”
    if(',' != byte)				            //,
    {
        return;
    }
    
    len = GetOneChar(&byte);                //读出后面数据的长度
    if(0X0D != byte)
    {
        return;
    }

    sleep_ms(5);
    ctr = len + 150;
	while(len > 0)                          //根据长度不停的读取数据
	{
		byte = ReadNextByte();
        if(byte == 0x100)                   //如果数据还没有接收完
        {
            if(ctr == 0)                    //不可能超过110ms的
            {
                sleep_ms(1);             //睡眠10ms
                return;
            }
            sleep_ms(1);                 //睡眠10ms
            ctr--;
            continue;
        }

        len--;

        if(iprev->len < LEN_OF_IPRECV)      //数据转移到数据缓存
        {
            iprev->buffer[iprev->rp++] = byte;
            if(iprev->rp >= LEN_OF_IPRECV)  //卷绕处理
            {
                iprev->rp = 0;
            }
            iprev->len++;                   //接收到的数据长度累加
        }
	}
    
    DecodeRecvDataNone(mrfb);
}


/************************************************************************
 * @function: DecodeRecvIPSend
 * @描述: 收到+MIPSEND后的处理
 * 
 * @参数: 
 * @param: mrfb 处理后的信息结构
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvIPSend(ModemRecvFeedBack* mrfb)
{
    /*第一种回复
        AT+MIPSEND=1,"4444" 
        +MIPSEND: 1,0,2046      //+MIPSEND:<Socket_ID>,<Status>,<FreeSize>. 
        OK                      //2048-2 chars 'DD' = 2046
    
      第二种回复
        AT+MIPSEND? 
        +MIPSEND: 1,2048        //+MIPSEND:<Socket_ID>,<FreeSize>>
        +MIPSEND: 2,2048
        OK
        +QTUNBUF: 2048,3584,1536,448
    */
    
    uint16 byte;
    uint16 num[4];
    uint8 n = 0;                        //解析出的数值个数
                                        //初始化为0xffff
    memset(num, 0xff, sizeof(num));
                                        //循环读取最大5个数值,直到该行读完
    for(uint8 uc_i = 0; uc_i < 4; uc_i++)
    {
        num[uc_i] = GetOneChar(&byte);	//获取第N个数字
        if(num[uc_i] != 0xffff)
        {
            n++;
        }
        
        if(byte == 0x0D || byte == 0x0A)//这一行读完则退出
        {
            break;
        }
    }
                                        //AT+MIPSEND?的回复帧
//	    if(n == 2)
    {
        SetModemMMS(num[1]);
    }
}


/************************************************************************
 * @function: DecodeRecvSimDrop
 * @描述: 检测到SIM卡掉卡后的处理方式
 * 
 * @参数: 
 * @param: mrfb 
 * @返回: 
 * @说明: 
 * @作者: ZJC (2015/01/29)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvSimDrop(ModemRecvFeedBack* mrfb)
{
    SYS_MODM_SendCmd("AT+QSIMDET", 10, 0);//执行AT+GTSIMRST 复位SIM卡
    sleep_ms(100);                     //不判接收
    
    DecodeRecvDataNone(mrfb);
}


/************************************************************************
 * @function: DecodeRecvPPPStt
 * @描述: 提取PPP连接状态值
 * 
 * @参数: 
 * @param: mrfb 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvPPPStt(ModemRecvFeedBack* mrfb)
{
    /*第一种情况:
    AT+MIPCALL=1,"mtnirancell","","" 
    OK
    +MIPCALL: 10.7.200.19
    或
    ERROR: <err>
    +MIPCALL: 0
    
    第二种情况:
    +MIPCALL?
    +MIPCALL: 1,123.145.167.230 //+MIPCALL: <status>[,<IP>]
    */
    uint8 str[32];
    uint16 byte;
    int num[4];
    uint8 ctget = false;
    uint8 uc_i = 0;
    uint8 n = 0;                        //解析出的数值个数
                                        //初始化为0xffff
    memset(num, 0, sizeof(num));
    memset(str,0,32);
    
    for(uc_i = 0; uc_i < 5; uc_i++)
    {
        byte = ReadNextByte();
//          if(byte == 0x100)
//          {
//              return;
//          }
        
        if(byte != '\r'&& byte != '\n')
        {
            str[0] = byte;    //保存该字符到第一个位置
            ctget = true;
            break;
        }
    }
    
    if(!ctget)                          //未扫描到双引号,退出
    {
        return;
    }
                                        //新第二个位置开始保存
    for(uc_i = 1; uc_i < sizeof(gucs_CCID); uc_i++)
    {
        byte = ReadNextByte();
        if((byte == '\r') || (byte == '\n'))//if(byte == 0x100)
        {
            break;
        }
        str[uc_i] = byte;
    }

    if(uc_i >= 11)
    {
        sscanf((const char *)str,"%d.%d.%d.%d",num,(num+1), (num+2), (num+3));
        DevModem->ModemIp = gul_localIP = (uint32)num[0] | ((uint32)num[1] << 8) | ((uint32)num[2] << 16) | ((uint32)num[3] << 24);
        DevModem->stt.bit.pppoe = 1;
    }
                                        
                                        //循环读取最大5个数值,直到该行读完
//	    for(uint8 uc_i = 0; uc_i < 5; uc_i++)
//	    {
//	        num[uc_i] = GetOneChar(&byte);	//获取第N个数字
//	        if(num[uc_i] != 0xffff)
//	        {
//	            n++;
//	        }
//	        
//	        if(byte == 0x0D || byte == 0x0A)//这一行读完则退出
//	        {
//	            break;
//	        }
//	    }
//	    
//	    if(n == 4)
//	    {
//	        gul_localIP = (uint32)num[0] | (num[1] << 8) | ((uint32)num[2] << 16) | ((uint32)num[3] << 24);
//	    }
//	    else if((n == 1) && (num[0] == 0))
//	    {
//	        DevModem->stt.bit.udpcc = 0;
//	        DevModem->stt.bit.tcpcc = 0;
//	        DevModem->stt.bit.ipcc = 0;
//	        DevModem->stt.bit.pppoe = 0;
//	        //不符合步骤目标,步骤异常中止.
//	        SYS_MODM_EndCnt();
//	        SYS_MODM_Error();
//	        //
//	    }
//	    else if((n == 5) && (num[0] == 1))
//	    {
//	        DevModem->stt.bit.pppoe = 1;
//	        gul_localIP = (uint32)num[1] | ((uint32)num[2] << 8) | ((uint32)num[3] << 16) | ((uint32)num[4] << 24);
//	    }

    DecodeRecvDataNone(mrfb);
}

/*************************************************************************
*接收AT指令序列                       //与STDAT_RECV对应
**************************************************************************/
const string STD_IPSTAT[] =
{
    "IP INITIAL",
    "IP START",
    "IP CONFIG",
    "IP IND",
    "IP GPRSACT",
    "IP STATUS",
    "TCP CONNECTING",
    "UDP CONNECTING",
    "IP CLOSE",
    "CONNECT OK",
    "PDP DEACT",
};

/************************************************************************
 * @function: DecodeRecvIPStt
 * @描述: 提取IP连接状态值
 * 
 * @参数: 
 * @param: mrfb 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvIPStt(ModemRecvFeedBack* mrfb)
{
    //+MIPSTAT: <socket_ID>,<n>[,<number_of_acknowl d_bytes >]
    
    /********************************************************************
    场景1:作为AT+MIPOPEN的异常回复帧.(需要针对IPCONNECT步骤进行异常中止)
    AT+MIPOPEN=1,34127,"217.219.84.194",1024,0 OK
    +MIPSTAT: 1,1
    AT+MIPOPEN=1,34127,"217.219.84.194",1024,0 OK
    +MIPOPEN: 1,1
    
    场景2:网络异常时的主动上报帧.
    ************************************************************************/
    
    uint16 socketid;
    uint16 stat;
    uint16 byte;
    //uint8 rdlen = 0;
    uint8 itmidx = 0;
    //uint8 rdbuf[64];
    uint8 linelen = 0;
    //uint16 ebytes = 0;
    //memset(rdbuf,0,64);
    
//	    rdlen =SER_Line(SYS_GPRS_PORT, uc_data, sizeof(uc_data));
//	
//	    for(itmidx = 0; itmidx < (sizeof(STD_IPSTAT) / sizeof(string)); itmidx++)
//	    {                                       //1)获取该AT指令特征字符串的识别长度
//	        uint8 itmlen = strlen(STD_IPSTAT[itmidx]);
//	                                            //2)读取的长度>=特征字符串长度,执行指令识别操作
//	        if(rdlen >= itmlen) 
//	        {
//	            if(0 == CmpBuffer(uc_data, (uint8*)STD_IPSTAT[itmidx], itmlen))
//	            {
//	                DequeueByte(itmlen);        //从串口缓存中,删除特征字符串
//	                break;              //返回符合的指令索引
//	            }
//	        }
//	    }

    
    socketid = GetOneChar(&byte);		    //获取第一个数字
    linelen = SER_LineLen(SYS_GPRS_PORT);
    //stat = GetOneChar(&byte);               //获取第二个数字(0正确,other失败)
//	    stat = itmidx;
//	    if(stat >= 4)
//	    {
//	        DevModem->stt.bit.pppoe = 1;        //ppp建立成功
//	    }
    if(socketid == SOCKET_TCP_CLIENT)
    {
        if(linelen > 9)
        {
            DevModem->stt.bit.tcpcc = 1;
        }
        else
        {
            DevModem->stt.bit.tcpcc = 0;
            //不符合步骤目标,步骤异常中止.
            if(gs_ModemStep == Modem_TcpCnt)
            {
                SYS_MODM_EndCnt();
                SYS_MODM_Error();
            }
            //
        }
    }
    else if(socketid == SOCKET_UDP_CLIENT)
    {
        if(linelen > 9)
        {
            DevModem->stt.bit.udpcc = 1;
        }
        else
        {
            DevModem->stt.bit.udpcc = 0;
            //不符合步骤目标,步骤异常中止.
            if(gs_ModemStep == Modem_UdpCnt)
            {
                SYS_MODM_EndCnt();
                SYS_MODM_Error();
            }
            //
        }
    }
                                            //根据tcp和udp链接状态确定ip状态
    if(DevModem->stt.bit.tcpcc || DevModem->stt.bit.udpcc)
    {
        DevModem->stt.bit.ipcc = 1;
    }
    else
    {
        DevModem->stt.bit.ipcc = 0;
    }

    DecodeRecvDataNone(mrfb);
}




/************************************************************************
 * @function: DecodeRecvIMEI
 * @描述: 获取通讯模块的IMEI
 * 
 * @参数: 
 * @param: mrfb 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvIMEI(ModemRecvFeedBack* mrfb)
{
    /*
    AT+CGSN? 
    +CGSN: “004400013805666” 
    OK
    */
    
    uint16 byte;
    uint8 ctget = false;                //是否扫描到有效内容的头

    ClearBuffer(gucs_IMEI, sizeof(gucs_IMEI));
    
                                        //扫描第一个双引号,最多扫描5个字符
    for(uint8 uc_i = 0; uc_i < 5; uc_i++)
    {
        byte = ReadNextByte();
        //if(byte == 0x100)
        //{
        //    return;
        //}
        
        if(byte != '\r'&& byte != '\n')
        {
            gucs_IMEI[0] = byte;
            ctget = true;
            break;
        }
    }
    
    if(!ctget)                          //未扫描到双引号,退出
    {
        return;
    }
                                        //IMEI号最多15字节
    for(uint8 uc_i = 1; uc_i < sizeof(gucs_IMEI); uc_i++)
    {
        byte = ReadNextByte();
        if((byte == '\r') || (byte == '\n'))
        {
            break;
        }
        gucs_IMEI[uc_i] = byte;
    }
                                        //删除行中剩余字符
    DecodeRecvDataNone(mrfb);
}




/************************************************************************
 * @function: DecodeRecvCCID
 * @描述: 获取SIM卡序列号
 * 
 * @参数: 
 * @param: mrfb 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvCCID(ModemRecvFeedBack* mrfb)
{
    /*
    AT+CCID? 
    +CCID: 89860018190839008096
    OK
    */
    
    uint16 byte;
    uint8 ctget = false;                //是否扫描到有效内容的头
    uint8 idx = 0;

    ClearBuffer(gucs_CCID, sizeof(gucs_CCID));

                                        //扫描第一个非空格字符,最多扫描5个字符
    for(uint8 uc_i = 0; uc_i < 5; uc_i++)
    {
        byte = ReadNextByte();
//	        if(byte == 0x100)
//	        {
//	            return;
//	        }
        
        if(byte != '\r'&& byte != '\n')
        {
            gucs_CCID[idx++] = byte;    //保存该字符到第一个位置
            ctget = true;
            break;
        }
    }
    
    if(!ctget)                          //未扫描到双引号,退出
    {
        return;
    }
                                        //新第二个位置开始保存
    for(uint8 uc_i = 1; uc_i < sizeof(gucs_CCID); uc_i++)
    {
        byte = ReadNextByte();
        if((byte == '\r') || (byte == '\n'))//if(byte == 0x100)
        {
            break;
        }
        gucs_CCID[uc_i] = byte;
    }
    
    DecodeRecvDataNone(mrfb);
}



/************************************************************************
 * @function: DecodeRecvCGMR
 * @描述: 获取模块版本信息
 * 
 * @参数: 
 * @param: mrfb 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvCGMR(ModemRecvFeedBack* mrfb)
{
    /*
    AT+CGMR
    +CGMR: "G510_V0D.00.16"
    OK
    */
    
    uint16 byte;
    uint8 ctget = false;                //是否扫描到有效内容的头

    ClearBuffer(gucs_ATMID, sizeof(gucs_ATMID));
    
                                        //扫描第一个双引号,最多扫描5个字符
    for(uint8 uc_i = 0; uc_i < 5; uc_i++)
    {
        byte = ReadNextByte();
        //if(byte == 0x100)
        //{
        //    return;
        //}
        
        if(byte == ' ')
        {
            ctget = true;
            break;
        }
    }
    
    if(!ctget)                          //未扫描到双引号,退出
    {
        return;
    }
    
    for(uint8 uc_i = 0; uc_i < sizeof(gucs_ATMID); uc_i++)
    {
        byte = ReadNextByte();
        if((byte == '\r') || (byte == '\n'))//if((byte == 0x100) || (byte == '"'))
        {
            break;
        }
        gucs_ATMID[uc_i] = byte;
    }
    
    DecodeRecvDataNone(mrfb);
}





/*******************************************************************************
*接收AT指令序列的枚举
*******************************************************************************/
typedef enum                      		
{
    M72X_READY = NUM_AT_RECVITM,

    M72X_SIMDROP,                           //SIM 掉卡
     
    M72X_MIPCALL,
    M72X_MIPCLOSE,                          //关闭连接/查询连接状态
    M72X_MIPOPEN,
    M72X_MIPRTCP,
    M72X_MIPRUDP,
    M72X_MIPSEND,
    M72X_MIPSTAT,                           //主动上报连接状态
    M72X_MIPSETS,                           //设置socket的buffersize&timerout回复,发送定长数据时timerout设0(用不到了)
    M72X_MIPPUSH,                           //+MIPSEND之前的发送状态上报帧

    M72X_IMEI,
    M72X_CCID,
    M72X_CGMR,
    M72X_QISACK,
    M72X_QISRVC,
    M72X_DEACT,
    M72X_ALREADY,
    M72X_CLOSE,
    M72X_CONNECTFAIL,
    M72X_SENDOK,
    M72X_RECEIVE,
    NUM_M72X_RECVITM,                       //可解析指令的数量
}M72X_RECV;


/*******************************************************************************
*接收AT指令序列
*******************************************************************************/
const string M72X_ATRecvHdr[NUM_M72X_RECVITM] =
{
    "CALL READY",
    
    "+SIM DROP",
      
    "AT+QILOCIP",//"+MIPCALL:",
    "CLOSE OK",//"+MIPCLOSE:",
    "CONNECT OK",   //"+MIPOPEN:",
    "+QIRD:",       //"+MIPRTCP:",
    "+MIPRUDP:",
    "+QTUNBUF:",      //"+MIPSEND:",
    "+QISTAT:",//"STATE: ",        //"+MIPSTAT:",
    "+MIPSETS:",
    "+MIPPUSH:",
    
    "AT+CGSN",                              //模块IMEI号
    "AT+QCCID",                              //SIM卡CCID号
    "Revision",//"+CGMR:",                              //模块版本信息
    "+QISACK:",
    "+QISRVC:",
    "DEACT",
    "ALREADY CONNECT",
    "CLOSE",
    "CONNECT FAIL",
    "SEND OK",
    "+RECEIVE:"
    
};
/************************************************************************
 * @function: DecodeRecvDataNone
 * @描述: 去除这一行中剩下的数据
 * 
 * @参数: 
 * @param: mrfb 处理后的信息结构(该函数用不到)
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvAlready(ModemRecvFeedBack* mrfb)
{
	uint16 byte;						//定义从串口读出的字节
	
	while(1)
	{
		byte = ReadNextByte();			//读取下一个字节
		if(byte == 0x0D || byte == 0x0A)//这一行读完则退出
		{
			break;
		}
		
		if(byte > 255)					//所有的数据都读完也退出
		{
			break;
		}	
	}
    mrfb->recv = M72X_MIPOPEN;
    //if(socketid == SOCKET_TCP_CLIENT)
    {
        {
            DevModem->stt.bit.tcpcc = 1;
        }
    }
    //else if(socketid == SOCKET_UDP_CLIENT)
    {
        {
            DevModem->stt.bit.udpcc = 1;
        }    
    }
    DevModem->stt.bit.ipcc = 1;
}


/*******************************************************************************
*AT指令接收预处理函数集
*******************************************************************************/
const DecodeRecvData gs_M72XDecodeRecvData[NUM_M72X_RECVITM] =
{
	DecodeRecvDataNone,					//M72X_READY,
    
    DecodeRecvSimDrop,
    
    DecodeRecvPPPStt,                   //M72X_MIPCALL,
    DecodeRecvDataNone,//DecodeRecvIPClose,                  //M72X_MIPCLOSE,
    DecodeRecvDataNone,                 //M72X_MIPOPEN, //仅用执行指令,不用查询指令,因此不用判定详细参数
	DecodeRecvTCPData,                  //M72X_MIPRTCP,
    DecodeRecvUDPData,                  //M72X_MIPRUDP,
    DecodeRecvIPSend,                   //M72X_MIPSEND,
	DecodeRecvIPStt,				    //M72X_MIPSTAT,
    DecodeRecvDataNone,                 //M72X_MIPSETS,
    DecodeRecvDataNone,                 //M72X_MIPPUSH,
    
    DecodeRecvIMEI,                     //M72X_IMEI
    DecodeRecvCCID,                     //M72X_CCID
    DecodeRecvCGMR,                     //M72X_CGMR
    
    DecodeRecvDataNone,
    DecodeRecvDataNone,
    DecodeRecvDataNone,
    
    DecodeRecvAlready,
    DecodeRecvIPClose,
    DecodeRecvIPFail,
    DecodeRecvDataNone,
    DecodeRecvTCPData,
};



/************************************************************************
 * @function: SYS_M72X_DecodeRecvCmd
 * @描述: 串口接收数据的解析处理
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_M72X_DecodeRecvCmd(void)
{
    uint8 order;
    uint16 byte;
    ModemRecvFeedBack mrfb;				//预处理的扫描结果

    if(DevModem->stt.bit.state_wait)    //不在等待'>'状态
    {
        //
    }                                   //如果没有找到行
    else if(SER_LineLen(SYS_GPRS_PORT) == 0)
    {
        if(SER_ToBeRead(SYS_GPRS_PORT) == LEN_UARTR_GPRS)
        {                               //如果缓存满了还没有找到行的话
            SER_Clear(SYS_GPRS_PORT); //清空缓存
        }
        return;
    }
    
    while(true)
    {                                   //扫描和识别合法指令
        order = ATCommandRecvMap(M72X_ATRecvHdr, NUM_M72X_RECVITM);
        
        if(order != 0xFF)               //已经扫描到了合法命令
        {                               //解析剩余内容
            if(order < NUM_M72X_RECVITM)
            {
                memset(&mrfb, 0x00, sizeof(ModemRecvFeedBack));
                mrfb.recv = order;
                
                if(order < NUM_AT_RECVITM)
                {
//#if (Modem_M72X_SMS_EN > 0)                
                    gs_STDATDecodeRecvData[order](&mrfb);
//#endif
                }
                else
                {
                    gs_M72XDecodeRecvData[order - NUM_AT_RECVITM](&mrfb);
                }
            
                if(DevModem->stt.bit.smWait == 0)	//不需要等待剩余的内容则产生接收xx
                {
                    SYS_MODM_SynRecv(&mrfb);
                }
            }
        }
        else
        {                               //不符合常规字符串,判定是否为短信数据
            if(DevModem->stt.bit.smWait)//短信接收
            {
                byte = ReadNextByte();
                if(IsBcdChar(byte))	    //判断是否是BCD数字
                {
                    DevModem->stt.bit.smWait = 0;
                                        //解析PDU短信
                    if(DecodeAndEnqueuePduMsg(byte))
                    {
                        memset(&mrfb, 0x00, sizeof(ModemRecvFeedBack));
                        mrfb.recv = AT_CMGR;	
                        mrfb.proced = DevModem->smsRev; //收到的短信
                        SYS_MODM_SynRecv(&mrfb);
                    }
                }
            }
                                        //删除整行
            if(order != 0xfe)
            {
                DequeueLine();
            }
        }
                                        //1)等待'>'
        if(DevModem->stt.bit.state_wait && 
           (SER_ToBeRead(SYS_GPRS_PORT) > 0))
        {
            continue;
        }
                                        //2)判断是否还存在新的一行
        if(SER_LineLen(SYS_GPRS_PORT) == 0)
        {
            SER_Clear(SYS_GPRS_PORT); //清空缓存(added by yzy 2014/11/13)
            break;				        //不存在则结束扫描
        }
    }
}





/************************************************************************
 * @function: M72X_Proc_DumbWait
 * @描述: 不做任何操作,继续等待下个数据
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
#define M72X_Proc_DumbWait AT_Proc_DumbWait










/************************************************************************
miss juice 2014/11/27 10:47:07
刘工,AT+MIPCALL=1这条指令发送到收到AT+MIPCALL=1,最大延时是多少?

广和通刘剑宇 2014/11/27 11:02:07
这个时间是网络决定，模块定义最大获取IP时间120S，超时就会上报失败

广和通刘剑宇 2014/11/27 11:03:25
有个超时获取设置

广和通刘剑宇 2014/11/27 11:06:07
AT+GTSET=”ACTRETRY”,X,Y
X: 取值范围：  5-30 ......... PDP激活的超时重试时间，单位：秒，模块默认30,
Y: 取值范围：2-4   激活的超时后，模块自动重新激活次数, 缺省4（不包括第一次超时次数）
************************************************************************/



/************************************************************************
 * @function: M72X_Proc_SetAPN
 * @描述: 设置APN
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8  0 解析成功进行下面的步骤
 * 		           1 解析失败不进行下面的步骤
 * @说明: 
 * @作者: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 M72X_Proc_SetAPN(void* content)
{
	uint8 len;
										//发送命令
	len = strlen((string)DevModem->log.apn);
	
	if(len > 32)						//如果大于32则返回错误
	{
		return 1;						//....
	}
    unsigned char uc_modembuff[MODEM_STRING_BUFF_LEN];
    memset(uc_modembuff,0,MODEM_STRING_BUFF_LEN);
    //+MIPCALL=<Operation>[<APN>/<phone number>,[<User name>,<Password>]]
    //其中<Operation>定义:
    //0-disconnect a link,
    //1-establish GPRS link,
    //2-establish CSD link
                                        //APN
//	    PrintValueData(
//	        "AT+QICSGP=1,\"%s\",\"",
//	        (uint32)DevModem->log->apn
//	        );
//	                                        //用户名
//	    PrintValueData(
//	        "%s\",\"",
//	        (uint32)DevModem->log->usr
//	        );
//	                                        //密码
//	    PrintValueData(
//	        "%s\"\r",
//	        (uint32)DevModem->log->pwd
//	        );
    SER_PrintValue(SYS_GPRS_PORT, uc_modembuff, MODEM_STRING_BUFF_LEN, MODEM_SER_TIMEOUT,
        "AT+QICSGP=1,\"%s\",\"%s\",\"%s\"\r",DevModem->log.apn,DevModem->log.usr,DevModem->log.pwd);
#if SYS_MNT_EN > 0                           //开始初始化AT通道
    LOG_DEBUG("%s\n", uc_modembuff);    
#endif

	SYS_MODM_StartCnt(500);             //启动超时计数器
    guc_ModemElement++;					//发送数据
	return 0;
}

/**************************************************************************
**初始化步骤
***************************************************************************/
const ModemElement gs_M72XInit[] =
{
    {"AT+QIMUX=1",      AT_OK, 500,  0, (CallBackElement)0},   //disable the function of MUXIP 

    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};

const ModemElement gs_M72XPPPoE[] =
{
	{"AT+QIFGCNT=0",    AT_OK, 200,  0, (CallBackElement)0},//set the context 0 as FGCNT	

    {"AT+CGATT=1",      AT_OK, 1200, 0, (CallBackElement)0},        //AT+CGATT=[<state>],附着GPRS网络或解除附着
//    {"AT+QIMODE=0",     AT_OK, 500,  0, (CallBackElement)0},    //set the session mode as non-transparent
    {"AT+QIDNSIP=0",    AT_OK, 2000,  0, (CallBackElement)0},    //use IP address as the address to establish TCP/UDP session
    {"_set APN",        AT_OK, 1000, 1, (CallBackElement)M72X_Proc_SetAPN},
    {"AT+QIDEACT",     AT_OK, 2000,  0, (CallBackElement)0},
    {"AT+QIREGAPP",     M72X_DEACT, 2000,  0, (CallBackElement)0},
    {"AT+QIACT",        AT_OK, 10000, 0, (CallBackElement)0},
	{(string)0, AT_OK, 0, 0, (CallBackElement)0},//{(string)0, M72X_MIPCALL, 0, 0, (CallBackElement)0},
};


const ModemElement gs_M72XDiscPPP[] =
{
    {"AT+CSQ", AT_OK, 200, 0, (CallBackElement)0},
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};




/************************************************************************
 * @function: M72X_Proc_MIPCONF
 * @描述: 使能发送状态上报功能
 * 
 * @参数: 
 * @param: content 上一步的执行结果
 * 
 * @返回: 
 * @return: uint8  0 解析成功进行下面的步骤
 *                  1 解析失败不进行下面的步骤 
 * @说明: 
 * @作者: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 M72X_Proc_MIPCONF(void* content)
{
    //AT+MIPCONF=<socket_ID>[[,<retr_num>][,<min_TO>][,<max_TO>][,<max_close_delay>] [,<is_nack_ind_req>]]
    //retr_num决定重发次数,也就是等待延时
    //is_nack_ind_req使能发送状态上报.
    //中间三个值建议不变,设为10,600,75
    uint8 SocketID;
    unsigned char uc_modembuff[64];
    
//	    if(gs_ModemStep == Modem_TcpCnt)	    //建立TCP
//		{
//	        SocketID = SOCKET_TCP_CLIENT;
//		}
//		else                                    //建立UDP
//		{
//	        SocketID = SOCKET_UDP_CLIENT;
//		}
//	    
//	                                            //Socket_ID
//	    PrintValueData(
//	        "AT+MIPCONF=%d,",
//	        SocketID
//	        );
//	    
//	    SYS_MODM_SendCmd("5,10,600,75,2", 13, 20);

    SER_PrintValue(SYS_GPRS_PORT, uc_modembuff, 64, MODEM_SER_TIMEOUT,
        "AT+QITCFG=%s\r\n","3,2,512,1");
#if SYS_MNT_EN > 0                           //开始初始化AT通道
    LOG_DEBUG("%s\n", uc_modembuff);    
#endif

    SYS_MODM_StartCnt(2000);
    guc_ModemElement++;				        //分步骤累加
    return 0;
}


/************************************************************************
 * @function: M72X_Proc_IPOpen
 * @描述: 创建IP连接
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8  0 解析成功进行下面的步骤
 *                  1 解析失败不进行下面的步骤
 * @说明: 
 * @作者: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 M72X_Proc_IPOpen(void* content)
{
	Word32 wd;
	uint8 uc_data[21];
    uint8 i;
    uint8 nettype;
    uint8 SocketID;
    uint8 * socketType = NULL;
    unsigned char uc_modembuff[MODEM_STRING_BUFF_LEN];
    TModemIPAddr* tia = &DevModem->ipAddr;   //非步骤首条指令,使用全局量而非出传入(TModemIPAddr*)content;
    wd.lword = tia->addr;

    if(gs_ModemStep == Modem_TcpCnt)	    //建立TCP
	{
	    socketType = SOCKET_TCP_STR;
        SocketID = SOCKET_TCP_CLIENT;
        nettype = NET_TYPE_TCPCLIENT;
	}
	else                                    //建立UDP
	{
	    socketType = SOCKET_UDP_STR;
        SocketID = SOCKET_UDP_CLIENT;
        nettype = NET_TYPE_UDP;
	}

    //AT+MIPOPEN=<Socket_ID>,<Source_Port>,<Remote_IP>,<Remote_Port>,<Protocol>
    /************************************************
    AT+MIPOPEN=1,34127,"217.219.84.194",1024,0 OK
    +MIPSTAT: 1,1
    AT+MIPOPEN=1,34127,"217.219.84.194",1024,0 OK
    +MIPOPEN: 1,1
    *************************************************/
                                            //Socket_ID
//	    PrintValueData(
//	        "AT+MIPOPEN=%d,",
//	        SocketID
//	        );
//	                                            //Source_Port
//	    PrintValueData(
//	        "%d,\"",
//	        (gul_Random % (49999 - 5000)) + 5001//取  5001--49999  之间的随机数
//	        );

	for(i = 0; i < 4; i++)                  //循环转换IP地址
	{
		NumToBcdString(uc_data + i * 4, wd.byte[i], 3);
		uc_data[i * 4 + 3] = '.';
	}
											//转换端口号
//	    for(i = 0; i < 3; i++)                  //打印目标地址前3字节
//		{
//	        PrintValueData(
//	            "%d.",
//	            wd.byte[i]
//	            );
//		}
//	                                            //打印目标地址第四字节
//		PrintValueData(
//	        "%d\"",
//	        wd.byte[3]
//	        );
//	                                            //发送端口号
//	    PrintValueData(
//	        ",%d,",
//	        tia->port
//	        );
//	                                            //协议类型
//	    PrintValueData( 
//	        "%d\r",
//	        nettype
//	        );

    SER_PrintValue(SYS_GPRS_PORT, uc_modembuff, MODEM_STRING_BUFF_LEN, MODEM_SER_TIMEOUT,
        "AT+QIOPEN=%d,\"%s\",\"%d.%d.%d.%d\",\"%u\"\r\n", SocketID, socketType,wd.byte[0], 
        wd.byte[1],wd.byte[2],wd.byte[3],tia->port);

#if SYS_MNT_EN > 0                           //开始初始化AT通道
    LOG_DEBUG("%s\n", uc_modembuff);    
#endif

	SYS_MODM_StartCnt(90000);                  //建议超时设置1.5分钟
	guc_ModemElement++;						//分步骤累加
	
	return 0;
}



/************************************************************************
18:22:44
广和通刘剑宇 2014/11/20 18:22:44
"MIPCONF"仅最后一个参数有用,
指令设置后，成功发送到服务器后会自动上报，如下。具体使用见手册说明。
  建议使用如下设置参数。（建议后面暂时做调试使用）

AT+MIPCALL=1,"CMNET"
OK
+MIPCALL: 10.255.198.138
AT+MIPCONF=1,5,5,600,75,2  --设置服务器成功收到数据后，上报。
OK
AT+MIPOPEN=1,,"203.195.189.64",3000,0
OK
+MIPOPEN: 1,1
AT+MIPSEND=1,5
>12345
OK
+MIPPUSH: 1,0,5  ---表示服务器成功接到数据
************************************************************************/

/************************************************************************
广和通-刘剑宇  12:03:08
AT+MIPSETS=1,1024,100 ERROR  
这条指令G610-A20模块是不支持的,A50支持
广和通-刘剑宇  12:09:45
AT+MIPSEND=1,5你们是定长发送数据
掌剪草  12:10:06
但是那个超时时间不设置，没关系么？
广和通-刘剑宇  12:10:07
发送定长数据就不需要使用这个指令,没关系的
发送不定长数据时,需要设置超时时间.
************************************************************************/

/********A20与A50模块关于IPOPEN指令的差异*********************************
*A20:先返回“OK”,再返回“+MIPOPEN: 1,1”;侦听时,客户端连接上时,上报的IP地址没有引号
*A50:先返回“+MIPOPEN: 1,1”,再返回“OK”;上报的IP地址有引号.
************************************************************************/
const ModemElement gs_M72XTcpCnt[] =
{
//	    {"AT+QIREGAPP",     AT_OK, 200,  0, (CallBackElement)0},
//	    {"AT+QIACT",        AT_OK, 5000, 0, (CallBackElement)0},
    {"ATE1", AT_OK, 500, 0, (CallBackElement)0},
    {"AT+QILOCIP",        AT_OK, 5000, 0, (CallBackElement)0},
    {"ATE0", M72X_MIPCALL, 500, 0, (CallBackElement)0},
    //{"_ip config",      AT_OK, 500, 1, (CallBackElement)M72X_Proc_MIPCONF},//yzy 建议设置,避免不同模块版本默认值不同
//	    {"AT+QIMUX=1",      AT_OK, 500,  0, (CallBackElement)0},
    {"_open ip link",   AT_OK, 500, 1, (CallBackElement)M72X_Proc_IPOpen},
//	    {"AT+QIMUX=0",      AT_OK, 500,  0, (CallBackElement)0},
    {"AT+QTUNBUF?",     M72X_MIPOPEN, 200, 0, (CallBackElement)0},          //需兼容A20&A50MIPSEND
	{"_dump wait",      M72X_MIPSEND, 0, 1, (CallBackElement)M72X_Proc_DumbWait},
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};

#define gs_M72XUdpCnt gs_M72XTcpCnt		//说明UDP连接的步骤和TCP是一致的






/************************************************************************
 * @function: M72X_Proc_DisCntTcp
 * @描述: 断开GPRS连接
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 M72X_Proc_DisCntTcp(void* content)
{
    uint8 chn = (uint32)content;
    uint8 SocketID;

    SocketID = SOCKET_TCP_CLIENT;
                                        //发送整条指令//AT+MYNETCLOSE=<socketid>
    PrintValueData(
        "AT+QICLOSE\r",
        SocketID
        );
#if SYS_MNT_EN > 0                           //开始初始化AT通道
    LOG_DEBUG("TCP:AT+QICLOSE\n");    
#endif
    SYS_MODM_StartCnt(5000);
    return 0;

}



/************************************************************************
 * @function: M72X_Proc_DisCntUdp
 * @描述: 断开GPRS连接
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 M72X_Proc_DisCntUdp(void* content)
{
    //uint8 chn = (uint32)content;
    uint8 SocketID;
	
    SocketID = SOCKET_UDP_CLIENT;
//	                                        //发送整条指令//AT+MYNETCLOSE=<socketid>
//	    PrintValueData(
//	        "AT+MIPCLOSE=%d\r",
//	        SocketID
//	        );
    PrintValueData(
        "AT+QICLOSE\r",
        SocketID
        );
#if SYS_MNT_EN > 0                           //开始初始化AT通道
    LOG_DEBUG("UDP:AT+QICLOSE\n");    
#endif    
    SYS_MODM_StartCnt(5000);
    return 0;

}

const ModemElement gs_M72XDisTcp[] = 
{
    {"_close tcp link", AT_OK, 3000, 1, (CallBackElement)M72X_Proc_DisCntTcp},
    {(string)0, M72X_SENDOK, 50, 0, (CallBackElement)0},    //在接收函数中处理
};


const ModemElement gs_M72XDisUdp[] = 
{
    {"_close udp link", AT_OK, 3000, 1, (CallBackElement)M72X_Proc_DisCntUdp},
    {(string)0, M72X_SENDOK, 50, 0, (CallBackElement)0},    //在接收函数中处理
};





/************************************************************************
 * @function: M72X_Proc_SendTcpCmd
 * @描述: 发送远程数据命令
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 M72X_Proc_SendTcpCmd(void* content)
{
    //AT+MIPSEND=<Socket_ID>,<Data>
                                        //发送数据
    PrintValueData(
        "AT+QISEND=0,%d\r",            //SOCKET_TCP_CLIENT
        DevModem->ipSend->len
        );
#if SYS_MNT_EN > 0                           //开始初始化AT通道
    LOG_DEBUG("AT+QISEND=0,%d\r\n", DevModem->ipSend->len);    
#endif
	SYS_MODM_StartCnt(5000);               //启动超时计数器
                                        //
	DevModem->stt.bit.state_wait = 1;   //开始等待>
	guc_ModemElement++;		
    return 0;							//表示准备数据成功
}


/************************************************************************
 * @function: M72X_Proc_SendUdpCmd
 * @描述: 发送远程数据命令
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 M72X_Proc_SendUdpCmd(void* content)
{
    //AT+MIPSEND=<Socket_ID>,<Dlen>
    //>
    //HEXDATA
                                        //发送数据
    PrintValueData(
        "AT+MIPSEND=1,%d\r",            //SOCKET_UDP_CLIENT
        DevModem->ipSend->len
        );
#if SYS_MNT_EN > 0                           //开始初始化AT通道
    LOG_DEBUG("AT+MIPSEND=1,%d\r\n", DevModem->ipSend->len);    
#endif
	SYS_MODM_StartCnt(5000);               //启动超时计数器
                                        //
	DevModem->stt.bit.state_wait = 1;   //开始等待>
	guc_ModemElement++;		
    return 0;							//表示准备数据成功
}


/************************************************************************
 * @function: GDAT_Proc_SendIPFlow
 * @描述: 发送IP数据
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 M72X_Proc_SendIPFlow(void* content)
{
    //AT+MIPSEND=<Socket_ID>,<Dlen>
    //>
    //HEXDATA
//	    PrintValueData(
//	        "AT+QISEND=%d\r\n>",            //SOCKET_TCP_CLIENT
//	        DevModem->ipSend->len
//	        );    
//	#if SYS_MNT_EN > 0                           //开始初始化AT通道
//	    LOG_DEBUG("AT+QISEND=%d\r\n", DevModem->ipSend->len);    
//	#endif
    
                                        //发送(yzy,可以不用发送"\r\n")
    SendCommadData(DevModem->ipSend->buffer, DevModem->ipSend->len);
#if SYS_MNT_EN > 0                           //开始初始化AT通道
    LOG_DUMPHEX(LOG_LEVEL_DEBUG,"", DevModem->ipSend->buffer,DevModem->ipSend->len);    
#endif

    SYS_MODM_StartCnt(20000);              //启动超时计数器
	guc_ModemElement++;	
    return 0;
}


const ModemElement gs_M72XTcpSend[] =
{
	{"_tcp send start", AT_OK, 0, 1, (CallBackElement)M72X_Proc_SendTcpCmd},
	{"_tcp send data", AT_GT, 0, 1, (CallBackElement)M72X_Proc_SendIPFlow},
//	    {"_wait again", M72X_SENDOK, 0, 1, (CallBackElement)M72X_Proc_DumbWait},
    {(string)0, M72X_SENDOK, 0, 0, (CallBackElement)0},
};


const ModemElement gs_M72XUdpSend[] =
{
	{"_udp send start", AT_OK, 0, 1, (CallBackElement)M72X_Proc_SendUdpCmd},
	{"_udp send data", AT_GT, 0, 1, (CallBackElement)M72X_Proc_SendIPFlow},
//	    {"_wait again", M72X_SENDOK, 0, 0, (CallBackElement)M72X_Proc_DumbWait},
    {(string)0, M72X_SENDOK, 0, 0, (CallBackElement)0},
};










//查询状态时,不查询PPP状态,只查询IP状态.当IP连接失败时,断开PPP+IP并全部按需重连.
const ModemElement gs_M72XIPStt[] =
{
	{(string)"AT+QISTAT", AT_OK, 600, 0, (CallBackElement)0},//QISTAT
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};


/************************************************************************
 * @function: M72X_Proc_ShutDown
 * @描述: 关闭
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/11/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 M72X_Proc_ShutDown(void* content)
{
    //发送三帧数据,只要有一帧回复OK就算步骤完成.
    
//	    SYS_MODM_SendCmd("AT+GTSIMRST", 11, 0);//执行AT+GTSIMRST 复位SIM卡
//	    sleep_ms(100);                     //不判接收
  
    SYS_MODM_SendCmd("AT+CFUN=1,1", 11, 0);//Hardware reset.
    sleep_ms(50);                     //不判接收
    
    SYS_MODM_SendCmd("AT+QPOWD=1", 10, 0);//G610-A50执行AT+CPWROFF 后再去关机
    sleep_ms(50);                     //不判接收
    
//	    SYS_MODM_SendCmd("AT+MRST", 7, 0);    //G610-A20执行AT+MRST 后再去关机

    SYS_MODM_StartCnt(2000);                //启动超时计数器
	guc_ModemElement++;
    return 0;
}


const ModemElement gs_M72XReset[] =
{
	{"_shutdown before reset", AT_OK, 50, 1, (CallBackElement)M72X_Proc_ShutDown},
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};

#define  gs_M72XShutDown   gs_M72XReset




const ModemElement gs_M72XInfo[] =
{
    
    {"ATE1",        AT_OK,      500, 0, (CallBackElement)0},
    {"AT+CGSN",     AT_OK,      500, 0, (CallBackElement)0},      //获取模块IMEI
    {"_dumb wait",  M72X_IMEI,  200, 1, (CallBackElement)M72X_Proc_DumbWait},
    {"AT+QCCID",    AT_OK,      600, 0, (CallBackElement)0},      //获取SIM卡CCID
    {"_dumb wait",  M72X_CCID,  200, 1, (CallBackElement)M72X_Proc_DumbWait},
    {"ATE0", AT_OK, 200, 0, (CallBackElement)0},    
    {"AT+CGMR",     AT_OK,      500, 0, (CallBackElement)0},      //获取模块版本信息
    {"_dumb wait",  M72X_CGMR,  200, 1, (CallBackElement)M72X_Proc_DumbWait},
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};


//非标准AT指令步骤集合
const ModemElement* const gs_M72X[] =
{
    gs_M72XPPPoE,                       //Modem_PPPoE,
    gs_M72XDiscPPP,                     //Modem_PPPDiscnt,
    gs_M72XTcpCnt,                      //Modem_TcpCnt,
    gs_M72XUdpCnt,                      //Modem_UdpCnt,
    gs_M72XDisTcp,                      //Modem_TcpDiscnt,
    gs_M72XDisUdp,                      //Modem_UdpDiscnt,
    gs_M72XTcpSend,                     //Modem_TcpSend,
    gs_M72XUdpSend,                     //Modem_UdpSend,
    gs_M72XIPStt,                       //Modem_IPState,
    gs_M72XReset,                       //Modem_Reset,
    gs_M72XShutDown,                    //Modem_ShutDown,
    (void*)0,                           //Modem_RecvIPData, //M72X接收数据均主动上报,无读取操作
    gs_M72XInfo,                        //Modem_Info
    gs_M72XInit,                        //Modem_Spec_Init

};




/************************************************************************
 * @function: SYS_M72X_ProcRecvAtCommand
 * @描述: 模块解析预处理好的接收指令,在部件步骤中
 * 
 * @参数: 
 * @param: mrfb 
 * @返回: 
 * @说明: 在 SYS_TASK_MODM 进程中调用
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_M72X_ProcRecvAtCommand(ModemRecvFeedBack* mrfb)
{
	ModemElement* me;
	
	DevModem->stt.bit.state_wait = 0;	//任何部件的开始都不需要等待
	
	if(gs_ModemStep >= NUM_ALL_MODEMSTEP)
	{
		DevModem->stt.bit.state_mid = 0;
	}
	                                    //获取当前的结构指针
    if(gs_ModemStep >= NUM_STD_MODEMSTEP)
    {
        me = (ModemElement*)gs_M72X[gs_ModemStep - NUM_STD_MODEMSTEP];
    }
    else
    {
//#if (Modem_M72X_SMS_EN > 0)     
        me = (ModemElement*)gs_STDAT[gs_ModemStep];
//#endif
    }
    
    if(me == (void*)0)			        //如果该部件不支持
    {
        return;
    }
    
	me += guc_ModemElement;
	
    switch(mrfb->recv)
    {
        case M72X_READY:                //主动上报:模块首次准备就绪时上报.
            SYS_MODM_Step(Modem_Init);
            break;

        case AT_CMTI:                 //主动上报:新信息提醒
            SYS_MODM_Step(Modem_ListMsg);
            break;

            //M72X系列产品,接收到数据后直接将数据内容以unsolicited类型主动上报
            //而非先上报接收数据基本信息,然后再由用户去读取.
        case M72X_MIPRTCP:              //主动上报:新IP数据
        case M72X_MIPRUDP:
        case M72X_RECEIVE:
            //SYS_MODM_Step(Modem_RecvIPData);
            if(DevModem->ipRev->len > 0)
            {                           //通知应用层读取缓存中的IP数据
                DevModem->stt.bit.ipRevData = 1;
            }
//	            if((DevModem->gsRev[0]->len > 0) || (DevModem->gsRev[1]->len > 0))
//	            {
//	                DevModem->stt.bit.gsip = 1;
//	            }
            break;

        case AT_RING:
            if(guc_M72XRingCount == 0)
            {
                guc_M72XRingCount++;
            }
            else
            {
                guc_M72XRingCount = 0;
                DevModem->stt.bit.ringRev = 1;
                SYS_MODM_Step(Modem_HoldUp);
            }
            break;

        case AT_ERROR:
            if(gs_ModemStep == Modem_PPPDiscnt)
            {
                DevModem->stt.bit.pppoe = 0;
                DevModem->stt.bit.ipcc = 0;
                DevModem->stt.bit.tcpcc = 0;
                DevModem->stt.bit.udpcc = 0;
            }
            SYS_MODM_EndCnt();		//结束超时计数器的计时	
            SYS_MODM_Error();		//收到ERROR之后的处理
            break;

        case AT_CME:
            SYS_MODM_EndCnt();		//结束超时计数器的计时	
            SYS_MODM_Error();		//收到ERROR之后的处理
            break;
            			
        default:
            if(mrfb->recv == me->recv)
            {
                SYS_MODM_EndCnt();	//结束超时计数器的计时	
                if(me->flag & 0x01)
                {
                    if(me->cbe(mrfb->proced) != 0)
                    {
                        SYS_MODM_Error();
                        break;      //跳出swtich,case
                    }
                    
                    if(me->send == (string)0)
                    {
                        SYS_MODM_Finish();
                    }
                }
                else if(me->send != (string)0)
                {
                                    //发送后续元素的命令	
                    SYS_MODM_SendCmd((uint8*)me->send, strlen(me->send), me->overtime);
                                    //子步骤增加
                    guc_ModemElement++;
                }
                else				//否则就表示已经顺利完成了所有的步骤
                {
                    DevModem->stt.bit.state_mid = 0;
                    guc_ModemElement = 0;
                                    //操作成功后的处理
                    SYS_MODM_Finish();
                }
            }
            else
            {
                //此处不宜做操作.
                //1.只要在步骤中定义想要等待的回复帧,在等待期间收到其他帧,先判别是否为需要处理的主动上报帧.
                //对既不做主动上报帧处理,又不是步骤想要的帧.直接舍弃,并继续等待.
                //
            }
    }

}



/************************************************************************
 * @function: SYS_M72X_ProcUnsolictData
 * @描述: 模块主动上告数据的处理
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_M72X_ProcUnsolictData(ModemRecvFeedBack* mrfb)
{
    switch(mrfb->recv)
    {
        case M72X_READY:                //主动上报:模块首次准备就绪时上报.
            SYS_MODM_Step(Modem_Init);
            break;

        case AT_CMTI:                 //主动上报:新信息提醒
            SYS_MODM_Step(Modem_ListMsg);
            break;

        case M72X_MIPRTCP:			    //主动上报:新IP数据提醒
        case M72X_MIPRUDP:
        case M72X_RECEIVE:
            //SYS_MODM_Step(Modem_RecvIPData);
            if(DevModem->ipRev->len > 0)
            {                           //通知应用层读取缓存中的IP数据
                DevModem->stt.bit.ipRevData = 1;
            }
//	            if((DevModem->gsRev[0]->len > 0) || (DevModem->gsRev[1]->len > 0))
//	            {
//	                DevModem->stt.bit.gsip = 1;
//	            }
            break;

        case AT_RING:
            if(guc_M72XRingCount == 0)
            {
                guc_M72XRingCount++;
            }
            else
            {
                guc_M72XRingCount = 0;
                DevModem->stt.bit.ringRev = 1;
                SYS_MODM_Step(Modem_HoldUp);
            }
            break;
    }
}


/************************************************************************
 * @function: SYS_M72X_ModemMainLoop
 * @描述: 循环
 * 
 * @参数: 
 * @param: msc 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_M72X_ModemMainLoop(ModemStepCommand* msc)
{
    ModemElement* me;
                                        //散转合法性的判断
    if(msc->command >= NUM_ALL_MODEMSTEP)
    {
        return;			            //非法则直接返回
    }
                                        //获取当前部件的命令步骤
    if(msc->command >= NUM_STD_MODEMSTEP)
    {
        me = (ModemElement*)gs_M72X[msc->command - NUM_STD_MODEMSTEP];
    }
    else
    {
//#if (Modem_M72X_SMS_EN > 0)     
        me = (ModemElement*)gs_STDAT[msc->command];
//#endif
    }		
    
    if(me == (void*)0)			        //如果该部件不支持
    {
        return;
    }
    
    gs_ModemStep = msc->command;	    //当前部件
	
    DevModem->stt.bit.state_wait = 0;   //任何部件的开始都不需要等待

    if(me->flag & 0x01)			        //判断是否需要特殊处理
    {
        switch(me->cbe(msc->msg))	    //进行特殊处理之后再进行后续处理
        {
            case 0:
                DevModem->stt.bit.state_mid = 1; 
                guc_ModemElement = 1;	//元素步骤清零
                break;
            case 1:
                SYS_MODM_Error();	    //处理错误
                break;
        }
    }
    else				                //否则直接发送数据
    {
        SYS_MODM_SendCmd((uint8*)me->send, strlen(me->send), me->overtime);
        DevModem->stt.bit.state_mid = 1;//当前转入不可打断状态
        guc_ModemElement = 1;		    //元素步骤清零
    }
}






