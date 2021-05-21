/**
 * @file      dev_pwrchk.h
 * @brief     ����ԴAD����������Ӧ��ͷ�ļ�
 * @details   ��ͷ�ļ������˵�ԴAD������״̬�����ƽṹ�弰��ؽӿڣ�
              Ҳ�����˵�Դ״̬ת������������ϵͳ�������е������ѹ�ֽ�㣻
              Ҳ��������������ϵͳ�������е������ѹ��������
              �������˵�Դ״̬�л��Ļ��͵�ѹֵ����ֹ��ѹ΢С������ɵ����С�
 * @mainpage  ����Ӧ��
 * @author    yzy
 * @email     
 * @version   1.0.0
 * @date      2019-3-18
 * @license   captain
 */
#ifndef _PROC_POWER_ADC_H_
#define _PROC_POWER_ADC_H_
/*------------------------------------------------------------------------------------
************************************���Ͷ���******************************************
------------------------------------------------------------------------------------*/

/** ��Դ״̬ö�� */
typedef enum _ET_POW_STA_	
{
	POWUNKNOW = 0,	///<״̬����״̬Ϊ0ʱֹͣ�жϣ�������ʽ״̬��1��ʼ
	POWNOR,			///<��Դ����̬
	POWOWN,			///<��ԴǷѹ̬
	POWLOW,			///<��Դ��ѹ̬
	POWCUT,			///<��Դ����̬
}ET_POW_STA;
/** ��Դ״̬ת��ö�� */

typedef enum _ET_POW_STA_CHG_	
{
	pNorToOwn = 0,	///<������Ƿѹ
	pNorToLow,		///<��������ѹ
	pNorToCut,		///<����������
	pOwnToNor,		///<Ƿѹ������
	pOwnToLow,		///<Ƿѹ����ѹ
	pOwnToCut,		///<Ƿѹ������
	pLowToNor,		///<��ѹ������
	pLowToOwn,		///<��ѹ��Ƿѹ
	pLowToCut,		///<��ѹ������
	pCutToNor,		///<���������
	pCutToOwn,		///<�����Ƿѹ
	pCutToLow,		///<�������ѹ
}ET_POW_STA_CHG;

/** 
 * @brief �����ѹ��ע��ṹ��
 * @details �����ѹ��ע��ṹ�壬����ǰ��ѹ״ָ̬�룬��ǰ״̬
 */
typedef struct _ST_SPC_VOL_PNT_		
{
	unsigned char   *State;		///<�ش�״ָ̬��
	unsigned char   Cursta;		///<��ǰ״̬
	unsigned char   Presta;		///<��һ��״̬
	unsigned char   SetTimes;	///<�趨�Ƚϴ���
	unsigned char   SameTimes;	///<�����бȽϽ����ͬ�Ĵ���
	unsigned short  Voltage;	///<ע��Ƚϵ�ѹ��
}ST_SPC_VOL_PNT;

/** ��Դ��ѹ״̬ת����,��λmV */
typedef struct _ST_POW_ALM_PNT_		
{
	unsigned short  VolOwn;		///<Ƿѹ�� 
	unsigned short  VolLow;		///<��ѹ��
	unsigned short  VolCut;		///<�����
}ST_POW_ALM_PNT;
/** ��Դ״̬�¼���ṹ�� */
typedef struct _ST_FSM_TABLE
{
    short           CurState;			///> ��ǰ״̬
    short           Event;				///> �¼�
    short           EventActID;//void            (*EventAct)();		///> ����ָ��
    short           NextState;			///> ��һ��״̬
}ST_FSM_TABLE, *P_ST_FSM_TABLE;

/** ��Դ״̬�任����ṹ�� */
typedef struct _ST_FSM
{
	short               CurState;		///> ��ǰ״̬
	short               PreState;		///> ǰһ״̬
	const ST_FSM_TABLE  *FsmTable;		///> ״̬�¼���ָ��
}ST_FSM, *P_ST_FSM;
/*------------------------------------------------------------------------------------
*************************************�궨��*******************************************
------------------------------------------------------------------------------------*/
#define SYS_START_VOLT			4000///<ϵͳ������ѹ����λmV��4�ڼ�����ã�,�����˵�ѹ���߲ŻῪʼ���ر�߲������г�ʼ����

#define POWVOL_FUL				5800///<����������ʾ��ѹ��,��λmV
#define POWVOL_OWN				4800//5200///<����Ƿѹת����ѹ��,��λmv
#define POWVOL_LOW				4570//5000/<�����ѹת����ѹ��,��λmv
#define POWVOL_CUT				3600//1000///<�������ת����ѹ��,��λmv

#define HYSTERESISVOL			100	///<100mV�����͵�ѹ 

#ifndef ABOVE
    #define ABOVE				1
    #define BELOW				0
#endif


/*------------------------------------------------------------------------------------
****************************ȫ��ʹ�ñ������ӿں�������********************************
------------------------------------------------------------------------------------*/
/**
 * @brief ��Դ��ʼ���м��ȴ�����
 * @details ��ʼ����Դģ��Ӳ���ӿڼ������ʼ״̬����ȡ���Դ��ѹֵ����δ�ﵽϵͳ������ѹ
 *      ֵ����ʾ��HELLO��15����ʾ���ﵽϵͳ������ѹ�������ϵ��Դ�¼����ϵ糣ѹ���ϵ�Ƿ
 *      ѹ���ϵ��ѹ�����������ÿ3������һ�εĵ�Դ��ѹ�������󣬽�����
 * @param �޲���
 * @return ���غ󣬱�ʾ�⹩���ѹ�������ȴ����
 * @retval ����1���ɹ�
 * @note �ϵ�����ʱ���ȵ���
 * @attention �ڵ��ô洢�ӿ�ǰ����Ҫ�ȵ��ñ��ӿڣ���֤����������
 * @warning Ĭ��ϵͳ������ѹ��������4�ڼ�����Σ����״��ϵ糬�����ݳ����أ���Ҫ����ġ�
 * @exception ������ѹ�쳣������ʾHELLO��ʾ��
 */
int InitSysPowManage(void);

/**
 * @brief ��ȡϵͳ�ⲿ��Դ״̬�ӿ�
 * @details ��ȡϵͳ��Դ��ǰ״̬����δ֪����ѹ��Ƿѹ����ѹ�͵��硣
 * @param �޲���
 * @return ֱ�ӷ���ö�ٽṹ��_ET_POW_STA��ö��״̬
 * @retval 0δ֪��1��ѹ��2Ƿѹ��3��ѹ��4����
 * @note ��������
 * @attention 
 * @warning 
 * @exception 
 */
ET_POW_STA GetSysPowSta(void);

/**
 * @brief ��ѯϵͳ�ⲿ��Դ��ѹֵ�ӿ�
 * @details ��ȡϵͳ�ⲿ��Դ��ǰ��ѹֵ����ѹֵͨ��ADת��ֵ����·������ѹ��������ɡ�
 * @param �޲���
 * @return ֱ�ӷ��ص�ѹֵ����λmV
 * @retval ����ֵΪ�����ͣ���3001��ʾ3.001V
 * @note ��������
 * @attention 
 * @warning 
 * @exception 
 */
unsigned short GetSysPow(void);
/**
 * @brief ��Դ�������
 * @details ÿ3������һ�Σ�����ⲿ��Դ��ѹֵ��һ�ǽ���ѹֵ�������ѹ����бȽ��жϣ��ٸ��������ѹ
 *          ����״̬�����ǽ���ѹֵ��ϵͳ��Դ״̬��ѹ����бȽ��жϣ�ȷ�ϵ�ǰ״̬���������ĵ�Դ�¼�
 *          �����õ�Դ״̬ת�����к�����ת����Դ״̬�����е�Դ�¼���������������Դ�¼���
 * @param ��
 * @return ��
 * @retval ��
 * @note ��Ϊ�͹��Ķ�ʱ��������
 * @attention 
 * @warning 
 * @exception 
 */
void PowManageProcess(void);


void SVD_IRQHandlerWithPara(void * para);
void InitPwrChk(void);
int PwrProtect(void);
void Init_SVD(void);
void Close_SVD(void);

#endif
