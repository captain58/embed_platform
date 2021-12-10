/******************************Copyright(c)******************************
**                         
**
** File Name: dev_led.c
** Author: 
** Date Last Update: 2019-11-02
** Description: LEDģ���ļ�(����GPO������Ӧ��)
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
** ��v5.0�Ĳ��:1)��ʱ������10��tick��Ϊ1��tick,�����ṩ�������˸Ƶ��
**            2)�ӿں����е� ����ʱ�� ��λ��Ϊtick.
*************************************************************************/
#define EXT_LED
#ifdef __MODULE__
#include "WOSsys.h"
#endif

#include "sys.h"
#include "hal.h"
#include "bsp.h"



/************************************************************************
 * @ģ�������ʾ
 ************************************************************************/
#if (SYS_LED_BEEP_EN > 0)
    
    #if !defined(LED_NUM) || (LED_NUM < 1)
        #define LED_NUM  1
        #warning "LED���������"
    #endif
     
    
//    #if !defined(ID_SWTIMER_LED)
//        #define ID_SWTIMER_LED    0xff
//        #warning "�붨��LEDɨ�趨ʱ��ID��ź�"
//    #endif
#endif




/************************************************************************
**LED���ƽṹ����Ϣ
************************************************************************/
typedef struct
{
    uint8  type;                         //��ʾ����������(0ֻ��һ��,1��˸,2����,������)
    uint8  off_interval;                 //��ʾ��Ϩ����(type == 1��Ч)
    uint8  on_interval;                  //��ʾ�Ƶ������(type == 1��Ч)
    uint8  curstt;                       //��ǰ��on����off.0��ʾoff.
    uint16 time;                         //��ʾ�����ó���ʱ��(0xFFFF��ʾ������Ч)
    uint8  count;                        //��ʾ����˸��ʱ��(type == 1��Ч)
}LedLights;

LedLights* gsp_LedLights;

static ktimer_t     gst_LedTimer; 

/************************************************************************
 * @Function: SYS_BlinkDev_OnOff
 * @Description: LED���ز���
 * 
 * @Arguments: 
 * @param: led �Ʊ��ö��,���� LED_ENUM
 * @param: oc true����, false Ϩ��
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_BlinkDev_OnOff(uint32 led, bool oc)
{
#if (SYS_LED_BEEP_EN > 0)
    _IF_TRUE_RETURN_VOID(led >= LED_NUM);   //��������
                                            //��ȡLED�˿�ָ��
    GPO_PORTS *gp = (GPO_PORTS*)gs_LedPort + led;
    if(oc)                                  //����ߵ�ƽ
    {
        if(gp->rvs)                         //�߼���ƽ��ת
        {
            HAL_GPIO_SetPinState(gp->port, gp->port->pinnum, false);
        }
        else
        {
            HAL_GPIO_SetPinState(gp->port, gp->port->pinnum, true);
        }
    }
    else                                    //����͵�ƽ
    {
        if(gp->rvs)
        {
            HAL_GPIO_SetPinState(gp->port, gp->port->pinnum, true);
        }
        else
        {
            HAL_GPIO_SetPinState(gp->port, gp->port->pinnum, false);
        }
    }
#endif
}




#if (SYS_LED_BEEP_EN > 0)
/************************************************************************
 * @Function: LED_Server
 * @Description: LED��ʱ�������(tick����)
 * 
 * @Arguments: 
 * @param: pdata 
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool LED_Server(void* pdata)
{                                      
    for(uint32 i = 0; i < LED_NUM; i++) //ѭ�������LED
    {
        LedLights *ll = gsp_LedLights + i;//��ȡLED���ƽṹ��ָ��
        
        if(ll->time != 0xFFFF)
        {
            if(ll->time > 0)            //������Ч����ʱ��
            {
                ll->time--;
            }
            else
            {
                ll->type = 0xFF;
            }
        }
        
		if(ll->type == 0)               //ֻ��һ�µ�
		{
			SYS_BlinkDev_OnOff(i, true);
			ll->type = 0xFF;            //��һ�ν����Ͳ�����
		}
		else if(ll->type == 1)          //��˸
		{
            if(ll->curstt)              //��ǰ����
            {
                if(ll->count >= ll->on_interval)
                {                       
                    SYS_BlinkDev_OnOff(i, false);//�����ʱ��֮���Ϩ��
                    ll->curstt = 0;     
                    ll->count = 0;      //����������
                    continue;
                }
            }
            else                        //��ǰϨ��
            {
                if(ll->count >= ll->off_interval)
                {                       //�����ʱ��֮��͵���
                    SYS_BlinkDev_OnOff(i, true);
                    ll->curstt = 1;
                    ll->count = 0;      //����������
                    continue;
                }
            }
			ll->count++;                //�������ۼ�
		}
        else if(ll->type == 2)          //����
        {
            SYS_BlinkDev_OnOff(i, true);
        }
        else
        {
            SYS_BlinkDev_OnOff(i, false);      //��������ر�LED
        }
    }
    
    return true;

}
#endif



/************************************************************************
 * @Function: SYS_BlinkDev_Init
 * @Description: LED���ܳ�ʼ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/

void SYS_BlinkDev_Init(void)
{
#if (SYS_LED_BEEP_EN > 0)
    GPO_PORTS* gpo;
    
    //ѭ����ʼ������LED�˿�
    for(uint32 uc_i = 0; uc_i < LED_NUM; uc_i++)
    {
        gpo = (GPO_PORTS*)gs_LedPort + uc_i;//LED�˿���Ϣ
                                            //���ö˿ڹ���,����Ϊ���ģʽ
//        HAL_GPIO_PinConfig(gpo->cp->port, gpo->cp->pin, gpo->cp->mode);
//        HAL_GPIO_SetPinDIR(gpo->cp->port, gpo->cp->pin, true);
        HAL_GPIO_PinConfig(gpo->port);        
        if(gpo->rvs != gpo->ival)           //��ͬ�����
        {
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, 1);
        }
        else
        {
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, 0);
        }
    }
                                            //���뻺��
    gsp_LedLights = (LedLights*)m_malloc(sizeof(LedLights) * LED_NUM);
    memset((uint8*)gsp_LedLights, 0, sizeof(LedLights) * LED_NUM);
#ifndef __NO_SYS__
    //�������Ʒ���ʱ��
//    SYS_Timer_Create(LED_Server, __NULL, 10, ID_SWTIMER_LED, false);
    
//	    krhino_timer_create(&gst_LedTimer, "gst_LedTimer", LED_Server,
//	                        2, 1, 0, 1);   
//	    
    SysTimer_DlyConfig(LED_Server, __NULL, 1, 10);
	SysTimer_DlyStart(1);

#endif
#endif
}

void SYS_BlinkDev_Idel()
{
#if (SYS_LED_BEEP_EN > 0)
    GPO_PORTS* gpo;

    //ѭ����ʼ������LED�˿�
    for(uint32 uc_i = 0; uc_i < LED_NUM; uc_i++)
    {
        gpo = (GPO_PORTS*)gs_LedPort + uc_i;//LED�˿���Ϣ
                                            //���ö˿ڹ���,����Ϊ���ģʽ
        HAL_GPIO_PinConfig(gpo->port);        
        if(gpo->rvs != gpo->ival)           //��ͬ�����
        {
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, 1);
        }
        else
        {
            HAL_GPIO_SetPinState(gpo->port, gpo->port->pinnum, 0);
        }
    }

    m_free(gsp_LedLights);gsp_LedLights=NULL;
#endif
}
//	void SYS_BlinkDev_WakeUp()
//	{
//	#if (SYS_LED_BEEP_EN > 0)
//	    COMPORT* gpo;
//	
//	    //ѭ����ʼ������LED�˿�
//	    for(uint32 uc_i = 0; uc_i < LED_NUM; uc_i++)
//	    {
//	        gpo = (COMPORT*)gs_LedPort + uc_i;//LED�˿���Ϣ
//	                                            //���ö˿ڹ���,����Ϊ���ģʽ
//	        HAL_GPIO_PinConfig(&gpo->gpio[gpo->pingrp], gpo->pinnum, gpo->type, gpo->analog, gpo->dir);        
//	        if(gpo->rvs != gpo->ival)           //��ͬ�����
//	        {
//	            HAL_GPIO_SetPinState(&gpo->gpio[gpo->pingrp], gpo->pinnum, 1);
//	        }
//	        else
//	        {
//	            HAL_GPIO_SetPinState(&gpo->gpio[gpo->pingrp], gpo->pinnum, 0);
//	        }
//	    }
//	#endif

//}


/************************************************************************
 * @Function: SYS_LED_BlinkSet
 * @Description: LED�����ӿں���(50%ռ�ձ�)
 * 
 * @Arguments: 
 * @param: led �Ʊ��ö��,���� LED_ENUM
 * @param: type 0 ֻ��һ��,1 ��˸,2 ����,���� Ϩ��
 * @param: interval ��˸���,ʱ�䵥λ100mstypeΪ1ʱ��Ч)
 * @param: time ����ʱ�� ��λ100ms(0��ʾ������Ч)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_Dev_HalfBlinkSet(uint32_t led, uint8_t type, uint8_t interval, uint16_t time)
{
#if (SYS_LED_BEEP_EN > 0)

    uint8 ont;
    uint8 offt;
    
    ont = interval >> 1;
    offt = interval - ont;
    
    if(ont < 1)
    {
        ont = 1;
    }
    
    if(offt < 1)
    {
        offt = 1;
    }

    SYS_Dev_OptBlinkSet(led, type, ont, offt, time);
#endif    
}




/************************************************************************
 * @Function: SYS_Dev_HalfBlinkSetAll
 * @Description: LEDͳһ����Ϩ��ӿں���(50%ռ�ձ�)
 * 
 * @Arguments: 
 * @param: type 0 ֻ��һ��, 1 ��˸, 2 ����,���� Ϩ��
 * @param: interval ��˸���,ʱ�䵥λ100mstypeΪ1ʱ��Ч)
 * @param: time ����ʱ�� ��λ100ms(0��ʾ������Ч)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_Dev_HalfBlinkSetAll(uint8_t type, uint8_t interval, uint16_t time)
{
#if (SYS_LED_BEEP_EN > 0)

    uint8 ont;
    uint8 offt;
    
    ont = interval >> 1;
    offt = interval - ont;
    
    if(ont < 1)
    {
        ont = 1;
    }
    
    if(offt < 1)
    {
        offt = 1;
    }
    
    SYS_Dev_OptBlinkSetAll(type, ont, offt, time);
#endif
}





/************************************************************************
 * @Function: SYS_Dev_OptBlinkSet
 * @Description: LED����Ϩ��ӿں���
 * 
 * @Arguments: 
 * @param: led �Ʊ��ö��,���� LED_ENUM
 * @param: type 0 ֻ��һ��, 1 ��˸, 2 ����,���� Ϩ��
 * @param: oninterval �������,ʱ�䵥λ100ms(typeΪ1ʱ��Ч)
 * @param: offinterval Ϩ����,ʱ�䵥λ100ms(typeΪ1ʱ��Ч)
 * @param: time ����ʱ�� ��λ100ms(0��ʾ������Ч)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_Dev_OptBlinkSet(uint32_t led, uint8_t type, uint8_t oninterval, uint8_t offinterval, uint16_t time)
{
#if (SYS_LED_BEEP_EN > 0)
    _IF_TRUE_RETURN_VOID(led >= LED_NUM);    //��������
    
    SYS_ENTER_SCRT();

    LedLights* ll = gsp_LedLights + led;    //LED�����ṹ��
    ll->type = type;                        //��������
    _IF_TRUE_DO(oninterval == 0, oninterval = 1);   //���������Ϊ0
    _IF_TRUE_DO(offinterval == 0, offinterval = 1); //Ϩ������Ϊ0
    ll->off_interval = offinterval;                 //Ϩ����
    ll->on_interval = oninterval;                   //�������
                                            //��Чʱ��
    if(time == 0)
    {
        ll->time = 0xFFFF;
    }
    else
    {
        ll->time = time;
    }
                                            //��������ֵ���interval��Χ��ʱ����.//����ʵ��blinkset()�Ŀ��ظ�����.
	if(((!ll->curstt) && (ll->count > ll->off_interval)) || 
       (ll->curstt && (ll->count > ll->on_interval)))
	{
		ll->count = 0;
	}
    
    SYS_EXIT_SCRT();
#endif
}




/************************************************************************
 * @Function: SYS_Dev_OptBlinkSetAll
 * @Description: LEDͳһ����Ϩ��ӿں���
 * 
 * @Arguments: 
 * @param: type 0 ֻ��һ��, 1 ��˸, 2 ����,���� Ϩ��
 * @param: oninterval ��˸���,ʱ�䵥λtick(typeΪ1ʱ��Ч)
 * @param: offinterval Ϩ����,ʱ�䵥λtick(typeΪ1ʱ��Ч)
 * @param: time ����ʱ�� ��λtick(0��ʾ������Ч)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_Dev_OptBlinkSetAll(uint8_t type, uint8_t oninterval, uint8_t offinterval, uint16_t time)
{
#if (SYS_LED_BEEP_EN > 0)
    LedLights* ll;
    uint32 bak;
    
    SYS_ENTER_SCRT();

    _IF_TRUE_DO(oninterval == 0, oninterval = 1);   //���������Ϊ0
    _IF_TRUE_DO(offinterval == 0, offinterval = 1); //Ϩ������Ϊ0

    for(uint32 led = 0; led < LED_NUM; led++)
    {
        ll = gsp_LedLights + led;                   //LED�����ṹ��
        ll->type = type;                            //��������
        ll->off_interval = offinterval;             //Ϩ����
        ll->on_interval = oninterval;               //�������
                                                    //��Чʱ��
        if(time == 0)
        {
            ll->time = 0xFFFF;
        }
        else
        {
            ll->time = time;
        }
        
        if(led == 0)                        //���յ�һ���Ƶ�count����ֵ.
        {                                   //��������ֵ���interval��Χ��ʱ����.//����ʵ��blinkset()�Ŀ��ظ�����.
        	if(((!ll->curstt) && (ll->count > ll->off_interval)) || 
               (ll->curstt && (ll->count > ll->on_interval)))
        	{
        		ll->count = 0;
        	}
            bak = ll->count;
        }
        else
        {  
            ll->count = bak;
        }
    }
    
    SYS_EXIT_SCRT();
#endif
}



/************************************************************************
 * @Function: SYS_LED_TIMEROC
 * @Description: LED��ʱ�����غ���
 * 
 * @Arguments: 
 * @param: oc true����, false ֹͣ
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_LED_TIMEROC(bool oc)
{
#if (SYS_LED_BEEP_EN > 0)
    if(oc)
    {
//        SYS_Timer_Start(ID_SWTIMER_LED);
        krhino_timer_start(&gst_LedTimer);
    }
    else
    {
//        SYS_Timer_Stop(ID_SWTIMER_LED); 
        krhino_timer_stop(&gst_LedTimer);
    }
#endif
}


