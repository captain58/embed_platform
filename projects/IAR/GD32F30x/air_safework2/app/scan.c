/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: scan.c
**��   ��   ��: yzy
**����޸�����: 2019��6��1��
**��        ��: �ͼ�ɨ����̵ĺ����ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2019��6��1��
** �衡��: ԭʼ�汾
********************************************************************************************************/

#define EXT_SCAN

#include "public.h"
#include "lib_com/calclib_gw.h"
#include "hlv.h"
#include "netp.h"
#include "task.h"
#include "netp_fm.h"
#include "alert.h"
#include <sal_import.h>

#include "atmodem.h"
#include "paradef.h"
cpu_stack_t  gs_TKSlvStack[TASK_SLV_STKL];
ktask_t      gs_TKSlvHandle;
kbuf_queue_t gs_TKSlvQueue;
char         gc_TKSlvQbuf[MSG_BUFF_LEN*2];
static char         g_TKSlvQ_buf_recv[BUFQUEUE_MSG_MAX+4];
static size_t       g_TKSlvQ_recv_size;

uint32_t g_ulFtp_Timeout = 0;
extern kbuf_queue_t gs_TKFarpQueue;

//�ⲿflash��غ궨��
#define FLS_PROGRAM_ADDR         (4096 + 256)       //�������ⲿflash�е�λ��(ǰ32�ֽ��������upatemark)
#define FLS_PROGRAM_MAXLEN       524288     //�����ܿռ��С512K�ֽ�


#define FLS_UPDATEMARK_ADDR      0          //����������������Flash�е�λ��

/*******************************************************************************
**�����������������Զ�������ı��,�Ӷ�����������ʱ������ͣ��
**�ڸ�λ֮��õ��ظ�
********************************************************************************/
const uint32 gul_UpdataMarkA_APP[4] = 
{
    0x12345678, 0x87654321, 0x11223344, 0x55667788
};


const uint32 gul_UpdataMarkB_APP[4] = 
{
    0,0,0,0
};



/************************************************************************
 * @function: CheckFileHead
 * @����: У��dpg�ļ�ͷ�ĸ�ʽ
 * 
 * @����: 
 * @param: head  
 * 
 * @����: 
 * @return: uint8 SYS_ERR_OK, SYS_ERR_FT
 * @����: xugl (2010-4-23)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 CheckFileHead(uint8* head)
{
    uint8 tmp;
    tmp = DoSum(head, 31);
    if(head[31] != tmp)
    {
        return SYS_ERR_FT;
    }

    if(CmpBuffer(head, "DPG2", 4))
    {
        return SYS_ERR_FT;
    }
                                        //���̴������ƥ���Ϊȫ��,�����˳�
    if(CmpBuffer(head + 4, (uint8*)gucs_PrjCode, 6) && 
       CmpBuffer(head + 4, "AAAAAA", 6))
    {
        return SYS_ERR_FT;
    }

    SYS_OK();
}
#define UPGRADE_TYPE_PROC   0
#define UPGRADE_TYPE_PARA   1
#define UPGRADE_TYPE_NETP   2
#ifdef UPGRADE_CHECK

extern const char g_programarray[];
#endif
//extern IWDG_HandleTypeDef hiwdg;
/***********************************************************
 * @function_name: ES_APP_IAP_Check
 * @function_file: iap_file.c
 * @����: ����Flash����������ĺϷ���         
 * @����: 
 * @param:  startAddr  ��������λ��FLASH����ʼƫ��
 * @param�� length �������ݳ���
 * @����: 
 * @return: true  ���ݺϷ�,����ʼ���г�������
           false  ���ݷǷ�,�����ܽ��г�������
 * @����:  xugl (2010-11-11)
 *******************************************************/
bool IAP_CheckCRC(uint32 startAddr, uint32 length)
{
    uint8 uc_data[64];                  //����������ȡ������
    uint16 crc = 0xFFFF;                //CRCУ��ֵ
    uint16 cmp;                         //CRCУ��ֵ�ĶԱ�
    int i = 0;
    uint32 curAdress = startAddr;		//������ʼ��ַ
    
    if((length & 0x03) != 0)            //�жϳ��ȵĺϷ���
    {
        return false;                   //���ش���
    }
    
    SYS_FILE_Open();
    
    while(length > 0)                   //���ϵĶ�ȡ����Ȼ�����У��
    {
        if(length > 64)                //����64�ֽڶ�64�ֽ�
        {
            
            SYS_FILE_Read(uc_data, curAdress, 64);
            crc = DoCrcRtuModbus(uc_data, 64, crc);
#ifdef UPGRADE_CHECK
            for(i = 0; i < 64; i ++)
            {
                if(uc_data[i] != g_programarray[curAdress- startAddr+ i])
                    break;
            }  
            if(i != 64)
            {
                break;
            }
#endif
            length -= 64;
            curAdress += 64;

            
        }
        else                            //�����ȡʣ�����ݵĳ���
        {
            SYS_FILE_Read(uc_data, curAdress, length);
#ifdef UPGRADE_CHECK            
            for(i = 0; i < length; i ++)
            {
                if(uc_data[i] != g_programarray[curAdress - startAddr+ i])
                    break;
            }   
            if(i != length)
            {
                break;
            }
#endif
                                        //����4���ֽ���CRCУ��
            crc = DoCrcRtuModbus(uc_data, length, crc);
                                        //��ȡCRCУ��ĶԱ�ֵ
            SYS_FILE_Read(&cmp, curAdress+length, 2);
            //cmp = ByteToInt(uc_data + length);
            length = 0;                 //�Ѿ���ȡ�����е�����
        }
#ifndef DEBUG        
        HAL_IWDG_Refresh(&hiwdg); //ι��
#endif
    }
    
    SYS_FILE_Close();
    
    if(cmp != crc)                      //�ж�CRC��У��ֵ�Ƿ����
    {
        return false;
    }
    return true;                        //����OK        
}

/***********************************************************
 * @function_name: ES_APP_IAP_Check
 * @function_file: iap_file.c
 * @����: ����Flash����������ĺϷ���         
 * @����: 
 * @param:  db  ���ݿ�
 * @param�� len ����
 * @����: 
 * @return: true  ���ݺϷ�,����ʼ���г�������
           false  ���ݷǷ�,�����ܽ��г�������
 * @����:  xugl (2010-11-11)
 *******************************************************/
bool IAP_Check(uint32 length)
{
	return IAP_CheckCRC(FLS_PROGRAM_ADDR, length);
}

/***********************************************************
 * @function_name: ES_IAP_Mark
 * @function_file: iap_file.c
 * @����: ���û�������������         
 * @����: 
 * @param:   setorclr ���û������������� 
 * @param:   ID       ����ID
 * @param:  length   ���ݳ���
 * @param:  address  ���ݵ�ַ
 * @����: 
 * @return:  
 * @����:  xugl (2010-11-11)
 *******************************************************/
void IAP_Mark(bool setorclr, uint32 length, uint32 address)
{
    uint8 uc_data[28];   

    if(setorclr)                        //���ñ��
    {
                                        //����д����
        memcpy(uc_data, (void*)gul_UpdataMarkA_APP, sizeof(gul_UpdataMarkA_APP));
        uc_data[16] = 0;               //����ID
                                        //����
        LongToByte(uc_data + 18, length+4);
                                        //��ַ
        LongToByte(uc_data + 22, address);
                                        //����CRCУ��
        IntToByte(uc_data + 26, DoCrcRtuModbus(uc_data, 26, 0xFFFF));
    }
    else                                //�����������ݾͿ�����
    {
        memcpy(uc_data, (void*)gul_UpdataMarkB_APP, sizeof(gul_UpdataMarkB_APP));
    }
    
    SYS_FILE_Open();
	SYS_FILE_Write(uc_data, FLS_UPDATEMARK_ADDR, 28);
    SYS_FILE_Close();
}

/************************************************************************
 * @function: ES_IAP_Ready
 * @����: 
 * 
 * @����: 
 * @param: address ������ʼ��ַ
 * @param: length �û�������Ҫ�����ĳ���ĳ���
 * @param: isreset ������֮���Ƿ���Ҫ��λ�û�����
 * 
 * @����: 
 * @return: bool  true�ɹ�/falseʧ��
 * @˵��: 
 * @����: xugl (2013/7/16)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
bool IAP_Ready(uint32 address, uint32 length, bool isreset)
{
    if(length > 0x80000)                //��֧����ô������������
    {
        return false;                   //���ش���
    }
                                        //ȷ���������ݵĺϷ���,���������ͱ��ݲ���CRC������ȷ����Ϊ����ȷ
    if(!IAP_Check(length))
    {
	    return false;                   //���ش���
    }
                                        //д��������־
    IAP_Mark(true, length, address);
                                        //����ϵͳ��λ
    

    return true;
}
int Upgrade_Netp_FW(uint32_t len)
{
    TDataBlock db;
//    ST_NETP_FW_BASE tmp;
    uint32_t addr = 256;
    
//    while (len >= sizeof(ST_NETP_FW_BASE))
//    {
//        
//        if(SYS_FILE_DB_Open(DB_UPDATE, &db, TDB_MODE_RW) != SYS_ERR_OK)
//        {
//            return -1;
//        }  
//        if(SYS_ERR_OK != SYS_FILE_DB_ReadFrom(&db, (uint8_t * )&tmp, sizeof(ST_NETP_FW_BASE), addr))
//        {
//            SYS_FILE_DB_Close(&db); 
//            return -1;
//        }
//        SYS_FILE_DB_Close(&db); 
//        if(SYS_ERR_OK != Netp_Register_Set(&tmp))
//        {
//            return -1;
//
//        }
//        len -= sizeof(ST_NETP_FW_BASE);
//        addr += sizeof(ST_NETP_FW_BASE);
//    }

    

    return 0;
}
/************************************************************************
 * @function: Farp_WM_FTP_Open
 * @����: ����GPRS����
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/1/14)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
int Farp_WM_FTP_Open(void)
{
    if(SYS_ERR_OK != HAL_FTP_Check(gs_FtpPara.ftp_user, gs_FtpPara.ftp_pwd))
    {
        return -1;
    }
    if(SYS_ERR_OK != HAL_FTP_Start(gs_FtpPara.ftp_domain, gs_FtpPara.port, gs_FtpPara.ftp_fold))
    {
        return -1;
    }
    return 0;
}
#define CON_FTP_TRANS_LEN 1024
#ifdef UPGRADE_CHECK
uint8_t guc_ftpbufbak[CON_FTP_TRANS_LEN];
#endif
const uint8_t FW_KEY[16] ={'C','o','p','y','r','i','g','h','t',' ','H','R','C','o','m','m'};
int Farp_WM_FTP_Get(uint16_t * val)
{
//	    uint8_t outbuf[1000];
    uint8_t * guc_ftpbuf = NULL;

    uint16_t len = CON_FTP_TRANS_LEN;
    guc_ftpbuf = aos_malloc(len);
    if(guc_ftpbuf == NULL)
    {
        LOG_ERROR(DBGFMT_A"malloc failed! \n", DBGARG_A);
        return -1;
    }
    memset(guc_ftpbuf,0,CON_FTP_TRANS_LEN);
    TDataBlock db;
    int ret  = HAL_FTP_Get(gs_FtpPara.ftp_file, guc_ftpbuf, gs_FtpPara.getLen, &len);
    
//	    if(SYS_ERR_OK != HAL_FTP_Get(gs_FtpPara.ftp_file, outbuf, gs_FtpPara.getLen, &len))
//	    {
//	        return -1;
//	    }
    uint8_t * pxor = guc_ftpbuf;
    for(int loop=0;loop<len;loop++)
    {
        *pxor = (*pxor)^FW_KEY[loop&0xf];
        pxor++;
    }

    if(ret == SYS_ERR_OK)
    {
        if(gs_FtpPara.getLen == 0)
        {
            if(SYS_ERR_OK == CheckFileHead(guc_ftpbuf))
            {
            
                if(SYS_FILE_DB_Open(DB_UPDATE, &db, TDB_MODE_RW) != SYS_ERR_OK)
                {
                    aos_free(guc_ftpbuf);
                    return -1;
                }  
                if(SYS_ERR_OK != SYS_FILE_DB_WriteFrom(&db, guc_ftpbuf, len, gs_FtpPara.getLen))
                {
                    SYS_FILE_DB_Close(&db); 
                    aos_free(guc_ftpbuf);
                    return -1;
                }
                SYS_FILE_DB_Close(&db); 

            }
            else
            {
                aos_free(guc_ftpbuf);
                return 2;
            }
            
        }
        else
        {
            if(SYS_FILE_DB_Open(DB_UPDATE, &db, TDB_MODE_RW) != SYS_ERR_OK)
            {
                aos_free(guc_ftpbuf);
                return -1;
            }  
            if(SYS_ERR_OK != SYS_FILE_DB_WriteFrom(&db, guc_ftpbuf, len, gs_FtpPara.getLen))
            {
                SYS_FILE_DB_Close(&db); 
                aos_free(guc_ftpbuf);
                return -1;
            }
            SYS_FILE_DB_Close(&db); 

            

        }
#ifdef UPGRADE_CHECK

        if(SYS_FILE_DB_Open(DB_UPDATE, &db, TDB_MODE_RW) != SYS_ERR_OK)
        {
            aos_free(guc_ftpbuf);
            return -1;
        }  

        if(SYS_ERR_OK != SYS_FILE_DB_ReadFrom(&db, guc_ftpbufbak, len, gs_FtpPara.getLen))
        {
            SYS_FILE_DB_Close(&db); 
            aos_free(guc_ftpbuf);
            return -1;
        }

        
        SYS_FILE_DB_Close(&db);

        if(CmpBuffer(guc_ftpbufbak, guc_ftpbuf, len) != 0)
        {
            aos_free(guc_ftpbuf);
            return -100;
        }

#endif        
        *val = len;
        if(len != CON_FTP_TRANS_LEN)
        {
            aos_free(guc_ftpbuf);
            return 1;
        }
        else
        {
            aos_free(guc_ftpbuf);
            return 0;
        }
    }
    else if(ret == 1)
    {
        aos_free(guc_ftpbuf);
        return 3;
    }
    else
    {
        aos_free(guc_ftpbuf);
        return -1;
    }
}

uint8 guc_FtpLinkWaitSec;
extern uint16 gui_FarpWmRcSec;
extern uint8 RUN_Para_RW(uint32 addr, uint8* buff, uint16 len, bool rw);

int Task_Ftp(void)
{
    uint8_t outbuf[64];
    uint16_t framelen = 0;
    REP_CFG repcfg;
    TDataBlock db;
    int ret = 0;
    if(gs_SysVar.mDGstt & HLV_STT_NENG)
    {
        return 0;
    }
    
    if(gs_FarpVar.wmst & WMS_STT_GSM)//���źž�����
    {
//	        frm.chninfo = PST_CHN_TCP;        //0����ͨ��;3Զ��ͨ��
    }
    else
    {
        //if(gs_FarpVar.wmst & WMS_STT_GSM)     //�������ź�
//	        {
//	            if(guc_FtpLinkWaitSec > 0)     //���ڵȴ���������
//	            {
//	                guc_FtpLinkWaitSec--;
//	                return ;
//	            }
//	            if(gs_FtpPara.flag == FTP_STEP_OPEN || gs_FtpPara.flag == FTP_STEP_ING)
//	            {
//	                extern kbuf_queue_t gs_TKFarpQueue;
//	                gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FARP_PRIOCNT], 1);
//	                guc_FtpLinkWaitSec = 30;
//	                return ;
//	            }
//	        }

        return ret;
    }


    if(g_ulFtp_Timeout)
    {
        g_ulFtp_Timeout--;
        return ret;
    }
    if(gs_FtpPara.flag == FTP_STEP_WAIT || gs_FtpPara.flag == FTP_STEP_START)
    {
        uint32_t t1 = CalcSecs((uint8_t *)&(GetTime()->sec), false);
        uint32_t t2 = CalcSecs((uint8_t *)&gs_FtpPara.time.sec, false);
        if(t1 > t2)
        {
            gs_FtpPara.flag = FTP_STEP_OPEN;
            gs_FtpPara.fileLen = 0;
            gs_FtpPara.getLen = 0;
            ret = 1;
        }
        else
        {
            ret = 0;
        }
        gs_FtpPara.err_times = 0;
    }
    else if(gs_FtpPara.flag == FTP_STEP_OPEN)
    {
        
//	        gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FTP_OPEN], 1);
        if(0 == Farp_WM_FTP_Open())
        {
            gs_FarpVar.wmst |= WMS_STT_FTP;
//	            gs_OS.Message_Send(&gs_TKSlvQueue, &msgidA[MSG_FTP_OPEN_SUCC], 1);
            g_ulFtp_Timeout = 0;
            gs_FtpPara.flag = FTP_STEP_ING;
            gs_FtpPara.err_times = 0;

        }
        else
        {
            gs_FarpVar.wmst &= ~WMS_STT_FTP;
            krhino_buf_queue_send(&gs_TKFarpQueue, &msgidA[MSG_FARP_DISGPRS], 1);
            gui_FarpWmRcSec = 10;
//	            gs_OS.Message_Send(&gs_TKSlvQueue, &msgidA[MSG_FTP_OPEN_FAILED], 1);
            gs_FtpPara.err_times++;
            g_ulFtp_Timeout = 60 * gs_FtpPara.err_times;//15�볬ʱ

        }
        ret = 1;
    }
    else if(gs_FtpPara.flag == FTP_STEP_ING)
    {
//	        gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FTP_GET], 1);
//	        g_ulFtp_Timeout = 120;//15�볬ʱ
        uint16_t glen = 0;
        uint8_t msg_buf[3];
        for(int i = 0; i < 180; i ++)
        {
            HB_RetLive(TASK_SLV_TKID);
            int ret = Farp_WM_FTP_Get(&glen);
            gs_SysVar.mDGstt |= HLV_STT_USBE;
            if(0 == ret)
            {
    //	            msg_buf[0] = MSG_FTP_GET_SUCC;
    //	            memcpy(msg_buf+1, (uint8_t *)&glen, 2);
    //	        
    //	            gs_OS.Message_Send(&gs_TKSlvQueue, msg_buf, 3);
    //	        uint16_t len = 0;
    //	        memcpy((uint8_t *)&len, &g_TKSlvQ_buf_recv[1], 2);
                gs_FtpPara.getLen += glen;
                gs_FtpPara.fileLen += glen;
                gs_FtpPara.err_times = 0;
                
                g_ulFtp_Timeout = 0;
                gs_FarpVar.wmst |= WMS_STT_FTP;
    //	                    uint8 msg = MSG_FARP_RECVVALIDIP;
                krhino_buf_queue_send(&gs_TKFarpQueue, &msgidA[MSG_FARP_RECVVALIDIP], 1);
                if((gs_FtpPara.getLen % 0x1000) == 0)
                    RUN_Para_RW(RUNPARA_FTP, &gs_FtpPara, RUNPARA_FTP_PLEN, true);

            }
            else if(ret == 1)
            {
    //	            msg_buf[0] = MSG_FTP_GET_FNS;
    //	            memcpy(msg_buf+1, (uint8_t *)&glen, 2);
    //	        
    //	            gs_OS.Message_Send(&gs_TKSlvQueue, msg_buf, 3);
                HAL_FTP_Close();
                gs_FarpVar.wmst &= ~WMS_STT_FTP;
    //	uint16_t len = 0;
    //	            memcpy((uint8_t *)&len, &g_TKSlvQ_buf_recv[1], 2);
                gs_FtpPara.getLen += glen;
                gs_FtpPara.fileLen += glen;
                gs_FtpPara.err_times = 0;
                
                g_ulFtp_Timeout = 0;
                gs_FtpPara.flag = FTP_STEP_FINISH;            
                krhino_buf_queue_send(&gs_TKFarpQueue, &msgidA[MSG_FARP_RECVVALIDIP], 1);
                break;
            }
            else if(ret == 2)
            {
    //	            gs_OS.Message_Send(&gs_TKSlvQueue, &msgidA[MSG_FTP_PROJ_FAILED], 1);
                g_ulFtp_Timeout = 2;
                gs_FtpPara.flag = FTP_STEP_FAILED;

                HAL_FTP_Close();
                gs_FarpVar.wmst &= ~WMS_STT_FTP;
                break;

            }
            else if(ret == 3)//CME err 625
            {
                gs_FtpPara.err_times++;
                g_ulFtp_Timeout = 15*gs_FtpPara.err_times;
                gs_FtpPara.flag = FTP_STEP_OPEN;
                HAL_FTP_Close();
                gs_FarpVar.wmst &= ~WMS_STT_FTP;   
                break;
            }
            else    
            {
    //	            gs_OS.Message_Send(&gs_TKSlvQueue, &msgidA[MSG_FTP_GET_FAILED], 1);
                gs_FtpPara.err_times++;
                if(gs_FtpPara.err_times > 2)
                {
                
                    g_ulFtp_Timeout = 15*gs_FtpPara.err_times;
                    gs_FtpPara.flag = FTP_STEP_OPEN;
                    HAL_FTP_Close();
                    gs_FarpVar.wmst &= ~WMS_STT_FTP;     
                }
                else
                {
                    g_ulFtp_Timeout = 5;
                }
                break;
            } 
        }
        ret = 1;

    }
    else if(gs_FtpPara.flag == FTP_STEP_FINISH)
    {
//	        gs_OS.Message_Send(&gs_TKFarpQueue, msgidA[MSG_FTP_GET], 1);
//	        g_ulFtp_Timeout = 15;//15�볬ʱ
        //У���ļ�
        
        if(SYS_FILE_DB_Open( DB_UPDATE, &db, TDB_MODE_RW) != SYS_ERR_OK)
        {
            return 0;
        }  
        if(SYS_ERR_OK != SYS_FILE_DB_ReadFrom(&db, outbuf, 32, 0))
        {
            SYS_FILE_DB_Close(&db); 
            return 0;
        }
        SYS_FILE_DB_Close(&db); 
        
        if(CheckFileHead(outbuf) != SYS_ERR_OK)
        {
            return 0;
        }
        RUN_Para_RW(RUNPARA_FTP, &gs_FtpPara, RUNPARA_FTP_PLEN, true);
        
                                        //��������
        uint32_t faddr = ByteToLong(outbuf + 24);
        uint32_t flen = ByteToInt24(outbuf + 14);
//	        uint16_t framelen;
        uint8_t type = outbuf[28];
        repcfg.en = 0;
        repcfg.pseq = 3;
        
        if(type == 0)
        {
            
            if(true == IAP_Ready(faddr, flen, true))
            {
                memset(outbuf,0,64);
                gs_SysVar.mDGstt |= HLV_STT_USBP;
                framelen = VS_BuildReportFrm(0x05, 0x8353, 0, outbuf);
                REP_ChnSend(outbuf, framelen, PST_CHN_TCP, &repcfg);
                LOG_INFO("IAP ready,now sys will reboot !!!\n");
                aos_msleep(1000);
                aos_reboot();

//	                gs_FtpPara.flag = FTP_STEP_REBOOT;
                g_ulFtp_Timeout = 10;//15�볬ʱ
            }
            else
            {
//	                memset(outbuf,0,64)
//	                    
//	                framelen = VS_BuildReportFrm(0x05, 0x8353, 0x21, outbuf);
//	                REP_ChnSend(outbuf, framelen, PST_CHN_TCP, &repcfg);
//	                gs_FtpPara.flag = FTP_STEP_IDEL;
                goto ERR_FTP;

            }
            
        }
        else if(type == 1)//����
        {   
            gs_FtpPara.flag = FTP_STEP_IDEL;
        }
            
        else if(type == 2)//����
        {   
            
            if(!IAP_Check(flen))
            {
                goto ERR_FTP;                   //���ش���
            }
//	            gs_FtpPara.flag = FTP_STEP_IDEL;
            if(SYS_ERR_OK != Upgrade_Netp_FW(flen))
            {
                goto ERR_FTP;                   //���ش���
            }
            memset(outbuf,0,64);
               
            framelen = VS_BuildReportFrm(0x05, 0x8353, 0, outbuf);
            REP_ChnSend(outbuf, framelen, PST_CHN_TCP, &repcfg);
        
            SYS_Netp_Init();
            
//	            aos_msleep(1000);
//	            aos_reboot();
        }
        else
        {
            goto ERR_FTP; 
        }
        ret = 1;
        
        gs_FtpPara.flag = FTP_STEP_IDEL;
        RUN_Para_RW(RUNPARA_FTP, &gs_FtpPara, RUNPARA_FTP_PLEN, true);
    }
    
    else if(gs_FtpPara.flag == FTP_STEP_FAILED)
    {
        ret = 1;
        goto ERR_FTP; 

    }
    else
    {
        //aos_msleep(1000); 
        ret = 0;
    }
//	    else if(gs_FtpPara.flag == FTP_STEP_REBOOT)
//	    {
//	
//	    }

    if(gs_FtpPara.err_times > 10)
    {
        //ftp ʧ��
//	        gs_OS.Message_Send(&gs_TKFarpQueue, &msgidA[MSG_FTP_CLOSE], 1);
        HAL_FTP_Close();
        gs_FarpVar.wmst &= ~WMS_STT_FTP;

        g_ulFtp_Timeout = 15;//15�볬ʱ
        goto ERR_FTP;
    }
    return ret;
ERR_FTP:
    memset(outbuf,0,64);
    gs_SysVar.mDGstt &= ~(HLV_STT_USBP | HLV_STT_USBE);
    framelen = VS_BuildReportFrm(0x05, 0x8353, 0x21, outbuf);
    REP_ChnSend(outbuf, framelen, PST_CHN_TCP, &repcfg);
    gs_FtpPara.flag = FTP_STEP_IDEL;
    gs_FtpPara.err_times = 0;
    RUN_Para_RW(RUNPARA_FTP, &gs_FtpPara, RUNPARA_FTP_PLEN, true);
    
    return ret;
    
}


/************************************************************************
**��������: void SLV_SecProc(void* pdata)
**�� ��: �봦��
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 09-09-09
*************************************************************************/
void SLV_SecProc(void)
{
    if(Task_Ftp())
    {
        return ;
    }
    Task_SecProc();
    

    ALRT_ActiveReport();
//	    Led_OpSignal();
}


/************************************************************************
**��������: void SLV_MinProc(void* pdata)
**�� ��: �ִ���
**
**�� ��: 
**�� ��: 
**
**�� ע: 
**-----------------------------------------------------------------------
**�� ��: yzy
**�� ��: 09-09-09
*************************************************************************/
void SLV_MinProc(void)
{
    Task_MinProc();
}


//---------------------------------------------------------------------






/************************************************************************
 * @function: SLV_InitProc
 * @����: �ͼ����̵ĳ�ʼ������
 * @����: 
 * @����: 
 * @˵��: 
 * @����: yzy (2014/3/12)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SLV_InitProc(void)
{
    ;
}

int32_t guc_CardLen = 0;

#if (SYS_LOW_POWER > 0)

typedef struct _ST_AIR_SAFE_MNG_
{
    uint32_t delay;
    uint8_t interval;
    uint8_t switch_stt;
    int32_t card_stt;
    uint8_t stt;
}ST_AIR_SAFE_MNG;
ST_AIR_SAFE_MNG gst_asw_mng;


#else

#endif

#define CON_WIRELESS_WORK_DELAY 30 //1����
#define CON_WIRELESS_INIT_DELAY 30 //1����

uint8_t airsafe_mng_init()
{
#if (SYS_LOW_POWER > 0)

    memset((uint8_t *)&gst_asw_mng, 0, sizeof(ST_AIR_SAFE_MNG));
    gst_asw_mng.delay = CON_WIRELESS_INIT_DELAY;

#endif
}
#define CON_AIR_SAFE_STATUS_IEDL        0
#define CON_AIR_SAFE_STATUS_HOOKUP      0x01
#define CON_AIR_SAFE_STATUS_HOOKDOWN    0x02
#define CON_AIR_SAFE_STATUS_WAIT    0x04        //�����ÿ����������60��ͨѶ
#define CON_AIR_SAFE_SLEEP_RUN_1M   0x08

uint8_t get_status(void)
{
    
    uint8_t ret = 0;
#if (SYS_LOW_POWER > 0)
    static uint32_t ulcount = 0;

    ulcount++;
    uint8_t ucswitch_stt = guc_SwitchOnOff;
    int32_t lcard_stt = guc_CardLen;

    if(gst_asw_mng.delay >0)
    {
        gst_asw_mng.delay --;
    }

    if(ucswitch_stt > 0)//�������ߣ�����ͨѶ
    {
        gst_asw_mng.delay = CON_WIRELESS_WORK_DELAY;
        gst_asw_mng.switch_stt = ucswitch_stt;
        ret |= CON_AIR_SAFE_STATUS_HOOKUP;
    }
    else
    {
        if(gst_asw_mng.delay >0)//���ӵ��ߺ�����һ��ʱ��
        {
            ret |= CON_AIR_SAFE_STATUS_WAIT;
        }
        else
        {
//            if((ulcount % CON_WIRELESS_WORK_DELAY) == 0)
//            {
//                ret |= CON_AIR_SAFE_SLEEP_RUN_1M;
//            }
        }
    }

    if(gst_asw_mng.stt != ret)
    {//״̬�����仯
        if(0 == ret)
        {
            extern kbuf_queue_t gs_RFMngQueue;
            krhino_buf_queue_send(&gs_RFMngQueue, &msgidA[MSG_ENTER_SLEEP], 1);
        }
        else
        {
            extern kbuf_queue_t gs_RFMngQueue;
            krhino_buf_queue_send(&gs_RFMngQueue, &msgidA[MSG_WAKE_UP], 1);

        }
        gst_asw_mng.stt = ret;
    }
    
#else
    ret |= CON_AIR_SAFE_STATUS_HOOKUP;
#endif
    return ret;

}
    volatile int g_i=0;
extern uint8_t guc_AllowLogin;
/*******************************************************************************
 * @function_name:  ES_SLV_Task
 * @function_file:  
 * @����: �ͼ����̴�����
 * 
 * 
 * @����: 
 * 
 * @����: 
 * @����: yzy (2011-1-24)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
void SYS_SLV_Task(void)
{
    uint8_t stt;
    uint16_t count = 0;
    //������Ϣ
    krhino_buf_queue_create(&gs_TKSlvQueue, "slv_queue",
                         gc_TKSlvQbuf, MSG_BUFF_LEN, BUFQUEUE_MSG_MAX);
    
    SYS_MSG_Apply(TASK_SLV_TKID, MSG_CLS_TM);

    //gs_OS.Message_Send(&gs_TKSlvQueue, &msgidA[MSG_FARP_CHECK], 1);
    aos_msleep(2000);
    
    airsafe_mng_init();
    
    for(;;)
    {
        
        //LOG_DEBUG("--------------------SLV wait msg-----------\n");
        krhino_buf_queue_recv(&gs_TKSlvQueue, RHINO_WAIT_FOREVER, g_TKSlvQ_buf_recv,
                                 &g_TKSlvQ_recv_size);
        //LOG_DEBUG("SLV recv msg[%d]\n", g_TKSlvQ_buf_recv[0]);
        //��ȡ������Ϣ
        switch(g_TKSlvQ_buf_recv[0])        //������Ϣ�ֱ���
        {
            case MSG_SEC:                   //����Ϣ����
            
//            if(count % 60)
            //SYS_AD_Scan(0, &gn_VBat);

            case MSG_EVENT_CHANGE:
            {
                stt = get_status();
#ifndef MASTER_NODE    
                if(stt > 0)
                {
                    if(guc_SwitchOnOff > 0)
                    {
                        if(!guc_SwitchNorErr)
                        {
                            for(g_i = 0; g_i < 1; g_i++)
                            {
                                int32_t CardLen = HAL_RFID_ReadCardID(nDeviceCardId, guc_CardLen);
                                if(CardLen > 0 /*&& CardLen != guc_CardLen*/)
                                {
                                    guc_CardLen = CardLen;
                                    extern kbuf_queue_t gs_RFMngQueue;
                                    krhino_buf_queue_send(&gs_RFMngQueue, &msgidA[MSG_SWITCH_CHANGE], 1);
                                    break;
                                    
                                }
                                msleep(10);
                            }
                            HAL_RFID_Sleep();
                            g_i = 0x11;
                        }

                        SYS_AD_Scan(0, &gn_VBat);
                        if(gn_VBat >= 3400)
                        {
                            SYS_Dev_OptBlinkSet(SYS_LED_BAT, 3, 0, 0, 0); 
                        }
                        else
                        {
                            SYS_Dev_OptBlinkSet(SYS_LED_BAT, 1, 100, 100, 0); 
                        }                        
                        
                    }
                    if(guc_CardLen > 0 && guc_SwitchOnOff > 0)
                    {
                        if(guc_BuzzerNorErr != 1)
                        {
                            SYS_Dev_OptBlinkSet(GPIO_BUZ_CARD, 2, 0, 0, 100); 
                            guc_BuzzerNorErr = 1;
                        }
                        if(!guc_SwitchNorErr)
                        {
                            extern kbuf_queue_t gs_RFMngQueue;
                            krhino_buf_queue_send(&gs_RFMngQueue, &msgidA[MSG_SWITCH_CHANGE], 1);
                        }                        
                        guc_SwitchNorErr = 1;
                        if(0 == guc_AllowLogin)
                        SYS_Dev_OptBlinkSet(SYS_LED_CARD, 2, 0, 0, 0); 
                        
                    }
                    else
                    {

                        if(guc_SwitchNorErr)
                        {
                            extern kbuf_queue_t gs_RFMngQueue;
                            krhino_buf_queue_send(&gs_RFMngQueue, &msgidA[MSG_SWITCH_CHANGE], 1);
                            SYS_Dev_OptBlinkSet(GPIO_BUZ_CARD, 2, 0, 0, 100); 
                        }                        
                        if(/*guc_CardLen <= 0 && */guc_SwitchOnOff == 0)
                        {
                            if(guc_SwitchNorErr)
                            {
                                extern kbuf_queue_t gs_RFMngQueue;
                                krhino_buf_queue_send(&gs_RFMngQueue, &msgidA[MSG_SWITCH_CHANGE], 1);

                                guc_CardLen = 0;
                            }

                        
                            guc_BuzzerNorErr = 0;
                            guc_SwitchNorErr = 0;
                            if(0 == guc_AllowLogin)
                                SYS_Dev_OptBlinkSet(SYS_LED_CARD, 3, 0, 0, 0); 
                        }
                    }
                }
#endif                  
                break;
            }
            case MSG_MIN: 
//                count ++;
//                SYS_AD_Scan(0, &gn_VBat);
//
//                if(gn_VBat >= 3600)
//                {
//                    SYS_Dev_OptBlinkSet(SYS_LED_BAT, 3, 0, 0, 0); 
//                }
//                else
//                {
//                    SYS_Dev_OptBlinkSet(SYS_LED_BAT, 1, 100, 100, 0); 
//                }
                
                break;
            case MSG_LIVE:
				HB_RetLive(TASK_SLV_TKID);
                break;
        }
    }
}



/******************************************************************************
**�ͼ����̿鳣�����ֶ���
******************************************************************************/
//	const TESTcbConst gs_TkSlv = 
//	{
//	    ES_SLV_Task,
//	    TASK_SLV_TKID >> 3,                    //Ӧ�ù������
//	    TASK_SLV_TKID,                         //���̱��
//	    TASK_SLV_TKID & 7,                     //���ȼ��ڽ��̱��
//	    true,                                      //��Ϣѭ�����ƽ���
//	};

const KTaskConst gs_TkSLV = 
{
    &gs_TKSlvHandle,
    SYS_SLV_Task,                           //Ӧ�ù������
    gs_TKSlvStack,
    TASK_SLV_TKID,                           //���̱��
    true,                                    //��Ϣѭ�����ƽ���
    TASK_SLV_STKL,
    "Slv",
    &gs_TKSlvQueue,
};



