/******************************Copyright(c)******************************
**                          
**
** File Name: 
** Author: 
** Date Last Update: 2019-11-02
** Description: 
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#define EXT_HAL_GPIO

#include "sys.h"
#include "hal.h"



#define PCF_SWJCFG_MASK                    ((uint32_t)0xF8FFFFFFU)  /*!< AFIO_PCF register SWJCFG mask */


/************************************************************************
 * @Function: HAL_InitGPIO
 * @Description: 初始化GPIO端口
 * @Arguments: 
 * @Auther: yzy
 * Date: 2015/5/8
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void HAL_InitGPIO(void)
{
    //开启IOCON功率模块
//    Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_IOCON);
    
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);

//	    uint32_t temp_reg = AFIO_PCF0;
//	    temp_reg &= PCF_SWJCFG_MASK;
//	    temp_reg |= 0x04000000;
//	
//	    AFIO_PCF0 = temp_reg;
    rcu_periph_clock_enable(RCU_AF);

    gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP,ENABLE);

    //开启GPIO功率模块(传入参数用不到)
//	Chip_GPIO_Init(LPC_GPIO);
//    __HAL_RCC_GPIOF_CLK_ENABLE();
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    __HAL_RCC_GPIOB_CLK_ENABLE(); 
//    __HAL_RCC_GPIOC_CLK_ENABLE();
}




/************************************************************************
 * @Function: HAL_GPIO_PinConfig
 * @Description: 配置某个管脚的工作参数
 * 
 * @Arguments: 
 * @param: port 组号
 * @param: pin 管脚
 * @param: mode 配置 
 * @Auther: yzy
 * Date: 2015/5/8
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void HAL_GPIO_PinConfig(COMPORT * pGpio)
{
  
    gpio_init(pGpio->pingrp, pGpio->modefunc, pGpio->speed, 1 << pGpio->pinnum);
//    GPIO_InitTypeDef GPIO_InitStruct = {0};
//
////    uint32 modefunc = 0;
//    GPIO_InitStruct.Pin = 0x01 << pin;
//    //设置为多功能口\普通io口
//    switch(mode & GPIO_MODE_FUNCMASK)
//    {
//        case GPIO_MODE_FUNC0:
//            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//            break;
//            
//        case GPIO_MODE_FUNC1:
//            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//            break;
//            
//        case GPIO_MODE_FUNC2:
//            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//            break;
//            
//        case GPIO_MODE_FUNC3:
//            GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//            break;
//            
//        case GPIO_MODE_FUNC4:
//            GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//            break;
//            
//        case GPIO_MODE_FUNC5:
//            GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//            break;
//            
//        case GPIO_MODE_FUNC6:
//            GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//            break;
//            
//        case GPIO_MODE_FUNC7:
//            GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
//            break;
//        case GPIO_MODE_FUNC8:
//            GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
//            break;
//            
//        case GPIO_MODE_FUNC9:
//            GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
//            break;
//            
//        case GPIO_MODE_FUNCA:
//            GPIO_InitStruct.Mode = GPIO_MODE_EVT_FALLING;
//            break;
//        case GPIO_MODE_FUNCB:
//            GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING_FALLING;
//            break;
//                        
//        default:
//            //error
//            break;
//    }
//
//    //设置上下拉电阻
//    if(mode & GPIO_MODE_PULLUP)
//    {                                   //上拉
//        GPIO_InitStruct.Pull = GPIO_PULLUP;
//    }
//    else if(mode & GPIO_MODE_PULLDOWN)
//    {                                   //下拉
//        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//    }
//    else
//    {
//        GPIO_InitStruct.Pull = GPIO_NOPULL;
//    }
//    //设置IO速度
//    if(mode & GPIO_SPEED_MEDIUM)
//    {                                   ///*!< range  4 MHz to 10 MHz, please refer to the product datasheet */
//        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
//    }
//    else if(mode & GPIO_SPEED_HIGH)
//    {                                   ///*!< range 10 MHz to 50 MHz, please refer to the product datasheet */
//        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    }
//    else
//    {///*!< range up to 2 MHz, please refer to the product datasheet */
//        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    }
//
//
//    switch(mode & GPIO_MODE_AFMASK)
//    {
//        case GPIO_MODE_AF0:
//            GPIO_InitStruct.Alternate = GPIO_AF0;
//            break;
//        case GPIO_MODE_AF1:
//            GPIO_InitStruct.Alternate = GPIO_AF1;
//            break;        
//        case GPIO_MODE_AF2:
//            GPIO_InitStruct.Alternate = GPIO_AF2;
//            break;        
//        case GPIO_MODE_AF3:
//            GPIO_InitStruct.Alternate = GPIO_AF3;
//            break;        
//        case GPIO_MODE_AF4:
//            GPIO_InitStruct.Alternate = GPIO_AF4;
//            break;        
//        case GPIO_MODE_AF5:
//            GPIO_InitStruct.Alternate = GPIO_AF5;
//            break;   
//        case GPIO_MODE_AF6:
//            GPIO_InitStruct.Alternate = GPIO_AF6;
//            break;   
//        default:
//            GPIO_InitStruct.Alternate = GPIO_AF0;
//            break;
//    }
//    HAL_GPIO_Init(/*&((GPIO_TypeDef  *)GPIOA)[port]*/GPIOx, &GPIO_InitStruct);
    //设置开漏(output)
//    if(mode & GPIO_MODE_OPENDRAIN)
//    {
//        modefunc |= IOCON_OPENDRAIN_EN;
//    }
    
    //设置模拟口(仅对某些analog pin有效)
//    if(port == 0)
//    {
//        if(((pin >= 11) && (pin <= 16)) || ((pin >= 22) && (pin <= 23)))
//        {
//            if(!(mode & GPIO_MODE_ANALOG))
//            {
//                modefunc |= IOCON_DIGMODE_EN;
//            }
//        }
//    }
    
    //do setting
//    Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, modefunc);
//    
//    
//    //设置输出\输入
//    if(mode & GPIO_MODE_OUTPUT)
//    {
//        Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
//    }
//    else if(mode & GPIO_MODE_INPUT)
//    {
//        Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
//    }
    
}





