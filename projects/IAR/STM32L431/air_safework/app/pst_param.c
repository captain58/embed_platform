/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: pst.c
**创   建   人: yzy
**最后修改日期: 2013年7月3日
**描        述: 1376.2参数载入部分
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2013年7月3日
** 描　述: 原始版本
**-------------------------------------------------------------------------------------------------------
** 修改人: 
** 版  本: 
** 描  述: 
********************************************************************************************************/









/************************************************************************ 
  *参数存储结构体
************************************************************************/
typedef struct
{
    uint8  paralen;             //长度(重复)
    uint8  paralenbk;           //长度(重复)
    uint16 crc;                 //整体CRC在尾部
    uint8  darray1;             //第一个字节数据内容...
    //uint8 darray2~darrayn;
    
}shortParaHeader_t;             //简单参数存储结构头






/************************************************************************
 * @function: WriteShortPara
 * @描述: 写入简单参数
 * 
 * @参数: 
 * @param: dbno 数据块
 * @param: addr 数据在数据块中的起始地址
 * @param: pdata 数据缓存
 * @param: plen 数据长度
 * 
 * @返回: 
 * @return: uint8 SYS_ERR_OK/SYS_ERR_FT
 * @说明: 
 * @作者: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 WriteShortPara(uint8 dbno, uint32 addr, uint8* pdata, uint8 plen)
{
    uint8 temp[8];
    shortParaHeader_t* hdr = (void*)temp;
                                            //写入参数数据域
//    if(SYS_ERR_OK != WriteFlash(dbno, pdata, plen, addr + 4))
//    {
//        return SYS_ERR_FT;
//    }
                                            //写入4字节头
    hdr->paralen = plen;
    hdr->paralenbk = plen;
    hdr->crc = DoCrc16(pdata, plen, 0x5555);
//    if(SYS_ERR_OK != WriteFlash(dbno, (uint8*)hdr, 4, addr))
//    {
//        return SYS_ERR_FT;
//    }
    SYS_OK();
}




/************************************************************************
 * @function: ReadShortPara
 * @描述: 读取简单参数
 * 
 * @参数: 
 * @param: dbno 数据块
 * @param: addr 数据在数据块中的起始地址
 * @param: pbuff   数据缓存
 * @param: bufflen 数据的最大允许长度
 * 
 * @返回: 
 * @return: 0 读取失败. others 读取的数据长度
 * @说明: 读取失败时,会默认将缓存清零
 * @作者: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 ReadShortPara(uint8 dbno, uint32 addr, uint8* pbuff, uint8 bufflen)
{
    uint16 crc;
    uint8 temp[8];
    shortParaHeader_t* hdr = (void*)temp;

    if(SYS_ERR_OK == ReadFlash(dbno, (uint8*)hdr, sizeof(shortParaHeader_t), addr))
    {
        if((hdr->paralen != hdr->paralenbk) || (hdr->paralen > bufflen))
        {
            memset(pbuff, 0, bufflen);
            return 0;
        }
        
        if(SYS_ERR_OK == ReadFlash(dbno, pbuff, hdr->paralen, addr + 4))
        {
            crc = DoCrc16(pbuff, hdr->paralen, 0x5555);
            if(hdr->crc == crc)
            {
                return hdr->paralen;
            }
        }
    }
    return 0;
}



/************************************************************************
 * @function: CrackShortPara
 * @描述: 清空简单参数
 * 
 * @参数: 
 * @param: dbno 数据块
 * @param: addr 数据在数据块中的起始地址
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void CrackShortPara(uint8 dbno, uint32 addr)
{
    uint8 temp[8];
    shortParaHeader_t* hdr = (void*)temp;
                                //置无效数据
    memset(temp, 0xff, sizeof(temp));
                                //写入
//    WriteFlash(dbno, (uint8*)hdr, 4, addr);
}



/************************************************************************
 * @function: GD_Para_RW
 * @描述: 读或写flash参数
 * 
 * @参数: 
 * @param: addr flash偏移地址
 * @param: buff 读写缓存
 * @param: len 需要读写的长度
 * @param: rw false读; true写
 * 
 * @返回: 
 * @return: uint8  SYS_ERR_OK/SYS_ERR_FT
 * @说明: 
 * @作者: yzy (2014/2/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 GD_Para_RW(uint32 addr, uint8* buff, uint16 len, bool rw)
{
    if(rw)
    {
        if(SYS_ERR_OK == WriteFlash(DB_PARA, buff, len, addr))
        {
            SYS_OK();
        }
    }
    else
    {
        if(SYS_ERR_OK == ReadFlash(DB_PARA, buff, len, addr))
        {
            SYS_OK();
        }
    }
    
    return SYS_ERR_FT;
}

/************************************************************************
 * @function: RUN_Para_RW
 * @描述: 读或写flash参数
 * 
 * @参数: 
 * @param: addr flash偏移地址
 * @param: buff 读写缓存
 * @param: len 需要读写的长度
 * @param: rw false读; true写
 * 
 * @返回: 
 * @return: uint8  SYS_ERR_OK/SYS_ERR_FT
 * @说明: 
 * @作者: yzy (2014/2/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 RUN_Para_RW(uint32 addr, uint8* buff, uint16 len, bool rw)
{
//    if(rw)
//    {
//        if(SYS_ERR_OK == WriteFlash(DB_RUN_PARA, buff, len, addr))
//        {
//            SYS_OK();
//        }
//    }
//    else
//    {
//        if(SYS_ERR_OK == ReadFlash(DB_RUN_PARA, buff, len, addr))
//        {
//            SYS_OK();
//        }
//    }
    
    return SYS_ERR_FT;
}


//	uint8_t g_tmp_netpPara[2048];
uint8 Netp_Framework_RW(uint32 addr, uint8* buff, uint16 len, bool rw)
{
//    if(rw)
//    {
////        memcpy(g_tmp_netpPara+addr, buff, len);
//        if(SYS_ERR_OK == WriteFlash(DB_PARA_NETP, buff, len, addr))
//        {
//            SYS_OK();
//        }
//    }
//    else
//    {
////        memcpy(buff, g_tmp_netpPara+addr, len);
//        if(SYS_ERR_OK == ReadFlash(DB_PARA_NETP, buff, len, addr))
//        {
//            SYS_OK();
//        }
//    }
    
    return SYS_ERR_FT;
}


/************************************************************************
 * @function: GD_ParaShort_W
 * @描述: 带校验及长度信息的短参数的写
 * 
 * @参数: 
 * @param: addr 参数地址
 * @param: buff 数据缓存(__NULL表示删除)
 * @param: len  写入数据的长度(=参数长度)
 * 
 * @返回: 
 * @return: uint8  SYS_ERR_OK/SYS_ERR_FT
 * @说明: 需确保该地址起始存储的是短参数
 * @作者: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 GD_ParaShort_W(uint32 addr, uint8* buff, uint8 len)
{
    return WriteShortPara(DB_PARA, addr, buff, len);
}



/************************************************************************
 * @function: GD_ParaShort_R
 * @描述: 带校验及长度信息的短参数的读
 * 
 * @参数: 
 * @param: fn  参数标识
 * @param: buff 数据缓存
 * @param: len  数据缓存长度(>=参数长度)
 * 
 * @返回: 
 * @return: uint8  0 读取失败. others 读取的数据长度
 * @说明: 需确保该地址起始存储的是短参数
 * @作者: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 GD_ParaShort_R(uint32 addr, uint8* buff, uint8 len)
{
    return ReadShortPara(DB_PARA, addr, buff, len);
}

#if 0

/************************************************************************
 * @function: GD_ParaShort_Rst
 * @描述: 带校验及长度信息的短参数的复位
 * 
 * @参数: 
 * @param: addr 
 * @返回: 
 * @说明: 需确保该地址起始存储的是短参数
 * @作者: yzy (2014/3/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void GD_ParaShort_Rst(uint32 addr)
{
    CrackShortPara(DB_PARA, addr);
}




/************************************************************************
 * @function: GD_ParaShort_W
 * @描述: 带校验及长度信息的短参数的写
 * 
 * @参数: 
 * @param: fn  参数标识
 * @param: buff 数据缓存(__NULL表示删除)
 * @param: len  写入数据的长度(=参数长度)
 * 
 * @返回: 
 * @return: uint8  SYS_ERR_OK/SYS_ERR_FT
 * @说明: 
 * @作者: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 GD_ParaShort_Fn_W(uint16 fn, uint8* buff, uint8 len)
{
    const S_PARAINFO* pinfo;
                                        //查找参数地址
    pinfo = GetParaInfo(fn);
    if(pinfo == __NULL)
    {
        return SYS_ERR_FT;
    }
                                        //判断是否为短地址参数
    if((pinfo->type & PARA_ORG_MASK) != PARA_ORG_ST)
    {
        return SYS_ERR_FT;
    }

    if(__NULL == buff)                  //删除
    {
        CrackShortPara(DB_PARA, pinfo->addr);
    }
    else
    {                                   //长度必须一致
        if(len != pinfo->len)
        {
            return SYS_ERR_FT;
        }
    }
                                        //执行读写操作
    return WriteShortPara(DB_PARA, pinfo->addr, buff, pinfo->len);
}




/************************************************************************
 * @function: GD_ParaShort_Fn_R
 * @描述: 带校验及长度信息的短参数的读
 * 
 * @参数: 
 * @param: fn  参数标识
 * @param: buff 数据缓存
 * @param: len  数据缓存长度(>=参数长度)
 * 
 * @返回: 
 * @return: uint8  0 读取失败. others 读取的数据长度
 * @说明: 
 * @作者: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 GD_ParaShort_Fn_R(uint16 fn, uint8* buff, uint8 len)
{
    const S_PARAINFO* pinfo;
    uint8 rdlen;
                                        //查找参数地址
    pinfo = GetParaInfo(fn);
    if(pinfo == __NULL)
    {
        return 0;
    }
                                        //判断是否为短地址参数
    if((pinfo->type & PARA_ORG_MASK) != PARA_ORG_ST)
    {
        return 0;
    }
                                        //缓存长度只可以大,不可以小
    if(len < pinfo->len)
    {
        return 0;
    }
                                        //执行读写操作
    rdlen = ReadShortPara(DB_PARA, pinfo->addr, buff, len);
    if((rdlen == 0) || (rdlen != pinfo->len))  //读取失败,或参数长度不一致
    {
        return 0;
    }

    return pinfo->len;

}






/************************************************************************
 * @function: GD_ParaShort_Fn_Rst
 * @描述: 带校验及长度信息的短参数的清除
 * 
 * @参数: 
 * @param: fn 参数标识
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/3/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 GD_ParaShort_Fn_Rst(uint16 fn)
{
    const S_PARAINFO* pinfo;
                                        //查找参数地址
    pinfo = GetParaInfo(fn);
    if(pinfo == __NULL)
    {
        return SYS_ERR_FT;
    }
                                        //判断是否为短地址参数
    if((pinfo->type & PARA_ORG_MASK) != PARA_ORG_ST)
    {
        return SYS_ERR_FT;
    }
                                        //执行读写操作
    CrackShortPara(DB_PARA, pinfo->addr);
    SYS_OK();

}




/************************************************************************
 * @function: GD_ParaDispatch_W
 * @描述: 分点存储类参数的写
 * 
 * @参数: 
 * @param: addr  参数地址
 * @param: buff  写数据缓存(__NULL表示删除该点)
 * @param: dplen 需要写的长度(=参数长度)
 * @param: dpidx 存储点序号(0-)
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 需确保该地址起始存储类型正确
 * @作者: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 GD_ParaDispatch_W(uint32 addr, uint8* buff, uint8 dplen, uint16 dpidx)
{
    uint8 bmps[32];
    uint32 data_addr;
                                        //传入索引超出位图范围
    if(dpidx >= (sizeof(bmps) * 8))
    {
        return SYS_ERR_FT;
    }
                                        //从FLASH读取位图
    if(GD_Para_RW(addr, bmps, sizeof(bmps), false) != SYS_ERR_OK)
    {
        return SYS_ERR_FT;
    }

    if(__NULL == buff)                  //删除
    {
        if(STR_IsBitClr(bmps, dpidx))   //已经不存在
        {
            SYS_OK();
        }
                                        //更新位图
        STR_ClrBit(bmps, dpidx);
    }
    else                                //设置
    {
        data_addr = addr + sizeof(bmps) + dplen * dpidx;
        GD_Para_RW(data_addr, buff, dplen, true);
                                        //更新位图
        STR_SetBit(bmps, dpidx);
    }
                                        //位图存入FLASH
    GD_Para_RW(addr, bmps, sizeof(bmps), true);
    SYS_OK();
}




/************************************************************************
 * @function: GD_ParaDispatch_R
 * @描述: 分点存储类参数的读
 * 
 * @参数: 
 * @param: addr 参数地址
 * @param: buff 读数据缓存
 * @param: dplen 需要读的长度(=参数长度)
 * @param: dpidx 存储点序号(0-)
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 需确保该地址起始存储类型正确
 * @作者: yzy (2014/3/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 GD_ParaDispatch_R(uint32 addr, uint8* buff, uint8 dplen, uint16 dpidx)
{
    uint8 bmps[32];
    uint32 data_addr;
                                        //传入索引超出位图范围
    if(dpidx >= (sizeof(bmps) * 8))
    {
        return SYS_ERR_FT;
    }
                                        //从FLASH读取位图
    if(GD_Para_RW(addr, bmps, sizeof(bmps), false) != SYS_ERR_OK)
    {
        return SYS_ERR_FT;
    }
                                        //该存储点不存在
    if(STR_IsBitClr(bmps, dpidx))
    {
        return SYS_ERR_FT;
    }

    data_addr = addr + sizeof(bmps) + dplen * dpidx;
    return GD_Para_RW(data_addr, buff, dplen, false);
}





/************************************************************************
 * @function: GD_ParaDispatch_Rst
 * @描述: 分点存储类参数的复位清除
 * 
 * @参数: 
 * @param: addr 参数地址
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 需确保该地址起始存储类型正确
 * @作者: yzy (2014/3/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 GD_ParaDispatch_Rst(uint32 addr)
{
    uint8 bmps[32];
                                        //位图存入FLASH
    memset(bmps, 0, sizeof(bmps));
    GD_Para_RW(addr, bmps, sizeof(bmps), true);
    SYS_OK();
}







/************************************************************************
 * @function: GD_ParaDispatch_Fn_W
 * @描述: 分点存储类参数的写
 * 
 * @参数: 
 * @param: fn 参数标识
 * @param: buff 写数据缓存(__NULL表示删除该点)
 * @param: len  需要写的长度(<=参数长度)
 * @param: dpidx 存储点序号(0-)
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 GD_ParaDispatch_Fn_W(uint16 fn, uint8* buff, uint8 len, uint16 dpidx)
{
    const S_PARAINFO* pinfo;
    uint8 bmps[32];
    uint32 addr;
                                        //传入索引超出位图范围
    if(dpidx >= (sizeof(bmps) * 8))
    {
        return SYS_ERR_FT;
    }
                                        //查找参数地址
    pinfo = GetParaInfo(fn);
    if(pinfo == __NULL)
    {
        return SYS_ERR_FT;
    }
                                        //判断是否为分点存储参数
    if((pinfo->type & PARA_ORG_MASK) != PARA_ORG_DP)
    {
        return SYS_ERR_FT;
    }
                                        //从FLASH读取位图
    if(GD_Para_RW(pinfo->addr, bmps, sizeof(bmps), false) != SYS_ERR_OK)
    {
        return SYS_ERR_FT;
    }

    if(__NULL == buff)                  //删除
    {
        if(STR_IsBitClr(bmps, dpidx))   //已经不存在
        {
            SYS_OK();
        }
                                        //更新位图
        STR_ClrBit(bmps, dpidx);
    }
    else                                //设置
    {
        if(len > pinfo->len)            //写入数据长度不得大于实际参数长度
        {
            return SYS_ERR_FT;
        }
                                        //写入数据
        addr = pinfo->addr + sizeof(bmps) + pinfo->len * dpidx;
        GD_Para_RW(addr, buff, len, true);
                                        //更新位图
        STR_SetBit(bmps, dpidx);
    }
                                        //位图存入FLASH
    GD_Para_RW(pinfo->addr, bmps, sizeof(bmps), true);
    SYS_OK();
}







/************************************************************************
 * @function: GD_ParaDispatch_Fn_R
 * @描述: 分点存储类参数的读
 * 
 * @参数: 
 * @param: fn 参数标识
 * @param: buff 读数据缓存
 * @param: len  需要读的长度(与参数长度无关)
 * @param: dpidx 存储点序号(0-)
 * 
 * @返回: 
 * @return: uint8  SYS_ERR_FT/SYS_ERR_OK
 * @说明: 
 * @作者: yzy (2014/3/11)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 GD_ParaDispatch_Fn_R(uint16 fn, uint8* buff, uint8 len, uint16 dpidx)
{
    const S_PARAINFO* pinfo;
    uint8 bmps[32];
    uint32 addr;
                                        //传入索引超出位图范围
    if(dpidx >= (sizeof(bmps) * 8))
    {
        return SYS_ERR_FT;
    }
                                        //查找参数地址
    pinfo = GetParaInfo(fn);
    if(pinfo == __NULL)
    {
        return SYS_ERR_FT;
    }
                                        //判断是否为分点存储参数
    if((pinfo->type & PARA_ORG_MASK) != PARA_ORG_DP)
    {
        return SYS_ERR_FT;
    }
                                        //从FLASH读取位图
    if(GD_Para_RW(pinfo->addr, bmps, sizeof(bmps), false) != SYS_ERR_OK)
    {
        return SYS_ERR_FT;
    }
                                        //该存储点不存在
    if(STR_IsBitClr(bmps, dpidx))
    {
        return SYS_ERR_FT;
    }

    addr = pinfo->addr + sizeof(bmps) + pinfo->len * dpidx;
    return GD_Para_RW(addr, buff, len, false);
}




/************************************************************************
 * @function: GD_ParaDispatch_Fn_Rst
 * @描述: 分点存储类参数的复位清除
 * 
 * @参数: 
 * @param: fn 参数标识
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/3/12)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 GD_ParaDispatch_Fn_Rst(uint16 fn)
{
    const S_PARAINFO* pinfo;
    uint8 bmps[32];
                                        //查找参数地址
    pinfo = GetParaInfo(fn);
    if(pinfo == __NULL)
    {
        return SYS_ERR_FT;
    }
                                        //判断是否为分点存储参数
    if((pinfo->type & PARA_ORG_MASK) != PARA_ORG_DP)
    {
        return SYS_ERR_FT;
    }
                                        //位图存入FLASH
    memset(bmps, 0, sizeof(bmps));
    GD_Para_RW(pinfo->addr, bmps, sizeof(bmps), true);
    SYS_OK();
}


#endif


