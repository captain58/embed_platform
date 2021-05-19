#ifndef __CARD_OPERATION_H__
#define __CARD_OPERATION_H__

#include "Iso7816.h"

#define Card_SelectFile(x)              ISO_SelectFile(APDU_PIPE_CARD,x)
#define Card_ReadBinary(x,y,z,n)        ISO_ReadBinary(APDU_PIPE_CARD,x,y,z,n)
#define Card_ReadContinues(a,b)         ISO_ReadContinues(APDU_PIPE_CARD,a,b)
#define Card_WriteBinary(x,y,z,n)       ISO_WriteBinary(APDU_PIPE_CARD,x,y,z,n)
#define Card_WriteBinaryMac(x,y,z,n,m)  ISO_WriteBinaryMac(APDU_PIPE_CARD,x,y,z,n,m)
#define Card_GetRnd(x,y)                ISO_GetRnd(APDU_PIPE_CARD,x,y)
#define Card_ExtAuther(x,y)             ISO_ExtAuther(APDU_PIPE_CARD,x,y)
#define Card_IntAuther(x,y,z)           ISO_IntAuther(APDU_PIPE_CARD,x,y,z)
#define CardReadBalance(x)              ISO_ReadBalance(APDU_PIPE_CARD,x)
#define Card_GenerateKey(x,y,z,n)       ISO_GenerateKey(APDU_PIPE_CARD,x,y,z,n)
#define CardCrypt(x,y,z,n,m)            ISO_Crypt(APDU_PIPE_CARD,x,y,z,n,m)
#define CardConsumeInit(x,y,z)          ISO_ConsumeInit(APDU_PIPE_CARD,x,y,z)
#define CardConsume(x,y)                ISO_Consume(APDU_PIPE_CARD,x,y)
#define ReadPublicInfo(x,y,z)               Card_ReadBinary(0x15,x,y,z)
#define ReadUserCardInfo(x,y,z)             Card_ReadBinary(0x0a,x,y,z)
#define ReadSetupCardInfo(x,y,z)            Card_ReadBinary(0x0a,x,y,z)
#define ReadCardWriteBackInfo(x,y,z)        Card_ReadBinary(0x0c,x,y,z)
#define ReadPublicInfoAll(x)               Card_ReadBinary(0x15,0x00,x,0x70)
#define ReadUserCardInfoAll(x)             Card_ReadBinary(0x0a,0x00,x,0x92)
#define ReadSetupCardInfoAll(x)            Card_ReadBinary(0x0a,0x00,x,0x90)
#define ReadCardWriteBackInfoAll(x)        Card_ReadBinary(0x0c,0x00,x,0x60)



#define Esam_SelectFile(x)          ISO_SelectFile(APDU_PIPE_ESAM,x)
#define Esam_ReadBinary(x,y,z,n)    ISO_ReadBinary(APDU_PIPE_ESAM,x,y,z,n)
#define Esam_WriteBinary(x,y,z,n)   ISO_WriteBinary(APDU_PIPE_ESAM,x,y,z,n)
#define Esam_GenerateKey(x,y,z,n)   ISO_GenerateKey(APDU_PIPE_ESAM,x,y,z,n)
#define Esam_GetRnd(x,y)            ISO_GetRnd(APDU_PIPE_ESAM,x,y)
#define Esam_VerifyPin(x,y)         ISO_VerifyPin(APDU_PIPE_ESAM,x,y)
#define EsamReadBalance(x)          ISO_ReadBalance(APDU_PIPE_ESAM,x)
#define EsamReadContinues(x,y)      ISO_ReadContinues(APDU_PIPE_ESAM,x,y)
#define EsamCrypt(x,y,z,n,m)        ISO_Crypt(APDU_PIPE_ESAM,x,y,z,n,m)
#define EsamConsumeInit(x,y,z)      ISO_ConsumeInit(APDU_PIPE_ESAM,x,y,z)
#define EsamConsume(x,y)            ISO_Consume(APDU_PIPE_ESAM,x,y)
#define EsamChargeInit(x,y,z)       ISO_ChargeInit(APDU_PIPE_ESAM,x,y,z)
#define EsamCharge(x)               ISO_Charge(APDU_PIPE_ESAM,x)
#define Esam_IntAuther(x,y,z)       ISO_IntAuther(APDU_PIPE_ESAM,x,y,z)
#define Esam_ExtAuther(x,y)         ISO_ExtAuther(APDU_PIPE_ESAM,x,y)
#define EsamReadConsumeKey(x)       Esam_ReadBinary(0x08,0x10,x,0x10)


typedef enum
{
    BASEVAL_AmountGas,
    BASEVAL_Remain,
    USERVAL_Maxremain,
    BASEVAL_ChargeALL,
    USERVAL_Charge,
    
    USERVAL_MagnetTimes,
    USERVAL_MaxFlowTimes,
    USERVAL_OpenTimes,
    USERVAL_MeterRegValue,
    USERVAL_IdleCloseTimes,
    
}eValueType;

unsigned char cardOpRtcIsValid(void);

unsigned long cardOpGetValue(eValueType tType);
void cardOpSetValue(eValueType tType,unsigned long tVal);

unsigned char cardOpMeterIsInuse(void);

void cardOpWriteMeterID(unsigned char *tID,unsigned char tLen);
unsigned char cardOpCheckMeterID(unsigned char *tID,unsigned char tLen);

unsigned char Esam_Init(void);
unsigned int Esam_SendApdu(pCpuApdu Cmd);
void Esam_DeInit(void);

unsigned char Card_Init(void);
unsigned int Card_SendApdu(pCpuApdu Cmd);
void Card_DeInit(void);

void cardDelayMs(unsigned int ms);


#include <k_api.h>
extern ksem_t      sem_ReadCard;
void readCard_task(void *arg);

#endif


