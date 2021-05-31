/******************************Copyright(c)******************************
**                          
**
** File Name: dev_gpi.h
** Author: 
** Date Last Update: 2019-11-02
** Description: ��ͨ������豸ͷ�ļ�
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#ifndef _DEV_GPI_H_
#define _DEV_GPI_H_

#ifndef EXT_DEV_GPI
#define EXT_DEV_GPI extern
#endif 





/************************************************************************
**GPI����˿�ɨ��ṹ����Ϣ
*************************************************************************/
typedef struct
{
    uint32 gpistt;                          //��������˿�״̬
    uint32 keyfevt;                         //�����½����¼�
    uint32 keyrevt;                         //�����������¼�
    uint32 keylevt;                         //���������¼�
    uint32 keyfevtbak;                      //�����½����¼�����
    uint32 keyrevtbak;                      //�����������¼�����
    uint32 keylevtbak;                      //���������¼�����
    //
    uint32 fgpistt;                         //��������˿�״̬
    uint32 fgpifevt;                        //���ٶ˿��½���
    uint32 fgpirevt;                        //���ٶ˿�������
    uint32 fgpifevtbak;                     //���ٶ˿��½��ر���
    uint32 fgpirevtbak;                     //���ٶ˿������ر���
    
    //ģ���ڲ�ʹ��,����ֻ��
#if (SYS_LGPI_EN > 0)
    uint8 cnt[LGPI_PORT_NUM];                //����˿�ɨ�������
    uint8 lastcnt[LGPI_PORT_NUM];            //��������ʱ�������
#endif
    
#if (SYS_FGPI_EN > 0)
    uint8 fcnt[FGPI_PORT_NUM];               //���ٶ˿�ɨ�������
#endif
}GPIO_IN;

EXT_DEV_GPI GPIO_IN * gsp_GpioStt;













/************************************************************************
*����˿��¼�����ö��
*************************************************************************/
typedef enum
{
    GPI_TYPE_LIFEVT = 0,            //�����½���
    GPI_TYPE_LIREVT,                //����������
    GPI_TYPE_LILEVT,                //��������
    GPI_TYPE_FIFEVT,                //�����½���
    GPI_TYPE_FIREVT,                //����������

}GPI_ENUM;


/************************************************************************
 * @Function: SYS_GPI_Init
 * @Description: �����ģ���ʼ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_GPI_Init(void);

void SYS_GPI_WakeUp(void);

/************************************************************************
 * @Function: SYS_GPI_EvtMessageLoop
 * @Description: GPI�¼���Ϣ�ַ�����,�ṩ����Ϣ��Ԫ
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_GPI_EvtMessageLoop(void);

/************************************************************************
 * @Function: SYS_GPI_GetStt
 * @Description: ��ȡ���������״̬
 * 
 * @Arguments: 
 * @param: type 0:���ٿ�����״̬
 *              1:���ٿ�����״̬
 * 
 * @Note: 
 * @Return: uint32  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint32 SYS_GPI_GetStt(uint8 type);

/************************************************************************
 * @Function: SYS_GPI_Event
 * @Description: ��ȡ������¼�
 * 
 * @Arguments: 
 * @param: type �¼�����ö��,����ΪGPI_ENUM
 * 
 * @Note: 
 * @Return: uint32  
 * @Auther: yzy
 * Date: 2015/5/27
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint32 SYS_GPI_Event(uint8 type);




#endif
