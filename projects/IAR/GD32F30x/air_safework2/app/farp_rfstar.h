#ifndef _FARP_RF_STAR_
#define _FARP_RF_STAR_

#ifndef EXT_FARP
#define EXT_FARP extern
#endif

typedef struct 
{
    uint8_t wkmd;                             //����ģʽ(0�͹���     1�ǵ͹���)
    
    uint8_t login;                          //��¼֡�Ƿ��ͳɹ�

    uint8_t paired;

    uint8 rdct;                             //�ز�����
    uint8 rtct;                             //���Դ���
    uint8 mcsq;                             //�ź�����
    uint16 hbcy;                            //��������(s)
    //uint16 rdtm;                            //�ز����
    uint16 rttm;                            //���Լ��
    uint16 slep;                            //����������ʱ��
    uint32 dcip;                            //��վ��ַ
    uint16 dcpt;                            //��վ�˿ں�

    uint8_t modemip[4];                         //�ն˵�IP��ַ
    uint32 wmsv;                            //Զ������è����汾��
    uint32 wmst;                            //����è״̬��

    uint8 ldlen;                            //��¼֡����
    uint8 ld[64];                           //��¼֡
    uint8 lodlen;                           //�˳���½֡����
    uint8 lod[64];                          //�˳���½֡
    uint8 hklen;                            //����֡����
    uint8 hkd[64];                          //����֡
}FarpVar;

EXT_FARP FarpVar gs_FarpVar;                //Զ�̶˿ڵĲ���


#endif
