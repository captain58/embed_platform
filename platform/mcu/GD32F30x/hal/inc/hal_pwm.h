/******************************Copyright(c)******************************
**                                
**
** File Name: hal_pwm.h
** Author: 
** Date Last Update: 2015-05-18
** Description: pwm输出
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#ifndef _HAL_HW_PWM_H_
#define _HAL_HW_PWM_H_

#ifndef EXT_HAL_PWM
#define EXT_HAL_PWM    extern
#endif
#define CON_PWM_OFF     0
#define CON_PWM_ON      1
#define CON_PWM_CENTRE  2

void HAL_Init_PWM(uint32_t para);
uint8_t HAL_Adj_PWM(uint32_t ledlm, int32_t adj, float fix);
uint8_t HAL_PWM_Test(uint32_t init, uint32_t cmp);
uint8_t HAL_PWM_STS(void);
void HAL_PWM_RCHF_ADJ(void);
int32_t HAL_PWM_GetLm(void);
uint8_t HAL_PWM_OnOff(uint8_t onoff);

#endif
