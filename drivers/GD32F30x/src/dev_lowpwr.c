/******************************Copyright(c)******************************
**                          
**
** File Name: dev_lowpwr.c
** Author: 
** Date Last Update: 2019-11-02
** Description: �͹���
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
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
 * @����: �͹��Ĺܽż��
 * 
 * @����: 
 * 
 * @����: 
 * @return:  0:����ȥ�͹���   
 * @����: yzy (2015-01-19)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
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
 * @brief MCU����
 */
void _lp_sleep(void)
{
	PMU_SleepCfg_InitTypeDef SleepCfg_InitStruct;
//		SleepCfg_InitStruct.PMOD = PMU_LPMCFG_PMOD_SLEEP;			//����ģʽ����
//		SleepCfg_InitStruct.SLPDP = PMU_LPMCFG_SLPDP_SLEEP;			//DeepSleep����
//		//SleepCfg_InitStruct.DSLPRAM_EXT = PMU_LPMCFG_DSLPRAM_EXT_ALLRAMON;//DeepSleepģʽ��RAM���ݱ�������
//		SleepCfg_InitStruct.CVS = DISABLE;							//�ں˵�ѹ���Ϳ���
//		SleepCfg_InitStruct.XTOFF = PMU_LPMCFG_XTOFF_DIS;			//�ر�XTLF������SLEEP/DEEPSLEEP��������	
//		SleepCfg_InitStruct.SCR = 0;								//M0ϵͳ���ƼĴ�����һ������Ϊ0����	

    /*�µ縴λ����*/
    //pdr��bor�����µ縴λ����Ҫ��һ��
    //����Դ��ѹ�����µ縴λ��ѹʱ��оƬ�ᱻ��λס        
    //pdr��ѹ��λ��׼���ǹ��ļ��ͣ������޷�������
    //bor��ѹ��λ׼ȷ������Ҫ����2uA����
    ANAC_PDRCON_PDREN_Setable(ENABLE);      //��PDR
    ANAC_BORCON_OFF_BOR_Setable(ENABLE);    //�ر�BOR 3uA
    
//	    RCC_SYSCLKSEL_LPM_RCLP_OFF_Setable(ENABLE); //�ر�rclp 0.2uA


	SleepCfg_InitStruct.PMOD		= PMU_LPMCFG_PMOD_SLEEP;			//����ģʽ����
	SleepCfg_InitStruct.SLPDP		= PMU_LPMCFG_SLPDP_DEEPSLEEP;		//DeepSleep����
	SleepCfg_InitStruct.DSLPRAM_EXT = PMU_LPMCFG_DSLPRAM_EXT_ALLRAMON;	//DeepSleepģʽ��RAM���ݱ�������
	SleepCfg_InitStruct.CVS			= DISABLE;							//�ں˵�ѹ���Ϳ���
	SleepCfg_InitStruct.XTOFF		= PMU_LPMCFG_XTOFF_DIS;				//XTLF���ƣ�����SLEEP/DEEPSLEEP��������	
	SleepCfg_InitStruct.SCR			= 0;								//M0ϵͳ���ƼĴ�����һ������Ϊ0����	

//    AltFunIO( GPIOG, GPIO_Pin_8, ALTFUN_PULLUP );   //PG8;//SWDTCK
//    AltFunIO( GPIOG, GPIO_Pin_9, ALTFUN_PULLUP );   //PG9;//SWDTDO  
	IWDT_Clr();	

	PMU_SleepCfg_Init(&SleepCfg_InitStruct);//��������
	__WFI();//��������
	IWDT_Clr();
}


#ifdef __MODULE__
uint8_t SYS_Pwr_SleepCfg()
{
//�رմ���
    SYS_UART_Close(UART_SETP_PORT);
    SYS_UART_Close(UART_FARP_PORT);
    SYS_UART_Close(UART_IRDA_PORT);

//io����
    HAL_GPIO_PinConfig(GPIOF, 11, 3, 1, 0);

    SYS_BlinkDev_Idel();
    SYS_GPO_Idel();
    SYS_AD_Idel();
    
//	    HAL_LcdOff();
    
    return 0;
}

uint8_t SYS_Pwr_WakeUpCfg()
{
//��������
	    SYS_UART_Open(UART_SETP_PORT);
//	    SYS_UART_Open(UART_FARP_PORT);
//	    SYS_UART_Open(UART_IRDA_PORT);
//    SerialSets ss;
//    ss.baudrate = 9600;
//    ss.parit = Parit_N;
//    ss.databits = DataBits_8bits;
//    ss.stopbits = StopBits_1;
//    SYS_SER_Init(SETP_PORT_NO, &ss);

//io����
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



