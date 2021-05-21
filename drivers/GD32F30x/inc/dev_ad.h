/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: dev_ad.h
**创   建   人: yzy
**最后修改日期: 2015年1月19日
**描        述: AD管理进程函数配置头文件
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2015年1月19
** 描　述: 原始版本
******************************************************************************/
#ifndef _DEV_AD_H_
#define _DEV_AD_H_

#ifndef EXT_DEV_AD
#define EXT_DEV_AD extern
#endif 


#if (SYS_AD_EN > 0)

#if AD_NUM > 0

/******************************************************************************
**GPI输入端口扫描结构体信息
******************************************************************************/
typedef struct
{

    uint32 value[AD_NUM];          //最多8通道

}ADC;

EXT_DEV_AD  ADC* gsp_AdStt;


#endif
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
int SYS_AD_Scan(uint8_t ch, uint32_t * value);

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
uint32 SYS_AD_GetValue(uint8 di);

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
void SYS_AD_Init(void);

void SYS_AD_Idel(void);


#endif

#endif
