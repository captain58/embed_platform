/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: bdef.h
**创   建   人: yzy
**最后修改日期: 2010年1月14日
**描        述: esos操作系统的头文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2010年1月14日
** 描　述: 原始版本
********************************************************************************************************/


#ifndef _BDEF_H_
#define _BDEF_H_


#include <stdint.h>
/**********************************************************************************************************
*                   数据类型1
**********************************************************************************************************/
typedef signed char int8;               //8位有符号数
#ifndef uint8
typedef unsigned char uint8;            //8位无符号数
#endif
typedef short int16;                    //16位有符号数
typedef unsigned short uint16;          //16位无符号数
typedef long int32;                     //32位有符号数
typedef unsigned long uint32;           //32位无符号数
typedef signed long long int64;         //64位有符号数
typedef unsigned long long uint64;      //64位无符号数
typedef uint8 Boolean;                  //Bool 类型
typedef void* Object;                   //定义任意类型为Object
typedef const char * String;            //定义字符串类型
typedef int8 TResult;                   //返回的结果类型
typedef void (*TCallback)(void);
typedef void (*SCallback)(uint8 data);
typedef void (*ThreadFunc)(void * arg);
typedef void (*RCallback)(uint8 * data);

typedef uint32 TStack;
//typedef Boolean uint8_t;                 //yzy 与 lpc_types.h->stdboo.h 中uint8ool 定义重复
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
*                   数据类型2
**********************************************************************************************************/
typedef unsigned char  BOOLEAN;                 /* 布尔变量                                 */
typedef unsigned char  INT8U;                   /* 无符号8位整型变量                        */
typedef signed   char  INT8S;                   /* 有符号8位整型变量                        */
typedef unsigned short INT16U;                  /* 无符号16位整型变量                       */
typedef signed   short INT16S;                  /* 有符号16位整型变量                       */
typedef unsigned int   INT32U;                  /* 无符号32位整型变量                       */
typedef signed   int   INT32S;                  /* 有符号32位整型变量                       */
typedef float          FP32;                    /* 单精度浮点数(32位长度)                 */
typedef double         FP64;                    /* 双精度浮点数(64位长度)                 */

typedef INT32U         OS_STK;                  /* 堆栈是32位宽度                           */







/**********************************************************************************************************
*                    数据类型4
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
*                    数据常量
**********************************************************************************************************/
/*                                          //yzy 与lpc_types.h中的TRUE定义冲突
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
**定义系统函数返回结果
**********************************************************/
#define SYS_ERR_OK          0          //没有错误
#define SYS_ERR_EXIST       (1)        //不存在
#define SYS_ERR_FT          (2)        //其他致命错误
#define SYS_ERR_NRES        (3)        //还有资源被占用无法删除
#define SYS_ERR_PRIOR       (4)        //优先级错误
#define SYS_ERR_TIME        (5)        //超时错误
#define SYS_ERR_NE          (6)        //数量不足
#define SYS_ERR_GT          (7)        //数量越限
#define SYS_ERR_NSUSP       (8)        //进程没有挂起
#define SYS_ERR_TYPE        (10)       //类型不匹配
#define SYS_ERR_VAR         (11)       //参数错误
#define SYS_ERR_CHK         (12)       //验证不正确
#define SYS_ERR_MAIN        (13)       //主存储区错误
#define SYS_ERR_BACK        (14)       //备份存储区错误
#define SYS_ERR_WAIT        (15)       //等待
#define SYS_ERR_DEF         (16)       //使用默认数据
#define SYS_ERR_NOREPLY     (17)       //无需回复
#define SYS_ERR_DATA        (18)       //带数据
//#define SYS_ERR_NOREPLY     (19)       //无需回复
//#define SYS_ERR_DATA        (20)       //带数据
#define SYS_ERR_DI          (21)       //无效DI

#define SYS_ERR_UNKNOW      (127)

#define ER(X)              (SYS_ERR_OK == (X))

#define SYS_VAR_CHECK(X)    {if(X){return SYS_ERR_VAR;}}
#define SYS_OK()            {return SYS_ERR_OK;}
#define SYS_ERR()            {return SYS_ERR_FT;}
#define SYS_NOREPLY()            {return SYS_ERR_NOREPLY;}

//API错误码定义
#define API_SUCCESS						0		//操作成功
#define API_ERR_FAILURE					-1		//操作失败
#define API_ERR_PARA					-2		//参数有误
#define API_ERR_BUF_OVERFLOW			-3		//缓冲区溢出
#define API_ERR_MEM_LACK				-4		//内存不足				
#define API_ERR_TIMEOUT					-5		//超时

//宏函数
#define ARR_CNT(x)				(sizeof(x) / sizeof(x[0]))

//	/******************************************************************************
//	**兼容旧版本宏定义
//	******************************************************************************/
//	#define SYS_ERR_OK	SYS_ERR_OK
//	#define SA_ERR_VAR	SYS_ERR_VAR
//	#define SYS_ERR_FT   SYS_ERR_FT
//	
//	#define SA_VAR_CHECK(X)  {if(X){return SA_ERR_VAR;}}
//	#define SYS_OK()			 {return SYS_ERR_OK;}




/******************************************************************************
**定义比特位宏
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


