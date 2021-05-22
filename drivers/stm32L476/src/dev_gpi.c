/****************************************Copyright (c)*************************
**                               	________�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: dev_gpio.h
**��   ��   ��: yzy
**����޸�����: 2010��3��10��
**��        ��: ��ͨ�������(����LED���)�˿ڹ������̺�������ͷ�ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ---------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2010��3��10��
** �衡��: ԭʼ�汾
******************************************************************************/
#define EXT_DEV_GPI

#include "aos/include.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	#include "lib_app.h"
//	#include "hal.h"
#include "drv.h"
#include "gpio.h"
#include "tasks.h"

static GPIO sGpio;
#if SYS_INT_EN > 0
extern const InterruptID* const gss_IntSID[NO_OF_INTERRUPT];
#endif

#if (SYS_LGPI_EN > 0)
//	static ktimer_t     timer_Lgpi;

/*******************************************************************************
 * @function_name:  SYS_LGPI_Scan
 * @function_file:  gpi.c
 * @����: ��������˿�ɨ���¼�
 * 
 * 
 * @����: 
 * @param: pdata(void*): 
 * 
 * @����: 
 * @return:  Boolean   
 * @����: yzy (2010-03-10)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
//	Boolean SYS_LGPI_Scan(void* pdata)
extern gpio_dev_t brd_gpi_table[];
void SYS_LGPI_Scan(void *timer, void *arg)
{
    uint8   uc_i;
    gpio_dev_t* lp;
    uint32_t val = 0;
#if LGPI_KEY_NUM > 0
//	    uint8 tkid;
//	
//	    extern const KTaskDeclare __TKDeclare[SYS_TK_NUM];
//	    const KTaskDeclare* dec = __TKDeclare;
//	    
//	    for(tkid = 0; tkid < SYS_TK_NUM; tkid++)
//	    {
//	        if(guc_MsgApplied[tkid] & Bit_Map8[MSG_CLS_GPIO])
//	        {
//	            break;
//	        }
//	    }
//	    if(tkid >= SYS_TK_NUM)
//	        return;
    //���������״̬ɨ��(��ȥ�����ܼ����������ж�)
    for(uc_i = 0; uc_i < LGPI_KEY_NUM; uc_i++)
    {
        lp = (brd_gpi_table + uc_i);
                                                //��ǰ�ߵ�ƽ.�ж��½��¼�
//	        if(lp->pullup)
//	        {
//	            Chip_GPIO_SetPinState(lp->pingrp, lp->pinnum, true);
//	            sleep_ms(100);
//	        }
        hal_gpio_input_get(lp, &val);

        if(gsp_GpioStt->gpistt & (0x01 << uc_i))
        {
            //uint8 p = Chip_GPIO_GetMaskedPortValue(lp->port->pingrp);
 
            if(val == 0)//��
            //if(!((p >> lp->port->pinnum) & 1))
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
            if(val == GPIO_PIN_SET)//��
            {
                gsp_GpioStt->cnt[uc_i]++;       //�������ۼ�
            }
            else                                //�͵�ƽ
            {
                gsp_GpioStt->cnt[uc_i] = 0;     //����������
                if(gsp_GpioStt->lastcnt[uc_i] <= SYS_LGPI_LAST)
                {
                    gsp_GpioStt->lastcnt[uc_i]++;   //�͵�ƽ״̬�°������ּ������ۼ�
                }
            }
        }
//	        if(lp->pullup)
//	        {
//	            Chip_GPIO_SetPinState(lp->pingrp, lp->pinnum, false);
//	        }
        
                                                //�����¼�
        if(gsp_GpioStt->cnt[uc_i] >= SYS_LGPI_GATE)
        {
            CPSR_ALLOC();
            RHINO_CPU_INTRPT_DISABLE();        
            //�½����¼�
            gsp_GpioStt->keyfevt |= gsp_GpioStt->gpistt & (0x01 << uc_i);
            gsp_GpioStt->gpistt ^= (0x01 << uc_i);//״̬ȡ��
            gsp_GpioStt->keyrevt |= gsp_GpioStt->gpistt & (0x01 << uc_i);//�������¼�
            RHINO_CPU_INTRPT_ENABLE();

            gsp_GpioStt->cnt[uc_i] = 0;         //����������
            
        }
                                                //���������¼��ж�
        if(gsp_GpioStt->lastcnt[uc_i] == SYS_LGPI_LAST)
        {
//	            gsp_GpioStt->lastcnt[uc_i] = SYS_LGPI_LAST - SYS_LGPI_GATE;
            gsp_GpioStt->keylevt |= (0x01 << uc_i);
        }
    }
#endif
    
    //��ͨGPI����˿�ɨ��(��ȥ������)
//	    for(uc_i = LGPI_KEY_NUM; uc_i < LGPI_PORT_NUM; uc_i++)
//	    {
//	        lp = (GPI_PORTS*)(gs_LGPIPort + uc_i);
//	
//	        if(lp->pullup)
//	        {
//	            Chip_GPIO_SetPinState(lp->pingrp, lp->pinnum, true);
//	        }
//	        
//	        if(Chip_GPIO_GetPinState(lp->port->pingrp, lp->port->pinnum) == true)//�ߵ�ƽ
//	        {
//	            gsp_GpioStt->gpistt |= (0x01 << uc_i);
//	        }
//	        else                                    //�͵�ƽ
//	        {
//	            gsp_GpioStt->gpistt &= ~(0x01 << uc_i);
//	        }
//	
//	        if(lp->pullup)
//	        {
//	            Chip_GPIO_SetPinState(lp->pingrp, lp->pinnum, false);
//	        }
//	    }
    
    //return true;
}
#endif




#if (SYS_FGPI_EN > 0)
//	static ktimer_t     timer_Fgpi;

/*******************************************************************************
 * @function_name:  SYS_FGPI_Scan
 * @function_file:  dev_gpio.c
 * @����: ��������˿�ɨ�账��,��Ҫ��ͣ������ָʾ�˿�
 * 
 * @����: 
 * 
 * @����: 
 * @return:  Boolean   
 * @����: yzy (2010-03-12)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
//	Boolean SYS_FGPI_Scan(void* pdata)
void SYS_FGPI_Scan(void *timer, void *arg)

{
    uint8   uc_i;
    GPI_PORTS* lp;
    
    for(uc_i = 0; uc_i < FGPI_PORT_NUM; uc_i++)
    {
        lp = (GPI_PORTS*)(gs_FGPIPort + uc_i);
        
        if(lp->pullup)
        {
            Chip_GPIO_SetPinState(lp->pingrp, lp->pinnum, true);
        }
                                                //��ǰ�ߵ�ƽ.�ж��½��¼�
        if(gsp_GpioStt->fgpistt & (0x01 << uc_i))
        {
            if(Chip_GPIO_GetPinState(lp->port->pingrp, lp->port->pinnum) == false)//��
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
            if(Chip_GPIO_GetPinState(lp->port->pingrp, lp->port->pinnum) == true)//��
            {
                gsp_GpioStt->fcnt[uc_i]++;      //�������ۼ�
            }
            else                                //�͵�ƽ
            {
                gsp_GpioStt->fcnt[uc_i] = 0;    //����������
            }
        }
        if(lp->pullup)
        {
            Chip_GPIO_SetPinState(lp->pingrp, lp->pinnum, false);
        }
        
                                                //�����¼�
        if(gsp_GpioStt->fcnt[uc_i] >= SYS_FGPI_GATE)
        {                                       //�½����¼�
            gsp_GpioStt->fgpifevt |= gsp_GpioStt->fgpistt & (0x01 << uc_i);
            gsp_GpioStt->fgpistt ^= (0x01 << uc_i);//״̬ȡ��
            gsp_GpioStt->fgpirevt |= gsp_GpioStt->fgpistt & (0x01 << uc_i);//�������¼�
            gsp_GpioStt->fcnt[uc_i] = 0;           //����������
        }
    }
}
#endif

#if (SYS_HALL_EN > 0)
SCallback * g_func_hall = NULL;

static void SYS_HALL_cb(void *timer, void *arg)
{
    InterruptID * id = (InterruptID *)arg;

//	    if(id->hallch == 0)
//	    {
    if(g_func_hall != NULL)
    {
        //intHallCallback(id->hallch);
        ((SCallback)g_func_hall)(id->hallch);
    }
//	    }
//	    else if(id->hallch == 1)
//	    {
//	        intHallCallback(2);
//	    }
}
#endif
/*******************************************************************************
 * @function_name:  SYS_GPI_Init
 * @function_file:  gpi.c
 * @����: 
 * 
 * @����: 
 * @����: 
 * @����: yzy (2010-03-10)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_GPI_Init(void)
{
#if ((SYS_LGPI_EN > 0) || (SYS_FGPI_EN > 0))
    gpio_dev_t* lp;
#endif
    uint8 uc_i = 0;
    uint32_t val = 0;

                                            //���뻺��
    gsp_GpioStt = &sGpio;
    ClearBuffer((uint8*)gsp_GpioStt, sizeof(GPIO));
    memset(&gsp_GpioStt->gpistt, 0xFF, sizeof(gsp_GpioStt->gpistt));//����GPI�˿ڳ�ʼ״̬
    memset(&gsp_GpioStt->fgpistt, 0xFF, sizeof(gsp_GpioStt->fgpistt));//����GPI�˿ڳ�ʼ״̬
//	    gsp_GpioStt->gpistt = 0xFFFFFFFF;       //����GPI�˿ڳ�ʼ״̬
//	    gsp_GpioStt->fgpistt = 0xFFFFFFFF;      //����GPI�˿ڳ�ʼ״̬
    
#if (SYS_FGPI_EN > 0)
                                            //��ʼ��Ϊ����״̬
    for(uint8 uc_i = 0; uc_i < FGPI_PORT_NUM; uc_i++)
    {
        lp = (GPI_PORTS*)(gs_FGPIPort + uc_i);
                                            //���ö˿ڹ���,����Ϊ����ģʽ
                                                                                
        Chip_GPIO_SetPinDigit(lp->port->pingrp, lp->port->pinnum);
        
        Chip_GPIO_SetPinPFSEG(lp->port->pingrp, lp->port->pinnum, lp->port->pinseg);
        Chip_GPIO_SetPinOutputMode(lp->port->pingrp, lp->port->pinnum, lp->port->modefunc & 0x01);
        
        Chip_GPIO_SetPinDIRInput(lp->port->gpio, lp->port->pingrp, lp->port->pinnum);
    }
                                            //�������ٶ˿�ɨ�趨ʱ��
//	    SYS_Timer_Create(SYS_FGPI_Scan, __NULL, 1, TIMER_ID_FGPI, false);
//	    krhino_timer_create(&timer_Fgpi, "timer_Fgpi", SYS_FGPI_Scan,
//	                            10, 1, 0, 1);                                            
#endif
    
#if (SYS_LGPI_EN > 0)
                                            //��ʼ��Ϊ����״̬
//	    for(uint8 uc_i = 0; uc_i < GPI_NUM; uc_i++)
//	    {
//	        lp = (brd_gpi_table + uc_i);
//	                                            //���ö˿ڹ���,����Ϊ����ģʽ
//	        Chip_GPIO_SetPinDigit(lp->port->pingrp, lp->port->pinnum);
//	        
//	        Chip_GPIO_SetPinPFSEG(lp->port->pingrp, lp->port->pinnum, lp->port->pinseg);
//	        Chip_GPIO_SetPinOutputMode(lp->port->pingrp, lp->port->pinnum, lp->port->modefunc & 0x01);
//	
//	        Chip_GPIO_SetPinDIRInput(lp->port->pingrp, lp->port->pinnum);
//	    }
                                            //�������ٶ˿�ɨ�趨ʱ��
//	    SYS_Timer_Create(SYS_LGPI_Scan, __NULL, 10, TIMER_ID_LGPI, false);
//	    krhino_timer_create(&timer_Lgpi, "timer_Lgpi", SYS_LGPI_Scan,
//	                            2, 2, 0, 1);                                            
#if LGPI_KEY_NUM > 0
    //���������״̬ɨ��(��ȥ�����ܼ����������ж�)
//	    for(uc_i = 0; uc_i < LGPI_KEY_NUM; uc_i++)
//	    {
//	        lp = (brd_gpi_table + uc_i);
//	                                                //��ǰ�ߵ�ƽ.�ж��½��¼�
//	//	        if(lp->pullup)
//	//	        {
//	//	            Chip_GPIO_SetPinState(lp->pingrp, lp->pinnum, true);
//	//	            sleep_ms(100);
//	//	        }
//	        hal_gpio_input_get(lp, &val);
//	        if(val == GPIO_PIN_RESET)//��
//	        {
//	            gsp_GpioStt->gpistt ^= (0x01 << uc_i);//״̬ȡ��
//	        }
//	
//	    }
#endif

#endif

#if (SYS_INT_EN > 0)
//	    for(uint8 uc_i = 0; uc_i < NO_OF_INTERRUPT; uc_i++)
//	    {
//	        if(gss_IntSID[uc_i]->mode == 0)
//	        {
//	            Init_Intx(gss_IntSID[uc_i], (timer_cb_t __far *)SYS_LGPI_Scan);
//	        }
//	        else if(gss_IntSID[uc_i]->mode == 1)
//	        {
//	            Init_Intx(gss_IntSID[uc_i], (timer_cb_t __far *)SYS_HALL_cb);
//	        }
//	    }
#endif    

}


//	void SYS_HALL_Init(SCallback __far * hall)
//	{
//	    g_func_hall = hall;
//	}



/*******************************************************************************
 * @function_name:  SYS_GPI_EvtMessageLoop
 * @function_file:  dev_gpio.c
 * @����: GPI�˿��¼���Ϣ�ַ�����
 * 
 * @����: 
 * @����: 
 * @����: yzy (2010-03-13)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
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
                
            }
            if(gsp_GpioStt->fgpirevt & ~gsp_GpioStt->fgpirevtbak)          //���ٶ˿�������
            {
                msg = MSG_FIREVT;
//	                SYS_Message_Send(MSG_FIREVT, tkid);
            }
            if(gsp_GpioStt->keyfevt & ~gsp_GpioStt->keyfevtbak)           //���ٶ˿��½���
            {
                msg = MSG_LIFEVT;
//	                SYS_Message_Send(MSG_LIFEVT, tkid);
            }
            if(gsp_GpioStt->keyrevt & ~gsp_GpioStt->keyrevtbak)           //���ٶ˿�������
            {
                msg = MSG_LIREVT;
//	                SYS_Message_Send(MSG_LIREVT, tkid);
            }
            if(gsp_GpioStt->keylevt & ~gsp_GpioStt->keylevtbak)           //���ٶ˿ڱ���
            {
                msg = MSG_LILEVT;
//	                SYS_Message_Send(MSG_LILEVT, tkid);
            }
            if(msg != 0xff)
            {
                krhino_buf_queue_send(dec[tkid].ktask->msg, &msg, 1);
            }
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

















/*******************************************************************************
 * @function_name:  SYS_GPI_GetStt
 * @function_file:  dev_gpio.c
 * @����: ��ȡ���������״̬
 * 
 * 
 * @����: 
 * @param: type 0:���ٿ�����״̬
 *              1:���ٿ�����״̬
 * 
 * @����: 
 * @return:  uint32   
 * @����: yzy (2018��3��22��)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
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
/*******************************************************************************
 * @function_name:  SYS_GPI_Get_PinStt
 * @function_file:  dev_gpio.c
 * @����: ��ȡ���������״̬
 * 
 * 
 * @����: 
 * @param: type 0:���ٿ�����״̬ 
 *              1:���ٿ�����״̬
 * @param: pin ����iomap.h�ĹܽŶ���
 * @����:   true �ߵ�ƽ false �͵�ƽ
 * @return:  bool   
 * @����: yzy (2018��3��22��)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
bool SYS_GPI_Get_PinStt(uint8 type, GPIENUM pin)
{
    if(type == 0)
    {
        return (bool)(gsp_GpioStt->gpistt & (1 << pin));
    }
    else
    {
        return (bool)(gsp_GpioStt->fgpistt & (1 << pin));
    }

}

/*******************************************************************************
 * @function_name:  SYS_GPI_Event
 * @function_file:  dev_gpio.c
 * @����: ��ȡ������¼�
 * 
 * 
 * @����: 
 * @param: type�¼�����,��ʽΪGPI_T
 * 
 * @����: 
 * @return:  uint16   
 * @����: yzy (2018��3��22��)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint32 SYS_GPI_Event(uint8 type)
{
    uint32 result;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    
    switch((GPI_T)type)
    {
		case GPI_TYPE_LIFEVT:              //��ȡ���ٿ��½����¼�
            result = gsp_GpioStt->keyfevt;
            gsp_GpioStt->keyfevt = 0;
            break;
        case GPI_TYPE_LIREVT:              //��ȡ���ٿ��������¼�
            result = gsp_GpioStt->keyrevt;
            gsp_GpioStt->keyrevt = 0;
            break;
        case GPI_TYPE_LILEVT:              //��ȡ���ٿڱ����¼�
            result = gsp_GpioStt->keylevt;
            gsp_GpioStt->keylevt= 0;
            break;
        case GPI_TYPE_FIFEVT:              //��ȡ���ٿ��½����¼�
            result = gsp_GpioStt->fgpifevt;
            gsp_GpioStt->fgpifevt= 0;
            break;
        case GPI_TYPE_FIREVT:              //��ȡ���ٿ��������¼�
            result = gsp_GpioStt->fgpirevt;
            gsp_GpioStt->fgpirevt = 0;
            break;
    }
    RHINO_CPU_INTRPT_ENABLE();
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


#if SYS_INT_EN > 0

/*******************************************************************************
 * @function_name:  SYS_KEY_Open
 * @function_file:  dev_gpi.c
 * @����: �򿪰����¼�
 * 
 * 
 * @����: 
 * @param: key
 * 
 * @����: 
 * @return:  uint16   
 * @����: yzy (2018-03-02)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_KEY_Open(const InterruptID* Iid)
{
    Iid->intx_Enable();
}

/*******************************************************************************
 * @function_name:  SYS_KEY_Close
 * @function_file:  dev_gpi.c
 * @����: �رհ����¼�
 * 
 * 
 * @����: 
 * @param: key
 * 
 * @����: 
 * @return:  uint16   
 * @����: yzy (2018-03-02)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_KEY_Close(const InterruptID* Iid)
{
    Iid->intx_Disable();
}

/*******************************************************************************
 * @function_name:  SYS_KEY_Close
 * @function_file:  dev_gpi.c
 * @����: �����ж�ʹ��
 * 
 * 
 * @����: 
 * @param: key
 * 
 * @����: 
 * @return:  uint16   
 * @����: yzy (2018-03-02)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_KEY_Start(void)
{
    for(uint8 uc_i = 0; uc_i < NO_OF_INTERRUPT; uc_i++)
    {
        SYS_KEY_Open((const InterruptID*)gss_IntSID[uc_i]);
    }
}
#endif