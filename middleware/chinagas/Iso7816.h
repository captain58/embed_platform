#ifndef __ISO7816_H__
#define __ISO7816_H__
#define APDU_MAX_LEN    256

typedef union
{
    unsigned char buf[APDU_MAX_LEN];
    struct
    {
        unsigned char CLA;
        unsigned char INS;
        unsigned char P1;
        unsigned char P2;
        unsigned char Le;
        unsigned char Data[];
    } Cmd;
} Stu_CpuApdu, *pCpuApdu;

typedef struct _CARD_CASHE
{
    unsigned char Result[2]; //·µ»Ø½á¹û

} Stu_CardCashe;

typedef struct
{
    unsigned char (*Iso7816_ReadChar)(unsigned char *ch);
    unsigned char (*Iso7816_WriteChar)(unsigned char *ch);
} Iso7816_Stu;

typedef enum
{
    APDU_PIPE_CARD,
    APDU_PIPE_ESAM,
} eApduPipe;


unsigned char ISO_ReadBinary(eApduPipe tApduPipe, unsigned char tFile, unsigned char tAddr, char *tBuf, unsigned char tLen);
unsigned char ISO_WriteBinary(eApduPipe tApduPipe, unsigned char tFile, unsigned char tAddr, char *tBuf, unsigned char tLen);
unsigned char ISO_WriteBinaryMac(eApduPipe tApduPipe, unsigned char tFile, char *tMac, unsigned char tAddr, char *tBuf, unsigned char tLen);
unsigned char ISO_GetRnd(eApduPipe tApduPipe, char *tRndBuf, unsigned char len);
unsigned char ISO_SelectFile(eApduPipe tApduPipe, unsigned int fileid);
unsigned char ISO_GenerateKey(eApduPipe tApduPipe, unsigned char tp1, unsigned char tp2, unsigned char tle, char *tCSN);

unsigned char ISO_VerifyPin(eApduPipe tApduPipe, unsigned char tle, char *tPin);
unsigned char ISO_ReadBalance(eApduPipe tApduPipe, unsigned long *buflong);
unsigned char ISO_ReadContinues(eApduPipe tApduPipe, unsigned char len, char *tData);
unsigned char ISO_Crypt(eApduPipe tApduPipe, unsigned char tp1, unsigned char tp2, unsigned char len, char *tData, char *reData);
unsigned char ISO_ConsumeInit(eApduPipe tApduPipe, unsigned long buflong, char *tBufCode, char *reData);
unsigned char ISO_Consume(eApduPipe tApduPipe, char *tbuf, char *reData);
unsigned char ISO_ChargeInit(eApduPipe tApduPipe, unsigned long buflong, char *tBufCode, char *reData);
unsigned char ISO_Charge(eApduPipe tApduPipe, char *tbuf);
unsigned char ISO_ExtAuther(eApduPipe tApduPipe, unsigned char keyindex, char *tCode);
unsigned char ISO_IntAuther(eApduPipe tApduPipe, unsigned char keyindex, char *tCode, char *reData);

#endif

