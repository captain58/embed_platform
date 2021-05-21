/******************************Copyright(c)******************************
**                           
**
** File Name: dev_gpo.c
** Author: 
** Date Last Update: 2019-11-02
** Description: ��ͨ������豸
** Note: 
*******************************History***********************************
** Date: 2019-11-01
** Author: yzy
** Description: 
*************************************************************************/
#define EXT_DEV_PWM
//	#define EXT_IIC_LSENSOR

#include "sys.h"
#include "hal.h"
#include "bsp.h"
#include "math.h"
#include "stdlib.h"
#include "log.h"
#include <string.h>
#include "main.h"
uint32_t g_nPow = 40;//40W
uint32_t g_mLmPerW = 100;
unsigned char display[9]={0,0,0,0,0,' ','l','u','x'};//��ʾ����

TESRes gs_iicRes;

ST_PWM_Sensor_CLBRV g_stCalib;
ST_PWM_Sensor_CLBRV g_stCalibBak;
/*********************************************
����ת��,ʮ����������ת����10����
����ʮ�����Ʒ�Χ��0x0000-0x270f��0-9999��
����ֳɸ�ʮ��ǧλ����ascii������ʾ��
**********************************************/
//	void conversion(unsigned int i)  
//	{  
//	   	display[0]=i/10000+0x30 ;
//	    i=i%10000;    //ȡ������
//		display[1]=i/1000+0x30 ;
//	    i=i%1000;    //ȡ������
//	    display[2]=i/100+0x30 ;
//	    i=i%100;    //ȡ������
//	    display[3]=i/10+0x30 ;
//	    i=i%10;     //ȡ������
//	    display[4]=i+0x30;  
//	}

ST_PWM_Sensor_CLBRV * SYS_PWM_Get_Calibration(void)
{
    if(0 == memcmp((uint8_t *)&g_stCalib,(uint8_t *)&g_stCalibBak,sizeof(ST_PWM_Sensor_CLBRV)))
    {
        return &g_stCalib;
    }
    else
    {
        if(g_stCalib.crc16 == DoCrc16((uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555))
        {
            memcpy((uint8_t *)&g_stCalibBak, (uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV));
            return &g_stCalib;
        }
        else if(g_stCalibBak.crc16 == DoCrc16((uint8_t *)&g_stCalibBak, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555))
        {
            memcpy((uint8_t *)&g_stCalib, (uint8_t *)&g_stCalibBak, sizeof(ST_PWM_Sensor_CLBRV));
            return &g_stCalib;
        }
        else
        {
            return NULL;
        }
    }
}

uint8_t  SYS_PWM_Set_Calibration(ST_PWM_Sensor_CLBRV * para)
{
    memset((uint8_t *)&g_stCalib,0,sizeof(ST_PWM_Sensor_CLBRV));

    memcpy(&g_stCalib, (uint8_t *)para, sizeof(ST_PWM_Sensor_CLBRV));
    g_stCalib.crc16 = DoCrc16((uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555);

    memcpy((uint8_t *)&g_stCalibBak, (uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV));

    return 0;
}

static uint8_t _Set_Calibration_Sensor(uint32_t value, uint32_t target)
{
    if(0 == memcmp((uint8_t *)&g_stCalib,(uint8_t *)&g_stCalibBak,sizeof(ST_PWM_Sensor_CLBRV)))
    {
        g_stCalib.calibration_value = value;
        g_stCalib.std_value = target;
        g_stCalib.crc16 = DoCrc16((uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555);
        
        memcpy((uint8_t *)&g_stCalibBak, (uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV));
        return 0;
    }
    else
    {
        if(g_stCalib.crc16 == DoCrc16((uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555))
        {
            g_stCalib.calibration_value = value;
            g_stCalib.std_value = target;
            g_stCalib.crc16 = DoCrc16((uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555);
            memcpy((uint8_t *)&g_stCalibBak, (uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV));
            return 0;
        }
        else if(g_stCalibBak.crc16 == DoCrc16((uint8_t *)&g_stCalibBak, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555))
        {
        
            g_stCalibBak.calibration_value = value;
            g_stCalib.std_value = target;
            g_stCalibBak.crc16 = DoCrc16((uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555);
            memcpy((uint8_t *)&g_stCalib, (uint8_t *)&g_stCalibBak, sizeof(ST_PWM_Sensor_CLBRV));
            return 0;
        }
        else
        {
            return 1;
        }
    }
}
static uint8_t _Set_Calibration_EnvFix(float fvalue, uint32_t target, int32_t ftarget)
{
    if(0 == memcmp((uint8_t *)&g_stCalib,(uint8_t *)&g_stCalibBak,sizeof(ST_PWM_Sensor_CLBRV)))
    {
        g_stCalib.ffix = fvalue;
        g_stCalib.std_value = target;
        g_stCalib.std_offset = ftarget;
        g_stCalib.crc16 = DoCrc16((uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555);
        
        memcpy((uint8_t *)&g_stCalibBak, (uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV));
        return 0;
    }
    else
    {
        if(g_stCalib.crc16 == DoCrc16((uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555))
        {
            g_stCalib.ffix = fvalue;
            g_stCalib.std_value = target;
            g_stCalib.std_offset = ftarget;
            g_stCalib.crc16 = DoCrc16((uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555);
            memcpy((uint8_t *)&g_stCalibBak, (uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV));
            return 0;
        }
        else if(g_stCalibBak.crc16 == DoCrc16((uint8_t *)&g_stCalibBak, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555))
        {
        
            g_stCalibBak.ffix = fvalue;
            g_stCalib.std_value = target;
            g_stCalib.std_offset = ftarget;
        
            g_stCalibBak.crc16 = DoCrc16((uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555);
            memcpy((uint8_t *)&g_stCalib, (uint8_t *)&g_stCalibBak, sizeof(ST_PWM_Sensor_CLBRV));
            return 0;
        }
        else
        {
            return 1;
        }
    }
}

int32_t SYS_PWM_Sensor_Calibrate(uint32_t valve, uint32_t target)
{
//	    SYS_SEM_Wait(&gs_iicRes, 0);         //�ȴ���Դ�ı��ͷ�

//	    uint8_t ret=I2C_LS_WriteLX(0x01);                //BH1750 ��ʼ�� 
//    uint8_t i = 3;
    uint32_t lux_data = 0;
//    uint16_t ud = 0;
//    uint8_t data[2];
    int32_t diff = 0;
//	    msleep(500);          
//	    while(i--){                          //ѭ��   
//	        ret=I2C_LS_WriteLX(0x01);                //power on
//	        msleep(30);  
//	        ret=I2C_LS_WriteLX(0x10);                //H- resolution mode
//	        msleep(180);                   //��Լ180ms
//	        if(ret==0){
//	            I2C_LS_ReadLX(data);          //��iic���߶�ȡ��ֵ    
//	//              ud = (data[0] << 8) + data[1];
//	            lux_data+=(float)((data[0] << 8) + data[1])/1.2; //pdf�ĵ���7ҳ
//	
//	
//	        }  
//	
//	    }
    lux_data = SYS_SensorLumen_Get();

//	    SYS_SEM_Release(&gs_iicRes);

    diff = (int32_t)(valve - (int32_t)lux_data);


//	    memset((uint8_t *)&g_stCalib,0,sizeof(ST_PWM_Sensor_CLBRV));
//	    g_stCalib.calibration_value = diff;
//	    g_stCalib.crc16 = DoCrc16((uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555);
//	
//	    memcpy((uint8_t *)&g_stCalibBak, (uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV));
    _Set_Calibration_Sensor(diff, target);


    return diff;
}
//����У׼
float SYS_PWM_Env_Calibrate(uint32_t value, uint32_t target, uint32_t * retTarget)
{

//	    SYS_SEM_Wait(&gs_iicRes, 0);         //�ȴ���Դ�ı��ͷ�

//	    uint8_t ret=I2C_LS_WriteLX(0x01);                //BH1750 ��ʼ�� 
//	    uint8_t i = 3;
    uint32_t lux_data = 0;
//    uint16_t ud = 0;
//    uint8_t data[2];
    float fix = 1;
    float rate = 1;
    int32_t fixTarget;
    if(target == 0 || value == 0) return 0;
    
//	    HAL_PWM_OnOff(1);//��������
    
//	    msleep(500);          
//	    while(i--){                          //ѭ��   
//	        ret=I2C_LS_WriteLX(0x01);                //power on
//	        msleep(30);  
//	        ret=I2C_LS_WriteLX(0x10);                //H- resolution mode
//	        msleep(180);                   //��Լ180ms
//	        if(ret==0){
//	            I2C_LS_ReadLX(data);          //��iic���߶�ȡ��ֵ    
//	//              ud = (data[0] << 8) + data[1];
//	            lux_data+=(float)((data[0] << 8) + data[1])/1.2; //pdf�ĵ���7ҳ
//	
//	
//	        }  
//	
//	    }
//	    lux_data /= 3;
//	
//	    SYS_SEM_Release(&gs_iicRes);
    lux_data = SYS_SensorLumen_Get();
    if(lux_data != 0)
    {
        fix = (float)value/lux_data;

        rate = (float)value / target;
        fixTarget = target - (int32_t)((float)lux_data / rate);
        *retTarget = fixTarget;

    //	    memset((uint8_t *)&g_stCalib,0,sizeof(ST_PWM_Sensor_CLBRV));
    //	    g_stCalib.calibration_value = diff;
    //	    g_stCalib.crc16 = DoCrc16((uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555);
    //	
    //	    memcpy((uint8_t *)&g_stCalibBak, (uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV));
        _Set_Calibration_EnvFix(fix, target, fixTarget);
    }

    return fix;






}




/************************************************************************
 * @Function: SYS_PWM_Init
 * @Description: pwm���ܳ�ʼ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_PWM_Init(ST_PWM_Sensor_CLBRV * para)
{
#if (SYS_PWM_EN > 0)

    const COMPORT* cp = &gs_PwmGpio;     //�˿�����

    HAL_GPIO_PinConfig(&cp->gpio[cp->pingrp], cp->pinnum, cp->modefunc, cp->dir);
                                                

    HAL_GPIO_SetPinState(&cp->gpio[cp->pingrp], cp->pinnum, 0);

    HAL_Init_PWM(0);
    Init_I2C_LS(&gs_LSensorPort);
    SYS_SEM_Create(1, &gs_iicRes);

    memset((uint8_t *)&g_stCalib,0,sizeof(ST_PWM_Sensor_CLBRV));
//	    g_stCalib.calibration_value = calibration_value;
//	    g_stCalib.ffix = fix;
    memcpy(&g_stCalib, (uint8_t *)para, sizeof(ST_PWM_Sensor_CLBRV));
    g_stCalib.crc16 = DoCrc16((uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV)-2, 0x5555);

    memcpy((uint8_t *)&g_stCalibBak, (uint8_t *)&g_stCalib, sizeof(ST_PWM_Sensor_CLBRV));
    
#endif
}


/************************************************************************
 * @Function: SYS_PWM_Adj
 * @Description: ����������
 * 
 * @Arguments: 
 * @param: curlx            ��ǰ����ǿ�ȣ���λlx�տ�˹
 * @param: targetlx         Ŀ�����ǿ�ȣ���λlx�տ�˹
 * @param: area             ���
 * @param: lednum            led�Ƹ���
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8_t SYS_PWM_Adj(int32_t curlx, int32_t targetlx, uint32_t area, uint16_t lednum, uint32_t ledpara)
{
    SYS_VAR_CHECK(lednum == 0);
    //��������
    
    ST_PWM_Sensor_CLBRV *pstfix = SYS_PWM_Get_Calibration();
    
    int32_t targetlm = (targetlx) * area / lednum;
    int32_t curlm = curlx * area / lednum;
    int32_t adjlm = targetlm - curlm;
    if(abs(adjlm) > ledpara)//g_nPow*g_mLmPerW)
    {
        if(adjlm > 0)
        {
            adjlm = ledpara;//g_nPow*g_mLmPerW;
        }
        else
        {
            adjlm = -ledpara;//g_nPow*g_mLmPerW;
        }
    }
    LOG_DEBUG(DBGFMT"SYS_PWM_Adj lm=[%d]\n",DBGARG,adjlm);
    
    //��������
    if(adjlm != 0 && pstfix->ffix != 0)
    {
        return HAL_Adj_PWM(ledpara, adjlm, pstfix->ffix );//g_nPow*g_mLmPerW
    }
    else
    {
        return 0;
    }

}

//calibrate
int32_t SYS_SensorLumen_Get(void)
{

    SYS_SEM_Wait(&gs_iicRes, 0);         //�ȴ���Դ�ı��ͷ�
    uint8_t i = 2;
    float lux_data = 0;
    uint8_t data[20];
    uint8_t count=0;
    uint8_t ret=I2C_LS_WriteLX(0x01);                //BH1750 ��ʼ�� 
    uint32_t ud = 0;
    msleep(10);          
    while(i--){                          //ѭ��   
        ret=I2C_LS_WriteLX(0x01);                //power on
        msleep(10);  
        ret=I2C_LS_WriteLX(0x10);                //H- resolution mode
        msleep(180);                   //��Լ180ms
        if(ret==0){
            I2C_LS_ReadLX(data);          //��iic���߶�ȡ��ֵ    
//              ud = (data[0] << 8) + data[1];
            ud = (data[0] << 8) + data[1];
            //((data[0] << 8) + data[1]);///1.2; //pdf�ĵ���7ҳ
            if(ud < 1000)
            {
                lux_data+=(float)ud;
                count++;
              //break;
            }
              

        }  

    }
    if(count > 0)
        lux_data = lux_data / count;
    else
        lux_data = 0;
    SYS_SEM_Release(&gs_iicRes);
    ST_PWM_Sensor_CLBRV *pstfix = SYS_PWM_Get_Calibration();
    if(NULL != pstfix)
    {
        lux_data+=pstfix->calibration_value;
//	        lux_data*=pstfix->ffix;
    }
    return (int32_t)lux_data;
}


/************************************************************************
 * @Function: SYS_PWM_Proc
 * @Description: pwm������� 
 * 1�տ�˹=1�����Ĺ�ͨ�����ȷֲ���1ƽ��������ϵ��նȡ���: 1lux=1lm/ƽ���ס� �տ�˹���նȵĵ�λ,����Ϊlux��lx;�����ǹ�ͨ���Ĺ��ʵ�λ,����Ϊlm��
 * @Arguments: 
 * @param: lx           Ŀ����������λ�տ�˹
 * @param: retlx        ��ǰ��������λ�տ�˹
 * @param: retlm        ��ǰpwmռ�ձ�����
 * @param: area         ��� 
 * @param:  lednum       ������
 * @param:  ledpara       �Ʋ���������*(lm per kw)
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/6/1
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8_t SYS_PWM_Proc(uint32_t lx, uint32_t * retlx, uint32_t * retlm, int32_t area, uint16_t lednum, uint32_t ledpara)
{
    float lux_data = 0;

    uint32_t fixLx = lx - ((ST_PWM_Sensor_CLBRV *)SYS_PWM_Get_Calibration())->std_offset;

    lux_data = SYS_SensorLumen_Get();
//	    gs_SysVar.terstt.bit.lux = (uint16_t)lux_data;
    if(area  <= 0) area = 1;
    SYS_PWM_Adj((int32_t)lux_data,fixLx,area,lednum, ledpara);
    msleep(500);  
    
    lux_data = SYS_SensorLumen_Get();

    gs_SysVar.terstt.bit.lux = (uint16_t)lux_data;
    *retlm = HAL_PWM_GetLm();
    *retlx = (uint32_t)(lux_data);
    if(CON_PWM_OFF == HAL_PWM_STS() && lux_data > (fixLx))
    {
        LOG_DEBUG(DBGFMT"CON_PWM_OFF but lm is good enough lx=[%d]\n",DBGARG,(uint32_t)(lux_data));
        return 0;
    }
    if(CON_PWM_ON == HAL_PWM_STS() && lux_data < (fixLx))
    {
        LOG_DEBUG(DBGFMT"CON_PWM_ON but lm is not enough lx=[%d]\n",DBGARG,(uint32_t)(lux_data));
        return 0;
    }

    
    lux_data /= (float)(fixLx);

    
    if(CON_PWM_OFF == HAL_PWM_STS())
    {
        if(lux_data >= 1.1 || lux_data <= 0.90)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(lux_data >= 1.02 || lux_data <= 0.98)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    
}


uint8_t SYS_PWM_Check(uint32_t lx,  uint32_t  * rlx, uint32_t * retlm)
{
//    uint8_t ret = 0;//I2C_LS_WriteLX(0x01);                //BH1750 ��ʼ�� 
    uint32_t lux_data = 0;
//    uint8_t data[2];

//    uint8_t i = 3;
//	    while(i--){                          //ѭ��   
//	        ret=I2C_LS_WriteLX(0x01);                //power on
//	        msleep(30); 
//	        ret=I2C_LS_WriteLX(0x10);                //H- resolution mode
//	        msleep(180);                   //��Լ180ms
//	        if(ret==0){
//	            I2C_LS_ReadLX(data);          //��iic���߶�ȡ��ֵ    
//	//              ud = (data[0] << 8) + data[1];
//	            lux_data+=(float)((data[0] << 8) + data[1])/1.2; //pdf�ĵ���7ҳ
//	
//	
//	        }  
//	
//	    }
    uint32_t fixLx = lx - ((ST_PWM_Sensor_CLBRV *)SYS_PWM_Get_Calibration())->std_offset;
    lux_data = SYS_SensorLumen_Get();
    gs_SysVar.terstt.bit.lux = (uint16_t)lux_data;

    *retlm = HAL_PWM_GetLm();
    *rlx = (uint32_t)(lux_data);
    if(CON_PWM_OFF == HAL_PWM_STS() && lux_data >= (fixLx))
    {
        //LOG_DEBUG(DBGFMT"CON_PWM_OFF but lm is good enough lx=[%d]\n",DBGARG,(uint32_t)(lux_data/3));
        return 0;
    }
    if(CON_PWM_ON == HAL_PWM_STS() && lux_data <= (fixLx))
    {
        //LOG_DEBUG(DBGFMT"CON_PWM_ON but lm is not enough lx=[%d]\n",DBGARG,(uint32_t)(lux_data/3));
        return 0;
    }   
    float rate = (float)lux_data / (float)(fixLx);
//    lux_data /= (float)(fixLx);
    if(CON_PWM_OFF == HAL_PWM_STS())
    {
        if(rate > 1.15 || rate < 0.85)
        {
            return 1;
        }
        else
        {
            return 0;
        }  

    }
    else
    {
        if(rate >= 1.1 || rate <= 0.90)
        {
            return 1;
        }
        else
        {
            return 0;
        }    
    }
}

void SYS_PWM_RC_ADJ(void)
{
    HAL_PWM_RCHF_ADJ();
}

