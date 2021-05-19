/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: dev_led.c
**��   ��   ��: yzy
**����޸�����: 2010��09��10��
**��        ��: led�豸ģ��
*********************************************************************************************************/
#define EXT_DEV_LED

#include "aos/include.h"
#include "k_types.h"
#include "k_api.h"
#include "drv.h"
#include "hal.h"
#include "gpio.h"

static LedLights sLed[LED_NUM];
static ktimer_t     timer_Led;

extern gpio_dev_t brd_led_table[];

/*******************************************************************************
 * @function_name:  SYS_LED_OnOff
 * @function_file:  dev_gpio.c
 * @����: LED���غ���
 * @����: 
 * @param: led(LEDNO): 
 * @param: oc(bool): true����, false Ϩ��
 * @����: 
 * @����: yzy (2010-03-11)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_LED_OnOff(uint32 led, uint8_t oc)
{
    _IF_TRUE_RETURN_VOID(led >= LED_NUM);   //��������
                                            //��ȡLED�˿�ָ��
//	    GPO_PORTS *gp = (GPO_PORTS*)(gs_LedPort + led);
    gpio_dev_t *gp = brd_led_table+led;
    led_dev_priv * rvs = (led_dev_priv *)(gp->priv);
    if(oc)                                  //����ߵ�ƽ
    {
        if(*(rvs->rvs))                         //�߼���ƽ��ת
        {
//	            Chip_GPIO_SetPinState(gp->port->pingrp, gp->port->pinnum, false);
            hal_gpio_output_low(gp);
        }
        else
        {
//	            Chip_GPIO_SetPinState(gp->port->pingrp, gp->port->pinnum, true);
            hal_gpio_output_high(gp);
        }
    }
    else                                    //����͵�ƽ
    {
        if(*(rvs->rvs))
        {
//	            Chip_GPIO_SetPinState(gp->port->pingrp, gp->port->pinnum, true); 
            hal_gpio_output_high(gp);
        }
        else
        {
//	            Chip_GPIO_SetPinState(gp->port->pingrp, gp->port->pinnum, false);
            hal_gpio_output_low(gp);
        }
    }
}





/*******************************************************************************
 * @function_name:  LED_Server
 * @function_file:  dev_gpio.c
 * @����: LED��ʱ�������(100ms����)
 * 
 * 
 * @����: 
 * @param: pdata(void*): 
 * 
 * @����: 
 * @return:  Boolean   
 * @����: yzy (2010-03-11)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void LED_Server(void *timer, void *arg)
{                                            //ѭ�������LED
    for(uint32 i = 0; i < LED_NUM; i++)
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
			SYS_LED_OnOff(i, true);
			ll->type = 0xFF;            //��һ�ν����Ͳ�����
		}
		else if(ll->type == 1)          //��˸
		{
            if(ll->curstt)              //��ǰ����
            {
                if(ll->count >= ll->on_interval)
                {                       
                    SYS_LED_OnOff(i, false);//�����ʱ��֮���Ϩ��
                    ll->curstt = 0;     
                    ll->count = 0;      //����������
                    continue;
                }
            }
            else                        //��ǰϨ��
            {
                if(ll->count >= ll->off_interval)
                {                       //�����ʱ��֮��͵���
                    SYS_LED_OnOff(i, true);
                    ll->curstt = 1;
                    ll->count = 0;      //����������
                    continue;
                }
            }
			ll->count++;                //�������ۼ�
		}
        else if(ll->type == 2)          //����
        {
            SYS_LED_OnOff(i, true);
        }
        else
        {
            SYS_LED_OnOff(i, false);      //��������ر�LED
        }
    }
    
    //return true;
}




/*******************************************************************************
 * @function_name:  SYS_LED_Init
 * @function_file:  dev_gpio.c
 * @����: LED��ʼ��
 * 
 * @����: 
 * @����: 
 * @����: yzy (2010-03-11)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_LED_Init(void)
{
    GPO_PORTS* gpo;  
    
    //ѭ����ʼ������LED�˿�
//	    for(uint32 uc_i = 0; uc_i < LED_NUM; uc_i++)
//	    {
//	        gpo = (GPO_PORTS*)(gs_LedPort + uc_i);  //LED�˿���Ϣ
//	                                                //���ö˿ڹ���,����Ϊ���ģʽ
//	                                                                                    
//	        Chip_GPIO_SetPinDigit(gpo->port->pingrp, gpo->port->pinnum);
//	        
//	        Chip_GPIO_SetPinPFSEG(gpo->port->pingrp, gpo->port->pinnum, gpo->port->pinseg);
//	        Chip_GPIO_SetPinOutputMode(gpo->port->pingrp, gpo->port->pinnum, gpo->port->modefunc & 0x01);
//	        Chip_GPIO_SetPinDIROutput(gpo->port->pingrp, gpo->port->pinnum);   
//	    }
    
                                                //���뻺��
    gsp_LedLights = &sLed;//(LedLights*)SYS_GetMEM(sizeof(LedLights) * LED_NUM);
    ClearBuffer((uint8*)gsp_LedLights, sizeof(LedLights) * LED_NUM);
    
    
    //�������Ʒ���ʱ��
//	    SYS_Timer_Create(LED_Server, __NULL, 10, TIMER_ID_LED, false);
//	    krhino_timer_create(&timer_Led, "timer_Led", LED_Server,
//	                            10, 10, 0, 1);                                            

}


/************************************************************************
 * @function: SYS_LED_BlinkSet
 * @����: LED����Ϩ��ӿں���
 * 
 * @����: 
 * @param: led ������LED�����
 * @param: type 0 ֻ��һ��,1 ��˸,2 ����,���� Ϩ��
 * @param: interval ��˸���,ʱ�䵥λ100ms(typeΪ1ʱ��Ч)
 * @param: time ����ʱ�� ��λ1s(0��ʾ������Ч)
 * @����: 
 * @˵��: 
 * @����: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_LED_BlinkSet(LedNo led, uint8 type, uint8 interval, uint8 time)
{
    if(interval > 1)                //��˸�����1�����ڵ���,��������Ϩ��
    {
        SYS_LED_BlinkSet1(led, type, 1, interval - 1, time);
    }
    else
    {
        SYS_LED_BlinkSet1(led, type, 1, 1, time);
    }
}



/************************************************************************
 * @function: SYS_LED_BlinkSetAll
 * @����: LEDͳһ����Ϩ��ӿں���
 * 
 * @����: 
 * @param: type 0 ֻ��һ��, 1 ��˸, 2 ����,���� Ϩ��
 * @param: interval ��˸���,ʱ�䵥λ100ms(typeΪ1ʱ��Ч)
 * @param: time ����ʱ�� ��λ1s(0��ʾ������Ч)
 * @����: 
 * @˵��: 
 * @����: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_LED_BlinkSetAll(uint8 type, uint8 interval, uint8 time)
{
    if(interval > 1)                //��˸�����1�����ڵ���,��������Ϩ��
    {
        SYS_LED_BlinkSetAll1(type, 1, interval - 1, time);
    }
    else
    {
        SYS_LED_BlinkSetAll1(type, 1, 1, time);
    }
}




/************************************************************************
 * @function: SYS_LED_BlinkSet1
 * @����: LED����Ϩ��ӿں���
 * 
 * @����: 
 * @param: led ������LED�����
 * @param: type 0 ֻ��һ��, 1 ��˸, 2 ����,���� Ϩ��
 * @param: oninterval �������,ʱ�䵥λ100ms(typeΪ1ʱ��Ч)
 * @param: offinterval Ϩ����,ʱ�䵥λ100ms(typeΪ1ʱ��Ч)
 * @param: time ����ʱ�� ��λ1s(0��ʾ������Ч)
 * @����: 
 * @˵��: 
 * @����: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_LED_BlinkSet1(LedNo led, uint8 type, uint8 oninterval, uint8 offinterval, uint8 time)
{
    _IF_TRUE_RETURN_VOID(led >= LED_NUM);    //��������
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();

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
        ll->time = time * 10;
    }
                                            //��������ֵ���interval��Χ��ʱ����.//����ʵ��blinkset()�Ŀ��ظ�����.
	if(((!ll->curstt) && (ll->count > ll->off_interval)) || 
       (ll->curstt && (ll->count > ll->on_interval)))
	{
		ll->count = 0;
	}
    
    RHINO_CPU_INTRPT_ENABLE();
}




/************************************************************************
 * @function: SYS_LED_BlinkSetAll1
 * @����: LEDͳһ����Ϩ��ӿں���
 * 
 * @����: 
 * @param: type 0 ֻ��һ��, 1 ��˸, 2 ����,���� Ϩ��
 * @param: oninterval ��˸���,ʱ�䵥λ100ms(typeΪ1ʱ��Ч)
 * @param: offinterval Ϩ����,ʱ�䵥λ100ms(typeΪ1ʱ��Ч)
 * @param: time ����ʱ�� ��λ1s(0��ʾ������Ч)
 * @����: 
 * @˵��: 
 * @����: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_LED_BlinkSetAll1(uint8 type, uint8 oninterval, uint8 offinterval, uint8 time)
{
    LedLights* ll;
    uint32 bak;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();


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
            ll->time = time * 10;
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
    
    RHINO_CPU_INTRPT_ENABLE();
}


/************************************************************************
 * @function: SYS_LED_TIMEROC
 * @����: LED��ʱ�����غ���
 * 
 * @����: 
 * @param: oc(bool): true����, false ֹͣ
 * @����: 
 * @˵��: 
 * @����: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_LED_TIMEROC(uint8 oc)
{
    if(oc)
    {
        krhino_timer_start(&timer_Led);
    }
    else
    {
        krhino_timer_stop(&timer_Led);
    }
}


