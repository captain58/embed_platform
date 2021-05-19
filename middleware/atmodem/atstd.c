/****************************************Copyright (c)**************************************************
**                               	____科技有限公司
**                                     		开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: atstd.c
**创   建   人: yzy
**最后修改日期: 2014年11月19日
**描        述: 通用AT指令集的驱动函数集
**注        意: GSM Rec07.07/GSM Rec07.05/ITU-T Rec.V25ter
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2014年11月19日
** 描　述: 原始版本
********************************************************************************************************/
#define EXT_ATSTD

#include "public.h"
#include "k_types.h"
#include "k_api.h"
//	#include "lib_app.h"
#include "hal.h"


#include "atmodem.h"

#include "modemlib.h"
#include "atstd.h"




/************************************************************************
 * 驱动层短信处理流程
 <1>收到unsolicited指令.+CMTI: <mem>,<index>
 <2>List所有短信,保存各短信的存储索引到FIFO中
    AT+CMGL=4
    +CMGL: 1,1,,160 xxxxxx
    +CMGL: 2,1,,82 xxxxxx
    +CMGL: 3,0,,155 xxxxxx
 <3>若buffer中无未处理短信,
        <3.1>从FIFO中出列一条未读短信索引
        <3.2>Read该索引对应的短信内容到buffer
        <3.3>发送消息通知应用层处理.
        <3.4>Delete已读取的短信.AT+CMGD=X
   若buffer中有未处理短信,等待应用层处理后再继续后续操作.

 *----------------------------------------------------------
 <4>定时查询接收短信FIFO中是否仍存在未读短信,存在则继续<3>.
 *----------------------------------------------------------
 *
 * 按照该机制设计后,接收处理后立即删除,通常情况下存储空间中不会积压多条短信
 ************************************************************************/


/************************************************************************
 *驱动层与应用层交互状态控制时序图
 *DRIVER TF
 *-------------------------------------------------------------
     |读取SMS到buffer|
    (1)             (2)
                    \
                    \
                    \
                    V
                    (3)       (4)                           (5)
                                |应用层处理buffer中数据|
 *-------------------------------------------------------------
 *APP TF
 说明:
     (2)对应驱动层<3.2>步骤完成的时刻
     (1)-(2)为驱动层执行SMS操作的时间.
     (2)-(3)表示驱动层通知应用层buffer数据中有未处理的短信.
     (4)-(5)为应用层处理buffer数据的时间
     
     stt.bit.smsBufSem:
        buffer占用时间(1)-(5),占用期间不能执行SMS读取操作来覆盖buffer内容.
     stt.bit.smsdex:
        buffer数据有效(2)-(5),该时间段buffer中存在有效数据,且未被应用层处理.
************************************************************************/






/************************************************************************
**需要删除的短信的索引
************************************************************************/
uint16 gui_DelSmgIndex;


/************************************************************************
 * @function: DecodeRecvDataCMTI
 * @描述: CMTI后需要欲处理的内容
 * 
 * @参数: 
 * @param: mrfb 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvDataCMTI(ModemRecvFeedBack* mrfb)
{
    //+CMTI: <mem>,<index>
    //例+CMTI: "SM",1 表示新接收信息,存储位置SM_1
    //例+CMTI: "ME",7 表示新接收信息,存储位置ME_7
	
	DecodeRecvDataNone(mrfb);       //对于我们的应用,不用关心上报信息.
}



/************************************************************************
 * @function: DecodeRecvDataCMGL
 * @描述: CMGL后需要欲处理的内容,读取短信列表后执行的函数
 * 
 * @参数: 
 * @param: mrfb 处理后的信息结构
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvDataCMGL(ModemRecvFeedBack* mrfb)
{
    //+CMGL:<index>,<stat>,[<alpha>],<length><CR><LF><pdu><CR><LF>
    
	uint16 byte;
	uint16 smsidx;
										
	smsidx = GetOneChar(&byte);	        //获取一个字节的信息
	
	if((smsidx != 0xffff) && (smsidx != 0))//将新短信放入列表
	{
        SYS_FIFO_Enqueue(&gs_FifoNewMessage, &smsidx);//需等待所有CMGL都入队后,再进行读取
		sleep_ms(4);                              //等待数据接收完全,其实内容无用,但需收全末尾的0x0A等.
	}
	
	if((byte != 0x0D) && (byte != 0x0A))//如果还没有到行尾
	{
		DecodeRecvDataNone(mrfb);		//去除本行剩余的无效信息
	}
}


/************************************************************************
 * @function: DecodeRecvDataCMGR
 * @描述: CMGR后需要欲处理的内容,收到短信（且需等待短信接收）
 * 
 * @参数: 
 * @param: mrfb 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/11/13)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DecodeRecvDataCMGR(ModemRecvFeedBack* mrfb)
{
    //AT+CMGR=<index>
    //+CMGR:<stat>,[<alpha>],<length><CR><LF><pdu>
    
	mrfb = mrfb;
	DecodeRecvDataNone(mrfb);			//去除本行剩余的无效信息
	sleep_ms(10);                    //等待短信接收
	DevModem->stt.bit.smWait = 0x01;	//不直接发同步消息,而是在解析完短信后再发消息
}



/*************************************************************************
*接收AT指令序列                       //与STDAT_RECV对应
**************************************************************************/
const string STD_ATRecvHdr[NUM_AT_RECVITM] =
{
    "OK",
    
    "RING",
    "ERROR",
    "NO CARRIER",
    "BUSY",
    "+ABC",
    ">",
    "+CSQ:",
    "+CREG:",
    "+CME:",                            //错误提醒
    "+CMTI:",                           //新短信提醒
    "+CMGS:",
    "+CMGR:",
    "+CMGD:",
    "+CMGL:",
};




/*************************************************************************
*AT指令接收预处理函数集                 //与STDAT_RECV对应
**************************************************************************/
const DecodeRecvData gs_STDATDecodeRecvData[] =
{
    DecodeRecvDataNone,					//AT_OK = 0,
    
    DecodeRecvDataNone,                 //AT_RING,
    DecodeRecvDataOneChar,              //AT_ERROR,
    DecodeRecvDataNone,                 //AT_NC,
    DecodeRecvDataNone,                 //AT_BS,
    DecodeRecvDataNone,                 //AT_CONNECT,接受远程连接指令回复
    DecodeRecvDataNone,                 //AT_GT,
    DecodeRecvDataOneChar,              //AT_CSQ,
    DecodeRecvDataTwoChar,              //AT_CREG,
    DecodeRecvDataNone,                 //AT_CME,
    DecodeRecvDataCMTI,                 //AT_CMTI,
    DecodeRecvDataNone,                 //AT_CMGS,
    DecodeRecvDataCMGR,                 //AT_CMGR,
    DecodeRecvDataNone,                 //AT_CMGD,
    DecodeRecvDataCMGL,                 //AT_CMGL,
    
};




/************************************************************************
 * @function: ATCommandRecvMap
 * @描述:  扫描串口缓存中的数据是否符合命令序列
 * 
 * @参数: 
 * @param: atRecvHdr at指令识别码字符串列表
 * @param: itmnum 列表项长度
 * 
 * @返回: 
 * @return: uint8   0xFF 行首数据不符合指令特征
 *                  0xfe 无行首数据/执行清空缓存
 *                  others 扫描到符合指令的索引编号/删除串口缓存行特征字符串
 *
 * @说明: AT指令规则
 *      1)接收短信时分两行回复,第二行没有特征字符
 *      2)发送SMS,或某些模块发送IP数据时.先发送发送概要,然后需等待模块回复'>'(非完整行)
 *      3)除了上述两种特例外,收到的帧基本可以通过两个步骤来判定类型:读取行+判定特征字符
 *      4)某些指令如+MIPRTCP,
 *              有时先收到数据,帧超时后再收到行结尾符0x0D/0x0A,
 *              有时会先收到0x0D/0x0A后跟数据,无结尾符.
 *              因此3)应修正为:读取行头数据+判定特征字符
 * @作者: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 ATCommandRecvMap(const string* atRecvHdr, uint8 itmnum)
{
    uint8 rdbuf[96];                        //读取数据的缓存,需要能容纳最长的特征字符串
    uint16 rdlen = 0;                        //已读取数据的长度
    uint16 linelen = 0;
    uint16 ebytes;

	uint16 itmlen;                          //当前特征字符串长度
    uint8 itmidx;                           //AT指令的索引
                                            //0)读取带数据首行中,大于最长特征字符串长度的数据
    while(1)
    {
        ebytes = 0;
        memset(rdbuf,0,96);
        rdlen = PeekByte(rdbuf, 0, sizeof(rdbuf));
        if(rdlen == 0)
        {
            return 0xfe;
        }
                                            //剔除行首数据前的结尾符0x0D/0x0A,保证存放行的首个字节
        linelen = LineLen(rdbuf, rdlen, &ebytes);
        if(linelen == 1)                    //only end character
        {
            DequeueByte(ebytes);
            continue;
        }
        else
        {
            break;
        }
    }
#if SYS_MNT_EN > 0                           //开始初始化AT通道
    LOG_DEBUG("%s\n", rdbuf);    
#endif
    //搜索标准指令集
    for(itmidx = 0; itmidx < NUM_AT_RECVITM; itmidx++)
    {                                       //1)获取该AT指令特征字符串的识别长度
        itmlen = strlen(STD_ATRecvHdr[itmidx]);
//	        ASSERT(itmlen <= sizeof(rdbuf));
                                            //2)读取的长度>=特征字符串长度,执行指令识别操作
        for(uint8 idx = 0; idx <= 3; idx++)
        {
            if(rdlen - idx >= itmlen) 
            {
                if(0 == CmpBuffer(rdbuf+idx, (uint8*)STD_ATRecvHdr[itmidx], itmlen))
                {
                    if(idx == 0)
                    {
                        DequeueByte(itmlen);        //从串口缓存中,删除特征字符串
                    }
                    return itmidx;              //返回符合的指令索引
                }
            }
        }
    }
    
    //搜索模块自定义指令集
    for(itmidx = 0; itmidx < itmnum - NUM_AT_RECVITM; itmidx++)
    {                                       //1)获取该AT指令特征字符串的识别长度
        itmlen = strlen(atRecvHdr[itmidx]);
//	        ASSERT(itmlen <= sizeof(rdbuf));
                                            //2)读取的长度>=特征字符串长度,执行指令识别操作
        for(uint8 idx = 0; idx <= 3; idx++)
        {
            if(rdlen - idx >= itmlen) 
            {
                if(0 == CmpBuffer(rdbuf + idx, (uint8*)atRecvHdr[itmidx], itmlen))
                {
                    if(idx == 0)
                    {
                        DequeueByte(itmlen);        //从串口缓存中,删除特征字符串
                    }
                    return (NUM_AT_RECVITM + itmidx);//返回符合的指令索引
                }
            }
        }
    }

    return 0xff;
}







/**************************************************************************
**初始化步骤
***************************************************************************/
const ModemElement gs_ATInit[] =
{
    {"ATE0", AT_OK, 500, 0, (CallBackElement)0},
    {"AT+CNMI=2,1", AT_OK, 200, 0, (CallBackElement)0},        //设置消息提示模式,GSM_Rec_07.05
    {"AT+CMGF=0", AT_OK, 200, 0, (CallBackElement)0},          //设置SMS格式,GSM_Rec_07.05
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};



/************************************************************************
 * @function: AT_Proc_CSQ
 * @描述: 对信号质量进行解析
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 AT_Proc_CSQ(void* content)
{
    uint8 value = (uint8)((uint32)content);

    DevModem->stt.bit.signl = value;    //记录信号质量
    
    SYS_MODM_StartCnt(200);               //启动超时计数器
    guc_ModemElement++;				    //元素步骤加加
    
    return 0;
}





/************************************************************************
 * @function: AT_Proc_Creg
 * @描述: 对注册情况进行解析
 * 
 * @参数: 
 * @param: content 当前的注册情况
 * 
 * @返回: 
 * @return: uint8  0 解析成功进行下面的步骤
 *		           1 解析失败不进行下面的步骤
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 AT_Proc_Creg(void* content)
{
    Word32 wd;
    wd.lword = (uint32)content;			//获取内容
                                        //判断当前是否注册成功(1:已注册本地,5:已注册漫游)
    if((wd.byte[1] != 1) && (wd.byte[1] != 5))
    {
        DevModem->stt.bit.regtt = 0;
    }
    else					            //否则则表示注册成功
    {
        DevModem->stt.bit.regtt = 1;
    }
    
    SYS_MODM_StartCnt(200);               //启动超时计数器
    guc_ModemElement++;				    //元素步骤加加
    return 0;
}

/*************************************************************************
**查询信号质量和注册情况
**************************************************************************/
const ModemElement gs_ATQAndReg[] =
{
    {"AT+CSQ", AT_OK, 500, 0, (CallBackElement)0},                       //信号质量,GSM_Rec_07.07
    {"_csq ret", AT_CSQ, 200, 1, (CallBackElement)AT_Proc_CSQ},
    //{"AT+CPIN?", AT_OK, 0, 200, (CallBackElement)0},                     //sim解码情况
    //{"_cpin ret", AT_CREG, 0, 1, (CallBackElement)AT_Proc_Creg},
    {"AT+CREG?", AT_OK, 0, 500, (CallBackElement)0},                     //网络注册情况,GSM_Rec_07.07
    {"_creg ret", AT_CREG, 0, 1, (CallBackElement)AT_Proc_Creg},
    
    {(string)0, AT_OK, 0, 200, (CallBackElement)0},
};



uint8 uc_smsNumPdu[16]; //pdu格式短信号码(自动补齐到偶数长度,不含'+')
uint8 uc_smsNumLen;     //pdu格式短信号码长度(自动调整到偶数,不含'+')
uint8 uc_smsNumType;    //pdu格式短信号码类型(0国内,1国外)

/************************************************************************
 * @function: AT_Proc_SendSmsCmd
 * @描述: 发送短信命令
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8   0 解析成功进行下面的步骤
 *		            1 解析失败不进行下面的步骤
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 AT_Proc_SendSmsCmd(void* content)
{
                                        //需要发送的短信内容
    TModemSMS* sc = DevModem->smsSend;  //(TModemSMS*)content;
    uint8 templen;
    uint8 i, tempch;
                                        //获取目标号码及其长度
    if(sc->number[0] == '+')            //国际号码
    {
        uc_smsNumLen = sc->numlength - 1;
        if(uc_smsNumLen > 16)           //如果目标号码长度超过16位
        {
            return 1;
        }
        MoveBuffer(sc->number + 1, uc_smsNumPdu, uc_smsNumLen);
        uc_smsNumType = 0x01;
    }
    else                                //国内号码
    {
        uc_smsNumLen = sc->numlength;
        if(uc_smsNumLen > 16)           //如果目标号码长度超过16位
        {
            return 1;
        }
        MoveBuffer(sc->number, uc_smsNumPdu, uc_smsNumLen);
        uc_smsNumType = 0x00;
    }
    
    if(uc_smsNumLen & 0x01)			    //奇数个数字要补成偶数
    {
        uc_smsNumPdu[uc_smsNumLen] = 'F';//加上'F'
        uc_smsNumLen++;
    }
    
    templen = uc_smsNumLen;
    templen >>= 1;
    
    for(i = 0; i < templen; i++)
    {
        tempch = uc_smsNumPdu[2 * i];		    //进行前后字节的交换
        uc_smsNumPdu[2 * i] = uc_smsNumPdu[2 * i + 1];
        uc_smsNumPdu[2 * i + 1] = tempch;	
    }

    templen += 6 + 2 + sc->smslength;   //AT+CMGS=len的len的值
                                        //
    DevModem->stt.bit.state_wait = 1;   //开始等待>	
    
    PrintValueData(
        "AT+CMGS=%d\r",
        templen
        );

    SYS_MODM_StartCnt(600);              //启动超时计数器
    
    return 0;							//表示准备数据成功
}




/************************************************************************
 * @function: AT_Proc_SendSmsFlow
 * @描述: 发送短信数据
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8  0 解析成功进行下面的步骤
 *		           1 解析失败不进行下面的步骤
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 AT_Proc_SendSmsFlow(void* content)
{
    TModemSMS* sc = DevModem->smsSend;
    uint8 tempdata[16];
    uint8 templen;
    uint8 i;
                                        //发送PDU格式的号码长度等
    if(uc_smsNumType == 1)
    {
        PrintValueData(
            "001100%x291",              //"91",国际号码
            uc_smsNumLen
            );
    }
    else
    {
        PrintValueData(
            "001100%x281",              //"81",国内号码
            uc_smsNumLen
            );
    }
                                        //发送目标号码
    SER_SendData(
        SYS_GPRS_PORT,
        uc_smsNumPdu,
        uc_smsNumLen,
        1000);
                                        //发送短信格式
    PrintValueData(
        "00%x201",
        sc->fmt
        );
                                        //发送短信的长度
    PrintValueData(
        "%x2",
        sc->smslength
        );
                                        //发送短信内容
    for(i = 0; i < sc->smslength; i += 8)
    {
                                        //8个字节,8个字节的进行转换
        if(i + 8 < sc->smslength)
        {
            templen = 8;
        }
        else
        {
            templen = sc->smslength - i;
        }
    
        NumToHexString(                 //从字节到Hex字符串的转换
            tempdata,
            sc->sms + i,
            templen
            );
    
        SER_SendData(                   //输出一定的字节
            SYS_GPRS_PORT,
            tempdata,
            templen * 2,
            1000);
    }
    tempdata[0] = 0x1A;                 //结尾符
    SYS_MODM_SendCmd(tempdata, 1, 600);	//发送短信结束
    guc_ModemElement++;			//发送数据
    return 0;
}




/************************************************************************
**发送短信
*************************************************************************/
const ModemElement gs_ATSendSmg[] =
{
    {"_sms send start", AT_OK, 200, 1, AT_Proc_SendSmsCmd},       //发送短信,GSM_Rec_07.05
    {"_sms send data", AT_GT, 600, 1, AT_Proc_SendSmsFlow},
    {(string)0, AT_CMGS, 0, 0, (CallBackElement)0},
};



/************************************************************************
 * @function: AT_Proc_ReadMsg
 * @描述: 读取短信
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8  0 解析成功进行下面的步骤
 *		           1 解析失败不进行下面的步骤
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 AT_Proc_ReadMsg(void* content)
{
    //AT+CMGR=<index>
    //+CMGR:<stat>,[<alpha>],<length><CR><LF><pdu>
    
    uint16 smsidx;
    if(gs_FifoNewMessage.count > 0)	    //如果有新的短信读取
    {                                   //获取要读取的短信的索引
        SYS_FIFO_Get(&gs_FifoNewMessage, &smsidx);
    
        PrintValueData(
            "AT+CMGR=%d\r",
            smsidx
            );
            
        SYS_MODM_StartCnt(600);          //启动超时计数器
        gui_DelSmgIndex = smsidx;      //下一次要删除的短信的索引
    }
    else
    {
        return 1;
    }
    
    return 0;
}

/**************************************************************************
**接收短信
***************************************************************************/
const ModemElement gs_ATReadSmg[] =
{
    {"_read sms", AT_OK, 600, 1, (CallBackElement)AT_Proc_ReadMsg},
    {(string)0, AT_CMGR, 0, 0, (CallBackElement)0},
};


const ModemElement gs_ATListSmg[] =
{
    {"AT+CMGL=4",AT_OK, 300, 0, (CallBackElement)0},//AT+CMGL[=<stat>]
    {(string)0, AT_CMGL, 0, 0,  (CallBackElement)0},
};




/************************************************************************
 * @function: AT_Proc_DelMsg
 * @描述: 删除短信
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8  0 解析成功进行下面的步骤
		   1 解析失败不进行下面的步骤
 * @说明: 
 * @作者: yzy (2013/11/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 AT_Proc_DelMsg(void* content)
{
    PrintValueData(
        "AT+CMGD=%d\r",
        gui_DelSmgIndex
        );
    SYS_MODM_StartCnt(600);          //启动超时计数器
    return 0;
}

const ModemElement gs_ATDelSmg[] =
{
    {"_del sms", AT_OK, 600, 1, (CallBackElement)AT_Proc_DelMsg},
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};



/************************************************************************
 * @function: AT_Proc_ATD
 * @描述: CSD操作(未测试验证)
 * 
 * @参数: 
 * @param: content 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/11/17)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 AT_Proc_ATD(void* content)
{
    SYS_MODM_SendCmd("ATD13606503620", 14, 600);
    return 0;
}


const ModemElement gs_ATCSD[] = 		
{
    {"_set atd", AT_OK, 600, 1, (CallBackElement)AT_Proc_ATD},
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};


const ModemElement gs_ATcsca[] =
{
    {"AT+CSCA?", AT_OK, 200, 0, (CallBackElement)0},
    {(string)0, AT_OK, 0, 0, (CallBackElement)0},
};




const ModemElement gs_ATHoldUp[] =
{
	{"ATH", AT_OK, 500, 0, (CallBackElement)0},
	{(string)0, AT_OK, 0, 0, (CallBackElement)0},
};




/************************************************************************
 * 标准操作的步骤集合
 ************************************************************************/
const ModemElement* const gs_STDAT[] =
{
    gs_ATInit,                          //Modem_Init,
    gs_ATQAndReg,                       //Modem_QAndReg,
    gs_ATSendSmg,                       //Modem_SendMsg,
    gs_ATReadSmg,                       //Modem_ReadMsg,
    gs_ATListSmg,                       //Modem_ListMsg,
    gs_ATDelSmg,                        //Modem_DelMsg,
    gs_ATCSD,                           //Modem_CSDCall,
    (void*)0,                           //Modem_CSDBecall,
    gs_ATcsca,                          //Modem_CSCA,
    gs_ATHoldUp,                        //Modem_HoldUp,
};


const uint8 STDAT_ELEMENT_NUM = (sizeof(gs_STDAT) / sizeof(void*));



