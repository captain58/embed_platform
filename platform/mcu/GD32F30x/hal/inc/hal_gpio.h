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
** Description: 文件创建.
** 1)由于PORTJ寄存器地址不连续,为了设计效率考虑.先不支持177/176 pin和 145/144pin
**   特有的PORTG\PORTJ.
*************************************************************************/
#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

#ifndef EXT_HAL_GPIO
#define EXT_HAL_GPIO extern
#endif 





/************************************************************************
 * GPIO端口枚举宏
 ************************************************************************/
#define GPIO_PORT_0      0        
#define GPIO_PORT_1      1        
#define GPIO_PORT_2      2        
#define GPIO_PORT_3      3        
#define GPIO_PORT_4      4        
#define GPIO_PORT_5      5        
#define GPIO_PORT_6      6        
#define GPIO_PORT_7      7        
#define GPIO_PORT_8      8        
#define GPIO_PORT_9      9        
#define GPIO_PORT_A      0x0A        
#define GPIO_PORT_B      0x0B        
#define GPIO_PORT_C      0x0C        
#define GPIO_PORT_D      0x0D        
#define GPIO_PORT_E      0x0E        
#define GPIO_PORT_F      0x0F        
       

/************************************************************************
 * GPIO端口枚举宏
 ************************************************************************/
//#define GPIO_PIN_0      0        
//#define GPIO_PIN_1      1        
//#define GPIO_PIN_2      2        
//#define GPIO_PIN_3      3        
//#define GPIO_PIN_4      4        
//#define GPIO_PIN_5      5        
//#define GPIO_PIN_6      6        
//#define GPIO_PIN_7      7        


//typedef struct _GPIO_CFG_ST_
//{
//    uint32_t GPIOx;
//    uint32_t mode;
//    uint32_t speed;
//    uint8_t pin;
//    uint8_t dir;
//}GPIO_CFG_ST;






/************************************************************************
 * @Function: HAL_GPIO_GetPinDIR
 * @Description: 读取管脚输入方向
 * 
 * @Arguments: 
 * @param: port 组号
 * @param: pin 具体管脚编号
 * @param: output 
 * @Auther: yzy
 * Date: 2015/5/10
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
//STATIC INLINE bool HAL_GPIO_GetPinDIR(uint8 port, uint8 pin)
//{
//    return (bool)(Chip_GPIO_GetPinDIR(LPC_GPIO, port, pin));
//}




/************************************************************************
 * @Function: HAL_GPIO_SetPinDIR
 * @Description: 设置管脚输入方向
 * 
 * @Arguments: 
 * @param: port 组号
 * @param: pin 具体管脚编号
 * @param: output 
 * @Auther: yzy
 * Date: 2015/5/10
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
//STATIC INLINE void HAL_GPIO_SetPinDIR(uint8 port, uint8 pin, bool output)
//{
//    if(output)
//    {
//        Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
//    }
//    else
//    {
//        Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
//    }
//}



/************************************************************************
 * @Function: HAL_GPIO_GetPinState
 * @Description: 读取管脚输入值
 * 
 * @Arguments: 
 * @param: port 组号
 * @param: pin 具体管脚编号
 * 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/8
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
STATIC INLINE bool HAL_GPIO_GetPinState(void * GPIOx, uint8 pin)
{
    return (bool)(gpio_input_bit_get(((COMPORT *)GPIOx)->pingrp, 1 << pin));//&((GPIO_TypeDef  *)GPIOA)[port]
}


//
///************************************************************************
// * @Function: HAL_GPIO_SetPinState
// * @Description: 设置管脚输出
// * 
// * @Arguments: 
// * @param: port 组号
// * @param: pin 具体管脚编号
// * 
// * @Return: bool  
// * @Auther: yzy
// * Date: 2015/5/8
// *-----------------------------------------------------------------------
// * @History: 
// ************************************************************************/
STATIC INLINE void HAL_GPIO_SetPinState(void  * GPIOx, uint8 pin, uint8_t setting)
{
    gpio_bit_write(((COMPORT *)GPIOx)->pingrp, 1 << pin, (bit_status)setting);
//    HAL_GPIO_WritePin(GPIOx, 1 << pin, (GPIO_PinState)setting);
}
//
//
//
//
/************************************************************************
 * @Function: HAL_InitGPIO
 * @Description: 初始化GPIO端口
 * @Arguments: 
 * @Auther: yzy
 * Date: 2015/5/8
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void HAL_InitGPIO(void);




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
void HAL_GPIO_PinConfig(COMPORT * pGpio);







#endif




