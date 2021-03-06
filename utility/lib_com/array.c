/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: Array.c
**创   建   人: yzy
**最后修改日期: 2007年1月15日
**描        述: 队列函数集
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2007年1月15日
** 描　述: 原始版本
********************************************************************************************************/
#define EXT_ARRAY

#include "bdef.h"
#include "cdef.h"

#include "libs.h"
#include <k_api.h>


#if SYS_FIFO_EN > 0


/************************************************************************
 * @function: SYS_FIFO_Create
 * @描述: 创建一个FIFO
 * 
 * @参数: 
 * @param: type 创建的FIFO的数据单元的类型
 * @param: fifo 需要创建的fifo
 * @param: buffer 需要创建的FIFO的数据的缓存位置
 * @param: length 需要创建的FIFO的数据单元的长度(字节数)
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_FIFO_Create(size_array type, FIFO* fifo, void* buffer, uint16 length)
{
    SYS_VAR_CHECK(type > length);
    //SYS_VAR_CHECK(((uint32)buffer) & 0x03);
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();

    fifo->type = type;                  //获取数据类型
    fifo->count = 0;                    //队列数据为0
                                        //计算获取队列的容量
    fifo->capcity = length / type;
    fifo->buffers = buffer;             //缓存起始的位置
                                        //缓存结束的位置
    fifo->buffere = (uint8*)buffer + (uint16)type * fifo->capcity;
    fifo->head = buffer;                //头
    fifo->tail = buffer;                //尾
    
    RHINO_CPU_INTRPT_ENABLE();
    SYS_OK();                            //创建成功,返回ok
}



/************************************************************************
 * @function: SYS_FIFO_Enqueue
 * @描述: 将一个数据排到队列里面去
 * 
 * @参数: 
 * @param: fifo 需要入队的fifo
 * @param: content 入队的内容
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_FIFO_Enqueue(FIFO* fifo, void* content)
{
    void* temp;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    if(fifo->count >= fifo->capcity)    //判断当前队列是否已经满了  
    {
        RHINO_CPU_INTRPT_ENABLE();
        return SYS_ARRAY_ERR_FUL;        //返回队列已经满了
    }
    
    temp = fifo->tail;                  //获取当前的队尾
    
                                        //队尾往前移动
    fifo->tail = (uint8*)fifo->tail + (uint16)fifo->type;
    
    if(fifo->tail >= fifo->buffere)     //卷绕处理
    {
        fifo->tail = fifo->buffers;
    }
    
    fifo->count++;                      //队列中有效项的数量增加
                                        //将数据拷贝到队尾
    memcpy(temp, content, (uint16)fifo->type);
    RHINO_CPU_INTRPT_ENABLE();
    SYS_OK();                            //返回入队成功  
}



/************************************************************************
 * @function: SYS_FIFO_Dequeue
 * @描述: 从FIFO里面获取一个数据,这个数据不再存在FIFO里面
 * 
 * @参数: 
 * @param: fifo 需要出队的fifo, 注意这个函数本身是进程安全的,但是获取的内容则不一定了.
 * 
 * @返回: 
 * @return: void*  __NULL表示队列空,其它:出队数据的内容的地址
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void* SYS_FIFO_Dequeue(FIFO* fifo)
{
    void* temp;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    if(fifo->count == 0)                //判断是否队列为空
    {
        RHINO_CPU_INTRPT_ENABLE();
        return __NULL;
    }
    fifo->count--;
    
    temp = fifo->head;                  //获取当前的队列头
    fifo->head = (uint8*)fifo->head + (uint16)fifo->type;
    
    if(fifo->head >= fifo->buffere)     //卷绕处理
    {
        fifo->head = fifo->buffers;
    }
 	
    RHINO_CPU_INTRPT_ENABLE();
    return temp;
}

/************************************************************************
 * @function: SYS_FIFO_Get
 * @描述: 从FIFO里面获取一个数据,这个数据不再存在FIFO里面
 * 
 * @参数: 
 * @param: fifo 需要出队的fifo
 * @param: content 获取的数据保存的位置,这个函数是进程安全的.
 * 
 * @返回: 
 * @return: uint8  成功/失败
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_FIFO_Get(FIFO* fifo, void* content)
{
    void* temp;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    if(fifo->count == 0)                //判断是否队列为空
    {
        RHINO_CPU_INTRPT_ENABLE();
        return SYS_ARRAY_ERR_EPY;        //缓存是空的
    }
    fifo->count--;
    
    temp = fifo->head;                  //获取当前的队列头
    fifo->head = (uint8*)fifo->head + (uint16)fifo->type;
    
    if(fifo->head >= fifo->buffere)     //卷绕处理
    {
        fifo->head = fifo->buffers;
    }
                                        //获取数据  
    memcpy(content, temp, (uint16)fifo->type);
    RHINO_CPU_INTRPT_ENABLE();
    SYS_OK();
}




/************************************************************************
 * @function: SYS_FIFO_Peek
 * @描述: 从FIFO里面获取一个数据,这个数据还在FIFO里面注意这个函数本身是进程安全的,但是获取的内容则不一定了.
 * 
 * @参数: 
 * @param: fifo 需要获取数据的fifo
 * 
 * @返回: 
 * @return: void*  __NULL表示队列空   其它:获取数据的内容的地址
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void* SYS_FIFO_Peek(FIFO* fifo)
{
    void* temp;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    if(fifo->count == 0)                //判断是否队列为空
    {
        RHINO_CPU_INTRPT_ENABLE();
        return __NULL;
    }
    temp = fifo->head;                  //获取当前的队列头
    RHINO_CPU_INTRPT_ENABLE();
    return temp;
}


/************************************************************************
 * @function: SYS_FIFO_Clear
 * @描述: 清除FIFO
 * 
 * @参数: 
 * @param: fifo 需要清除数据的fifo
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_FIFO_Clear(FIFO* fifo)
{
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    fifo->count = 0;
    fifo->tail = fifo->buffers;
    fifo->head = fifo->buffers;
    RHINO_CPU_INTRPT_ENABLE();
}

#endif

#if SYS_LIFO_EN > 0

/************************************************************************
 * @function: SYS_LIFO_Create
 * @描述: 创建一个LIFO
 * 
 * @参数: 
 * @param: type 创建的LIFO的数据单元的类型
 * @param: lifo 需要创建的lifo
 * @param: buffer 需要创建的LIFO的数据的缓存位置
 * @param: length 需要创建的LIFO的数据的长度(字节数)
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_LIFO_Create(size_array type, LIFO* lifo, void* buffer, uint16 length)
{
    SYS_VAR_CHECK(type > length);
    SYS_VAR_CHECK(((uint32)buffer) & 0x03);
    SYS_ENTER_SCRT();

    lifo->type = type;                  //获取数据类型

                                        //计算获取队列的容量
    lifo->bottom = buffer;              //缓存起始的位置
                                        //结束位置
    lifo->top = (uint8*)buffer + (length / type) * (uint16)type;
    
    lifo->sp = lifo->bottom;            //当前指针                                              
    SYS_EXIT_SCRT();
    SYS_OK();                            //返回创建成功
}



/************************************************************************
 * @function: SYS_LIFO_Push
 * @描述: 将一个数据推入LIFO中
 * 
 * @参数: 
 * @param: lifo 需要入队的lifo
 * @param: content 入队的内容
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_LIFO_Push(LIFO* lifo, void* content)
{
    void* temp;
    
    SYS_ENTER_SCRT();
    if(lifo->sp >= lifo->top)           //判断堆栈是否已经满了
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_FUL;            //返回入栈错误
    }
    
    temp = lifo->sp;                    //获取当前的位置
                                        //位置前移  
    lifo->sp = (uint8*)lifo->sp + (uint16)lifo->type;
    
                                        //转移数据
    memcpy(temp, content, (uint16)lifo->type);
    
    SYS_EXIT_SCRT();
    SYS_OK();                            //返回入栈成功       
}




/************************************************************************
 * @function: SYS_LIFO_Pop
 * @描述: 从LIFO里面获取一个数据,这个数据不再存在LIFO里面
 * 
 * @参数: 
 * @param: lifo 需要出队的lifo
 * 
 * @返回: 
 * @return: void*  __NULL表示队列空 其它:出队数据的内容的地址
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void* SYS_LIFO_Pop(LIFO* lifo)
{
    void* temp;
    
    SYS_ENTER_SCRT();
    
    if(lifo->sp <= lifo->bottom)        //确认栈里面还有数据存在
    {
        SYS_EXIT_SCRT();
        return __NULL;
    }
                                        //弹出一个数据
    lifo->sp = (uint8*)lifo->sp - (uint16)lifo->type;
    temp = lifo->sp;
    SYS_EXIT_SCRT();
    return temp;
}



/************************************************************************
 * @function: SYS_LIFO_Get
 * @描述: 从LIFO里面获取一个数据,这个数据不再存在LIFO里面
 * 
 * @参数: 
 * @param: lifo 需要出队的lifo
 * @param: content 保存弹出的数据内容
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_LIFO_Get(LIFO* lifo, void* content)
{
    void* temp;
    
    SYS_ENTER_SCRT();
    
    if(lifo->sp <= lifo->bottom)        //确认栈里面还有数据存在
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_EPY;
    }
                                        //弹出一个数据
    lifo->sp = (uint8*)lifo->sp - (uint16)lifo->type;
    temp = lifo->sp;
    
    memcpy(content, temp, (uint16)lifo->type);
    SYS_EXIT_SCRT();
    SYS_OK();
}





/************************************************************************
 * @function: SYS_LIFO_Peek
 * @描述: 从LIFO里面获取一个数据,这个数据还在LIFO里面
 * 
 * @参数: 
 * @param: lifo 需要获取数据的lifo
 * 
 * @返回: 
 * @return: void*  __NULL表示队列空 其它:出队数据的内容的地址
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void* SYS_LIFO_Peek(LIFO* lifo)
{
    void* temp;
    
    SYS_ENTER_SCRT();
    
    if(lifo->sp <= lifo->bottom)        //确认栈里面还有数据存在
    {
        SYS_EXIT_SCRT();
        return __NULL;
    }
                                        //弹出一个数据
    temp = (uint8*)lifo->sp - (uint16)lifo->type;
    SYS_EXIT_SCRT();
    return temp;
    
}


/************************************************************************
 * @function: SYS_LIFO_Clear
 * @描述: 清除LIFO
 * 
 * @参数: 
 * @param: lifo 需要清除数据的fifo
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_LIFO_Clear(LIFO* lifo)
{
    SYS_ENTER_SCRT();
    lifo->sp = lifo->bottom;
    SYS_EXIT_SCRT();
}
#endif

#if SYS_ARRAY_EN > 0


/************************************************************************
 * @function: SYS_Array_Create
 * @描述: 创建一个Array
 * 
 * @参数: 
 * @param: type 表示队列类型的长度
 * @param: array 需要被初始化的ARRAY
 * @param: buffer 队列所需要使用的缓存
 * @param: length 表示上面的缓存的大?.ǖノ蛔纸?)
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_Array_Create(size_array type, ARRAY* array, void* buffer, uint16 length)
{
    uint16 i;

    SYS_VAR_CHECK(type + 4 > length);
    SYS_VAR_CHECK(((uint32)buffer) & 0x03);
    
    SYS_ENTER_SCRT();
                                        //计算这个队列的容量
    array->capcity = Divu16(length , (uint16)type + 4).word[0];
    array->count = 0;                   //当前有效的数量肯定是0
    
    array->link = buffer;               //获取链表的起始位置
    
    array->data = (uint8*)buffer + array->capcity * 4;
    
    for(i = 0; i < array->capcity; i++) //循环并设置数据
    {
        array->link[i].word[0] = i - 1; //指向前一块
        array->link[i].word[1] = i + 1; //指向后一块
    }
    
                                        //实现双向环线链表
    array->link[0].word[0] = i;         //第一块的上一块指向最后一块
    array->link[i].word[1] = 0;         //最后一块的下一块指向第一块
    
    array->empty = 0;                   //空闲的指向第一块
    array->used = 0xFFFF;               //使用的指向空
    
    SYS_EXIT_SCRT();
    
    SYS_OK();    
}



/************************************************************************
 * @function: SYS_Array_Add
 * @描述: 往队列里面增加一个数据
 * 
 * @参数: 
 * @param: array 需要增加数据的的ARRAY
 * @param: content 增加的内容
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_Array_Add(ARRAY* array, void* content)
{
    void* temp;
    uint16 link;
    uint16 newlink;
    
    SYS_ENTER_SCRT();
    if(array->count >= array->capcity)  //确保队列没有溢出
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_FUL;
    }
                                        //获取需要保存的数据的位置
    temp = array->empty * (uint16)array->type + (uint8*)array->data;
    
    array->count++;                     //队列有效数据增加
                                        //转移数据
    memcpy(temp, content, (uint16)array->type);
    
    newlink = array->empty;             //新使用的
    
    
    if(array->count < array->capcity)   //如果还有空闲块的话
    {
                                        //当前空闲块指向下一个空闲块
        array->empty = array->link[newlink].word[1];
                                        //获取环形双向链表的最后一块
        link = array->link[newlink].word[0];
                                        //最后一块的下一块指向当前空闲块
        array->link[link].word[1] = array->empty;
                                        //当前空闲块的上一块指向最后一块
        array->link[array->empty].word[0] = link;                               
    }
    else                                
    {
        array->empty = 0xFFFF;          //已经使用完所有的空闲块
    }
    
    
    if(array->count > 1)                //如果不是第一数据
    {   
                                        //指向最后一块
        link = array->link[array->used].word[0];
                                        //刚才的最后一块的下一块指向刚刚被分配的那一块
        array->link[link].word[1] = newlink;
                                        //刚分配的上一块指向刚刚的最后一块
        array->link[newlink].word[0] = link;
                                        //被分配的那一块的下一块指向第一块
        array->link[newlink].word[1] = array->used;
                                        //上一个使用的块指向刚被分配的那一块
        array->link[array->used].word[0] = newlink;
        
    }
    else                                //创建第一个数据块
    {
        array->used = newlink;
        
        array->link[array->used].word[0] = newlink;
        
        array->link[array->used].word[1] = newlink;
            
    }    
    SYS_EXIT_SCRT();
    SYS_OK();    
}


/************************************************************************
 * @function: SYS_Array_Remove
 * @描述: 
 * 
 * @参数: 
 * @param: array 要删除数据的ARRAY
 * @param: index 要删除数据的索引
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_Array_Remove(ARRAY* array, uint16 index)
{
    uint16 i;
    uint16 link;
    uint16 newlink;
    
    SYS_ENTER_SCRT();
    if(array->count <= index)           //删除的数据需要存在
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_EPY;
    }
    
    link = array->used;                 //指向第一个
    
    for(i = 0; i < index; i++)          //循环获取索引
    {
        link = array->link[link].word[1];
    }
    
    newlink = link;
    
    array->count--;                     //队列有效数据的数量减少
    
    if(array->count > 0)                //表示还有数据存在
    {
                                        //表示被删除项的上一项
        link = array->link[newlink].word[0];
                                        //表示上一项的下一项指针指向...
        array->link[link].word[1] = array->link[newlink].word[1];
                                        //表示被删除项的下一项
        link = array->link[newlink].word[1];
                                        //表示下一项的上一项指针指向
        array->link[link].word[0] = array->link[newlink].word[0];
        
        if(index == 0)                  //判断是否删除了第一项
        {
            array->used = link;         //第一项指向下一项
        }
        
    }
    else
    {
        array->used = 0xFFFF;           //表示没有任何数据存在了
    }
    
                                        //是否是第一个空闲项
    if(array->count < array->capcity - 1)
    {
        link = array->empty;            //获取当前的空闲项
                                        //删除项的上一项为当前空闲项的上一项
        array->link[newlink].word[0] = array->link[link].word[0];
                                        //删除项的下一项为当前空闲项
        array->link[newlink].word[1] = link;
                                        //当前空闲项的上一项为删除项
        array->link[link].word[0] = newlink;
                                            
        link = array->link[newlink].word[0];
                                        //最后项的下一项为删除项
        array->link[link].word[1] = newlink;
        
        array->empty = newlink;         //设置当前项为删除项
    }
    else                                //第一个空闲项
    {
        array->empty = newlink;
        array->link[newlink].word[0] = newlink;
        array->link[newlink].word[1] = newlink;
    }           
    
    SYS_EXIT_SCRT();
    SYS_OK();
}


/************************************************************************
 * @function: SYS_Array_Get
 * @描述: 从Array里面获取一个数据
 * 
 * @参数: 
 * @param: array 需要获取的数据所在的array
 * @param: index 需要获取的数据的索引
 * 
 * @返回: 
 * @return: void*  __NULL 获取数据失败   其它,获取的所在的地址
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void* SYS_Array_Get(ARRAY* array, uint16 index)
{
    void* temp;
    uint16 i;
    uint16 link;
    
    
    SYS_ENTER_SCRT();
    if(array->count <= index)           //删除的数据需要存在
    {
        SYS_EXIT_SCRT();
        return __NULL;
    }
    
    link = array->used;                 //指向第一个
    
    for(i = 0; i < index; i++)          //循环获取索引
    {
        link = array->link[link].word[1];
    }
                                        //指向需要获取数据的位置
    temp = link * (uint16)array->type + (uint8*)array->data;
    
    SYS_EXIT_SCRT();
    
    return temp;                        //返回结果
}


/************************************************************************
 * @function: SYS_Array_Set
 * @描述: 将Array里面的一个数据的内容更改
 * 
 * @参数: 
 * @param: array 需要更改的数据所在的array
 * @param: index 需要更改的数据的索引
 * @param: content 需要设置的内容
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 SYS_Array_Set(ARRAY* array, uint16 index, void* content)
{
    void* temp;
    uint16 i;
    uint16 link;
    
    
    SYS_ENTER_SCRT();
    if(array->count <= index)           //删除的数据需要存在
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_EPY;
    }
    
    link = array->used;                 //指向第一个
    
    for(i = 0; i < index; i++)          //循环获取索引
    {
        link = array->link[link].word[1];
    }
                                        //指向需要获取数据的位置
    temp = link * (uint16)array->type + (uint8*)array->data;
                                        //设置数据
    memcpy(temp, content,(uint16)array->type);
    
    SYS_EXIT_SCRT();
    
    SYS_OK();    
}


/************************************************************************
 * @function: SYS_Array_Clear
 * @描述: 清除ARRAY
 * 
 * @参数: 
 * @param: array 需要清除数据的array
 * @返回: 
 * @说明: 
 * @作者: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SYS_Array_Clear(ARRAY* array)
{
    SYS_Array_Create(                    //重新创建一个array
        array->type, 
        array,     
        (void*)array->link, 
        array->capcity * ((uint16)array->type + 4));
}


#endif



