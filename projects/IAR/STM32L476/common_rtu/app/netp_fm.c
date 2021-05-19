/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: Hexing_Dlms.c
**创   建   人: ZJC
**最后修改日期: 2014年11月3日
**描        述: 抄表框架
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: ZJC
** 版  本: v1.0
** 日　期: 2014年11月3日
** 描　述: 原始版本
********************************************************************************************************/
#define NETP_FM


#include "public.h"
#include "lib_com/calclib_gw.h"


#include "hlv.h"
#include "netp.h"
#include "paradef.h"
#include "inp.h"
#include "evt.h"
#include "netp_fm.h"

uint8  guc_mtUpdata_sendcnt;       //给表计升级，发送重启计时，防止升级过程中复位，或异常，可以继续当前步骤


/******************************************************************************
**组网端口实时数据(海兴DLMS)  OBIS列表
******************************************************************************/
//	uint8 const HX_DLMSComand[NETP_READ_HX_OBIS_MAX][9]={  //命令ID   	
//	 	
//	    {0,1, 0,0,97,129,0,255,2}, //事件状态寄存器 0    //实时
//	    {0,7, 1,0,99,98,0,255,2}, //标准事件 1    //实时
//	    {0,7, 1,0,99,98,1,255,2}, //欺诈事件 2    //实时
//	    {0,7, 1,0,99,98,2,255,2}, //继电器事件 3    //实时
//	    {0,7, 1,0,99,98,4,255,2}, //电网事件 4    //实时
//	    {0,7, 0,0,98,27,128,255,2}, //正向有功电能 5 //15min
//	    {0,7, 0,0,98,28,128,255,2}, //反向有功电能 6 //15min
//	    {0,7, 0,0,98,29,128,255,2}, //电压 7 //15min
//	    {0,7, 0,0,98,30,128,255,2}, //电流 8 //15min
//	    {0,7, 0,0,98,31,128,255,2}, //功率 9 //15min
//	    {0,7, 0,0,98,32,128,255,2}, //日冻结 10 //15min
//	    {0,7, 0,0,98,7,128,255,2},  //月冻结 11 //15min
//	    {0,1, 0,0,202,11,5,255,2},  //校时命令 12 //15min
//	    
//	    {0x00,0x12, 0x00,0x00,0x2C,0x00,0x00,0xFF,0x05},  //写使能
//	    {0x00,0x01, 0x00,0x00,0x2C,0x80,0x00,0xFF,0x02},  //写升级模式：
//	    {0x00,0x12, 0x00,0x00,0x2C,0x00,0x00,0xFF,0x01},  //升级初始化，信息头
//	    {0x00,0x12, 0x00,0x00,0x2C,0x00,0x00,0xFF,0x02},  //文件传输
//	    {0x00,0x12, 0x00,0x00,0x2C,0x00,0x00,0xFF,0x03},  //读位图
//	    {0x00,0x12, 0x00,0x00,0x2C,0x00,0x00,0xFF,0x03},  //验证
//	    {0x00,0x16, 0x00,0x00,0x0F,0x00,0x02,0xFF,0x04},  //设置升级时间
//	    {0x00,0x12, 0x00,0x00,0x2C,0x00,0x00,0xFF,0x04},  //立即执行
//	    {0x00,0x12, 0x00,0x00,0x2C,0x00,0x00,0xFF,0x06},  //读状态
//	    {0x00,0x01, 0x01,0x00,0x00,0x02,0x08,0xFF,0x02},  //读程序版本，防止重复升级
//	};	

/************************************************************************
 * @function: ByteToInt
 * @描述: 将两字节的数据转换成一个16位的数
 * 
 * @参数: 
 * @param: byte 存放要转换的两个字节的缓存
 * 
 * @返回: 
 * @return: uint16  两个字节转换成16位数之后的结果
 * @说明: 比如byte[0] = 0x03, byte[1] = 0x02那么转换后的结果为0x0302
 * @作者: yzy (2013/6/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint16 DLMS_ByteToInt(uint8* byte)
{
    Word16 wd;                              //定义一个双字节联合体
    wd.byte[0] = byte[1];                   //第一个字节
    wd.byte[1] = byte[0];                   //第二个字节
    return wd.word;
}

/************************************************************************
 * @function: ByteToLong
 * @描述: 四个字节转换成32位数
 * 
 * @参数: 
 * @param: byte 存放要转换的四个字节的缓存
 * 
 * @返回: 
 * @return: uint32  四个字节转换成32位数之后的结果
 * @说明: 比如byte[0] = 0x03, byte[1] = 0x02,
 *         byte[2] = 0x04, byte[3] = 0x05,
 *         那么转换后的结果为0x03020405
 * @作者: yzy (2013/6/6)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint32 DLMS_ByteToLong(uint8* byte)
{
    Word32 wd;
    wd.byte[0] = byte[3];                   //依次移动4个字节
    wd.byte[1] = byte[2];
    wd.byte[2] = byte[1];
    wd.byte[3] = byte[0];
    return wd.lword;
}

/************************************************************************
 * @function: DLMS_ByteToInt24
 * @描述: 三个字节转换成24位数
 * 
 * @参数: 
 * @param: byte 存放要转换的三个字节的缓存
* @param: isbcd 输入格式(假hex,真bcd)
 * 
 * @返回: 
 * @return: uint32  三个字节转换成24位数之后的结果
 * @说明: 比如byte[0] = 0x03, byte[1] = 0x02, byte[2] = 0x04，
 *        那么转换后的结果为0x030204
 * @作者: zjc (2014/10/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint32 DLMS_ByteToInt24(uint8* byte, bool isbcd)
{
    Word32 wd;
    InvBuffer(byte, 3);
    if(isbcd)
    {
        BcdToHex(byte, wd.byte, 3, 3);
    }
    else
    {
        MoveBuffer(byte, wd.byte, 3);
    }
    wd.byte[3] = 0;                         //最高字节无效
    return wd.lword;
}



/************************************************************************
 * @function: DLMS_ByteToData
 * @描述: 根据DLMS返回帧自动判断，数据类型，并添加数据
 * 
 * @参数: 
 * @param: byte 输入缓冲地址
 * @param: m    输入数据位置
 * @返回: 
 * @return: uint32  结果
 * @说明: 
 *        
 * @作者: zjc (2014/10/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint32  DLMS_ByteToData(uint8* byte, uint8 *m)
{   
    uint32 dtword = 0;
    uint8  caseid = byte[*m];
    *m += 1;
    switch(caseid)
    {
    case COMM_DATATYPE_INT32:
        dtword = DLMS_ByteToLong(byte + *m);
        *m += 4;
        break;
    case COMM_DATATYPE_INT32U:
        dtword = DLMS_ByteToLong(byte + *m);
        *m += 4;
        break;
    case COMM_DATATYPE_INT16:
        dtword = DLMS_ByteToInt(byte + *m);
        *m += 2;
        break;
    case COMM_DATATYPE_INT16U:
        dtword = DLMS_ByteToInt(byte + *m);
        *m += 2;
        break;
    case COMM_DATATYPE_INT8:
        dtword = byte[*m];
        *m += 1;
        break;
    case COMM_DATATYPE_INT8U:
        dtword = byte[*m];
        *m += 1;
        break;
    }

    return dtword;
    
}


/************************************************************************
 * @function: NETP_CheckFrame
 * @描述: 指定帧类型的有效帧判断
 * 
 * @参数: 
 * @param: NetpFrameStr 需要判断的帧的结构
 * @param: buffer 需要判断有效帧的缓存
 * @param: length 需要判断的帧的长度
 * 
 * @返回: 
 * @return: uint8  PST_ERR_OK
 *      PST_ERR_VAR
 *      PST_ERR_TS
 *      PST_ERR_NE
 * @说明: 
 * @作者: yzy (2013/7/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 NETP_CheckFrame(const NetpFrameStr* str, uint8* buffer, uint16 length, uint16_t *outLen)
{
//	    const PSTFrameStr* str;                 //定义指向帧信息结构体
    volatile uint16 len;                   //定义帧数据区长度
    uint16 ut_i;                             //循环变量
    uint16 sumlen;                          //待校验数据长度
    uint16 sumos;                           //校验和存放的位置
    uint16 cs;

//	    SYS_VAR_CHECK(num >= PST_FRM_NO);        //参数验证
    SYS_VAR_CHECK(length > PST_LEN_MAX);     //参数验证
                                            //获取当前要扫描的规约的结构体
//	    str = (PSTFrameStr*)gss_PSTFrameStr[num];

    if(str->extlen > length)                //判断是否已经达到最小帧长度
    {
        return PST_ERR_TS;                  //返回扫描长度太短
    }
                                            //判断帧头
    for(ut_i = 0; ut_i < str->hlen; ut_i++)
    {                                       //如果与匹配字符匹配那么继续下一个
        if(buffer[ut_i] == str->head[ut_i])
        {
            continue;
        }
                                            //如果当前的匹配字符为任意匹配字符则继续
        if(str->head[ut_i] == PST_FRM_ANY) 
        {
            continue;
        }
        return PST_ERR_NE;                  //返回不是有效帧
    }

    if(str->latr & PST_LATR_NOLEN)          //没有数据区长度
    {
        len = 0;                            //则定义数据区长度为0
        
        //在数据区无数据长度的情况下，先判断校验和，确认数据区长度
        if(!(str->atr & PST_ATR_END))           //有结尾符
        {
                                            //循环扫描结尾符
            for(ut_i = str->extlen - 1; ut_i < length; ut_i++)
            {
                if(buffer[ut_i] == str->end)
                {
                    len = ut_i + 1 - str->extlen;
                    break;
                }
            }
            if(len == 0)
            {
                return PST_ERR_NE;                  //返回不是有效帧
            }
        }
        else
        {
            len = length - str->extlen;
        }
    }
    else                                    //有数据区长度
    {
        if(str->latr & PST_LATR_DBLEN)      //如果是双字节长度
        {
            if(str->latr &  PST_LATR_BFLEN) //高字节在前
            {
                len = buffer[str->lp + 1]           //数据长度
                    + (buffer[str->lp ] << 8);
            }
            else                            //低字节在前
            {
                len = buffer[str->lp]           //数据长度
                    + (buffer[str->lp + 1] << 8);
            }
        }
        else                                //否则就是单字节的帧长
        {
            len = buffer[str->lp];          //数据长度
        }
        
        if(str->latr & PST_LATR_DBLEN)      //如果是双字节长度
        {
            len <<= str->llft;              //为防止代码被编译器优化，需要申明为volatile属性
            len &= 0xffff;
            len >>= str->llft;
            len >>= str->lrft;
        }
        else
        {
            len <<= str->llft;
            len &= 0xff;
            len >>= str->llft;
            len >>= str->lrft;
        }
    }

    if(len + str->extlen > length)          //如果数据区长度与最小帧长度之后大于扫描的帧长那么肯定不是一个有效帧
    {
        return PST_ERR_NE;                  //返回不是有效帧    
    }
                                            
    if(!(str->atr & PST_ATR_PARIT))         //有校验位
    {
                                            //计算待校验数据的长度
        sumlen = len + str->extlen;
        if(str->cssl & 0x80)                //负
        {
            if(sumlen > (str->cssl & 0x7F))
            {
                sumlen -= (str->cssl & 0x7F);
            }
            else
            {
                return PST_ERR_NE;
            }
        }
        else                                //正
        {
            sumlen += (str->cssl & 0x7F);
        }
                                            //计算校验和存放的位置偏移
        sumos = len + str->extlen;
        if(str->csco & 0x80)                //负
        {
            if(sumos > (str->csco & 0x7F))
            {
                sumos -= (str->csco & 0x7F);
            }
            else
            {
                return PST_ERR_NE;
            }
        }
        else                                //正
        {
            sumos += (str->csco & 0x7F);
        }
        
        switch(str->atr & PST_ATR_CSCAL)
        {
            case PST_ATR_CSCAL_SC:          //累加和
                cs = DoSum(
                    buffer + str->cssp, 
                    sumlen
                    );
                
                if((cs & 0xFF) != buffer[sumos])
                {
                    return PST_ERR_NE;      //返回不是有效帧   
                }
                break;
                
            case PST_ATR_CSCAL_SD:          //双字节累加和
                cs = DoSum(
                    buffer + str->cssp, 
                    sumlen
                    );
                
                if(cs != ByteToInt(buffer + sumos))
                {
                    return PST_ERR_NE;      //返回不是有效帧   
                }
                break;
            case PST_ATR_CSCAL_SD1:          //双字节累加和
                cs = DoSum(
                    buffer + str->cssp, 
                    sumlen
                    );
                cs &= 0xFF;
                if(cs != ByteToInt(buffer + sumos))
                {
                    return PST_ERR_NE;      //返回不是有效帧   
                }
                break;

            case PST_ATR_CSCAL_BS:          //位加和
                cs = DoBitSum(
                    buffer + str->cssp, 
                    sumlen
                    );

                if((uint8)cs != buffer[sumos])
                {
                    return PST_ERR_NE;      //返回不是有效帧      
                }
                break;
                
            case PST_ATR_CSCAL_CRC1:
                cs = DoCrcDlms(
                    buffer + str->cssp, 
                    sumlen,
                    0xffff
                    );
                
                if(cs != ByteToInt(buffer + sumos))
                {
                    return PST_ERR_NE;      //返回不是有效帧   
                }
                break;
            case PST_ATR_CSCAL_CRC2:    //EDMI, 在判断检验和之前，需要做转码处理
                for(uint16 ui_k = 1; ui_k < (length - 1); ui_k++)  //去掉转义符
                {
                    if((buffer[ui_k] == 0x10) && ((buffer[ui_k + 1] == 0x42) || (buffer[ui_k + 1] == 0x43) ||
                        (buffer[ui_k + 1] == 0x50) || (buffer[ui_k + 1] == 0x51) || (buffer[ui_k + 1] == 0x53)))
                    {	
                        buffer[ui_k] = buffer[ui_k + 1] - 0x40;
                        MoveBuffer(buffer + ui_k + 2, buffer + ui_k + 1, sumlen + 1 - ui_k);
                        length --;
                        sumlen--;
                        sumos--;
                    }
                }
                cs = DoCrc16(
                    buffer + str->cssp, 
                    sumlen, 0
                    );
                
                if(cs !=( ((uint16)buffer[sumos] << 8) + (uint16)buffer[sumos + 1]))
                {
                    return PST_ERR_NE;      //返回不是有效帧   
                }
                break;
            case PST_ATR_CSCAL_CRC3:
                cs = DoCrcRtuModbus(
                    buffer + str->cssp, 
                    sumlen,
                    0xffff
                    );
                
                if(cs != ByteToInt(buffer + sumos))
                {
                    return PST_ERR_NE;      //返回不是有效帧   
                }
                break;
                
            default:
                return PST_ERR_NE;          //返回不是有效帧         
        }
    }
    *outLen = len + str->extlen;
    if(!(str->atr & PST_ATR_END))           //有结尾符
    {
        if(str->latr & PST_LATR_NOLEN)      //没有数据区长度
        {
                                            //循环扫描结尾符
            for(ut_i = str->extlen - 1; ut_i < length; ut_i++)
            {
                if(buffer[ut_i] == str->end)
                {
                    SYS_OK();                //返回扫描到有效帧
                }
            }
            return PST_ERR_NE;              //返回不是有效帧  
        }
        else
        {                                   //判断是否为结尾符
            if(buffer[str->extlen - 1 + len] != str->end)
            {
                return PST_ERR_NE;          //返回不是有效帧      
            }
        }
    }
    SYS_OK();                                //返回扫描到有效帧
}


/************************************************************************
 * @function: NETP_CheckFrame2
 * @描述: 帧有效性检查,允许一定数量的前导符,并过滤前导符
 * 
 * @参数: 
 * @param: str 需要判断的帧结构
 * @param: buffer 收发缓存
 * @param: len 输入为接收数据长度,输出为有效数据长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/11/3)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/

uint8 NETP_CheckFrame2(const NetpFrameStr* str, uint8* buffer, uint16* len)
{
    uint16 tplen;
    uint16_t frame_len = 0;
    for(uint32 uc_i = 0; (uc_i < 7) && ((*len) - uc_i) > 0; uc_i++)
    {
        tplen = (*len) - uc_i;
        if(NETP_CheckFrame(str, buffer + uc_i, tplen, &frame_len) == PST_ERR_OK)
        {
            MoveBuffer(buffer + uc_i, buffer, frame_len);
            (*len) = frame_len;
            return 0;
        }
    }
    return 2;
}

/*********************************************************************************************************
**函数名称  int NETP_DoneFrame(const NetpComposeFrameStr * frmstr, const NetpFrameDataStr * frameDtaStr, uint8_t * buf, uint8* addr, uint16* length)
**输入
        frmstr 打包的结构体
        frameDtaStr 数据域打包配置
        buf 数据缓存
        addr 打包的地址            
        length 输入为打包缓存的最大长度，输出为打包好之后的长度
**输出  PST_ERR_OK
        PST_ERR_VAR
**      PST_ERR_FT
**说明  对抄表帧帧进行打包，主动通讯
** 全局变量:
** 调用模块:
**
**------------------------------------------------------------------------------------------------------
** 作  者: yzy
** 日　期: 2019年6月18日
** 内  容: 
********************************************************************************************************/
int NETP_DoneFrame(const NetpComposeFrameStr * frmstr, 
                            const NetpFrameDataStr * frameDtaStr, 
                            uint8_t * buf, uint8_t* addr, uint8_t *data, uint16_t* length)
{
    int m = 0;
    int frame_len = 0,len_area = 0,data_area=0;
    int i = 0;
    if(NULL == frmstr || NULL == frameDtaStr || 
        NULL == buf || NULL == addr || *(uint32_t *)frmstr == 0xFFFFFFFF)
        return SYS_ERR_FT;
//帧头
    if((frmstr->frm_bit & PST_FRM_BIT_HEAD) == PST_FRM_BIT_HEAD && frmstr->hlen <= 32)
    {
        memcpy(buf + m, frmstr->head, frmstr->hlen);
        m+=frmstr->hlen;
    }
    if((frmstr->len_bit & PST_FRM_BIT_HEAD) == PST_FRM_BIT_HEAD)
    {
        frame_len+=frmstr->hlen;
    }
    
//地址域    
    if((frmstr->frm_bit & PST_FRM_BIT_ADDR) == PST_FRM_BIT_ADDR && frmstr->alen <= 32)
    {
        memcpy(buf + (frmstr->adlp & 0x7f), addr, frmstr->alen);
        if((frmstr->adlp & 0x80) == 0x80)
        {
            m+=frmstr->alen;
            if((frmstr->len_bit & PST_FRM_BIT_ADDR) == PST_FRM_BIT_ADDR)
            {
                frame_len+=frmstr->hlen;
            }
            
        }
    }
    
//功能码
    if((frmstr->frm_bit & PST_FRM_BIT_CMD) == PST_FRM_BIT_CMD && frmstr->cmdlen <= 4)
    {
        memcpy(buf + (frmstr->cmdlp & 0x7f), &frameDtaStr->cmd, frmstr->cmdlen);
        m+=frmstr->cmdlen;
    }
    if((frmstr->len_bit & PST_FRM_BIT_CMD) == PST_FRM_BIT_CMD)
    {
        frame_len+=frmstr->cmdlen;
    }

//DI域
    if((frmstr->frm_bit & PST_FRM_BIT_DI) == PST_FRM_BIT_DI && frameDtaStr->dilen <= 4)
    {
        memcpy(buf + (frmstr->DIatr & 0x7f), frameDtaStr->scmd, frameDtaStr->dilen);
        m+=frameDtaStr->dilen;
    }
    if((frmstr->len_bit & PST_FRM_BIT_DI) == PST_FRM_BIT_DI)
    {
        frame_len+=frameDtaStr->dilen;
    }
//长度域
    
    if(frmstr->latr & PST_LATR_NOLEN)          //没有数据区长度域
    {

    }
    else
    {
        if(frmstr->latr & PST_LATR_DBLEN)      //如果是双字节长度
        {
            len_area=2;
        }
        else
        {
            len_area=1;
        }
        if(frmstr->latr & PST_LATR_RELEN)
        {
            m+=2*len_area;
        }
        else
        {
            m+=len_area;
        }
    }
    
//数据域
    if((frmstr->frm_bit & PST_FRM_BIT_DATA) == PST_FRM_BIT_DATA)
    {
        {
            for(i=0;i<frameDtaStr->inum;i++)
            {
                
                if(SYS_ERR_OK != ADATA_FunctionW(frameDtaStr->item[i].id, 
                    (frameDtaStr->item[i].mult << 8)| frameDtaStr->item[i].len & 0xFF, 
                    buf+m, data))
                {
                    SYS_ERR();
                }
                else
                {
                    m+=frameDtaStr->item[i].len;
                    data_area+=frameDtaStr->item[i].len;
                }
            }
        }
    
//	        memcpy(frm->send + (frmstr->cmdlp & 0x7f), frameDtaStr->cmd, frmstr->cmdlen);
//	        m+=frmstr->cmdlen;
    }
    if((frmstr->len_bit & PST_FRM_BIT_DATA) == PST_FRM_BIT_DATA)
    {
        frame_len+=data_area;
    }
//校验域    
    if((frmstr->len_bit & PST_FRM_BIT_CS) == PST_FRM_BIT_CS)
    {
        frame_len+=frmstr->cssl & 0x7f;
    }
    if((frmstr->len_bit & PST_FRM_BIT_TAIL) == PST_FRM_BIT_TAIL)           //有结尾符
    {
        frame_len+=1;                              //长度增加
    }

    if(frmstr->latr & PST_LATR_NOLEN)          //没有数据区长度域
    {
//	       return 0;                           //没有长度区的话后面没法打包
    }
    else                                    //有数据区长度
    {

        if(frmstr->latr & PST_LATR_DBLEN)      //如果是双字节长度
        {
            uint16_t tmp_len = (uint16_t)frame_len;
            
            tmp_len <<= frmstr->llft;
            tmp_len &= 0xffff;
            tmp_len >>= frmstr->llft;
            tmp_len <<= frmstr->lrft;
            frame_len = tmp_len;
        }
        else
        {
            uint8_t tmp_len = (uint8_t)frame_len;
        
            tmp_len <<= frmstr->llft;
            tmp_len &= 0xff;
            tmp_len >>= frmstr->llft;
            tmp_len <<= frmstr->lrft;
            frame_len = tmp_len;
        }
        
        //左右移过后，其他字段暂时没定义，碰到再增加
       if(frmstr->latr & PST_LATR_DBLEN)      //如果是双字节长度
       {
           if(frmstr->latr &  PST_LATR_BFLEN) //高字节在前
           {

               buf[frmstr->lp] = frame_len >> 8;
               buf[frmstr->lp + 1] = (uint8_t)frame_len;
               
           }
           else                            //低字节在前
           {

               buf[frmstr->lp + 1] = frame_len >> 8;
               buf[frmstr->lp] = (uint8_t)frame_len;
               
           }
       }
       else                                //否则就是单字节的帧长
       {
           buf[frmstr->lp] = (uint8_t)frame_len;
       }
       
    }

    if(m+3 > PST_LEN_MAX)     //如果数据区长度与最小帧长度之后大于扫描的帧长那么肯定不是一个有效帧
    {
       return 0;                           //返回不是有效帧    
    }
    int sumlen = 0;
    if(!(frmstr->atr & PST_ATR_PARIT))         //有校验位
    {
                                           //计算待校验数据的长度

        if(m>=frmstr->cssp)
            sumlen = m-frmstr->cssp;
        uint16_t cs = 0;
        switch(frmstr->atr & PST_ATR_CSCAL)
        {
            case PST_ATR_CSCAL_SC:            
                cs = DoSum(                 //进行累加和运算
                    buf + frmstr->cssp, 
                    sumlen
                    );
                //累加和赋值
                buf[m++] = (uint8)cs;
                break;

            case PST_ATR_CSCAL_SD:            
                cs = DoSum(                 //进行累加和运算
                    buf + frmstr->cssp,  
                    sumlen
                    );
                //累加和赋值
                IntToByte(buf+m, cs);
                m+=2;                                           
                break;
            case PST_ATR_CSCAL_SD1:            
                cs = DoSum(                 //进行累加和运算
                    buf + frmstr->cssp,  
                    sumlen
                    );
                //累加和赋值
                cs &= 0xff;
                IntToByte(buf+m, cs);
                m+=2;                                           
                break;
            case PST_ATR_CSCAL_BS:
                cs = DoBitSum(              //进行位加和运算
                    buf + frmstr->cssp,  
                    sumlen
                    );
                //累加和赋值
                buf[m++] = (uint8)cs;
                break;

            case PST_ATR_CSCAL_CRC1:
                cs = DoCrcDlms(
                    buf + frmstr->cssp,
                    sumlen,
                    0xffff
                    );
                IntToByte(buf+m, cs);
                m+=2;
                break;
            case PST_ATR_CSCAL_CRC3:
                cs = DoCrcRtuModbus(
                    buf + frmstr->cssp,
                    sumlen,
                    0xffff
                    );
                IntToByte(buf+m, cs);
                m+=2;
                break;

            default:
                return PST_ERR_FT;                   //返回打包失败
        }
//	        len += str->extlen - 1;             //长度增加
    }

   if(!(frmstr->atr & PST_ATR_END))           //有结尾符
   {
       buf[m++] = frmstr->end;             //判断是否为结尾符            
   }

   *length = m;


   return PST_ERR_OK;                   //返回发生致命错误
}  


/************************************************************************
 * @function: _RefreshTrnFlag
 * @描述: 刷新测量点存在标志位
 * 
 * @参数: 
 * @param: tstno 测量点号
 * @param: isok 真:存在,假:丢失
 * @返回: 
 * @说明: 
 * @作者: yzy (2011-9-30)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void _RefreshTrnFlag(uint8 tstno, bool isok)
{
    if(isok)
    {
        gs_SysVar.terstt.lword |= (0x1 << (16 + tstno));
        
                                            //不通-->通
        if(gss_ALRTStt[tstno].ew1.trn & 0x01)
        {
            gss_ALRTStt[tstno].ewf1.trn = 0x01;
        }
        gss_ALRTStt[tstno].ew1.trn = 0x00;
    }
    else
    {
         gs_SysVar.terstt.lword &= ~(0x1 << (16 + tstno));
   
                                            //通-->不通
        if(!(gss_ALRTStt[tstno].ew1.trn & 0x01))
        {
            gss_ALRTStt[tstno].ewr1.trn = 0x01;
        }
        gss_ALRTStt[tstno].ew1.trn = 0x01;
	
    }
}

/************************************************************************
 * @function: dataminok
 * @描述: 判断时间的合法性
 * 
 * @参数: 
 * @param: date 年月日时分
 * 
 * @返回: 
 * @return: uint8  1：合法；0：非法
 *                  
 * @说明: 
 * @作者: zjc (2014/8/21)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 dataminok(uint8* date)
{
    const uint8* monthday;
    if(date[0] < 7 || date[0] > 99)     //年的合法性
    {
        return 0;
    }
    
    if((date[0] & 0x03) == 0)           //判断今年是否为闰年
    {
        monthday = guc_BYearMonth;
    }
    else
    {
        monthday = guc_LYearMonth;  
    }
    
    if(date[1] == 0 || date[1] > 12)    //月的合法性
    {
        return 0;
    }
                                        //日的合法性
    if(date[2] == 0 || date[2] > monthday[date[1] - 1])
    {
        return 0;
    }
    if(date[3] > 59 || date[4] > 59 )  //时分的合法性
    {
        return 0;
    }
    return 1;
}

/************************************************************************
 * @function: DLMS_UpEvtERC40414243
 * @描述: 上送表计的事件ID：ERC40414243
 * 
 * @参数: 
 * @param:
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void DLMS_UpEvtERC40414243(uint8 uc_i, uint8 di, uint8 *buffer)
//	{
//	    uint8   inbuf[16];
//	    uint8   inbuflen;
//	    uint16 usyear;
//	    
//	    inbuflen = 7;
//	    inbuf[0]= buffer[13];  //事件代码
//	    usyear=((uint16)buffer[0]<<8)+buffer[1];
//	    inbuf[5] = ByteHexToBcd(usyear-2000);       //年
//	    inbuf[4] = ByteHexToBcd(buffer[2]);    //月
//	    inbuf[3] = ByteHexToBcd(buffer[3]);    //日
//	    inbuf[2] = ByteHexToBcd(buffer[5]);    //时
//	    inbuf[1] = ByteHexToBcd(buffer[6]);   //分
//	    inbuf[6] = ByteHexToBcd(buffer[7]);   //秒
//	    
//	    ALRT_DLMS_ERC40_41_42_43(uc_i, di, inbuf, inbuflen);
//	}


/************************************************************************
 * @function: DLMS_PackRead
 * @描述: 打包数据读取帧
 * 
 * @参数: 
 * @param: buffer 
 * @param: di 
 * @param: tstno 
 * @param: cmdtype 
 * @返回: 
 * @return: uint8  
 * @说明: 就是在DLMS_Pack()基础上简单打包下
 * @作者: ZJC (2014-8-12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint16 DLMS_PackRead(ST_ACT_INPBUF_L *inpLbuf, ST_ACT_INPBUF_S *inpSbuf, ST_RDMT_CMD *stpActCmd)
//	{
//	    uint8 m = 0;
//	    uint32 DLMSAddrss = 0,addr;
//	    uint16 uiCrc;
//	    uint8  parabuf[LEN_TST_CFG];
//	    
//	    inpLbuf->buf[m++] = 0x7E;  //帧头
//	    inpLbuf->buf[m++] = 0xA0;
//	    m++;
//	    inpLbuf->buf[m++] = 0x00;  //固定地址
//	    inpLbuf->buf[m++] = 0x02;
//	    
//	    //检索出搜表档案的逻辑地址
//	    
//	    addr = F10_PADDR + LEN_TST_BMPS + LEN_TST_CFG * stpActCmd->tstno;
//	    GD_Para_RW(addr, parabuf, LEN_TST_CFG, false);   //读档案地址
//	    for(uint8 uc_i = 0; uc_i < CORE_TST_NO; uc_i++)
//	    {
//	        if(STR_IsBitSet(gs_dlmsMt.eft, uc_i) &&                                     //档案有效
//	           (CmpBuffer(gs_dlmsMt.mt[uc_i].bgaddr, parabuf + 6, 6) == 0) )   //地址相同
//	        {
//	            DLMSAddrss = uc_i + DLMS_SADDR_OFFSET;//取出档案中的地址
//	            break;
//	        }
//	    }
//	    if(DLMSAddrss < DLMS_SADDR_OFFSET)  //未找到有效的逻辑地址
//	    {
//	        return 0;
//	    }
//	
//	    DLMSAddrss <<= 1;
//	    uiCrc = DLMSAddrss & 0x00FF;
//	    DLMSAddrss <<= 1;
//	    DLMSAddrss = DLMSAddrss&0xFE00;
//	    DLMSAddrss = DLMSAddrss|uiCrc|0x01;
//	    inpLbuf->buf[m++] = DLMSAddrss>>8;
//	    inpLbuf->buf[m++] = DLMSAddrss;  //HDLC地址
//	    inpLbuf->buf[m++] = 0x03;//固定地址
//	    
//	    
//	    if(stpActCmd->cmdtype == NETP_DLMS_TYPE_LINK)   //链路层握手
//	    {
//	        inpLbuf->buf[m++] = 0x93;
//	        m += 2;//CRC
//	        MoveBuffer(inpSbuf->buf, &inpLbuf->buf[m] ,inpSbuf->len);
//	        m += inpSbuf->len;  
//	        ucDlmsSendCnt = 0;
//	        ucDlmsReceCnt = 0;
//	    }
//	    else if(stpActCmd->cmdtype == NETP_DLMS_TYPE_APP)   //应用层握手
//	    {
//	        inpLbuf->buf[m++] = 0x10;
//	        m += 2;//CRC
//	        MoveBuffer(inpSbuf->buf, &inpLbuf->buf[m] ,inpSbuf->len);
//	        m += inpSbuf->len; 
//	        ucDlmsSendCnt = 1;
//	        ucDlmsReceCnt = 0;
//	    }
//	    else
//	    {   
//	        
//	        inpLbuf->buf[m++] = ((((ucDlmsReceCnt)<<5)&0xF0)|0x10)+(((ucDlmsSendCnt)<<1)&0x0E);  //控制字
//	        
//	        if(stpActCmd->cmdtype == NETP_DLMS_TYPE_LINK_AF)  
//	        {
//	            inpLbuf->buf[8] = ((inpLbuf->buf[8] & 0xF0) | 0x01);
//	        }
//	        else
//	        {
//	            m += 2;//CRC
//	            inpLbuf->buf[m++] = 0xE6;
//	            inpLbuf->buf[m++] = 0xE6;
//	            inpLbuf->buf[m++] = 0x00;
//	            ucDlmsSendCnt ++;
//	        }
//	        
//	        if(stpActCmd->cmdtype == NETP_DLMS_TYPE_GET)  //读取
//	        {
//	            inpLbuf->buf[m++] = 0xC0;
//	            inpLbuf->buf[m++] = 0x01;
//	            inpLbuf->buf[m++] = 0xC1;
//	        }
//	        else if(stpActCmd->cmdtype == NETP_DLMS_TYPE_GET_AF) //后续帧的读取方式
//	        {
//	            inpLbuf->buf[m++] = 0xC0;
//	            inpLbuf->buf[m++] = 0x02;
//	            inpLbuf->buf[m++] = 0xC1;
//	        }
//	        else  if(stpActCmd->cmdtype == NETP_DLMS_TYPE_SET)  //设置报文
//	        {
//	            inpLbuf->buf[m++] = 0xC1;
//	            inpLbuf->buf[m++] = 0x01;
//	            inpLbuf->buf[m++] = 0xC1;
//	        }
//	        else  if(stpActCmd->cmdtype == NETP_DLMS_TYPE_ACTION)  //执行报文
//	        {
//	            inpLbuf->buf[m++] = 0xC3;
//	            inpLbuf->buf[m++] = 0x01;
//	            inpLbuf->buf[m++] = 0xC1;
//	        }
//	        if(stpActCmd->cmdtype != NETP_DLMS_TYPE_LINK_AF)  
//	        {
//	           MoveBuffer(inpSbuf->buf, &inpLbuf->buf[m] ,inpSbuf->len);
//	            m += inpSbuf->len; 
//	        }
//	    }
//	    m += 2;//CRC
//	    inpLbuf->buf[m++] = 0x7E;//帧尾
//	    inpLbuf->buf[2] = m - 2;//帧长度
//	    uiCrc = DoCrcDlms(&inpLbuf->buf[1], 8, 0xffff);  //计算校验和
//	    inpLbuf->buf[9] = (uint8)uiCrc;
//	    inpLbuf->buf[10] = (uint8)(uiCrc>>8);
//	    uiCrc = DoCrcDlms(&inpLbuf->buf[1], m-4, 0xffff);  //计算校验和
//	    inpLbuf->buf[m-3] = (uint8)uiCrc;
//	    inpLbuf->buf[m-2] = (uint8)(uiCrc>>8);	
//	                     
//	    return m;
//	}

/************************************************************************
 * @function: DLMS_ExtractData
 * @描述: 提取有效数据内容
 * 
 * @参数: 
 * @param: buffer 接收数据缓存,传出有效数据内容
 * @param: length 传入为接收缓存长度,传出为有效数据内容长度
 * @param: di 标识(空表示不判标识)
 * 
 * @返回: 
 * @return: uint8 NETP_ERR_OK,NETP_ERR_NT(有回复,但不支持),NETP_ERR_FT
 * @说明: 
 * @作者: yzy (2011-9-7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	uint8 DLMS_ExtractData(uint8* buffer, uint16* length)
//	{
//	    PST_Frame frame;
//	    DLT_Frame* frm = &frame._dlt;
//	
//	    if(__PST_CheckFrame(PST_FRM_DLMS_HDLC_NO, buffer, length) == PST_ERR_OK)
//	    {                                                   //解析收到的数据
//	        DLMS_Decode(buffer, &frame, __NULL);
//	        if(frm->len > 0)                     //转移读取到的数据
//	        {
//	            MoveBuffer(frm->recv, buffer, frm->len);
//	            (*length) = frm->len;
//	        }
//	
//	        SYS_OK();
//	    }
//	
//	    (*length) = 0;
//	    return NETP_ERR_FT;
//	}
uint8 NETP_Decode(uint8* receive, uint16_t len, PST_Frame* frame, NetpFrameStr * pframeStr, 
                        NetpFrameDataStr * pDataStr, uint8_t* addr, uint8_t addrlen)
{
    NETP_Frame* frm = &frame->_netp;
    S_TstCurveData stdata;
    uint8_t * data = NULL;
    uint16_t cmd = 0;
    if(addr != __NULL)                  //需要判断地址
    {
        //判断接收到的地址是否和要求的一致
        if(CmpBuffer(addr, receive + (pframeStr->adlp & 0x7F), addrlen) != 0)
        {
            return PST_ERR_FT;          //返回发生错误
        }
    }
    memcpy(&cmd, receive+pframeStr->cmdlp, pframeStr->cmdlen);
    if(cmd != pDataStr->cmd)
    {
        //return PST_ERR_FT;
    }
    if(pframeStr->DIatr != PST_DIATR_NONE)
    {
        //二级数据项校验
    }
    memset((uint8_t *)&stdata, 0, sizeof(S_TstCurveData));
    data = receive + pframeStr->dlp;

    
    frm->recv = receive + pframeStr->dlp;           //接收到的数据的数据区
    frm->len = len - pframeStr->dlp - (pframeStr->cssl&0x7f);              //长度
    if(frm->len > 256)
    {
        frm->len = 256;
    }
    SYS_OK();
}


/************************************************************************
 * @function: DLMS_ExtractData
 * @描述: 提取有效数据内容
 * 
 * @参数: 
 * @param: buffer 接收数据缓存,传出有效数据内容
 * @param: length 传入为接收缓存长度,传出为有效数据内容长度
 * @param: di 标识(空表示不判标识)
 * 
 * @返回: 
 * @return: uint8 NETP_ERR_OK,NETP_ERR_NT(有回复,但不支持),NETP_ERR_FT
 * @说明: 
 * @作者: yzy (2011-9-7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 NETP_ExtractData(uint8* buffer, uint16* length, const NetpFrameStr * pframeStr, 
                            const NetpFrameDataStr * pDataStr, uint8_t *addr, uint8_t addrlen)
{
    PST_Frame frame;
    NETP_Frame* frm = &frame._netp;
//	    NetpFrameStr * pframeStr = &fw->frameStr;
//	    NetpFrameDataStr * pDataStr = &fw->frameDtaStr[index];

    if(NETP_CheckFrame2(pframeStr, buffer, length) == PST_ERR_OK)
    {                                                   //解析收到的数据
        if(*length != pDataStr->rlen)
        {
            goto EX_ERR;
        }
        if(PST_ERR_OK == NETP_Decode(buffer, *length, &frame, pframeStr, pDataStr, addr, addrlen))
        {
            if(frm->len > 0)                     //转移读取到的数据
            {
                MoveBuffer(frm->recv, buffer, frm->len);
                (*length) = frm->len;
            }
            else
            {
                goto EX_ERR;
            }
        }
        else
        {
            goto EX_ERR;
        }
        SYS_OK();
    }
EX_ERR:
    (*length) = 0;
    return NETP_ERR_FT;
}


/************************************************************************
 * @function: Netp_ReadDLMS
 * @描述: 组网端口DLMS数据读取处理
 * 
 * @参数: 
 * @param: buffer 接收数据缓存
 * @param: di 数据标识
 * @param: length 接收数据的预计长度
 * @param: tstno 测量点号
 * @param: cmdtype 命令类型
 * @返回: 
 * @return: uint8  NETP_ERR_OK,NETP_ERR_NT(有回复,但不支持),NETP_ERR_FT
 * @说明: 
 * @作者: ZJC (2014/8/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Netp_CommonRead(ST_ACT_INPBUF_L *inpLbuf, uint8_t * data, 
                                ST_RDMT_CMD *stpActCmd)
{
    uint8 err;
    
    uart_config_t ss;                          //超时时间:单位100ms
    
    PentCfgToSS(stpActCmd->baud_cfg, &ss);
    ST_Netp_FW * fw = stpActCmd->fw;
                                            //打包
//	    inpLbuf->len = DLMS_PackRead(inpLbuf, inpSbuf, stpActCmd);

    err = NETP_DoneFrame(&fw->compFrameStr, &fw->frameDtaStr[stpActCmd->di], inpLbuf->buf, 
                    &stpActCmd->addr, data, &inpLbuf->len);
    if(err != 0)
    {
        return NETP_ERR_FT;
    }
                                            //数据传输通讯
    if(Netp_Send(stpActCmd->port + NETP_PORT_NO, inpLbuf->buf, &inpLbuf->len, 2, &ss, &fw->frameStr) != NETP_ERR_OK)
    {
        return NETP_ERR_FT;
    }
                                            //提取数据内容
    //len = sizeof(buffer);
    err = NETP_ExtractData(inpLbuf->buf, &inpLbuf->len, &fw->frameStr,&fw->frameDtaStr[stpActCmd->di],
                            &stpActCmd->addr,stpActCmd->addr_len);
    if(err != NETP_ERR_OK)
    {
        return err;
    }                                                 //转移读取到的数据
    //MoveBuffer(tmp, buffer, len);
    //inpLbuf->len = len;
    
    ucDlmsReceCnt ++;
    SYS_OK();
}

/************************************************************************
 * @function: Netp_DLMSRead
 * @描述: 组网端口DLMS数据读取处理
 * 
 * @参数: 
 * @param: buffer 接收数据缓存
 * @param: di 数据标识
 * @param: length 接收数据的预计长度
 * @param: tstno 测量点号
 * @param: cmdtype 命令类型
 * @返回: 
 * @return: uint8  NETP_ERR_OK,NETP_ERR_NT(有回复,但不支持),NETP_ERR_FT
 * @说明: 
 * @作者: ZJC (2014/8/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/

//	uint8 Netp_DLMSRead(ST_ACT_INPBUF_L *inpLbuf, ST_ACT_INPBUF_S *inpSbuf, ST_RDMT_CMD *stpActCmd)
//	{   uint8 err;
//	    scm_dlms_t* scm = &gs_scmDlms;
//	    if(((scm->stt & SCM_STT_EXMASK) != SCM_STT_FIN))
//	    {
//	        ES_LED_BlinkSet(LED_MBUS, 1, 0, 0);
//	        return NETP_ERR_NT;
//	    }
//	    
//	    for(uint8 uc_i = 0; uc_i < 2; uc_i++)
//	    {
//	        err = Netp_ReadDLMS(inpLbuf, inpSbuf, stpActCmd);
//	        if((err == NETP_ERR_OK) || (err == NETP_ERR_NT))
//	        {
//	            break;
//	        }
//	    }
//	    return err;
//	}


/************************************************************************
 * @function: Act_BuildFrame
 * @描述: HXING表计，请求数据组帧
 * 
 * @参数: 
 * @param:
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/11/7)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint16_t Netp_Send_With_Retry(ST_ACT_INPBUF_L *inpLbuf, ST_RDMT_CMD *stpActCmd)
{
    uint8_t m=0;
    uint8_t err;
    
    uint16 usyear;
    ST_ACT_INPBUF_S   inBuf;

    for(uint8_t uc_i = 0; uc_i < 2; uc_i++)
    {
        err = Netp_CommonRead(inpLbuf, NULL, stpActCmd);
        if((err == NETP_ERR_OK) || (err == NETP_ERR_NT))
        {
            break;
        }
    }
    return err;
}


/************************************************************************
 * @function: Netp_mtGetEvt
 * @描述: 获取事件
 * 
 * @参数: 
 * @param:
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Netp_mtGetEvt(ST_ACT_INPBUF_L *inpLbuf, ST_ACT_INPBUF_S *inpSbuf, ST_RDMT_CMD *stpDlmsCmd)
//	{
//	    stpDlmsCmd->cmdtype = NETP_DLMS_TYPE_GET;
//	    HX_SendDlms(inpLbuf, inpSbuf, stpDlmsCmd);   //读事件
//	    
//	    if(inpLbuf->buf[1] == 0x01) //单帧
//	    {
//	        if(inpLbuf->len>10)
//	        {
//	            if(inpLbuf->buf[4] == 0x01 && inpLbuf->buf[5] == 0x01 )
//	            {
//	                DLMS_UpEvtERC40414243(stpDlmsCmd->tstno, stpDlmsCmd->di, &inpLbuf->buf[10]);
//	            }
//	        }
//	    }
//	    else //多帧
//	    {
//	        while(1)
//	        {
//	            if(inpLbuf->buf[1] != 0x02 || (inpLbuf->buf[3] != 0xff && inpLbuf->buf[3] != 0x00))
//	            {
//	                break;//不合法帧
//	            }
//	            if((inpLbuf->buf[4] == 0) && (inpLbuf->buf[5] == 0)&&
//	               (inpLbuf->buf[6] == 0) && (inpLbuf->buf[7] == 1))//多帧的第一帧
//	            {
//	                DLMS_UpEvtERC40414243(stpDlmsCmd->tstno, stpDlmsCmd->di, &inpLbuf->buf[16]);
//	            }
//	            else
//	            {
//	                DLMS_UpEvtERC40414243(stpDlmsCmd->tstno, stpDlmsCmd->di, &inpLbuf->buf[14]);
//	            }
//	            
//	            if(inpLbuf->buf[3] == 0xff)  //最后一帧
//	            {
//	                break;
//	            }
//	            else
//	            {
//	                MoveBuffer(&inpLbuf->buf[4], inpSbuf->buf, 4);
//	                
//	                stpDlmsCmd->cmdtype = NETP_DLMS_TYPE_GET_AF;
//	                HX_SendDlms(inpLbuf, inpSbuf, stpDlmsCmd);   //读事件后续帧
//	            }
//	        }
//	    }
//	}
/************************************************************************
 * @function: Netp_ReadEvent
 * @描述: 读取DLMS事件
 * 
 * @参数: 
 * @param: index 抄表端口索引
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Netp_ReadEvent(uint8 index)
//	{
//	    ST_ACT_INPBUF_L    inLbuf;
//	    ST_ACT_INPBUF_S    inSbuf;
//	    ST_RDMT_CMD stDlmscmd;
//	    
//	    uint8 mtevtsta[4];
//	    uint8 YMDhms[6];
//	    uint8 YMDhms_1[6];
//	    uint8 flag=0;
//	    
//	    if(gs_InpExtracb[0].gbbreak)
//	    {
//	        return;
//	    }
//	                                     
//	    for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++) //外部测量点循环读取处理
//	    {                
//	        if(gss_CDATATst[uc_i].ef != 0x01)  //要求:1.测量点有效,2.485表
//	        {
//	            continue;
//	        }
//	        HB_RetLive();                  //抄表期间主动保活
//	        
//	        stDlmscmd.tstno = uc_i;
//	        stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	        stDlmscmd.di = NETP_READ_DLMS_ES;  
//	        
//	        if(HX_SendDlms(&inLbuf, &inSbuf, &stDlmscmd) == NETP_ERR_OK)
//	        {
//	            MoveBuffer(&inLbuf.buf[5], mtevtsta, 4);
//	            if((mtevtsta[3] & 0x0F) != 0x00)   //有事件产生
//	            {
//	                ES_FRAM_Open();
//	                Fram_SafeRead((uint8 *)&gs_NetpReadStEVT, FM_EVTDT,sizeof(gs_NetpReadStEVT));   //从铁电读取上次事件发生时间
//	                ES_FRAM_Close();
//	
//	                UpdateSysTimeStruct();  //更新系统时标
//	                
//	                if(CalcMins(gs_NetpReadStEVT[uc_i].time, 0) > CalcMins(gs_SysTime.YMDhms, 0) ||  //启动时间大于当前时间
//	                   dataminok(gs_NetpReadStEVT[uc_i].time) == 0 )  //时间非法     
//	                {
//	                    MoveBuffer(gs_SysTime.YMDhms, gs_NetpReadStEVT[uc_i].time, 5);
//	                    StepDays(gs_NetpReadStEVT[uc_i].time,-1);   //时间提前一天
//	                }
//	                MoveBuffer(gs_NetpReadStEVT[uc_i].time, YMDhms, 5); //起始时间
//	                YMDhms[5]  = 0;
//	                MoveBuffer(gs_SysTime.YMDhms,YMDhms_1,5);  //当前时间为截止时间
//	                YMDhms_1[5] = 59;
//	                MoveBuffer(YMDhms_1, gs_NetpReadStEVT[uc_i].time, 5); //取当前时间为本次数事件发生时间
//	                flag = 1;   //事件发生时间有变动
//	                
//	                MoveBuffer(YMDhms, inSbuf.buf, 6);
//	                MoveBuffer(YMDhms_1, inSbuf.buf + 6, 6);
//	                if((mtevtsta[3]&0x01)==0x01)  //发现标准事件
//	                {
//	                    stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	                    stDlmscmd.di = NETP_READ_DLMS_E1;
//	                    Netp_mtGetEvt(&inLbuf, &inSbuf, &stDlmscmd);//读标准事件
//	                    mtevtsta[3] &= ~0x01;
//	                }
//	                if((mtevtsta[3]&0x02)==0x02)  //发现欺诈事件
//	                {
//	                    stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	                    stDlmscmd.di = NETP_READ_DLMS_E2;
//	                    Netp_mtGetEvt(&inLbuf, &inSbuf, &stDlmscmd);//读欺诈事件
//	                    mtevtsta[3] &= ~0x02;
//	                }
//	                if((mtevtsta[3]&0x04)==0x04)  //发现继电器事件
//	                {
//	                    stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	                    stDlmscmd.di = NETP_READ_DLMS_E3;
//	                    Netp_mtGetEvt(&inLbuf, &inSbuf, &stDlmscmd);//读继电器事件
//	                    mtevtsta[3] &= ~0x04;
//	                }
//	                if((mtevtsta[3]&0x08)==0x08)  //发现电网事件
//	                {
//	                    stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	                    stDlmscmd.di = NETP_READ_DLMS_E4;
//	                    Netp_mtGetEvt(&inLbuf, &inSbuf, &stDlmscmd);//读电网事件
//	                    mtevtsta[3] &= ~0x08;
//	                }
//	                inSbuf.buf[0] = 0x06;
//	                MoveBuffer(mtevtsta, &inSbuf.buf[1], 4);
//	                
//	                stDlmscmd.cmdtype = NETP_DLMS_TYPE_SET;
//	                stDlmscmd.di = 0;  
//	                HX_SendDlms(&inLbuf, &inSbuf, &stDlmscmd);
//	            }
//	        }
//	    }
//	    if(flag == 1)   //
//	    {   //保证一分钟最多写一次
//	        ES_FRAM_Open();
//	        Fram_SafeWrite((uint8 *)&gs_NetpReadStEVT, FM_EVTDT, sizeof(gs_NetpReadStEVT));   //往铁电写事件时间
//	        ES_FRAM_Close();
//	    }
//	}

//	
//	/************************************************************************
//	 * @function: ReadDlmsLastDayData
//	 * @描述: DLMS表读上一日的日冻结数据
//	 * 
//	 * @参数: 
//	 * @param: rdenable  1：必须读一次数据；0：根据情况决定是否读数据 
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: zjc (2014-8-16)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void ReadDlmsLastDayData(uint8 rdenable)
//	{
//	    ST_ACT_INPBUF_L    inLbuf;
//	    ST_ACT_INPBUF_S    inSbuf;
//	    ST_RDMT_CMD stDlmscmd;
//	    
//	    S_TstDLMSExtraData tstdata;           //存放表计内历史日冻结数据
//	    S_DAYDATA_SEG0 daydata;             //存放终端内历史日冻结数据
//	    const S_DATASEGINFO* segstr;
//	    uint8 YMDhms[6];   //时标
//	    uint8 YMDhms_1[6];   //上一天时标
//	    uint32 addr;
//	    uint16 bidx;
//	    uint8 flag = 0;                     //标志是否替换原数据
//	    uint8 readflag =0 , m;
//	           
//	    if(gs_InpExtracb[0].gbbreak)
//	    {
//	        return;
//	    }
//	    UpdateSysTimeStruct();  //更新系统时标
//	    
//	    //时间段参数
//	    MoveBuffer(gs_SysTime.YMDhms, YMDhms, 6);  //当前时间
//	    ClearBuffer(&YMDhms[3], 3);
//	    MoveBuffer(YMDhms,YMDhms_1, 6);  //当前时间
//	    StepDays(YMDhms_1, -1);      //提前到上一天
//	    
//	    for(uint8 uc_k = 0; uc_k < 3; uc_k++)   //确认3轮
//	    {
//	        for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++)  //外部测量点循环读取处理
//	        {    
//	            if(gss_CDATATst[uc_i].ef != 0x01)   //要求:1.测量点有效,2.485表
//	            { 
//	                continue;
//	            }
//	            HB_RetLive();                  //抄表期间主动保活
//	                
//	            flag = 0;                
//	            readflag = 0 ;
//	            if(rdenable == 1)
//	            {
//	                readflag = 1;
//	            }  
//	            if((gs_SysTime.YMDhms[3] == 0) && (uc_k == 0))   //
//	            {   
//	                readflag = 1;   //每日零点一定要获取一次冻结数据
//	            }
//	            if(gdw_ReadDayData(YMDhms_1, uc_i, 0, (uint8*)&daydata) != SA_ERR_OK)  //数据读出来不对，则必须补抄
//	            {
//	                readflag = 1;
//	            }
//	            else
//	            {
//	                if(STR_IsBitClr(daydata.seg.eft, EFT_DAY_WP) || STR_IsBitClr(daydata.seg.eft, EFT_DAY_WN))  //位图无效，再次补抄
//	                {
//	                    readflag = 1;
//	                }
//	            }
//	            if(readflag == 0)   //无需读取
//	            {
//	                continue;
//	            }
//	            MoveBuffer(YMDhms, inSbuf.buf, 6);   //取当日零点的日冻结数据
//	            MoveBuffer(YMDhms, inSbuf.buf + 6, 6);
//	            
//	            stDlmscmd.tstno = uc_i;
//	            stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	            stDlmscmd.di = NETP_READ_DLMS_DE;  
//	            if(HX_SendDlms(&inLbuf, &inSbuf, &stDlmscmd) == NETP_ERR_OK)
//	            {
//	                if(inLbuf.buf[3] != 0x00 || inLbuf.buf[4] != 0x01 || inLbuf.len < 52)  //不合法帧跳出
//	                {
//	                    continue;
//	                }
//	                //解析数据
//	                m = 22;
//	                tstdata.Wp[0] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[1] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[2] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[3] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[4] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wn[0] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	
//	                if(CmpBuffer((uint8*)tstdata.Wp, (uint8*)daydata.seg.Wp, sizeof(daydata.seg.Wp)) || STR_IsBitClr(daydata.seg.eft, EFT_DAY_WP))
//	                {
//	                    MoveBuffer((uint8*)tstdata.Wp, (uint8*)daydata.seg.Wp, sizeof(daydata.seg.Wp));
//	                    STR_SetBit(daydata.seg.eft, EFT_DAY_WP);
//	                    flag = 1;
//	                }
//	        
//	                if(CmpBuffer((uint8*)tstdata.Wn, (uint8*)daydata.seg.Wn, sizeof(daydata.seg.Wn)) || STR_IsBitClr(daydata.seg.eft, EFT_DAY_WN))
//	                {
//	                    MoveBuffer((uint8*)tstdata.Wn, (uint8*)daydata.seg.Wn, sizeof(daydata.seg.Wn));
//	                    STR_SetBit(daydata.seg.eft, EFT_DAY_WN);
//	                    flag = 1;
//	                }
//	                                            //保存替换数据后的结构体
//	                if(flag)
//	                {                           //ADDRESS(DIVIDED BY TST)
//	                    BuildTimeLabel(TTYPE_YMDHMS, YMDhms, &daydata.seg.stime);
//	                    daydata.seg.rtime = gs_SysTime;
//	                                            //保存到上一天日冻结
//	                    addr = _DayDataAddr(YMDhms_1, uc_i, 0, &segstr, &bidx);
//	                    if(addr == 0xffffffff)
//	                    {
//	                        continue;
//	                    }
//	                    WriteFlashWithCRC(DB_DAY, (uint8*)&daydata, segstr->seglen, addr);
//	                    STR_ClrFlashBit(DB_DAY, 0, bidx, 0xffff);//0有效,1无效
//	                }
//	            }
//	        }
//	    }
//	}
//	
//	/************************************************************************
//	 * @function: ReadDlmsLastMonEnergy
//	 * @描述: DLMS表读上一日的月冻结电能
//	 * 
//	 * @参数: 
//	 * @param: rdenable  1：必须读一次数据；0：根据情况决定是否读数据 
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: zjc (2014-8-16)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void ReadDlmsLastMonEnergy(uint8 rdenable)
//	{
//	    ST_ACT_INPBUF_L    inLbuf;
//	    ST_ACT_INPBUF_S    inSbuf;
//	    ST_RDMT_CMD stDlmscmd;
//	    
//	    S_TstDLMSExtraData tstdata;           //存放表计内历史日冻结数据
//	    S_DAYDATA_SEG0 daydata;             //存放终端内历史日冻结数据
//	    const S_DATASEGINFO* segstr;
//	    uint8 YMDhms[6];   //时标
//	    uint8 YMDhms_1[6];   //时标
//	    uint32 addr;
//	    uint16 bidx;
//	    uint8 flag = 0;                     //标志是否替换原数据
//	    uint8 readflag = 0, m; 
//	    
//	    if(gs_InpExtracb[0].gbbreak)
//	    {
//	        return;
//	    }
//	    UpdateSysTimeStruct();  //更新系统时标
//	    
//	    //时间段参数
//	    MoveBuffer(gs_SysTime.YMDhms,YMDhms,6);  //当前时间
//	    YMDhms[2] = 1;
//	    ClearBuffer(&YMDhms[3],3);
//	    MoveBuffer(YMDhms,YMDhms_1,6);  //当前时间
//	    StepDays(YMDhms_1, -1);      //提前到上一天.
//	    
//	    for(uint8 uc_k = 0; uc_k < 3; uc_k++)   //确认3轮
//	    {
//	        for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++)  //外部测量点循环读取处理
//	        {        
//	            if(gss_CDATATst[uc_i].ef != 0x01)  //要求:1.测量点有效,2.485表
//	            {
//	                continue;
//	            }
//	            HB_RetLive();                  //抄表期间主动保活
//	                
//	            flag = 0;                
//	            readflag =0 ;
//	            if(rdenable == 1)
//	            {
//	                readflag = 1;
//	            } 
//	            if((gs_SysTime.YMDhms[2] == 1) && (uc_k == 0))   //第一次读月冻结
//	            {   
//	                readflag = 1;   //每月第一天  一定要获取一次冻结数据
//	            }
//	            if(gdw_ReadMonthData(YMDhms_1, uc_i, 0, (uint8*)&daydata) != SA_ERR_OK)  //数据读出错误，则必须补抄一次数据
//	            {
//	                readflag = 1;
//	            }
//	            else
//	            {
//	                if(STR_IsBitClr(daydata.seg.eft, EFT_MONTH_WP) || STR_IsBitClr(daydata.seg.eft, EFT_MONTH_WN))
//	                {
//	                    readflag = 1;  //数据位图失效，则补抄一次
//	                }
//	            }
//	            if(readflag == 0)   //无需读取
//	            {
//	                continue;
//	            }
//	            
//	            MoveBuffer(YMDhms, inSbuf.buf, 6);   //取上一天的日冻结数据
//	            MoveBuffer(YMDhms, inSbuf.buf + 6, 6);
//	            
//	            stDlmscmd.tstno = uc_i;
//	            stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	            stDlmscmd.di = NETP_READ_DLMS_DE;  
//	            if(HX_SendDlms(&inLbuf, &inSbuf, &stDlmscmd) == NETP_ERR_OK)
//	            {
//	                if(inLbuf.buf[3] != 0x00 || inLbuf.buf[4] != 0x01 || inLbuf.len < 52)  //不合法帧跳出
//	                {
//	                    continue;
//	                }
//	                //解析数据
//	                m = 22;
//	                tstdata.Wp[0] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[1] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[2] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[3] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wp[4] = DLMS_ByteToData(inLbuf.buf, &m) * 10;
//	                tstdata.Wn[0] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	
//	                if(CmpBuffer((uint8*)tstdata.Wp, (uint8*)daydata.seg.Wp, sizeof(daydata.seg.Wp)) || STR_IsBitClr(daydata.seg.eft, EFT_MONTH_WP))
//	                {
//	                    MoveBuffer((uint8*)tstdata.Wp, (uint8*)daydata.seg.Wp, sizeof(daydata.seg.Wp));
//	                    STR_SetBit(daydata.seg.eft, EFT_MONTH_WP);
//	                    flag = 1;
//	                }
//	        
//	                if(CmpBuffer((uint8*)tstdata.Wn, (uint8*)daydata.seg.Wn, sizeof(daydata.seg.Wn)) || STR_IsBitClr(daydata.seg.eft, EFT_MONTH_WN))
//	                {
//	                    MoveBuffer((uint8*)tstdata.Wn, (uint8*)daydata.seg.Wn, sizeof(daydata.seg.Wn));
//	                    STR_SetBit(daydata.seg.eft, EFT_MONTH_WN);
//	                    flag = 1;
//	                }
//	                                            //保存替换数据后的结构体
//	                if(flag)
//	                {                           //ADDRESS(DIVIDED BY TST)
//	                    BuildTimeLabel(TTYPE_YMDHMS, YMDhms, &daydata.seg.stime);
//	                    daydata.seg.rtime = gs_SysTime;
//	                                            //保存到上一天日冻结
//	                    addr = _MonthDataAddr(YMDhms_1, uc_i, 0, &segstr, &bidx);
//	                    if(addr == 0xffffffff)
//	                    {
//	                        continue;
//	                    }
//	                    WriteFlashWithCRC(DB_MONTH, (uint8*)&daydata, segstr->seglen, addr);
//	                    STR_ClrFlashBit(DB_MONTH, 0, bidx, 0xffff);//0有效,1无效
//	                }
//	            }
//	        }
//	    }
//	}
//	
//	/************************************************************************
//	 * @function: ReadDlmsLastMonthDMData
//	 * @描述: DLMS表读上一月的月冻结数据
//	 * 
//	 * @参数: 
//	 * @param: rdenable  1：必须读一次数据；0：根据情况决定是否读数据 
//	 * 
//	 * @返回: 
//	 * @return: uint8  
//	 * @说明: 
//	 * @作者: zjc (2014-8-18)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void ReadDlmsLastMonthDMData(uint8 rdenable)
//	{
//	    ST_ACT_INPBUF_L    inLbuf;
//	    ST_ACT_INPBUF_S    inSbuf;
//	    ST_RDMT_CMD stDlmscmd;
//	    
//	    S_TstDmdData tstdata;               //存放表计内历史月冻结数据
//	    S_MONDATA_SEG2 monthdata;             //存放终端内历史月冻结数据
//	    const S_DATASEGINFO* segstr;
//	    uint8 YMDhms[6];   //时标
//	    uint8  YMDhms_1[6];
//	    uint32 addr;
//	    uint16 bidx;
//	    uint8 flag = 0; 
//	    uint8 readflag;
//	    
//	    if(gs_InpExtracb[0].gbbreak)
//	    {
//	        return;
//	    }
//	    UpdateSysTimeStruct();  //更新系统时标
//	    
//	    MoveBuffer(gs_SysTime.YMDhms,YMDhms,6);  //当前时间
//	    YMDhms[2] = 1;
//	    ClearBuffer(&YMDhms[3],3);
//	    MoveBuffer(YMDhms,YMDhms_1,6);  //当前时间
//	    StepDays(YMDhms_1, -1);      //提前到上一天.
//	    
//	    for(uint8 uc_k = 0; uc_k < 3; uc_k++)   //确认3轮
//	    {                                 
//	        for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++) //外部测量点循环读取处理
//	        {         
//	            if(gss_CDATATst[uc_i].ef != 0x01)  //要求:1.测量点有效,2.485表
//	            {
//	                continue;
//	            }
//	            HB_RetLive();                  //抄表期间主动保活
//	                
//	            flag = 0;                
//	            readflag =0 ;
//	            if(rdenable == 1)
//	            {
//	                readflag = 1;
//	            } 
//	            if((gs_SysTime.YMDhms[2] == 1) && (uc_k == 0))   //第一次读月冻结
//	            {   
//	                readflag = 1;   //每月第一天  一定要获取一次冻结数据
//	            }
//	            if(gdw_ReadMonthData(YMDhms_1, uc_i, 2, (uint8*)&monthdata) != SA_ERR_OK)  //数据读出错误，则必须补抄一次数据
//	            {
//	                readflag = 1;
//	            }
//	            else
//	            {
//	                if(STR_IsBitClr(monthdata.seg.eft, EFT_MON_DPP) || STR_IsBitClr(monthdata.seg.eft, EFT_MON_DPPT))
//	                {
//	                    readflag = 1;
//	                }
//	            }
//	            if(readflag == 0)   //无需读取
//	            {
//	                continue;
//	            }
//	            
//	            MoveBuffer(YMDhms, inSbuf.buf, 6);   
//	            MoveBuffer(YMDhms, inSbuf.buf + 6, 6);
//	            
//	            stDlmscmd.tstno = uc_i;
//	            stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
//	            stDlmscmd.di = NETP_READ_DLMS_DM;  
//	            if(HX_SendDlms(&inLbuf, &inSbuf, &stDlmscmd) == NETP_ERR_OK)
//	            {
//	                if(inLbuf.buf[3] != 0x00 || inLbuf.buf[4] != 0x01 || inLbuf.len < 70)  //不合法帧跳出
//	                {
//	                    continue;
//	                }
//	                //解析数据
//	                tstdata.dpp[0] = DLMS_ByteToInt24(inLbuf.buf + 24, true) * 10;
//	                tstdata.dppt[0] = FMT_UnifyTime(inLbuf.buf + 27, true, true);
//	                tstdata.dpp[1] = DLMS_ByteToInt24(inLbuf.buf + 34, true) * 10;
//	                tstdata.dppt[1] = FMT_UnifyTime(inLbuf.buf + 37, true, true);
//	                tstdata.dpp[2] = DLMS_ByteToInt24(inLbuf.buf + 44, true) * 10;
//	                tstdata.dppt[2] = FMT_UnifyTime(inLbuf.buf + 47, true, true);
//	                tstdata.dpp[3] = DLMS_ByteToInt24(inLbuf.buf + 54, true) * 10;
//	                tstdata.dppt[3] = FMT_UnifyTime(inLbuf.buf + 57, true, true);
//	                tstdata.dpp[4] = DLMS_ByteToInt24(inLbuf.buf + 64, true) * 10;
//	                tstdata.dppt[4] = FMT_UnifyTime(inLbuf.buf + 67, true, true);
//	
//	                if(CmpBuffer((uint8*)tstdata.dpp, (uint8*)monthdata.seg.dpp, sizeof(monthdata.seg.dpp)) || STR_IsBitClr(monthdata.seg.eft, EFT_MON_DPP))
//	                {
//	                    MoveBuffer((uint8*)tstdata.dpp, (uint8*)monthdata.seg.dpp, sizeof(monthdata.seg.dpp));
//	                    STR_SetBit(monthdata.seg.eft, EFT_MON_DPP);
//	                    flag = 1;
//	                }
//	                
//	                if(CmpBuffer((uint8*)tstdata.dppt, (uint8*)monthdata.seg.dppt, sizeof(monthdata.seg.dppt)) || STR_IsBitClr(monthdata.seg.eft, EFT_MON_DPPT))
//	                {
//	                    MoveBuffer((uint8*)tstdata.dppt, (uint8*)monthdata.seg.dppt, sizeof(monthdata.seg.dppt));
//	                    STR_SetBit(monthdata.seg.eft, EFT_MON_DPPT);
//	                    flag = 1;
//	                }
//	                                            //保存替换数据后的结构体
//	                if(flag)
//	                {                           //ADDRESS(DIVIDED BY TST)
//	                    BuildTimeLabel(TTYPE_YMDHMS, YMDhms, &monthdata.seg.stime);
//	                    monthdata.seg.rtime = gs_SysTime;
//	                                            //保存到上一天日冻结
//	                    addr = _MonthDataAddr(YMDhms_1, uc_i, 2, &segstr, &bidx);
//	                    if(addr == 0xffffffff)
//	                    {
//	                        continue;
//	                    }
//	                    WriteFlashWithCRC(DB_MONTH, (uint8*)&monthdata, segstr->seglen, addr);
//	                    STR_ClrFlashBit(DB_MONTH, 0, bidx, 0xffff);//0有效,1无效
//	                }
//	            }
//	        }
//	    }
//	}



/************************************************************************
 * @function: Netp_ReadCure
 * @描述: 读取DLMS负荷通道数据，
 * 
 * @参数: 
 * @param: time  曲线点时间
 * @param: din   召测的数据项
 * @param: uc_i 测量点
 * @param: rdtype 读数据类型  true：实时；false：补点
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/


 


uint8 Netp_ReadCure(uint8 *time , uint8 *din, uint8 uc_i, PST_Netp_FW fw)
{
    ST_ACT_INPBUF_L    inLbuf;
//	    ST_ACT_INPBUF_S    inSbuf;
//	    uint8_t buf[ACT_MAX_INBUF_L_LEN];
    ST_RDMT_CMD stDlmscmd;
    S_TstCurveData tstdata;           //存放表计内历史负荷数据
    S_CURDATA_SEG0 curedata;             //存放终端内历史日冻结数据
    NETP_Frame frame;
    uint8 flag = 0;                     //标志是否替换原数据
    uint8 netflag = NETP_ERR_FT, m;
    
//	    HB_RetLive();                  //抄表期间主动保活
    memset(&tstdata,0,LEN_CUR_SEG0);

    //根据表内冻结时间读取对应的终端冻结数据
    gdw_ReadCureData(time, uc_i, (uint8 *)&curedata); //读曲线的数据

    for(int i = 0; i < fw->datanum; i++)
    {
//	        memset(buf, 0, ACT_MAX_INBUF_S_LEN);
//	        frame->recv = buf;
//	        frame->send = buf;
        NetpFrameDataStr * pDataStr = &fw->frameDtaStr[i];
        if( STR_IsBitSet(din ,i) )  //
        {
            stDlmscmd.port = gss_CDATATst[uc_i].pt;
            stDlmscmd.addr = gss_CDATATst[uc_i].info.main_addr;  
            stDlmscmd.addr_len = 1;
            stDlmscmd.fw = fw;
            stDlmscmd.baud_cfg = gss_CDATATst[uc_i].bs;
            stDlmscmd.di = i;
            if(Netp_Send_With_Retry(&inLbuf, &stDlmscmd) == NETP_ERR_OK)
            {
                flag = 1;
                _RefreshTrnFlag(uc_i, true);
                int m = 0;
                for(int i = 0; i < pDataStr->rinum; i++)
                {
                    switch(pDataStr->retItem[i].atr)
                    {
                        case NETP_READ_TIME:
                            break;
                        case NETP_READ_CUR_FLOW:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.SI))
                            {
    //	                            memcpy(tstdata.SI, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.SI, (uint8*)curedata.seg.SI, sizeof(curedata.seg.SI)) || 
                                    STR_IsBitClr(curedata.seg.eft, EFT_MIN_SI))
                                {
                                
                                    MoveBuffer((uint8*)tstdata.SI, (uint8*)curedata.seg.SI, sizeof(curedata.seg.SI));
                                    STR_SetBit(curedata.seg.eft, EFT_MIN_SI);
                                    flag = 1;
                                }  
                            }
                            break;
                        case NETP_READ_WORKING_CUR_FLOW:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.WI))
                            {
//	                                memcpy(tstdata.WI, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.WI, (uint8*)curedata.seg.WI, sizeof(curedata.seg.WI)) || 
                                    STR_IsBitClr(curedata.seg.eft, EFT_MIN_WI))
                                {
                                    MoveBuffer((uint8*)tstdata.WI, (uint8*)curedata.seg.WI, sizeof(curedata.seg.WI));
                                    STR_SetBit(curedata.seg.eft, EFT_MIN_WI);
                                    flag = 1;
                                }           
                            }
                            break;
                        
                        case NETP_READ_ACCUMULATE_TOTAL:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.ST))
                            {
//	                            memcpy(tstdata.ST, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.ST, (uint8*)curedata.seg.ST, sizeof(curedata.seg.ST)) || 
                                    STR_IsBitClr(curedata.seg.eft, EFT_MIN_ST))
                                {
                                
                                    MoveBuffer((uint8*)tstdata.ST, (uint8*)curedata.seg.ST, sizeof(curedata.seg.ST));
                                    STR_SetBit(curedata.seg.eft, EFT_MIN_ST);
                                    flag = 1;
                                }
                            }
                            
                            break;
                        case NETP_READ_WORKING_ACCUMULATE_TOTAL:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.WT))
                            {
//	                                memcpy(tstdata.WT, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.WT, (uint8*)curedata.seg.WT, sizeof(curedata.seg.WT)) || 
                                    STR_IsBitClr(curedata.seg.eft, EFT_MIN_WT))
                                {
                                    MoveBuffer((uint8*)tstdata.WT, (uint8*)curedata.seg.WT, sizeof(curedata.seg.WT));
                                    STR_SetBit(curedata.seg.eft, EFT_MIN_WT);
                                    flag = 1;
                                }
                            }
                            break;
                        case NETP_READ_TEMP:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.TMP))
                            {
//	                            memcpy(tstdata.TMP, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.TMP, (uint8*)curedata.seg.TMP, sizeof(curedata.seg.TMP)) || 
                                    STR_IsBitClr(curedata.seg.eft, EFT_MIN_TMP))
                                {
                                    MoveBuffer((uint8*)tstdata.TMP, (uint8*)curedata.seg.TMP, sizeof(curedata.seg.TMP));
                                    STR_SetBit(curedata.seg.eft, EFT_MIN_TMP);
                                    flag = 1;
                                }
                            }
                            break;
                        case NETP_READ_PRESSURE:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.P))
                            {
//	                            memcpy(tstdata.P, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.P, (uint8*)curedata.seg.P, sizeof(curedata.seg.P)) || 
                                    STR_IsBitClr(curedata.seg.eft, EFT_MIN_P))
                                {
                                    MoveBuffer((uint8*)tstdata.P, (uint8*)curedata.seg.P, sizeof(curedata.seg.P));
                                    STR_SetBit(curedata.seg.eft, EFT_MIN_P);
                                    flag = 1;
                                }               
                            }
                            break;
                        case NETP_READ_REMAIND_GAS:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.RM))
                            {
//	                            memcpy(tstdata.P, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.RM, (uint8*)curedata.pseg.RM, sizeof(curedata.pseg.RM)) || 
                                    STR_IsBitClr(curedata.pseg.eft, EFT_MIN_RM))
                                {
                                    MoveBuffer((uint8*)tstdata.RM, (uint8*)curedata.pseg.RM, sizeof(curedata.pseg.RM));
                                    STR_SetBit(curedata.pseg.eft, EFT_MIN_RM);
                                    flag = 1;
                                }               
                            }                            
                            break;
                        case NETP_READ_ACCUMULATE_TOTAL_PREPAY:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.ST))
                            {
//                              memcpy(tstdata.ST, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.ST, (uint8*)curedata.pseg.ST, sizeof(curedata.pseg.ST)) || 
                                    STR_IsBitClr(curedata.pseg.eft, EFT_MIN_ST))
                                {
                                
                                    MoveBuffer((uint8*)tstdata.ST, (uint8*)curedata.pseg.ST, sizeof(curedata.pseg.ST));
                                    STR_SetBit(curedata.pseg.eft, EFT_MIN_ST);
                                    flag = 1;
                                }
                            }
                            break;
                        case NETP_READ_CUR_PRICE:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.UP))
                            {
//                              memcpy(tstdata.ST, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.UP, (uint8*)curedata.pseg.UP, sizeof(curedata.pseg.UP)) || 
                                    STR_IsBitClr(curedata.pseg.eft, EFT_MIN_PRICE))
                                {
                                
                                    MoveBuffer((uint8*)tstdata.UP, (uint8*)curedata.pseg.UP, sizeof(curedata.pseg.UP));
                                    STR_SetBit(curedata.pseg.eft, EFT_MIN_PRICE);
                                    flag = 1;
                                }
                            }
                            
                            break;
                        case NETP_READ_WXIC_METER_STT:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)tstdata.STT))
                            {
//                              memcpy(tstdata.ST, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)tstdata.STT, (uint8*)curedata.pseg.STT, sizeof(curedata.pseg.STT)) || 
                                    STR_IsBitClr(curedata.pseg.eft, EFT_MIN_MSTT))
                                {
                                
                                    MoveBuffer((uint8*)tstdata.STT, (uint8*)curedata.pseg.STT, sizeof(curedata.pseg.STT));
                                    STR_SetBit(curedata.pseg.eft, EFT_MIN_MSTT);
                                    flag = 1;
                                }
                            }
                        case NETP_READ_VOL_HADFLOW:
                            if(pDataStr->retItem[i].len < 12 && SYS_ERR_OK == ADATA_FunctionW(pDataStr->retItem[i].id, 
                                           (pDataStr->retItem[i].mult << 8)| pDataStr->retItem[i].len & 0xFF, 
                                           inLbuf.buf+m,  (uint8*)&tstdata.vol))
                            {
//                              memcpy(tstdata.ST, inLbuf.buf+m, pDataStr->retItem[i].len);
                                if(CmpBuffer((uint8*)&tstdata.vol, (uint8*)&curedata.pseg.vol, 3) || 
                                    STR_IsBitClr(curedata.pseg.eft, EFT_MIN_VOLH))
                                {
                                
                                    MoveBuffer((uint8*)&tstdata.vol, (uint8*)&curedata.pseg.vol, 3);
                                    STR_SetBit(curedata.pseg.eft, EFT_MIN_VOLH);
                                    flag = 1;
                                }
                            }
                            
                            break;                                           
                        case NETP_READ_WARN_FLAG:
                        case NETP_READ_STT:
                        default:
                            break;
                    }
                    
                    m+=pDataStr->retItem[i].len;
                }

//	                if(CmpBuffer((uint8*)tstdata.ST, (uint8*)curedata.seg.ST, sizeof(curedata.seg.ST)) || 
//	                    STR_IsBitClr(curedata.seg.eft, EFT_MIN_ST))
//	                {
//	                    MoveBuffer((uint8*)tstdata.ST, (uint8*)curedata.seg.ST, sizeof(curedata.seg.ST));
//	                    STR_SetBit(curedata.seg.eft, EFT_MIN_ST);
//	                    flag = 1;
//	                }
//	                if(inLbuf.buf[3] == 0x00 && inLbuf.buf[4] == 0x01 && inLbuf.len >= 27)  //不合法帧跳出
//	                {
//	                    m = 22;
//	                    tstdata.Wp[0] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                    if(inLbuf.len > 28)
//	                    {
//	                        tstdata.Wp[1] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                        tstdata.Wp[2] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                        tstdata.Wp[3] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                        STR_SetBit(curedata.seg.eft, EFT_MIN_WPF);
//	                    }
//	                    else
//	                    {
//	                        STR_ClrBit(curedata.seg.eft, EFT_MIN_WPF);
//	                    }
//	                    if(CmpBuffer((uint8*)tstdata.Wp, (uint8*)curedata.seg.Wp, sizeof(curedata.seg.Wp)) || STR_IsBitClr(curedata.seg.eft, EFT_MIN_WP))
//	                    {
//	                        MoveBuffer((uint8*)tstdata.Wp, (uint8*)curedata.seg.Wp, sizeof(curedata.seg.Wp));
//	                        STR_SetBit(curedata.seg.eft, EFT_MIN_WP);
//	                        flag = 1;
//	                    }
//	                }
                netflag = NETP_ERR_OK;
            }
            else
            {
                _RefreshTrnFlag(uc_i, false);
            }
        }


    }



//	    MoveBuffer(time, inSbuf.buf, 6);   
//	    MoveBuffer(time, inSbuf.buf + 6, 6);
//	    
//	    stDlmscmd.tstno = uc_i;
//	    stDlmscmd.cmdtype = NETP_DLMS_TYPE_GET;
    

//	    if( STR_IsBitSet(din ,NETP_READ_DLMS_WN) && ((netflag == NETP_ERR_OK) || (rdtype == false)))  //无功电能负荷
//	    {
//	        stDlmscmd.di = NETP_READ_DLMS_WN;  
//	        if(HX_SendDlms(&inLbuf, &inSbuf, &stDlmscmd) == NETP_ERR_OK)
//	        {
//	            if(inLbuf.buf[3] == 0x00 && inLbuf.buf[4] == 0x01 && inLbuf.len >= 27)  //不合法帧跳出
//	            {
//	                m = 22;
//	                tstdata.Wn[0] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                if(inLbuf.len > 28)
//	                {
//	                    tstdata.Wn[1] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                    tstdata.Wn[2] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                    tstdata.Wn[3] = DLMS_ByteToData(inLbuf.buf, &m) / 10;
//	                    STR_SetBit(curedata.seg.eft, EFT_MIN_WNF);
//	                }
//	                else
//	                {
//	                    STR_ClrBit(curedata.seg.eft, EFT_MIN_WNF);
//	                }
//	                if(CmpBuffer((uint8*)tstdata.Wn, (uint8*)curedata.seg.Wn, sizeof(curedata.seg.Wn)) || STR_IsBitClr(curedata.seg.eft, EFT_MIN_WN))
//	                {
//	                    MoveBuffer((uint8*)tstdata.Wn, (uint8*)curedata.seg.Wn, sizeof(curedata.seg.Wn));
//	                    STR_SetBit(curedata.seg.eft, EFT_MIN_WN);
//	                    flag = 1;
//	                }
//	            }
//	            netflag = NETP_ERR_OK;
//	        }
//	    }

    if(flag)
    {                           //ADDRESS(DIVIDED BY TST)
        BuildTimeLabel(TTYPE_YMDHMS, time, &curedata.seg.stime);
        curedata.seg.dev = gss_CDATATst[uc_i].info.addr;  
        memcpy(curedata.seg.factory_id, fw->device_name, 4);
        UpdateSysTimeStruct();  //更新系统时标
        curedata.seg.rtime = FMT_UnifyTime(gs_SysTime.YMDhms, false, true);   //保存抄表时间
        gdw_SaveCureData(time, uc_i, (uint8*)&curedata);  //保存到
    }
    return netflag;
}

ST_Netp_Reg gs_netpRegMap[CON_MAX_DEVICE_TYPES];

int Netp_Register_Init(void)
{
    int i = 0;
    int m = 0;
    uint8_t tmp[10];
    uint8_t ret = 0;
    memset((uint8_t *)&gs_netpRegMap[0], 0, sizeof(ST_Netp_Reg) * CON_MAX_DEVICE_TYPES);
    for(int i = 0; i < CON_MAX_DEVICE_TYPES; i++)
    {
        memset(tmp,0,10);
        ret = Netp_Framework_RW(NETP_FW_PADDR + i * NETP_FW_PLEN, tmp, sizeof(ST_Netp_Reg), false);
        if(SYS_ERR_OK == ret && tmp[0] == i)
        {
            memcpy((uint8_t *)&gs_netpRegMap[m++],tmp, sizeof(ST_Netp_Reg));
        }
    }
    gs_SysVar.netpRegNum = m;
    
    return 0;
}



int Netp_Register_Set(ST_NETP_FW_BASE * fw)
{
    int ret = 0;
    if(fw->pf >= CON_MAX_DEVICE_TYPES)
    {
        return -1;
    }
    ret = Netp_Framework_RW(NETP_FW_PADDR + fw->pf  * NETP_FW_PLEN, (uint8_t *)fw, NETP_FW_PLEN, true);
    if(SYS_ERR_OK == ret)
    {
//	        if(gs_SysVar.netpRegNum < MAX_DEVICE_TYPES)
//	            memcpy((uint8_t *)&gs_netpRegMap[gs_SysVar.netpRegNum++],(uint8_t *)fw, sizeof(ST_Netp_Reg));
        return 0;
    }
    return -1;
}

int Netp_Register_Check(uint8_t pf, PST_Netp_FW fw)
{
    return -1;
}


int Netp_Get_Framework(uint8_t pf, PST_Netp_FW fw)
{
    if(pf >= CON_MAX_DEVICE_TYPES)
    {
        return -1;
    }

    int ret = Netp_Framework_RW(NETP_FW_PADDR + pf * NETP_FW_PLEN, fw, NETP_FW_PART_PLEN, false);

    if(SYS_ERR_OK == ret)
    {
        if(fw->pf < CON_MAX_DEVICE_TYPES && IsAsciiChar(fw->device_name[0])
            && IsAsciiChar(fw->device_name[1]) && IsAsciiChar(fw->device_name[2])
            && IsAsciiChar(fw->device_name[3]))
        {
            SYS_OK();
        }
        else
        {
            SYS_ERR();
        }
    }
    else
    {
        SYS_ERR();
    }
}
//	#pragma location = ".mmm"
ST_Netp_FW gs_netp_fw_tmp[MCB_PORT_NUM];
/************************************************************************
 * @function: Netp_ReadCureData
 * @描述: 读取DLMS负荷通道数据，包括15分钟曲线，
 * 
 * @参数: 
 * @param: index 抄表端口索引
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_ReadCureData(uint8 index)
{   
    uint8 curbitmap[5];  //40bit
    uint8 YMDhms[6];   //时标
    uint8 ucmin;
    uint8_t ret = 0;
    ST_Netp_FW * fw = &gs_netp_fw_tmp[index];
    
    ClearBuffer(curbitmap, sizeof(curbitmap));
//    STR_SetBits(curbitmap, 1, NETP_READ_TX13_F, 1);  //天信1.3
//	    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_WN, 1);  //抄正向无功电能
//	    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_U, 1);  //抄电压
//	    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_I, 1);  //抄电流
//	    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_P, 1);  //抄功率
    
//	    if(gs_InpExtracb[0].gbbreak)
//	    {
//	        return;
//	    }
    UpdateSysTimeStruct();  //更新系统时标
    
    MoveBuffer(gs_SysTime.YMDhms, YMDhms, 6);  //当前时间  
    ucmin = YMDhms[4] % gs_PstPara.cp_interval;
    YMDhms[4] = YMDhms[4] - ucmin;   //取上次 曲线保存分钟点
    YMDhms[5] = 0;  //秒
                              //外部测量点循环读取处理
    for(uint8 uc_i = 0; uc_i < CORE_TST_NO; uc_i++)
    {       
        if(!((gss_CDATATst[uc_i].info.type == 0x01 || gss_CDATATst[uc_i].info.type == 0x03)
            && gss_CDATATst[uc_i].pt == index))   //要求:1流量计 3IC卡控制器
        {
            continue;
        }
        ret = Netp_Get_Framework(gss_CDATATst[uc_i].pf, fw);
//	            uint8_t pnum = GetP
        if(SYS_ERR_OK == ret && fw->pf < CON_MAX_DEVICE_TYPES)
        {
            for(uint8_t uc_j = 0; uc_j < fw->datanum; uc_j++)
            {
                
                STR_SetBits(curbitmap, 1, uc_j, 1);
            }
            Netp_ReadCure(YMDhms, curbitmap, uc_i, fw);   //读实时曲线
        }
    }
    memcpy(gs_NetpReadStt[index].extime.YMDhms, YMDhms, 6);
//	    for(uint8 uc_i = 0; uc_i < 2; uc_i++)   //查询两轮
//	    {
//	        Netp_ReReadCureData(0, 4);         //每次查询前面4个点
//	    }
}


/************************************************************************
 * @function: Netp_ReReadCureData
 * @描述: 每隔一个时间段查询，时间段内曲线是否遗漏，对遗漏曲线进行补招
 * 
 * @参数: 
 * @param: index 抄表端口索引
 * @param: readnum 补点数
 * @返回: 
 * @说明: 
 * @作者: zjc (2014/8/14)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	void Netp_ReReadCureData(uint8 index, uint8 readnum)
//	{
//	    uint8 curbitmap[5];  //40bit
//	    S_CURDATA_SEG0 curedata;             //存放终端内历史日冻结数据
//	    uint8 YMDhms[6],YMDhmsbf[6];   //
//	    uint8 ucmin;
//	    
//	//	    if(gs_InpExtracb[0].gbbreak)
//	//	    {
//	//	        return;
//	//	    }
//	    UpdateSysTimeStruct();  //更新系统时标
//	    
//	    MoveBuffer(gs_SysTime.YMDhms, YMDhmsbf, 6);  //当前时间
//	    ucmin = YMDhmsbf[4] % gs_PstPara.cp_interval;
//	    YMDhmsbf[4] = YMDhmsbf[4] - ucmin;   //取上次 曲线保存分钟点
//	    YMDhmsbf[5] = 0;  //秒
//	    StepMins(YMDhmsbf, -((readnum - 1) * 15));  
//	    
//	    for(uint8 uc_i = 0; uc_i < CORE_TST_NO; uc_i++)
//	    {   
//	//	        if(gss_CDATATst[uc_i].ef != 0x01)   //要求:1.测量点有效,2.485表
//	//	        {
//	//	            continue;
//	//	        }
//	        MoveBuffer(YMDhmsbf, YMDhms, 6);
//	        
//	        for(uint8 uc_j = 0; uc_j< readnum; uc_j++)   // 补时间段内所有的点数
//	        {
//	            ClearBuffer(curbitmap, sizeof(curbitmap));
//	            if(gdw_ReadCureData(YMDhms, uc_i, (uint8 *)&curedata) != SYS_ERR_OK) //读取曲线数据错误，则补全部数据项
//	            {
//	                STR_SetBits(curbitmap, 1, NETP_READ_DLMS_WP, 1);  //抄正向有功电能
//	                STR_SetBits(curbitmap, 1, NETP_READ_DLMS_WN, 1);  //抄正向无功电能
//	                STR_SetBits(curbitmap, 1, NETP_READ_DLMS_U, 1);  //抄电压
//	                STR_SetBits(curbitmap, 1, NETP_READ_DLMS_I, 1);  //抄电流
//	                STR_SetBits(curbitmap, 1, NETP_READ_DLMS_P, 1);  //抄功率
//	            }
//	            else
//	            {
//	                if(STR_IsBitClr(curedata.seg.eft, EFT_MIN_WP))  //正向有功电能  是否有效
//	                {
//	                    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_WP, 1);  
//	                }
//	                if(STR_IsBitClr(curedata.seg.eft, EFT_MIN_WN))  //正向无功电能    是否有效
//	                {
//	                    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_WN, 1);  
//	                }
//	                if(STR_IsBitClr(curedata.seg.eft, EFT_MIN_U))  //电压是否有效
//	                {
//	                    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_U, 1);  
//	                }
//	                if(STR_IsBitClr(curedata.seg.eft, EFT_MIN_I))  //电流是否有效
//	                {
//	                    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_I, 1);
//	                }
//	                if(STR_IsBitClr(curedata.seg.eft, EFT_MIN_P))  //功率  是否有效
//	                {
//	                    STR_SetBits(curbitmap, 1, NETP_READ_DLMS_P, 1);  
//	                }
//	            }
//	            if( STR_CalcBitSetNum(curbitmap ,sizeof(curbitmap)) )
//	            {
//	                if(Netp_ReadCure(YMDhms ,curbitmap, uc_i, false) != NETP_ERR_OK)   //读实时曲线
//	                {
//	                    //如果第一个点补数据无任何返回，则不再补后续的点，防止补数据时间过长影响后续的抄表
//	                    break;
//	                }
//	            }
//	            StepMins(YMDhms, 15);  //步进15分钟，查询下一个曲线点
//	        }
//	    }
//	}



unsigned char GetProtocolNum(unsigned char *tMan, unsigned char *tPro)
{
    unsigned char tDeviceMan[4], i, re;
    unsigned char DEVICE_MAN[5];
    tDeviceMan[0] = *(tMan + 0);
    tDeviceMan[1] = *(tMan + 1);
    tDeviceMan[2] = *(tPro + 0);
    tDeviceMan[3] = *(tPro + 1);
    re = 0xff;
    
    for(i= 0; i < CON_MAX_DEVICE_TYPES; i++)
    {
        
        Netp_Framework_RW(NETP_FW_PADDR+i*NETP_FW_PLEN, DEVICE_MAN, 5, false);
        if(memcmp(&DEVICE_MAN[1], tDeviceMan, 4) == 0)
        {
            re = DEVICE_MAN[0];
            break;
        }
    }
    return re;
}

