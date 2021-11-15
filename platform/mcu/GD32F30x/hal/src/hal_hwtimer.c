/******************************Copyright(c)******************************
**                          
**
** File Name: hal_hwtimer.c
** Author: 
** Date Last Update: 2015-05-18
** Description: ͨ��Ӳ����ʱ��
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
** 1)����TIMER32_0ʵ��4·Ӳ����ʱ��
*************************************************************************/
#define EXT_HAL_HWTIMER

#include "sys.h"
#include "hal.h"



/************************************************************************
 *��ʱclock��
 ************************************************************************/
#define CLK_HAL_HWTIMER                (SystemCoreClock)


/************************************************************************
 *��ʾ4·�Ƚ�����ʱ����֮��Ӧ�õ��õĺ���
 ************************************************************************/
THwTimerFunc gfs_hwTimerDelay[4];
void* paraHwTimerDelay[4];


/************************************************************************
 *��ʾ4·�Ƚ�����ʱʱ��,��λms
 ************************************************************************/
static uint32 gul_HwTimerDelay[4];          //�ܵĶ�ʱʱ����



/************************************************************************
 *Ĭ�ϵĿմ�����
 ************************************************************************/
void _sysTimerDoNothing(void *a)
{
    //do nothing
}



/************************************************************************
 *�����17xx����,���������
 ************************************************************************/
#define LPC_HWTIMER     LPC_TIMER32_0
#define HWTIMER_IRQn    TIMER_32_0_IRQn





/************************************************************************
 * @function: TIMER3_IRQHandler
 * @����: Timer3 ���жϴ����� 
 * @����: 
 * @����: 
 * @˵��: Timer3��������Ҫ���ڲ���һЩ��ײ�Ķ�ʱ
 * @����: yzy (2014/5/22)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
#ifndef GD32F30X_HD
    void TIMER0_TRG_CMT_TIMER10_IRQHandler(void)
#else
    void TIMER0_TRG_CMT_IRQHandler(void)
#endif
{
    /* clear TIMER interrupt flag */
    timer_interrupt_flag_clear(TIMER0,TIMER_INT_FLAG_CMT);

    krhino_intrpt_enter();
    gfs_hwTimerDelay[0](paraHwTimerDelay[0]);
    krhino_intrpt_exit();
    

}



void TIMER1_IRQHandler(void)

{
    /* clear TIMER interrupt flag */
    timer_interrupt_flag_clear(TIMER1,TIMER_INT_FLAG_CMT);

    krhino_intrpt_enter();
    gfs_hwTimerDelay[1](paraHwTimerDelay[1]);
    krhino_intrpt_exit();
    

}
/************************************************************************
 * @Function: HAL_InitSysTimer
 * @Description: ���ܳ�ʼ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/18
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void HAL_InitSysTimer(void)
{
//	    Chip_TIMER_Init(LPC_HWTIMER);            //��timer3����ģ��
//	    
//	    //Ϊ�ƿ�ϸ��,���治ʹ��nxp��
//	    LPC_HWTIMER->CTCR = 0;                   //ѡ���������Ϊ��ʱ��ģʽ
//	    LPC_HWTIMER->PR = 0;                     //ѡ��ÿ��PCLK����+1         //Chip_TIMER_PrescaleSet(0);
//	    LPC_HWTIMER->MCR = 0;                    //����4·ƥ��ͨ��
//	    LPC_HWTIMER->TCR = TIMER_RESET;          //��λ��ʹ�ܼ�����           //Chip_TIMER_Reset(LPC_HWTIMER)
//	    LPC_HWTIMER->IR = 0xFF;                  //��λ���е�T3�ж�
//	    LPC_HWTIMER->TCR = TIMER_ENABLE;         //��ʼ����                  //Chip_TIMER_Enable(LPC_HWTIMER)
    
    //��ʼ���ص������Ĵ�����
    for(uint8 uc_i = 0; uc_i < 4; uc_i++)
    {
        gfs_hwTimerDelay[uc_i] = _sysTimerDoNothing;
        paraHwTimerDelay[uc_i] = __NULL;
        gul_HwTimerDelay[uc_i] = CLK_HAL_HWTIMER;
        //SysTimer_DlyStop(uc_i);
    }
}

extern void Error_Handler(void);

static uint8 Hal_Timer_Config(uint8 tmrIdx)
{
//	    ETIM_InitTypeDef init_para;
//	    ETIMx_Type* ETIMx;
//      volatile uint08 EtimNum;
//  
//      EtimNum = ((uint32_t)ETIMx - ETIMER1_BASE)>>5;
	timer_oc_parameter_struct timer_ocintpara;
	timer_parameter_struct timer_initpara;
	timer_break_parameter_struct timer_breakpara;
    
    uint32 cycle;
    SYS_VAR_CHECK(tmrIdx >= 3);           //�жϲ�������ȷ��
    cycle = gul_HwTimerDelay[tmrIdx];    //��ȡ��ʱ���
    SYS_VAR_CHECK(cycle > 1000);           //�жϲ�������ȷ��

    //�ź�Դ����
//	    init_para.sig_src_para.SIG1SEL = ETIMx_ETxINSEL_SIG1SEL_GROUP1;     /* �ڲ��ź�1Դѡ��GROUP1 */
    
    switch(tmrIdx)
    {
        case 0:
            {
              
                 /* -----------------------------------------------------------------------
                TIMER0 configuration:
                generate 3 complementary PWM signal.
                TIMER0CLK is fixed to systemcoreclock, the TIMER0 prescaler is equal to 119 
                so the TIMER0 counter clock used is 1MHz.
                insert a dead time equal to 200/systemcoreclock =1.67us 
                configure the break feature, active at low level, and using the automatic
                output enable feature.
                use the locking parameters level 0.
                ----------------------------------------------------------------------- */

//	                rcu_periph_clock_enable(RCU_TIMER0);
//	
//	                timer_deinit(TIMER0);
//	
//	                /* TIMER0 configuration */
//	                timer_initpara.prescaler         = 119;
//	                timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
//	                timer_initpara.counterdirection  = TIMER_COUNTER_UP;
//	                timer_initpara.period            = 599;
//	                timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
//	                timer_initpara.repetitioncounter = 0;
//	                timer_init(TIMER0,&timer_initpara);
//	
//	                 /* CH0/CH0N,CH1/CH1N and CH2/CH2N configuration in timing mode */
//	                timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
//	                timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE;
//	                timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
//	                timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
//	                timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_HIGH;
//	                timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_HIGH;
//	
//	                timer_channel_output_config(TIMER0,TIMER_CH_0,&timer_ocintpara);
//	                timer_channel_output_config(TIMER0,TIMER_CH_1,&timer_ocintpara);
//	                timer_channel_output_config(TIMER0,TIMER_CH_2,&timer_ocintpara);
//	
//	                timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,299);
//	                timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_TIMING);
//	                timer_channel_output_shadow_config(TIMER0,TIMER_CH_0,TIMER_OC_SHADOW_ENABLE);
//	
//	                timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,299);
//	                timer_channel_output_mode_config(TIMER0,TIMER_CH_1,TIMER_OC_MODE_TIMING);
//	                timer_channel_output_shadow_config(TIMER0,TIMER_CH_1,TIMER_OC_SHADOW_ENABLE);
//	
//	                timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,299);
//	                timer_channel_output_mode_config(TIMER0,TIMER_CH_2,TIMER_OC_MODE_TIMING);
//	                timer_channel_output_shadow_config(TIMER0,TIMER_CH_2,TIMER_OC_SHADOW_ENABLE);
//	
//	
//	                /* automatic output enable, break, dead time and lock configuration*/
//	                timer_breakpara.runoffstate      = TIMER_ROS_STATE_ENABLE;
//	                timer_breakpara.ideloffstate     = TIMER_IOS_STATE_ENABLE ;
//	                timer_breakpara.deadtime         = 164;
//	                timer_breakpara.breakpolarity    = TIMER_BREAK_POLARITY_LOW;
//	                timer_breakpara.outputautostate  = TIMER_OUTAUTO_ENABLE;
//	                timer_breakpara.protectmode      = TIMER_CCHP_PROT_OFF;
//	                timer_breakpara.breakstate       = TIMER_BREAK_DISABLE;
//	                timer_break_config(TIMER0,&timer_breakpara);
//	                
//	                /* TIMER0 primary output function enable */
//	                timer_primary_output_config(TIMER0,ENABLE);
#ifndef GD32F30X_HD
						nvic_irq_enable(TIMER0_TRG_CMT_TIMER10_IRQn, 2, 1);
#else
						nvic_irq_enable(TIMER0_TRG_CMT_IRQn, 0, 1);
#endif
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);

//	    uint32_t temp_reg = AFIO_PCF0;
//	    temp_reg &= PCF_SWJCFG_MASK;
//	    temp_reg |= 0x04000000;
//	
//	    AFIO_PCF0 = temp_reg;
				rcu_periph_clock_enable(RCU_TIMER0);
				
				timer_deinit(TIMER0);
				
				/* TIMER0 configuration */
				timer_initpara.prescaler		 = 119;
				timer_initpara.alignedmode		 = TIMER_COUNTER_EDGE;
				timer_initpara.counterdirection  = TIMER_COUNTER_UP;
				timer_initpara.period			 = 599;
				timer_initpara.clockdivision	 = TIMER_CKDIV_DIV1;
				timer_initpara.repetitioncounter = 0;
				timer_init(TIMER0,&timer_initpara);
				
				 /* CH0/CH0N,CH1/CH1N and CH2/CH2N configuration in timing mode */
				timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
				timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE;
				timer_ocintpara.ocpolarity	 = TIMER_OC_POLARITY_HIGH;
				timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
				timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_HIGH;
				timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_HIGH;
				
				timer_channel_output_config(TIMER0,TIMER_CH_0,&timer_ocintpara);
				timer_channel_output_config(TIMER0,TIMER_CH_1,&timer_ocintpara);
				timer_channel_output_config(TIMER0,TIMER_CH_2,&timer_ocintpara);
				
				timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,299);
				timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_TIMING);
				timer_channel_output_shadow_config(TIMER0,TIMER_CH_0,TIMER_OC_SHADOW_ENABLE);
				
				timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,299);
				timer_channel_output_mode_config(TIMER0,TIMER_CH_1,TIMER_OC_MODE_TIMING);
				timer_channel_output_shadow_config(TIMER0,TIMER_CH_1,TIMER_OC_SHADOW_ENABLE);
				
				timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,299);
				timer_channel_output_mode_config(TIMER0,TIMER_CH_2,TIMER_OC_MODE_TIMING);
				timer_channel_output_shadow_config(TIMER0,TIMER_CH_2,TIMER_OC_SHADOW_ENABLE);
				
				
				/* automatic output enable, break, dead time and lock configuration*/
				timer_breakpara.runoffstate 	 = TIMER_ROS_STATE_ENABLE;
				timer_breakpara.ideloffstate	 = TIMER_IOS_STATE_ENABLE ;
				timer_breakpara.deadtime		 = 164;
				timer_breakpara.breakpolarity	 = TIMER_BREAK_POLARITY_LOW;
				timer_breakpara.outputautostate  = TIMER_OUTAUTO_ENABLE;
				timer_breakpara.protectmode 	 = TIMER_CCHP_PROT_OFF;
				timer_breakpara.breakstate		 = TIMER_BREAK_DISABLE;
				timer_break_config(TIMER0,&timer_breakpara);
				
				/* TIMER0 primary output function enable */
				timer_primary_output_config(TIMER0,ENABLE);
				    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER0);
					timer_slave_mode_select(TIMER0,TIMER_SLAVE_MODE_EVENT);
				/* TIMER0 channel control update interrupt enable */
				timer_interrupt_enable(TIMER0,TIMER_INT_CMT);
				/* TIMER0 break interrupt disable */
				timer_interrupt_disable(TIMER0,TIMER_INT_BRK);
				
				/* TIMER0 counter enable */
				timer_enable(TIMER0);

            }
            break;
        case 1:
            {
                //�ź�Դ����
                rcu_periph_clock_enable(RCU_TIMER1);

                timer_deinit(TIMER1);

                /* TIMER0 configuration */
                timer_initpara.prescaler         = 119;
                timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
                timer_initpara.counterdirection  = TIMER_COUNTER_UP;
                timer_initpara.period            = 599;
                timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
                timer_initpara.repetitioncounter = 0;
                timer_init(TIMER1,&timer_initpara);

                 /* CH0/CH0N,CH1/CH1N and CH2/CH2N configuration in timing mode */
                timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
                timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE;
                timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
                timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
                timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_HIGH;
                timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_HIGH;

                timer_channel_output_config(TIMER1,TIMER_CH_0,&timer_ocintpara);
                timer_channel_output_config(TIMER1,TIMER_CH_1,&timer_ocintpara);
                timer_channel_output_config(TIMER1,TIMER_CH_2,&timer_ocintpara);

                timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,299);
                timer_channel_output_mode_config(TIMER1,TIMER_CH_0,TIMER_OC_MODE_TIMING);
                timer_channel_output_shadow_config(TIMER1,TIMER_CH_0,TIMER_OC_SHADOW_ENABLE);

                timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_1,299);
                timer_channel_output_mode_config(TIMER1,TIMER_CH_1,TIMER_OC_MODE_TIMING);
                timer_channel_output_shadow_config(TIMER1,TIMER_CH_1,TIMER_OC_SHADOW_ENABLE);

                timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_2,299);
                timer_channel_output_mode_config(TIMER1,TIMER_CH_2,TIMER_OC_MODE_TIMING);
                timer_channel_output_shadow_config(TIMER1,TIMER_CH_2,TIMER_OC_SHADOW_ENABLE);


                /* automatic output enable, break, dead time and lock configuration*/
                timer_breakpara.runoffstate      = TIMER_ROS_STATE_ENABLE;
                timer_breakpara.ideloffstate     = TIMER_IOS_STATE_ENABLE ;
                timer_breakpara.deadtime         = 164;
                timer_breakpara.breakpolarity    = TIMER_BREAK_POLARITY_LOW;
                timer_breakpara.outputautostate  = TIMER_OUTAUTO_ENABLE;
                timer_breakpara.protectmode      = TIMER_CCHP_PROT_OFF;
                timer_breakpara.breakstate       = TIMER_BREAK_DISABLE;
                timer_break_config(TIMER1,&timer_breakpara);
                
                /* TIMER0 primary output function enable */
                timer_primary_output_config(TIMER1,ENABLE);
            }
            break;

       
        
        default:
            break;
    }

    SYS_OK();
}
/************************************************************************
 * @Function: SysTimer_DlyConfig
 * @Description: ����ϵͳ��ʱ��
 * 
 * @Arguments: 
 * @param: func ��ʾ��ǰ·�жϲ���֮����õĺ���
 * @param: fpara �жϴ������Ĵ������
 * @param: tmrIdx ��ʾ�ڼ�·
 * @param: mills ��ʾ�����ʱ�Ƕ���ms
 * 
 * @Attention: Ӧ��ģ��Ӧ����ϵͳ������ʱ����Ѿ�����Щ�Ĵ������ú�,��������ǽ��̰�ȫ,���жϰ�ȫ 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/5/18
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SysTimer_DlyConfig(THwTimerFunc func, void* fpara, uint8 tmrIdx, uint16 mills)
{
    SYS_VAR_CHECK(tmrIdx >= 4);              //�жϲ�������ȷ��
    SYS_VAR_CHECK(mills == 0);               //�жϲ�������ȷ��
    gfs_hwTimerDelay[tmrIdx] = func;        //�ص��������������
    paraHwTimerDelay[tmrIdx] = fpara;
                                            //��ʱ������
    gul_HwTimerDelay[tmrIdx] = mills;

    Hal_Timer_Config(tmrIdx);

//	    SysTimer_DlyStart(tmrIdx);
    SYS_OK();
}



/************************************************************************
 * @Function: SysTimer_DlyStart
 * @Description: ����Ӳ����ʱ��
 * 
 * @Arguments: 
 * @param: tmrIdx ��ʾ�ڼ�· 
 * 
 * @Attention: ����������ǽ��̰�ȫ��
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/5/18
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SysTimer_DlyStart(uint8 tmrIdx)
{

    switch(tmrIdx)
    {
    case 0:
        //�ź�Դ����
//        /* TIMER0 channel control update interrupt enable */
//        timer_interrupt_enable(TIMER0,TIMER_INT_CMT);
//        /* TIMER0 break interrupt disable */
//        timer_interrupt_enable(TIMER0,TIMER_INT_BRK);
//
//        /* TIMER0 counter enable */
//        timer_enable(TIMER0);
//#ifndef GD32F30X_HD
//		nvic_irq_enable(TIMER0_TRG_CMT_TIMER10_IRQn, 2, 1);
//#else
//		nvic_irq_enable(TIMER0_TRG_CMT_IRQn, 0, 1);
//#endif

        break;
    case 1:
        //�ź�Դ����
		/* TIMER1 channel control update interrupt enable */
		timer_interrupt_enable(TIMER1,TIMER_INT_CMT);
		/* TIMER1 break interrupt disable */
		timer_interrupt_disable(TIMER1,TIMER_INT_BRK);

		/* TIMER1 counter enable */
		timer_enable(TIMER1);


        break;        
    default:
   
        break;
    }
	
	SYS_OK();  
}

/************************************************************************
 * @Function: SysTimer_DlyStop
 * @Description: ֹͣӲ����ʱ��
 * 
 * @Arguments: 
 * @param: tmrIdx ��ʾ�ڼ�· 
 * 
 * @Note: ����������ǽ��̰�ȫ��
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/5/18
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SysTimer_DlyStop(uint8 tmrIdx)
{
    SYS_VAR_CHECK(tmrIdx >= 4);           //�жϲ�������ȷ��
    switch(tmrIdx)                      //���ݺ������ɢת
    {
        case 0:                         //�ر��жϹ���
            /* TIMER0 channel control update interrupt enable */
            timer_interrupt_disable(TIMER0,TIMER_INT_CMT);
            /* TIMER0 break interrupt disable */
            timer_interrupt_disable(TIMER0,TIMER_INT_BRK);

            /* TIMER0 counter enable */
            timer_disable(TIMER0);

            break;
        case 1:                         //�ر��жϹ���
			/* TIMER0 channel control update interrupt enable */
			timer_interrupt_disable(TIMER1,TIMER_INT_CMT);
			/* TIMER0 break interrupt disable */
			timer_interrupt_disable(TIMER1,TIMER_INT_BRK);

			/* TIMER0 counter enable */
			timer_disable(TIMER1);


            break;

        default:
            return SYS_ERR_FT;
//	            Chip_TIMER_MatchDisableInt(LPC_HWTIMER, 3);
//	            Chip_TIMER_StopOnMatchDisable(LPC_HWTIMER, 3);
            break;
    }
    SYS_OK();                            //����ֹͣ�ɹ�
}




/************************************************************************
 * @Function: SysTimer_GetStatus
 * @Description: ��ȡӲ����ʱ��״̬
 * 
 * @Arguments: 
 * @param: tmrIdx ��ʾ�ڼ�· 
 * 
 * @Note: ����������ǽ��̰�ȫ��
 * @Return: bool trueʹ��,false����  
 * @Auther: yzy
 * Date: 2015/5/18
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool SysTimer_GetStatus(uint8 tmrIdx)
{
    SYS_VAR_CHECK(tmrIdx >= 4);              //�жϲ�������ȷ��
//	    ETIMx_Type* ETIMx;
    uint8_t stt = 0;
    switch(tmrIdx)
    {
        case 0:
            stt = timer_flag_get(TIMER0, TIMER_INT_FLAG_CMT);
            break;
        case 1:
			stt = timer_flag_get(TIMER1, TIMER_INT_FLAG_CMT);
            break;       
        default:
            return false;
            break;
    }
    if(SET != stt)
    {
        return false;
    }
    else
    {
        return true;
    }
}




#if (true)   //����Ӳ����ʱ������


/************************************************************************
 * @����:����hwtimer������
 ************************************************************************/
TCasHwTimer* gsp_CasHwTimerLink = __NULL;

uint8 CasHwTimerID = 0xff;




/************************************************************************
 * @Function: casHwTimerCreate
 * @Description: ��������ר�ö�ʱ��(Ŀ��Ϊ�˼���Ӳ����ʱ������������)
 * 
 * @Arguments: 
 * @param: tickfunc ��ʱ������
 * @param: tickpara �������
 * @param: timerid ����Ķ�ʱ����
 * @param: dlytick ��ʱ��tick��
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/3
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 casHwTimerCreate(TCasHwTimerFunc tickfunc, void* tickpara, uint8 timerid, uint16 dlytick)
{
    TCasHwTimer* p;
    TCasHwTimer* q, *r; 

    SYS_ENTER_AllSCRT();                     //���жϺ���������Դ,���豣֤�жϰ�ȫ

    p = gsp_CasHwTimerLink;
    while(p != __NULL)                      //ѭ��ɨ�赽����timer
    {
        if(p->timerid == timerid)           //��ID��timer�Ѿ��������򷵻�
        {
            SYS_EXIT_AllSCRT();
            return SYS_ERR_EXIST;
        }
        q = p;                              //������һ��
        p = p->pnext;                       //��һ��
    }
                                            //����ֵ
    r = (TCasHwTimer*)m_malloc(sizeof(TCasHwTimer));
	r->timerid = timerid;
    r->tickfunc = tickfunc;
    r->tickpara = tickpara;
    r->timeout = dlytick;
    r->dlycnt = 0;
    r->pnext = __NULL;
    r->stoped = 1;                          //Ĭ��ֹͣ
    
    if(gsp_CasHwTimerLink == __NULL)
    {
        gsp_CasHwTimerLink = r;
    }
    else
    {
        q->pnext = r;
    }

    SYS_EXIT_AllSCRT();                      //�������
    SYS_OK();
}



/************************************************************************
 * @Function: casHwTimerStart
 * @Description: ����timer
 * 
 * @Arguments: 
 * @param: timerid 
 * 
 * @Note: 
 * @Return: TResult  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 casHwTimerStart(uint8 timerid)
{
    TCasHwTimer* p;

    SYS_VAR_CHECK(CasHwTimerID >= 4);
    SYS_ENTER_AllSCRT();                     //���жϺ���������Դ,���豣֤�жϰ�ȫ

    p = gsp_CasHwTimerLink;
    while(p != __NULL)                      //ѭ��ɨ�赽����timer
    {
        if(p->timerid == timerid)           //��ID��timer�Ѿ��������򷵻�
        {
            p->stoped = 0;                  //����          
            p->dlycnt = p->timeout + 1;     //��ֹ��ʱ����;ֹͣ����������ʱ,dlyctr ֵ������ timeout.
            
            if(!SysTimer_GetStatus(CasHwTimerID))
            {
                SysTimer_DlyStart(CasHwTimerID);
            }
            SYS_EXIT_AllSCRT();
            return SYS_ERR_OK;
        }
        
        p = p->pnext;                       //��һ��
    }
    SYS_EXIT_AllSCRT();
    return SYS_ERR_EXIST;                    //������ͼ���������ڵ�timer
}


/************************************************************************
 * @Function: casHwTimerStop
 * @Description: ֹͣtimer
 * 
 * @Arguments: 
 * @param: timerid 
 * 
 * @Note: 
 * @Return: TResult  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 casHwTimerStop(uint8 timerid)
{
    TCasHwTimer* p;
    uint8 ret = SYS_ERR_EXIST;
    bool _allstop = true;                   //�Ƿ����ж�ʱ����ֹͣ
    
    SYS_VAR_CHECK(CasHwTimerID >= 4);
    
    SYS_ENTER_AllSCRT();                     //���жϺ���������Դ,���豣֤�жϰ�ȫ
    p = gsp_CasHwTimerLink;
    
    while(p != __NULL)                      //ѭ��ɨ�赽����timer
    {
        if(p->timerid == timerid)           //��ID��timer�Ѿ��������򷵻�
        {
            p->stoped = 1;                  //ֹͣ
            ret = SYS_ERR_OK;                //ִ�гɹ�
        }
        
        if(p->stoped != 1)                  //�ж��Ƿ����ж�ʱ����ֹͣ
        {
            _allstop = false;
        }
        
        p = p->pnext;                       //��һ��
    }
    
    if(_allstop)
    {
        SysTimer_DlyStop(CasHwTimerID);
    }
    SYS_EXIT_AllSCRT();
    
    return ret;                             //������ͼ���������ڵ�timer
}

uint8 casHwTimerClose(uint8 timerid)
{
    TCasHwTimer* p;
    TCasHwTimer* q;
    uint8 ret = SYS_ERR_EXIST;
    bool _allstop = true;                   //�Ƿ����ж�ʱ����ֹͣ
    uint8_t flg = 0;
    SYS_VAR_CHECK(CasHwTimerID >= 4);
    
    SYS_ENTER_AllSCRT();                     //���жϺ���������Դ,���豣֤�жϰ�ȫ
    p = gsp_CasHwTimerLink;
    
    while(p != __NULL)                      //ѭ��ɨ�赽����timer
    {
        flg = 0;
        if(p->timerid == timerid)           //��ID��timer�Ѿ��������򷵻�
        {
            p->stoped = 1;                  //ֹͣ
            ret = SYS_ERR_OK;                //ִ�гɹ�
            flg = 1;
        }
        
        if(p->stoped != 1)                  //�ж��Ƿ����ж�ʱ����ֹͣ
        {
            _allstop = false;
        }
        q=p;
        p = p->pnext;                       //��һ��
        if(flg)
        {
//#ifndef __MODULE__    
            m_free(q);
//#else
//            FnSFree(q);
//#endif    
            q=NULL;
        }
    }
    
    if(_allstop)
    {
        SysTimer_DlyStop(CasHwTimerID);
    }
    SYS_EXIT_AllSCRT();
    
    return ret;         


}

/************************************************************************
 * @Function: _casHwTimerTickProc
 * @Description: ��ʱ�жϴ���
 * 
 * @Arguments: 
 * @param: para 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/6/2
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void _casHwTimerTickProc(void* para)
{
    TCasHwTimer* p = gsp_CasHwTimerLink;

    if(p != __NULL)
    { 
        while(p != __NULL)                  //����ѭ���������timer
        {
            if(p->stoped == 0)              //�ж��Ƿ�����
            {
                p->dlycnt--;                //����ʱ

                if(p->dlycnt == 0)          //��0�򴥷�
                {
                    p->dlycnt = p->timeout; //timer�ָ�Ȼ����ûص�����
                    p->tickfunc(p->tickpara);
                }
            }
            p = p->pnext;                   //��һ��
        }
    }
}




/************************************************************************
 * @Function: casHwTimerConfig
 * @Description: ����
 * 
 * @Arguments: 
 * @param: sysTimerID �󶨵�systimerID
 * @param: tickMills ���ζ�ʱ�жϵĺ�����
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/3
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 casHwTimerConfig(uint8 sysTimerID, uint16 tickMills)
{
    uint8 ret;
    SYS_VAR_CHECK(sysTimerID >= 4);
    
    SYS_ENTER_AllSCRT();                     //���жϺ���������Դ,���豣֤�жϰ�ȫ
    CasHwTimerID = sysTimerID;
    ret = SysTimer_DlyConfig(_casHwTimerTickProc, __NULL, sysTimerID, tickMills);
    SYS_EXIT_AllSCRT();
    
    return ret;
}


#endif          //#if (true)   //����Ӳ����ʱ������

//	void RCHF_Adj_EtimxCfg(ETIMx_Type* ETIMx)
//	{
//		ETIM_InitTypeDef init_para;
//		volatile uint8_t EtimNum;
//	
//		EtimNum = ((uint32_t)ETIMx - ETIMER1_BASE)>>5;
//		
//		//�ź�Դ����
//		init_para.sig_src_para.SIG1SEL = ETIMx_ETxINSEL_SIG1SEL_GROUP1;		/* �ڲ��ź�1Դѡ��GROUP1 */
//		init_para.sig_src_para.SIG2SEL = ETIMx_ETxINSEL_SIG2SEL_GROUP2;		/* �ڲ��ź�2Դѡ��GROUP2 */
//		
//		switch(EtimNum)
//		{
//			case 0:	//ETIM1
//				//�ź�Դ����
//				init_para.sig_src_para.GRP1SEL = ETIMx_ETxINSEL_GRP1SEL_ET1_APBCLK;		/* GROUP1 �ź�ѡ��APBCLK */
//				init_para.sig_src_para.GRP2SEL = ETIMx_ETxINSEL_GRP2SEL_ET1_XTLF;		/* GROUP2 �ź�ѡ��XTLF */			
//				//ET1ʱ��ʹ��	
//				RCC_PERCLK_SetableEx( ET1CLK, ENABLE );		
//				//NVIC�ж�����
//				NVIC_DisableIRQ(ETIM1_IRQn);	
//				break;
//	
//			case 1:	//ETIM2
//				//�ź�Դ����
//				init_para.sig_src_para.GRP1SEL = ETIMx_ETxINSEL_GRP1SEL_ET2_APBCLK;		/* GROUP1 �ź�ѡ��APBCLK */
//				init_para.sig_src_para.GRP2SEL = ETIMx_ETxINSEL_GRP2SEL_ET2_XTLF;		/* GROUP2 �ź�ѡ��XTLF */		
//				//ET2ʱ��ʹ��	
//				RCC_PERCLK_SetableEx( ET2CLK, ENABLE );	
//				//NVIC�ж�����
//				NVIC_DisableIRQ(ETIM2_IRQn);			
//				break;
//	
//			case 2:	//ETIM3
//				//�ź�Դ����
//				init_para.sig_src_para.GRP1SEL = ETIMx_ETxINSEL_GRP1SEL_ET3_APBCLK;		/* GROUP1 �ź�ѡ��APBCLK */
//				init_para.sig_src_para.GRP2SEL = ETIMx_ETxINSEL_GRP2SEL_ET3_XTLF;		/* GROUP2 �ź�ѡ��XTLF */		
//				//ET3ʱ��ʹ��
//				RCC_PERCLK_SetableEx( ET3CLK, ENABLE );	
//				//NVIC�ж�����
//				NVIC_DisableIRQ(ETIM3_IRQn);		
//				break;
//	
//			case 3:	//ETIM4
//				//�ź�Դ����
//				init_para.sig_src_para.GRP1SEL = ETIMx_ETxINSEL_GRP1SEL_ET4_APBCLK;		/* GROUP1 �ź�ѡ��APBCLK */
//				init_para.sig_src_para.GRP2SEL = ETIMx_ETxINSEL_GRP2SEL_ET4_XTLF;		/* GROUP2 �ź�ѡ��XTLF */		
//				//ET4ʱ��ʹ��
//				RCC_PERCLK_SetableEx( ET4CLK, ENABLE );		
//				//NVIC�ж�����
//				NVIC_DisableIRQ(ETIM4_IRQn);
//				break;
//			
//			default:
//				break;
//		}
//		init_para.sig_src_para.PRESCALE1 = 0;    		//1��Ƶ		/* ETxԤ��Ƶ�Ĵ���1 */ 00��ʾ1��Ƶ��FF��ʾ256��Ƶ
//		init_para.sig_src_para.PRESCALE2 = 127;    		//128��Ƶ		/* ETxԤ��Ƶ�Ĵ���2 */ 00��ʾ1��Ƶ��FF��ʾ256��Ƶ
//		
//		//���������
//		init_para.ctrl_para.EXFLT = ENABLE;								/* �������������˲�ʹ�ܴ� */
//		init_para.ctrl_para.MOD = ETIMx_ETxCR_MOD_CAPTURE;				/* ����ģʽѡ��׽ģʽ */
//		init_para.ctrl_para.CASEN = DISABLE;							/* ��չ��ʱ�������ر� */
//		init_para.ctrl_para.EDGESEL = ETIMx_ETxCR_EDGESEL_RISING;		/* ����ģʽ���ط�ʽѡ�񣨼���ʱ��ѡ��mcu_clkʱ��λ��Ч�����ǲ���mcu_clkʱ�������ؼ����� */
//	
//		init_para.ctrl_para.PWM = DISABLE;								/* PWM����ر� */
//	
//		init_para.ctrl_para.CAPMOD = ETIMx_ETxCR_CAPMOD_PERIOD;			/* ��׽ģʽ���� */
//		init_para.ctrl_para.CAPCLR = ENABLE;							/* �����㲶׽ģʽ���� *///ʹ�ܺ����������0����׽����һ����Ч��֮��timer�ſ�ʼ������֮��Ĳ�׽���������㣬ֻ�йرն�ʱ�����ٴ�
//		init_para.ctrl_para.CAPONCE = ENABLE;							/* ���β�׽���� */
//		init_para.ctrl_para.CAPEDGE = ETIMx_ETxCR_CAPEDGE_RISING;		/* ��׽��ѡ�� */
//	
//		init_para.ctrl_para.CMPIE = DISABLE;		/* ��չ��ʱ���Ƚ��ж�ʹ�� */
//		init_para.ctrl_para.CAPIE = DISABLE;		/* ��չ��ʱ����׽�ж�ʹ�� */
//		init_para.ctrl_para.OVIE = DISABLE;			/* ��չ��ʱ������ж�ʹ�� */
//	
//		init_para.ctrl_para.INITVALUE = 0; 		//������ֵ;		/* ETx��ֵ�Ĵ��� */
//		init_para.ctrl_para.CMP = 0;			/* ETx�ȽϼĴ��� */
//		
//		init_para.ctrl_para.CEN = DISABLE;		/* �������� */
//		
//		//��ʼ��ETIM
//		ETIMx_Init(ETIMx, &init_para);
//	}
//	
//	//xtlf��32768Hzʱ����Ϊ�ο�Դ
//	//��������������APB����Ƶ�����
//	//ClkMode 1 = 8M
//	//ClkMode 2 = 16M
//	//ClkMode 3 = 24M
//	//ClkMode 4 = 32M
//	void RCHF_Adj_Proc(ETIMx_Type* ETIMx, uint8_t ClkMode )
//	{
//		volatile uint16 Temp16;
//		unsigned char n,m,tempbyte,TrimBuf;	
//		float DIVclkmode = 1.0;
//		
//		//FDETO	ͣ����ģ�����
//		//1��XTLFδͣ��
//		//0��XTLFͣ��
//		if( RESET == ANAC_FDETIF_FDETO_Chk())//xtlfͣ��
//		{
//			//ʹ�ò��Բ���		
//			RCC_Init_RCHF_Trim(ClkMode);
//			return;
//		}
//		
//		RCHF_Adj_EtimxCfg(ETIMx);
//	
//		switch(ClkMode)
//		{
//			case 1://8
//				ETIMx_ETxPESCALE1_Write(ETIMx, 0);//����Դ1��Ƶ
//				DIVclkmode		= 1.0;
//				break;
//			
//			case 2://16/2
//				ETIMx_ETxPESCALE1_Write(ETIMx, 1);//����Դ2��Ƶ
//				DIVclkmode		= 1.0;
//				break;
//			
//			case 3://24/4
//				ETIMx_ETxPESCALE1_Write(ETIMx, 3);//����Դ4��Ƶ
//				DIVclkmode		= 8.0/6.0;
//				break;
//			
//			case 4://36/4
//				ETIMx_ETxPESCALE1_Write(ETIMx, 3);//����Դ4��Ƶ
//				DIVclkmode		= 8.0/9.0;
//				break;
//			
//			default://8
//				ETIMx_ETxPESCALE1_Write(ETIMx, 0);//����Դ����Ƶ
//				DIVclkmode		= 1.0;
//				break;	
//		}	
//			
//		for( m=0; m<6; m++ )
//		{			
//			ETIMx_ETxCR_CEN_Setable(ETIMx, ENABLE);	//������ʱ��
//			
//			for( n=0; n<2; n++ ) //�������ж���Ҫ3.906ms = 128/32768 s
//			{		
//				Temp16 = 0;
//				Do_DelayStart();
//				{
//					if( SET == ETIMx_ETxIF_CAPIF_Chk(ETIMx) ) {Temp16 = 1; break;}//�ȴ���׽�жϱ�־
//				}While_DelayMsEnd(7.5*clkmode);		//7.5ms
//				ETIMx_ETxIF_CAPIF_Clr(ETIMx);		//�����׽�жϱ�־
//			}	
//			ETIMx_ETxCR_CEN_Setable(ETIMx, DISABLE);	//�رն�ʱ��
//			if( Temp16 == 0 ) continue;
//				
//			Temp16 = ETIMx_ETxIVR_Read(ETIMx);		//��ȡ��׽ֵ
//	        if( Temp16 == 0 ) continue;
//			if( (Temp16>(31250-120)/DIVclkmode)&&(Temp16<(31250+120)/DIVclkmode) ) //return;//����������ֱֵ�ӷ���
//			{
//				break;
//			}
//			else
//			{
//				TrimBuf = RCC_RCHFTRIM_Read();	//��ȡRCHFУ׼ֵ
//				if( Temp16 > 31250/DIVclkmode )	//31250 = 8M*128/32768; 156.25 = (15625*2/4*2M)*40k
//				{
//					Temp16 -= 31250/DIVclkmode;
//					tempbyte = (uint8_t)((float)Temp16/(156.25/DIVclkmode)+0.5);
//					
//					if( tempbyte > 0x20 ) tempbyte = 0x20;
//					
//					if( TrimBuf >= tempbyte )
//					{
//						RCC_RCHFTRIM_Write(TrimBuf - tempbyte);//����RCHFУ׼ֵ
//					}
//					else
//					{
//						RCC_RCHFTRIM_Write(0x00);//����RCHFУ׼ֵ
//					}			
//				}
//				else
//				{
//					Temp16 = (uint16_t)(31250/DIVclkmode-Temp16);
//					tempbyte = (uint8_t)((float)Temp16/(156.25/DIVclkmode)+0.5);
//					
//					if( tempbyte > 0x20 ) tempbyte = 0x20;
//					
//					if( (TrimBuf + tempbyte) <= 0x7F)
//					{
//						RCC_RCHFTRIM_Write(TrimBuf + tempbyte);//����RCHFУ׼ֵ
//					}
//					else
//					{
//						RCC_RCHFTRIM_Write(0x7F);//����RCHFУ׼ֵ
//					}
//				}
//				
//				if( m == 5 )				//δ�ҵ�����ֵ,�������2�ε�ֵȡƽ��
//				{
//					TrimBuf += RCC_RCHFTRIM_Read();
//					RCC_RCHFTRIM_Write((uint32_t)(TrimBuf/2.0+0.5));				
//				}
//			}
//		}
//	}


