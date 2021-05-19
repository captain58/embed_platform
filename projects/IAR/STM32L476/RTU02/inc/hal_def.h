/****************************************Copyright (c)*************************
**                               	________�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: hal_def.h
**��   ��   ��: yzy
**����޸�����: 2011��1��12��
**��        ��: HAL����ͷ�ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ---------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2018��1��25��
** �衡��: ԭʼ�汾
******************************************************************************/
#ifndef _HAL_DEF_H_
#define _HAL_DEF_H_



#include "aos/bdef.h"

#include "cdef.h"

//	#include "hal_uart9.h"

//#include "ior5f10wmg_ext.h"
//#include "ior5f10wmg.h"

#include "libs.h"

#include <k_api.h>
typedef struct _SUart
{
//	    uint8 ch;           //uartʵ��ʹ��ͨ��(0~3)
//	    uint8 port;         //uart�ṹ������
    uint8 has_Wait_rcv; //�Ƿ����û��ڵȴ���������
    kevent_t event;     //alios event���
}__SUart;


#include "ext.h"
#include "uart.h"


/******************************************************************************
**���ڷ������Ϣ��
******************************************************************************/
typedef struct
{
    //uart����ز���
    uart_dev_t * handle;
    uint8 port;                         //uart�ṹ������ 
    USART_TypeDef *  USARTx;             //���ھ��
    uint8* uart_RecvFlag;               //���ݽ�����ϱ�־
    uint8* uart_ByteTimeout;//�����ֽڼ䳬ʱ
    uint8* uart_RecvStart;//����������־

}SerialID;


/******************************************************************************
**Flash,����SPI�ӿ�
******************************************************************************/
#define SPI_DEV_ID_FLSA         0
#define SPI_DEV_ID_FLSB         1
#define SPI_DEV_ID_FRAM         2


/******************************************************************************
**�ⲿ�жϷ������Ϣ��
******************************************************************************/
typedef struct
{
    //��ز���
    uint8 ch:4;
    uint8 port:4;
    uint8 isRising;     //�������ж�
    uint8 mode; // 0 key; 1 ���� 2����
    uint8 hallch; //1 hallA;2 hallB
    TCallback intx_Enable;        //ʹ��
    TCallback intx_Disable;        //����
    TCallback intx_Priority;        //�ж����ȼ�
    //
    GPIO_TypeDef   *    INTx;             //�жϿھ��
    TCallback  pinConfig;          //�жϿڹܽ����ú�����
}InterruptID;

/******************************************************************************
**�ⲿ�жϷ������Ϣ��
******************************************************************************/

/*
    Timer Mode Register mn (TMRmn)
*/
/* Selection of macro clock (MCK) of channel n (CKSmn1 - CKSmn0) */
#define CLOCK_SELECT_CKM0             (0x0) /* operation clock CK0 set by PRS register */ 
#define CLOCK_SELECT_CKM1             (0x1) /* operation clock CK1 set by PRS register */
#define CLOCK_SELECT_CKM2             (0x2) /* operation clock CK2 set by PRS register */
#define CLOCK_SELECT_CKM3             (0x3) /* operation clock CK3 set by PRS register */
/* Selection of count clock (CCK) of channel n (CCSmn) */
#define CLOCK_MODE_CKS                (0x0) /* macro clock MCK specified by CKSmn bit */  
#define CLOCK_MODE_TIMN               (0x1) /* valid edge of input signal input from TImn pin */
/* Selection of slave/master of channel n (MASTERmn) */
#define COMBINATION_SLAVE             (0x0) /* operates as slave channel */
#define COMBINATION_MASTER            (0x1) /* operates as master channel */
/* Operation explanation of channel 1 or 3 (SPLIT) */
#define BITS16_MODE                   (0x0) /* operates as 16 bits timer */  
#define BITS8_MODE                    (0x1) /* operates as 8 bits timer */
/* Setting of start trigger or capture trigger of channel n (STSmn2 - STSmn0) */
#define TRIGGER_SOFTWARE              (0x0) /* only software trigger start is valid */
#define TRIGGER_TIMN_VALID            (0x1) /* TImn input edge is used as a start/capture trigger */
#define TRIGGER_TIMN_BOTH             (0x2) /* TImn input edges are used as a start/capture trigger */
#define TRIGGER_MASTER_INT            (0x4) /* interrupt signal of the master channel is used */
/* Selection of TImn pin input valid edge (CISmn1 - CISmn0) */
#define TIMN_EDGE_FALLING             (0x0) /* falling edge */
#define TIMN_EDGE_RISING              (0x1) /* rising edge */
#define TIMN_EDGE_BOTH_LOW            (0x2) /* both edges (when low-level width is measured) */
#define TIMN_EDGE_BOTH_HIGH           (0x3) /* both edges (when high-level width is measured) */
/* Operation mode of channel n (MDmn3 - MDmn0) */
#define MODE_INTERVAL_TIMER           (0x0) /* interval timer mode */
#define MODE_CAPTURE                  (0x4) /* capture mode */
#define MODE_EVENT_COUNT              (0x6) /* event counter mode */
#define MODE_ONE_COUNT                (0x8) /* one count mode */
#define MODE_HIGHLOW_MEASURE          (0xC) /* high-/low-level width measurement mode */
#define MODE_PWM_MASTER               (0x1) /* PWM function (master channel) mode */
#define MODE_PWM_SLAVE                (0x9) /* PWM function (slave channel) mode */
#define MODE_ONESHOT                  (0x8) /* one-shot pulse output mode */
/* Setting of starting counting and interrupt (MDmn0) */
#define START_INT_UNUSED              (0x0) /* interrupt is not generated when counting is started */
#define START_INT_USED                (0x1) /* interrupt is generated when counting is started */



typedef struct
{
    //��ز���
    uint8 ch:4;
    uint8 port:4;
    uint8 timerx_Int;                    //�жϼ���
    uint8 timerx_Int_Prior;              //�ж����ȼ�
    TIMER_MODE option;
    TimerSets * ts;
//	    TCallback timerx_Enable;        //ʹ��
//	    TCallback timerx_Disable;        //����
//	    TCallback timerx_Priority;        //�ж����ȼ�
    //
    GPIO_TypeDef   *    INTx;             //�жϿھ��
    TCallback  pinConfig;          //�жϿڹܽ����ú�����
}TimerID;

#endif


