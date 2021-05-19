#ifndef __CARDAPP_H__
#define __CARDAPP_H__

#include <stdint.h>

typedef struct
{
    uint32_t TempCharge;
}stuCardOpData;



/*
BCD��  �û����ֲ�ͬ������
10H ������
11H ������
12H ���û���������������
1EH ����ת�ƿ�
1FH ����1����
��Ϣ��鿨��20H
Ӧ�����ÿ���30H
���㿨��50H
*/
#define ESAM_CONSUME 0
#define CARD_CONSUME 1

typedef struct//esamӦ����Ϣ
{
    unsigned char IssueCode[6];             //������������
    unsigned char CompanyCode[2];           //��˾����
    unsigned char UserCode[8];              //�û���
    unsigned char UseType;                  //�û���������
    unsigned char ChargeLimit[4];           //��ֵ�޶�
    unsigned char WarningNum[2];            //��������
    unsigned char Overdraught[2];           //͸֧�޶�
    unsigned char MaxFlow;                  //����������
    unsigned char StatisticalDate;          //��������ͳ������
    unsigned char WarningClose[2];          //Ԥ�ط�����ֵ����
    unsigned char IdleClose;                //�������ط�����
    unsigned char LimitIdleCloseTimes;      //�������忨��������
    unsigned char Blank1;                   //Ԥ��
    unsigned char ThisPrice[4];             //��ǰ����
    unsigned char ThisStepModel[31];        //���ݼ�Ԥ��λ
    unsigned char NextPrice[4];             //��ʱ��Ч����
    unsigned char NextStepModel[16];        //��ʱ���ݼ�Ԥ��λ
    unsigned char PriceDelayTime[4];        //��ʱ����ʱ��
    unsigned char PriceDelayBlank[7];       //��ʱ���ݼ�Ԥ��λ
    unsigned char PulseX[4];                //����ϵ��
    unsigned char EareCode[4];              //��������
    unsigned char AppVer[4];                //����汾��
    unsigned char Blank2[33];               //����Ԥ��λ
    unsigned char CRC2;                     //0x2a--0x8e���ۼӺ��û�������У����
} Stu_EsamInfo, *pStu_EsamInfo;
typedef struct
{
    unsigned char ChargeAllInt[4];
    unsigned char ChargeAllDec[4];
    unsigned char ChargeRnd[6];
} Stu_ChargeFile, *pStu_ChargeFile;

typedef struct
{
    unsigned char MagnetTimes;
    unsigned char MaxFlowTimes;
    unsigned char OpenTimes;
    unsigned char blank1[3];
    unsigned char MeterReg;
    unsigned char IdleCloseTimes;
    unsigned char TotalUseInt[4];
    unsigned char TotalUseDec[4];
    unsigned char TotalUseMoney[8];
    unsigned char MonthData[48];
    unsigned char blank2[38];
    unsigned char CRC16[2];
} Stu_MeterOpFile, *pStu_MeterOpFile;

typedef union
{
    unsigned char buf[0xa0];
    //Stu_EsamInfo EsamInfo;
    struct//�û���Ӧ����Ϣ
    {
        union
        {
            unsigned char value;
            struct
            {
                unsigned char : 3;
                unsigned char UpdateMeterPrice: 1;  //���±��ڵ���
                unsigned char UpdateUseType: 1;     //����������������
                unsigned char : 1;
                unsigned char UpdateCMC: 1;         //�������ù�˾����
                unsigned char : 1;                  //�������ó���ȼ������
            };
        } OpCode1;                              //����������1
        union
        {
            unsigned char value;
            struct
            {
                unsigned char : 1;
                unsigned char UpdateIdleClose: 1;   //�������ò������ط�����
                unsigned char UpdateWarningClose: 1; //��������Ԥ�ط�ֵ
                unsigned char StatisticalDate: 1;   //����������������ͳ������
                unsigned char UpdateMaxFlow: 1;     //�������ô���������
                unsigned char UpdateOverdraught: 1; //��������͸֧�޶�
                unsigned char UpdateWarning: 1;     //�������ñ�������
                unsigned char UpdateChargeMax: 1;   //�������ó�ֵ�޶�
            };
        } OpCode2;                 //����������2
        unsigned char OpCode3;                  //����������3
        unsigned char OpCode4;                  //����������4
        unsigned char Blank1[6];                //Ԥ��
        unsigned char CompanyCode[2];           //��˾����
        unsigned char UserCode[8];              //�û���
        unsigned char UseType;                  //�û���������
        unsigned char ChargeLimit[4];           //��ֵ�޶�
        unsigned char WarningNum[2];            //��������
        unsigned char Overdraught[2];           //͸֧�޶�
        unsigned char MaxFlow;                  //����������
        unsigned char StatisticalDate;          //��������ͳ������
        unsigned char WarningClose[2];          //Ԥ�ط�����ֵ����
        unsigned char IdleClose;                //�������ط�����
        unsigned char LimitIdleCloseTimes;      //�������忨��������
        unsigned char LastChargeRnd[6];         //�ϴι��������
        unsigned char ThisChargeRnd[6];         //���ι��������
        unsigned char ThisPrice[4];             //������Ч����
        unsigned char ThisStepModel[27];        //���ݼ�Ԥ��λ
        unsigned char NextPrice[4];             //��ʱ��Ч����
        unsigned char NextStepModel[16];        //��ʱ���ݼ�Ԥ��λ
        unsigned char PriceDelayTime[4];        //��ʱ����ʱ��
        unsigned char PriceDelayBlank[7];       //��ʱ���ݼ�Ԥ��λ
        unsigned char Blank2[14];               //����Ԥ��λ
        unsigned char CheckChargeTimes[4];      //����1�������ô���
        unsigned char CRC[1];                   //�û�������У����
        unsigned char ChargeIndex[2];           //�������
    } UserCardInfo;
    struct
    {
        unsigned char IssueCode[6];             //ת���±��̱��
        unsigned char CompanyCode[2];           //ת���±�˾����
        unsigned char UserCode[8];              //ת���±��û���
        unsigned char TotalUseInt[4];
        unsigned char TotalUseDec[2];
        unsigned char TotalUseMoneyInt[4];
        unsigned char TotalUseMoneyDec[2];
        unsigned char RemainInt[4];
        unsigned char RemainDec[2];
        unsigned char AmountChargeInt[4];
        unsigned char AmountChargeDec[2];
        unsigned char MagnetTimes;
        unsigned char FlowTimes;
        unsigned char OpenDoorTimes;
        unsigned char Blank1;
        unsigned char MeterReg;
        unsigned char InsertTime[4];
        unsigned char EareCode[4];
        unsigned char Ver[4];
        unsigned char Blank2[28];
        unsigned char Crc1;
        unsigned char CardBackErrCode;
        unsigned char CardBackErrStr[8];
        unsigned char CardBackCrc;
    } UserCardWriteBack;
    struct//���ÿ�Ӧ����Ϣ
    {
        struct
        {
            unsigned char : 3;
            unsigned char UpdateMeterPrice: 1;  //���±��ڵ���
            unsigned char UpdateUseType: 1;   //����������������
            unsigned char : 1;
            unsigned char UpdateCMC: 1;         //�������ù�˾����
            unsigned char UpdatetIssueCode: 1;    //�������ó���ȼ������
        } OpCode1;                 //����������1
        struct
        {

            unsigned char UpdateMeterTime: 1;       //�������ñ���ʱ��
            unsigned char UpdateIdleClose: 1;       //�������ò������ط�����
            unsigned char UpdateWarningClose: 1;    //��������Ԥ�ط�ֵ
            unsigned char UpdateStatisticalDate: 1; //����������������ͳ������
            unsigned char UpdateMaxFlow: 1;         //�������ô���������
            unsigned char UpdateOverdraught: 1;     //��������͸֧�޶�
            unsigned char UpdateWarning: 1;         //�������ñ�������
            unsigned char UpdateChargeMax: 1;       //�������ó�ֵ�޶�
        } OpCode2;                 //����������2
        struct
        {

            unsigned char ReleaseDoor: 1;       //��������Ȩ��
            unsigned char : 1;                  //
            unsigned char CleanEsamInfo: 1;     //���ESAMģ��������Ϣ
            unsigned char CleanEsamMeterID: 1;  //���ESAM�ڱ��
            unsigned char : 4;                  //
        } OpCode3;                 //����������3
        unsigned char OpCode4;                  //����������4
        unsigned char IssueCode[6];             //������������
        unsigned char CompanyCode[2];           //��˾����
        unsigned char Blank1[8];              //�û���
        unsigned char UseType;                  //�û���������
        unsigned char ChargeLimit[4];           //��ֵ�޶�
        unsigned char WarningNum[2];            //��������
        unsigned char Overdraught[2];           //͸֧�޶�
        unsigned char MaxFlow;                  //����������
        unsigned char StatisticalDate;          //��������ͳ������
        unsigned char WarningClose[2];          //Ԥ�ط�����ֵ����
        unsigned char IdleClose;                //�������ط�����
        unsigned char LimitIdleCloseTimes;      //�������忨��������
        unsigned char MeterTime[6];             //����ʱ��
        unsigned char CRC1;                     //0x00--0x28���ۼӺ�
        unsigned char ThisPrice[4];             //������Ч����
        unsigned char ThisStepModel[27];        //���ݼ�Ԥ��λ
        unsigned char NextPrice[4];             //��ʱ��Ч����
        unsigned char NextStepModel[16];        //��ʱ���ݼ�Ԥ��λ
        unsigned char PriceDelayTime[4];        //��ʱ����ʱ��
        unsigned char PriceDelayBlank[7];       //��ʱ���ݼ�Ԥ��λ
        unsigned char PulseX[4];                //����ϵ��
        unsigned char Blank2[35];               //����Ԥ��λ
        unsigned char CRC2;                     //0x2a--0x8e���ۼӺ��û�������У����
    } SetupCardInfo;
    struct //������Ϣ�ļ�
    {
        unsigned char InssuCode[8];
        unsigned char CardType;
        unsigned char Ver;
        unsigned char SerialNum[8];
        unsigned char EffectiveDate[4];
        unsigned char InvaildDate[4];
        unsigned char EareCode[4];
        unsigned char UserName[50];
    } PublicInfo;
    struct //�������Ϣ�ļ�
    {
        union
        {
            unsigned char Value;
            struct
            {
                unsigned char : 1;
                unsigned char MonthData: 1;         //�����ۻ�������������
                unsigned char AmountCharge: 1;      //���ۻ�������
                unsigned char EsamBalance: 1;       //��ESAM��Ǯ��
                unsigned char TotalUse_Money: 1;    //���ۻ�������Ԥ����
                unsigned char TotalUse_Gas: 1;      //���ۻ���������
                unsigned char UserID: 1;            //���û���
                unsigned char ClearMeterRnd: 1;     //������ڹ��������
            };
        } OpCode1;                 //����������1
        union
        {
            unsigned char Value;
            struct
            {
                unsigned char All: 1;               //�ָ��������ã�Ԥ����
                unsigned char : 4;
                unsigned char OpenTimes: 1;         //�忪������
                unsigned char MaxFlowTimes: 1;      //�����������
                unsigned char MagnetTimes: 1;       //���Ÿ�����
            };
        } OpCode2;                 //����������2
        unsigned char OpCode3;
        unsigned char CRC;
    } ClearCardInfo;
    struct
    {
        unsigned char State;                    //״̬��־
        unsigned char IssueCode[6];             //ת���±��̱��
        unsigned char CompanyCode[2];           //ת���±�˾����
        unsigned char UserCode[8];              //ת���±��û���
        unsigned char RemainInt[4];             //ת��������
        unsigned char RemainDec[2];             //ת����С��(4λ)
        unsigned char TranOutTime[5];           //ת���ɱ�ʱ��
        unsigned char TranInTime[5];            //ת���±�ʱ��
        unsigned char UseType;                  //�û���������
    } TransCardInfo;
} CpuFile_Stu, *pCpuFile_Stu;

unsigned char ConsumeNum(unsigned char tSource, unsigned long tNum);

unsigned char GetCardType(void);
unsigned char Card_ConsumeNum(unsigned long tNum);
//unsigned char Esam_ConsumeNum(unsigned long tNum);
#define Esam_ConsumeNum(a)  ConsumeNum(ESAM_CONSUME,a)
unsigned char Esam_ChargeNum(unsigned long tNum);

#define EsamReadAll(c) Esam_ReadBinary(0x0a,0x00,c,0x90)
//unsigned char EsamReadAll(unsigned char *buf);
#define WriteEsamOpInfo(x,y,z) Esam_WriteBinary(0x0e,x,y,z)
//unsigned char WriteEsamOpInfo(unsigned char addr,char *tbuf,unsigned char tlen);
#define ReadEsamOpInfo(x,y,z)   Esam_ReadBinary(0x0e,x,y,z)
//unsigned char ReadEsamOpInfo(unsigned char addr,char *tbuf,unsigned char tlen);
#define WriteEsamChargeInfo(x,y,z)      Esam_WriteBinary(0x0b,x,y,z)
//unsigned char WriteEsamChargeInfo(unsigned char addr,char *tbuf,unsigned char tlen);
#define ReadEsamChargeInfo(x,y,z)   Esam_ReadBinary(0x0b,x,y,z)
//unsigned char ReadEsamChargeInfo(unsigned char addr,char *tbuf,unsigned char tlen);

unsigned char DealUserCard(unsigned char cardtype);
unsigned char DealSetupCard(void);
unsigned char DealTransCard(void);
unsigned char DealInfoCheckCard(void);
unsigned char DealClearCard(void);
void UserCardWriteBack(unsigned char ChargeRe);
unsigned char Esam_ChargeNumInit(unsigned long tNum, char *tReData);
unsigned char Esam_ChargeNumCmpt(char *tDataBuf1);

unsigned char UserReadCard(void);



#endif
