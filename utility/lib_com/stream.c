
#include "aos/bdef.h"
#include "cdef.h"

#include "libs.h"

const uint8 Bit_Count4[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};


/************************************************************************
**函数名uint8bool STR_IsBcd(uint8* data, uint8 num)
**描 述: 判断是否为压缩型bcd格式
**
**输 入: 
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 2010-12-10
*************************************************************************/
uint8 STR_IsBcd(uint8* data, uint8 num)
{
    uint16 k;
    for(k = 0; k < num; k++)
    {
        if(((data[k] & 0x0F) > 0x09) || ((data[k] & 0xF0) > 0x90))
        {
            return false;
        }
    }
    
    return true;
}



/************************************************************************
**函数名称: uint8 STR_IsEmpty(uint8* s, uint8 num)
**描 述: 判断是否空串
**
**输 入: 
**输 出: false有内容
         true空串
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
uint8 STR_IsEmpty(uint8* s, uint8 num)
{
    for(uint8 uc_i = 0; uc_i < num; uc_i++)
    {
        if(s[uc_i] != 0)
        {
            return false;
        }
    }
    return true;
}
uint8 STR_IsFull(uint8* s, uint8 num)
{
    for(uint8 uc_i = 0; uc_i < num; uc_i++)
    {
        if(s[uc_i] != 0xFF)
        {
            return false;
        }
    }
    return true;
}


/************************************************************************
**函数名uint8bool STR_IsBitSet(uint8* stream, uint16 bit)
**描 述: 判断某位是否置一
**
**输 入: steam 比特流, bit 要判断的位数(从0开始)
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 10-01-26
*************************************************************************/
uint8 STR_IsBitSet(uint8* stream, uint16 bit)
{
    if(stream[bit >> 3] & Bit_Map8[bit & 0x07])
    {
        return true;
    }
    else
    {
        return false;
    }
}


/************************************************************************
**函数名uint8bool STR_IsBitClr(uint8* stream, uint16 bit)
**描 述: 判断某位是否为零
**
**输 入: steam 比特流, bit 要判断的位数(从0开始)
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 10-01-26
*************************************************************************/
uint8 STR_IsBitClr(uint8* stream, uint16 bit)
{
    if((stream[bit >> 3] & Bit_Map8[bit & 0x07]) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/************************************************************************
 * @function: STR_IsBitsState
 * @描述: 判断某些位是否全为1或0
 * 
 * 
 * @参数: 
 * @param: stream  比特流
 * @param: bit  要判断的起始位数(从0开始)
 * @param: num  要判断的位数
 * @param: oc  真:为1,假:为0
 * 
 * @返回: 
 * @return:  uint8   
 * @作者: yzy (2010-10-8)
 ***----------------------------------------------------------------------
 * @修改人: yzy(2010-12-06)修改首尾比特在相同字节内时判断错误的BUG
 *************************************************************************/
uint8 STR_IsBitsState(uint8* stream, uint16 bit, uint8 num, uint8 oc)
{
    uint16 byte1, byte2;                //首尾比特对应的字节
    uint16 bit1, bit2;                  //首尾比特对应的位数
    uint16 k;
    uint8 mask1, mask2;
    uint8 cmp1, cmp2, cmpm;
    
    if(num == 0)                        //至少判断1位
    {
        num = 1;
    }
                                        //计算首尾比特对应的字节
    byte1 = (bit >> 3);
    byte2 = ((bit + num - 1) >> 3);
                                        //计算掩码
    if(byte1 != byte2)                  //首尾比特在不同字节
    {
        bit1 = bit & 0x07;
        bit2 = (bit + num - 1) & 0x07;
        mask1 = ~Bit_Mask8[bit1];
        mask2 = Bit_Mask8[bit2 + 1];
    }
    else                                //首尾比特在相同字节
    {
        bit1 = bit & 0x07;
        bit2 = bit1 + num;
        mask1 = ((~Bit_Mask8[bit1]) & Bit_Mask8[bit2]);
        mask2 = mask1;
    }
                                        //计算比较值
    cmp1 = oc ? mask1 : 0;
    cmp2 = oc ? mask2 : 0;
    cmpm = oc ? 0xFF : 0;
                                        //判断首字节
    if((stream[byte1] & mask1) != cmp1)
    {
        return false;
    }
                                        //判断尾字节
    if(byte2 > byte1)
    {
        if((stream[byte2] & mask2) != cmp2)
        {
            return false;
        }
    }
                                        //判断中间字节
    if(byte2 > (byte1 + 1))
    {
        for(k = (byte1 + 1); k < byte2; k++)
        {
            if(stream[k] != cmpm)
            {
                return false;
            }
        }
    }
    return true;
}


/************************************************************************
**函数名称: uint16 STR_CalcBitSetNum(uint8* dat, uint8 num)
**描 述: 找出二进制流dat中1的位数
**
**输 入: dat 比特流
         no 字节数
**输 出: 
**
**备 注: 
**--------------------------------------------------------
**作 者: yzy
**日 期: 09-01-19
*************************************************************************/
uint16 STR_CalcBitSetNum(uint8* dat, uint8 num)
{
    uint16 r = 0;
    for(uint8 uc_i = 0; uc_i < num; uc_i++)
    {
        r += Bit_Count4[dat[uc_i] & 0x0F];
        r += Bit_Count4[dat[uc_i] >> 4];
    }
    return r;
}



/************************************************************************
 * @function: STR_CalcBitClrNum
 * @描述: 找出二进制流dat中0的位数
 * 
 * @参数: 
 * @param: dat 比特流
 * @param: num 字节数
 * 
 * @返回: 
 * @return: uint16  
 * @说明: 
 * @作者: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint16 STR_CalcBitClrNum(uint8* dat, uint8 num)
{
    uint16 all = num << 3;
    uint16 clrnum = all - STR_CalcBitSetNum(dat, num);
    return clrnum;

}



/************************************************************************
**函数名称: uint16 STR_CalcBitSetNum_Before(uint8* str, uint16 idx)
**描 述: 计算比特流的idx位前一共有几位置1(含idx位)
**
**输 入: str 比特流
         idx 要计算的那一位
**输 出: idx前置一的位数
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 2009-03-16
*************************************************************************/
uint16 STR_CalcBitSetNum_Before(uint8* str, uint16 idx)
{
    uint16 r;
    uint8 byte;
                                                    //整字节的位数
    byte = idx >> 3;
    r = STR_CalcBitSetNum(str, byte);
                                                    //散位的位数
    idx &= 0x0007;
    byte = str[byte];
    byte &= (1 << (idx + 1))  - 1;
    r += Bit_Count4[byte & 0xf];
    r += Bit_Count4[byte >> 4];

    return r;
}



/************************************************************************
**函数名称: uint16 STR_GetFirstBit(uint8* str, uint8 nuint8bool set)
**描 述: 查找比特流中首个为0或1的位
**
**输 入: str 比特流
         num 比特流的字节数
         set true 为1, false 为0
**输 出: 0xFFFF: 找不到
         others: str中首个为一的位
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
uint16 STR_GetFirstBit(uint8* str, uint8 num, uint8 set)
{
    uint16 ret = 0;
    uint8 cs = (set == true) ? 0 : 0xFF;
    uint8 tmp;

    for(uint8 uc_i = 0; uc_i < num; uc_i++)
    {
        if(str[uc_i] == cs)
        {
            ret += 8;
        }
        else
        {
            tmp = cs ^ str[uc_i];
            ret += Bit_Maps[tmp];
            return ret;
        }
    }
    return 0xFFFF;

}


/************************************************************************
**函数名称: uint16 STR_GetFirstSetBit(uint8* str, uint8 num)
**描 述: 查找比特流中首个为一的位
**
**输 入: str 比特流
         num 比特流的字节数
**输 出: 0xFFFF: 找不到
         others: str中首个为一的位
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
uint16 STR_GetFirstSetBit(uint8* str, uint8 num)
{
    return STR_GetFirstBit(str, num, true);
}

/************************************************************************
**函数名称: uint16 STR_GetFirstClrBit(uint8* str, uint8 num)
**描 述: 查找比特流中首个为零的位
**
**输 入: str 比特流
         num 比特流的字节数
**输 出: 0xFFFF: 找不到
         others: str中首个为零的位
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
uint16 STR_GetFirstClrBit(uint8* str, uint8 num)
{
    return STR_GetFirstBit(str, num, false);
}

/************************************************************************
**函数名称: uint16 STR_GetSpecSetBit(uint8* dat, uint8 datlen, uint16 xh)
**描 述: 查找比特流中第N个为1位的位索引
**
**输 入: dat 比特流
         datlen 比特流的字节数
         xh 第N个位(0-!)
**输 出: 0xFFFF: 找不到
         others: 首个为一的位(0~!)
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
uint16 STR_GetSpecSetBit(uint8* dat, uint8 datlen, uint16 xh)
{
    uint16 index = 0;                   //位索引
    uint16 k = 0;                       //序号索引
    uint8 bytecnt;
    
    if(xh >= (datlen << 3))             //参数合法性判断
    {
        return 0xFFFF;
    }
    
    for(uint8 uc_i = 0; uc_i < datlen; uc_i++)
    {
        bytecnt = 0;
        bytecnt += Bit_Count4[dat[uc_i] & 0x0F];
        bytecnt += Bit_Count4[dat[uc_i] >> 4];
        if(xh >= k + bytecnt)           //要找的序号的位不在该字节,继续处理下字节
        {
            index += 8;
            k += bytecnt;
            continue;
        }
                                        //找到字节,细找具体的位
        for(uint8 uc_j = 0; uc_j < 8; uc_j++)
        {
            if(dat[uc_i] & Bit_Map8[uc_j])
            {
                if(k == xh)
                {
                    index += uc_j;
                    return index;
                }
                k++;
            }
        }
    }
    
    return 0xFFFF;

}




/************************************************************************
**函数名称: uint16 STR_GetSpecClrBit(uint8* dat, uint8 datlen, uint16 xh)
**描 述: 查找比特流中第N个为0位的位索引
**
**输 入: dat 比特流
         datlen 比特流的字节数
         xh 第N个位(0-!)
**输 出: 0xFFFF: 找不到
         others: 首个为0的位(0~!)
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
uint16 STR_GetSpecClrBit(uint8* dat, uint8 datlen, uint16 xh)
{
    uint16 index = 0;                   //位索引
    uint16 k = 0;                       //序号索引
    uint8 bytecnt;
    
    if(xh >= (datlen << 3))             //参数合法性判断
    {
        return 0xFFFF;
    }
    
    for(uint8 uc_i = 0; uc_i < datlen; uc_i++)
    {
        bytecnt = 0;
        bytecnt += 4 - Bit_Count4[dat[uc_i] & 0x0F];
        bytecnt += 4 - Bit_Count4[dat[uc_i] >> 4];
        if(xh >= k + bytecnt)           //要找的序号的位不在该字节,继续处理下字节
        {
            index += 8;
            k += bytecnt;
            continue;
        }
                                        //找到字节,细找具体的位
        for(uint8 uc_j = 0; uc_j < 8; uc_j++)
        {
            if(!(dat[uc_i] & Bit_Map8[uc_j]))
            {
                if(k == xh)
                {
                    index += uc_j;
                    return index;
                }
                k++;
            }
        }
    }
    
    return 0xFFFF;

}





/************************************************************************
**函数名称: uint16 STR_GetLastSetBit(uint8* str, uint8 num)
**描 述: 查找比特流中最后一个置一的位
**
**输 入: str 比特流
         num 比特流的字节数
**输 出: 0xFFFF: 找不到
         others: 位索引
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 2009-03-16
*************************************************************************/
uint16 STR_GetLastSetBit(uint8* str, uint8 num)
{
    uint16 ret;
    uint8 i;

    if(num == 0)                             //字节数不得少于1
    {
        return 0xFFFF;
    }
    
    ret = num * 8 - 1;
    while(num)
    {
        i = num - 1;

        if(str[i] == 0)                     //若该字节为零
        {
            ret -= 8;
        }
        else                                //否则查找字节中最后的置一位
        {
            for(uint8 k = 0; k < 8; k++)
            {
                if(str[i] & Bit_Map8[7 - k])
                {
                    return ret;
                }
                ret--;
            }
        }
        
        num--;
    }

    return 0xFFFF;

}



/************************************************************************
**函数名称: uint16 STR_GetLastClrBit(uint8* str, uint8 num)
**描 述: 查找比特流中最后一个置零的位
**
**输 入: str 比特流
         num 比特流的字节数
**输 出: 0xFFFF: 找不到
         others: 位索引
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 2009-03-16
*************************************************************************/
uint16 STR_GetLastClrBit(uint8* str, uint8 num)
{
    uint16 ret;
    uint8 i;

    if(num == 0)                            //字节数不得少于1
    {
        return 0xFFFF;
    }
    
    ret = num * 8 - 1;
    while(num)
    {
        i = num - 1;

        if(str[i] == 0xFF)                  //若该字节8个位全部置位
        {
            ret -= 8;
        }
        else                                //否则查找字节中最后的置零位
        {
            for(uint8 k = 0; k < 8; k++)
            {
                if(str[i] & Bit_Map8[7 - k] == 0)
                {
                    return ret;
                }
                ret--;
            }
        }
        
        num--;
    }

    return 0xFFFF;

}



/************************************************************************
**函数名称: uint16 STR_GetNextBit(uint8* str, uint8 num, uint16 cuint8bool set)
**描 述: 求cur位之后第一个为1或为0的位索引
**
**输 入: str 比特流
         num 比特流长度
         cur 当前所在的位置
         set true 为1; false 为0
**输 出: 0xFFFF: 找不到
         others: 位索引
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
uint16 STR_GetNextBit(uint8* str, uint8 num, uint16 cur, uint8 set)
{
    uint16 ret = cur & 0xFFF8;
    uint16 bytep = cur >> 3;
    uint16 bitp = cur & 0x0007;
    uint16 idx;
    _IF_TRUE_RETURN_Z(cur >= (num << 3), 0xFFFF);
                           
    uint8 tmp = str[bytep];             //获取cur所在的字节
    if(set)
    {
        tmp &= ~Bit_Mask8[bitp + 1];    //清零cur及其之前的位
    }
    else
    {
        tmp |= Bit_Mask8[bitp + 1];     //置一cur之前的位
        tmp = ~tmp;                     //取反
    }

    if(tmp)                             //尚有置一的位
    {
        ret += Bit_Maps[tmp];
        return ret;
    }
    else
    {
        ret += 8;
        bytep++;                        //指向下字节
                                        //获取下字节起首个置一的位索引
        idx = STR_GetFirstBit(&str[bytep], num - bytep, set ? true : false);
        if(idx == 0xFFFF)
        {
            return 0xFFFF;
        }
        else
        {
            ret += idx; 
            return ret;
        }
    }

}


/************************************************************************
**函数名称: uint16 STR_GetNextSetBit(uint8* str, uint8 num, uint16 cur)
**描 述: 求cur位之后第一个置一的位索引
**
**输 入: str 比特流
         num 比特流长度
         cur 当前所在的位置
**输 出: 0xFFFF: 找不到
         others: 位索引
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
uint16 STR_GetNextSetBit(uint8* str, uint8 num, uint16 cur)
{
    return STR_GetNextBit(str, num, cur, true);
}


/************************************************************************
**函数名称: uint16 STR_GetNextClrBit(uint8* str, uint8 num, uint16 cur)
**描 述: 求cur位之后第一个为零的位索引
**
**输 入: str 比特流
         num 比特流长度
         cur 当前所在的位置
**输 出: 0xFFFF: 找不到
         others: 位索引
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
uint16 STR_GetNextClrBit(uint8* str, uint8 num, uint16 cur)
{
    return STR_GetNextBit(str, num, cur, false);
}


/************************************************************************
 * @function: STR_GetThisNextSetBit
 * @描述: 求cur位自身及之后第一个置一的位索引
 * 
 * @参数: 
 * @param: str 
 * @param: num 
 * @param: cur 
 * 
 * @返回: 
 * @return: uint16  
 * @说明: 
 * @作者: yzy (2011-3-16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint16 STR_GetThisNextSetBit(uint8* str, uint8 num, uint16 cur)
{
    _IF_TRUE_RETURN_Z(cur >= (num << 3), 0xFFFF);
    if(STR_IsBitSet(str, cur))
    {
        return cur;
    }
    return STR_GetNextSetBit(str, num, cur);
}


/************************************************************************
 * @function: STR_GetThisNextClrBit
 * @描述: 求cur位自身及之后第一个为零的位索引
 * 
 * @参数: 
 * @param: str 
 * @param: num 
 * @param: cur 
 * 
 * @返回: 
 * @return: uint16  
 * @说明: 
 * @作者: yzy (2011-3-16)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint16 STR_GetThisNextClrBit(uint8* str, uint8 num, uint16 cur)
{
    _IF_TRUE_RETURN_Z(cur >= (num << 3), 0xFFFF);
    if(STR_IsBitClr(str, cur))
    {
        return cur;
    }
    return STR_GetNextClrBit(str, num, cur);
}




/************************************************************************
**函数名称: void STR_Or(uint8* s1, uint8* s2, uint8* des, uint16 num)
**描 述: 将s1和s2比特流相或,输出到des比特流
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
void STR_Or(uint8* s1, uint8* s2, uint8* des, uint16 num)
{
    for(uint16 i = 0; i < num; i++)
    {
        des[i] = s1[i] | s2[i];
    }
}


/************************************************************************
**函数名uint8bool STR_And(uint8* s1, uint8* s2, uint8* des, uint8 num)
**描 述: 将s1和s2比特流相与,输出到des比特流
**
**输 入: 
**输 出: true: 输出为全零比特流
         false: 输出为非零比特流
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
uint8 STR_And(uint8* s1, uint8* s2, uint8* des, uint8 num)
{
    uint8 iszero = 1;

    for(uint8 i = 0; i < num; i++)
    {
        des[i] = s1[i] & s2[i];
        if(des[i] != 0)
        {
            iszero = 0;
        }
    }
    return iszero ? true : false;
}

/************************************************************************
**函数名称: void STR_Xor(uint8* s, uint8* des, uint8 num)
**描 述: des = s1 ^ s2
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 09-11-07
*************************************************************************/
void STR_Xor(uint8* s1, uint8* s2, uint8* des, uint8 num)
{
    for(uint8 i = 0; i < num; i++)
    {
        des[i] = s1[i] ^ s2[i];
    }
}


/************************************************************************
**函数名称: void STR_AndNot(uint8* s1, uint8* s2, uint8* des, uint8 num)
**描 述: des = s1 & ~s2
**       即获取s1中置位而s2中为零的位
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
void STR_AndNot(uint8* s1, uint8* s2, uint8* des, uint8 num)
{
    for(uint8 i = 0; i < num; i++)
    {
        des[i] = s1[i] & (~s2[i]);
    }
}


/************************************************************************
**函数名称: void STR_Not(uint8* s, uint8* des, uint8 num)
**描 述: 将s取反,输出到des比特流
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
void STR_Not(uint8* s, uint8* des, uint8 num)
{
    for(uint8 i = 0; i < num; i++)
    {
        des[i] = ~s[i];
    }
}


/************************************************************************
**函数名称: void STR_SetBits()
**描 述: 设置比特流中某几位
**
**输 入: stream 比特流
        count 需要设置的bit的数量(<=8)
        start 从哪一个位开始设置
        value 设置的值
**输 出: 
**
**备 注: 对 SetStreamBits() 的扩展
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
void STR_SetBits(uint8* stream, uint8 count, uint16 start, uint8 value)
{
    uint16 bytep, bitp;                 //定义字节的起始位置,和bit的起始位置
    Word16 temp;                        //临时变量

    if(count > 8 || count == 0)         //不能超过8个位
    {
        return;                         //返回0
    }
    bytep = start >> 3;                 //字节位置
    bitp = start & 0x7;                 //bit字节内位置

    value &= Bit_Mask8[count];          //消除没有用的数据位

    temp.byte[0] = stream[bytep];       //字节0
    temp.byte[1] = stream[bytep + 1];   //字节1
                                        //获取有效的数据位
    temp.word &= ~(Bit_Mask16[count + bitp] - Bit_Mask16[bitp]);
    temp.word |= (uint16)value << bitp; //得到最终的数据
    stream[bytep] = temp.byte[0];       //字节0

    if(bitp + count > 8)                //判断第二个字节是否需要修正
    {                                   //字节1
        stream[bytep + 1] = temp.byte[1];
    }

}




/************************************************************************
 * @function: STR_IsBitsSet
 * @描述: 判断某些位是否全置一
 * 
 * 
 * @参数: 
 * @param: stream  比特流
 * @param: bit 要判断的起始位数(从0开始)
 * @param: num 要判断的位数
 * 
 * @返回: 
 * @return:  uint8 
 * @作者: yzy (2010-10-7)
 ***----------------------------------------------------------------------
 * @修改人: 
 *************************************************************************/
uint8 STR_IsBitsSet(uint8* stream, uint16 bit, uint8 num)
{
    return STR_IsBitsState(stream, bit, num, true);
}


/************************************************************************
 * @function: STR_IsBitsClr
 * @描述: 判断某些位是否全为零
 * 
 * 
 * @参数: 
 * @param: stream  比特流
 * @param: bit 要判断的起始位数(从0开始)
 * @param: num 要判断的位数
 * 
 * @返回: 
 * @return:  uint8 
 * @作者: yzy (2010-10-7)
 ***----------------------------------------------------------------------
 * @修改人: 
 *************************************************************************/
uint8 STR_IsBitsClr(uint8* stream, uint16 bit, uint8 num)
{
    return STR_IsBitsState(stream, bit, num, false);
}

/************************************************************************
**函数名称: void STR_SetBit(uint8* stream, uint16 bit)
**描 述: 单位置一
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 10-01-26
*************************************************************************/
void STR_SetBit(uint8* stream, uint16 bit)
{
    stream[bit >> 3] |= Bit_Map8[bit & 7];
}


/************************************************************************
**函数名称: STR_GetBits(uint8* stream, uint8 count, uint16 start)
**描 述: 读取比特流中某几位
**
**输 入: steram 比特流
        count 需要读取的bit的数量(<=8)
        start 从哪一个位开始读取
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
uint8 STR_GetBits(uint8* stream, uint8 count, uint16 start)
{
    uint8 bytep, bitp;                  //定义字节的起始位置,和bit的起始位置
    uint8 result;                       //定义结果

    if(count >= 8 || count == 0)        //不能超过8个位
    {
        return 0;                       //返回0
    }

    bytep = start >> 3;                 //字节位置
    bitp = start & 0x7;                 //bit字节内位置

                                        //获取结果构成的字节
    result = (stream[bytep] >> bitp) + (stream[bytep + 1] << (8 - bitp));
    result &= Bit_Mask8[count];         //消掉不需要的位
    return result;                      //返回结果
}



/************************************************************************
**函数名称: void STR_ClrBit(uint8* stream, uint16 bit)
**描 述: 单位置零
**
**输 入: 
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: yzy
**日 期: 10-01-26
*************************************************************************/
void STR_ClrBit(uint8* stream, uint16 bit)
{
    stream[bit >> 3] &= ~Bit_Map8[bit & 7];
}




/************************************************************************
**函数名称: STR_AdvSetBits
**描 述: 设置比特流的若干位
**
**输 入: stream 比特流
        count 需要设置的bit的数量
        start 从哪一个位开始设置
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
void STR_AdvSetBits(uint8* stream, uint16 count, uint16 start)
{
    uint16 bytes, bits;                 //起始位所在的字节索引和字节内位索引
    uint16 bytee, bite;                 //结束位所在的字节索引和字节内位索引

    bytes = start >> 3;                 //
    bits = start & 0x7;                 //
    bytee = (start + count) >> 3;       //该位不用置
    bite = (start + count) & 0x7;       //该位不用置
    
    if(bytes == bytee)
    {
        for(uint16 ui_i = bits; ui_i < bite; ui_i++)
        {
            STR_SetBit(stream + bytes, ui_i);
        }
    }
    else
    {                                   //第一个字节的置位
        for(uint16 ui_i = bits; ui_i < 8; ui_i++)
        {
            STR_SetBit(stream + bytes, ui_i);
        }
                                        //中间字节
        for(uint16 ui_i = bytes + 1; ui_i < bytee; ui_i++)
        {
            stream[ui_i] = 0xFF;
        }
                                        //最后字节的置位
        for(uint16 ui_i = 0; ui_i < bite; ui_i++)
        {
            STR_SetBit(stream + bytee, ui_i);
        }
    }
}


/************************************************************************
**函数名称: STR_AdvClrBits
**描 述: 设置比特流的若干位
**
**输 入: stream 比特流
        count 需要设置的bit的数量
        start 从哪一个位开始设置
**输 出: 
**
**备 注: 
**-----------------------------------------------------------------------
**作 者: 
**日 期: 
*************************************************************************/
void STR_AdvClrBits(uint8* stream, uint16 count, uint16 start)
{
    uint16 bytes, bits;                 //起始位所在的字节索引和字节内位索引
    uint16 bytee, bite;                 //结束位所在的字节索引和字节内位索引

    bytes = start >> 3;                 //
    bits = start & 0x7;                 //
    bytee = (start + count) >> 3;       //该位不用清
    bite = (start + count) & 0x7;       //该位不用清
    
    if(bytes == bytee)
    {
        for(uint16 ui_i = bits; ui_i < bite; ui_i++)
        {
            STR_ClrBit(stream + bytes, ui_i);
        }
    }
    else
    {                                   //第一个字节的置位
        for(uint16 ui_i = bits; ui_i < 8; ui_i++)
        {
            STR_ClrBit(stream + bytes, ui_i);
        }
                                        //中间字节
        for(uint16 ui_i = bytes + 1; ui_i < bytee; ui_i++)
        {
            stream[ui_i] = 0;
        }
                                        //最后字节的置位
        for(uint16 ui_i = 0; ui_i < bite; ui_i++)
        {
            STR_ClrBit(stream + bytee, ui_i);
        }
    }
}




