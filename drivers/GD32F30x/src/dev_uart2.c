/******************************Copyright(c)******************************
**                         
**
** File Name: dev_uart.c
** Author: 
** Date Last Update: 2019-11-02
** Description: UART����ͨ���������
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
** 1)����ʹ��open��close�����Ľӿں���: SYS_SER_Read()\SYS_SER_Clear()\
**      SYS_SER_Peek()\SYS_SER_Line()\SYS_SER_LineLen()
** 2)����:��ĳ�����̶�ռĳ����,�����ڽ��̳�ʼ����open()Ȼ������close()����
** 3)�ܽ�:����open\close�����ĺ�����:SYS_SER_ToBeRead()
*************************************************************************/
#define EXT_DEV_UART
#ifdef __MODULE__
#include "WOSsys.h"
#endif


#include "sys.h"
#include "hal.h"
#include "bsp.h"
//#include <string.h>
#include "apis.h"
#include "tasks.h"
#include "uartcfg.h"
#if (NO_OF_SERIAL == 0)
    #error "û�д��ڲ���Ҫ�������ļ�!"
#endif




/************************************************************************
**�����ⲿ����
************************************************************************/
void _UartClearHook(uint8 port, uint8* buffer, uint16 length);
void _UartReadHook(uint8 port, uint8* buffer, uint16 length);
void _UartWriteHook(uint8 port, uint8* buffer, uint16 length);


/************************************************************************
**�����ⲿ����
************************************************************************/
#ifndef __MODULE__
extern const SerialID* const gss_UartSID[NO_OF_SERIAL];
#else
extern SerialID* gss_UartSID[NO_OF_SERIAL];
#endif
extern const uint8_t _ucPortMap[];
//int _UartSSToUS(SerialSets ss,UART_InitTypeDef *us)
//{
////	    SerialSets ss;
////	    ss.baudrate = 9600;
////	    ss.databits = DataBits_8bits;
////	    ss.parit = Parit_N;
////	    ss.stopbits = StopBits_1;   
////	    Uartx_Config(&ss, sid);s
//
//    us->BaudRate = ss.baudrate;
//    switch(ss.parit)
//    {
//    case Parit_E:
//        us->ParityBit = EVEN;
//        break;
//    case Parit_O:
//        us->ParityBit = ODD;
//        break;   
//    case Parit_N:
//    default:
//        us->ParityBit = NONE;
//      break;
//        
//    }
//
//    us->StopBit = (UART_StopBitTypeDef)ss.stopbits;
//    us->DataBit = (UART_DataBitTypeDef)ss.databits;
//
//    return 0;
////	    
////	    if(handleuart->Baud == UartBaud1200)            UART_para.BaudRate = 1200;
////	    else if(handleuart->Baud == UartBaud2400)       UART_para.BaudRate = 2400;
////	    else if(handleuart->Baud == UartBaud4800)       UART_para.BaudRate = 4800;
////	    else if(handleuart->Baud == UartBaud9600)       UART_para.BaudRate = 9600;
////	    else if(handleuart->Baud == UartBaud19200)      UART_para.BaudRate = 19200;
////	    else if(handleuart->Baud == UartBaud38400)      UART_para.BaudRate = 38400;
////	    else if(handleuart->Baud == UartBaud115200)     UART_para.BaudRate = 115200;
////	    else if(handleuart->Baud == UartBaud10753)      UART_para.BaudRate = 10753;
////	    if(handleuart->Parity == 0)                                 UART_para.ParityBit = NONE;
////	    else if(handleuart->Parity == 1)                            UART_para.ParityBit = ODD;
////	    else if(handleuart->Parity == 2)                            UART_para.ParityBit = EVEN; 
////	    if((handleuart->StopBit == 0)||(handleuart->StopBit == 1))  UART_para.StopBit = OneBit;
////	    else if(handleuart->StopBit == 2)                           UART_para.StopBit = TwoBit;
////	    if(handleuart->DataBit == 0)                                UART_para.DataBit = Seven7Bit;
////	    else if(handleuart->DataBit == 1)                           UART_para.DataBit = Eight8Bit;
////	    else if(handleuart->DataBit == 2)                           UART_para.DataBit = Nine9Bit;
//
//}

int _FindExtPort(uint8_t port)
{
    for(int i = 0; i < NO_OF_SERIAL; i++)
    {
        if(_ucPortMap[i] == port)
            return i;
    }

    return -1;
}
/************************************************************************
 * @Function: SYS_SER_Init
 * @Description: ��������
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * @param: ss 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_SER_Init(uint8 port, void * ss)
{
    if(_ucPortMap[port] >= NO_OF_SERIAL)
    {
        return;
    }
    
//    UART_SInitTypeDef us;
// 
//    _UartSSToUS(*(SerialSets*)ss,&us);
#if SYS_UARTS_EN > 0
    if(_ucPortMap[port] == SYS_UARTS_IDX)
    {
        UartS_Config(ss, gss_UartSID[_ucPortMap[port]]);
    }
    else
#endif
    {
        Uartx_Config((SerialSets *)ss, gss_UartSID[_ucPortMap[port]]);
    }

}




#ifndef __NO_SYS__ 
/************************************************************************
 * @Function: SYS_SER_OpenCallBack
 * @Description: ���ڴ�
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * @param: isAnsyc true:�첽,�򲻿����˳�.false:ͬ��,�ȴ�ֱ����ȡ��Դ
 * @param: cbs �ص�����,__NULL��ʾ��(�ص������в����з�ʵʱ����,����ͬ����Ϣ,��TKsuspend)
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool SYS_SER_OpenCallBack(uint8 port, bool isAnsyc, TCallback cbs)
{

    if(_ucPortMap[port] >= NO_OF_SERIAL)            //���жϴ��ں�
    {
        return false;
    }
    
//    if(isAnsyc)                         //������첽��
//    {
//        if(SYS_SEM_Accept(&gss_UartRes[_ucPortMap[port]]) > 0)
//        {
//            gfs_SerialCallBack[_ucPortMap[port]] = cbs;
//        }
//        else
//        {
//            return false;
//        }
//    }
//    else                                //ͬ���ȴ�
//    {
//        if(SYS_SEM_Wait(&gss_UartRes[_ucPortMap[port]], 0) == SYS_ERR_OK)
//        {
//            gfs_SerialCallBack[_ucPortMap[port]] = cbs;
//        }
//    }
//#ifndef __NO_SYS__     
//                                        //Ϊ��ǰ�û����̱��
//    gucs_UartUserTkid[_ucPortMap[port]] = gs_TkTcbCur->tkid;
//#endif    
    return true;                        //���ش򿪴��ڳɹ�
}

#endif

/************************************************************************
 * @Function: SYS_SER_Open
 * @Description: ���ڴ�
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * @param: isAnsyc true:�첽,�򲻿����˳�.false:ͬ��,�ȴ�ֱ����ȡ��Դ
 * 
 * @Note: 
 * @Return: bool  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
bool SYS_SER_Open(uint8 port, bool isAnsyc)
{
#ifndef __NO_SYS__ 
    return SYS_SER_OpenCallBack(_ucPortMap[port], isAnsyc, __NULL);
#else
//	    Uartx_Open(gss_UartSID[_ucPortMap[port]]);

    return true;
#endif
}


/************************************************************************
 * @Function: SYS_SER_Close
 * @Description: ���ڹر�
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_SER_Close(uint8 port)
{
    if(_ucPortMap[port] >= NO_OF_SERIAL)            //���жϴ��ں�
    {
        return;
    }
#ifndef __NO_SYS__ 
//    if(gucs_UartUserTkid[_ucPortMap[port]] == gs_TkTcbCur->tkid)
//    {
//        gucs_UartUserTkid[_ucPortMap[port]] = 0xFF;      //�ָ��ɳ�ʼֵ
//        SYS_SEM_Release(&gss_UartRes[_ucPortMap[port]]);   //�ͷ�����ź���
//
////	#if (SYS_UARTSEM_EN > 0)
////	        SYS_SEM_Release((TESRes *)gss_UartSID[_ucPortMap[port]]->uart_rx_sem);
////	        SYS_SEM_Release((TESRes *)gss_UartSID[_ucPortMap[port]]->uart_tx_sem);
////	#endif
//
//        
//        return;
//    }
#else
//	    Uartx_Close(gss_UartSID[_ucPortMap[port]]);
#endif    
    return;
}



/************************************************************************
 * @Function: SYS_SER_Clear
 * @Description: ��մ��ڻ���
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_SER_Clear(uint8_t port)
{
    SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;

    if(_ucPortMap[port] >= NO_OF_SERIAL)
    {
        return;
    }
#ifndef __NO_SYS__     
                                    //�жϴ����Ƿ��ѱ���������ռ��
//    if(gucs_UartUserTkid[_ucPortMap[port]] != gs_TkTcbCur->tkid)
//    {
//        return;
//    }
#endif    
#if UART_CLEAR_HOOK_EN > 0
    uint16 count;
    count = uart->rcnt;             //���ڻ�����ʵ�ʿɶ�������
    if(count > (uart->rlen - uart->pr))
    {
        _UartClearHook(_ucPortMap[port], &uart->rbuff[uart->pr], uart->rlen - uart->pr);
        _UartClearHook(_ucPortMap[port], &uart->rbuff[0], count - (uart->rlen - uart->pr));
    }
    else
    {
        _UartClearHook(_ucPortMap[port], &uart->rbuff[uart->pr], count);
    }
#endif
    
    SYS_ENTER_AllSCRT();
    uart->rcnt = 0;
    uart->rp = 0;
    uart->pr = 0;
    uart->tp = 0;
    uart->tcnt = 0;
    uart->tbuff = __NULL;
    SYS_EXIT_AllSCRT();

}




/************************************************************************
 * @Function: SYS_SER_ReadDataExport
 * @Description: ���ڽ������ݵĸ��Ƶ���,���ڻ��������ݲ�ɾ��
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * @param: func ��������
 * 
 * @Note: 
 * @Return: uint16 ����������
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: ����ʵ�ֲο� SYS_SER_Clear()
 ************************************************************************/
void SYS_SER_ReadDataExport(uint8 port, TDataExport func)
{
    SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;

    if(_ucPortMap[port] >= NO_OF_SERIAL)
    {
        return;
    }
#ifndef __NO_SYS__     
                                    //�жϴ����Ƿ��ѱ���������ռ��
//    if(gucs_UartUserTkid[_ucPortMap[port]] != gs_TkTcbCur->tkid)
//    {
//        return;
//    }
#endif
#if (true)
    uint16 count;
    count = uart->rcnt;             //���ڻ�����ʵ�ʿɶ�������
    if(count > (uart->rlen - uart->pr))
    {
        func(_ucPortMap[port], &uart->rbuff[uart->pr], uart->rlen - uart->pr);
        func(_ucPortMap[port], &uart->rbuff[0], count - (uart->rlen - uart->pr));
    }
    else
    {
        func(_ucPortMap[port], &uart->rbuff[uart->pr], count);
    }
#endif

}




/************************************************************************
 * @Function: SYS_SER_WriteOption
 * @Description: �������ݷ���
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * @param: buffer �������ݻ���
 * @param: length �������ݳ���
 * @param: opt ѡ��:b0=1������������,b0=0ϵͳ��������
 * 
 * @Note: 
 * @Return: uint8  �ɹ�\ʧ��
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
int SYS_SER_WriteOption(uint8 port, uint8* buffer, uint16 length, uint16 opt)
{
    volatile SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;
                                    //�����Ϸ����ж�
    if((_ucPortMap[port] >= NO_OF_SERIAL) || (length == 0))
    {
        return SYS_ERR_FT;
    }
#ifndef __NO_SYS__     
                                    //�жϴ����Ƿ��ѱ���������ռ��
//    if(gucs_UartUserTkid[_ucPortMap[port]] != gs_TkTcbCur->tkid)
//    {
//        return SYS_ERR_FT;
//    }
#endif 

//    return hal_uart_send(gss_UartSID[_ucPortMap[port]], buffer, length, 3000);
//}
//	                                    //���÷��ͽṹ��
    uart->tbuff = buffer;           //���͵����ݻ���
    uart->tlen = length;            //���ͻ��泤��
    uart->tcnt = length;            //���͵��ֽ���
    uart->tp = 0;
                                    //�������Ͳ��ȴ��������
#if SYS_UARTS_EN > 0
    if(_ucPortMap[port] == SYS_UARTS_IDX)
    {
        UartS_StartSend(gss_UartSID[_ucPortMap[port]]);
    }
    else
#endif
    {
        Uartx_StartSend(gss_UartSID[_ucPortMap[port]]);
    }
    
#if UART_WRITE_HOOK_EN > 0
    _UartWriteHook(_ucPortMap[port], buffer, length);
#endif
//	    uint16_t tick = 100000;
    while(1)
    {
        if(uart->tcnt == 0)
        {
            break;
        }
        
        if(opt & 0x0001)
        {
            //������������
        }
        else
        {
#ifndef __NO_SYS__           
            //ϵͳ��������,Ĭ�Ϸ�ʽ.
            msleep(10);
#else
            msDelay(10);
            static int stcnt = 0;
            if(stcnt!= 0 && stcnt == uart->tcnt)
            {

                {
                    Uartx_Send(gss_UartSID[_ucPortMap[port]]);
                }              

            }
            stcnt = uart->tcnt;
#endif              
        }
    }
//    msleep(1);
//    Uartx_WaitSendBufNull(gss_UartSID[_ucPortMap[port]]);
    SYS_OK();
}


//	extern int guc_count;
//	extern uint8_t * p_tmpbuf;
/************************************************************************
 * @Function: SYS_SER_Write
 * @Description: �������ݷ���
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * @param: buffer �������ݻ���
 * @param: length �������ݳ���
 * 
 * @Note: 
 * @Return: uint8  �ɹ�\ʧ��
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8_t SYS_SER_Write(uint8_t port, uint8_t* buffer, uint16_t length, uint32_t to)
{
//    memcpy_s(p_tmpbuf+guc_count, buffer, length);
//    guc_count+=length;

  
  
    return SYS_SER_WriteOption(port, buffer, length, 0);
}





/************************************************************************
 * @Function: SYS_SER_Read
 * @Description: �������ݶ�ȡ
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * @param: buffer �������,__NULL��ʾ����
 * @param: length ��ȡ����
 * 
 * @Note: 
 * @Return: uint16  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_Read(uint8 port, uint8* buffer, uint16 length, uint32_t to)
{
    SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;
    uint16 pr;
    uint16 count;
    uint16 i;
    uint16_t leftLength = length;
                                    //�����Ϸ����ж�
    if((_ucPortMap[port] >= NO_OF_SERIAL) || (length == 0))
    {
        return 0;
    }
#ifndef __NO_SYS__     
                                    //�жϴ����Ƿ��ѱ���������ռ��
//    if(gucs_UartUserTkid[_ucPortMap[port]] != gs_TkTcbCur->tkid)
//    {
//        return 0;
//    }
#endif    
    while(to > 0)
    {
        SYS_ENTER_AllSCRT();
        count = uart->rcnt;
        if(count > length)              //��ȡʵ�ʶ�ȡ�����ݳ���
        {
            count = length;
        }
        SYS_EXIT_AllSCRT();
                                        //ѭ��ת������
        if(__NULL != buffer)            //not dequeue
        {
            pr = uart->pr;              //���ݶ�ȡ��ʼָ��
            for(i = 0; i < count; i++)
            {
                buffer[i] = uart->rbuff[pr];
                pr++;
                if(pr >= uart->rlen)    //����
                {
                    pr = 0;
                }
            }
        }
        else                            //dequeue
        {
#if UART_READ_HOOK_EN > 0
            if(count > (uart->rlen - uart->pr))
            {
                _UartReadHook(_ucPortMap[port], &uart->rbuff[uart->pr], uart->rlen - uart->pr);
                _UartReadHook(_ucPortMap[port], &uart->rbuff[0], count - (uart->rlen - uart->pr));
            }
            else
            {
                _UartReadHook(_ucPortMap[port], &uart->rbuff[uart->pr], count);
            }
#endif
        }
        
        uart->pr += count;              //���ݴ���ָ��ǰ��
        while(uart->pr >= uart->rlen)   //����
        {
            uart->pr -= uart->rlen;
        }
        
        SYS_ENTER_AllSCRT();
        uart->rcnt -= count;            //δ�������ݳ��ȵݼ� yzy->rcnt��handler�лᱻ�ı�
        SYS_EXIT_AllSCRT();

        leftLength -= count;

        if(0 == leftLength)
        {
            break;
        }
        msleep(10);
        to -= 10;
    }
#if UART_READ_HOOK_EN > 0
     _IF_TRUE_DO(__NULL != buffer, _UartReadHook(_ucPortMap[port], buffer, count));
#endif

    return count;
}
/************************************************************************
 * @Function: SYS_SER_Recv_IT
 * @Description: �������ݶ�ȡ
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * @param: buffer �������,__NULL��ʾ����
 * @param: length ��ȡ����
 * 
 * @Note: 
 * @Return: uint16  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
#if (SYS_UARTSEM_EN > 0)

uint32_t SYS_SER_Recv_IT(uint8 port, void *data, uint32_t expect_size,
                      uint32_t *recv_size, uint32_t timeout)
{
    uint8_t *pdata = (uint8_t *)data;  
    uint32_t rx_count = 0;
//    uint32_t count = 0;
//	    uint16 pr;
//    int32_t ret = -1;
    SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;

    if((_ucPortMap[port] >= NO_OF_SERIAL) || (expect_size == 0)
        || __NULL == data)
    {
        return 0;
    }
#ifndef __NO_SYS__     
                                    //�жϴ����Ƿ��ѱ���������ռ��
    if(gucs_UartUserTkid[_ucPortMap[port]] != gs_TkTcbCur->tkid)
    {
        return 0;
    }
#endif    

    while ( rx_count < expect_size )
    {
//	        SYS_ENTER_AllSCRT();
//	        count = uart->rcnt;
//	        
//	        SYS_EXIT_AllSCRT();
//	                                        //ѭ��ת������
//	        pr = uart->pr;              //���ݶ�ȡ��ʼָ��
        while(uart->rcnt > 0)
        {
            pdata[rx_count++] = uart->rbuff[uart->pr];
            SYS_ENTER_AllSCRT();
            uart->pr++;
            if(uart->pr >= uart->rlen)    //����
            {
                uart->pr = 0;
            }
            uart->rcnt--;
            SYS_EXIT_AllSCRT();
            if(rx_count==expect_size)
            {
                break;
            }
        }


        if(rx_count==expect_size)
        {
            break;
        }
        if(SYS_ERR_OK == SYS_SEM_Wait((TESRes *)gss_UartSID[_ucPortMap[port]]->uart_rx_sem, timeout))
        {
            while(uart->rcnt > 0)
            {
                pdata[rx_count++] = uart->rbuff[uart->pr];
                SYS_ENTER_AllSCRT();
                uart->pr++;
                if(uart->pr >= uart->rlen)    //����
                {
                    uart->pr = 0;
                }
                uart->rcnt--;
                SYS_EXIT_AllSCRT();
                if(rx_count==expect_size)
                {
                    break;
                }
            }
        }
        else
        {
            break;
        }
    }

    if (recv_size != NULL)
    {
        *recv_size = rx_count;
    }

  
    return rx_count;
}
#endif
/************************************************************************
 * @Function: SYS_SER_Peek
 * @Description: ���ڽ������ݿ�̽,ִ�к�������Ȼ�����ڽ��ջ�����
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * @param: buffer ��ſ�̽���ݵĻ���
 * @param: start �ӵڼ������ݿ�ʼ��̽(0-) 
 * @param: length ����̽���ݳ���
 * 
 * @Note: 
 * @Return: uint16  ��̽�õ���ʵ�����ݳ���
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_Peek(uint8 port, uint8* buffer, uint16 start, uint16 length)
{
    SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;
    uint16 pr;
    uint16 count;
    uint16 i;
                                    //�����Ϸ����ж�
    if((_ucPortMap[port] >= NO_OF_SERIAL) || (length == 0))
    {
        return 0;
    }
    
    if(start >= uart->rcnt)         //��ʼ�ֽ����Ϸ����ж�
    {
        return 0;
    }
#ifndef __NO_SYS__     
//                                    //�жϴ����Ƿ��ѱ���������ռ��
//    if(gucs_UartUserTkid[_ucPortMap[port]] != gs_TkTcbCur->tkid)
//    {
//        return 0;
//    }
#endif
    SYS_ENTER_AllSCRT();
    if((start + length) < uart->rcnt)//"����ȡ����"��"���ڻ�����ʵ�ʿɶ�������"�ıȽ�
    {
        count = length;
    }
    else
    {
        count = uart->rcnt - start;
    }
    SYS_EXIT_AllSCRT();
                                    //ѭ��ת������
    pr = uart->pr + start;          //���ݶ�ȡ��ʼָ��
    if(pr >= uart->rlen)
    {
        pr -= uart->rlen;
    }
    
    for(i = 0; i < count; i++)
    {
        buffer[i] = uart->rbuff[pr];
        pr++;
        if(pr >= uart->rlen)        //����
        {
            pr = 0;
        }
    }

    return count;
}




/************************************************************************
 * @Function: SYS_SER_ReadByte
 * @Description: �Ӵ��ڶ�ȡһ���ֽ�����
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * 
 * @Note: 
 * @Return: uint16  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_ReadByte(uint8 port)
{
    uint8 result;

    if(SYS_SER_Read(port, &result, 1, 0) == 1)
    {
        return result;
    }
    return 256;
}






/************************************************************************
 * @Function: SYS_SER_ToBeRead
 * @Description: ���ڿɶ�ȡ���ݵĳ���
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * 
 * @Note: 
 * @Return: uint16  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_ToBeRead(uint8 port)
{
    SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;
    uint16 result;
    if(_ucPortMap[port] >= NO_OF_SERIAL)
    {
        return 0;
    }

    SYS_ENTER_AllSCRT();
    result = uart->rcnt;
    SYS_EXIT_AllSCRT();

    return result;
}




/************************************************************************
 * @Function: SYS_SER_Line
 * @Description: ���ڶ�ȡ������(��δ�ҵ��н�����,���ڻ��汻����)
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * @param: buffer ������ݻ���,__NULL��ʾ����
 * @param: length ������ݻ���ĳ���
 * 
 * @Note: 
 * @Return: uint16  ��ȡ���г���(����һ����β��).
 *          0��ʾ�����ݻ�δ�ҵ��н�����,1��ʾ��������β��
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_Line(uint8 port, uint8* buffer, uint16 length)
{
    SerialBuffer* uart = gss_UartSID[_ucPortMap[port]]->buffer;
    uint16 count;
    uint16 pr;
    uint16 i;
    uint8 sbyte;

    if(_ucPortMap[port] >= NO_OF_SERIAL)
    {
        return 0;
    }

    if(uart->rcnt == 0)             //δ����������ݳ���Ϊ0,����
    {
        return 0;
    }
#ifndef __NO_SYS__     
                                    //�жϴ����Ƿ��ѱ���������ռ��
//    if(gucs_UartUserTkid[_ucPortMap[port]] != gs_TkTcbCur->tkid)
//    {
//        return 0;
//    }
#endif
    SYS_ENTER_AllSCRT();
    count = uart->rcnt;             //δ�������ݳ���
    if(count > length)              //���������ݳ���
    {
        count = length;
    }
    SYS_EXIT_AllSCRT();
                                    //ѭ���ж���β,ͬʱת������
    pr = uart->pr;                  //����ָ��
    for(i = 0; i < count; i++)      //ѭ����ȡ����
    {
        sbyte = uart->rbuff[pr];
        _IF_TRUE_DO(__NULL != buffer, buffer[i] = sbyte);
        pr++;
        if(pr >= uart->rlen)
        {
            pr = 0;
        }
        if((sbyte == '\r') || (sbyte == '\n'))
        {
            break;
        }
    }

    if(i == count)                  //������һ������
    {
        return 0;
    }
    count = i + 1;
    
#if UART_READ_HOOK_EN > 0
    if(__NULL == buffer)
    {
        if(count > (uart->rlen - uart->pr))
        {
            _UartReadHook(_ucPortMap[port], &uart->rbuff[uart->pr], uart->rlen - uart->pr);
            _UartReadHook(_ucPortMap[port], &uart->rbuff[0], count - (uart->rlen - uart->pr));
        }
        else
        {
            _UartReadHook(_ucPortMap[port], &uart->rbuff[uart->pr], count);
        }
    }
#endif
    
    uart->pr += count;              //����ָ��ǰ��
    while(uart->pr >= uart->rlen)
    {
        uart->pr -= uart->rlen;
    }
    
    SYS_ENTER_AllSCRT();
    uart->rcnt -= count;            //δ�������ݳ��ȵݼ�
    SYS_EXIT_AllSCRT();

    
#if UART_READ_HOOK_EN > 0
     _IF_TRUE_DO(__NULL != buffer, _UartReadHook(_ucPortMap[port], buffer, count));
#endif

    return count;

}







/************************************************************************
 * @Function: SYS_SER_LineLen
 * @Description: ��ȡ���ڵ�ǰ�г���
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * 
 * @Note: 
 * @Return: uint16  �г���(����һ����β��)
 *          0��ʾ�����ݻ�δ�ҵ��н�����,1��ʾ��������β��
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint16 SYS_SER_LineLen(uint8 port)
{
    SerialBuffer* uart = gss_UartSID[port]->buffer;
    uint16 count;
    uint16 pr;
    uint16 i;

    if(port >= NO_OF_SERIAL)
    {
        return 0;
    }

    if(uart->rcnt == 0)                 //û��Ϊ��ȡ���ݷ���0
    {
        return 0;
    }
#ifndef __NO_SYS__ 
                                        //�жϴ����Ƿ��ѱ���������ռ��
//    if(gucs_UartUserTkid[port] != gs_TkTcbCur->tkid)
//    {
//        return 0;
//    }
#endif            
    count = uart->rcnt;                 //δ�������ݳ���
    pr = uart->pr;                      //����ָ��
    for(i = 0; i < count; i++)          //ѭ���ж�
    {
        if((uart->rbuff[pr] == '\r') || (uart->rbuff[pr] == '\n'))
        {
            break;
        }
        pr++;
        if(pr >= uart->rlen)
        {
            pr = 0;
        }
    }
    
    if(i == count)                      //������������
    {
        return 0;
    }
    
    count = i + 1;
    
    return count;

}





/************************************************************************
 * @Function: SYS_SER_PrintValueOption
 * @Description: ��������,�����ַ����ĸ�ʽ�����з���
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * @param: str ���͵������ַ���
 * @param: value ���͵�����
 * @param: opt ѡ��:b0=1������������,b0=0ϵͳ��������
 * 
 * @Note: 
 * @Return: uint8  
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_SER_PrintValueOption(uint8 port, string str, uint32 value, uint8 opt)
{
//    uint32 i;
//    uint32 len = strlen(str);
//    uint16 templen;
//    uint8 data[12];
//
//    if(len == 0)                        //����ַ�������Ϊ0��ֱ�ӷ���
//    {
//        SYS_OK();                        //���ش���ɹ�
//    }
//
//    for(i = 0; i < len; i++)            //ɨ��ֱ������'%'
//    {                                        
//        if(str[i] == '%')               //�ж��Ƿ�Ϊ'%'        
//        {                               
//            if(str[i + 1] == 'x' 
//               || str[i + 1] == 'd' 
//               || str[i + 1] == 's'
//               )
//            {                           //��ӡǰ����ַ���
//                SYS_SER_WriteOption(port, (uint8*)str, i, opt);
//                break;                  //����
//            }
//        }
//    }
//    if(i == len)                        //����Ѿ�ɨ����������һ��Ϊ%��ֱ���˳�
//    {                                   //�����Ϣ
//       SYS_SER_WriteOption(port, (uint8*)str, len, opt);
//       SYS_OK();                         //���ش���ɹ�
//    }
//
//    switch(str[i + 1])                  //ѡ������
//    {
//        case 'x':                       //HEX���  
//                                        //�ж��Ƿ�Ϊ'0' - '9'      
//            if(!IsBcdChar(str[i + 2]))
//            {
//                break;                  //���ǵĻ��˳�
//            }
//                                        //���ֵ�Hex�ַ�����ת��
//            NumToHexString(
//                data, 
//                (uint8*)&value, 
//                (str[i + 2] - '0') >> 1
//                );
//                                        //��ӡת������������
//            SYS_SER_WriteOption(
//                port, 
//                data, 
//                str[i + 2] - '0',
//                opt
//                );
//            i++;                        //�������Ĵ���
//            break;
//        case 'd':                       //�����ַ����    
//                                        //�ж��Ƿ�Ϊ��ʽ�����
//            if(IsBcdChar(str[i + 2]))  
//            {
//                templen = str[i + 2] - '0';
//                i++;
//            }
//            else
//            {
//                templen = 0;
//            }
//                                        //���ֵ�BCD�ַ�����ת��
//            templen = NumToBcdString(   
//                data, 
//                value, 
//                templen
//                );
//
//            SYS_SER_WriteOption(         //�������
//                port, 
//                data, 
//                templen,
//                opt
//                );
//            break;
//        case 's':                       //ֱ������ַ���        
//            SYS_SER_WriteOption(         //����ַ���
//                port, 
//                (uint8*)value, 
//                strlen((string)value),
//                opt
//                );
//            break;
//    }
//    i += 2;
//    
//    if(i >= len)                        //����Ѿ�ɨ����������һ��Ϊ%��ֱ���˳�
//    {
//       SYS_OK();                         //���ش���ɹ�
//    }
//                                        //������������
//    SYS_SER_WriteOption(port, (uint8*)str + i, strlen(str + i), opt);

    SYS_OK();                            //���ؽ��
}





/************************************************************************
 * @Function: SYS_SER_PrintValue
 * @Description: ��������,�����ַ����ĸ�ʽ�����з���
 * 
 * @Arguments: 
 * @param: port �������(��Ӳ�����ں�)
 * @param: str ���͵������ַ���
 * @param: value ���͵�����
 * 
 * @Note: 
 * @Return: uint8  �ɹ�/ʧ��
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
uint8 SYS_SER_PrintValue(uint8 port, string str, uint32 value)
{
    return SYS_SER_PrintValueOption(port, str, value, 0);
}


void UART_Init(void)
{
    SYS_UART_Init();
}

/************************************************************************
 * @Function: SYS_UART_Init
 * @Description: ����ģ���ʼ��
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_UART_Init(void)
{

    SerialSets ss;
//    UART_InitTypeDef us;
    ss.baudrate = 9600;
    ss.databits = DataBits_8bits;
    ss.parit = Parit_N;
    ss.stopbits = StopBits_1;
//    ss.flowctrl = FLOW_CONTROL_DISABLED;
//    ss.mode = MODE_TX_RX;
//    _UartSSToUS(ss,&us);

    for(uint8 uc_i = 0; uc_i < NO_OF_SERIAL; uc_i++)
    {
        gucs_UartUserTkid[uc_i] = 0xFF;
#ifndef __NO_SYS__         
//        SYS_SEM_Create(1, &gss_UartRes[uc_i]);
        aos_sem_new(&gss_UartRes[uc_i], 1);

#endif        
        gucs_UartRevFlag[uc_i] = 0;
    
    #if SYS_UARTS_EN > 0
        if(uc_i == SYS_UARTS_IDX)
        {
            Init_UartS(gss_UartSID[uc_i]);
        }
        else
    #endif
        {
            Uartx_Init((SerialID *)gss_UartSID[uc_i], &ss);
        }
    }
}

void SYS_UART_Close(uint8 port)
{
    Uartx_Close(gss_UartSID[_ucPortMap[port]]);

}

void SYS_UART_Open(uint8 port)
{
    Uartx_Open(gss_UartSID[_ucPortMap[port]]);

//	    UART_SInitTypeDef us;
//	    SerialSets ss;
//	    ss.baudrate = 9600;
//	    ss.parit = Parit_N;
//	    ss.databits = DataBits_8bits;
//	    ss.stopbits = StopBits_1;
//	
//	    _UartSSToUS(ss,&us);
//	#if SYS_UARTS_EN > 0
//	    if(_ucPortMap[port] == SYS_UARTS_IDX)
//	    {
//	        UartS_Config(&us, gss_UartSID[_ucPortMap[port]]);
//	    }
//	    else
//	#endif
//	    {
//	        Uartx_Config(&us, gss_UartSID[_ucPortMap[port]]);
//	    }

}

void SYS_UART_DeInit(void)
{
    
    for(uint8 uc_i = 0; uc_i < NO_OF_SERIAL; uc_i++)
    {
        Uartx_DeInit(gss_UartSID[uc_i]);
    }
}




//#if (SYS_UARTEVT_EN > 0)
//#ifdef __NO_SYS__
//#include "define.h"
//#endif
/************************************************************************
 * @Function: _UartRevMessageDelivery
 * @Description: �����������Ƿ�������ô�����Ϣ,��������Ϣ
 * 
 * @Arguments: 
 * @param: uartidx ����
 * @param: msg_recv ��Ӧ�Ĵ��ڽ�����Ϣ
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
//void _UartRevMessageDelivery(uint8 uartidx, uint8 msg_recv)
//{
//    uint8 grptk;
//    uint8 subtk;
//    uint8 tkmap;
//    int extPort = -1;
//    for(grptk = 0; grptk < sizeof(gucs_MsgUartTkMap[uartidx]); grptk++)
//    {
//        tkmap = gucs_MsgUartTkMap[uartidx][grptk];
//        extPort = _FindExtPort(uartidx);
//        if(extPort < 0)
//            return;
//        while(tkmap)
//        {
//            subtk = FBit_Maps(tkmap);
//#ifndef __NO_SYS__ 
//            SYS_Message_Send(msg_recv+extPort, (grptk << 3) + subtk);
//#else
//            //extern uint8_t  gucSMskEvt[S_MSK_MAXMDL];
////	            gucSMskEvt[ (grptk << 3) + subtk] |= 1 << uartidx;
//            SysSndMsg( (grptk << 3) + subtk,  msg_recv + extPort);
//#endif
//            tkmap &= ~Bit_Map8[subtk];
//        }
//    }
//}
//#endif

void _UartRevMessageDelivery(uint8 uartidx, uint8 msg_recv)
{
    uint8 subtk;
    uint8 tkmap;
    
    extern const KTaskDeclare __TKDeclare[SYS_TK_NUM];
    const KTaskDeclare* dec = __TKDeclare;
    
    for(uint8 uc_i = 0; uc_i < sizeof(guc_MsgUartTkMap[0]); uc_i++)
    {
        tkmap = guc_MsgUartTkMap[uartidx][uc_i];
        while(tkmap)
        {
            subtk = Bit_Maps[tkmap];
//	            SYS_Message_Send(msg_recv, (uc_i << 3) + subtk);
//	
//	            msg = MSG_YEAR;
//	            dec+=(uc_i << 3) + subtk;
            krhino_buf_queue_send(dec[(uc_i << 3) + subtk].ktask->msg, &msg_recv, 1);

            
            tkmap &= ~Bit_Map8[subtk];
        }
    }
}


/************************************************************************
 * @Function: SYS_UART_EvtMessageLoop
 * @Description: UART�������ݽ�����Ϣ�ַ�����
 * @Arguments: 
 * @Note: 
 * @Auther: yzy
 * Date: 2015/5/31
 *-----------------------------------------------------------------------
 * @History: 
 ************************************************************************/
void SYS_UART_EvtMessageLoop(void)
{
#if (SYS_UARTEVT_EN > 0)
    for(uint8 uartidx = 0; uartidx < NO_OF_SERIAL; uartidx++)
    {
        if(gucs_UartRevFlag[uartidx])
        {                               //���ڻص�����,��ִ��
            if(gfs_SerialCallBack[uartidx] != __NULL)
            {
                gfs_SerialCallBack[uartidx]();
            }
            else                        //����,���н�����Ϣ�ַ�
            {
                _UartRevMessageDelivery(uartidx, MSG_UART_IDX0);
            }
            gucs_UartRevFlag[uartidx] = 0;
        }
    }
#endif
}


