/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: bdef.h
**��   ��   ��: yzy
**����޸�����: 2010��1��14��
**��        ��: esos����ϵͳ��ͷ�ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2010��1��14��
** �衡��: ԭʼ�汾
********************************************************************************************************/


#ifndef _BDEF_H_
#define _BDEF_H_


#include <stdint.h>
/**********************************************************************************************************
*                   ��������1
**********************************************************************************************************/
typedef signed char int8;               //8λ�з�����
#ifndef uint8
typedef unsigned char uint8;            //8λ�޷�����
#endif
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
typedef void (*SCallback)(uint8 data);
typedef void (*ThreadFunc)(void * arg);
typedef void (*RCallback)(uint8 * data);

typedef uint32 TStack;
//typedef Boolean uint8_t;                 //yzy �� lpc_types.h->stdboo.h ��uint8ool �����ظ�
typedef String string;
typedef uint8  BS8;

#ifndef TRUE
#define TRUE               1
#endif

#ifndef FALSE
#define FALSE              0
#endif


typedef unsigned char       BOOL; 

/**********************************************************************************************************
*                   ��������2
**********************************************************************************************************/
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







/**********************************************************************************************************
*                    ��������4
**********************************************************************************************************/
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

typedef union{
    float fword;
    uint8 byte[4];
}Float32;
typedef union{
    double dword;
    uint8 byte[8];
}Double64;

/**********************************************************************************************************
*                    ���ݳ���
**********************************************************************************************************/
/*                                          //yzy ��lpc_types.h�е�TRUE�����ͻ
#ifndef TRUE
#define TRUE        1
#endif

#ifndef FALSE
#define FALSE       0
#endif
*/

#ifndef true
#define true        1
#endif

#ifndef false
#define false       0
#endif

#ifndef __NULL
#define __NULL      ((void*)0)
#endif

#ifndef NULL
#define	NULL	    0
#endif




/**
 * @brief Functional State Definition
 */
//typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define PARAM_FUNCTIONALSTATE(State) ((State == DISABLE) || (State == ENABLE))

/**
 * @brief Flag Status and Interrupt Flag Status type definition
 */
//typedef enum {RESET = 0, SET = !RESET} FlagStatus, IntStatus, SetState;
#define PARAM_SETSTATE(State) ((State == RESET) || (State == SET))

/**
 * @ Status type definition
 */
//typedef enum {ERROR = 0, SUCCESS = !ERROR} Status;

#define RISING_EDGE_INT   1
#define FALLING_EDGE_INT  0

/* _BIT(n) sets the bit at position "n"
 * _BIT(n) is intended to be used in "OR" and "AND" expressions:
 * e.g., "(_BIT(3) | _BIT(7))".
 */
#undef _BIT
/* Set bit macro */
#define _BIT(n) (1 << (n))

/* _SBF(f,v) sets the bit field starting at position "f" to value "v".
 * _SBF(f,v) is intended to be used in "OR" and "AND" expressions:
 * e.g., "((_SBF(5,7) | _SBF(12,0xF)) & 0xFFFF)"
 */
#undef _SBF
/* Set bit field macro */
#define _SBF(f, v) ((v) << (f))

/********************************************************* 
**����ϵͳ�������ؽ��
**********************************************************/
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
//#define SYS_ERR_NOREPLY     (19)       //����ظ�
//#define SYS_ERR_DATA        (20)       //������
#define SYS_ERR_DI          (21)       //��ЧDI

#define SYS_ERR_UNKNOW      (127)

#define ER(X)              (SYS_ERR_OK == (X))

#define SYS_VAR_CHECK(X)    {if(X){return SYS_ERR_VAR;}}
#define SYS_OK()            {return SYS_ERR_OK;}
#define SYS_ERR()            {return SYS_ERR_FT;}
#define SYS_NOREPLY()            {return SYS_ERR_NOREPLY;}

//API�����붨��
#define API_SUCCESS						0		//�����ɹ�
#define API_ERR_FAILURE					-1		//����ʧ��
#define API_ERR_PARA					-2		//��������
#define API_ERR_BUF_OVERFLOW			-3		//���������
#define API_ERR_MEM_LACK				-4		//�ڴ治��				
#define API_ERR_TIMEOUT					-5		//��ʱ

//�꺯��
#define ARR_CNT(x)				(sizeof(x) / sizeof(x[0]))

//	/******************************************************************************
//	**���ݾɰ汾�궨��
//	******************************************************************************/
//	#define SYS_ERR_OK	SYS_ERR_OK
//	#define SA_ERR_VAR	SYS_ERR_VAR
//	#define SYS_ERR_FT   SYS_ERR_FT
//	
//	#define SA_VAR_CHECK(X)  {if(X){return SA_ERR_VAR;}}
//	#define SYS_OK()			 {return SYS_ERR_OK;}




/******************************************************************************
**�������λ��
******************************************************************************/
#define LBIT(X)      ((uint32)1 << X)

 
#ifdef __CC_ARM
#define INLINE  __inline
#else
#define INLINE inline
#endif

/* Static data/function define */
#define STATIC static


#endif


