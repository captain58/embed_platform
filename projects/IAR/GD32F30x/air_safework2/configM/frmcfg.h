/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: frmcfg
**��   ��   ��: yzy
**����޸�����: 2013��6��8��
**��        ��: ֡��������ģ�������ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
**������: yzy
**��  ��: v1.0
**�ա���: 2013��6��8��
**�衡��: ԭʼ�汾
**-------------------------------------------------------------------------------------------------------
** �޸���: 
** ��  ��: 
** ��  ��: 
********************************************************************************************************/


#ifndef _FRMCFG_H_
#define _FRMCFG_H_


/******************************************************************************
**PST_Frame�ṹ�干ͬ��ʹ��
******************************************************************************/
#define _DLT_FRM_INC    1               //645��Լ�ṹ��
#define _VS_FRM_INC     1               //
#define _ZY_FRM_INC     1
#define _NETP_FRM_INC     0
#define _HXRF_FRM_INC       1               //����RFͨ��


/******************************************************************************
**ͨѶ��Լ������
******************************************************************************/
#define PST_FRM_VS_EN   1               //���ǹ�Լ
#define PST_FRM_VS_NO   0

#define PST_FRM_BG_EN   0               //645��Լ
#define PST_FRM_BG_NO   (PST_FRM_VS_NO + PST_FRM_VS_EN)

#define PST_FRM_ZY_EN   1               //____��չ
#define PST_FRM_ZY_NO   (PST_FRM_BG_NO + PST_FRM_BG_EN)

#define PST_FRM_MB_EN   1               //____��չ
#define PST_FRM_MB_NO   (PST_FRM_ZY_NO + PST_FRM_ZY_EN)

#define PST_FRM_WL_1_EN  1              //ȫ��΢��������Э��1.0
#define PST_FRM_WL_1_NO  (PST_FRM_MB_EN + PST_FRM_MB_NO)
                                        //�ܵ�֡��������
#define PST_FRM_NO      (PST_FRM_WL_1_EN + PST_FRM_WL_1_NO)


//	typedef enum 
//	{
//	    CID_TXV12   = 0x00,  // ����V1.2
//	    CID_TXV13   = 0x01,  // ����V1.3 
//	    CID_TXVMA   = 0x02,  // ����MODBUS/A
//	    CID_TXVMB   = 0x03,  // ����MODBUS/B
//	    CID_TXLUX   = 0x04,  // ����LUXЭ��
//	    CID_XKVZD   = 0x05,  // �¿��Զ���
//	    CID_XKVM1   = 0x06,  // �¿�MODBUS/1
//	    CID_XKVM2   = 0x07,  // �¿�MODBUS/2
//	    CID_CNV12   = 0x08,  // ����MODBUS V1.2
//	    CID_CNVZD   = 0x09,  // �����Զ���
//	    CID_DWVPM2  = 0x0a,  // ����MODBUS PTZ-BOX2.0
//	    CID_DWVPM3  = 0x0b,  // ����MODBUS PTZ-BOX3.0
//	    CID_ETM1    = 0x0c,  // ����˹��MODBUS(EK-205)
//	    CID_ITRON   = 0x0d,  // ����MODBUS
//	    CID_DTZD    = 0x0e,  // ��ͨ�Զ���
//	    CID_DTM1    = 0x0f,  // ��ͨMODBUS/1
//	    CID_DWVPM3X = 0x10,  // ����MODBUS PTZ-BOX3.0 ��Э�飨long�ͣ�
//	    CID_WXFKQ   = 0X11,  // ���Ƿ��ſ�����
//	    CID_WXIC    = 0x12,  // ����IC��������
//	    CID_WXFMG   = 0x13,  // ����FMG������
//	    CID_WXUMU   = 0x14,  // ���ǳ�����������
//	    CID_CNV20   = 0x15,  // ����MODBUS V2.0
//	    CID_SDAS    = 0x16,  // ˹����ASͨ��Э��(AS-25\32\40\50\80)(ʵΪ��׼Modbus RTUЭ��)
//	    CID_SKFL    = 0x17,  // �¹�����(SICK)FL500ϵ��
//	    CID_SDM2    = 0x18,  // ˹����ASͨ��Э��(AS-100\150\200)
//	    CID_TXCPU   = 0x19,  // ����CPU��
//	    CID_PT100T  = 0x1A,  // PT100/PT100���¶ȱ�����
//	    CID_PT100P  = 0x1B,  // PT100/PT100��ѹ��������
//	}E_DEVICE_PYTE;
// ����V1.2                                                         
#define				   CID_TXV12    0x00   
// ����V1.3                                                         
#define				   CID_TXV13    0x01   
// ����MODBUS/A                                                     
#define				   CID_TXVMA    0x02   
// ����MODBUS/B                                                     
#define				   CID_TXVMB    0x03   
// ����LUXЭ��                                                      
#define				   CID_TXLUX    0x04   
// �¿��Զ���                                                       
#define				   CID_XKVZD    0x05   
// �¿�MODBUS/1                                                     
#define				   CID_XKVM1    0x06   
// �¿�MODBUS/2                                                     
#define				   CID_XKVM2    0x07   
// ����MODBUS V1.2                                                  
#define				   CID_CNV12    0x08   
// �����Զ���                                                       
#define				   CID_CNVZD    0x09   
// ����MODBUS PTZ-BOX2.0                                            
#define				   CID_DWVPM2   0x0a   
// ����MODBUS PTZ-BOX3.0                                            
#define				   CID_DWVPM3   0x0b   
// ����˹��MODBUS(EK-205)                                           
#define				   CID_ETM1     0x0c   
// ����MODBUS                                                       
#define				   CID_ITRON    0x0d   
// ��ͨ�Զ���                                                       
#define				   CID_DTZD     0x0e   
// ��ͨMODBUS/1                                                     
#define				   CID_DTM1     0x0f   
// ����MODBUS PTZ-BOX3.0 ��Э�飨long�ͣ�                           
#define				   CID_DWVPM3X  0x10   
// ���Ƿ��ſ�����                                                   
#define				   CID_WXFKQ    0X11   
// ����IC��������                                                   
#define				   CID_WXIC     0x12   
// ����FMG������                                                    
#define				   CID_WXFMG    0x13   
// ���ǳ�����������                                                 
#define				   CID_WXUMU    0x14   
// ����MODBUS V2.0                                                  
#define				   CID_CNV20    0x15   
// ˹����ASͨ��Э��(AS-25\32\40\50\80)(ʵΪ��׼Modbus RTUЭ��)      
#define				   CID_SDAS     0x16   
// �¹�����(SICK)FL500ϵ��                                          
#define				   CID_SKFL     0x17   
// ˹����ASͨ��Э��(AS-100\150\200)                                 
#define				   CID_SDM2     0x18   
// ����CPU��                                                        
#define				   CID_TXCPU    0x19   
// PT100/PT100���¶ȱ�����                                          
#define				   CID_PT100T   0x1A
// PT100/PT100��ѹ��������    
#define    			   CID_PT100P   0x1B  
//�Ϻ��Ŷ������Ǳ�Modbus
#define    			   CID_XDMA     0x1C  
//�Ϻ�˼��˹�������Ǳ�Modbus
#define    			   CID_SYMA     0x1D  
//����˼�������Ǳ�Modbus
#define    			   CID_SZMA     0x1E  
//�к�ά˼Modbus
#define    			   CID_ZHMA     0x1F 
//ʤ��˹Modbus
#define    			   CID_SSMA     0x20 
//����˹Modbus
#define    			   CID_HDMA     0x21 
//������Modbus
#define    			   CID_RTMA     0x22 

/*******************************************************************************
ͨ����Ŷ���
*******************************************************************************/
//	typedef enum
//	{
//	    PST_CHN_SET = 0,             //����485
//	    PST_CHN_IRD,                 //����
//	    PST_CHN_TCP,                 //GPRS�ͻ�������
//	    PST_CHN_NETP,
//	    PST_CHN_DUMB,                //��ͨ��
//	}CHN_ENUM;



#define PST_LEN_MAX     (uint16)0x200   //����һ֡�Ϊ256�ֽ�
#define PST_CHN_NO      2               //�ܵ�ͨ������



#ifdef EXT_DEV_FRM



/*******************************************************************************
**����ͨ����Ϣ
*******************************************************************************/
#if PST_CHN_NO > 0                      //����ͨ��0����Ϣ
const uint8 gucs_PSTChn0frms[] =        //����ͨ��0֧�ֵĹ�Լ�ı��
{
    PST_FRM_VS_NO,
    PST_FRM_ZY_NO,    
};
#define PST_CHN0_HCHARS  6              //����ͨ��0�����ǰ����������
#endif

#if PST_CHN_NO > 1                      //����ͨ��1����Ϣ             
const uint8 gucs_PSTChn1frms[] =        //����ͨ��1֧�ֵĹ�Լ�ı��   
{                                                                     
    PST_FRM_VS_NO,
    PST_FRM_ZY_NO,    
    PST_FRM_WL_1_NO,
};                                                                    
#define PST_CHN1_HCHARS  6              //����ͨ��1�����ǰ���������� 
#endif

#if PST_CHN_NO > 2                      //����ͨ��2����Ϣ             
const uint8 gucs_PSTChn2frms[] =        //����ͨ��2֧�ֵĹ�Լ�ı��   
{                                                                     
    PST_FRM_VS_NO,
    PST_FRM_ZY_NO,    
};                                                                    
#define PST_CHN2_HCHARS  6              //����ͨ��2�����ǰ���������� 
#endif

#if PST_CHN_NO > 3                      //����ͨ��3����Ϣ             
const uint8 gucs_PSTChn3frms[] =        //����ͨ��3֧�ֵĹ�Լ�ı��   
{                                                                     
    PST_FRM_VS_NO,
    PST_FRM_ZY_NO,    
};                                                                    
#define PST_CHN3_HCHARS  6             //����ͨ��3�����ǰ���������� 
#endif

#endif              //#ifdef EXT_DEV_FRM
#endif

