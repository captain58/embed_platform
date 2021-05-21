
/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: apis.c
**创   建   人: yzy
**最后修改日期: 2011年1月3日
**描        述: SmartAlly2.0的操作系统公开函数名定位文件
**注        意:
********************************************************************************************************/
#define EXT_APIS
#include "public.h"

#include "k_types.h"
#include "drv.h"



const _F_ARRAY gs_Array =
{
    #if SYS_FIFO_EN > 0
    SYS_FIFO_Create,
    SYS_FIFO_Enqueue,
    SYS_FIFO_Dequeue,
    SYS_FIFO_Get,
    SYS_FIFO_Peek,
    SYS_FIFO_Clear,
    #endif
    
    #if SYS_LIFO_EN > 0
    SYS_LIFO_Create,
    SYS_LIFO_Push,
    SYS_LIFO_Pop,
    SYS_LIFO_Get,
    SYS_LIFO_Peek,
    SYS_LIFO_Clear,
    #endif
    
    #if SYS_ARRAY_EN > 0
    SYS_Array_Create,
    SYS_Array_Add,
    SYS_Array_Remove,
    SYS_Array_Get,
    SYS_Array_Set,
    SYS_Array_Clear,
    #endif
};


//	#if SYS_CALCLIB_EN > 0
//	const _F_CALC gs_Calclib =
//	{
//	    MoveBuffer,
//	    InvBuffer,
//	    CmpBuffer,
//	    ClearBuffer,
//	    memset,
//	    strlen,
//	    memcpy,
//	    strcpy,
//	    Divu8,
//	    Divu16,
//	    ByteToInt,
//	    ByteToLong,
//	    ByteToInt24,
//	    IntToByte,
//	    LongToByte,
//	    Int24ToByte,
//	    ByteBcdToHex,
//	    ByteArrayBcdToHex,
//	    IntBcdToHex,
//	    LongBcdToHex,
//	    ByteHexToBcd,
//	    ByteArrayHexToBcd,
//	    IntHexToBcd,
//	    LongHexToBcd,
//	    BcdToHex,
//	    HexToBcd,
//	    IsHexChar,
//	    IsBcd,
//	    IsBcdChar,
//	    HexCharToValue,
//	    ValueToHexChar,
//	    BcdCharToValue,
//	    BcdStringToNum,
//	    NumToBcdString,
//	    HexStringToNum,
//	    NumToHexString,
//	    ByteArrayBcdToHexString,
//	    AbsVal,
//	    AbsVal64,
//	    DoSum,
//	    DoBitSum,
//	    DoCrc16,
//	    DoCrcDlms,
//	    CalcWeek,
//	    CalcDayHex,
//	    CalcHourHex,
//	    CalcMinuteHex,
//	    CalcMins,
//	    CalcSecs,
//	    CalcTimeFromMins,
//	    CalcTimeFromSecs,
//	    U16_little_big_endian,
//	    U32_little_big_endian,
//	    CheckBufferCRC,
//	    BuildBufferCRC,
//	    sqrt_16,
//	    MidWaySearch,
//	    LongMidWaySearch,
//	    ADVMidWaySearch,
//	    CopyBufferToStr,
//	    CopyStrToBuffer,
//	    CopyBufferToStrOffset,
//	    CopyStrToBufferOffset,
//	    FloatToFixPoint,
//	    CalcUartRecvTimeOut,
//	};
//	#endif

const _F_OS gs_OS =         
{
//	    SYS_Message_AyncSend,
    krhino_buf_queue_send,
//	    SYS_Message_SyncSend,
//	    SYS_Message_Wait,
//	    SYS_Message_Get,
//	    SYS_Sched,
//	    SYS_SEM_Create,
//	    SYS_SEM_Get,
//	    krhino_sem_give,
//	    SYS_SEM_Wait,
//	    SYS_SEM_Accept,
//	    SYS_TK_Create,
//	    SYS_TK_Resume,
    aos_msleep,
//	    SYS_TK_Suspend,
//	
};
//	
//	
//	const _F_TIMER gs_Timer =
//	{
//	    SYS_Timer_Create,
//	    SYS_Timer_Start,
//	    SYS_Timer_StartOne,
//	    SYS_Timer_Stop,
//	};

//	
//	const _F_MEM gs_Mem =
//	{
//	    SYS_GetMEM,
//	};







#if (SYS_GPI_EN > 0) || (SYS_GPO_EN > 0)
const _F_GPIO gs_GPIO=
{
    SYS_LED_Init,
    SYS_LED_BlinkSet,
    SYS_LED_BlinkSetAll,
    SYS_LED_BlinkSet1,
    SYS_LED_BlinkSetAll1,
    SYS_GPI_Init,
    SYS_GPI_GetStt,
    SYS_GPI_Event,
    SYS_GPI_Get_PinStt,
    SYS_GPO_Init,
    SYS_GPO_Out,
};
#endif



const _F_RTC gs_RealTime= 
{
    SYS_ReadDateTime,
    SYS_ReadBCDDateTime,
    SYS_WriteDateTime,
    SYS_WriteBCDDateTime,
    GetTime,

};
#if SYS_SER_EN >0
const _F_UART gs_Uart=
{
    SER_Open,
    SER_Close,
    SER_Clear,
    SER_Init,  
//	    SER_ReadByte,
    SER_RecvData,
//	    SER_Line,
//	    SER_LineLen,
    SER_SendData,
    SER_ToBeRead_By_Port,
//	    SER_PrintValue,
//	    SER_Peek,
};
#endif

//	#if SYS_SER_EN >0
//	const _F_UART gs_Uart=
//	{
//	    SER_Open,
//	    SYS_SER_Close,
//	    SER_Clear,
//	    SYS_SER_Init,  
//	    SER_ReadByte,
//	    SYS_SER_Read,
//	    SER_Line,
//	    SER_LineLen,
//	    SER_SendData,
//	    SER_ToBeRead,
//	    SER_PrintValue,
//	};
//	#endif


//	const _F_DEV gs_DEVProc=
//	{
//	    SYS_DEV_Open,
//	    SYS_DEV_Add,
//	    SYS_DEV_Close,
//	};




const _F_FRM gs_FRM =
{
    PST_CheckFrame,                     //根据帧信息结构体进行帧识别
    PST_Channel,                        //扩展的帧识别(支持前导符的识别和过滤)
    PST_DoneFrame,                      //根据帧信息结构体对数据进行打包
    FRM_Decode,                         //帧解析
    FRM_Proc,                           //帧处理
    PST_CheckFrame2,                    //
};




