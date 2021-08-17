/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: SYS_file.c
**创   建   人: yzy
**最后修改日期: 2018年2月20日
**描        述: 文件设备操作
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2018年2月20日
** 描　述: 原始版本
********************************************************************************************************/

#define EXT_FILE
#define EXT_DEV_FILE
#define EXT_IIC_EEPROM
#define EXT_MXFLASH

//	#include <stdint.h>
//	#include "bdef.h"
//	#include "cdef.h"
//	#include "bsp.h"
//	
//	#include "iomap.h"
//	//	#include "flsmap.h"
//	//	#include <lib_app.h>
//	
//	
//	//	#include "apis.h"
//	#include "k_types.h"
//	#include "k_api.h"
//	//#include "uartcfg.h"
//	#include "lib_app.h"
//	#include "hal.h"
#include "aos/include.h"
#include "hal.h"
#include "flsmap.h"
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
 * @修改人: houpm (2010/2/20)
 **********************************************************/
uint8 DevFls_ProcMSG(TDevFlsOP* op)
{
    uint8 result = SYS_ERR_FT;
    if(op->type == DEV_FLS_TYPE_IDEL)//操作空
    {

    }

#if SYS_EEPROM_EN > 0
    
    else if(op->type == DEV_FLS_TYPE_EEPROM)//操作FRAM
    {
       switch(op->rw)
       {
           case DEV_FLS_RW_R:           //读操作
               result = Eeprom_Read((uint8*)op->obj, op->addr, op->len);
               break;
           case DEV_FLS_RW_W:           //写操作
               result = Eeprom_Write((uint8*)op->obj, op->addr, op->len);
               break;
       }
    }
#endif    
#if SYS_FILE_EN > 0

    else if(op->type == DEV_FLS_TYPE_FLS)     //操作Flash
    {
       switch(op->rw)
       {
//           case DEV_FLS_RW_R:           //读操作
//               result = FLS_Read((uint8*)op->obj, op->addr, op->len);
//               break;
//           case DEV_FLS_RW_W:           //写操作
//               FLS_PreWrite();
//               result = FLS_Write((uint8*)op->obj, op->addr, op->len);
//               break;
//           case DEV_FLS_RW_S:           //设置操作
//               FLS_PreWrite();
//               result = FLS_Set((uint8)(uint32)op->obj, op->addr, op->len);
//               break;
//           case DEV_FLS_RW_E:           //擦除操作
//               FLS_PreWrite();
//               result = FLS_Erase(op->addr, op->len);
//               break;
//           case DEV_FLS_RW_BE:           //擦除操作
//               FLS_PreWrite();
//               result = FLS_BErase(op->addr, op->len);
//               break;
//               
//           case DEV_FLS_RW_WS:          //Flash特殊写入操作
//               FLS_PreWrite();
//               result = FLS_WriteSpec((uint8*)op->obj, op->addr, op->len);
//               break;
       }
    }
#endif
#if SYS_FRAM_EN > 0
    #if SYS_FLASH_FRAM_EN == 0

    else if(op->type == DEV_FLS_TYPE_FRAM)//操作FRAM
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
    }

    #else

    else if(op->type == DEV_FLS_TYPE_VFRAM)//操作VFRAM
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
    }
    #endif
#endif
    return result;

    
}



#if SYS_FILE_EN > 0

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
 * @修改人: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FILE_Write(uint8* buffer, uint32 addr, uint32 length)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != krhino_cur_task_get())
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
 * @修改人: houpm (2010/2/21)
 **********************************************************/
TResult SYS_FILE_Read(uint8* buffer, uint32 addr, uint32 length)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != krhino_cur_task_get())
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
    if(guc_FileUserTkid != krhino_cur_task_get())
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
        if(CmpBuffer(cmp2, cmp3, 5) != 0)   //比对理论实际值
        {
            err |= Bit_Map8[k];
            break;
        }
    }
    
    return err;
}


/***********************************************************
 * @function_name: SYS_FILE_Open
 * @function_file: SYS_file.c
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
TResult SYS_FILE_Open(void)
{
    //申请资源
    if(guc_FileUserTkid != NULL && guc_FileUserTkid == krhino_cur_task_get())
        return SYS_ERR_OK;
    krhino_sem_take(&gs_FileRes, RHINO_WAIT_FOREVER);
    guc_FileUserTkid = krhino_cur_task_get();  //为当前用户进程编号
    return SYS_ERR_OK;
}

/***********************************************************
 * @function_name: SYS_FILE_Close
 * @function_file: SYS_file.c
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
    if(guc_FileUserTkid == krhino_cur_task_get())
    {
        guc_FileUserTkid = NULL;        //恢复成初始值
        //krhino_sem_give(&gs_FileRes);    //释放这个信号量
        krhino_sem_give(&gs_FileRes);
    }
}

#include <assert.h>


/***********************************************************
 * @function_name: SYS_FILE_DB_Open
 * @function_file: SYS_file.c
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
 * @修改人: houpm (2010/2/21)
 **********************************************************/
TResult SYS_FILE_DB_Open(uint16 ID, TDataBlock* db, TDB_MODE RW)
{
    uint32 start;
    uint16 nblock;
    uint16 k;
                                        //设置数据操作块的变量
    start = 0;
    nblock = 0;
    for(k = 0; k < LEN_DB_LIST; k++)
    {
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
    }
    
    if(k == LEN_DB_LIST)
    {
        return SYS_ERR_FT;
    }
                                        //
    if((gss_FileBlocks[k].atr & RW) != RW)
    {
        return SYS_ERR_TYPE;
    }
                                        //
    assert(start < 0x000FFFFF);         //确保左移12位后不溢出uint32_max.
    db->start = DB_LEN_BLOCK(start);
    db->length = DB_LEN_BLOCK(nblock);
    db->RW = gss_FileBlocks[k].atr;
    db->ID = ID;
    
    if((db->start == 0) && (nblock > 1))//升级数据块首个block用来存放updatemark
    {
        db->start = DB_LEN_BLOCK(1);
        nblock -= 1;                    //实际容量少一个block
    }
                                        //申请资源
    SYS_FILE_Open();
    
    SYS_OK();
}


/***********************************************************
 * @function_name: SYS_FILE_DB_Close
 * @function_file: SYS_file.c
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
 * @修改人: houpm (2010/2/21)
 **********************************************************/
void SYS_FILE_DB_Close(TDataBlock* db)
{
    SYS_FILE_Close();
}


/***********************************************************
 * @function_name: SYS_FILE_DB_ReadFrom
 * @function_file: SYS_file.c
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
 * @修改人: houpm (2010/2/21)
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
    if(guc_FileUserTkid != krhino_cur_task_get())
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
 * @function_file: SYS_file.c
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
 * @修改人: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_ReadWithCRC(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    TResult r;
    r = SYS_FILE_DB_ReadFrom(db, buffer, length, addr);

    if(ER(r))
    {
        if(!CheckBufferCRC(buffer, length, true))
        {
            memset(buffer, 0x00, length);
            r = SYS_ERR_CHK;
        }
    }
    return r;
}

/***********************************************************
 * @function_name: SYS_FILE_DB_ReadWithBackup
 * @function_file: SYS_file.c
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
 * @修改人: houpm (2010/2/22)
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
 * @function_file: SYS_file.c
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
 * @修改人: houpm (2010/2/22)
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
    if(guc_FileUserTkid != krhino_cur_task_get())
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
 * @function_file: SYS_file.c
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
 * @修改人: houpm (2010/2/22)
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
    if(guc_FileUserTkid != krhino_cur_task_get())
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
 * @function_file: SYS_file.c
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
 * @修改人: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FILE_DB_WriteWithCRC(TDataBlock* db, uint8* buffer, uint32 length, uint32 addr)
{
    BuildBufferCRC(buffer, length, true);
    return SYS_FILE_DB_WriteFrom(db, buffer, length, addr);
}


/***********************************************************
 * @function_name: SYS_FILE_DB_WriteWithBackup
 * @function_file: SYS_file.c
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
 * @修改人: houpm (2010/2/22)
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
    if(guc_FileUserTkid != krhino_cur_task_get())
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
 * @function_file: SYS_file.c
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
 * @修改人: houpm (2010/3/3)
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
 * @function_file: SYS_file.c
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
 * @修改人: houpm (2010/2/22)
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
    if(guc_FileUserTkid != krhino_cur_task_get())
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

TResult SYS_FILE_DB_BErase(TDataBlock* db, uint32 sectorfrom, uint32 sectornum)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
    uint32 dbsectors;                   //数据块包含的总扇区数量
                                        //确认参数是否正确
    SYS_VAR_CHECK(sectorfrom > sectornum);
    SYS_VAR_CHECK(sectornum == 0);
    
                                        //判断是否已被其他进程占用
    if(guc_FileUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }
                                        //总扇区数量
    dbsectors = (db->length >> 16);
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

    op.addr = db->start + (sectorfrom << 16);//要擦除区域在数据块内的绝对地址
    op.len = sectornum;                 //擦除扇区总数
    op.obj = __NULL;
    op.type = DEV_FLS_TYPE_FLS;
    op.rw = DEV_FLS_RW_BE;
    
    result = DevFls_ProcMSG(&op);
    
    return result;
}


#endif


#if SYS_FRAM_EN > 0


#if (SYS_FLASH_FRAM_EN > 0)


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
TResult SYS_FRAM_Open(void)
{
//	    SYS_SEM_Wait(&gs_FramRes, 0);
    krhino_sem_take(&gs_FramRes, RHINO_WAIT_FOREVER);
    guc_FramUserTkid = krhino_cur_task_get();  //为当前用户进程编号
    
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
 * @作者: yzy (2018/5/2)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_FRAM_Close(void)
{
    if(guc_FramUserTkid == krhino_cur_task_get())
    {
        guc_FramUserTkid = 0xFF;        //恢复成初始值
        krhino_sem_give(&gs_FramRes);    //释放这个信号量
    }
}

/************************************************************************
 * @function: SYS_FRAM_SafeWrite
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
TResult SYS_FRAM_SafeWrite(uint8* data, uint16 num)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
                                        //判断是否已被其他进程占用
    if(guc_FramUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }
    
    op.addr = num;                      //地址
    op.len = (uint32)&gs_flsFramConst;  //长度(虚拟铁电信息结构体)
    op.obj = data;                      //缓存
    op.type = DEV_FLS_TYPE_VFRAM;       //类型
    op.rw = DEV_FLS_RW_W;               //读写方式
    result = DevFls_ProcMSG(&op);

    return result;                      //未知错误
}

/************************************************************************
 * @function: SYS_FRAM_SafeRead
 * @描述: 从一个铁电条目中读取数据
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
TResult SYS_FRAM_SafeRead(uint8* data, uint16 num)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
                                        //判断是否已被其他进程占用
    if(guc_FramUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }
    
    op.addr = num;                      //地址
    op.len = (uint32)&gs_flsFramConst;  //长度(虚拟铁电信息结构体)
    op.obj = data;                      //缓存
    op.type = DEV_FLS_TYPE_VFRAM;       //类型
    op.rw = DEV_FLS_RW_R;               //读写方式
    result = DevFls_ProcMSG(&op);
    
    return result;                      //未知错误
}



/************************************************************************
 * @function: Fram_SafeRead
 * @描述: 从铁电的安全数据区中连续的读取数据
 *         (使用前后需要执行open和close)
 * 
 * @参数: 
 * @param: data 读取的数据需要放入的缓存
 * @param: sno 读取的数据的起始块号
 * @param: len 读取的数据的长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Fram_SafeRead(uint8* data, uint16 sno, uint16 len)
{
    uint16 k = len >> 5;                //获取总共需要的数据块数
    uint8 t[32];

    if(len & 0x1F)
    {
        k++;
    }

    for(uint16 m = 0; m < k; m++)       //分块读取数据
    {
                                        //读取数据
        if(SYS_FRAM_SafeRead(t, sno + m) != SYS_ERR_OK)
        {
            if(SYS_FRAM_SafeRead(t, sno + m) != SYS_ERR_OK)
            {
                continue;
            }
        }
        if((m < (k - 1)) || !(len & 0x1F))  //判断是否最后的字节了
        {
            memcpy(data + (m << 5), t, 32);
        }
        else
        {
            memcpy(data + (m << 5), t, len & 0x1F);
        }
    }
    
    SYS_OK();
}


/************************************************************************
 * @function: Fram_SafeWrite
 * @描述: 从铁电的安全数据区中连续的写入数据
 *       (使用前后需要执行open和close)
 * 
 * @参数: 
 * @param: data 写入的数据的缓存
 * @param: sno 写入的数据的起始块号
 * @param: len 写入的数据的长度
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Fram_SafeWrite(uint8* data, uint16 sno, uint16 len)
{
    uint16 k = len >> 5;                //获取总共需要的数据块数

    if(len & 0x1F)
    {
        k ++;
    }

    for(uint16 m = 0; m < k; m++)       //分块写入数据
    {
        if(SYS_FRAM_SafeWrite(data + (m << 5), sno + m) != SYS_ERR_OK)
        {
            SYS_FRAM_SafeWrite(data + (m << 5), sno + m);
        }
    }
    
    SYS_OK();
}



/***********************************************************
 * @function_name: SYS_FRAM_Write
 * @function_file: SYS_file.c
 * @描述:写入铁电数据,buffer的最后两个字节预留给CRC校验
 * 
 * 
 * @参数: 
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FRAM_Write(uint8* buffer, uint32 length, uint32 addr)
{
    SYS_OK();
}

/***********************************************************
 * @function_name: SYS_FRAM_Read
 * @function_file: SYS_file.c
 * @描述:从铁电里面读取数据
 * 
 * 
 * @参数: 
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: houpm (2010/2/22)
 **********************************************************/
TResult SYS_FRAM_Read(uint8* buffer, uint32 length, uint32 addr)
{
    SYS_OK();
}



/************************************************************************
 * @function: SYS_FRAM_Init
 * @描述: 铁电设备初始化
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/21)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_FRAM_Init(void)
{
    //fram使用SPI接口,故不需要单独创建资源.
}




#endif               //#if SYS_FLASH_FRAM_EN > 0




#endif              //#if SYS_FRAM_EN > 0


#if (SYS_EEPROM_EN > 0)

/************************************************************************
 * @function: SYS_EEPROM_Open
 * @描述: 向一个eeprom条目中写入数据
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
TResult SYS_EEPROM_Open(void)
{
    krhino_sem_take(&gs_EepromRes, RHINO_WAIT_FOREVER);
    guc_EepromUserTkid = krhino_cur_task_get();  //为当前用户进程编号
    return SYS_ERR_OK;
}


/************************************************************************
 * @function: SYS_EEPROM_Close
 * @描述: eeprom关闭
 * 
 * @参数: 
 * @param:  
 * @返回: 
 * @说明: 
 * @作者: yzy (2018/5/2)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_EEPROM_Close(void)
{
    if(guc_EepromUserTkid == krhino_cur_task_get())
    {
        guc_EepromUserTkid = NULL;        //恢复成初始值
        krhino_sem_give(&gs_EepromRes);    //释放这个信号量
    }
}

/************************************************************************
 * @function: Eeprom_SafeWrite
 * @描述: 向一个eeprom条目中写入数据
 * 
 * @参数: 
 * @param: data 欲写入的内容
 * @param: num eeprom条目编号
 * 
 * @返回: 
 * @return: TResult
 * @说明: 
 * @作者: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Eeprom_SafeWrite(uint8* data, uint32 addr, uint16 length)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
                                        //判断是否已被其他进程占用
    if(guc_EepromUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }
    
    op.addr = addr;                      //地址
    op.len = length;  //长度
    op.obj = data;                      //缓存
    op.type = DEV_FLS_TYPE_EEPROM;       //类型
    op.rw = DEV_FLS_RW_W;               //读写方式
    result = DevFls_ProcMSG(&op);

    return result;                      //未知错误
}

/************************************************************************
 * @function: Eeprom_SafeRead
 * @描述: 从eeprom中读取数据
 * 
 * @参数: 
 * @param: data 一个eeprom条目,大小至少为32字节,否则会导致缓存溢出
 * @param: addr 偏移地址
 * @param: length 长度
 * @返回: 
 * @return: TResult  
 * @说明: 
 * @作者: yzy (2013/10/28)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 Eeprom_SafeRead(uint8* data, uint32 addr, uint16 length)
{
    TDevFlsOP op;                       //定义一个Flash操作结构
    uint8 result;
                                        //判断是否已被其他进程占用
    if(guc_EepromUserTkid != krhino_cur_task_get())
    {
        return SYS_ERR_FT;
    }
    
    op.addr = addr;                      //地址
    op.len = (uint32)length;  //长度
    op.obj = data;                      //缓存
    op.type = DEV_FLS_TYPE_EEPROM;       //类型
    op.rw = DEV_FLS_RW_R;               //读写方式
    result = DevFls_ProcMSG(&op);
    
    return result;                      //未知错误
}



/***********************************************************
 * @function_name: SYS_EEPROM_Write
 * @function_file: SYS_file.c
 * @描述:写入eeprom数据,buffer的最后两个字节预留给CRC校验
 * 
 * 
 * @参数: 
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: houpm (2010/2/22)
 **********************************************************/
TResult SYS_EEPROM_Write(uint8* buffer, uint32 length, uint32 addr)
{
    SYS_OK();
}

/***********************************************************
 * @function_name: SYS_FRAM_Read
 * @function_file: SYS_file.c
 * @描述:从eeprom里面读取数据
 * 
 * 
 * @参数: 
 * @param:buffer  
 * @param:length  
 * @param:addr  
 * 
 * @返回: 
 * @return: TResult 
 * @作者:
 *---------------------------------------------------------
 * @修改人: houpm (2010/2/22)
 **********************************************************/
TResult SYS_EEPROM_Read(uint8* buffer, uint32 length, uint32 addr)
{
    SYS_OK();
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
    
    krhino_sem_create(&gs_FileRes, "flash_sem", 1);
    krhino_sem_create(&gs_EepromRes, "eeprom_sem", 1);
    guc_FileUserTkid = NULL;
    guc_EepromUserTkid = NULL;
}
/************************************************************************
 * @function: SYS_FILE_Start
 * @描述: 文件设备开始
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/21)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/


unsigned char g_ucFileOpenFlag = 0xFF;

void SYS_FILE_Start(void)
{
    uint8_t ret = SYS_ERR_OK;
#if SYS_EEPROM_EN > 0
    Init_I2C_EEPROM(&gs_EepromPort);            //初始化i2c总线
#endif
#if SYS_FILE_EN > 0
    ret = Flash_Init();
#endif
    if(ret == CBB_ERR_OK)
    {
        g_ucFileOpenFlag = CON_FILE_OPEN;
    }

}


/************************************************************************
 * @function: SYS_FILE_Stop
 * @描述: 文件设备关闭
 * @参数: 
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/6/21)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/

void SYS_FILE_Stop(void)
{
#if SYS_EEPROM_EN > 0
    I2C_EEPROM_Idel();            //初始化i2c总线
#endif

#if SYS_FILE_EN > 0
    Flash_Close();
#endif


}







