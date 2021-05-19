/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: netp.c
**��   ��   ��: yzy
**����޸�����: 2007��9��5��
**��        ��: ����������ͨѶ�˿ڵ�ͨ�ú���
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2007��9��5��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_NETP
//	#include "es.h"
//	#include "hal.h"
//	#include "bsp.h"
//	#include "app.h"
//	#include "apis.h"
//	#include "extapi.h"


#include "public.h"
#include "lib_com/calclib_gw.h"

#include "netp.h"
#include "hlv.h"
//	#include "netp_ditbl.c"
#include "paradef.h"
#include "pst_zy.h"
#include "evt.h"
#include "netp_fm.h"
#include "task.h"
#define  NETPREADCNT     2              //������ʶ���ض�����

cpu_stack_t  gs_TKNetpStack[TASK_NETP_STKL];
ktask_t      gs_TKNetpHandle;
kbuf_queue_t gs_TKNetpQueue;
char         gc_TKNetpQbuf[MSG_BUFF_LEN];
static char         g_TKNetpQ_buf_recv[MSG_BUFF_LEN];
static size_t       g_TKNetpQ_recv_size;

extern kbuf_queue_t gs_TKNetp2Queue;
extern kbuf_queue_t gs_TKNetp3Queue;
extern kbuf_queue_t gs_TKNetp4Queue;


//netp�˿ں���ʵ�ʴ�����ŵĶ�Ӧ��ϵ
#if (MCB_PORT_NUM == 4)
const uint8 NETP_PHY_PORT[MCB_PORT_NUM] = {NETP_PORT_NO, NETP2_PORT_NO, NETP3_PORT_NO, NETP4_PORT_NO};
#elif (MCB_PORT_NUM == 3)
const uint8 NETP_PHY_PORT[MCB_PORT_NUM] = {NETP_PORT_NO, NETP2_PORT_NO, NETP3_PORT_NO};
#elif(MCB_PORT_NUM == 2)
const uint8 NETP_PHY_PORT[MCB_PORT_NUM] = {NETP_PORT_NO, NETP2_PORT_NO};
#elif(MCB_PORT_NUM == 1)
const uint8 NETP_PHY_PORT[MCB_PORT_NUM] = {NETP_PORT_NO};
#endif



/************************************************************************
 * @function: BG_PackRead
 * @����: ������ݶ�ȡ֡
 * 
 * @����: 
 * @param: buffer
 * @param: di 
 * @param: tstno 
 * 
 * @����: ֡����
 * @return: uint8  
 * @˵��: ������BG_Pack()�����ϼ򵥴����
 * @����: yzy (2011-9-5)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint16 BG_PackRead(uint8* buffer, uint32 di, uint8 tstno)
//	{
//	    PST_Frame frame;
//	    DLT_Frame* frm = &(frame._dlt);
//	    uint16 len;
//	
//	    frm->cmd = 0x01;                                     //������
//	    frm->atr = 0;                                        //����
//	    frm->send = buffer;                                  //������ʼ��ַ
//	    frm->wd.word[0] = di;                                //���ݱ�ʶ
//	    frm->wd.word[1] = 0xFFFF;
//	    frm->len = 2;                                        //����
//	                                                        //������
//	//	    if(BG_Pack(frm, gss_CDATATst[tstno].ad, &len) != PST_ERR_OK)
//	//	    {
//	//	        return 0;
//	//	    }
//	    return len;
//	}



/************************************************************************
 * @function: XBG_PackRead
 * @����: ������ݶ�ȡ֡
 * 
 * @����: 
 * @param: buffer 
 * @param: di 
 * @param: tstno 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: ������XBG_Pack()�����ϼ򵥴����
 * @����: yzy (2011-9-5)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint16 XBG_PackRead(uint8* buffer, uint32 di, uint8 tstno)
//	{
//	    PST_Frame frame;
//	    DLT_Frame* frm = &(frame._dlt);
//	    uint16 len;
//	
//	    frm->cmd = 0x11;                                     //������
//	    frm->atr = 0;                                        //����
//	    frm->send = buffer;                                  //������ʼ��ַ
//	    frm->wd.word[0] = di;                                //���ݱ�ʶ
//	    frm->wd.word[1] = di >> 16;
//	    frm->len = 4;                                        //����������
//	                                                        //�²�����
//	//	    if(XBG_Pack(frm, gss_CDATATst[tstno].ad, &len) != PST_ERR_OK)
//	//	    {
//	//	        return 0;
//	//	    }
//	    return len;
//	}






/************************************************************************
 * @function: BG_ExtractData
 * @����: ��ȡ��Ч��������
 * 
 * @����: 
 * @param: buffer �������ݻ���,������Ч��������
 * @param: length ����Ϊ���ջ��泤��,����Ϊ��Ч�������ݳ���
 * @param: di ��ʶ(�ձ�ʾ���б�ʶ)
 * 
 * @����: 
 * @return: uint8 NETP_ERR_OK,NETP_ERR_NT,NETP_ERR_FT
 * @˵��: 
 * @����: yzy (2011-9-7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 BG_ExtractData(uint8* buffer, uint16* length, uint32 di)
//	{
//	    DLT_Frame frm;
//	    uint16 frmdi;
//	
//	    *length = PST_CheckFrame2(PST_FRM_BG_NO, buffer, *length);
//	    if(*length)
//	    {                                                   //�����յ�������
//	//	        BG_Decode(buffer, &frm, __NULL);
//	
//	        if(frm.cmd & 0x40)                              //1.��֧�ָ�����
//	        {
//	            (*length) = 0;
//	            return NETP_ERR_NT;
//	        }
//	                                                //2.�ж����ݱ�ʶ
//	        if(di != 0)
//	        {
//	            frmdi = ByteToLong(frm.recv);
//	            if((uint32)frmdi != di)
//	            {
//	                return NETP_ERR_FT;
//	            }
//	        }
//	
//	        if(frm.len > (*length) + 2)                     //ת�ƶ�ȡ��������
//	        {
//	            MoveBuffer(frm.recv + 2, buffer, (*length));
//	        }
//	        else
//	        {
//	            MoveBuffer(frm.recv + 2, buffer, frm.len - 2);
//	            (*length) = frm.len - 2;
//	        }
//	
//	        SYS_OK();
//	    }
//	
//	    (*length) = 0;
//	    return NETP_ERR_FT;
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: XBG_ExtractData
//	 * @����: ��ȡ��Ч��������
//	 * 
//	 * @����: 
//	 * @param: buffer �������ݻ���,������Ч��������
//	 * @param: length ����Ϊ���ջ��泤��,����Ϊ��Ч�������ݳ���
//	 * @param: di ��ʶ(�ձ�ʾ���б�ʶ)
//	 * 
//	 * @����: 
//	 * @return: uint8 NETP_ERR_OK,NETP_ERR_NT(�лظ�,����֧��),NETP_ERR_FT
//	 * @˵��: 
//	 * @����: yzy (2011-9-7)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 XBG_ExtractData(uint8* buffer, uint16* length, uint32 di)
//	{
//	    DLT_Frame frm;
//	    uint32 frmdi;
//	
//	    *length = PST_CheckFrame2(PST_FRM_BG_NO, buffer, *length);
//	    if(*length)
//	    {                                                   //�����յ�������
//	//	        BG_Decode(buffer, &frm, __NULL);
//	        if(frm.cmd == 0x94)
//	        {
//	            return SYS_ERR_OK;
//	        }
//	        if(frm.cmd & 0x40)                              //��֧�ָ�����
//	        {
//	            (*length) = 0;
//	            return NETP_ERR_NT;
//	        }
//	                                                //2.�ж����ݱ�ʶ
//	        if(di != 0)
//	        {
//	            frmdi = ByteToLong(frm.recv);
//	            if(frmdi != di)
//	            {
//	                return NETP_ERR_FT;
//	            }
//	        }
//	
//	        if(frm.len > (*length) + 4)                     //ת�ƶ�ȡ��������
//	        {
//	            MoveBuffer(frm.recv + 4, buffer, (*length));
//	        }
//	        else
//	        {
//	            MoveBuffer(frm.recv + 4, buffer, frm.len - 4);
//	            (*length) = frm.len - 4;
//	        }
//	
//	        SYS_OK();
//	    }
//	
//	    (*length) = 0;
//	    return NETP_ERR_FT;
//	}
void Netp_ReadFinish(uint8 index);

/************************************************************************
 * @function: Netp_PentTran
 * @����: �����˿�����͸������
 * 
 * @����: 
 * @param: buffer �շ�����
 * @param: length �շ��������ݳ���
 * @param: ss �˿�����
 * @param: port �˿ں�(0-!)
 * @param: overtime �ȴ�ʱ��,��λ100ms
 * @param: nfrm ͨѶ�Ĺ�Լ���,0xff��ʾΪ�м�
 * 
 * @����: 
 * @return: uint8  SYS_ERR_OK, SYS_ERR_FT
 * @˵��: 
 * @����: yzy (2011-9-8)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Netp_PentTran(uint8* buffer, uint16* length, uart_config_t * ss, uint16 overtime, uint8 port)
{
    uint8 phyport;
    uint16 ulen = 0, plen = 0;
    uint8_t protocol;
    if(port >= MCB_PORT_NUM)
    {
        return NETP_ERR_FT;
    }
    phyport = NETP_PHY_PORT[port];
//	    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);//LED_485_T
                                                        //�򿪴���
    
    //phyport = NETP_PORT_NO;
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);           //��һ��

//	    ss->baudrate = NETP_PORT_BAUD;
//	    ss->databits = DataBits_8bits;
//	    ss->parit = Parit_N;
//	    ss->stopbits = StopBits_1;
//	                                            //�򿪴���
//	    gs_GPIO.GPO_Out(GPO_485_PWR,true);
    Netp_PowerMng(1, NETP_PORT_PWR_TRANS);

    aos_msleep(200);
    gs_Uart.open(phyport, false);
    gs_Uart.Init(phyport, ss);
    gs_Uart.Clear(phyport);                            //��մ��ڻ���,������
    gs_Uart.Write(phyport, buffer, *length,1000);
    ulen = 0;                                        //���ճ���Ϊ0
    while(overtime--)                                   //�ڹ涨����ʱ�ڼ��ڽ�����Ч֡���ж�
    {
        aos_msleep(100);                             //�ӳ�100ms          
        if(gs_Uart.ToBeRead(phyport) > 0)               //�ж��Ƿ��������յ�
        {
            ulen += gs_Uart.Read(
                                     phyport, 
                                     buffer + ulen, 
                                     256 - (ulen & 0xFF),
                                     1000);

            ulen &= 0xFF;                          //���ݳ��Ȳ�����256
            gs_GPIO.BlinkSet(GPIO_LED_485, 1, 2, 5);
                                                        //�ж�֡����Ч��                                         
//	            if(PST_Channel(port, buffer, ulen, &protocol) == PST_ERR_OK)
//	            {
//	                gs_GPIO.BlinkSet(GPIO_LED_485, 2, 0, 0);
//	                if(ulen > 0)
//	                {
//	                    gs_Uart.close(phyport);                             //�ͷŴ���
//	                    *length = ulen;
//	                    SYS_OK();
//	                }
//	            }
        }
        else
        {
            if(ulen > 0)//���ݽ�����ϣ���Ϊ��ȷ����ʲôЭ�飬����û���ж�֡Э��
            {
                gs_Uart.Close(phyport);                             //�ͷŴ���
                gs_Uart.Clear(phyport);
                *length = ulen;
                gs_GPIO.BlinkSet(GPIO_LED_485, 3, 0, 0);
                
                Netp_PowerMng(0, NETP_PORT_PWR_TRANS);
                SYS_OK();
	
            }
        }
    }
    gs_Uart.Close(phyport);                             //�ͷŴ���
    *length = 0;
    gs_GPIO.BlinkSet(GPIO_LED_485, 3, 0, 0);
    Netp_PowerMng(0, NETP_PORT_PWR_TRANS);
    
    return NETP_ERR_FT;                             //��ʱ�����ݻָ�
}

int Netp_PowerMng(uint8_t onoff, uint8_t flag)
{

    if(onoff)
    {
        gs_SysVar.netpTsk |= (1 << flag);
        gs_GPIO.GPO_Out(GPO_485_PWR, true);
    }
    else
    {
        gs_SysVar.netpTsk &= ~(1 << flag);
        if(gs_SysVar.netpTsk == 0)
        {
            if(gs_SysVar.mDGstt & HLV_STT_NENG)
                gs_GPIO.GPO_Out(GPO_485_PWR, false);
        }
    }


    return SYS_ERR_OK;
}

/************************************************************************
 * @function: Netp_Send
 * @����: �����˿�͸������(��������)
 * 
 * @����: 
 * @param: phyport �˿�
 * @param: buffer �շ�����
 * @param: len ���ͳ���
 * @param: overtime ��ʱʱ��, ��λ100ms
 * @param: ss ���ڿ�����
 * @param: fw ֡����
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2011-11-2)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 Netp_Send(uint8_t phyport, uint8* buffer, uint16* len, 
                    uint8 overtime, uart_config_t *ss, NetpFrameStr * frameStr)
{
    //scm_dlms_t* scm = &gs_scmDlms;
//	    SerialSets ss;
//	    uint8 phyport;
                                            //�˿�����
   /* if(gss_CDATATst[tstno].pt >= MCB_PORT_NUM)
    {
        return NETP_ERR_FT;
    }*/
//	    CDATATst * pt = 
//	    phyport = NETP_PORT_NO;
    //uint8_t p[4] = {0xFE, 0xFE, 0xFE, 0xFE};
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);           //��һ��

//	    ss.baudrate = NETP_PORT_BAUD;
//	    ss.databits = DataBits_8bits;
//	    ss.parit = Parit_N;
//	    ss.stopbits = StopBits_1;
                                            //�򿪴���
//	    gs_GPIO.GPO_Out(GPO_MBUS_PWR,false);
    gs_Uart.open(phyport, false);
    gs_Uart.Init(phyport, ss);
    gs_Uart.Clear(phyport);
    aos_msleep(100);
    //gs_Uart.Write(phyport, p, 4, 100);
    gs_Uart.Write(phyport, buffer, *len, 1000);   //��������

    *len = 0;                                            //���ճ���Ϊ0
    while(overtime--)                                   //�ڹ涨����ʱ�ڼ��ڽ�����Ч֡���ж�
    {
        aos_msleep(10);                             //�ж��Ƿ��������յ�
//	        if(gs_Uart.ToBeRead(phyport) > 0)
        {
           *len += gs_Uart.Read(
                           phyport, 
                           buffer + *len, 
                           256 - (*len & 0x3ff),
                           1000);
            *len &= 0x3ff;                                //������һ���ֽ����ڵ�
                                                        //�ж�֡����Ч�� 
            if(NETP_CheckFrame2(frameStr, buffer, len) == PST_ERR_OK)
            {    
                //�رմ���
                gs_GPIO.BlinkSet(GPIO_LED_485, 1, 2, 5);    //��
                gs_Uart.Close(phyport);
                aos_msleep(50);
                
                SYS_OK();
            }
        }
    }
    gs_Uart.Close(phyport);                             //�ͷŴ���
    gs_GPIO.BlinkSet(GPIO_LED_485, 3, 0, 0);  //��
    
    return NETP_ERR_FT;
}
#define CON_NETP_TST_LEN 16
#define CON_NETP_485_STT_BIT 28
uint8 Netp_Check(uint8_t port)
{
    uint8_t tstdat[CON_NETP_TST_LEN]={0x12,0x34,0x56,0x78,0x87,0x65,0x43,0x21,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
    uint8_t buff[CON_NETP_TST_LEN];
    uint8_t phyport = NETP_PORT_NO + port;
    uint8_t len = 0;
    uart_config_t ss;
    uint8_t overtime = 0xff;
    gs_GPIO.BlinkSet(GPIO_LED_485, 0, 0, 0);           //��һ��
    PentCfgToSS(0x18, &ss);

    LOG_DEBUG("check 485-%d start !!!\n", port+1);
    aos_msleep(1500 + port*500);
    gs_Uart.open(phyport, false);
    gs_Uart.Init(phyport, &ss);
    gs_Uart.Clear(phyport);
                                            
    gs_Uart.Write(phyport, tstdat, sizeof(tstdat), 1000);   //��������
    memset(buff, 0, CON_NETP_TST_LEN);

    while(overtime--) //�ڹ涨����ʱ�ڼ��ڽ�����Ч֡���ж�
    {
        aos_msleep(10);//�ж��Ƿ��������յ�
        
        if(gs_Uart.ToBeRead(phyport) > 0)
        {
            len += gs_Uart.Read(
                       phyport, 
                       buff+len, 
                       CON_NETP_TST_LEN-len,
                       1500);
            
            LOG_DEBUG("check 485-%d data:\n", port+1);
            LOG_DUMPHEX(LOG_LEVEL_DEBUG, "\ncheck:\n", buff, CON_NETP_TST_LEN);
            if(len >= CON_NETP_TST_LEN && 0 == memcmp(buff, tstdat,CON_NETP_TST_LEN))
            {
                gs_SysVar.terstt.lword &= ~(1 << (CON_NETP_485_STT_BIT + port));
                gs_GPIO.BlinkSet(GPIO_LED_485, 1, 2, 5);//��
                gs_Uart.Close(phyport);
                aos_msleep(50);
                
                LOG_DEBUG("check 485-%d  success!!!\n", port+1);
                SYS_OK();
            
            }
        }
    }
    gs_Uart.Close(phyport);//�ͷŴ���
    gs_GPIO.BlinkSet(GPIO_LED_485, 3, 0, 0);//��
    gs_SysVar.terstt.lword |= (1 << (CON_NETP_485_STT_BIT + port));
    
    LOG_DEBUG("check 485-%d  failed???\n", port+1);
    return NETP_ERR_FT;
}


//	uint8 Netp_Trans2(uint8* buffer, uint16* len, uint8 overtime, ST_Netp_FW * fw)
//	{
//	
//	}
uint8 Netp_Trans(uint8* buffer, uint16* len, uint8 overtime, uint8 nfrm)
{
    //scm_dlms_t* scm = &gs_scmDlms;
    SerialSets ss;
    uint8 phyport;
                                            //�˿�����
   /* if(gss_CDATATst[tstno].pt >= MCB_PORT_NUM)
    {
        return NETP_ERR_FT;
    }*/
//	    CDATATst * pt = 
    phyport = NETP_PORT_NO;
    //gs_GPIO.LED_BlinkSet(LED_485_T, 0, 0, 0);           //��һ��

    ss.baudrate = NETP_PORT_BAUD;
    ss.databits = DataBits_8bits;
    ss.parit = Parit_N;
    ss.stopbits = StopBits_1;
                                            //�򿪴���
//	    gs_GPIO.GPO_Out(GPO_MBUS_PWR,false);
    gs_Uart.open(phyport, false);
    gs_Uart.Init(phyport, &ss);
    gs_Uart.Clear(phyport);
                                            
    gs_Uart.Write(phyport, buffer, *len, 1000);   //��������

    *len = 0;                                            //���ճ���Ϊ0
    while(overtime--)                                   //�ڹ涨����ʱ�ڼ��ڽ�����Ч֡���ж�
    {
        aos_msleep(10);                             //�ж��Ƿ��������յ�
//	        if(gs_Uart.ToBeRead(phyport) > 0)
        {
           *len += gs_Uart.Read(
                           phyport, 
                           buffer + *len, 
                           1024 - (*len & 0x3ff),
                           1000);
            *len &= 0x3ff;                                //������һ���ֽ����ڵ�
                                                        //�ж�֡����Ч�� 
            if(PST_CheckFrame3(nfrm, buffer, len) == PST_ERR_OK)
            {    
                //�رմ���
                //scm->recerr = 0;
                gs_GPIO.BlinkSet(GPIO_LED_485, 2, 0, 0);    //��
                gs_Uart.Close(phyport);
                aos_msleep(50);
                
                SYS_OK();
            }
        }
    }
    gs_Uart.Close(phyport);                             //�ͷŴ���
//	    scm->recerr++;
//	    if(scm->recerr > 20)    //��������޷��յ���ȷ������֡����������MBUSģ��
//	    {
//	        scm->recerr = 0;
//	        gs_GPIO.GPO_Out(GPO_485_PWR,true);//�ر�MBUS��Դ
//	        aos_msleep(350);
//	        gs_GPIO.GPO_Out(GPO_485_PWR,false);//��MBUS��Դ
//	        aos_msleep(350);
//	    }
    gs_GPIO.BlinkSet(GPIO_LED_485, 3, 0, 0);  //��
    
    return NETP_ERR_FT;
}







/************************************************************************
 * @function: Netp_ReadBG
 * @����: �����˿ڲ������ݶ�ȡ����
 * 
 * @����: 
 * @param: buffer �������ݻ���
 * @param: di �����ʶ
 * @param: length �������ݵ�Ԥ�Ƴ���
 * @param: tstno �������
 * 
 * @����: 
 * @return: uint8  NETP_ERR_OK,NETP_ERR_NT(�лظ�,����֧��),NETP_ERR_FT
 * @˵��: 
 * @����: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 Netp_ReadBG(uint8* buffer, uint32 di, uint16 length, uint8 tstno)
//	{
//	    uint8 tmp[256];
//	    uint16 len;
//	    uint8 err;
//	                                            //���
//	    len = BG_PackRead(tmp, di, tstno);
//	    if(len == 0)
//	    {
//	        return NETP_ERR_FT;
//	    }
//	                                            //���ݴ���ͨѶ
//	    if(Netp_Trans(tmp, &len, 40, PST_FRM_BG_NO) != NETP_ERR_OK)
//	    {
//	        return NETP_ERR_FT;
//	    }
//	                                            //��ȡ��������
//	    len = sizeof(tmp);
//	    err = BG_ExtractData(tmp, &len, di);
//	    if(err != NETP_ERR_OK)
//	    {
//	        return err;
//	    }
//	
//	    if(len > length)                                    //ʵ�ʶ�ȡ���ȴ������󳤶�
//	    {
//	        MoveBuffer(tmp, buffer, length);
//	    }
//	    else
//	    {                                                   //ת�ƶ�ȡ��������
//	        memset(buffer, 0, length);
//	        MoveBuffer(tmp, buffer, len);
//	    }
//	
//	    SYS_OK();
//	}
//	


//	uint8 Netp_BGRead(uint8* buffer, uint32 di, uint16 length, uint8 tstno)
//	{
//	    uint8 err;
//	    for(uint8 uc_i = 0; uc_i < NETPREADCNT; uc_i++)
//	    {
//	        err = Netp_ReadBG(buffer, di, length, tstno);
//	        if((err == NETP_ERR_OK) || (err == NETP_ERR_NT))
//	        {
//	            break;
//	        }
//	    }
//	    return err;
//	}




/************************************************************************
 * @function: Netp_ReadXBG
 * @����: �����˿ڲ������ݶ�ȡ����
 * 
 * @����: 
 * @param: buffer �������ݻ���
 * @param: di ���ݱ�ʶ
 * @param: length �������ݵ�Ԥ�Ƴ���
 * @param: tstno �������
 * 
 * @����: 
 * @return: uint8  NETP_ERR_OK,NETP_ERR_NT(�лظ�,����֧��),NETP_ERR_FT
 * @˵��: 
 * @����: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 Netp_ReadXBG(uint8* buffer, uint32 di, uint16 length, uint8 tstno)
//	{
//	    uint8 tmp[256];
//	    uint16 len;
//	    uint8 err;
//	                                            //���
//	    len = XBG_PackRead(tmp, di, tstno);
//	    if(len == 0)
//	    {
//	        return NETP_ERR_FT;
//	    }
//	                                            //���ݴ���ͨѶ
//	    if(Netp_Trans(tmp, &len, 40, PST_FRM_BG_NO) != NETP_ERR_OK)
//	    {
//	        return NETP_ERR_FT;
//	    }
//	                                            //��ȡ��������
//	    len = sizeof(tmp);
//	    err = XBG_ExtractData(tmp, &len, di);
//	    if(err != NETP_ERR_OK)
//	    {
//	        return err;
//	    }
//	
//	    if(len > length)                                    //ʵ�ʶ�ȡ���ȴ������󳤶�
//	    {
//	        MoveBuffer(tmp, buffer, length);
//	    }
//	    else
//	    {                                                   //ת�ƶ�ȡ��������
//	        memset(buffer, 0, length);
//	        MoveBuffer(tmp, buffer, len);
//	    }
//	
//	    SYS_OK();
//	}


//	uint8 Netp_XBGRead(uint8* buffer, uint32 di, uint16 length, uint8 tstno)
//	{
//	    uint8 err;
//	    for(uint8 uc_i = 0; uc_i < NETPREADCNT; uc_i++)
//	    {
//	        err = Netp_ReadXBG(buffer, di, length, tstno);
//	        if((err == NETP_ERR_OK) || (err == NETP_ERR_NT))
//	        {
//	            break;
//	        }
//	    }
//	    return err;
//	}





/************************************************************************
 * @function: Netp_Read
 * @����: �����˿ڶ����ݴ���
 * 
 * @����: 
 * @param: buffer �������ݻ���
 * @param: di �����ı�ʶ
 * @param: length �������ݵĳ���
 * @param: tstno �������
 * 
 * @����: 
 * @return: uint8  NETP_ERR_OK,NETP_ERR_NT(�лظ�,����֧��),NETP_ERR_FT
 * @˵��: 
 * @����: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 Netp_Read(uint8* buffer, uint32 di, uint16 length, uint8 tstno)
//	{
//	     switch(gss_CDATATst[tstno].pf)          //����ͨѶ��Լ�ֱ���
//	    {
//	       case TST_PFID_DLT97:                          //����1997
//	            return Netp_BGRead(buffer, di, length, tstno);
//	        case TST_PFID_DLT07:                          //����2007
//	            return Netp_XBGRead(buffer, di, length, tstno);
//	        default:
//	            return NETP_ERR_FT;
//	    }
//	    //return Netp_DLMSRead(buffer, length, tstno, NETP_TYPE_GET, inbuf)
//	}








const uint8_t netp_check[4]={0xa5,0xa5,0xa5,0xa5};

/************************************************************************
 * @function: Netp_PreInit
 * @����: ����˿ڳ�ʼ��
 * 
 * @����: 
 * @param: index �˿ڱ��
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_PreInit(uint8 index)
{
    uint8_t temp[4];

    if(index >= MCB_PORT_NUM)
    {
        return;
    }
                                        //�������״̬
    ClearBuffer((uint8*)&gss_NetpPortStt[index], sizeof(NETPSTR));
                                        //�������ͳ��
    ClearBuffer((uint8*)gs_NetpReadStt, sizeof(gs_NetpReadStt));
    gs_GPIO.GPO_Out(GPO_485_PWR,false);
    gs_PstPara.cp_interval=300;

    extern const ST_Netp_FW* const gss_NetpFrameFw[];
    memset(temp,0,4);
    if(GD_Para_RW(NETP_CHECK, temp, 4, false) == SYS_ERR_OK)
    {
#ifndef DEBUG    
        if(memcmp(netp_check, temp, 4) != 0)
#endif            
        {
            for(int i = 0; i < Netp_Get_FW_num(); i++)
            {
                Netp_Register_Set(gss_NetpFrameFw[i]);
            }    
            GD_Para_RW(NETP_CHECK, netp_check, 4, true);
        }
    }
//	    Netp_MtUpdataParaInit();
}












/************************************************************************
 * @function: InitNetpRB
 * @����: ��������ǰ��״̬����λ����
 * 
 * @����: 
 * @param: rb ״̬�ṹ��
 * @����: 
 * @˵��: 
 * @����: yzy (2011-3-14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void InitNetpRB(NETPSTR* rb)
//	{
//	    NETPRD* tbl;
//	    uint8 method;                           //����Ƿ���Ҫ����
//	
//	    rb->idx = 0;
//	    rb->prio = 1;                           //Ĭ���ޱس���
//	
//	    ClearBuffer(rb->bmp_m, SIZE_NETPBMP);
//	    ClearBuffer(rb->bmp_n, SIZE_NETPBMP);
//	    for(uint8 uc_i = 0; uc_i < rb->tbllen; uc_i++)
//	    {
//	        tbl = rb->tbl + uc_i;
//	        if(tbl->patr & NTRD_PATR_MUST)
//	        {
//	            STR_SetBit(rb->bmp_m, uc_i);
//	            rb->prio = 0;                   //���ڱس���
//	        }
//	        else
//	        {                                   //���������жϸñ�ʶ�Ƿ���Ҫ����
//	            method = IsNetpDIRead(rb->tstno, tbl);
//	            if((method == 0) || (method == 2))
//	            {
//	                continue;
//	            }
//	                                            //���ñ�ʶ�ڴ�����λͼ�м��Ա��
//	            if(tbl->zhtp == NTRD_ZHTP_MUFL)
//	            {
//	                if(tbl->zhid == 0xFF || tbl->zhid == 0)
//	                {                           //�鳭/��
//	                    STR_SetBit(rb->bmp_n, uc_i);
//	                }
//	                else if(gss_CDATATst[rb->tstno].fl > 1)
//	                {                           //�����
//	                    if(tbl->zhid <= gss_CDATATst[rb->tstno].fl)
//	                    {
//	                        STR_SetBit(rb->bmp_n, uc_i);
//	                    }
//	                }
//	            }
//	            else
//	            {
//	                STR_SetBit(rb->bmp_n, uc_i);
//	            }
//	        }
//	    }
//	}


/************************************************************************ 
**������������
*************************************************************************/
//	typedef void (*ftype_ntrd)(uint8*, NETPSTR*, uint8, uint8);


/************************************************************************
**��������: ReadDataToNetpRam
**�� ��: ��ͨ��������ͨ����ȡ�����ݷ����ڴ�����
**
**�� ��: nt ��Ϣ�ṹ�� 
**       buffer ��ȡ������
**       cstrc Ҫ������ڴ�
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��:  yzy
**�� ��:  11-01-25
*************************************************************************/
//	void ReadDataToNetpRam(uint8* buffer, NETPSTR* rb)
//	{
//	    uint32 val;
//	    uint32 mask;
//	    uint8* pbuf = buffer;
//	    NETPRD* nt = rb->tbl + rb->idx;
//	    uint8  issign;
//	                                            //ѭ�������ȡ������
//	    for(uint8 uc_j = 0; uc_j < nt->zhnum; uc_j++)
//	    {
//	        if(nt->datr & NTRD_DATR_SPEC)       //���⴦��
//	        {
//	            (*(ftype_ntrd)nt->func)(pbuf + nt->len * uc_j, rb, rb->tstno, uc_j);
//	            continue;
//	        }
//	
//	        if(nt->len == 1)                    //���ֽ�����
//	        {
//	            val = *(pbuf + uc_j);
//	        }
//	        else if(nt->len == 2)               //���ֽ�����
//	        {
//	            val = ByteToInt(pbuf + 2 * uc_j);
//	        }
//	        else if(nt->len == 3)               //���ֽ�����
//	        {
//	            val = ByteToInt24(pbuf + 3 * uc_j);
//	        }
//	        else if(nt->len == 4)               //���ֽ�����
//	        {
//	            val = ByteToLong(pbuf + 4 * uc_j);
//	        }
//	        else                                //ֱ��ת�Ʋ��˳�
//	        {
//	            MoveBuffer(pbuf, rb->data + nt->add, nt->len * nt->zhnum);
//	            break;
//	        }
//	                                            //��������������ȥ������
//	        CLEAR_FLAG(issign);
//	        if(nt->datr & NTRD_DATR_INSG)
//	        {
//	            mask = 1 << ((nt->len << 3) - 1);
//	            if(val & mask)
//	            {
//	                val &= ~mask;
//	                SET_FLAG(issign);
//	            }
//	        }
//	                                            //��Ҫװ����HEX
//	        if(nt->datr & NTRD_DATR_THEX)
//	        {
//	            if(IsBcd(val))
//	            {
//	                val = LongBcdToHex(val);
//	            }
//	            else
//	            {
//	                val = 0;
//	            }
//	        }
//	                                            //����С��λ
//	        if(nt->datr & NTRD_DATR_PDIV)
//	        {
//	            val /= guls_Do10Value[nt->dgt];
//	        }                                   //��չС��λ
//	        else if(nt->datr & NTRD_DATR_PMUL)
//	        {
//	            val *= guls_Do10Value[nt->dgt];
//	        }
//	                                            //�����������Ҫ���Ϸ���
//	        if(nt->datr & NTRD_DATR_OUSG)
//	        {
//	            if(issign)
//	            {
//	                val = -val;
//	            }
//	        }
//	
//	        if(nt->datr & NTRD_DATR_BEXT)
//	        {                                   //�ֽ���չ��4�ֽ�,2->4,3->4
//	            LongToByte(rb->data + nt->add + 4 * uc_j, val);
//	        }
//	        else                                //����ֱ��ת��
//	        {
//	            MoveBuffer(
//	                     (uint8*)&val, 
//	                     rb->data + nt->add + nt->len * uc_j,
//	                     nt->len
//	                     );
//	        }
//	    }
//	}






/************************************************************************
 * @function: Netp_GetTstData
 * @����: ���սṹ����Ϣ��������,��������.
 * 
 * @����: 
 * @param: rb 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2011-3-18)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 Netp_GetTstData(NETPSTR* rb)
//	{
//	    uint8   buffer[256];
//	    NETPRD* nt;
//	    uint16  index;
//	    uint8   err;
//	    uint8   errcnt = 0;
//	    uint8   flag = 0;                       //��־�Ա�ʶ�Ƿ񳭶��ɹ�
//	                                            //��֧���ն˲�����
//	    _IF_TRUE_RETURN_Z((rb->tstno == 0) || (rb->tstno >= CORE_TST_NO), NETP_ERR_FT);
//	
//	    InitNetpRB(rb);
//	    while(1)
//	    {                                       //��ȡ������ʶ
//	        if(rb->prio == 0)
//	        {
//	            index = STR_GetThisNextSetBit(rb->bmp_m, SIZE_NETPBMP, rb->idx);
//	            if(index == 0xFFFF)
//	            {                               //�س���ʧ��,�ж�Ϊ����ʧ�ܲ��˳�
//	                _IF_TRUE_RETURN_Z(flag == 0, SYS_ERR_FT);
//	                rb->idx = 0;
//	                rb->prio++;
//	                continue;
//	            }
//	        }
//	        else
//	        {
//	            index = STR_GetThisNextSetBit(rb->bmp_n, SIZE_NETPBMP, rb->idx);
//	            if(index == 0xFFFF)
//	            {
//	                break;
//	            }
//	        }
//	
//	        rb->idx = index;
//	        nt = rb->tbl + rb->idx;
//	        STR_ClrBit(rb->bmp_m, rb->idx);
//	        STR_ClrBit(rb->bmp_n, rb->idx);
//	                                            //����������
//	        err = Netp_Read(buffer, nt->di, sizeof(buffer), rb->tstno);
//	        if(err == NETP_ERR_OK)              //�����ظ�
//	        {
//	            flag = 1;
//	            ReadDataToNetpRam(buffer, rb);
//	            for(uint8 uc_i = 0; uc_i < nt->zhnum; uc_i++)
//	            {
//	                STR_SetBit(rb->eft, nt->sbit + uc_i);
//	                if(nt->patr & NTRD_PATR_SKIP)//������Ҫ�õ�λ
//	                {
//	                    STR_ClrBit(rb->bmp_m, rb->idx + 1 + uc_i);
//	                    STR_ClrBit(rb->bmp_n, rb->idx + 1 + uc_i);
//	                }
//	            }
//	            errcnt = 0;
//	        }
//	        else if(err == NETP_ERR_NT)          //�ظ��ܾ�֡
//	        {
//	            STR_ClrBit(rb->eft, nt->sbit);
//	            errcnt = 0;
//	        }
//	        else                                //ͨѶ��ͨ
//	        {
//	            STR_ClrBit(rb->eft, nt->sbit);
//	            if(errcnt++ > 5)                //������γ���ʧ��,�ж�Ϊ����ʧ�ܲ��˳�
//	            {
//	                return NETP_ERR_FT;
//	            }
//	        }
//	        rb->idx++;
//	    }
//	    SYS_OK();
//	
//	}


/************************************************************************
 * @function: gdw_Check97LastDayData
 * @����: ȷ��97�������ն������ݵĴ���
 * 
 * @����: 
 * @param: tstno ������� 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2011-10-7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	uint8 gdw_Check97LastDayData(uint8 tstno)
//	{
//	//	    S_DAYDATA_SEG0 daydata;
//	//	    bool flag;
//	//	                                                //97��,�ҳ���ɹ�
//	//	    if((gss_CDATATst[tstno].pf != TST_PFID_DLT07) && (gss_ALRTStt[tstno].ew1.trn == 0))
//	//	    {                                           //��ȡ���ն�������
//	//	        if(gdw_ReadDayData(gs_SysLastDayTime.YMDhms, tstno, 0, (uint8*)&daydata) != SYS_ERR_OK)
//	//	        {
//	//	            return SYS_ERR_FT;
//	//	        }
//	//	        
//	//	        flag = STR_IsBitSet(daydata.seg.eft, EFT_DAY_WP);
//	//	        if(!flag)                               //�������ݲ�����
//	//	        {
//	//	            gdw_SaveTstDayData(tstno, 0);       //ʹ��ʵʱ������
//	//	        }
//	//	    }
//	    SYS_OK();
//	}
//	
//	#if (GDW_07DIN_EN > 0)
//	
//	
//	/************************************************************************
//	 * @function: gdw_Check07LastDayData
//	 * @����: ʹ��07�������������ն���Ƚ�,����һ���ѱ�������Ϊ׼
//	 * 
//	 * @����: 
//	 * @param: tstno ������� 
//	 * 
//	 * @����: 
//	 * @return: uint8  
//	 * @˵��: �˴��ɸ���Ϊ����������������.
//	 * @����: yzy (2011-10-7)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	uint8 gdw_Check07LastDayData(uint8 tstno)
//	{
//	    NETPSTR netpstr;
//	    S_Tst07ExtraData tstdata;           //��ű������ʷ�ն�������
//	    S_DAYDATA_SEG0 daydata;             //����ն�����ʷ�ն�������
//	    const S_DATASEGINFO* segstr;
//	    uint8 YMDhm[5];
//	    uint8 YMDhm_l[5];                   //��һ��ʱ��
//	    uint32 addr;
//	    uint16 bidx;
//	    uint8 flag = 0;                     //��־�Ƿ��滻ԭ����
//	
//	    if((gss_CDATATst[tstno].pf == TST_PFID_DLT07) && (gss_ALRTStt[tstno].ew1.trn == 0))
//	    {
//	                                        //����ǰ��׼������
//	        ClearBuffer((uint8*)&tstdata, sizeof(tstdata));
//	        netpstr.data = (uint8*)&tstdata;
//	        netpstr.eft = tstdata.eft;
//	        netpstr.tstno = tstno;
//	        netpstr.tbl = (NETPRD*)gss_NetpXgExtra;
//	        netpstr.tbllen = LEN_XGEXTRA;
//	
//	        if(Netp_GetTstData(&netpstr) == SYS_ERR_OK)
//	        {
//	            tstdata.rtime = gs_SysTime;
//	                                        //���ڶ���ʱ�����Ҫ��
//	            if(STR_IsBitClr(tstdata.eft, EFT_EXT_TSD))
//	            {
//	                return SYS_ERR_FT;
//	            }
//	                                        //˳��ת��
//	            ByteArrayBcdToHex(tstdata.Tsd, YMDhm, 5);
//	            InvBuffer(YMDhm, 5);
//	            MoveBuffer(YMDhm, YMDhm_l, 5);
//	            StepDays(YMDhm_l, -1);      //��ǰ����һ��.
//	                                        //���ݱ��ڶ���ʱ���ȡ��Ӧ���ն˶�������
//	            gdw_ReadDayData(YMDhm_l, tstno, 0, (uint8*)&daydata);
//	                                        //�жϱ�����ն���ʱ���Ƿ�Ϊ�ն˵���ʱ��
//	            //if(CmpBuffer(YMDhm, gs_SysTime.YMDhms, 3))
//	            //{
//	            //    return SYS_ERR_FT;
//	            //}
//	                                       //׼���ṹ������
//	            if(STR_IsBitSet(tstdata.eft, EFT_EXT_WPD))
//	            {
//	                if(CmpBuffer((uint8*)tstdata.Wpd, (uint8*)daydata.seg.Wp, sizeof(daydata.seg.Wp)) || STR_IsBitClr(daydata.seg.eft, EFT_DAY_WP))
//	                {
//	                    MoveBuffer((uint8*)tstdata.Wpd, (uint8*)daydata.seg.Wp, sizeof(daydata.seg.Wp));
//	                    STR_SetBit(daydata.seg.eft, EFT_DAY_WP);
//	                    flag = 1;
//	                }
//	            }
//	
//	            if(STR_IsBitSet(tstdata.eft, EFT_EXT_WND))
//	            {
//	                if(CmpBuffer((uint8*)tstdata.Wnd, (uint8*)daydata.seg.Wn, sizeof(daydata.seg.Wn)) || STR_IsBitClr(daydata.seg.eft, EFT_DAY_WN))
//	                {
//	                    MoveBuffer((uint8*)tstdata.Wnd, (uint8*)daydata.seg.Wn, sizeof(daydata.seg.Wn));
//	                    STR_SetBit(daydata.seg.eft, EFT_DAY_WN);
//	                    flag = 1;
//	                }
//	            }
//	                                        //�����滻���ݺ�Ľṹ��
//	            if(flag)
//	            {                           //ADDRESS(DIVIDED BY TST)
//	                BuildTimeLabel(TTYPE_YMDHMS, YMDhm, &daydata.seg.stime);
//	                daydata.seg.rtime = gs_SysTime;
//	                                        //���浽��һ���ն���
//	                addr = _DayDataAddr(YMDhm_l, tstno, 0, &segstr, &bidx);
//	                if(addr == 0xffffffff)
//	                {
//	                    return SYS_ERR_FT;
//	                }
//	                WriteFlashWithCRC(DB_DAY, (uint8*)&daydata, segstr->seglen, addr);
//	                STR_ClrFlashBit(DB_DAY, 0, bidx, 0xffff);//0��Ч,1��Ч
//	            }
//	        }
//	    }
//	    SYS_OK();
//	}
//	
//	#endif









/************************************************************************
 * @function: Netp_ReadTstRealData
 * @����: ��ȡ��ȡʵʱ����
 * 
 * @����: 
 * @param: port �˿ں�(0-!)
 * @����: 
 * @˵��: 
 * @����: yzy (2011-11-2)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Netp_ReadTstRealData(uint8 port)
//	{
//	    _IF_TRUE_RETURN_VOID(port >= MCB_PORT_NUM);
//	                                            //�ⲿ������ѭ����ȡ����
//	    for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++)
//	    {
//	        if(gs_InpExtracb[port].gbbreak)
//	        {
//	            return;
//	        }
//	                                            //Ҫ��:1.��������Ч,2.485��
//	        if((gss_CDATATst[uc_i].ef != 0x01) || (gss_CDATATst[uc_i].tf != 0x01) || (gss_CDATATst[uc_i].pt != port))
//	        {
//	            continue;
//	        }
//	
//	        //HB_RetLive();                  //�����ڼ���������
//	                                            //��ֵ�������ݽṹ��
//	        NETPSTR netpstr;
//	        S_TstRealData tstdata;
//	        ClearBuffer((uint8*)&tstdata, sizeof(tstdata));
//	        netpstr.data = (uint8*)&tstdata;
//	        netpstr.eft = tstdata.eft;
//	        netpstr.tstno = uc_i;
//	        if(gss_CDATATst[uc_i].pf == TST_PFID_DLT97)      //645-1997
//	        {
//	            netpstr.tbl = (NETPRD*)gss_NetpBgTstData;
//	            netpstr.tbllen = LEN_BGTSTDATA;
//	        }
//	        else if(gss_CDATATst[uc_i].pf == TST_PFID_DLT07) //645-2007
//	        {
//	            netpstr.tbl = (NETPRD*)gss_NetpXgTstData;
//	            netpstr.tbllen = LEN_XGTSTDATA;
//	        }
//	
//	        if(Netp_GetTstData(&netpstr) != SYS_ERR_OK)
//	        {
//	            _RefreshTrnFlag(uc_i, false);
//	
//	            //���ʵʱ������Ч���
//	            ClearBuffer(gss_TstRealData[uc_i].eft, sizeof(gss_TstRealData[uc_i].eft));
//	            if(gss_TstRealData[uc_i].errcnt < 0xFF)
//	            {
//	                gss_TstRealData[uc_i].errcnt++;
//	            }
//	            gss_TstRealData[uc_i].rtime = gs_SysTime;
//	        }
//	        else
//	        {
//	            _RefreshTrnFlag(uc_i, true);
//	
//	            //�����¼�����ж�  
//	            //EVT_ERC12_Proc(&tstdata, uc_i); //ʱ�䳬���ж�
//	            //EVT_ERC27_28_29_30_Proc(&tstdata, uc_i);//�����쳣�ж�
//	            
//	            //�洢ʵʱ����
//	            tstdata.errcnt = gss_TstRealData[uc_i].errcnt;
//	            MoveBuffer((uint8*)&tstdata, (uint8*)&gss_TstRealData[uc_i], sizeof(S_TstRealData));
//	            gss_TstRealData[uc_i].rtime = gs_SysTime;
//	
//	            //��ѹͳ��
//	            //STAT_CBVoltage(uc_i, gs_InpExtracb[port].netitv);
//	        }
//	    }
//	
//	    
//	}



/************************************************************************
 * @function: Netp_ReadTstDemandData
 * @����:  ����FLASH��������(Ϊ���õ�ʱ����ʱȥ����,�������¶���)
 * 
 * @����: 
 * @param: port 
 * @����: SYS_ERR_FT, SYS_ERR_OK
 * @˵��: 
 * @����: yzy (2014/3/4)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Netp_ReadTstDemandData(uint8 port)
//	{
//	    uint8 uc_i;
//	    S_TstDmdData* pdata = &gs_tstdmd[port];
//	
//	    _IF_TRUE_RETURN_VOID(port >= MCB_PORT_NUM);
//	    
//	    for(uc_i = 1; uc_i < CORE_TST_NO; uc_i++)
//	    {
//	        if(gs_InpExtracb[port].gbbreak)
//	        {
//	            return;
//	        }
//	                                            //Ҫ��:1.��������Ч,2.485��
//	//	        if((gss_CDATATst[uc_i].ef != 0x01) || (gss_CDATATst[uc_i].pt != port) || (gss_ALRTStt[uc_i].ew1.trn == 0x01))
//	//	        {
//	//	            continue;
//	//	        }
//	                                            //Ҫ��:2.485�򽻲ɲ�����
//	        if((gss_CDATATst[uc_i].tf != 0x01) && (gss_CDATATst[uc_i].tf != 0x05))
//	        {
//	            continue;
//	        }
//	
//	        //HB_RetLive();                              //�����ڼ���������
//	                                            //��ֵ�������ݽṹ��
//	        NETPSTR netpstr;
//	        ClearBuffer((uint8*)pdata, sizeof(S_TstDmdData));
//	        netpstr.data = (uint8*)pdata;
//	        netpstr.eft = pdata->eft;
//	        netpstr.tstno = uc_i;
//	        if(gss_CDATATst[uc_i].pf == TST_PFID_DLT97)                  //645-1997
//	        {
//	            netpstr.tbl = (NETPRD*)gss_NetpBgTstDemand;
//	            netpstr.tbllen = LEN_BGTSTDEMAND;
//	        }
//	        else if(gss_CDATATst[uc_i].pf == TST_PFID_DLT07)             //645-2007
//	        {
//	            netpstr.tbl = (NETPRD*)gss_NetpXgTstDemand;
//	            netpstr.tbllen = LEN_XGTSTDEMAND;
//	        }
//	        else
//	        {
//	            continue;
//	        }
//	                                            //��ȡ������
//	        if(Netp_GetTstData(&netpstr) == SYS_ERR_OK)
//	        {
//	            MoveBuffer(gs_SysTime.YMDhms, pdata->ymdhm, 5);  
//	            //SA_REALDATA_Write((uint8*)pdata, uc_i , 1);
//	        }
//	    }
//	}



/************************************************************************
 * @function: Netp_ReadEvtRealData
 * @����:  �����¼���Ҫ��ʵʱ����(Ϊ���õ�ʱ����ʱȥ����,�������¶���)
 * 
 * @����: 
 * @param: 
 * @����: SYS_ERR_FT, SYS_ERR_OK
 * @˵��: 
 * @����: zhenggb (2013-06-05)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	void Netp_ReadEvtRealData(uint8 port)
//	{
//	    uint8 uc_i;
//	
//	//	    S_EvtRealData* levtreal;                            //�����ʷֵ
//	//	    uint8 buffer[SA_FLASH_REALDATA_ITMBABY_BYTE];       //�����ʷֵ
//	//	    levtreal = (S_EvtRealData*)buffer;
//	//	    S_EvtRealData* pdata = &gs_TstEvtData[port];        //ʵʱ��ȡ���ݴ�ŵĻ���
//	//	
//	//	    _IF_TRUE_RETURN_VOID(port >= MCB_PORT_NUM);
//	//	    
//	//	
//	//	    for(uc_i = 1; uc_i < CORE_TST_NO; uc_i++)
//	//	    {
//	//	        if(gs_InpExtracb[port].gbbreak)
//	//	        {
//	//	            return;
//	//	        }
//	//	                                                    //Ҫ��:1.��������Ч,2.485��
//	//	        if((gss_CDATATst[uc_i].ef != 0x01) || (gss_CDATATst[uc_i].pt != port) || (gss_ALRTStt[uc_i].ew1.trn))
//	//	        {
//	//	            continue;
//	//	        }
//	//	                                                    //Ҫ��:2.485�򽻲ɲ�����
//	//	        if((gss_CDATATst[uc_i].tf != 0x01) && (gss_CDATATst[uc_i].tf != 0x05))
//	//	        {
//	//	            continue;
//	//	        }
//	//	
//	//	        //HB_RetLive();                               //�����ڼ���������
//	//	                                                        //��ֵ�������ݽṹ��
//	//	        NETPSTR netpstr;
//	//	        ClearBuffer((uint8*)pdata, sizeof(S_EvtRealData));
//	//	        netpstr.data = (uint8*)pdata;
//	//	        netpstr.eft =  pdata->eft;
//	//	        netpstr.tstno = uc_i;
//	//	
//	//	        if(gss_CDATATst[uc_i].pf == TST_PFID_DLT97)                  //645-1997
//	//	        {
//	//	            netpstr.tbl = (NETPRD*)gss_NetpBgEvtData;
//	//	            netpstr.tbllen = LEN_BGEVTDATA;
//	//	        }
//	//	        else if(gss_CDATATst[uc_i].pf == TST_PFID_DLT07)             //645-2007
//	//	        {
//	//	            netpstr.tbl = (NETPRD*)gss_NetpXgEvtData;
//	//	            netpstr.tbllen = LEN_XGEVTDATA;
//	//	        }
//	//	        else
//	//	        {
//	//	            continue;
//	//	        }
//	//	                                            //��ȡ������
//	//	        if(Netp_GetTstData(&netpstr) == SYS_ERR_OK)
//	//	        {
//	//	//	            if(SA_REALDATA_Read(buffer, uc_i, 0) == SYS_ERR_OK)
//	//	//	            {
//	//	//	                EVT_ERC8_Proc((uint8*)levtreal,     (uint8*)pdata, uc_i);    //�����¼��ж�
//	//	//	                EVT_ERC13_33_Proc((uint8*)levtreal, (uint8*)pdata, uc_i);
//	//	//	                EVT_ERC37_38_Proc((uint8*)levtreal, (uint8*)pdata, uc_i);
//	//	//	                EVT_ERC40_41_Proc((uint8*)levtreal, (uint8*)pdata, uc_i);
//	//	//	            }
//	//	//	            MoveBuffer(gs_SysTime.YMDhms, pdata->ymdhm , 5); 
//	//	//	            SA_REALDATA_Write((uint8*)pdata, uc_i, 0);    
//	//	        }
//	//	    }
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: Netp_ReadExDayData
//	 * @����: ����ʷ���ݶ����ȡ
//	 * @����: 
//	 * @����: 
//	 * @˵��: 1.��֤07����������Ϊ������ 2.�ٴ��ж������ն������ݴ���,�������ڲ���
//	 * @����: yzy (2011-10-7)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void Netp_ReadExDayData(uint8 port)
//	{
//	    for(uint8 uc_i = 1; uc_i < CORE_TST_NO; uc_i++)
//	    {
//	        if(gs_InpExtracb[port].gbbreak)
//	        {
//	            return;
//	        }
//	        
//	        if((gss_CDATATst[uc_i].ef == 0) || (gss_CDATATst[uc_i].pt != port))
//	        {
//	            continue;
//	        }
//	        
//	        //gdw_Check97LastDayData(uc_i);       //���������ݲ�����,�����ʵʱ���ݶ���
//	//#if (GDW_07DIN_EN > 0)               
//	//        gdw_Check07LastDayData(uc_i);       //ʱ�Ӻ˶�07�����ն�������
//	//#endif
//	    }
//	}

/************************************************************************
 * @function: Netp_InitData
 * @����: �������³���
 * 
 * @����: 
 * @param: index ����˿�����
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_InitData(uint8 index)
{
//	    if(gs_InpExtracb[index].gbbreak)
//	    {
//	        return;
//	    }
    if(gs_SysVar.mDGstt & HLV_STT_LENG)
    {
        gs_SysVar.mLPstt &= ~HLV_LPTASK_CP;
        return;
    }
    
    //LOG_DEBUG("~~~~~~~~~~~~~port[%d] cp wait start ~~~~~~~", index);
//	    if(gs_SysVar.mDGstt & HLV_STT_NENG)
//	    {
//	        aos_msleep(5000);
//	    }    
    //LOG_DEBUG("~~~~~~~~~~~~~port[%d] cp going start ~~~~~~~", index);

                                        //����ǰͳ��һ��
    void Netp_StatisticProc(uint8 index);
    Netp_StatisticProc(index);
    
    gs_NetpReadStt[index].BSTOP = 0;
    gs_NetpReadStt[index].start = gs_SysTime;
    
    Netp_ReadCureData(index);          

//	    Netp_ReadTstRealData(index);        //��ȡʵʱ����
    //Netp_ReadExDayData(index);          //��ȡ�ն��Ჹ������
    //Netp_ReadTstDemandData(index);      //��ȡ��������
    //Netp_ReadEvtRealData(index);        //��ȡͳ������


    Netp_ReadFinish(index);


    gs_NetpReadStt[index].end = gs_SysTime;
    gs_NetpReadStt[index].BSTOP = 1;
    //LOG_DEBUG("port[%d] CP Finish note slv to Report \n", index);
//	    uint8_t msg[2];
//	    msg[0] = MSG_NETP_FINISH;
//	    msg[1] = index;
//	    extern kbuf_queue_t gs_TKSlvQueue;
//	    kstat_t t =  gs_OS.Message_Send(&gs_TKSlvQueue, msg,2);
    JudgeTaskFlag(index);
    Netp_StatisticProc(index);          //�������ͳ��һ��
}

/************************************************************************
 * @function: Netp_StatisticProc
 * @����: ����˿�״̬ͳ��
 * 
 * @����: 
 * @param: index 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/7)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_StatisticProc(uint8 index)
{
    uint8 num_mt;
    uint8 num_fl;
    uint8 fl_idx;
    uint8 fl_list[NUM_TSTFAILREC];

    if(index >= MCB_PORT_NUM)
    {
        return;
    }
	
    //��ʼ��
    num_mt = 0;
    num_fl = 0;
    fl_idx = 0;
    ClearBuffer((uint8*)fl_list, sizeof(fl_list));
	
    //ͳ��
    for(uint8 uc_i = 0; uc_i < CORE_TST_NO; uc_i++)
    {
        if((gss_CDATATst[uc_i].pt == index))
        {
            num_mt++;                           //�������ۼ�
            if(gss_ALRTStt[uc_i].ew1.trn & 0x01)
            {
                num_fl++;                       //����ʧ�������ۼ�
                                                //��¼ʧ�ܵĲ������
                if(fl_idx < NUM_TSTFAILREC)
                {
                    fl_list[fl_idx] = uc_i;
                    fl_idx++;
                }
//	                gs_SysVar.terstt.lword &= ~(0x1 << (16 + uc_i));
            }
            else
            {
//	                gs_SysVar.terstt.lword |= (0x1 << (16 + uc_i));
            }
        }
    }
	
    gs_NetpReadStt[index].num_mt = num_mt;
    gs_NetpReadStt[index].num_fl = num_fl;
    MoveBuffer((uint8*)fl_list, (uint8*)gs_NetpReadStt[index].fl_list, sizeof(fl_list));
}

//===========�洢=========================










/************************************************************************
 * @function: Netp_StMonProc
 * @����: ����������
 * 
 * @����: 
 * @param: cbport ����˿ں�
 * @����: 
 * @˵��: 
 * @����: yzy (2011-9-20)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_StMonProc(uint8 cbport)
{
    if((guc_MonSaveFlag & Bit_Map8[cbport]) == 0)
    {
        guc_MonSaveFlag |= Bit_Map8[cbport];
        //gdw_SaveMonthData(cbport);                      //����������

        //���ж˿�ȫ�����������,������ͳ��(������ͳ��ǰ)
        //   if((guc_MonSaveFlag & Bit_Mask8[MCB_PORT_NUM]) == Bit_Mask8[MCB_PORT_NUM])
        //   {
        //    STAT_TMMonSave();
        //    if(GetTime()->dmon == 1)
        //    {                                   //����,���㵱���ն�ͳ��
        //        STAT_TMMonCrack(GetTime()->month);
        //       gdw_CrackMonthData(gs_SysTime.YMDhms);
        //     }
        //   }
        //
    }
}

/************************************************************************
**��������: void Netp_StDayProc(uint8 cbport)
**�� ��: ����������
**
**�� ��:  cbport ����˿ں�
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 2009-08-01
*************************************************************************/
void Netp_StDayProc(uint8 cbport)
{
    if((guc_DaySaveFlag & Bit_Map8[cbport]) == 0)
    {
        guc_DaySaveFlag |= Bit_Map8[cbport];

//	        gdw_SaveDayData(cbport);                        //�����ն���һ��������
    }
}

/************************************************************************
**��������: void Netp_StHourProc(uint8 cbport)
**�� ��: ����Сʱ����
**
**�� ��:  cbport ����˿ں�
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 2009-08-01
*************************************************************************/
void Netp_StHourProc(uint8 cbport)
{
    if((guc_HourSaveFlag & Bit_Map8[cbport]) == 0)
    {
        guc_HourSaveFlag |= Bit_Map8[cbport];
//	        gdw_SaveHourData(cbport);
    }
}


/************************************************************************
**��������: void Netp_StRecProc(uint8 cbport)
**�� ��: ���᳭��������
**
**�� ��:  cbport ����˿ں�
**�� ��: 
**
**�� ע: 
**--------------------------------------------------------
**�� ��: yzy
**�� ��: 2009-08-01
*************************************************************************/
void Netp_StRecProc(uint8 cbport)
{
    if((guc_RecSaveFlag & Bit_Map8[cbport]) == 0)
    {
        guc_RecSaveFlag |= Bit_Map8[cbport];
//	        gdw_SaveRecData(cbport, 0);
//	        gdw_SaveRecData(cbport, 2);
    }
}







/************************************************************************
 * @function: Netp_ReadFinish
 * @����: �˿ڳ������
 * 
 * @����: 
 * @param: pdata ����˿ں�
 * @����: 
 * @˵��: 
 * @����: yzy (2011-9-20)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_ReadFinish(uint8 index)
{                                           //��ȡ����˿ں�
    if(index >= MCB_PORT_NUM)
    {
        return;
    }
                                            //���ݶ���
//	    UpdateSysTimeStruct();                  //�ȸ�����ʱ���
//	    Netp_StMonProc(index);
//	    Netp_StDayProc(index);
//	    Netp_StHourProc(index);
//	    Netp_StRecProc(index);//����������
	
    //==================
    EVT_ERC21_31_Proc(index);               //����
}

/************************************************************************
 * @function: Netp_CheckTstTime
 * @����: ���¹ҵ����й㲥Уʱ
 * 
 * @����: 
 * @param: index ����˿�����
 * @����: 
 * @˵��: ����\���߹�Լ:689999999999996808067A64454938455D16
 * @����: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_CheckTstTime(uint8 index)
{
//	    SerialSets ss;
//	    DLT_Frame frm;
//	    uint8 tmp[32];
//	    uint16 len;
//	    const uint8 forchar[3] = {0xFE, 0xFE, 0xFE};
//	    uint8 port;
//	    port = NETP_PHY_PORT[index];
//	    for(uint8 i = 0; i < 2; i++)
//	    {
//	        frm.cmd = 0x08;                         //������
//	        frm.atr = 0;                            //����
//	        frm.send = tmp;                         //������ʼ��ַ
//	        frm.wd.lword = 0;                       //���ݱ�ʶ
//	        frm.len = 6;                            //����
//	        gs_RealTime.ReadBCDDateTime((TIME*)(tmp + 10));
//	        BG_Pack(&frm, (uint8*)gucs_BGALLAddr[0], &len);
//	                                                //��������
//	        ss.baudrate = (i == 0) ? 1200 : 2400;
//	        ss.databits = DataBits_8bits;
//	        ss.parit = Parit_E;
//	        ss.stopbits = StopBits_1;
//	
//	        //gs_GPIO.LED_BlinkSet(LED_485_T, 0, 0, 0);//��һ��
//	                                                //�򿪴���
//	        gs_Uart.open(port, false);   
//	        gs_Uart.Init(port, &ss);
//	        gs_Uart.Clear(port);
//	        gs_Uart.Write(port, (uint8*)forchar, sizeof(forchar));    //ǰ����
//	        gs_Uart.Write(port, tmp, len);
//	        gs_Uart.close(port);
//	        aos_msleep(200);  
//	    }
//	    gs_InpExtracb[index].gbbreak = 0;
    //gs_OS.Message_Send(MSG_NETP_IDATA, TASK_NETP_TKID + index);
}


/************************************************************************
 * @function: Netp_RecvProc
 * @����: �����˿ڽ��յ�����֮������ݴ���
 * 
 * @����: 
 * @param: data 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_RecvProc(void)
{
    uint8 pdata[256];
    uint16 length;
    uint8 err;
    
    //here we should apply to occupy the resource
    //otherwise netp active read-received data will be readout here.
    gs_Uart.open(NETP_PORT_NO, false);

//	    while(gs_Uart.ToBeRead(NETP_PORT_NO) > 256)
//	    {
//	        length = gs_Uart.Read(NETP_PORT_NO, pdata, 256);
//	    }
    length = gs_Uart.Read(NETP_PORT_NO, pdata, 256, 5000);
    
    aos_msleep(2);
    gs_Uart.Close(NETP_PORT_NO);
    
    //��־
    //Netp_DiscoSetRecvFlag();
                                        //������ͨ��4������
    err = PstFrmProc(PST_CHN_NETP, pdata, length, pdata, sizeof(pdata));
    if(SYS_ERR_OK == err)
    {
        gul_noPlcReadSecs = 0;        //���㰲ȫ��־(***)
    }
    
}

/*******************************************************************************
 * @function_name:  Netp_SecProc
 * @function_file:  netp.c
 * @����: �봦��
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2011-04-25)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void Netp_SecProc(void)
{
//	    scm_dlms_t* scm = &gs_scmDlms;

    gul_noPlcReadSecs++;
                                        //�쳣��ȫ���Ƶ�ʵ��
    if((gul_noPlcReadSecs > 86400))
    {
        gul_noPlcReadSecs = 0;
//	        SYS_Reset();
    }

//	    if(!(scm->stt & SCM_STT_FIROK) &&
//	       ((scm->stt & SCM_STT_EXMASK) == SCM_STT_IDLE))
//	    {
//	        if((GetSysRunSecs() > 4)         //�ȴ�MBUS��Դ�ȶ�
//	           )
//	        {                               //���ϵ��δ�����ѱ�,������
//	            ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID);
//	        }
//	    }
                                        //ִ���ѱ�����
//    Netp_FindSecProc();
//	    
//	    if(!(scm->stt & SCM_STT_FIROK) && 
//	       ((scm->stt & SCM_STT_EXMASK) == SCM_STT_IDLE))
//	    {
//	                                        //���ϵ��δ�����ѱ�,������
//	        ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID);
//	    }
//	    
//	    Netp_MtUpdataPro();
   
}

/************************************************************************
 * @function: Netp_chktimeisok
 * @����: �жϱ�ƶ�ʱʱ���Ƿ�
 * 
 * @����: 
 * @param:  
 * @����: 
 * @˵��: 
 * @����: zjc (2015/5/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_chktimeisok(void)  //
{
    
//	    UpdateSysTimeStruct();  //����ϵͳʱ��
    
    if(gs_NetpMtTime.period > 0 )//
    {
        uint32 syshour = CalcHourHex(gs_SysTime.YMDhms[0], gs_SysTime.YMDhms[1], gs_SysTime.YMDhms[2], 
                                        gs_SysTime.YMDhms[3]);
        
        if((syshour >= gs_NetpMtTime.hours) && ((syshour - gs_NetpMtTime.hours) % gs_NetpMtTime.period == 0)) //����ʱ��������
        {
            if((gs_SysTime.YMDhms[4] >= gs_NetpMtTime.datatime[4]) && (gs_SysTime.YMDhms[4] >= 1))   //����ʱ�� ���ӵ���
            {
                if(gs_NetpMtTime.stt == 0)  //��ִֻ֤��һ��
                {
                    gs_NetpMtTime.stt = 1;
//                    ES_Message_Send(MSG_NETP_CKTIME, TASK_NETP_TKID);   //��ʼУʱ
                }
            }
            else    //��������δ��֮ǰ����״̬λ׼����
            {
                gs_NetpMtTime.stt = 0;
            }
        }
        
    }
}

/************************************************************************
 * @function: Netp_MinProc
 * @����: ��������
 * 
 * @����: 
 * @param:  
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/22)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_MinProc(void)  //
{
//	    scm_dlms_t* scm = &gs_scmDlms;
    uint8 bmaps[LEN_TST_BMPS];
    
//	    if(((scm->stt & SCM_STT_EXMASK) != SCM_STT_FIN))
//	    {
//	        gs_GPIO.BlinkSet(GPIO_LED_485, 1, 0, 0);
//	        return;
//	    }
    UpdateSysTimeStruct();  //����ϵͳʱ��
    
    if(gs_SysVar.mDGstt & HLV_STT_LENG)
    {
        gs_SysVar.mLPstt &= ~HLV_LPTASK_CP;
        return;
    }
//	    Netp_chktimeisok(); //�жϸ���ƶ�ʱʱ���Ƿ���
    //uint8_t min_interval = (gs_PstPara.cp_interval / 60);
    //min_interval = (min_interval) ? min_interval:1;
    if((gs_SysTime.YMDhms[4] % gs_PstPara.cp_interval) == 0)  //n���Ӻ���ȥ��������ߵ�
    {
        aos_msleep(100);
        GD_Para_RW(F10_PADDR, bmaps, LEN_TST_BMPS, false);   //��������λͼ
        if(STR_IsEmpty(bmaps, LEN_TST_BMPS))  //������λͼΪ�գ����ٴ��ѱ�
        {
            return ;
        }
        else
        {
//	                uint8 msg = MSG_NETP_CURDATA;
        //          krhino_buf_queue_send(&gs_TKFarpQueue, &msg, 1);
            Task_Clear();
//	            gs_GPIO.GPO_Out(GPO_485_PWR, true);
            
            Netp_PowerMng(1, NETP_PORT_PWR_485);
            gs_SysVar.mLPstt |= HLV_LPTASK_CP;
            
            if(gs_SysVar.mDGstt & HLV_STT_NENG)
            {
                HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/RHINO_CONFIG_TICKS_PER_SECOND);
                //LOG_DEBUG("~~~~~~~~~~~~~port cp wait start ~~~~~~~");
                aos_msleep(2500);
                //LOG_DEBUG("~~~~~~~~~~~~~port cp going start ~~~~~~~");   

            }    
            
            aos_msleep(100);
            
            uint8 YMDhms[6];   //ʱ��
            uint8 ucmin;            
            MoveBuffer(gs_SysTime.YMDhms, YMDhms, 6);  //��ǰʱ��  
            ucmin = YMDhms[4] % gs_PstPara.cp_interval;
            YMDhms[4] = YMDhms[4] - ucmin;   //ȡ�ϴ� ���߱�����ӵ�
            YMDhms[5] = 0;
            memcpy(gss_TaskATimer.time.YMDhms, YMDhms, 6);

            gs_OS.Message_Send(&gs_TKNetpQueue, &msgidA[MSG_NETP_CURDATA], 1);//(MSG_NETP_CURDATA, TASK_NETP_TKID);  //����������
            gs_OS.Message_Send(&gs_TKNetp2Queue, &msgidA[MSG_NETP_CURDATA], 1);
            gs_OS.Message_Send(&gs_TKNetp3Queue, &msgidA[MSG_NETP_CURDATA], 1);
            gs_OS.Message_Send(&gs_TKNetp4Queue, &msgidA[MSG_NETP_CURDATA], 1);

        }
    }
    //gs_OS.Message_Send(MSG_NETP_EVNDATA, TASK_NETP_TKID);  //���¼�
    
    if(dbflag.hour == 1)//Сʱ��������
    {
        if(gs_SysTime.YMDhms[4] < NETP_READDAY_MIN)  //n���Ӻ���ȥ����������
        {
            return ;
        }
        if((gs_SysTime.YMDhms[3] % NETP_REREADDAY_HOUR) == 0)   //
        {      
           //gs_OS.Message_Send(MSG_NETP_DAYDATA, TASK_NETP_TKID);  //��ȡ�ն�������
        }
        dbflag.hour = 0;    //�رձ�־
    }
    if(dbflag.day == 1)  //����������
    {
        if(gs_SysTime.YMDhms[4] < NETP_READMONTH_MIN)  //n���Ӻ���ȥ����������
        {
            return ;
        }
        //gs_OS.Message_Send(MSG_NETP_MONDATA, TASK_NETP_TKID);  //��ȡ�¶�������
        dbflag.day = 0; //�رձ�־
    }
}


/************************************************************************
 * @function: Netp_HourProc
 * @����: Сʱ����
 * 
 * @����: 
 * @param:  
 * @����: 
 * @˵��: 
 * @����: zjc (2014/8/22)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Netp_HourProc(void)  //
{ 
//	    scm_dlms_t* scm = &gs_scmDlms;
//	    
//	    if(((scm->stt & SCM_STT_EXMASK) != SCM_STT_FIN))
//	    {
//	        return;
//	    }
    
//	    UpdateSysTimeStruct();  //����ϵͳʱ��
    
    if( (gs_SysTime.YMDhms[3] % NETP_REREADCURE_HOUR) == 0 )  //N��Сʱ��ѯһ�Σ��Ƿ�����Ƿ��������
    {
//	        ES_Message_Send(MSG_NETP_BCURDATA, TASK_NETP_TKID);  //����������
    }
    dbflag.hour = 1;     //�����ն�������
    
    
}

/*******************************************************************************
 * @function_name:  Netp_DayProc
 * @function_file:  netp.c
 * @����: �~����
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2011-04-25)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void Netp_DayProc(void)
{                                       //��λMBUS
    //gs_GPIO.GPO_Out(GPO_485_PWR, false);
//	    aos_msleep(300);
//	    gs_GPIO.GPO_Out(GPO_485_PWR, true);
//	    aos_msleep(300);
    
    dbflag.day = 1;  //�����¶�������
}


extern const uint8 netp_hwtest_frm1[];



void NetpErrProb(void)
{
    SerialSets ss;
    uint8 phyport;
    uint8 overtime;
    uint8 buffer[50];
    uint16 len;
    
//	    ES_ENTER_SCRT();
//	
//	    for(uint8 xx = 0; xx < 20; xx++)
//	    {
//	        MoveBuffer((uint8*)netp_hwtest_frm1, buffer, 37);
//	        phyport = NETP_PORT_NO;
//	        ss.baudrate = NETP_PORT_BAUD;
//	        ss.databits = DataBits_8bits;
//	        ss.parit = Parit_N;
//	        ss.stopbits = StopBits_1;
//	                                                //�򿪴���
//	        gs_GPIO.GPO_Out(GPO_485_PWR,false);
//	        gs_Uart.open(phyport, false);
//	        gs_Uart.Init(phyport, &ss);
//	        gs_Uart.Clear(phyport);
//	                                                
//	        gs_Uart.Write(phyport, buffer, 37);   //��������
//	
//	        len = 0;                                            //���ճ���Ϊ0
//	        overtime = 40;
//	        while(overtime--)                                   //�ڹ涨����ʱ�ڼ��ڽ�����Ч֡���ж�
//	        {
//	            aos_msleep(10);                             //�ж��Ƿ��������յ�
//	            if(gs_Uart.ToBeRead(phyport) > 0)
//	            {
//	               len += gs_Uart.Read(
//	                               phyport, 
//	                               buffer + len, 
//	                               256 - (len & 0xFF),
//	                               5000);
//	
//	                len &= 0xFF;                                //������һ���ֽ����ڵ�
//	                                                            //�ж�֡����Ч�� 
//	                if(__PST_CheckFrame(PST_FRM_DLMS_HDLC_NO, buffer, &len) == PST_ERR_OK)
//	                {    
//	                    //�رմ���
//	                    gs_GPIO.LED_BlinkSet(GPIO_LED_485, 2, 0, 0);    //��
//	                    gs_Uart.close(phyport);
//	                    aos_msleep(50);
//	                    break;
//	                }
//	            }
//	        }
//	        gs_Uart.close(phyport);
//	    }
//	    
//	    ES_EXIT_SCRT();
}
//extern const ST_Netp_FW gs_netp_fw_tx13;
//extern const ST_Netp_FW gs_netp_fw_wxic;

/*******************************************************************************
 * @function_name:  SYS_Netp_Init
 * @function_file:  netp.c
 * @����: ���̳�ʼ��
 * 
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2011-03-03)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_Netp_Init(void)
{



//	    Netp_Register_Set(gss_NetpFrameFw[1]);
    Netp_Register_Init();
}

/************************************************************************
 * @function: SYS_NETP_Task
 * @����: ������̴�����
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/2/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SYS_NETP_Task(void)
{
    //������Ϣ
    krhino_buf_queue_create(&gs_TKNetpQueue, "netp_queue",
                         gc_TKNetpQbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);
    
    SYS_Netp_Init();

    SYS_MSG_Apply(TASK_NETP_TKID, MSG_CLS_TM);
//	    SYS_MSG_ApplyExt(TASK_NETP_TKID, MSG_CLS_UART, NETP_PORT_NO);
    aos_msleep(500);
//	    uint8_t msg[2];
//	    msg[0] = MSG_NETP_TASK_RUN_FLAG_PER_485;
//	    msg[1] = 0;
//	    extern kbuf_queue_t gs_TKSlvQueue;
//	    kstat_t t = gs_OS.Message_Send(&gs_TKSlvQueue, msg,2);
//	    LOG_DEBUG("port[0] CP flag note slv to sign [%d] \n", t);
    SetTaskRunFlag(0);
//	    gs_OS.Message_Send(&gs_TKNetpQueue, &msgidA[MSG_NETP_CHECK], 1);
    for(;;)
    {
        //��ȡ������Ϣ
        krhino_buf_queue_recv(&gs_TKNetpQueue, RHINO_WAIT_FOREVER, g_TKNetpQ_buf_recv,
                                  &g_TKNetpQ_recv_size);

        switch(g_TKNetpQ_buf_recv[0])                        //������Ϣ�ֱ���
        {
            case MSG_SEC:
                
                Netp_SecProc();
           
                
                break;
            case MSG_MIN:
                gs_SysVar.mLPstt |= HLV_LPTASK_SMSG_NETP;
                
                Netp_MinProc();       //����
                gs_SysVar.mLPstt &= ~HLV_LPTASK_SMSG_NETP;
                
                break;
            case MSG_HOUR:
                Netp_HourProc();    
                break;
            case MSG_DAY:
                Netp_DayProc();         
                break;
            case MSG_NETP_IDATA:            //�˿�1���ݳ�����
//	                Netp_ReReadCureData(0,24);        //��ǰ24���������
//	                ReadDlmsLastDayData(1);          //��ȡ�ն������
//	                ReadDlmsLastMonEnergy(1);        //��ȡ�¶������
//	                ReadDlmsLastMonthDMData(1);      //��ȡ�¶�������
                break;
            case MSG_NETP_CURDATA:   //����������
                Netp_InitData(0);
                break;
            case MSG_NETP_BCURDATA:   //����������
//	                Netp_ReReadCureData(0,12);      //��ǰ12���������     
                break;
            case MSG_NETP_DAYDATA:                //���ն�������
//	                ReadDlmsLastDayData(0);          //��ȡ�ն������
                break;
            case MSG_NETP_MONDATA:                //���¶�������
//	                ReadDlmsLastMonEnergy(0);        //��ȡ�¶������
//	                ReadDlmsLastMonthDMData(0);      //��ȡ�¶�������
//	                ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID);   //�¶������ݳ�����������ѱ� 
                break;
            case MSG_NETP_EVNDATA:                //���¼�
//	                Netp_ReadEvent(0);   //��DLMS�¼�
                break;
            case MSG_NETP:      //����ڱ���ͨ��
                Netp_RecvProc();
                break;
            case MSG_NETP_CHECK:
                Netp_Check(0);
                break;
            case MSG_LIVE:
                HB_RetLive(TASK_NETP_TKID);
                break;
        }
    }
}

/******************************************************************************
**������̿鳣�����ֶ���
******************************************************************************/
const KTaskConst gs_TkNetp = 
{
    &gs_TKNetpHandle,
    SYS_NETP_Task,                           //Ӧ�ù������
    gs_TKNetpStack,
    TASK_NETP_TKID,                           //���̱��
    true,                                    //��Ϣѭ�����ƽ���
    TASK_NETP_STKL,
    "netp",
    &gs_TKNetpQueue,
};



