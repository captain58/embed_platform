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




uint8_t ADC_Wait_Finish(void)
{	
	Do_DelayStart();	
	{
		if(SET == ANAC_ADCIF_ADC_IF_Chk()) return 0;		
	}While_DelayMsEnd(8*clkmode);//�ȴ�8ms
	
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

    if(lp->ctrlen)
    {
//	        HAL_GPIO_SetPinState(&lp->gpio[lp->pingrp], lp->pinnum,1);
        SYS_GPO_Out(lp->ctrlno,1);
    }
    
    HAL_GPIO_PinConfig(&lp->gpio[lp->pingrp], lp->pinnum, lp->type, 1, 0);
    
//	RCC_PERCLK_SetableEx(ANACCLK, ENABLE);		//ģ���·����ʱ��ʹ��
	RCC_PERCLK_SetableEx(ADCCLK, ENABLE);		//ADCʱ��ʹ��
	RCC_PERCLKCON2_ADCCKSEL_Set(RCC_PERCLKCON2_ADCCKSEL_RCHFDIV16);	//ADC����ʱ�����ã����ɸ���1M
	
	ANAC_ADC_Channel_SetEx(lp->chn);				//ADC����ͨ��ѡ��
	ANAC_ADCCON_ADC_IE_Setable(DISABLE);		//�жϽ�ֹ
	ANAC_ADCCON_ADC_EN_Setable(DISABLE);		//ADC�ر�

	ANAC_ADCCON_ADC_EN_Setable(ENABLE);		//ADC����
	
	ADC_Wait_Finish();						//������һ��ת�����
	ANAC_ADCIF_ADC_IF_Clr();				//����жϱ�־
	
	fTempADC = 0;
	for(i=0; i<4; i++)
	{
		ANAC_ADCIF_ADC_IF_Clr();			//����жϱ�־
		
		ADCData[i] = 0;
		if(0 == ADC_Wait_Finish())			//�ȴ�ת�����
		{
			ADCData[i] = ANAC_ADCDATA_Read();	//��ȡADֵ
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
	fVlotage = ANAC_ADC_VoltageCalc(fTempADC);//ADֵת��Ϊ��ѹ

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

    
//		RCC_PERCLK_SetableEx(ANACCLK, DISABLE);		//ģ���·����ʱ��ʹ��
//		RCC_PERCLK_SetableEx(ADCCLK, DISABLE);		//ADCʱ��ʹ��

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

                                            //���뻺��
    gsp_AdStt = &__AdStt;//(ADC*)m_malloc(sizeof(ADC));
    memset_s((uint8*)gsp_AdStt, 0, sizeof(ADC));
    
//	    lp = (ADPORT*)(gs_ADPort + uc_i);
    
    for(uc_i = 0; uc_i < AD_PORT_NUM; uc_i++)
    {
        lp = (ADPORT*)(gs_ADPort + uc_i);
       
        HAL_GPIO_PinConfig(&lp->gpio[lp->pingrp], lp->pinnum, lp->lptype, 0, lp->lpdir);
        HAL_GPIO_SetPinState(&lp->gpio[lp->pingrp], lp->pinnum, lp->lpval);

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

    for(uc_i = 0; uc_i < AD_PORT_NUM; uc_i++)
    {
        lp = (ADPORT*)(gs_ADPort + uc_i);
       
        HAL_GPIO_PinConfig(&lp->gpio[lp->pingrp], lp->pinnum, lp->lptype, 0, lp->lpdir);
        HAL_GPIO_SetPinState(&lp->gpio[lp->pingrp], lp->pinnum, lp->lpval);

    }

}


#endif

