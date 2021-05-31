/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: GD_cal.c
**创   建   人: yzy
**最后修改日期: 2019-06-18
**描        述: 国电函数库
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2019-06-18
** 描　述: 原始版本
********************************************************************************************************/
#define EXT_GD_CAL

//	#include "es.h"
//	#include "hal.h"
//	#include "bsp.h"
//	#include "app.h"
//	#include "apis.h"
//	#include "extapi.h"
#include "aos/bdef.h"
#include "cdef.h"
#include "uart.h"
#include "stdbool.h"                            
#include "libs.h"


#include "dev_realtime.h"
#include "lib_com/calclib_gw.h"

#include "string.h"
                            //统一时间格式项偏移
const uint8 fmt_offset[6] = {0, 6, 11, 17, 21, 28};
                            //统一时间格式项位数
const uint8 fmt_bitnum[6] = {6, 5, 6, 4, 7, 4};




/*************************************对FMT时间格式的描述************************************************
**FMT格式为4字节32位数
**当FMT为全0时,数据无效,分时日月年全部清零
**
**当FMT非全0,且高4位为全1时,FMT格式如下: 
**                  B0-5分,B6-10时,B11-16日,B17-20月,B21-27年
**当FMT非全0,且高4位非全1时,FMT格式如下
**                  FMT为秒总数形式的时间
*********************************************************************************************************/

/************************************************************************
 * @function: FMT_UnifyTime
 * @描述: 将mHDMY压缩成4字节格式
 * 
 * @参数: 
 * @param: time 输入时间
 * @param: isbcd 输入格式(假hex,真bcd)
 * @param: bigendian 输入顺序(假:分时日月年,真:年月日时分)
 * 
 * @返回: 
 * @return: uint32 
 * @说明: 该格式中,数值大小与时间后先具有正相关性
 * @作者: yzy (2011-11-25)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint32 FMT_UnifyTime(uint8* time, bool isbcd, bool bigendian)
{
    uint32 ret = 0;
    uint8 tmp[5];                           //分时日月年
    MoveBuffer(time, tmp, 5);

    if(isbcd)                               //将bcd转为hex
    {
        ByteArrayBcdToHex(time, tmp, 5);
    }

    if(bigendian)                           //顺序转为分时日月年
    {
        InvBuffer(tmp, 5);
    }

    for(uint8 uc_i = 0; uc_i < 5; uc_i++)   //压缩分时日月年
    {
        ret |= ((tmp[uc_i] & Bit_Mask8[fmt_bitnum[uc_i]]) << fmt_offset[uc_i]);
    }
    ret |= 0xF0000000;                      //属性位全部置一


    return ret;

}



/************************************************************************
 * @function: FMT_ReUnifyTime
 * @描述: 将4字节时间格式还原成mHDMY
 * 
 * @参数: 
 * @param: out 输出
 * @param: unify 输入4字节时间
 * @param: isbcd 输出格式(假hex,真bcd)
 * @param: bigendian 输出顺序(假:分时日月年,真:年月日时分)
 * @返回: 
 * @说明: 
 * @作者: yzy (2011-11-25)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void FMT_ReUnifyTime(uint8* out, uint32 unify, bool isbcd, bool bigendian)
{
    uint32 mask;
    for(uint8 uc_i = 0; uc_i < 5; uc_i++)
    {                                       //掩码
        mask = Bit_Mask32[fmt_bitnum[uc_i]] << fmt_offset[uc_i];
        out[uc_i] = (unify & mask) >> fmt_offset[uc_i];
    }
                                            //格式转换
    if(isbcd)
    {
        ByteArrayHexToBcd(out, out, 5);
    }

    if(bigendian)
    {
        InvBuffer(out, 5);
    }
    
}


/************************************************************************
 * @function: BuildTimeLabel
 * @描述: 创建时间标签
 * 
 * @参数: 
 * @param: ttype 
 * @param: input 
 * @param: output 
 * 
 * @返回: 
 * @return: uint8  成功/失败
 * @说明: 
 * @作者: yzy (2011-3-5)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 BuildTimeLabel(E_TTYPE ttype, uint8* input, S_TimeLabel* output)
{
    S_TimeLabel stl;
    uint8 smhDMY[6];
    if(ttype == TTYPE_YMDHMS)
    {
        MoveBuffer(input, stl.YMDhms, 6);
        //
        MoveBuffer(input, smhDMY, 6);   //转成smhDMY
        InvBuffer(smhDMY, 6);
        stl.mins = CalcMins(smhDMY + 1, false);
    }
    else if(ttype == TTYPE_SMHDMY)
    {
        MoveBuffer(input, smhDMY, 6);
        stl.mins = CalcMins(smhDMY + 1, false);
        //
        InvBuffer(smhDMY, 6);           //转成YMDhms
        MoveBuffer(smhDMY, stl.YMDhms, 6);
    }
    else if(ttype == TTYPE_MINS)
    {
        MoveBuffer(input, (uint8*)&stl.mins, 4);
        stl.YMDhms[0] = 0;
        CalcTimeFromMins(stl.YMDhms + 1, stl.mins);
        InvBuffer(stl.YMDhms, 6);       //转成YMDhms
    }
    else
    {
        return SYS_ERR_FT;
    }
    BuildBufferCRC((uint8*)&stl, sizeof(S_TimeLabel), true);
    MoveBuffer((uint8*)&stl, (uint8*)output, sizeof(S_TimeLabel));
    SYS_OK();
}



/************************************************************************
 * @function: CheckTimeLabel
 * @描述: 校验时间标识
 * 
 * @参数: 
 * @param: stl 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2011-3-5)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 CheckTimeLabel(S_TimeLabel* stl)
{
    if(true == CheckBufferCRC((uint8*)stl, sizeof(S_TimeLabel), true))
    {
        SYS_OK();
    }
    return SYS_ERR_FT;
}



/************************************************************************
**数据转发_通讯波特率
*************************************************************************/
//	const uint8 cfgbaud[8] = {1, 2, 4, 8, 16, 24, 32, 64};
const uint16 cfgbaud[8] = {32, 64, 384, 4, 8, 16, 1, 2};

/************************************************************************
 * @function: PentCfgToSS
 * @描述: 透明转发控制字转成串口结构体格式
 * 
 * @参数: 
 * @param: cfg
 * @param: ss 
 * @返回: 
 * @说明: 
 * @作者: yzy (2011-9-30)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 PentCfgToSS(uint8 cfg, void * ss)
{
//    ss->baud_rate = cfgbaud[cfg & 0x07] * 300;
//    if(cfg & 0x80)
//    {
//        ss->stop_bits = STOP_BITS_2;
//    }
//    else
//    {
//        ss->stop_bits = STOP_BITS_1;
//    }
//
//    if(cfg & 0x40)
//    {
//        ss->parity = (cfg & 0x20) ? ODD_PARITY : EVEN_PARITY;
//        if((cfg & 0x18) == 0x10)
//        {
//            ss->data_width = DATA_WIDTH_8BIT;
//        }
//        else
//        {
//            ss->data_width = DATA_WIDTH_9BIT;
//        }  
//    }
//    else
//    {
//        ss->parity = NO_PARITY;
//        if((cfg & 0x18) == 0x10)
//        {
//            ss->data_width = DATA_WIDTH_7BIT;
//        }
//        else
//        {
//            ss->data_width = DATA_WIDTH_8BIT;
//        }        
//    }
//                                        //通讯参数(只支持7/8数据位)
//
//    ss->flow_control = FLOW_CONTROL_DISABLED;
//    ss->mode = MODE_TX_RX;
//    
    SYS_OK();
}


/************************************************************************
** 函数名称: void STAT_MaxValue(void)
** 描 述: 统计数据的日最大值处理
** 输 入:  cmper 统计数据结构体
            val 当前的数值
** 输 出:
**
** 作 者: yzy
** 日 期: 2008年4月13日
**-----------------------------------------------------------------------
** 修 改: yzy
** 内 容: 增加了cmper->mAttrib的赋值(为了调试时清晰,并无实际作用)
** 日 期: 2008-10-7
*************************************************************************/
bool STAT_MaxValue(STATCmper* cmper, uint32 val)
{
    cmper->mAttrib = STAT_CMP_MAX;
    if(val > cmper->mVal)               //判断是否比当前的最大值要大
    {
        const TIME* time = GetTime();   //获取时间
        cmper->mHour = ByteHexToBcd(time->hour);
        cmper->mMinute = ByteHexToBcd(time->min);
        cmper->mVal = val;              //获取当前的最大值
        return true;
    }
    return false;                       //最大最小值发生变化
}


/************************************************************************
** 函数名称: void STAT_MaxValue(void)
** 描 述: 统计数据的日最小值处理
** 输 入: cmper 统计数据结构体
**        val 当前的数值
** 输 出:
**
** 作 者: yzy
** 日 期: 2008年4月13日
**-----------------------------------------------------------------------
** 修 改: yzy
** 内 容: 增加了cmper->mAttrib的赋值
** 日 期: 2008-10-7
*************************************************************************/
bool STAT_MinValue(STATCmper* cmper, uint32 val)
{
    cmper->mAttrib = STAT_CMP_MIN;
    if(val < cmper->mVal)                //判断是否比当前的最小值要大  
    {                                                                  
        const TIME* time = GetTime();   //获取时间 
        cmper->mHour = ByteHexToBcd(time->hour);                                     
        cmper->mMinute = ByteHexToBcd(time->min);                                    
        cmper->mVal = val;               //获取当前的最小值            
        return true;                                                   
    }
    return false;                        //最大最小值发生变化
}


/************************************************************************
** 函数名称: void STAT_SignedMaxValue(void)
** 描 述: 有符号统计数据的日最大值处理
** 输 入: cmper 统计数据结构体
**        val 当前的数值
** 输 出:
**
** 作 者: yzy
** 日 期: 2008年4月13日
**-----------------------------------------------------------------------
** 修 改: yzy
** 内 容: 增加了cmper->mAttrib的赋值
** 日 期: 2008-10-7
*************************************************************************/
bool STAT_SignedMaxValue(STATCmper* cmper, int32 val)
{
     cmper->mAttrib = STAT_CMP_MAX;
                                        //有符号数进行绝对值的比较
    if(AbsVal(val) > AbsVal((int32)cmper->mVal))
    {                                   
        const TIME* time = GetTime();   //获取时间
        cmper->mHour = ByteHexToBcd(time->hour);                                     
        cmper->mMinute = ByteHexToBcd(time->min);                                    
        cmper->mVal = val;              //当前的最大值
        return true;
    }
    return false;
}

/************************************************************************
** 函数名称: void STAT_SignedMinValue(void)
** 描 述: 有符号统计数据的日最小值处理
** 输 入:  cmper 统计数据结构体
**         val 当前的数值
** 输 出:
**
** 作 者: yzy
** 日 期: 2008年4月13日
**-----------------------------------------------------------------------
** 修 改: yzy
** 内 容: 增加了cmper->mAttrib的赋值
** 日 期: 2008-10-7
*************************************************************************/
bool STAT_SignedMinValue(STATCmper* cmper, int32 val)
{
    cmper->mAttrib = STAT_CMP_MIN;
                                        //有符号数进行绝对值的比较
    if(AbsVal(val) < AbsVal((int32)cmper->mVal))
    {
        const TIME* time = GetTime();   //获取时间
        cmper->mHour = ByteHexToBcd(time->hour);                                     
        cmper->mMinute = ByteHexToBcd(time->min);                                    
        cmper->mVal = val;              //获取当前的最大最小值
        return true;                    
    }
    return false;
}


/************************************************************************
**函数名称: void StepForward_15Mins(uint8* time, bool isym)
**描 述: 将time时间定格到之后最近的一刻钟时间
**
**输 入: time YYMMDDHHmm(isym真,HEX),
**            mmHHDDMMYY(isym假)
**输 出: 
**
**备 注: 用于小时曲线数据(如输入:07-08-02 15:23;输出为07-08-02 15:30)
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
void StepForward_15Mins(uint8* time, bool isym)
{
    uint8 dif;
    if(isym == false)
    {
        InvBuffer(time, 5);
    }

    dif = time[4] % 15;
    if(dif)                             //若未对对齐则求出差
    {
        dif = 15 - dif;
        StepMins(time, dif); 
    }

    if(isym == false)
    {
        InvBuffer(time, 5);
    }
}



/************************************************************************
**函数名称: void StepForward_60Mins(uint8* time, bool isym)
**描 述: 将time时间定格到之后最近的一刻钟时间
**
**输 入: time YYMMDDHHmm(isym真,HEX),
**            mmHHDDMMYY(isym假)
**输 出: 
**
**备 注: 用于小时曲线数据(如输入:07-08-02 15:23;输出为07-08-02 16:00)
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
void StepForward_60Mins(uint8* time, bool isym)
{
    uint8 dif;
    if(isym == false)
    {
        InvBuffer(time, 5);
    }

    dif = time[4];
    if(dif)                             //若未对对齐则求出差
    {
        dif = 60 - dif;
        StepMins(time, dif); 
    }

    if(isym == false)
    {
        InvBuffer(time, 5);
    }
}

/************************************************************************
**函数名称: uint8 ValueToHexChar(uint8 val, bool iscap)
**描 述: HexCharToValue的反运算
**
**输 入: val:0-0x0f 
         iscap: 输出的ascii是否为大写 
**输 出: 0xFF:无法转换 
         如val=11, 若iscap为真则输出'B';为假,则输出'b'.
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 2009-05-21
*************************************************************************/
/*
uint8 ValueToHexChar(uint8 val, bool iscap)
{
    if(val <= 9)
    {
        val += '0';                          //0-9
    }
    else if(val >= 0x0A && val <= 0x0F)
    {
        if(iscap)
        {
            val += 'A' - 10;                //A-F
        }
        else
        { 
            val += 'a' - 10;                //a-f
        }
    }
    else
    {
        val = 0xFF;                         //无效
    }

    return val;                             //返回最终结果
}
*/


/************************************************************************
**函数名称: uint8 DecimalNumToBcdString(uint8* ch, uint32 num, uint8 digit)
**描 述: 将32位hex转换成带小数点的BCD字符串
**
**输 入: ch 输出缓存 
**       num 4字节数据
**       digit 小数位数(<= 4)
**输 出: 转换后的长度
**
**备 注: 若num = 32264,digit = 4,则输出"3.2264",返回6 
**       若num = 365,digit = 4,则输出"0.0365",返回6
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 2009-06-15
*************************************************************************/
uint8 DecimalNumToBcdString(uint8* ch, uint32 num, uint8 digit)
{
    uint8 len;
    uint8 ret;

    if(digit > 4)                           //暂定小数位数不大于4
    {
        return 0;
    }

    len = NumToBcdString(ch, num, 0);
    if(len >= (digit + 1))
    {                                       //若为hex4.4,则"32264"->"3.2264"
        MoveBuffer(ch + (len - digit), ch + (len - digit) + 1, digit);
        ch[len - digit] = '.';              //嵌入小数点
        ret = len + 1;
    }
    else
    {                                       //若为hex4.4,则"365"->"0.0365"
        MoveBuffer(ch, ch + (digit + 2) - len, len);
        memset(ch, '0', (digit + 2) - len);
        ch[1] = '.';                        //嵌入小数点(hex4.4)
        ret = digit + 2;
    }

    return ret;

}


//calclib.c中将非压缩性BCD称之为BcdChar

/************************************************************************
**函数名称: void ByteArrayBcdToHexString(uint8* bcdary, uint8* ascii, uint32 len, bool big)
**描 述: 将BCD字节串转成ascii
**
**输 入: bcdary 需要转换的数据
         ascii  转换好的字符串
         len 需要转换的数据的长度
         big 是否big-endian输出
**输 出: 如 hexary[0] = 0x01, hexary[1] = 0x0A, len = 2, 
**       若big为真,输出ascii[0]-[3]为 '0' '1' '0' 'A' 
         若big为假,输出ascii[0]-[3]为 '0' 'A' '0' '1'
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 2009-05-21
*************************************************************************/
/*
void ByteArrayBcdToHexString(uint8* bcdary, uint8* ascii, uint32 len, bool big)
{
    for(uint8 uc_i = 0; uc_i < len; uc_i++)
    {
        if(big)
        {
            ascii[(uc_i << 1)] = ValueToHexChar(bcdary[uc_i] >> 4, true);
            ascii[(uc_i << 1) + 1] = ValueToHexChar(bcdary[uc_i] & 0x0F, true);
        }
        else
        {
            ascii[((len - uc_i - 1) << 1)] = ValueToHexChar(bcdary[uc_i] >> 4, true);
            ascii[((len - uc_i - 1) << 1) + 1] = ValueToHexChar(bcdary[uc_i] & 0x0F, true);
        }
    }
}
*/





/************************************************************************
**函数名称: bool StepDays(uint8* time, int16 step)
**描 述: 对时间进行日步进
**
**输 入: time YYMMDD(hex,年在前)
         step 步进的天数
**输 出: true成功 false失败
**
**备 注: 如time指向 08-01-31 23点,step = 2,则输出time为 08-02-01 01点
**--------------------------------------------------------
**作 者: yzy
**日 期: 2009-01-19,未测试
*************************************************************************/
bool StepDays(uint8* time, int16 step)
{
    uint8 tmp[5];
    MoveBuffer(time, tmp, 3);
    tmp[3] = 0;
    tmp[4] = 0;
    
    uint8 ret = StepMins(tmp, step * 1440);
    if(ret)
    {
        MoveBuffer(tmp, time, 3);
    }
    
    return ret;
}



/************************************************************************
**函数名称: void GD01ToHexTime(uint8* dat, uint8* hex)
**描 述: 国电01格式转为hex格式日期(ssmmHHDDMMYY)
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 2008-12-09
*************************************************************************/
void GD01ToHexTime(uint8* dat, uint8* hex)
{
    uint8 tmp[6];

    MoveBuffer(dat, tmp, 6);
    tmp[4] &= 0x1F;
    ByteArrayBcdToHex(tmp, hex, 6);
}



/************************************************************************
**函数名称: uint16 GD07ToHex22(uint8* dat)
**描 述: 国电07格式转为hex2.2
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 2008-12-09
*************************************************************************/
uint16 GD07ToHex22(uint8* dat)
{
    uint16 r;
    BcdToHex(dat,(uint8*)&r, 2, 2);
    r *= 10;
    return r;
}

/************************************************************************
**函数名称: int16 GD06ToShex22(uint8* dat)
**描 述: 国电06格式转为shex2.2
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 
*************************************************************************/
int16 GD06ToShex22(uint8* dat)
{
    uint16 tmp = ByteToInt(dat);
    int16 r = (int16)IntBcdToHex(tmp & 0x7FFF);
    if(tmp & 0x8000)
    {
        r = -r;
    }
    return r;
}

/************************************************************************
**函数名称: int32 GD06ToShex43(uint8* dat)
**描 述: 国电06格式转为shex4.3
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 
*************************************************************************/
int32 GD06ToShex43(uint8* dat)
{
    return ((int32)GD06ToShex22(dat) * 10);
}


/************************************************************************
**函数名称: int16 GD05ToShex23(uint8* dat)
**描 述: 国电05格式转为shex2.3
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 09-11-24
*************************************************************************/
int16 GD05ToShex23(uint8* dat)
{
    return (GD06ToShex22(dat));
}


/************************************************************************
**函数名称: uint32 GD23ToHex44(uint8* dat)
**描 述: 国电23格式转为hex3.4
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 
*************************************************************************/
uint32 GD23ToHex44(uint8* dat)
{
    uint32 r = 0;
    BcdToHex(dat, (uint8*)&r, 3, 4);
    return r;
}




/************************************************************************
**函数名称: uint32 GD25ToShex43(uint8* dat)
**描 述: 国电25格式转为hex4.3
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 09-09-01
*************************************************************************/
int32 GD25ToShex43(uint8* dat)
{
    uint32 tmp = ByteToInt24(dat);
    int32 r = (int32)LongBcdToHex(tmp & 0x007FFFFF);
    if(tmp & 0x00800000)
    {
        r = -r;
    }
    return r;

}


/************************************************************************
**函数名称: int32 GD03ToShex42(uint8* dat)
**描 述: 
**
**输 入: 
**输 出: 无论输入为kwh还是mwh,输出都统一为kwh单位
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 2008-12-02
*************************************************************************/
int32 GD03ToShex42(uint8* dat)
{
    uint8 ismul;                            //单位标志 0 kwh 1 mkwh
    uint8 issign;                           //符号标志 0 正  1 负
    ismul = ((dat[3] & 0x40) == 0 ? 0 : 1);
    issign = ((dat[3] & 0x10) == 0 ? 0 : 1);

    uint8 tmp[4];
    int32 r;
    tmp[0] = dat[0];
    tmp[1] = dat[1];
    tmp[2] = dat[2];
    tmp[3] = dat[3] & 0x0F;
    r = (int32)ByteToLong(tmp);
    r = LongBcdToHex((uint32)r);
    r = r * 100;                            //两位小数

    if(ismul)
    {
        r *= 1000;
    }
    if(issign)
    {
        r = -r;
    }
    
    return r;

}



const uint32 guls_fmt_gd02_coef[8] = {100000000, 10000000, 1000000, 100000, 10000, 1000, 100 ,10};
/************************************************************************
**函数名称: int32 GD02ToShex44(uint8* dat)
**描 述: 将国电02格式装换成shex4.4格式
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 2008-12-08
*************************************************************************/
int32 GD02ToShex44(uint8* dat)
{
    uint32 bemul = guls_fmt_gd02_coef[dat[1] >> 5];
    uint8 issign = (dat[1] & 0x10) ? 1 : 0;
    uint8 tmp[2];
    int32 r;
    tmp[0] = dat[0];
    tmp[1] = dat[1] & 0x0F;
    BcdToHex(tmp,(uint8*)&r, 2, 4);
    r *= bemul;
    if(issign)
    {
        r = -r;
    }
    return r;
}


/************************************************************************
**函数名称: void Shex22ToGD05(int16 dat, uint8* buf)
**描 述: shex2.2格式到国电05格式
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 
*************************************************************************/
void Shex22ToGD05(int16 dat, uint8* buf)
{
    uint16 r = 0;
    if(dat < 0)
    {
        dat = -dat;
        r = 0x8000;
    }
    dat /= 10;
    r |= IntHexToBcd(dat);

    IntToByte(buf, r);
}


/************************************************************************
**函数名称: Shex23ToGD05(int16 dat, uint8 *buf)
**描 述: 将Shex2.3格式的数据转换为国电规约05格式
**输 入: dat:要转换的数据
         buf:存储转换数据的指针
**输 出: 
**--------------------------------------------------------
**作　者: 
**日　期: 
*************************************************************************/
void Shex23ToGD05(int16 dat, uint8 *buf)
{
    uint16 r = 0;
    if(dat < 0)
    {
        dat = -dat;
        r = 0x8000;
    }
    dat /= 100;
    r |= IntHexToBcd(dat);

    IntToByte(buf, r);
}

/************************************************************************
**函数名称: Shex23ToGD05Percent(int16 dat, uint8 *buf)
**描 述: 将Shex2.3格式的数据转换为国电规约05格式(%)
**输 入: dat:要转换的数据
         buf:存储转换数据的指针
**输 出: 要求输入<=9.999,即输出<=999.9%
**--------------------------------------------------------
**作　者: 
**日　期: 
*************************************************************************/
void Shex23ToGD05Percent(int16 dat, uint8 *buf)
{
    uint16 r = 0;
    if(dat < 0)                             //提取符号
    {
        dat = -dat;
        r = 0x8000;
    }

    if(dat > 9999)                          //超出数据格式05的范围   
    {
        dat = 9999;
    }

    r |= IntHexToBcd(dat);
    IntToByte(buf, r);
}


/************************************************************************
**函数名称: void Hex43ToGD06(uint32 dat, uint8 *buf)
**描 述: hex4.3转换为国电格式06
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 08-01-14
*************************************************************************/
void Hex43ToGD06(uint32 dat, uint8 *buf)
{
    if(dat >= 80000)
    {                                       //超出格式06
        *buf = 0x99;
        *(buf + 1) = 0x79;
    }
    else
    {                                       //+5四舍五入
        HexToBcd(buf, (dat + 5) / 10, 2, 2);
    }
}


/************************************************************************
**函数名称: void Hex22ToGD07(uint16 dat, uint8 *buf)
**描 述: hex2.2转换为国电格式07
**
**输 入: dat:要转换的数据 
**       buf:存储转换数据的指针 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
void Hex22ToGD07(uint16 dat, uint8 *buf)
{
    uint16 r;
    r = LongHexToBcd(dat) >> 4;
    IntToByte(buf, r);
}




/************************************************************************
**名 称: void Shex44ToGD09(int32 dat, uint8 *buf)
**描 述: 将Shex4.4格式转换为国电09格式(sXX.XXXX)
**输 入: dat:要转换的数据
         buf:存储转换数据的指针
**输 出: 如 data = 0xFFFFFFFF(-0.0001)
**       转换后*buf后为0x01,0x00,0x80
**--------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
void Shex44ToGD09(int32 dat, uint8 *buf)
{
    uint32 r = 0;
    if(dat < 0)
    {
        dat = -dat;
        r |= 0x800000;
    }
    r |= LongHexToBcd(dat);
    Int24ToByte(buf, r);
}



/************************************************************************
**函数名称: void Hex42ToGD11(uint32 dat, uint8* buf)
**描 述: Hex42格式转换成国电11格式
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-09-09
*************************************************************************/
void Hex42ToGD11(uint32 dat, uint8* buf)
{
    LongToByte(buf, LongHexToBcd(dat));
}




/************************************************************************
**函数名称: void Hex42ToGD13(uint32 dat, uint8* buf)
**描 述: Hex42格式转换成国电13格式
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 10-02-25
*************************************************************************/
void Hex42ToGD13(uint32 dat, uint8* buf)
{
    LongToByte(buf, LongHexToBcd(dat * 100));
}




/************************************************************************
**函数名称: void Hex42ToGD14(uint32 dat, uint8* buf)
**描 述: 
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 09-01-13
*************************************************************************/
void Hex42ToGD14(uint32 dat, uint8* buf)
{
    LongToByte(buf, LongHexToBcd(dat));
    buf[4] = 0;
}



/************************************************************************
**名 称: void Shex44ToGD23(int32 data, uint8 *buf)
**描 述: Shex4.4格式转换为国电23格式(无符号)
**输 入:    
**输 出: 如 data = 0xFFFFFFFF(-1); 转换后*buf后为0x01,0x00,0x00
**说 明: 格式23类似与格式09,只是少了符号
**--------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
void Shex44ToGD23(int32 data, uint8 *buf)
{
    if(data < 0)
    {
        data = -data;
    }

    Int24ToByte(buf, LongHexToBcd(data));
}



/************************************************************************
**函数名称: void Shex43ToGD25(uint32 dat, uint8 *buf)
**描 述: shex4.3转换为国电格式25
**
**输 入: 
**输 出: 
**
**备 注: 未调试
**--------------------------------------------------------
**作 者: yzy
**日 期: 09-09-25
*************************************************************************/
void Shex43ToGD25(int32 dat, uint8* buf)
{
    uint8 sign;                     //符号位
    if(dat < 0)                     //绝对值
    {
        dat = -dat;
        sign = 1;
    }
    else
    {
        sign = 0;
    }
                                    //数值
    if(dat >= 800000)
    {
        buf[0] = 0x99;
        buf[1] = 0x99;
        buf[2] = 0x79;
    }
    else
    {
        HexToBcd(buf, dat, 3, 4);
    }
                                    //符号
    if(sign)
    {
        buf[2] |= 0x80;
    }

}

/************************************************************************
**函数名称: void Shex44ToGD02(uint8* buffer, int32 data)
**描 述: shex4.4转换为国电格式02
**
**输 入: data允许显示范围:
**       +-(0.001*10000(GD02限制)~214748.3647*10000(Shex44限制))
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 09-01-06
*************************************************************************/
void Shex44ToGD02(uint8* buffer, int32 data)
{
    uint16 r = 0;
    int8 g;                             //阶

    if(data < 0)
    {
        data = -data;
        r |= 0x1000000;                 //符号位
    }

    //hex44->hex44(3位有效位)*(10的g次方)
    if(data >= 1000000000)              //>=100000.0000[受Shex44格式限制,data最大为2147483646]
    {
        data /= 10000000;
        g = 3;
    }
    else if(data >= 100000000)          //>=10000.0000
    {
        data /= 1000000;
        g = 2;
    }
    else if(data >= 10000000)           //>=1000.0000
    {
        data /= 100000;
        g = 1;
    }
    else if(data >= 1000000)            //>=100.0000
    {
        data /= 10000;
        g = 0;
    }
    else if(data >= 100000)             //>=10.0000
    {
        data /= 1000;
        g = -1;
    }
    else if(data >= 10000)              //>=1.0000
    {
        data /= 100;
        g = -2;
    }
    else                                //<=(0.)9999[受GD02格式限制,只能表示10的-3次方]
    {
        data /= 10;
        g = -3;
    }

    //hex44(3位有效位)*(10的g次方)->GD02
    r |= (IntHexToBcd((uint16)data) & 0x0FFF);
    r |= ((4-g) << 13);
    IntToByte(buffer, r);
}






/************************************************************************
**函数名称: void Shex42ToGD03(uint8* buffer, int32 data);
**描 述: 
**
**输 入: 
**输 出: 
**
**备 注: 一般使用kwh单位,若超出kwh表示范围则使用Mwh.
**--------------------------------------------------------
**作 者: yzy
**日 期: 2009-01-03
*************************************************************************/
void Shex42ToGD03(uint8* buffer, int32 data)
{
    uint32 r = 0;

    if(data < 0)
    {
        data = -data;
        r |= 0x10000000;                //符号位
    }
    data /= 100;                        //去小数位

    if(data > 9999999)                  //若超出kwh能表示的范围
    {
        data /= 1000;                   //由Mwh表示
        r |= 0x40000000;
    }

    r |= LongHexToBcd((uint32)data);
    LongToByte(buffer, r);

}




/************************************************************************
 * @function: HexTimeToGD01
 * @描述: 年月日时分秒转国电格式01
 * 
 * @参数: 
 * @param: YMDHms 
 * @param: data 
 * @返回: 
 * @说明: 
 * @作者: yzy (2011-9-30)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void HexTimeToGD01(uint8* YMDHms, uint8* data)
{
    uint8 smHDMY[6];
    uint8 dweek;
                                        //倒序
    MoveBuffer(YMDHms, smHDMY, 6);
    InvBuffer(smHDMY, 6);
                                        //格式转换
    dweek = CalcWeek(smHDMY + 3, 0);
    if(dweek == 0)
    {
        dweek = 7;
    }
    ByteArrayHexToBcd(smHDMY, data, 6);
    smHDMY[4] |= dweek << 5;
}












