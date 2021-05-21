/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: dev_vfram.h
**��   ��   ��: yzy
**����޸�����: 2010��2��20��
**��        ��: �豸flash��������
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2010��2��20��
** �衡��: ԭʼ�汾
********************************************************************************************************/
#define EXT_DEV_VFRAM
#include "sys.h"
#include "hal.h"
#include "bsp.h"





#if SYS_FLASH_FRAM_EN > 0
/*
//principal: 
//we use 4k flash to emulate 32byte fram. In which,from bottom to end, 
//0-18 for bitmap, 
//256-512 for crc, 
//512-end for data(1 item use 4096B flash, thus one sector could rewrite 112 = (4096 - 512)/32 times without erase).
//total contain 112 items, 32 bytes one item.
*/

typedef struct
{                           //��bit_n=1,��ʾindex[n]δ������,������Ϊ0xff;
    Word16 indexgrp;        //��bit_n=0,��ʾindex[n]�ѱ�����,��ֵ��Χ{0xfe,0xfc,0xf8,0xf0,0xe0,0xc0,0x80,0x00};
    uint8  index[16];
}flsTemIdx;                             //�����ṹ��


//��λͼ����,1��ʾ����λͼȫ��δ��ʹ��(��0xFF).
//find first set bit. zero is no bit set, thus be assumed as 8
//const uint8 LSBMapTbl[256] =
//{
//    8, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x00 to 0x0F                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x10 to 0x1F                             */
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x20 to 0x2F                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x30 to 0x3F                             */
//    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x40 to 0x4F                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x50 to 0x5F                             */
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x60 to 0x6F                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x70 to 0x7F                             */
//    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x80 to 0x8F                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x90 to 0x9F                             */
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xA0 to 0xAF                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xB0 to 0xBF                             */
//    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xC0 to 0xCF                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xD0 to 0xDF                             */
//    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xE0 to 0xEF                             */
//    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xF0 to 0xFF                             */
//};



/************************************************************************
 * @function: VFRAM_Search
 * @����: ����λͼ�͵�ǰ��������Ŀ������д���λ��
 * 
 * @����: 
 * @param: address ������Ŀ��λ��flash���׵�ַ
 * 
 * @����: 
 * @return: uint8  ��������λ����Ŀ���еĴ洢λ������
 * @˵��:
 * @����: yzy (2011-2-25)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 VFRAM_Search(uint32 address)
{
    uint8 uc_i;
    uint8 tmp;
    uint8 result;
    flsTemIdx temindex;
                                        //��������
    if(FLS_Read((uint8*)&temindex, address, sizeof(temindex)) != SYS_ERR_OK)
    {
        return 0xFF;
    }
    
    if(0xFFFF == temindex.indexgrp.word)//��δ������(����ϵͳ��ʼ�����״̬)
    {
        return 0;
    }
                                        //������׸���Чλ��λͼ���������ֽ�����
    if(temindex.indexgrp.byte[0] == 0)  //index[0]-index[7]�ѱ�����(����һ��ȫ��Ϊ��)
    {
        uc_i = 8 - 1;
        tmp = temindex.indexgrp.byte[1];
    }
    else                                //
    {
        uc_i = (uint8)(0 - 1);
        tmp = temindex.indexgrp.byte[0];
    }
    
    /*
    //Ϊ��ʡ����ռ�,�öδ��������´����滻
    uc_i += LSBMapTbl[tmp];
                                            //������Чλ��λ����
    return (uc_i * 8) + LSBMapTbl[temindex.index[uc_i]];
    //Ϊ��ʡ����ռ�,�öδ��������´����滻
    */
    uc_i += ((tmp == 0) ? 8 : Bit_Maps[tmp]);
                                        //������Чλ��λ����
    result = uc_i << 3;
    result += ((temindex.index[uc_i] == 0) ? 8 : Bit_Maps[temindex.index[uc_i]]);
    
    return result;
    

    
    /*
    //��:��temindex.indexgrp.byte[0]=0,byte[1]=0xFF,��temindex.index[7]=0;
    //      ���Ӧtmp=0xFF,uc_i=7+0=7,��Ӧindex_1st=56+8=64.
    //��:��temindex.indexgrp.byte[0]=0,byte[1]=0xFF,��temindex.index[7]=0xfe;
    //      ���Ӧtmp=0xFF,uc_i=7+0=7,��Ӧindex_1st=56+1=57.
    //��:��temindex.indexgrp.byte[0]=0xfe,byte[1]=0xFF,��temindex.index[1]=0x80;
    //      ���Ӧtmp=0xfe,uc_i=-1+1=0,��Ӧindex_1st=0+7=7.
    */
}



/************************************************************************
 * @function: VFRAM_Reset
 * @����: ������ʼ��flash-fram
 * 
 * @����: 
 * @param: from_addr ��ʼflash��ַ,������������Ϊ SA_FLASH_FRAM_ADDR
 * @param: sectornum Ҫ��������������,������������Ϊ((SA_FLASH_FRAM_ADDREND - SA_FLASH_FRAM_ADDR) / 4096)
 * 
 * @����: 
 * @return: uint8 �ɹ���ʧ��
 * @˵��:
 * @����: yzy (2011-2-25)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 VFRAM_Reset(uint32 from_addr, uint32 sectornum)
{
    return FLS_Erase(from_addr, sectornum);
}


/************************************************************************
 *������Чλ
 *��ʵ�� Bit_Mask8[]��ȡ��
 ************************************************************************/
const uint8 gucs_FramMarkBits[8] = {0xfe,0xfc,0xf8,0xf0,0xe0,0xc0,0x80,0x00};


/************************************************************************
 * @function: VFRAM_WriteMark
 * @����: ��indexָ������Ŀ��Ա����λ����
 * 
 * @����: 
 * @param: index ��ǰ��Ŀ��flash�е�����(0-?)
 * @param: addr ��ǰ��Ŀ����Ӧflash���׵�ַ(4K����)
 * 
 * @����: 
 * @return: bool  
 * @˵��:   ����:��index=0ʱ,д���flsTemIdx.indexgrp=0xfe,flsTemIdx.index[0]=0xfe.
 *          ����:��index=7ʱ,д���flsTemIdx.indexgrp=0xfe,flsTemIdx.index[0]=0.
 *          ����:��index=8ʱ,д���flsTemIdx.indexgrp=0xfc,flsTemIdx.index[0]=0,flsTemIdx.index[1]=0xfe.
 * @����: yzy (2011-2-25)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
bool VFRAM_WriteMark(uint8 index, uint32 addr)
{
    uint8 uc_y, uc_x;
    uc_y = (uint8)(index >> 3);         //�ҳ������ֽ�
    uc_x = (uint8)(index & 0x07);       //�ҳ����������ֽڵľ���λ
    
    if(uc_x == 0)                       //��ʼһ���µ������ֽ�ʱ,��Ӧ����λͼ�ֽ���Ӧλ��λ
    {
        FLS_PreWrite();                 //д����λͼ�ֽ�.
        FLS_WritePage(
                 (uint8*)gucs_FramMarkBits + (uc_y & 7), //��������
                 addr + (uc_y >> 3) ,                    //Ŀ���ַ
                 1);                                     //���ݳ���
    }
                                        //д���Աλͼ�ֽ�
    FLS_PreWrite();
    FLS_WritePage(
        (uint8*)gucs_FramMarkBits + uc_x,               //��������
        addr + 2 + uc_y,                                //Ŀ���ַ
        1);                                             //���ݳ���
    SYS_OK();
}


/************************************************************************
 * @function: SYS_VFRAM_WriteSample
 * @����: ��һ����Ŀ��д������
 * 
 * @����: 
 * @param: data ��д�������
 * @param: num ������Ŀ���
 * @param: finfo ��Ŀ��Ϣ�ṹ��
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2011-2-25)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_VFRAM_WriteSample(uint8* data, uint16 num, const TFlsCycleConst* finfo)
{
    bool es;                                //�Ƿ�ִ�в�������
    uint32 address;
    uint16 crc;
    uint8 index;
    uint8 err;
                                            //������֤
    SYS_VAR_CHECK(num >= finfo->keyno);
    
    err = 0;                                //�������
    crc = DoCrcDlms(data, finfo->itm_baby_byte, 0x5555);//����������ݽ���У��

    //===========================================================
                                            //�ҵ�������ַ
    address = finfo->fls_addr + (num * finfo->itm_sectors * 4096);
    if(address >= finfo->fls_addr_bk)
    {
        return SYS_ERR_VAR;
    }

    index = VFRAM_Search(address);        //����λͼ�ҵ�ǰӦ��д���λ��
    if(index >= finfo->itm_baby_num)
    {
        FLS_Erase(address, finfo->itm_sectors);
        index = 0;
    }
                                            //д����������
    es = false;
    FLS_PreWrite();
    if(FLS_WritePage(data, address + 512 + index * finfo->itm_baby_byte, finfo->itm_baby_byte) == SA_ERR_OK)
    {
        VFRAM_WriteMark(index, address);
                                            //д�뵱ǰ���ݿ��CRCУ��
        FLS_PreWrite();                     //Ԥ����
        if(FLS_WritePage((uint8*)&crc, address + 256 + index * 2, 2) != SA_ERR_OK)
        {
            es = true;
        }
    }
    else
    {
        es = true;
    }
    
    if(es)
    {
        FLS_Erase(address, finfo->itm_sectors);
        index = 0;

        FLS_PreWrite();
        FLS_WritePage(data, address + 512 + index * finfo->itm_baby_byte, finfo->itm_baby_byte);
        VFRAM_WriteMark(index, address);
        FLS_PreWrite();                     //Ԥ����
        FLS_WritePage((uint8*)&crc, address + 256 + index * 2, 2);
        err++;                              //�������+1
    }
    
    //===========================================================
    if(finfo->fls_addr_bk == finfo->fls_addr_end)//�ޱ�����
    {
        if(err)
        {
            return SYS_ERR_FT;              //���ط�����������      
        }
        SYS_OK();
    }
    
    //===========================================================
                                            //�ҵ�������ַ
    address = finfo->fls_addr_bk + (num * finfo->itm_sectors * 4096);
    if(address >= finfo->fls_addr_end)
    {
        return SYS_ERR_VAR;
    }

    index = VFRAM_Search(address);
    if(index >= finfo->itm_baby_num)
    {
        FLS_Erase(address, finfo->itm_sectors);
        index = 0;
    }
                                            //д����������
    es = false;
    FLS_PreWrite(); 
    if(FLS_WritePage(data, address + 512 + index * finfo->itm_baby_byte, finfo->itm_baby_byte) == SA_ERR_OK)
    {
        VFRAM_WriteMark(index, address);
                                            //д�뵱ǰ���ݿ��CRCУ��
        FLS_PreWrite();                     //Ԥ����
        if(FLS_WritePage((uint8*)&crc, address + 256 + index * 2, 2) != SA_ERR_OK)
        {
            es = true;
        }
    }
    else
    {
        es = true;
    }
    
    if(es)
    {
        FLS_Erase(address, finfo->itm_sectors);
        index = 0;

        FLS_PreWrite();
        FLS_WritePage(data, address + 512 + index * finfo->itm_baby_byte, finfo->itm_baby_byte);
        VFRAM_WriteMark(index, address);
        FLS_PreWrite();                     //Ԥ����
        FLS_WritePage((uint8*)&crc, address + 256 + index * 2, 2);
        err++;                              //�������+1
    }
    
    //===========================================================
    if(err >= 2)
    {
        return SYS_ERR_FT;              //���ط�����������      
    }
    SYS_OK();
}


/************************************************************************
 * @function: SYS_VFRAM_ReadSample
 * @����: ��ȡһ����Ŀ����
 * 
 * @����: 
 * @param: data һ����Ŀ,��С����Ϊ������Ŀ���ֽڳ���,����ᵼ�»������
 * @param: num ��Ŀ���
 * @param: finfo ��Ŀ��Ϣ�ṹ��
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2013/11/2)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_VFRAM_ReadSample(uint8* data, uint16 num, const TFlsCycleConst* finfo)
{
    uint32 addr;
    uint16 crc;
    uint8 index;                            //������¼��FLASH�е�����
    
    SYS_VAR_CHECK(num >= finfo->keyno);      //������֤
    
    //===========================================================
                                            //��Ŀ���������׵�ַ
    addr = finfo->fls_addr + (num * finfo->itm_sectors * 4096);
    if(addr >= finfo->fls_addr_bk)
    {
        return SYS_ERR_VAR;
    }
    
    index = VFRAM_Search(addr);             //����λͼ�ҵ�ǰ��¼��λ��
                                            //��ȡ�Ǳ���������
    if((index != 0) && (index <= finfo->itm_baby_num))
    {
        index--;                            //ǰ��λ��Ϊ�Ѽ�¼
        if(FLS_Read(                        //��ȡ����
            data, 
            addr + 512 + index * finfo->itm_baby_byte, 
            finfo->itm_baby_byte) == SA_ERR_OK)
        {
            if(FLS_Read(                    //��ȡCRC
                (uint8*)&crc, 
                addr + 256 + index * 2,
                2) == SA_ERR_OK)
            {                               //�������ݺ�CRC�ıȽ�
                if(crc == DoCrcDlms(data, finfo->itm_baby_byte, 0x5555))
                {
                   SYS_OK();                 //���سɹ�
                }
            }
        }
    }
    
    //===========================================================
    if(finfo->fls_addr_bk == finfo->fls_addr_end)//�ޱ�����
    {
        ClearBuffer(data, finfo->itm_baby_byte);
        return SYS_ERR_FT;                       //����ʧ��
    }
    
    //===========================================================
                                           //��Ŀ�������������׵�ַ
    addr = finfo->fls_addr_bk + (num * finfo->itm_sectors * 4096); 
    if(addr >= finfo->fls_addr_end)
    {
        return SYS_ERR_VAR;
    }
    
    index = VFRAM_Search(addr);            //����λͼ�ҵ�ǰ��¼��λ��
                                           //��ȡ����������
    if((index != 0) && (index < finfo->itm_baby_num))
    {
        index--;
        if(FLS_Read(                       //��ȡ����
            data, 
            addr + 512 + index * finfo->itm_baby_byte, 
            finfo->itm_baby_byte) == SA_ERR_OK)
        {
            if(FLS_Read(                   //��ȡCRC
                (uint8*)&crc, 
                addr + 256 + index * 2,
                2) == SA_ERR_OK)
            {                               //�������ݺ�CRC�ıȽ�
                if(crc == DoCrcDlms(data, finfo->itm_baby_byte, 0x5555))
                {
                   SYS_OK();                 //���سɹ�
                }
            }
        }
    }
    
    //===========================================================
    ClearBuffer(data, finfo->itm_baby_byte);
    
    return SYS_ERR_FT;                       //����ʧ��

}



/************************************************************************
 * @function: SYS_VFRAM_IsConfigLegal
 * @����: �ж��������õĺϷ���
 * 
 * @����: 
 * @param: 
 * 
 * @����: 
 * @return: uint8  0�Ϸ�,�����Ƿ�
 * @˵��: 
 * @����: yzy (2013/11/2)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 SYS_VFRAM_IsConfigLegal(const TFlsCycleConst* finfo)
{
    uint32 tmp;
                                            //�Ƿ���������FLASHѭ���洢������:λͼ����ֻ��16�ֽ�"
    if(finfo->itm_baby_num >= 128)
    {
        return 1;
    }
                                            //�жϵ������ݷ���������Ƿ��㹻
    tmp = finfo->itm_baby_byte * finfo->itm_baby_num;
    if(tmp > ((4096 * finfo->itm_sectors) - 512))
    {
        return 2;
    }
                                            //����sector����
    if((finfo->fls_addr & 0xfff) || (finfo->fls_addr_bk & 0xfff))
    {
        return 3;
    }
                                            //�Ǳ��������ȷǷ�
    tmp = finfo->itm_sectors * 4096 * finfo->keyno;
    if(finfo->fls_addr_bk != (finfo->fls_addr + tmp))
    {
        return 4;
    }
                                            //�б�����,�ұ��������ȷǷ�
    if((finfo->fls_addr_bk != finfo->fls_addr_end) 
       && (finfo->fls_addr_end != (finfo->fls_addr_bk + tmp)))
    {
        return 5;
    }
    
    return 0;
}

#endif                  //#if SYS_FLASH_FRAM_EN > 0




