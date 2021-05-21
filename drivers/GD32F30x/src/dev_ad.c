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




uint8_t ADC_Wait_Finish(void)
{	
	Do_DelayStart();	
	{
		if(SET == ANAC_ADCIF_ADC_IF_Chk()) return 0;		
	}While_DelayMsEnd(8*clkmode);//等待8ms
	
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

    if(lp->ctrlen)
    {
//	        HAL_GPIO_SetPinState(&lp->gpio[lp->pingrp], lp->pinnum,1);
        SYS_GPO_Out(lp->ctrlno,1);
    }
    
    HAL_GPIO_PinConfig(&lp->gpio[lp->pingrp], lp->pinnum, lp->type, 1, 0);
    
//	RCC_PERCLK_SetableEx(ANACCLK, ENABLE);		//模拟电路总线时钟使能
	RCC_PERCLK_SetableEx(ADCCLK, ENABLE);		//ADC时钟使能
	RCC_PERCLKCON2_ADCCKSEL_Set(RCC_PERCLKCON2_ADCCKSEL_RCHFDIV16);	//ADC工作时钟配置，不可高于1M
	
	ANAC_ADC_Channel_SetEx(lp->chn);				//ADC输入通道选择
	ANAC_ADCCON_ADC_IE_Setable(DISABLE);		//中断禁止
	ANAC_ADCCON_ADC_EN_Setable(DISABLE);		//ADC关闭

	ANAC_ADCCON_ADC_EN_Setable(ENABLE);		//ADC启动
	
	ADC_Wait_Finish();						//丢弃第一个转换结果
	ANAC_ADCIF_ADC_IF_Clr();				//清除中断标志
	
	fTempADC = 0;
	for(i=0; i<4; i++)
	{
		ANAC_ADCIF_ADC_IF_Clr();			//清除中断标志
		
		ADCData[i] = 0;
		if(0 == ADC_Wait_Finish())			//等待转换完成
		{
			ADCData[i] = ANAC_ADCDATA_Read();	//读取AD值
			fTempADC += ADCData[i];
		}
		else
		{
			break;
		}
	}
	
	if( i == 4 )
	{
		fTempADC = fTempADC/4.0;
        ret = 0;
	}
	fVlotage = ANAC_ADC_VoltageCalc(fTempADC);//AD值转换为电压

    *value = (uint32_t)(fVlotage * 1000 * lp->vnum)/1000;
    gsp_AdStt->value[ch] =  *value;
    if(lp->ctrlen)
    {
//	        HAL_GPIO_SetPinState(&lp->gpio[lp->pingrp], lp->pinnum,0);
        SYS_GPO_Out(GPO_ADC_VBAT,0);
    }
//	
//	    for(uc_i = 0; uc_i < AD_NUM; uc_i++)
//	    {
//	        lp = (ADPORT*)(gs_ADPort + uc_i);
//	      
//	        ul_toalvalue = 0;
//	        uc_k = 0;
//	        for(uc_j = 0; uc_j < 8; uc_j++)
//	        {
//	            ui_value = 0;
//	            Chip_ADC_SetStartMode(lp->adc, lp->seq);
//	            while(Chip_ADC_ReadStatus(lp->adc, lp->chn) == RESET);
//	            
//	            if(Chip_ADC_ReadValue(lp->adc, lp->chn, &ui_value) == true)//
//	            {
//	                ul_toalvalue += (uint32)ui_value * lp->vref / 4096 * lp->vnum;
//	                uc_k++;
//	            }
//	        }
//	        if(uc_k > 0)
//	        {
//	            gsp_AdStt->value[uc_i] =  ul_toalvalue / uc_k;
//	        }
//	        
//	    }

    
//		RCC_PERCLK_SetableEx(ANACCLK, DISABLE);		//模拟电路总线时钟使能
//		RCC_PERCLK_SetableEx(ADCCLK, DISABLE);		//ADC时钟使能

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

                                            //申请缓存
    gsp_AdStt = &__AdStt;//(ADC*)m_malloc(sizeof(ADC));
    memset_s((uint8*)gsp_AdStt, 0, sizeof(ADC));
    
//	    lp = (ADPORT*)(gs_ADPort + uc_i);
    
    for(uc_i = 0; uc_i < AD_PORT_NUM; uc_i++)
    {
        lp = (ADPORT*)(gs_ADPort + uc_i);
       
        HAL_GPIO_PinConfig(&lp->gpio[lp->pingrp], lp->pinnum, lp->lptype, 0, lp->lpdir);
        HAL_GPIO_SetPinState(&lp->gpio[lp->pingrp], lp->pinnum, lp->lpval);

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

    for(uc_i = 0; uc_i < AD_PORT_NUM; uc_i++)
    {
        lp = (ADPORT*)(gs_ADPort + uc_i);
       
        HAL_GPIO_PinConfig(&lp->gpio[lp->pingrp], lp->pinnum, lp->lptype, 0, lp->lpdir);
        HAL_GPIO_SetPinState(&lp->gpio[lp->pingrp], lp->pinnum, lp->lpval);

    }

}


#endif

