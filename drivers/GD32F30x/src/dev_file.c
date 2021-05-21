/******************************Copyright(c)******************************
**                         
**
** File Name: dev_file.h
** Author: 
** Date Last Update: 2019-11-02
** Description: 
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#define EXT_DEV_FILE

#include "sys.h"
#include "hal.h"
#include "bsp.h"
#include <string.h>

/******************************************************************************
**动态数据块配置的合法性判断
******************************************************************************/
#if SYS_FILE_DYM_EN > 0
    #if DB_DYM_DB_NUM > 168
        #warning "动态块太多了!"
    #endif

    #if ((1 << DB_DYM_DB_2M) != DB_DYM_DB_SIZE)
        #warning "动态块定义错误!"
    #endif 
#endif



/******************************************************************************
**声明外部变量
******************************************************************************/
extern uint32 gul_FramSize;
extern const S_FILEBLOCKS gss_FileBlocks[];
extern const uint8 LEN_DB_LIST;
extern const TFlsCycleConst gs_flsFramConst;
#ifndef __NO_SYS__
TESRes gs_FileRes;
uint8 guc_SpiioUserTkid;

#endif
/******************************************************************************
**动态数据块信息缓存指针,指向的缓存大小为(DB_DYM_DB_NUM + 1) * 3 + 1 
**数据缓存说明, 字节0--DB_DYM_DB_NUM - 1: 数据块链表 
**              字节DB_DYM_DB_NUM--DB_DYM_DB_NUM * 2 - 1    分配的动态块的起始块
**              字节DB_DYM_DB_NUM * 2--DB_DYM_DB_NUM*3 - 1  分配的动态块的数据块数量
**              字节DB_DYM_DB_NUM*3                     空闲块头指针
**              字节DB_DYM_DB_NUM*3 + 1                 空闲块数量
**              字节DB_DYM_DB_NUM*3 + 2--(DB_DYM_DB_NUM + 1) * 3  CRC校验
******************************************************************************/
uint8* DymBuffer;




/***********************************************************
 * @function_name: DevFls_ProcMSG
 * @function_file: dev_fls.c
 * @描述:  处理队列消息(注意,调用前必须锁定对应总线的资源量)
 * 
 * 
 * @参数: 
 * @param:msg  
 * @返回: 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/20)
 **********************************************************/
uint8 DevFls_ProcMSG(TDevFlsOP* op)
{
    uint8 result = SYS_ERR_FT;

#if SYS_IFLASH_EN == 1

    if(op->type == DEV_FLS_TYPE_IFLS)     //操作Flash
    {
       switch(op->rw)
       {
           case DEV_FLS_RW_R:           //读操作
               result = HAL_IFLASH_Read((uint8*)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_W:           //写操作
//	               FLS_PreWrite();
               result = HAL_IFLASH_Write((uint8*)op->obj, op->addr, op->len);
               break;
#ifndef __NO_SYS__           
           case DEV_FLS_RW_S:           //设置操作
//	               FLS_PreWrite();
               result = HAL_IFLASH_Set((uint8)(uint32)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_E:           //擦除操作
//	               FLS_PreWsrite();
               result = HAL_IFLASH_Erase(op->addr, op->len);
               break;
           case DEV_FLS_RW_WS:          //Flash特殊写入操作
//	               FLS_PreWrite();
               result = HAL_IFLASH_WriteSpec((uint8*)op->obj, op->addr, op->len, 1);
               break;
#endif           
       }
       return result;
    }
#endif        
#if SYS_FLASH_EN == 1

    if(op->type == DEV_FLS_TYPE_FLS)     //操作Flash
    {
       switch(op->rw)
       {
           case DEV_FLS_RW_R:           //读操作
               result = FLS_Read((uint8*)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_W:           //写操作
               FLS_PreWrite();
               result = FLS_Write((uint8*)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_S:           //设置操作
               FLS_PreWrite();
               result = FLS_Set((uint8)(uint32)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_E:           //擦除操作
               FLS_PreWrite();
               result = FLS_Erase(op->addr, op->len);
               break;
           case DEV_FLS_RW_WS:          //Flash特殊写入操作
               FLS_PreWrite();
               result = FLS_WriteSpec((uint8*)op->obj, op->addr, op->len);
               break;
       }
       return result;
    }
#endif    
#if SYS_E2_EN == 1
    
    if(op->type == DEV_FLS_TYPE_E2)//操作e2
    {
       switch(op->rw)
       {
           case DEV_FLS_RW_R:           //读操作
               result = SYS_E2_Read((uint8*)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_W:           //写操作
//	               FRAM_PreWrite();
               result = SYS_E2_Write((uint8*)op->obj, op->addr, op->len);
               break;
       }
       return result;
    }
    
#endif

#if SYS_FRAM_EN > 0
#if SYS_FLASH_FRAM_EN == 0

    if(op->type == DEV_FLS_TYPE_FRAM)//操作FRAM
    {
       switch(op->rw)
       {
           case DEV_FLS_RW_R:           //读操作
               result = FRAM_Read((uint8*)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_W:           //写操作
               FRAM_PreWrite();
               result = FRAM_Write((uint8*)op->obj, op->addr, op->len);
               break;
       }
       return result;
    }

#else

    if(op->type == DEV_FLS_TYPE_VFRAM)//操作VFRAM
    {
        switch(op->rw)
        {
           case DEV_FLS_RW_R:           //读操作
               result = SYS_VFRAM_ReadSample((uint8*)op->obj, op->addr, (const TFlsCycleConst*)op->len);
               break;
           case DEV_FLS_RW_W:           //写操作
               result = SYS_VFRAM_WriteSample((uint8*)op->obj, op->addr, (const TFlsCycleConst*)op->len);
               break;
        }
        
        return result;
    }
#endif
#endif
    return result;

    
}
#if (SYS_E2_EN > 0)


/************************************************************************
 * @function: SYS_FRAM_Open
 * @描述: 向一个铁电条目中写入数据
 * 
 * @参数: 
 * @param: data 欲写入的内容
 * @param: num 铁电条目编号
 * 
 * @返回: 
 * @return: TResult
 * @说明: 
 * @作者: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
TResult SYS_E2_Open(void)
{
//	    SYS_SEM_Wait(&gs_FramRes, 0);
//	    guc_FramUserTkid = gs_TkTcbCur->tkid;  //为当前用户进程编号
    
    return SYS_ERR_OK;
}


/************************************************************************
 * @function: SYS_SER_Close
 * @描述: 串口关闭
 * 
 * @参数: 
 * @param:  
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/8/15)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_E2_Close(void)
{
//	    if(guc_FramUserTkid == gs_TkTcbCur->tkid)
//	    {
//	        guc_FramUserTkid = 0xFF;        //恢复成初始值
//	        SYS_SEM_Release(&gs_FramRes);    //释放这个信号量
//	    }
}

/************************************************************************
 * @function: SYS_E2_SafeWrite
 * @描述: 向一个e2条目中写入数据
 * 
 * @参数: 
 * @param: data 欲写入的内容
 * @param: num 铁电条目编号
 * 
 * @返回: 
 * @return: TResult
 * @说明: 
 * @作者: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_E2_SafeWrite(uint8* data, uint32_t addr, uint32_t len)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
#ifndef __NO_SYS__    
                                        //判断是否已被其他进程占用
    if(guc_FramUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif    
    op.addr = addr;                      //地址
    op.len = len;  //长度(虚拟铁电信息结构体)
    op.obj = data;                      //缓存
    op.type = DEV_FLS_TYPE_E2;       //类型
    op.rw = DEV_FLS_RW_W;               //读写方式
    result = DevFls_ProcMSG(&op);

    return result;                      //未知错误
}

/************************************************************************
 * @function: SYS_E2_SafeRead
 * @描述: 从一个e2条目中读取数据
 * 
 * @参数: 
 * @param: data 一个铁电条目,大小至少为32字节,否则会导致缓存溢出
 * @param: num 铁电条目编号
 * 
 * @返回: 
 * @return: TResult  
 * @说明: 
 * @作者: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_E2_SafeRead(uint8* data, uint32_t addr, uint32_t len)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
#ifndef __NO_SYS__    
    
                                        //判断是否已被其他进程占用
    if(guc_FramUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif    
    op.addr = addr;                      //地址
    op.len = len;  //长度(虚拟铁电信息结构体)
    op.obj = data;                      //缓存
    op.type = DEV_FLS_TYPE_E2;       //类型
    op.rw = DEV_FLS_RW_R;               //读写方式
    result = DevFls_ProcMSG(&op);
    
    return result;                      //未知错误
}

//#include "stdlib.h"
/************************************************************************
 * @function: SYS_FILE_Test
 * @描述: flash功能测试
 * @参数: 
 * 
 * @返回: 
 * @return: uint32  SYS_ERR_OK 功能正常, SYS_ERR_FT 功能异常(该类型为了与dev_file2.c一致)
 * @说明: 
 * @作者: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint32 SYS_E2_Test(void)
{
    uint8 cmp1[DEF_EE_BUF];                          //原始数据
    uint8 cmp2[DEF_EE_BUF];                          //修改后理论写入数据
    uint8 cmp3[DEF_EE_BUF];                          //修改后实际写入数据
    uint32 err = 0;
#ifndef __NO_SYS__
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif    
                                            //执行测试
    for(int k = 0; k < DEF_EE_SIZE/DEF_EE_BUF; k++)
    {
                                            //读取原始数据
        if(SYS_E2_SafeRead(cmp1, k*DEF_EE_BUF, DEF_EE_BUF) != SYS_ERR_OK)
        {
            err = k+1;
            break;
        }
                                            //修改原始数据
        for(uint8 i = 0; i < DEF_EE_BUF; i++)
        {
            cmp2[i] = cmp1[i] + i;
        }
                                            //写入理论写入数据
        if(SYS_E2_SafeWrite(cmp2, k*DEF_EE_BUF, DEF_EE_BUF) != SYS_ERR_OK)
        {
            err = k+1;
            break;
        }
                                            //读取实际写入数据
        if(SYS_E2_SafeRead(cmp3, k*DEF_EE_BUF, DEF_EE_BUF) != SYS_ERR_OK)
        {
            err = k+1;
            break;
        }
        
        SYS_E2_SafeWrite(cmp1, k*DEF_EE_BUF, DEF_EE_BUF);   //恢复原始值
        if(memcmp(cmp2, cmp3, DEF_EE_BUF) != 0)   //比对理论实际值
        {
            err = k+1;
            break;
        }
    }
    
    return err;
}





#endif               //#if SYS_FLASH_FRAM_EN > 0





/***********************************************************
 * @function_name: SYS_FILE_Open
 * @function_file: es_file.c
 * @描述:打开数据块
 * 
 * 
 * @参数: 
 * @param:
 * 
 * @返回: 
 * @return:  
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/21)
 **********************************************************/
void SYS_FILE_Open(void)
{
#ifndef __NO_SYS__
    SYS_SEM_Wait(&gs_FileRes, 0);        //申请资源
    guc_FileUserTkid = gs_TkTcbCur->tkid;  //为当前用户进程编号
#endif    
}

/***********************************************************
 * @function_name: SYS_FILE_Close
 * @function_file: es_file.c
 * @描述:关闭数据块
 * 
 * 
 * @参数: 
 * @param:
 * 
 * @返回: 
 * @return:  
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/21)
 **********************************************************/
void SYS_FILE_Close(void)
{
#ifndef __NO_SYS__
    if(guc_FileUserTkid == gs_TkTcbCur->tkid)
    {
        guc_FileUserTkid = 0xFF;        //恢复成初始值
        SYS_SEM_Release(&gs_FileRes);    //释放这个信号量
    }
#endif    
}



/***********************************************************
 * @function_name: SYS_FILE_DB_Open
 * @function_file: es_file.c
 * @描述:打开指定块号的数据块
 * 
 * 
 * @参数: 
 * @param:ID  数据块编号
 * @param:db  数据块结构
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/21)
 **********************************************************/
TResult SYS_FILE_DB_Open(uint16_t user, uint16_t ID, TDataBlock* db, TDB_MODE RW)
{
    uint32 start;
    uint16 nblock;
    uint16 k;
    uint16_t id_level = 0;
    uint32_t err = 0;
    SYS_VAR_CHECK(user >= TDB_PERMS_USER && ((ID % 0x100) < 8));
    //设置数据操作块的变量
    id_level = ID >> 8;
    start = 0;
    nblock = 0;
    for(k = 0; k < LEN_DB_LIST; k++)
    {
        if((gss_FileBlocks[k].dbno + gss_FileBlocks[k].sml) < (id_level << 8))
        {
            continue;
        }
    
        if(ID > (gss_FileBlocks[k].dbno + gss_FileBlocks[k].sml))
        {
            start += (gss_FileBlocks[k].sml + 1) * gss_FileBlocks[k].blocks;
        }
        else if((ID >= gss_FileBlocks[k].dbno) && (ID <= gss_FileBlocks[k].dbno + gss_FileBlocks[k].sml))
        {
            start += (ID - gss_FileBlocks[k].dbno) * gss_FileBlocks[k].blocks;
            nblock = gss_FileBlocks[k].blocks;
            break;
        }
        else
        {
            return SYS_ERR_FT;
        }
        if((gss_FileBlocks[k].dbno + gss_FileBlocks[k].sml) >= ((id_level + 1)<< 8))
        {
            err = SYS_ERR_FT;
            break;
        }
        
    }
    
    if(k == LEN_DB_LIST || err != 0)
    {
        return SYS_ERR_FT;
    }
                                        //
    if((gss_FileBlocks[k].atr & RW) != RW)
    {
        return SYS_ERR_TYPE;
    }
                                        //
    ASSERT(start < 0x000FFFFF);         //确保左移12位后不溢出uint32_max.
    if(id_level == 0)
    {
        db->start = DB_LEN_BLOCK(0, start);
        db->length = DB_LEN_BLOCK(0, nblock);
    }
    else
    {
        db->start = DB_LEN_BLOCK(1, start);
        db->length = DB_LEN_BLOCK(1, nblock);
    }
    db->RW = gss_FileBlocks[k].atr;
    db->ID = ID;
    
//	    if((db->start == 0) && (nblock > 1))//升级数据块首个block用来存放updatemark
//	    {
//	        db->start = DB_LEN_BLOCK(1);
//	        nblock -= 1;                    //实际容量少一个block
//	    }
                                        //申请资源
    SYS_FILE_Open();
    
    SYS_OK();
}


/***********************************************************
 * @function_name: SYS_FILE_DB_Close
 * @function_file: es_file.c
 * @描述:关闭指定块号的数据块
 * 
 * 
 * @参数: 
 * @param:db  数据块句柄
 * 
 * @返回: 
 * @return:  
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/21)
 **********************************************************/
void SYS_FILE_DB_Close(TDataBlock* db)
{
    SYS_FILE_Close();
}
#if SYS_IFLASH_EN > 0

/***********************************************************
 * @function_name: SYS_FILE_Write
 * @function_file: dev_file.c
 * @描述:写入数据
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult _SYS_IFLS_Write(uint8* buffer, uint32 addr, uint32 length)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
#ifndef __NO_SYS__    
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif    
    op.addr = addr;                     //地址
    op.len = length;                    //长度
    op.obj = buffer;                    //缓存
    op.type = DEV_FLS_TYPE_IFLS;         //类型
    op.rw = DEV_FLS_RW_W;               //读写方式

                                        //发送消息
    result = DevFls_ProcMSG(&op);  
    return result;                      //未知错误
}



/***********************************************************
 * @function_name: SYS_FILE_ReadFromAddr
 * @function_file: app_calc.c
 * @描述:从指定的偏移地址开始读取数据
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/21)
 **********************************************************/
TResult _SYS_IFLS_Read(uint8* buffer, uint32 addr, uint32 length)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
#ifndef __NO_SYS__     
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif    
    op.addr = addr;                     //地址
    op.len = length;                    //长度
    op.obj = buffer;                    //缓存
    op.type = DEV_FLS_TYPE_IFLS;         //类型
    op.rw = DEV_FLS_RW_R;               //读写方式
    
    result = DevFls_ProcMSG(&op);
    return result;
}

/************************************************************************
 * @function: SYS_FILE_Test
 * @描述: flash功能测试
 * @参数: 
 * 
 * @返回: 
 * @return: uint32  SYS_ERR_OK 功能正常, SYS_ERR_FT 功能异常(该类型为了与dev_file2.c一致)
 * @说明: 
 * @作者: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//#include "stdlib.h"
#define DEF_IFLS_BUF          512
#define DEF_IFLS_SIZE         0x40000

/************************************************************************
 * @function: SYS_FILE_Test
 * @描述: flash功能测试
 * @参数: 
 * 
 * @返回: 
 * @return: uint32  SYS_ERR_OK 功能正常, SYS_ERR_FT 功能异常(该类型为了与dev_file2.c一致)
 * @说明: 
 * @作者: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint32 SYS_IFLS_Test(void)
{
    uint8 cmp1[DEF_IFLS_BUF];                          //原始数据
    uint8 cmp2[DEF_IFLS_BUF];                          //修改后理论写入数据
    uint8 cmp3[DEF_IFLS_BUF];                          //修改后实际写入数据
    uint32 err = 0;
    SYS_FILE_Open();
#ifndef __NO_SYS__

    //判断是否已被其他进程占用
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif    
                                            //执行测试
    for(int k = 256; k < DEF_IFLS_SIZE/DEF_IFLS_BUF; k++)
    {
                                            //读取原始数据
        if(_SYS_IFLS_Read(cmp1, k*DEF_IFLS_BUF, DEF_IFLS_BUF) != SYS_ERR_OK)
        {
            err = k+1;
            break;
        }
                                            //修改原始数据
        for(int i = 0; i < DEF_IFLS_BUF; i++)
        {
            cmp2[i] = cmp1[i] + i;
        }
                                            //写入理论写入数据
        if(_SYS_IFLS_Write(cmp2, k*DEF_IFLS_BUF, DEF_IFLS_BUF) != SYS_ERR_OK)
        {
            err = k+1;
            break;
        }
                                            //读取实际写入数据
        if(_SYS_IFLS_Read(cmp3, k*DEF_IFLS_BUF, DEF_IFLS_BUF) != SYS_ERR_OK)
        {
            err = k+1;
            break;
        }
        
        _SYS_IFLS_Write(cmp1, k*DEF_IFLS_BUF, DEF_IFLS_BUF);   //恢复原始值
        if(memcmp(cmp2, cmp3, DEF_IFLS_BUF) != 0)   //比对理论实际值
        {
            err = k+1;
            break;
        }
    }
    SYS_FILE_Close();
    return err;
}


/***********************************************************
 * @function_name: SYS_IFILE_DB_ReadFrom
 * @function_file: es_file.c
 * @描述:从指定的偏移开始读取数据
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/21)
 **********************************************************/
TResult SYS_IFILE_DB_ReadFrom(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
    
    if(!(db->RW & TDB_MODE_R))          //需要支持读属性才允许读取
    {
        return SYS_ERR_TYPE;
    }
#ifndef __NO_SYS__    
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif
    op.addr = db->start + addr;         //地址
    op.len = length;                    //长度
    op.obj = buffer;                    //缓存
    op.type = DEV_FLS_TYPE_IFLS;         //类型
    op.rw = DEV_FLS_RW_R;               //读写方式

    result = DevFls_ProcMSG(&op);

    return result;
}
/***********************************************************
 * @function_name: SYS_IFILE_DB_WriteFrom
 * @function_file: es_file.c
 * @描述:写入数据
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_WriteFrom(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    if(!(db->RW & TDB_MODE_W))          //需要支持读属性才允许读取
    {
        return SYS_ERR_TYPE;
    }
                                        //确认参数是否正确
    SYS_VAR_CHECK(db->length < addr + length);
#ifndef __NO_SYS__    
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;

    op.addr = db->start + addr;         //地址
    op.len = length;                    //长度
    op.obj = buffer;                    //缓存
    op.type = DEV_FLS_TYPE_IFLS;         //类型
    op.rw = DEV_FLS_RW_W;               //读写方式

    result = DevFls_ProcMSG(&op);
    
    return result; 
}
/***********************************************************
 * @function_name: SYS_IFILE_DB_SetFrom
 * @function_file: es_file.c
 * @描述:设置相同数据
 * 
 * 
 * @参数: 
 * @param:db    
 * @param:addr 
 * @param:length
 * @param:templet
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_SetFrom(TDataBlock* db, uint32 addr, uint32 length, uint8 templet)
{
    if(!(db->RW & TDB_MODE_W))          //需要支持读属性才允许读取
    {
        return SYS_ERR_TYPE;
    }
                                        //确认参数是否正确
    SYS_VAR_CHECK(db->length < addr + length);
                                        //判断是否已被其他进程占用
#ifndef __NO_SYS__                                         
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;

    op.addr = db->start + addr;         //地址
    op.len = length;                    //长度
    op.obj = (Object)templet;           //样本
    op.type = DEV_FLS_TYPE_IFLS;         //类型
    op.rw = DEV_FLS_RW_S;               //读写方式

    result = DevFls_ProcMSG(&op);
    
    return result;
}
/***********************************************************
 * @function_name: SYS_IFILE_DB_Erase
 * @function_file: es_file.c
 * @描述:
 * 
 * 
 * @参数: 
 * @param: db 要擦除的数据块
 * @param: sectorfrom 起始擦除扇区(0~)
 * @param: sectornum 擦除扇区的数量(超出数据块大小部分不擦)
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_Erase(TDataBlock* db, uint32 sectorfrom, uint32 sectornum)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
    uint32 dbsectors;                   //数据块包含的总扇区数量
                                        //确认参数是否正确
    SYS_VAR_CHECK(sectorfrom > sectornum);
    SYS_VAR_CHECK(sectornum == 0);
    
                                        //判断是否已被其他进程占用
#ifndef __NO_SYS__                                         
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif    
                                        //总扇区数量
    dbsectors = (db->length >> 9);
                                        //起始sector必须在数据块范围内
    if(sectorfrom >= dbsectors)
    {
        return SYS_ERR_FT;
    }
                                        //擦除扇区数量必须在数据块范围内
    if((sectorfrom + sectornum) > dbsectors)
    {
        sectornum = dbsectors - sectorfrom;
    }

    op.addr = db->start + (sectorfrom << 9);//要擦除区域在数据块内的绝对地址
    op.len = sectornum;                 //擦除扇区总数
    op.obj = __NULL;
    op.type = DEV_FLS_TYPE_IFLS;
    op.rw = DEV_FLS_RW_E;
    
    result = DevFls_ProcMSG(&op);
    
    return result;
}
/*******************************************************************************
** @function_name:  SYS_FILE_DB_WriteSpec
** @function_file:  dev_file.c
** @描述: 文件数据块特殊写操作(适用于无须保存原有数据的写操作,特别适用于大数据量连续写入)
** 
** 
** @参数: 
** @param: db 
** @param: buffer 
** @param: length 
** @param: addr 
** 
** @返回: 
** @return:  TResult   
** @作者: yzy (2011-07-25)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
TResult SYS_IFILE_DB_WriteSpec(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    if(!(db->RW & TDB_MODE_W))          //需要支持写属性才允许读取
    {
        return SYS_ERR_TYPE;
    }
                                        //确认参数是否正确
    SYS_VAR_CHECK(db->length < addr + length);
    
                                        //判断是否已被其他进程占用
#ifndef __NO_SYS__                                         
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
#endif
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;

    op.addr = db->start + addr;         //地址
    op.len = length;                    //长度
    op.obj = buffer;                    //缓存
    op.type = DEV_FLS_TYPE_IFLS;         //类型
    op.rw = DEV_FLS_RW_WS;              //读写方式

    result = DevFls_ProcMSG(&op);
    
    return result;
}


/***********************************************************
 * @function_name: SYS_FILE_DB_WriteWithCRC
 * @function_file: es_file.c
 * @描述:带CRC校验的写入,注意写入的缓存要开大两个字节
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_WriteWithCRC(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    BuildBufferCRC(buffer, length, true);
    return SYS_IFILE_DB_WriteFrom(db, buffer, length, addr);
}


/***********************************************************
 * @function_name: SYS_FILE_DB_WriteWithBackup
 * @function_file: es_file.c
 * @描述:写入数据,不仅带CRC校验,还带备份,这样的block实际能用的大小只有
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_WriteWithBackup(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TResult r1, r2; 

    if(!(db->RW & TDB_MODE_BK))         //确保支持备份块属性
    {
        return SYS_ERR_TYPE;
    }
                                        //确认参数的正确性
    SYS_VAR_CHECK(addr + length > (db->length >> 1));
                                        //备份地址
    uint32 bkaddr = addr + (db->length >> 1);
                                        //带CRC校验的写入
    r1 = SYS_IFILE_DB_WriteWithCRC(db, buffer, length, addr);
    r2 = SYS_IFILE_DB_WriteWithCRC(db, buffer, length, bkaddr);
    
    if(!ER(r1) && ER(r2))               //主区错误,备份区正确
    {
        return SYS_ERR_MAIN;
    }

    if(!ER(r2) && ER(r1))               //备份区错误,主区正确
    {
        return SYS_ERR_BACK;
    }
    
    if(!ER(r2) && !ER(r1))              //备份区错误,主区错误
    {
        return SYS_ERR_CHK;
    }
    SYS_OK();
}


/***********************************************************
 * @function_name: SYS_FILE_DB_ReadWithCRC
 * @function_file: es_file.c
 * @描述:带校验的读取
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_ReadWithCRC(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TResult r;
    r = SYS_IFILE_DB_ReadFrom(db, buffer, length, addr);

    if(ER(r))
    {
        if(!CheckBufferCRC(buffer, length, true))
        {
            memset_s(buffer, 0x00, length);
            r = SYS_ERR_CHK;
        }
    }
    return r;
}

/***********************************************************
 * @function_name: SYS_FILE_DB_ReadWithBackup
 * @function_file: es_file.c
 * @描述:带备份+校验的读取
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_IFILE_DB_ReadWithBackup(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TResult r; 

    if(!(db->RW & TDB_MODE_BK))         //确保支持备份块属性
    {
        return SYS_ERR_TYPE;
    }
                                        //确认参数的正确性
    SYS_VAR_CHECK(addr + length > (db->length >> 1));
                                        //备份地址
    uint32 bkaddr = addr + (db->length >> 1);
                                        //带CRC校验的读取
    r = SYS_IFILE_DB_ReadWithCRC(db, buffer, length, addr);

    if(!ER(r))                          //主区错误则读取备份区
    {
        r = SYS_IFILE_DB_ReadWithCRC(db, buffer, length, bkaddr);
        if(ER(r))
        {
            return SYS_ERR_MAIN;         //主区错误,但备份区正确
        }
    }
    return r;                           //返回写入结果

}

#endif


#if SYS_FLASH_EN > 0
/***********************************************************
 * @function_name: SYS_FILE_Write
 * @function_file: dev_file.c
 * @描述:写入数据
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_FILE_Write(uint8* buffer, uint32 addr, uint32 length)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
    
    op.addr = addr;                     //地址
    op.len = length;                    //长度
    op.obj = buffer;                    //缓存
    op.type = DEV_FLS_TYPE_FLS;         //类型
    op.rw = DEV_FLS_RW_W;               //读写方式

                                        //发送消息
    result = DevFls_ProcMSG(&op);  
    return result;                      //未知错误
}



/***********************************************************
 * @function_name: SYS_FILE_ReadFromAddr
 * @function_file: app_calc.c
 * @描述:从指定的偏移地址开始读取数据
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/21)
 **********************************************************/
TResult SYS_FILE_Read(uint8* buffer, uint32 addr, uint32 length)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
    
    op.addr = addr;                     //地址
    op.len = length;                    //长度
    op.obj = buffer;                    //缓存
    op.type = DEV_FLS_TYPE_FLS;         //类型
    op.rw = DEV_FLS_RW_R;               //读写方式
    
    result = DevFls_ProcMSG(&op);
    return result;
}

/************************************************************************
 * @function: SYS_FILE_Test
 * @描述: flash功能测试
 * @参数: 
 * 
 * @返回: 
 * @return: uint32  SYS_ERR_OK 功能正常, SYS_ERR_FT 功能异常(该类型为了与dev_file2.c一致)
 * @说明: 
 * @作者: yzy (2013/6/8)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint32 SYS_FILE_Test(void)
{
    uint8 cmp1[8];                          //原始数据
    uint8 cmp2[8];                          //修改后理论写入数据
    uint8 cmp3[8];                          //修改后实际写入数据
    uint32 err = 0;

                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
    
    //越级使用底层函数,以便指定操作某块flash
    uint8 ReadFlsInChip(uint8* buffer, uint32 address, uint16 length, uint8 x);
    uint8 WriteFlsInChip(uint8* buffer, uint32 addr, uint16 length, uint8 x, uint8 sdmode);
                                            //执行测试
    for(uint8 k = 0; k < FLS_CHIP_NO; k++)
    {
                                            //读取原始数据
        if(ReadFlsInChip(cmp1, 0, 5, k) != SYS_ERR_OK)
        {
            err |= Bit_Map8[k];
            break;
        }
                                            //修改原始数据
        for(uint8 i = 0; i < 5; i++)
        {
            cmp2[i] = cmp1[i] + 2;
        }
                                            //写入理论写入数据
        if(WriteFlsInChip(cmp2, 0, 5, k, 0) != SYS_ERR_OK)
        {
            err |= Bit_Map8[k];
            break;
        }
                                            //读取实际写入数据
        if(ReadFlsInChip(cmp3, 0, 5, k) != SYS_ERR_OK)
        {
            err |= Bit_Map8[k];
            break;
        }
        
        WriteFlsInChip(cmp1, 0, 5, k, 0);   //恢复原始值
        if(memcmp(cmp2, cmp3, 5) != 0)   //比对理论实际值
        {
            err |= Bit_Map8[k];
            break;
        }
    }
    
    return err;
}

/***********************************************************
 * @function_name: SYS_FILE_DB_ReadFrom
 * @function_file: es_file.c
 * @描述:从指定的偏移开始读取数据
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/21)
 **********************************************************/
TResult SYS_FILE_DB_ReadFrom(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
    
    if(!(db->RW & TDB_MODE_R))          //需要支持读属性才允许读取
    {
        return SYS_ERR_TYPE;
    }
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }

    op.addr = db->start + addr;         //地址
    op.len = length;                    //长度
    op.obj = buffer;                    //缓存
    op.type = DEV_FLS_TYPE_FLS;         //类型
    op.rw = DEV_FLS_RW_R;               //读写方式

    result = DevFls_ProcMSG(&op);

    return result;
}


/***********************************************************
 * @function_name: SYS_FILE_DB_ReadWithCRC
 * @function_file: es_file.c
 * @描述:带校验的读取
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_ReadWithCRC(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TResult r;
    r = SYS_FILE_DB_ReadFrom(db, buffer, length, addr);

    if(ER(r))
    {
        if(!CheckBufferCRC(buffer, length, true))
        {
            memset_s(buffer, 0x00, length);
            r = SYS_ERR_CHK;
        }
    }
    return r;
}

/***********************************************************
 * @function_name: SYS_FILE_DB_ReadWithBackup
 * @function_file: es_file.c
 * @描述:带备份+校验的读取
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_ReadWithBackup(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TResult r; 

    if(!(db->RW & TDB_MODE_BK))         //确保支持备份块属性
    {
        return SYS_ERR_TYPE;
    }
                                        //确认参数的正确性
    SYS_VAR_CHECK(addr + length > (db->length >> 1));
                                        //备份地址
    uint32 bkaddr = addr + (db->length >> 1);
                                        //带CRC校验的读取
    r = SYS_FILE_DB_ReadWithCRC(db, buffer, length, addr);

    if(!ER(r))                          //主区错误则读取备份区
    {
        r = SYS_FILE_DB_ReadWithCRC(db, buffer, length, bkaddr);
        if(ER(r))
        {
            return SYS_ERR_MAIN;         //主区错误,但备份区正确
        }
    }
    return r;                           //返回写入结果

}


/***********************************************************
 * @function_name: SYS_FILE_DB_WriteFrom
 * @function_file: es_file.c
 * @描述:写入数据
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_WriteFrom(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    if(!(db->RW & TDB_MODE_W))          //需要支持读属性才允许读取
    {
        return SYS_ERR_TYPE;
    }
                                        //确认参数是否正确
    SYS_VAR_CHECK(db->length < addr + length);
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }

    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;

    op.addr = db->start + addr;         //地址
    op.len = length;                    //长度
    op.obj = buffer;                    //缓存
    op.type = DEV_FLS_TYPE_FLS;         //类型
    op.rw = DEV_FLS_RW_W;               //读写方式

    result = DevFls_ProcMSG(&op);
    
    return result; 
}



/***********************************************************
 * @function_name: SYS_FILE_DB_SetFrom
 * @function_file: es_file.c
 * @描述:设置相同数据
 * 
 * 
 * @参数: 
 * @param:db    
 * @param:addr 
 * @param:length
 * @param:templet
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_SetFrom(TDataBlock* db, uint32 addr, uint32 length, uint8 templet)
{
    if(!(db->RW & TDB_MODE_W))          //需要支持读属性才允许读取
    {
        return SYS_ERR_TYPE;
    }
                                        //确认参数是否正确
    SYS_VAR_CHECK(db->length < addr + length);
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }

    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;

    op.addr = db->start + addr;         //地址
    op.len = length;                    //长度
    op.obj = (Object)templet;           //样本
    op.type = DEV_FLS_TYPE_FLS;         //类型
    op.rw = DEV_FLS_RW_S;               //读写方式

    result = DevFls_ProcMSG(&op);
    
    return result;
}


/***********************************************************
 * @function_name: SYS_FILE_DB_WriteWithCRC
 * @function_file: es_file.c
 * @描述:带CRC校验的写入,注意写入的缓存要开大两个字节
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_WriteWithCRC(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    BuildBufferCRC(buffer, length, true);
    return SYS_FILE_DB_WriteFrom(db, buffer, length, addr);
}


/***********************************************************
 * @function_name: SYS_FILE_DB_WriteWithBackup
 * @function_file: es_file.c
 * @描述:写入数据,不仅带CRC校验,还带备份,这样的block实际能用的大小只有
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_WriteWithBackup(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TResult r1, r2; 

    if(!(db->RW & TDB_MODE_BK))         //确保支持备份块属性
    {
        return SYS_ERR_TYPE;
    }
                                        //确认参数的正确性
    SYS_VAR_CHECK(addr + length > (db->length >> 1));
                                        //备份地址
    uint32 bkaddr = addr + (db->length >> 1);
                                        //带CRC校验的写入
    r1 = SYS_FILE_DB_WriteWithCRC(db, buffer, length, addr);
    r2 = SYS_FILE_DB_WriteWithCRC(db, buffer, length, bkaddr);
    
    if(!ER(r1) && ER(r2))               //主区错误,备份区正确
    {
        return SYS_ERR_MAIN;
    }

    if(!ER(r2) && ER(r1))               //备份区错误,主区正确
    {
        return SYS_ERR_BACK;
    }
    
    if(!ER(r2) && !ER(r1))              //备份区错误,主区错误
    {
        return SYS_ERR_CHK;
    }
    SYS_OK();
}

/*******************************************************************************
** @function_name:  SYS_FILE_DB_WriteSpec
** @function_file:  dev_file.c
** @描述: 文件数据块特殊写操作(适用于无须保存原有数据的写操作,特别适用于大数据量连续写入)
** 
** 
** @参数: 
** @param: db 
** @param: buffer 
** @param: length 
** @param: addr 
** 
** @返回: 
** @return:  TResult   
** @作者: yzy (2011-07-25)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
TResult SYS_FILE_DB_WriteSpec(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    if(!(db->RW & TDB_MODE_W))          //需要支持写属性才允许读取
    {
        return SYS_ERR_TYPE;
    }
                                        //确认参数是否正确
    SYS_VAR_CHECK(db->length < addr + length);
    
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }

    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;

    op.addr = db->start + addr;         //地址
    op.len = length;                    //长度
    op.obj = buffer;                    //缓存
    op.type = DEV_FLS_TYPE_FLS;         //类型
    op.rw = DEV_FLS_RW_WS;              //读写方式

    result = DevFls_ProcMSG(&op);
    
    return result;
}

/***********************************************************
 * @function_name: SYS_FILE_DB_BufferCheck
 * @function_file: es_file.c
 * @描述:校验存放在buffer中长度为lenght的数据,
 *        如果buffer中的数据CRC校验通过则返回.
 *        否则从flash中更新数据.flash数据存放在db,地址为addr.
 * 
 * 
 * @参数: 
 * @param:db  
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/3/3)
 **********************************************************/
TResult SYS_FILE_DB_BufferCheck(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TResult r = SYS_ERR_OK;

    if(!CheckBufferCRC(buffer, length, true))
    {
        r = SYS_ERR_CHK;
    }

    if(!ER(r))
    {
        r = SYS_FILE_DB_ReadWithBackup(db, buffer, length, addr);
    }
    
    return r;
}   


/***********************************************************
 * @function_name: SYS_FILE_DB_Erase
 * @function_file: es_file.c
 * @描述:
 * 
 * 
 * @参数: 
 * @param: db 要擦除的数据块
 * @param: sectorfrom 起始擦除扇区(0~)
 * @param: sectornum 擦除扇区的数量(超出数据块大小部分不擦)
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: yzy (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_Erase(TDataBlock* db, uint32 sectorfrom, uint32 sectornum)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
    uint32 dbsectors;                   //数据块包含的总扇区数量
                                        //确认参数是否正确
    SYS_VAR_CHECK(sectorfrom > sectornum);
    SYS_VAR_CHECK(sectornum == 0);
    
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != gs_TkTcbCur->tkid)
    {
        return SYS_ERR_FT;
    }
                                        //总扇区数量
    dbsectors = (db->length >> 12);
                                        //起始sector必须在数据块范围内
    if(sectorfrom >= dbsectors)
    {
        return SYS_ERR_FT;
    }
                                        //擦除扇区数量必须在数据块范围内
    if((sectorfrom + sectornum) > dbsectors)
    {
        sectornum = dbsectors - sectorfrom;
    }

    op.addr = db->start + (sectorfrom << 12);//要擦除区域在数据块内的绝对地址
    op.len = sectornum;                 //擦除扇区总数
    op.obj = __NULL;
    op.type = DEV_FLS_TYPE_FLS;
    op.rw = DEV_FLS_RW_E;
    
    result = DevFls_ProcMSG(&op);
    
    return result;
}


#endif






/************************************************************************
 * @function: SYS_FILE_Init
 * @描述: 文件设备初始化
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/21)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_FILE_Init(void)
{
    //flash使用SPI接口,故不需要单独创建资源.
#ifndef __NO_SYS__
    SYS_SEM_Create(1, &gs_FileRes);
#endif
}












