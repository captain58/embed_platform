/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: Array.c
**��   ��   ��: yzy
**����޸�����: 2007��1��15��
**��        ��: ���к�����
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2007��1��15��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_ARRAY

#include "bdef.h"
#include "cdef.h"

#include "libs.h"
#include <k_api.h>


#if SYS_FIFO_EN > 0


/************************************************************************
 * @function: SYS_FIFO_Create
 * @����: ����һ��FIFO
 * 
 * @����: 
 * @param: type ������FIFO�����ݵ�Ԫ������
 * @param: fifo ��Ҫ������fifo
 * @param: buffer ��Ҫ������FIFO�����ݵĻ���λ��
 * @param: length ��Ҫ������FIFO�����ݵ�Ԫ�ĳ���(�ֽ���)
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_FIFO_Create(size_array type, FIFO* fifo, void* buffer, uint16 length)
{
    SYS_VAR_CHECK(type > length);
    //SYS_VAR_CHECK(((uint32)buffer) & 0x03);
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();

    fifo->type = type;                  //��ȡ��������
    fifo->count = 0;                    //��������Ϊ0
                                        //�����ȡ���е�����
    fifo->capcity = length / type;
    fifo->buffers = buffer;             //������ʼ��λ��
                                        //���������λ��
    fifo->buffere = (uint8*)buffer + (uint16)type * fifo->capcity;
    fifo->head = buffer;                //ͷ
    fifo->tail = buffer;                //β
    
    RHINO_CPU_INTRPT_ENABLE();
    SYS_OK();                            //�����ɹ�,����ok
}



/************************************************************************
 * @function: SYS_FIFO_Enqueue
 * @����: ��һ�������ŵ���������ȥ
 * 
 * @����: 
 * @param: fifo ��Ҫ��ӵ�fifo
 * @param: content ��ӵ�����
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_FIFO_Enqueue(FIFO* fifo, void* content)
{
    void* temp;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    if(fifo->count >= fifo->capcity)    //�жϵ�ǰ�����Ƿ��Ѿ�����  
    {
        RHINO_CPU_INTRPT_ENABLE();
        return SYS_ARRAY_ERR_FUL;        //���ض����Ѿ�����
    }
    
    temp = fifo->tail;                  //��ȡ��ǰ�Ķ�β
    
                                        //��β��ǰ�ƶ�
    fifo->tail = (uint8*)fifo->tail + (uint16)fifo->type;
    
    if(fifo->tail >= fifo->buffere)     //���ƴ���
    {
        fifo->tail = fifo->buffers;
    }
    
    fifo->count++;                      //��������Ч�����������
                                        //�����ݿ�������β
    memcpy(temp, content, (uint16)fifo->type);
    RHINO_CPU_INTRPT_ENABLE();
    SYS_OK();                            //������ӳɹ�  
}



/************************************************************************
 * @function: SYS_FIFO_Dequeue
 * @����: ��FIFO�����ȡһ������,������ݲ��ٴ���FIFO����
 * 
 * @����: 
 * @param: fifo ��Ҫ���ӵ�fifo, ע��������������ǽ��̰�ȫ��,���ǻ�ȡ��������һ����.
 * 
 * @����: 
 * @return: void*  __NULL��ʾ���п�,����:�������ݵ����ݵĵ�ַ
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void* SYS_FIFO_Dequeue(FIFO* fifo)
{
    void* temp;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    if(fifo->count == 0)                //�ж��Ƿ����Ϊ��
    {
        RHINO_CPU_INTRPT_ENABLE();
        return __NULL;
    }
    fifo->count--;
    
    temp = fifo->head;                  //��ȡ��ǰ�Ķ���ͷ
    fifo->head = (uint8*)fifo->head + (uint16)fifo->type;
    
    if(fifo->head >= fifo->buffere)     //���ƴ���
    {
        fifo->head = fifo->buffers;
    }
 	
    RHINO_CPU_INTRPT_ENABLE();
    return temp;
}

/************************************************************************
 * @function: SYS_FIFO_Get
 * @����: ��FIFO�����ȡһ������,������ݲ��ٴ���FIFO����
 * 
 * @����: 
 * @param: fifo ��Ҫ���ӵ�fifo
 * @param: content ��ȡ�����ݱ����λ��,��������ǽ��̰�ȫ��.
 * 
 * @����: 
 * @return: uint8  �ɹ�/ʧ��
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_FIFO_Get(FIFO* fifo, void* content)
{
    void* temp;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    if(fifo->count == 0)                //�ж��Ƿ����Ϊ��
    {
        RHINO_CPU_INTRPT_ENABLE();
        return SYS_ARRAY_ERR_EPY;        //�����ǿյ�
    }
    fifo->count--;
    
    temp = fifo->head;                  //��ȡ��ǰ�Ķ���ͷ
    fifo->head = (uint8*)fifo->head + (uint16)fifo->type;
    
    if(fifo->head >= fifo->buffere)     //���ƴ���
    {
        fifo->head = fifo->buffers;
    }
                                        //��ȡ����  
    memcpy(content, temp, (uint16)fifo->type);
    RHINO_CPU_INTRPT_ENABLE();
    SYS_OK();
}




/************************************************************************
 * @function: SYS_FIFO_Peek
 * @����: ��FIFO�����ȡһ������,������ݻ���FIFO����ע��������������ǽ��̰�ȫ��,���ǻ�ȡ��������һ����.
 * 
 * @����: 
 * @param: fifo ��Ҫ��ȡ���ݵ�fifo
 * 
 * @����: 
 * @return: void*  __NULL��ʾ���п�   ����:��ȡ���ݵ����ݵĵ�ַ
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void* SYS_FIFO_Peek(FIFO* fifo)
{
    void* temp;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    if(fifo->count == 0)                //�ж��Ƿ����Ϊ��
    {
        RHINO_CPU_INTRPT_ENABLE();
        return __NULL;
    }
    temp = fifo->head;                  //��ȡ��ǰ�Ķ���ͷ
    RHINO_CPU_INTRPT_ENABLE();
    return temp;
}


/************************************************************************
 * @function: SYS_FIFO_Clear
 * @����: ���FIFO
 * 
 * @����: 
 * @param: fifo ��Ҫ������ݵ�fifo
 * @����: 
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
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
 * @����: ����һ��LIFO
 * 
 * @����: 
 * @param: type ������LIFO�����ݵ�Ԫ������
 * @param: lifo ��Ҫ������lifo
 * @param: buffer ��Ҫ������LIFO�����ݵĻ���λ��
 * @param: length ��Ҫ������LIFO�����ݵĳ���(�ֽ���)
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_LIFO_Create(size_array type, LIFO* lifo, void* buffer, uint16 length)
{
    SYS_VAR_CHECK(type > length);
    SYS_VAR_CHECK(((uint32)buffer) & 0x03);
    SYS_ENTER_SCRT();

    lifo->type = type;                  //��ȡ��������

                                        //�����ȡ���е�����
    lifo->bottom = buffer;              //������ʼ��λ��
                                        //����λ��
    lifo->top = (uint8*)buffer + (length / type) * (uint16)type;
    
    lifo->sp = lifo->bottom;            //��ǰָ��                                              
    SYS_EXIT_SCRT();
    SYS_OK();                            //���ش����ɹ�
}



/************************************************************************
 * @function: SYS_LIFO_Push
 * @����: ��һ����������LIFO��
 * 
 * @����: 
 * @param: lifo ��Ҫ��ӵ�lifo
 * @param: content ��ӵ�����
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_LIFO_Push(LIFO* lifo, void* content)
{
    void* temp;
    
    SYS_ENTER_SCRT();
    if(lifo->sp >= lifo->top)           //�ж϶�ջ�Ƿ��Ѿ�����
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_FUL;            //������ջ����
    }
    
    temp = lifo->sp;                    //��ȡ��ǰ��λ��
                                        //λ��ǰ��  
    lifo->sp = (uint8*)lifo->sp + (uint16)lifo->type;
    
                                        //ת������
    memcpy(temp, content, (uint16)lifo->type);
    
    SYS_EXIT_SCRT();
    SYS_OK();                            //������ջ�ɹ�       
}




/************************************************************************
 * @function: SYS_LIFO_Pop
 * @����: ��LIFO�����ȡһ������,������ݲ��ٴ���LIFO����
 * 
 * @����: 
 * @param: lifo ��Ҫ���ӵ�lifo
 * 
 * @����: 
 * @return: void*  __NULL��ʾ���п� ����:�������ݵ����ݵĵ�ַ
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void* SYS_LIFO_Pop(LIFO* lifo)
{
    void* temp;
    
    SYS_ENTER_SCRT();
    
    if(lifo->sp <= lifo->bottom)        //ȷ��ջ���滹�����ݴ���
    {
        SYS_EXIT_SCRT();
        return __NULL;
    }
                                        //����һ������
    lifo->sp = (uint8*)lifo->sp - (uint16)lifo->type;
    temp = lifo->sp;
    SYS_EXIT_SCRT();
    return temp;
}



/************************************************************************
 * @function: SYS_LIFO_Get
 * @����: ��LIFO�����ȡһ������,������ݲ��ٴ���LIFO����
 * 
 * @����: 
 * @param: lifo ��Ҫ���ӵ�lifo
 * @param: content ���浯������������
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_LIFO_Get(LIFO* lifo, void* content)
{
    void* temp;
    
    SYS_ENTER_SCRT();
    
    if(lifo->sp <= lifo->bottom)        //ȷ��ջ���滹�����ݴ���
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_EPY;
    }
                                        //����һ������
    lifo->sp = (uint8*)lifo->sp - (uint16)lifo->type;
    temp = lifo->sp;
    
    memcpy(content, temp, (uint16)lifo->type);
    SYS_EXIT_SCRT();
    SYS_OK();
}





/************************************************************************
 * @function: SYS_LIFO_Peek
 * @����: ��LIFO�����ȡһ������,������ݻ���LIFO����
 * 
 * @����: 
 * @param: lifo ��Ҫ��ȡ���ݵ�lifo
 * 
 * @����: 
 * @return: void*  __NULL��ʾ���п� ����:�������ݵ����ݵĵ�ַ
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void* SYS_LIFO_Peek(LIFO* lifo)
{
    void* temp;
    
    SYS_ENTER_SCRT();
    
    if(lifo->sp <= lifo->bottom)        //ȷ��ջ���滹�����ݴ���
    {
        SYS_EXIT_SCRT();
        return __NULL;
    }
                                        //����һ������
    temp = (uint8*)lifo->sp - (uint16)lifo->type;
    SYS_EXIT_SCRT();
    return temp;
    
}


/************************************************************************
 * @function: SYS_LIFO_Clear
 * @����: ���LIFO
 * 
 * @����: 
 * @param: lifo ��Ҫ������ݵ�fifo
 * @����: 
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
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
 * @����: ����һ��Array
 * 
 * @����: 
 * @param: type ��ʾ�������͵ĳ���
 * @param: array ��Ҫ����ʼ����ARRAY
 * @param: buffer ��������Ҫʹ�õĻ���
 * @param: length ��ʾ����Ļ���Ĵ��.���λ�ֽ�)
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_Array_Create(size_array type, ARRAY* array, void* buffer, uint16 length)
{
    uint16 i;

    SYS_VAR_CHECK(type + 4 > length);
    SYS_VAR_CHECK(((uint32)buffer) & 0x03);
    
    SYS_ENTER_SCRT();
                                        //����������е�����
    array->capcity = Divu16(length , (uint16)type + 4).word[0];
    array->count = 0;                   //��ǰ��Ч�������϶���0
    
    array->link = buffer;               //��ȡ�������ʼλ��
    
    array->data = (uint8*)buffer + array->capcity * 4;
    
    for(i = 0; i < array->capcity; i++) //ѭ������������
    {
        array->link[i].word[0] = i - 1; //ָ��ǰһ��
        array->link[i].word[1] = i + 1; //ָ���һ��
    }
    
                                        //ʵ��˫��������
    array->link[0].word[0] = i;         //��һ�����һ��ָ�����һ��
    array->link[i].word[1] = 0;         //���һ�����һ��ָ���һ��
    
    array->empty = 0;                   //���е�ָ���һ��
    array->used = 0xFFFF;               //ʹ�õ�ָ���
    
    SYS_EXIT_SCRT();
    
    SYS_OK();    
}



/************************************************************************
 * @function: SYS_Array_Add
 * @����: ��������������һ������
 * 
 * @����: 
 * @param: array ��Ҫ�������ݵĵ�ARRAY
 * @param: content ���ӵ�����
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_Array_Add(ARRAY* array, void* content)
{
    void* temp;
    uint16 link;
    uint16 newlink;
    
    SYS_ENTER_SCRT();
    if(array->count >= array->capcity)  //ȷ������û�����
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_FUL;
    }
                                        //��ȡ��Ҫ��������ݵ�λ��
    temp = array->empty * (uint16)array->type + (uint8*)array->data;
    
    array->count++;                     //������Ч��������
                                        //ת������
    memcpy(temp, content, (uint16)array->type);
    
    newlink = array->empty;             //��ʹ�õ�
    
    
    if(array->count < array->capcity)   //������п��п�Ļ�
    {
                                        //��ǰ���п�ָ����һ�����п�
        array->empty = array->link[newlink].word[1];
                                        //��ȡ����˫����������һ��
        link = array->link[newlink].word[0];
                                        //���һ�����һ��ָ��ǰ���п�
        array->link[link].word[1] = array->empty;
                                        //��ǰ���п����һ��ָ�����һ��
        array->link[array->empty].word[0] = link;                               
    }
    else                                
    {
        array->empty = 0xFFFF;          //�Ѿ�ʹ�������еĿ��п�
    }
    
    
    if(array->count > 1)                //������ǵ�һ����
    {   
                                        //ָ�����һ��
        link = array->link[array->used].word[0];
                                        //�ղŵ����һ�����һ��ָ��ոձ��������һ��
        array->link[link].word[1] = newlink;
                                        //�շ������һ��ָ��ոյ����һ��
        array->link[newlink].word[0] = link;
                                        //���������һ�����һ��ָ���һ��
        array->link[newlink].word[1] = array->used;
                                        //��һ��ʹ�õĿ�ָ��ձ��������һ��
        array->link[array->used].word[0] = newlink;
        
    }
    else                                //������һ�����ݿ�
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
 * @����: 
 * 
 * @����: 
 * @param: array Ҫɾ�����ݵ�ARRAY
 * @param: index Ҫɾ�����ݵ�����
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_Array_Remove(ARRAY* array, uint16 index)
{
    uint16 i;
    uint16 link;
    uint16 newlink;
    
    SYS_ENTER_SCRT();
    if(array->count <= index)           //ɾ����������Ҫ����
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_EPY;
    }
    
    link = array->used;                 //ָ���һ��
    
    for(i = 0; i < index; i++)          //ѭ����ȡ����
    {
        link = array->link[link].word[1];
    }
    
    newlink = link;
    
    array->count--;                     //������Ч���ݵ���������
    
    if(array->count > 0)                //��ʾ�������ݴ���
    {
                                        //��ʾ��ɾ�������һ��
        link = array->link[newlink].word[0];
                                        //��ʾ��һ�����һ��ָ��ָ��...
        array->link[link].word[1] = array->link[newlink].word[1];
                                        //��ʾ��ɾ�������һ��
        link = array->link[newlink].word[1];
                                        //��ʾ��һ�����һ��ָ��ָ��
        array->link[link].word[0] = array->link[newlink].word[0];
        
        if(index == 0)                  //�ж��Ƿ�ɾ���˵�һ��
        {
            array->used = link;         //��һ��ָ����һ��
        }
        
    }
    else
    {
        array->used = 0xFFFF;           //��ʾû���κ����ݴ�����
    }
    
                                        //�Ƿ��ǵ�һ��������
    if(array->count < array->capcity - 1)
    {
        link = array->empty;            //��ȡ��ǰ�Ŀ�����
                                        //ɾ�������һ��Ϊ��ǰ���������һ��
        array->link[newlink].word[0] = array->link[link].word[0];
                                        //ɾ�������һ��Ϊ��ǰ������
        array->link[newlink].word[1] = link;
                                        //��ǰ���������һ��Ϊɾ����
        array->link[link].word[0] = newlink;
                                            
        link = array->link[newlink].word[0];
                                        //��������һ��Ϊɾ����
        array->link[link].word[1] = newlink;
        
        array->empty = newlink;         //���õ�ǰ��Ϊɾ����
    }
    else                                //��һ��������
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
 * @����: ��Array�����ȡһ������
 * 
 * @����: 
 * @param: array ��Ҫ��ȡ���������ڵ�array
 * @param: index ��Ҫ��ȡ�����ݵ�����
 * 
 * @����: 
 * @return: void*  __NULL ��ȡ����ʧ��   ����,��ȡ�����ڵĵ�ַ
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void* SYS_Array_Get(ARRAY* array, uint16 index)
{
    void* temp;
    uint16 i;
    uint16 link;
    
    
    SYS_ENTER_SCRT();
    if(array->count <= index)           //ɾ����������Ҫ����
    {
        SYS_EXIT_SCRT();
        return __NULL;
    }
    
    link = array->used;                 //ָ���һ��
    
    for(i = 0; i < index; i++)          //ѭ����ȡ����
    {
        link = array->link[link].word[1];
    }
                                        //ָ����Ҫ��ȡ���ݵ�λ��
    temp = link * (uint16)array->type + (uint8*)array->data;
    
    SYS_EXIT_SCRT();
    
    return temp;                        //���ؽ��
}


/************************************************************************
 * @function: SYS_Array_Set
 * @����: ��Array�����һ�����ݵ����ݸ���
 * 
 * @����: 
 * @param: array ��Ҫ���ĵ��������ڵ�array
 * @param: index ��Ҫ���ĵ����ݵ�����
 * @param: content ��Ҫ���õ�����
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_Array_Set(ARRAY* array, uint16 index, void* content)
{
    void* temp;
    uint16 i;
    uint16 link;
    
    
    SYS_ENTER_SCRT();
    if(array->count <= index)           //ɾ����������Ҫ����
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_EPY;
    }
    
    link = array->used;                 //ָ���һ��
    
    for(i = 0; i < index; i++)          //ѭ����ȡ����
    {
        link = array->link[link].word[1];
    }
                                        //ָ����Ҫ��ȡ���ݵ�λ��
    temp = link * (uint16)array->type + (uint8*)array->data;
                                        //��������
    memcpy(temp, content,(uint16)array->type);
    
    SYS_EXIT_SCRT();
    
    SYS_OK();    
}


/************************************************************************
 * @function: SYS_Array_Clear
 * @����: ���ARRAY
 * 
 * @����: 
 * @param: array ��Ҫ������ݵ�array
 * @����: 
 * @˵��: 
 * @����: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_Array_Clear(ARRAY* array)
{
    SYS_Array_Create(                    //���´���һ��array
        array->type, 
        array,     
        (void*)array->link, 
        array->capcity * ((uint16)array->type + 4));
}


#endif



