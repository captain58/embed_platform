/******************************Copyright(c)******************************
**                          
**
** File Name: dev_gpi.c
** Author: 
** Date Last Update: 2019-11-02
** Description: ��ͨ������豸
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#define EXT_GPI
#define EXT_DEV_GPI
#ifdef __MODULE__
#include "WOSsys.h"
#endif
#include "sys.h"
#include "hal.h"
#include "bsp.h"


#include "tasks.h"


/************************************************************************
 * @ģ�������ʾ
 ************************************************************************/
#if (SYS_LGPI_EN > 0)
    
    #if !defined(LGPI_PORT_NUM) || (LGPI_PORT_NUM < 1)
        #define LGPI_PORT_NUM  1
        #warning "LGPI���������"
    #endif

    #if !defined(LGPI_LAST) || !defined(LGPI_GATE) || (LGPI_LAST < LGPI_GATE) || (LGPI_GATE < 1)
        #define LGPI_LAST  1
        #define LGPI_GATE  1
        #warning "LGPIɨ�����޺����"
    #endif

//    #if !defined(ID_SWTIMER_LGPI)
//        #define ID_SWTIMER_LGPI    0xff
//        #warning "�붨��LGPIɨ�趨ʱ��ID��ź�"
//    #endif
    
#endif

        
#if (SYS_FGPI_EN > 0)
                                     
    #if !defined(FGPI_PORT_NUM) || (FGPI_PORT_NUM < 1)
        #define FGPI_PORT_NUM  1
        #warning "FGPI���������"
    #endif
        
    #if !defined(FGPI_GATE) || (FGPI_GATE < 1)
        #define FGPI_GATE   1
        #warning "FGPIɨ�����޺����"
    #endif

    #if !defined(ID_SWTIMER_FGPI)
        #define ID_SWTIMER_FGPI    0xff
        #warning "�붨��FGPIɨ�趨ʱ��ID��ź�"
    #endif
        
#endif


#if (SYS_GPIEVT_EN > 0)
     
    #if !defined(MSG_FIFEVT) || !defined(MSG_FIFEVT) || !defined(MSG_FIFEVT) \
                             || !defined(MSG_FIFEVT) || !defined(MSG_FIFEVT)
        #error "�붨��GPI�¼���"
    #endif
     
#endif







#if (SYS_LGPI_EN > 0)
                               
static ktimer_t     timer_Lgpi;                               
                               
/************************************************************************
 * @Function: SYS_LGPI_Scan
 * @Description: ��������˿�ɨ�趨ʱ������
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
void SYS_LGPI_Scan(void)
{
    uint8  uc_i;
    GPI_PORTS* gpi;
    COMPORT* cp;
    
    for(uc_i = 0; uc_i < LGPI_PORT_NUM; uc_i++)
    {
        gpi = (GPI_PORTS*)gs_LGPIPort + uc_i;
//	        cp = (COMPORT*)gpi->cp;
        
        _IF_TRUE_DO(gpi == __NULL, continue);
        
        if(gpi->filter)                             //(��ȥ�����ܼ����������ж�)(����Ӧ��:����)
        {
            if(gsp_GpioStt->gpistt & (0x01 << uc_i))//��ǰ�ߵ�ƽ.�ж��½��¼�
            {
                if(!HAL_GPIO_GetPinState(gpi->port, gpi->port->pinnum))
                {
                    gsp_GpioStt->cnt[uc_i]++;       //�������ۼ�
                }
                else                                //�������
                {
                    gsp_GpioStt->cnt[uc_i] = 0;
                }
                gsp_GpioStt->lastcnt[uc_i] = 0;     //�ߵ�ƽ״̬�°������ּ���������
            }
            else                                    //��ǰ�͵�ƽ
            {
                if(HAL_GPIO_GetPinState(gpi->port, gpi->port->pinnum))//��
                {
                    gsp_GpioStt->cnt[uc_i]++;       //�������ۼ�
                }
                else                                //�͵�ƽ
                {
                    gsp_GpioStt->cnt[uc_i] = 0;     //����������
                    
                    if(gsp_GpioStt->lastcnt[uc_i] <= LGPI_LAST)
                        gsp_GpioStt->lastcnt[uc_i]++;   //�͵�ƽ״̬�°������ּ������ۼ�
                }
            }
                                                    //�����¼�
            if(gsp_GpioStt->cnt[uc_i] >= LGPI_GATE)
            {                                       //�½����¼�
                gsp_GpioStt->keyfevt |= gsp_GpioStt->gpistt & (0x01 << uc_i);
                gsp_GpioStt->gpistt ^= (0x01 << uc_i);//״̬ȡ��
                gsp_GpioStt->keyrevt |= gsp_GpioStt->gpistt & (0x01 << uc_i);//�������¼�
                gsp_GpioStt->cnt[uc_i] = 0;         //����������
            }
                                                    //���������¼��ж�
            if(gsp_GpioStt->lastcnt[uc_i] == LGPI_LAST)
            {
//	                gsp_GpioStt->lastcnt[uc_i] = 0;//LGPI_LAST - LGPI_GATE;
                gsp_GpioStt->keylevt |= (0x01 << uc_i);
            }
        }
        else                                        //��ͨGPI����˿�ɨ��(��ȥ������)
        {
            if(HAL_GPIO_GetPinState(gpi->port, gpi->port->pinnum))//��
            {
                gsp_GpioStt->gpistt |= (0x01 << uc_i);
            }
            else                                    //��
            {
                gsp_GpioStt->gpistt &= ~(0x01 << uc_i);
            }
        }
    }
    
//	    return true;
}
#endif




#if (SYS_FGPI_EN > 0)
/************************************************************************
 * @Function: SYS_FGPI_Scan
 * @Description: ��������˿�ɨ�趨ʱ������
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
bool SYS_FGPI_Scan(void* pdata)
{
    uint8 uc_i;
    GPI_PORTS* gpi;
//    COMPORT* cp;
    
    for(uc_i = 0; uc_i < FGPI_PORT_NUM; uc_i++)
    {
        gpi = (GPI_PORTS*)(gs_FGPIPort + uc_i);
//        cp = (COMPORT*)gpi->cp;
        
        _IF_TRUE_DO(gpi == __NULL, continue);

        
        if(gpi->filter)
        {
                                                    //��ǰ�ߵ�ƽ.�ж��½��¼�
            if(gsp_GpioStt->fgpistt & (0x01 << uc_i))
            {
                if(!HAL_GPIO_GetPinState(&gpi->cp->gpio[gpi->cp->pingrp], gpi->cp->pinnum))//��
                {
                    gsp_GpioStt->fcnt[uc_i]++;      //�������ۼ�
                }
                else                                //�������
                {
                    gsp_GpioStt->fcnt[uc_i] = 0;
                }
            }
            else                                    //��ǰ�͵�ƽ
            {
                if(HAL_GPIO_GetPinState(&gpi->cp->gpio[gpi->cp->pingrp], gpi->cp->pinnum))//��
                {
                    gsp_GpioStt->fcnt[uc_i]++;      //�������ۼ�
                }
                else                                //�͵�ƽ
                {
                    gsp_GpioStt->fcnt[uc_i] = 0;    //����������
                }
            }
                                                    //�����¼�
            if(gsp_GpioStt->fcnt[uc_i] >= FGPI_GATE)
            {                                       //�½����¼�
                gsp_GpioStt->fgpifevt |= gsp_GpioStt->fgpistt & (0x01 << uc_i);
                gsp_GpioStt->fgpistt ^= (0x01 << uc_i);//״̬ȡ��
                gsp_GpioStt->fgpirevt |= gsp_GpioStt->fgpistt & (0x01 << uc_i);//�������¼�
                gsp_GpioStt->fcnt[uc_i] = 0;           //����������
            }
        }
        else                                        //��ȥ������
        {
            if(HAL_GPIO_GetPinState(&gpi->cp->gpio[gpi->cp->pingrp], gpi->cp->pinnum))//��
            {
                gsp_GpioStt->fgpistt |= (0x01 << uc_i);
            }
            else                                    //��
            {
                gsp_GpioStt->fgpistt &= ~(0x01 << uc_i);
            }
        }
    }
    
	return true;
}

#endif

#ifdef __NO_SYS__    
void SYS_GPI_GoScan(void* pdata)
{
    SysSndMsg(D_GENDRV_MDL_NO,2);
}
#endif

GPIO_IN __GpioStt;

/************************************************************************
 * @Function: SYS_GPI_Init
 * @Description: �����ģ���ʼ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_GPI_Init(void)
{
#if ((SYS_LGPI_EN > 0) || (SYS_FGPI_EN > 0))
    GPI_PORTS* gpi;
    uint16 timeout;
    uint8 uc_i = 0;
#endif
                                            //���뻺��
    gsp_GpioStt = &__GpioStt;//(GPIO_IN*)m_malloc(sizeof(GPIO_IN));
    memset((uint8*)gsp_GpioStt, 0, sizeof(GPIO_IN));
    gsp_GpioStt->gpistt = 0xFFFFFFFF;       //����GPI�˿ڳ�ʼ״̬
    gsp_GpioStt->fgpistt = 0xFFFFFFFF;      //����GPI�˿ڳ�ʼ״̬
    
#if (SYS_FGPI_EN > 0)
                                            //��ʼ��Ϊ����״̬
    for(uc_i = 0; uc_i < FGPI_PORT_NUM; uc_i++)
    {
        gpi = (GPI_PORTS*)gs_FGPIPort + uc_i;
        _IF_TRUE_DO(gpi == __NULL, continue);
                                            //���ö˿ڹ���,����Ϊ����ģʽ
//	        HAL_GPIO_PinConfig(gpi->cp->port, gpi->cp->pin, gpi->cp->mode);
//	        HAL_GPIO_SetPinDIR(gpi->cp->port, gpi->cp->pin, false);
        HAL_GPIO_PinConfig(&gpi->cp->gpio[gpi->cp->pingrp], gpi->cp->pinnum, gpi->cp->type, gpi->cp->analog, gpi->cp->dir);
        
    }
                                            //�������ٶ˿�ɨ�趨ʱ��(10ms)
    timeout = SYS_TICK_PER_SEC / 100;
    timeout = (timeout < 1) ? 1 : timeout;
#ifndef __NO_SYS__     
    SYS_Timer_Create(SYS_FGPI_Scan, __NULL, timeout, ID_SWTIMER_FGPI, false);
#endif    
#endif
    
#if (SYS_LGPI_EN > 0)
//	    uint8_t interuptSwitch = 0;
                                            //��ʼ��Ϊ����״̬
    for(uc_i = 0; uc_i < LGPI_PORT_NUM; uc_i++)
    {
        gpi = (GPI_PORTS*)gs_LGPIPort + uc_i;
        _IF_TRUE_DO(gpi == __NULL, continue);
                                            //���ö˿ڹ���,����Ϊ����ģʽ
//	        HAL_GPIO_PinConfig(gpi->cp->port, gpi->cp->pin, gpi->cp->mode);
//	        HAL_GPIO_SetPinDIR(gpi->cp->port, gpi->cp->pin, false);
        HAL_GPIO_PinConfig(gpi->port);
//	        if(gpi->handleen)
//	        {
//	            HAL_GPIO_EInt_Cfg(gpi->handleno, &gpi->gpio[gpi->pingrp], gpi->pinnum, gpi->edge, SYS_LGPI_Scan, NULL);
//	        }

    }

//	    if(interuptSwitch) NVIC_EnableIRQ(GPIO_IRQn);
#ifndef __NO_SYS__        
                                            //�������ٶ˿�ɨ�趨ʱ��(100ms)
    timeout = SYS_TICK_PER_SEC / 10;
    timeout = (timeout < 1) ? 1 : timeout;
     
//    SYS_Timer_Create(SYS_LGPI_Scan, __NULL, timeout, ID_SWTIMER_LGPI, false);
//    krhino_timer_create(&timer_Fgpi, "timer_Fgpi", SYS_FGPI_Scan,
//                            10, 1, 0, 1);   
//	    krhino_timer_create(&timer_Lgpi, "timer_Lgpi", SYS_LGPI_Scan,
//	                            krhino_ms_to_ticks(10), krhino_ms_to_ticks(10), 0, 1);         
#endif
#endif
}



void SYS_GPI_WakeUp(void)
{
#if ((SYS_LGPI_EN > 0) || (SYS_FGPI_EN > 0))
    GPI_PORTS* gpi;
    uint8 uc_i = 0;
//	    uint8_t interuptSwitch = 0;

    for(uc_i = 0; uc_i < LGPI_PORT_NUM; uc_i++)
    {
        gpi = (GPI_PORTS*)gs_LGPIPort + uc_i;
        _IF_TRUE_DO(gpi == __NULL, continue);
                                            //���ö˿ڹ���,����Ϊ����ģʽ
//        if(gpi->handleen)
//        {
//            HAL_GPIO_EInt_Cfg(gpi->handleno, &gpi->gpio[gpi->pingrp], gpi->pinnum, gpi->edge, SYS_GPI_GoScan, NULL);
////	            interuptSwitch = 1;
//        }

    }
//	    if(interuptSwitch) 
    //ˮ�����л����жϳ���
//    NVIC_EnableIRQ(GPIO_IRQn);
#endif
}


/************************************************************************
 * @Function: SYS_GPI_EvtMessageLoop
 * @Description: GPI�¼���Ϣ�ַ�����,�ṩ����Ϣ��Ԫ
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/

//void SYS_GPI_EvtMessageLoop(void)
//{
//#if (SYS_GPIEVT_EN > 0)
//    uint8 tkid;
//    uint8 evtchg = 0;
//    
//    SYS_ENTER_SCRT();        //yzy:������·���:���жϵ���N������ʱ,evt�����仯,��ñ仯�޷���ǰN�������еõ�����
//                                            //���ٶ˿��½���
//    if(gsp_GpioStt->fgpifevt & ~gsp_GpioStt->fgpifevtbak)
//    {
//        gsp_GpioStt->fgpifevtbak = gsp_GpioStt->fgpifevt;
//        evtchg |= 0x01;
//    }
//                                            //���ٶ˿�������
//    if(gsp_GpioStt->fgpirevt & ~gsp_GpioStt->fgpirevtbak)
//    {
//        gsp_GpioStt->fgpirevtbak = gsp_GpioStt->fgpirevt;
//        evtchg |= 0x02;
//    }
//                                            //���ٶ˿��½���
//    if(gsp_GpioStt->keyfevt & ~gsp_GpioStt->keyfevtbak)
//    {
//        gsp_GpioStt->keyfevtbak = gsp_GpioStt->keyfevt;
//        evtchg |= 0x04;
//    }
//                                            //���ٶ˿�������
//    if(gsp_GpioStt->keyrevt & ~gsp_GpioStt->keyrevtbak)
//    {
//        gsp_GpioStt->keyrevtbak = gsp_GpioStt->keyrevt;
//        evtchg |= 0x08;
//    }
//                                            //���ٶ˿ڱ���
//    if(gsp_GpioStt->keylevt & ~gsp_GpioStt->keylevtbak)
//    {
//        gsp_GpioStt->keylevtbak = gsp_GpioStt->keylevt;
//        evtchg |= 0x10;
//    } 
//    SYS_EXIT_SCRT();         //yzy
//    
//    
//    for(tkid = 0; tkid < SYS_TK_NUM; tkid++)
//    {
//        if(guc_MsgApplied[tkid] & Bit_Map8[MSG_CLS_GPIO])
//        {
//#ifndef __NO_SYS__ 
//        
//            if(evtchg & 0x01)               //���ٶ˿��½���
//            {
//                SYS_Message_Send(MSG_FIFEVT , tkid);
//            }
//            if(evtchg & 0x02)               //���ٶ˿�������
//            {
//                SYS_Message_Send(MSG_FIREVT, tkid);
//            }
//            if(evtchg & 0x04)                   //���ٶ˿��½���
//            {
//                SYS_Message_Send(MSG_LIFEVT, tkid);
//            }
//            if(evtchg & 0x08)                   //���ٶ˿�������
//            {
//                SYS_Message_Send(MSG_LIREVT, tkid);
//            }
//            if(evtchg & 0x10)                   //���ٶ˿ڱ���
//            {
//                SYS_Message_Send(MSG_LILEVT, tkid);
//            }
//#else
//            if(evtchg & 0x01)               //���ٶ˿��½���
//            {
//                SysSndMsg(tkid,  MSG_FIFEVT);
//            }
//            if(evtchg & 0x02)               //���ٶ˿�������
//            {
//                SysSndMsg(tkid,  MSG_FIREVT);
//            }
//            if(evtchg & 0x04)                   //���ٶ˿��½���
//            {
//                SysSndMsg(tkid,  MSG_LIFEVT);
//            }
//            if(evtchg & 0x08)                   //���ٶ˿�������
//            {
//                SysSndMsg(tkid,  MSG_LIREVT);
//            }
//            if(evtchg & 0x10)                   //���ٶ˿ڱ���
//            {
//                SysSndMsg(tkid,  MSG_LILEVT);
//            }
//
//
////	
////	            if(evtchg != 0)
////	            {
////	                SysSndMsg(tkid,  MSG_GPI);
////	            }
//#endif
//        }
//        
//    }
//#endif
//}

void SYS_GPI_EvtMessageLoop(void)
{
    uint8 tkid;
    uint8 msg = 0xff;
    
    extern const KTaskDeclare __TKDeclare[SYS_TK_NUM];
    const KTaskDeclare* dec = __TKDeclare;
    
    for(tkid = 0; tkid < SYS_TK_NUM; tkid++)
    {
        msg = 0xff;
        if(guc_MsgApplied[tkid] & Bit_Map8[MSG_CLS_GPIO])
        {
            if(gsp_GpioStt->fgpifevt & ~gsp_GpioStt->fgpifevtbak)          //���ٶ˿��½���
            {
                msg = MSG_FIFEVT;
                krhino_buf_queue_send(dec[tkid].ktask->msg, &msg, 1);
                
            }
            if(gsp_GpioStt->fgpirevt & ~gsp_GpioStt->fgpirevtbak)          //���ٶ˿�������
            {
                msg = MSG_FIREVT;
                krhino_buf_queue_send(dec[tkid].ktask->msg, &msg, 1);
//	                SYS_Message_Send(MSG_FIREVT, tkid);
            }
            if(gsp_GpioStt->keyfevt & ~gsp_GpioStt->keyfevtbak)           //���ٶ˿��½���
            {
                msg = MSG_LIFEVT;
                krhino_buf_queue_send(dec[tkid].ktask->msg, &msg, 1);
//	                SYS_Message_Send(MSG_LIFEVT, tkid);
            }
            if(gsp_GpioStt->keyrevt & ~gsp_GpioStt->keyrevtbak)           //���ٶ˿�������
            {
                msg = MSG_LIREVT;
                krhino_buf_queue_send(dec[tkid].ktask->msg, &msg, 1);
//	                SYS_Message_Send(MSG_LIREVT, tkid);
            }
            if(gsp_GpioStt->keylevt & ~gsp_GpioStt->keylevtbak)           //���ٶ˿ڱ���
            {
                msg = MSG_LILEVT;
//	                SYS_Message_Send(MSG_LILEVT, tkid);
                krhino_buf_queue_send(dec[tkid].ktask->msg, &msg, 1);
            }
//            if(msg != 0xff)
//            {
//                krhino_buf_queue_send(dec[tkid].ktask->msg, &msg, 1);
//            }
        }
    }
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
            //yzy:������·���:���жϵ���N������ʱ,evt�����仯,��ñ仯�޷���ǰN�������еõ�����
    gsp_GpioStt->fgpifevtbak = gsp_GpioStt->fgpifevt;
    gsp_GpioStt->fgpirevtbak = gsp_GpioStt->fgpirevt;
    gsp_GpioStt->keyfevtbak = gsp_GpioStt->keyfevt;
    gsp_GpioStt->keyrevtbak = gsp_GpioStt->keyrevt;
    gsp_GpioStt->keylevtbak = gsp_GpioStt->keylevt;
    
    RHINO_CPU_INTRPT_ENABLE();
    
}




/************************************************************************
 * @Function: SYS_GPI_GetStt
 * @Description: ��ȡ���������״̬
 * 
 * @Arguments: 
 * @param: type 0:���ٿ�����״̬
 *              1:���ٿ�����״̬
 * 
 * @Note: 
 * @Return: uint32  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint32 SYS_GPI_GetStt(uint8 type)
{
    if(type == 0)
    {
        return gsp_GpioStt->gpistt;
    }
    else
    {
        return gsp_GpioStt->fgpistt;
    }
}


bool SYS_GPI_GetLPort(uint8_t port)
{
    return HAL_GPIO_GetPinState(gs_LGPIPort[port].port, gs_LGPIPort[port].port->pinnum);
}
/************************************************************************
 * @Function: SYS_GPI_Event
 * @Description: ��ȡ������¼�
 * 
 * @Arguments: 
 * @param: type �¼�����ö��,����ΪGPI_ENUM
 * 
 * @Note: 
 * @Return: uint32  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint32 SYS_GPI_Event(uint8 type)
{
    uint32 result = 0;
    
    SYS_ENTER_SCRT();
    switch((GPI_ENUM)type)
    {
		case GPI_TYPE_LIFEVT:              //��ȡ���ٿ��½����¼�
            result = gsp_GpioStt->keyfevt;
            gsp_GpioStt->keyfevt = 0;
            gsp_GpioStt->keyfevtbak = 0;
            break;
        case GPI_TYPE_LIREVT:              //��ȡ���ٿ��������¼�
            result = gsp_GpioStt->keyrevt;
            gsp_GpioStt->keyrevt = 0;
            gsp_GpioStt->keyrevtbak = 0;
            break;
        case GPI_TYPE_LILEVT:              //��ȡ���ٿڱ����¼�
            result = gsp_GpioStt->keylevt;
            gsp_GpioStt->keylevt= 0;
            gsp_GpioStt->keylevtbak= 0;
            break;
        case GPI_TYPE_FIFEVT:              //��ȡ���ٿ��½����¼�
            result = gsp_GpioStt->fgpifevt;
            gsp_GpioStt->fgpifevt= 0;
            gsp_GpioStt->fgpifevtbak= 0;
            break;
        case GPI_TYPE_FIREVT:              //��ȡ���ٿ��������¼�
            result = gsp_GpioStt->fgpirevt;
            gsp_GpioStt->fgpirevt = 0;
            gsp_GpioStt->fgpirevtbak = 0;
            break;
    }
   SYS_EXIT_SCRT();
   
   return result;
   
}

void SYS_GPI_Event_Clear(void)
{
    gsp_GpioStt->fgpirevt = 0;
    gsp_GpioStt->fgpifevt= 0;
    gsp_GpioStt->keylevt= 0;
    gsp_GpioStt->keyrevt = 0;
    gsp_GpioStt->keyfevt = 0;
}
