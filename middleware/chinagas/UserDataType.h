#ifndef __USERDATATYPE_H__
#define __USERDATATYPE_H__

//定义自定义参数
typedef union
{
    unsigned char value;
    struct
    {
        unsigned char ErrClock      : 1;
        unsigned char HadUser       : 1;
        unsigned char Magnet        : 1;
        unsigned char RemainFew     : 1;

        unsigned char LowPow        : 1;
        unsigned char DoorState     : 1;
        unsigned char DoorAuthority : 1;
        unsigned char CutMark       : 1;
    };
} Stu_MeterReg;

typedef struct
{
    Stu_MeterReg MeterReg;
    unsigned char MagnetTimes;
    unsigned char MaxFlowTimes;
    unsigned char OpenTimes;
    unsigned char IdleCloseTimes;
    //unsigned char LimitIdleCloseTimes;
    unsigned long TotalUseInt;
    unsigned long TotalUseDec;

    unsigned long JustWarning;
    unsigned char StatisticalDate;
    unsigned long MonthAmountUse[12];//1---12有效


} StuUser, *pStuUser;

typedef struct
{
    unsigned char Year;
    unsigned char Month;
    unsigned char Day;
    unsigned char WeekDay;
    unsigned char Hours;
    unsigned char Minutes;
    unsigned char Seconds;
    //unsigned char BLANK;//补位
} StuDate, *pStuDate;

#endif

