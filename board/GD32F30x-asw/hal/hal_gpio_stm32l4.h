/*
 * Copyright (C) 2015-2017 Captain Group Holding Limited
 */

#ifndef __HAL_GPIO_STM32L4_H
#define __HAL_GPIO_STM32L4_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l4xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "aos/kernel.h"

#define PINS_IN_GROUP  16
#define GROUP_GPIOA    0
#define GROUP_GPIOB    1	 
#define GROUP_GPIOC    2
#define GROUP_GPIOD    3	 
#define GROUP_GPIOE    4
	 
#define HAL_GPIO_0                 ((uint8_t)0)        /* represent GPIOA pin 0 */
#define HAL_GPIO_1                 ((uint8_t)1)        /* represent GPIOA pin 1 */
#define HAL_GPIO_2                 ((uint8_t)2)        /* represent GPIOA pin 2 */
#define HAL_GPIO_3                 ((uint8_t)3)        /* represent GPIOA pin 3 */
#define HAL_GPIO_4                 ((uint8_t)4)        /* represent GPIOA pin 4 */
#define HAL_GPIO_5                 ((uint8_t)5)        /* represent GPIOA pin 5 */
#define HAL_GPIO_6                 ((uint8_t)6)        /* represent GPIOA pin 6 */
#define HAL_GPIO_7                 ((uint8_t)7)        /* represent GPIOA pin 7 */
#define HAL_GPIO_8                 ((uint8_t)8)        /* represent GPIOA pin 8 */
#define HAL_GPIO_9                 ((uint8_t)9)        /* represent GPIOA pin 9 */
#define HAL_GPIO_10                ((uint8_t)10)       /* represent GPIOA pin 10 */
#define HAL_GPIO_11                ((uint8_t)11)       /* represent GPIOA pin 11 */
#define HAL_GPIO_12                ((uint8_t)12)       /* represent GPIOA pin 12 */
#define HAL_GPIO_13                ((uint8_t)13)       /* represent GPIOA pin 13 */
#define HAL_GPIO_14                ((uint8_t)14)       /* represent GPIOA pin 14 */
#define HAL_GPIO_15                ((uint8_t)15)       /* represent GPIOA pin 15 */
	 
#define HAL_GPIO_16                ((uint8_t)16)       /* represent GPIOB pin 0 */
#define HAL_GPIO_17                ((uint8_t)17)       /* represent GPIOB pin 1 */
#define HAL_GPIO_18                ((uint8_t)18)       /* represent GPIOB pin 2 */
#define HAL_GPIO_19                ((uint8_t)19)       /* represent GPIOB pin 3 */
#define HAL_GPIO_20                ((uint8_t)20)       /* represent GPIOB pin 4 */
#define HAL_GPIO_21                ((uint8_t)21)       /* represent GPIOB pin 5 */
#define HAL_GPIO_22                ((uint8_t)22)       /* represent GPIOB pin 6 */
#define HAL_GPIO_23                ((uint8_t)23)       /* represent GPIOB pin 7 */
#define HAL_GPIO_24                ((uint8_t)24)       /* represent GPIOB pin 8 */
#define HAL_GPIO_25                ((uint8_t)25)       /* represent GPIOB pin 9 */
#define HAL_GPIO_26                ((uint8_t)26)       /* represent GPIOB pin 10 */
#define HAL_GPIO_27                ((uint8_t)27)       /* represent GPIOB pin 11 */
#define HAL_GPIO_28                ((uint8_t)28)       /* represent GPIOB pin 12 */
#define HAL_GPIO_29                ((uint8_t)29)       /* represent GPIOB pin 13 */
#define HAL_GPIO_30                ((uint8_t)30)       /* represent GPIOB pin 14 */
#define HAL_GPIO_31                ((uint8_t)31)       /* represent GPIOB pin 15 */

#define HAL_GPIO_32                ((uint8_t)32)       /* represent GPIOC pin 0 */
#define HAL_GPIO_33                ((uint8_t)33)       /* represent GPIOC pin 1 */
#define HAL_GPIO_34                ((uint8_t)34)       /* represent GPIOC pin 2 */
#define HAL_GPIO_35                ((uint8_t)35)       /* represent GPIOC pin 3 */
#define HAL_GPIO_36                ((uint8_t)36)       /* represent GPIOC pin 4 */
#define HAL_GPIO_37                ((uint8_t)37)       /* represent GPIOC pin 5 */
#define HAL_GPIO_38                ((uint8_t)38)       /* represent GPIOC pin 6 */
#define HAL_GPIO_39                ((uint8_t)39)       /* represent GPIOC pin 7 */
#define HAL_GPIO_40                ((uint8_t)40)       /* represent GPIOC pin 8 */
#define HAL_GPIO_41                ((uint8_t)41)       /* represent GPIOC pin 9 */
#define HAL_GPIO_42                ((uint8_t)42)       /* represent GPIOC pin 10 */
#define HAL_GPIO_43                ((uint8_t)43)       /* represent GPIOC pin 11 */
#define HAL_GPIO_44                ((uint8_t)44)       /* represent GPIOC pin 12 */
#define HAL_GPIO_45                ((uint8_t)45)       /* represent GPIOC pin 13 */
#define HAL_GPIO_46                ((uint8_t)46)       /* represent GPIOC pin 14 */
#define HAL_GPIO_47                ((uint8_t)47)       /* represent GPIOC pin 15 */	

#define HAL_GPIOD_00               ((uint8_t)48)       /* represent GPIOD pin 0 */   
#define HAL_GPIOD_01               ((uint8_t)49)       /* represent GPIOD pin 1 */   
#define HAL_GPIOD_02               ((uint8_t)50)       /* represent GPIOD pin 2 */   
#define HAL_GPIOD_03               ((uint8_t)51)       /* represent GPIOD pin 3 */   
#define HAL_GPIOD_04               ((uint8_t)52)       /* represent GPIOD pin 4 */   
#define HAL_GPIOD_05               ((uint8_t)53)       /* represent GPIOD pin 5 */   
#define HAL_GPIOD_06               ((uint8_t)54)       /* represent GPIOD pin 6 */   
#define HAL_GPIOD_07               ((uint8_t)55)       /* represent GPIOD pin 7 */   
#define HAL_GPIOD_08               ((uint8_t)56)       /* represent GPIOD pin 8 */   
#define HAL_GPIOD_09               ((uint8_t)57)       /* represent GPIOD pin 9 */   
#define HAL_GPIOD_10               ((uint8_t)58)       /* represent GPIOD pin 10 */  
#define HAL_GPIOD_11               ((uint8_t)59)       /* represent GPIOD pin 11 */  
#define HAL_GPIOD_12               ((uint8_t)60)       /* represent GPIOD pin 12 */  
#define HAL_GPIOD_13               ((uint8_t)61)       /* represent GPIOD pin 13 */  
#define HAL_GPIOD_14               ((uint8_t)62)       /* represent GPIOD pin 14 */  
#define HAL_GPIOD_15               ((uint8_t)63)       /* represent GPIOD pin 15 */	 


#define HAL_GPIOE_00               ((uint8_t)64)       /* represent GPIOE pin 0 */ 
#define HAL_GPIOE_01               ((uint8_t)65)       /* represent GPIOE pin 1 */ 
#define HAL_GPIOE_02               ((uint8_t)66)       /* represent GPIOE pin 2 */ 
#define HAL_GPIOE_03               ((uint8_t)67)       /* represent GPIOE pin 3 */ 
#define HAL_GPIOE_04               ((uint8_t)68)       /* represent GPIOE pin 4 */ 
#define HAL_GPIOE_05               ((uint8_t)69)       /* represent GPIOE pin 5 */ 
#define HAL_GPIOE_06               ((uint8_t)70)       /* represent GPIOE pin 6 */ 
#define HAL_GPIOE_07               ((uint8_t)71)       /* represent GPIOE pin 7 */ 
#define HAL_GPIOE_08               ((uint8_t)72)       /* represent GPIOE pin 8 */ 
#define HAL_GPIOE_09               ((uint8_t)73)       /* represent GPIOE pin 9 */ 
#define HAL_GPIOE_10               ((uint8_t)74)       /* represent GPIOE pin 10 */
#define HAL_GPIOE_11               ((uint8_t)75)       /* represent GPIOE pin 11 */
#define HAL_GPIOE_12               ((uint8_t)76)       /* represent GPIOE pin 12 */
#define HAL_GPIOE_13               ((uint8_t)77)       /* represent GPIOE pin 13 */
#define HAL_GPIOE_14               ((uint8_t)78)       /* represent GPIOE pin 14 */
#define HAL_GPIOE_15               ((uint8_t)79)       /* represent GPIOE pin 15 */

#define HAL_GPIOF_00               ((uint8_t)80)       /* represent GPIOE pin 0 */ 
#define HAL_GPIOF_01               ((uint8_t)81)       /* represent GPIOE pin 1 */ 
#define HAL_GPIOF_02               ((uint8_t)82)       /* represent GPIOE pin 2 */ 
#define HAL_GPIOF_03               ((uint8_t)83)       /* represent GPIOE pin 3 */ 
#define HAL_GPIOF_04               ((uint8_t)84)       /* represent GPIOE pin 4 */ 
#define HAL_GPIOF_05               ((uint8_t)85)       /* represent GPIOE pin 5 */ 
#define HAL_GPIOF_06               ((uint8_t)86)       /* represent GPIOE pin 6 */ 
#define HAL_GPIOF_07               ((uint8_t)87)       /* represent GPIOE pin 7 */ 
#define HAL_GPIOF_08               ((uint8_t)88)       /* represent GPIOE pin 8 */ 
#define HAL_GPIOF_09               ((uint8_t)89)       /* represent GPIOE pin 9 */ 
#define HAL_GPIOF_10               ((uint8_t)90)       /* represent GPIOE pin 10 */
#define HAL_GPIOF_11               ((uint8_t)91)       /* represent GPIOE pin 11 */
#define HAL_GPIOF_12               ((uint8_t)92)       /* represent GPIOE pin 12 */
#define HAL_GPIOF_13               ((uint8_t)93)       /* represent GPIOE pin 13 */
#define HAL_GPIOF_14               ((uint8_t)94)       /* represent GPIOE pin 14 */
#define HAL_GPIOF_15               ((uint8_t)95)       /* represent GPIOE pin 15 */

#define GPIOA_SPEED	 GPIO_SPEED_FREQ_VERY_HIGH

#ifdef __cplusplus
}
#endif

#endif /* __HAL_GPIO_STM32L4_H */
