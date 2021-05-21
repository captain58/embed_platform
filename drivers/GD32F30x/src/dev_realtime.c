/******************************Copyright(c)******************************
**                         
**
** File Name: dev_realtime.c
** Author: ʵʱʱ���豸
** Date Last Update: 2019-11-02
** Description: 
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#define EXT_DEV_REALTIME
#ifdef __MODULE__
#include "WOSsys.h"
#endif
#include "sys.h"
#include "hal.h"
#include "bsp.h"
//	#include "string.h"
#include "apis.h"

/******************************************************************************
**��¼��ǰʱ��ı���
******************************************************************************/
TIME gs_Time;                               //��ǰʱ��
TIME gs_TimeBk;                             //����ʱ��



/******************************************************************************
**ʱ�Ӳ�����Դ
******************************************************************************/
TESRes gs_RealTimeRes;



/******************************************************************************
**�Ƿ�������ⲿӲʱ��Уʱ
******************************************************************************/
bool f_timecheck;							//�Ƿ�������ⲿӲʱ��Уʱ



#if (SYS_RTC_EN == 0)

TIME gs_vtime;                  //RTC������ʱ�������
    

/************************************************************************
 * @Function: _virtualTimeInit
 * @Description: ��ʼ������RTCʱ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void _virtualTimeInit(void)
{   //2007��1��1������1
    TIME* time = &gs_vtime;
    time->sec = 0;
    time->min = 0;
    time->hour = 0;
    time->day = 1;
    time->month = 1;
    time->year = 7;
    time->dyear = 0;
    time->week = 1;
}



/************************************************************************
 * @Function: VirtualTimeSecProc
 * @Description: ����RTC��ʱ����
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void _virtualTimeSecProc(void)
{
    TIME* time = &gs_vtime;
    const uint8* monthday;
    
    if((time->year & 0x03) == 0)        //�жϽ����Ƿ�Ϊ����
    {
        monthday = guc_BYearMonth;
    }
    else
    {
        monthday = guc_LYearMonth;
    }
    SYS_ENTER_SCRT();
                                            //�����
    time->sec++;
    if(time->sec >= 60)
    {
        time->sec = 0;
        time->min++;
    }
                                            //�ָ���
    if(time->min >= 60)
    {
        time->min = 0;
        time->hour++;
    }
                                            //Сʱ����
    if(time->hour >= 24)
    {
        time->hour = 0;
        time->day++;
        time->week++;
        time->dyear++;
    }
                                            //�ո���
    if((time->day) == 0 || (time->day > monthday[time->month - 1]))
    {
        time->day = 1;
        time->month++;
    }
                                            //�¸���,dyear����
    if((time->month == 0) || (time->month > 12))
    {
        time->month = 1;
        time->dyear = 0;
        time->year++;
    }
                                            //�����
    if(time->year > 99)
    {
        time->year = 0;
    }
    
    if(time->week > 6)                    //dweek����
    {
        time->week = 0;
    }
    SYS_EXIT_SCRT();
}



/************************************************************************
 * @Function: _getVirtualTime
 * @Description: ��ȡ����RTCʱ��
 * 
 * @Arguments: 
 * @param: datetime 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void _getVirtualTime(TIME* datetime)
{
    TIME* time = &gs_vtime;
    datetime->sec = time->sec;
    datetime->min = time->min;
    datetime->hour = time->hour;
    datetime->day = time->day;
    datetime->month = time->month;
    datetime->year = time->year;
    datetime->dweek = time->dweek;
    datetime->dyear = time->dweek;
}



/************************************************************************
 * @Function: _setVirtualTime
 * @Description: ��������RTCʱ��
 * 
 * @Arguments: 
 * @param: datetime 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void _setVirtualTime(TIME* datetime)
{
    //SYS_ENTER_AllSCRT();           //1)����ʱ�Ӳ��漰�жϴ���,�ڽ����м�ʱ
                                    //2)���̽ӿں��������� gs_RealTimeRes ��Ϊ��д����.
                                    //3)�ú����� _SystemTaskInitHook()�е���,��������ڿ����ж�
    TIME* time = &gs_vtime;
    time->sec = datetime->sec;
    time->min = datetime->min;
    time->hour = datetime->hour;
    time->day = datetime->day;
    time->month = datetime->month;
    time->year = datetime->year;
    time->dweek = CalcWeek((uint8*)&datetime->day);
    time->dyear = datetime->dweek;
    
    //SYS_EXIT_AllSCRT();
}


#endif

    
    

#if (RTC_HEX > 0)

/************************************************************************
 * @Function: SYS_MCU_ReadDateTime
 * @Description: ��ȡ�ڲ�ʱ��,��ʽhex
 * 
 * @Arguments: 
 * @param: datetime ʱ��ṹ�建��
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_ReadDateTime(TIME* datetime)
{
#if (SYS_RTC_EN == 0)
    _getVirtualTime(datetime);
    return SYS_ERR_OK;
    
#else
    HAL_RTC_GetTimes(datetime);
    return SYS_ERR_OK;
#endif
}


/************************************************************************
 * @Function: SYS_MCU_ReadBCDDateTime
 * @Description: ��ȡ�ڲ�ʱ��,��ʽBCD
 * 
 * @Arguments: 
 * @param: datetime 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_ReadBCDDateTime(TIME* datetime)
{ 
    if(SYS_MCU_ReadDateTime(datetime) == SYS_ERR_OK)
    {
        datetime->sec = ByteHexToBcd(datetime->sec);
        datetime->min = ByteHexToBcd(datetime->min);
        datetime->hour = ByteHexToBcd(datetime->hour);
        datetime->day = ByteHexToBcd(datetime->day);
        datetime->month = ByteHexToBcd(datetime->month);
        datetime->year = ByteHexToBcd(datetime->year);
        datetime->dweek = datetime->dweek;
        datetime->dyear = gp_calclib->IntHexToBcd(datetime->dyear);
        return SYS_ERR_OK;
    }
    
    return SYS_ERR_FT;
}


/************************************************************************
 * @Function: SYS_MCU_WriteDateTime
 * @Description: �����ڲ�ʱ��,��ʽhex
 * 
 * @Arguments: 
 * @param: datetime ʱ��ṹ��,���е���,��,ʱ,��,��,�������ȷ
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_WriteDateTime(TIME* datetime)
{
    const uint8* monthday;
                                            //�ж��������ݵĺϷ���
    if(datetime->year > 99 || datetime->month == 0 || 
       datetime->month > 12 || datetime->day == 0 || 
       datetime->hour >= 24 || datetime->min >= 60 || 
       datetime->sec >= 60)
    {
        return SYS_ERR_FT;
    }
    
    if((datetime->year & 0x03) == 0)        //�жϽ����Ƿ�Ϊ����
    {
        monthday = guc_BYearMonth;
    }
    else
    {
        monthday = guc_LYearMonth;  
    }
                                            //�յĺϷ���
    if(datetime->day > monthday[datetime->month - 1])
    {
        return SYS_ERR_FT;
    }
                                            //��ǰΪһ���еĵڼ���
    datetime->dyear = gui_MonthBeginDay[datetime->month - 1] + datetime->day - 1;
                                            //�жϽ����Ƿ�Ϊ����,�Ƿ��ѹ�2��
    if((datetime->year & 0x03) == 0 && datetime->month > 2)
    {
        datetime->dyear++;
    }
    datetime->dweek = CalcWeek((uint8*)&datetime->day);
    
#if (SYS_RTC_EN == 0)
    _setVirtualTime(datetime);
    return SYS_ERR_OK;
    
#else
    HAL_RTC_SetTimes(datetime);
    return SYS_ERR_OK;
#endif
}


/************************************************************************
 * @Function: SYS_MCU_WriteBCDDateTime
 * @Description: �����ڲ�ʱ��(��ʽbcd)
 * 
 * @Arguments: 
 * @param: datetime ʱ��ṹ�����е���,��,ʱ,��,��,�������ȷ
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_WriteBCDDateTime(TIME* datetime)
{
    datetime->sec = ByteBcdToHex(datetime->sec);
    datetime->min = ByteBcdToHex(datetime->min);
    datetime->hour = ByteBcdToHex(datetime->hour);
    datetime->day = ByteBcdToHex(datetime->day);
    datetime->month = ByteBcdToHex(datetime->month);
    datetime->year = ByteBcdToHex(datetime->year);
    datetime->dweek = datetime->dweek;
    datetime->dyear = gp_calclib->IntBcdToHex(datetime->dyear);
    return SYS_MCU_WriteDateTime(datetime);
}


#else

/************************************************************************
 * @Function: SYS_MCU_ReadDateTime
 * @Description: ��ȡ�ڲ�ʱ��,��ʽhex
 * 
 * @Arguments: 
 * @param: datetime ʱ��ṹ�建��
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_ReadBCDDateTime(TIME* datetime)

{
#if (SYS_RTC_EN == 0)
    _getVirtualTime(datetime);
    return SYS_ERR_OK;
    
#else
    HAL_RTC_GetTimes(datetime);
    return SYS_ERR_OK;
#endif
}


/************************************************************************
 * @Function: SYS_MCU_ReadBCDDateTime
 * @Description: ��ȡ�ڲ�ʱ��,��ʽBCD
 * 
 * @Arguments: 
 * @param: datetime 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_ReadDateTime(TIME* datetime)

{ 
    if(SYS_MCU_ReadBCDDateTime(datetime) == SYS_ERR_OK)
    {
        datetime->sec = ByteBcdToHex(datetime->sec);
        datetime->min = ByteBcdToHex(datetime->min);
        datetime->hour = ByteBcdToHex(datetime->hour);
        datetime->day = ByteBcdToHex(datetime->day);
        datetime->month = ByteBcdToHex(datetime->month);
        datetime->year = ByteBcdToHex(datetime->year);
        datetime->week = datetime->week;
        datetime->dyear = ByteBcdToHex(datetime->dyear);
        return SYS_ERR_OK;
    }
    
    return SYS_ERR_FT;
}

/************************************************************************
 * @Function: SYS_MCU_WriteBCDDateTime
 * @Description: �����ڲ�ʱ��(��ʽbcd)
 * 
 * @Arguments: 
 * @param: datetime ʱ��ṹ�����е���,��,ʱ,��,��,�������ȷ
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_WriteBCDDateTime(TIME* datetime)
{
    datetime->sec = ByteHexToBcd(datetime->sec);
    datetime->min = ByteHexToBcd(datetime->min);
    datetime->hour = ByteHexToBcd(datetime->hour);
    datetime->day = ByteHexToBcd(datetime->day);
    datetime->month = ByteHexToBcd(datetime->month);
    datetime->year = ByteHexToBcd(datetime->year);
    datetime->week = datetime->week;
    datetime->dyear = ByteHexToBcd(datetime->dyear);
    return HAL_RTC_SetTimes(datetime);
}

/************************************************************************
 * @Function: SYS_MCU_WriteDateTime
 * @Description: �����ڲ�ʱ��,��ʽhex
 * 
 * @Arguments: 
 * @param: datetime ʱ��ṹ��,���е���,��,ʱ,��,��,�������ȷ
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_MCU_WriteDateTime(TIME* datetime)
{
    const uint8* monthday;
                                            //�ж��������ݵĺϷ���
    if(datetime->year > 99 || datetime->month == 0 || 
       datetime->month > 12 || datetime->day == 0 || 
       datetime->hour >= 24 || datetime->min >= 60 || 
       datetime->sec >= 60)
    {
        return SYS_ERR_FT;
    }
    
    if((datetime->year & 0x03) == 0)        //�жϽ����Ƿ�Ϊ����
    {
        monthday = guc_BYearMonth;
    }
    else
    {
        monthday = guc_LYearMonth;  
    }
                                            //�յĺϷ���
    if(datetime->day > monthday[datetime->month - 1])
    {
        return SYS_ERR_FT;
    }
                                            //��ǰΪһ���еĵڼ���
    datetime->dyear = gui_MonthBeginDay[datetime->month - 1] + datetime->day - 1;
                                            //�жϽ����Ƿ�Ϊ����,�Ƿ��ѹ�2��
    if((datetime->year & 0x03) == 0 && datetime->month > 2)
    {
        datetime->dyear++;
    }
    datetime->week = CalcWeek((uint8*)&datetime->day);
    
#if (SYS_RTC_EN == 0)
    _setVirtualTime(datetime);
    return SYS_ERR_OK;
    
#else
    SYS_MCU_WriteBCDDateTime(datetime);
    return SYS_ERR_OK;
#endif
}



#endif


/************************************************************************
 * @Function: RTC_ProcTEsMsg
 * @Description: ʵʱʱ�Ӵ������������Ϣ������
 * 
 * @Arguments: 
 * @param: OpType ��������
 * @param: obj 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 RTC_ProcTEsMsg(uint8 OpType, TIME* obj)
{
#if SYS_HRTC_EN > 0
    uint8 time[10];                         //ʱ�ӻ���
#endif
    uint8 result = SYS_ERR_FT;
    
	switch(OpType)                         //������Ϣ�������ͷǱ���
    {

        case HRTC_RW_RB:                    //��ȡBCD��ʽʱ��
            result = SYS_MCU_ReadBCDDateTime(obj);
            break;
            
        case HRTC_RW_RH:                    //��ȡHEX��ʽʱ��
            result = SYS_MCU_ReadDateTime(obj);
            break;
            
        case HRTC_RW_WB:                    //д��BCD��ʽʱ��
#if SYS_HRTC_EN > 0
            memcpy_s(time, (uint8*)obj, 3);//���ʱ
            memcpy_s(time + 4,(uint8*)obj + 3,  3);//������
            time[3] = CalcWeek(time + 4);          //��
            result = I2C_HRTC_WriteBcdTime(time);
#endif
            result = SYS_MCU_WriteBCDDateTime(obj);      //ͬʱ����RTCʱ��,�ֽ�˳��һ��TIME���ͺ͸�time[]
            break;
            
        case HRTC_RW_WH:                    //д��HEX��ʽʱ��
#if SYS_HRTC_EN > 0
            memcpy_s(time, (uint8*)obj, 3);//���ʱ
            memcpy_s(time + 4, (uint8*)obj + 3, 3);//������
            time[3] = CalcWeek(time + 4);          //��
            result = I2C_HRTC_WriteHexTime(time);
#endif
            result = SYS_MCU_WriteDateTime((TIME*)obj); 
            break;
            
        default:
            break;
    
    }
    
    return result;

}



/************************************************************************
 * @Function: SYS_WriteDateTime
 * @Description: ����ʱ��,��ʽHEX
 * 
 * @Arguments: 
 * @param: datetime 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_WriteDateTime(TIME* datetime)
{
    uint8 result;
#ifndef __NO_SYS__     
    SYS_SEM_Wait(&gs_RealTimeRes, 0);
    result = RTC_ProcTEsMsg(HRTC_RW_WH, datetime);
    SYS_SEM_Release(&gs_RealTimeRes);
#else
    result = RTC_ProcTEsMsg(HRTC_RW_WH, datetime);
#endif
    return result;
}


/************************************************************************
 * @Function: SYS_WriteBCDDateTime
 * @Description: ����ʱ��,��ʽbcd
 * 
 * @Arguments: 
 * @param: datetime 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_WriteBCDDateTime(TIME* datetime)
{
    uint8 result;
#ifndef __NO_SYS__     
    SYS_SEM_Wait(&gs_RealTimeRes, 0);
    result = RTC_ProcTEsMsg(HRTC_RW_WB, datetime);
    SYS_SEM_Release(&gs_RealTimeRes);
#else
    result = RTC_ProcTEsMsg(HRTC_RW_WB, datetime);
#endif    
    return result; 
}


/************************************************************************
 * @Function: SYS_ReadDateTime
 * @Description: ��ȡʱ��,��ʽHEX
 * 
 * @Arguments: 
 * @param: datetime 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_ReadDateTime(TIME* datetime)
{
    uint8 result;
#ifndef __NO_SYS__ 
    SYS_SEM_Wait(&gs_RealTimeRes, 0);
    result = RTC_ProcTEsMsg(HRTC_RW_RH, datetime);
    SYS_SEM_Release(&gs_RealTimeRes);
#else
    result = RTC_ProcTEsMsg(HRTC_RW_RH, datetime);
#endif
    return result; 
}

/************************************************************************
 * @Function: SYS_ReadBCDDateTime
 * @Description: ��ȡʱ��,��ʽbcd
 * 
 * @Arguments: 
 * @param: datetime 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_ReadBCDDateTime(TIME* datetime)
{
    uint8 result;
#ifndef __NO_SYS__ 
    SYS_SEM_Wait(&gs_RealTimeRes, 0);
    result = RTC_ProcTEsMsg(HRTC_RW_RB, datetime);
    SYS_SEM_Release(&gs_RealTimeRes);
#else
    result = RTC_ProcTEsMsg(HRTC_RW_RB, datetime);
#endif    
    return result; 
}





/************************************************************************
 * @Function: GetTime
 * @Description: ��ȡ��ǰ��ʱ������ڵ�ȫ����Ϣ, �����Ϣֻ��
 * @Arguments: 
 * 
 * @Note: 
 * @Return: const TIME*  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
const TIME* GetTime(void)
{
    return (const TIME*)&gs_Time;
}



/************************************************************************
 * @Function: GetSysRunSecs
 * @Description: ��ȡϵͳ��������ʱ��
 * @Arguments: 
 * 
 * @Note: 
 * @Return: uint32  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint32 GetSysRunSecs(void)
{
    return gul_SysRunSecs;
}


    
#if SYS_HRTC_EN > 0

/************************************************************************
 * @Function: SYS_RTC_CheckTime
 * @Description: ʵʱʱ��Уʱ
 * @Arguments: 
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_RTC_CheckTime(void)
{
    uint8 buff[7];                              //ssmmhhwwddmmyy
    uint8 time[6];
    uint32 sec,sec1;
    TIME tm;
    
    if(I2C_HRTC_ReadHexTime(buff) == SYS_ERR_OK)  //��ȡӲʱ��ʱ��
    {
        memcpy_s(time, buff, 3);               //���ʱ
        memcpy_s(time + 3, buff + 4, 3);       //������(������)
        sec = CalcSecs(time, false);
        
        I2C_HRTC_ReadHexTime(buff);
        memcpy_s(time, buff, 3);               //���ʱ
        memcpy_s(time + 3, buff + 4, 3);       //������(������)
        sec1 = CalcSecs(time, false);
        if(AbsVal(sec1 - sec) <= 2)		        //���ζ�ȡӲ��RTC��ʱ�����2�����ڲ�д��MCU��RTC
        {                                       //ת�����ݸ�ʽ
            tm.sec = buff[0];
            tm.min = buff[1];
            tm.hour = buff[2];
            tm.day = buff[4];
            tm.month = buff[5];
            tm.year = buff[6];
            return SYS_MCU_WriteDateTime(&tm);
        }
    }
    return SYS_ERR_FT;
}
#endif



#ifndef __NO_SYS__ 
/************************************************************************
 * @Function: SYS_RTC_SecProc
 * @Description: ʵʱʱ���봦��
 * 
 * @Arguments: 
 * @param: pdata 
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool SYS_RTC_SecProc(void* pdata)
{
#if (SYS_RTC_EN == 0)
    _virtualTimeSecProc();
#endif
                                            //��ȡ�ڲ�RTCʱ��
    if(SYS_MCU_ReadDateTime(&gs_Time) != SYS_ERR_OK)
    {
        SYS_MCU_ReadDateTime(&gs_Time);
    }
                                            //ϵͳ����ʱ����һ
    gul_SysRunSecs++;
    
#if (true)                                  //�¼��ַ�
    extern const TESTcbDeclare __TKDeclare[SYS_TK_NUM];
    const TESTcbDeclare* dec = __TKDeclare + 1;

    for(uint32 uc_i = 1; uc_i < (SYS_TK_NUM - 1); uc_i++, dec++)
    {
                                            //����ý��̲������򲻷���
        if(dec->tcbc == __NULL || dec->stklen == 0)
        {
            continue;
        }
        
        if(!(gucs_MsgApplied[uc_i] & Bit_Map8[MSG_CLS_TM]))
        {
            continue;
        }
        
        if(dec->tcbc->tbl != 0)             //ʱ����Ϣ�������Ϣ����
        {
            if(gs_TimeBk.year != gs_Time.year) //���¼�
            {
                SYS_Message_Send(MSG_YEAR, uc_i);
            }
            if(gs_TimeBk.month != gs_Time.month)//���¼�
            {
                SYS_Message_Send(MSG_MON, uc_i);
            }
            if(gs_TimeBk.day != gs_Time.day) //���¼�
            {
                SYS_Message_Send(MSG_DAY, uc_i);
            }
            if(gs_TimeBk.hour != gs_Time.hour) //Сʱ�¼�
            {
                SYS_Message_Send(MSG_HOUR, uc_i);
            }
            if(gs_TimeBk.min != gs_Time.min)   //���¼�
            {
				f_timecheck = true;			//���ֺ��30������RTCУʱ
                SYS_Message_Send(MSG_MIN, uc_i);
            }
            SYS_Message_Send(MSG_SEC, uc_i); //��������Ϣ
        }
    }
#endif

#if SYS_HRTC_EN > 0
    if((gs_Time.sec >= 30) && (gs_Time.sec <= 50) && f_timecheck)          	//�ڵ�30��Уʱ.ÿ����Уʱһ��.
    {
        SYS_RTC_CheckTime();
        f_timecheck = false;
    }
#endif
                                            //����ʱ��
    memcpy_s((uint8*)&gs_TimeBk, (uint8*)&gs_Time, sizeof(TIME));

    return true;
}
#else
#include "define.h"
/************************************************************************
 * @Function: SYS_RTC_SecProc
 * @Description: ʵʱʱ���봦��
 * 
 * @Arguments: 
 * @param: pdata 
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_RTC_SecProc(void)
{
#if (SYS_RTC_EN == 0)
    _virtualTimeSecProc();
#endif
                                            //��ȡ�ڲ�RTCʱ��
    if(SYS_MCU_ReadDateTime(&gs_Time) != SYS_ERR_OK)
    {
        SYS_MCU_ReadDateTime(&gs_Time);
    }
                                            //ϵͳ����ʱ����һ
    gul_SysRunSecs++;
    
#if (true)                                  //�¼��ַ�
    //extern const TESTcbDeclare __TKDeclare[SYS_TK_NUM];
    //const TESTcbDeclare* dec = __TKDeclare + 1;

    for(uint32 uc_i = 0; uc_i < (SYS_TK_NUM); uc_i++)
    {

        
//        if(!(gucs_MsgApplied[uc_i] & Bit_Map8[MSG_CLS_TM]))
//        {
//            continue;
//        }
//        
////	        if(dec->tcbc->tbl != 0)             //ʱ����Ϣ�������Ϣ����
//        {
//            if(gs_TimeBk.year != gs_Time.year) //���¼�
//            {
//                //SYS_Message_Send(MSG_YEAR, uc_i);
//            }
//            if(gs_TimeBk.month != gs_Time.month)//���¼�
//            {
//                //SYS_Message_Send(MSG_MON, uc_i);
//            }
//            if(gs_TimeBk.day != gs_Time.day) //���¼�
//            {
//                //SYS_Message_Send(MSG_DAY, uc_i);
//            }
//            if(gs_TimeBk.hour != gs_Time.hour) //Сʱ�¼�
//            {
//                //SYS_Message_Send(MSG_HOUR, uc_i);
//            }
//            if(gs_TimeBk.min != gs_Time.min)   //���¼�
//            {
//				f_timecheck = true;			//���ֺ��30������RTCУʱ
//                //SYS_Message_Send(MSG_MIN, uc_i);
//                SET_SYSE_TMR_MINU;
//            }
//            //SYS_Message_Send(MSG_SEC, uc_i); //��������Ϣ
//        }
    }
#endif

#if SYS_HRTC_EN > 0
    if((gs_Time.sec >= 30) && (gs_Time.sec <= 50) && f_timecheck)          	//�ڵ�30��Уʱ.ÿ����Уʱһ��.
    {
        SYS_RTC_CheckTime();
        f_timecheck = false;
    }
#endif
                                            //����ʱ��
    memcpy_s((uint8*)&gs_TimeBk, (uint8*)&gs_Time, sizeof(TIME));

    //return true;
}


#endif
/************************************************************************
 * @Function: SYS_RTC_Init
 * @Description: �豸ģ���ʼ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_RTC_Init(void)
{
#if (SYS_RTC_EN == 0)
    _virtualTimeInit();
#else
    HAL_InitRTC();
#endif
    
#if SYS_HRTC_EN > 0
    Init_I2C_HRTC(&gs_HrtcPort);            //��ʼ��i2c����
    SYS_RTC_CheckTime();
#endif
 #ifndef __NO_SYS__ 
    //�����û���Դ
    SYS_SEM_Create(1, &gs_RealTimeRes);
    //����һ����ʱ��(��ʱʱ��1s)    
    SYS_Timer_Create(SYS_RTC_SecProc, __NULL, SYS_TICK_PER_SEC, ID_SWTIMER_RTC, false);
#endif	

    gul_SysRunSecs = 0;
	                                        //��ʼ������ʱ��
    SYS_MCU_ReadDateTime(&gs_Time);          //
    memcpy_s((uint8*)&gs_TimeBk, (uint8*)&gs_Time, sizeof(TIME));
    
    f_timecheck = true;					    //�ϵ�������RTCУʱ
}


/************************************************************************
 * @Function: SYS_MSG_Init
 * @Description: ��Ϣ�ַ�ģ���ʼ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void APP_RTC_Init(void)
{

#ifdef  __NO_SYS__
    s_S_MSK_INT_TYPE sTmType;

    sTmType.ucType = SYS_TM_1s;
    sTmType.ucPri = 0;  ///
    //������Ϣ�ַ�����ʱ��
    SMskSetTmFunc(sTmType, SYS_RTC_SecProc);
#else
    //SYS_Timer_Create(MSG_Server, __NULL, 1, ID_SWTIMER_MSG, false);
#endif
}

void APP_RTC_DeInit(void)
{

#ifdef  __NO_SYS__
//	    s_S_MSK_INT_TYPE sTmType;
//	
//	    sTmType.ucType = SYS_TM_1s;
//	    sTmType.ucPri = 0;  ///
    //������Ϣ�ַ�����ʱ��
    SMskClrTmFunc(SYS_TM_1s);
#else
    //SYS_Timer_Create(MSG_Server, __NULL, 1, ID_SWTIMER_MSG, false);
#endif
}


