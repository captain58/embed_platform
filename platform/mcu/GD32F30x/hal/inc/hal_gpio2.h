/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: gpio.c
**创   建   人: yzy
**最后修改日期: 2018年3月7日
**描        述: gpio(HAL)
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2018年3月7日
** 描　述: 原始版本
********************************************************************************************************/
#ifndef __FDW_MCU_GPIO_H__
#define __FDW_MCU_GPIO_H__

#ifndef _IRQSERVERFT_
#define _IRQSERVERFT_
    typedef void(*IRQServerFT)(void);
#endif

//	#ifndef EXT_GPIO
//	#define EXT_GPIO extern
//	#endif 
typedef struct __ST_EXIT_INT__
{
    GPIOx_Type * gpio;
    uint8_t pin;
    uint8_t type;
    IRQServerFT func;
}ST_EXIT_INT;
#define MCU_GPIO_INT_NUM 16

void HAL_InitGPIO();
/*******************************************************************************
 * @function_name:  MCU_GPIO_Config
 * @function_file: mcu_gpio.c
 * @描述: GPIO配置
 * 
 * 
 * @参数: 
 * @param: GPIOx:  寄存器 GPIOA...GPIOD
 * @param: Pin: 管脚序号
 * @param: Type: dir为1时，输出
 *        Type 0 = 普通 
 *        Type 1 = OD (OD功能仅部分特殊功能支持)
 *        Type 2 = 普通+上拉 
 *        Type 3 = OD+上拉
 *        dir为0时，输入
 *        Type 0 = 普通 
 *        Type 1 = 上拉
 * @param: analog: 1模电，0数电
 * @param: dir: 1输出，0输入
 * @返回: 
 * @作者: yzy (2018-09-27)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
void HAL_GPIO_PinConfig(GPIOx_Type* GPIOx, uint8_t Pin, uint8_t Type, uint8_t analogen, uint8_t dir);

/*******************************************************************************
 * @function_name:  MCU_GPIO_Ext_Int
 * @function_file: mcu_gpio.c
 * @描述: GPIO外部中断配置
 * 
 * 
 * @参数: 
 * @param: channel: 中断通道，总共提供16个通道 
 * @param: GPIOx:  寄存器 GPIOA...GPIOD
 * @param: Pin: 管脚序号
 * @param: Type: 低四位
 *        Type 0 = //上升沿
 *        Type 1 = //下降沿
 *        Type 2 = //上升和下降沿
 *        Type 3 = 关闭
 *        高四位，输入
 *        0 = NWKUP功能关闭
 *        1 = NWKUP功能开启
 * @param: func: 中断回调函数
 * @返回:     0成功，1失败
 * @作者: yzy (2018-09-27)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint8_t HAL_GPIO_Ext_Int(uint8_t channel, GPIOx_Type* GPIOx, uint8_t Pin, uint8_t Type, IRQServerFT func);
/*******************************************************************************
 * @function_name:  HAL_GPIO_SetPinState
 * @function_file: mcu_gpio.c
 * @描述: GPIO管脚设置
 * 
 * @参数: 
 * @param: GPIOx:  寄存器 GPIOA...GPIOD
 * @param: Pin: 管脚序号
 * @param: value: 1拉高，0拉低
 * @返回: 
 * @作者: yzy (2018-09-27)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
STATIC INLINE void HAL_GPIO_SetPinState(GPIOx_Type* GPIOx, uint8_t Pin, uint8_t value)
{
    uint32_t pinbit = 1 << Pin;

    if(value)
    {
        GPIO_SetBits(GPIOx, pinbit);
    }
    else
    {
        GPIO_ResetBits(GPIOx, pinbit);

    }
}
/*******************************************************************************
 * @function_name:  HAL_GPIO_GetPinState
 * @function_file: mcu_gpio.c
 * @描述: GPIO管脚状态读取
 * 
 * @参数: 
 * @param: GPIOx:  寄存器 GPIOA...GPIOD
 * @param: Pin: 管脚序号
 * @返回:     1高电平，0低电平
 * @作者: yzy (2018-09-27)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
STATIC INLINE uint8_t HAL_GPIO_GetPinState(GPIOx_Type* GPIOx, uint8_t Pin)
{
    uint32_t pinbit = 1 << Pin;
    return GPIO_ReadInputDataBit(GPIOx, pinbit);
}

#endif

