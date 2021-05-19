/****************************************Copyright (c)**************************************************
**                                  ________¿Æ¼¼ÓĞÏŞ¹«Ë¾
**                                          ¿ª·¢²¿
**
**
**--------------ÎÄ¼şĞÅÏ¢--------------------------------------------------------------------------------
**ÎÄ   ¼ş   Ãû: Array.c
**´´   ½¨   ÈË: yzy
**×îºóĞŞ¸ÄÈÕÆÚ: 2007Äê1ÔÂ15ÈÕ
**Ãè        Êö: ¶ÓÁĞº¯Êı¼¯
**×¢        Òâ:
**--------------ÀúÊ·°æ±¾ĞÅÏ¢----------------------------------------------------------------------------
** ´´½¨ÈË: yzy
** °æ  ±¾: v1.0
** ÈÕ¡¡ÆÚ: 2007Äê1ÔÂ15ÈÕ
** Ãè¡¡Êö: Ô­Ê¼°æ±¾
********************************************************************************************************/
#define EXT_ARRAY

#include "bdef.h"
#include "cdef.h"

#include "libs.h"
#include <k_api.h>


#if SYS_FIFO_EN > 0


/************************************************************************
 * @function: SYS_FIFO_Create
 * @ÃèÊö: ´´½¨Ò»¸öFIFO
 * 
 * @²ÎÊı: 
 * @param: type ´´½¨µÄFIFOµÄÊı¾İµ¥ÔªµÄÀàĞÍ
 * @param: fifo ĞèÒª´´½¨µÄfifo
 * @param: buffer ĞèÒª´´½¨µÄFIFOµÄÊı¾İµÄ»º´æÎ»ÖÃ
 * @param: length ĞèÒª´´½¨µÄFIFOµÄÊı¾İµ¥ÔªµÄ³¤¶È(×Ö½ÚÊı)
 * 
 * @·µ»Ø: 
 * @return: uint8  
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
uint8 SYS_FIFO_Create(size_array type, FIFO* fifo, void* buffer, uint16 length)
{
    SYS_VAR_CHECK(type > length);
    //SYS_VAR_CHECK(((uint32)buffer) & 0x03);
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();

    fifo->type = type;                  //»ñÈ¡Êı¾İÀàĞÍ
    fifo->count = 0;                    //¶ÓÁĞÊı¾İÎª0
                                        //¼ÆËã»ñÈ¡¶ÓÁĞµÄÈİÁ¿
    fifo->capcity = length / type;
    fifo->buffers = buffer;             //»º´æÆğÊ¼µÄÎ»ÖÃ
                                        //»º´æ½áÊøµÄÎ»ÖÃ
    fifo->buffere = (uint8*)buffer + (uint16)type * fifo->capcity;
    fifo->head = buffer;                //Í·
    fifo->tail = buffer;                //Î²
    
    RHINO_CPU_INTRPT_ENABLE();
    SYS_OK();                            //´´½¨³É¹¦,·µ»Øok
}



/************************************************************************
 * @function: SYS_FIFO_Enqueue
 * @ÃèÊö: ½«Ò»¸öÊı¾İÅÅµ½¶ÓÁĞÀïÃæÈ¥
 * 
 * @²ÎÊı: 
 * @param: fifo ĞèÒªÈë¶ÓµÄfifo
 * @param: content Èë¶ÓµÄÄÚÈİ
 * 
 * @·µ»Ø: 
 * @return: uint8  
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
uint8 SYS_FIFO_Enqueue(FIFO* fifo, void* content)
{
    void* temp;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    if(fifo->count >= fifo->capcity)    //ÅĞ¶Ïµ±Ç°¶ÓÁĞÊÇ·ñÒÑ¾­ÂúÁË  
    {
        RHINO_CPU_INTRPT_ENABLE();
        return SYS_ARRAY_ERR_FUL;        //·µ»Ø¶ÓÁĞÒÑ¾­ÂúÁË
    }
    
    temp = fifo->tail;                  //»ñÈ¡µ±Ç°µÄ¶ÓÎ²
    
                                        //¶ÓÎ²ÍùÇ°ÒÆ¶¯
    fifo->tail = (uint8*)fifo->tail + (uint16)fifo->type;
    
    if(fifo->tail >= fifo->buffere)     //¾íÈÆ´¦Àí
    {
        fifo->tail = fifo->buffers;
    }
    
    fifo->count++;                      //¶ÓÁĞÖĞÓĞĞ§ÏîµÄÊıÁ¿Ôö¼Ó
                                        //½«Êı¾İ¿½±´µ½¶ÓÎ²
    memcpy(temp, content, (uint16)fifo->type);
    RHINO_CPU_INTRPT_ENABLE();
    SYS_OK();                            //·µ»ØÈë¶Ó³É¹¦  
}



/************************************************************************
 * @function: SYS_FIFO_Dequeue
 * @ÃèÊö: ´ÓFIFOÀïÃæ»ñÈ¡Ò»¸öÊı¾İ,Õâ¸öÊı¾İ²»ÔÙ´æÔÚFIFOÀïÃæ
 * 
 * @²ÎÊı: 
 * @param: fifo ĞèÒª³ö¶ÓµÄfifo, ×¢ÒâÕâ¸öº¯Êı±¾ÉíÊÇ½ø³Ì°²È«µÄ,µ«ÊÇ»ñÈ¡µÄÄÚÈİÔò²»Ò»¶¨ÁË.
 * 
 * @·µ»Ø: 
 * @return: void*  __NULL±íÊ¾¶ÓÁĞ¿Õ,ÆäËü:³ö¶ÓÊı¾İµÄÄÚÈİµÄµØÖ·
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
void* SYS_FIFO_Dequeue(FIFO* fifo)
{
    void* temp;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    if(fifo->count == 0)                //ÅĞ¶ÏÊÇ·ñ¶ÓÁĞÎª¿Õ
    {
        RHINO_CPU_INTRPT_ENABLE();
        return __NULL;
    }
    fifo->count--;
    
    temp = fifo->head;                  //»ñÈ¡µ±Ç°µÄ¶ÓÁĞÍ·
    fifo->head = (uint8*)fifo->head + (uint16)fifo->type;
    
    if(fifo->head >= fifo->buffere)     //¾íÈÆ´¦Àí
    {
        fifo->head = fifo->buffers;
    }
 	
    RHINO_CPU_INTRPT_ENABLE();
    return temp;
}

/************************************************************************
 * @function: SYS_FIFO_Get
 * @ÃèÊö: ´ÓFIFOÀïÃæ»ñÈ¡Ò»¸öÊı¾İ,Õâ¸öÊı¾İ²»ÔÙ´æÔÚFIFOÀïÃæ
 * 
 * @²ÎÊı: 
 * @param: fifo ĞèÒª³ö¶ÓµÄfifo
 * @param: content »ñÈ¡µÄÊı¾İ±£´æµÄÎ»ÖÃ,Õâ¸öº¯ÊıÊÇ½ø³Ì°²È«µÄ.
 * 
 * @·µ»Ø: 
 * @return: uint8  ³É¹¦/Ê§°Ü
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
uint8 SYS_FIFO_Get(FIFO* fifo, void* content)
{
    void* temp;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    if(fifo->count == 0)                //ÅĞ¶ÏÊÇ·ñ¶ÓÁĞÎª¿Õ
    {
        RHINO_CPU_INTRPT_ENABLE();
        return SYS_ARRAY_ERR_EPY;        //»º´æÊÇ¿ÕµÄ
    }
    fifo->count--;
    
    temp = fifo->head;                  //»ñÈ¡µ±Ç°µÄ¶ÓÁĞÍ·
    fifo->head = (uint8*)fifo->head + (uint16)fifo->type;
    
    if(fifo->head >= fifo->buffere)     //¾íÈÆ´¦Àí
    {
        fifo->head = fifo->buffers;
    }
                                        //»ñÈ¡Êı¾İ  
    memcpy(content, temp, (uint16)fifo->type);
    RHINO_CPU_INTRPT_ENABLE();
    SYS_OK();
}




/************************************************************************
 * @function: SYS_FIFO_Peek
 * @ÃèÊö: ´ÓFIFOÀïÃæ»ñÈ¡Ò»¸öÊı¾İ,Õâ¸öÊı¾İ»¹ÔÚFIFOÀïÃæ×¢ÒâÕâ¸öº¯Êı±¾ÉíÊÇ½ø³Ì°²È«µÄ,µ«ÊÇ»ñÈ¡µÄÄÚÈİÔò²»Ò»¶¨ÁË.
 * 
 * @²ÎÊı: 
 * @param: fifo ĞèÒª»ñÈ¡Êı¾İµÄfifo
 * 
 * @·µ»Ø: 
 * @return: void*  __NULL±íÊ¾¶ÓÁĞ¿Õ   ÆäËü:»ñÈ¡Êı¾İµÄÄÚÈİµÄµØÖ·
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
void* SYS_FIFO_Peek(FIFO* fifo)
{
    void* temp;
    
    CPSR_ALLOC();
    RHINO_CPU_INTRPT_DISABLE();
    if(fifo->count == 0)                //ÅĞ¶ÏÊÇ·ñ¶ÓÁĞÎª¿Õ
    {
        RHINO_CPU_INTRPT_ENABLE();
        return __NULL;
    }
    temp = fifo->head;                  //»ñÈ¡µ±Ç°µÄ¶ÓÁĞÍ·
    RHINO_CPU_INTRPT_ENABLE();
    return temp;
}


/************************************************************************
 * @function: SYS_FIFO_Clear
 * @ÃèÊö: Çå³ıFIFO
 * 
 * @²ÎÊı: 
 * @param: fifo ĞèÒªÇå³ıÊı¾İµÄfifo
 * @·µ»Ø: 
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
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
 * @ÃèÊö: ´´½¨Ò»¸öLIFO
 * 
 * @²ÎÊı: 
 * @param: type ´´½¨µÄLIFOµÄÊı¾İµ¥ÔªµÄÀàĞÍ
 * @param: lifo ĞèÒª´´½¨µÄlifo
 * @param: buffer ĞèÒª´´½¨µÄLIFOµÄÊı¾İµÄ»º´æÎ»ÖÃ
 * @param: length ĞèÒª´´½¨µÄLIFOµÄÊı¾İµÄ³¤¶È(×Ö½ÚÊı)
 * 
 * @·µ»Ø: 
 * @return: uint8  
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
uint8 SYS_LIFO_Create(size_array type, LIFO* lifo, void* buffer, uint16 length)
{
    SYS_VAR_CHECK(type > length);
    SYS_VAR_CHECK(((uint32)buffer) & 0x03);
    SYS_ENTER_SCRT();

    lifo->type = type;                  //»ñÈ¡Êı¾İÀàĞÍ

                                        //¼ÆËã»ñÈ¡¶ÓÁĞµÄÈİÁ¿
    lifo->bottom = buffer;              //»º´æÆğÊ¼µÄÎ»ÖÃ
                                        //½áÊøÎ»ÖÃ
    lifo->top = (uint8*)buffer + (length / type) * (uint16)type;
    
    lifo->sp = lifo->bottom;            //µ±Ç°Ö¸Õë                                              
    SYS_EXIT_SCRT();
    SYS_OK();                            //·µ»Ø´´½¨³É¹¦
}



/************************************************************************
 * @function: SYS_LIFO_Push
 * @ÃèÊö: ½«Ò»¸öÊı¾İÍÆÈëLIFOÖĞ
 * 
 * @²ÎÊı: 
 * @param: lifo ĞèÒªÈë¶ÓµÄlifo
 * @param: content Èë¶ÓµÄÄÚÈİ
 * 
 * @·µ»Ø: 
 * @return: uint8  
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
uint8 SYS_LIFO_Push(LIFO* lifo, void* content)
{
    void* temp;
    
    SYS_ENTER_SCRT();
    if(lifo->sp >= lifo->top)           //ÅĞ¶Ï¶ÑÕ»ÊÇ·ñÒÑ¾­ÂúÁË
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_FUL;            //·µ»ØÈëÕ»´íÎó
    }
    
    temp = lifo->sp;                    //»ñÈ¡µ±Ç°µÄÎ»ÖÃ
                                        //Î»ÖÃÇ°ÒÆ  
    lifo->sp = (uint8*)lifo->sp + (uint16)lifo->type;
    
                                        //×ªÒÆÊı¾İ
    memcpy(temp, content, (uint16)lifo->type);
    
    SYS_EXIT_SCRT();
    SYS_OK();                            //·µ»ØÈëÕ»³É¹¦       
}




/************************************************************************
 * @function: SYS_LIFO_Pop
 * @ÃèÊö: ´ÓLIFOÀïÃæ»ñÈ¡Ò»¸öÊı¾İ,Õâ¸öÊı¾İ²»ÔÙ´æÔÚLIFOÀïÃæ
 * 
 * @²ÎÊı: 
 * @param: lifo ĞèÒª³ö¶ÓµÄlifo
 * 
 * @·µ»Ø: 
 * @return: void*  __NULL±íÊ¾¶ÓÁĞ¿Õ ÆäËü:³ö¶ÓÊı¾İµÄÄÚÈİµÄµØÖ·
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
void* SYS_LIFO_Pop(LIFO* lifo)
{
    void* temp;
    
    SYS_ENTER_SCRT();
    
    if(lifo->sp <= lifo->bottom)        //È·ÈÏÕ»ÀïÃæ»¹ÓĞÊı¾İ´æÔÚ
    {
        SYS_EXIT_SCRT();
        return __NULL;
    }
                                        //µ¯³öÒ»¸öÊı¾İ
    lifo->sp = (uint8*)lifo->sp - (uint16)lifo->type;
    temp = lifo->sp;
    SYS_EXIT_SCRT();
    return temp;
}



/************************************************************************
 * @function: SYS_LIFO_Get
 * @ÃèÊö: ´ÓLIFOÀïÃæ»ñÈ¡Ò»¸öÊı¾İ,Õâ¸öÊı¾İ²»ÔÙ´æÔÚLIFOÀïÃæ
 * 
 * @²ÎÊı: 
 * @param: lifo ĞèÒª³ö¶ÓµÄlifo
 * @param: content ±£´æµ¯³öµÄÊı¾İÄÚÈİ
 * 
 * @·µ»Ø: 
 * @return: uint8  
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
uint8 SYS_LIFO_Get(LIFO* lifo, void* content)
{
    void* temp;
    
    SYS_ENTER_SCRT();
    
    if(lifo->sp <= lifo->bottom)        //È·ÈÏÕ»ÀïÃæ»¹ÓĞÊı¾İ´æÔÚ
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_EPY;
    }
                                        //µ¯³öÒ»¸öÊı¾İ
    lifo->sp = (uint8*)lifo->sp - (uint16)lifo->type;
    temp = lifo->sp;
    
    memcpy(content, temp, (uint16)lifo->type);
    SYS_EXIT_SCRT();
    SYS_OK();
}





/************************************************************************
 * @function: SYS_LIFO_Peek
 * @ÃèÊö: ´ÓLIFOÀïÃæ»ñÈ¡Ò»¸öÊı¾İ,Õâ¸öÊı¾İ»¹ÔÚLIFOÀïÃæ
 * 
 * @²ÎÊı: 
 * @param: lifo ĞèÒª»ñÈ¡Êı¾İµÄlifo
 * 
 * @·µ»Ø: 
 * @return: void*  __NULL±íÊ¾¶ÓÁĞ¿Õ ÆäËü:³ö¶ÓÊı¾İµÄÄÚÈİµÄµØÖ·
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
void* SYS_LIFO_Peek(LIFO* lifo)
{
    void* temp;
    
    SYS_ENTER_SCRT();
    
    if(lifo->sp <= lifo->bottom)        //È·ÈÏÕ»ÀïÃæ»¹ÓĞÊı¾İ´æÔÚ
    {
        SYS_EXIT_SCRT();
        return __NULL;
    }
                                        //µ¯³öÒ»¸öÊı¾İ
    temp = (uint8*)lifo->sp - (uint16)lifo->type;
    SYS_EXIT_SCRT();
    return temp;
    
}


/************************************************************************
 * @function: SYS_LIFO_Clear
 * @ÃèÊö: Çå³ıLIFO
 * 
 * @²ÎÊı: 
 * @param: lifo ĞèÒªÇå³ıÊı¾İµÄfifo
 * @·µ»Ø: 
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
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
 * @ÃèÊö: ´´½¨Ò»¸öArray
 * 
 * @²ÎÊı: 
 * @param: type ±íÊ¾¶ÓÁĞÀàĞÍµÄ³¤¶È
 * @param: array ĞèÒª±»³õÊ¼»¯µÄARRAY
 * @param: buffer ¶ÓÁĞËùĞèÒªÊ¹ÓÃµÄ»º´æ
 * @param: length ±íÊ¾ÉÏÃæµÄ»º´æµÄ´óĞ.¨µ¥Î»×Ö½Ú)
 * 
 * @·µ»Ø: 
 * @return: uint8  
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
uint8 SYS_Array_Create(size_array type, ARRAY* array, void* buffer, uint16 length)
{
    uint16 i;

    SYS_VAR_CHECK(type + 4 > length);
    SYS_VAR_CHECK(((uint32)buffer) & 0x03);
    
    SYS_ENTER_SCRT();
                                        //¼ÆËãÕâ¸ö¶ÓÁĞµÄÈİÁ¿
    array->capcity = Divu16(length , (uint16)type + 4).word[0];
    array->count = 0;                   //µ±Ç°ÓĞĞ§µÄÊıÁ¿¿Ï¶¨ÊÇ0
    
    array->link = buffer;               //»ñÈ¡Á´±íµÄÆğÊ¼Î»ÖÃ
    
    array->data = (uint8*)buffer + array->capcity * 4;
    
    for(i = 0; i < array->capcity; i++) //Ñ­»·²¢ÉèÖÃÊı¾İ
    {
        array->link[i].word[0] = i - 1; //Ö¸ÏòÇ°Ò»¿é
        array->link[i].word[1] = i + 1; //Ö¸ÏòºóÒ»¿é
    }
    
                                        //ÊµÏÖË«Ïò»·ÏßÁ´±í
    array->link[0].word[0] = i;         //µÚÒ»¿éµÄÉÏÒ»¿éÖ¸Ïò×îºóÒ»¿é
    array->link[i].word[1] = 0;         //×îºóÒ»¿éµÄÏÂÒ»¿éÖ¸ÏòµÚÒ»¿é
    
    array->empty = 0;                   //¿ÕÏĞµÄÖ¸ÏòµÚÒ»¿é
    array->used = 0xFFFF;               //Ê¹ÓÃµÄÖ¸Ïò¿Õ
    
    SYS_EXIT_SCRT();
    
    SYS_OK();    
}



/************************************************************************
 * @function: SYS_Array_Add
 * @ÃèÊö: Íù¶ÓÁĞÀïÃæÔö¼ÓÒ»¸öÊı¾İ
 * 
 * @²ÎÊı: 
 * @param: array ĞèÒªÔö¼ÓÊı¾İµÄµÄARRAY
 * @param: content Ôö¼ÓµÄÄÚÈİ
 * 
 * @·µ»Ø: 
 * @return: uint8  
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
uint8 SYS_Array_Add(ARRAY* array, void* content)
{
    void* temp;
    uint16 link;
    uint16 newlink;
    
    SYS_ENTER_SCRT();
    if(array->count >= array->capcity)  //È·±£¶ÓÁĞÃ»ÓĞÒç³ö
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_FUL;
    }
                                        //»ñÈ¡ĞèÒª±£´æµÄÊı¾İµÄÎ»ÖÃ
    temp = array->empty * (uint16)array->type + (uint8*)array->data;
    
    array->count++;                     //¶ÓÁĞÓĞĞ§Êı¾İÔö¼Ó
                                        //×ªÒÆÊı¾İ
    memcpy(temp, content, (uint16)array->type);
    
    newlink = array->empty;             //ĞÂÊ¹ÓÃµÄ
    
    
    if(array->count < array->capcity)   //Èç¹û»¹ÓĞ¿ÕÏĞ¿éµÄ»°
    {
                                        //µ±Ç°¿ÕÏĞ¿éÖ¸ÏòÏÂÒ»¸ö¿ÕÏĞ¿é
        array->empty = array->link[newlink].word[1];
                                        //»ñÈ¡»·ĞÎË«ÏòÁ´±íµÄ×îºóÒ»¿é
        link = array->link[newlink].word[0];
                                        //×îºóÒ»¿éµÄÏÂÒ»¿éÖ¸Ïòµ±Ç°¿ÕÏĞ¿é
        array->link[link].word[1] = array->empty;
                                        //µ±Ç°¿ÕÏĞ¿éµÄÉÏÒ»¿éÖ¸Ïò×îºóÒ»¿é
        array->link[array->empty].word[0] = link;                               
    }
    else                                
    {
        array->empty = 0xFFFF;          //ÒÑ¾­Ê¹ÓÃÍêËùÓĞµÄ¿ÕÏĞ¿é
    }
    
    
    if(array->count > 1)                //Èç¹û²»ÊÇµÚÒ»Êı¾İ
    {   
                                        //Ö¸Ïò×îºóÒ»¿é
        link = array->link[array->used].word[0];
                                        //¸Õ²ÅµÄ×îºóÒ»¿éµÄÏÂÒ»¿éÖ¸Ïò¸Õ¸Õ±»·ÖÅäµÄÄÇÒ»¿é
        array->link[link].word[1] = newlink;
                                        //¸Õ·ÖÅäµÄÉÏÒ»¿éÖ¸Ïò¸Õ¸ÕµÄ×îºóÒ»¿é
        array->link[newlink].word[0] = link;
                                        //±»·ÖÅäµÄÄÇÒ»¿éµÄÏÂÒ»¿éÖ¸ÏòµÚÒ»¿é
        array->link[newlink].word[1] = array->used;
                                        //ÉÏÒ»¸öÊ¹ÓÃµÄ¿éÖ¸Ïò¸Õ±»·ÖÅäµÄÄÇÒ»¿é
        array->link[array->used].word[0] = newlink;
        
    }
    else                                //´´½¨µÚÒ»¸öÊı¾İ¿é
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
 * @ÃèÊö: 
 * 
 * @²ÎÊı: 
 * @param: array ÒªÉ¾³ıÊı¾İµÄARRAY
 * @param: index ÒªÉ¾³ıÊı¾İµÄË÷Òı
 * 
 * @·µ»Ø: 
 * @return: uint8  
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
uint8 SYS_Array_Remove(ARRAY* array, uint16 index)
{
    uint16 i;
    uint16 link;
    uint16 newlink;
    
    SYS_ENTER_SCRT();
    if(array->count <= index)           //É¾³ıµÄÊı¾İĞèÒª´æÔÚ
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_EPY;
    }
    
    link = array->used;                 //Ö¸ÏòµÚÒ»¸ö
    
    for(i = 0; i < index; i++)          //Ñ­»·»ñÈ¡Ë÷Òı
    {
        link = array->link[link].word[1];
    }
    
    newlink = link;
    
    array->count--;                     //¶ÓÁĞÓĞĞ§Êı¾İµÄÊıÁ¿¼õÉÙ
    
    if(array->count > 0)                //±íÊ¾»¹ÓĞÊı¾İ´æÔÚ
    {
                                        //±íÊ¾±»É¾³ıÏîµÄÉÏÒ»Ïî
        link = array->link[newlink].word[0];
                                        //±íÊ¾ÉÏÒ»ÏîµÄÏÂÒ»ÏîÖ¸ÕëÖ¸Ïò...
        array->link[link].word[1] = array->link[newlink].word[1];
                                        //±íÊ¾±»É¾³ıÏîµÄÏÂÒ»Ïî
        link = array->link[newlink].word[1];
                                        //±íÊ¾ÏÂÒ»ÏîµÄÉÏÒ»ÏîÖ¸ÕëÖ¸Ïò
        array->link[link].word[0] = array->link[newlink].word[0];
        
        if(index == 0)                  //ÅĞ¶ÏÊÇ·ñÉ¾³ıÁËµÚÒ»Ïî
        {
            array->used = link;         //µÚÒ»ÏîÖ¸ÏòÏÂÒ»Ïî
        }
        
    }
    else
    {
        array->used = 0xFFFF;           //±íÊ¾Ã»ÓĞÈÎºÎÊı¾İ´æÔÚÁË
    }
    
                                        //ÊÇ·ñÊÇµÚÒ»¸ö¿ÕÏĞÏî
    if(array->count < array->capcity - 1)
    {
        link = array->empty;            //»ñÈ¡µ±Ç°µÄ¿ÕÏĞÏî
                                        //É¾³ıÏîµÄÉÏÒ»ÏîÎªµ±Ç°¿ÕÏĞÏîµÄÉÏÒ»Ïî
        array->link[newlink].word[0] = array->link[link].word[0];
                                        //É¾³ıÏîµÄÏÂÒ»ÏîÎªµ±Ç°¿ÕÏĞÏî
        array->link[newlink].word[1] = link;
                                        //µ±Ç°¿ÕÏĞÏîµÄÉÏÒ»ÏîÎªÉ¾³ıÏî
        array->link[link].word[0] = newlink;
                                            
        link = array->link[newlink].word[0];
                                        //×îºóÏîµÄÏÂÒ»ÏîÎªÉ¾³ıÏî
        array->link[link].word[1] = newlink;
        
        array->empty = newlink;         //ÉèÖÃµ±Ç°ÏîÎªÉ¾³ıÏî
    }
    else                                //µÚÒ»¸ö¿ÕÏĞÏî
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
 * @ÃèÊö: ´ÓArrayÀïÃæ»ñÈ¡Ò»¸öÊı¾İ
 * 
 * @²ÎÊı: 
 * @param: array ĞèÒª»ñÈ¡µÄÊı¾İËùÔÚµÄarray
 * @param: index ĞèÒª»ñÈ¡µÄÊı¾İµÄË÷Òı
 * 
 * @·µ»Ø: 
 * @return: void*  __NULL »ñÈ¡Êı¾İÊ§°Ü   ÆäËü,»ñÈ¡µÄËùÔÚµÄµØÖ·
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
void* SYS_Array_Get(ARRAY* array, uint16 index)
{
    void* temp;
    uint16 i;
    uint16 link;
    
    
    SYS_ENTER_SCRT();
    if(array->count <= index)           //É¾³ıµÄÊı¾İĞèÒª´æÔÚ
    {
        SYS_EXIT_SCRT();
        return __NULL;
    }
    
    link = array->used;                 //Ö¸ÏòµÚÒ»¸ö
    
    for(i = 0; i < index; i++)          //Ñ­»·»ñÈ¡Ë÷Òı
    {
        link = array->link[link].word[1];
    }
                                        //Ö¸ÏòĞèÒª»ñÈ¡Êı¾İµÄÎ»ÖÃ
    temp = link * (uint16)array->type + (uint8*)array->data;
    
    SYS_EXIT_SCRT();
    
    return temp;                        //·µ»Ø½á¹û
}


/************************************************************************
 * @function: SYS_Array_Set
 * @ÃèÊö: ½«ArrayÀïÃæµÄÒ»¸öÊı¾İµÄÄÚÈİ¸ü¸Ä
 * 
 * @²ÎÊı: 
 * @param: array ĞèÒª¸ü¸ÄµÄÊı¾İËùÔÚµÄarray
 * @param: index ĞèÒª¸ü¸ÄµÄÊı¾İµÄË÷Òı
 * @param: content ĞèÒªÉèÖÃµÄÄÚÈİ
 * 
 * @·µ»Ø: 
 * @return: uint8  
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
uint8 SYS_Array_Set(ARRAY* array, uint16 index, void* content)
{
    void* temp;
    uint16 i;
    uint16 link;
    
    
    SYS_ENTER_SCRT();
    if(array->count <= index)           //É¾³ıµÄÊı¾İĞèÒª´æÔÚ
    {
        SYS_EXIT_SCRT();
        return SYS_ARRAY_ERR_EPY;
    }
    
    link = array->used;                 //Ö¸ÏòµÚÒ»¸ö
    
    for(i = 0; i < index; i++)          //Ñ­»·»ñÈ¡Ë÷Òı
    {
        link = array->link[link].word[1];
    }
                                        //Ö¸ÏòĞèÒª»ñÈ¡Êı¾İµÄÎ»ÖÃ
    temp = link * (uint16)array->type + (uint8*)array->data;
                                        //ÉèÖÃÊı¾İ
    memcpy(temp, content,(uint16)array->type);
    
    SYS_EXIT_SCRT();
    
    SYS_OK();    
}


/************************************************************************
 * @function: SYS_Array_Clear
 * @ÃèÊö: Çå³ıARRAY
 * 
 * @²ÎÊı: 
 * @param: array ĞèÒªÇå³ıÊı¾İµÄarray
 * @·µ»Ø: 
 * @ËµÃ÷: 
 * @×÷Õß: yzy (2013/6/9)
 *-----------------------------------------------------------------------
 * @ĞŞ¸ÄÈË: 
 ************************************************************************/
void SYS_Array_Clear(ARRAY* array)
{
    SYS_Array_Create(                    //ÖØĞÂ´´½¨Ò»¸öarray
        array->type, 
        array,     
        (void*)array->link, 
        array->capcity * ((uint16)array->type + 4));
}


#endif



