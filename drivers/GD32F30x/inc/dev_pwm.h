/******************************Copyright(c)******************************
**                          
**
** File Name: dev_pwm.h
** Author: yzy
** Date Last Update: 2019-11-02
** Description: pwm�����ļ�
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#ifndef _DEV_PWM_H_
#define _DEV_PWM_H_

#ifndef EXT_DEV_PWM
#define EXT_DEV_PWM extern
#endif 

typedef struct _ST_PWM_SENSOR_CLBRV
{
    int32_t calibration_value;//������У׼
    uint32_t std_value;//������У׼��׼ֵ
    uint32_t ullux_by_calibrator;//����У׼ֵ
    float   ffix;//����У׼ϵ��
    int32_t std_offset;//��׼ֵƫ��
    uint16_t reserver;
    uint16_t crc16;
}ST_PWM_Sensor_CLBRV;

void SYS_PWM_Init(ST_PWM_Sensor_CLBRV * para);
uint8_t SYS_PWM_Adj(int32_t curlx, int32_t targetlx, uint32_t area, uint16_t lednum, uint32_t ledpara);
uint8_t SYS_PWM_Check(uint32_t lx,  uint32_t * const retlx, uint32_t * retlm);

uint8_t SYS_PWM_Proc(uint32_t lx, uint32_t * retlx, uint32_t * retlm, int32_t area, uint16_t lednum, uint32_t ledpara);
void SYS_PWM_RC_ADJ(void);
int32_t SYS_PWM_Sensor_Calibrate(uint32_t valve, uint32_t target);
int32_t SYS_SensorLumen_Get(void);
float SYS_PWM_Env_Calibrate(uint32_t valve, uint32_t target, uint32_t * retTarget);
ST_PWM_Sensor_CLBRV * SYS_PWM_Get_Calibration(void);
uint8_t  SYS_PWM_Set_Calibration(ST_PWM_Sensor_CLBRV * para);

#endif

