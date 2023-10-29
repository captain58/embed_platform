/****************************************Copyright (c)*************************
**                               	________�Ƽ����޹�˾
**                                     		������
**
**
**--------------�ļ���Ϣ-------------------------------------------------------
**��   ��   ��: dev_ad.h
**��   ��   ��: yzy
**����޸�����: 2015��1��19��
**��        ��: AD������̺����ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ---------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2015��1��19��
** �衡��: ԭʼ�汾
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
//		}While_DelayMsEnd(8*clkmode);//�ȴ�8ms
	
	return 1;//��ʱ
}

/*******************************************************************************
 * @function_name:  SYS_AD_Scan
 * @function_file:  dev_ad.c
 * @����: AD���ú���
 * 
 * @����: 
 * 
 * @����: 
 * @return:  Boolean   
 * @����: yzy (2015-01-19)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
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
//		//ʹ�ü򵥺�������
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
//	//			ANAC_ADCIF_ADC_IF_Clr();			//����жϱ�־
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
//	//			if(0 == ADC_Wait_Finish())			//�ȴ�ת�����
//	//			{
//	//				ADCData[i] = ANAC_ADCDATA_Read();	//��ȡADֵ
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
//	//		fVlotage = fTempADC * lp->item->vref / 4096 ;//ADֵת��Ϊ��ѹ
//		fVlotage = fTempADC * lp->item->vref / 4096 ;//ADֵת��Ϊ��ѹ
//	
//	    *value = (uint32_t)(fVlotage * 1000 * lp->item->vnum )/1000;
//	    gsp_AdStt->value[ch] =  *value;
//		return ret;
//	}


/*******************************************************************************
 * @function_name:  SYS_AD_Scan
 * @function_file:  dev_ad.c
 * @����: AD���ú���
 * 
 * @����: 
 * 
 * @����: 
 * @return:  Boolean   
 * @����: yzy (2015-01-19)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
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
//ʹ�ü򵥺�������

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
    //			ANAC_ADCIF_ADC_IF_Clr();			//����жϱ�־

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
    //			if(0 == ADC_Wait_Finish())			//�ȴ�ת�����
    //			{
    //				ADCData[i] = ANAC_ADCDATA_Read();	//��ȡADֵ
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
	fVlotage = fTempADC * lp->item->vref / 4096 ;//ADֵת��Ϊ��ѹ

    *value = (uint32_t)(fVlotage * 1000 * lp->item->vnum )/1000;
    gsp_AdStt->value[ch] =  *value;


return ret;
}
/*******************************************************************************
 * @function_name:  SYS_AD_GetValue
 * @function_file:  dev_ad.c
 * @����: ȡ��AD���ú������
 * 
 * @����: di AD���
 * 
 * @����: AD����ֵ
 * @return:     
 * @����: yzy (2015-01-23)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint32 SYS_AD_GetValue(uint8 di)
{
    return gsp_AdStt->value[di];
}

ADC __AdStt;

/*******************************************************************************
 * @function_name:  SYS_AD_Init
 * @function_file:  dev_ad.c
 * @����: 
 * 
 * @����: 
 * @����: 
 * @����: yzy (2015-01-19)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_AD_Init(void)
{
    uint8   uc_i = 0;  
    ADPORT* lp;
//	    ADC_CLOCK_SETUP_T adc_clock;
//	    RCC_PERCLK_SetableEx(ANACCLK, ENABLE);      //ģ���·����ʱ��ʹ��
//	    RCC_PERCLK_SetableEx(ADCCLK, ENABLE);       //ADCʱ��ʹ��
//	    RCC_PERCLKCON2_ADCCKSEL_Set(RCC_PERCLKCON2_ADCCKSEL_RCHFDIV16); //ADC����ʱ�����ã����ɸ���1M
    rcu_periph_clock_enable(RCU_ADC0);
    //���뻺��
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


//	        Chip_ADC_Init(lp->adc, &adc_clock); // 12λģʽ�������ĵ�Դ����ADC

        //Chip_ADC_SetSampleRate(lp->adc, &adc_clock, ADC_MAX_SAMPLE_RATE);// ����Ϊ����ADCʱ������ 

//	        Chip_IOCON_PinMuxSet(LPC_IOCON, lp->pingrp, lp->pinnum, lp->modefunc ); // ��ֹ����/�����ͽ�������ģʽ
//	        Chip_ADC_SetSequencerBits(lp->adc, lp->seq, 1 << lp->chn);
        //Chip_ADC_Int_SetChannelCmd(lp->adc, 1<<lp->chn, ENABLE);
        
        //Chip_ADC_SetBurstCmd(lp->adc, DISABLE);
        
        //Chip_ADC_EnableChannel(lp->adc, (ADC_CHANNEL_T)lp->chn, ENABLE);
        
//	        Chip_ADC_SetStartMode(lp->adc, lp->seq);
    }
//	    ANAC_ADCCON_ADC_IE_Setable(DISABLE);        //�жϽ�ֹ
//	    ANAC_ADCCON_ADC_EN_Setable(DISABLE);        //ADC�ر�

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

