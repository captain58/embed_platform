/****************************************Copyright (c)**************************
**                               	________�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------
**��   ��   ��: realtime.c
**��   ��   ��: yzy
**����޸�����: 2018��04��28��
**��        ��: ʵʱʱ�Ӵ�����
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2018��04��28��
** �衡��: ԭʼ�汾
*******************************************************************************/
#define EXT_DEV_REALTIME
#include "rtc.h"
#include "hal_rtc_stm32l4.h"
#include "aos/include.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	#include "lib_app.h"
//	#include "hal.h"
#include "tasks.h"




/******************************************************************************
**��¼��ǰʱ��ı���
******************************************************************************/
TIME gs_Time;                               //��ǰʱ��
TIME gs_TimeBk;                             //����ʱ��



/******************************************************************************
**ʱ�Ӳ�����Դ
******************************************************************************/
ksem_t gs_RealTimeRes;
ktimer_t     g_s_timer;



/******************************************************************************
**�Ƿ�������ⲿӲʱ��Уʱ
******************************************************************************/
bool f_timecheck;							//�Ƿ�������ⲿӲʱ��Уʱ



#if (SYS_RTC_EN == 0)

TIME gs_vtime;                  //RTC������ʱ�������
    
/*******************************************************************************
 * @function_name:  VirtualTimeInit
 * @function_file:  realtime.c
 * @����: ��ʼ������RTCʱ��
 * 
 * 
 * @����: 
 * @param:   
 * 
 * @����: 
 * @return:    
 * @����: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void VirtualTimeInit(void)
{   //2007��1��1������1
    TIME* time = &gs_vtime;
    time->sec = 0;
    time->min = 0;
    time->hour = 0;
    time->dmon = 1;
    time->month = 1;
    time->year = 7;
    time->dyear = 0;
    time->dweek = 1;
}


/*******************************************************************************
 * @function_name:  VirtualTimeSecProc
 * @function_file:  realtime.c
 * @����: ����RTC��ʱ����
 * 
 * 
 * @����: 
 * @param:   
 * 
 * @����: 
 * @return:    
 * @����: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void VirtualTimeSecProc(void)
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
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
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
        time->dmon++;
        time->dweek++;
        time->dyear++;
    }
                                            //�ո���
    if((time->dmon) == 0 || (time->dmon > monthday[time->month - 1]))
    {
        time->dmon = 1;
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
    
    if(time->dweek > 6)                     //dweek����
    {
        time->dweek = 0;
    }
    RHINO_CPU_INTRPT_ENABLE();
}


/*******************************************************************************
 * @function_name:  GetVirtualTime
 * @function_file:  realtime.c
 * @����: ��ȡ����RTCʱ��
 * 
 * 
 * @����: 
 * @param:   
 * 
 * @����: 
 * @return:    
 * @����: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void GetVirtualTime(TIME* datetime)
{
    TIME* time = &gs_vtime;
    datetime->sec = time->sec;
    datetime->min = time->min;
    datetime->hour = time->hour;
    datetime->dmon = time->dmon;
    datetime->month = time->month;
    datetime->year = time->year;
    datetime->dweek = time->dweek;
    datetime->dyear = time->dweek;
}


/*******************************************************************************
 * @function_name:  SetVirtualTime
 * @function_file:  realtime.c
 * @����: ��������RTCʱ��
 * 
 * 
 * @����: 
 * @param:   
 * 
 * @����: 
 * @return:    
 * @����: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SetVirtualTime(TIME* datetime)
{
    //SYS_ENTER_AllSCRT();           //1)2015-3-27 yzy ɾ��
                                    //2)����gs_RealTimeRes��Ϊ��д����.��ɾ��.
                                    //3)�ú�����_SystemTaskInitHook()�е���,��������ڿ����ж�
    TIME* time = &gs_vtime;
    time->sec = datetime->sec;
    time->min = datetime->min;
    time->hour = datetime->hour;
    time->dmon = datetime->dmon;
    time->month = datetime->month;
    time->year = datetime->year;
    time->dweek = CalcWeek((uint8*)&datetime->dmon, 0);
    time->dyear = datetime->dweek;
    
    //SYS_EXIT_AllSCRT();
}


#endif

    
    
    
#if SYS_HRTC_EN > 0
/*******************************************************************************
 * @function_name:  SYS_RTC_CheckTime
 * @function_file:  realtime.c
 * @����: ʵʱʱ��Уʱ
 * 
 * 
 * @����: 
 * @param: pdata  
 * 
 * @����: 
 * @return:  bool   
 * @����: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 SYS_RTC_CheckTime(void)
{
    uint8 buff[7];//ssmmhhwwddmmyy
    uint8 time[6];
    uint32 sec,sec1;
    TIME tm;
    
    if(I2C_HRTC_ReadHexTime(buff) == SYS_ERR_OK)             //��ȡӲʱ��ʱ��
    {
        MoveBuffer(buff, time, 3);               //���ʱ
        MoveBuffer(buff + 4, time + 3, 3);       //������(������)
        sec = CalcSecs(time, false);
        
        I2C_HRTC_ReadHexTime(buff);
        MoveBuffer(buff, time, 3);               //���ʱ
        MoveBuffer(buff + 4, time + 3, 3);       //������(������)
        sec1 = CalcSecs(time, false);
        if(AbsVal(sec1 - sec) <= 2)		        //���ζ�ȡӲ��RTC��ʱ�����2�����ڲ�д��MCU��RTC
        {                                                   //ת�����ݸ�ʽ
            tm.sec = buff[0];
            tm.min = buff[1];
            tm.hour = buff[2];
            tm.dmon = buff[4];
            tm.month = buff[5];
            tm.year = buff[6];
            return SYS_MCU_WriteDateTime(&tm);
        }
    }
    return SYS_ERR_FT;
}
#endif



/*******************************************************************************
 * @function_name:  SYS_RTC_SecProc
 * @function_file:  realtime.c
 * @����: ʵʱʱ���봦��
 * 
 * 
 * @����: 
 * @param: pdata  
 * 
 * @����: 
 * @return:  bool   
 * @����: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_RTC_SecProc(void *timer, void *arg)
{
#if (SYS_RTC_EN == 0)
    VirtualTimeSecProc();
#endif
    uint8_t msg = 0;
    //printf("SYS_RTC_SecProc = %d\n", gul_SysRunSecs);

                                            //��ȡ�ڲ�RTCʱ��
    if(SYS_MCU_ReadDateTime(&gs_Time) != SYS_ERR_OK)
    {
        SYS_MCU_ReadDateTime(&gs_Time);
    }
                                            //ϵͳ����ʱ����һ
    gul_SysRunSecs++;
    
    extern const KTaskDeclare __TKDeclare[SYS_TK_NUM];
    const KTaskDeclare* dec = __TKDeclare;

    for(uint32 uc_i = 0; uc_i < SYS_TK_NUM; uc_i++, dec++)
    {
                                            //����ý��̲������򲻷���
        if(dec->ktask == __NULL || dec->stklen == 0)
        {
            continue;
        }
        
        if(!(guc_MsgApplied[uc_i] & Bit_Map8[MSG_CLS_TM]))
        {
            continue;
        }
        
        if(dec->ktask->tbl != 0 && dec->ktask->msg != NULL)             //ʱ����Ϣ�������Ϣ����
        {
            if(gs_TimeBk.year != gs_Time.year) //���¼�
            {
//	                SYS_Message_Send(MSG_YEAR, uc_i);
                msg = MSG_YEAR;
                krhino_buf_queue_send(dec->ktask->msg, &msg, 1);
            }
            if(gs_TimeBk.month != gs_Time.month)//���¼�
            {
//	                SYS_Message_Send(MSG_MON, uc_i);
                msg = MSG_MON;
                krhino_buf_queue_send(dec->ktask->msg, &msg, 1);
                
            }
            if(gs_TimeBk.dmon != gs_Time.dmon) //���¼�
            {
//	                SYS_Message_Send(MSG_DAY, uc_i);
                msg = MSG_DAY;
                krhino_buf_queue_send(dec->ktask->msg, &msg, 1);

            }
            if(gs_TimeBk.hour != gs_Time.hour) //Сʱ�¼�
            {
//	                SYS_Message_Send(MSG_HOUR, uc_i);
                msg = MSG_HOUR;
                krhino_buf_queue_send(dec->ktask->msg, &msg, 1);
                
            }
            if(gs_TimeBk.min != gs_Time.min)   //���¼�
            {
				f_timecheck = true;			//���ֺ��30������RTCУʱ
//	                SYS_Message_Send(MSG_MIN, uc_i);
                msg = MSG_MIN;
                krhino_buf_queue_send(dec->ktask->msg, &msg, 1);
                
            }
//	            SYS_Message_Send(MSG_SEC, uc_i); //��������Ϣ
            msg = MSG_SEC;
            krhino_buf_queue_send(dec->ktask->msg, &msg, 1);
            
        }
    }
    
#if SYS_HRTC_EN > 0
    if((gs_Time.sec >= 30) && (gs_Time.sec <= 50) && f_timecheck)          	//�ڵ�30��Уʱ.ÿ����Уʱһ��.
    {
        SYS_RTC_CheckTime();
        f_timecheck = false;
    }
#endif
                                            //����ʱ��
    MoveBuffer((uint8*)&gs_Time, (uint8*)&gs_TimeBk, sizeof(TIME));


}

rtc_dev_t g_stRtcDev;

/*******************************************************************************
 * @function_name:  SYS_RTC_Init
 * @function_file:  realtime.c
 * @����: 
 * 
 * @����: 
 * @����: 
 * @����: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_RTC_Init(void)
{
#if (SYS_RTC_EN == 0)
    VirtualTimeInit();
#else
    g_stRtcDev.port = 0;
    g_stRtcDev.config.format = HAL_RTC_FORMAT_BCD;

    hal_rtc_init(&g_stRtcDev);

    //HAL_Init_RTC();
    //HAL_RTC_Start();
#endif
    
#if SYS_HRTC_EN > 0
    Init_I2C_HRTC(&gs_HrtcPort);            //��ʼ��i2c����
    SYS_RTC_CheckTime();
#endif

                                            //�����û���Դ
//	    SYS_SEM_Create(1, &gs_RealTimeRes);
    krhino_sem_create(&gs_RealTimeRes, "RealTime", 1);

                                            //����һ����ʱ��(��ʱʱ��1s)
    gul_SysRunSecs = 0;
//	    SYS_Timer_Create(SYS_RTC_SecProc, __NULL, 100, TIMER_ID_RTC, false);
    krhino_timer_create(&g_s_timer, "sec_timer", SYS_RTC_SecProc,
                        krhino_ms_to_ticks(1000), krhino_ms_to_ticks(1000), 0, 1);
	                                        //��ʼ������ʱ��
    SYS_MCU_ReadDateTime(&gs_Time);          //
    MoveBuffer((uint8*)&gs_Time, (uint8*)&gs_TimeBk, sizeof(TIME));
    
    f_timecheck = true;					    //�ϵ�������RTCУʱ

}

void SYS_RTC_Start(void)
{
//	#if (SYS_RTC_EN == 1)
//	    HAL_RTC_Start();
//	#endif

}

/*******************************************************************************
 * @function_name:  SYS_MCU_ReadBCDDateTime
 * @function_file:  realtime.c
 * @����: ��ȡRTC��ʱ��,��ʽBCD
 * @����: 
 * @param: tm(TIME*): ʱ��ṹ�建��
 * @����: 
 * @����: yzy (2010-11-16)
 ******************************************************************************/
uint8 SYS_MCU_ReadBCDDateTime(TIME * datetime)
{
    
//	    TDev* dev = SYS_DEV_Open(DEV_NO_SYS, 0, __NULL);
//	    if(dev != __NULL)
//	    {                                       //��ȡʱ��
//	        ((TDevSys*)dev->devfunc)->GetTime(datetime);
//	        SYS_DEV_Close(dev);
//	        return SYS_ERR_OK;
//	    }
    
    rtc_time_t rtc_t;

    if(!hal_rtc_get_time(&g_stRtcDev,&rtc_t))
    {
        datetime->sec = rtc_t.sec;
        datetime->min = rtc_t.min;
        datetime->hour = rtc_t.hr;
        datetime->dmon = rtc_t.date;
        datetime->month = rtc_t.month;
        datetime->year = rtc_t.year;
        datetime->dweek = rtc_t.weekday;
        return SYS_ERR_OK;
    }
    return SYS_ERR_FT;
}

/*******************************************************************************
 * @function_name:  SYS_MCU_ReadDateTime
 * @function_file:  realtime.c
 * @����: ��ȡRTC��ʱ��,��ʽHEX
 * 
 * @����: 
 * @param: tm(TIME*): ʱ��ṹ�建��
 * @����: 
 * @����: yzy (2010-11-16)
 ******************************************************************************/
uint8 SYS_MCU_ReadDateTime(TIME* datetime)
{ 
#if (SYS_RTC_EN == 0)
    GetVirtualTime(datetime);
    return SYS_ERR_OK;
#else
    if(SYS_MCU_ReadBCDDateTime(datetime) == SYS_ERR_OK)
    {
        datetime->sec = ByteBcdToHex(datetime->sec);
        datetime->min = ByteBcdToHex(datetime->min);
        datetime->hour = ByteBcdToHex(datetime->hour);
        datetime->dmon = ByteBcdToHex(datetime->dmon);
        datetime->month = ByteBcdToHex(datetime->month);
        datetime->year = ByteBcdToHex(datetime->year);
        datetime->dweek = datetime->dweek;
//	        datetime->dyear = IntHexToBcd(datetime->dyear);
        return SYS_ERR_OK;
    }
    return SYS_ERR_FT;
#endif    
}

/*******************************************************************************
 * @function_name:  SYS_MCU_WriteDateTime
 * @function_file:  dev_realtime.c
 * @����: ����ARM RTCʱ��
 * 
 * 
 * @����: 
 * @param: datetime ʱ��ṹ��,���е���,��,ʱ,��,��,�������ȷ
 * 
 * @����: 
 * @return:  uint8   
 * @����: yzy (2011-02-21)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 SYS_MCU_WriteDateTime(TIME * datetime)
{
    const uint8* monthday;
    rtc_time_t rtc_t;
                                            //�ж��������ݵĺϷ���
    if(datetime->year > 99 || datetime->month == 0 || 
       datetime->month > 12 || datetime->dmon == 0 || 
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
    if(datetime->dmon > monthday[datetime->month - 1])
    {
        return SYS_ERR_FT;
    }
                                            //��ǰΪһ���еĵڼ���
//	    datetime->dyear = gui_MonthBeginDay[datetime->month - 1] + datetime->dmon - 1;
//	                                            //�жϽ����Ƿ�Ϊ����,�Ƿ��ѹ�2��
//	    if((datetime->year & 0x03) == 0 && datetime->month > 2)
//	    {
//	        datetime->dyear++;
//	    }
    datetime->dweek = CalcWeek((uint8*)&datetime->dmon, 1);
    
#if (SYS_RTC_EN == 0)
    SetVirtualTime(datetime);
    return SYS_ERR_OK;
    
#else
                                            //���豸
//	    TDev* dev = SYS_DEV_Open(DEV_NO_SYS, 0, __NULL);
//	    if(dev != __NULL)
//	    {                                       //����ARMRTCʱ��
//	        ((TDevSys*)dev->devfunc)->SetTime(datetime);
//	        SYS_DEV_Close(dev); 
//	        return SYS_ERR_OK;
//	    }
    rtc_t.sec = ByteHexToBcd(datetime->sec);
    rtc_t.min = ByteHexToBcd(datetime->min);
    rtc_t.hr = ByteHexToBcd(datetime->hour);
    rtc_t.date = ByteHexToBcd(datetime->dmon);
    rtc_t.month = ByteHexToBcd(datetime->month);
    rtc_t.year = ByteHexToBcd(datetime->year);
    rtc_t.weekday = datetime->dweek;
    

    if(!hal_rtc_set_time(&g_stRtcDev,(const rtc_time_t*)&rtc_t))
    {
        return SYS_ERR_OK;
    }

    return SYS_ERR_FT;
#endif
}

/*******************************************************************************
 * @function_name:  SYS_MCU_WriteBCDDateTime
 * @function_file:  dev_realtime.c
 * @����: ����ARM RTCʱ��(BCD��ʽ)
 * 
 * 
 * @����: 
 * @param: datetime ʱ��ṹ��BCD��ʽ,���е���,��,ʱ,��,��,�������ȷ
 * 
 * @����: 
 * @return:  uint8   
 * @����: yzy (2011-02-21)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint8 SYS_MCU_WriteBCDDateTime(TIME* datetime)
{
    datetime->sec = ByteBcdToHex(datetime->sec);
    datetime->min = ByteBcdToHex(datetime->min);
    datetime->hour = ByteBcdToHex(datetime->hour);
    datetime->dmon = ByteBcdToHex(datetime->dmon);
    datetime->month = ByteBcdToHex(datetime->month);
    datetime->year = ByteBcdToHex(datetime->year);
    datetime->dweek = datetime->dweek;
//	    datetime->dyear = IntBcdToHex(datetime->dyear);
    return SYS_MCU_WriteDateTime(datetime);
}





/*******************************************************************************
 * @function_name:  RTC_ProcTEsMsg
 * @function_file:  realtime.c
 * @����: ʵʱʱ�Ӵ������������Ϣ������
 * 
 * 
 * @����: 
 * @param: OpType ��������  HRTC_RW_RB��
 * @����: 
 * @����: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
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
            MoveBuffer((uint8*)obj, time, 3);//���ʱ
            MoveBuffer((uint8*)obj + 3, time + 4, 3);//������
            time[3] = CalcWeek(time + 4, 0);          //��
            result = I2C_HRTC_WriteBcdTime(time);
#endif
            result |= SYS_MCU_WriteBCDDateTime(obj);      //ͬʱ����RTCʱ��,�ֽ�˳��һ��TIME���ͺ͸�time[]
            break;
            
        case HRTC_RW_WH:                    //д��HEX��ʽʱ��
#if SYS_HRTC_EN > 0
            MoveBuffer((uint8*)obj, time, 3);//���ʱ
            MoveBuffer((uint8*)obj + 3, time + 4, 3);//������
            time[3] = CalcWeek(time + 4, 0);          //��
            result = I2C_HRTC_WriteHexTime(time);
#endif
            result |= SYS_MCU_WriteDateTime((TIME*)obj); 
            break;
            
        default:
            break;
    
    }
    
    return result;

}


/*******************************************************************************
 * @function_name:  SYS_HRTC_WriteDateTime
 * @function_file:  realtime.c
 * @����: ����ʱ��,��ʽHEX
 * 
 * @����: 
 * @param: tm(TIME*): ʱ��ṹ�建��
 * @����: 
 * @����: yzy (2010-11-16)
 ******************************************************************************/
uint8 SYS_WriteDateTime(TIME* datetime)
{
    uint8 result;
    
    krhino_sem_take(&gs_RealTimeRes, RHINO_WAIT_FOREVER);
    result = RTC_ProcTEsMsg(HRTC_RW_WH, datetime);
    krhino_sem_give(&gs_RealTimeRes);
    return result;
}

/*******************************************************************************
 * @function_name:  SYS_HRTC_ReadDateTime
 * @function_file:  realtime.c
 * @����: ��ȡʱ��,��ʽHEX
 * 
 * @����: 
 * @param: tm(TIME*): ʱ��ṹ�建��
 * @����: 
 * @����: yzy (2010-11-16)
 ******************************************************************************/
uint8 SYS_ReadDateTime(TIME* datetime)
{
    uint8 result;
    krhino_sem_take(&gs_RealTimeRes, RHINO_WAIT_FOREVER);
    result = RTC_ProcTEsMsg(HRTC_RW_RH, datetime);
    krhino_sem_give(&gs_RealTimeRes);
    return result; 
}


/*******************************************************************************
 * @function_name:  SYS_HRTC_ReadDateTime
 * @function_file:  realtime.c
 * @����: ��ȡ�ⲿRTC��ʱ��,��ʽBCD 
 * @����: 
 * @param: tm(TIME*): ʱ��ṹ�建��
 * @����: 
 * @����: yzy (2010-11-16)
 ******************************************************************************/
uint8 SYS_ReadBCDDateTime(TIME* datetime)
{
    uint8 result;
    krhino_sem_take(&gs_RealTimeRes, RHINO_WAIT_FOREVER);
    result = RTC_ProcTEsMsg(HRTC_RW_RB, datetime);
    krhino_sem_give(&gs_RealTimeRes);
    return result; 
}

/*******************************************************************************
 * @function_name:  SYS_HRTC_ReadDateTime
 * @function_file:  realtime.c
 * @����: �����ⲿRTC��ʱ��,��ʽBCD
 * @����: 
 * @param: tm(TIME*): ʱ��ṹ�建��
 * @����: 
 * @����: yzy (2010-11-16)
 ******************************************************************************/
uint8 SYS_WriteBCDDateTime(TIME* datetime)
{
    uint8 result;
    krhino_sem_take(&gs_RealTimeRes, RHINO_WAIT_FOREVER);
    result = RTC_ProcTEsMsg(HRTC_RW_WB, datetime);
    krhino_sem_give(&gs_RealTimeRes);
    return result; 
}





/*******************************************************************************
 * @function_name:  GetTime
 * @function_file:  realtime.c
 * @����: ��ȡ��ǰ��ʱ������ڵ�ȫ����Ϣ, �����Ϣֻ��
 * 
 * 
 * @����: 
 * @param: 
 * @����: ��ǰʱ��ṹ��
 * @����: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
const TIME* GetTime(void)
{
    return (const TIME*)&gs_Time;
}



/*******************************************************************************
 * @function_name:  GetSysRunSecs
 * @function_file:  realtime.c
 * @����: ��ȡϵͳ��������ʱ��
 * 
 * 
 * @����: 
 * @param: 
 * @����: ��ǰϵͳ����������
 * @����: yzy (2010-02-20)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint32 GetSysRunSecs(void)
{
    return gul_SysRunSecs;
}

