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

#include "aos/include.h"
#include "hal.h"
//#include "ulog.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	#include "lib_app.h"
//	#include "hal.h"
#include "ulog/ulog.h"




#if (SYS_AD_EN > 0)

#define AD_WITH_TIMER 0

static ADC adc_mem;
#if (AD_WITH_TIMER > 0)
static ktimer_t     timer_ad;
#endif
//static uint8 g_ucADchannel = 0;
extern const ADPORT gs_ADPort[];
ADC_HandleTypeDef  AdcHandle[3];
uint32_t adcRank[] = {ADC_REGULAR_RANK_1,ADC_REGULAR_RANK_2,ADC_REGULAR_RANK_3};
//	aos_mutex_t g_admutex;
aos_sem_t   g_admutex;
uint16_t uhADCxConvertedValue[AD_CONVERSION][AD_NUM];

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
//	Boolean SYS_AD_Scan(void* pdata)
//	static uint8_t SYS_AD_Scan(void *timer, void *arg)
//	{
//	    //uint8   uc_i = 0;
//	    uint8   uc_j = 0;
//	    uint8   uc_k;
//	    uint16 ui_value;
//	    uint32 ul_toalvalue;
//	    ADPORT* lp;
//	    uint8 channel = *(uint8 * )arg;
//	    //LOG_DEBUG("SYS_AD_Scan uc_i =  %d uc_j = %d channel = %d\n", uc_i, uc_j, channel);
//	    extern const uint16 AD_PORT_NUM;
//	    ADC_ChannelConfTypeDef sConfig;
//	    
//	    _IF_TRUE_RETURN_Z(channel >= AD_PORT_NUM, SYS_ERR_FT);
//	    
//	    aos_mutex_lock((aos_mutex_t *)&g_admutex, AOS_WAIT_FOREVER);
//	    //for(uc_i = 0; uc_i < AD_NUM; uc_i++)
//	    {
//	        if (HAL_ADC_DeInit(&AdcHandle[channel]) != HAL_OK)
//	        {
//	          /* ADC de-initialization Error */
//	             goto AD_ERR;
//	        }
//	        lp = (ADPORT*)(gs_ADPort + channel);
//	        AdcHandle[channel].Instance                   = lp->adc;
//	        AdcHandle[channel].Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;          /* Asynchronous clock mode, input ADC clock not divided */
//	        AdcHandle[channel].Init.Resolution            = ADC_RESOLUTION_12B;            /* 12-bit resolution for converted data */
//	        AdcHandle[channel].Init.DataAlign             = ADC_DATAALIGN_RIGHT;           /* Right-alignment for converted data */
//	        AdcHandle[channel].Init.ScanConvMode          = ENABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
//	        AdcHandle[channel].Init.EOCSelection          = DISABLE;                       /* EOC flag picked-up to indicate conversion end */
//	        AdcHandle[channel].Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
//	        AdcHandle[channel].Init.NbrOfConversion       = 3;                             /* Parameter discarded because sequencer is disabled */
//	        AdcHandle[channel].Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
//	        AdcHandle[channel].Init.NbrOfDiscConversion   = 0;                             /* Parameter discarded because sequencer is disabled */
//	        AdcHandle[channel].Init.ExternalTrigConv      = ADC_SOFTWARE_START;//ADC_EXTERNALTRIGCONV_T1_CC1;   /* Software start to trig the 1st conversion manually, without external event */
//	        AdcHandle[channel].Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
//	        AdcHandle[channel].Init.DMAContinuousRequests = DISABLE;                       /* DMA one-shot mode selected (not applied to this example) */
//	
//	        if (HAL_ADC_Init(&AdcHandle[channel]) != HAL_OK)
//	        {
//	          /* ADC initialization Error */
//	            //Error_Handler();
//	            goto AD_ERR;
//	        }
//	                
//	        GPIO_InitTypeDef          GPIO_InitStruct;
//	        
//	        /*##-2- Configure peripheral GPIO ##########################################*/
//	        /* ADC Channel GPIO pin configuration */
//	        GPIO_InitStruct.Pin = lp->port;
//	        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
//	        GPIO_InitStruct.Pull = GPIO_NOPULL;
//	        HAL_GPIO_Init(lp->gpio, &GPIO_InitStruct);
//	
//	        sConfig.Channel = lp->channel;
//	        sConfig.Rank = adcRank[channel];
//	        sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
//	        sConfig.SingleDiff = ADC_SINGLE_ENDED;
//	        sConfig.OffsetNumber = ADC_OFFSET_NONE;
//	        sConfig.Offset = 0;        
//	        if (HAL_ADC_ConfigChannel(&AdcHandle[channel], &sConfig) != HAL_OK)
//	        {
//	          /* Channel Configuration Error */
//	          //LOG_ERROR("HAL_ADC_ConfigChannel failed !!!!!!!!!!!\n");
//	            goto AD_ERR;
//	        }
//	        ul_toalvalue = 0;
//	        uc_k = 0;
//	
//	        for(uc_j = 0; uc_j < 8; uc_j++)
//	        {
//	            ui_value = 0;
//	            
//	//	            if (ADC_Enable(&AdcHandle[channel]) != HAL_OK)
//	//	            {
//	//	              /* Start Conversation Error */
//	//	              return SYS_ERR_FT;
//	//	            }
//	//	                
//	            HAL_ADCEx_Calibration_Start(&AdcHandle[channel], ADC_SINGLE_ENDED);
//	
//	            //AdcHandle[channel].Instance->CR |= 0x04;
//	            if (HAL_ADC_Start(&AdcHandle[channel]) != HAL_OK)
//	            {
//	              /* Start Conversation Error */
//	                goto AD_ERR;
//	            }
//	
//	            if (HAL_ADC_PollForConversion(&AdcHandle[channel], 50) != HAL_OK)
//	            {
//	            /* End Of Conversion flag not set on time */
//	                //return SYS_ERR_FT;
//	                goto AD_ERR;
//	            }
//	
//	            /* Check if the continuous conversion of regular channel is finished */
//	            if ((HAL_ADC_GetState(&AdcHandle[channel]) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC)
//	            {
//	            /*##-5- Get the converted value of regular channel  ########################*/
//	                ui_value = HAL_ADC_GetValue(&AdcHandle[channel]);
//	            }            
//	//	            while((HAL_ADC_GetState(lp->AdcHandle) & HAL_ADC_STATE_READY) == HAL_ADC_STATE_READY);
//	
//	            //ui_value = HAL_ADC_GetValue(AdcHandle[channel);
//	            LOG_INFO("ADC channel[%d] = %d!\n", channel, ui_value);
//	            
//	//	            if(Chip_ADC_ReadValue(lp->adc, lp->chn, &ui_value) == true)//
//	            {
//	                ul_toalvalue += (uint32)ui_value * lp->vref / 4095 * lp->vnum;
//	                uc_k++;
//	            }
//	        }
//	
//	        if(uc_k > 0)
//	        {
//	            gsp_AdStt->value[channel] =  ul_toalvalue / uc_k;
//	        }
//	        
//	    }
//	//	    krhino_timer_stop(&timer_ad);
//	//		return true;
//	    aos_mutex_unlock((aos_mutex_t *)&g_admutex);
//	
//	    SYS_OK();
//	AD_ERR:
//	    aos_mutex_unlock((aos_mutex_t *)&g_admutex);
//	
//	    SYS_ERR();
//	}

static uint8_t SYS_AD_Scan(void *timer, void *arg)
{
    uint8 channel = *(uint8 * )arg;
    uint16 ui_value;
    uint32 ul_toalvalue;
    ADPORT* lp;
    lp = (ADPORT*)(gs_ADPort);

    extern const uint16 AD_PORT_NUM;
    if(AD_PORT_NUM > 1)
    {
        uint8_t tmpPortNum = 0;
        int i = 0;
        for(i = 0; i < AD_PORT_NUM; i++)
        {
            lp = (ADPORT*)(gs_ADPort + i);
            tmpPortNum += lp->chnum;
            if(channel < tmpPortNum)
            {
                break;
            }
        }
        if(i == AD_PORT_NUM)
        {
            return SYS_ERR_GT;
        }
    }
    HAL_ADCEx_Calibration_Start(lp->adhandle, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(lp->adhandle, (uint32_t*)&uhADCxConvertedValue, AD_CONVERSION*lp->chnum);	

    
    int ret = aos_sem_wait(&g_admutex, 2000);
    if(ret == 0)
    {
        for(int uc_i = 0; uc_i < lp->chnum; uc_i++)
        {
            ul_toalvalue = 0;
            for(int uc_j = 0; uc_j < AD_CONVERSION; uc_j++)
            {
                ui_value = uhADCxConvertedValue[uc_j][uc_i];

                ul_toalvalue += (uint32)ui_value * lp->item[uc_i].vref / 4095 * lp->item[uc_i].vnum;
            }
            gsp_AdStt->value[lp->choffset + uc_i] =  ul_toalvalue / AD_CONVERSION;
        }
        
        SYS_OK();
    }
    else
    {
        SYS_ERR();
    }
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
uint32 SYS_AD_GetValue(uint8 channel)
{
#if (AD_WITH_TIMER > 0)

    return gsp_AdStt->value[channel];
#else
    SYS_AD_Scan(NULL, (void *)&channel);

    return gsp_AdStt->value[channel];
#endif
}



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
//	void SYS_AD_Init(void)
//	{
//	    uint8   uc_i = 0;  
//	    ADPORT* lp;
//	                                            //申请缓存
//	    gsp_AdStt = &adc_mem;//(ADC*)SYS_GetMEM(sizeof(ADC));
//	    ClearBuffer((uint8*)gsp_AdStt, sizeof(ADC));
//	
//	    ADC_ChannelConfTypeDef sConfig;
//	//	    ADC_HandleTypeDef    AdcHandle;
//	//	    if (0 != aos_mutex_new(&g_admutex)) {
//	//	        LOG_ERROR("Creating ad mutex failed (%s %d).", __func__, __LINE__);
//	//	        return ;
//	//	    }
//	    if (0 != aos_sem_new(&g_admutex, 0)) {
//	        LOG_INFO( "Creating ad mutex failed (%s %d).", __func__, __LINE__);
//	        return ;
//	    }
//	
//	    if(AD_NUM > 0)
//	    {
//	        
//	        __HAL_RCC_ADC_CLK_ENABLE();
//	    }
//	    //for(uc_i = 0; uc_i < AD_NUM; uc_i++)
//	    {
//	    
//	        lp = (ADPORT*)(gs_ADPort + uc_i);
//	        
//	        AdcHandle[uc_i].Instance          = lp->adc;
//	//	        if (HAL_ADC_DeInit(&AdcHandle[uc_i]) != HAL_OK)
//	//	        {
//	//	          /* ADC de-initialization Error */
//	//	            Error_Handler();
//	//	        }
//	        AdcHandle[uc_i].Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;          /* Asynchronous clock mode, input ADC clock not divided */
//	        AdcHandle[uc_i].Init.Resolution            = ADC_RESOLUTION_12B;            /* 12-bit resolution for converted data */
//	        AdcHandle[uc_i].Init.DataAlign             = ADC_DATAALIGN_RIGHT;           /* Right-alignment for converted data */
//	        AdcHandle[uc_i].Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
//	        AdcHandle[uc_i].Init.EOCSelection          = DISABLE;                       /* EOC flag picked-up to indicate conversion end */
//	        AdcHandle[uc_i].Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
//	        AdcHandle[uc_i].Init.NbrOfConversion       = AD_NUM;                             /* Parameter discarded because sequencer is disabled */
//	        AdcHandle[uc_i].Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
//	        AdcHandle[uc_i].Init.NbrOfDiscConversion   = 0;                             /* Parameter discarded because sequencer is disabled */
//	        AdcHandle[uc_i].Init.ExternalTrigConv      = ADC_SOFTWARE_START;//ADC_EXTERNALTRIGCONV_T1_CC1;   /* Software start to trig the 1st conversion manually, without external event */
//	        AdcHandle[uc_i].Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
//	        AdcHandle[uc_i].Init.DMAContinuousRequests = DISABLE;                       /* DMA one-shot mode selected (not applied to this example) */
//	
//	        if (HAL_ADC_Init(&AdcHandle[uc_i]) != HAL_OK)
//	        {
//	          /* ADC initialization Error */
//	            Error_Handler();
//	        }
//	
//	        
//	    }
//	    
//	    
//	//	    lp = (ADPORT*)(gs_ADPort + uc_i);
//	
//	    for(uc_i = 0; uc_i < AD_NUM; uc_i++)
//	    {
//	
//	        lp = (ADPORT*)(gs_ADPort + uc_i);
//	        
//	        GPIO_InitTypeDef          GPIO_InitStruct;
//	        
//	        /*##-2- Configure peripheral GPIO ##########################################*/
//	        /* ADC Channel GPIO pin configuration */
//	        GPIO_InitStruct.Pin = lp->port;
//	        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
//	        GPIO_InitStruct.Pull = GPIO_NOPULL;
//	        HAL_GPIO_Init(lp->gpio, &GPIO_InitStruct);
//	        /* ADC Initialization */
//	
//	        
//	//	        lp->AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
//	//	        lp->AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
//	//	        lp->AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
//	//	        lp->AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
//	//	        lp->AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
//	//	        lp->AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T2_TRGO;       /* Conversion start trigged at each external event */
//	//	        lp->AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;
//	//	        lp->AdcHandle.Init.DMAContinuousRequests = ENABLE;
//	//	        lp->AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
//	//	        lp->AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
//	//	
//	//	        ADC_EXTERNALTRIGCONVEDGE_FALLING
//	
//	//	        AdcHandle[uc_i].Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
//	//	        AdcHandle[uc_i].Init.Resolution = ADC_RESOLUTION_12B;
//	//	        AdcHandle[uc_i].Init.DataAlign = ADC_DATAALIGN_RIGHT;
//	//	        AdcHandle[uc_i].Init.ScanConvMode = ADC_SCAN_DISABLE;
//	//	        AdcHandle[uc_i].Init.EOCSelection = ADC_EOC_SINGLE_CONV;
//	//	        AdcHandle[uc_i].Init.LowPowerAutoWait = DISABLE;
//	//	        AdcHandle[uc_i].Init.ContinuousConvMode = DISABLE;
//	//	        AdcHandle[uc_i].Init.NbrOfConversion = 1;
//	//	        AdcHandle[uc_i].Init.DiscontinuousConvMode = DISABLE;
//	//	        AdcHandle[uc_i].Init.ExternalTrigConv = ADC_SOFTWARE_START;
//	//	        AdcHandle[uc_i].Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
//	//	        AdcHandle[uc_i].Init.DMAContinuousRequests = DISABLE;
//	//	        AdcHandle[uc_i].Init.Overrun = ADC_OVR_DATA_PRESERVED;
//	//	        AdcHandle[uc_i].Init.OversamplingMode = DISABLE;
//	
//	        
//	        
//	        /*##-3- Configure the NVIC #################################################*/
//	//	        HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
//	//	        HAL_NVIC_EnableIRQ(ADC_IRQn);
//	
//	//	        /* Configure ADC regular channel */
//	//	        sConfig.Channel      = lp->channel;
//	//	        sConfig.Rank         = 1;
//	//	        sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
//	        sConfig.Channel = lp->channel;
//	        sConfig.Rank = ADC_REGULAR_RANK_1+uc_i;
//	        sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
//	        sConfig.SingleDiff = ADC_SINGLE_ENDED;
//	        sConfig.OffsetNumber = ADC_OFFSET_NONE;
//	        sConfig.Offset = 0;        
//	        if (HAL_ADC_ConfigChannel(&AdcHandle[0], &sConfig) != HAL_OK)
//	        {
//	          /* Channel Configuration Error */
//	          Error_Handler();
//	        }
//	
//	
//	    }
//	#if (AD_WITH_TIMER > 0)
//	    krhino_timer_create(&timer_ad, "timer_ad", SYS_AD_Scan,
//	                            10, 0, (void *)&g_ucADchannel, 0);     
//	#endif
//	}

void SYS_AD_Init(void)
{
    uint8   uc_i = 0;  
    ADPORT* lp;
                                            //申请缓存
    gsp_AdStt = &adc_mem;//(ADC*)SYS_GetMEM(sizeof(ADC));
    ClearBuffer((uint8*)gsp_AdStt, sizeof(ADC));

    if (0 != aos_sem_new(&g_admutex, 0)) {
        LOG_INFO( "Creating ad mutex failed (%s %d).", __func__, __LINE__);
        return ;
    }

}

#endif

