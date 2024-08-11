/******************************Copyright(c)******************************
**                          
**
** File Name: dev_lowpwr.c
** Author: 
** Date Last Update: 2019-11-02
** Description: 低功耗
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#define EXT_DEV_LOWPWR
#ifdef __MODULE__
#include "WOSsys.h"
#endif
#include "sys.h"
#include "hal.h"
#include "bsp.h"
#include <intrinsics.h>
#include "core_cmInstr.h"
#include "Dev_disp.h"
//#include "log.h"
/*******************************************************************************
 * @function_name:  SYS_Pwr_ChkUnSleep
 * @function_file:  dev_lowpwr.c
 * @描述: 低功耗管脚检测
 * 
 * @参数: 
 * 
 * @返回: 
 * @return:  0:不进去低功耗   
 * @作者: yzy (2015-01-19)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8_t SYS_Pwr_ChkUnSleep()
{
//	    uint32_t gpistt = SYS_GPI_GetStt(0);
    uint8_t stt = HAL_GPIO_GetPinState(LOWPWR_UNSLEEP_PORT, LOWPWR_UNSLEEP_PIN);
    if(stt == Bit_SET || stt == SYS_ERR_UNKNOW)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}


/**
 * @brief MCU休眠
 */
void _lp_sleep(void)
{
	PMU_SleepCfg_InitTypeDef SleepCfg_InitStruct;
//		SleepCfg_InitStruct.PMOD = PMU_LPMCFG_PMOD_SLEEP;			//功耗模式配置
//		SleepCfg_InitStruct.SLPDP = PMU_LPMCFG_SLPDP_SLEEP;			//DeepSleep控制
//		//SleepCfg_InitStruct.DSLPRAM_EXT = PMU_LPMCFG_DSLPRAM_EXT_ALLRAMON;//DeepSleep模式下RAM数据保持配置
//		SleepCfg_InitStruct.CVS = DISABLE;							//内核电压降低控制
//		SleepCfg_InitStruct.XTOFF = PMU_LPMCFG_XTOFF_DIS;			//关闭XTLF，仅在SLEEP/DEEPSLEEP下起作用	
//		SleepCfg_InitStruct.SCR = 0;								//M0系统控制寄存器，一般配置为0即可	

    /*下电复位配置*/
    //pdr和bor两个下电复位至少要打开一个
    //当电源电压低于下电复位电压时，芯片会被复位住        
    //pdr电压档位不准但是功耗极低（几乎无法测量）
    //bor电压档位准确但是需要增加2uA功耗
    ANAC_PDRCON_PDREN_Setable(ENABLE);      //打开PDR
    ANAC_BORCON_OFF_BOR_Setable(ENABLE);    //关闭BOR 3uA
    
//	    RCC_SYSCLKSEL_LPM_RCLP_OFF_Setable(ENABLE); //关闭rclp 0.2uA


	SleepCfg_InitStruct.PMOD		= PMU_LPMCFG_PMOD_SLEEP;			//功耗模式配置
	SleepCfg_InitStruct.SLPDP		= PMU_LPMCFG_SLPDP_DEEPSLEEP;		//DeepSleep控制
	SleepCfg_InitStruct.DSLPRAM_EXT = PMU_LPMCFG_DSLPRAM_EXT_ALLRAMON;	//DeepSleep模式下RAM数据保持配置
	SleepCfg_InitStruct.CVS			= DISABLE;							//内核电压降低控制
	SleepCfg_InitStruct.XTOFF		= PMU_LPMCFG_XTOFF_DIS;				//XTLF控制，仅在SLEEP/DEEPSLEEP下起作用	
	SleepCfg_InitStruct.SCR			= 0;								//M0系统控制寄存器，一般配置为0即可	

//    AltFunIO( GPIOG, GPIO_Pin_8, ALTFUN_PULLUP );   //PG8;//SWDTCK
//    AltFunIO( GPIOG, GPIO_Pin_9, ALTFUN_PULLUP );   //PG9;//SWDTDO  
	IWDT_Clr();	

	PMU_SleepCfg_Init(&SleepCfg_InitStruct);//休眠配置
	__WFI();//进入休眠
	IWDT_Clr();
}


#ifdef __MODULE__
uint8_t SYS_Pwr_SleepCfg()
{
//关闭串口
    SYS_UART_Close(UART_SETP_PORT);
    SYS_UART_Close(UART_FARP_PORT);
    SYS_UART_Close(UART_IRDA_PORT);

//io配置
    HAL_GPIO_PinConfig(GPIOF, 11, 3, 1, 0);

    SYS_BlinkDev_Idel();
    SYS_GPO_Idel();
    SYS_AD_Idel();
    
//	    HAL_LcdOff();
    
    return 0;
}

uint8_t SYS_Pwr_WakeUpCfg()
{
//开启串口
	    SYS_UART_Open(UART_SETP_PORT);
//	    SYS_UART_Open(UART_FARP_PORT);
//	    SYS_UART_Open(UART_IRDA_PORT);
//    SerialSets ss;
//    ss.baudrate = 9600;
//    ss.parit = Parit_N;
//    ss.databits = DataBits_8bits;
//    ss.stopbits = StopBits_1;
//    SYS_SER_Init(SETP_PORT_NO, &ss);

//io配置
//	    SYS_BlinkDev_WakeUp();
    SYS_GPO_WakeUp();
    SYS_GPI_WakeUp();
    Init_SVD();
    SysTimer_DlyStart(5);

    return 0;
}
#else
#ifdef USR_DEBUG
__no_init u32 test1[256];
__no_init u8 test1i;
#endif

void SYS_Pwr_Sleep()
{
#ifdef LP_DEBUG
#include "log.h"
    extern uint8_t SYS_Chk_LPTaskStack(void);
    if(SYS_Pwr_ChkUnSleep() && 0 == SYS_Chk_LPTaskStack()/*&& 0 == SMskTmStt()*/)
    {
        LOG_INFO("go to sleep !!!\n");
    
        //MTmrIdle();

        SYS_E2_Idel();
        SYS_Pwr_SleepCfg();
        _lp_sleep();

#ifdef USR_DEBUG 
          test1[test1i ++] = *(uint32_t *)(0x40000108);
#endif  

        
        SYS_Pwr_WakeUpCfg();
        //MTmrInit();
        extern uint8_t SMskRebuildTmHandle(void);
        SMskRebuildTmHandle();
        SYS_E2_Init();
        LOG_DEBUG("i'am wake up !!!\n");
    }
    else
    {
        LOG_INFO("it's not sleep time, go to work!!!\n");
    
        //MTmrIdle();

//	        SYS_E2_Idel();
//	        SYS_Pwr_SleepCfg();
        msleep(500);
//	        SYS_Pwr_WakeUpCfg();
//	        //MTmrInit();
//	//          SYS_E2_Init();
//	        LOG_DEBUG("i'am wake up !!!\n");
        
    }
#endif                        
}

#endif



