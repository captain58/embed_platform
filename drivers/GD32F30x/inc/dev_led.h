/******************************Copyright(c)******************************
**                         
**
** File Name: dev_led.h
** Author: 
** Date Last Update: 2019-11-02
** Description: LEDģ��ͷ�ļ�(����GPO������Ӧ��)
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
** ��v5.0�Ĳ��:1)��ʱ������10��tick��Ϊ1��tick,�����ṩ�������˸Ƶ��
**            2)�ӿں����е� ����ʱ�� ��λ��Ϊtick.
*************************************************************************/
#ifndef _DEV_LED_H_
#define _DEV_LED_H_


#ifndef EXT_DEV_LED
#define EXT_DEV_LED extern
#endif
 





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
void SYS_BlinkDev_OnOff(uint32 led, bool oc);


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
void SYS_BlinkDev_Init(void);


/************************************************************************
 * @Function: SYS_LED_BlinkSet
 * @Description: LED�����ӿں���(50%ռ�ձ�)
 * 
 * @Arguments: 
 * @param: led �Ʊ��ö��,���� LED_ENUM
 * @param: type 0 ֻ��һ��,1 ��˸,2 ����,���� Ϩ��
 * @param: interval ��˸���,ʱ�䵥λtick(typeΪ1ʱ��Ч)
 * @param: time ����ʱ�� ��λtick(0��ʾ������Ч)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_Dev_HalfBlinkSet(uint32_t led, uint8_t type, uint8_t interval, uint16_t time);


/************************************************************************
 * @Function: SYS_LED_BlinkSetAll
 * @Description: LEDͳһ����Ϩ��ӿں���(50%ռ�ձ�)
 * 
 * @Arguments: 
 * @param: type 0 ֻ��һ��, 1 ��˸, 2 ����,���� Ϩ��
 * @param: interval ��˸���,ʱ�䵥λtick(typeΪ1ʱ��Ч)
 * @param: time ����ʱ�� ��λtick(0��ʾ������Ч)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_Dev_HalfBlinkSetAll(uint8_t type, uint8_t interval, uint16_t time);


/************************************************************************
 * @Function: SYS_Dev_OptBlinkSet
 * @Description: LED,beep����Ϩ��ӿں���,(����ռ�ձ�)
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
void SYS_Dev_OptBlinkSet(uint32_t led, uint8_t type, uint8_t oninterval, uint8_t offinterval, uint16_t time);


/************************************************************************
 * @Function: SYS_LED_OptBlinkSetAll
 * @Description: LEDͳһ����Ϩ��ӿں���(����ռ�ձ�)
 * 
 * @Arguments: 
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
void SYS_Dev_OptBlinkSetAll(uint8_t type, uint8_t oninterval, uint8_t offinterval, uint16_t time);


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
void SYS_LED_TIMEROC(bool oc);


void SYS_BlinkDev_Idel();
void SYS_BlinkDev_WakeUp();


#endif

