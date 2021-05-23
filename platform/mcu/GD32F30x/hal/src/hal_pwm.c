/******************************Copyright(c)******************************
**                          
**
** File Name: hal_hwtimer.c
** Author: 
** Date Last Update: 2015-05-18
** Description: pwm定时器
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
** 1)etime实现pwm输出
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

    
    //信号源参数
    init_para.sig_src_para.SIG1SEL = ETIMx_ETxINSEL_SIG1SEL_GROUP1;     /* 内部信号1源选择GROUP1 */
    

    //信号源参数
    init_para.sig_src_para.GRP1SEL = ETIMx_ETxINSEL_GRP1SEL_ET1_APBCLK;     /* GROUP1 信号选择APBCLK */
    //ET1时钟使能   
    RCC_PERCLK_SetableEx( ET1CLK, ENABLE );     
    //NVIC中断配置
    NVIC_DisableIRQ(ETIM1_IRQn);
    NVIC_SetPriority(ETIM1_IRQn,2);//中断优先级配置
    NVIC_EnableIRQ(ETIM1_IRQn);     
            
    init_para.sig_src_para.PRESCALE1 = 0;           //1分频       /* ETx预分频寄存器1 */ 00表示1分频，FF表示256分频
    init_para.sig_src_para.PRESCALE2 = 0;           //1分频       /* ETx预分频寄存器2 */ 00表示1分频，FF表示256分频
    
    
    //控制类参数
    init_para.ctrl_para.EXFLT = ENABLE;                             /* 引脚输入数字滤波使能打开 */
    init_para.ctrl_para.MOD = ETIMx_ETxCR_MOD_COUNTER;              /* 工作模式选择计数模式 */
    init_para.ctrl_para.CASEN = DISABLE;                            /* 扩展定时器级联关闭 */
    init_para.ctrl_para.EDGESEL = ETIMx_ETxCR_EDGESEL_RISING;       /* 计数模式采沿方式选择（计数时钟选择mcu_clk时该位无效，总是采用mcu_clk时钟上升沿计数） */

    init_para.ctrl_para.PWM = ENABLE;                               /* PWM输出关闭 */
    init_para.ctrl_para.CAPCLR = DISABLE;                           /* 带清零捕捉模式控制 */

    init_para.ctrl_para.CMPIE = DISABLE;            /* 扩展定时器比较中断使能 */
    init_para.ctrl_para.CAPIE = DISABLE;        /* 扩展定时器捕捉中断使能 */
    init_para.ctrl_para.OVIE = ENABLE;          /* 扩展定时器溢出中断使能 */

    //10KHz PWM波形输出示例
    init_para.ctrl_para.INITVALUE = 0xFFFF - 1000;    //通过计数初值控制pwm频率;        /* ETx初值寄存器 */
    init_para.ctrl_para.CMP = 0xFFFF - 1000;         //通过比较寄存器控制占空比      /* ETx比较寄存器 */
    
    init_para.ctrl_para.CEN = DISABLE;      /* 启动控制 */
    
    //初始化ETIM
    ETIMx_Init(ETIMx, &init_para);
    
    ETIMx_ETxCR_CEN_Setable(ETIMx, ENABLE); //启动定时器   


}
#define CON_PWM_OFF     0
#define CON_PWM_ON      1
#define CON_PWM_CENTRE  2
extern void RCHF_Adj_Proc(ETIMx_Type* ETIMx, uint8_t ClkMode );
uint8_t HAL_Adj_PWM(uint32_t ledlm, int32_t adj, float fix)
{
    //RCHF温度调校函数，RCHF=8MHZ全温区+/-2%，RCHF8MHZ以上需要进行温度调校。
    //调校的前提是有比较精准的外部32K晶体
    //调校按实际需求可以采用定时（时间用户自己确定）调校;也可以采用ADC查询温度，温差大于一定值再进行调校
    //任何一路ETIM都可以 

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
        
        ETIMx_ETxCR_CEN_Setable(ETIM1, DISABLE);    //启动定时器   
        ETIMx_ETxIVR_Write(ETIM1, 0xFFFF - 1000); /* ETx初值寄存器 */
        
        ETIMx_ETxCMP_Write(ETIM1, 0xffff - (1000 - (uint16_t)val));         /* ETx比较寄存器 */
        ETIMx_ETxCR_CEN_Setable(ETIM1, ENABLE); //启动定时器   
    }
    //RCHF_Adj_Proc(ETIM4, clkmode);    
    //HAL_PWM_RCHF_ADJ();
    return 0;
}

uint8_t HAL_PWM_Test(uint32_t init, uint32_t cmp)
{

    ETIMx_ETxCR_CEN_Setable(ETIM1, DISABLE);    //启动定时器   
    ETIMx_ETxIVR_Write(ETIM1, 0xFFFF - init); /* ETx初值寄存器 */
    
    ETIMx_ETxCMP_Write(ETIM1, 0xffff - cmp);         /* ETx比较寄存器 */
    ETIMx_ETxCR_CEN_Setable(ETIM1, ENABLE); //启动定时器   
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
