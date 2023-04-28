/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: dev_dec.h
**��   ��   ��: yzy
**����޸�����: 2018��3��7��
**��        ��: esos����ϵͳ�豸�������ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2018��3��7��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#ifndef _TASKS_H_
#define _TASKS_H_


#ifndef EXT_TASKS
#define EXT_TASKS   extern
#endif

/********************************************************* 
**������̿鳣������
**********************************************************/
typedef struct
{
    ktask_t * handle;
    ThreadFunc pfunc;    //���̵���������
    cpu_stack_t * stack;
    uint8 prior;        //���ȼ�
    uint8 tbl;          //��Ϣ��,���Ϊ0���ʾû����Ϣѭ������,������(yzy:�ڴ�������ʱ�����ж��Ƿ�����Ϣ�б�ṹ��)
    uint32 stklen;      //ջ�ĳ���(yzy:ò��û��)
    const name_t * name;        //��������
    kbuf_queue_t * msg;
    void * arg;
}KTaskConst;

/********************************************************* 
**���̵�����,��ϵͳ�����������������ȥ�������еĽ���
**���̵������̶���һ��section����
**********************************************************/
typedef struct{
    const KTaskConst * ktask;
    uint32 stklen;          //ջ�ĳ���,��λ:�ֽ�. yzy �����tcbc->stklen�к�����?ò�ƺ���û���õ�.
}KTaskDeclare;




EXT_TASKS  ktask_t* gs_TkHandles[SYS_TK_NUM];     //���н��̵�TCBָ��


/******************************************************************************
**main����
******************************************************************************/
#define TASK_MAIN_TKID           1
#define TASK_MAIN_STKL           384//
const extern KTaskConst gs_TkMAIN;

#define TASK_DECLARE_1_MAIN() {&gs_TkMAIN, TASK_MAIN_STKL}

/******************************************************************************
**main����
******************************************************************************/
#define TASK_RFMNG_TKID           10
#define TASK_RFMNG_STKL           384//
const extern KTaskConst gs_TkLP2PCmm;

#define TASK_DECLARE_10_RFMNG() {&gs_TkLP2PCmm, TASK_RFMNG_STKL}

/******************************************************************************
**����modem����
******************************************************************************/

#define DEV_MODEM_TKID           2
#define DEV_MODEM_STKL           1024//
const extern KTaskConst gs_TkDevModem;

#define TASK_DECLARE_2_MODEM() {&gs_TkDevModem, DEV_MODEM_STKL}

//	
//	/******************************************************************************
//	**PST����ͨѶ������
//	******************************************************************************/
//	#define TASK_PST_TKID           2
//	#define TASK_PST_STKL           512//512�����.FB����ʱ
//	extern ktask_t gs_TkPst;
//	#define TASK_DECLARE_2_PST() {&gs_TkPst, TASK_PST_STKL}
//	
//	
/******************************************************************************
**Զ��ͨ���������
******************************************************************************/
#define TASK_FARP_TKID          3
#define TASK_FARP_STKL          2048
const extern KTaskConst gs_TkFarp;
#define TASK_DECLARE_3_FARP() {&gs_TkFarp, TASK_FARP_STKL}
//	


/******************************************************************************
**netp����
******************************************************************************/
#define TASK_NETP_TKID          4
#define TASK_NETP_STKL          1024
extern const KTaskConst gs_TkNetp;
#define TASK_DECLARE_4_NETP() {&gs_TkNetp, TASK_NETP_STKL}

#define TASK_NETP2_TKID          5
//	#define TASK_NETP2_STKL          512
extern const KTaskConst gs_TkNetp2;
#define TASK_DECLARE_5_NETP() {&gs_TkNetp2, TASK_NETP_STKL}

#define TASK_NETP3_TKID          6
//	#define TASK_NETP3_STKL          512
extern const KTaskConst gs_TkNetp3;
#define TASK_DECLARE_6_NETP() {&gs_TkNetp3, TASK_NETP_STKL}

#define TASK_NETP4_TKID          7
//	#define TASK_NETP4_STKL          512
extern const KTaskConst gs_TkNetp4;
#define TASK_DECLARE_7_NETP() {&gs_TkNetp4, TASK_NETP_STKL}



/******************************************************************************
**hlv����
******************************************************************************/
#define TASK_HLV_TKID          2
#define TASK_HLV_STKL          1024
extern const KTaskConst gs_TkHLV;
#define TASK_DECLARE_2_HLV() {&gs_TkHLV, TASK_HLV_STKL}

#define TASK_SLV_TKID          9
#define TASK_SLV_STKL          192
extern const KTaskConst gs_TkSLV;
#define TASK_DECLARE_9_SLV() {&gs_TkSLV, TASK_SLV_STKL}

void SYS_TASKS_Init(void);


#endif


