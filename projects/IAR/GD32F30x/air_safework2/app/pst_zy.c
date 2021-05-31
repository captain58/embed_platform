/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: pst_zy.c
**��   ��   ��: yzy
**����޸�����: 2019��6��18��
**��        ��: ͨѶģ��____��Լ�����ļ���������ͨѶ�ͱ���ͨѶ
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2019��6��18��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_ZY
#define EXT_PSTZY

//	#include "es.h"
//	#include "hal.h"
//	#include "bsp.h"
//	#include "app.h"
//	#include "apis.h"
//	#include "extapi.h"

//	#include "defaultfram.c"

#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"
//#include "flsmap.h"
#include "saextapi.h"
#include "pst_param.c"
#include "lib_com/calclib_gw.h"
#include "netp.h"
#include "paradef.h"
#include "netp_fm.h"
#include "task.h"
#include "hlv.h"
#include "inp.h"
#include <sal_import.h>
#include "atmodem.h"
#include "alert.h"
#include <math.h>
extern kbuf_queue_t gs_TKHlvQueue;
extern kbuf_queue_t gs_TKNetpQueue;
extern kbuf_queue_t gs_MainQueue;
extern kbuf_queue_t gs_TKSlvQueue;
extern kbuf_queue_t gs_TKNetp2Queue;
extern kbuf_queue_t gs_TKNetp3Queue;
extern kbuf_queue_t gs_TKNetp4Queue;
//	uint8_t g_ucMeterPara[1024];
//	uint8_t g_ucPrepay[1024];
extern kbuf_queue_t gs_TKFarpQueue;
extern const NetpFrameDataStr gs_NetpFrameWXICPrepayWDataStr;
extern const NetpFrameDataStr gs_NetpFrameWXICPrepayRDataStr;
extern const NetpFrameDataStr gs_NetpFrameWXICRechargeWDataStr;
extern const NetpFrameDataStr gs_NetpFrameWXICPriceWDataStr;
extern const NetpFrameDataStr gs_NetpFrameWXICPriceRDataStr;

extern const NetpFrameDataStr gs_NetpFrameWXICVlvWDataStr;

uint8 PST_ZY_Pack(PST_Frame* frame, uint8 err, uint16* length);
uint8 RstSystemParam(uint16 PType);
uint8 RstPara(bool mode);
/************************************************************************
 * @function: ZY_OpDataBlock
 * @����: ֱ�Ӷ�ȡ����д�����ݿ�����
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  PST_ERR_RE:���ûظ�,PST_ERR_OK:����
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_OpDataBlock(const PST_TableStr* tbl, PST_Frame* frm)
{
//	    DLT_Frame* frame = &frm->_dlt;
//		uint8 ID = frame->recv[0];          //���ݿ���1�ֽ�
//		uint8 len = frame->recv[1];         //��ȡ����1�ֽ�
//		                                    //��ʼ��ַ3�ֽ�
//		uint32 addr = ByteToInt24(frame->recv + 2);
//		TDataBlock db;
//		
//		if(len > 250)                       //���ܶ�����220�ֽڵ�����
//		{
//		    return PST_ERR_VAR;             //�ظ��쳣����֡
//		}
//		                                    //���ļ��ɹ�
//		if(SYS_FILE_DB_Open(ID, &db, TDB_MODE_RW) != SYS_ERR_OK)
//		{
//	        return PST_ERR_FT;
//	    }
//	    
//	    if(addr + len > db.length)          //��д���ݳ�����DataBlock������
//	    {
//	        frame->len = 0;
//	        SYS_FILE_DB_Close(&db);
//	        return PST_ERR_VAR;
//	    }
//	    
//	    if(frame->cmd == 0x01)//������
//	    {
//	        if(SYS_FILE_DB_ReadFrom(&db, frame->send, len, addr) == SYS_ERR_OK)
//	        {
//	            frame->len = len;           //���ݳ���
//	            SYS_FILE_DB_Close(&db);
//	            return PST_ERR_OK;          //�ظ��ɹ�����֡
//	        }
//	    }
//	    else                                //д����
//	    {
//	        if(SYS_FILE_DB_WriteFrom(&db, frame->recv + 5, len, addr) == SYS_ERR_OK)
//	        {
//	            //׼���ظ�����
//	            frame->send[0] = ID;
//	            frame->send[1] = len;
//	            Int24ToByte(frame->send + 2, addr);
//	            frame->len = 5;
//	            SYS_FILE_DB_Close(&db);
//	            return PST_ERR_OK;          //�ظ��ɹ�����֡
//	        }
//	    }
//	    
//		SYS_FILE_DB_Close(&db);
	return PST_ERR_FT;                  //���ļ�ʧ�ܷ��ش�������֡
}


/************************************************************************
 * @function: ZY_RemoteUpWithReply
 * @����: Զ���������ݵ�д��
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  PST_ERR_RE:���ûظ�,PST_ERR_OK:����
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_RemoteUpWithReply(const PST_TableStr* tbl, PST_Frame* frm)
{
//	    DLT_Frame* frame = &frm->_dlt;
//	    TDataBlock db;
//	    uint8 len;
//	    uint32 address;
//	
//	    len = frame->recv[0];                   //��ȡ����(1�ֽ�)
//	    address = ByteToInt24(frame->recv + 1); //��ȡ��ʼ��ַ(3�ֽ�)
//	                                            //�����ݿ�
//	    SYS_FILE_DB_Open(DB_UPDATE, &db, TDB_MODE_RW);
//	                                            //д������
//	    SYS_FILE_DB_WriteFrom(&db, frame->recv + 4, len, address);
//	    SYS_FILE_DB_Close(&db);
//	    
//	    //׼���ظ�����
//	    frame->send[0] = len;
//	    Int24ToByte(frame->send + 1, address);
//	    frame->len = 4;
    
    return PST_ERR_OK;

}


/************************************************************************
 * @function: ZY_RemoteUpNoReply
 * @����: Զ���������ݵ�д��
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  PST_ERR_RE:���ûظ�,PST_ERR_OK:����
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_RemoteUpNoReply(const PST_TableStr* tbl, PST_Frame* frm)
{
//	    DLT_Frame* frame = &frm->_dlt;
//	    TDataBlock db;
//	    uint8 len;
//	    uint32 address;
//	
//	    len = frame->recv[0];                   //��ȡ����(1�ֽ�)
//	    address = ByteToInt24(frame->recv + 1); //��ȡ��ʼ��ַ(3�ֽ�)
//	                                            //�����ݿ�
//	    SYS_FILE_DB_Open(DB_UPDATE, &db, TDB_MODE_RW);
//	                                            //д������
//	    SYS_FILE_DB_WriteFrom(&db, frame->recv + 4, len, address);
//	    SYS_FILE_DB_Close(&db);
    
    return SYS_ERR_NOREPLY;

}



/************************************************************************
 * @function: ZY_RemoteUpdate
 * @����: Զ����������֡
 * 
 * @����: 
 * @param: tbl 
 * @param: frame 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_RemoteUpdate(const PST_TableStr* tbl, PST_Frame* frm)
{
    DLT_Frame* frame = &frm->_dlt;
    uint32 len;
	uint32 address;
	
	len = ByteToInt24(frame->recv);
	address = ByteToInt24(frame->recv + 3);
    //ES_IAP_Ready(address, len, true);
    return SYS_ERR_NOREPLY;
}



/************************************************************************
 * @function: ZY_MonOC
 * @����: �������ģʽ
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_MonOC(const PST_TableStr* tbl, PST_Frame* frm)
{
	    DLT_Frame* frame = &frm->_dlt;
//	    
//	    ES_TK_Resume(TASK_MNT_TKID); //�ָ����ӽ���
//	    ES_MNT_OC(true);            //�������ģʽ
//	    
    frame->len = 0;
    return PST_ERR_OK;
}




/************************************************************************
 * @function: ZY_AppVer
 * @����: Ӧ�ó���汾
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
#define CON_VER_MAX_LEN 32
extern const __root uint8_t gucs_softVer[];//="4G-LS-R(V1.0.";
extern const __root uint8_t gucs_HDVer[];//="4G-LS-R(V0.0.";
extern const __root uint32 gul_UsrHardcVer;// = 0x19071701;
uint8 ZY_AppVer(const PST_TableStr* tbl, PST_Frame* frm)
{
    ZY_Frame* frame = &frm->_zy;
    uint16 bgdi = frame->wd.word[0];
    extern const uint32 gul_OSVer;
    
    switch(bgdi)
    {
        case 0x4500:
            LongToByte(frame->send, gul_UsrFuncVer);
            frame->len = 4;
            break;
            
        case 0x4501:
            //LongToByte(frame->send, *((uint32*)FLS_BOOTVER_ADDR));
            frame->len = 4;
            break;
            
        case 0x4502:
//	            LongToByte(frame->send, gul_OSVer);
            frame->len = 4;
            break;
        case 0x8644:
            memset(frame->send,0,64);
            snprintf(frame->send, CON_VER_MAX_LEN-1, "%s%02x.%x)", gucs_softVer, gul_UsrFuncVer>>24, gul_UsrFuncVer%0x100);
            snprintf(frame->send+32, CON_VER_MAX_LEN-1, "%s%02x.%x)", gucs_HDVer, gul_UsrFuncVer>>24, gul_UsrHardcVer%0x100);
            frame->len = 64;
            break;
        default:
            frame->len = 0;
            return PST_ERR_FT;
    }

    SYS_OK();
}




/************************************************************************
 * @function: ZY_PrjCode
 * @����: ��ȡ��Ŀ���
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/10/20)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_PrjCode(const PST_TableStr* tbl, PST_Frame* frm)
{
    DLT_Frame* frame = &frm->_dlt;
    MoveBuffer((uint8*)gucs_PrjCode, frame->send, 6);
    InvBuffer(frame->send, 6);
    frame->len = 6;
    SYS_OK();
}



/************************************************************************
 * @function: ZY_ListMtInfo
 * @����: �г��¹ұ���Ϣ
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_ListMtInfo(const PST_TableStr* tbl, PST_Frame* frm)
{
//	    DLT_Frame* frame = &frm->_dlt;
//	    uint8* pt = frame->send;
//	    uint16 idx;                         //������Ĵӽڵ�����
//	    uint8 mtnum = 0;                    //�Ѵ���Ĵӽڵ�����
//	    uint8 rdMtNum;                      //����Ҫ��ȡ�Ĵӽڵ�����
//	    uint8 eft[DLMS_MT_MAP];
//	    dlms_mtdt_t mt;
//	    
//	                                        //�����������ݳ��ȺϷ����ж�
//	    if(frame->len == 0 + 2)             //�����ϰ汾Э��,��������������(�ƻ�)
//	    {
//	        idx = 0;
//	        rdMtNum = 0xff;
//	    }
//	    else if(frame->len == 2 + 2)        //�°汾Э��
//	    {
//	        idx = frame->recv[0];
//	        rdMtNum = frame->recv[1];
//	    }
//	    else
//	    {
//	        frame->len = 0;                 //���лظ�֡����������
//	        return PST_ERR_FT;
//	    }
//	
//	    pt[0] = 0x01;                       //�ӽڵ��豸����:�ɼ���
//	    pt[1] = 0;                          //�ӽڵ��Լ:͸������
//	    
//	                                        //��ϸ��Ϣ
//	    if(((gs_scmDlms.stt & SCM_STT_EXMASK) != SCM_STT_FIN) && gb_dlmsMtFlashCrcOk)
//	    {                                   //�ѱ�δ���ʱ,ʹ��flash���ݴ���ϴ��ѱ���Ϣ�ظ�
//	        ReadFlash(DB_MTLIST, eft, sizeof(eft), (&gs_dlmsMt.eft[0] - (uint8*)&gs_dlmsMt));
//	        
//	        for(uint8 uc_i = 0; uc_i < DLMS_MT_MAX; uc_i++)
//	        {
//	            
//	            idx = STR_GetThisNextSetBit(eft, sizeof(eft), idx);
//	            if(idx == 0xffff)
//	            {
//	                break;
//	            }
//	                                            //��������
//	            ReadFlash(DB_MTLIST, (uint8*)&mt, sizeof(mt), ((uint8*)&gs_dlmsMt.mt[idx] - (uint8*)&gs_dlmsMt));
//	            MoveBuffer(mt.bgaddr, pt + 4 + 7*uc_i, 6);
//	            pt[4 + 7*uc_i + 6] = (mt.systitle[3] == 0x01) ? 3 : 4;
//	                                            //���¼�¼
//	            if(mtnum >= rdMtNum)
//	            {
//	                break;
//	            }
//	            mtnum++;
//	            idx++;
//	        }
//	                                            //�ӽڵ㸽���ڵ�������
//	        pt[2] = STR_CalcBitSetNum(eft, sizeof(eft));
//	        pt[3] = mtnum;                      //�����ϱ�����
//	    }
//	    else
//	    {
//	        for(uint8 uc_i = 0; uc_i < DLMS_MT_MAX; uc_i++)
//	        {
//	            idx = STR_GetThisNextSetBit(gs_dlmsMt.eft, sizeof(gs_dlmsMt.eft), idx);
//	            if(idx == 0xffff)
//	            {
//	                break;
//	            }
//	                                            //��������
//	            MoveBuffer(gs_dlmsMt.mt[idx].bgaddr, pt + 4 + 7*uc_i, 6);
//	            pt[4 + 7*uc_i + 6] = (gs_dlmsMt.mt[idx].systitle[3] == 0x01) ? 3 : 4;
//	                                            //���¼�¼
//	            if(mtnum >= rdMtNum)
//	            {
//	                break;
//	            }
//	            mtnum++;
//	            idx++;
//	        }
//	                                            //�ӽڵ㸽���ڵ�������
//	        pt[2] = STR_CalcBitSetNum(gs_dlmsMt.eft, sizeof(gs_dlmsMt.eft));
//	        pt[3] = mtnum;                      //�����ϱ�����
//	    }
//	        
//	
//	    frame->len = mtnum * 7 + 4;

    SYS_OK();
}




/************************************************************************
 * @function: ZY_PortTrans
 * @����: �˿�ת��
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: ��:������02c01201037EA0230002FEFF0393E4B0818014050207D0060207D00704000000010804000000013AF27E
 *       ��ʾ�˿�2��4800-n-8-1���÷���dlms�㲥֡.
 * @����: yzy (2014/8/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_PortTrans(const PST_TableStr* tbl, PST_Frame* frm)
{
    DLT_Frame* frame = &frm->_dlt;
    uint8 port = frame->recv[0];        //ת���˿ں�
    uint8 wsec = frame->recv[4];        //ת���ȴ��ظ�ʱ��
    uint16 frmlen;                      //��Ч��������
    uint8 dlen;
    SerialSets ss;

    if(frame->len <= (2 + 5))           //ת��������̫��
    {
        frame->len = 0;
        return SYS_ERR_FT;
    }

    //�˿ں�

    port = 0;//NETP_PORT_NO;
    
    //�����ʼ��ֽ�����
    ss.baudrate = ByteToInt(frame->recv + 1);
    if(frame->recv[3] & 0x80)
    {
        ss.databits = DataBits_7bits;
    }
    else
    {
        ss.databits = DataBits_8bits;
    }
    if(frame->recv[3] & 0x40)
    {
        ss.stopbits = StopBits_2;
    }
    else
    {
        ss.stopbits = StopBits_1;
    }
    switch(frame->recv[3] & 0x03)
    {
        case 1:
            ss.parit = Parit_N;
            break;
        case 2:
            ss.parit = Parit_O;
            break;
        default:
            ss.parit = Parit_E;
            break;
    }

    //׼���ظ�����
    frame->send[0] = frame->recv[0];
                                        //����
//    gs_Uart.open(port, false);
    gs_Uart.Init(port, &ss);
    gs_Uart.Write(port, frame->recv + 5, frame->len - 2 - 5, 0);
                                        //׼������
    dlen = 0;
    frmlen = 0;
    for(uint16 k = 0; k < (wsec * 20); k++)
    {
        dlen += gs_Uart.Read(port, frame->send + 1 + dlen, 0xff - dlen, 0);
        if(dlen > 0)                    //�յ�����
        {                               //�ж��Ƿ�Ϊdlms֡
            //frmlen = PST_CheckFrame2(PST_FRM_DLMS_HDLC_NO, frame->send + 1, dlen);
            if(!frmlen)                 //��Ч֡,�����ж��Ƿ�Ϊbg֡
            {
                frmlen = PST_CheckFrame2(PST_FRM_BG_NO, frame->send + 1, dlen);
            }

            if(frmlen)                  //��Ч֡,��ǰ�˳��ȴ�
            {
                break;
            }
        }
        aos_msleep(500);
    }
                                        //��Ч֡,��ʵ�ʽ������ݻظ�
    if(!frmlen)
    {
        frmlen = dlen;
    }
//    gs_Uart.close(NETP_PORT_NO);
                                        //�ظ�����
    frame->len = frmlen + 1;

    SYS_OK();
}




/************************************************************************
 * @function: ZY_DiscoStart
 * @����: ���������ѱ�
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_DiscoStart(const PST_TableStr* tbl, PST_Frame* frm)
{
    DLT_Frame* frame = &frm->_dlt;

    //ES_Message_Send(MSG_NETP_DISCOSTART, TASK_NETP_TKID);
    frame->len = 0;
	return PST_ERR_OK;                  //
}




/************************************************************************
 * �ⲿ�����DLMS͸��ָ��
 * @Ϊ�����߼�����,�ѱ�ʱ��ֹת��
 ************************************************************************/
typedef struct
{
    uint8  flag;                        //͸��ָ��ȴ��ظ����
    uint8  ChnInfo;                     //����͸����ͨ��
    uint16 saddr;                       //�ȴ��ظ���DLMS��ƶ̵�ַ
    uint8  bgaddr[6];                   //��֡����Ҫ��6�ֽڵ�ַ
    uint32 startSec;                    //����ʱ��ϵͳʱ��(���ڽ�������ʱ,�ж������Ƿ�ʱ)
    uint16 crc;
}DlmsTransInfo_t;

DlmsTransInfo_t gs_dlmsTransInfo;


/************************************************************************
 *dlms͸���ȴ��ظ������ʱ
 ************************************************************************/
#define SEC_DLMSTRANS_WAIT     4

//(Ϊ���Ӳ���������ȡ�ķǳ����ʩ,����ֵ�����������)
#define HMS_MBUSSTABLE_WAIT    150      //*10ms(���鷢��800ms�ɹ���Ϊ80%,1s�ɹ���Ϊ90%)


/************************************************************************
 * @function: ZY_DlmsTransRecvProc
 * @����: ����ͨ���յ����ݺ�,����DLMS͸���ж�����ĺ���
 * 
 * @����: 
 * @param: buffer ���յ���dlms֡ 
 * @param: legth ֡����
 * 
 * @����: 
 * @return: uint8 SYS_ERR_OK_����֡�ѱ�����. SYS_ERR_FT δ������
 * @˵��: 
 * @����: yzy (2014/10/20)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_DlmsTransRecvProc(uint8* buffer, uint16 length)
{
    PST_Frame frm;
    DLT_Frame* frame = &frm._dlt;
    
    if(gs_dlmsTransInfo.flag == 0xAA)   //�Ƿ���͸��ָ��ȴ��ظ�
    {                                   //��Ϣ������У��
        if(false == CheckBufferCRC((uint8*)&gs_dlmsTransInfo, sizeof(gs_dlmsTransInfo), true))
        {
            return SYS_ERR_FT;
        }
                                        //�ȴ��ظ���ʱЧ���ж�
        if((GetSysRunSecs() < gs_dlmsTransInfo.startSec) || 
           (GetSysRunSecs() > (gs_dlmsTransInfo.startSec + SEC_DLMSTRANS_WAIT)))
        {
            return SYS_ERR_FT;
        }
                                        //�ظ�֡�Ķ̵�ַ�Ϸ����ж�
        if(((gs_dlmsTransInfo.saddr >> 8) != buffer[6]) || 
           ((gs_dlmsTransInfo.saddr & 0xff) != buffer[7]))
        {
            return SYS_ERR_FT;
        }
                                        //׼���ظ�����
        MoveBuffer(buffer + 14, buffer + 6, length - 17);//����������.(14:11B֡ͷ+3B������ǰ�˹̶��ַ�.17:14+3Bβ��У�鼰֡β)
        MoveBuffer(gs_dlmsTransInfo.bgaddr, buffer, 6);

        frame->len = length - 17 + 6;
        frame->recv = __NULL;
        frame->send = buffer;
        frame->cmd = 0x01;
        frame->atr = 0;
        frame->tstno = 0;
        frame->wd.word[0] = 0x4703;
        
        //***************************************************************
                                        //�ƶ������򵽺��ʵ�λ��
        MoveBuffer(buffer, buffer + 12, frame->len);
                                        //������ݱ�ʶ
        IntToByte(frame->send + 10, frame->wd.word[0]);
        frame->len += 2;
                                        //�������֡
        PST_ZY_Pack(&frm, PST_ERR_OK, &length);
                                        //��������
        gfs_PSTChnSend[gs_dlmsTransInfo.ChnInfo & PST_FRM_CHNNO](frame->send, length);
        
        //***************************************************************
        gs_dlmsTransInfo.flag = 0;      //������
        
        return SYS_ERR_OK;
    }
    else
    {
        return SYS_ERR_FT;
    }
}




/************************************************************************
 * @function: ZY_DlmsTrans
 * @����: Dlms�ɼ�������ת��
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/10/20)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_DlmsTrans(const PST_TableStr* tbl, PST_Frame* frm)
{
//	    DLT_Frame* frame = &frm->_dlt;
//	    scm_dlms_t* scm = &gs_scmDlms;
//	    uint16 mtidx;
//	    uint8 dlen;
//	    uint8 bgaddr[6];
//	                                        //��ȥ2�ֽڱ�ʶ�ĳ���,ʣ���Ϊ�����򳤶�
//	    frame->len -= 2;
//	                                        //�����ѱ������,��ֹ����,�ظ����˳�
//	    if((scm->stt & SCM_STT_EXMASK) != SCM_STT_FIN)
//	    {
//	        memset(frame->send, 0xff, 6);
//	        frame->len = 6;
//	        return PST_ERR_OK;
//	    }
//	                                        //��ȡ�����ַ
//	    MoveBuffer(frame->recv, bgaddr, 6);
//	                                        //�ȶԲ����ַ
//	    for(mtidx = 0; mtidx < DLMS_MT_MAX; mtidx++)
//	    {
//	        if(STR_IsBitSet(gs_dlmsMt.eft, mtidx))
//	        {
//	            if(0 == CmpBuffer(gs_dlmsMt.mt[mtidx].bgaddr, bgaddr, 6))
//	            {
//	                break;
//	            }
//	        }
//	    }
//	    
//	    if(mtidx == DLMS_MT_MAX)            //�޷��ҵ��ñ�,�ظ����˳�
//	    {
//	        memset(frame->send, 0xff, 6);
//	        frame->len = 6;
//	        return PST_ERR_OK;
//	    }
//	                                        //��ȡ�����Ķ̵�ַ
//	    mtidx += DLMS_SADDR_OFFSET;
//	                                        //����ԭ֡��ɾ�����ַ
//	    frame->recv += 6;                   //6B ���ַ
//	    frame->len -= 6;                    //6B ���ַ
//	                                        //���dlms_data
//	    dlen = DLMS_AppReadPack(frame->recv, frame->len, frame->send, mtidx);
//	    if(dlen)                            //��Ա��ɼ����¹ҽڵ�
//	    {
//	                                        //��ֵ���յ�dlms�ظ�֡ʱʹ�õ���Ϣ�ṹ��
//	        gs_dlmsTransInfo.ChnInfo = frm->chninfo;
//	        gs_dlmsTransInfo.saddr = CalcDlmsSaddr(mtidx);
//	        MoveBuffer(bgaddr, gs_dlmsTransInfo.bgaddr, 6);
//	        gs_dlmsTransInfo.startSec = GetSysRunSecs();
//	        gs_dlmsTransInfo.flag = 0xAA;
//	        BuildBufferCRC((uint8*)&gs_dlmsTransInfo, sizeof(gs_dlmsTransInfo), true);
//	        
//	                                        //ͨ������ͨ�����Ͳ��ȴ��ظ�
//	        //���ڱ�ϵͳ��DLMS�¹ұ�,���Բ�ȡ���͵ȴ��ظ��Ļ���.
//	        //��Ϊ�Ѿ�ȷ������ڲ�ִ�з���.
//	        //����DLT�Ĳɼ�������.
//	#if (CHORUS_MBUS_DISTURB_SPEC_EN > 0)
//	        gs_OS.TK_Sleep(HMS_MBUSSTABLE_WAIT);//����ʱMBUS���յ�·����ָ���,��Ҫ�ȴ�ֱ��MBUS�ȶ�
//	#endif
//	        gs_Uart.open(NETP_PORT_NO, false);
//	        gs_Uart.Clear(NETP_PORT_NO);
//	        gs_Uart.Write(NETP_PORT_NO, frame->send, dlen);
//	        gs_Uart.close(NETP_PORT_NO);
//	        
//	        return PST_ERR_RE;              //�˴�ֻ������,���ظ�.�ڽ��յ��Ϸ����ٻظ�
//	    }
//	    else
//	    {
//	        memset(frame->send, 0xff, 6);
//	        frame->len = 6;
//	        return PST_ERR_OK;
//	    }
    return 0;
}




/************************************************************************
 * @function: ZY_SerialNO
 * @����: ��ȡ���������к�
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_SerialNO(const PST_TableStr* tbl, PST_Frame* frm)
{
    DLT_Frame* frame = &frm->_dlt;
    
    if(frame->cmd == 0x01)          //������
    {
        MoveBuffer(gucs_InpAddr, frame->send, 6);
        frame->len = 6;
        return PST_ERR_OK;
    }
    else
    {
        if(frame->len != 6 + 2)             //�����������ݳ��ȺϷ����ж�
        {
            frame->len = 0;                 //���лظ�֡����������
            return PST_ERR_FT;
        }
                                            //���ļ��ɹ�
        if(GD_Para_RW(INPADDR, frame->recv, 6, true) == PST_ERR_OK)
        {
            //ParamLoad_InpAddr();
            frame->len = 0;
            return PST_ERR_OK;              //�ظ��ɹ�����֡
        }
        
        frame->len = 0;
        return PST_ERR_FT;                  //���ļ�ʧ�ܷ��ش�������֡
    }
}

/************************************************************************
 * @function: ParamLoad_F251
 * @����: �ն˵�ַ
 * @����: 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ParamLoad_F251(void)
{
    uint8 addrbak[6];
    uint8 temp[6];
    
    
    if(GD_Para_RW(F251_PADDR, temp, 6, false) != SYS_ERR_OK)
    {
        GD_Para_RW(F251_PADDR, temp, 6, false);
    }

    MoveBuffer(gs_PstPara.Addr, addrbak, 6);
    MoveBuffer(temp, gs_PstPara.Addr, 6);
    if(CmpBuffer(gs_PstPara.Addr, addrbak, 6) && (gl_SysRunStt & HRS_PARALD))
    {
//	       gs_OS.Message_Send(MSG_FARP_PARACHG, TASK_FARP_TKID);
       
        gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FARP_PARACHG], 1);
        gs_OS.Message_Send(&gs_MainQueue, &msgidA[MSG_MAIN_BLE_CHK], 1);

    }
    
//	    MoveBuffer(gs_PstPara.Addr, gucs_FcAddr + 2, 2);
//	    MoveBuffer(gs_PstPara.Addr + 2, gucs_FcAddr, 2);
    
    SYS_OK();
}
/************************************************************************
 * @function: ParamLoad_F251
 * @����: �ն˵�ַ
 * @����: 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ParamLoad_Test(void)
{
    uint8 addrbak[6];
    uint8 temp[6];
    
    uint8 buff[96];
    uint32 ip;
    uint16 pt;
    uint32 ib;
    uint16 pb;
    uint16 len_apn;
    uint8 uc_i;
    
    if(GD_Para_RW(F251_PADDR, temp, 6, false) != SYS_ERR_OK)
    {
        GD_Para_RW(F251_PADDR, temp, 6, false);
    }

    MoveBuffer(gs_PstPara.Addr, addrbak, 6);
    MoveBuffer(temp, gs_PstPara.Addr, 6);
    if(CmpBuffer(gs_PstPara.Addr, addrbak, 6) && (gl_SysRunStt & HRS_PARALD))
    {
//	       gs_OS.Message_Send(MSG_FARP_PARACHG, TASK_FARP_TKID);
       
        gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FARP_PARACHG], 1);
//	        gs_OS.Message_Send(&gs_MainQueue, &msgidA[MSG_MAIN_BLE_CHK], 1);

    }
    
                                            //��ȡ����
    GD_Para_RW(PARA_GPRS, buff, PARA_GPRS_PLEN, false);
                                            //�жϲ����仯,�����Ƿ�����
    ip = RByteToLong(buff);
    pt = RByteToInt(buff + 4);
    len_apn = strlen((string)(buff + 6));
    if(len_apn > 30)
    {
        len_apn = 30;
    }
                                           //���ǳ���ˢ�²���
    if(gl_SysRunStt & HRS_PARALD)
    {
        if((gs_FarpVar.dcip != ip) || 
           (gs_FarpVar.dcpt != pt) ||
//	           (gs_FarpVar.dcib != ib) || 
//	           (gs_FarpVar.dcpb != pb) ||
           (len_apn != strlen((string)gs_FarpVar.gapn)) || 
           (CmpBuffer(buff + 6, gs_FarpVar.gapn, len_apn) != 0))
        {                                   //ִ�в������Ĵ���������
            gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FARP_PARACHG], 1);//(MSG_FARP_PARACHG, TASK_FARP_TKID);
        }
    }

    gs_FarpVar.dcip = ip;                   //��վ��ѡip��ַ
    gs_FarpVar.dcpt = pt;                   //��վ��ѡ�˿ں�
    gs_FarpVar.dcib = ip;                   //��վ����ip��ַ
    gs_FarpVar.dcpb = pt;                   //��վ���ö˿ں�
                                            //APN
    if(len_apn == 0)
    {                                       //��ʼ��Ϊcmnet
        MoveBuffer("cmnet", gs_FarpVar.gapn, 6);
    }
    else
    {                                       //�ж��Ƿ��зǷ��ַ�
        for(uc_i = 0; uc_i < len_apn; uc_i++)
        {
            if(buff[6 + uc_i] > 127)
            {
                break;
            }
        }

        if(uc_i != len_apn)                 //�Ƿ�,��ʼ��Ϊcmnet
        {
            MoveBuffer("cmnet", gs_FarpVar.gapn, 6);
        }
        else
        {
            MoveBuffer(buff + 6, gs_FarpVar.gapn, 30);
            gs_FarpVar.gapn[29] = 0;
        }
    }
    GD_Para_RW(PARA_HB, (uint8_t *)&gs_FarpVar.hbcy, 2, false);
    if(gs_FarpVar.hbcy > 900 || gs_FarpVar.hbcy < 60)
    {
        gs_FarpVar.hbcy = 300;
    }
    
    SYS_OK();
}

/************************************************************************
 * @function: ZY_GwAddr
 * @����: ��ȡ�����òɼ�����ַ
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: zjc (2015/3/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_GwAddr(const PST_TableStr* tbl, PST_Frame* frm)
{
    ZY_Frame* frame = &frm->_zy;
    
    if(frame->cmd == 0x01)          //������
    {
        MoveBuffer(gs_PstPara.Addr, frame->send, 6);
//	        frame->send[4] = 0;
//	        frame->send[5] = 0;
        frame->len = 6;
        return PST_ERR_OK;
    }
    else
    {
        if(frame->len != 6 + 10)             //�����������ݳ��ȺϷ����ж�
        {
            frame->len = 0;                 //���лظ�֡����������
            frame->send[0]= 0x18;
            frame->send[1]= 0;
            memcpy(frame->send + 2, (uint8_t *)&gs_SysVar.terstt.lword, 4);
            
            frame->len = 6;

            return PST_ERR_FT;
        }
//	        uint8 addrtemp[4];
//	        MoveBuffer(frame->recv, addrtemp + 2, 2);
//	        MoveBuffer(frame->recv + 2, addrtemp, 2);

        if(GD_Para_RW(F251_PADDR, frame->recv, 6, true) == PST_ERR_OK)
        {
//	            ParamLoad_F251();

            memcpy(frame->send, (uint8_t *)&gs_SysVar.terstt.lword, 4);
            
            frame->len = 4;
            
            return PST_ERR_OK;              //�ظ��ɹ�����֡
        }
        
        frame->send[0]= 0x01;
        frame->send[1]= 0;
        memcpy(frame->send + 2, (uint8_t *)&gs_SysVar.terstt.lword, 4);
        
        frame->len = 6;

        return PST_ERR_FT;                  //���ļ�ʧ�ܷ��ش�������֡
    }
}


/************************************************************************
 * @function: ZY_OpenPackage
 * @����: ��ȡ�����òɼ�����ַ
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: zjc (2015/3/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_OpenPackage(const PST_TableStr* tbl, PST_Frame* frm)
{
    DLT_Frame* frame = &frm->_dlt;
    
    if(frame->cmd == 0x01)          //������
    {
        GD_Para_RW(PARA_COPY_INTERVAL, frame->send, 2, false);
//	        MoveBuffer((uint8_t *)&gs_PstPara.cp_interval, frame->send, 2);
        frame->len = 10+2;
        return PST_ERR_OK;
    }
    else
    {
        if(frame->len != 10 + 2)             //�����������ݳ��ȺϷ����ж�
        {
            frame->len = 0;                 //���лظ�֡����������
            return SYS_ERR_NOREPLY;
        }

        
        GD_Para_RW(PARA_COPY_INTERVAL, frame->recv, 2, true);
        uint16_t value = 0; 
        MoveBuffer(frame->recv, (uint8_t *)&value, 2);
        
//	        gs_PstPara.cp_interval
        if(value >= 60)
        {
            gs_PstPara.cp_interval = value/60;
        }
        else
        {
            gs_PstPara.cp_interval = 1;
        }
            
        gs_FarpVar.login = 1;
        frame->len = 0;
        return SYS_ERR_NOREPLY;                  //���ļ�ʧ�ܷ��ش�������֡
    }
}


/************************************************************************
 * @function: ZY_ReWriteTestBard
 * @����: ����ά���ڲ�����
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: zjc (2015/5/6)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_ReWriteTestBard(const PST_TableStr* tbl, PST_Frame* frm)
{
    DLT_Frame* frame = &frm->_dlt;
    
    uint32  testbaud = ByteToLong(frame->recv);
    
    SerialSets ss;                      //���ô���
    ss.databits = DataBits_8bits;       //8λ����λ
    ss.parit = Parit_E;                 //û��У��λ
    ss.stopbits = StopBits_1;           //1λֹͣλ
    ss.baudrate = testbaud;
    gs_Uart.Init(SETP_PORT_NO, &ss);    //��ʼ������  
    
    
    frame->len = 0;
    return PST_ERR_OK;
}






/************************************************************************
 * @function: ZY_OpTime
 * @����: ��д��ǰ���ʱ��,������Ϊ6�ֽ�ʱ�䣬BCD��������ʱ����
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_OpTime(const PST_TableStr* tbl, PST_Frame* frm)
{        
    uint8 buff[7];
    ZY_Frame* frame = &frm->_zy;
    
    if(frame->cmd == 0x01)              //��ȡʱ��
    {
        const TIME* time = GetTime();
        frame->send[0] = ByteHexToBcd(time->sec);
        frame->send[1] = ByteHexToBcd(time->min);
        frame->send[2] = ByteHexToBcd(time->hour);
        frame->send[3] = ByteHexToBcd(time->day);
        frame->send[4] = ByteHexToBcd(time->month);
        frame->send[5] = ByteHexToBcd(time->year);
        frame->len = 6;
        return PST_ERR_OK;
    }
    else if(frame->cmd == 0x04)         //д��ʱ��
    {    
        MoveBuffer(frame->recv, buff, 6); //ת������

        ByteArrayBcdToHex(buff, buff, 6);
        if(gs_RealTime.WriteDateTime((TIME*)buff) == SYS_ERR_OK)
        {
        
            memcpy(frame->send , (uint8_t *)&gs_SysVar.terstt.lword, 4);
            
            frame->len = 4;
            SYS_OK(); 
        }
    }
    frame->send[0]= 0x19;
    frame->send[1]= 0;
    memcpy(frame->send + 2, (uint8_t *)&gs_SysVar.terstt.lword, 4);
    
    frame->len = 6;
    
    return PST_ERR_VAR;
}
extern uint8_t g_ucPutcharEn;
uint8 ZY_OpLog(const PST_TableStr* tbl, PST_Frame* frm)
{        
    uint8 buff[7];
    uint8_t m = 0;
    ZY_Frame* frame = &frm->_zy;
    
    if(frame->cmd == 0x01)              //��ȡʱ��
    {
        frame->send[0] = g_ucPutcharEn;

        frame->len = 1;
        return PST_ERR_OK;
    }
    else if(frame->cmd == 0x04)         //д����־����
    {    
        g_ucPutcharEn = frame->recv[0];

        frame->send[m++] = 0;
        frame->send[m++] = 0;
        memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
        m+=4;
        frame->len = m;
        SYS_OK(); 
    }
    frame->send[0]= 0x19;
    frame->send[1]= 0;
    memcpy(frame->send + 2, (uint8_t *)&gs_SysVar.terstt.lword, 4);
    
    frame->len = 6;
    
    return PST_ERR_VAR;
}


/************************************************************************
 * @function: ZY_SysRunSec
 * @����: ϵͳ��������ʱ��
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_SysRunSec(const PST_TableStr* tbl, PST_Frame* frm)
{
    DLT_Frame* frame = &frm->_dlt;
    LongToByte(frame->send, GetSysRunSecs());
    frame->len = 4;
    return PST_ERR_OK;
}



/************************************************************************
 * @function: ZY_ResetProg
 * @����: ϵͳ��λ
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_ResetProg(const PST_TableStr* tbl, PST_Frame* frm)
{
    //SYS_Reset();
    SYS_OK();
}





extern unsigned char ReadFlsInChip(unsigned char* buffer, unsigned long address, unsigned short length, unsigned char x);
extern unsigned char  WriteFlsInChip(unsigned char * buffer, unsigned long  addr, unsigned short length, unsigned char  x, unsigned char  sdmode);

/************************************************************************
 * @function: Test_Flash
 * @����: flash����
 * @����: 
 * 
 * @����: 
 * @return: uint32   
 * @˵��:
 * @����: yzy (2010-10-25)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint32 Test_Flash(void)
{
    uint8 cmp1[8];                      //ԭʼ����
    uint8 cmp2[8];                      //�޸ĺ�����д������
    uint8 cmp3[8];                      //�޸ĺ�ʵ��д������
    uint32 err = 0;
    
    //Խ��ʹ�õײ㺯��,�Ա�ָ������ĳ��flash
    //uint8 ReadFlsInChip(uint8* buffer, uint32 address, uint16 length, uint8 x);
    //uint8 WriteFlsInChip(uint8* buffer, uint32 addr, uint16 length, uint8 x, uint8 sdmode);
    
    
    for(uint8 k = 0; k < FLS_CHIP_NO; k++)
    {
                                            //��ȡԭʼ����
        if(ReadFlsInChip(cmp1, 0, 5, k) != SYS_ERR_OK)
        {
            err |= Bit_Map8[k];
            break;
        }
                                            //�޸�ԭʼ����
        for(uint8 i = 0; i < 5; i++)
        {
            cmp2[i] = cmp1[i] + 2;
        }
                                            //д������д������
        if(WriteFlsInChip(cmp2, 0, 5, k, 0) != SYS_ERR_OK)
        {
            err |= Bit_Map8[k];
            break;
        }
                                            //��ȡʵ��д������
        if(ReadFlsInChip(cmp3, 0, 5, k) != SYS_ERR_OK)
        {
            err |= Bit_Map8[k];
            break;
        }
        
        WriteFlsInChip(cmp1, 0, 5, k, 0);    //�ָ�ԭʼֵ
        if(CmpBuffer(cmp2, cmp3, 5) != 0)   //�ȶ�����ʵ��ֵ
        {
            err |= Bit_Map8[k];
            break;
        }
    }
    if(err)
        return 1;
    else
        return 0;
}




/************************************************************************
 * @function: Test_LED
 * @����: led����
 * 
 * @����: 
 * @param:  
 * 
 * @����: 
 * @return: uint32  
 * @˵��:
 * @����: yzy (2010-10-25)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint32 Test_LED(void)
{
    SYS_LED_BlinkSetAll(2, 0, 5);//����LED����5��
    return 0;
}


/************************************************************************
 * @function: Test_NetpRead
 * @����: ����ͨ������
 * 
 * @����: 
 * @param: 
 * 
 * @����: 
 * @return: uint32   
 * @˵��:
 * @����: yzy (2010-10-25)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint32 Test_NetpRead(void)
{
    uint8 Netp_HwTest(void);
    if(Netp_HwTest() == SYS_ERR_OK)
    {
        return 0;
    }
    else
    {
        return 1;
    }

}


/************************************************************************
 * @function: Test_Gprs
 * @����: GPRSͨѶ����
 * 
 * @����: 
 * @param: 
 * 
 * @����: 
 * @return: uint32   
 * @˵��:
 * @����: zjc (2015-01-30)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint32 Test_Gprs(void)
{
    uint32 stt = 0;
    if(gs_FarpVar.wmst & WMS_STT_CLN)   //
    {
        stt = 0x03;
    }
    else if(gs_FarpVar.wmst & WMS_STT_GSM)
    {
        stt = 0x02;
    }
    else if(DevModem->stt.bit.chnrdy)
    {
        stt = 0x01;
    }
    else
    {
        stt = 0;
    }
//	    if(gs_FarpVar.mcsq <= 31)
//	    {
//	        stt |= (gs_FarpVar.mcsq << 8);
//	    }

    return stt;
}




uint8 guc_wdtTestFlag = 0;      //����Ƿ���Ҫִ�п��Ź�Ӳ������





/************************************************************************
 * @function: Test_WatchDog
 * @����: ���Ź�����
 * @����: 
 * 
 * @����: 
 * @return: uint32   
 * @˵��:
 * @����: yzy (2010-10-25)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint32 Test_WatchDog(void)
{
    guc_wdtTestFlag = 0xA5;
    return 0;
}


/************************************************************************
 * @function: ZY_InitFlashData
 * @����: ��ʼ��flash����
 * @����: 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2011-3-4)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_InitFlashData(void)
{
    uint8 oc;
                                        //��ʼ����������(����ͨѶ����)
    oc = RstSystemParam(PARA_TYPE_ALL & (~PARA_TYPE_FARP));
    if(oc)
    {
        return SYS_ERR_FT;
    }
    

//	    memset(g_ucMeterPara, 0, 1024);
    
    SYS_OK();
}



/*********************************************************************************************************
** ��������: uint8 ZY_InitFramData(void)
** ��������: �������ݳ�ʼ��
** �䡡��:  ��
** �䡡��:  
**
** ȫ�ֱ���: ��
** ����ģ��: ��
**
** ������: yzy
** �ա���: 2008��4��23��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
//	uint8 ZY_InitFramData(void)
//	{
//	    const S_FRAMDATAS* pfd;
//	    uint8 framno;
//	    uint8 sml;
//	    uint8 oc;
//	    uint8 temp[32];
//	
//	    for(uint8 uc_i = 0; uc_i < SA_FRAMINIT_NUM; uc_i++)
//	    {
//	        pfd = gss_SafeFramDatas + uc_i;
//	        framno = pfd->framno;
//	        sml = pfd->sml;
//	        
//	        for(uint8 uc_k = 0; uc_k < sml + 1; uc_k++, framno++)
//	        {
//	            if(pfd->type == 1)          //���⺯������
//	            {
//	                if(pfd->ford == __NULL)
//	                {
//	                    //DO NOTHING
//	                    oc = SYS_ERR_OK; 
//	                }
//	                else
//	                {
//	                    oc = ((ftype_rstfram)pfd->ford)(framno);
//	                }
//	            }
//	            else                        //����
//	            {
//	                ES_FRAM_Open();
//	                if(pfd->ford == __NULL)
//	                {
//	                    memset(temp, 0, sizeof(temp));
//	                    oc = ES_FRAM_SafeWrite(temp, framno);
//	                }
//	                else
//	                {
//	                    oc = ES_FRAM_SafeWrite((uint8*)pfd->ford, framno);
//	                }
//	                ES_FRAM_Close();
//	            }
//	            _IF_TRUE_RETURN_Z(oc != SYS_ERR_OK, PST_ERR_FT);
//	        }
//	    }
//	    
//	    SYS_OK();
//	}



/************************************************************************
 * @function: WdtTestPreStartPush
 * @����: ׼��Ӳ�����Ե������ϱ�
 *
 * @����: 
 * @param: data �ϱ����ݻ���,����Ϊ__no_init����
 * @param: length �ϱ����ݳ���
 * @param: chninfo �ϱ�ͨ��
 * 
 * @����: 
 * @return: bool true-׼����ϲ����������ϱ�,false-���������ϱ�
 * @˵��:
 * @����: yzy (2010-10-25)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
bool WdtTestPreStartPush(uint8* data, uint8 length, uint8 chninfo)
{
    TWdtTstUp st;
    if(guc_wdtTestFlag == 0xA5)
    {
        guc_wdtTestFlag = 0;
        
        //��ֵ�ϱ��ṹ��
        st.len = length;
        st.crc = DoCrc16(data, length, 0x5555);
        st.chn = chninfo;
        st.key = 0xBBBB;
        
        //�洢�ϱ���Ϣ������
        //SYS_FILE_Open();
        //SYS_FILE_Write((uint8*)&st, FLS_WDTTST_ADDR, sizeof(st));
        //SYS_FILE_Write(data, FLS_WDTTST_ADDR + sizeof(st), length);
        //SYS_FILE_Close();
        
        //ִ��Ӳ�����Ź�����(ֻ�ܴ���Ӳ�����Ź���λ,����ʹ�������λ)
//	        ES_WDT_DIS();
        
        return true;
    }
    else
    {
        guc_wdtTestFlag = 0;
        return false;
    }
}


/************************************************************************
 * @function: WdtTestExeStartPush
 * @����: Ӳ�������龰�µ�,�������ϱ�֡.
 * 
 * @����: 
 * @param: 
 * 
 * @����: 
 * @return: void ���⹫��,��������ʱ����(���ƻ����ͻ���������֮ǰ)
 * @˵��: 
 * @����: yzy (2014/2/9)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void WdtTestExeStartPush(void)
{
    TWdtTstUp st;
    uint8 tmp[64];                      //ע��:��Ҫ�㹻�������в������ϱ�֡,ͬʱ���ǵ����̵߳�ջ�������
    
    //���뿴�Ź����Ա��
    SYS_FILE_Open();
    SYS_FILE_Read((uint8*)&st, FLS_WDTTST_ADDR, sizeof(st));
    SYS_FILE_Close();
    
    if(st.key == 0xBBBB)
    {
        if(st.len <= sizeof(tmp))       //�����������
        {
            //0)�����ϱ�����
            SYS_FILE_Open();
            SYS_FILE_Read(tmp, FLS_WDTTST_ADDR + sizeof(st), st.len);
            SYS_FILE_Close();
            
            //1)У��
            if(st.crc == DoCrc16(tmp, st.len, 0x5555))
            {   //2)����
                gfs_PSTChnSend[st.chn](tmp, st.len);
            }
        }
        
        //3)������Ź���λ��־(��ֹ���ͺ�
        memset((uint8*)&st, 0, sizeof(st));
        SYS_FILE_Open();
        SYS_FILE_Write((uint8*)&st, FLS_WDTTST_ADDR, sizeof(st));
        SYS_FILE_Close();
    }
}



/********************************************************** 
//Ӳ�����Ժ���ԭ��
**********************************************************/
typedef struct
{
    uint8  index;                      //���(0-)
    uint8  bits;
    uint32_t mask;
    uint32 (*func)(void);               //������Ժ���
}THwTest;

//	uint16_t (*extfunc)(uint8_t *, uint16_t, uint32_t);               //���庯��

/********************************************************** 
//���Ժ���ԭ��
**********************************************************/
typedef struct
{
    uint8  index;                      //���(0-)
    uint16_t (*extfunc)(uint8_t id, uint8_t *, uint16_t, uint32_t);               //���庯��
}TExtendFunc;


/**********************************************************
����Ӳ������������.
**********************************************************/
const THwTest gss_HwTest[] = 
{
    {0, 4, 1 << 4,Test_Flash},        //bit0 flash
    {1, 32, 0, Test_LED},          //bit1 led
//	    {2, Test_WatchDog},     //bit2 WDT
//	    {5, Test_NetpRead},     //bit5 ����ͨ��
    {8, 8, 3 << 8,Test_Gprs},         //bit8 GPRSͨ�Ų���

};

uint16_t datatype_bigpig(uint8_t id, uint8_t *pucData, uint16_t usPara1, uint32_t ulPara2)
{
	uint32_t ulAddr = 0;
	uint16_t usLen = 0;
	uint16_t i = 0;
    uint16_t m = 0;

//200	��չ����-���ڴ�
//201	��չ����-��dataflash

//202	��չ����-���¼�������Ϣ




	switch(id)
	{
	case 0://�ڴ�
		ulAddr = pucData[0] + pucData[1] * 0x100 + pucData[2] * 0x10000 + pucData[3] * 0x1000000;

        memcpy(&usLen, pucData+4, 2);
//			usLen = (uint16_t)ulPara2;
        if(usLen > 480)
        {
            usLen = 480;
        }
		pucData[4] = (unsigned char)usLen;
		pucData[5] = (unsigned char)(usLen / 0x100);
        m+=6;
		memcpy(pucData + m, ((unsigned char*)ulAddr), usLen);
        m+=usLen;
		break;


	case 1://dataflash
		ulAddr = pucData[0] + pucData[1] * 0x100 + pucData[2] * 0x10000 + pucData[3] * 0x1000000;

        memcpy(&usLen, pucData+4, 2);
        if(usLen > 480)
        {
            usLen = 480;
        }
		pucData[4] = (unsigned char)usLen;
		pucData[5] = (unsigned char)(usLen / 0x100);
        m+=6;
        SYS_FILE_Open();
        SYS_FILE_Read(pucData+m, ulAddr, usLen);
        SYS_FILE_Close();
        m+=usLen;
		break;

	case 2://�¼�������Ϣ
	    CBB_FILE_Read_Event_Num(DB_FLASH,pucData,pucData+1);
        m+=2;
		break;

	}
    return m;
}

const TExtendFunc gss_ExtFunc[] = 
{
    {0, datatype_bigpig},        //
    {1, datatype_bigpig},        //
    {2, datatype_bigpig},        //
};

extern uint8_t g_ucPutcharEn;

/************************************************************************
 * @function: ZY_Test
 * @����: ��������
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: zjc (2015/3/11)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_Test(const PST_TableStr* tbl, PST_Frame* frm)
{
    ZY_Frame* frame = &frm->_zy;
    uint16_t errCode = 0;
    TIME curTime;
    SYS_MCU_ReadBCDDateTime(&curTime);
    
    int m = 0;
    if(frame->cmd == 0x01)          //������
    {
//	        MoveBuffer(gs_PstPara.Addr, frame->send, 6);
//	        frame->send[4] = 0;
//	        frame->send[5] = 0;
        GD_Para_RW(F251_PADDR, frame->send, 6, false);
        m+=6;

        frame->send[m++] = curTime.year;
        frame->send[m++] = curTime.month;
        frame->send[m++] = curTime.day;
        frame->send[m++] = curTime.hour;
        frame->send[m++] = curTime.min;
        frame->send[m++] = curTime.sec;
        GD_Para_RW(PARA_GPRS, frame->send+m, PARA_GPRS_PLEN, false);
        m+=96;
        GD_Para_RW(PARA_HB, frame->recv+m, PARA_HB_LEN, false);
        m+=2;
        memset(frame->send+m,0,64);
        snprintf(frame->send+m, CON_VER_MAX_LEN-1, "%s%02x.%x)", gucs_softVer, gul_UsrFuncVer>>24, gul_UsrFuncVer%0x100);
        snprintf(frame->send+m+32, CON_VER_MAX_LEN-1, "%s%02x.%x)", gucs_HDVer, gul_UsrFuncVer>>24, gul_UsrHardcVer%0x100);
        m+=64;
        frame->send[m++] = DevModem->stt.bit.signl;
        if((gs_SysVar.terstt.lword & 0xf0000000) != 0)
        {
            frame->send[m++] = 2;
        }
        else
        {
            frame->send[m++] = 1;
        }
        
        gs_SysVar.AI0 = SYS_AD_GetValue(AD_ID_AI0) / 100;
        gs_SysVar.AI1 = SYS_AD_GetValue(AD_ID_AI1) / 100;
        uint32 hwtstret = 0;

        for(uint8 i = 0; i < sizeof(gss_HwTest)/sizeof(THwTest); i++)
        {
            hwtstret = gss_HwTest[i].func();
            gs_SysVar.terstt.lword = (gs_SysVar.terstt.lword & (~gss_HwTest[i].mask)) | (hwtstret << gss_HwTest[i].bits);
        }
//	        memcpy(frame->send + m, (uint8_t *)&errCode, 2);
//	        m+=2;
        memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 6);
        m+=6;

        frame->len = m;
        
        //gs_SysVar.mLPstt &= ~HLV_LPTASK_TST;
//	        gs_GPIO.GPO_Out(GPO_485_PWR, false);
        Netp_PowerMng(0, NETP_PORT_PWR_485);
        
        return PST_ERR_OK;
    }
    else
    {
        gs_SysVar.mLPstt |= HLV_LPTASK_TST;
#ifndef DEBUG
        g_ucPutcharEn = 0;
 #endif
        if(GD_Para_RW(F251_PADDR, frame->recv, 6, true) != PST_ERR_OK)
        {
            errCode = ERR_HARDFAULT_WRITE;

            goto ERR_TESE;
        }
        m+=6;
        TIME curTime;
        curTime.year    = frame->recv[m++];
        curTime.month   = frame->recv[m++];
        curTime.day    = frame->recv[m++];
        curTime.hour    = frame->recv[m++];
        curTime.min     = frame->recv[m++];
        curTime.sec     = frame->recv[m++];
        if(SYS_ERR_OK == SYS_MCU_WriteBCDDateTime(&curTime))
        {
            gs_SysVar.terstt.bit.rtccheck = 0;
            
            gs_SysVar.terstt.bit.rtcstt = 0;
        }
        else
        {
            gs_SysVar.terstt.bit.rtccheck = 1;
            gs_SysVar.terstt.bit.rtcstt = 1;
            
        }

        if(GD_Para_RW(PARA_GPRS, frame->recv+m, PARA_GPRS_PLEN, true) != PST_ERR_OK)

        {
            errCode = ERR_HARDFAULT_WRITE;
            goto ERR_TESE;
        }
        m+=PARA_GPRS_PLEN;
        
        if(GD_Para_RW(PARA_HB, frame->recv+m, PARA_HB_LEN, true) != PST_ERR_OK)
        {
            errCode = ERR_HARDFAULT_WRITE;

            goto ERR_TESE;
        }
        m+=PARA_HB_LEN; 
        if(RstPara(0) != SYS_ERR_OK)
        {
            
            errCode = ERR_HARDFAULT_WRITE;
            goto ERR_TESE;
        }
        aos_msleep(5000);

        //DevModem->stt.bit.signl

        

//	        gs_GPIO.GPO_Out(GPO_485_PWR, true);
        Netp_PowerMng(1, NETP_PORT_PWR_485);
        
        gs_SysVar.terstt.bit.chk4851 = 0;
        gs_SysVar.terstt.bit.chk4852 = 0;
        gs_SysVar.terstt.bit.chk4853 = 0;
        gs_SysVar.terstt.bit.chk4854 = 0;
        
        if(frame->recv[m++] == 1)
        {
            gs_OS.Message_Send(&gs_TKNetpQueue, &msgidA[MSG_NETP_CHECK], 1);
        }
        
        if(frame->recv[m++] == 1)
        {
        
            gs_OS.Message_Send(&gs_TKNetp2Queue, &msgidA[MSG_NETP_CHECK], 1);
        }
        if(frame->recv[m++] == 1)
        {
            gs_OS.Message_Send(&gs_TKNetp3Queue, &msgidA[MSG_NETP_CHECK], 1);
        }
        if(frame->recv[m++] == 1)
        {
            gs_OS.Message_Send(&gs_TKNetp4Queue, &msgidA[MSG_NETP_CHECK], 1);
        }
        
        gs_OS.Message_Send(&gs_TKSlvQueue, &msgidA[MSG_FARP_CHECK], 1);
        gs_SysVar.terstt.bit.blecheck = 1;
        gs_OS.Message_Send(&gs_MainQueue, &msgidA[MSG_MAIN_BLE_CHK], 1);
        
        m=0;
        frame->send[m++]= 0;
        frame->send[m++]= 0;
        
        memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 6);
        m+=6;
        frame->len = m;

        return PST_ERR_OK;                  //���ļ�ʧ�ܷ��ش�������֡
    }
ERR_TESE:
    m = 0;
    memcpy(frame->send + m, (uint8_t *)&errCode, 2);
    m+=2;
    memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 6);
    m+=6;
//	    frame->send[m++] = 0;
//	    frame->send[m++] = 0;
    frame->len = m;

    return PST_ERR_FT;
}


/************************************************************************
 * @function: ZY_HwTest
 * @����: ��������
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_HwTest(const PST_TableStr* tbl, PST_Frame* frm)
{
    DLT_Frame* frame = &frm->_dlt;
    uint8 smp[8] = {0};
    uint8 num = 0;                      //ʵ�ʲ����������
    
    if(frame->len != (2 + 8))           //������̫��
    {
        frame->len = 0;
        return SYS_ERR_FT;
    }
                                        //׼����������
    for(uint8 i = 0; i < sizeof(gss_HwTest)/sizeof(THwTest); i++)
    {
        if(STR_IsBitSet(frame->recv, gss_HwTest[i].index))
        {
            LongToByte(frame->send + 8 + (num << 2), gss_HwTest[i].func());
            STR_SetBit(smp, gss_HwTest[i].index);
            num++;
        }
    }
                                        //λͼ
    MoveBuffer(smp, frame->send, 8);
    frame->len = 8 + (num << 2);
                                        //
    SYS_OK();
}

/************************************************************************
 * @function: UserVarInit
 * @����: �ָ���������
 * 
 * @����: 
 * @param: tbl 
 * @param: frame 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 UserVarInit(void)
{
    uint8 oc;

    oc = ZY_InitFlashData();
    _IF_TRUE_RETURN_Z(oc != SYS_ERR_OK, PST_ERR_FT);
//	    oc = ZY_InitFramData();
//	    _IF_TRUE_RETURN_Z(oc != SYS_ERR_OK, PST_ERR_FT);
    SYS_OK();

}

/************************************************************************
 * @function: RstPara
 * @����: �ؽ��ļ�ϵͳ,���ָ�����Ĭ�ϲ���
 * 
 * @����: 
 * @param: mode mode (true��ͨѶ����,false����ͨѶ����)
 * 
 * @����: 
 * @return: uint8  SA_ERR_OK,SA_ERR_FT
 * @˵��: 
 * @����: yzy (2014/2/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 RstPara(bool mode)
{                                       //�ͻ���
    uint8 ret = SYS_ERR_OK;
    if(UserVarInit() != SYS_ERR_OK)
    {
        ret = SYS_ERR_FT;
    }
    
    if(mode)                            //��ʼ��ͨѶ����
    {
        RstSystemParam(PARA_TYPE_FARP);
    }
                                        //���Ͳ���������Ϣ
//	    gs_OS.Message_Send(MSG_HLV_FARP, TASK_SLV_TKID);
//	    gs_OS.Message_Send(MSG_HLV_TASK, TASK_SLV_TKID);
//	    gs_OS.Message_Send(MSG_HLV_INP,  TASK_SLV_TKID);
//	    gs_OS.Message_Send(MSG_HLV_PST,  TASK_SLV_TKID);
    //a.���������¼�
    gs_OS.Message_Send(&gs_TKHlvQueue, &msgidA[MSG_HLV_CLR], 1);
    gs_OS.Message_Send(&gs_TKHlvQueue, &msgidA[MSG_ALRT_CLR], 1);
//	    gs_OS.Message_Send(&gs_MainQueue, &msgidA[MSG_HLV_CLR], 1);
//	
//	    gs_OS.Message_Send(&gs_TKSlvQueue, &msgidA[MSG_HLV_CLR], 1);
                                
                                //b.��¼��ʼ���¼�
    gs_OS.Message_Send(&gs_TKHlvQueue, &msgidA[MSG_ALRT_ERC1], 1);
                                //��ʼ��������Ŀ0������
    MoveBuffer((uint8*)&GetTime()->sec, gs_FramMisData.tdt, 6);
    InvBuffer(gs_FramMisData.tdt, 6);
    gs_FramMisData.ver = (uint32)gul_UsrFuncVer;
    gs_FramMisData.e1cnt = 0;
    gs_FramMisData.e1pt = 0;
    gs_FramMisData.rcnt = 0;
    gs_FramMisData.rpt = 0;

    gs_SysVar.mLPsysstt = 0;        //��ǰΪͣ��״̬
    gs_SysVar.mDGcnt = 0;                   //��ǰ������Ϊ5
    gs_SysVar.terstt.lword = 0;
    gs_SysVar.mLPstt = 0;
    extern const ST_Netp_FW* const gss_NetpFrameFw[];
    for(int i = 0; i < Netp_Get_FW_num(); i++)
    {
        Netp_Register_Set(gss_NetpFrameFw[i]);
    }    
    extern const uint8_t netp_check[4];
    GD_Para_RW(NETP_CHECK, netp_check, 4, true);
    Netp_Register_Init();


    gs_OS.Message_Send(&gs_TKHlvQueue, &msgidA[MSG_HLV_FARP], 1);
//	    gs_OS.Message_Send(&gs_TKHlvQueue, &msgidA[MSG_HLV_TASK], 1);
    gs_OS.Message_Send(&gs_TKHlvQueue, &msgidA[MSG_HLV_INP], 1);
    gs_OS.Message_Send(&gs_TKHlvQueue, &msgidA[MSG_HLV_PST], 1);

    return ret;
}

/************************************************************************
 * @function: ZY_UserVarInit
 * @����: �ָ���������
 * 
 * @����: 
 * @param: tbl 
 * @param: frame 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_UserVarInit(const PST_TableStr* tbl, PST_Frame* frm)
{
    DLT_Frame* frame = &frm->_dlt;
    UserVarInit();           //�û����ݳ�ʼ��
//	    RstSystemParam(PARA_TYPE_FARP);//ͨѶ������ʼ��
    
    uint8 stt[32];
    ClearBuffer(stt, sizeof(stt));
//	    ES_FRAM_Open();
//	    ES_FRAM_SafeWrite(stt, FM_PARASTT);
//	    ES_FRAM_Close();
    
    frame->len = 0;
    SYS_OK();

}
/************************************************************************
 * @function: ZY_HxNkVer
 * @����: �ڿذ汾��
 * 
 * @����: 
 * @param: tbl 
 * @param: frm 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_HxNkVer(const PST_TableStr* tbl, PST_Frame* frm)
{
    DLT_Frame* frame = &frm->_dlt;
    extern const uint8 DefParaC1F21F22[20];
    MoveBuffer((uint8*)DefParaC1F21F22, frame->recv, sizeof(DefParaC1F21F22));
    InvBuffer(frame->recv, sizeof(DefParaC1F21F22));
    frame->len = sizeof(DefParaC1F21F22);
    SYS_OK();
    
}


/*********************************************************************************
* Function: ���������
* Description:    
* Calls:          
* Called By:      
* Data Accessed: 
* Data Updated:  
* Input:      
* Output:         
* Return:         
* Others:         
*******************************************************************************/
unsigned char GetMeterNum(void)
{
	unsigned char uci;
	unsigned char ucLegalMeterCount;
	unsigned char ucPnType;

    uint8_t type = 0;
	ucLegalMeterCount = 0;

	for(uci = 0; uci < CORE_TST_NO; uci ++)
	{
        GD_Para_RW(F10_PADDR + LEN_TST_BMPS + LEN_TST_CFG * uci, &type, 1, false);
		if(type != 0 && type <= 5)
		{
		    ucLegalMeterCount++;  
		}
              
	}

	return ucLegalMeterCount;

}

/*********************************************************************************
* Function: 
* Description:    
* Calls:          
* Called By:      
* Data Accessed: 
* Data Updated:  
* Input:      
* Output:         
* Return:         
* Others:         
*******************************************************************************/
uint8_t GetAddrByDev(uint8_t dev)
{
	unsigned char uci;

	for(uci = 0; uci < CORE_TST_NO; uci ++)
	{
	    if(gss_CDATATst[uci].info.main_addr == dev)
        {
            return gss_CDATATst[uci].info.addr;
        }   
              
	}

	return 0xff;

}

unsigned char GetPrepayNum(void)
{
	unsigned char uci;
	unsigned char ucLegalMeterCount;
	unsigned char ucPnType;

    uint8_t type = 0;
	ucLegalMeterCount = 0;

	for(uci = 0; uci < CORE_TST_NO; uci ++)
	{
        GD_Para_RW(F10_PADDR + LEN_TST_BMPS + LEN_TST_CFG * uci, &type, 1, false);
		if(type != 0)
		{
		    ucLegalMeterCount++;  
		}
              
	}

	return ucLegalMeterCount;

}
extern ST_Netp_Reg gs_netpRegMap[CON_MAX_DEVICE_TYPES];
/************************************************************************
 * @function: ZY_ReadSupportNetpFW
 * @����: ��ȡ�ն�֧�ֵ�����������
 * 
 * @����: 
 * @param: frame 
 * @param: pinfo 
 * @param: extra 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: (flash�ṹ:LEN_TST_BMPS�ֽ����λͼ + num������[2B��� + 2B������� + ...])
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_ReadSupportNetpFW(const PST_TableStr* tbl, PST_Frame* frm)
{
    uint8* pts;
    uint32 base_addr = F10_PADDR + LEN_TST_BMPS;         //��������ַ
    uint32 addr;                            //��ַ
    uint8 xh;                              //���
    uint16 num;                             //����
    uint8 max = CORE_TST_NO ;            //������
    uint8 k,m;
    uint8 bmps[LEN_TST_BMPS];               //���λͼ
    
    ZY_Frame* frame = &frm->_zy;
    //int m = 0;
    uint8_t sum = 0;
                                            //��FLASH��ȡλͼ
                                            //������\��ȡ������
    num = frame->recv[0];//ByteToInt(extra->rbuf);
    if(num > max)
    {
        num = max;
    }
    if(frame->cmd == 0x01)  
    {
        uint8  exnum = 0;               //ʵ�ʷ��ص�����
        pts = frame->send + 1;
        for(int i = 0; i < CON_MAX_DEVICE_TYPES; i++)
        {

            if(gs_netpRegMap[i].pf == 0 && gs_netpRegMap[i].device_name[0] == 0)
                break;
            memcpy(pts, (uint8_t *)&gs_netpRegMap[i].device_name[0], 4);
            exnum++;
            pts += 4;  //ָ��
        }

        frame->send[0]= exnum;
                                        //����ֵ
        frame->len = (4 * exnum + 1);
    }
    else
    {
        SYS_ERR();
    }
    SYS_OK();
}
/************************************************************************
 * @function: ParaProcF10
 * @����: F10�������������
 * 
 * @����: 
 * @param: frame 
 * @param: pinfo 
 * @param: extra 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: (flash�ṹ:LEN_TST_BMPS�ֽ����λͼ + num������[2B��� + 2B������� + ...])
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ParaProcMeterInfo(const PST_TableStr* tbl, PST_Frame* frm)
{
    uint8* pts;
    uint32 base_addr = F10_PADDR + LEN_TST_BMPS;         //��������ַ
    uint32 addr;                            //��ַ
    uint8 xh;                              //���
    uint16 num;                             //����
    uint8 max = CORE_TST_NO ;            //������
    uint8 k,m;
    uint8 bmps[LEN_TST_BMPS];               //���λͼ
    
    ZY_Frame* frame = &frm->_zy;
    //int m = 0;
    uint8_t sum = 0;
                                            //��FLASH��ȡλͼ
                                            //������\��ȡ������
    num = frame->recv[0];//ByteToInt(extra->rbuf);
    if(num > max)
    {
        num = max;
    }
    if(frame->cmd == 0x01)  
    {
        uint8  exnum = 0;               //ʵ�ʷ��ص�����
        uint8_t meter_num = GetMeterNum();
        STMeterInfo meterInfo_tmp;
        pts = frame->send + 1;
        for(k = 0; k < CORE_TST_NO; k++)
        {

            GD_Para_RW(base_addr+LEN_TST_CFG * k, (uint8_t *)&meterInfo_tmp, LEN_TST_CFG, false);
//	            memcpy((uint8_t *)&meterInfo_tmp, g_ucMeterPara + LEN_TST_CFG * k, LEN_TST_CFG);
            if(meterInfo_tmp.type != 0 && meterInfo_tmp.type <= 5)
            {
                memcpy(pts, (uint8_t *)&meterInfo_tmp, LEN_TST_CFG);
                exnum++;
                pts += LEN_TST_CFG;  //ָ��
            }
        }
                                        //��������
        frame->send[0]= exnum;
                                        //����ֵ
        frame->len = (LEN_TST_CFG * exnum + 1);
    }
    else                 //����
    {                                       //����ֵ
//	        extra->len = (LEN_TST_CFG * num) + 2;
        if(GD_Para_RW(F10_PADDR, (uint8*)bmps, LEN_TST_BMPS, false) != SYS_ERR_OK)
        {
            return SYS_ERR_FT;
        }

        for(k = 0; k < num; k++)
        {                                   //��ȡ���\���
            xh = frame->recv[2 + LEN_TST_CFG * k] & 0x0f;
                                            //�жϺϷ���
            if((xh > max) || (xh == 0))
            {
                frame->send[0]= 0x1b;
                frame->send[1]= 0;
                memcpy(frame->send + 2, (uint8_t *)&gs_SysVar.terstt.lword, 4);
                
                frame->len = 6;
                return SYS_ERR_FT;
            }

            //��������(������1�����׶�)
            addr = base_addr + LEN_TST_CFG * (xh - 1);
            GD_Para_RW(addr, frame->recv + 1 + LEN_TST_CFG * k, LEN_TST_CFG, true);

            frame->send[1+k] = 0;
            
            STR_SetBits(bmps, 1, xh - 1, 1);
//	            memcpy(g_ucMeterPara + addr, frame->recv + 1 + LEN_TST_CFG * k, LEN_TST_CFG);
        }
        
        frame->send[0]= GetMeterNum();
                                        //����ֵ
        frame->len = 1+num;
        GD_Para_RW(F10_PADDR, bmps, LEN_TST_BMPS, true);

    }

    SYS_OK();
}

uint8 ParaProcMeterInfoDel(const PST_TableStr* tbl, PST_Frame* frm)
{
    uint32 base_addr = F10_PADDR + LEN_TST_BMPS;         //��������ַ
    uint32 addr;                            //��ַ
    uint8 xh;                              //���
    uint8 max = CORE_TST_NO;            //������
    uint8 k,m=0;
    ZY_Frame* frame = &frm->_zy;
    //int m = 0;
    uint8_t sum = 0;
    uint8_t devaddr = 0;
    uint8_t type = 0;
    uint16_t err = 0x1C;//ɾ�����豸��ַ�����ڵ�
    uint8 bmps[LEN_TST_BMPS];               //���λͼ

    if(GD_Para_RW(F10_PADDR, (uint8*)bmps, LEN_TST_BMPS, false) != SYS_ERR_OK)
    {
        return SYS_ERR_FT;
    }

    devaddr = frame->recv[0] & 0x0f;//ByteToInt(extra->rbuf);
    if(devaddr > max)
    {
        goto ERR_DEL;
    }

    if(frame->cmd == 0x05)                  //ɾ��
    {                                       //����ֵ
//	        extra->len = (LEN_TST_CFG * num) + 2;
        addr = base_addr + LEN_TST_CFG * (devaddr - 1);
//	        memcpy(g_ucMeterPara + addr, &type, 1);
        gs_SysVar.terstt.lword &= ~(0x1 << (16 + (devaddr - 1)));
        GD_Para_RW(addr, &type, 1, true);
                                        //��������
        frame->send[m++]= GetMeterNum();
        memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
        m+=4;
        frame->len = m;
        
        STR_SetBits(bmps, 1, devaddr - 1, 1);
        GD_Para_RW(F10_PADDR, bmps, LEN_TST_BMPS, true);
        
        SYS_OK();
    }
    else
    {
        goto ERR_DEL;
    }

ERR_DEL:   
    m=0;
    memcpy(frame->send + m, (uint8_t *)&err, 2);
    m+=2;

    memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
    m+=4;
    frame->len = m;

    SYS_ERR();

    
}
/************************************************************************
 * @function: ParaPrepayInfo
 * @����: �����豸������Ԥ���Ѳ�Ʒ��
 * 
 * @����: 
 * @param: frame 
 * @param: pinfo 
 * @param: extra 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: (flash�ṹ:LEN_TST_BMPS�ֽ����λͼ + num������[2B��� + 2B������� + ...])
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
 
ST_Netp_FW gs_netp_fw_trans[MCB_PORT_NUM];

uint8 ParaPrepayInfo(const PST_TableStr* tbl, PST_Frame* frm)
{
    uint8* pts;
    uint32 base_addr = PREPAYPARA_PADDR;         //��������ַ
    uint32 addr;                            //��ַ
    uint8 xh;                              //���
    uint16 num;                             //����
    uint8 max = CORE_TST_NO ;            //������
    uint8 k,m = 0;
    uint8_t timetmp[6];
    ZY_Frame* frame = &frm->_zy;
    uint8_t sum = 0;
    ST_ACT_INPBUF_L    inLbuf;
    ST_RDMT_CMD stDlmscmd;
    
//	    gs_GPIO.GPO_Out(GPO_485_PWR, true);
    Netp_PowerMng(1, NETP_PORT_PWR_PREPAY);
    
    
                                            //��FLASH��ȡλͼ
                                            //������\��ȡ������
    if(frame->cmd == 0x01)  
    {
        uint8  exnum = 0;               //ʵ�ʷ��ص�����
        uint8_t meter_num = 0;// = GetPrepayNum();
        STMeterInfo meterInfo_tmp;
        STPrepayDevInfo meterPrepayInfo;

        m++;//����λ��Ԥ��
        const TIME* time = GetTime();
        frame->send[m++] = ByteHexToBcd(time->year);
        frame->send[m++] = ByteHexToBcd(time->month);
        frame->send[m++] = ByteHexToBcd(time->day);
        frame->send[m++] = ByteHexToBcd(time->hour);
        frame->send[m++] = ByteHexToBcd(time->min);        
        frame->send[m++] = ByteHexToBcd(time->sec);
//	        frame->len = 6;
        
        pts = frame->send + 1;
        
//	        memcpy(pts, time, 6);
//	        pts+=6;
//	        m+=7;
        for(k = 0; k < CORE_TST_NO; k++)
        {
//	            GD_Para_RW(F10_PADDR+LEN_TST_BMPS+LEN_TST_CFG * k, (uint8_t *)&meterInfo_tmp, LEN_TST_CFG, false);
//	            memcpy((uint8_t *)&meterInfo_tmp, g_ucMeterPara + LEN_TST_CFG * k, LEN_TST_CFG);
            if(!(gss_CDATATst[k].info.type == 0x03 || gss_CDATATst[k].info.type == 0x02))
            {
//	                memcpy(pts, g_ucPrepay + 6 + LEN_TST_CFG * k, LEN_PREPAY_CFG);
//	                exnum++;
//	                pts += LEN_PREPAY_CFG;  //ָ��
                continue;
            }
            else
            {
            
                
                ST_Netp_FW * fw = &gs_netp_fw_trans[gss_CDATATst[k].pt];
                uint8_t ret = Netp_Get_Framework(gss_CDATATst[k].pf, fw);
                if(SYS_ERR_OK == ret && fw->pf < CON_MAX_DEVICE_TYPES)
                {
                    uint8_t mlen[2];
                    memcpy((uint8_t *)&fw->frameDtaStr[0], (uint8_t *)&gs_NetpFrameWXICPrepayRDataStr, sizeof(NetpFrameDataStr));

                    stDlmscmd.port = gss_CDATATst[k].pt;
                    stDlmscmd.addr = gss_CDATATst[k].info.main_addr;  
                    stDlmscmd.addr_len = 1;
//                    stDlmscmd.fw = fw;
                    stDlmscmd.baud_cfg = gss_CDATATst[k].bs;
                    stDlmscmd.di = 0;
                    fw->frameDtaStr[0].item[0].mult = k;
                    mlen[0] = 0;
                    mlen[1] = 0x12;
                    
                    uint8_t err = Netp_Send_With_Retry2(&inLbuf, mlen, &stDlmscmd);
                    if((err == NETP_ERR_OK))
                    {
                        memset(frame->send + m, 0, LEN_PREPAY_CFG  );
                        frame->send[m] = gss_CDATATst[k].info.addr & 0x0f;
                        memcpy(frame->send + m + 1, gss_CDATATst[k].info.factory_model, 4  );
                        memcpy(frame->send + m + 5, inLbuf.buf + 1, inLbuf.len - 1);
                        m+=LEN_PREPAY_CFG;
//	                        frame->send[m++]= 0x06;
//	                        frame->send[m++]= 0;
                    }
                    else
                    {
//	                        frame->len = 0;
//	                        SYS_ERR();
                        continue;
                    }
                    
                    exnum++;
                }
                else
                {
//	                    frame->len = 0;
//	                    SYS_ERR();
                    continue;
                }                
            }
        }
                                        //��������
        frame->send[0]= exnum;
                                        //����ֵ
//	        memcpy(frame->send+1, time, 6);                                        
        frame->len = (LEN_PREPAY_CFG * exnum + 7);
    }
    else                 //����
    {                                      
//	        uint8_t * time = NULL;
        num = frame->recv[0];//ByteToInt(extra->rbuf);
        memcpy(timetmp, frame->recv + 1, 6);
        if(num > max)
        {
            num = max;
        }
        
        frame->send[m++] = num;
        m+=6;

        for(k = 0; k < num; k++)
        {                                   //��ȡ���\���
            xh = frame->recv[7 + LEN_PREPAY_CFG * k] & 0x0f;
            if((xh > max) || (xh == 0) || gss_CDATATst[xh-1].pt >= MCB_PORT_NUM 
                || gss_CDATATst[xh-1].info.type != 0x03 )
            {
                frame->send[m++]= 0x06;
                frame->send[m++]= 0;
            }
            else
            {

                ST_Netp_FW * fw = &gs_netp_fw_trans[gss_CDATATst[xh-1].pt];
                addr = base_addr + LEN_PREPAY_CFG * (xh - 1);
                GD_Para_RW(addr, frame->recv+7 + LEN_PREPAY_CFG * k, LEN_PREPAY_CFG, true);
                
                uint8_t ret = Netp_Get_Framework(gss_CDATATst[xh-1].pf, fw);
                if(SYS_ERR_OK == ret && fw->pf < CON_MAX_DEVICE_TYPES)
                {
                    memcpy((uint8_t *)&fw->frameDtaStr[0], (uint8_t *)&gs_NetpFrameWXICPrepayWDataStr, sizeof(NetpFrameDataStr));

                    stDlmscmd.port = gss_CDATATst[xh-1].pt;
                    stDlmscmd.addr = gss_CDATATst[xh-1].info.main_addr;  
                    stDlmscmd.addr_len = 1;
//                    stDlmscmd.fw = fw;
                    stDlmscmd.baud_cfg = gss_CDATATst[xh-1].bs;
                    stDlmscmd.di = 0;
                    fw->frameDtaStr[0].item[0].mult = xh - 1;
                    uint8_t err = Netp_Send_With_Retry2(&inLbuf, NULL, &stDlmscmd);
                    if((err == NETP_ERR_OK) || (err == NETP_ERR_NT))
                    {
                        memcpy(frame->send + m, inLbuf.buf + 1, 2);
                        m+=2;
//	                        const uint8_t wprepay_s[] = {0x02,0,0};
//	                        if(0 == memcmp(inLbuf.buf, wprepay_s, 3))
//	                        {
//	                            frame->send[m++]= 0;
//	                            frame->send[m++]= 0;
//	                        }
//	                        else
//	                        {
//	                            frame->send[m++]= 0x06;
//	                            frame->send[m++]= 0;
//	                        }
                    }
                    else
                    {
                        frame->send[m++]= 0x06;
                        frame->send[m++]= 0;
                    }
                }
                else
                {
                    frame->send[m++]= 0x06;
                    frame->send[m++]= 0;
                }
                LOG_INFO("write PrepayInfo dev[%d] ret=%02x %02x %02x\n", k+1, inLbuf.buf[0],inLbuf.buf[1],inLbuf.buf[2]);
            }

            //��������(������1�����׶�)
//	            memcpy(g_ucMeterPara + addr, frame->recv + 1 + LEN_PREPAY_CFG * k, LEN_PREPAY_CFG);
        }
        
//	        frame->send[0]= 0;
//	                                        //����ֵ
//	        frame->len = 1;
//	        return SYS_ERR_NOREPLY;
        
        memcpy(frame->send+1, timetmp, 6);
        
        frame->len = m;
    }
//	    gs_GPIO.GPO_Out(GPO_485_PWR, false);
    Netp_PowerMng(0, NETP_PORT_PWR_PREPAY);
    
    SYS_OK();
}
/************************************************************************
 * @function: ParaRechargeInfo
 * @����: ��ֵ
 * 
 * @����: 
 * @param: frame 
 * @param: pinfo 
 * @param: extra 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: (flash�ṹ:LEN_TST_BMPS�ֽ����λͼ + num������[2B��� + 2B������� + ...])
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ParaRechargeInfo(const PST_TableStr* tbl, PST_Frame* frm)
{
    uint8* pts;
    uint32 base_addr = RECHARGE_PADDR;         //��������ַ
    uint32 addr;                            //��ַ
    uint8 xh;                              //���
    uint16 num;                             //����
    uint8 max = CORE_TST_NO ;            //������
    uint8 k,m = 0;
    ZY_Frame* frame = &frm->_zy;
    uint8_t exnum = 0;

    ST_ACT_INPBUF_L    inLbuf;
    ST_RDMT_CMD stDlmscmd;
    Netp_PowerMng(1, NETP_PORT_PWR_RECHARGE);
    if(frame->cmd == 0x01)  
    {                                      
        uint8_t * time = NULL;
        num = frame->recv[0];//ByteToInt(extra->rbuf);
        time = frame->recv + 1;
        if(num > max)
        {
            num = max;
        }
        exnum = 0;
        m+=7;
        

        for(k = 0; k < num; k++)
        {                                   //��ȡ���\���
            xh = frame->recv[7 + LEN_RECHARGE_INFO * k] & 0x0f;
            
            if((xh > max) || (xh == 0) || gss_CDATATst[xh-1].pt >= MCB_PORT_NUM 
                || gss_CDATATst[xh-1].info.type != 0x03 )
            {
                //�������
                break;
            }
            else
            {

                ST_Netp_FW * fw = &gs_netp_fw_trans[gss_CDATATst[xh-1].pt];
                addr = base_addr + LEN_PREPAY_CFG * (xh - 1);
                GD_Para_RW(addr, frame->recv+7 + LEN_PREPAY_CFG * k, LEN_PREPAY_CFG, true);
                
                uint8_t ret = Netp_Get_Framework(gss_CDATATst[xh-1].pf, fw);
                if(SYS_ERR_OK == ret && fw->pf < CON_MAX_DEVICE_TYPES)
                {
                    uint8_t tmp[10];
                    memcpy((uint8_t *)&fw->frameDtaStr, (uint8_t *)&gs_NetpFrameWXICRechargeWDataStr, sizeof(NetpFrameDataStr));

                    stDlmscmd.port = gss_CDATATst[xh-1].pt;
                    stDlmscmd.addr = gss_CDATATst[xh-1].info.main_addr;  
                    stDlmscmd.addr_len = 1;
//                    stDlmscmd.fw = fw;
                    stDlmscmd.baud_cfg = gss_CDATATst[xh-1].bs;
                    stDlmscmd.di = 0;
                    memset(tmp,0,10);
                    memcpy(tmp, frame->recv+7+ LEN_PREPAY_CFG*k + 5, 10);
                    uint8_t err = Netp_CommonRead(&inLbuf, tmp, &stDlmscmd);
                    if((err == NETP_ERR_OK))
                    {
                        memset(frame->send + m, 0, LEN_RECHARGE_INFO );
                        frame->send[m] = gss_CDATATst[xh-1].info.addr & 0x0f;
                        memcpy(frame->send + m + 1, gss_CDATATst[xh-1].info.factory_model, 4  );
                        memcpy(frame->send + m + 5, inLbuf.buf+1, inLbuf.len-1);
                        m+=LEN_RECHARGE_INFO;
                        exnum++;
                    }
                    else
                    {
                        break;
                    }
                    
                }
                else
                {
                    break;
                }
            }
        }
        
        frame->send[0] = exnum;
        memcpy(frame->send+1, time, 6);
        
        frame->len = m;
    }
    Netp_PowerMng(0, NETP_PORT_PWR_RECHARGE);

    SYS_OK();
}

//	uint8 ParaRechargeInfo(const PST_TableStr* tbl, PST_Frame* frm)
//	{
//	    uint8* pts;
//	    uint32 base_addr = 0;         //��������ַ
//	    uint32 addr;                            //��ַ
//	    uint8 xh;                              //���
//	    uint16 num;                             //����
//	    uint8 max = CORE_TST_NO ;            //������
//	    uint8 k,m = 0;
//	    ZY_Frame* frame = &frm->_zy;
//	    //int m = 0;
//	    uint8_t sum = 0;
//	    uint8_t * time = NULL;
//	                                            //��FLASH��ȡλͼ
//	                                            //������\��ȡ������
//	    num = frame->recv[0];//ByteToInt(extra->rbuf);
//	    time = frame->recv + 1;
//	    if(num > max)
//	    {
//	        num = max;
//	    }
//	    if(frame->cmd == 0x01)  
//	    {
//	        uint8  exnum = 0;               //ʵ�ʷ��ص�����
//	        uint8_t meter_num = GetPrepayNum();
//	        STMeterInfo meterInfo_tmp;
//	        STChargeInfo meterRechargeInfo;
//	        STChargeret meterRechargeRet;
//	        pts = frame->send + 1;
//	        
//	        memcpy(pts, g_ucPrepay, 6);
//	        pts+=6;
//	        for(k = 0; k < CORE_TST_NO; k++)
//	        {
//	
//	//              GD_Para_RW(LEN_TST_CFG * k, (uint8_t *)&meterInfo_tmp, LEN_TST_CFG, false);
//	            memcpy((uint8_t *)&meterInfo_tmp, g_ucMeterPara + LEN_TST_CFG * k, LEN_TST_CFG);
//	            if(meterInfo_tmp.type != 0)
//	            {
//	                memcpy(&meterRechargeInfo, g_ucPrepay + 6 + LEN_TST_CFG * k, LEN_PREPAY_CFG);
//	                memcpy(&meterRechargeRet,&meterRechargeInfo, 5);
//	                meterRechargeRet.errno = 0;
//	                memcpy(&meterRechargeRet.recharge[0],&meterRechargeInfo.recharge[0], 10);
//	                meterRechargeRet.meterstt = gs_SysVar.terstt.lword;
//	                memcpy(pts, (uint8_t *)&meterRechargeRet, LEN_PREPAY_CFG);
//	
//	                exnum++;
//	                pts += LEN_PREPAY_CFG;  //ָ��
//	            }
//	        }
//	                                        //��������
//	        frame->send[0]= exnum;
//	                                        //����ֵ
//	        frame->len = (LEN_PREPAY_CFG * exnum + 7);
//	    }
//	    else                 //����
//	    {                                       //����ֵ
//	//	        extra->len = (LEN_TST_CFG * num) + 2;
//	
//	        for(k = 0; k < num; k++)
//	        {                                   //��ȡ���\���
//	            xh = frame->recv[7 + LEN_RECHARGE_INFO * k] & 0x0f;
//	                                            //�жϺϷ���
//	            if((xh > max) || (xh == 0))
//	            {
//	                frame->send[m++]= 0x1b;
//	                frame->send[m++]= 0;
//	                
//	                memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
//	                m+=4;
//	                
//	                frame->len = m;
//	                return SYS_ERR_FT;
//	            }
//	
//	            //��������(������1�����׶�)
//	            addr = base_addr + LEN_RECHARGE_INFO * (xh - 1);
//	            memcpy(g_ucMeterPara + addr, frame->recv + 1 + LEN_RECHARGE_INFO * k, LEN_RECHARGE_INFO);
//	        }
//	        
//	//	        frame->send[0]= 0;
//	//	                                        //����ֵ
//	//	        frame->len = 1;
//	        return SYS_ERR_NOREPLY;
//	
//	
//	    }
//	
//	    SYS_OK();
//	}

/************************************************************************
 * @function: ParaRechargeInfo
 * @����: ��ֵ
 * 
 * @����: 
 * @param: frame 
 * @param: pinfo 
 * @param: extra 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: (flash�ṹ:LEN_TST_BMPS�ֽ����λͼ + num������[2B��� + 2B������� + ...])
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ParaPriceAdjustInfo(const PST_TableStr* tbl, PST_Frame* frm)
{
    uint8* pts;
    uint32 base_addr = PRICE_PADDR;         //��������ַ
    uint32 addr;                            //��ַ
    uint8 xh;                              //���
    uint16 num;                             //����
    uint8 max = CORE_TST_NO ;            //������
    uint8 k,m = 0;
    uint8_t timetmp[6];
    ZY_Frame* frame = &frm->_zy;
    //int m = 0;
    uint8_t sum = 0;
//	    uint8_t * dev = NULL;
    ST_ACT_INPBUF_L    inLbuf;
    ST_RDMT_CMD stDlmscmd;
    Netp_PowerMng(1, NETP_PORT_PWR_PRICE);
                                            //��FLASH��ȡλͼ
                                            //������\��ȡ������
    if(frame->cmd == 0x01)  
    {
        uint8  exnum = 0;               //ʵ�ʷ��ص�����
        uint8_t meter_num = 0;// = GetPrepayNum();
        STMeterInfo meterInfo_tmp;
        STPrepayDevInfo meterPrepayInfo;

        m++;//����λ��Ԥ��
        const TIME* time = GetTime();
        frame->send[m++] = ByteHexToBcd(time->year);
        frame->send[m++] = ByteHexToBcd(time->month);
        frame->send[m++] = ByteHexToBcd(time->day);
        frame->send[m++] = ByteHexToBcd(time->hour);
        frame->send[m++] = ByteHexToBcd(time->min);        
        frame->send[m++] = ByteHexToBcd(time->sec);
       
        for(k = 0; k < CORE_TST_NO; k++)
        {
            if(gss_CDATATst[k].info.type != 0x03)
            {
                continue;
            }
            else
            {
                ST_Netp_FW * fw = &gs_netp_fw_trans[gss_CDATATst[k].pt];
                uint8_t ret = Netp_Get_Framework(gss_CDATATst[k].pf, fw);
                if(SYS_ERR_OK == ret && fw->pf < CON_MAX_DEVICE_TYPES)
                {
                    uint8_t mlen[2];
                    memcpy((uint8_t *)&fw->frameDtaStr, (uint8_t *)&gs_NetpFrameWXICPriceRDataStr, sizeof(NetpFrameDataStr));

                    stDlmscmd.port = gss_CDATATst[k].pt;
                    stDlmscmd.addr = gss_CDATATst[k].info.main_addr;  
                    stDlmscmd.addr_len = 1;
//                    stDlmscmd.fw = fw;
                    stDlmscmd.baud_cfg = gss_CDATATst[k].bs;
                    stDlmscmd.di = 0;
                    fw->frameDtaStr[0].item[0].mult = k;
                    mlen[0] = 0;
                    mlen[1] = 0x2f;
                    uint8_t err = Netp_CommonRead(&inLbuf, mlen, &stDlmscmd);
                    if((err == NETP_ERR_OK))
                    {
                        memset(frame->send + m, 0, LEN_PRICEADJUST_INFO + 2  );
                        frame->send[m+2] = gss_CDATATst[k].info.addr & 0x0f;
                        memcpy(frame->send + m + 3, gss_CDATATst[k].info.factory_model, 4  );
                        memcpy(frame->send + m + 7, inLbuf.buf+1, inLbuf.len-1);
                        m+=LEN_PRICEADJUST_INFO + 2;
//	                        frame->send[m++]= 0x06;
//	                        frame->send[m++]= 0;
                    }
                    else
                    {
//	                        frame->len = 0;
//	                        SYS_ERR();
                        continue;
                    }
                    
                    exnum++;
                }
                else
                {
//	                    frame->len = 0;
//	                    SYS_ERR();
                    continue;
                }                
            }
        }
                                        //��������
        frame->send[0]= exnum;
                                        //����ֵ
//	        memcpy(frame->send+1, time, 6);                                        
        frame->len = ((LEN_PRICEADJUST_INFO + 2) * exnum + 7);
    }
    else                 //����
    {                                      
        num = frame->recv[0];//ByteToInt(extra->rbuf);
        if(num > max)
        {
            num = max;
        }
        memcpy(timetmp, frame->recv + 1, 6);
        frame->send[m++] = num;
        m+=6;
        

        for(k = 0; k < num; k++)
        {                                   //��ȡ���\���
            xh = frame->recv[7 + LEN_PRICEADJUST_INFO * k] & 0x0f;
            
            if((xh > max) || (xh == 0) || gss_CDATATst[xh-1].pt >= MCB_PORT_NUM 
                || gss_CDATATst[xh-1].info.type != 0x03 )
            {
                frame->send[m++]= 0x23;
                frame->send[m++]= 0;
            }
            else
            {
//	                gs_GPIO.GPO_Out(GPO_485_PWR, true);

                ST_Netp_FW * fw = &gs_netp_fw_trans[gss_CDATATst[xh-1].pt];
                addr = base_addr + LEN_PRICEADJUST_INFO * (xh - 1);
                GD_Para_RW(addr, frame->recv+7 + LEN_PRICEADJUST_INFO * k, LEN_PRICEADJUST_INFO, true);
                
                uint8_t ret = Netp_Get_Framework(gss_CDATATst[xh-1].pf, fw);
                if(SYS_ERR_OK == ret && fw->pf < CON_MAX_DEVICE_TYPES)
                {
                    
                    uint8_t tmp[47];
                    memcpy((uint8_t *)&fw->frameDtaStr, (uint8_t *)&gs_NetpFrameWXICPriceWDataStr, sizeof(NetpFrameDataStr));

                    stDlmscmd.port = gss_CDATATst[xh-1].pt;
                    stDlmscmd.addr = gss_CDATATst[xh-1].info.main_addr;  
                    stDlmscmd.addr_len = 1;
//                    stDlmscmd.fw = fw;
                    stDlmscmd.baud_cfg = gss_CDATATst[xh-1].bs;
                    stDlmscmd.di = 0;
//	                    fw->frameDtaStr[0].item[0].b = xh - 1;
                    memset(tmp,0,47);

                    memcpy(tmp, frame->recv+7+ LEN_PRICEADJUST_INFO*k + 5, 47);

                    uint8_t err = Netp_CommonRead(&inLbuf, tmp, &stDlmscmd);
                    if((err == NETP_ERR_OK))
                    {
//	                        frame->send[m++]= 0;
//	                        frame->send[m++]= 0;
                        memcpy(frame->send + m, inLbuf.buf + 1, 2);
                        m+=2;
                    }
                    else
                    {
                        frame->send[m++]= 0x23;
                        frame->send[m++]= 0;
                    }
                }
                else
                {
                    frame->send[m++]= 0x23;
                    frame->send[m++]= 0;
                }
                
            }

            //��������(������1�����׶�)
//	            memcpy(g_ucMeterPara + addr, frame->recv + 1 + LEN_PREPAY_CFG * k, LEN_PREPAY_CFG);
        }
        
//	        frame->send[0]= 0;
//	                                        //����ֵ
//	        frame->len = 1;
//	        return SYS_ERR_NOREPLY;
        
        memcpy(frame->send+1, timetmp, 6);
        
        frame->len = m;
    }
    Netp_PowerMng(0, NETP_PORT_PWR_PRICE);

    SYS_OK();
}

//	{
//	    uint8* pts;
//	    uint32 base_addr = 0;         //��������ַ
//	    uint32 addr;                            //��ַ
//	    uint8 xh;                              //���
//	    uint16 num;                             //����
//	    uint8 max = CORE_TST_NO ;            //������
//	    uint8 k,m = 0;
//	    ZY_Frame* frame = &frm->_zy;
//	    //int m = 0;
//	    uint8_t sum = 0;
//	    uint8_t * time = NULL;
//	                                            //��FLASH��ȡλͼ
//	                                            //������\��ȡ������
//	    num = frame->recv[0];//ByteToInt(extra->rbuf);
//	    time = frame->recv + 1;
//	    if(num > max)
//	    {
//	        num = max;
//	    }
//	    if(frame->cmd == 0x01)  
//	    {
//	        uint8  exnum = 0;               //ʵ�ʷ��ص�����
//	        uint8_t meter_num = GetMeterNum();
//	        STMeterInfo meterInfo_tmp;
//	        STPriceAdjust meterPriceInfo;
//	        pts = frame->send + 1;
//	        
//	        memcpy(pts, g_ucPrepay, 6);
//	        pts+=6;
//	        for(k = 0; k < CORE_TST_NO; k++)
//	        {
//	
//	//              GD_Para_RW(LEN_TST_CFG * k, (uint8_t *)&meterInfo_tmp, LEN_TST_CFG, false);
//	            memcpy((uint8_t *)&meterInfo_tmp, g_ucMeterPara + LEN_TST_CFG * k, LEN_TST_CFG);
//	            if(meterInfo_tmp.type != 0)
//	            {
//	                memcpy(&meterPriceInfo, g_ucPrice + 6 + LEN_PRICEADJUST_INFO * k, LEN_PRICEADJUST_INFO);
//	
//	                memcpy(pts, (uint8_t *)&meterPriceInfo, LEN_PRICEADJUST_INFO);
//	
//	                exnum++;
//	                pts += LEN_PREPAY_CFG;  //ָ��
//	            }
//	        }
//	                                        //��������
//	        frame->send[0]= exnum;
//	                                        //����ֵ
//	        frame->len = (LEN_PREPAY_CFG * exnum + 7);
//	    }
//	    else                 //����
//	    {                                       //����ֵ
//	//	        extra->len = (LEN_TST_CFG * num) + 2;
//	
//	        for(k = 0; k < num; k++)
//	        {                                   //��ȡ���\���
//	            xh = frame->recv[7 + LEN_PRICEADJUST_INFO * k] & 0x0f;
//	                                            //�жϺϷ���
//	            if((xh > max) || (xh == 0))
//	            {
//	                frame->send[m++]= 0x1b;
//	                frame->send[m++]= 0;
//	                
//	                memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
//	                m+=4;
//	                
//	                frame->len = m;
//	                return SYS_ERR_FT;
//	            }
//	
//	            //��������(������1�����׶�)
//	            addr = base_addr + LEN_PRICEADJUST_INFO * (xh - 1);
//	            memcpy(g_ucPrice + addr, frame->recv + 1 + LEN_PRICEADJUST_INFO * k, LEN_PRICEADJUST_INFO);
//	        }
//	        
//	//	        frame->send[0]= 0;
//	//	                                        //����ֵ
//	//	        frame->len = 1;
//	        return SYS_ERR_NOREPLY;
//	
//	
//	    }
//	
//	    SYS_OK();
//	}


uint8_t ZY_Report_Confirm(const PST_TableStr* tbl, PST_Frame* frm)
{
    
    ZY_Frame* frame = &frm->_zy;

    if(tbl->tid == 0xA621)
    {
        STR_SetBit(bmap_AckRecv, 0);
    }
    else if(tbl->tid == 0xA622)
    {
        STR_SetBit(bmap_AckRecv, 1);
    }
    else if(tbl->tid == 0xA624)
    {
        STR_SetBit(bmap_AckRecv, 2);
    }    
    return SYS_ERR_NOREPLY;
}
#define VALVE_OPEN  0xAA
#define VALVE_CLOSE 0x55
#define VALVE_SLEEP 0x99
uint8_t ZY_Valve_Ctr(const PST_TableStr* tbl, PST_Frame* frm)
{
    uint8* pts;
    uint32 addr;                            //��ַ
    uint8 xh,type = 0;                              //���
    uint16 num;                             //����
    uint8 max = CORE_TST_NO ;            //������
    uint8 k,m = 0;
    uint8_t timetmp[6];
    ZY_Frame* frame = &frm->_zy;
    //int m = 0;
    uint8_t sum = 0;
//	    uint8_t * dev = NULL;
    ST_ACT_INPBUF_L    inLbuf;
    ST_RDMT_CMD stDlmscmd;
    uint16_t errCode = 0;
    Netp_PowerMng(1, NETP_PORT_PWR_VAVLE);
    
                                            //��FLASH��ȡλͼ
                                            //������\��ȡ������
    if(frame->cmd == 0x04)  
    {                                      
        m = 0;
        xh = frame->recv[0] & 0x0f;
        type = frame->recv[1];
        
        if((xh > max) || (xh == 0) || gss_CDATATst[xh-1].pt >= MCB_PORT_NUM 
            || !(gss_CDATATst[xh-1].info.type == 0x03 || gss_CDATATst[xh-1].info.type == 0x02))
        {
            errCode = 0x1D;
            goto ERR_VALVE;
        }
        else
        {
            ST_Netp_FW * fw = &gs_netp_fw_trans[gss_CDATATst[xh-1].pt];
            
            uint8_t ret = Netp_Get_Framework(gss_CDATATst[xh-1].pf, fw);
            if(SYS_ERR_OK == ret && fw->pf < CON_MAX_DEVICE_TYPES)
            {
                
                uint8_t tmp[2];
                if(type == VALVE_OPEN)
                {
                    //����
                    tmp[0] = 0xFF;
                    tmp[1] = 0;
                }
                else if(type == VALVE_CLOSE)
                {
                    //�ط�
                    tmp[0] = 0;
                    tmp[1] = 0;
                    
                }
                else if(type == VALVE_SLEEP)
                {
                    //ֹͣ
                    tmp[0] = 0xCC;
                    tmp[1] = 0;
                    
                }
                else
                {
                    errCode = 0x21;
                    goto ERR_VALVE;
                }                    
                memcpy((uint8_t *)&fw->frameDtaStr, (uint8_t *)&gs_NetpFrameWXICVlvWDataStr, sizeof(NetpFrameDataStr));

                stDlmscmd.port = gss_CDATATst[xh-1].pt;
                stDlmscmd.addr = gss_CDATATst[xh-1].info.main_addr;  
                stDlmscmd.addr_len = 1;
//                stDlmscmd.fw = fw;
                stDlmscmd.baud_cfg = gss_CDATATst[xh-1].bs;
                stDlmscmd.di = 0;

                uint8_t err = Netp_CommonRead(&inLbuf, tmp, &stDlmscmd);
                if((err == NETP_ERR_OK))
                {
                    if(inLbuf.buf[0] == 0 && inLbuf.buf[1] == 0xAC)
                    {
                        if(inLbuf.buf[2] == tmp[0] && inLbuf.buf[3] == tmp[1])
                        {
                            frame->send[m++]= gss_CDATATst[xh-1].info.addr & 0x0f;
                            frame->send[m++]= type;
                            memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
                            m+=4;                                
                        }
                        else
                        {
                            errCode = 0x21;
                            goto ERR_VALVE;

                        }
                    }
                    else
                    {
                        errCode = 0x21;
                        goto ERR_VALVE;
                    
                    }
                }
                else
                {
                    errCode = 0x21;
                    goto ERR_VALVE;
                }
            }
            else
            {
                errCode = 0x1D;
                goto ERR_VALVE;
            }
        }

        frame->len = m;
    }
    Netp_PowerMng(0, NETP_PORT_PWR_VAVLE);

    SYS_OK();
ERR_VALVE:
    memcpy(frame->send + m, (uint8_t *)&errCode, 2);
    m+=2;
    memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
    m+=4;
    frame->len = m;
    Netp_PowerMng(0, NETP_PORT_PWR_VAVLE);
    
    SYS_ERR();
}
//	
//	{
//	    uint8 max = CORE_TST_NO;            //������
//	    uint8 k,m=0;
//	    ZY_Frame* frame = &frm->_zy;
//	    uint8_t devaddr = 0;
//	    uint8_t type = 0;
//	    uint16_t err = 0x1D;//ɾ�����豸��ַ�����ڵ�
//	    
//	    devaddr = frame->recv[0] & 0x0f;//ByteToInt(extra->rbuf);
//	    type = frame->recv[1];
//	
//	    if(devaddr > max)
//	    {
//	        goto ERR_VALVE;
//	    }
//	
//	    if(type == VALVE_OPEN)
//	    {
//	        //����
//	    }
//	    else if(type == VALVE_CLOSE)
//	    {
//	        //�ط�
//	    }
//	    else if(type == VALVE_SLEEP)
//	    {
//	        //ֹͣ
//	    }
//	    else
//	    {
//	        err = 0x21;
//	        goto ERR_VALVE;
//	    }
//	        
//	    SYS_OK();
//	ERR_VALVE:
//	    memcpy(frame->send + m, (uint8_t *)&err, 2);
//	    m+=2;
//	    memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
//	    m+=4;
//	
//	    return SYS_ERR_FT;
//	}

uint8_t ZY_ParaRst(const PST_TableStr* tbl, PST_Frame* frm)
{
    ZY_Frame* frame = &frm->_zy;
    uint8_t type = 0;
    uint16_t err = 1;//
    uint8_t m = 0;
    type = frame->recv[0];
    if(frame->len == 0x0A)
    {
        if(RstPara(true) != SYS_ERR_OK)
        {
            
            err = 0x01;
            goto ERR_RST;
        }

    }
    else
    {
        if(RstPara(type == 0 ? true : false) != SYS_ERR_OK)
        {
            
            err = 0x01;
            goto ERR_RST;
        }
    }
    memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
    m+=4;
    frame->len = m;
    SYS_OK();
ERR_RST:
    memcpy(frame->send + m, (uint8_t *)&err, 2);
    m+=2;
    memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
    m+=4;
    frame->len = m;
    return SYS_ERR_FT;
}

/************************************************************************
 * @function: ParamLoad_CP
 * @����: �ն˳������
 * @����: 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ParamLoad_CP(void)
{
    uint16 value = 0;
    
    if(GD_Para_RW(PARA_COPY_INTERVAL, &value, 2, false) != SYS_ERR_OK)
    {
        GD_Para_RW(PARA_COPY_INTERVAL, &value, 2, false);
    }

    if(value >= 60)
    {
        gs_PstPara.cp_interval = value/60;
    }
    else
    {
        gs_PstPara.cp_interval = 1;
    }

    SYS_OK();
}
/************************************************************************
 * @function: ParamLoad_HB
 * @����: �������
 * @����: 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ParamLoad_HB(void)
{
    uint16 value = 0;
    
    if(GD_Para_RW(PARA_HB, &value, 2, false) != SYS_ERR_OK)
    {
        GD_Para_RW(PARA_HB, &value, 2, false);
    }
    gs_FarpVar.hbcy = value;
    if(gs_FarpVar.hbcy > 900 || gs_FarpVar.hbcy < 60)
    {
        gs_FarpVar.hbcy = 300;
    }
    SYS_OK();
}

/************************************************************************
 * @function: ParamLoad_Gprs
 * @����: �ն�gprs����
 * @����: 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ParamLoad_Gprs(void)
{
    uint8 buff[96];
    uint32 ip;
    uint16 pt;
    uint32 ib;
    uint16 pb;
    uint16 len_apn;
    uint8 uc_i;
                                            //��ȡ����
    GD_Para_RW(PARA_GPRS, buff, PARA_GPRS_PLEN, false);
                                            //�жϲ����仯,�����Ƿ�����
    ip = RByteToLong(buff);
    pt = RByteToInt(buff + 4);
    len_apn = strlen((string)(buff + 6));
    if(len_apn > 30)
    {
        len_apn = 30;
    }
                                           //���ǳ���ˢ�²���
    if(gl_SysRunStt & HRS_PARALD)
    {
        if((gs_FarpVar.dcip != ip) || 
           (gs_FarpVar.dcpt != pt) ||
//	           (gs_FarpVar.dcib != ib) || 
//	           (gs_FarpVar.dcpb != pb) ||
           (len_apn != strlen((string)gs_FarpVar.gapn)) || 
           (CmpBuffer(buff + 6, gs_FarpVar.gapn, len_apn) != 0))
        {                                   //ִ�в������Ĵ���������
            gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FARP_PARACHG], 1);//(MSG_FARP_PARACHG, TASK_FARP_TKID);
        }
    }

    gs_FarpVar.dcip = ip;                   //��վ��ѡip��ַ
    gs_FarpVar.dcpt = pt;                   //��վ��ѡ�˿ں�
    gs_FarpVar.dcib = ip;                   //��վ����ip��ַ
    gs_FarpVar.dcpb = pt;                   //��վ���ö˿ں�
                                            //APN
    if(len_apn == 0)
    {                                       //��ʼ��Ϊcmnet
        MoveBuffer("cmnet", gs_FarpVar.gapn, 6);
    }
    else
    {                                       //�ж��Ƿ��зǷ��ַ�
        for(uc_i = 0; uc_i < len_apn; uc_i++)
        {
            if(buff[6 + uc_i] > 127)
            {
                break;
            }
        }

        if(uc_i != len_apn)                 //�Ƿ�,��ʼ��Ϊcmnet
        {
            MoveBuffer("cmnet", gs_FarpVar.gapn, 6);
        }
        else
        {
            MoveBuffer(buff + 6, gs_FarpVar.gapn, 30);
            gs_FarpVar.gapn[29] = 0;
        }
    }
    GD_Para_RW(PARA_HB, (uint8_t *)&gs_FarpVar.hbcy, 2, false);
    if(gs_FarpVar.hbcy > 900 || gs_FarpVar.hbcy < 60)
    {
        gs_FarpVar.hbcy = 300;
    }
    SYS_OK();
}
/************************************************************************
 * @function: ParamLoad_ftp
 * @����: �ն�ftp����
 * @����: 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
extern uint32_t g_ulFtp_Timeout;
uint8 ParamLoad_Ftp(void)
{
    uint8 buff[PARA_FTP_PLEN];
    TIME * curT = GetTime();
    TIME getT;
    uint8_t * p = NULL;
    uint8_t * q = NULL;
            //��ȡ����
    GD_Para_RW(PARA_FTP, buff, PARA_FTP_PLEN, false);
    
    RUN_Para_RW(RUNPARA_FTP, &gs_FtpPara, RUNPARA_FTP_PLEN, false);
    //�жϲ����仯,�����Ƿ�����
    //InvBuffer(buff, 6);
//	    memcpy((uint8_t *)&getT, buff, 6);
    getT.year = ByteBcdToHex(buff[0]);
    getT.month = ByteBcdToHex(buff[1]);
    getT.day = ByteBcdToHex(buff[2]);
    getT.hour = ByteBcdToHex(buff[3]);
    getT.min = ByteBcdToHex(buff[4]);
    getT.sec = ByteBcdToHex(buff[5]);    
    if(gs_FtpPara.flag == FTP_STEP_START || gs_FtpPara.flag == FTP_STEP_WAIT)
    {
        uint32_t t1 = CalcSecs((uint8*)&(GetTime()->sec), false);
        uint32_t t2 = CalcSecs(&getT, false);
        
        if(t1 > t2)
        {
            //��������
            gs_FtpPara.flag = FTP_STEP_OPEN;
            g_ulFtp_Timeout = 5;
            
        }
        else
        {

            gs_FtpPara.flag = FTP_STEP_WAIT;

        }
        gs_FtpPara.fileLen = 0;
        gs_FtpPara.getLen = 0;

    }
    else if(gs_FtpPara.flag == FTP_STEP_ING)
    {
        gs_FtpPara.flag = FTP_STEP_OPEN;
        gs_FtpPara.err_times = 0;
        g_ulFtp_Timeout = 5;
    }
    else
    {
        gs_FtpPara.flag = FTP_STEP_IDEL;
        gs_FtpPara.err_times = 0;
    }
        
    memcpy((uint8_t *)&gs_FtpPara.time.sec, &getT.sec, 6);
    memset(gs_FtpPara.ftp_domain, 0, FTP_DOMAIN_MAX_LEN);
    if(strlen(buff+6) < FTP_DOMAIN_MAX_LEN)
    {
        strcpy(gs_FtpPara.ftp_domain,buff+6);
    }
    else
    {
        SYS_ERR();
    }    

    memcpy((uint8_t *)&gs_FtpPara.port,buff+6+FTP_DOMAIN_MAX_LEN,2);
    memset(gs_FtpPara.ftp_fold, 0, FTP_DOMAIN_MAX_LEN);
    if(strlen(buff+6+FTP_DOMAIN_MAX_LEN+2) < FTP_DOMAIN_MAX_LEN)
    {
        p = buff+6+FTP_DOMAIN_MAX_LEN+2;
        q = strrchr(p,'/');
        if(q!= NULL)
        {
            memcpy(gs_FtpPara.ftp_fold,buff+6+FTP_DOMAIN_MAX_LEN + 2, q-p);
        }
        
        else
        {
            SYS_ERR();
        }
    }
    else
    {
        SYS_ERR();
    }

//	    if(strlen(buff+6+FTP_DOMAIN_MAX_LEN+2 + FTP_DOMAIN_MAX_LEN) < FTP_PARA_MAX_LEN)
//	        strcpy(gs_FtpPara.ftp_fold,buff+6+FTP_DOMAIN_MAX_LEN + 2);
    
    memcpy((uint8_t *)&gs_FtpPara.ftp_file[0],
                buff+6+FTP_DOMAIN_MAX_LEN+2+FTP_DOMAIN_MAX_LEN,
                FTP_PARA_MAX_LEN * 3);


    RUN_Para_RW(RUNPARA_FTP, &gs_FtpPara, RUNPARA_FTP_PLEN, true);

    SYS_OK();
}

/************************************************************************
 * @function: ZY_Copy_Interval
 * @����: ��ȡ�����ϸ���
 * 
 * @����: 
 * @param: frame 
 * @param: pinfo 
 * @param: extra 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: (flash�ṹ:LEN_TST_BMPS�ֽ����λͼ + num������[2B��� + 2B������� + ...])
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_Copy_Interval(const PST_TableStr* tbl, PST_Frame* frm)
{
    uint8* pts;
    uint32 base_addr = 0;         //��������ַ
    uint32 addr;                            //��ַ
    uint8 xh;                              //���
    uint16 value;                             //����
    uint8 max = CORE_TST_NO ;            //������
    uint8 k,m=0;
    ZY_Frame* frame = &frm->_zy;

    if(frame->cmd == 0x04)                  //����RTU�ϸ�ʱ��
    {
//	        MoveBuffer(frame->recv, (uint8_t *)&gs_PstPara.cp_interval, 2);


        if(GD_Para_RW(PARA_COPY_INTERVAL, frame->recv, 2, true) == PST_ERR_OK)
        {
//	            ParamLoad_F251();

            memcpy(frame->send, (uint8_t *)&gs_SysVar.terstt.lword, 4);
            
            frame->len = 4;
            
            return PST_ERR_OK;              //�ظ��ɹ�����֡
        }
        
        frame->send[0]= 0x01;
        frame->send[1]= 0;
        memcpy(frame->send + 2, (uint8_t *)&gs_SysVar.terstt.lword, 4);
        
        frame->len = 6;    
        SYS_ERR();
    }
    else                                //��ȡ
    {
        if(GD_Para_RW(PARA_COPY_INTERVAL, frame->send, 2, false) != PST_ERR_OK)
        {
            value = gs_PstPara.cp_interval * 60;
            MoveBuffer((uint8_t *)&value, frame->send, 2);
        }
        m+=2;
    }
    memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
    m+=4;
    frame->len = m;
    
    SYS_OK();
}
/************************************************************************
 * @function: ZY_HeartBeat
 * @����: ��ȡ�����������
 * 
 * @����: 
 * @param: frame 
 * @param: pinfo 
 * @param: extra 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: (flash�ṹ:LEN_TST_BMPS�ֽ����λͼ + num������[2B��� + 2B������� + ...])
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_HeartBeat(const PST_TableStr* tbl, PST_Frame* frm)
{
    uint8* pts;
    uint32 base_addr = 0;         //��������ַ
    uint32 addr;                            //��ַ
    uint8 xh;                              //���
    uint16 value;                             //����
    uint8 max = CORE_TST_NO ;            //������
    uint8 k,m=0;
    ZY_Frame* frame = &frm->_zy;

    if(frame->cmd == 0x04)                  //����RTU�ϸ�ʱ��
    {
//	        MoveBuffer(frame->recv, (uint8_t *)&gs_PstPara.cp_interval, 2);


        if(GD_Para_RW(PARA_HB, frame->recv, 2, true) == PST_ERR_OK)
        {
//	            ParamLoad_F251();
            frame->send[0]= 0;
            frame->send[1]= 0;

            memcpy(frame->send + 2, (uint8_t *)&gs_SysVar.terstt.lword, 4);
            
            frame->len = 6;
            
            return PST_ERR_OK;              //�ظ��ɹ�����֡
        }
        
        frame->send[0]= 0x01;
        frame->send[1]= 0;
        memcpy(frame->send + 2, (uint8_t *)&gs_SysVar.terstt.lword, 4);
        
        frame->len = 6;    
        SYS_ERR();
    }
    else                                //��ȡ
    {
        if(GD_Para_RW(PARA_HB, frame->send, 2, false) != PST_ERR_OK)
        {
            value = gs_FarpVar.hbcy;
            MoveBuffer((uint8_t *)&value, frame->send, 2);
        }
        m+=2;
    }
    memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
    m+=4;
    frame->len = m;
    
    SYS_OK();
}

/************************************************************************
 * @function: ZY_Para_GPRS
 * @����: ��ȡ�����ϸ���
 * 
 * @����: 
 * @param: frame 
 * @param: pinfo 
 * @param: extra 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: (flash�ṹ:LEN_TST_BMPS�ֽ����λͼ + num������[2B��� + 2B������� + ...])
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_Para_GPRS(const PST_TableStr* tbl, PST_Frame* frm)
{
    uint8* pts;
    uint32 base_addr = 0;         //��������ַ
    uint32 addr;                            //��ַ
    uint8 xh;                              //���
    uint16 num;                             //����
    uint8 max = CORE_TST_NO ;            //������
    uint8 k,m=0;
    uint16_t errCode = 1;
    ZY_Frame* frame = &frm->_zy;

    if(frame->cmd == 0x04)                  //����GPRS����
    {
        if(GD_Para_RW(PARA_GPRS, frame->recv, PARA_GPRS_PLEN, true) == PST_ERR_OK)
        {
//              ParamLoad_F251();

            memcpy(frame->send, (uint8_t *)&gs_SysVar.terstt.lword, 4);
            
            frame->len = 4;
            
            return PST_ERR_OK;              //�ظ��ɹ�����֡
        }
        else
        {
            errCode = 1;
            goto ERR_GPRS;
        }
    }
    else                                //��ȡ
    {
        if(GD_Para_RW(PARA_GPRS, frame->send, PARA_GPRS_PLEN, false) == PST_ERR_OK)
        {
            m+=96;

            memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
            m+=4;

            
//	            return PST_ERR_OK;              //�ظ��ɹ�����֡
        }
        else
        {
            errCode = 1;
            goto ERR_GPRS;
        }    
//	        MoveBuffer((uint8_t *)&gs_FarpVar.dcip, frame->send, 96);
//	        m+=96;
//	        memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
//	        m+=4;        
    }
    
    frame->len = m;
    SYS_OK();
ERR_GPRS:
    
    memcpy(frame->send + m, (uint8_t *)&errCode, 2);
    m+=2;
    memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
    m+=4;
    frame->len = m;
    
    SYS_ERR();
}
/************************************************************************
 * @function: ZY_Para_Ftp
 * @����: ��ȡ����ftp�������
 * 
 * @����: 
 * @param: frame 
 * @param: pinfo 
 * @param: extra 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: (flash�ṹ:LEN_TST_BMPS�ֽ����λͼ + num������[2B��� + 2B������� + ...])
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_Para_Ftp(const PST_TableStr* tbl, PST_Frame* frm)
{
    uint8* pts;
    uint32 base_addr = 0;         //��������ַ
    uint32 addr;                            //��ַ
    uint8 xh;                              //���
    uint16 num;                             //����
    uint8 max = CORE_TST_NO ;            //������
    uint8 k,m=0;
    uint16_t errCode = 1;
    ZY_Frame* frame = &frm->_zy;

    if(frame->cmd == 0x04)                  //����GPRS����
    {
        if(GD_Para_RW(PARA_FTP, frame->recv, PARA_FTP_PLEN, true) == PST_ERR_OK)
        {


            gs_FtpPara.flag = FTP_STEP_START;
            gs_FtpPara.err_times = 0;
            if(RUN_Para_RW(RUNPARA_FTP, &gs_FtpPara, 1, true) != PST_ERR_OK)
            {
                errCode = 1;
                goto ERR_FTP;            
            }

            errCode = 0;
            memcpy(frame->send + m, (uint8_t *)&errCode, 2);
            m+=2;

            memcpy(frame->send+m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
            m+=4;
            frame->len = m;
            
            return PST_ERR_OK;              //�ظ��ɹ�����֡
        }
        else
        {
            errCode = 1;
            goto ERR_FTP;
        }
    }
    else if(frame->cmd == 0x05) //ȷ��
    {
        STR_SetBit(bmap_AckRecv, 3);
        return SYS_ERR_NOREPLY;
    }
    else//��ȡ
    {
    
        if(GD_Para_RW(PARA_FTP, frame->recv, PARA_FTP_PLEN, false) == PST_ERR_OK)
        {
            m+=PARA_FTP_PLEN;
        }
        else
        {
            errCode = 1;
            goto ERR_FTP;
        }
    }
    
    frame->len = m;
    SYS_OK();
ERR_FTP:
    
    memcpy(frame->send + m, (uint8_t *)&errCode, 2);
    m+=2;
    memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
    m+=4;
    frame->len = m;
    
    SYS_ERR();
}


/************************************************************************
 * @function: PentTrans485
 * @����: 
 * 
 * @����: 
 * @param: port �˿ں�(0-!)
 * @param: cfg ����
 * @param: length ���뷢�ͳ���,�������ճ���
 * @param: sbuff ���ͻ���
 * @param: rbuff ���ջ���
 * @param: rbuff ��λ100ms
 * 
 * @����: 
 * @return: uint8 SA_ERR_OK, SYS_ERR_FT
 * @˵��: 
 * @����: yzy (2011-10-10)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PentTrans485(uint8 port, uint8 cfg, uint16* length, uint8* sbuff, uint8* rbuff, uint16 overtime)
{
//    uart_config_t ss;                          //��ʱʱ��:��λ100ms
    uint8 result;
    
    
//    PentCfgToSS(cfg, &ss);
//                                            //����������������ջ���
//    
//                                            //ִ��.ע��,�ú����շ�ʹ��ͬһ����
//    result = Netp_PentTran(sbuff, length, &ss, overtime, port);
//    if(result == SYS_ERR_OK && (*length > 0))
//    {
//        
//        MoveBuffer(sbuff, rbuff , *length);
//    }                                        
    return result;
}

/************************************************************************
 * @function: Gd_Afn10_F1
 * @����: ͸��ת��
 * 
 * @����: 
 * @param: recv ����֡��������
 * @param: send ����֡��������
 * @param: length ����֡�������ݳ������
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2011-11-3)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_Transparent(const PST_TableStr* tbl, PST_Frame* frm)

//(uint8* recv, uint8* send, uint8* length, PST_Frame* frame)
{
    uint16 len;
    uint16 overtime;
    uint8 cfg;
    uint8 port;
    int m = 0;
    //uint8 portidx;
    ZY_Frame* frame = &frm->_zy;

    if(frame->cmd == 0x04)
    {

    }
    //len = frame->recv[m++];
    port = frame->recv[m++];                         //�˿ں�
    if(port > 3)
    {
        if(port == 0x1f)
        {
            uint16_t index = 0;
            index = ADVMidWaySearch(
                    (uint8*)gss_ExtFunc,
                    sizeof(gss_ExtFunc) / sizeof(TExtendFunc),
                    sizeof(TExtendFunc),
                    frame->recv[m++]
                    );
            
            if(index == 0xFFFF)                 //û���������򷵻����ݱ�ʶ����
            {
                frame->send[0] = port;
                frame->len = 1;
                return PST_ERR_OK;
            }
            
            const TExtendFunc* tbl;
            tbl = gss_ExtFunc + index;         //���ݽṹ

            len = tbl->extfunc(tbl->index, frame->recv+m, 0,0);
            
            memcpy(frame->send, frame->recv, len+m);
            frame->len = len+m;
            return PST_ERR_OK;
        }
        frame->send[0] = port;
        frame->len = 1;
        return PST_ERR_FT;
    }
    else
    {
       /*
                                            //ת��ʧ�ܲ��ظ�
        if((port == PORT_CB1_NO) || (port == PORT_CB2_NO) || (port == PORT_CB3_NO))
        {
            portidx = _PORT_INDEX(port);
            if(portidx != 0xff)
            {
                PentTrans485(portidx, cfg, &len, recv + 6, send + 3, overtime);
            }
        }
        else
        {
            len = 0;
        }
        */
        cfg = frame->recv[m++];                          //ͨ�ſ�����
        overtime = frame->recv[m];
        overtime = frame->recv[m] & 0x7F;
        overtime = overtime * 10;
        m++;
        len = frame->len - (10+m);            //ת�����ݳ���
        
        //�ɼ���ֻ��һ���˿ڣ����Զ˿ں���ȷ�����ж�
        if(SYS_ERR_OK == PentTrans485(port, cfg, &len, frame->recv + m, frame->send + 1, overtime))
                                            //���
        {
//	            frame->send[0] = len+1;                                    
            frame->send[0] = port;
            frame->len =  len+1;
            
            return PST_ERR_OK;
        }
        else
        {
            frame->send[0] = port;
            frame->len = 1;
            return PST_ERR_OK;
        }
    }
}


/************************************************************************
 * @function: ZY_CurrData_Post
 * @����: ��������������(F89-F94, F101-F104)
 * 
 * @����: 
 * @param: tbl 
 * @param: extra 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: xugl (2014/2/24)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
extern ST_Netp_FW gs_report_fw;

//	uint16_t Pack_ReportData(uint16_t id, ST_Netp_FW * fw, uint8_t * buf, uint16_t len,S_CURDATA_SEG0 * curdata)
//	{
//	    uint16_t m = 0;
//	    uint16_t eft_tmp = 0;
//	    memcpy(buf+m, &curdata->seg.dev, 5);
//	    m+=5;
//	    eft_tmp = fw->eft;
//	    if(id == 0xA621)
//	    {
//	        if(STR_IsBitSet((uint8_t *)&eft_tmp, EFT_MIN_ST) 
//	            && fw->plen[EFT_MIN_ST] < 12
//	            && m + fw->plen[EFT_MIN_ST] < len)
//	        {
//	            memcpy(buf+m, &curdata->seg.ST[0], fw->plen[EFT_MIN_ST]);
//	            m+=fw->plen[EFT_MIN_ST];
//	        }
//	
//	        if(STR_IsBitSet((uint8_t *)&eft_tmp, EFT_MIN_WT)
//	            && fw->plen[EFT_MIN_WT] < 12
//	            && m + fw->plen[EFT_MIN_WT] < len)
//	        {
//	                
//	            memcpy(buf+m, &curdata->seg.ST[0], fw->plen[EFT_MIN_WT]);
//	            m+=fw->plen[EFT_MIN_WT];
//	        }
//	
//	        if(STR_IsBitSet((uint8_t *)&eft_tmp, EFT_MIN_SI)
//	            && fw->plen[EFT_MIN_SI] < 12
//	            && m + fw->plen[EFT_MIN_SI] < len)
//	        {
//	                
//	            memcpy(buf+m, &curdata->seg.ST[0], fw->plen[EFT_MIN_SI]);
//	            m+=fw->plen[EFT_MIN_SI];
//	        }
//	
//	
//	        
//	        memcpy(buf+m, &curdata->seg.dev, 57);
//	        m+=57;
//	    }
//	
//	    return 0;
//	
//	}
uint8 ZY_CurrData(const PST_TableStr* tbl, PST_Frame* frm)//(const PST_DataStr* tbl, DATA_EXTRA* extra)
{
    S_HOURDATA_SEG0 hourdata;
    //S_QUARDATA_SEG0 quardata;
    S_CURDATA_SEG0  curdata;
    
    ZY_Frame* frame = &frm->_zy;
    //uint16 pn = extra->pn;
    //uint16 fn = extra->fn;
//	    uint8* pdata = frame->send + 7;         //�������ָ��
//	    uint8  n = extra->info[6];              //����
    uint8  time[8];                         //ʱ��
    uint16 idx;
    int m = 7;
    int exnum = 0;

    //if(frame->cmd == 0x01)    
    {
        memset(time,0,8);
        ByteArrayBcdToHex(frame->recv, time, 5);
        for(uint8 uc_i = 0; uc_i < CORE_TST_NO; uc_i++)
            
        {       
            if(!((tbl->tid == 0xA621 && gss_CDATATst[uc_i].info.type == 0x01) ||
                (tbl->tid == 0xA622 && gss_CDATATst[uc_i].info.type == 0x03)))//Ҫ��:1������ 3IC��������
            {
                continue;
            }
            if(gdw_ReadCureData(time, uc_i, (uint8 *)&curdata) == SYS_ERR_OK) //�����ߵ�����
            {
                if(SYS_ERR_OK != Netp_Get_Framework(gss_CDATATst[uc_i].pf, &gs_report_fw))
                {
                    continue;
                }

                if(tbl->tid == 0xA621)
                {
                    memcpy(frame->send+m, &curdata.seg.dev, 57);
                    m+=57;
                }
                else if(tbl->tid == 0xA622)
                {
                    if(curdata.pseg.mflag == 1)
                    {
                        memcpy(frame->send+m, &curdata.pseg.dev, 67);
                        m+=67;
                    }
                    else
                    {
                        memcpy(frame->send+m, &curdata.pseg.dev, 31);
                        m+=31;
                        memset(frame->send+m, 0, 36);
                        m+=36;
                    }
                }
                else
                {
                    continue;
                }
                exnum++;
            }
            else
            {
//	                frame->send[m++] = gss_CDATATst[uc_i].info.addr;
//	                memcpy(frame->send + m, gss_CDATATst[uc_i].info.factory_model, 4);
//	                m+=4;
//	                if(tbl->tid == 0xA621)
//	                {
//	                    memset(frame->send+m, 0xEE, 57);
//	                    m+=52;
//	                }
//	                else if(tbl->tid == 0xA622)
//	                {
//	                    memset(frame->send+m, 0xEE, 67);
//	                    //frame->send[m+31] = 0;
//	                    m+=67;
//	                }
//	                else
//	                {
//	                    continue;
//	                }
                
            }
            
        }
    }
    frame->send[m++] = gs_SysVar.batVal;

    frame->send[m++] = gs_FarpVar.mcsq;
    memcpy(frame->send + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
    m+=4;

    
    if(exnum > 0)
    {
        frame->send[0] = exnum;
        ByteArrayHexToBcd(time, frame->send+1, 6);//ͬʱ�ı����м����лظ�����ʼʱ��
        frame->len = m;
    }
    else
    {
        frame->len = 0;
        SYS_ERR();
    }

    SYS_OK();

}


uint8 ZY_WarningData(const PST_TableStr* tbl, PST_Frame* frm)//(const PST_DataStr* tbl, DATA_EXTRA* extra)
{
    ZY_Frame* frame = &frm->_zy;
    int exnum = 0;
    uint8 from;
    uint8 to;
    uint32_t len = 0;
    from = frame->recv[0];
    to =    frame->recv[1];
    
    
    len += ALRT_ReadLog(&from, &to, frame->send + 1, 0);
    //frame->send[0] = to;
    //frame->len = len+1;



    
    if(to > 0)
    {
        frame->send[0] = to;
        frame->len = len+1;
    }
    else
    {
        
        frame->send[0] = 0;
        frame->len = 1;
    }

    SYS_OK();

}


/************************************************************************
**Afn0AF10��ͨѶ�����ʶ�Ӧ�� 
*************************************************************************/
const uint8 BaudRate[8] = {0, 2, 4, 8, 16, 24, 32, 64};    //*300 = ������,Ĭ��1200bps
extern ST_Netp_FW gs_netp_fw_tmp[MCB_PORT_NUM];

/************************************************************************
 * @function: ParamLoad_F10
 * @����: ������������ò���
 * @����: 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: M�ֽ����λͼ+ n * 27����
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ParamLoad_MeterInfo(void)
{
    uint8 buf[32];
    uint8 bmps[LEN_TST_BMPS];
    uint8 tst;
    uint16 k;
    uint8 port;
                                            //���������
    ClearBuffer((uint8*)gss_CDATATst, sizeof(gss_CDATATst));
                                            //��ȡ���λͼ
    if(GD_Para_RW(F10_PADDR, bmps, LEN_TST_BMPS, false) != SYS_ERR_OK)
    {
        SYS_OK();
    }
                                            //ѭ��ɨ�����λͼ��Чλ
    for(k = 0; k < CORE_TST_NO; k++)
    {
        if(STR_IsBitClr(bmps, k))
        {
            continue;
        }
                                            //��ȡ���������
        if(GD_Para_RW(F10_PADDR + LEN_TST_BMPS + k * LEN_TST_CFG, buf, LEN_TST_CFG, false) != SYS_ERR_OK)
        {
            continue;
        }
                                            //��ȡ�������
        tst = buf[1] & 0x0F;
        
        port = buf[1] >> 4;
        if((tst == 0) || (tst > CORE_TST_NO) || port >= MCB_PORT_NUM)
        {
            continue;
        }
                                            //��Լ����
        gss_CDATATst[k].pt = port;
        //gss_CDATATst[k].pf = GetProtocolNum(gss_CDATATst[k].info.factory_model, gss_CDATATst[k].info.protocol);//buf[0];
        gss_CDATATst[k].pf = GetProtocolNum(buf+4, buf+6);

        //���ʼ��˿ں�
        //gss_CDATATst[k].bs = buf[3];//BaudRate[buf[3] >> 5];
        
        //if((gss_CDATATst[k].bs & 0xf0) == 0)
        {
            ST_Netp_FW * fw = &gs_netp_fw_tmp[gss_CDATATst[k].pt];
            uint8_t ret = Netp_Get_Framework(gss_CDATATst[k].pf, fw);
            
            if(ret != SYS_ERR_OK || fw->pf >= CON_MAX_DEVICE_TYPES)
            {
                continue;
            }
            gss_CDATATst[k].bs = fw->bs;
        }
        
//	        if(gss_CDATATst[k].bs == 0)
//	        {
//	            gss_CDATATst[k].bs = 6;
//	        }
                                            //��ַ
        MoveBuffer(buf, &gss_CDATATst[k].info, LEN_TST_CFG);
                                            //����
    }
    SYS_OK();

}
/************************************************************************
 * @function: RstParaF10
 * @����: 
 * 
 * @����: 
 * @param: pinfo 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/2/13)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 RstParaF10(PST_TableStr* pinfo)
{
    SetFlashFrom(DB_PARA, F10_PADDR, LEN_TST_BMPS+CORE_TST_NO*LEN_TST_CFG, 0);
                                        //������1Ĭ��Ϊ����
    //SetFlashFrom(DB_PARA, F10_PADDR, 1, 0x02);
    //GD_Para_RW(F10_PADDR + LEN_TST_BMPS + LEN_TST_CFG, (uint8*)DefParaF10, sizeof(DefParaF10), true);
//	    DLMS_ClrRegisMt();  //�����ѱ�����Ϣ
    
    SYS_OK();
}
//const PST_TableStr gss_PstZyTbl[];


const uint8 DefParaGWAddr[6] =
{                                       //�ն�ͨ�Ų���(�ȴ���վȷ�ϳ�ʱʱ��8��,�ȴ�ȷ���ط�����2��,���������ϱ�������վȷ��,����2����)
   1, 0, 0, 0, 0, 0x60
};
const uint8 DefParaCP[2] =
{                                       //�ն�ͨ�Ų���(�ȴ���վȷ�ϳ�ʱʱ��8��,�ȴ�ȷ���ط�����2��,���������ϱ�������վȷ��,����2����)
   0x2c, 0x01
};
//F3\F4Զ�̲���
const uint8 DefParaGPRS[96] =
{//0x73EC21A4 8012
    0x73, 0xEC, 0x21, 0xA4,  0x1F, 0x4C,        //����IP���˿�:115.236.33.164:8012
                                        //APN 
    'C', 'M', 'N', 'E', 'T', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    'u', 's', 'e', 'r', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    'p', 'w', 'd', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

}; 
const uint8 DefParaFtp[200] = {0};

/*************************************************************************************
**____�Զ����Լ�ı��
*************************************************************************************/
const PST_TableStr gss_PstZyTbl[] =
{
    {                       //�������
        0x2001,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ZY_Test,
        (void*)ZY_Test,
        0,
        0,
        
        0,
        (void*)ParamLoad_Test,
        __NULL,
        __NULL,
        
    },    

    {                                   //�����豸������Ԥ���Ѳ�Ʒ��
        0x8044,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ParaPrepayInfo,
        (void*)ParaPrepayInfo,
        0x0000,
        0x0000,
        
        0,
        __NULL,
        __NULL,
        __NULL,
        
    },    
    {                                   //��ֵ
        0x8045,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ParaRechargeInfo,
        (void*)ParaRechargeInfo,
        0x0000,
        0x0000,
        
        0,
        
        __NULL,
        __NULL,
        __NULL,
    },  
    {                                   //����
        0x8319,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ParaPriceAdjustInfo,
        (void*)ParaPriceAdjustInfo,
        0x0000,
        0x0000,
        
        0,
        
        __NULL,
        __NULL,
        __NULL,
    },  
    {                                   //RTU ftp����
        0x8353,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ZY_Para_Ftp,      
        (void*)ZY_Para_Ftp,
        RUNPARA_FTP_PLEN,
        RUNPARA_FTP,
        
        0,
        
        (void*)ParamLoad_Ftp,
        DefParaFtp,
        __NULL,
    },  
    {                                   //����
        0x8630,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        __NULL,        
        (void*)ZY_Valve_Ctr,
        0x0000,
        0x0000,
        
        PARA_TYPE_TASK,
        
        __NULL,
        __NULL,
        __NULL,
    },
    {                                   //RTU�ϸ�ʱ��
        0x8641,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ZY_Copy_Interval,      
        (void*)ZY_Copy_Interval,
        0x0002,
        PARA_COPY_INTERVAL,
        
        PARA_TYPE_PST,
        
        (void*)ParamLoad_CP,
        DefParaCP,
        __NULL,
    },  
    {                                   //RTU GPRS����
        0x8642,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ZY_Para_GPRS,      
        (void*)ZY_Para_GPRS,
        PARA_GPRS_PLEN,
        PARA_GPRS,
        
        PARA_TYPE_FARP,
        
        (void*)ParamLoad_Gprs,
        DefParaGPRS,
        __NULL,
    },  
    {                                   //���豸����
        0x8643,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ParaProcMeterInfo,
        __NULL,
        0x0000,
        0x0000,
        0,
        __NULL,
        __NULL,
        __NULL,
        
    },

    {                                   //Ӧ�ó���汾��
        0x8644,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ZY_AppVer,
        __NULL,
        0x0000,
        0x0000,
        
        0,
        
        __NULL, 
        __NULL,
        __NULL,        
    },

    {                                   //����������
        0x8645,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ZY_ReadSupportNetpFW,
        __NULL,
        0x0000,
        0x0000,
        
        0,
        
        __NULL, 
        __NULL,
        __NULL,        
    },
    {                                   //RTU�������
        0xA617,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ZY_HeartBeat,      
        (void*)ZY_HeartBeat,
        0x0002,
        PARA_HB,
        
        PARA_TYPE_FARP,
        
        ParamLoad_HB,
        DefParaCP,
        __NULL,
    },  

    {                                   //���Ӱ�װ�豸
        0xA618,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        __NULL,        
        (void*)ParaProcMeterInfo,
        0x0000,
        0x0000,
        PARA_TYPE_INP,
        (void*)ParamLoad_MeterInfo,
        __NULL,
        (void*)RstParaF10,
    },
    {                                   //��װ��
        0xA619,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ZY_OpenPackage,        
        (void*)ZY_OpenPackage,
        0x0000,
        0x0000,
        0,
        __NULL,
        __NULL,
        __NULL,
        
    },  
    {                                   //ɾ���Ѱ�װ���豸
        0xA620,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        __NULL,        
        (void*)ParaProcMeterInfoDel,
        0x0000,
        0x0000,
        0,
        (void*)ParamLoad_MeterInfo,
        __NULL,
        __NULL,
        
        
    },  
    
    {                                   //�����ϸ��(�󸶷�)
        0xA621,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ZY_CurrData,        
        (void*)ZY_Report_Confirm,
        0x0000,
        0x0000,
        0,
        __NULL,
        __NULL,
        __NULL,
        
    },  
    {                                   //�����ϸ��(Ԥ����)
        0xA622,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ZY_CurrData,        
        (void*)ZY_Report_Confirm,
        0x0000,
        0x0000,
        0,
        __NULL,
        __NULL,
        __NULL,
        
    },  
    {                                   //���豸����
        0xA623,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ParaProcMeterInfo,
        __NULL,
        0x0000,
        0x0000,
        
        0,
        __NULL,
        __NULL,
        __NULL,
        
    },
    
    {                                   //�澯�ϸ������֡
        0xA624,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ZY_WarningData,
        (void*)ZY_Report_Confirm,
        0x0000,
        0x0000,
        
        0,
        
        __NULL,   
        __NULL,
        __NULL,
        
    },  
    {                       //RTU��ַ�޸�
        0xC618,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ZY_GwAddr,
        (void*)ZY_GwAddr,
        6,
        F251_PADDR,
        
        PARA_TYPE_FARP,
        (void*)ParamLoad_F251,
        DefParaGWAddr,
        __NULL,
        
    },    
//	    {                                   //���ſ���
//	        0xC710,
//	        PST_TBL_SMLR1 | 3,
//	        0x0000,
//	        0x0000,
//	        __NULL,        
//	        (void*)ZY_Valve_Ctr,
//	        0x0000,
//	        0x0000,
//	        
//	        PARA_TYPE_TASK,
//	        
//	        __NULL,
//	        __NULL,
//	        __NULL,
//	    },    
    {
        0xC710,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        NULL,
        (void*)ZY_ParaRst,
        0,
        0,
        
        0,
        __NULL,
        __NULL,
        __NULL,
        
    },    
    {                                   //͸��
        0xFAF1,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        __NULL,        
        (void*)ZY_Transparent,

        0x0000,
        0x0000,
        
        0,
        
        __NULL, 
        __NULL,
        __NULL,
        
    },

    
    {
        0xFAF3,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        NULL,
        (void*)ZY_ParaRst,
        0,
        0,
        
        0,
        __NULL,
        __NULL,
        __NULL,
        
    },    
    {
        0xFC02,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ZY_OpTime,
        (void*)ZY_OpTime,
        0,
        0,
        
        0,
        __NULL,
        __NULL,
        __NULL,
        
    },    
    {
        0xFC03,
        PST_TBL_SMLR1 | 3,
        0x0000,
        0x0000,
        (void*)ZY_OpLog,
        (void*)ZY_OpLog,
        0,
        0,
        
        0,
        __NULL,
        __NULL,
        __NULL,
        
    },      
    
};

/************************************************************************
 * @function: LoadSystemParam
 * @����: ��������
 * 
 * @����: 
 * @param: PType ��������
 * 
 * @����: 
 * @return: uint8  �ɹ�/ʧ��
 * @˵��:
 * @����: yzy (2011-2-24)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 LoadSystemParam(uint16 PType)
{
    const PST_TableStr* pinfo;
    uint16 num;
    uint16 k;
    uint8 err = 0;
                                        //����
    PType &= PARA_TYPE_ALL;
                                        //ѭ��
    num = sizeof(gss_PstZyTbl)/ sizeof(PST_TableStr);

    for(k = 0; k < num; k++)
    {
        pinfo = gss_PstZyTbl + k;
        if(pinfo->type & PType)
        {
            if(pinfo->load != __NULL)
            {
                if((*(ftype_paraload)pinfo->load)() != SYS_ERR_OK)
                {
                    err++;
                }
            }
        }
    }

    if(err)
    {
        return SYS_ERR_FT;
    }
    SYS_OK();

}
//extern IWDG_HandleTypeDef hiwdg;

/************************************************************************
 * @function: RstSystemParam
 * @����: ������ʼ��,�ָ�����Ĭ�ϲ���
 * 
 * @����: 
 * @param: PType 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2011-4-1)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 RstSystemParam(uint16 PType)
{
    const PST_TableStr* pinfo;
    uint16 num;
    uint16 k;
    uint8 err = 0;
                                        //����
    PType &= PARA_TYPE_ALL;
                                        //��������ṹ�峤��
    num = sizeof(gss_PstZyTbl)/ sizeof(PST_TableStr);

    for(k = 0; k < num; k++)
    {
        pinfo = gss_PstZyTbl + k;
        if(pinfo->type & PType)
        {
#ifndef DEBUG        
            HAL_IWDG_Refresh(&hiwdg); //ι��
#endif
            if(pinfo->f_def != __NULL)      //����������뺯��
            {
                err += (*(ftype_pararst)pinfo->f_def)((PST_TableStr*)pinfo);
            }
            else if(pinfo->a_def != __NULL) //����������
            {
                err += GD_Para_RW(pinfo->addr, (uint8*)pinfo->a_def, pinfo->len, true);
            }
            else                            //����ȫ�����
            {
                err += SetFlashFrom(DB_PARA, pinfo->addr, pinfo->len, 0);
            }
        }
    }
    
    if(err)
    {
        return SYS_ERR_FT;
    }
    SYS_OK();
}


/************************************************************************
 * @function: ZY_JudgeAddr
 * @����: �Ƚ�ͨѶ��ַ�Ƿ�Ϸ�,ֻ�Ƚϸ�5���ֽ�
 * 
 * @����: 
 * @param: buf ͨ�ŵ�ַ������׵�ַ,����ռ�Ϊ6���ֽڵĳ���
 * 
 * @����: 
 * @return: uint8
            0: �Ƿ���ַ
            1: �㲥��ַ 9999999999
            2: ��λ��ַ ����A+�����ַ�
            4: ��ͨ���ַ
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 ZY_JudgeAddr(uint8* buf)
{
    uint8 data[6];                      //6���ֽڵ���ʱ�������û����ַ��
    uint8 index;                        //ѭ������

    for(index = 0; index < 6; index ++) //�жϹ㲥��ַ
    {
        if(buf[index] != 0x99)
        {
            break;
        }
    }
    if(index == 6)                      //�㲥��ַ,����1
    {
        return 1;
    }

    MoveBuffer(gs_PstPara.Addr, data, 6);  //��ȡ�ڲ�ͨѶ���ַ

    if(CmpBuffer(buf, data, 6) == 0)    //��ͨ���ַ����4
    {
        return 4;
    }

                                        //�ж��Ƿ�Ϊ���ܱ��ַ������λ���
    for(index = 12; index > 0; index--)
    {
        if((index - 1)& 0x01)
        {
            if(((buf[(index - 1)>>1] & 0xF0) != 0xA0) &&
               ((buf[(index - 1)>>1] & 0xF0) != (data[(index - 1)>>1] & 0xF0)))
            {
                break;
            }
        }
        else
        {
            if(((buf[(index - 1)>>1] & 0x0F) != 0x0A) &&
               ((buf[(index - 1)>>1] & 0x0F) != (data[(index - 1)>>1] & 0x0F)))
            {
                break;
            }
        }
    }

    if(index > 0)                       //û���ж����˳�����Ϊ�Ǵ���ı��
    {
        return 0;
    }
    else                                //������Ϊ����λ��Ż������ܱ��
    {
        return 2;
    }
}

/************************************************************************
**��������: uint8 GD_BuildLinkFrm(uint8 type, uint8* buffer)
**�� ��: ������¼֡,�˳�֡,����֡(AFN02)
**
**�� ��: type 1 ��¼֡ 2 �˳�֡ 3 ����֡
         buffer ���֡����
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
uint8 VS_BuildLinkFrm(uint8 type, uint16_t id, uint8* buffer)
{
    uint16 len;
    uint32 di;
    int m = 0;
    int len_pos = 0;
    TIME sectime;
    //uint8  time[16];
    //uint8  week;
//	    if(type == 0x08)
//	    {
//	        len = (13 << 2) + _VER_GDW_; 
//	    }
//	    else
//	    {
//	        len = (12 << 2) + _VER_GDW_;        //GDW376.1-2009
//	    }
    buffer[m++] = 0x68;
    memcpy(buffer+m, gs_PstPara.Addr, 6);
    m+=6;
    buffer[m++] = 0x68;
    buffer[m++] = type | 0x80;
    len_pos = m;
    m+=2;
    SYS_MCU_ReadBCDDateTime(&sectime);

    
    buffer[m++] = sectime.year;
    buffer[m++] = sectime.month;
    buffer[m++] = sectime.day;
    buffer[m++] = sectime.hour;
    buffer[m++] = sectime.min;
    buffer[m++] = sectime.sec;
    buffer[m++] = 0x80 | 0x06;
    buffer[m++] = id;
    buffer[m++] = id >> 8;
    
    buffer[m++] = guc_pseq;

    if(id == 0xA619)
    {
        memcpy(buffer+m, gs_PstPara.device, 20);
        m+=20;
    
        memcpy(buffer+m, gs_PstPara.Addr, 6);
        m+=6;
    }
    buffer[len_pos] = (uint8_t)(m - (len_pos + 2));
    buffer[len_pos+1] = (uint8_t)((m - (len_pos + 2)) >> 8);
    
//	    memcpy(buffer+len_pos, m+2,2);
//	    IntToByte(buffer + 1, len);         //������
//	    IntToByte(buffer + 3, len);
//	    buffer[6] = 0xC9;                   //������
//	                                        //����ն˵�ַ
//	    MoveBuffer(gs_PstPara.Addr, 
//	               buffer + 7, 
//	               sizeof(gs_PstPara.Addr));
//	    buffer[11] = 0;                     //����ַ��MSAΪ0
//	    buffer[12] = 0x02;                  //afn
//	    buffer[13] = SEQ_FIR_FIN | SEQ_CON | (guc_pseq++ & 0x0F); //seq(Ҫ����վȷ��)
//	                                        //���ݵ�Ԫ��ʶ
//	    if(type == 1)
//	    {
//	        di = F1 << 16;
//	    }
//	    else if(type == 2)
//	    {
//	        di = F2 << 16;
//	    }
//	    else
//	    {
//	        di = F3 << 16;
//	                                        //����֡���ϵͳ����ʱ��(13������ԼҪ��)
//	        //gs_RealTime.ReadBCDDateTime((TIME*)time);
//	        //MoveBuffer(time, buffer + 18, 6);
//	        //week = GetTime()->dweek;
//	        //if(week == 0)
//	        //{
//	        //week = 7;
//	        //}
//	        //buffer[22] += (week << 5);
//	        buffer[18] = gs_FarpVar.mcsq;
//	        
//	    }
//	    LongToByte(buffer + 14, di);
//	                                        //����֡
    len = PST_DoneFrame(PST_FRM_ZY_EN, buffer);
    
    return (uint8)len;                  //����֡�ĳ���
}


/************************************************************************
**��������: uint8 GD_BuildLinkFrm(uint8 type, uint8* buffer)
**�� ��: ������¼֡,�˳�֡,����֡(AFN02)
**
**�� ��: type 1 ��¼֡ 2 �˳�֡ 3 ����֡
         buffer ���֡����
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: 
**�� ��: 
*************************************************************************/
uint8_t VS_BuildReportFrm(uint8_t type, uint16_t id, uint16_t err, uint8* buffer)
{
    uint16 len;
    uint32 di;
    int m = 0;
    int len_pos = 0;
    TIME sectime;
    buffer[m++] = 0x68;
    memcpy(buffer+m, gs_PstPara.Addr, 6);
    m+=6;
    buffer[m++] = 0x68;
    buffer[m++] = type | 0x80;
    len_pos = m;
    m+=2;
    SYS_MCU_ReadBCDDateTime(&sectime);

    
    buffer[m++] = sectime.year;
    buffer[m++] = sectime.month;
    buffer[m++] = sectime.day;
    buffer[m++] = sectime.hour;
    buffer[m++] = sectime.min;
    buffer[m++] = sectime.sec;
    buffer[m++] = 0x80 | 0x05;
    buffer[m++] = id;
    buffer[m++] = id >> 8;
    
    buffer[m++] = guc_pseq;

//	    if(id == 0xA619)
//	    {
//	        memcpy(buffer+m, gs_PstPara.device, 20);
//	        m+=20;
//	    
//	        memcpy(buffer+m, gs_PstPara.Addr, 6);
//	        m+=6;
//	    }
    memcpy(buffer+m, (uint8_t *)&err, 2);
    m+=2;
    memcpy(buffer + m, (uint8_t *)&gs_SysVar.terstt.lword, 4);
    m+=4;

    
    buffer[len_pos] = (uint8_t)(m - (len_pos + 2));
    buffer[len_pos+1] = (uint8_t)((m - (len_pos + 2)) >> 8);
    

    //����֡
    len = PST_DoneFrame(PST_FRM_ZY_EN, buffer);
    
    return (uint8)len;                  //����֡�ĳ���
}


/************************************************************************
 * @function: PST_ZY_Decode
 * @����: �жϵ�ǰ���������Ƿ�Ϊ�Ϸ�֡,����ǵĻ��������Ϊ����ͨѶ�ṹ��
 * 
 * @����: 
 * @param: receive ���յ����ݵĻ���
 * @param: send ��Ҫ���͵����ݵĻ���
 * @param: sbuflen ���ͻ��泤��
 * @param: frame ���ݽ����ɵĽṹ��
 * 
 * @����: 
 * @return: uint8  PST_ERR_OK/PST_ERR_FT
 * @˵��: 
 * @����: yzy (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_ZY_Decode(uint8* receive, uint8* send, uint16 sbuflen, PST_Frame* frame)
{
    ZY_Frame* frm = &frame->_zy;
    if(ZY_JudgeAddr(receive + 1) == 0)  //��ַ���Ϸ�
    {
        return PST_ERR_FT;
    }
                                        //�����벻�Ϸ�
    frm->recv = receive + 21;           //���յ�������
    frm->send = send + 21;              //�������ݻ������ʼ��ַ
    frm->cmd = receive[8];              //������
    frm->len = receive[9]+receive[10]*0x100;              //����
    frm->time = &receive[11];
    frm->atr = receive[17];
    frm->seq = receive[20];

//	    if((frm->cmd != 0x01) &&
//	       (frm->cmd != 0x04) &&
//	       (frm->cmd != 0x05))
//	    {
//	        return PST_ERR_FT;
//	    }
                                        //������-33
//	    for(uint8 uc_i = 0; uc_i < frm->len; uc_i ++)
//	    {
//	        receive[uc_i + 10] -= 0x33;     //��33����
//	    }

    frm->wd.byte[0] = receive[18];
    frm->wd.byte[1] = receive[19];

//	    switch(frm->cmd)                    //��������ѡ����չ���ݱ�ʶ��λ��
//	    {
//	        case 0x01:                      //��ȡ����
//	            frm->wd.byte[2] = receive[12];
//	            frm->wd.byte[3] = receive[13];
//	            break;
//	        case 0x04:                      //д������
//	        case 0x05:
//	            frm->wd.byte[2] = receive[16];
//	            frm->wd.byte[3] = receive[17];
//	            /*
//	            frm->atr |= PST_FRM_ATRWR;  //��ʾд������
//	            frm->len -= 4;              //���ȼ�4,����
//	            frm->recv += 4;             //������ǰ�ƶ�4��
//	            */
//	            break;
//	        default:
//	            return PST_ERR_FT;          //��֧�������Ŀ�����
//	    }

    SYS_OK();                            //������ͨ��
}



/************************************************************************
 * @function: PST_ZY_Proc
 * @����: 
 * 
 * @����: 
 * @param: frm ���ݽ����ɵĽṹ��
 * 
 * @����: 
 * @return: uint8  PST_ERR_OK/PST_ERR_FT
 * @˵��: 
 * @����: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_ZY_Proc(PST_Frame* frm)
{
    ZY_Frame* frame = &frm->_zy;
    uint16 index;
    uint16 length;
    const PST_TableStr* tbl;
    uint8 err;

    //Уʱ
    TIME curTime;
    curTime.year = frame->time[0];
    curTime.month = frame->time[1];
    curTime.day = frame->time[2];
    curTime.hour = frame->time[3];
    curTime.min = frame->time[4];
    curTime.sec = frame->time[5];
    uint32_t t1 = CalcSecs((uint8_t *)&(GetTime()->sec), false);
    uint32_t t2 = CalcSecs((uint8_t *)&curTime.sec, true);
    if(abs(t2 - t1) > 300)
    {
        if(SYS_ERR_OK == SYS_MCU_WriteBCDDateTime(&curTime))
        {
            gs_SysVar.terstt.bit.rtcstt = 0;
        }
    }

    if(frame->cmd == 0x08 || frame->cmd == 0x09)
    {
        SYS_OK();
    }

    index = ADVMidWaySearch(
        (uint8*)gss_PstZyTbl,
        sizeof(gss_PstZyTbl) / sizeof(PST_TableStr),
        sizeof(PST_TableStr),
        frame->wd.word[0]
        );

    if(index == 0xFFFF)                 //û���������򷵻����ݱ�ʶ����
    {
        frame->len = 2;                 //����sendָ��
        frame->send -= 21;
        err = PST_ERR_DI;
                                        //���ݱ�ʶ����
        LongToByte(frame->send + 10, frame->wd.word[0]);
        goto PST_ZY_PROC_PACK;
    }

    tbl = gss_PstZyTbl + index;         //���ݽṹ

    if(frame->cmd == 0x01)
    {
        if(tbl->read == __NULL)
        {
            frame->len = 10;
            err = PST_ERR_FT;           //������ݱ�ʶ��֧�ֶ�,ֱ�ӻظ�����֡
        }
        else
        {                               //ִ�ж�����
            err = ((PSTTblPrc)tbl->read)(tbl, frm);
        }
    }
    else                                //д�����ݵ�����
    {
        if(tbl->write == __NULL)
        {
            frame->len = 10;
            err = PST_ERR_FT;           //������ݱ�ʶ��֧��д,ֱ�ӻظ�����֡
        }
        else
        {                               //ִ��д����
            err = ((PSTTblPrc)tbl->write)(tbl, frm);
            
            if(tbl->load != __NULL)       //�������
            {
                (*(ftype_paraload)tbl->load)();
            }
            
        }
    }

//	    frame->len += 2;
    frame->send -= 21;
                                        //���ݱ�ʶ
    IntToByte(frame->send + 18, frame->wd.word[0]);

PST_ZY_PROC_PACK:

    if(err == SYS_ERR_NOREPLY)               //����Ҫ�ظ�����֡,ֱ���˳�
    {
        SYS_OK();
    }

    PST_ZY_Pack(frm, err, &length);     //�������֡

                                        //��������
    if(false == WdtTestPreStartPush(frame->send, length, frm->chninfo & PST_FRM_CHNNO))//�߱����Ź����Թ��ܵĶ������
    {
        gfs_PSTChnSend[frm->chninfo & PST_FRM_CHNNO](frame->send, length);
    }

    SYS_OK();
}


/************************************************************************
 * @function: PST_ZY_Pack
 * @����: ����Դ֡���д��,����ͨѶ
 * 
 * @����: 
 * @param: frame ����Ľṹ��
 * @param: err 
 * @param: length ����Ϊ����������󳤶�,���Ϊ�����֮��ĳ���
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 PST_ZY_Pack(PST_Frame* frame, uint8 err, uint16* length)
{
    ZY_Frame* frm = &frame->_zy;
    TIME curTime;
    SYS_MCU_ReadBCDDateTime(&curTime);
    int data_len = frm->len + 10;
    if(err != PST_ERR_OK)
    {
        frm->send[8] = frm->cmd | 0xC0; //������
    }
    else
    {
        frm->send[8] = frm->cmd | 0x80; //������
    }
    
    frm->send[9] = data_len;            //����
    frm->send[10] = data_len >> 8;            //����
                                        //+33����
//	    for(uint8 uc_i = 0; uc_i < frm->send[9]; uc_i++)
//	    {
//	        frm->send[uc_i + 10] += 0x33;
//	    }

                                        //��ַ
    memcpy(frm->send + 1, gs_PstPara.Addr, 6);

    frm->send[11] = curTime.year;
    frm->send[12] = curTime.month;
    frm->send[13] = curTime.day;
    frm->send[14] = curTime.hour;
    frm->send[15] = curTime.min;
    frm->send[16] = curTime.sec;
    frm->send[17] = frm->atr;
    frm->send[18] = frm->wd.byte[0];
    frm->send[19] = frm->wd.byte[1];
    frm->send[20] = frm->seq;

                                        //�������
    *length = gs_FRM.DoneFrame(PST_FRM_ZY_NO, frm->send);

    if(*length > 0)                     //�������>0
    {
        SYS_OK();
    }
    return PST_ERR_FT;                  //���ط�����������
}







/************************************************************************
 * @function: PST_ZY_Do0F
 * @����: ����Դ֡���д��,����ͨѶ
 * 
 * @����: 
 * @param: data: 0F��һ���ֽڵĻ�����ָ��,��ʽ:C,DI1,DI0,������
 * @param: len: �������������ܳ���,����Ϊ��������������
 * 
 * @����: 
 * @return: uint8  
 * @˵��: �����ն˹�Լ0F������
 *      �����������Ӵ���ָ�뿪ʼ����,���ΰ���C,DI1,DI0,������
 *      ������C:01Ϊ��,04/05Ϊд
 * @����: yzy (2014/5/27)
 *-----------------------------------------------------------------------
 * @�޸���: 
********************************************************************************************************/
uint8 PST_ZY_Do0F(uint8* data, uint16* len)
{
    PST_Frame frm;
    DLT_Frame* frame = &frm._dlt;
    uint16 index;
    const PST_TableStr* tbl;
    uint8 err;

    frame->tstno = 0;
    frame->len = *len;
    frame->cmd = data[0];                  //������ ����Ϊ01/04/05

    frame->wd.byte[0] = data[1];           //���ݱ�ʶ
    frame->wd.byte[1] = data[2];
    frame->wd.byte[2] = 0;
    frame->wd.byte[3] = 0;
    
    frame->len -= 2;
    frame->recv = data + 3;
    frame->send = data + 3;

    if((frame->cmd != 0x01) &&
       (frame->cmd != 0x04) &&
       (frame->cmd != 0x05))
    {
        err = PST_ERR_FT;
        goto PST_ZY_DO0F_RET;
    }

    index = ADVMidWaySearch(
        (uint8*)gss_PstZyTbl,
        sizeof(gss_PstZyTbl) / sizeof(PST_TableStr),
        sizeof(PST_TableStr),
        frame->wd.word[0]
        );

    if(index == 0xFFFF)                 //û���������񷵻����ݱ�ʶ����
    {
        err = PST_ERR_DI;
        goto PST_ZY_DO0F_RET;
    }

    tbl = gss_PstZyTbl + index;         //���ݽṹ

    if(frame->cmd == 0x01)
    {
        if(tbl->read == __NULL)
        {
            err = PST_ERR_FT;           //������ݱ�ʶ��֧�ֶ���ֱ�ӻظ�����֡
            goto PST_ZY_DO0F_RET;
        }
        else
        {                               //ִ�ж�����
            frame->len = 220;           //��ȡ���ݵĳ���
            err = ((PSTTblPrc)tbl->read)(tbl, &frm);
        }
    }
    else                                //д�����ݵ�����
    {
        if(tbl->write == __NULL)
        {
            err = PST_ERR_FT;           //������ݱ�ʶ��֧��д��ֱ�ӻظ�����֡
            goto PST_ZY_DO0F_RET;
        }
        else
        {                               //ִ��д����
            err = ((PSTTblPrc)tbl->write)(tbl, &frm);
        }
    }
    
PST_ZY_DO0F_RET:
    if(err == SYS_ERR_NOREPLY)               //�������Ҫ�ظ�������PST_ERR_RE
    {
        frame->len = 0;
        return SYS_ERR_NOREPLY;
    }
    
    if(err != PST_ERR_OK)               //�ظ�������
    {
        data[0] = frame->cmd + 0xC0;
        frame->len = 1;
        frame->send[0] = err;
    }
    else
    {
        data[0] = frame->cmd + 0x80;
    }

    IntToByte(data + 1, frame->wd.word[0]); //���ݱ�ʶ
    *len = 3 + frame->len;

    return err;
}





/************************************************************************
 * @function: Task_ExecuteA
 * @����: 1��������
 * 
 * @����: 
 * @param: frame-֡�����ṹ��.(����ģ������֡����)
 **          frame.recv //�������
 **          frame.send //���ͻ�����
 **          frame.tbd\tstno(��CONλ)\cmd\atr����д,���:Task_Proc()
 * @param: st_bk ��һ���ʱ��(���ʱ������)
 * @param: nt_bk ��ǰʱ��(���ʱ������)
 * @param: con 
 * @����: 
 * @˵��: 1.extra.info[7]Ϊ�����ܶ� 
 **       2.����̵���,�����ڱ�����ѯ,�����������ϱ�
 * @����: yzy (2011-5-19)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void Task_Execute(PST_Frame* frm,  uint8* st_bk, uint8* nt_bk, bool con)
{
    ZY_Frame* frame = &frm->_zy;
    const PST_TableStr* tbl;
    DATA_EXTRA extra;
    REP_CFG repcfg;
    uint8* sbuf = frame->send;
    uint8* pctx;                            //�������׸�diλ��
    uint8  num = frame->recv[8];            //�����di����
    uint8  st[6];                           //��ʼʱ��,������,smhDMY
    uint8  nt[6];                           //��ǰʱ��,������,smhDMY
    uint8  rbuffer[7];                      //�������ݵ�Ԫ
    uint16 fn_err = 0;                      //ʧ�ܵ�fn��
    uint16 fn_num = 0;                      //�����fn��
    uint16 len_frm = 0;                     //�ܳ���
    uint16 len_di = 0;                      //��ʶ�����ݳ���

    repcfg.en = con ? 1 : 0;

//	    uint8 err =((ftype_0cproc)tbl->proc)(tbl, &extra);

    uint16 index = ADVMidWaySearch(
        (uint8*)gss_PstZyTbl,
        sizeof(gss_PstZyTbl) / sizeof(PST_TableStr),
        sizeof(PST_TableStr),
        frame->wd.word[0]
        );
    if(index == 0xFFFF)                 //û���������򷵻����ݱ�ʶ����
    {
//	        frame->len = 2;                 //����sendָ��
//	        frame->send -= 21;
//	        err = PST_ERR_DI;
//	                                        //���ݱ�ʶ����
//	        LongToByte(frame->send + 10, frame->wd.word[0]);
        return;
    }

    tbl = gss_PstZyTbl + index;         //���ݽṹ
    if(tbl->tid == 0xA621)
    {
        repcfg.pseq = 0;
    }
    else
    {
        repcfg.pseq = 1;
    }
    
    uint8 err = ((PSTTblPrc)tbl->read)(tbl, frm);
    if(SYS_ERR_OK == err)
    {
        frame->send -= 21;
        
        if(PST_ERR_OK == PST_ZY_Pack(frm,err,&len_frm))
        {
            REP_ChnSend(frame->send, len_frm, frm->chninfo, &repcfg);
        }
    }

    gs_OS.TK_Sleep(500);

}






