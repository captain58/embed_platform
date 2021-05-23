/******************************Copyright(c)******************************
**                          
**
** File Name: hal_hwtimer.c
** Author: 
** Date Last Update: 2015-05-18
** Description: pwm��ʱ��
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
** 1)etimeʵ��pwm���
*************************************************************************/
#define EXT_HAL_PWM

#include "sys.h"
#include "hal.h"

uint32_t g_nstandard = 0;
int32_t g_nCurPwm = 0;
void HAL_Init_PWM(uint32_t para)
{
    ETIM_InitTypeDef init_para;
    ETIMx_Type* ETIMx = ETIM1;
//	    g_nstandard = standard;
    g_nCurPwm = 0;

    
    //�ź�Դ����
    init_para.sig_src_para.SIG1SEL = ETIMx_ETxINSEL_SIG1SEL_GROUP1;     /* �ڲ��ź�1Դѡ��GROUP1 */
    

    //�ź�Դ����
    init_para.sig_src_para.GRP1SEL = ETIMx_ETxINSEL_GRP1SEL_ET1_APBCLK;     /* GROUP1 �ź�ѡ��APBCLK */
    //ET1ʱ��ʹ��   
    RCC_PERCLK_SetableEx( ET1CLK, ENABLE );     
    //NVIC�ж�����
    NVIC_DisableIRQ(ETIM1_IRQn);
    NVIC_SetPriority(ETIM1_IRQn,2);//�ж����ȼ�����
    NVIC_EnableIRQ(ETIM1_IRQn);     
            
    init_para.sig_src_para.PRESCALE1 = 0;           //1��Ƶ       /* ETxԤ��Ƶ�Ĵ���1 */ 00��ʾ1��Ƶ��FF��ʾ256��Ƶ
    init_para.sig_src_para.PRESCALE2 = 0;           //1��Ƶ       /* ETxԤ��Ƶ�Ĵ���2 */ 00��ʾ1��Ƶ��FF��ʾ256��Ƶ
    
    
    //���������
    init_para.ctrl_para.EXFLT = ENABLE;                             /* �������������˲�ʹ�ܴ� */
    init_para.ctrl_para.MOD = ETIMx_ETxCR_MOD_COUNTER;              /* ����ģʽѡ�����ģʽ */
    init_para.ctrl_para.CASEN = DISABLE;                            /* ��չ��ʱ�������ر� */
    init_para.ctrl_para.EDGESEL = ETIMx_ETxCR_EDGESEL_RISING;       /* ����ģʽ���ط�ʽѡ�񣨼���ʱ��ѡ��mcu_clkʱ��λ��Ч�����ǲ���mcu_clkʱ�������ؼ����� */

    init_para.ctrl_para.PWM = ENABLE;                               /* PWM����ر� */
    init_para.ctrl_para.CAPCLR = DISABLE;                           /* �����㲶׽ģʽ���� */

    init_para.ctrl_para.CMPIE = DISABLE;            /* ��չ��ʱ���Ƚ��ж�ʹ�� */
    init_para.ctrl_para.CAPIE = DISABLE;        /* ��չ��ʱ����׽�ж�ʹ�� */
    init_para.ctrl_para.OVIE = ENABLE;          /* ��չ��ʱ������ж�ʹ�� */

    //10KHz PWM�������ʾ��
    init_para.ctrl_para.INITVALUE = 0xFFFF - 1000;    //ͨ��������ֵ����pwmƵ��;        /* ETx��ֵ�Ĵ��� */
    init_para.ctrl_para.CMP = 0xFFFF - 1000;         //ͨ���ȽϼĴ�������ռ�ձ�      /* ETx�ȽϼĴ��� */
    
    init_para.ctrl_para.CEN = DISABLE;      /* �������� */
    
    //��ʼ��ETIM
    ETIMx_Init(ETIMx, &init_para);
    
    ETIMx_ETxCR_CEN_Setable(ETIMx, ENABLE); //������ʱ��   


}
#define CON_PWM_OFF     0
#define CON_PWM_ON      1
#define CON_PWM_CENTRE  2
extern void RCHF_Adj_Proc(ETIMx_Type* ETIMx, uint8_t ClkMode );
uint8_t HAL_Adj_PWM(uint32_t ledlm, int32_t adj, float fix)
{
    //RCHF�¶ȵ�У������RCHF=8MHZȫ����+/-2%��RCHF8MHZ������Ҫ�����¶ȵ�У��
    //��У��ǰ�����бȽϾ�׼���ⲿ32K����
    //��У��ʵ��������Բ��ö�ʱ��ʱ���û��Լ�ȷ������У;Ҳ���Բ���ADC��ѯ�¶ȣ��²����һ��ֵ�ٽ��е�У
    //�κ�һ·ETIM������ 

    int32_t val = (int32_t)((float)adj*1000/fix) ;
    val/=ledlm;
    
    
    val += g_nCurPwm;// + (int32_t)(1000 * adj / (int32_t)ledlm);

    if(val > 1000)
    {
        val = 1000;
    }
    else if(val < 0)
    {
        val = 0;
    }
        

//	    if(abs(1000 * adj / ledlm) > 1000 - g_nCurPwm )
//	    = 1000 - g_nCurPwm - (int32_t)(1000 * adj / ledlm);
    if(val >= 0 && val <= 1000 && val != g_nCurPwm)
    {
        g_nCurPwm = val;
        
        ETIMx_ETxCR_CEN_Setable(ETIM1, DISABLE);    //������ʱ��   
        ETIMx_ETxIVR_Write(ETIM1, 0xFFFF - 1000); /* ETx��ֵ�Ĵ��� */
        
        ETIMx_ETxCMP_Write(ETIM1, 0xffff - (1000 - (uint16_t)val));         /* ETx�ȽϼĴ��� */
        ETIMx_ETxCR_CEN_Setable(ETIM1, ENABLE); //������ʱ��   
    }
    //RCHF_Adj_Proc(ETIM4, clkmode);    
    //HAL_PWM_RCHF_ADJ();
    return 0;
}

uint8_t HAL_PWM_Test(uint32_t init, uint32_t cmp)
{

    ETIMx_ETxCR_CEN_Setable(ETIM1, DISABLE);    //������ʱ��   
    ETIMx_ETxIVR_Write(ETIM1, 0xFFFF - init); /* ETx��ֵ�Ĵ��� */
    
    ETIMx_ETxCMP_Write(ETIM1, 0xffff - cmp);         /* ETx�ȽϼĴ��� */
    ETIMx_ETxCR_CEN_Setable(ETIM1, ENABLE); //������ʱ��   
    RCHF_Adj_Proc(ETIM4, clkmode);  
    return 0;
}

uint8_t HAL_PWM_STS(void)
{
    switch(g_nCurPwm)
    {
    case 0:
        return CON_PWM_OFF;
    case 1000:
        return CON_PWM_ON;
    default:
        return CON_PWM_CENTRE;
    }
}

int32_t HAL_PWM_GetLm(void)
{
    return g_nCurPwm;
}


void HAL_PWM_RCHF_ADJ(void)
{
    SYS_ENTER_SCRT();
    RCHF_Adj_Proc(ETIM4, clkmode);    
    SYS_EXIT_SCRT();
}
