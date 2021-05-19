#include <string.h>

#include "Iso7816.h"
#include "hwCtrl.h"
#include "cardOperation.h"

const unsigned char CmdSelectFile[5] =      {0x00, 0xA4, 0x00, 0x00, 0x02};//选择文件
//const unsigned char CmdReadPublic[5] =      {0x00, 0xB0, 0x95, 0x00, 0x70};//选择文件
//const unsigned char CmdReadUserCardInfo[5] ={0x00, 0xB0, 0x8a, 0x00, 0x92};//选择文件
//const unsigned char CmdReadSetupCardInfo[5]={0x00, 0xB0, 0x8a, 0x00, 0x90};//选择文件
const unsigned char CmdGetBalance[5] =      {0x80, 0x5c, 0x00, 0x02, 0x04};
//const unsigned char CmdGetCSN[5] =          {0x00, 0xb0, 0x88, 0x08, 0x08};
const unsigned char CmdGetRND[5] =          {0x00, 0x84, 0x00, 0x00, 0x08};
//const unsigned char CmdEsamCalCSN[5] =      {0x80, 0xfa, 0x00, 0x03, 0x08};
//const unsigned char CmdEsamCalDES[5]=       {0x80, 0xF8, 0x00, 0x00, 0x08};
const unsigned char CmdExtAuther[5] =        {0x00, 0x82, 0x00, 0x00, 0x08};
const unsigned char CmdIntAuther[5] =        {0x00, 0x88, 0x00, 0x00, 0x08};
const unsigned char CmdConsumeInit[5] =      {0x80, 0x50, 0x01, 0x02, 0x0B};
const unsigned char CmdConsume[5] =          {0x80, 0x54, 0x01, 0x00, 0x0F};
//const unsigned char CmdReadConsumeKey[5]=   {0x00, 0xB0, 0x88, 0x10, 0x10};
const unsigned char CmdCharge[5] =           {0x80, 0x52, 0x00, 0x00, 0x0B};

Stu_CpuApdu IsoApdu;
extern void copy_str_roll(char *buf1, char *buf2, unsigned int len);


unsigned int ISO_SendApdu(eApduPipe tApduPipe, pCpuApdu tIsoApdu)
{
    unsigned int re = 0;
    if(tApduPipe == APDU_PIPE_CARD)
    {
        re = Card_SendApdu(tIsoApdu);
    }
    else if(tApduPipe == APDU_PIPE_ESAM)
    {
        re = Esam_SendApdu(tIsoApdu);
    }
    return re;
}
unsigned char ISO_ReadBinary(eApduPipe tApduPipe, unsigned char tFile, unsigned char tAddr, char *tBuf, unsigned char tLen)
{
    unsigned int re;
    IsoApdu.Cmd.CLA = 0x00;
    IsoApdu.Cmd.INS = 0xB0;
    IsoApdu.Cmd.P1 = 0x80 | tFile;
    IsoApdu.Cmd.P2 = tAddr;
    IsoApdu.Cmd.Le = tLen;
    //DelayMs(10);
    //    if(tApduPipe==APDU_PIPE_ESAM)
    //    {
    //        DelayMs(20);
    //    }
    re = ISO_SendApdu(tApduPipe, &IsoApdu);
    if((re & 0xff00) == 0x6100)
    {
        ISO_ReadContinues(tApduPipe, (re & 0x0ff), (char *)tBuf);
        return 0;
    }
    else if(re == 0x9000) // || (re&0xff00)==0x6100)
    {
        memcpy(tBuf, IsoApdu.Cmd.Data, tLen);
        return 0;
    }
    else
        return 1;
}

unsigned char ISO_WriteBinary(eApduPipe tApduPipe, unsigned char tFile, unsigned char tAddr, char *tBuf, unsigned char tLen)
{
    unsigned int re;
    IsoApdu.Cmd.CLA = 0x00;
    IsoApdu.Cmd.INS = 0xD6;
    IsoApdu.Cmd.P1 = 0x80 | tFile;
    IsoApdu.Cmd.P2 = tAddr;
    IsoApdu.Cmd.Le = tLen;
    memcpy(IsoApdu.Cmd.Data, tBuf, tLen);
    re = ISO_SendApdu(tApduPipe, &IsoApdu);
    if(re == 0x9000)
        return 0;
    else
        return 1;
}

unsigned char ISO_WriteBinaryMac(eApduPipe tApduPipe, unsigned char tFile, char *tMac, unsigned char tAddr, char *tBuf, unsigned char tLen)
{
    unsigned int re;
    IsoApdu.Cmd.CLA = 0x04;
    IsoApdu.Cmd.INS = 0xD6;
    IsoApdu.Cmd.P1 = 0x80 | tFile;
    IsoApdu.Cmd.P2 = tAddr;
    IsoApdu.Cmd.Le = tLen + 4;
    memcpy(IsoApdu.Cmd.Data, tBuf, tLen);
    memcpy(IsoApdu.Cmd.Data + tLen, tMac, 4);
    re = ISO_SendApdu(tApduPipe, &IsoApdu);
    if(re == 0x9000)
        return 0;
    else
        return 1;
}

unsigned char ISO_SelectFile(eApduPipe tApduPipe, unsigned int fileid)
{
    unsigned int re;
    memcpy(IsoApdu.buf, CmdSelectFile, sizeof(CmdSelectFile));
    IsoApdu.Cmd.Data[0] = fileid / 0x100;
    IsoApdu.Cmd.Data[1] = fileid % 0x100;
    cardDelayMs(10);
    re = ISO_SendApdu(tApduPipe, &IsoApdu);
    if((re & 0xff00) == 0x6100)
        return 0;
    else
        return 1;
}

unsigned char ISO_GetRnd(eApduPipe tApduPipe, char *tRndBuf, unsigned char len)
{
    unsigned int re;
    memcpy(IsoApdu.buf, CmdGetRND, sizeof(CmdGetRND));
    IsoApdu.Cmd.Le = len;
    re = ISO_SendApdu(tApduPipe, &IsoApdu);
    if(re == 0x9000)
    {
        memcpy(tRndBuf, IsoApdu.Cmd.Data, len);
        return 0;
    }
    else
        return 1;
}

unsigned char ISO_GenerateKey(eApduPipe tApduPipe, unsigned char tp1, unsigned char tp2, unsigned char tle, char *tCSN)
{
    IsoApdu.Cmd.CLA = 0x80;
    IsoApdu.Cmd.INS = 0x1A;
    IsoApdu.Cmd.P1 = tp1;
    IsoApdu.Cmd.P2 = tp2;
    IsoApdu.Cmd.Le = tle;
    //memcpy(EsamApdu.buf, CmdEsamCalCSN, sizeof(CmdEsamCalCSN));
    memcpy(IsoApdu.Cmd.Data, tCSN, tle);
    if(ISO_SendApdu(tApduPipe, &IsoApdu) == 0x9000)
    {
        return 0;
    }
    else
        return 1;
}

unsigned char ISO_VerifyPin(eApduPipe tApduPipe, unsigned char tle, char *tPin)
{
    IsoApdu.Cmd.CLA = 0x00;
    IsoApdu.Cmd.INS = 0x20;
    IsoApdu.Cmd.P1 = 0x00;
    IsoApdu.Cmd.P2 = 0x00;
    IsoApdu.Cmd.Le = tle;
    memcpy(IsoApdu.Cmd.Data, tPin, tle);
    if(ISO_SendApdu(tApduPipe, &IsoApdu) == 0x9000)
    {
        return 0;
    }
    else
        return 1;
}

unsigned char ISO_ReadBalance(eApduPipe tApduPipe, unsigned long *buflong)
{
    memcpy(IsoApdu.buf, CmdGetBalance, sizeof(CmdGetBalance));
    if(ISO_SendApdu(tApduPipe, &IsoApdu) == 0x9000)
    {
        copy_str_roll((char *)buflong, (char *)IsoApdu.Cmd.Data, 4);
        return 0;
    }
    else
        return 1;
}

unsigned char ISO_ReadContinues(eApduPipe tApduPipe, unsigned char len, char *tData)
{
    unsigned int re;
    IsoApdu.Cmd.CLA = 0;
    IsoApdu.Cmd.INS = 0xC0;
    IsoApdu.Cmd.P1 = 0;
    IsoApdu.Cmd.P2 = 0;
    IsoApdu.Cmd.Le = len;
    re = ISO_SendApdu(tApduPipe, &IsoApdu);
    if(re == 0x9000)
    {
        if(tData != NULL)
            memcpy(tData, IsoApdu.Cmd.Data, len);
        return 0;
    }
    else
    {
        return 1;
    }
}

unsigned char ISO_Crypt(eApduPipe tApduPipe, unsigned char tp1, unsigned char tp2, unsigned char len, char *tData, char *reData)
{
    unsigned int re;
    IsoApdu.Cmd.CLA = 0x80;
    IsoApdu.Cmd.INS = 0xFA;
    IsoApdu.Cmd.P1 = tp1;
    IsoApdu.Cmd.P2 = tp2;
    IsoApdu.Cmd.Le = len;
    //memcpy(EsamApdu.buf, CmdEsamCalDES, sizeof(CmdEsamCalDES));
    memcpy(IsoApdu.Cmd.Data, tData, len);

    re = ISO_SendApdu(tApduPipe, &IsoApdu);
    if((re & 0xff00) == 0x6100)
    {
        ISO_ReadContinues(tApduPipe, (re & 0x0ff), (char *)reData);
        return 0;
    }
    else if(re == 0x9000)
    {
        //memcpy(reDate,);
        return 0;
    }
    else
        return 1;
}

unsigned char ISO_ConsumeInit(eApduPipe tApduPipe, unsigned long buflong, char *tBufCode, char *reData)
{
    unsigned int re;
    memcpy(IsoApdu.buf, CmdConsumeInit, sizeof(CmdConsumeInit));
    IsoApdu.Cmd.Data[0] = 0x01;
    copy_str_roll((char *)IsoApdu.Cmd.Data + 1, (char *)&buflong, 4);
    memcpy(IsoApdu.Cmd.Data + 5, tBufCode, 6);
    re = ISO_SendApdu(tApduPipe, &IsoApdu);
    if((re & 0xff00) == 0x6100)
    {
        ISO_ReadContinues(tApduPipe, (re & 0x0ff), reData);
        return 0;
    }
    else if(re == 0x9000)
    {
        return 0;
    }
    else
        return 1;
}

unsigned char ISO_Consume(eApduPipe tApduPipe, char *tbuf, char *reData)
{
    unsigned int re;
    memcpy(IsoApdu.buf, CmdConsume, sizeof(CmdConsume));
    memcpy(IsoApdu.Cmd.Data, tbuf, 15);
    re = ISO_SendApdu(tApduPipe, &IsoApdu);
    if((re & 0xff00) == 0x6100 )
    {
        ISO_ReadContinues(tApduPipe, (re & 0x0ff), reData);
        return 0;
    }
    else if(re == 0x9000)
    {
        //memcpy(reDate,);
        return 0;
    }
    else
        return 1;
}

unsigned char ISO_ChargeInit(eApduPipe tApduPipe, unsigned long buflong, char *tBufCode, char *reData)
{
    unsigned int re;
    memcpy(IsoApdu.buf, CmdConsumeInit, sizeof(CmdConsumeInit));
    IsoApdu.Cmd.P1 = 0x00; //圈存初始化

    IsoApdu.Cmd.Data[0] = 0x01;
    copy_str_roll((char *)IsoApdu.Cmd.Data + 1, (char *)&buflong, 4);
    memcpy(IsoApdu.Cmd.Data + 5, tBufCode, 6);
    re = ISO_SendApdu(tApduPipe, &IsoApdu);
    if((re & 0xff00) == 0x6100)
    {
        ISO_ReadContinues(tApduPipe, (re & 0x0ff), reData);
        return 0;
    }
    else if(re == 0x9000)
    {
        return 0;
    }
    else
        return 1;
}

unsigned char ISO_Charge(eApduPipe tApduPipe, char *tbuf)
{
    unsigned int re;
    char reData[8];
    memcpy(IsoApdu.buf, CmdCharge, sizeof(CmdCharge));
    memcpy(IsoApdu.Cmd.Data, tbuf, IsoApdu.Cmd.Le);
    re = ISO_SendApdu(tApduPipe, &IsoApdu);
    if((re & 0xff00) == 0x6100)
    {
        ISO_ReadContinues(tApduPipe, (re & 0x0ff), reData);
        return 0;
    }
    else if(re == 0x9000)
    {
        //memcpy(reDate,);
        return 0;
    }
    else
        return 1;
}

//外部认证
unsigned char ISO_ExtAuther(eApduPipe tApduPipe, unsigned char keyindex, char *tCode)
{
    unsigned int re;
    memcpy(IsoApdu.buf, CmdExtAuther, sizeof(CmdExtAuther));
    memcpy(IsoApdu.Cmd.Data, tCode, 8);
    IsoApdu.Cmd.P2 = keyindex;
    re = ISO_SendApdu(tApduPipe, &IsoApdu);
    if(re == 0x9000)
        return 0;
    else
        return 1;
}

//内部认证
unsigned char ISO_IntAuther(eApduPipe tApduPipe, unsigned char keyindex, char *tCode, char *reData)
{
    unsigned int re;
    memcpy(IsoApdu.buf, CmdIntAuther, sizeof(CmdIntAuther));
    memcpy(IsoApdu.Cmd.Data, tCode, 8);
    IsoApdu.Cmd.P2 = keyindex;
    re = ISO_SendApdu(tApduPipe, &IsoApdu);
    if((re & 0xff00) == 0x6100)
    {
        ISO_ReadContinues(tApduPipe, (re & 0x0ff), reData);
        return 0;
    }
    else if(re == 0x9000)
    {
        //memcpy(reDate,);
        return 0;
    }
    else
        return 1;
}








