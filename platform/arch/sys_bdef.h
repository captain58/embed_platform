/******************************Copyright(c)******************************
**                         
**
** File Name: 
** Author: 
** Date Last Update: 2019-11-02
** Description: ƽ̨�����������Ͷ����ļ�
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#ifndef _SYS_BDEF_H_
#define _SYS_BDEF_H_



/************************************************************************
**���岻ͬ��������Ԥ����ָ���
*************************************************************************/
#ifdef __CC_ARM
#define INLINE  __inline
#else
#define INLINE inline
#endif


#define STATIC static



/************************************************************************
**��������1
*************************************************************************/
typedef signed char int8;               //8λ�з�����
typedef unsigned char uint8;            //8λ�޷�����
typedef short int16;                    //16λ�з�����
typedef unsigned short uint16;          //16λ�޷�����
typedef long int32;                     //32λ�з�����
typedef unsigned long uint32;           //32λ�޷�����
typedef signed long long int64;         //64λ�з�����
typedef unsigned long long uint64;      //64λ�޷�����
typedef uint8 Boolean;                  //Bool ����
typedef void* Object;                   //������������ΪObject
typedef const char * String;            //�����ַ�������
typedef int8 TResult;                   //���صĽ������
typedef void (*TCallback)(void);
typedef uint32 TStack;
typedef void (*TPCallback)(uint8);

typedef String string;
typedef uint8  BS8;



#include <stdbool.h>                    //����bool����


/************************************************************************
**��������2
*************************************************************************/
typedef unsigned char  BOOLEAN;                 /* ��������                                 */
typedef unsigned char  INT8U;                   /* �޷���8λ���ͱ���                        */
typedef signed   char  INT8S;                   /* �з���8λ���ͱ���                        */
typedef unsigned short INT16U;                  /* �޷���16λ���ͱ���                       */
typedef signed   short INT16S;                  /* �з���16λ���ͱ���                       */
typedef unsigned int   INT32U;                  /* �޷���32λ���ͱ���                       */
typedef signed   int   INT32S;                  /* �з���32λ���ͱ���                       */
typedef float          FP32;                    /* �����ȸ�����(32λ����)                 */
typedef double         FP64;                    /* ˫���ȸ�����(64λ����)                 */

typedef INT32U         OS_STK;                  /* ��ջ��32λ���                           */







/************************************************************************
**��������3
*************************************************************************/
typedef union{
    uint16 word;
    uint8 byte[2];
}Word16;


typedef union{
    uint32 lword;
    uint16 word[2];
    uint8 byte[4];
}Word32;

typedef union{
    uint64 qword;
    uint32 lword[2];
    uint16 word[4];
    uint8 byte[8];
}Word64;


/************************************************************************
**���ݳ���
*************************************************************************/
#ifndef __NULL
#define __NULL      ((void*)0)
#endif

#ifndef NULL
#define	NULL	    0
#endif






/************************************************************************
**����ϵͳ�������ؽ��
*************************************************************************/
#define SYS_ERR_OK          0          //û�д���
#define SYS_ERR_EXIST       (1)        //������
#define SYS_ERR_FT          (2)        //������������
#define SYS_ERR_NRES        (3)        //������Դ��ռ���޷�ɾ��
#define SYS_ERR_PRIOR       (4)        //���ȼ�����
#define SYS_ERR_TIME        (5)        //��ʱ����
#define SYS_ERR_NE          (6)        //��������
#define SYS_ERR_GT          (7)        //����Խ��
#define SYS_ERR_NSUSP       (8)        //����û�й���
#define SYS_ERR_TYPE        (10)       //���Ͳ�ƥ��
#define SYS_ERR_VAR         (11)       //��������
#define SYS_ERR_CHK         (12)       //��֤����ȷ
#define SYS_ERR_MAIN        (13)       //���洢������
#define SYS_ERR_BACK        (14)       //���ݴ洢������
#define SYS_ERR_WAIT        (15)       //�ȴ�
#define SYS_ERR_DEF         (16)       //ʹ��Ĭ������
#define SYS_ERR_NOREPLY     (17)       //����ظ�
#define SYS_ERR_DATA        (18)       //������
#define SYS_ERR_ENCRYPT     (19)       //���ܴ���
#define SYS_ERR_ADDR        (20)       //��ַ��
#define SYS_ERR_UNKNOW      (127)

#define ER(X)              (SYS_ERR_OK == (X))

#define SYS_VAR_CHECK(X)    {if(X){return SYS_ERR_VAR;}}
#define SYS_VAR_CHECK2(X)    {if(X){return ;}}
#define SYS_VAR_CHECK_R0(X)    {if(X){return 0;}}

#define SYS_OK()            {return SYS_ERR_OK;}
#define SYS_ERR()            {return SYS_ERR_FT;}
typedef enum
{
    K_SUCCESS = 0u,
    K_SYS_FATAL_ERR,
    K_SYS_SP_ERR,
    K_RUNNING,
    K_STOPPED,
    K_INV_PARAM,
    K_NULL_PTR,
    K_INV_ALIGN,
    K_KOBJ_TYPE_ERR,
    K_KOBJ_DEL_ERR,
    K_KOBJ_DOCKER_EXIST,
    K_KOBJ_BLK,
    K_KOBJ_SET_FULL,
    K_NOTIFY_FUNC_EXIST,

    K_MM_POOL_SIZE_ERR = 100u,
    K_MM_ALLOC_SIZE_ERR,
    K_MM_FREE_ADDR_ERR,
    K_MM_CORRUPT_ERR,
    K_DYN_MEM_PROC_ERR,
    K_NO_MEM,
    K_RINGBUF_FULL,
    K_RINGBUF_EMPTY,

    K_SCHED_DISABLE = 200u,
    K_SCHED_ALREADY_ENABLED,
    K_SCHED_LOCK_COUNT_OVF,
    K_INV_SCHED_WAY,

    K_TASK_INV_STACK_SIZE = 300u,
    K_TASK_NOT_SUSPENDED,
    K_TASK_DEL_NOT_ALLOWED,
    K_TASK_SUSPEND_NOT_ALLOWED,
    K_SUSPENDED_COUNT_OVF,
    K_BEYOND_MAX_PRI,
    K_PRI_CHG_NOT_ALLOWED,
    K_INV_TASK_STATE,
    K_IDLE_TASK_EXIST,

    K_NO_PEND_WAIT = 400u,
    K_BLK_ABORT,
    K_BLK_TIMEOUT,
    K_BLK_DEL,
    K_BLK_INV_STATE,
    K_BLK_POOL_SIZE_ERR,

    K_TIMER_STATE_INV = 500u,

    K_NO_THIS_EVENT_OPT = 600u,

    K_BUF_QUEUE_INV_SIZE = 700u,
    K_BUF_QUEUE_SIZE_ZERO,
    K_BUF_QUEUE_FULL,
    K_BUF_QUEUE_MSG_SIZE_OVERFLOW,
    K_QUEUE_FULL,
    K_QUEUE_NOT_FULL,

    K_SEM_OVF = 800u,
    K_SEM_TASK_WAITING,

    K_MUTEX_NOT_RELEASED_BY_OWNER = 900u,
    K_MUTEX_OWNER_NESTED,
    K_MUTEX_NESTED_OVF,

    K_NOT_CALLED_BY_INTRPT = 1000u,
    K_TRY_AGAIN,

    K_WORKQUEUE_EXIST = 1100u,
    K_WORKQUEUE_NOT_EXIST,
    K_WORKQUEUE_WORK_EXIST,
    K_WORKQUEUE_BUSY,
    K_WORKQUEUE_WORK_RUNNING,

    K_TASK_STACK_OVF = 1200u,
    K_INTRPT_STACK_OVF,

    K_STATE_ALIGN = 0x10000000u /* keep enum 4 bytes */
} kstat_t;


/************************************************************************
**���ݾɰ汾�궨��
*************************************************************************/
#define SA_ERR_OK	SYS_ERR_OK
#define SA_ERR_VAR	SYS_ERR_VAR
#define SA_ERR_FT   SYS_ERR_FT

#define SA_VAR_CHECK(X)  {if(X){return SA_ERR_VAR;}}
#define SA_OK()			 {return SA_ERR_OK;}



/************************************************************************
**�������λ��
*************************************************************************/
#define LBIT(X)      (1 << X)

#include <stdint.h>
typedef union                            //32λ������
{
	uint32_t    ul;
	int32_t	    l;
	uint16_t    ului[2];
	int16_t     uli[2];
	uint8_t     uluc[4];
	int8_t      ulc[4];
}UNINT32;


typedef unsigned char    u8;      // �޷���8λ���ͱ���
typedef unsigned char    U8;      // �޷���8λ���ͱ���
typedef signed   char    s8;       // �з���8λ���ͱ���
typedef signed   char    S8;       // �з���8λ���ͱ���
typedef unsigned short   u16;     // �޷���16λ���ͱ���
typedef unsigned short   U16;     // �޷���16λ���ͱ���
typedef signed   short   s16;      // �з���16λ���ͱ���
typedef signed   short   S16;      // �з���16λ���ͱ���
typedef unsigned long int     u32;     // �޷���32λ���ͱ���
typedef unsigned long int     U32;     // �޷���32λ���ͱ���
typedef signed   long int     s32;      // �з���32λ���ͱ���
typedef signed   long int     S32;      // �з���32λ���ͱ���


typedef unsigned long long     u64;     // �޷���32λ���ͱ���
typedef unsigned long long     U64;     // �޷���32λ���ͱ���
typedef signed   long long     s64;      // �з���32λ���ͱ���
typedef signed   long long     S64;      // �з���32λ���ͱ���

#define BIT bit 

 /*  
typedef	signed char		s8;
typedef	signed char		S8;
typedef	unsigned char	u8;
typedef	unsigned char	U8;

typedef signed short 		s16;
typedef signed short 		S16;
typedef	unsigned short 	u16;
typedef	unsigned short 	U16;

typedef signed long		s32;
typedef signed long		S32;
typedef	unsigned long	u32;
typedef	unsigned long	U32;

typedef signed long long 		s64;
typedef signed long long 		S64;
typedef	unsigned long long 		u64;
typedef	unsigned long long 		U64;
*/
typedef struct                            //�ֽڽṹ��
{
	u8	b0:1;
	u8	b1:1;
	u8	b2:1;
	u8	b3:1;
	u8	b4:1;
	u8	b5:1;
	u8	b6:1;
	u8	b7:1;
}SBIT;

typedef union                            //�ֽڹ�����
{
	u8 uc;
	SBIT b;
}UNBIT;

#ifdef	REG_BIG_MODE
#define	uiuc0	uiuc[1]					///���ݾ��Եĵ�8λ�����ģʽ���Ǹߵ�ַ��С��ģʽ���ǵ͵�ַ
#define	uiuc1	uiuc[0]
#define	uic0	uic[1]
#define	uic1	uic[0]
#else
#define	uiuc0	uiuc[0]					///���ݾ��Եĵ�8λ�����ģʽ���Ǹߵ�ַ��С��ģʽ���ǵ͵�ַ
#define	uiuc1	uiuc[1]
#define	uic0	uic[0]
#define	uic1	uic[1]
#endif


typedef union                            //16λ������
{
	u16 ui;
	s16 i;
	u8 uiuc[2];
	s8 uic[2];
}UNINT16;

#ifdef	REG_BIG_MODE
#define	uluc0	uluc[3]
#define	uluc1	uluc[2]
#define	uluc2	uluc[1]
#define	uluc3	uluc[0]
#define	ulc0	ulc[3]
#define	ulc1	ulc[2]
#define	ulc2	ulc[1]
#define	ulc3	ulc[0]
#define	ului0	ului[1]
#define	ului1	ului[0]
#define	uli0	uli[1]
#define	uli1	uli[0]
#else
#define	uluc0	uluc[0]
#define	uluc1	uluc[1]
#define	uluc2	uluc[2]
#define	uluc3	uluc[3]
#define	ulc0	ulc[0]
#define	ulc1	ulc[1]
#define	ulc2	ulc[2]
#define	ulc3	ulc[3]
#define	ului0	ului[0]
#define	ului1	ului[1]
#define	uli0	uli[0]
#define	uli1	uli[1]
#endif



#define	uduc0	uduc[0]
#define	uduc1	uduc[1]
#define	uduc2	uduc[2]
#define	uduc3	uduc[3]
#define	uduc4	uduc[4]
#define	uduc5	uduc[5]
#define	uduc6	uduc[6]
#define	uduc7	uduc[7]
#define	udc0	udc[0]
#define	udc1	uduc[1]
#define	udc2	uduc[2]
#define	udc3	uduc[3]
#define	udc4	uduc[4]
#define	udc5	uduc[5]
#define	udc6	uduc[6]
#define	udc7	uduc[7]
#define	udui0	udui[0]
#define	udui1	udui[1]
#define	udui2	udui[2]
#define	udui3	udui[3]
#define	udi0	udi[0]
#define	udi1	udi[1]
#define	udi2	udi[2]
#define	udi3	udi[3]
#define	udul0	udul[0]
#define	udul1	udul[1]
#define	udl0	udl[0]
#define	udl1	udl[1]
typedef union                            //32λ������
{
	u64	ud;
	s64	d;
	u32	udul[2];
	u16	udui[4];
	u8	uduc[8];
}UN64;

#define	TRUE	1
#define	FALSE	0
#define	true	1
#define	false	0
#if 0 // :D
#define false 	0
#define true 	0x55
#endif

#ifndef		BIT0
	#define BIT0	0x00000001
#endif
#ifndef		BIT1
	#define BIT1	0x00000002
#endif
#ifndef		BIT2
	#define BIT2	0x00000004
#endif
#ifndef		BIT3
	#define BIT3	0x00000008
#endif
#ifndef		BIT4
	#define BIT4	0x00000010
#endif
#ifndef		BIT5
	#define BIT5	0x00000020
#endif
#ifndef		BIT6
	#define BIT6	0x00000040
#endif
#ifndef		BIT7
	#define BIT7	0x00000080
#endif
#ifndef		BIT8
	#define BIT8	0x00000100
#endif
#ifndef		BIT9
	#define BIT9	0x00000200
#endif
#ifndef		BIT10
	#define BIT10	0x00000400
#endif
#define BIT11	0x00000800
#define BIT12	0x00001000
#define BIT13	0x00002000
#define BIT14	0x00004000
#define BIT15	0x00008000
#define BIT16	0x00010000
#define BIT17	0x00020000
#define BIT18	0x00040000
#define BIT19	0x00080000
#define BIT20	0x00100000
#define BIT21	0x00200000
#define BIT22	0x00400000
#define BIT23	0x00800000
#define BIT24	0x01000000
#define BIT25	0x02000000
#define BIT26	0x04000000
#define BIT27	0x08000000
#define BIT28	0x10000000
#define BIT29	0x20000000
#define BIT30	0x40000000
#define BIT31	0x80000000

#define UN8     UNINT8
#define UN16    UNINT16
#define UN32    UNINT32


#define CON_KEY1_BIT BIT0
#define CON_KEY2_BIT BIT1
#define CON_KEY3_BIT BIT2
#define CON_KEY4_BIT BIT3
#define CON_KEY5_BIT BIT4
#define CON_KEY6_BIT BIT5
#define CON_KEY7_BIT BIT6
#define CON_KEY8_BIT BIT7
#define CON_KEY9_BIT BIT8
#define CON_KEY10_BIT BIT9
#define CON_KEY11_BIT BIT10
#define CON_KEY12_BIT BIT11
#define CON_KEY13_BIT BIT12
#define CON_KEY14_BIT BIT13
#define CON_KEY15_BIT BIT14
#define CON_KEY16_BIT BIT15



#endif


