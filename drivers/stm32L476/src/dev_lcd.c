/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#define EXT_DEV_LCD
#include "include.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	#include "lib_app.h"
//	#include "hal.h"

/***********************************************************************************************************************
 Extern
***********************************************************************************************************************/
//	extern void MCU_LcdOpen(void);
//	extern void MCU_LcdClose(void);
//	extern void MCU_LcdAllSet(void);
//	extern void MCU_LcdWriteReg(unsigned char Seg, unsigned char Reg);
//	extern unsigned char MCU_LcdReadReg(unsigned char Seg);
//	extern void MCU_LcdInit(void);

//	#define LcdWriteReg(a,b) MCU_LcdWriteReg(a,b)
//	#define LcdReadReg(a) MCU_LcdReadReg(a)
//	#define LcdAllSet() MCU_LcdAllSet()
//	#define LcdOn()     MCU_LcdOpen()
//	#define LcdOff()    MCU_LcdClose()
//	#define LcdInit()   MCU_LcdInit()

//extern void LcdWriteReg(unsigned char Seg, unsigned char Reg);
//extern unsigned char LcdReadReg(unsigned char Seg);
//extern void LcdAllSet(void);
//extern void LcdInit(void);
//extern void LcdOn(void);
//extern void LcdOff(void);
void SYS_LCD_Init(void);
/***********************************************************************************************************************
 Pre
***********************************************************************************************************************/
void LcdWriteBuf(unsigned char index, unsigned char buf);

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/**
 * ��ʾ���ݻ���
 * 
 * @author Zzzzz (2014-11-09)
 */
unsigned char LcdDataBuf[21];

/**
 * ������ֵ��
 *
 * @author Zzzzz (2014-11-09)
 */
const char LcdDigit[] =
{
    //����
    0x5f,//0xaf,  // '0'
    0x50,//0x06,  // '1'
    0x6b,//0x6d,  // '2'
    0x79,//0x4f,  // '3'
    0x74,//0xc6,  // '4'
    0x3d,//0xcb,  // '5'
    0x3f,//0xeb,  // '6'
    0x58,//0x0e,  // '7'
    0x7f,//0xef,  // '8'
    0x7d,//0xcf,   // '9'
    0x7e,//0xee,   //A 10
    0x37,//0xe3,   //b 11
    0x0f,//0xa9,   //c 12
    0x73,//0x67,   //d 13
    0x2f,//0xe9,   //E 14
    0x2e,//0xe8,   //F 15
    //�ַ�
    0x20,//0x40,   //- 16
    0x76,//0xe6,   //H 17
    0x07,//0xa1,   //L 18
    0x32,//0x62,   //n 19
    0x5f,//0x23,   //O 20
    0x6e,//0xec,   //p 21
    0x22,//0x60,   //r 22
    0x33,//0x63,   //o 23
    0x00,          //  24
    0x27,          //t 25
    0x7d,//0xab,   //g 26
    0x5e,//0xae,   //N 27
    0x57,//0xa7,   //U 28
    0xa8,          //R 29
    0x10,//0x02,   //i 30
    0x23,//0xa9,   //c 31
};

/**
 * д�Ĵ�����λ
 * 
 * @author Zzzzz (2014-11-09)
 * 
 * @param index 
 *          �Ĵ������
 * @param buf
 *           
 */
void LcdWriteBuf(unsigned char index, unsigned char buf)
{
    LcdDataBuf[index] &= 0xf0;
    LcdDataBuf[index] |= buf & 0x0f;
    Chip_LCD_WriteReg(guc_SEG_Tbl[index], LcdDataBuf[index]);//WriteLcdReg(index, LcdDataBuf[index]);
}

/**
 * д�Ĵ�����λ
 * 
 * @author Zzzzz (2014-11-09)
 * 
 * @param index
 *          �Ĵ������
 * @param buf 
 */
void LcdWriteBufHi(unsigned char index, unsigned char buf)
{
    LcdDataBuf[index] &= 0x0f;
    LcdDataBuf[index] |= buf << 4;
    LcdDataBuf[index] |= buf & 0x10;
    Chip_LCD_WriteReg(guc_SEG_Tbl[index], LcdDataBuf[index]);//WriteLcdReg(index, LcdDataBuf[index]);
}

/** 
 * ��λ 
 * 
 * @author Zzzzz (2014-11-09)
 * 
 * @param index 
 *          �Ĵ������
 * @param bits 
 *          LCD_DISP_BIT_0��LCD_DISP_BIT_7 
 */
void LcdSetBit(unsigned char index,unsigned char bits)
{
    LcdDataBuf[index] |= bits;
    Chip_LCD_WriteReg(guc_SEG_Tbl[index], LcdDataBuf[index]);//WriteLcdReg(index, LcdDataBuf[index]);
}

/**
 * ��λ
 *
 * @author Zzzzz (2014-11-09)
 * 
 * @param index
 *          �Ĵ������ 
 * @param bits 
 *          LCD_DISP_BIT_0��LCD_DISP_BIT_7
 */
void LcdResetBit(unsigned char index,unsigned char bits)
{
    LcdDataBuf[index] &= ~(bits);
    Chip_LCD_WriteReg(guc_SEG_Tbl[index], LcdDataBuf[index]);//WriteLcdReg(index, LcdDataBuf[index]);
}

/**
 * ������ʾ��ʶ
 * 
 * @author Zzzzz (2014-11-09)
 *
 * @param MarkIndex 
 *          ��ʶ��� 
 * @param t
 *          0-����ʾ 1-��ʾ 
 */
void OV_LcdDispMark(_EnumMark MarkIndex,char t)
{
    switch(MarkIndex)
    {
    case MARK_AMOUNT:
        t?(LcdSetBit(2,2)):(LcdResetBit(2,2));
        break;
    case MARK_PRICE:
        t?(LcdSetBit(6,2)):(LcdResetBit(6,2));
        break;
    case MARK_REMAIN:
        t?(LcdSetBit(2,4)):(LcdResetBit(2,4));
        break;
    case MARK_STEP:
        t?(LcdSetBit(5,8)):(LcdResetBit(5,8));
        break;
    case MARK_SIGN1:
        t?(LcdSetBit(5,1)):(LcdResetBit(5,1));
        break;
    case MARK_SIGN2:
        t?(LcdSetBit(5,2)):(LcdResetBit(5,2));
        break;
    case MARK_SIGN3:
        t?(LcdSetBit(5,4)):(LcdResetBit(5,4));
        break;
    case MARK_ERR:
        t?(LcdSetBit(6,1)):(LcdResetBit(6,1));
        break;
    case MARK_GAS:
        t?(LcdSetBit(1,1)):(LcdResetBit(1,1));
        break;
    /*case MARK_CURRENT:
        t?(LcdSetBit(14,7)):(LcdResetBit(14,7));
        break;*/
    case MARK_MONEY:
        t?(LcdSetBit(1,2)):(LcdResetBit(1,2));
        break;
    /*case MARK_PAST:
        t?(LcdSetBit(14,4)):(LcdResetBit(14,4));
        break;*/
    case MARK_MONTH:
        t?(LcdSetBit(2,1)):(LcdResetBit(2,1));
        break;
    case MARK_OPEN:
        t?(LcdSetBit(6,8)):(LcdResetBit(6,8));
        break;
    case MARK_CLOSE:
        t?(LcdSetBit(0,1)):(LcdResetBit(0,1));
        break;
    case MARK_BATT1:
        t?(LcdSetBit(0,2)):(LcdResetBit(0,2));
        break;
    case MARK_BATT2:
        t?(LcdSetBit(0,4)):(LcdResetBit(0,4));
        break;
    case MARK_BATT3:
        t?(LcdSetBit(0,8)):(LcdResetBit(0,8));
        break;
    case MARK_BATT_FREAM:
        t?(LcdSetBit(0,2)):(LcdResetBit(0,2));
        break;
    /*case MARK_CHECK:
        t?(LcdSetBit(1,4)):(LcdResetBit(1,4));
        break;*/
    case MARK_CONSUMED:
        t?(LcdSetBit(2,8)):(LcdResetBit(2,8));
        break;
    case MARK_FREEZE:
        t?(LcdSetBit(1,4)):(LcdResetBit(1,4));
        break;
    case MARK_BACKSLASH:
        t?(LcdSetBit(8,8)):(LcdResetBit(8,8));
        break;
   /* case MARK_PRESS:
        t?(LcdSetBit(0,6)):(LcdResetBit(0,6));
        break;*/
    case MARK_CHARGE:
        t?(LcdSetBit(1,8)):(LcdResetBit(1,8));
        break;
    case MARK_YAN:
        t?(LcdSetBit(20,8)):(LcdResetBit(20,8));
        break;
    case MARK_M3:
        t?(LcdSetBit(6,4)):(LcdResetBit(6,4));
        break;
    case MARK_1:
        t?(LcdSetBit(4,8)):(LcdResetBit(4,8));
        break;
    case MARK_2POINT:
        t?(LcdSetBit(10,8)):(LcdResetBit(10,8));
        break;
    default:
        break;
    }
}

/**
 * ��ʾС����
 * 
 * @author Zzzzz (2014-11-09)
 * 
 * @param line 
 * @param dots
 */

/*void LcdSetDots(char line,unsigned int dots)
{
    if(line==1)
    {
        switch(dots)
        {
        case 1:
            LcdSetBit(2,4);
            LcdResetBit(4,4);
            LcdResetBit(6,4);
            LcdResetBit(8,4);
            break;
        case 2:
            LcdSetBit(4,4);
            LcdResetBit(2,4);
            LcdResetBit(6,4);
            LcdResetBit(8,4);
            break;
        case 3:
            LcdSetBit(6,4);
            LcdResetBit(2,4);
            LcdResetBit(4,4);
            LcdResetBit(8,4);
            break;
        case 4:
            LcdSetBit(8,4);
            LcdResetBit(2,4);
            LcdResetBit(4,4);
            LcdResetBit(6,4);
            break;
        default:
            LcdResetBit(2,4);
            LcdResetBit(4,4);
            LcdResetBit(6,4);
            LcdResetBit(8,4);
            break;
        }
    }
    else
    {*/
void LcdSetDots(unsigned int dots)
{
        switch(dots)
        {
        case 1:
            LcdResetBit(12,8);
            LcdResetBit(14,8);
            LcdResetBit(16,8);
            LcdSetBit(18,8);
            break;
        case 2:
            LcdResetBit(12,8);
            LcdResetBit(14,8);
            LcdSetBit(16,8);
            LcdResetBit(18,8);
            break;
        case 3:
            LcdResetBit(12,8);
            LcdSetBit(14,8);
            LcdResetBit(16,8);
            LcdResetBit(18,8);
            break;
        case 4:
            LcdSetBit(12,8);
            LcdResetBit(14,8);
            LcdResetBit(16,8);
            LcdResetBit(18,8);
            break;
        default:
            LcdResetBit(12,8);
            LcdResetBit(14,8);
            LcdResetBit(16,8);
            LcdResetBit(18,8);
            break;
        }
    }

void OV_LcdClearValueMark(void)
{
    OV_DispMonth(0);
    OV_LcdDispMark(MARK_AMOUNT,0);
    OV_LcdDispMark(MARK_PRICE,0);
    OV_LcdDispMark(MARK_REMAIN,0);
    OV_LcdDispMark(MARK_STEP,0);
    OV_LcdDispMark(MARK_GAS,0);
    OV_LcdDispMark(MARK_MONEY,0);
    OV_LcdDispMark(MARK_MONTH,0);
    OV_LcdDispMark(MARK_CONSUMED,0);
    OV_LcdDispMark(MARK_FREEZE,0);
    OV_LcdDispMark(MARK_BACKSLASH,0);
    OV_LcdDispMark(MARK_CHARGE,0);
    OV_LcdDispMark(MARK_YAN,0);
    OV_LcdDispMark(MARK_M3,0);
    OV_LcdDispMark(MARK_1,0);
    OV_LcdDispMark(MARK_2POINT,0);
    LcdSetDots(0);
}

/**
 * ��ʾ�ַ�
 * 
 * @author Zzzzz (2014-11-09)
 * 
 * @param index
 *          �ַ����
 * @param ch 
 *          ��������
 */
void LcdDispChar(unsigned char index, unsigned char ch)
{
    if(index < 7)
    {
        LcdWriteBuf(8 + index * 2, LcdDigit[ch] >> 4 | (LcdDataBuf[8 + index * 2] & 0x08));
        LcdWriteBuf(7 + index * 2, LcdDigit[ch] & 0x0f);
    }
    else
    {
        index=2;
        LcdWriteBuf(8 - index * 2, LcdDigit[ch] >> 4 | (LcdDataBuf[8 + index * 2] & 0x00));
        LcdWriteBuf(7 - index * 2, LcdDigit[ch] & 0x0f);
    }
}

/**
 * ��LcdDispChar������ͬ�Ķ���ӿ�
 *  
 * @author Zzzzz (2014-11-09)
 *
 * @param index 
 * @param ch 
 */
void OV_LcdDispChar(unsigned char index, unsigned char ch)
{
    LcdDispChar(index,ch);
}

/**
 * ˢ����ʾ
 * 
 * @author Zzzzz (2014-11-09)
 * 
 * @param tLcdDataBuf 
 *          ��ʾ�ļĴ�������
 */
void OV_LcdRefresh(void)
{
    unsigned char index;
    for(index=0;index<21;index++)
    {
        Chip_LCD_WriteReg(guc_SEG_Tbl[index], LcdDataBuf[index]);//WriteLcdReg(index, LcdDataBuf[index]);
    }
}

/**
 * �����Ļ��ʾ
 * 
 * @author Zzzzz (2014-11-09)
 */
void OV_LcdClear(void)
{
     unsigned char i;
    for(i = 0; i < 21; i++)
    {
        LcdDataBuf[i]=0;
        LcdWriteBuf(i, 0);
    }//MCU_LcdAllClr();
}

/**
 * ȫ��
 * 
 * @author Zzzzz (2014-11-09)
 */
void OV_LcdDispAll(void)
{
     unsigned char i;
    for(i = 0; i < 21; i++)
    {
        LcdDataBuf[i]=0xff;
        LcdWriteBuf(i, 0xff);
    }//MCU_LcdAllClr();
    //MCU_LcdAllSet();
}

/**
 * �����Ļ������
 * 
 * @author Zzzzz (2014-11-09)
 *
 * @param line 
 */
void OV_LcdClearNum(void)
{
    unsigned char i;
    
    for(i=0;i<8;i++)
    {
        LcdDispChar(i, LCD_CHAR_BLANK);
    }
    LcdResetBit(12,8);
    LcdResetBit(14,8);
    LcdResetBit(16,8);
    LcdResetBit(18,8);
}

/**
 * ��ʾ��һ�е�����
 * 
 * @author Zzzzz (2014-11-09)
 * 
 * @param num
 *          ��ֵ 
 * @param forceN 
 *          С����λ��
 */
/*void LcdDispNumLine1(long num,char forceN)
{
    unsigned char k,DispCH;
    num%=100000;
    k=0;
    DispCH=num/10000;
    num-=(long)DispCH*10000;
    if(k==1 || DispCH!=0 || forceN>=5) 
    {
        k=1;
        LcdDispChar(7,DispCH);
    }
    else
    {
        LcdDispChar(7,LCD_CHAR_BLANK);
    }
    DispCH=num/1000;
    num-=(long)DispCH*1000;
    if(k==1 || DispCH!=0 || forceN>=4) 
    {
        k=1;
        LcdDispChar(8,DispCH);
    }
    else
    {
        LcdDispChar(8,LCD_CHAR_BLANK);
    }
    DispCH=num/100;
    num-=(long)DispCH*100;
    if(k==1 || DispCH!=0 || forceN>=3) 
    {
        k=1;
        LcdDispChar(9,DispCH);
    }
    else
    {
        LcdDispChar(9,LCD_CHAR_BLANK);
    }
    DispCH=num/10;
    num-=(long)DispCH*10;
    if(k==1 || DispCH!=0 || forceN>=2)
    {
        k=1;
        LcdDispChar(10,DispCH);
    }
    else
    {
        LcdDispChar(10,LCD_CHAR_BLANK);
    }
    DispCH=num;
    LcdDispChar(11,DispCH);
}*/

/**
 * ��ʾ�ڶ��е�����
 *
 * @author Zzzzz (2014-11-09)
 * 
 * @param num
 * @param forceN 
 */
/*void LcdDispNumLine2(long num,char forceN)
{
    unsigned char k,DispCH;
    num%=10000000;
    k=0;
    DispCH=num/1000000;
    num-=(long)DispCH*1000000;
    if(k==1 || DispCH!=0 || forceN>=7) 
    {
        k=1;
        LcdDispChar(0,DispCH);
    }
    else
    {
        LcdDispChar(0,LCD_CHAR_BLANK);
    }
    
    DispCH=num/100000;
    num-=(long)DispCH*100000;
    if(k==1 || DispCH!=0 || forceN>=6) 
    {
        k=1;
        LcdDispChar(1,DispCH);
    }
    else
    {
        LcdDispChar(1,LCD_CHAR_BLANK);
    }
    
    DispCH=num/10000;
    num-=(long)DispCH*10000;
    if(k==1 || DispCH!=0 || forceN>=5) 
    {
        k=1;
        LcdDispChar(2,DispCH);
    }
    else
    {
        LcdDispChar(2,LCD_CHAR_BLANK);
    }
    
    DispCH=num/1000;
    num-=(long)DispCH*1000;
    if(k==1 || DispCH!=0 || forceN>=4) 
    {
        k=1;
        LcdDispChar(3,DispCH);
    }
    else
    {
        LcdDispChar(3,LCD_CHAR_BLANK);
    }
    
    DispCH=num/100;
    num-=(long)DispCH*100;
    if(k==1 || DispCH!=0 || forceN>=3) 
    {
        k=1;
        LcdDispChar(4,DispCH);
    }
    else
    {
        LcdDispChar(4,LCD_CHAR_BLANK);
    }
    
    DispCH=num/10;
    num-=(long)DispCH*10;
    if(k==1 || DispCH!=0 || forceN>=2) 
    {
        k=1;
        LcdDispChar(5,DispCH);
    }
    else
    {
        LcdDispChar(5,LCD_CHAR_BLANK);
    }
    
    DispCH=num;
    LcdDispChar(6,DispCH);
}*/
void LcdDispNumLine2(long num,char forceN)
{
    unsigned char k,DispCH,Mark;
    Mark=0;
    if(num<0)
    {
        num=-num;
        Mark=1;
    }
    num%=10000000;
    k=0;
    DispCH=num/1000000;
    num-=(long)DispCH*1000000;
    if(k>0 || DispCH!=0 || forceN>=7) 
    {
        k=1;
        LcdDispChar(0,DispCH);
    }
    else
    {
        LcdDispChar(0,LCD_CHAR_BLANK);
    }
    
    DispCH=num/100000;
    num-=(long)DispCH*100000;
    if(k>0 || DispCH!=0 || forceN>=6) 
    {
        if(k==0)
            k=2;
        LcdDispChar(1,DispCH);
    }
    else
    {
        LcdDispChar(1,LCD_CHAR_BLANK);
    }
    
    DispCH=num/10000;
    num-=(long)DispCH*10000;
    if(k>0 || DispCH!=0 || forceN>=5) 
    {
        if(k==0)
            k=3;
        LcdDispChar(2,DispCH);
    }
    else
    {
        LcdDispChar(2,LCD_CHAR_BLANK);
    }
    
    DispCH=num/1000;
    num-=(long)DispCH*1000;
    if(k>0 || DispCH!=0 || forceN>=4) 
    {
        if(k==0)
            k=4;
        LcdDispChar(3,DispCH);
    }
    else
    {
        LcdDispChar(3,LCD_CHAR_BLANK);
    }
    
    DispCH=num/100;
    num-=(long)DispCH*100;
    if(k>0 || DispCH!=0 || forceN>=3) 
    {
        if(k==0)
            k=5;
        LcdDispChar(4,DispCH);
    }
    else
    {
        LcdDispChar(4,LCD_CHAR_BLANK);
    }
    
    DispCH=num/10;
    num-=(long)DispCH*10;
    if(k>0 || DispCH!=0 || forceN>=2) 
    {
        if(k==0)
            k=6;
        LcdDispChar(5,DispCH);
    }
    else
    {
        LcdDispChar(5,LCD_CHAR_BLANK);
    }
    
    DispCH=num;
    LcdDispChar(6,DispCH);
    
    if(Mark)
    {
        if(k>=2)
            LcdDispChar(k-2,LCD_CHAR_SEPARATE);
        else
            LcdDispChar(0,LCD_CHAR_SEPARATE);
    }
}

/**
 * ��ʾ��С�������ֵ
 *  
 * @author Zzzzz (2014-11-09)
 * @param num 
 *          ��ֵ
 * @param dots 
 *          С����λ�� 
 */
void OV_LcdDispNumWithDot(long num,char dots)
{
    unsigned char numlen,t=1;
    numlen = dots>>4;
    dots  &= 0x0f;
    while((dots>0) && t)
    {
        if(num%10==0)
        {
            dots-=1;
            num/=10;
        }
        else
        {
            t=0;
        }
    }
    while(num>9999999)
    {
        if(dots>0)
        {
            num/=10;
            dots-=1;
        }
        else
        {
            num=num%10000000;
        }
    }
    LcdSetDots(dots);
    /*if(line==ExtLine)
    {
        LcdDispNumLine1(num,dots+1);
    }
    else 
    */ 
    {
        if(numlen>0)
        {
            LcdDispNumLine2(num,numlen);
        }
        else
        {
            LcdDispNumLine2(num,dots+1);
        }
    }
}
/**
 *��ʾʣ������
 *  
 * num-��ֵ  dots-С����λ��  b-�·� 
*/
void OV_DispRemainGas(long num,char dots,unsigned int b)
{   
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_REMAIN),1);
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
   
}
/**
 * ��ʾʣ����
 * 
 * @author Administrator (2014-11-21) 
 * num-��ֵ  dots-С����λ��  b-�·� 
 */
void OV_DispRemainMoney(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_REMAIN),1);
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);

}
/**
 * ��ʾ��������
 * 
 * @author Administrator (2014-11-21) 
 * num-��ֵ  dots-С����λ��  b-�·� 
 */
void OV_DispConsumedGas(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_CONSUMED),1);
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);

}
/**
 * ��ʾ���ѽ��
 * 
 * @author Administrator (2014-11-21) 
 * num-��ֵ  dots-С����λ��  b-�·� 
 */
void OV_DispConsumedMoney(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_CONSUMED),1);
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);
}
/**
 * ��ʾ��ֵ����
 * 
 * @author Administrator (2014-11-21) 
 * num-��ֵ  dots-С����λ��  b-�·� 
 */
void OV_DispChargeGas(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_CHARGE),1);
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);
}
/**
 * ��ʾ��ֵ���
 * 
 * @author Administrator (2014-11-21) 
 * num-��ֵ  dots-С����λ��  b-�·� 
 */
void OV_DispChargeMoney(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_CHARGE),1);
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);
}
/**
 * ��ʾ�ۻ�����
 * 
 * @author Administrator (2014-11-21) 
 * num-��ֵ  dots-С����λ��  b-�·� 
 */
void OV_DispAmountGas(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_AMOUNT),1);
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);
}
/**
 * ��ʾ�ۻ����
 * 
 * @author Administrator (2014-11-21) 
 * num-��ֵ  dots-С����λ��  b-�·� 
 */
void OV_DispAmountMoney(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_AMOUNT),1);
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);
}
/**
 * ��ʾ��������
 * 
 * @author Administrator (2014-11-21) 
 * num-��ֵ  dots-С����λ��  b-�·� 
 */
void OV_DispFreezeGas(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_FREEZE),1);
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);
}
/**
 * ��ʾ������
 * 
 * @author Administrator (2014-11-21) 
 * num-��ֵ  dots-С����λ��  b-�·� 
 */
void OV_DispFreezeMoney(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_FREEZE),1);
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);
}
/**
 * ��ʾ����
 * 
 * @author Administrator (2014-11-21) 
 * num-��ֵ  dots-С����λ��   
 */
void OV_DispPrice(long num,char dots)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_PRICE),1);
    OV_LcdDispMark((MARK_BACKSLASH),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispNumWithDot(num,dots);
}
/**
 * ��ʾ����
 * 
 * @author Administrator (2014-11-21)
 */
void OV_DispErr(void)
{
    OV_LcdDispMark((MARK_ERR),1);
}
/**
 * ��ʱ����״̬
 * 
 * @author Administrator (2014-11-21) 
 * 0-��ʾ���ſ���1-��ʾ���Ź� 
 */
void OV_DispValve(char c)
{   
    if(c)
    {
        OV_LcdDispMark((MARK_CLOSE),1);
        OV_LcdDispMark((MARK_OPEN),0);
    }
    else
    {
        OV_LcdDispMark((MARK_CLOSE),0);
        OV_LcdDispMark((MARK_OPEN),1);
    }
}
/**
 * ��ʾ����
 * 
 * @author Administrator (2014-11-21)
 */
void OV_DispBatt(unsigned int i)
{   
    switch(i)
    {   
    case 1://OV_LcdDispMark((MARK_BATT_FREAM),1);
           OV_LcdDispMark((MARK_BATT1),1);
           OV_LcdDispMark((MARK_BATT2),0);
           OV_LcdDispMark((MARK_BATT3),0);
           break;
    case 2://OV_LcdDispMark((MARK_BATT_FREAM),1);
           OV_LcdDispMark((MARK_BATT1),1);
           OV_LcdDispMark((MARK_BATT2),1);
           OV_LcdDispMark((MARK_BATT3),0);
           break;
    case 3://OV_LcdDispMark((MARK_BATT_FREAM),1);
           OV_LcdDispMark((MARK_BATT1),1);
           OV_LcdDispMark((MARK_BATT2),1);
           OV_LcdDispMark((MARK_BATT3),1);
           break;
    default:
        //OV_LcdDispMark((MARK_BATT_FREAM),1);
        OV_LcdDispMark((MARK_BATT1),0);
        OV_LcdDispMark((MARK_BATT2),0);
        OV_LcdDispMark((MARK_BATT3),0);
        break;
    }
}
/**
 * ��ʾ�ź�
 * 
 * @author Administrator (2014-11-21)
 */
void OV_DispSign(unsigned int i)
{
    switch(i)
    {   
    
    case 1:OV_LcdDispMark((MARK_SIGN1),1);
           break;
    case 2:
           OV_LcdDispMark((MARK_SIGN1),1);
           OV_LcdDispMark((MARK_SIGN2),1);
           break;
    case 3:
           OV_LcdDispMark((MARK_SIGN1),1);
           OV_LcdDispMark((MARK_SIGN2),1);
           OV_LcdDispMark((MARK_SIGN3),1);
           break;
    default:
           OV_LcdDispMark((MARK_SIGN1),0);
           OV_LcdDispMark((MARK_SIGN2),0);
           OV_LcdDispMark((MARK_SIGN3),0);
           break;
    }
}

/**
 * ��ʾ�·�
 * 
 * @author Administrator (2014-11-24)
 */
void OV_DispMonth(unsigned int b)
{
    if(b==0||b>12)
    {   
        LcdWriteBuf(4,0x00);
        LcdWriteBuf(3,0x00);
    }
    else if(b<=9)
    {
        OV_LcdDispMark((MARK_MONTH),1);
        LcdDispChar(9,b);
    }
    else 
    {
        OV_LcdDispMark((MARK_MONTH),1);
        LcdDispChar(9,b-10);
        LcdSetBit(4,8);
    }
}

/**
 * ��ʾʱ�� 00:00:00
 * 
 * @author Administrator (2014-11-24) 
 * a-ʱ b-�� c-��  ʱ�� ��ʽ��XXʱXX��XX��
 */
void OV_DispTime(unsigned int a,unsigned int b,unsigned int c)
{
    OV_LcdClearValueMark();
    LcdSetBit(10,8);
    LcdSetBit(12,8);
    LcdSetBit(16,8);

    LcdDispChar(0,LCD_CHAR_BLANK);
    LcdDispChar(1,a/10);
    LcdDispChar(2,a%10);

    LcdDispChar(3,b/10);
    LcdDispChar(4,b%10);

    LcdDispChar(5,c/10);
    LcdDispChar(6,c%10);
}

/**
 * ��ʾ����  XXXXXX(��141124)
 * 
 * @author Administrator (2014-11-24) 
 * n-�� m-�� k-��  ��ʽ��XX��XX��XX�� 
 */
void OV_DispDate(unsigned int n,unsigned int m,unsigned int k)
{
    OV_LcdClearValueMark();
        
    LcdDispChar(0,LCD_CHAR_BLANK);
    LcdDispChar(1,n/10);
    LcdDispChar(2,n%10);

    LcdDispChar(3,m/10);
    LcdDispChar(4,m%10);

    LcdDispChar(5,k/10);
    LcdDispChar(6,k%10);

}

/**
 * ��ʾ HELLO
 * 
 * @author Administrator (2014-11-24)
 */
void OV_DispHello(void)
{
    OV_LcdClearValueMark();

    OV_LcdDispChar(0,LCD_CHAR_BLANK);        
    OV_LcdDispChar(1,LCD_CHAR_H);
    OV_LcdDispChar(2,LCD_CHAR_E);
    OV_LcdDispChar(3,LCD_CHAR_L);
    OV_LcdDispChar(4,LCD_CHAR_L);
    OV_LcdDispChar(5,LCD_CHAR_O);
    OV_LcdDispChar(6,LCD_CHAR_BLANK);        
}

/**
 * ��ʾEnd
 * 
 * @author Administrator (2014-11-24)
 */
void OV_DispEnd(void)
{
    OV_LcdClearValueMark();
    OV_LcdDispChar(0,LCD_CHAR_BLANK);    
    OV_LcdDispChar(1,LCD_CHAR_BLANK);    
    OV_LcdDispChar(2,LCD_CHAR_E);
    OV_LcdDispChar(3,LCD_CHAR_n);
    OV_LcdDispChar(4,LCD_CHAR_d);
    OV_LcdDispChar(5,LCD_CHAR_BLANK);    
    OV_LcdDispChar(6,LCD_CHAR_BLANK);    
}

/**
 * ��ʾEnd
 * 
 * @author Administrator (2014-11-24)
 */
void OV_DispSucc(void)
{
    OV_LcdClearValueMark();
    
    OV_LcdDispChar(0,LCD_CHAR_BLANK);    
    OV_LcdDispChar(1,LCD_CHAR_S);
    OV_LcdDispChar(2,LCD_CHAR_U);
    OV_LcdDispChar(3,LCD_CHAR_C);
    OV_LcdDispChar(4,LCD_CHAR_C);
    OV_LcdDispChar(5,LCD_CHAR_BLANK);    
    OV_LcdDispChar(6,LCD_CHAR_BLANK);    
}
/**
 * ��ʾEnd
 * 
 * @author Administrator (2014-11-24)
 */
void OV_DispFail(void)
{
    OV_LcdClearValueMark();
    
    OV_LcdDispChar(0,LCD_CHAR_BLANK);    
    OV_LcdDispChar(1,LCD_CHAR_F);
    OV_LcdDispChar(2,LCD_CHAR_A);
    OV_LcdDispChar(3,LCD_CHAR_I);
    OV_LcdDispChar(4,LCD_CHAR_L);
    OV_LcdDispChar(5,LCD_CHAR_BLANK);    
    OV_LcdDispChar(6,LCD_CHAR_BLANK);    
}

/**
 * ��ʾ���ݽ��
 * 
 * @author Administrator (2014-11-24) 
 *  num-��ֵ  dots-С����λ��  b-�·�
 */
void OV_DispStepMoney(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
}
/**
 * ��ʾ��������
 * 
 * @author Administrator (2014-11-24) 
 * num-��ֵ  dots-С����λ��  b-�·� 
 */
void OV_DispStepGas(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
}
/**
 * ��ʾ����ʣ������
 * 
 * @author Administrator (2014-11-24) 
 * num-��ֵ  dots-С����λ��  b-�·� 
 */
void OV_DispStepRemainGas(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_REMAIN),1);
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
}
/**
 * ��ʾ����ʣ����
 * 
 * @author Administrator (2014-11-24) 
 *  num-��ֵ  dots-С����λ��  b-�·�
 */
void OV_DispStepRemainMoney(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_REMAIN),1);
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
}


/**
 * ��ʾ���ݽ��
 * 
 * @author Administrator (2014-11-24) 
 *  num-��ֵ  dots-С����λ��  b-���ݼ���
 */
void OV_DispStepNMoney(long num,char dots,unsigned char b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
    OV_LcdDispMark((MARK_MONTH),0);
}

/**
 * ��ʾ��������
 * 
 * @author Administrator (2014-11-24) 
 * num-��ֵ  dots-С����λ��  b-���ݼ��� 
 */
void OV_DispStepNVol(long num,char dots,unsigned char b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
    OV_LcdDispMark((MARK_MONTH),0);
}
/**
 * ��ʾĳ����ʣ������
 * 
 * @author Administrator (2014-11-24) 
 * num-��ֵ  dots-С����λ��  b-���ݼ��� 
 */
void OV_DispStepNRemainVol(long num,char dots,unsigned char b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_REMAIN),1);
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
    OV_LcdDispMark((MARK_MONTH),0);
    
    
}
/**
 * ��ʾĳ����ʣ����
 * 
 * @author Administrator (2014-11-24) 
 *  num-��ֵ  dots-С����λ��  b-���ݼ���
 */
void OV_DispStepNRemainMoney(long num,char dots,unsigned char b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_REMAIN),1);
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
    OV_LcdDispMark((MARK_MONTH),0);
}
/**
 * ��ʾ���ݵ���
 * 
 * @author Administrator (2014-11-21) 
 * num-��ֵ  dots-С����λ��  b-���ݼ��� 
 */
void OV_DispStepNPrice(long num,char dots,char b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_PRICE),1);
    OV_LcdDispMark((MARK_BACKSLASH),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
    OV_LcdDispMark((MARK_MONTH),0);
}

/**
 * LCD��
 * 
 * @author Administrator (2014-11-24)
 */
void OV_LcdOn(void)
{
    //LcdOn();
    
    Chip_LCD_PowerOn(NULL);
}
/**
 * LCD��
 * 
 * @author Administrator (2014-11-24)
 */
void OV_LcdOff(void)
{
//	    LcdOff();
    Chip_LCD_PowerOff(NULL);
}
/**
 * LCD��ʼ��
 * 
 * @author Administrator (2014-11-24)
 */
void OV_LcdInit(void)
{
    //LcdInit();
    //OV_DispHello();
    SYS_LCD_Init();
}


/**
 * ��ʾ����һ����ĸ�Ĳ���
 * 
 * @author Zzzzz (2014-12-11)
 * 
 * @param ch1 
 *          ������ĸ1 
 * @param num 
 *          ����ֵ 
 *           
 */
void OV_LcdDispPara1(unsigned char ch1,unsigned long num)
{
    OV_LcdClearValueMark();
    OV_LcdDispNumWithDot(num,0);        
    OV_LcdDispChar(0,ch1);    
    OV_LcdDispChar(1,LCD_CHAR_SEPARATE);
}

/**
 * ��ʾ����������ĸ�Ĳ���
 * 
 * @author Zzzzz (2014-12-11)
 * 
 * @param ch1 
 *          ������ĸ1 
 * @param ch2 
 *          ������ĸ2
 * @param num 
 *          ����ֵ 
 *           
 */
void OV_LcdDispPara2(unsigned char ch1,unsigned char ch2,unsigned int num)
{
    OV_LcdClearValueMark();
    OV_LcdDispNumWithDot(num,0);        
    
    OV_LcdDispChar(0,ch1);    
    OV_LcdDispChar(1,ch2);
    OV_LcdDispChar(2,LCD_CHAR_SEPARATE);
}

/**
 * �ֶ���ʾIP��ַ
 * 
 * @author Zzzzz (2014-12-11)
 * 
 * @param c 
 *          c=0 ��ʾ��һ�� �� 192.168.
 *          c=1 ��ʾ�ڶ��� �� 000.001
 * @param ip0 
 *          ipֵ 
 * @param ip1 
 *          ipֵ 
 */
void OV_LcdDispIP(unsigned char c,unsigned char ip0,unsigned char ip1)
{
    OV_LcdClearValueMark();  
    OV_LcdDispChar(0,ip0/100);
    ip0%=100;
    OV_LcdDispChar(1,ip0/10);
    ip0%=10;
    OV_LcdDispChar(2,ip0);
    LcdSetDots(4);
    
    OV_LcdDispChar(3,ip1/100);
    ip1%=100;
    OV_LcdDispChar(4,ip1/10);
    ip1%=10;
    OV_LcdDispChar(5,ip1);
    OV_LcdDispChar(6,LCD_CHAR_BLANK);    
    
    
    if(c==0)
    {
        LcdSetBit(18,8);
    }
}
void OV_LcdDispErr(unsigned int num)
{
    OV_LcdClearValueMark();
    OV_LcdDispChar(0,LCD_CHAR_BLANK);
    OV_LcdDispChar(1,LCD_CHAR_BLANK);
    OV_LcdDispChar(2,LCD_CHAR_E);
    OV_LcdDispChar(3,LCD_CHAR_SEPARATE);
    OV_LcdDispChar(4,num/10);
    OV_LcdDispChar(5,num%10);
    OV_LcdDispChar(6,LCD_CHAR_BLANK);



}


void OV_DispErrNum(unsigned char ErrNum)
{
    OV_LcdDispErr(ErrNum);
    OV_LcdRefresh();
}

void LCDTest(void)
{
   OV_LcdInit();
   OV_LcdOn();
   OV_LcdDispAll();
   OV_DispHello();
   OV_LcdDispPara2(LCD_CHAR_A,LCD_CHAR_A,123);
   OV_LcdDispPara1(LCD_CHAR_P,65535);
   OV_LcdDispIP(0,192,168);
   OV_LcdDispIP(1,32,11);
   
    OV_DispEnd();
    OV_DispSucc();
    OV_DispFail();
    OV_DispBatt(0);
    OV_DispBatt(1);
    OV_DispBatt(2);
    OV_DispBatt(3);
    OV_DispBatt(2);
    OV_DispBatt(1);
    OV_DispBatt(0);
   //MCU_LcdAllSet();
   OV_LcdDispNumWithDot(345,3);
   
   OV_LcdDispNumWithDot(3450,3);
   
   OV_LcdDispNumWithDot(123456789l,3);
   OV_DispRemainGas(112,3,12);
   OV_DispTime(16,45,00);
   OV_DispStepRemainMoney(-5635,4,13);
   OV_DispFreezeGas(2345678,4,11);
   
//   OV_LcdClearNum();
//   OV_DispDate(14,11,25);
//   OV_DispPrice(75,3);
//   MCU_LcdClose();
}

/*******************************************************************************
* @function_name:  SYS_LCD_Init
* @function_file:  dev_lcd.c
* @����: LCD�豸��ʼ��
* 
* @����: 
* @����: 
* @����: yzy (2018-03-06)
*-----------------------------------------------------------------------------
* @�޸���: 
* @�޸�˵��: 
******************************************************************************/
void SYS_LCD_Init(void)
{
#if SYS_LCD_EN > 0
    COMPORT* gpo;

    Chip_LCD_Init(NULL, NULL);

    Chip_LCD_PowerOff(NULL);
    Chip_LCD_LCDM0_Config(_00_LCD_DISPLAY_WAVEFORM_A | _0D_LCD_DISPLAY_MODE1 | _00_LCD_VOLTAGE_MODE_EXTERNAL, false);


                                     //ѭ����ʼ������LCD�˿�
    for(uint32 uc_i = 0; uc_i < LCD_PORT_NUM; uc_i++)
    {
        gpo = (COMPORT*)gs_LCDPort + uc_i;  //GPO�˿�ָ��
                                         
        Chip_GPIO_SetPinDigit(gpo->pingrp, gpo->pinnum);

        Chip_GPIO_SetPinPFSEG(gpo->pingrp, gpo->pinnum, gpo->pinseg);
        Chip_GPIO_SetPinOutputMode(gpo->pingrp, gpo->pinnum, gpo->modefunc & 0x01);
        Chip_GPIO_SetPinPullUpMode(gpo->pingrp, gpo->pinnum, (gpo->modefunc >> 2) & 0x01);
        //          Chip_IOCON_PinMuxSet(LPC_IOCON, gpo->port->pingrp, gpo->port->pinnum, gpo->port->modefunc);
        Chip_GPIO_SetPinDIROutput(gpo->pingrp, gpo->pinnum);
    }
//	    PU3 &= 0xFAU;
//	            PU5 &= 0x80U;
//	            PU7 &= 0x00U;
//	            POM5 &= 0x87U;
//	            PIM5 &= 0xD7U;
//	            PFSEG0 |= 0xF0U;
//	            PFSEG1 |= 0xF7U;
//	            PFSEG2 |= 0x5FU;
//	            P3 &= 0xFAU;
//	            PM3 &= 0xFAU;
//	            P5 &= 0x80U;
//	            PM5 &= 0x80U;
//	            P7 &= 0x00U;
//	            PM7 &= 0x00U;

//	    krhino_sem_create(&gs_GPORes, "gpo_sem", 1);

    Chip_LCD_LCDM1_Config(_00_LCD_DISPLAY_PATTERN_A, true);
    Chip_LCD_LCDC0_Config(_07_LCD_CLOCK_FSUB_FIL_8, false);



#endif
}

void OV_DispLoading(unsigned char step)
{
    unsigned char i;
    OV_LcdClearValueMark();
    if(step>7)
        step=7;
    for(i=0;i<step;i++)
        OV_LcdDispChar(i,LCD_CHAR_SEPARATE);
//    OV_LcdDispChar(1,LCD_CHAR_SEPARATE);
//    OV_LcdDispChar(2,LCD_CHAR_SEPARATE);
//    OV_LcdDispChar(3,LCD_CHAR_SEPARATE);
//    OV_LcdDispChar(4,LCD_CHAR_SEPARATE);
//    OV_LcdDispChar(5,LCD_CHAR_SEPARATE);
//    OV_LcdDispChar(6,LCD_CHAR_SEPARATE);
    OV_LcdRefresh();
}

 
const _F_LCD gs_Lcd = 
{
    OV_LcdOn,
    OV_LcdOff,
    OV_LcdInit,
    OV_LcdClearNum,
    OV_DispLoading,
    OV_DispEnd,
    OV_DispErrNum,

};


