/****************************************Copyright (c)**************************************************
**                                  ____�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: pst_bg.c
**��   ��   ��: yzy
**����޸�����: 2017��9��1��
**��        ��: ͨѶģ��645��Լ�����ļ���������ͨѶ�ͱ���ͨѶ
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2017��9��1��
** �衡��: ԭʼ�汾
********************************************************************************************************/


#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"



/******************************************************************
**ȫC��ַ
******************************************************************/
const uint8 gucs_VSALLAddr[][6] =
{
    {0x99, 0x99, 0x99, 0x99, 0x99, 0x99}, 
    {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA},
    {0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB},
    {0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC},
    {0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD},
    {0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
};


/*********************************************************************************************************
**��������  uint8 VS_Decode(uint8* receive, DLT_Frame* frm, uint8* addr)
**����  receive ���յ����ݵĻ���        
        code    ���ݽ����ɵĽṹ��
        addr    ��Ҫ�ظ��ı��ַ
**���  PST_ERR_OK
**      PST_ERR_FT
**˵��  �������յ��Ĳ���֡�Ľ������
** ȫ�ֱ���:
** ����ģ��:
**
**------------------------------------------------------------------------------------------------------
** ��  ��: yzy
** �ա���: 2017��9��1��
** ��  ��: 
********************************************************************************************************/
uint8 VS_Decode(uint8* receive, DLT_Frame* frm, uint8* addr)
{
    if(addr != __NULL)                  //��Ҫ�жϵ�ַ
    {
                                        //�жϽ��յ��ĵ�ַ�Ƿ��Ҫ���һ��
        if(CmpBuffer(addr, receive + 1, 6) != 0)
        {
            return PST_ERR_FT;          //���ط�������
        }
    }
    frm->cmd = receive[8];              //������
    frm->len = receive[9];              //����
    frm->recv = receive + 10;           //���յ������ݵ�������
                                        //ѭ����33
    for(uint32 uc_i = 0; uc_i < frm->len; uc_i ++)
    {
        receive[uc_i + 10] -= 0x33;     //��33����
    }

    frm->wd.lword = 0;                  //���е����ݱ�ʶ��0����

    if(frm->len > 2)                    //������ȴ��ھ������ݱ�ʶ
    {
        frm->wd.byte[0] = receive[10];  //���ֽ�
        frm->wd.byte[1] = receive[11];  //���ֽ�        
        if(frm->len > 4)                //������ȴ���4�Ϳ�������չ���ݱ�ʶ
        {
                                        //���ֽ�
            frm->wd.byte[2] = receive[12];  
                                        //���ֽ�
            frm->wd.byte[3] = receive[13];  
        }
    }
    SYS_OK();
}

/*********************************************************************************************************
**��������  uint8 VS_Pack(PST_Frame* frm, uint8* addr, uint16* length)
**����  frm ����Ľṹ��
        addr ����ĵ�ַ            
        length ����Ϊ����������󳤶ȣ����Ϊ�����֮��ĳ���
**���  PST_ERR_OK
        PST_ERR_VAR
**      PST_ERR_FT
**˵��  ��645֡���д��������ͨѶ
** ȫ�ֱ���:
** ����ģ��:
**
**------------------------------------------------------------------------------------------------------
** ��  ��: yzy
** �ա���: 2017��9��1��
** ��  ��: 
********************************************************************************************************/
uint8 VS_Pack(DLT_Frame* frm, uint8* addr, uint16* length)
{
   frm->send[8] = frm->cmd;             //������
   frm->send[9] = frm->len;             //����

   if(frm->wd.word[0] > 0)              //����������ݱ�־
   {
       frm->send[10] = frm->wd.byte[0]; //�ֽ�0
       frm->send[11] = frm->wd.byte[1]; //�ֽ�1
       if(frm->wd.word[1] < 0xFFFF)     //������չ��־
       {
           if(frm->atr & PST_FRM_ATRWR )//�ж��Ƿ���д����
           {
                                        //д���ݵĻ���չ���ݱ�־��д������
               frm->send[16] = frm->wd.byte[2];
               frm->send[17] = frm->wd.byte[3];
           }
           else
           {                            //���������д�����ݱ�ʶ֮��
               frm->send[12] = frm->wd.byte[2];
               frm->send[13] = frm->wd.byte[3];
           }
       }
   }
                                        //+33����
   for(uint8 uc_i = 0; uc_i < frm->send[9]; uc_i ++)
   {
       frm->send[uc_i + 10] += 0x33;
   }
                                        //�������
   *length = PST_DoneFrame(PST_FRM_VS_NO, frm->send);

   if(*length > 0)                      //�������>0
   {
       if(addr != __NULL)               //��Ҫ�����ַ
       {
           MoveBuffer(                  //ת�Ƶ�ַ�е�����
               addr, 
               frm->send + 1, 
               6
               );

           frm->send[*length - 2] = DoSum(frm->send, *length - 2);
       }
       
       SYS_OK();
   }
   return PST_ERR_FT;                   //���ط�����������
}   


/*********************************************************************************************************
**��������  uint8 XVS_Pack(PST_Frame* frm, uint8* addr, uint16* length)
**����  frm ����Ľṹ��
        addr ����ĵ�ַ            
        length ����Ϊ����������󳤶�,���Ϊ�����֮��ĳ���
**���  PST_ERR_OK
        PST_ERR_VAR
**      PST_ERR_FT
**˵��  ��645-2007֡���д��,����ͨѶ
** ȫ�ֱ���:
** ����ģ��:
**
**------------------------------------------------------------------------------------------------------
** ��  ��: yzy
** �ա���: 2018��3��22��
** ��  ��: ���645-2007�Ĵ��
********************************************************************************************************/
uint8 XVS_Pack(DLT_Frame* frm, uint8* addr, uint16* length)
{
   frm->send[8] = frm->cmd;             //������
   frm->send[9] = frm->len;             //����

   if(frm->wd.lword > 0)                //����������ݱ�־
   {
       frm->send[10] = frm->wd.byte[0]; //�ֽ�0
       frm->send[11] = frm->wd.byte[1]; //�ֽ�1
       frm->send[12] = frm->wd.byte[2]; //�ֽ�2
       frm->send[13] = frm->wd.byte[3]; //�ֽ�3
   }
                                        //+33����
   for(uint8 uc_i = 0; uc_i < frm->send[9]; uc_i ++)
   {
       frm->send[uc_i + 10] += 0x33;
   }
                                        //�������
   *length = PST_DoneFrame(PST_FRM_VS_NO, frm->send);

   if(*length > 0)                      //�������>0
   {
       if(addr != __NULL)               //��Ҫ�����ַ
       {
           MoveBuffer(                  //ת�Ƶ�ַ�е�����
               addr, 
               frm->send + 1, 
               6
               );

           frm->send[*length - 2] = DoSum(frm->send, *length - 2);
       }
       
       SYS_OK();
   }
   return PST_ERR_FT;                   //���ط�����������
}   






/*********************************************************************************************************
**��������  uint8 PST_VS_Decode(uint8* receive, uint8* send, uint16 sbuflen, PST_Frame* code)
**����  receive ���յ����ݵĻ���
        send    ��Ҫ���͵����ݵĻ���
        code    ���ݽ����ɵĽṹ��
**���  PST_ERR_OK
**      PST_ERR_FT
**˵��  �жϵ�ǰ���������Ƿ�Ϊ�Ϸ�֡������ǵĻ��������Ϊ����ͨѶ�ṹ��
** ȫ�ֱ���:
** ����ģ��:
**
**------------------------------------------------------------------------------------------------------
** ��  ��: yzy
** �ա���: 2017��9��1��
** ��  ��: 
********************************************************************************************************/
uint8 PST_VS_Decode(uint8* receive, uint8* send, uint16 sbuflen, PST_Frame* frm)
{
    frm->_dlt.recv = receive;                //���յ�������
    frm->_dlt.send = send;                   //�������ݻ������ʼ��ַ
    frm->_dlt.cmd = receive[8];              //������
    frm->_dlt.len = receive[9] + 12;         //����
    frm->_dlt.wd.byte[0] = send[10] - 0x33;  //���ݱ�ʶ�ֽ�0
    frm->_dlt.wd.byte[1] = send[11] - 0x33;  //���ݱ�ʶ�ֽ�1

    SYS_OK();                            //������ͨ��
}


/*********************************************************************************************************
**��������  uint8 PST_VS_Proc(PST_Frame* frm)
**����  
        code    ���ݽ����ɵĽṹ��
**���  PST_ERR_OK
**      PST_ERR_FT
**˵��  ������͸��ת�����ڲ�
** ȫ�ֱ���:
** ����ģ��:
**
**------------------------------------------------------------------------------------------------------
** ��  ��: yzy
** �ա���: 2017��9��1��
** ��  ��: 
********************************************************************************************************/
uint8 PST_VS_Proc(PST_Frame* frm)
{
    uint16 len;
    frm->_dlt.cmd = 0xC1;
    frm->_dlt.len = 0x01;
    frm->_dlt.wd.lword = 0;
    frm->_dlt.send[10] = 0x02;
    VS_Pack(&frm->_dlt, gs_PstPara.Addr, &len);
                                        //��������
    gfs_PSTChnSend[frm->chninfo & PST_FRM_CHNNO]
    (
        frm->_dlt.send,
        len
    );

    SYS_OK();
}



/*******************************************************************************
 * @function_name:  WaitForHdlc
 * @function_file:  
 * @����: �ȴ�HDLC֡�ĵ���������
 * 
 * 
 * @����: buffer ����
 * @����: size �����С
 * @����: fHdlcProc ������. 
 * 
 * @����: ��Ч֡����
 * @����: yzy (2018��3��22��)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
/*uint16 WaitForHdlc(uint8* buffer, uint16 size, fHdlcProc func, uint8 port)
{
    uint16 len;
    uint16 hdlclen;
    uint8 module;
    
    len = 0;
    for(uint8 uc_i = 0; uc_i < 60; uc_i++)
    {
        len += gs_Uart.Read(port, buffer + len, size - len);
        if(len > 0)                     //�յ�����
        {
            hdlclen = fHdlcParseBuf(buffer, len, &module);
            if(hdlclen)                 //��Чhdlc��ʽ����
            {
                if(func)                //���⴦��
                {
                    func(buffer, hdlclen, module);
                }
                else                    //Ĭ�ϴ���
                {
                    fHdlcDataProc(buffer, hdlclen, module);
                }
                return hdlclen;
            }
        }
        gs_OS.TK_Sleep(5);
    }
    return 0;
}*/



/*******************************************************************************
 * @function_name:  WaitForBg
 * @function_file:  
 * @����: �ȴ�VS֡�ĵ���������
 * 
 * 
 * @����: buffer ����
 * @����: size �����С
 * @����: fBgProc ������. 
 * 
 * @����: ��Ч֡����
 * @����: yzy (2018��3��22��)
 *-----------------------------------------------------------------------------
 * @�޸���: 
 * @�޸�˵��: 
 ******************************************************************************/
uint16 WaitForVs(uint8* buffer, uint16 size, fBgProc func, uint8 port)
{
    uint16 len;
    uint8  bglen;
    
    len = 0;
    for(uint8 uc_i = 0; uc_i < 60; uc_i++)
    {
        len += SER_RecvData(port, buffer + len, size - len,100);
        if(len > 0)                     //�յ�����
        {
            bglen = PST_CheckFrame2(PST_FRM_VS_NO, buffer, len);
            if(bglen)                   //��Чbg��ʽ����
            {
                if(func)                //���⴦��
                {
                    func(buffer, bglen);
                }
                return bglen;
            }
        }
        aos_msleep(50);
    }
    return 0;
}



