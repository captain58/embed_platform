/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: stream.h
**��   ��   ��: yzy
**����޸�����: 2011-04-01
**��        ��: �ַ���������ͷ�ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2011-04-01
** �衡��: ԭʼ�汾
********************************************************************************************************/
#ifndef _STRING_H_
#define _STRING_H_


/************************************************************************
**������uint8bool STR_IsBcd(uint8* data, uint8 num)
**�� ��: 
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 2010-12-10
*************************************************************************/
uint8 STR_IsBcd(uint8* data, uint8 num);


/************************************************************************
**��������: uint8 STR_IsEmpty(uint8* s, uint8 num)
**�� ��: ����Ƿ�Ϊ�մ�(ȫΪ��)
**
**�� ��: s ������; no �ֽڳ���
**�� ��: false������
         true�մ�
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
uint8 STR_IsEmpty(uint8* s, uint8 num);


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
uint8 STR_IsBitSet(uint8* stream, uint16 bit);

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
uint8 STR_IsBitClr(uint8* stream, uint16 bit);

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
 * @�޸���: 
 *************************************************************************/
uint8 STR_IsBitsState(uint8* stream, uint16 bit, uint8 num, uint8 oc);


/************************************************************************
**��������: uint16 STR_CalcBitSetNum(uint8* dat, uint8 num)
**�� ��: �ҳ���������dat��1��λ��
**
**�� ��: dat ������
         num �ֽ���
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 
*************************************************************************/
uint16 STR_CalcBitSetNum(uint8* dat, uint8 num);

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
uint16 STR_CalcBitClrNum(uint8* dat, uint8 num);


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
**�� ��: 
**�� ��: 
*************************************************************************/
uint16 STR_CalcBitSetNum_Before(uint8* str, uint16 idx);


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
uint16 STR_GetFirstBit(uint8* str, uint8 num, uint8 set);


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
uint16 STR_GetFirstSetBit(uint8* str, uint8 num);


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
uint16 STR_GetFirstClrBit(uint8* str, uint8 num);



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
uint16 STR_GetSpecSetBit(uint8* dat, uint8 datlen, uint16 xh);

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
uint16 STR_GetSpecClrBit(uint8* dat, uint8 datlen, uint16 xh);


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
**�� ��: 
**�� ��: 
*************************************************************************/
uint16 STR_GetLastSetBit(uint8* str, uint8 num);


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
uint16 STR_GetLastClrBit(uint8* str, uint8 num);


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
uint16 STR_GetNextBit(uint8* str, uint8 num, uint16 cur, uint8 set);



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
uint16 STR_GetNextSetBit(uint8* str, uint8 num, uint16 cur);


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
uint16 STR_GetNextClrBit(uint8* str, uint8 num, uint16 cur);



/************************************************************************
 * @function: STR_GetThisNextSetBit
 * @����: ��curλ������֮���һ����һ��λ����
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
uint16 STR_GetThisNextSetBit(uint8* str, uint8 num, uint16 cur);



/************************************************************************
 * @function: STR_GetThisNextClrBit
 * @����: ��curλ������֮���һ��Ϊ���λ����
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
uint16 STR_GetThisNextClrBit(uint8* str, uint8 num, uint16 cur);


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
void STR_Or(uint8* s1, uint8* s2, uint8* des, uint16 num);

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
uint8 STR_And(uint8* s1, uint8* s2, uint8* des, uint8 num);

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
void STR_Xor(uint8* s1, uint8* s2, uint8* des, uint8 num);

/************************************************************************
**��������: void STR_AndNot(uint8* s1, uint8* s2, uint8* des, uint8 num)
**�� ��: des = s1 && ~s2 
**       ����ȡs1����λ��s2��Ϊ���λ 
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
void STR_AndNot(uint8* s1, uint8* s2, uint8* des, uint8 num);

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
void STR_Not(uint8* s, uint8* des, uint8 num);




/************************************************************************
**��������: void STR_SetBits()
**�� ��: ���ñ�������ĳ��λ
**
**�� ��: steram ������
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
void STR_SetBits(uint8* stream, uint8 count, uint16 start, uint8 value);



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
uint8 STR_IsBitsSet(uint8* stream, uint16 bit, uint8 num);



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
uint8 STR_IsBitsClr(uint8* stream, uint16 bit, uint8 num);



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
void STR_SetBit(uint8* stream, uint16 bit);

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
void STR_ClrBit(uint8* stream, uint16 bit);

/************************************************************************
**��������: STR_AdvSetBits
**�� ��: ���ñ�����������λ
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
void STR_AdvSetBits(uint8* stream, uint16 count, uint16 start);

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
void STR_AdvClrBits(uint8* stream, uint16 count, uint16 start);

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
uint8 STR_GetBits(uint8* stream, uint8 count, uint16 start);

#endif




