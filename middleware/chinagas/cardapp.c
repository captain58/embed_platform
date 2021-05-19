#include <stdio.h>
#include <string.h>
#include "common.h"

#include "Iso7816.h"
#include "cardapp.h"
#include "UserErrCode.h"
#include "UserDataType.h"
#include "UserOP.h"
#include "hwCtrl.h"
#include "cardOperation.h"
#include "drv_lcd.h"

unsigned char EsamDataBuf[0x90];
//unsigned char ErrBackStr[8];
CpuFile_Stu CpuFileBuf;
pCpuFile_Stu Card;

stuCardOpData CardOpData;

extern StuUser userPara;

extern int DES_Encrypt(char *plainFile, int count, char *keyStr, char *cipherFile);
extern int DES_Decrypt(char *cipherFile, int count, char *keyStr, char *plainFile);
extern int DES3_Encrypt(char *plainFile, int count, char *keyStr, char *cipherFile);

extern void DelayMs(unsigned int ms);

unsigned char GetCardType(void)
{
    Card = &CpuFileBuf;
    if(ReadPublicInfo(&Card->PublicInfo.CardType - Card->buf,(char *)&Card->PublicInfo.CardType,1) == 0)
    {
        return Card->PublicInfo.CardType;
    }
    else
    {
        return 0xff;
    }
}

extern const unsigned char CmdGetRND[5];
extern void ClockSwToEsam(void);
extern void ClockSwToCard(void);
//Stu_CpuApdu CpuUserApdu;
unsigned char VerifyUserCard(unsigned char tKeyIndex)
{
    char CSNbuf[8], RNDbuf[8], CardVerifyCodeBuf[8];
    if(Card_ReadBinary(0x08, 0x08, CSNbuf, 0x08) != 0)
        return USER_ERR_READ_BINARY;
    if(Card_GetRnd(RNDbuf, 8) != 0)
        return USER_ERR_GET_RND;
    Esam_Init();
    //���ܼ����ʼ��
    if(Esam_GenerateKey(0x00, tKeyIndex, 8, CSNbuf) != 0)
        return USER_ERR_GENERATE_KEY;
    //des����
    if(EsamCrypt(0x00, 0x00, 8, RNDbuf, CardVerifyCodeBuf) != 0)
        return USER_ERR_GENERATE_KEY;
    //��Ƭ�ڲ���֤
    if(Card_ExtAuther(tKeyIndex, CardVerifyCodeBuf) != 0)
    {
        //memcpy(ErrBackStr,CSNbuf,8);
        return USER_ERR_CARD_EXT_AUTHER;
    }
    return 0;
}

//��֤��Ƭ
unsigned char VerifyCard(void)
{
    char CSNbuf[8], RNDbuf[8], EsamVerifyCodeBuf[8], CardVerifyCodeBuf[8];
    if(Card_ReadBinary(0x08, 0x08, CSNbuf, 0x08) != 0)
        return USER_ERR_READ_BINARY;
    Esam_Init();
    if(Esam_GetRnd(RNDbuf, 8) != 0)
        return USER_ERR_GET_RND;

    //���ܼ����ʼ��
    if(Esam_GenerateKey(0x00, 0x05, 8, CSNbuf) != 0)
        return USER_ERR_GENERATE_KEY;
    //des����
    if(EsamCrypt(0x00, 0x00, 8, RNDbuf, EsamVerifyCodeBuf) != 0)
        return USER_ERR_GENERATE_KEY;

    //��Ƭ�ڲ���֤
    if(Card_IntAuther(0x02, RNDbuf, CardVerifyCodeBuf) == 0)
    {
        if(memcmp(CardVerifyCodeBuf, EsamVerifyCodeBuf, 8) == 0)
            return 0;
        else
            return USER_ERR_CARD_INT_AUTHER;
    }
    else
        return USER_ERR_CARD_INT_AUTHER;
}

//��֤���
unsigned char VerifyMeter(void)
{
    char CSNbuf[8], RNDbuf[8], CardVerifyCodeBuf[8];
    if(Card_ReadBinary(0x08, 0x08, CSNbuf, 0x08) != 0)
        return USER_ERR_READ_BINARY;
    if(Card_GetRnd(RNDbuf, 8) != 0)
        return USER_ERR_GET_RND;
    Esam_Init();
    //���ܼ����ʼ��
    if(Esam_GenerateKey(0x00, 0x03, 8, CSNbuf) != 0)
        return USER_ERR_GENERATE_KEY;
    //des����
    if(EsamCrypt(0x00, 0x00, 8, RNDbuf, CardVerifyCodeBuf) != 0)
        return USER_ERR_GENERATE_KEY;
    //��Ƭ�ڲ���֤
    if(Card_ExtAuther(0x03, CardVerifyCodeBuf) != 0)
    {
        //memcpy(ErrBackStr,CSNbuf,8);
        return USER_ERR_CARD_EXT_AUTHER;
    }
    return 0;
}

//д�ļ� ��ȫ��֤
unsigned char CardWriteFileSecurity(unsigned char tFile, unsigned char tAddr, char *tBuf, unsigned char tLen)
{
    char Key[16], RNDbuf[8], tDatabuf[8];
    if(Card_GetRnd(RNDbuf, 8) != 0)
        return USER_ERR_GET_RND;
    Esam_Init();
    //if(Esam_SelectFile(0xdf01)!=0)
    //    return USER_ERR_SELECT_FILE;
    if(Esam_ReadBinary(0x08, 0x20, Key, 16) != 0)
        return USER_ERR_READ_BINARY;
    DES3_Encrypt(RNDbuf, 8, Key, tDatabuf);
    if(Card_ExtAuther(0x02, tDatabuf) != 0)
        return USER_ERR_CARD_EXT_AUTHER;
    if(Card_WriteBinary(tFile, tAddr, tBuf, tLen) != 0)
        return USER_ERR_UPDATE_BINARY;
    return 0;
}

//д�ļ� ��ȫ��Կ��֤
unsigned char CardWriteFileSecurityKey(unsigned char tFile, unsigned char tAddr, char *tBuf, unsigned char tLen)
{
    unsigned char re;
    re = VerifyUserCard(0x02);
    if(re != 0)
        return re;
    if(Card_WriteBinary(tFile, tAddr, tBuf, tLen) != 0)
        return USER_ERR_UPDATE_BINARY;
    return 0;
}

//д�ļ� mac+����
unsigned char CardWriteFileMac(unsigned char tFile, unsigned char addr, unsigned char *buf, unsigned char len)
{
    unsigned int re;
    char tDatabuf[32];
    unsigned char tDataLen;
    char CSNbuf[8], RNDbuf[8], MACbuf[4];
    if(Card_ReadBinary(0x08, 0x08, CSNbuf, 0x08) != 0)
        return USER_ERR_READ_BINARY;
    memset(RNDbuf, 0x00, 8);
    if(Card_GetRnd(RNDbuf, 4) != 0)
        return USER_ERR_GET_RND;
    Esam_Init();
    //if(Esam_SelectFile(0xdf01)!=0)
    //    return USER_ERR_SELECT_FILE;

    re = Esam_GenerateKey(0x00, 0x01, 8, CSNbuf);
    if(re != 0)
        return USER_ERR_GENERATE_KEY;

    tDataLen = 8 + 5 + len; //�����8�ֽ�+ָ��5�ֽ�+��������len
    memcpy(tDatabuf, RNDbuf, 8);
    tDatabuf[8] = 0x04;
    tDatabuf[9] = 0xd6;
    tDatabuf[10] = 0x80 | tFile;
    tDatabuf[11] = addr;
    tDatabuf[12] = len + 4;
    memcpy(tDatabuf + 13, buf, len);
    if(EsamCrypt(0x03, 0x00, tDataLen, tDatabuf, MACbuf) != 0)
        return USER_ERR_GENERATE_KEY;

    if(Card_WriteBinaryMac(tFile, MACbuf, addr, (char *)buf, len) != 0)
        //    memcpy(buf+len,MACbuf,4);
        //    if(Card_WriteBinary(tFile,addr,(char *)buf,len+4)!=0)
        return USER_ERR_UPDATE_BINARY;
    return 0;
}

//#define WriteUserCardInfo(x,y,z) CardWriteFileMac(0x0a,x,y,z)
unsigned char WriteUserCardInfo(unsigned char addr,unsigned char *buf,unsigned char len)
{
    return CardWriteFileMac(0x0A,addr,buf,len);
}
//#define WritePublicCardInfo(x,y,z)  CardWriteFileMac(0x15,x,y,z)
unsigned char WritePublicCardInfo(unsigned char addr,unsigned char *buf,unsigned char len)
{
    return CardWriteFileMac(0x15,addr,buf,len);
}

//дEsamӦ����Ϣ�ļ� ���ⲿ��֤
unsigned char WriteEsamUserInfo(unsigned char addr, char *tbuf, unsigned char tlen)
{
    char Autherbuf[8], RNDbuf[8];
    if(Esam_GetRnd(RNDbuf, 8) != 0)
        return USER_ERR_GET_RND;
    if(Card_IntAuther(0x01, RNDbuf, Autherbuf) != 0)
        return USER_ERR_CARD_INT_AUTHER;
    if(Esam_ExtAuther(0x02, Autherbuf) != 0)
        return USER_ERR_ESAM_EXT_AUTHER;
    if(Esam_WriteBinary(0x0a, addr, tbuf, tlen) != 0)
        return USER_ERR_UPDATE_BINARY;
    return 0;
}

unsigned char WriteCardType(unsigned char tType)
{
    unsigned char re;
    re = WritePublicCardInfo(0x08, &tType, 1);
    return re;
}

unsigned char WriteCardDate(void)
{
    unsigned char re, tbuf[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    re = WritePublicCardInfo(0x12, tbuf, 8);
    //                FormatRoll((unsigned char *)&tRemainTimes,4);
    re = WriteUserCardInfo((Card->UserCardInfo.CheckChargeTimes - Card->buf), tbuf, 4);

    return re;
}

unsigned char UserCardCharge(unsigned long tCharge)
{
    long tRemain;
    long buflong;
    int bufint;
    unsigned char bufchar;
    unsigned char re;
    char tDataBuf1[8], tDataBuf2[8];
    //�˶Թ��������
    memset(tDataBuf2, 0, 8);
    if(ReadEsamChargeInfo(0x08, tDataBuf1, 6) != 0)
        return USER_ERR_READ_BINARY;

    //    if(memcmp(tDataBuf2,tDataBuf1,6)!=0)//�жϹ���������Ƿ�Ϊ0
    //    {
    //        //��Ϊ0
    //        //�жϱ��ڹ���������Ƿ����ϴι����������ͬ
    //        if(memcmp(tDataBuf1,Card->UserCardInfo.LastChargeRnd,6)!=0)//�ж��ϴι�����������湺��������Ƿ���ͬ
    //        {
    //��ͬ
    //�жϱ��ڹ���������Ƿ��뱾�ι������������ͬ
    if(memcmp(tDataBuf1, Card->UserCardInfo.ThisChargeRnd, 6) == 0) //�жϱ��ι�����������湺��������Ƿ���ͬ
    {
        //��ͬ
        if (CardOpData.TempCharge > 0)
        {
            //�Ѿ���ֵ������ֱ��������������ÿ���־
            if(tCharge != 0)
            {
                Card_ConsumeNum(tCharge);
            }
            re = Esam_ChargeNum(CardOpData.TempCharge);
            if(re != 0)
            {
                return re;
            }

            //                    bufchar=0x12;
            //                    re=WritePublicCardInfo(0x08,&bufchar,1);
            re = WriteCardType(0x12);
            if(re != 0)
            {
                Esam_ConsumeNum(tCharge);
                return USER_ERR_CHANGE_CARDTYPE;
            }


            bufchar = 0x00;
            re = Card_ReadBinary(0x15, 0x08, (char *)&bufchar, 1);
            if(re != 0)
            {
                //CardOpData.TempCharge=0;
                Esam_ConsumeNum(tCharge);
                return USER_ERR_READ_BINARY;
            }
            else if(bufchar != 0x12)
            {
                Esam_ConsumeNum(tCharge);
                return USER_ERR_CHANGE_CARDTYPE;
            }

            MeterCharge(0, CardOpData.TempCharge);
            CardOpData.TempCharge = 0;
            return 0;
        }
        else
        {
            if(tCharge != 0)
            {
                //Card_ConsumeNum(tCharge);
                return USER_ERR_RND_MATCH;
            }
            //                    else
            //                    {
            //                        bufchar=0x12;
            //                        re=WritePublicCardInfo(0x08,&bufchar,1);
            re = WriteCardType(0x12);
            return re;
            //                    }
        }
    }
    //            else
    //            {
    //                //��ͬ
    //                memcpy(ErrBackStr,tDataBuf1,6);
    //                CardOpData.TempCharge=0;
    //                return USER_ERR_RND_MATCH;
    //            }
    //        }
    //    }

    //���´�����Ϣ��ʹ�������esam������ͬ
    //EsamSetRemain();
    //    re=UpdateRemain();
    //    if(re!=0)
    //        return re;

    //�ж��Ƿ񳬹���ֵ����
    tRemain = cardOpGetValue(BASEVAL_Remain);
    if(tRemain + (long)tCharge > cardOpGetValue(USERVAL_Maxremain))
    {
        //        buflong=tRemain/100;
        //        copy_str_roll((char *)ErrBackStr,(char *)&buflong,4);
        //        bufint=tRemain%100;
        //        bufint*=100;
        //        copy_str_roll((char *)ErrBackStr+4,(char *)&bufint,2);
        return USER_ERR_CHARGE_MAX;
    }
    //    //���ݿ��ڴ����������
    //    memcpy(tDataBuf1,&tCharge,4);
    //    memcpy(tDataBuf1+4,&Meter.ChargeIndex,2);
    //    bufint=CrcGet(tDataBuf1,6);
    //    memcpy(tDataBuf1+6,&bufint);

    //�����ϴι�������뼰��������
    CardOpData.TempCharge = tCharge;
    buflong = cardOpGetValue(BASEVAL_ChargeALL) + tCharge;
    buflong /= 100;
    copy_str_roll((char *)EsamDataBuf, (char *)&buflong, 4);
    buflong = cardOpGetValue(BASEVAL_ChargeALL) + tCharge;
    buflong %= 100;
    buflong *= 100000;
    copy_str_roll((char *)(EsamDataBuf + 4), (char *)&buflong, 4);
    //    memcpy(EsamDataBuf,&buflong,4);
    memcpy(EsamDataBuf + 8, (char *)Card->UserCardInfo.ThisChargeRnd, 6);
    if(WriteEsamChargeInfo(0x00, (char *)EsamDataBuf, 0x0f) != 0)
    {
        return USER_ERR_UPDATE_BINARY;
    }

    //Ȧ���ֵ
    re = Esam_ChargeNum(tCharge);
    if(re != 0)
    {
        return re;
    }
    //Ȧ��ɹ���۳����ڴ���
    //����ʱ��Ƭ���γ����ڹ����������д�������ٴβ����û��������ٳ�ֵ
    //
    //��ֵ�ɹ���
    re = Card_ConsumeNum(tCharge); //����
    if(re != 0)
    {
        //BeepMs(100);
        //���Ѳ��ɹ�����������
        Card_Init();
        CardReadBalance((unsigned long *)&buflong);
        re = Card_ConsumeNum(buflong); //����
        if(re != 0)
        {
            Esam_ConsumeNum(tCharge);
            return re;
        }
        else
            re = 0;
    }

    //��д������
    //    bufchar=0x12;
    //    re=WritePublicCardInfo(0x08,&bufchar,1);
    re = WriteCardType(0x12);
    if(re != 0)
    {
        Esam_ConsumeNum(tCharge);
        return USER_ERR_CHANGE_CARDTYPE;
    }
    bufchar = 0x00;
    re = Card_ReadBinary(0x15, 0x08, (char *)&bufchar, 1);
    if(re != 0)
    {
        //CardOpData.TempCharge=0;
        Esam_ConsumeNum(tCharge);
        return USER_ERR_READ_BINARY;
    }
    else if(bufchar != 0x12)
    {
        Esam_ConsumeNum(tCharge);
        return USER_ERR_CHANGE_CARDTYPE;
    }
    CardOpData.TempCharge = 0;
    MeterCharge(0, tCharge);
    //EsamSetRemain();
    return re;
}

void UserCardWriteBack(unsigned char ChargeRe)
{
    long buflong;
    int bufint;
    pStu_EsamInfo pEsamInfo;
    //unsigned char CardBackStr[10];
    //���淴д����
    memset(Card->buf, 0x00, 0x60);
    pEsamInfo = (pStu_EsamInfo)EsamDataBuf;
    if(EsamReadAll((char *)pEsamInfo) != 0)
        return;
    memcpy(Card->UserCardWriteBack.IssueCode, pEsamInfo->IssueCode, 6);
    memcpy(Card->UserCardWriteBack.CompanyCode, pEsamInfo->CompanyCode, 2);
    memcpy(Card->UserCardWriteBack.UserCode, pEsamInfo->UserCode, 8);

    buflong = cardOpGetValue(BASEVAL_AmountGas) / 100;
    bufint = (cardOpGetValue(BASEVAL_AmountGas) % 100) * 100;
    copy_str_roll((char *)(Card->UserCardWriteBack.TotalUseInt), (char *)&buflong, 4);
    copy_str_roll((char *)(Card->UserCardWriteBack.TotalUseDec), (char *)&bufint, 2);

    buflong = cardOpGetValue(BASEVAL_Remain) / 100;
    bufint = (cardOpGetValue(BASEVAL_Remain) % 100) * 100;
    copy_str_roll((char *)(Card->UserCardWriteBack.RemainInt), (char *)&buflong, 4);
    copy_str_roll((char *)(Card->UserCardWriteBack.RemainDec), (char *)&bufint, 2);

    buflong = cardOpGetValue(BASEVAL_ChargeALL) / 100;
    bufint = (cardOpGetValue(BASEVAL_ChargeALL) % 100) * 100;
    copy_str_roll((char *)(Card->UserCardWriteBack.AmountChargeInt), (char *)&buflong, 4);
    copy_str_roll((char *)(Card->UserCardWriteBack.AmountChargeDec), (char *)&bufint, 2);

    Card->UserCardWriteBack.MagnetTimes = cardOpGetValue(USERVAL_MagnetTimes);
    Card->UserCardWriteBack.FlowTimes = cardOpGetValue(USERVAL_MaxFlowTimes);
    Card->UserCardWriteBack.OpenDoorTimes = cardOpGetValue(USERVAL_OpenTimes);
    Card->UserCardWriteBack.MeterReg = cardOpGetValue(USERVAL_MeterRegValue);
}

const char CMCConsumeCode[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
extern int Cal_MAC(char *StrIN, int count, char *StrKey, char *StrInit, char *StrOut);
//const char SYSDATE[4]={0x20,0x14,0x02,0x12};
//const char SYSTIME[3]={0x14,0x02,0x12};
//const char MyID[6]={0x01,0x02,0x03,0x04,0x05,0x06};
const char MACDataEnd[8] = {0x80, 0, 0, 0, 0, 0, 0, 0};
const char MACInitStr[8] = {0, 0, 0, 0, 0, 0, 0, 0};
//extern unsigned char EsamReadConsumeKey(char *tbuf);
unsigned char ConsumeNum(unsigned char tSource, unsigned long tNum)
{
    char tReData[16], tDataBuf1[32];
    char ConsumKey[16];
    char tKey1[8], tMac1[4], tMac2[4];
    unsigned char timestr[7];
    unsigned char re;
    if(Esam_Init() != 0)
        return USER_ERR_ESAM_INIT;
    memset(tReData, 0xff, 16);
    //if(Esam_SelectFile(0xdf01)!=0)
    //   return USER_ERR_SELECT_FILE;
    if(Esam_ReadBinary(0x08, 0x10, ConsumKey, 0x10) != 0)
        //    if(EsamReadConsumeKey(ConsumKey)!=0)
        return USER_ERR_READ_BINARY;

    if(tSource == ESAM_CONSUME)
    {
        re = EsamConsumeInit(tNum, (char *)CMCConsumeCode, tReData);
    }
    else if(tSource == CARD_CONSUME)
    {
        //ClockSwToCard();
        re = CardConsumeInit(tNum, (char *)CMCConsumeCode, tReData);
    }
    if(re != 0)
        return USER_ERR_CONSUME_INIT;
    //�������ݣ�
    //4�ֽڵ���Ǯ�������
    //2�ֽڵ���Ǯ���������
    //3�ֽ�͸֧�޶�
    //1�ֽ���Կ�汾��
    //1�ֽ��㷨��ʶ
    //4�ֽ��û���/ESAM�����

    memcpy(tDataBuf1, tReData + 11, 4);
    memcpy(tDataBuf1 + 4, tReData + 4, 2);
    tDataBuf1[6] = 0;
    tDataBuf1[7] = 0;
    //4 �ֽ��û���/ESAM �����
    //2 �ֽڵ���Ǯ���������
    //0000

    //���������Կ
    DES3_Encrypt(tDataBuf1, 8, ConsumKey, tKey1);
    //����mac2
    copy_str_roll(tDataBuf1, (char *)&tNum, 4);
    tDataBuf1[4] = 0x80;
    tDataBuf1[5] = 0x00;
    tDataBuf1[6] = 0x00;
    tDataBuf1[7] = 0x00;
    //��������MAC2
    Cal_MAC(tDataBuf1, 8, tKey1, (char *)MACInitStr, tMac2);

    //����mac1

    //��֯MAC����������
    //�����
    //+06
    //+000000000011���ն˻���ţ����ұ��ɹ涨���Լ����ն˻���ţ�
    //+20130713�����ڣ�
    //+115859��ʱ�䣩[����ʱ��Ϊ����ʱ������ʱ��]
    copy_str_roll(tDataBuf1, (char *)&tNum, 4);
    tDataBuf1[4] = 0x06;
    //RefreshMeterTime();
    timestr[0] = 0x20;
    timestr[1] = 0;//Globe.MeterTime.Year;
    timestr[2] = 0;//Globe.MeterTime.Month;
    timestr[3] = 0;//Globe.MeterTime.Day;
    timestr[4] = 0;//Globe.MeterTime.Hours;
    timestr[5] = 0;//Globe.MeterTime.Minutes;
    timestr[6] = 0; //Globe.MeterTime.Seconds;

    memcpy(tDataBuf1 + 5, CMCConsumeCode, 6);
    memcpy(tDataBuf1 + 11, timestr, 7);
    //memcpy(tDataBuf1+15,SYSTIME,3);
    memcpy(tDataBuf1 + 18, MACDataEnd, 8);
    //��������MAC
    Cal_MAC(tDataBuf1, 24, tKey1, (char *)MACInitStr, tMac1);
    //�û�������
    memset(tDataBuf1, 0xff, 32);
    tDataBuf1[0] = 0;
    tDataBuf1[1] = 0;
    tDataBuf1[2] = 0;
    tDataBuf1[3] = 0;
    memcpy(tDataBuf1 + 4, timestr, 7);
    //    memcpy(tDataBuf1+8,SYSTIME,3);
    memcpy(tDataBuf1 + 11, tMac1, 4);
    if(tSource == ESAM_CONSUME)
    {
        re = EsamConsume(tDataBuf1, tReData);
    }
    else if(tSource == CARD_CONSUME)
    {
        re = CardConsume(tDataBuf1, tReData);
    }
    if(re != 0)
        return USER_ERR_CONSUME;
    else if(memcmp(tReData + 4, tMac2, 4) != 0)
        return USER_ERR_MAC;

    return 0;
}

unsigned char Card_ConsumeNum(unsigned long tNum)
{
    unsigned char re;
    char tCSN[8], tReData[16], tDataBuf1[32], tMac1[4], tMac2[4];
    if(Card_ReadBinary(0x08, 0x08, tCSN, 0x08) != 0)
        return USER_ERR_READ_BINARY;
    re = CardConsumeInit(tNum, (char *)CMCConsumeCode, tReData);
    if(re != 0)
        return USER_ERR_CONSUME_INIT;
    //8�ֽ��û�����ɢ����
    //4�ֽ�α�����
    //2�ֽڽ������
    //2�ֽ�0x00
    memcpy(tDataBuf1, tCSN, 8);
    memcpy(tDataBuf1 + 8, tReData + 11, 4);
    memcpy(tDataBuf1 + 12, tReData + 4, 2);
    tDataBuf1[14] = 0x00;
    tDataBuf1[15] = 0x00;
    //esam���Ѽ����ʼ��
    if(Esam_GenerateKey(0x01, 0x04, 0x10, tDataBuf1) != 0)
        return USER_ERR_GENERATE_KEY;
    //esam����mac2����
    copy_str_roll(tDataBuf1, (char *)&tNum, 4);
    if(EsamCrypt(0x02, 0x00, 0x04, tDataBuf1, tMac2) != 0)
        return USER_ERR_GENERATE_KEY;

    //esam����mac1����
    copy_str_roll(tDataBuf1, (char *)&tNum, 4);
    tDataBuf1[4] = 0x06;
    memcpy(tDataBuf1 + 5, CMCConsumeCode, 6);
    //RefreshMeterTime();
    tDataBuf1[11] = 0x20;
    tDataBuf1[12] = 0;//Globe.MeterTime.Year;
    tDataBuf1[13] = 0;//Globe.MeterTime.Month;
    tDataBuf1[14] = 0;//Globe.MeterTime.Day;
    tDataBuf1[15] = 0;//Globe.MeterTime.Hours;
    tDataBuf1[16] = 0;//Globe.MeterTime.Minutes;
    tDataBuf1[17] = 0; //Globe.MeterTime.Seconds;

    if(EsamCrypt(0x02, 0x00, 0x12, tDataBuf1, tMac1) != 0)
        return USER_ERR_GENERATE_KEY;

    memset(tDataBuf1, 0x00, 4);
    tDataBuf1[4] = 0x20;
    tDataBuf1[5] = 0;//Globe.MeterTime.Year;
    tDataBuf1[6] = 0;//Globe.MeterTime.Month;
    tDataBuf1[7] = 0;//Globe.MeterTime.Day;
    tDataBuf1[8] = 0;//Globe.MeterTime.Hours;
    tDataBuf1[9] = 0;//Globe.MeterTime.Minutes;
    tDataBuf1[10] = 0; //Globe.MeterTime.Seconds;
    memcpy(tDataBuf1 + 11, tMac1, 4);
    //�û�������
    re = CardConsume(tDataBuf1, tReData);
    if(re != 0)
        return USER_ERR_CONSUME;
    if(memcmp(tMac2, tReData + 4, 4) != 0)
        return USER_ERR_MAC;

    return 0;
}


//unsigned char Esam_ConsumeNum(unsigned long tNum)
//{
//    return ConsumeNum(ESAM_CONSUME,tNum);
//}

unsigned char Esam_ChargeNumInit(unsigned long tNum, char *tReData)
{
    char tDataBuf1[32], tCSN[8];
    if(Esam_Init() != 0)
        return USER_ERR_ESAM_INIT;
    if(Esam_ReadBinary(0x08, 0x00, tCSN, 0x08) != 0)
        return USER_ERR_READ_BINARY;
    tDataBuf1[0] = 0x12;
    tDataBuf1[1] = 0x34;
    if(Esam_VerifyPin(2, tDataBuf1) != 0)
        return USER_ERR_PIN;
    if(EsamChargeInit(tNum, (char *)CMCConsumeCode, tReData) != 0)
        return USER_ERR_CHARGE_INIT;
    memcpy(tReData + 16, tCSN, 8);
    return 0;
}

unsigned char Esam_ChargeNumCmpt(char *tDataBuf1)
{
    if(EsamCharge(tDataBuf1) != 0)
        return USER_ERR_CHARGE;
    return 0;

}

unsigned char Esam_ChargeNum(unsigned long tNum)
{
    char tReData[16], tDataBuf1[32];
    char tCSN[8], tMac1[4], tMac2[4];
    unsigned char timestr[7];
    if(Esam_Init() != 0)
        return USER_ERR_ESAM_INIT;
    //if(Esam_SelectFile(0xdf01)!=0)
    //    return USER_ERR_SELECT_FILE;
    if(Esam_ReadBinary(0x08, 0x00, tCSN, 0x08) != 0)
        return USER_ERR_READ_BINARY;
    tDataBuf1[0] = 0x12;
    tDataBuf1[1] = 0x34;
    if(Esam_VerifyPin(2, tDataBuf1) != 0)
        return USER_ERR_PIN;
    if(EsamChargeInit(tNum, (char *)CMCConsumeCode, tReData) != 0)
        return USER_ERR_CHARGE_INIT;
    //ClockSwToCard();
    if(Card_SelectFile(0xdf01) != 0)
        return USER_ERR_SELECT_FILE;
    memcpy(tDataBuf1, tCSN, 8);
    memcpy(tDataBuf1 + 8, tReData + 8, 4);
    memcpy(tDataBuf1 + 12, tReData + 4, 2);
    tDataBuf1[14] = 0x80;
    tDataBuf1[15] = 0x00;
    if(Card_GenerateKey(0x01, 0x01, 0x10, tDataBuf1) != 0)
        return USER_ERR_GENERATE_KEY;
    //����mac1
    memcpy(tDataBuf1, tReData, 4);
    copy_str_roll(tDataBuf1 + 4, (char *)&tNum, 4);
    tDataBuf1[8] = 0x02;
    memcpy(tDataBuf1 + 9, CMCConsumeCode, 6);
    if(CardCrypt(0x02, 0x00, 0x0f, tDataBuf1, tMac1) != 0)
        return USER_ERR_GENERATE_KEY;
    if(memcmp(tMac1, tReData + 12, 4) != 0)
        return USER_ERR_MAC;
    //����mac2
    //RefreshMeterTime();
    timestr[0] = 0x20;
    timestr[1] = 0;//Globe.MeterTime.Year;
    timestr[2] = 0;//Globe.MeterTime.Month;
    timestr[3] = 0;//Globe.MeterTime.Day;
    timestr[4] = 0;//Globe.MeterTime.Hours;
    timestr[5] = 0;//Globe.MeterTime.Minutes;
    timestr[6] = 0;//Globe.MeterTime.Seconds;

    copy_str_roll(tDataBuf1, (char *)&tNum, 4);
    tDataBuf1[4] = 0x02;
    memcpy(tDataBuf1 + 5, CMCConsumeCode, 6);
    memcpy(tDataBuf1 + 11, timestr, 7);
    //memcpy(tDataBuf1+15,SYSTIME,3);
    if(CardCrypt(0x02, 0x00, 0x12, tDataBuf1, tMac2) != 0)
        return USER_ERR_GENERATE_KEY;

    memcpy(tDataBuf1, timestr, 7);
    //    memcpy(tDataBuf1+4,SYSTIME,3);
    memcpy(tDataBuf1 + 7, tMac2, 4);

    //ClockSwToEsam();
    if(EsamCharge(tDataBuf1) != 0)
        return USER_ERR_CHARGE;
    return 0;
}

unsigned char DataTranOut(void)
{
    long tRemain;
    int tRemainDec;
    unsigned char re;
    pStu_EsamInfo pEsamInfo;
    //��ȡEsam�ڵ���Ϣ
    Esam_Init();
    //if(Esam_SelectFile(0xdf01)!=0)
    //    return USER_ERR_SELECT_FILE;
    pEsamInfo = (pStu_EsamInfo)EsamDataBuf;
    if(EsamReadAll((char *)pEsamInfo) != 0)
        return USER_ERR_READ_BINARY;

    Card->TransCardInfo.State = 0xa5;
    memcpy(Card->TransCardInfo.IssueCode, pEsamInfo->IssueCode, 6);           //ת���±��̱��
    memcpy(Card->TransCardInfo.CompanyCode, pEsamInfo->CompanyCode, 2);         //ת���±�˾����
    //memcpy(Card->TransCardInfo.UserCode,pEsamInfo->UserCode,8);              //ת���±��û���
    Card->TransCardInfo.UseType = pEsamInfo->UseType;

    tRemain = cardOpGetValue(BASEVAL_Remain) / 100;
    copy_str_roll((char *)Card->TransCardInfo.RemainInt, (char *)&tRemain, 4);  //ת��������
    tRemain = cardOpGetValue(BASEVAL_Remain) % 100;
    tRemainDec = tRemain * 100;
    copy_str_roll((char *)Card->TransCardInfo.RemainDec, (char *)&tRemainDec, 2);  //ת����С��(4λ)

    //RefreshMeterTime();
    Card->TransCardInfo.TranOutTime[0] = 0;//Globe.MeterTime.Year;
    Card->TransCardInfo.TranOutTime[1] = 0;//Globe.MeterTime.Month;
    Card->TransCardInfo.TranOutTime[2] = 0;//Globe.MeterTime.Day;
    Card->TransCardInfo.TranOutTime[3] = 0;//Globe.MeterTime.Hours;
    Card->TransCardInfo.TranOutTime[4] = 0;//Globe.MeterTime.Minutes;

    //memcpy(Card->TransCardInfo.TranOutTime,5);           //ת���ɱ�ʱ��
    //memcpy(Card->TransCardInfo.TranInTime,5);            //ת���±�ʱ��

    //    //��������������ݣ��ָ���������
    //    memset(EsamDataBuf,0x00,0x90);
    //    WriteEsamOpInfo(0x00,(char *)EsamDataBuf,0x70);
    //    WriteEsamChargeInfo(0x00,(char *)EsamDataBuf,0x0f);
    //
    //    pEsamInfo=(pStu_EsamInfo)EsamDataBuf;
    //    pEsamInfo->UseType=0x01;//����ʽ
    //    pEsamInfo->Overdraught[1]=0x05;
    //    buflong=99999999;
    //    copy_str_roll((char *)pEsamInfo->ChargeLimit,(char *)&buflong,4);
    //    pEsamInfo->StatisticalDate=0x01;
    //
    //    WriteEsamUserInfo(0x00,(char *)EsamDataBuf,0x90);
    re = CardWriteFileSecurityKey(0x0c, 0x00, (char *)Card->buf, 0x22);
    if(re == 0)
    {
        //������ڴ���
        EsamClearRemain();
        return 0;
    }
    else
    {
        return re;
    }
}

unsigned char DataTranIn(void)
{
    long tRemain;
    int tRemainDec;
    unsigned char re;
    pStu_EsamInfo pEsamInfo;
    //��ȡEsam�ڵ���Ϣ
    Esam_Init();
    //if(Esam_SelectFile(0xdf01)!=0)
    //    return USER_ERR_SELECT_FILE;
    pEsamInfo = (pStu_EsamInfo)EsamDataBuf;
    if(EsamReadAll((char *)pEsamInfo) != 0)
        return USER_ERR_READ_BINARY;
    if(pEsamInfo->UseType != Card->TransCardInfo.UseType)
        return USER_ERR_METER_TYPE;
    //if(memcmp(pEsamInfo->UserCode,Card->TransCardInfo.UserCode,8)!=0)
    //    return USER_ERR_CARDID;

    Card->TransCardInfo.State = 0xaa;
    memcpy(Card->TransCardInfo.IssueCode, pEsamInfo->IssueCode, 6);           //ת���±��̱��
    memcpy(Card->TransCardInfo.CompanyCode, pEsamInfo->CompanyCode, 2);         //ת���±�˾����
    memcpy(Card->TransCardInfo.UserCode, pEsamInfo->UserCode, 8);            //ת���±��û���


    copy_str_roll((char *)&tRemain, (char *)Card->TransCardInfo.RemainInt, 4);  //ת��������
    copy_str_roll((char *)&tRemainDec, (char *)Card->TransCardInfo.RemainDec, 2);  //ת����С��(4λ)
    tRemain = tRemain * 100 + tRemainDec / 100;
    
    //RefreshMeterTime();
    Card->TransCardInfo.TranInTime[0] = 0;//Globe.MeterTime.Year;
    Card->TransCardInfo.TranInTime[1] = 0;//Globe.MeterTime.Month;
    Card->TransCardInfo.TranInTime[2] = 0;//Globe.MeterTime.Day;
    Card->TransCardInfo.TranInTime[3] = 0;//Globe.MeterTime.Hours;
    Card->TransCardInfo.TranInTime[4] = 0;//Globe.MeterTime.Minutes;
    re = CardWriteFileSecurityKey(0x0c, 0x00, (char *)Card->buf, 0x22);
    if(re == 0)
    {
        cardOpSetValue(BASEVAL_Remain, cardOpGetValue(BASEVAL_Remain) + tRemain);
        EsamWriteRemain(0, cardOpGetValue(BASEVAL_Remain));
        if(tRemain >= 0)
            Esam_ChargeNum(tRemain);
        else
            Esam_ConsumeNum(-tRemain);

        return 0;
    }
    else
        return re;
}

unsigned char UserCardUpdateInfo(void)
{
    unsigned char re;
    pStu_EsamInfo pEsamInfo;
    if((Card->UserCardInfo.OpCode1.value == 0) && (Card->UserCardInfo.OpCode2.value == 0))
        return 0;

    //��ȡEsam�ڵ���Ϣ
    Esam_Init();
    //if(Esam_SelectFile(0xdf01)!=0)
    //    return USER_ERR_SELECT_FILE;
    pEsamInfo = (pStu_EsamInfo)EsamDataBuf;
    if(EsamReadAll((char *)pEsamInfo) != 0)
        return USER_ERR_READ_BINARY;

    //���²���
    if(Card->UserCardInfo.OpCode1.UpdateMeterPrice)   //���±��ڵ���
        memcpy(pEsamInfo->ThisPrice, Card->UserCardInfo.ThisPrice, 4);
    if(Card->UserCardInfo.OpCode1.UpdateUseType)      //����������������
        pEsamInfo->UseType = Card->UserCardInfo.UseType;
    if(Card->UserCardInfo.OpCode1.UpdateCMC)          //�������ù�˾����
        memcpy(pEsamInfo->CompanyCode, Card->UserCardInfo.CompanyCode, 2);

    if(Card->UserCardInfo.OpCode2.UpdateIdleClose)    //�������ò������ط�����
    {
        pEsamInfo->IdleClose = Card->UserCardInfo.IdleClose;
        pEsamInfo->LimitIdleCloseTimes = Card->UserCardInfo.LimitIdleCloseTimes;
    }
    if(Card->UserCardInfo.OpCode2.UpdateWarningClose) //��������Ԥ�ط�ֵ
        memcpy(pEsamInfo->WarningClose, Card->UserCardInfo.WarningClose, 2);
    if(Card->UserCardInfo.OpCode2.StatisticalDate)    //����������������ͳ������
        pEsamInfo->StatisticalDate = Card->UserCardInfo.StatisticalDate;
    if(Card->UserCardInfo.OpCode2.UpdateMaxFlow)      //�������ô���������
        pEsamInfo->MaxFlow = Card->UserCardInfo.MaxFlow;
    if(Card->UserCardInfo.OpCode2.UpdateOverdraught)  //��������͸֧�޶�
        memcpy(pEsamInfo->Overdraught, Card->UserCardInfo.Overdraught, 2);
    if(Card->UserCardInfo.OpCode2.UpdateWarning)      //�������ñ�������
        memcpy(pEsamInfo->WarningNum, Card->UserCardInfo.WarningNum, 2);
    if(Card->UserCardInfo.OpCode2.UpdateChargeMax)    //�������ó�ֵ�޶�
        memcpy(pEsamInfo->ChargeLimit, Card->UserCardInfo.ChargeLimit, 4);

    //����У�飬д����Ϣ
    pEsamInfo->CRC2 = CrcAddGet((unsigned char *)pEsamInfo, 0x8e);
    re = WriteEsamUserInfo(0x00, (char *)pEsamInfo, 0x90);
    if(re != 0)
        return re;
    UpdateFromEsam(ESAM_UPDATE_APP_INFO);
    return 0;
}



/**
 *  �û�������
 *  ��֤����Ч��
 */
extern void OV_DispLoading(unsigned char step);
unsigned char DealUserCard(unsigned char cardtype)
{
    unsigned char re;
    unsigned long tCharge, buflong, tRemainTimes;
    //��֤��Ƭ��Ч��
    re = VerifyCard();
    if(re != 0)
        return re;
    re = VerifyMeter();
    if(re != 0)
        return re;
    OV_DispLoading(2);
    //�����������Ч����
    if(cardtype == 0x1f)
    {
        if(cardOpMeterIsInuse()==1)
        {
            re = USER_ERR_CHECK_CARD_LOCK;
        }
        else
        {
            if(cardOpRtcIsValid()==0)
            {
                re=0;
            }
            else
            {
                re = CheckCardDate(Card->PublicInfo.EffectiveDate, Card->PublicInfo.InvaildDate);          //��⿨��Чʱ��
            }
        }
        if(re != 0)
            return re;
    }
    re = ReadUserCardInfoAll((char *)Card->buf);  //���û���Ӧ����Ϣ�ļ�
    if(re != 0)
        return re;
    re = CardReadBalance(&tCharge);    //��Ǯ�����
    if(re != 0)
        return re;
    OV_DispLoading(3);
    switch(cardtype)
    {
    case 0x1f://������
        copy_str_roll((char *)&tRemainTimes, (char *)Card->UserCardInfo.CheckChargeTimes, 4);
        if(tRemainTimes)
        {
            re = Esam_ChargeNum(0); //��0ֵ���ڼ�⿨Ƭ��Ч��
            if(re != 0)
                break;
            OV_DispLoading(4);
            //������ͬ������ʹ����������ֵ
            if(cardOpGetValue(BASEVAL_Remain) != tCharge)
            {
                cardOpSetValue(BASEVAL_Remain, tCharge);
                EsamWriteRemain(0, tCharge);
                EsamReadBalance(&buflong);//��Ǯ������
                OV_DispLoading(5);
                if(buflong > tCharge)
                {
                    //Ǯ����������
                    Esam_ConsumeNum(buflong - tCharge);
                }
                else
                {
                    //Ǯ����������
                    Esam_ChargeNum(tCharge - buflong);
                }
                tRemainTimes--;//����ʹ�ô���
                FormatRoll((unsigned char *)&tRemainTimes, 4);
                re = WriteUserCardInfo((Card->UserCardInfo.CheckChargeTimes - Card->buf), (unsigned char *)&tRemainTimes, 4);
            }
        }
        else
        {
            re = USER_ERR_CHECK_TIMES_0;
            //            tRemainTimes=100000;
            //            FormatRoll((unsigned char *)&tRemainTimes,4);
            //            re=WriteUserCardInfo((Card->UserCardInfo.CheckChargeTimes-Card->buf),(unsigned char *)&tRemainTimes,4);
        }
        break;
    case 0x12:
        if(CardOpData.TempCharge == 0)
        {
            if(cardOpMeterIsInuse()==1)
            {
                if(cardOpCheckMeterID(Card->UserCardInfo.UserCode, 8) != 0)
                {
                    re = USER_ERR_CARDID;
                }
            }
            else
            {
                re = USER_ERR_CARDID;
            }
            break;
        }
    case 0x11://������
        if(CardOpData.TempCharge == 0) //�˶Կ���
        {
            if(cardOpMeterIsInuse()==1)
            {
                if(cardOpCheckMeterID(Card->UserCardInfo.UserCode, 8) != 0)
                {
                    re = USER_ERR_CARDID;
                    break;
                }
            }
            else
            {
                //EsamClearRemain();
//                re = WriteEsamUserInfo(0x08, (char *)Card->UserCardInfo.UserCode, 8);
//                if(re != 0)
//                    re = USER_ERR_UPDATE_BINARY;
                re = USER_ERR_CARDID;
                break;
            }
            UserCardUpdateInfo();
            OV_DispLoading(4);
            re = UserCardCharge(tCharge);
            OV_DispLoading(5);
            if(re == 0)
                EsamSetTranMark(ESAM_TRAN_MARK_LOCK_TRAN);
            break;
        }
    case 0x10://������
        //д�뿨��
        if(cardOpMeterIsInuse()==0)
        {
            EsamClearRemain();
            re = WriteEsamUserInfo(0x08, (char *)Card->UserCardInfo.UserCode, 8);
            if(re != 0)
                re = USER_ERR_UPDATE_BINARY;
            else
            {
                cardOpWriteMeterID(Card->UserCardInfo.UserCode, 8);
                UserCardUpdateInfo();
                OV_DispLoading(4);
                re = UserCardCharge(tCharge);
            }
        }
        else
        {
            if(cardOpCheckMeterID(Card->UserCardInfo.UserCode, 8) != 0)
            {
                //                memcpy(ErrBackStr,Meter.CardId,8);
                re = USER_ERR_HAD_USER;
            }
            else
            {
                UserCardUpdateInfo();
                OV_DispLoading(4);
                re = UserCardCharge(tCharge);
            }
        }
        OV_DispLoading(5);
        if(re == 0)
        {
            EsamGetTranMark(&buflong);
            if(buflong != ESAM_TRAN_MARK_HAD_TRAN)
                EsamSetTranMark(ESAM_TRAN_MARK_CAN_TRAN);
        }
        break;
    default:
        break;
    }

    //    if(re!=0)
    //    {
    //        if(IOCardBslRead())
    //            return USER_ERR_NOCARD;
    //        else
    //            return re;
    //    }
    return re;
}

//���ÿ�
unsigned char DealSetupCard(void)
{
    //    unsigned long buflong;
    unsigned char re;
    pStu_EsamInfo pEsamInfo;
    pStu_MeterOpFile pMeterOpFile;

    if(ReadSetupCardInfoAll((char *)Card->buf) != 0) //���û���Ӧ����Ϣ�ļ�
        return USER_ERR_READ_BINARY;

    //��ȡEsam�ڵ���Ϣ
    Esam_Init();
    //if(Esam_SelectFile(0xdf01)!=0)
    //    return USER_ERR_SELECT_FILE;
    pEsamInfo = (pStu_EsamInfo)EsamDataBuf;
    if(EsamReadAll((char *)pEsamInfo) != 0)
        return USER_ERR_READ_BINARY;

    //���²���
    if(Card->SetupCardInfo.OpCode1.UpdateMeterPrice)        //���±��ڵ���
        memcpy(pEsamInfo->ThisPrice, Card->SetupCardInfo.ThisPrice, 4);
    if(Card->SetupCardInfo.OpCode1.UpdateUseType)           //����������������
        pEsamInfo->UseType = Card->SetupCardInfo.UseType;
    if(Card->SetupCardInfo.OpCode1.UpdateCMC)               //�������ù�˾����
        memcpy(pEsamInfo->CompanyCode, Card->SetupCardInfo.CompanyCode, 2);
    if(Card->SetupCardInfo.OpCode1.UpdatetIssueCode)        //�������ó���ȼ������
        memcpy(pEsamInfo->IssueCode, Card->SetupCardInfo.IssueCode, 6);
    if(Card->SetupCardInfo.OpCode2.UpdateMeterTime)         //�������ñ���ʱ��
    {
        if(SetMeterTime(Card->SetupCardInfo.MeterTime) != 0)
        {
            extern void OV_DispErrNum(unsigned char ErrNum);
            extern void cardDelayMs(unsigned int ms);
            OV_DispErrNum(USER_ERR_INVALID_TIME);
            cardDelayMs(1000);
        }
    }
    if(Card->SetupCardInfo.OpCode2.UpdateIdleClose)         //�������ò������ط�����
    {
        pEsamInfo->IdleClose = Card->SetupCardInfo.IdleClose;
        pEsamInfo->LimitIdleCloseTimes = Card->SetupCardInfo.LimitIdleCloseTimes;
    }
    if(Card->SetupCardInfo.OpCode2.UpdateWarningClose)      //��������Ԥ�ط�ֵ
        memcpy(pEsamInfo->WarningClose, Card->SetupCardInfo.WarningClose, 2);
    if(Card->SetupCardInfo.OpCode2.UpdateStatisticalDate)   //����������������ͳ������
        pEsamInfo->StatisticalDate = Card->SetupCardInfo.StatisticalDate;
    if(Card->SetupCardInfo.OpCode2.UpdateMaxFlow)           //�������ô���������
        pEsamInfo->MaxFlow = Card->SetupCardInfo.MaxFlow;
    if(Card->SetupCardInfo.OpCode2.UpdateOverdraught)       //��������͸֧�޶�
        memcpy(pEsamInfo->Overdraught, Card->SetupCardInfo.Overdraught, 2);
    if(Card->SetupCardInfo.OpCode2.UpdateWarning)           //�������ñ�������
        memcpy(pEsamInfo->WarningNum, Card->SetupCardInfo.WarningNum, 2);
    if(Card->SetupCardInfo.OpCode2.UpdateChargeMax)         //�������ó�ֵ�޶�
        memcpy(pEsamInfo->ChargeLimit, Card->SetupCardInfo.ChargeLimit, 4);
    if(Card->SetupCardInfo.OpCode3.CleanEsamMeterID)        //���ESAM�ڱ��
        memset(pEsamInfo->UserCode, 0x00, 8);
    //����У�飬д����Ϣ
    pEsamInfo->CRC2 = CrcAddGet((unsigned char *)pEsamInfo, 0x8e);
    re = WriteEsamUserInfo(0x00, (char *)pEsamInfo, 0x90);
    if(re != 0)
        return re;

    pMeterOpFile = (pStu_MeterOpFile)EsamDataBuf; //&Meter.UserPara.MeterOpFile;
    if(Card->SetupCardInfo.OpCode3.ReleaseDoor)        //��������Ȩ��
    {
        pMeterOpFile->IdleCloseTimes = 0;
        if(WriteEsamOpInfo(0x07, (char *)&pMeterOpFile->IdleCloseTimes, 1) != 0)
            return USER_ERR_UPDATE_BINARY;
    }

    if(Card->SetupCardInfo.OpCode3.CleanEsamInfo)      //���ESAMģ��������Ϣ
    {
        memset(pMeterOpFile, 0x00, 0x70);
        if(WriteEsamOpInfo(0x00, (char *)pMeterOpFile, 0x70) != 0)
            return USER_ERR_UPDATE_BINARY;
        EsamClearRemain();
        //        EsamReadBalance(&buflong);
        //        Esam_ConsumeNum(buflong);
    }
    return 0;
}

//��鿨
unsigned char DealInfoCheckCard(void)
{
    long buflong;
    int bufint;
    unsigned char re, i;
    pStu_EsamInfo pEsamInfo;
    pStu_MeterOpFile pMeterOpFile;
    //RefreshMeterTime();
    //0x00--0x6f
    //��ȡEsam�ڵ���Ϣ
    Esam_Init();
    //if(Esam_SelectFile(0xdf01)!=0)
    //    return USER_ERR_SELECT_FILE;
    pEsamInfo = (pStu_EsamInfo)EsamDataBuf;
    if(EsamReadAll((char *)pEsamInfo) != 0)
        return USER_ERR_READ_BINARY;
    memcpy(Card->buf, pEsamInfo, 0x1f);
    *(Card->buf + 0x1e) = cardOpGetValue(USERVAL_IdleCloseTimes);;
    pMeterOpFile = (pStu_MeterOpFile)EsamDataBuf;
    if(ReadEsamOpInfo(0x00, (char *)pMeterOpFile, sizeof(Stu_MeterOpFile)) != 0)
        return USER_ERR_READ_BINARY;

    buflong = cardOpGetValue(BASEVAL_AmountGas) / 100;
    bufint = (cardOpGetValue(BASEVAL_AmountGas) % 100) * 100;
    copy_str_roll((char *)(Card->buf + 0x1f), (char *)&buflong, 4);
    copy_str_roll((char *)(Card->buf + 0x23), (char *)&bufint, 2);

    memcpy(Card->buf + 0x37, &pMeterOpFile->MagnetTimes, 3);
    pMeterOpFile->MeterReg = cardOpGetValue(USERVAL_MeterRegValue);
    memcpy(Card->buf + 0x3b, &pMeterOpFile->MeterReg, 1);
    for(i = 0; i < 12; i++)
    {
        copy_str_roll((char *)(Card->buf + 0x3c + i * 4), (char *)(pMeterOpFile->MonthData + i * 4), 4);
    }
    //�忨����
    Card->buf[0x6c] = 0;//Globe.MeterTime.Year;
    Card->buf[0x6d] = 0;//Globe.MeterTime.Month;
    Card->buf[0x6e] = 0;//Globe.MeterTime.Day;
    Card->buf[0x6f] = 0;//Globe.MeterTime.Hours;
    //�ܹ�����
    //if(ReadEsamChargeInfo(0x00,(char *)EsamDataBuf,sizeof(Stu_ChargeFile))!=0)
    //    return USER_ERR_READ_BINARY;
    buflong = cardOpGetValue(BASEVAL_ChargeALL) / 100;
    bufint = (cardOpGetValue(BASEVAL_ChargeALL) % 100) * 100;
    copy_str_roll((char *)(Card->buf + 0x31), (char *)&buflong, 4);
    copy_str_roll((char *)(Card->buf + 0x35), (char *)&bufint, 2);
    //memcpy(Card->buf+0x31,EsamDataBuf,6);

    //ʣ����
    //    EsamSetRemain();
    buflong = cardOpGetValue(BASEVAL_Remain);
    //if(buflong<0)
    //    buflong=0;
    bufint = buflong % 100;
    bufint *= 100;
    buflong /= 100;
    copy_str_roll((char *)Card->buf + 0x2b, (char *)&buflong, 4);
    copy_str_roll((char *)Card->buf + 0x2f, (char *)&bufint, 2);
    //0x70--0xbf//Ԥ��

    re = CardWriteFileSecurity(0x0c, 0x00, (char *)Card->buf, 0x70);
    return re;
}

//����ת�ƿ�
unsigned char DealTransCard(void)
{
    unsigned long buflong;
    unsigned char re = 0;
    if(Card_ReadBinary(0x0c, 0x00, (char *)Card->buf, 0x22) != 0) //�����ת����Ϣ�ļ�
        return USER_ERR_READ_BINARY;
    if(Card->TransCardInfo.State == 0xaa)
        return USER_ERR_TRANS_END;
    Esam_Init();
    //if(Esam_SelectFile(0xdf01)!=0)
    //    return USER_ERR_SELECT_FILE;

    switch(Card->TransCardInfo.State)
    {
    case 0x00://����
        if(cardOpMeterIsInuse()==0)
            return USER_ERR_NOUSER;
        re = DataTranOut();
        if(re == 0)
        {
            EsamSetTranMark(ESAM_TRAN_MARK_HAD_TRAN);
        }
        break;
    case 0xa5://ת��
        EsamGetTranMark(&buflong);
        if(buflong == ESAM_TRAN_MARK_HAD_TRAN)
            return USER_ERR_HAD_TRANS_ONCE;
        else if(buflong == ESAM_TRAN_MARK_LOCK_TRAN)
            return USER_ERR_TRAN_LOCK;
        else if(buflong != ESAM_TRAN_MARK_CAN_TRAN) //û�п���
            return USER_ERR_TRANS_STATE;
        re = DataTranIn();
        if(re == 0)
        {
            EsamSetTranMark(ESAM_TRAN_MARK_HAD_TRAN);
        }
        break;
    default:
        re = USER_ERR_TRANS_STATE;
        break;
    }
    return re;
}

//�����
unsigned char DealClearCard(void)
{
    unsigned long buflong;
    unsigned int tCrc16;
    unsigned char re = 0;
    pStu_EsamInfo pEsamInfo;
    pStu_MeterOpFile pMeterOpFile;

    if(Card_ReadBinary(0x0a, 0x00, (char *)Card->buf, 0x04) != 0) //��Ӧ����Ϣ�ļ�
        return USER_ERR_READ_BINARY;

    //��ȡEsam�ڵ���Ϣ
    Esam_Init();
    //if(Esam_SelectFile(0xdf01)!=0)
    //    return USER_ERR_SELECT_FILE;

    if(Card->ClearCardInfo.OpCode2.All == 0)              //�ָ��������ã�Ԥ����
    {
        //������Ϣ�ļ�
        memset(EsamDataBuf, 0x00, 0x0f);
        if(Card->ClearCardInfo.OpCode1.AmountCharge)       //���ۻ�������
            re = WriteEsamChargeInfo(0x00, (char *)EsamDataBuf, 0x08);

        if(Card->ClearCardInfo.OpCode1.ClearMeterRnd)      //������ڹ��������
            re = WriteEsamChargeInfo(0x08, (char *)EsamDataBuf, 0x06);
        if(re != 0)
            return USER_ERR_UPDATE_BINARY;

        //Ӧ����Ϣ�ļ�
        if(Card->ClearCardInfo.OpCode1.UserID)             //���û���
            re = WriteEsamUserInfo(0x08, (char *)EsamDataBuf, 8);
        if(re != 0)
            return re;

        //�����ļ�
        pMeterOpFile = (pStu_MeterOpFile)EsamDataBuf;
        if(ReadEsamOpInfo(0x00, (char *)EsamDataBuf, 0x70) != 0)
            return USER_ERR_READ_BINARY;
        if(Card->ClearCardInfo.OpCode1.MonthData)          //�����ۻ�������������
            memset(pMeterOpFile->MonthData, 0x00, 48);
        if(Card->ClearCardInfo.OpCode1.TotalUse_Money)     //���ۻ�������Ԥ����
            memset(pMeterOpFile->TotalUseMoney, 0x00, 8);
        if(Card->ClearCardInfo.OpCode1.TotalUse_Gas)       //���ۻ���������
        {
            memset(pMeterOpFile->TotalUseInt, 0x00, 8);
            pMeterOpFile->blank2[2] = 0;
        }
        if(Card->ClearCardInfo.OpCode2.OpenTimes)          //�忪������
        {
            pMeterOpFile->OpenTimes = 0x00;
            cardOpSetValue(USERVAL_IdleCloseTimes,0);
        }
        if(Card->ClearCardInfo.OpCode2.MaxFlowTimes)       //�����������
            pMeterOpFile->MaxFlowTimes = 0x00;
        if(Card->ClearCardInfo.OpCode2.MagnetTimes)        //���Ÿ�����
            pMeterOpFile->MagnetTimes = 0x00;

        tCrc16 = Crc16Get((unsigned char *)EsamDataBuf, 0x6e);
        memcpy(pMeterOpFile->CRC16, &tCrc16, 2);
        //DelayMs(10);
        re = WriteEsamOpInfo(0x00, (char *)pMeterOpFile, 0x70);
        if(re != 0)
            return USER_ERR_UPDATE_BINARY;
        //Ǯ���ļ�
        if(Card->ClearCardInfo.OpCode1.EsamBalance)        //��ESAM��Ǯ��
        {
            EsamClearRemain();
            //            EsamReadBalance(&buflong);
            //            Esam_ConsumeNum(buflong);
        }
    }
    else
    {
        if((Card->ClearCardInfo.OpCode1.Value == 0x04) &&
                (Card->ClearCardInfo.OpCode2.Value == 0x81))
        {
            memset(EsamDataBuf, 0x00, 0x90);
            WriteEsamOpInfo(0x00, (char *)EsamDataBuf, 0x70);       //���������Ϣ�ļ�
            WriteEsamChargeInfo(0x00, (char *)EsamDataBuf, 0x0f);   //�����ֵ��Ϣ�ļ�
            Esam_WriteBinary(0x0f, 0x00, (char *)EsamDataBuf, 0x10); //�����ʱ��Ϣ�ļ�

            pEsamInfo = (pStu_EsamInfo)EsamDataBuf;
            pEsamInfo->UseType = 0x01; //����ʽ
            pEsamInfo->WarningNum[0] = 0x00; //Ĭ��5.00
            pEsamInfo->WarningNum[1] = 0x05;
            pEsamInfo->Overdraught[0] = 0x00;
            pEsamInfo->Overdraught[1] = 0x00; //Ĭ��0
            buflong = 999999;
            copy_str_roll((char *)pEsamInfo->ChargeLimit, (char *)&buflong, 4);
            pEsamInfo->StatisticalDate = 0x01;
            re = WriteEsamUserInfo(0x00, (char *)EsamDataBuf, 0x90); //����û���Ϣ�ļ�
            if(re != 0)
                return re;
            EsamClearRemain();
        }
        else
            return USER_ERR_CLEAR_ALL;
    }
    return 0;
}


unsigned char UserReadCard(void)
{
    unsigned char re = 0, cardtype, init;
    init = Card_Init();
    if(init == 0)
    {
        cardtype = GetCardType();
        Esam_DeInit();
        Esam_Init();
        switch(cardtype)
        {
        case 0x10://������
        case 0x11://������
            OV_DispLoading(1);
            //SaveToEsam();
            EsamWriteRemain(0, cardOpGetValue(BASEVAL_Remain)); //���浱ǰ����
            re = DealUserCard(cardtype);
            OV_DispLoading(6);
            UserCardWriteBack(re);
            OV_DispLoading(7);
            if(re == 0)
            {
                OV_DispEnd();
                UpdateFromEsam(ESAM_UPDATE_REMAIN);
                UpdateFromEsam(ESAM_UPDATE_APP_INFO);
            }
            else
                UpdateFromEsam(ESAM_UPDATE_REMAIN);
            break;
        case 0x12://��֤����
            OV_DispLoading(1);
            re = DealUserCard(cardtype);
            UserCardWriteBack(re);
            OV_DispLoading(7);
            break;
        case 0x1f://����1����
            //WriteCardDate();
            OV_DispLoading(1);
            EsamWriteRemain(0, cardOpGetValue(BASEVAL_Remain)); //���浱ǰ����
            re = DealUserCard(cardtype);
            OV_DispLoading(6);
            if(re != USER_ERR_CHECK_CARD_LOCK)
                UpdateFromEsam(ESAM_UPDATE_REMAIN);
            OV_DispLoading(7);
            break;
        case 0x1e://����ת�ƿ�
            OV_DispLoading(7);
            SaveToEsam();
            re = DealTransCard();
            UpdateFromEsam(ESAM_UPDATE_ALL);
            break;
        case 0x20://��Ϣ��鿨
            OV_DispLoading(7);
            SaveToEsam();
            re = DealInfoCheckCard();
            break;
        case 0x30://Ӧ�����ÿ�
            OV_DispLoading(7);
            SaveToEsam();
            re = DealSetupCard();
            UpdateFromEsam(ESAM_UPDATE_ALL);
            break;
        case 0x50://���㿨
            OV_DispLoading(7);
            SaveToEsam();
            re = DealClearCard();
            UpdateFromEsam(ESAM_UPDATE_ALL);
            break;
        case 0xff:
            re = USER_ERR_UNKNOW;
            break;
        case 0xfe:
            re = USER_ERR_SELECT_FILE;
            break;
        default:
            re = USER_ERR_UNKNOW;
            break;
        }
    }
    else
    {        
        re = USER_ERR_UNKNOW;
    }
    Esam_DeInit();
    Card_DeInit();
    return re;
}





