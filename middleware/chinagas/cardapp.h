#ifndef __CARDAPP_H__
#define __CARDAPP_H__

#include <stdint.h>

typedef struct
{
    uint32_t TempCharge;
}stuCardOpData;



/*
BCD码  用户区分不同卡类型
10H 开户卡
11H 购气卡
12H 旧用户卡（卡内无气）
1EH 回收转移卡
1FH 生产1方卡
信息检查卡：20H
应用设置卡：30H
清零卡：50H
*/
#define ESAM_CONSUME 0
#define CARD_CONSUME 1

typedef struct//esam应用信息
{
    unsigned char IssueCode[6];             //厂商生产代码
    unsigned char CompanyCode[2];           //公司代码
    unsigned char UserCode[8];              //用户号
    unsigned char UseType;                  //用户用气类型
    unsigned char ChargeLimit[4];           //充值限额
    unsigned char WarningNum[2];            //报警气量
    unsigned char Overdraught[2];           //透支限额
    unsigned char MaxFlow;                  //大流量门限
    unsigned char StatisticalDate;          //月用气量统计日期
    unsigned char WarningClose[2];          //预关阀气量值设置
    unsigned char IdleClose;                //不计量关阀天数
    unsigned char LimitIdleCloseTimes;      //不计量插卡开阀次数
    unsigned char Blank1;                   //预留
    unsigned char ThisPrice[4];             //当前单价
    unsigned char ThisStepModel[31];        //阶梯价预留位
    unsigned char NextPrice[4];             //延时生效单价
    unsigned char NextStepModel[16];        //延时阶梯价预留位
    unsigned char PriceDelayTime[4];        //延时调价时间
    unsigned char PriceDelayBlank[7];       //延时阶梯价预留位
    unsigned char PulseX[4];                //脉冲系数
    unsigned char EareCode[4];              //地区代码
    unsigned char AppVer[4];                //程序版本号
    unsigned char Blank2[33];               //功能预留位
    unsigned char CRC2;                     //0x2a--0x8e的累加和用户卡数据校验码
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
    struct//用户卡应用信息
    {
        union
        {
            unsigned char value;
            struct
            {
                unsigned char : 3;
                unsigned char UpdateMeterPrice: 1;  //更新表内单价
                unsigned char UpdateUseType: 1;     //更新设置用气类型
                unsigned char : 1;
                unsigned char UpdateCMC: 1;         //更新设置公司代码
                unsigned char : 1;                  //更新设置厂商燃气表编号
            };
        } OpCode1;                              //操作命令码1
        union
        {
            unsigned char value;
            struct
            {
                unsigned char : 1;
                unsigned char UpdateIdleClose: 1;   //更新设置不计量关阀参数
                unsigned char UpdateWarningClose: 1; //更新设置预关阀值
                unsigned char StatisticalDate: 1;   //更新设置月用气量统计日期
                unsigned char UpdateMaxFlow: 1;     //更新设置大流量门限
                unsigned char UpdateOverdraught: 1; //更新设置透支限额
                unsigned char UpdateWarning: 1;     //更新设置报警气量
                unsigned char UpdateChargeMax: 1;   //更新设置充值限额
            };
        } OpCode2;                 //操作命令码2
        unsigned char OpCode3;                  //操作命令码3
        unsigned char OpCode4;                  //操作命令码4
        unsigned char Blank1[6];                //预留
        unsigned char CompanyCode[2];           //公司代码
        unsigned char UserCode[8];              //用户号
        unsigned char UseType;                  //用户用气类型
        unsigned char ChargeLimit[4];           //充值限额
        unsigned char WarningNum[2];            //报警气量
        unsigned char Overdraught[2];           //透支限额
        unsigned char MaxFlow;                  //大流量门限
        unsigned char StatisticalDate;          //月用气量统计日期
        unsigned char WarningClose[2];          //预关阀气量值设置
        unsigned char IdleClose;                //不计量关阀天数
        unsigned char LimitIdleCloseTimes;      //不计量插卡开阀次数
        unsigned char LastChargeRnd[6];         //上次购气随机码
        unsigned char ThisChargeRnd[6];         //本次购气随机码
        unsigned char ThisPrice[4];             //立即生效单价
        unsigned char ThisStepModel[27];        //阶梯价预留位
        unsigned char NextPrice[4];             //延时生效单价
        unsigned char NextStepModel[16];        //延时阶梯价预留位
        unsigned char PriceDelayTime[4];        //延时调价时间
        unsigned char PriceDelayBlank[7];       //延时阶梯价预留位
        unsigned char Blank2[14];               //功能预留位
        unsigned char CheckChargeTimes[4];      //生产1方卡可用次数
        unsigned char CRC[1];                   //用户卡数据校验码
        unsigned char ChargeIndex[2];           //交易序号
    } UserCardInfo;
    struct
    {
        unsigned char IssueCode[6];             //转入新表厂商编号
        unsigned char CompanyCode[2];           //转入新表公司代码
        unsigned char UserCode[8];              //转入新表用户号
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
    struct//设置卡应用信息
    {
        struct
        {
            unsigned char : 3;
            unsigned char UpdateMeterPrice: 1;  //更新表内单价
            unsigned char UpdateUseType: 1;   //更新设置用气类型
            unsigned char : 1;
            unsigned char UpdateCMC: 1;         //更新设置公司代码
            unsigned char UpdatetIssueCode: 1;    //更新设置厂商燃气表编号
        } OpCode1;                 //操作命令码1
        struct
        {

            unsigned char UpdateMeterTime: 1;       //更新设置表内时间
            unsigned char UpdateIdleClose: 1;       //更新设置不计量关阀参数
            unsigned char UpdateWarningClose: 1;    //更新设置预关阀值
            unsigned char UpdateStatisticalDate: 1; //更新设置月用气量统计日期
            unsigned char UpdateMaxFlow: 1;         //更新设置大流量门限
            unsigned char UpdateOverdraught: 1;     //更新设置透支限额
            unsigned char UpdateWarning: 1;         //更新设置报警气量
            unsigned char UpdateChargeMax: 1;       //更新设置充值限额
        } OpCode2;                 //操作命令码2
        struct
        {

            unsigned char ReleaseDoor: 1;       //开启阀门权限
            unsigned char : 1;                  //
            unsigned char CleanEsamInfo: 1;     //清除ESAM模块运行信息
            unsigned char CleanEsamMeterID: 1;  //清除ESAM内表号
            unsigned char : 4;                  //
        } OpCode3;                 //操作命令码3
        unsigned char OpCode4;                  //操作命令码4
        unsigned char IssueCode[6];             //厂商生产代码
        unsigned char CompanyCode[2];           //公司代码
        unsigned char Blank1[8];              //用户号
        unsigned char UseType;                  //用户用气类型
        unsigned char ChargeLimit[4];           //充值限额
        unsigned char WarningNum[2];            //报警气量
        unsigned char Overdraught[2];           //透支限额
        unsigned char MaxFlow;                  //大流量门限
        unsigned char StatisticalDate;          //月用气量统计日期
        unsigned char WarningClose[2];          //预关阀气量值设置
        unsigned char IdleClose;                //不计量关阀天数
        unsigned char LimitIdleCloseTimes;      //不计量插卡开阀次数
        unsigned char MeterTime[6];             //设置时间
        unsigned char CRC1;                     //0x00--0x28的累加和
        unsigned char ThisPrice[4];             //立即生效单价
        unsigned char ThisStepModel[27];        //阶梯价预留位
        unsigned char NextPrice[4];             //延时生效单价
        unsigned char NextStepModel[16];        //延时阶梯价预留位
        unsigned char PriceDelayTime[4];        //延时调价时间
        unsigned char PriceDelayBlank[7];       //延时阶梯价预留位
        unsigned char PulseX[4];                //脉冲系数
        unsigned char Blank2[35];               //功能预留位
        unsigned char CRC2;                     //0x2a--0x8e的累加和用户卡数据校验码
    } SetupCardInfo;
    struct //公共信息文件
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
    struct //清除卡信息文件
    {
        union
        {
            unsigned char Value;
            struct
            {
                unsigned char : 1;
                unsigned char MonthData: 1;         //清月累积用气量抄表数
                unsigned char AmountCharge: 1;      //清累积购气量
                unsigned char EsamBalance: 1;       //清ESAM内钱包
                unsigned char TotalUse_Money: 1;    //清累积用气金额（预留）
                unsigned char TotalUse_Gas: 1;      //清累积用气气量
                unsigned char UserID: 1;            //清用户号
                unsigned char ClearMeterRnd: 1;     //清零表内购气随机码
            };
        } OpCode1;                 //操作命令码1
        union
        {
            unsigned char Value;
            struct
            {
                unsigned char All: 1;               //恢复出厂设置（预留）
                unsigned char : 4;
                unsigned char OpenTimes: 1;         //清开阀测试
                unsigned char MaxFlowTimes: 1;      //清大流量测试
                unsigned char MagnetTimes: 1;       //清电磁干扰数
            };
        } OpCode2;                 //操作命令码2
        unsigned char OpCode3;
        unsigned char CRC;
    } ClearCardInfo;
    struct
    {
        unsigned char State;                    //状态标志
        unsigned char IssueCode[6];             //转入新表厂商编号
        unsigned char CompanyCode[2];           //转入新表公司代码
        unsigned char UserCode[8];              //转入新表用户号
        unsigned char RemainInt[4];             //转移量整数
        unsigned char RemainDec[2];             //转移量小数(4位)
        unsigned char TranOutTime[5];           //转出旧表时间
        unsigned char TranInTime[5];            //转入新表时间
        unsigned char UseType;                  //用户用气类型
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
