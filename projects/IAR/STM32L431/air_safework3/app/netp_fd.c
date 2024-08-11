/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: netp_fd.c
**创   建   人: yzy
**最后修改日期: 2013年6月8日
**描        述: 组网端口(非单独进程,嵌入到pst进程中)
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
**创建人: yzy
**版  本: v1.0
**日　期: 2013年6月8日
**描　述: 原始版本
********************************************************************************************************/
#define EXT_NETP_FD
#include "es.h"
#include "hal.h"
#include "bsp.h"
#include "app.h"
#include "apis.h"
#include "extapi.h"


const uint8 err_addr[6] = {0xee, 0xee, 0xee, 0xee, 0xee, 0xee};


/************************************************************************
 * @function: MapSystitleToBgaddr
 * @描述: 根据systitle换算出表号(只能得到表号低8位,高8位留空为零)
 * 
 * @参数: 
 * @param: systitle 输入,8字节
 * @param: bgaddr 输出,6字节
 * 
 * @返回: 
 * @return: uint8 协议类型:3单相,4三相,0xFF处理异常
 * @说明: 举例:systitle-48 58 45 01 30 D1 52 59.单相:表号:0x0D15259=13718105
 * @作者: yzy (2014/6/2)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
/*
uint8 DLMS_MapSystitleToBgaddr(uint8* systitle, uint8* bgaddr)
{
    uint32 tmp;
                                        //取出systitle中代表表号的那部分
    tmp = ByteToLong(systitle + 4);
    InvBuffer((uint8*)&tmp, sizeof(tmp));
    tmp &= 0x0FFFFFFF;
                                        //转换成bcd
    memset(bgaddr, 0, 6);
    HexToBcd(bgaddr, tmp, 5, 4);

    if(systitle[3] == 0x01)
    {
        return 3;
    }
    else if(systitle[3] == 0x03)
    {
        return 4;
    }
    else
    {
        return 0xff;
    }
}
*/

/************************************************************************
 * @function: CalcDlmsSaddr
 * @描述: 根据短地址计算出DLMS实际报文中的地址域
 *
 * @参数:
 * @param: saddr 短地址
 *
 * @返回:
 * @return: uint16 DLMS报文中的短地址域
 * @说明:
 * @作者: yzy (2014/6/4)
 *-----------------------------------------------------------------------
 * @修改人:
 ************************************************************************/
uint16 CalcDlmsSaddr(uint16 saddr)
{
    uint16 mtidx;
    uint16 tmp;
                                        //计算HDLC短地址
    mtidx = saddr;
	mtidx <<= 1;
	tmp = mtidx & 0x00FF;
	mtidx <<= 1;
	mtidx &= 0xFE00;
	mtidx = (mtidx | tmp | 0x01);

    /*
    uint8 hfaddr = saddr >> 8;
    uint8 lfaddr = saddr;
    outbuffer[5] = ((hfaddr << 1) + (lfaddr >> 7)) << 1;
    outbuffer[6] = (lfaddr << 1) + 0x01;
    */

    return mtidx;
}


/************************************************************************
 * @function: DLMS_AppReadPack
 * @描述: dlms应用层数据帧打包(增加0x7E帧头,0x7E帧尾,CRC)
 *
 * @参数:
 * @param: indata 应用层数据域
 * @param: inlen 应用层数据长度
 * @param: outbuffer 输出帧缓存
 * @param: saddr 短地址
 *
 * @返回:
 * @return: uint8  打包好的长度
 * @说明:
 * @作者: yzy (2014/6/4)
 *-----------------------------------------------------------------------
 * @修改人:
 ************************************************************************/
uint8 DLMS_AppReadPack(uint8* indata, uint8 inlen, uint8* outbuffer, uint16 saddr)
{
    uint16 crc;
    uint16 mtidx;
                                        //计算HDLC短地址
    mtidx = CalcDlmsSaddr(saddr);
                                        //保护数据域
    MoveBuffer(indata, outbuffer + 14, inlen);
                                        //移入帧头
    outbuffer[0] = 0x7E;
    outbuffer[1] = 0xA0;                //frame type(无后续帧)
    outbuffer[1] |= (inlen + 15) >> 8;  //length_1
    outbuffer[2] = (inlen + 15) & 0xFF; //length_2
    outbuffer[3] = 0x00;                //固定(目标地址一部分)
    outbuffer[4] = 0x02;                //固定(目标地址一部分)
    outbuffer[5] = mtidx >> 8;          //hdlc短地址(目标地址一部分)
    outbuffer[6] = mtidx;               //hdlc短地址(目标地址一部分)

    outbuffer[7] = 0x03;                //固定,源地址
    outbuffer[8] = 0x54;                //不知道是什么东西.
                                        //校验帧头crc
    crc = DoCrcDlms(outbuffer + 1, 8, 0xffff);
    outbuffer[9] = (uint8)crc;
    outbuffer[10] = (uint8)(crc >> 8);
                                        //填充数据内容并作全帧crc
    outbuffer[11] = 0xE6;
    outbuffer[12] = 0xE6;
    outbuffer[13] = 0;
    crc = DoCrcDlms(outbuffer + 1, 14 + inlen - 1, 0xffff);
    outbuffer[14 + inlen] = (uint8)crc;
    outbuffer[15 + inlen] = (uint8)(crc >> 8);
    outbuffer[16 + inlen] = 0x7E;
                                        //返回总长度
    return inlen + 17;
}




/************************************************************************
 * @function: PST_DLMS_HDLC_Decode
 * @描述: 判断当前缓存里面是否为合法帧,如果是的话则解析成为被动通讯结构体
 *
 * @参数:
 * @param: receive 接收到数据的缓存
 * @param: send 需要发送的数据的缓存
 * @param: sbuflen 发送缓存长度
 * @param: frame 数据解析成的结构体
 *
 * @返回:
 * @return: uint8
 * @说明:
 * @作者: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @修改人:
 ************************************************************************/
uint8 PST_DLMS_HDLC_Decode(uint8* recv, uint8* send, uint16 sbuflen, PST_Frame* frame)
{
    DLMS_Frame* frm = &frame->_dlms;

    #if (ES_MNT_EN > 0)
    ES_MNT_Puts("\r\nreceive valid dlms frame\r\n");
    #endif

    frm->cmd = recv[14];
    frm->recv = recv + 15;
    frm->send = send;
    
    frm->len = recv[2] + ((recv[1] & 0x7f) << 8) - 2 - 14;        //2字节crc,14字节(Byte1-ByteCmd).

    ES_OK();
}




/************************************************************************
 * @function: PST_DLMS_HDLC_Proc
 * @描述:
 *
 * @参数:
 * @param: frm 数据解析成的结构体
 *
 * @返回:
 * @return: uint8  PST_ERR_OK/PST_ERR_FT
 * @说明:
 * @作者: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @修改人:
 ************************************************************************/
uint8 PST_DLMS_HDLC_Proc(PST_Frame* frame)
{
    DLMS_Frame* frm = &frame->_dlms;

    //**********************************
                                        //声明,执行DLMS判定
    uint8 ZY_DlmsTransRecvProc(uint8* buffer, uint16 length);
    if(ES_ERR_OK == ZY_DlmsTransRecvProc(frm->recv - 15, frm->len + 2 + 14 + 2))
    {
        return ES_ERR_OK;               //已被透传处理函数处理,不需要进行后续处理
    }

    //**********************************
    if(frm->cmd == 0x1E)                //discover response
    {
        uint8 Netp_DiscoUpRecvProc(uint8* data);
        Netp_DiscoUpRecvProc(frm->recv);
    }
    else
    {
        //其他帧不处理,plc通讯帧使用等待抄表法
    }
    ES_OK();
}




//完整的搜表复位帧
const uint8 dlms_scm_reset[27] =
{
    0x7E,0xA0,0x19,0x00,0x02,0xFE,0xFF,0x03,
    0x13,0xF1,0x0B,0xE6,0xE6,0x00,0x19,0x08,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x7B,0x0C,0x7E
};


//发现帧部分(新版本)
const uint8 dlms_scm_discover[15] =
{
    0x7E,0xA0,0x15,0x00,0x02,0xFE,0xFF,0x03,
    0x13,0x6B,0xBA,0xE6,0xE6,0x00,0x1D
};

const uint8 dlms_scm_tst_discover[23]=
{
   0x7E,0xA0,0x15,0x00,0x02,0xFE,0xFF,0x03,
   0x13,0x6B,0xBA,0xE6,0xE6,0x00,0x1D,0x64,
   0x00,0x19,0x00,0x00,0x53,0x99,0x7E
};
//注册帧部分
const uint8 dlms_scm_register[24] =
{
    0x7E,0xA0,0x23,0x00,0x02,0xFE,0xFF,0x03,
    0x13,0xEC,0x34,0xE6,0xE6,0x00,0x1C,0xFE,
    0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0x01
};


//读表号帧
const uint8 dlms_scm_getid[30] =
{
    0x7E,0xA0,0x1C,0x00,0x02,0xFE,0xFF,0x03,
    0x32,0xD9,0xCB,0xE6,0xE6,0x00,0xC0,0x01,
    0xC1,0x00,0x01,0x00,0x00,0x60,0x01,0x00,
    0xFF,0x02,0x00,0x89,0xA0,0x7E
};

//#ifdef DEBUG
//const uint8 systitle_examp1[8] = {0x48, 0x58, 0x45, 0x01, 0x30, 0xD1, 0x52, 0x59};
//const uint8 systitle_examp2[8] = {0x48, 0x58, 0x45, 0x01, 0x30, 0xD1, 0x52, 0x60};
//#endif

/************************************************************************
 * @function: DLMS_ClearRegisMt
 * @描述: 清空注册节点信息表
 * @参数: kpmap位图,保留置一的节点,删除置零的节点.
 * @返回:
 * @说明:
 * @作者: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @修改人:
 ************************************************************************/
void DLMS_ClearRegisMt(uint8* kpmap)
{
    uint8 bidx;
    if(kpmap == __NULL)
    {
        ClearBuffer((uint8*)&gs_dlmsMt, sizeof(gs_dlmsMt));
    }
    else
    {
        STR_And(gs_dlmsMt.eft, kpmap, gs_dlmsMt.eft, sizeof(gs_dlmsMt.eft));
        //STR_Or(gs_dlmsMt.eft, kpmap, gs_dlmsMt.eft, sizeof(gs_dlmsMt.eft));
    }
    
    if(STR_CalcBitSetNum(gs_dlmsMt.eft, DLMS_MT_MAP) >= CORE_TST_NO)//发现存储位图已满
    {
        for(bidx = 0; bidx < CORE_TST_NO; bidx++)
        {
            if(STR_IsBitClr(kpmap, bidx))//查找搜表位图是否存在空位
            {
                STR_ClrBit(gs_dlmsMt.eft, bidx);    //将存储位图清空
            }
        }
    }
    
//#ifdef DEBUG
//    MoveBuffer((uint8*)systitle_examp1, gs_dlmsMt.mt[0].systitle, 8);
//    MoveBuffer((uint8*)systitle_examp2, gs_dlmsMt.mt[1].systitle, 8);
//    gs_dlmsMt.eft[0] = 0x03;
//#endif
}

/************************************************************************
 * @function: DLMS_ClrRegisMt
 * @描述: 清除搜表档案
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: ZJC (2014/8/21)
 *-----------------------------------------------------------------------
 ************************************************************************/
void DLMS_ClrRegisMt(void)
{
    ClearBuffer((uint8*)&gs_dlmsMt,sizeof(gs_dlmsMt));  //将档案清空
    WriteFlashWithCRC(DB_MTLIST, (uint8*)&gs_dlmsMt, sizeof(gs_dlmsMt), 0);
    ClearBuffer((uint8*)&gs_scmDlms, sizeof(gs_scmDlms));
    
    //ClearBuffer((uint8*)&gs_NetpReadStEVT, sizeof(gs_NetpReadStEVT));       //将事件全部清除
    //Fram_SafeWrite((uint8 *)&gs_NetpReadStEVT, FM_EVTDT, sizeof(gs_NetpReadStEVT));   //往铁电写事件时间
}

/************************************************************************
 * @function: DLMS_SaveRegisMt
 * @描述: 保存下挂表信息列表
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/7/1)
 *-----------------------------------------------------------------------
 * @修改人: ZJC(2014-08-21)
 * @说明: 将搜表档案导入到F10表计档案
 ************************************************************************/
void DLMS_SaveRegisMt(void)
{
    bool ucNewFindFlag;
    uint8 uc_i,uc_j;
    uint8 defparaf10[LEN_TST_CFG];
    uint8 bmapsbf[LEN_TST_BMPS];
    uint8 bmaps[LEN_TST_BMPS];
    uint32 addr;
    
    gs_GPIO.LED_BlinkSet(LED_MBUS, 3, 0, 0);  //灭
    if(STR_IsEmpty(gs_dlmsMt.eft, DLMS_MT_MAP))//如果没有发现任何新表，则再次搜表
    {
        ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID);
        return;
    }
    
    gb_dlmsMtFlashCrcOk = WriteFlashWithCRC(DB_MTLIST, (uint8*)&gs_dlmsMt, sizeof(gs_dlmsMt), 0);//写搜表档案
    
    GD_Para_RW(F10_PADDR, bmaps, LEN_TST_BMPS, false);   //读抄表档案位图
    ClearBuffer(bmapsbf,LEN_TST_BMPS);
    for(uc_i = 0; uc_i < CORE_TST_NO; uc_i++)//扫描搜表档案
    {
        if(STR_IsBitSet(gs_dlmsMt.eft, uc_i) &&                                     //搜表有效
           (CmpBuffer(gs_dlmsMt.mt[uc_i].bgaddr, (uint8*)&DefParaF10[6], 6) != 0))  //搜表地址为非0
        {
            STR_SetBits(bmapsbf, 1, uc_i, 1);  //更新位图
            ucNewFindFlag = true;
            for(uc_j = 2; uc_j < CORE_TST_NO; uc_j++)//扫描抄表档案
            {
                addr = F10_PADDR + LEN_TST_BMPS + LEN_TST_CFG * uc_j;
                GD_Para_RW(addr, defparaf10, LEN_TST_CFG, false);   //读档案地址
                if(STR_IsBitSet(bmaps, uc_j) &&                                     //档案有效
                   (CmpBuffer(gs_dlmsMt.mt[uc_i].bgaddr, defparaf10 + 6, 6) == 0) )   //地址相同
                {
                    ucNewFindFlag = false;//该表的档案已经存在
                    break;
                }
            }
            if(ucNewFindFlag == true)   //发现是新表
            {
                MoveBuffer((uint8*)DefParaF10, defparaf10, LEN_TST_CFG);//导入默认参数
                MoveBuffer(gs_dlmsMt.mt[uc_i].bgaddr, defparaf10 + 6, 6);  //填充表号
                
                MoveBuffer(gs_PstPara.Addr + 2, defparaf10 + 20, 2);  //填写所属采集器通信地址
                MoveBuffer(gs_PstPara.Addr, defparaf10 + 22, 2);  
                
                if(gc_FindMtMode == 1)  //自动建立F10档案
                {
                    for(uc_j = 2; uc_j < CORE_TST_NO; uc_j++)//扫描抄表档案
                    {
                        if(STR_IsBitClr(bmaps, uc_j))   //找到空位
                        {
                            defparaf10[0] = uc_j;
                            defparaf10[2] = uc_j;
                            addr = F10_PADDR + LEN_TST_BMPS + LEN_TST_CFG * uc_j;
                            GD_Para_RW(addr, defparaf10, LEN_TST_CFG, true);   //写F10档案地址
                            ParamLoad_F10();    //载入档案信息
                            ALRT_ERC39_46(uc_j, uc_j, defparaf10, 39);   //发现新表
                            break;
                        }
                    }
                }
                else 
                {
                    ALRT_ERC39_46(9999, 9999, defparaf10, 39);   //发现新表
                }
            }
        }   
    }
    
    if(STR_IsEmpty(bmapsbf, LEN_TST_BMPS))  //档案位图为空，则再次搜表
    {
        ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID);
    }
    else
    {
        ES_Message_Send(MSG_NETP_IDATA, TASK_NETP_TKID);//搜表后再进行上电后首轮抄表
    }
}


/************************************************************************
 * @function: DLMS_LoadRegisMt
 * @描述: 载入下挂表信息列表
 * @参数: 
 * 
 * @返回: 
 * @return: bool  真:执行成功, 假:载入错误
 * @说明: 
 * @作者: yzy (2014/7/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
bool DLMS_LoadRegisMt(void)
{
    gb_dlmsMtFlashCrcOk = ReadFlashWithCRC(DB_MTLIST, (uint8*)&gs_dlmsMt, sizeof(gs_dlmsMt), 0);
    return gb_dlmsMtFlashCrcOk;
}





/************************************************************************
 * @function: DLMS_ResetRegisSend
 * @描述: 复位搜表注册状态
 * 
 * @参数: 
 * @param: pdata 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void DLMS_ResetRegisSend(void)
{
    gfs_PSTChnSend[PST_CHN_NET]((uint8*)dlms_scm_reset, sizeof(dlms_scm_reset));
}




/************************************************************************
 * @function: DLMS_DiscoverSend
 * @描述: 搜表发现帧
 * 
 * @参数:
 * @param: pdata 外部传入的发送缓存,目的在于减少栈深度
 * @param: ratio 上报概率(0-100)
 * @param: slots 上报时间片(单位1ms)
 * @返回: uint8 帧长度
 * @说明: 
 * @作者: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 DLMS_DiscoverSend(uint8* pdata, uint8 ratio, uint16 slots)
{
    uint16 crc;
    uint8 len;
                                            //参数合法性判断
    if(ratio > 100)
    {
        ratio = 100;
    }
                                            //帧头
    MoveBuffer((uint8*)dlms_scm_discover, pdata, sizeof(dlms_scm_discover));
    len = sizeof(dlms_scm_discover);
                                            //上报概率&时间片
    pdata[len++] = ratio;
    pdata[len++] = (uint8)(slots >> 8);
    pdata[len++] = (uint8)slots;
    pdata[len++] = 0;
    pdata[len++] = 0;
                                            //crc
    crc = DoCrcDlms(pdata + 1, len - 1, 0xffff);
    pdata[len++] = (uint8)crc;
    pdata[len++] = (uint8)(crc >> 8);
    pdata[len++] = 0x7E;
                                            //发送
    gfs_PSTChnSend[PST_CHN_NET](pdata, len);
                                            //返回长度
    return len;
}







/************************************************************************
 * @function: DLMS_RegisSend
 * @描述: 搜表注册帧
 * 
 * @参数:
 * @param: pdata 外部传入的发送缓存,目的在于减少栈深度
 * @param: systitle 八字节systemtitle
 * @param: saddr 逻辑地址,短地址
 * 
 * @返回: 
 * @return: uint8  帧长度
 * @说明: 
 * @作者: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 DLMS_RegisSend(uint8* pdata, uint8* systitle, uint16 saddr)
{
    uint16 crc;
    uint8 len;
                                            //帧头
    MoveBuffer((uint8*)dlms_scm_register, pdata, sizeof(dlms_scm_register));
    len = sizeof(dlms_scm_register);
                                            //数据域
    MoveBuffer(systitle, pdata + len, 8);
    len += 8;
    pdata[len++] = (uint8)(saddr >> 8);
    pdata[len++] = (uint8)saddr;
                                            //crc
    crc = DoCrcDlms(pdata + 1, len - 1, 0xffff);
    pdata[len++] = (uint8)crc;
    pdata[len++] = (uint8)(crc >> 8);
    pdata[len++] = 0x7E;
                                            //发送并重发
    gfs_PSTChnSend[PST_CHN_NET](pdata, len);
    gs_OS.TK_Sleep(100);
    gfs_PSTChnSend[PST_CHN_NET](pdata, len);
    gs_OS.TK_Sleep(100);
                                            //返回长度
    return len;
}




/************************************************************************
 * @function: DLMS_GetMtId
 * @描述: 读取表号
 * 
 * @参数:
 * @param: pdata 外部传入的发送缓存,目的在于减少栈深度
 * @param: pdmax 发送缓存长度
 * @param: saddr 输入,逻辑地址,短地址
 * @param: outaddr 输出,表号
 * 
 * @返回: 
 * @return: uint8  成功/失败
 * @说明: 举例: 回复帧 7ea0210300020005300778e6e700c40101000a0c323031343333333335353031686b7e
 *            其中,表号为 0a0c323031343333333335353031
 * @作者: yzy (2014/7/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 DLMS_GetMtId(uint8* pdata, uint8 pdmax, uint16 saddr, uint8* outaddr, uint8 trycnt)
{
    uint16 crc;
    uint16 dlen;
    uint8 tmp;
    uint16 mtidx;
                                        //发送并获取地址
    for(uint8 uc_i = 0; uc_i < trycnt; uc_i++)//重试N次
    {
                                        //移入举例帧.
        MoveBuffer((uint8*)dlms_scm_getid, pdata, sizeof(dlms_scm_getid));
                                        //替换目标地址
        mtidx = CalcDlmsSaddr(saddr);
        pdata[5] = mtidx >> 8;          //hdlc短地址
        pdata[6] = mtidx;               //hdlc短地址
                                        //校验帧头crc
        crc = DoCrcDlms(pdata + 1, 8, 0xffff);
        pdata[9] = (uint8)crc;
        pdata[10] = (uint8)(crc >> 8);
                                        //全帧crc
        tmp = pdata[2] - 15;            //倒推出数据长度
        crc = DoCrcDlms(pdata + 1, 14 + tmp - 1, 0xffff);
        pdata[14 + tmp] = (uint8)crc;
        pdata[15 + tmp] = (uint8)(crc >> 8);
        pdata[16 + tmp] = 0x7E;
    
        gs_Uart.open(NETP_PORT_NO, false);
        ES_LED_BlinkSet(LED_MBUS, 1, 0, 0);
        gs_Uart.Write(NETP_PORT_NO, pdata, sizeof(dlms_scm_getid));
        dlen = WaitForDlms(pdata, pdmax, __NULL, NETP_PORT_NO);
        gs_Uart.close(NETP_PORT_NO);
                                         //提取表号和规约
        if(dlen > 32)
        {
            //ES_LED_BlinkSet(LED_MBUS, 1, 0, 0);
            dlen -= 23;                 //提取表号字节数:减去20字节表号前数据 + 3字节crc
            if((dlen > 0) && (dlen <= 12))//表号位数可变:1-12位
            {
                if(dlen != 12)
                {
                    MoveBuffer(pdata + 20, pdata + 20 + (12 - dlen), dlen);
                    memset(pdata + 20, 0x30, (12 - dlen));
                }
                HexStringToNum(pdata + 20, outaddr, 12);
                InvBuffer(outaddr, 6);
                gs_OS.TK_Sleep(50);     //延时500ms,防止某些逗比表计被发死
                return ES_ERR_OK;
            }
            gs_OS.TK_Sleep(50);         //延时500ms,防止某些逗比表计被发死
        }
    }

    return ES_ERR_FT;

}




//组网端口硬件测试帧.
const uint8 netp_hwtest_frm1[] = 
{
    0x7E,
    0xA0,0x23,
    0x00,0x02,
    0xFE,0xFF,
    0x03,
    0x93,
    0xE4,0xB0,

    0x81,0x80,0x14,0x05,0x02,0x07,0xD0,0x06,0x02,0x07,0xD0,0x07,0x04,0x00,0x00,0x00,0x01,0x08,0x04,0x00,0x00,0x00,0x01,
    0x3A,0xF2,
    0x7E
};


const uint8 netp_hwtest_frm2[] = 
{
    0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x13, 0x00, 0xDF, 0x16
};
/************************************************************************
 * @function: Netp_HwTest
 * @描述: 组网通道硬件测试.
 * @参数: 
 * 
 * @返回: 
 * @return: uint8  ES_ERR_OK/ES_ERR_FT
 * @说明: 测试条件,下挂单块DLMS表.
 * @作者: yzy (2014/6/4)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Netp_HwTest(void)
{
    uint16 dlen;
    uint8 rbuf[64];

    //使用DLMS帧试探
    gs_Uart.open(NETP_PORT_NO, false);
    ES_LED_BlinkSet(LED_MBUS, 1, 0, 0);
    gs_Uart.Write(NETP_PORT_NO, (uint8*)netp_hwtest_frm1, sizeof(netp_hwtest_frm1));
    dlen = WaitForDlms(rbuf, sizeof(rbuf), __NULL, NETP_PORT_NO);
    gs_Uart.close(NETP_PORT_NO);

//    if(dlen)
//    {
//        ES_LED_BlinkSet(LED_RXD, 0, 0, 0);
//        return ES_ERR_OK;
//    }
//
//    //使用DLT帧试探
//    gs_Uart.open(ES_NET_PORT, false);
//    ES_LED_BlinkSet(LED_TXD, 0, 0, 0);
//    gs_Uart.Write(ES_NET_PORT, (uint8*)netp_hwtest_frm2, sizeof(netp_hwtest_frm2));
//    dlen = WaitForBg(rbuf, sizeof(rbuf), __NULL, ES_NET_PORT);
//    gs_Uart.close(ES_NET_PORT);

    if(dlen)
    {
        ES_LED_BlinkSet(LED_MBUS, 1, 0, 0);
        return ES_ERR_OK;
    }
    else
    {
        return ES_ERR_FT;
    }
    
}




/************************************************************************
//搜表上报帧,单帧长度<48字节.若按48字节一帧计算,单帧上报时间10ms. 
//即使按照最大16块表计算,随机延时2000ms应该够了. 
//搜表策略:0)清除注册状态(连续三次)
//         1)50%,2s搜表,直到无表上报.(单轮搜完后对上报表进行注册.)
//         2)100%,2s搜表,直到连续3轮无表上报.(单轮搜完后对上报表进行注册.)
 ************************************************************************/


/************************************************************************
 * @function: Netp_DiscoCheckExist
 * @描述: 搜表前先验证已注册的表是否仍然存在.
 *          (在搜表RESET前调用.若存在,则添加到搜表发现节点位图中.)
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/7/1)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_DiscoCheckExist(void)
{
    uint8 buffer[48];
    uint8 bgaddr[6];
    uint8 bidx;
    uint8 errnum = 0;                   //已存在节点连续验证失败个数

    //查找systitle是否已经有记录
    for(bidx = 0; bidx < CORE_TST_NO; bidx++)
    {
        if(STR_IsBitSet(gs_dlmsMt.eft, bidx))
        {                               //根据短地址读取表号
            if(ES_ERR_OK == DLMS_GetMtId(buffer, sizeof(buffer), bidx + DLMS_SADDR_OFFSET, bgaddr, 2))
            {                           //比对表号
                if(!CmpBuffer(bgaddr, gs_dlmsMt.mt[bidx].bgaddr, 6))
                {                       //表号一致,加入到已发现列表中(操作同 Netp_DiscoUpRecvProc())
                    STR_SetBit(gs_scmDlms.bmap, bidx);
                }
                errnum = 0;
            }
            else                        //防止现场互换采集器时,上电搜表时间过长(满负荷24块表验证一遍需要近3分钟)
            {
                errnum++;
                if(errnum >= 5)         //连续n块表验证失败,则认为原有记录不可行,停止继续验证.
                {
                    break;
                }
            }
        }
    }
}



/************************************************************************
 * @function: Netp_DiscoStart
 * @描述: 启动从节点主动注册流程
 * @参数:
 * @返回:
 * @说明:
 * @作者: yzy (2013/7/12)
 *-----------------------------------------------------------------------
 * @修改人:
 ************************************************************************/
void Netp_DiscoStart(void)
{
    scm_dlms_t* scm = &gs_scmDlms;
                                        //若正在执行搜表,则直接退出
    if(((scm->stt & SCM_STT_EXMASK) != SCM_STT_IDLE) && 
       ((scm->stt & SCM_STT_EXMASK) != SCM_STT_FIN))
    {
        return;
    }
    ES_LED_BlinkSet(LED_MBUS, 1, 0, 0);
                                        //1)
    DLMS_LoadRegisMt();                 //载入上次搜表结果
    ClearBuffer((uint8*)scm, sizeof(scm_dlms_t));
                                        //2)
    scm->stt &= ~SCM_STT_EXMASK;
    scm->stt |= SCM_STT_CKEST;
    Netp_DiscoCheckExist();             //验证之前的节点是否仍然存在,仍存在的直接作为首次搜表上报节点
                                        //3)
    DLMS_ClearRegisMt(gs_scmDlms.bmap); //清除从节点已上报标记.

                                        //4)复位所有节点的注册状态
    gs_OS.TK_Sleep(50);                 //延时500ms
    for(uint8 uc_i = 0; uc_i < 3; uc_i++)
    {
        scm->stt &= ~SCM_STT_EXMASK;
        scm->stt |= SCM_STT_RESET;
        DLMS_ResetRegisSend();
        gs_OS.TK_Sleep(200);
    }
                                        //延时启动
    scm->vcnt = 1;                      //n秒后执行
    scm->startsecs = GetSysRunSecs();
    scm->stt &= ~SCM_STT_EXMASK;
    scm->stt |= SCM_STT_DISCO;          //最后置标记位
}



/*
许葛岚 -  说: (2014-06-10 12:39:22)
罗工在么?
现在发现DLMS表搜表帧中的time slots貌似没有.
你帮忙看下,采集器下挂14块表,我设置的是100%上报概率,time slots = 200.

理论上下挂表应该200*40ms = 8s内随机上报.
实际监视发现,下挂表每次只在发送数据的前2秒内上报

许葛岚 -  说: (2014-06-10 12:39:33)
7EA0150002FEFF03136BBAE6E6001D6400C80000804C7E
这是我下发的discover帧

罗彬 -  说: (2014-06-10 15:01:18)
表实际处理的时候,只用了time slots 2个字节中的低字节
罗彬 -  说: (2014-06-10 15:04:11)
第一个字节大于0或者第2个字节大于60的时候,回复的时间间隔都控制在2s左右~
*/
/************************************************************************
 * @function: Netp_FindSecProc
 * @描述: 从节点主动注册流程控制
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/7/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_FindSecProc(void)
{
    scm_dlms_t* scm = &gs_scmDlms;
    uint8 buffer[64];
    uint16 bidx;
                                        //注册发现已结束或尚未开始,退出
    if(((scm->stt & SCM_STT_EXMASK) == SCM_STT_FIN) || 
       ((scm->stt & SCM_STT_EXMASK) == SCM_STT_IDLE))
    {
        return;
    }
                                        //等待直到延时时间结束.
    if(scm->vcnt > 0)
    {
        scm->vcnt--;
        return;
    }
                                        //判断搜表最大持续时间,超过说明存在异常情况,强制停止.
    if(GetSysRunSecs() >= (scm->startsecs + 600))
    {
        scm->stt &= ~SCM_STT_EXMASK;
        scm->stt |= (SCM_STT_FIN | SCM_STT_FIROK);
        scm->err = 0xFF;
        DLMS_SaveRegisMt();
        return;
    }
                                        //根据上轮搜表结果进行操作
    bidx = STR_GetFirstSetBit(scm->bmap, sizeof(scm->bmap));
    if(bidx == 0xFFFF)                  //无待注册表
    {                                   //100%概率下,连续5次无上报,则认为完成.
        if((scm->ratio == 100) && (scm->nofd >= 5))
        {
            scm->stt &= ~SCM_STT_EXMASK;
            scm->stt |= (SCM_STT_FIN | SCM_STT_FIROK);
            DLMS_SaveRegisMt();
            return;
        }
                                        //1)增加一次
        scm->nofd++;
                                        //2)设置discover参数
        if(scm->ratio >= 100)           //概率到百分百,不再增加
        {
            scm->ratio = 100;
        }
        else if(scm->ratio == 0)        //概率为零,则初始化概率
        {
            scm->ratio = 10;
            scm->nofd = 0;              //切
        }
        else                            //概率在0-100之间
        {
            //快速方案
            if(scm->nofd >= 2) //本轮未接收到数据,&||,连续n次未收到有效帧时,增加概率
            {
                scm->ratio += 20;       //>100没关系,会自动限定在100
                scm->nofd = 0;          //切
            }
            
            
            /*//稳健方案
            if(scm->nofd >= 3) //本轮未接收到数据,&||,连续n次未收到有效帧时,增加概率
            {
                if(scm->ratio < 70)
                {
                    scm->ratio += 20;
                }
                else
                {
                    scm->ratio += 10;
                }
                scm->nofd = 0;          //切
            }*/
            
        }
        
        scm->slots = 50;                //50*40=2000ms
        scm->vcnt = 3;
        scm->ndnum = 0;
        scm->revd = 0;
        ClearBuffer(scm->bmap, sizeof(scm->bmap));
                                        //发送
        scm->stt &= ~SCM_STT_EXMASK;
        scm->stt |= SCM_STT_DISCO;
        DLMS_DiscoverSend(buffer, scm->ratio, scm->slots);
    }
    else                                //有待注册表
    {
        scm->stt &= ~SCM_STT_EXMASK;
        scm->stt |= SCM_STT_REGIS;
                                        //注册操作
        DLMS_RegisSend(buffer, gs_dlmsMt.mt[bidx].systitle, bidx + DLMS_SADDR_OFFSET);  //逻辑地址从2开始分配.
                                        //使用注册的逻辑地址读取表号
        if(ES_ERR_OK != DLMS_GetMtId(buffer, sizeof(buffer), bidx + DLMS_SADDR_OFFSET, gs_dlmsMt.mt[bidx].bgaddr, 3))
        {                               //错误,很可能是表串口被发死,或者MBUS问题
            MoveBuffer((uint8*)err_addr, gs_dlmsMt.mt[bidx].bgaddr, 6);
        }

        gs_OS.TK_Sleep(50);             //延时500ms
                                        //后续操作
        scm->vcnt = 2;                  //设置register等待延时
        scm->nofd = 0;
        STR_ClrBit(scm->bmap, bidx);    //清位图
    }

}



/************************************************************************
 * @function: Netp_DiscoUpRecvProc
 * @描述: 接收到主动上报发现节点时的处理函数
 * 
 * @参数: 
 * @param: data 数据域(DLMS控制字之后的数据) 
 * 
 * @返回: 
 * @return: uint8  成功/失败
 * @说明: 
 * @作者: yzy (2013/7/13)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Netp_DiscoUpRecvProc(uint8* data)
{
    scm_dlms_t* scm = &gs_scmDlms;
    uint16 bidx;
    uint8 isExist = 0;
    
    //查找systitle是否已经有记录
    for(bidx = 0; bidx < CORE_TST_NO; bidx++)
    {
        if(STR_IsBitSet(gs_dlmsMt.eft, bidx))
        {
            if(0 == CmpBuffer(gs_dlmsMt.mt[bidx].systitle, data + 1, 8))
            {                   //已经存在,跳出循环
                isExist = 1;
                break;
            }
        }
    }
    
    //若没有记录,填充
    if(!isExist)
    {
        bidx = STR_GetFirstClrBit(gs_dlmsMt.eft, DLMS_MT_MAP);
        if(bidx == 0xFFFF)
        {
            return ES_ERR_FT;
        }
                                //赋值systitle及有效标志
        MoveBuffer(data + 1, gs_dlmsMt.mt[bidx].systitle, 8);
        MoveBuffer((uint8*)err_addr, gs_dlmsMt.mt[bidx].bgaddr, 6);//无效
        STR_SetBit(gs_dlmsMt.eft, bidx);//注意:此时虽然置位有效,但bgaddr中为全零,非实际值
    }
    else
    {
        //异常状态:已经被注册的表,再次上报.
        //可能原因1)表异常,丢失注册状态,2)表串口异常,搜到systitle后的注册帧收不到,3)MBUS线路异常
    }
                                //赋值抄表状态有效标志
    STR_SetBit(scm->bmap, bidx);
    ES_OK();
}



/************************************************************************
 * @function: Netp_DiscoSetRecvFlag
 * @描述: 发现过程中接收到MBUS数据时置接收标志位
 * 
 * @参数: 
 * @param: 
 * 
 * @返回: 
 * @return: uint8  成功/失败
 * @说明: 
 * @作者: yzy (2013/7/13)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void Netp_DiscoSetRecvFlag(void)
{
    scm_dlms_t* scm = &gs_scmDlms;
    if((scm->stt & SCM_STT_EXMASK) == SCM_STT_DISCO)
    {
        if(scm->revd != 0xff)
        {
            scm->revd++;
        }
    }
}






/*******************************************************************************
 * @function_name:  ES_Netp_Init
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
void ES_Netp_Init(void)
{
                                        //初始化波特率
    SerialSets ss;
    ss.baudrate = NETP_PORT_BAUD;
    ss.parit = Parit_N;
    ss.databits = DataBits_8bits;
    ss.stopbits = StopBits_1;
    gs_Uart.Init(NETP_PORT_NO, &ss);
                                        //清除简单全局变量
    gul_noPlcReadSecs = 0;
                                        //清除全局结构体变量
    //DLMS_ClearRegisMt(__NULL);
    DLMS_LoadRegisMt();
    ClearBuffer((uint8*)&gs_scmDlms, sizeof(gs_scmDlms));
    
    GD_Para_RW(F241_PADDR, &gc_FindMtMode, 1, false);  //

    ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID); //搜表
}



/*******************************************************************************
 * @function_name:  Netp_MbusPowerOnDelay
 * @function_file:  netp.c
 * @描述: 等待Mbus电源稳定
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
void Netp_MbusPowerOnDelay(void)
{
    gs_GPIO.GPO_Out(GPO_MBUS_PWR, true);
    gs_OS.TK_Sleep(200);                //试验发现,只延时1s的情况下第一帧接收会异常.
}






