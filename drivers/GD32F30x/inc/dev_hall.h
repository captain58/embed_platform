/***********************************************************************************************************************
* �ļ�����dev_hall.h
* ��  ����
* ��  ����������������Ӧ��ͷ�ļ�
* �����ߣ�
* ��  �ڣ�
***********************************************************************************************************************/
#ifndef _DEV_HALL_H_
#define _DEV_HALL_H_

/***********************************************************************************************************************
Macro definitions �궨��
***********************************************************************************************************************/
#define HALL_A          0
#define HALL_B          1

#define HALL_ISNORMAL   0
#define HALL_ISERR      1

/***********************************************************************************************************************
Typedef definitions ���Ͷ���
***********************************************************************************************************************/
//���������ṹ�壬��Ҫ����
typedef struct _ST_HallCount_
{
    unsigned char HallACount;
    unsigned char HallBCount;

}ST_HALL_COUNT;

//���������ṹ��
typedef struct _HallCheck_ST
{
    unsigned char   Run;                            //������������б�־ 0��δ���� 1������
    unsigned char   Step;                           //���������̣�0������   1���Ź������
    unsigned short  Count;                          //�������Ź����жϼ�ʱ���� 
    
    unsigned short  TimeFilter;                     //������ʱ
    unsigned short  TimeHallAttack;                 //�Ź�����ʱ
    unsigned char   (*GetPortInput)(void);          //��ȡ���ŵ�ƽ
    void            (*HallAttackFun)(void);         //�Ź����ص�����
    void		    (*HallPulseFun)(void);          //������Ч����ص�����
}HallCheck_ST, *pHallCheck_ST;

/***********************************************************************************************************************
Global functionsȫ�ֺ���
***********************************************************************************************************************/
extern ST_HALL_COUNT    HallCount;
/*
 *  ������ʼ��
 */
void InitHall(void);
/*
 * �Ź����жϣ���ʼ���Ͱ���ʱ���ã�ֻ������������¿�������ߴŹ���״̬λ����Ź������ſ���Ȩ
 */
void HallAttackCheck(void);
/*
 * ��ȡ��ǰHall״̬
 */
unsigned char GetHallState(void);

#endif