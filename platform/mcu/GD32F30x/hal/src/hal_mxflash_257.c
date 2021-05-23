/******************************Copyright(c)******************************
**                          
**
** File Name: mxflash.c
** Author: 
** Date Last Update: 2015-05-18
** Description: mxic Flash��������:2IO��׼SPI��,����3�ֽڵ�ַ(<32MB),
**              �Լ�4�ֽڵ�ַ(>=32MB)оƬMX25L257
**              ���ݺ���������������(�����ݶ�ȡ�Ĵ��� memory density)
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
*************************************************************************/
#define EXT_MXFLASH

#include "sys.h"
#include "hal.h"




/************************************************************************
**FlashоƬ����
 ************************************************************************/
#define  FLS_TYPE_MX         0xC2
#define  FLS_TYPE_EON        0x1C




/************************************************************************
**sector ����
 ************************************************************************/
#define  LEN_FLS_SECTOR  4096        //sector ����



/************************************************************************
**����Flash����������
 ************************************************************************/
#define  MX_CMD_WRSR     0x01        //write status register
#define  MX_CMD_WRDI     0x04        //write disable
#define  MX_CMD_RDSR     0x05        //read status register
#define  MX_CMD_WREN     0x06        //write enable


#define  MX_CMD_READ     0x03        //Read data byte
#define  MX_CMD_SE       0x20        //Sector Erase
#define  MX_CMD_BE       0xD8        //Block Erase
#define  MX_CMD_CE       0x60        //Chip Erase
#define  MX_CMD_PP       0x02        //page program


#define  MX_CMD_RDID     0x9F        //Read Identification








/************************************************************************
**����FLASH Buffer
 ************************************************************************/
uint8* gcp_FlsBuffer;



/************************************************************************
**����FLASHоƬ����
 ************************************************************************/
#if FLS_CHIP_NO < 1
#warning "û��FlashоƬ����,����Ҫ������C�ļ�!"
#endif


/************************************************************************
**��CS�����ߵĺ�����ʽ
 ************************************************************************/
void FLS_csupA(void)
{
    SPI_CS_1_FLSA();
}


/************************************************************************
**��CS�����͵ĺ�����ʽ
 ************************************************************************/
void FLS_csdownA(void)
{
    SPI_CS_0_FLSA();
}


#if FLS_CHIP_NO > 1
/************************************************************************
**��CS�����ߵĺ�����ʽ
 ************************************************************************/
void FLS_csupB(void)
{
    SPI_CS_1_FLSB();
}


/************************************************************************
**��CS�����͵ĺ�����ʽ
 ************************************************************************/
void FLS_csdownB(void)
{
    SPI_CS_0_FLSB();
}
#endif



const uint32 guls_FLSChipSize[FLS_CHIP_NO] = 
{
    FLASH1_SIZE,
#if FLS_CHIP_NO > 1
    FLASH1_SIZE,
#endif
};



/************************************************************************
**������ͨFlash ����SPI������Ľṹ��
 ************************************************************************/
SPIIO gs_FlashSPI[FLS_CHIP_NO];

/************************************************************************
**��FlashоƬ�ж�ȡ��Flash״̬
 ************************************************************************/
uint8 guc_FlashStatus[FLS_CHIP_NO];




/************************************************************************
**Flash������ȫ״̬��
 ************************************************************************/
uint32 gul_FlashSafeCode;

/************************************************************************
**����Flash����״̬��
**bit0 flash�Ƿ����
**bit1 �Ƿ��з��������ݴ���
**bit2 �Ƿ��з���д���ݴ���
 ************************************************************************/
uint8 guc_FlashErrStt;


/************************************************************************
**�����ȡFlash״̬�Ľṹ��
 ************************************************************************/
const SPIIO gs_FlashGetStt[] = 
{
    {
        {MX_CMD_RDSR,},
        guc_FlashStatus,
        1,
        1,
        FLS_csupA,
        FLS_csdownA,   
    },
#if FLS_CHIP_NO > 1
    {
        {MX_CMD_RDSR,},
        guc_FlashStatus + 1,
        1,
        1,
        FLS_csupB,
        FLS_csdownB, 
    },
#endif
};

/************************************************************************
**����д��Flash״̬�ֵĽṹ��
 ************************************************************************/
const SPIIO gs_FlsWriteStatus[] = 
{
    {
        {MX_CMD_WRSR,},
        guc_FlashStatus,
        1,
        1,
        FLS_csupA,
        FLS_csdownA,   
    },
#if FLS_CHIP_NO > 1
    {
        {MX_CMD_WRSR,},
        guc_FlashStatus + 1,
        1,
        1,
        FLS_csupB,
        FLS_csdownB, 
    },
#endif
};

/************************************************************************
**FLashдʹ��
 ************************************************************************/
const SPIIO gs_FlsEnableWrite[] = 
{
    {
        {MX_CMD_WREN,},
        __NULL,
        0,
        1,
        FLS_csupA,
        FLS_csdownA
    },
#if FLS_CHIP_NO > 1

    {
        {MX_CMD_WREN,},
        __NULL,
        0,
        1,
        FLS_csupB,
        FLS_csdownB,
    },
#endif
};

/************************************************************************
**FLashд����
 ************************************************************************/
const SPIIO gs_FlsDisableWrite[] = 
{
    {
        {MX_CMD_WRDI,},
        __NULL,
        0,
        1,
        FLS_csupA,
        FLS_csdownA
    },
#if FLS_CHIP_NO > 1

    {
        {MX_CMD_WRDI,},
        __NULL,
        0,
        1,
        FLS_csupB,
        FLS_csdownB,
    },
#endif
};


/************************************************************************
**�����д���������Ľṹ��
 ************************************************************************/
const SPIIO gs_FlashDisableWP[] = 
{
    {
        {MX_CMD_WRSR, 0x42, },
        __NULL,
        0,
        2,
        FLS_csupA,
        FLS_csdownA,
    },
#if FLS_CHIP_NO > 1
    {
        {MX_CMD_WRSR, 0x42, },
        __NULL,
        0,
        2,
        FLS_csupB,
        FLS_csdownB,
    },
#endif
};

/************************************************************************
**�����ֹд���������Ľṹ��
 ************************************************************************/
const SPIIO gs_FlashEnableWP[] = 
{
    {
        {MX_CMD_WRSR, 0x7C, },
        __NULL,
        0,
        2,
        FLS_csupA,
        FLS_csdownA,
    },
#if FLS_CHIP_NO > 1
    {
        {MX_CMD_WRSR, 0x7C, },
        __NULL,
        0,
        2,
        FLS_csupB,
        FLS_csdownB,
    }
#endif
};





/************************************************************************
**��ȡFLASH״̬
 ************************************************************************/
#define FLS_GetStatus(X) {SPI_Read((SPIIO*)&gs_FlashGetStt[X]);}

#define FLS_WriteStatus(x) {SPI_Write((SPIIO*)&gs_FlsWriteStatus[x]);}

/************************************************************************
**дʹ��
 ************************************************************************/
#define FLS_EnableWrite(x) {SPI_Write((SPIIO*)&gs_FlsEnableWrite[x]);}

/************************************************************************
**д����
 ************************************************************************/
#define FLS_DisableWrite(x) {SPI_Write((SPIIO*)&gs_FlsDisableWrite[x]);}






/************************************************************************
**�رձ����ܽ�
 ************************************************************************/
void FLS_KillHwWp(void)
{
        SPI_WP_1_FLSA();
    #if FLS_CHIP_NO > 1
        SPI_WP_1_FLSB();
    #endif
}


/************************************************************************
**����Flashд����
 ************************************************************************/
void FLS_Disable_WP(uint32 X)
{
    FLS_EnableWrite(X);
    SPI_Write((SPIIO*)&gs_FlashDisableWP[X]);
}

/************************************************************************
**д����
 ************************************************************************/
void FLS_Enable_WP(uint32 X)
{
    FLS_EnableWrite(X);
    SPI_Write((SPIIO*)&gs_FlashEnableWP[X]);
}

/************************************************************************
**Flash�����Լ��˳��͹���ģʽ
 ************************************************************************/
extern uint8 _SPI_SendByte(uint8 byte);
#if FLS_CHIP_NO > 1
#define FLS_EnterDP() {FLS_csdownA(); FLS_csdownB(); _SPI_SendByte(0xB9);}
#define FLS_ExitDP()  {_SPI_SendByte(0xAB); FLS_csupA(); FLS_csupB();}
#else
#define FLS_EnterDP() {FLS_csdownA(); _SPI_SendByte(0xB9);}
#define FLS_ExitDP()  {_SPI_SendByte(0xAB); FLS_csupA();}
#endif


/***********************************************************
 * @function_name: FLS_FREE
 * @function_file: atdflash.c
 * @����:�жϴ洢����ǰ�Ƿ����
 * 
 * 
 * @����: 
 * @param:x  �洢�����
 * @param:imd  �Ƿ���Ҫ�����ж�
 * 
 * @����: 
 * @return: bool true ���� false ������
 * @����:
 *---------------------------------------------------------
 * @�޸���: yzy (2009-8-8)
 **********************************************************/
bool FLS_FREE(uint32 x, bool imd)
{
    FLS_GetStatus(x);                   //����Flash��״̬

    if(!(guc_FlashStatus[x] & 0x01))    //�ж�Flash��״̬
    {
        FLS_GetStatus(x);               //����Flash��״̬

        if(!(guc_FlashStatus[x] & 0x01))//�ٴ��ж�,��ֹ�жϴ���
        {
            return true;                //���о����Ϸ���
        }
    }
    if(imd)                             //�ж��Ƿ���Ҫ���ٷ��ص�
    {
                                        //�ӳ�100��ѭ��
        for(uint16 dly = 100; dly > 0; dly--);
    }
    else
    {
        msleep(10);                 //�ӳ�10ms
    }
    return false;                       //���ز�����
}

/*******************************************************************************
** @function_name:  EraseFlsSectorInChip
** @function_file:  mxflash.c
** @����: MX Flash ���߳�����
** 
** 
** @����: 
** @param: address(uint32): 
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 EraseFlsSectorInChip(uint32 address, uint8 x)
{
    uint8 err;
                                            //��ַ��������
    SYS_VAR_CHECK(address >= guls_FLSChipSize[x]);
    
    address &= ~0xFFF;                      //����ʼ��ַ

    FLS_EnableWrite(x);                     //дʹ��

    if(guls_FLSChipSize[x] < 0x2000000)     //<32M
    {
        gs_FlashSPI[x].command[0] = MX_CMD_SE;  //���߳�ָ��
                                                //���ַ
        gs_FlashSPI[x].command[1] = address >> 16;
        gs_FlashSPI[x].command[2] = address >> 8;
        gs_FlashSPI[x].command[3] = address;

        gs_FlashSPI[x].data = __NULL;
        gs_FlashSPI[x].cmdnum = 4;
        gs_FlashSPI[x].length = 0;
    }
    else                                    //>=32M
    {
        gs_FlashSPI[x].command[0] = MX_CMD_SE;  //���߳�ָ��
                                                //���ַ
        gs_FlashSPI[x].command[1] = address >> 24;
        gs_FlashSPI[x].command[2] = address >> 16;
        gs_FlashSPI[x].command[3] = address >> 8;
        gs_FlashSPI[x].command[4] = address;

        gs_FlashSPI[x].data = __NULL;
        gs_FlashSPI[x].cmdnum = 5;
        gs_FlashSPI[x].length = 0;
    }
                                            //�����߳�����
    err = SPI_Write(&gs_FlashSPI[x]);

    while(!FLS_FREE(x, true));              //�ȴ����Ŀ���

    //FLS_DisableWrite(x);                    //д����

    return err;
}



/*******************************************************************************
** @function_name:  EraseFlsBlockInChip
** @function_file:  mxflash.c
** @����: MX Flash 64K(block)���߳�����
** 
** 
** @����: 
** @param: address(uint32): 
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 EraseFlsBlockInChip(uint32 address, uint8 x)
{
    uint8 err;
                                            //��ַ��������
    SYS_VAR_CHECK(address >= guls_FLSChipSize[x]);
    
    address &= ~0xFFF;                      //����ʼ��ַ

    FLS_EnableWrite(x);                     //дʹ��

    if(guls_FLSChipSize[x] < 0x2000000)     //<32M
    {
        gs_FlashSPI[x].command[0] = MX_CMD_BE;  //���߳�ָ��
                                                //���ַ
        gs_FlashSPI[x].command[1] = address >> 16;
        gs_FlashSPI[x].command[2] = address >> 8;
        gs_FlashSPI[x].command[3] = address;

        gs_FlashSPI[x].data = __NULL;
        gs_FlashSPI[x].cmdnum = 4;
        gs_FlashSPI[x].length = 0;
    }
    else                                    //>=32M
    {
        gs_FlashSPI[x].command[0] = MX_CMD_BE;  //���߳�ָ��
                                                //���ַ
        gs_FlashSPI[x].command[1] = address >> 24;
        gs_FlashSPI[x].command[2] = address >> 16;
        gs_FlashSPI[x].command[3] = address >> 8;
        gs_FlashSPI[x].command[4] = address;

        gs_FlashSPI[x].data = __NULL;
        gs_FlashSPI[x].cmdnum = 5;
        gs_FlashSPI[x].length = 0;
    }
                                            //�����߳�����
    err = SPI_Write(&gs_FlashSPI[x]);

    while(!FLS_FREE(x, true));              //�ȴ����Ŀ���

    //FLS_DisableWrite(x);                    //д����

    return err;
}



/*******************************************************************************
** @function_name:  EraseFlsInChip
** @function_file:  mxflash.c
** @����: MX Flash ȫƬ��������
** 
** 
** @����: 
** @param: 
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 EraseFlsInChip(uint8 x)
{
    uint8 err;

    FLS_EnableWrite(x);                     //дʹ��

    gs_FlashSPI[x].command[0] = MX_CMD_CE;  //���߳�ָ��

    gs_FlashSPI[x].data = __NULL;
    gs_FlashSPI[x].cmdnum = 1;
    gs_FlashSPI[x].length = 0;
   
                                            //�����߳�����
    err = SPI_Write(&gs_FlashSPI[x]);

    while(!FLS_FREE(x, true));              //�ȴ����Ŀ���

    //FLS_DisableWrite(x);                    //д����

    return err;
}

/*******************************************************************************
** @function_name:  ReadFlsInChip
** @function_file:  mxflash.c
** @����: ��Flashָ����ַ��ȡָ�����ȵ��ֽ�����
** 
** 
** @����: 
** @param: buffer(uint8*): ���ݻ���
** @param: address(uint32): ��ַ
** @param: length(uint16): ��ȡ�����ݳ���
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 ReadFlsInChip(uint8* buffer, uint32 address, uint16 length, uint8 x)
{
                                        //��ȡ��λ�ò���,���ܿ�Ƭ��ȡ
    SYS_VAR_CHECK(length + address > guls_FLSChipSize[x]);
                                        //2xIO ��ȡ����ָ��
    if(guls_FLSChipSize[x] < 0x2000000)     //<32M
    {
        gs_FlashSPI[x].command[0] = MX_CMD_READ;
                                            //��ȡ��ַ
        gs_FlashSPI[x].command[1] = address >> 16;
        gs_FlashSPI[x].command[2] = address >> 8;
        gs_FlashSPI[x].command[3] = address;

        gs_FlashSPI[x].data = buffer;       //���ݻ���
        gs_FlashSPI[x].length = length;     //��ȡ����
        gs_FlashSPI[x].cmdnum = 4;          //�����
    }
    else                                    //>=32M
    {
        gs_FlashSPI[x].command[0] = MX_CMD_READ;
                                            //��ȡ��ַ
        gs_FlashSPI[x].command[1] = address >> 24;
        gs_FlashSPI[x].command[2] = address >> 16;
        gs_FlashSPI[x].command[3] = address >> 8;
        gs_FlashSPI[x].command[4] = address;

        gs_FlashSPI[x].data = buffer;       //���ݻ���
        gs_FlashSPI[x].length = length;     //��ȡ����
        gs_FlashSPI[x].cmdnum = 5;          //�����
    }

    return SPI_Read(&gs_FlashSPI[x]);
}



/*******************************************************************************
** @function_name:  CompareFlsInChip
** @function_file:  mxflash.c
** @����: ��Flashָ����ַ��ȡָ�����ȵ��ֽ�����,���봫�����ݱȽ��Ƿ����
** 
** 
** @����: 
** @param: buffer(uint8*): ���ݻ���
** @param: address(uint32): ��ַ
** @param: length(uint16): ��ȡ�����ݳ���
** 
** @����: 
** @return:  uint8   SYS_ERR_OK���,SYS_ERR_FT�����
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 CompareFlsInChip(uint8* buffer, uint32 address, uint16 length, uint8 x)
{
                                            //��ȡ��λ�ò���,���ܿ�Ƭ��ȡ
    SYS_VAR_CHECK(length + address > guls_FLSChipSize[x]);
    
    if(guls_FLSChipSize[x] < 0x2000000)     //<32M
    {
                                            //2xIO ��ȡ����ָ��
        gs_FlashSPI[x].command[0] = MX_CMD_READ;
                                            //��ȡ��ַ
        gs_FlashSPI[x].command[1] = address >> 16;
        gs_FlashSPI[x].command[2] = address >> 8;
        gs_FlashSPI[x].command[3] = address;

        gs_FlashSPI[x].data = buffer;       //���ݻ���
        gs_FlashSPI[x].length = length;     //��ȡ����
        gs_FlashSPI[x].cmdnum = 4;          //�����
    }
    else                                    //>=32M
    {
        gs_FlashSPI[x].command[0] = MX_CMD_READ;
                                            //��ȡ��ַ
        gs_FlashSPI[x].command[1] = address >> 24;
        gs_FlashSPI[x].command[2] = address >> 16;
        gs_FlashSPI[x].command[3] = address >> 8;
        gs_FlashSPI[x].command[4] = address;

        gs_FlashSPI[x].data = buffer;       //���ݻ���
        gs_FlashSPI[x].length = length;     //��ȡ����
        gs_FlashSPI[x].cmdnum = 5;          //�����
    }

    return SPI_ReadAndCompare(&gs_FlashSPI[x]);
}


/*******************************************************************************
** @function_name:  WriteFlsPageInChip
** @function_file:  mxflash.c
** @����: Flashҳ���
** 
** 
** @����: 
** @param: buffer(uint8*): ���ݻ���
** @param: address(uint32): Ƭ�ڵ�ַ
** @param: length(uint16): д�����ݵĳ���
** @param: x(uint32): 
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 WriteFlsPageInChip(uint8* buffer, uint32 address, uint16 length, uint8 x)
{
                                            //�жϲ�������ȷ��
    SYS_VAR_CHECK(length + (address & 0xFF) > 256);

    if(length == 0)                         //���д������ݳ���Ϊ0 
    {
        SYS_OK();                            //ֱ�ӷ���OK
    }
    
    FLS_EnableWrite(x);
    
    if(guls_FLSChipSize[x] < 0x2000000)     //<32M
    {
                                                //page program
        gs_FlashSPI[x].command[0] = MX_CMD_PP;     
                                                //��ַ���ֽ�
        gs_FlashSPI[x].command[1] = address >> 16; 
                                                //��ַ���ֽ�
        gs_FlashSPI[x].command[2] = address >> 8;
        gs_FlashSPI[x].command[3] = address;          //��ַ���ֽ�
                                         
        gs_FlashSPI[x].data = buffer;           //��������
        gs_FlashSPI[x].cmdnum = 4;              //�����
        gs_FlashSPI[x].length = length;         //���ݳ���
        
        SPI_Write(&gs_FlashSPI[x]);             //ִ������
        while(!FLS_FREE(x, true));              //�ȴ����Ŀ���
        
                                                //�����Ƚ�����
        gs_FlashSPI[x].command[0] = MX_CMD_READ;
        gs_FlashSPI[x].cmdnum = 4;              //4���ֽڵ�����
    }
    else
    {
        gs_FlashSPI[x].command[0] = MX_CMD_PP;     
                                                //��ַ
        gs_FlashSPI[x].command[1] = address >> 24;
        gs_FlashSPI[x].command[2] = address >> 16;
        gs_FlashSPI[x].command[3] = address >> 8;
        gs_FlashSPI[x].command[4] = address;
                                         
        gs_FlashSPI[x].data = buffer;           //��������
        gs_FlashSPI[x].cmdnum = 5;              //�����
        gs_FlashSPI[x].length = length;         //���ݳ���
        
        SPI_Write(&gs_FlashSPI[x]);             //ִ������
        while(!FLS_FREE(x, true));              //�ȴ����Ŀ���
        
                                                //�����Ƚ�����
        gs_FlashSPI[x].command[0] = MX_CMD_READ;
        gs_FlashSPI[x].cmdnum = 5;              //5���ֽڵ�����
    }
                                            //д������
    return SPI_ReadAndCompare(&gs_FlashSPI[x]);
}





//**********************************************************************************
//***********************�򵥺���**********************
//**********************************************************************************






/*******************************************************************************
** @function_name:  MultiFlashFilterExt
** @function_file:  mxflash.c
** @����: �����оƬ�ļ�ϵͳҪ������оƬ����
** 
** 
** @����: 
** @param: s_out: �������
** @param: address(uint32): ������ַ
** @param: length(uint16): �������ݵĳ���
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint32 MultiFlashFilterExt(S_FLSINCHIP* s_out, uint32 address, uint32 length)
{
    uint32 index = 0;                   //�漰���Ĵ洢оƬ���
    uint32 s1 = 0;                      //ĳƬFLASH��Ӧ����ʼ�߼���ַ
    uint32 s2 = 0;                      //ĳƬFLASH��Ӧ����ֹ�߼���ַ(����)
    uint32 back = address;
    uint8 sidx = 0;
    S_FLSINCHIP* fs;                    //
    
    _IF_TRUE_RETURN_Z(length == 0, 0);  //�����Ϸ����ж�
                                        //����Ҫ�������׸�оƬ
    s1 = 0;
    s2 = 0;
    for(index = 0; index < FLS_CHIP_NO; index++)
    {
        s2 += guls_FLSChipSize[index];
        if(address < s2)
        {
            break;
        }
        s1 += guls_FLSChipSize[index];
    }
                                        //δ�ҵ�,�˳�
    if(index == FLS_CHIP_NO)
    {
        return 0;
    }
                                        //���ÿ��оƬ�Ĳ�������
    for(; index < FLS_CHIP_NO; index++)
    {
        if(length == 0)
        {
            break;
        }
                                        //
        fs = &s_out[sidx];
        fs->x = index;
        fs->opoffset = address - back;
        fs->opaddr = address - s1;
        
        if((guls_FLSChipSize[index] - fs->opaddr) < length)
        {
            fs->oplen = length - (guls_FLSChipSize[index] - fs->opaddr);
        }
        else
        {
            fs->oplen = length;
        }
                                        //���±���(1)
        address = s2;
        length -= fs->oplen;
        sidx++;
                                        //׼���¸�оƬ(2)
        s1 += guls_FLSChipSize[index];
        s2 += guls_FLSChipSize[index];
    }
    
    return sidx;
    
}





/*******************************************************************************
** @function_name:  FLS_AllErase
** @function_file:  mxflash.c
** @����: MX Flash ȫƬ�߳�����
** 
** 
** @����: 
** @param: 
** 
** @����: 
** @return:  void   
** @����: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
void FLS_AllErase(void)
{
    EraseFlsInChip(0);
#if FLS_CHIP_NO > 0
    EraseFlsInChip(1);
#endif
}



/*******************************************************************************
** @function_name:  FLS_SectorErase
** @function_file:  mxflash.c
** @����: MX Flash ��4K�߳�����
** 
** 
** @����: 
** @param: address(uint32): 
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 FLS_SectorErase(uint32 address)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    uint8 num;
                                                //���Ҵ洢оƬ��Ƭ�ڵ�ַ
    num = MultiFlashFilterExt(fs, address, 1);
    if(num == 0)
    {
        return SA_ERR_FT;
    }
    
    return EraseFlsSectorInChip(fs[0].opaddr, fs[0].x);
}



/*******************************************************************************
** @function_name:  FLS_BlockErase
** @function_file:  mxflash.c
** @����: MX Flash ��64K�߳�����
** 
** 
** @����: 
** @param: address(uint32): 
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 FLS_BlockErase(uint32 address)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    uint8 num;
                                                //���Ҵ洢оƬ��Ƭ�ڵ�ַ
    num = MultiFlashFilterExt(fs, address, 1);
    if(num == 0)
    {
        return SA_ERR_FT;
    }
    
    return EraseFlsBlockInChip(fs[0].opaddr, fs[0].x);
}

/*******************************************************************************
** @function_name:  FLS_SectorBlankCheck
** @function_file:  mxflash.c
** @����: MX Flash �οհ��ж�
** 
** 
** @����: 
** @param: address(uint32)
** 
** @����: 
** @return:  bool true:ȫ��,false:�ǿ�
** @����: yzy (2012-06-09)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
bool FLS_SectorBlankCheck(uint32 address)
{
    address &= ~0xFFF;                      //����ʼ��ַ
    
    FLS_Read(gcp_FlsBuffer, address, 4096);
    for(uint32 uc_i = 0; uc_i < 4096; uc_i++)
    {
        if(gcp_FlsBuffer[uc_i] != 0xFF)
        {
            return false;
        }
    }

    return true;
}



/*******************************************************************************
** @function_name:  FLS_PageBlankCheck
** @function_file:  mxflash.c
** @����: MX Flash ҳ�հ��ж�
** 
** 
** @����: 
** @param: address(uint32)
** 
** @����: 
** @return:  bool true:ȫ��,false:�ǿ�
** @����: yzy (2012-06-09)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
bool FLS_PageBlankCheck(uint32 address)
{
    address &= ~0xFF;                      //ҳ��ʼ��ַ
    
    FLS_Read(gcp_FlsBuffer, address, 256);
    for(uint32 uc_i = 0; uc_i < 256; uc_i++)
    {
        if(gcp_FlsBuffer[uc_i] != 0xFF)
        {
            return false;
        }
    }

    return true;
}



/*******************************************************************************
** @function_name:  FLS_WritePage
** @function_file:  mxflash.c
** @����: Flashҳ���
** 
** 
** @����: 
** @param: buffer(uint8*): 
** @param: address(uint32): 
** @param: length(uint16): 
** @param: x(uint32): 
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 FLS_WritePage(uint8* buffer, uint32 address, uint16 length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    uint8 num;
                                                //���Ҵ洢оƬ��Ƭ�ڵ�ַ
    num = MultiFlashFilterExt(fs, address, 1);
    if(num == 0)
    {
        return SA_ERR_FT;
    }
    
    return WriteFlsPageInChip(buffer, fs[0].opaddr, length, fs[0].x);
}






//**********************************************************************************
//***********************���Ӻ���**********************
//**********************************************************************************











/*******************************************************************************
** @function_name:  FLS_Erase
** @function_file:  mxflash.c
** @����: MX Flash �߳�����
** 
** 
** @����: 
** @param: address: ��ʼ��ַ
** @param: sectornum ��������
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 FLS_Erase(uint32 address, uint32 sectornum)
{
    uint32 ul_i = 0;
    
    if(address & 0xFFF)
    {
        return FLS_ERR_VAR;
    }
                                        //ѭ������
    while(ul_i < sectornum)
    {
        //                              //ι��:����ִ�иú���ʱ,ι�����̵ȴ���Դʱ�����,���¸�λ
        HAL_WDT_Feed();
        //
        
        FLS_SectorErase(address);
        ul_i++;
        address += 0x1000;
    }

    return FLS_ERR_OK;
}









/*******************************************************************************
** @function_name:  FLS_Read
** @function_file:  mxflash.c
** @����: ��Flashָ����ַ��ȡָ�����ȵ��ֽ�����
** 
** 
** @����: 
** @param: buffer(uint8*): ���ݻ���
** @param: address(uint32): ��ַ
** @param: length(uint16): ��ȡ�����ݳ���
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 FLS_Read(uint8* buffer, uint32 address, uint16 length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    uint8 err = 0;
    uint8 num;

    num = MultiFlashFilterExt(fs, address, length);
    if(num == 0)
    {
        return SA_ERR_FT;
    }
    
    for(uint8 uc_i = 0; uc_i < num; uc_i++)
    {
        err |= ReadFlsInChip(buffer + fs[uc_i].opoffset, fs[uc_i].opaddr, fs[uc_i].oplen, fs[uc_i].x);
    }
    
    return err;
}






/************************************************************************
 * @function: FLS_PreWrite
 * @����: Ԥд����,��д����֮ǰ����Ҫ����Ԥд����
 * @����: 
 * 
 * @����: 
 * @return: uint8  
 * @˵��: 
 * @����: yzy (2014/6/18)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
uint8 FLS_PreWrite(void)
{
    gul_FlashSafeCode = 0xA5A5A5A5;
    SYS_OK();
}




/*******************************************************************************
** @function_name:  WriteFlsInChip
** @function_file:  mxflash.c
** @����: Flash д����
** 
** 
** @����: 
** @param: buffer(uint8*): ��д�����ݵĴ�Ż���
** @param: address(uint32): flashƬ�ڵ�ַ
** @param: length(uint16): ���ݳ���
** @param: x(uint8): �ڼ�Ƭflash
** @param: sdmode(uint8): �������ݱ���ģʽ.[0.������������(Ĭ��),1.��������λ�ڴ�д����֮ǰ������,2.ɾ����������]
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy(20111-01-10)
** @�޸�˵��: ���ĺ���ʵ�ַ�ʽ,�򻯳���
*******************************************************************************/
uint8 WriteFlsInChip(uint8* buffer, uint32 addr, uint16 length, uint8 x, uint8 sdmode)
{
    uint8  sidx, eidx;                      //д�����ʼҳ�ͽ���ҳ
    uint8  fwri;                            //�Ƿ���Ҫд���־
    uint8  fera;                            //�Ƿ���Ҫ������־
    uint8  m;
    uint8  n;
    //uint32 addrInFlash;                     //FLASH�ľ��Ե�ַ
    uint32 addrInSector;                    //������Ե�ַ
    uint32 addrOfSector;                    //
    uint16 ui_len = 0;                      //����SECTOR��д��ĳ���
                                            //��ȡ��λ�ò���,�������Ƭ��ȡ
    SYS_VAR_CHECK(length + addr > guls_FLSChipSize[x]);
    SYS_VAR_CHECK(length == 0);
    SYS_VAR_CHECK(sdmode != 0);

    /************************************************************************
    //ѭ������ÿ����Ҫ���ĵ�SECTOR
    ************************************************************************/
    while(true)                             //ѭ������ÿ����Ҫ���ĵ�SECTOR
    {
        if(length + (addr & 0xFFF) > 4096)  //���
        {
            ui_len = 4096 - (addr & 0xFFF); //���������ݳ���
        }
        else                                //���ڲ���
        {
            ui_len = length;                //�������ݳ���
        }

                                            //��ȡһ������
        while(SYS_ERR_OK != ReadFlsInChip(gcp_FlsBuffer, addr & (~0xFFF), 4096, x));
        
        /************************************************************************
        //ѭ���ж��Ƿ����д,��дǰ�Ƿ������
        ************************************************************************/
        fwri = 0;                           //��λ��־λ
        fera = 0;
                                            //ѭ���ж��Ƿ����д�����Ƿ���Ҫ����
        for(uint32 k = 0; k < ui_len; k++)
        {
            uint8 uc_data = gcp_FlsBuffer[(addr & 0xFFF) + k];
            if(uc_data != buffer[k])        //�ж��Ƿ���ԭ������һ��
            {
                fwri = 0x01;
                if((uc_data & buffer[k]) != buffer[k])//(uc_data != 0xFF)         //����ԭ�����ݲ�һ���ֲ���FF����Ҫ����
                {
                    fera = 0x01;
                    break;
                }
            }
        }
        
        /************************************************************************
        //д��
        ************************************************************************/
        if(fwri)                            //��Ҫд��
        {
            addrOfSector = addr & (~0xFFF);
            addrInSector = addr & 0xFFF;

            if(fera)                        //��Ҫ����
            {
                EraseFlsSectorInChip(addrOfSector, x);
                sidx = 0;                   //������Ҫ����д��
                eidx = 16;
            }
            else                            //��Ҫд���ҳ
            {
                sidx = addrInSector >> 8;
                eidx = (addrInSector + ui_len) >> 8;
                if((addrInSector + ui_len) & 0xFF)
                {
                    eidx += 1;
                }
            }
                                            //ת����Ҫд�������
            MoveBuffer(buffer, gcp_FlsBuffer + addrInSector, ui_len);
                                            // 
            for(m = 0; m < 3; m++)          //ѭ��д,���д3��
            {
                for(n = sidx; n < eidx; n++)//��ҳд��
                {
                                            
                    if(WriteFlsPageInChip(gcp_FlsBuffer + (n << 8), addrOfSector + (n << 8), 256, x) != FLS_ERR_OK)
                    {
                        EraseFlsSectorInChip(addrOfSector, x);
                        sidx = 0;
                        eidx = 16;
                        break;
                    }
                }
                
                if(n >= eidx)               //д��ɹ�,����
                {
                    break;
                }
            }
            
            if(m >= 3)
            {
                guc_FlashErrStt |= FLS_ERR_WRITE;//��־д�����ݷ�������
                return FLS_ERR_FT;          //����д�����ݴ���
            }
        }

        /************************************************************************
        //��
        ************************************************************************/
        length -= ui_len;                   //���������ݳ��ȵݼ�
        if(length == 0)                     //�����Ѿ��������
        {
            SYS_OK();                        //����д��ɹ�
        }

        buffer += ui_len;                   //��������ָ��ǰ��
        addr += ui_len;                     //ʵ�ʲ�����ַ
    }
}




/*******************************************************************************
** @function_name:  FLS_Write
** @function_file:  mxflash.c
** @����: Flash д����
** 
** 
** @����: 
** @param: buffer(uint8*): д�����ݵ����ݻ���
** @param: address(uint32): д�����ݵĵ�ַ
** @param: length(uint16): д�����ݵĳ���
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy
** @�޸�˵��: �����ܵؼ����߳��Ĵ���(��:��д��������ԭ������һ�»�ԭ������ΪFFʱ���ٲ���)
*******************************************************************************/
uint8 FLS_Write(uint8* buffer, uint32 address, uint16 length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    uint8 err = 0;
    uint8 num;
    
    if(gul_FlashSafeCode != 0xA5A5A5A5)     //�жϰ�ȫ�������Ƿ��
    {
        return FLS_ERR_FT;                  //������������
    }
    
    num = MultiFlashFilterExt(fs, address, length);
    if(num == 0)
    {
        gul_FlashSafeCode = 0;
        return SA_ERR_FT;
    }
    
    for(uint8 uc_i = 0; uc_i < num; uc_i++)
    {
        err |= WriteFlsInChip(buffer + fs[uc_i].opoffset, fs[uc_i].opaddr, fs[uc_i].oplen, fs[uc_i].x, 0);
    }
    
    gul_FlashSafeCode = 0;
    return err;
}




/*******************************************************************************
** @function_name:  FLS_WriteSpec
** @function_file:  mxflash.c
** @����: Flash ����д����(���������뱣��ԭ�����ݵ�д����,�ر������ڴ�����������д��)
** 
** 
** @����: 
** @param: buffer(uint8*): д�����ݵ����ݻ���
** @param: address(uint32): д�����ݵĵ�ַ
** @param: length(uint16): д�����ݵĳ���
** 
** @����: 
** @return:  uint8   
** @����: yzy (2014-03-27)
**-----------------------------------------------------------------------------
** @�޸���: yzy
** @�޸�˵��: 
*******************************************************************************/
uint8 FLS_WriteSpec(uint8* buffer, uint32 address, uint16 length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    uint8 err = 0;
    uint8 num;
    
    if(gul_FlashSafeCode != 0xA5A5A5A5)     //�жϰ�ȫ�������Ƿ��
    {
        return FLS_ERR_FT;                  //������������
    }
    
    num = MultiFlashFilterExt(fs, address, length);
    if(num == 0)
    {
        gul_FlashSafeCode = 0;
        return SA_ERR_FT;
    }
    
    for(uint8 uc_i = 0; uc_i < num; uc_i++)
    {                                       //˵��:��ȷ������д��ʱΪ��������ʱ,������ģʽ2.����ģʽ1������.
        err |= WriteFlsInChip(buffer + fs[uc_i].opoffset, fs[uc_i].opaddr, fs[uc_i].oplen, fs[uc_i].x, 1);
    }
    
    gul_FlashSafeCode = 0;
    return err;
}

/*******************************************************************************
** @function_name:  SetFlsInChip
** @function_file:  mxflash.c
** @����: ��Flashĳ��������Ϊ�ض�ֵ 
** 
** 
** @����: 
** @param: templet(uint8): ���������
** @param: address(uint32): д�����ݵĵ�ַ
** @param: length(uint16): д�����ݵĳ���
** @param: x(uint8): flashоƬ���
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy
** @�޸�˵��: ��������ͬ WriteFlsInChip()
*******************************************************************************/
uint8 SetFlsInChip(uint8 templet, uint32 addr, uint16 length, uint8 x)
{
    uint8  sidx, eidx;                      //д�����ʼҳ�ͽ���ҳ
    uint8  fwri;                            //�Ƿ���Ҫд���־
    uint8  fera;                            //�Ƿ���Ҫ������־
    uint8  m;
    uint8  n;
    //uint32 addrInFlash;                     //FLASH�ľ��Ե�ַ
    uint32 addrInSector;                    //������Ե�ַ
    uint32 addrOfSector;                    //
    uint16 ui_len = 0;                      //����SECTOR��д��ĳ���
                                            //��ȡ��λ�ò���,�������Ƭ��ȡ
    SYS_VAR_CHECK(length + addr > guls_FLSChipSize[x]);
    SYS_VAR_CHECK(length == 0);

    /************************************************************************
    //ѭ������ÿ����Ҫ���ĵ�SECTOR
    ************************************************************************/
    while(true)                             //ѭ������ÿ����Ҫ���ĵ�SECTOR
    {
        if(length + (addr & 0xFFF) > 4096)  //���
        {
            ui_len = 4096 - (addr & 0xFFF); //���������ݳ���
        }
        else                                //���ڲ���
        {
            ui_len = length;                //�������ݳ���
        }
        
                                            //��ȡһ������
        while(SYS_ERR_OK != ReadFlsInChip(gcp_FlsBuffer, addr & (~0xFFF), 4096, x));
        
        /************************************************************************
        //ѭ���ж��Ƿ����д,��дǰ�Ƿ������
        ************************************************************************/
        fwri = 0;                           //��λ��־λ
        fera = 0;
                                            //ѭ���ж��Ƿ����д�����Ƿ���Ҫ����
        for(uint32 k = 0; k < ui_len; k++)
        {
            uint8 uc_data = gcp_FlsBuffer[(addr & 0xFFF) + k];
            if(uc_data != templet)          //�ж��Ƿ���ԭ������һ��
            {
                fwri = 0x01;
                if((uc_data & templet) != templet)//(uc_data != 0xFF)         //����ԭ�����ݲ�һ���ֲ���FF����Ҫ����
                {
                    fera = 0x01;
                    break;
                }
            }
        }
     
        /************************************************************************
        //д��
        ************************************************************************/
        if(fwri)                            //��Ҫд��
        {
            addrOfSector = addr & (~0xFFF);
            addrInSector = addr & 0xFFF;

            if(fera)                        //��Ҫ����
            {
                EraseFlsSectorInChip(addrOfSector, x);
                sidx = 0;                   //������Ҫ����д��
                eidx = 16;
            }
            else                            //��Ҫд���ҳ
            {
                sidx = addrInSector >> 8;
                eidx = (addrInSector + ui_len) >> 8;
                if((addrInSector + ui_len) & 0xFF)
                {
                    eidx += 1;
                }
            }
                                                //��λ��Ҫд�������
            memset(gcp_FlsBuffer + (addr & 0xFFF), templet, ui_len);
            
            for(m = 0; m < 3; m++)          //ѭ��д,���д3��
            {
                for(n = sidx; n < eidx; n++)//��ҳд��
                {
                    if(WriteFlsPageInChip(gcp_FlsBuffer + (n << 8), addrOfSector + (n << 8), 256, x) != FLS_ERR_OK)
                    {
                        EraseFlsSectorInChip(addrOfSector, x);
                        sidx = 0;
                        eidx = 16;
                        break;
                    }
                }
                
                if(n >= eidx)               //д��ɹ�,����
                {
                    break;
                }
            }
            
            if(m >= 3)
            {
                guc_FlashErrStt |= FLS_ERR_WRITE;//��־д�����ݷ�������
                return FLS_ERR_FT;          //����д�����ݴ���
            }
        }

        /************************************************************************
        //��
        ************************************************************************/
        length -= ui_len;                   //���������ݳ��ȵݼ�
        if(length == 0)                     //�����Ѿ��������
        {
            SYS_OK();                        //����д��ɹ�
        }
        
        addr += ui_len;                     //ʵ�ʲ�����ַ
    }
}



/*******************************************************************************
** @function_name:  FLS_Set
** @function_file:  mxflash.c
** @����: Flash ������־
** 
** 
** @����: 
** @param: templet(uint8): ���������
** @param: address(uint32): д�����ݵĵ�ַ
** @param: length(uint16): д�����ݵĳ���
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy
** @�޸�˵��: ��������ͬFLS_Write()
*******************************************************************************/
uint8 FLS_Set(uint8 templet, uint32 address, uint16 length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    uint8 err = 0;
    uint8 num;
    
    if(gul_FlashSafeCode != 0xA5A5A5A5)     //�жϰ�ȫ�������Ƿ��
    {
        return FLS_ERR_FT;                  //������������
    }
    
    num = MultiFlashFilterExt(fs, address, length);
    if(num == 0)
    {
        gul_FlashSafeCode = 0;
        return SA_ERR_FT;
    }
    
    for(uint8 uc_i = 0; uc_i < num; uc_i++)
    {
        err |= SetFlsInChip(templet, fs[uc_i].opaddr, fs[uc_i].oplen, fs[uc_i].x);
    }
    
    gul_FlashSafeCode = 0;
    return err;
}







#define TEST_FLS_BYTES  4
/*******************************************************************************
** @function_name:  TestFlashChip
** @function_file:  
** @����: flashӲ���Լ�
** 
** 
** @����: 
** 
** @����: 
** @return:  uint8 λͼ.1:Ӳ������,0:Ӳ������.
**                 bit0-bit7�ֱ��ʾ��1-8��flash��Ӳ��״̬
** @����: yzy (2010-05-12)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 TestFlashChip(void)
{
    uint8 cmp1[TEST_FLS_BYTES];             //ԭʼ����
    uint8 cmp2[TEST_FLS_BYTES];             //�޸ĺ�����д������
    uint8 cmp3[TEST_FLS_BYTES];             //�޸ĺ�ʵ��д������
    uint32 chip_addr;                        //Ƭ�ڵ�ַ
    uint8 err = 0;
    
    for(uint8 k = 0; k < FLS_CHIP_NO; k++)
    {
        for(uint8 w = 0; w < 2; w++)        //��������:һ��Ƭͷ,һ��Ƭβ(ȷ��������ȷ)
        {
            chip_addr = w ? (guls_FLSChipSize[k] - TEST_FLS_BYTES) : 0;
                                            //��ȡԭʼ����
            if(ReadFlsInChip(cmp1, chip_addr, TEST_FLS_BYTES, k) != SYS_ERR_OK)
            {
                err |= Bit_Map8[k];
                break;
            }
                                            //�޸�ԭʼ����
            for(uint8 i = 0; i < TEST_FLS_BYTES; i++)
            {
                cmp2[i] = cmp1[i] + 2;
            }
                                            //д������д������
            if(WriteFlsInChip(cmp2, chip_addr, TEST_FLS_BYTES, k, 0) != SYS_ERR_OK)
            {
                err |= Bit_Map8[k];
                break;
            }
                                            //��ȡʵ��д������
            if(ReadFlsInChip(cmp3, chip_addr, TEST_FLS_BYTES, k) != SYS_ERR_OK)
            {
                err |= Bit_Map8[k];
                break;
            }
                                            //�ָ�ԭʼֵ
            WriteFlsInChip(cmp1, chip_addr, TEST_FLS_BYTES, k, 0);
                                            //�ȶ�����ʵ��ֵ
            if(memcmp(cmp2, cmp3, TEST_FLS_BYTES) != 0)
            {
                err |= Bit_Map8[k];
                break;
            }
        }
    }

    return err;
}







/*******************************************************************************
** @function_name:  Fls_ReadID
** @function_file:  mxflash.c
** @����: Flash��ȡID
** 
** 
** @����: 
** @param: buffer(uint8*): 
** @param: x(uint32): 
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-12)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 Fls_ReadID(uint8* buffer, uint32 x)
{
    gs_FlashSPI[x].command[0] = MX_CMD_RDID;  //��ȡFlash ID����
    gs_FlashSPI[x].data = buffer;
    gs_FlashSPI[x].cmdnum = 1;
    gs_FlashSPI[x].length = 3;
                                            //��ȡ����
    return SPI_Read(&gs_FlashSPI[x]);
}

/*******************************************************************************
** @function_name:  HAL_InitFlash
** @function_file:  mxflash.c
** @����: ��ʼ��Flash
** 
** @����: 
** 
** @����: 
** @return:  uint8   
** @����: yzy (2010-05-12)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
uint8 HAL_InitFlash(void)
{
    gcp_FlsBuffer = (uint8*)SYS_GetMEM(LEN_FLS_SECTOR);
    guc_FlashErrStt = 0;                    //��ʼ������״̬��
    
    Init_SPI(&gs_FlashSpiPort);
    
    FLS_KillHwWp();

    for(uint32 x = 0; x < FLS_CHIP_NO; x++)
    {
                                            //����cs�����߲����ĺ���
        gs_FlashSPI[x].csup = gs_FlashGetStt[x].csup;
                                            //����cs���õͲ����ĺ���
        gs_FlashSPI[x].csdown = gs_FlashGetStt[x].csdown;
        
        FLS_ExitDP();

        uint8 buffer[3];
        Fls_ReadID(buffer, x);              //��ȡFlash ID
                                            //�ж��ͺ�
        if((buffer[0] != FLS_TYPE_MX) && (buffer[0] != FLS_TYPE_EON))    
        {
            Fls_ReadID(buffer, x);          //�ض�һ��
                                            //�ж�״̬
            if((buffer[0] != FLS_TYPE_MX) && (buffer[0] != FLS_TYPE_EON))
            {                               //��������,Flash������
                guc_FlashErrStt = FLS_ERR_EXSIT;
            }
        }
        
        FLS_Disable_WP(x);
        while(!FLS_FREE(x, true));          //�ȴ�Flash����
    }
    
    if(guc_FlashErrStt)
    {
        return FLS_ERR_FT;                  //���ط�����������
    }
    else
    {
        SYS_OK();                            //���س�ʼ���ɹ�
    }
}
