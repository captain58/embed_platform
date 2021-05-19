
#include "aos/bdef.h"
#include "cdef.h"

#include "libs.h"

const uint8 Bit_Count4[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};


/************************************************************************
**������uint8bool STR_IsBcd(uint8* data, uint8 num)
**�� ��: �ж��Ƿ�Ϊѹ����bcd��ʽ
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 2010-12-10
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
**��������: uint8 STR_IsEmpty(uint8* s, uint8 num)
**�� ��: �ж��Ƿ�մ�
**
**�� ��: 
**�� ��: false������
         true�մ�
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
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
**������uint8bool STR_IsBitSet(uint8* stream, uint16 bit)
**�� ��: �ж�ĳλ�Ƿ���һ
**
**�� ��: steam ������, bit Ҫ�жϵ�λ��(��0��ʼ)
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 10-01-26
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
**������uint8bool STR_IsBitClr(uint8* stream, uint16 bit)
**�� ��: �ж�ĳλ�Ƿ�Ϊ��
**
**�� ��: steam ������, bit Ҫ�жϵ�λ��(��0��ʼ)
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 10-01-26
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
 * @����: �ж�ĳЩλ�Ƿ�ȫΪ1��0
 * 
 * 
 * @����: 
 * @param: stream  ������
 * @param: bit  Ҫ�жϵ���ʼλ��(��0��ʼ)
 * @param: num  Ҫ�жϵ�λ��
 * @param: oc  ��:Ϊ1,��:Ϊ0
 * 
 * @����: 
 * @return:  uint8   
 * @����: yzy (2010-10-8)
 ***----------------------------------------------------------------------
 * @�޸���: yzy(2010-12-06)�޸���β��������ͬ�ֽ���ʱ�жϴ����BUG
 *************************************************************************/
uint8 STR_IsBitsState(uint8* stream, uint16 bit, uint8 num, uint8 oc)
{
    uint16 byte1, byte2;                //��β���ض�Ӧ���ֽ�
    uint16 bit1, bit2;                  //��β���ض�Ӧ��λ��
    uint16 k;
    uint8 mask1, mask2;
    uint8 cmp1, cmp2, cmpm;
    
    if(num == 0)                        //�����ж�1λ
    {
        num = 1;
    }
                                        //������β���ض�Ӧ���ֽ�
    byte1 = (bit >> 3);
    byte2 = ((bit + num - 1) >> 3);
                                        //��������
    if(byte1 != byte2)                  //��β�����ڲ�ͬ�ֽ�
    {
        bit1 = bit & 0x07;
        bit2 = (bit + num - 1) & 0x07;
        mask1 = ~Bit_Mask8[bit1];
        mask2 = Bit_Mask8[bit2 + 1];
    }
    else                                //��β��������ͬ�ֽ�
    {
        bit1 = bit & 0x07;
        bit2 = bit1 + num;
        mask1 = ((~Bit_Mask8[bit1]) & Bit_Mask8[bit2]);
        mask2 = mask1;
    }
                                        //����Ƚ�ֵ
    cmp1 = oc ? mask1 : 0;
    cmp2 = oc ? mask2 : 0;
    cmpm = oc ? 0xFF : 0;
                                        //�ж����ֽ�
    if((stream[byte1] & mask1) != cmp1)
    {
        return false;
    }
                                        //�ж�β�ֽ�
    if(byte2 > byte1)
    {
        if((stream[byte2] & mask2) != cmp2)
        {
            return false;
        }
    }
                                        //�ж��м��ֽ�
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
**��������: uint16 STR_CalcBitSetNum(uint8* dat, uint8 num)
**�� ��: �ҳ���������dat��1��λ��
**
**�� ��: dat ������
         no �ֽ���
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 09-01-19
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
 * @����: �ҳ���������dat��0��λ��
 * 
 * @����: 
 * @param: dat ������
 * @param: num �ֽ���
 * 
 * @����: 
 * @return: uint16  
 * @˵��: 
 * @����: yzy (2013/6/20)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint16 STR_CalcBitClrNum(uint8* dat, uint8 num)
{
    uint16 all = num << 3;
    uint16 clrnum = all - STR_CalcBitSetNum(dat, num);
    return clrnum;

}



/************************************************************************
**��������: uint16 STR_CalcBitSetNum_Before(uint8* str, uint16 idx)
**�� ��: �����������idxλǰһ���м�λ��1(��idxλ)
**
**�� ��: str ������
         idx Ҫ�������һλ
**�� ��: idxǰ��һ��λ��
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 2009-03-16
*************************************************************************/
uint16 STR_CalcBitSetNum_Before(uint8* str, uint16 idx)
{
    uint16 r;
    uint8 byte;
                                                    //���ֽڵ�λ��
    byte = idx >> 3;
    r = STR_CalcBitSetNum(str, byte);
                                                    //ɢλ��λ��
    idx &= 0x0007;
    byte = str[byte];
    byte &= (1 << (idx + 1))  - 1;
    r += Bit_Count4[byte & 0xf];
    r += Bit_Count4[byte >> 4];

    return r;
}



/************************************************************************
**��������: uint16 STR_GetFirstBit(uint8* str, uint8 nuint8bool set)
**�� ��: ���ұ��������׸�Ϊ0��1��λ
**
**�� ��: str ������
         num ���������ֽ���
         set true Ϊ1, false Ϊ0
**�� ��: 0xFFFF: �Ҳ���
         others: str���׸�Ϊһ��λ
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
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
**��������: uint16 STR_GetFirstSetBit(uint8* str, uint8 num)
**�� ��: ���ұ��������׸�Ϊһ��λ
**
**�� ��: str ������
         num ���������ֽ���
**�� ��: 0xFFFF: �Ҳ���
         others: str���׸�Ϊһ��λ
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
uint16 STR_GetFirstSetBit(uint8* str, uint8 num)
{
    return STR_GetFirstBit(str, num, true);
}

/************************************************************************
**��������: uint16 STR_GetFirstClrBit(uint8* str, uint8 num)
**�� ��: ���ұ��������׸�Ϊ���λ
**
**�� ��: str ������
         num ���������ֽ���
**�� ��: 0xFFFF: �Ҳ���
         others: str���׸�Ϊ���λ
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
uint16 STR_GetFirstClrBit(uint8* str, uint8 num)
{
    return STR_GetFirstBit(str, num, false);
}

/************************************************************************
**��������: uint16 STR_GetSpecSetBit(uint8* dat, uint8 datlen, uint16 xh)
**�� ��: ���ұ������е�N��Ϊ1λ��λ����
**
**�� ��: dat ������
         datlen ���������ֽ���
         xh ��N��λ(0-!)
**�� ��: 0xFFFF: �Ҳ���
         others: �׸�Ϊһ��λ(0~!)
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
uint16 STR_GetSpecSetBit(uint8* dat, uint8 datlen, uint16 xh)
{
    uint16 index = 0;                   //λ����
    uint16 k = 0;                       //�������
    uint8 bytecnt;
    
    if(xh >= (datlen << 3))             //�����Ϸ����ж�
    {
        return 0xFFFF;
    }
    
    for(uint8 uc_i = 0; uc_i < datlen; uc_i++)
    {
        bytecnt = 0;
        bytecnt += Bit_Count4[dat[uc_i] & 0x0F];
        bytecnt += Bit_Count4[dat[uc_i] >> 4];
        if(xh >= k + bytecnt)           //Ҫ�ҵ���ŵ�λ���ڸ��ֽ�,�����������ֽ�
        {
            index += 8;
            k += bytecnt;
            continue;
        }
                                        //�ҵ��ֽ�,ϸ�Ҿ����λ
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
**��������: uint16 STR_GetSpecClrBit(uint8* dat, uint8 datlen, uint16 xh)
**�� ��: ���ұ������е�N��Ϊ0λ��λ����
**
**�� ��: dat ������
         datlen ���������ֽ���
         xh ��N��λ(0-!)
**�� ��: 0xFFFF: �Ҳ���
         others: �׸�Ϊ0��λ(0~!)
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
uint16 STR_GetSpecClrBit(uint8* dat, uint8 datlen, uint16 xh)
{
    uint16 index = 0;                   //λ����
    uint16 k = 0;                       //�������
    uint8 bytecnt;
    
    if(xh >= (datlen << 3))             //�����Ϸ����ж�
    {
        return 0xFFFF;
    }
    
    for(uint8 uc_i = 0; uc_i < datlen; uc_i++)
    {
        bytecnt = 0;
        bytecnt += 4 - Bit_Count4[dat[uc_i] & 0x0F];
        bytecnt += 4 - Bit_Count4[dat[uc_i] >> 4];
        if(xh >= k + bytecnt)           //Ҫ�ҵ���ŵ�λ���ڸ��ֽ�,�����������ֽ�
        {
            index += 8;
            k += bytecnt;
            continue;
        }
                                        //�ҵ��ֽ�,ϸ�Ҿ����λ
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
**��������: uint16 STR_GetLastSetBit(uint8* str, uint8 num)
**�� ��: ���ұ����������һ����һ��λ
**
**�� ��: str ������
         num ���������ֽ���
**�� ��: 0xFFFF: �Ҳ���
         others: λ����
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 2009-03-16
*************************************************************************/
uint16 STR_GetLastSetBit(uint8* str, uint8 num)
{
    uint16 ret;
    uint8 i;

    if(num == 0)                             //�ֽ�����������1
    {
        return 0xFFFF;
    }
    
    ret = num * 8 - 1;
    while(num)
    {
        i = num - 1;

        if(str[i] == 0)                     //�����ֽ�Ϊ��
        {
            ret -= 8;
        }
        else                                //��������ֽ���������һλ
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
**��������: uint16 STR_GetLastClrBit(uint8* str, uint8 num)
**�� ��: ���ұ����������һ�������λ
**
**�� ��: str ������
         num ���������ֽ���
**�� ��: 0xFFFF: �Ҳ���
         others: λ����
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 2009-03-16
*************************************************************************/
uint16 STR_GetLastClrBit(uint8* str, uint8 num)
{
    uint16 ret;
    uint8 i;

    if(num == 0)                            //�ֽ�����������1
    {
        return 0xFFFF;
    }
    
    ret = num * 8 - 1;
    while(num)
    {
        i = num - 1;

        if(str[i] == 0xFF)                  //�����ֽ�8��λȫ����λ
        {
            ret -= 8;
        }
        else                                //��������ֽ�����������λ
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
**��������: uint16 STR_GetNextBit(uint8* str, uint8 num, uint16 cuint8bool set)
**�� ��: ��curλ֮���һ��Ϊ1��Ϊ0��λ����
**
**�� ��: str ������
         num ����������
         cur ��ǰ���ڵ�λ��
         set true Ϊ1; false Ϊ0
**�� ��: 0xFFFF: �Ҳ���
         others: λ����
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
uint16 STR_GetNextBit(uint8* str, uint8 num, uint16 cur, uint8 set)
{
    uint16 ret = cur & 0xFFF8;
    uint16 bytep = cur >> 3;
    uint16 bitp = cur & 0x0007;
    uint16 idx;
    _IF_TRUE_RETURN_Z(cur >= (num << 3), 0xFFFF);
                           
    uint8 tmp = str[bytep];             //��ȡcur���ڵ��ֽ�
    if(set)
    {
        tmp &= ~Bit_Mask8[bitp + 1];    //����cur����֮ǰ��λ
    }
    else
    {
        tmp |= Bit_Mask8[bitp + 1];     //��һcur֮ǰ��λ
        tmp = ~tmp;                     //ȡ��
    }

    if(tmp)                             //������һ��λ
    {
        ret += Bit_Maps[tmp];
        return ret;
    }
    else
    {
        ret += 8;
        bytep++;                        //ָ�����ֽ�
                                        //��ȡ���ֽ����׸���һ��λ����
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
**��������: uint16 STR_GetNextSetBit(uint8* str, uint8 num, uint16 cur)
**�� ��: ��curλ֮���һ����һ��λ����
**
**�� ��: str ������
         num ����������
         cur ��ǰ���ڵ�λ��
**�� ��: 0xFFFF: �Ҳ���
         others: λ����
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
uint16 STR_GetNextSetBit(uint8* str, uint8 num, uint16 cur)
{
    return STR_GetNextBit(str, num, cur, true);
}


/************************************************************************
**��������: uint16 STR_GetNextClrBit(uint8* str, uint8 num, uint16 cur)
**�� ��: ��curλ֮���һ��Ϊ���λ����
**
**�� ��: str ������
         num ����������
         cur ��ǰ���ڵ�λ��
**�� ��: 0xFFFF: �Ҳ���
         others: λ����
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
uint16 STR_GetNextClrBit(uint8* str, uint8 num, uint16 cur)
{
    return STR_GetNextBit(str, num, cur, false);
}


/************************************************************************
 * @function: STR_GetThisNextSetBit
 * @����: ��curλ����֮���һ����һ��λ����
 * 
 * @����: 
 * @param: str 
 * @param: num 
 * @param: cur 
 * 
 * @����: 
 * @return: uint16  
 * @˵��: 
 * @����: yzy (2011-3-16)
 *-----------------------------------------------------------------------
 * @�޸���: 
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
 * @����: ��curλ����֮���һ��Ϊ���λ����
 * 
 * @����: 
 * @param: str 
 * @param: num 
 * @param: cur 
 * 
 * @����: 
 * @return: uint16  
 * @˵��: 
 * @����: yzy (2011-3-16)
 *-----------------------------------------------------------------------
 * @�޸���: 
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
**��������: void STR_Or(uint8* s1, uint8* s2, uint8* des, uint16 num)
**�� ��: ��s1��s2���������,�����des������
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
void STR_Or(uint8* s1, uint8* s2, uint8* des, uint16 num)
{
    for(uint16 i = 0; i < num; i++)
    {
        des[i] = s1[i] | s2[i];
    }
}


/************************************************************************
**������uint8bool STR_And(uint8* s1, uint8* s2, uint8* des, uint8 num)
**�� ��: ��s1��s2����������,�����des������
**
**�� ��: 
**�� ��: true: ���Ϊȫ�������
         false: ���Ϊ���������
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
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
**��������: void STR_Xor(uint8* s, uint8* des, uint8 num)
**�� ��: des = s1 ^ s2
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 09-11-07
*************************************************************************/
void STR_Xor(uint8* s1, uint8* s2, uint8* des, uint8 num)
{
    for(uint8 i = 0; i < num; i++)
    {
        des[i] = s1[i] ^ s2[i];
    }
}


/************************************************************************
**��������: void STR_AndNot(uint8* s1, uint8* s2, uint8* des, uint8 num)
**�� ��: des = s1 & ~s2
**       ����ȡs1����λ��s2��Ϊ���λ
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
void STR_AndNot(uint8* s1, uint8* s2, uint8* des, uint8 num)
{
    for(uint8 i = 0; i < num; i++)
    {
        des[i] = s1[i] & (~s2[i]);
    }
}


/************************************************************************
**��������: void STR_Not(uint8* s, uint8* des, uint8 num)
**�� ��: ��sȡ��,�����des������
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
void STR_Not(uint8* s, uint8* des, uint8 num)
{
    for(uint8 i = 0; i < num; i++)
    {
        des[i] = ~s[i];
    }
}


/************************************************************************
**��������: void STR_SetBits()
**�� ��: ���ñ�������ĳ��λ
**
**�� ��: stream ������
        count ��Ҫ���õ�bit������(<=8)
        start ����һ��λ��ʼ����
        value ���õ�ֵ
**�� ��: 
**
**�� ע: �� SetStreamBits() ����չ
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
void STR_SetBits(uint8* stream, uint8 count, uint16 start, uint8 value)
{
    uint16 bytep, bitp;                 //�����ֽڵ���ʼλ��,��bit����ʼλ��
    Word16 temp;                        //��ʱ����

    if(count > 8 || count == 0)         //���ܳ���8��λ
    {
        return;                         //����0
    }
    bytep = start >> 3;                 //�ֽ�λ��
    bitp = start & 0x7;                 //bit�ֽ���λ��

    value &= Bit_Mask8[count];          //����û���õ�����λ

    temp.byte[0] = stream[bytep];       //�ֽ�0
    temp.byte[1] = stream[bytep + 1];   //�ֽ�1
                                        //��ȡ��Ч������λ
    temp.word &= ~(Bit_Mask16[count + bitp] - Bit_Mask16[bitp]);
    temp.word |= (uint16)value << bitp; //�õ����յ�����
    stream[bytep] = temp.byte[0];       //�ֽ�0

    if(bitp + count > 8)                //�жϵڶ����ֽ��Ƿ���Ҫ����
    {                                   //�ֽ�1
        stream[bytep + 1] = temp.byte[1];
    }

}




/************************************************************************
 * @function: STR_IsBitsSet
 * @����: �ж�ĳЩλ�Ƿ�ȫ��һ
 * 
 * 
 * @����: 
 * @param: stream  ������
 * @param: bit Ҫ�жϵ���ʼλ��(��0��ʼ)
 * @param: num Ҫ�жϵ�λ��
 * 
 * @����: 
 * @return:  uint8 
 * @����: yzy (2010-10-7)
 ***----------------------------------------------------------------------
 * @�޸���: 
 *************************************************************************/
uint8 STR_IsBitsSet(uint8* stream, uint16 bit, uint8 num)
{
    return STR_IsBitsState(stream, bit, num, true);
}


/************************************************************************
 * @function: STR_IsBitsClr
 * @����: �ж�ĳЩλ�Ƿ�ȫΪ��
 * 
 * 
 * @����: 
 * @param: stream  ������
 * @param: bit Ҫ�жϵ���ʼλ��(��0��ʼ)
 * @param: num Ҫ�жϵ�λ��
 * 
 * @����: 
 * @return:  uint8 
 * @����: yzy (2010-10-7)
 ***----------------------------------------------------------------------
 * @�޸���: 
 *************************************************************************/
uint8 STR_IsBitsClr(uint8* stream, uint16 bit, uint8 num)
{
    return STR_IsBitsState(stream, bit, num, false);
}

/************************************************************************
**��������: void STR_SetBit(uint8* stream, uint16 bit)
**�� ��: ��λ��һ
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 10-01-26
*************************************************************************/
void STR_SetBit(uint8* stream, uint16 bit)
{
    stream[bit >> 3] |= Bit_Map8[bit & 7];
}


/************************************************************************
**��������: STR_GetBits(uint8* stream, uint8 count, uint16 start)
**�� ��: ��ȡ��������ĳ��λ
**
**�� ��: steram ������
        count ��Ҫ��ȡ��bit������(<=8)
        start ����һ��λ��ʼ��ȡ
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
uint8 STR_GetBits(uint8* stream, uint8 count, uint16 start)
{
    uint8 bytep, bitp;                  //�����ֽڵ���ʼλ��,��bit����ʼλ��
    uint8 result;                       //������

    if(count >= 8 || count == 0)        //���ܳ���8��λ
    {
        return 0;                       //����0
    }

    bytep = start >> 3;                 //�ֽ�λ��
    bitp = start & 0x7;                 //bit�ֽ���λ��

                                        //��ȡ������ɵ��ֽ�
    result = (stream[bytep] >> bitp) + (stream[bytep + 1] << (8 - bitp));
    result &= Bit_Mask8[count];         //��������Ҫ��λ
    return result;                      //���ؽ��
}



/************************************************************************
**��������: void STR_ClrBit(uint8* stream, uint16 bit)
**�� ��: ��λ����
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 10-01-26
*************************************************************************/
void STR_ClrBit(uint8* stream, uint16 bit)
{
    stream[bit >> 3] &= ~Bit_Map8[bit & 7];
}




/************************************************************************
**��������: STR_AdvSetBits
**�� ��: ���ñ�����������λ
**
**�� ��: stream ������
        count ��Ҫ���õ�bit������
        start ����һ��λ��ʼ����
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
void STR_AdvSetBits(uint8* stream, uint16 count, uint16 start)
{
    uint16 bytes, bits;                 //��ʼλ���ڵ��ֽ��������ֽ���λ����
    uint16 bytee, bite;                 //����λ���ڵ��ֽ��������ֽ���λ����

    bytes = start >> 3;                 //
    bits = start & 0x7;                 //
    bytee = (start + count) >> 3;       //��λ������
    bite = (start + count) & 0x7;       //��λ������
    
    if(bytes == bytee)
    {
        for(uint16 ui_i = bits; ui_i < bite; ui_i++)
        {
            STR_SetBit(stream + bytes, ui_i);
        }
    }
    else
    {                                   //��һ���ֽڵ���λ
        for(uint16 ui_i = bits; ui_i < 8; ui_i++)
        {
            STR_SetBit(stream + bytes, ui_i);
        }
                                        //�м��ֽ�
        for(uint16 ui_i = bytes + 1; ui_i < bytee; ui_i++)
        {
            stream[ui_i] = 0xFF;
        }
                                        //����ֽڵ���λ
        for(uint16 ui_i = 0; ui_i < bite; ui_i++)
        {
            STR_SetBit(stream + bytee, ui_i);
        }
    }
}


/************************************************************************
**��������: STR_AdvClrBits
**�� ��: ���ñ�����������λ
**
**�� ��: stream ������
        count ��Ҫ���õ�bit������
        start ����һ��λ��ʼ����
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
void STR_AdvClrBits(uint8* stream, uint16 count, uint16 start)
{
    uint16 bytes, bits;                 //��ʼλ���ڵ��ֽ��������ֽ���λ����
    uint16 bytee, bite;                 //����λ���ڵ��ֽ��������ֽ���λ����

    bytes = start >> 3;                 //
    bits = start & 0x7;                 //
    bytee = (start + count) >> 3;       //��λ������
    bite = (start + count) & 0x7;       //��λ������
    
    if(bytes == bytee)
    {
        for(uint16 ui_i = bits; ui_i < bite; ui_i++)
        {
            STR_ClrBit(stream + bytes, ui_i);
        }
    }
    else
    {                                   //��һ���ֽڵ���λ
        for(uint16 ui_i = bits; ui_i < 8; ui_i++)
        {
            STR_ClrBit(stream + bytes, ui_i);
        }
                                        //�м��ֽ�
        for(uint16 ui_i = bytes + 1; ui_i < bytee; ui_i++)
        {
            stream[ui_i] = 0;
        }
                                        //����ֽڵ���λ
        for(uint16 ui_i = 0; ui_i < bite; ui_i++)
        {
            STR_ClrBit(stream + bytee, ui_i);
        }
    }
}




