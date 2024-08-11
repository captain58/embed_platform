/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: dev_ad.h
**创   建   人: yzy
**最后修改日期: 2015年1月19日
**描        述: AD管理进程函数文件
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2015年1月19日
** 描　述: 原始版本
******************************************************************************/
#define EXT_DEV_AD

#ifdef __MODULE__
#include "WOSsys.h"
#endif
#include "sys.h"
#include "hal.h"
#include "bsp.h"



#if (SYS_AD_EN > 0)

const uint16 AD_PORT_NUM = (sizeof(gs_ADPort) / sizeof(ADPORT));
ADC* gsp_AdStt;

uint8_t ADC_Wait_Finish(void)
{	
//		Do_DelayStart();	
//		{
//			if(SET == ANAC_ADCIF_ADC_IF_Chk()) return 0;		
//		}While_DelayMsEnd(8*clkmode);//等待8ms
	
	return 1;//超时
}

/*******************************************************************************
 * @function_name:  SYS_AD_Scan
 * @function_file:  dev_ad.c
 * @描述: AD采用函数
 * 
 * @参数: 
 * 
 * @返回: 
 * @return:  Boolean   
 * @作者: yzy (2015-01-19)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
//	int SYS_AD_Scan(uint8_t ch, uint32_t * value)
//	{
//	    int ret = -1;
//	//    uint8   uc_i = 0, uc_j = 0, uc_k;
//	//    uint16 ui_value;
//	//    uint32 ul_toalvalue;
//	    ADPORT* lp;
//		uint16 ADCData[8];
//	    
//		uint16 VrefData[8];
//		uint8_t i;
//		volatile float fVlotage = 0,fTempVref = 0;
//		float fTempADC = 0;
//	    lp = (ADPORT*)(gs_ADPort + ch);
//		//使用简单函数配置
//	
//	//	    if(lp->ctrlen)
//	//	    {
//	//	//	        HAL_GPIO_SetPinState(&lp->gpio[lp->pingrp], lp->pinnum,1);
//	//	        SYS_GPO_Out(lp->ctrlno,1);
//	//	    }
//	    
//	    HAL_GPIO_PinConfig(&lp->item->gpio);
//	    
//	    /* ADC SCAN function enable */
//	    adc_special_function_config(lp->item->adcHandle, ADC_SCAN_MODE, ENABLE);
//	    adc_special_function_config(lp->item->adcHandle, ADC_CONTINUOUS_MODE, DISABLE); 
//	    /* ADC trigger config */
//	    adc_external_trigger_source_config(lp->item->adcHandle, ADC_INSERTED_CHANNEL, ADC0_1_2_EXTTRIG_INSERTED_NONE);
//	    /* ADC mode config */
//	    adc_mode_config(ADC_MODE_FREE);
//	    /* ADC data alignment config */
//	    adc_data_alignment_config(lp->item->adcHandle, ADC_DATAALIGN_RIGHT);  
//	    /* ADC channel length config */
//	    adc_channel_length_config(lp->item->adcHandle, ADC_INSERTED_CHANNEL, 2);
//	
//	  
//	    /* ADC temperature sensor channel config */
//	    adc_inserted_channel_config(lp->item->adcHandle, 0, lp->item->channel, ADC_SAMPLETIME_239POINT5);
//	    /* ADC internal reference voltage channel config */
//	    adc_inserted_channel_config(lp->item->adcHandle, 1, ADC_CHANNEL_17, ADC_SAMPLETIME_239POINT5);
//	
//	    adc_external_trigger_config(lp->item->adcHandle, ADC_INSERTED_CHANNEL, ENABLE);
//	  
//	    /* ADC temperature and Vrefint enable */
//	    adc_tempsensor_vrefint_enable();
//	 
//	    
//	    /* enable ADC interface */
//	    adc_enable(lp->item->adcHandle);
//	    msleep(1);
//	    /* ADC calibration and reset calibration */
//	    adc_calibration_enable(lp->item->adcHandle);
//	
//	
//	//	    /* ADC regular channel config */
//	//	    adc_regular_channel_config(lp->item->adcHandle, 0U, lp->item->channel, ADC_SAMPLETIME_7POINT5);
//	//	    adc_regular_channel_config(lp->item->adcHandle, 1, ADC_CHANNEL_17, ADC_SAMPLETIME_239POINT5);
//	//	    /* ADC software trigger enable */
//	//	    adc_software_trigger_enable(lp->item->adcHandle, ADC_REGULAR_CHANNEL);
//	//	
//	//	    /* wait the end of conversion flag */
//	//	    while(!adc_flag_get(lp->item->adcHandle, ADC_FLAG_EOC));
//	//	    /* clear the end of conversion flag */
//	//	    adc_flag_clear(lp->item->adcHandle, ADC_FLAG_EOC);
//	//	    /* return regular channel sample value */
//	//	    adc_regular_data_read(lp->item->adcHandle);
//	
//	    
//		fTempADC = 0;
//	    fTempVref = 0;
//		for(i=0; i<4; i++)
//		{
//	//			ANAC_ADCIF_ADC_IF_Clr();			//清除中断标志
//	
//	        /* ADC software trigger enable */
//	        adc_software_trigger_enable(lp->item->adcHandle, ADC_REGULAR_INSERTED_CHANNEL);
//	        
//	        /* wait the end of conversion flag */
//	        while(!adc_flag_get(lp->item->adcHandle, ADC_FLAG_EOC));
//	        /* clear the end of conversion flag */
//	        adc_flag_clear(lp->item->adcHandle, ADC_FLAG_EOC);
//	        /* return regular channel sample value */
//	        ADCData[i] = adc_inserted_data_read(lp->item->adcHandle,0);
//	        VrefData[i] = adc_inserted_data_read(lp->item->adcHandle,1);
//	        fTempADC += ADCData[i];
//	        fTempVref += VrefData[i];
//	        
//			
//	//			ADCData[i] = 0;
//	//			if(0 == ADC_Wait_Finish())			//等待转换完成
//	//			{
//	//				ADCData[i] = ANAC_ADCDATA_Read();	//读取AD值
//	//				fTempADC += ADCData[i];
//	//			}
//	//			else
//	//			{
//	//				break;
//	//			}
//		}
//		
//		if( i == 4 )
//		{
//			fTempADC = fTempADC/4.0;
//	        fTempVref = fTempVref/4;
//	        ret = 0;
//		}
//	    fTempVref = fTempVref * 3300 / 4096;
//	//		fVlotage = fTempADC * lp->item->vref / 4096 ;//AD值转换为电压
//		fVlotage = fTempADC * lp->item->vref / 4096 ;//AD值转换为电压
//	
//	    *value = (uint32_t)(fVlotage * 1000 * lp->item->vnum )/1000;
//	    gsp_AdStt->value[ch] =  *value;
//		return ret;
//	}


/*******************************************************************************
 * @function_name:  SYS_AD_Scan
 * @function_file:  dev_ad.c
 * @描述: AD采用函数
 * 
 * @参数: 
 * 
 * @返回: 
 * @return:  Boolean   
 * @作者: yzy (2015-01-19)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
int SYS_AD_Scan(uint8_t ch, uint32_t * value)
{
    int ret = -1;
//    uint8   uc_i = 0, uc_j = 0, uc_k;
//    uint16 ui_value;
//    uint32 ul_toalvalue;
    ADPORT* lp;
    uint16 ADCData[8];
    uint8_t i;
    volatile float fVlotage = 0;
    float fTempADC = 0;
    lp = (ADPORT*)(gs_ADPort + ch);
//使用简单函数配置

//	    if(lp->ctrlen)
//	    {
//	//	        HAL_GPIO_SetPinState(&lp->gpio[lp->pingrp], lp->pinnum,1);
//	        SYS_GPO_Out(lp->ctrlno,1);
//	    }
    
    HAL_GPIO_PinConfig(&lp->item->gpio);
    
    /* ADC mode config */
    adc_mode_config(ADC_MODE_FREE);
    /* ADC data alignment config */
    adc_data_alignment_config(lp->item->adcHandle, ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(lp->item->adcHandle, ADC_REGULAR_CHANNEL, 1U);
    
    /* ADC trigger config */
    adc_external_trigger_source_config(lp->item->adcHandle, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE); 
    /* ADC external trigger config */
    adc_external_trigger_config(lp->item->adcHandle, ADC_REGULAR_CHANNEL, ENABLE);
    
    /* enable ADC interface */
    adc_enable(lp->item->adcHandle);
    msleep(1U);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(lp->item->adcHandle);/* ADC mode config */
    adc_mode_config(ADC_MODE_FREE);
    /* ADC data alignment config */
    adc_data_alignment_config(lp->item->adcHandle, ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(lp->item->adcHandle, ADC_REGULAR_CHANNEL, 1U);
    
    /* ADC trigger config */
    adc_external_trigger_source_config(lp->item->adcHandle, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE); 
    /* ADC external trigger config */
    adc_external_trigger_config(lp->item->adcHandle, ADC_REGULAR_CHANNEL, ENABLE);
    
    /* enable ADC interface */
    adc_enable(lp->item->adcHandle);
    msleep(1U);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(lp->item->adcHandle);


    /* ADC regular channel config */
    adc_regular_channel_config(lp->item->adcHandle, 0U, lp->item->channel, ADC_SAMPLETIME_7POINT5);
    /* ADC software trigger enable */
    adc_software_trigger_enable(lp->item->adcHandle, ADC_REGULAR_CHANNEL);

    /* wait the end of conversion flag */
    while(!adc_flag_get(lp->item->adcHandle, ADC_FLAG_EOC));
    /* clear the end of conversion flag */
    adc_flag_clear(lp->item->adcHandle, ADC_FLAG_EOC);
    /* return regular channel sample value */
    adc_regular_data_read(lp->item->adcHandle);

    
    fTempADC = 0;
    for(i=0; i<4; i++)
    {
    //			ANAC_ADCIF_ADC_IF_Clr();			//清除中断标志

        /* ADC software trigger enable */
        adc_software_trigger_enable(lp->item->adcHandle, ADC_REGULAR_INSERTED_CHANNEL);
        
        /* wait the end of conversion flag */
        while(!adc_flag_get(lp->item->adcHandle, ADC_FLAG_EOC));
        /* clear the end of conversion flag */
        adc_flag_clear(lp->item->adcHandle, ADC_FLAG_EOC);
        /* return regular channel sample value */
        ADCData[i] = adc_regular_data_read(lp->item->adcHandle);
        fTempADC += ADCData[i];
        

    //			ADCData[i] = 0;
    //			if(0 == ADC_Wait_Finish())			//等待转换完成
    //			{
    //				ADCData[i] = ANAC_ADCDATA_Read();	//读取AD值
    //				fTempADC += ADCData[i];
    //			}
    //			else
    //			{
    //				break;
    //			}
    }

    if( i == 4 )
    {
    	fTempADC = fTempADC/4.0;
    ret = 0;
    }
	fVlotage = fTempADC * lp->item->vref / 4096 ;//AD值转换为电压

    *value = (uint32_t)(fVlotage * 1000 * lp->item->vnum )/1000;
    gsp_AdStt->value[ch] =  *value;


return ret;
}
/*******************************************************************************
 * @function_name:  SYS_AD_GetValue
 * @function_file:  dev_ad.c
 * @描述: 取得AD采用后的数据
 * 
 * @参数: di AD序号
 * 
 * @返回: AD采用值
 * @return:     
 * @作者: yzy (2015-01-23)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint32 SYS_AD_GetValue(uint8 di)
{
    return gsp_AdStt->value[di];
}

ADC __AdStt;

/*******************************************************************************
 * @function_name:  SYS_AD_Init
 * @function_file:  dev_ad.c
 * @描述: 
 * 
 * @参数: 
 * @返回: 
 * @作者: yzy (2015-01-19)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void SYS_AD_Init(void)
{
    uint8   uc_i = 0;  
    ADPORT* lp;
//	    ADC_CLOCK_SETUP_T adc_clock;
//	    RCC_PERCLK_SetableEx(ANACCLK, ENABLE);      //模拟电路总线时钟使能
//	    RCC_PERCLK_SetableEx(ADCCLK, ENABLE);       //ADC时钟使能
//	    RCC_PERCLKCON2_ADCCKSEL_Set(RCC_PERCLKCON2_ADCCKSEL_RCHFDIV16); //ADC工作时钟配置，不可高于1M
    rcu_periph_clock_enable(RCU_ADC0);
    //申请缓存
    gsp_AdStt = &__AdStt;//(ADC*)m_malloc(sizeof(ADC));
    memset((uint8*)gsp_AdStt, 0, sizeof(ADC));
    
//	    lp = (ADPORT*)(gs_ADPort + uc_i);
    
    for(uc_i = 0; uc_i < AD_PORT_NUM; uc_i++)
    {
            /* enable ADC clock */
        lp = (ADPORT*)(gs_ADPort + uc_i);
       
//	        HAL_GPIO_PinConfig(&lp->gpio[lp->pingrp], lp->pinnum, lp->lptype, 0, lp->lpdir);
//	        HAL_GPIO_SetPinState(&lp->gpio[lp->pingrp], lp->pinnum, lp->lpval);
        HAL_GPIO_PinConfig(&lp->item->gpio);

        /* ADC mode config */
        adc_mode_config(ADC_MODE_FREE);
        /* ADC data alignment config */
        adc_data_alignment_config(lp->item->adcHandle, ADC_DATAALIGN_RIGHT);
        /* ADC channel length config */
        adc_channel_length_config(lp->item->adcHandle, ADC_REGULAR_CHANNEL, 1U);
        
        /* ADC trigger config */
        adc_external_trigger_source_config(lp->item->adcHandle, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE); 
        /* ADC external trigger config */
        adc_external_trigger_config(lp->item->adcHandle, ADC_REGULAR_CHANNEL, ENABLE);

        /* enable ADC interface */
        adc_enable(lp->item->adcHandle);
        msleep(1U);
        /* ADC calibration and reset calibration */
        adc_calibration_enable(lp->item->adcHandle);/* ADC mode config */
        adc_mode_config(ADC_MODE_FREE);
        /* ADC data alignment config */
        adc_data_alignment_config(lp->item->adcHandle, ADC_DATAALIGN_RIGHT);
        /* ADC channel length config */
        adc_channel_length_config(lp->item->adcHandle, ADC_REGULAR_CHANNEL, 1U);
        
        /* ADC trigger config */
        adc_external_trigger_source_config(lp->item->adcHandle, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE); 
        /* ADC external trigger config */
        adc_external_trigger_config(lp->item->adcHandle, ADC_REGULAR_CHANNEL, ENABLE);

        /* enable ADC interface */
        adc_enable(lp->item->adcHandle);
        msleep(1U);
        /* ADC calibration and reset calibration */
        adc_calibration_enable(lp->item->adcHandle);


//	        Chip_ADC_Init(lp->adc, &adc_clock); // 12位模式和正常的电源设置ADC

        //Chip_ADC_SetSampleRate(lp->adc, &adc_clock, ADC_MAX_SAMPLE_RATE);// 设置为最大的ADC时钟速率 

//	        Chip_IOCON_PinMuxSet(LPC_IOCON, lp->pingrp, lp->pinnum, lp->modefunc ); // 禁止上拉/下拉和禁用数字模式
//	        Chip_ADC_SetSequencerBits(lp->adc, lp->seq, 1 << lp->chn);
        //Chip_ADC_Int_SetChannelCmd(lp->adc, 1<<lp->chn, ENABLE);
        
        //Chip_ADC_SetBurstCmd(lp->adc, DISABLE);
        
        //Chip_ADC_EnableChannel(lp->adc, (ADC_CHANNEL_T)lp->chn, ENABLE);
        
//	        Chip_ADC_SetStartMode(lp->adc, lp->seq);
    }
//	    ANAC_ADCCON_ADC_IE_Setable(DISABLE);        //中断禁止
//	    ANAC_ADCCON_ADC_EN_Setable(DISABLE);        //ADC关闭

//	    SYS_Timer_Create(SYS_AD_Scan, __NULL, 10, TIMER_ID_ADC, false);
}

void SYS_AD_Idel(void)
{
    ADPORT* lp;
    uint8   uc_i = 0; 
//	
//	    for(uc_i = 0; uc_i < AD_PORT_NUM; uc_i++)
//	    {
//	        lp = (ADPORT*)(gs_ADPort + uc_i);
//	       
//	        HAL_GPIO_PinConfig(&lp->gpio[lp->pingrp], lp->pinnum, lp->lptype, 0, lp->lpdir);
//	        HAL_GPIO_SetPinState(&lp->gpio[lp->pingrp], lp->pinnum, lp->lpval);
//	
//	    }

}


#endif

