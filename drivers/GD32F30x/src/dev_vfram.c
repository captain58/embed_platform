/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: dev_vfram.h
**创   建   人: yzy
**最后修改日期: 2010年2月20日
**描        述: 设备flash虚拟铁电
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2010年2月20日
** 描　述: 原始版本
********************************************************************************************************/
#define EXT_DEV_VFRAM
#include "sys.h"
#include "hal.h"
#include "bsp.h"





#if SYS_FLASH_FRAM_EN > 0
/*
//principal: 
//we use 4k flash to emulate 32byte fram. In which,from bottom to end, 
//0-18 for bitmap, 
//256-512 for crc, 
//512-end for data(1 item use 4096B flash, thus one sector could rewrite 112 = (4096 - 512)/32 times without erase).
//total contain 112 items, 32 bytes one item.
*/

typedef struct
{                           //若bit_n=1,表示index[n]未被操作,即数字为0xff;
    Word16 indexgrp;        //若bit_n=0,表示index[n]已被操作,数值范围{0xfe,0xfc,0xf8,0xf0,0xe0,0xc0,0x80,0x00};
    uint8  index[16];
}flsTemIdx;                             //索引结构体


//在位图组中,1表示该组位图全部未被使用(即0xFF).
//find first set bit. zero is no bit set, thus be assumed as 8
//const uint8 LSBMapTbl[256] =
//{
//    8, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x00 to 0x0F                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x10 to 0x1F                             */
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x20 to 0x2F                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x30 to 0x3F                             */
//    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x40 to 0x4F                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x50 to 0x5F                             */
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x60 to 0x6F                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x70 to 0x7F                             */
//    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x80 to 0x8F                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x90 to 0x9F                             */
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xA0 to 0xAF                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xB0 to 0xBF                             */
//    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xC0 to 0xCF                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xD0 to 0xDF                             */
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xE0 to 0xEF                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xF0 to 0xFF                             */
//};



/************************************************************************
 * @function: VFRAM_Search
 * @描述: 根据位图和当前操作的条目查找欲写入的位置
 * 
 * @参数: 
 * @param: address 单个条目块位于flash的首地址
 * 
 * @返回: 
 * @return: uint8  数据内容位于条目块中的存储位置索引
 * @说明:
 * @作者: yzy (2011-2-25)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 VFRAM_Search(uint32 address)
{
    uint8 uc_i;
    uint8 tmp;
    uint8 result;
    flsTemIdx temindex;
                                        //读出索引
    if(FLS_Read((uint8*)&temindex, address, sizeof(temindex)) != SYS_ERR_OK)
    {
        return 0xFF;
    }
    
    if(0xFFFF == temindex.indexgrp.word)//从未操作过(出厂系统初始化后的状态)
    {
        return 0;
    }
                                        //计算出首个无效位在位图中所处的字节索引
    if(temindex.indexgrp.byte[0] == 0)  //index[0]-index[7]已被操作(但不一定全部为零)
    {
        uc_i = 8 - 1;
        tmp = temindex.indexgrp.byte[1];
    }
    else                                //
    {
        uc_i = (uint8)(0 - 1);
        tmp = temindex.indexgrp.byte[0];
    }
    
    /*
    //为节省程序空间,该段代码由以下代码替换
    uc_i += LSBMapTbl[tmp];
                                            //返回无效位的位索引
    return (uc_i * 8) + LSBMapTbl[temindex.index[uc_i]];
    //为节省程序空间,该段代码由以下代码替换
    */
    uc_i += ((tmp == 0) ? 8 : Bit_Maps[tmp]);
                                        //返回无效位的位索引
    result = uc_i << 3;
    result += ((temindex.index[uc_i] == 0) ? 8 : Bit_Maps[temindex.index[uc_i]]);
    
    return result;
    

    
    /*
    //例:若temindex.indexgrp.byte[0]=0,byte[1]=0xFF,且temindex.index[7]=0;
    //      则对应tmp=0xFF,uc_i=7+0=7,对应index_1st=56+8=64.
    //例:若temindex.indexgrp.byte[0]=0,byte[1]=0xFF,且temindex.index[7]=0xfe;
    //      则对应tmp=0xFF,uc_i=7+0=7,对应index_1st=56+1=57.
    //例:若temindex.indexgrp.byte[0]=0xfe,byte[1]=0xFF,且temindex.index[1]=0x80;
    //      则对应tmp=0xfe,uc_i=-1+1=0,对应index_1st=0+7=7.
    */
}



/************************************************************************
 * @function: VFRAM_Reset
 * @描述: 出厂初始化flash-fram
 * 
 * @参数: 
 * @param: from_addr 起始flash地址,对于虚拟铁电为 SA_FLASH_FRAM_ADDR
 * @param: sectornum 要擦除的扇区数量,对于虚拟铁电为((SA_FLASH_FRAM_ADDREND - SA_FLASH_FRAM_ADDR) / 4096)
 * 
 * @返回: 
 * @return: uint8 成功或失败
 * @说明:
 * @作者: yzy (2011-2-25)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 VFRAM_Reset(uint32 from_addr, uint32 sectornum)
{
    return FLS_Erase(from_addr, sectornum);
}


/************************************************************************
 *铁电有效位
 *其实是 Bit_Mask8[]的取反
 ************************************************************************/
const uint8 gucs_FramMarkBits[8] = {0xfe,0xfc,0xf8,0xf0,0xe0,0xc0,0x80,0x00};


/************************************************************************
 * @function: VFRAM_WriteMark
 * @描述: 将index指定的条目成员索引位清零
 * 
 * @参数: 
 * @param: index 当前条目在flash中的索引(0-?)
 * @param: addr 当前条目所对应flash的首地址(4K对齐)
 * 
 * @返回: 
 * @return: bool  
 * @说明:   举例:当index=0时,写入的flsTemIdx.indexgrp=0xfe,flsTemIdx.index[0]=0xfe.
 *          举例:当index=7时,写入的flsTemIdx.indexgrp=0xfe,flsTemIdx.index[0]=0.
 *          举例:当index=8时,写入的flsTemIdx.indexgrp=0xfc,flsTemIdx.index[0]=0,flsTemIdx.index[1]=0xfe.
 * @作者: yzy (2011-2-25)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
bool VFRAM_WriteMark(uint8 index, uint32 addr)
{
    uint8 uc_y, uc_x;
    uc_y = (uint8)(index >> 3);         //找出索引字节
    uc_x = (uint8)(index & 0x07);       //找出具体索引字节的具体位
    
    if(uc_x == 0)                       //开始一个新的索引字节时,对应的组位图字节相应位置位
    {
        FLS_PreWrite();                 //写入组位图字节.
        FLS_WritePage(
                 (uint8*)gucs_FramMarkBits + (uc_y & 7), //数据内容
                 addr + (uc_y >> 3) ,                    //目标地址
                 1);                                     //数据长度
    }
                                        //写入成员位图字节
    FLS_PreWrite();
    FLS_WritePage(
        (uint8*)gucs_FramMarkBits + uc_x,               //数据内容
        addr + 2 + uc_y,                                //目标地址
        1);                                             //数据长度
    SYS_OK();
}


/************************************************************************
 * @function: SYS_VFRAM_WriteSample
 * @描述: 向一个条目中写入数据
 * 
 * @参数: 
 * @param: data 欲写入的内容
 * @param: num 铁电条目编号
 * @param: finfo 条目信息结构体
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2011-2-25)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_VFRAM_WriteSample(uint8* data, uint16 num, const TFlsCycleConst* finfo)
{
    bool es;                                //是否执行擦除操作
    uint32 address;
    uint16 crc;
    uint8 index;
    uint8 err;
                                            //参数验证
    SYS_VAR_CHECK(num >= finfo->keyno);
    
    err = 0;                                //错误次数
    crc = DoCrcDlms(data, finfo->itm_baby_byte, 0x5555);//对输入的数据进行校验

    //===========================================================
                                            //找到索引地址
    address = finfo->fls_addr + (num * finfo->itm_sectors * 4096);
    if(address >= finfo->fls_addr_bk)
    {
        return SYS_ERR_VAR;
    }

    index = VFRAM_Search(address);        //根据位图找当前应该写入的位置
    if(index >= finfo->itm_baby_num)
    {
        FLS_Erase(address, finfo->itm_sectors);
        index = 0;
    }
                                            //写入铁电数据
    es = false;
    FLS_PreWrite();
    if(FLS_WritePage(data, address + 512 + index * finfo->itm_baby_byte, finfo->itm_baby_byte) == SA_ERR_OK)
    {
        VFRAM_WriteMark(index, address);
                                            //写入当前数据块的CRC校验
        FLS_PreWrite();                     //预操作
        if(FLS_WritePage((uint8*)&crc, address + 256 + index * 2, 2) != SA_ERR_OK)
        {
            es = true;
        }
    }
    else
    {
        es = true;
    }
    
    if(es)
    {
        FLS_Erase(address, finfo->itm_sectors);
        index = 0;

        FLS_PreWrite();
        FLS_WritePage(data, address + 512 + index * finfo->itm_baby_byte, finfo->itm_baby_byte);
        VFRAM_WriteMark(index, address);
        FLS_PreWrite();                     //预操作
        FLS_WritePage((uint8*)&crc, address + 256 + index * 2, 2);
        err++;                              //错误次数+1
    }
    
    //===========================================================
    if(finfo->fls_addr_bk == finfo->fls_addr_end)//无备份区
    {
        if(err)
        {
            return SYS_ERR_FT;              //返回发生致命错误      
        }
        SYS_OK();
    }
    
    //===========================================================
                                            //找到索引地址
    address = finfo->fls_addr_bk + (num * finfo->itm_sectors * 4096);
    if(address >= finfo->fls_addr_end)
    {
        return SYS_ERR_VAR;
    }

    index = VFRAM_Search(address);
    if(index >= finfo->itm_baby_num)
    {
        FLS_Erase(address, finfo->itm_sectors);
        index = 0;
    }
                                            //写入铁电数据
    es = false;
    FLS_PreWrite(); 
    if(FLS_WritePage(data, address + 512 + index * finfo->itm_baby_byte, finfo->itm_baby_byte) == SA_ERR_OK)
    {
        VFRAM_WriteMark(index, address);
                                            //写入当前数据块的CRC校验
        FLS_PreWrite();                     //预操作
        if(FLS_WritePage((uint8*)&crc, address + 256 + index * 2, 2) != SA_ERR_OK)
        {
            es = true;
        }
    }
    else
    {
        es = true;
    }
    
    if(es)
    {
        FLS_Erase(address, finfo->itm_sectors);
        index = 0;

        FLS_PreWrite();
        FLS_WritePage(data, address + 512 + index * finfo->itm_baby_byte, finfo->itm_baby_byte);
        VFRAM_WriteMark(index, address);
        FLS_PreWrite();                     //预操作
        FLS_WritePage((uint8*)&crc, address + 256 + index * 2, 2);
        err++;                              //错误次数+1
    }
    
    //===========================================================
    if(err >= 2)
    {
        return SYS_ERR_FT;              //返回发生致命错误      
    }
    SYS_OK();
}


/************************************************************************
 * @function: SYS_VFRAM_ReadSample
 * @描述: 读取一个条目数据
 * 
 * @参数: 
 * @param: data 一个条目,大小至少为单个条目的字节长度,否则会导致缓存溢出
 * @param: num 条目编号
 * @param: finfo 条目信息结构体
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/11/2)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_VFRAM_ReadSample(uint8* data, uint16 num, const TFlsCycleConst* finfo)
{
    uint32 addr;
    uint16 crc;
    uint8 index;                            //此条记录在FLASH中的索引
    
    SYS_VAR_CHECK(num >= finfo->keyno);      //参数验证
    
    //===========================================================
                                            //条目数据区的首地址
    addr = finfo->fls_addr + (num * finfo->itm_sectors * 4096);
    if(addr >= finfo->fls_addr_bk)
    {
        return SYS_ERR_VAR;
    }
    
    index = VFRAM_Search(addr);             //根据位图找当前记录的位置
                                            //读取非备份区数据
    if((index != 0) && (index <= finfo->itm_baby_num))
    {
        index--;                            //前个位置为已记录
        if(FLS_Read(                        //读取数据
            data, 
            addr + 512 + index * finfo->itm_baby_byte, 
            finfo->itm_baby_byte) == SA_ERR_OK)
        {
            if(FLS_Read(                    //读取CRC
                (uint8*)&crc, 
                addr + 256 + index * 2,
                2) == SA_ERR_OK)
            {                               //进行数据和CRC的比较
                if(crc == DoCrcDlms(data, finfo->itm_baby_byte, 0x5555))
                {
                   SYS_OK();                 //返回成功
                }
            }
        }
    }
    
    //===========================================================
    if(finfo->fls_addr_bk == finfo->fls_addr_end)//无备份区
    {
        ClearBuffer(data, finfo->itm_baby_byte);
        return SYS_ERR_FT;                       //返回失败
    }
    
    //===========================================================
                                           //条目备份数据区的首地址
    addr = finfo->fls_addr_bk + (num * finfo->itm_sectors * 4096); 
    if(addr >= finfo->fls_addr_end)
    {
        return SYS_ERR_VAR;
    }
    
    index = VFRAM_Search(addr);            //根据位图找当前记录的位置
                                           //读取备份区数据
    if((index != 0) && (index < finfo->itm_baby_num))
    {
        index--;
        if(FLS_Read(                       //读取数据
            data, 
            addr + 512 + index * finfo->itm_baby_byte, 
            finfo->itm_baby_byte) == SA_ERR_OK)
        {
            if(FLS_Read(                   //读取CRC
                (uint8*)&crc, 
                addr + 256 + index * 2,
                2) == SA_ERR_OK)
            {                               //进行数据和CRC的比较
                if(crc == DoCrcDlms(data, finfo->itm_baby_byte, 0x5555))
                {
                   SYS_OK();                 //返回成功
                }
            }
        }
    }
    
    //===========================================================
    ClearBuffer(data, finfo->itm_baby_byte);
    
    return SYS_ERR_FT;                       //返回失败

}



/************************************************************************
 * @function: SYS_VFRAM_IsConfigLegal
 * @描述: 判断铁电配置的合法性
 * 
 * @参数: 
 * @param: 
 * 
 * @返回: 
 * @return: uint8  0合法,其他非法
 * @说明: 
 * @作者: yzy (2013/11/2)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_VFRAM_IsConfigLegal(const TFlsCycleConst* finfo)
{
    uint32 tmp;
                                            //是否满足虚拟FLASH循环存储区规则:位图数组只有16字节"
    if(finfo->itm_baby_num >= 128)
    {
        return 1;
    }
                                            //判断单项数据分配的容量是否足够
    tmp = finfo->itm_baby_byte * finfo->itm_baby_num;
    if(tmp > ((4096 * finfo->itm_sectors) - 512))
    {
        return 2;
    }
                                            //必须sector对齐
    if((finfo->fls_addr & 0xfff) || (finfo->fls_addr_bk & 0xfff))
    {
        return 3;
    }
                                            //非备份区长度非法
    tmp = finfo->itm_sectors * 4096 * finfo->keyno;
    if(finfo->fls_addr_bk != (finfo->fls_addr + tmp))
    {
        return 4;
    }
                                            //有备份区,且备份区长度非法
    if((finfo->fls_addr_bk != finfo->fls_addr_end) 
       && (finfo->fls_addr_end != (finfo->fls_addr_bk + tmp)))
    {
        return 5;
    }
    
    return 0;
}

#endif                  //#if SYS_FLASH_FRAM_EN > 0




