#include "string.h"
#include "UserOP.h"
#include "UserDataType.h"
#include "cardapp.h"
#include "UserErrCode.h"

#include "cardOperation.h"
#include "count.h"

#include "common.h"

extern CpuFile_Stu CpuFileBuf;
extern unsigned char EsamDataBuf[0x90];
StuUser userPara;

void TestClear(void)
{
    Esam_Init();
    EsamClearBalance();
    cardOpSetValue(BASEVAL_Remain, 0);
    cardOpSetValue(USERVAL_Charge, 0);
    EsamWriteRemain(0, 0);

    memset(EsamDataBuf, 0, 0x70);
    WriteEsamOpInfo(0x00, (char *)EsamDataBuf, 0x70);

    UpdateFromEsam(ESAM_UPDATE_REMAIN | ESAM_UPDATE_OP_INFO);
}

//����ESAM�ڵ���Ϣ
//��������Ϣ�ļ�
//������Ϣ��Ǯ��+��ʱ�ļ���
/**
 * ��ESAM�ڴ洢����
 * 
 * @author Zzzzz (2018/3/13)
 * 
 * @return unsigned char 
 */
unsigned char SaveToEsam(void)
{
    unsigned long tLong1;
    long tLong2;
    unsigned char re = 0;
    
    pStu_MeterOpFile pMeterOpFile;

    //д����
    Esam_DeInit();
    Esam_Init();
    EsamReadBalance(&tLong1);//=EsamReadRemain();
    tLong2 = cardOpGetValue(BASEVAL_Remain);
    if(tLong2 > 0)
    {
        if(tLong1 > tLong2)
            Esam_ConsumeNum(tLong1 - tLong2);
    }
    EsamWriteRemain(0, tLong2);

    //д��������Ϣ�ļ�
    pMeterOpFile = (pStu_MeterOpFile)EsamDataBuf;
    pMeterOpFile->MagnetTimes = userPara.MagnetTimes;
    pMeterOpFile->MaxFlowTimes = userPara.MaxFlowTimes;
    pMeterOpFile->OpenTimes = userPara.OpenTimes;
    pMeterOpFile->MeterReg = userPara.MeterReg.value;
    pMeterOpFile->IdleCloseTimes = userPara.IdleCloseTimes;

    userPara.TotalUseInt = cardOpGetValue(BASEVAL_AmountGas) / 100;
    userPara.TotalUseDec = (cardOpGetValue(BASEVAL_AmountGas) % 100) * 100000;
    copy_str_roll((char *)pMeterOpFile->TotalUseInt, (char *)&userPara.TotalUseInt, 4);
    copy_str_roll((char *)pMeterOpFile->TotalUseDec, (char *)&userPara.TotalUseDec, 4);
    memset(pMeterOpFile->TotalUseMoney, 0x00, 8);
    memcpy(pMeterOpFile->MonthData, userPara.MonthAmountUse, 48);

    //pMeterOpFile->blank2[0] = Meter.HallState;
    //pMeterOpFile->blank2[2] = Meter.AmountValid;
    
    re = WriteEsamOpInfo(0x00, (char *)EsamDataBuf, 0x70);
    if(re)
    {
        Esam_DeInit();
        Esam_Init();
        re = WriteEsamOpInfo(0x00, (char *)EsamDataBuf, 0x70);
    }
    return re;
}

/**
 * ��ESAM�и��²���
 * 
 * @author Zzzzz (2018/3/13)
 * 
 * @param updateCode �������ݱ��
 * 
 * @return unsigned char 
 *         �Ƿ�ɹ��ı�ǣ�0-�ɹ�����0-����������
 */
unsigned char UpdateFromEsam(unsigned char updateCode)
{
    unsigned long buflong;
    long buflong2;
    unsigned char re = 0, RetryTimes;
    pStu_EsamInfo pEsamInfo;
    pStu_ChargeFile pChargeFile;
    pStu_MeterOpFile pMeterOpFile;

    pEsamInfo = (pStu_EsamInfo)EsamDataBuf; //&Meter.UserPara.EsamUserFile;
    Esam_DeInit();
    Esam_Init();
    //����Ǯ��
    if(updateCode & ESAM_UPDATE_REMAIN)
    {
        EsamReadRemain(&buflong2);//���Զ���������
        {
            if(EsamReadBalance(&buflong) == 0) //��esam����
            {
                if(buflong2 < 0) //����Ϊ����
                {
                    cardOpSetValue(BASEVAL_Remain, buflong2); //����ǰ����
                    Esam_ConsumeNum(buflong);//���ESAM����
                }
                else//����Ϊ�������۳�esam�ж��ֵ
                {
                    if(buflong > buflong2)
                    {
                        Esam_ConsumeNum(buflong - buflong2);
                        buflong = buflong2;
                    }
                    cardOpSetValue(BASEVAL_Remain, buflong); //����ǰ����
                }
            }
            else
            {
                //��ʧ���ٶ�һ��
                if(EsamReadBalance(&buflong) != 0)
                {
                    buflong = 0;
                    cardOpSetValue(BASEVAL_Remain, buflong); //����ǰ����
                }
            }
        }
    }

    //��Ӧ���ļ�
    if(updateCode & ESAM_UPDATE_APP_INFO)
    {
        re = EsamReadAll((char *)pEsamInfo);
        //��ȡʧ�ܡ�����
        
        
        //��ȡ���̷��еı��
        //Esam_ReadBinary(0x0f, 0x10, (char *)Meter.MeterISSUID, 32);
        {
            cardOpWriteMeterID(pEsamInfo->UserCode, 8);
            pEsamInfo->UseType = 1;
            if(pEsamInfo->UseType == 1)
            {
                
            }
//            copy_str_roll((char *)&Meter.Maxremain, (char *)pEsamInfo->ChargeLimit, 4);
//            if(Meter.Maxremain > 9999999)
//                Meter.Maxremain = 9999999;
//            Meter.Maxremain *= 100;
//            Meter.UserPara.JustWarning = CharUnionInt(pEsamInfo->WarningNum[0], pEsamInfo->WarningNum[1]); //0x100l*pEsamInfo->WarningNum[0]+pEsamInfo->WarningNum[1];
//            Meter.UserPara.JustWarning *= 100;
//            Meter.Overdraught = CharUnionInt(pEsamInfo->Overdraught[0], pEsamInfo->Overdraught[1]); //0x100l*pEsamInfo->Overdraught[0]+pEsamInfo->Overdraught[1];
//            Meter.Overdraught *= 100;
//            Meter.Maxflow = pEsamInfo->MaxFlow;
//            Meter.UserPara.StatisticalDate = pEsamInfo->StatisticalDate;
//            Meter.Warning = CharUnionInt(pEsamInfo->WarningClose[0], pEsamInfo->WarningClose[1]); //0x100*pEsamInfo->WarningClose[0]+pEsamInfo->WarningClose[1];
//            Meter.Warning *= 100;
//            Meter.UnusedCloseTime = pEsamInfo->IdleClose;
//            Meter.UnusedCloseTime *= 24;
//            Meter.LimitCloseTimes = pEsamInfo->LimitIdleCloseTimes;
//            Meter.Price = 10000;
        }
    }
    //����������ļ�
    if(updateCode & ESAM_UPDATE_OP_INFO)
    {
        //��ȡ����
        pMeterOpFile = (pStu_MeterOpFile)EsamDataBuf;
        re = ReadEsamOpInfo(0x00, (char *)pMeterOpFile, 0x70);
        

        {
            //��������
//            Meter.UserPara.MagnetTimes = pMeterOpFile->MagnetTimes;
//            Meter.UserPara.MaxFlowTimes = pMeterOpFile->MaxFlowTimes;
//            Meter.UserPara.OpenTimes = pMeterOpFile->OpenTimes;
//            Meter.UserPara.MeterReg.value = pMeterOpFile->MeterReg;
//            Meter.UserPara.IdleCloseTimes = pMeterOpFile->IdleCloseTimes;
//            copy_str_roll((char *)&Meter.UserPara.TotalUseInt, (char *)pMeterOpFile->TotalUseInt, 4);
//            copy_str_roll((char *)&Meter.UserPara.TotalUseDec, (char *)pMeterOpFile->TotalUseDec, 4);
//            Meter.AmountGas = Meter.UserPara.TotalUseInt * 100 + Meter.UserPara.TotalUseDec / 100000;
//            memcpy(&Meter.UserPara.MonthAmountUse, pMeterOpFile->MonthData, 48);
//
//            Meter.HallState = pMeterOpFile->blank2[0];
//            Meter.AmountValid = pMeterOpFile->blank2[2];
        }
    }

    //����������Ϣ�ļ�
    if(updateCode & ESAM_UPDATE_CHARGE_INFO)
    {
        unsigned long tChargeAmount;
        pChargeFile = (pStu_ChargeFile)EsamDataBuf;
        ReadEsamChargeInfo(0x00, (char *)pChargeFile, 0x0e);
        copy_str_roll((char *)&buflong, (char *)(pChargeFile->ChargeAllInt), 4);
        tChargeAmount = buflong;
        copy_str_roll((char *)&buflong, (char *)(pChargeFile->ChargeAllDec), 4);
        tChargeAmount *= 100;
        tChargeAmount += buflong / 100000;
        cardOpSetValue(BASEVAL_ChargeALL,tChargeAmount);
    }

//    Meter.Setup.CloseWarning = SETUP_ON;
//    Meter.Setup.Precision = PRECISION_001;

//    memset(EsamDataBuf, 0, 8);
//    if(memcmp(EsamDataBuf, Meter.CardId, 8) == 0)
//        SetState(STATE_METER, STATE_METER_CHECK);
//    else
//        SetState(STATE_METER, STATE_METER_INUSE);

    return re;
}

unsigned char SetMeterTime(unsigned char *tTime)
{
    StuDate tDate;
    tDate.Year = tTime[0];
    tDate.Month = tTime[1];
    tDate.Day = tTime[2];
    tDate.Hours = tTime[3];
    tDate.Minutes = tTime[4];
    tDate.Seconds = tTime[5];
    if(tDate.Year > 0x99      || tDate.Month > 0x12     ||  tDate.Day > 0x31 ||
            tDate.Hours >= 0x24     || tDate.Minutes >= 0x60   ||  tDate.Seconds >= 0x60)
        return 1;
    //UpdateMeterTime(&tDate);
    return 0;
}

unsigned char CheckCardDate(unsigned char *tEffectiveDate, unsigned char *tInvaildDate)
{
    return 0;
}
void EsamClearBalance(void)
{
    unsigned long buflong;
    //���Ǯ���ڵĴ���
    EsamReadBalance(&buflong);
    if(buflong != 0)
        Esam_ConsumeNum(buflong);
}

//��Esam�ж�ȡ����
unsigned char EsamReadRemain(long *tReturn)
{
    unsigned char type;
    long tRemain = 0;
    unsigned int tCrc16 = 0;
    Esam_Init();

    //������
    Esam_ReadBinary(0x0f, 0x00, (char *)&tRemain, 4);
    Esam_ReadBinary(0x0f, 0x04, (char *)&tCrc16, 2);
    Esam_ReadBinary(0x0f, 0x06, (char *)&type, 1);
    if(tCrc16 == Crc16Get((unsigned char *)&tRemain, 4))
    {
        *tReturn = tRemain;
    }
    else
    {
        *tReturn = 0;
    }
    return type;
}

//��Esam��д�����
unsigned char EsamWriteRemain(unsigned char type, long tRemain)
{
    unsigned int tCrc16;
    Esam_Init();
    tCrc16 = Crc16Get((unsigned char *)&tRemain, 4);
    Esam_WriteBinary(0x0f, 0x00, (char *)&tRemain, 4);
    Esam_WriteBinary(0x0f, 0x04, (char *)&tCrc16, 2);
    Esam_WriteBinary(0x0f, 0x06, (char *)&type, 1);
    return 0;
}

void EsamClearRemain(void)
{
    cardOpSetValue(USERVAL_Charge, 0);
    EsamWriteRemain(0, 0);
    cardOpSetValue(BASEVAL_Remain, 0);
    EsamClearBalance();
}

void EsamSetTranMark(unsigned long tlong)
{
    Esam_WriteBinary(0x0f, 0x07, (char *)&tlong, 4);
}

void EsamGetTranMark(unsigned long *tlong)
{
    Esam_ReadBinary(0x0f, 0x07, (char *)tlong, 4);
}

unsigned char MeterCharge(unsigned char tChargeType, unsigned long T_charge)
{
    countCharge(T_charge);   
    return 0;
}

