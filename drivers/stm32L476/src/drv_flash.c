/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: drv_flash.c
**��   ��   ��: yzy
**����޸�����: 2015��3��28��
**��        ��: mxic Flash��������:2IO��׼SPI��,����3�ֽڵ�ַ(<32MB),�Լ�4�ֽڵ�ַ(>=32MB)оƬMX25L256
**              ���ݺ���������������(�����ݶ�ȡ�Ĵ��� memory density) 
**              
**ע        ��:     141:   1��1Ƭflash��4��4 Mbytes��1��1Kbytes����
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2018��6��15��
** �衡��: ԭʼ�汾
********************************************************************************************************/
//#include "bsp_common.h"
#include "hal.h"
#include "drv_flash.h"
#include "drv_spiio.h"
#include <string.h>
//#include <stdint.h>
#include <stdio.h>

//#define FLS_CHIP_NO     1
//#define FLASH1_SIZE     0x100000        //4MM

/******************************************************************************
**FlashоƬ����
******************************************************************************/
#define  FLS_TYPE_MX         0xC2
#define  FLS_TYPE_EON        0x1C

/******************************************************************************
**sector ����
******************************************************************************/
#define  LEN_FLS_SECTOR  4096        //sector ����
#ifdef MCURS
#define  BIT_FLS_BUFSFT  2           //flash������������λ,��Χ0-4,�ֱ��Ӧ�ڴ�����4096,2048,1024,512,256.//���ٲ������ڵ�ҳ����256�ֽ�
#else
#define  BIT_FLS_BUFSFT  0 
#endif

#define  LEN_FLS_BUF  (LEN_FLS_SECTOR >> BIT_FLS_BUFSFT) //�����buflen

#define  NUM_FLS_SECTBUF (1 << BIT_FLS_BUFSFT)           //����sector��Ӧ��buflen����
#define  MASK_FLS_BUFLEN (LEN_FLS_BUF - 1)               //buflen������.����,2048����Ϊ0x7FF



/******************************************************************************
**����Flash����������
******************************************************************************/
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


//(>=32M)
#define     MX_CMD_RDSCUR   0x15//mx25L25645GM2I ���� 0x2B        //Read ��ȫ�Ĵ���   
#define     MX_CMD_4BYTEADDR_BIT 0x20

#define     MX_CMD_EN4B     0xB7        //Enable 4-Byte address mode(���ڴ���16M)
#define     MX_CMD_EX4B     0xE9        //Exit 4-Byte mode


/******************************************************************************
**Flash,����SPI�ӿ�
******************************************************************************/
#define SPI_DEV_ID_FLSA         0
#define SPI_DEV_ID_FLSB         1
#define SPI_DEV_ID_FRAM         2

//	#if (SYS_FILE_EN > 0)
#define _IF_TRUE_RETURN_Z(X,Z)  {if(X){return (Z);}}


/******************************************************************************
**����FLASH Buffer
******************************************************************************/
//	#pragma location = ".mmm"
unsigned char gcp_FlsBuffer[LEN_FLS_BUF];



/*****************************************************************************
**����FLASHоƬ����
*****************************************************************************/
#if FLS_CHIP_NO < 1
#warning "û��FlashоƬ����,����Ҫ������C�ļ�!"
#endif

//	const uint8_t Bit_Map8[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

/*******************************************************************************
**��CS�����ߵĺ�����ʽ
********************************************************************************/
void FLS_csupA(void)
{
    SPI_CS_1(SPI_DEV_ID_FLSA);
}


/*******************************************************************************
**��CS�����͵ĺ�����ʽ
********************************************************************************/
void FLS_csdownA(void)
{
    SPI_CS_0(SPI_DEV_ID_FLSA);
}


#if FLS_CHIP_NO > 1
/*******************************************************************************
**��CS�����ߵĺ�����ʽ
********************************************************************************/
void FLS_csupB(void)
{
    SPI_CS_1(SPI_DEV_ID_FLSB);
}
/*******************************************************************************
**��CS�����͵ĺ�����ʽ
********************************************************************************/
void FLS_csdownB(void)
{
    SPI_CS_0(SPI_DEV_ID_FLSB);
}
#endif



const unsigned long guls_FLSChipSize[FLS_CHIP_NO] = 
{
    FLASH1_SIZE,
#if FLS_CHIP_NO > 1
    FLASH1_SIZE,
#endif
};



/*******************************************************************************
**������ͨFlash ����SPI������Ľṹ��
********************************************************************************/
SPIIO gs_FlashSPI[FLS_CHIP_NO];

/*******************************************************************************
**��FlashоƬ�ж�ȡ��Flash״̬
********************************************************************************/
unsigned char guc_FlashStatus[FLS_CHIP_NO];


/*******************************************************************************
**��FlashоƬ�ж���ȫ�Ĵ�����ֵ�������ж������ֽڻ������ֽ�״̬
********************************************************************************/
unsigned char guc_FlashSecu[FLS_CHIP_NO];


/*******************************************************************************
**Flash������ȫ״̬��
********************************************************************************/
unsigned long gul_FlashSafeCode;

/*******************************************************************************
**����Flash����״̬��
**bit0 flash�Ƿ����
**bit1 �Ƿ��з��������ݴ���
**bit2 �Ƿ��з���д���ݴ���
********************************************************************************/
unsigned char guc_FlashErrStt;


/*******************************************************************************
**�����ȡFlash״̬�Ľṹ��
********************************************************************************/
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

/*******************************************************************************
**����д��Flash״̬�ֵĽṹ��
********************************************************************************/
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

/******************************************************************************
**FLashдʹ��
******************************************************************************/
const SPIIO gs_FlsEnableWrite[] = 
{
    {
        {MX_CMD_WREN,},
        NULL,
        0,
        1,
        FLS_csupA,
        FLS_csdownA
    },
#if FLS_CHIP_NO > 1

    {
        {MX_CMD_WREN,},
        NULL,
        0,
        1,
        FLS_csupB,
        FLS_csdownB,
    },
#endif
};

/******************************************************************************
**FLashд����
******************************************************************************/
const SPIIO gs_FlsDisableWrite[] = 
{
    {
        {MX_CMD_WRDI,},
        NULL,
        0,
        1,
        FLS_csupA,
        FLS_csdownA
    },
#if FLS_CHIP_NO > 1

    {
        {MX_CMD_WRDI,},
        NULL,
        0,
        1,
        FLS_csupB,
        FLS_csdownB,
    },
#endif
};


/*******************************************************************************
**�����д���������Ľṹ��
********************************************************************************/
const SPIIO gs_FlashDisableWP[] = 
{
    {
        {MX_CMD_WRSR, 0x42, },
        NULL,
        0,
        2,
        FLS_csupA,
        FLS_csdownA,
    },
#if FLS_CHIP_NO > 1
    {
        {MX_CMD_WRSR, 0x42, },
        NULL,
        0,
        2,
        FLS_csupB,
        FLS_csdownB,
    },
#endif
};

/*******************************************************************************
**�����ֹд���������Ľṹ��
********************************************************************************/
const SPIIO gs_FlashEnableWP[] = 
{
    {
        {MX_CMD_WRSR, 0x7C, },
        NULL,
        0,
        2,
        FLS_csupA,
        FLS_csdownA,
    },
#if FLS_CHIP_NO > 1
    {
        {MX_CMD_WRSR, 0x7C, },
        NULL,
        0,
        2,
        FLS_csupB,
        FLS_csdownB,
    }
#endif
};




/*******************************************************************************
**�������4�ֽ�ָ��(>=32M)
********************************************************************************/
const SPIIO gs_FlashDisable4B[] = 
{
    {
        {MX_CMD_EX4B,},
        NULL,
        0,
        1,
        FLS_csupA,
        FLS_csdownA,
    },
#if FLS_CHIP_NO > 1
    {
        {MX_CMD_EX4B,},
        NULL,
        0,
        1,
        FLS_csupB,
        FLS_csdownB,
    }
#endif
};


/*******************************************************************************
**����ʹ��4�ֽ�ָ��(>=32M)
********************************************************************************/
const SPIIO gs_FlashEnable4B[] = 
{
    {
        {MX_CMD_EN4B, },
        NULL,
        0,
        1,
        FLS_csupA,
        FLS_csdownA,
    },
#if FLS_CHIP_NO > 1
    {
        {MX_CMD_EN4B, },
        NULL,
        0,
        1,
        FLS_csupB,
        FLS_csdownB,
    }
#endif
};

/*******************************************************************************
**�����ȡ��ȫ�Ĵ���(>=32M)
********************************************************************************/
const SPIIO gs_FlashGetSecu[] = 
{
    {
        {MX_CMD_RDSCUR, },
        guc_FlashSecu,
        1,
        1,
        FLS_csupA,
        FLS_csdownA,
    },
#if FLS_CHIP_NO > 1
    {
        {MX_CMD_RDSCUR, },
        guc_FlashSecu + 1,
        1,
        1,
        FLS_csupB,
        FLS_csdownB,
    }
#endif
};


/******************************************************************************
**��ȡFLASH״̬
******************************************************************************/
#define FLS_GetStatus(X) (SPI_Read((SPIIO*)&gs_FlashGetStt[X]))

#define FLS_WriteStatus(x) {SPI_Write((SPIIO*)&gs_FlsWriteStatus[x]);}

/******************************************************************************
**дʹ��
******************************************************************************/
#define FLS_EnableWrite(x) {SPI_Write((SPIIO*)&gs_FlsEnableWrite[x]);}

/******************************************************************************
**д����
******************************************************************************/
#define FLS_DisableWrite(x) {SPI_Write((SPIIO*)&gs_FlsDisableWrite[x]);}





/******************************************************************************
**����������ֽڵ�ַ(>=32M)
******************************************************************************/
#define FLS_Enable4B(x) {SPI_Write((SPIIO*)&gs_FlashEnable4B[x]);}


/******************************************************************************
**�˳��������ֽڵ�ַ״̬(>=32M)
******************************************************************************/
#define FLS_Disable4B(x) {SPI_Write((SPIIO*)&gs_FlashDisable4B[x]);}


/******************************************************************************
**��ȡ��ȫ�Ĵ�����ֵ(>=32M)
******************************************************************************/
#define FLS_GetSecu(x) {SPI_Read((SPIIO*)&gs_FlashGetSecu[x]);}
/************************************************************************
 * @function: SPI_Write
 * @����: д����
 * 
 * @����: 
 * @param: spi д�����ݵĵ�ַ�Ľṹ��Ϣ** 
 * 
 * @����: 
 * @return: unsigned char  
 * @˵��: 
 * @����: yzy (2018-07-02)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
//	unsigned char SPI_Write(SPIIO* spi);
//	
//	/************************************************************************
//	 * @function: SPI_Read
//	 * @����: ������
//	 * 
//	 * @����: 
//	 * @param: spi ��ȡ���ݵĵ�ַ�Ľṹ��Ϣ** 
//	 * 
//	 * @����: 
//	 * @return: unsigned char  
//	 * @˵��: 
//	 * @����: yzy (2018-07-02)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	unsigned char SPI_Read(SPIIO* spi);
//	
//	/************************************************************************
//	 * @function: SPI_ReadAndCompare
//	 * @����: �������ж�ȡָ�����ȵ����ݺͻ�����������ݽ��бȽ�
//	 * 
//	 * @����: 
//	 * @param: spi ��ȡ���ݵĵ�ַ�Ľṹ��Ϣ**  
//	 * 
//	 * @����: 
//	 * @return: unsigned char  
//	 * @˵��: 
//	 * @����: yzy (2018-07-02)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	unsigned char SPI_ReadAndCompare(SPIIO* spi);
//	
//	
//	/************************************************************************
//	 * @function: SPI_Close
//	 * @����: SPI����͹���
//	 * 
//	 * @����: 
//	 * @param: devidx �豸����(0-N)
//	 * 
//	 * @����: 
//	 * @return: 
//	 * @˵��: 
//	 * @����: yzy (2018-07-02)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void SPI_Close(unsigned char devidx);
//	
//	/************************************************************************
//	 * @function: SPI_Write
//	 * @����: д����
//	 * 
//	 * @����: 
//	 * @param: spi д�����ݵĵ�ַ�Ľṹ��Ϣ** 
//	 * 
//	 * @����: 
//	 * @return: unsigned char  
//	 * @˵��: 
//	 * @����: yzy (2018-07-02)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	unsigned char SPI_Write(SPIIO* spi)
//	{
//	    unsigned short ui_i;                         //ѭ������
//	    SYS_VAR_CHECK(spi->cmdnum > 8);
//	    _SPI_SCK_Down();//SPI_SCK_0();
//	    spi->csdown();                      //cs�߱�����
//	                                        //ѭ������ָ��������ڲ���ַ
//	    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
//	    {                                   //����ָ��͵�ַ
//	        _SPI_SendByte(spi->command[ui_i]);
//	    }
//	                                        //ѭ����������ֽ�
//	    for(ui_i = 0; ui_i < spi->length; ui_i ++)
//	    {                                        
//	        _SPI_SendByte(spi->data[ui_i]); //��������ֽ�        
//	    }
//	    spi->csup();                        //��CS������
//	    SYS_OK();
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: SPI_Read
//	 * @����: ������
//	 * 
//	 * @����: 
//	 * @param: spi ��ȡ���ݵĵ�ַ�Ľṹ��Ϣ** 
//	 * 
//	 * @����: 
//	 * @return: unsigned char  
//	 * @˵��: 
//	 * @����: yzy (2018-07-02)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	unsigned char SPI_Read(SPIIO* spi)
//	{
//	    unsigned short ui_i;                         //ѭ������
//	    SYS_VAR_CHECK(spi->cmdnum > 8);
//	    _SPI_SCK_Down();//SPI_SCK_0();
//	    spi->csdown();                      //cs�߱�����
//	                                        //ѭ������ָ��������ڲ���ַ
//	    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
//	    {                                   //����ָ��͵�ַ
//	        _SPI_SendByte(spi->command[ui_i]);
//	    }
//	                                        //ѭ����������ֽ�
//	    for(ui_i = 0; ui_i < spi->length; ui_i ++)
//	    {
//	                                        //��������ֽ�
//	        spi->data[ui_i] = _SPI_ReceiveByte();   
//	    }
//	    spi->csup();                        //��CS������
//	    
//	    
//	    return SPI_ReadAndCompare(spi);     //���ض����ȽϵĽ��
//	}
//	
//	
//	/************************************************************************
//	 * @function: SPI_ReadAndCompare
//	 * @����: �������ж�ȡָ�����ȵ����ݺͻ�����������ݽ��бȽ�
//	 * 
//	 * @����: 
//	 * @param: spi ��ȡ���ݵĵ�ַ�Ľṹ��Ϣ**  
//	 * 
//	 * @����: 
//	 * @return: unsigned char  
//	 * @˵��: 
//	 * @����: yzy (2018-07-02)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	unsigned char SPI_ReadAndCompare(SPIIO* spi)
//	{
//	    unsigned short ui_i;
//	    SYS_VAR_CHECK(spi->cmdnum > 8);    
//	    _SPI_SCK_Down();//SPI_SCK_0();
//	    spi->csdown();                      //cs�߱�����
//	    
//	                                        //ѭ������ָ��������ڲ���ַ
//	    //printf("_SPI_SendByte command =");
//	    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
//	    {                                   //����ָ��͵�ַ
//	        _SPI_SendByte(spi->command[ui_i]);
//	        //printf(" %x", spi->command[ui_i]);
//	    }   
//	    //printf("\n");
//	                                        //ѭ����������ֽ�
//	    for(ui_i = 0; ui_i < spi->length; ui_i ++)
//	    {
//	//        unsigned char tmp = _SPI_ReceiveByte();
//	        
//	                                        //��������ֽ�
//	        if(spi->data[ui_i] != _SPI_ReceiveByte())
//	//        if(spi->data[ui_i] != tmp)
//	        {
//	//            printf("i = %d data = %x, tmp = %x\n", ui_i, spi->data[ui_i], tmp);
//	            spi->csup();                //��CS������
//	            
//	            return 2;
//	        }
//	    }
//	    
//	    spi->csup();                        //��CS������
//	    SYS_OK();
//	}
//	
//	
//	/************************************************************************
//	 * @function: SPI_Close
//	 * @����: SPI����͹���
//	 * 
//	 * @����: 
//	 * @param: devidx �豸����(0-N)
//	 * 
//	 * @����: 
//	 * @return: 
//	 * @˵��: 
//	 * @����: yzy (2018-07-02)
//	 *-----------------------------------------------------------------------
//	 * @�޸���: 
//	 ************************************************************************/
//	void SPI_Close(unsigned char devidx)
//	{
//	    if(devidx == 0)
//	    {
//	        //SPI_SCK_0();
//	        //SPI_DOUT_0();
//	        _SPI_SCK_Down();
//	        _SPI_DOUT_Down();
//	    }
//	    else if(devidx == 1)
//	    {
//	    }
//	    else if(devidx == 2)
//	    {
//	    }
//	    else
//	    {
//	        return;
//	    }
//	}



/******************************************************************************
**�رձ����ܽ�
******************************************************************************/
void FLS_KillHwWp(void)
{
    SPI_WP_1(SPI_DEV_ID_FLSA);
#if FLS_CHIP_NO > 1
    SPI_WP_1(SPI_DEV_ID_FLSB);
#endif
}

/******************************************************************************
**�򿪹���ܽ�
******************************************************************************/
void FLS_OpenPwr(void)
{
    SPI_PWR_0(SPI_DEV_ID_FLSA);
#if FLS_CHIP_NO > 1
    SPI_PWR_0(SPI_DEV_ID_FLSB);
#endif
}

/******************************************************************************
**�رչ���ܽ�
******************************************************************************/
void FLS_ClosePwr(void)
{
    SPI_PWR_1(SPI_DEV_ID_FLSA);
    SPI_CS_0(0);
    SPI_WP_0(0);
    SPI_Close(0);
//        FLASH_CS(0);
//        FLASH_WP(0);
//        FLASH_MOSI(0);
//        FLASH_CLK(0);    
#if FLS_CHIP_NO > 1
    SPI_PWR_1(SPI_DEV_ID_FLSB);
    SPI_CS_0(1);
    SPI_WP_0(1);
    SPI_Close(1);

#endif
}

/******************************************************************************
**����Flashд����
******************************************************************************/
void FLS_Disable_WP(unsigned long X)
{
    FLS_EnableWrite(X);
    SPI_Write((SPIIO*)&gs_FlashDisableWP[X]);
}

/******************************************************************************
**д����
******************************************************************************/
//	void FLS_Enable_WP(unsigned long X)
//	{
//	    FLS_EnableWrite(X);
//	    SPI_Write((SPIIO*)&gs_FlashEnableWP[X]);
//	}

/******************************************************************************
**Flash�����Լ��˳��͹���ģʽ
******************************************************************************/
extern unsigned char _SPI_SendByte(unsigned char byte);
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
 * @�޸���: houpm (2009-8-8)
 **********************************************************/
bool FLS_FREE(unsigned long x, bool imd)
{
    int ret = FLS_GetStatus(x);                   //����Flash��״̬
//	    printf("line[%d]:guc_FlashStatus[x] = 0x%x ret = %d\n",__LINE__,guc_FlashStatus[x], ret);

    if(!(guc_FlashStatus[x] & 0x01))    //�ж�Flash��״̬
    {
        FLS_GetStatus(x);               //����Flash��״̬
//	        printf("line[%d]:guc_FlashStatus[x] = 0x%x ret = %d\n",__LINE__,guc_FlashStatus[x], ret);
        if(!(guc_FlashStatus[x] & 0x01))//�ٴ��ж�,��ֹ�жϴ���
        {
            return true;                //���о����Ϸ���
        }
    }
    for(unsigned short dly = 100; dly > 0; dly--);    
//	    if(imd)                             //�ж��Ƿ���Ҫ���ٷ��ص�
//	    {
//	                                        //�ӳ�100��ѭ��
//	        for(unsigned short dly = 100; dly > 0; dly--);
//	    }
//	    else
//	    {
//	        sleep_ms(1);                 //�ӳ�10ms
//	    }
    return false;                       //���ز�����
}

/*******************************************************************************
** @function_name:  EraseFlsSectorInChip
** @function_file:  mxflash.c
** @����: MX Flash ���߳�����
** 
** 
** @����: 
** @param: address(unsigned long): 
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
unsigned char EraseFlsSectorInChip(unsigned long address, unsigned char x)
{
    unsigned char err;
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

        gs_FlashSPI[x].data = NULL;
        gs_FlashSPI[x].cmdnum = 4;
        gs_FlashSPI[x].length = 0;
    }
    else                                    //>=32M
    {
        FLS_GetSecu(x);
        if(!(guc_FlashSecu[x] & MX_CMD_4BYTEADDR_BIT))
        {
            FLS_Enable4B(x);                //����ʹ��4�ֽ�ָ��
            return 2;
        }
        
        gs_FlashSPI[x].command[0] = MX_CMD_SE;  //���߳�ָ��
                                                //���ַ
        gs_FlashSPI[x].command[1] = address >> 24;
        gs_FlashSPI[x].command[2] = address >> 16;
        gs_FlashSPI[x].command[3] = address >> 8;
        gs_FlashSPI[x].command[4] = address;

        gs_FlashSPI[x].data = NULL;
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
** @����: MX Flash ���߳�����
** 
** 
** @����: 
** @param: address(unsigned long): 
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
unsigned char EraseFlsBlockInChip(unsigned long address, unsigned char x)
{
    unsigned char err;
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

        gs_FlashSPI[x].data = NULL;
        gs_FlashSPI[x].cmdnum = 4;
        gs_FlashSPI[x].length = 0;
    }
    else                                    //>=32M
    {
        FLS_GetSecu(x);
        if(!(guc_FlashSecu[x] & MX_CMD_4BYTEADDR_BIT))
        {
            FLS_Enable4B(x);                //����ʹ��4�ֽ�ָ��
            return 2;
        }
        
        gs_FlashSPI[x].command[0] = MX_CMD_BE;  //���߳�ָ��
                                                //���ַ
        gs_FlashSPI[x].command[1] = address >> 24;
        gs_FlashSPI[x].command[2] = address >> 16;
        gs_FlashSPI[x].command[3] = address >> 8;
        gs_FlashSPI[x].command[4] = address;

        gs_FlashSPI[x].data = NULL;
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
** @function_name:  ReadFlsInChip
** @function_file:  mxflash.c
** @����: ��Flashָ����ַ��ȡָ�����ȵ��ֽ�����
** 
** 
** @����: 
** @param: buffer(unsigned char*): ���ݻ���
** @param: address(unsigned long): ��ַ
** @param: length(unsigned short): ��ȡ�����ݳ���
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
unsigned char ReadFlsInChip(unsigned char* buffer, unsigned long address, unsigned short length, unsigned char x)
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
        FLS_GetSecu(x);
        if(!(guc_FlashSecu[x] & MX_CMD_4BYTEADDR_BIT))
        {
            FLS_Enable4B(x);                //����ʹ��4�ֽ�ָ��
            return 2;
        }
        
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
** @function_name:  WriteFlsPageInChip
** @function_file:  mxflash.c
** @����: Flashҳ���
** 
** 
** @����: 
** @param: buffer(unsigned char*): ���ݻ���
** @param: address(unsigned long): Ƭ�ڵ�ַ
** @param: length(unsigned short): д�����ݵĳ���
** @param: x(unsigned long): 
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
unsigned char WriteFlsPageInChip(unsigned char* buffer, unsigned long address, unsigned short length, unsigned char x)
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
        FLS_GetSecu(x);
        if(!(guc_FlashSecu[x] & MX_CMD_4BYTEADDR_BIT))
        {
            FLS_Enable4B(x);                    //����ʹ��4�ֽ�ָ��
            return 2;
        }
        
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
** @param: address(unsigned long): ������ַ
** @param: length(unsigned short): �������ݵĳ���
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
unsigned long MultiFlashFilterExt(S_FLSINCHIP* s_out, unsigned long address, unsigned long length)
{
    unsigned long index = 0;                   //�漰���Ĵ洢оƬ���
    unsigned long s1 = 0;                      //ĳƬFLASH��Ӧ����ʼ�߼���ַ
    unsigned long s2 = 0;                      //ĳƬFLASH��Ӧ����ֹ�߼���ַ(����)
    unsigned long back = address;
    unsigned char sidx = 0;
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
** @function_name:  FLS_SectorErase
** @function_file:  mxflash.c
** @����: MX Flash ��4K�߳�����
** 
** 
** @����: 
** @param: address(unsigned long): 
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
unsigned char FLS_SectorErase(unsigned long address)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    unsigned char num;
                                                //���Ҵ洢оƬ��Ƭ�ڵ�ַ
    num = MultiFlashFilterExt(fs, address, 1);
    if(num == 0)
    {
        return 2;
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
** @param: address(unsigned long): 
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
unsigned char FLS_BlockErase(unsigned long address)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    unsigned char num;
                                                //���Ҵ洢оƬ��Ƭ�ڵ�ַ
    num = MultiFlashFilterExt(fs, address, 1);
    if(num == 0)
    {
        return 2;
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
** @param: address(unsigned long)
** 
** @����: 
** @return:  bool true:ȫ��,false:�ǿ�
** @����: yzy (2012-06-09)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
//	bool FLS_SectorBlankCheck(unsigned long address)
//	{
//	    address &= ~0xFFF;                      //����ʼ��ַ
//	    
//	    PCB_FLS_Read(gcp_FlsBuffer, address, 4096);
//	    for(unsigned long uc_i = 0; uc_i < 4096; uc_i++)
//	    {
//	        if(gcp_FlsBuffer[uc_i] != 0xFF)
//	        {
//	            return false;
//	        }
//	    }
//	
//	    return true;
//	}



/*******************************************************************************
** @function_name:  FLS_PageBlankCheck
** @function_file:  mxflash.c
** @����: MX Flash ҳ�հ��ж�
** 
** 
** @����: 
** @param: address(unsigned long)
** 
** @����: 
** @return:  bool true:ȫ��,false:�ǿ�
** @����: yzy (2012-06-09)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
//	bool FLS_PageBlankCheck(unsigned long address)
//	{
//	    address &= ~0xFF;                      //ҳ��ʼ��ַ
//	    
//	    PCB_FLS_Read(gcp_FlsBuffer, address, 256);
//	    for(unsigned long uc_i = 0; uc_i < 256; uc_i++)
//	    {
//	        if(gcp_FlsBuffer[uc_i] != 0xFF)
//	        {
//	            return false;
//	        }
//	    }
//	
//	    return true;
//	}



/*******************************************************************************
** @function_name:  FLS_WritePage
** @function_file:  mxflash.c
** @����: Flashҳ���
** 
** 
** @����: 
** @param: buffer(unsigned char*): 
** @param: address(unsigned long): 
** @param: length(unsigned short): 
** @param: x(unsigned long): 
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
unsigned char FLS_WritePage(unsigned char* buffer, unsigned long address, unsigned short length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    unsigned char num;
                                                //���Ҵ洢оƬ��Ƭ�ڵ�ַ
    num = MultiFlashFilterExt(fs, address, 1);
    if(num == 0)
    {
        return 2;
    }
    
    return WriteFlsPageInChip(buffer, fs[0].opaddr, length, fs[0].x);
}






//**********************************************************************************
//***********************���Ӻ���**********************
//**********************************************************************************











/*******************************************************************************
** @function_name:  PCB_FLS_Erase
** @function_file:  mxflash.c
** @����: MX Flash �߳�����
** 
** 
** @����: 
** @param: address: ��ʼ��ַ
** @param: sectornum ��������
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
unsigned char FLS_Erase(unsigned long address, unsigned long sectornum)
{
    unsigned long ul_i = 0;
    unsigned char ret = 0;
    
//	    DRV_POW_CHECK(PCB_VBatVoltGet() < 2800);    
    
    if(address & 0xFFF)
    {
        return SYS_ERR_VAR;
    }
                                        //ѭ������
    while(ul_i < sectornum)
    {
        //                              //ι��:����ִ�иú���ʱ,ι�����̵ȴ���Դʱ�����,���¸�λ
        SYS_FeedDOG();
        //

        ret = FLS_SectorErase(address);
        if(ret != 0)
        {
            break;
        }
        ul_i++;
        address += 0x1000;

    }
    if(ret)
    {
        return SYS_ERR_VAR;
    }
    else
    {
        return SYS_ERR_OK;
    }
}


unsigned char FLS_BErase(unsigned long address, unsigned long sectornum)
{
    unsigned long ul_i = 0;
    unsigned char ret = 0;
    
//	    DRV_POW_CHECK(PCB_VBatVoltGet() < 2800);    
    
    if(address & 0xFFFF)
    {
        return SYS_ERR_VAR;
    }
                                        //ѭ������
    while(ul_i < sectornum)
    {
        //                              //ι��:����ִ�иú���ʱ,ι�����̵ȴ���Դʱ�����,���¸�λ
        SYS_FeedDOG();
        //

        ret = FLS_BlockErase(address);
        if(ret != 0)
        {
            break;
        }
        ul_i++;
        address += 0x10000;

    }
    if(ret)
    {
        return SYS_ERR_VAR;
    }
    else
    {
        return SYS_ERR_OK;
    }
}








/*******************************************************************************
** @function_name:  PCB_FLS_Read
** @function_file:  mxflash.c
** @����: ��Flashָ����ַ��ȡָ�����ȵ��ֽ�����
** 
** 
** @����: 
** @param: buffer(unsigned char*): ���ݻ���
** @param: address(unsigned long): ��ַ
** @param: length(unsigned short): ��ȡ�����ݳ���
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
unsigned char FLS_Read(unsigned char* buffer, unsigned long address, unsigned short length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    unsigned char err = 0;
    unsigned char num;
//	    DRV_POW_CHECK(PCB_VBatVoltGet() < 2800);    

    num = MultiFlashFilterExt(fs, address, length);
    if(num == 0)
    {
        return 2;
    }
    
    for(unsigned char uc_i = 0; uc_i < num; uc_i++)
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
 * @return: unsigned char  
 * @˵��: 
 * @����: yzy (2014/6/18)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
unsigned char FLS_PreWrite(void)
{
    gul_FlashSafeCode = 0xA5A5A5A5;
    SYS_OK();
}

//	#ifdef CHIP_R7F0C00X
#if (BIT_FLS_BUFSFT > 0)


/************************************************************************ 
 *�������ؽ��
 ************************************************************************/
#define FLS_WRSTT_WRI   0x01                //�Ƿ���Ҫд��
#define FLS_WRSTT_ERA   0x02                //�Ƿ���Ҫ����
#define FLS_WRSTT_SIS   0x04                //�Ƿ�����������뱸����һ���Դ���

/*******************************************************************************
** @function_name:  CompareFlsInChip
** @function_file:  mxflash.c
** @����: ��Flashָ����ַ��ȡָ�����ȵ��ֽ�����,���봫�����ݱȽ��Ƿ����
** 
** 
** @����: 
** @param: buffer(unsigned char*): ���ݻ���
** @param: address(unsigned long): ��ַ
** @param: length(unsigned short): ��ȡ�����ݳ���
** 
** @����: 
** @return:  unsigned char   SYS_ERR_OK���,2�����
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
unsigned char CompareFlsInChip(unsigned char* buffer, unsigned long address, unsigned short length, unsigned char x)
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
        FLS_GetSecu(x);
        if(!(guc_FlashSecu[x] & MX_CMD_4BYTEADDR_BIT))
        {
            FLS_Enable4B(x);                //����ʹ��4�ֽ�ָ��
            return 2;
        }
        
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

/************************************************************************
 * @function: SectorVerifyForWrite
 * @����:    1)ȷ����������ԭʼ������һ����
 *           2)ѭ���ж��Ƿ����д,��дǰ�Ƿ������
 * 
 * @����:
 * @param: buffer �жϵĻ���ָ��
 * @param: templet �жϵ��ַ�ģ��
 * @param: addr ��������flash�ĵ�ַ
 * @param: length ����SECTOR�в����ĳ���
 * @param: x �ڼ�Ƭflash
 * 
 * @����: 
 * @return: unsigned char  λͼ���:FLS_WRSTT_WRI/FLS_WRSTT_ERA/FLS_WRSTT_SIS
 * @˵��: 
 * @����: yzy (2014/10/21)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
unsigned char SectorVerifyForWrite(unsigned char* buffer, unsigned char templet, unsigned long addr, unsigned short length, unsigned char x)
{
    unsigned long addrInFlash;                     //FLASH�ľ��Ե�ַ
    unsigned long addrInSector;                    //������Ե�ַ
    unsigned long addrOfSector;                    //
    unsigned short bufferIdx = 0;
    unsigned short flsIdx = 0;
    unsigned short compLen;                     //���αȽϵ����ݳ���
    unsigned char  result = 0;
    unsigned char  m;
    unsigned char  n;
    
    addrInSector = addr & 0xFFF;
    addrOfSector = addr & (~0xFFF);
    addrInFlash = addrOfSector;         //����ָ��SECTORͷ��
    
    for(m = 0; m < NUM_FLS_SECTBUF; m++)//�����ѭ��x��(SECTOR = x * LEN_FLS_BUF)
    {
        for(n = 0; n < 2; n++)          //��αȶ�.���״αȶԴ���ʱ,�ٴαȶ�.
        {
            ReadFlsInChip(gcp_FlsBuffer, addrInFlash, LEN_FLS_BUF, x);
//	            printf("line[%d]:gcp_FlsBuffer[0]=0x%x\n", __LINE__, gcp_FlsBuffer[0]);
            if(CompareFlsInChip(gcp_FlsBuffer, (guls_FLSChipSize[x] >> 1) + addrInFlash, LEN_FLS_BUF, x) == SYS_ERR_OK)
            {
                break;                  //��ȷʱ����ѭ��
            }
        }
        
        if(n == 2)                      //����.���Ϊ������һ���Դ���.consistency error
        {
            result |= FLS_WRSTT_SIS;
        }
        addrInFlash += LEN_FLS_BUF;
        
                                        //�ж��Ƿ����д,��дǰ�Ƿ������
        //*************************************************************
        if(length)
        {
            if(addrInSector < ((m + 1) * LEN_FLS_BUF))
            {
                if((addrInSector + length) > ((m + 1) * LEN_FLS_BUF))
                {
                    compLen = LEN_FLS_BUF - (addrInSector & MASK_FLS_BUFLEN);
                }
                else
                {
                    compLen = length;
                }
                                        //����д��������ԭ�����ݽ��бȽ�
                for(flsIdx = 0; flsIdx < compLen; flsIdx++)
                {
                    if(buffer == NULL)//�ַ�ģ��ȶ�
                    {
                        templet = templet;
                    }
                    else                //ȫ������һ�ȶ�
                    {
                        templet = buffer[bufferIdx];
                        bufferIdx++;    //next byte
                    }
                                        //�ж��Ƿ���ԭ������һ��
                    if(gcp_FlsBuffer[(addrInSector & MASK_FLS_BUFLEN) + flsIdx] != templet)
                    {
                        result |= FLS_WRSTT_WRI;
                        
//	                        printf("line[%d]:addrInSector = 0x%x\n", __LINE__, addrInSector);
//	                        printf("line[%d]:gcp_FlsBuffer[%d] = 0x%x  templet = 0x%x\n", __LINE__, flsIdx, gcp_FlsBuffer[(addrInSector & MASK_FLS_BUFLEN) + flsIdx],templet);
                        if((gcp_FlsBuffer[(addrInSector & MASK_FLS_BUFLEN) + flsIdx] & templet) != templet)
                        {               //����ԭ�����ݲ�һ���ֲ���FF����Ҫ����
                            result |= FLS_WRSTT_ERA;
                            break;
                        }
                    }

                }
                
                addrInSector += compLen;
                length -= compLen;
            }
        }
        else
        {
            //�Ż�
            if((result & FLS_WRSTT_SIS) == FLS_WRSTT_SIS)
            {
                break;
            }
        }
        //*************************************************************
    }
    
    return result;
}



/************************************************************************
 * @function: SectorConsisForWrite
 * @����: ��sectorһ���Ծ���
 * 
 * @����: 
 * @param: sectoraddr ҳ����ʼ��ַ
 * @param: x �ڼ�Ƭflash
 * @����: 
 * @˵��: 
 * @����: yzy (2014/10/21)
 *-----------------------------------------------------------------------
 * @�޸���: 
 ************************************************************************/
void SectorConsisForWrite(unsigned long sectoraddr, unsigned char x)
{
    unsigned long tmp;
    unsigned char m,n;

    tmp = sectoraddr & (~0xFFF);
                                        //����������
    EraseFlsSectorInChip((guls_FLSChipSize[x] >> 1) + tmp, x);

    for(m = 0; m < NUM_FLS_SECTBUF; m++)//������ȡ��ȡ,��д�뱸����
    {
        ReadFlsInChip(gcp_FlsBuffer, tmp, LEN_FLS_BUF, x);
//	        printf("read ok,prepare to write\n");
        for(n=0;n<4;n++)
        {
            WriteFlsPageInChip(gcp_FlsBuffer+n*256, (guls_FLSChipSize[x] >> 1) + tmp + n*256, 256, x);
        }
        tmp += LEN_FLS_BUF;
    }
}

/*******************************************************************************
** @function_name:  WriteFlsInChip
** @function_file:  mxflash.c
** @����: Flash д����
** 
** 
** @����: 
** @param: buffer(unsigned char*): ��д�����ݵĴ�Ż���
** @param: address(unsigned long): flashƬ�ڵ�ַ
** @param: length(unsigned short): ���ݳ���
** @param: x(unsigned char): �ڼ�Ƭflash
** @param: sdmode(unsigned char): �������ݱ���ģʽ.[0.������������(Ĭ��),1.��������λ�ڴ�д����֮ǰ������,2.ɾ����������]
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy(20111-01-10)
** @�޸�˵��: ���ĺ���ʵ�ַ�ʽ,�򻯳���
*******************************************************************************/
unsigned char WriteFlsInChip(unsigned char* buffer, unsigned long addr, unsigned short length, unsigned char x, unsigned char sdmode)
{
    unsigned char  sidx, eidx;                      //д�����ʼҳ�ͽ���ҳ
    unsigned char  fstt = 0;
    unsigned char  m;
    unsigned char  n;
    unsigned long addrInSector;                    //������Ե�ַ
    unsigned long addrOfSector;                    //
    unsigned short ui_len = 0;                      //����SECTOR��д��ĳ���
    unsigned short ui_tmp;
                                            //��ȡ��λ�ò���,�������Ƭ��ȡ
    SYS_VAR_CHECK(length + addr > (guls_FLSChipSize[x] >> 1));//ֻ��ʹ��һ������
    SYS_VAR_CHECK(length == 0);
    SYS_VAR_CHECK(sdmode != 0);

    /************************************************************************
    //ѭ������ÿ����Ҫ���ĵ�SECTOR
    ************************************************************************/
    while(true)                             //ѭ������ÿ����Ҫ���ĵ�SECTOR
    {
        addrOfSector = addr & (~0xFFF);
        addrInSector = addr & 0xFFF;
        if(length + addrInSector > 4096)    //���
        {
            ui_len = 4096 - addrInSector;   //���������ݳ���
        }
        else                                //���ڲ���
        {
            ui_len = length;                //�������ݳ���
        }

        /************************************************************************
        //�ж�sector״̬:�Ƿ����һ���Դ���,�Ƿ���Ҫ����,д��
        ************************************************************************/
        fstt = SectorVerifyForWrite(buffer, 0, addr, ui_len, x);

        /************************************************************************
        //һ���Դ���,��д������
        ************************************************************************/
        if(fstt & FLS_WRSTT_SIS)            //��һ��,��д������
        {
            SectorConsisForWrite(addrOfSector, x);
            guc_FlashErrStt |= FLS_ERR_CSIS;
        }
//	        printf("fstt = 0x%x\n", fstt);
	
        /************************************************************************
        //����BAK������,���º�д��ORG��
        ************************************************************************/
        if(fstt & FLS_WRSTT_WRI)            //��Ҫд��
        {
            if(fstt & FLS_WRSTT_ERA)        //��Ҫ����
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
                                            //�����ֲ�����
            //��ǰsector��Ҫ���ĵ�ҳ:wrPage1-wrPage2(����)
            unsigned char wrPage1 = addrInSector >> 8;
            unsigned char wrPage2 = (addrInSector + ui_len - 1) >> 8;
            unsigned short bufferIdx = 0;
            
            for(m = 0; m < 3; m++)          //ѭ��д,���д3��
            {
                for(n = sidx; n < eidx; n++)//��ҳд��
                {
                                            //��BAKҳ��ȡ����
                    ui_tmp = (n - sidx) % (LEN_FLS_BUF >> 8);//����256
                    if(!ui_tmp)             //��Ҫ��flash�ж�ȡ�µ����ݵ�BUF
                    {
                        ReadFlsInChip(gcp_FlsBuffer, 
                                      (guls_FLSChipSize[x] >> 1) + addrOfSector + (n << 8), 
                                      LEN_FLS_BUF, 
                                      x);
                    }
                    else                    //�������ϴζ�ȡ��BUF��.
                    {
                        memcpy(gcp_FlsBuffer, 
                                   gcp_FlsBuffer + 256,                                    
                                   LEN_FLS_BUF - (ui_tmp << 8));//����256
                    }
	
                                            //�滻
                    //*************************************************
                    if(n == wrPage1)        //first page
                    {
                        if(n == wrPage2)    //ֻ��һҳ
                        {
                            memcpy(gcp_FlsBuffer + (addrInSector & 0xFF), 
                                    buffer, 
                                    ui_len);
                                            //��ʼ��bufferIdx
                            bufferIdx = ui_len;
                        }
                        else
                        {
                            memcpy(gcp_FlsBuffer + (addrInSector & 0xFF), 
                                    buffer, 
                                    256 - (addrInSector & 0xFF));
                                            //��ʼ��bufferIdx
                            bufferIdx = 256 - (addrInSector & 0xFF);
                        }
                    }
                    else if((n > wrPage1) && (n < wrPage2))
                    {                       //middle page
                        memcpy(gcp_FlsBuffer, buffer + bufferIdx,  256);
                        bufferIdx += 256;
                    }
                    else if(n == wrPage2)   //last page
                    {
                        memcpy(gcp_FlsBuffer, 
                                buffer + bufferIdx, 
                                (addrInSector + ui_len) & 0xFF);
                    }
                    //*************************************************
                                            //д��ORGҳ
                    if(WriteFlsPageInChip(gcp_FlsBuffer, 
                                          addrOfSector + (n << 8), 
                                          256, x) != SYS_ERR_OK)
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
                return SYS_ERR_FT;          //����д�����ݴ���
            }
        }
	
	
        /************************************************************************
        //����ORG������,��д��BAK��
        ************************************************************************/
        if(fstt & FLS_WRSTT_WRI)            //��Ҫд��
        {
            if(fstt & FLS_WRSTT_ERA)        //��Ҫ����
            {
                EraseFlsSectorInChip((guls_FLSChipSize[x] >> 1) + addrOfSector, x);
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
                                            // 
            for(m = 0; m < 3; m++)          //ѭ��д,���д3��
            {
                for(n = sidx; n < eidx; n++)//��ҳд��
                {
                                            //��ORGҳ��ȡ����
                    ui_tmp = (n - sidx) % (LEN_FLS_BUF >> 8);//����256
                    if(!ui_tmp)
                    {
                        ReadFlsInChip(gcp_FlsBuffer, 
                                      addrOfSector + (n << 8), 
                                      LEN_FLS_BUF, 
                                      x);
                    }
                    else
                    {
                        memcpy(gcp_FlsBuffer, 
                                gcp_FlsBuffer + 256, 
                                LEN_FLS_BUF - (ui_tmp << 8));//����256
                    }
                                            //д��BAKҳ
                    if(WriteFlsPageInChip(gcp_FlsBuffer, 
                                          (guls_FLSChipSize[x] >> 1) + addrOfSector + (n << 8), 
                                          256, x) != SYS_ERR_OK)
                    {
                        EraseFlsSectorInChip((guls_FLSChipSize[x] >> 1) + addrOfSector, x);
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
                return SYS_ERR_FT;          //����д�����ݴ���
            }
        }

        /************************************************************************
        //�޸Ĵ������,׼���´�ѭ��
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
** @function_name:  SetFlsInChip
** @function_file:  mxflash.c
** @����: ��Flashĳ��������Ϊ�ض�ֵ 
** 
** 
** @����: 
** @param: templet(uint8_t): ���������
** @param: address(uint32_t): д�����ݵĵ�ַ
** @param: length(uint16_t): д�����ݵĳ���
** @param: x(uint8_t): flashоƬ���
** 
** @����: 
** @return:  uint8_t   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy
** @�޸�˵��: ��������ͬ WriteFlsInChip()
*******************************************************************************/
uint8_t SetFlsInChip(uint8_t templet, uint32_t addr, uint16_t length, uint8_t x)
{
    uint8_t  sidx, eidx;                      //д�����ʼҳ�ͽ���ҳ
    uint8_t  fstt = 0;
    uint8_t  m;
    uint8_t  n;
    uint32_t addrInSector;                    //������Ե�ַ
    uint32_t addrOfSector;                    //
    uint16_t ui_len = 0;                      //����SECTOR��д��ĳ���
    uint16_t ui_tmp;
                                            //��ȡ��λ�ò���,�������Ƭ��ȡ
    SYS_VAR_CHECK(length + addr > (guls_FLSChipSize[x] >> 1));//ֻ��ʹ��һ������
    SYS_VAR_CHECK(length == 0);

    /************************************************************************
    //ѭ������ÿ����Ҫ���ĵ�SECTOR
    ************************************************************************/
    while(true)                             //ѭ������ÿ����Ҫ���ĵ�SECTOR
    {
        addrOfSector = addr & (~0xFFF);
        addrInSector = addr & 0xFFF;
        if(length + addrInSector > 4096)    //���
        {
            ui_len = 4096 - addrInSector;   //���������ݳ���
        }
        else                                //���ڲ���
        {
            ui_len = length;                //�������ݳ���
        }

        /************************************************************************
        //�ж�sector״̬:�Ƿ����һ���Դ���,�Ƿ���Ҫ����,д��
        ************************************************************************/
        fstt = SectorVerifyForWrite(NULL, templet, addr, ui_len, x);
//	        printf("line[%d],fstt = 0x%x\n", __LINE__, fstt);

        /************************************************************************
        //һ���Դ���,��д������
        ************************************************************************/
        if(fstt & FLS_WRSTT_SIS)            //��һ��,��д������
        {
            SectorConsisForWrite(addrOfSector, x);
            guc_FlashErrStt |= FLS_ERR_CSIS;
        }

        /************************************************************************
        //����BAK������,���º�д��ORG��
        ************************************************************************/
        if(fstt & FLS_WRSTT_WRI)            //��Ҫд��
        {
            if(fstt & FLS_WRSTT_ERA)        //��Ҫ����
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

            //�����ֲ�����
            //��ǰsector��Ҫ���ĵ�ҳ:wrPage1-wrPage2(����)
            unsigned char wrPage1 = addrInSector >> 8;
            unsigned char wrPage2 = (addrInSector + ui_len - 1) >> 8;
            unsigned short bufferIdx = 0;
            
            for(m = 0; m < 3; m++)          //ѭ��д,���д3��
            {
                for(n = sidx; n < eidx; n++)//��ҳд��
                {
                                            //��BAKҳ��ȡ����
                    ui_tmp = (n - sidx) % (LEN_FLS_BUF >> 8);//����256
                    if(!ui_tmp)             //��Ҫ��flash�ж�ȡ�µ����ݵ�BUF
                    {
                        ReadFlsInChip(gcp_FlsBuffer, 
                                      (guls_FLSChipSize[x] >> 1) + addrOfSector + (n << 8), 
                                      LEN_FLS_BUF, 
                                      x);
                    }
                    else                    //�������ϴζ�ȡ��BUF��.
                    {
                        memcpy(gcp_FlsBuffer, 
                                   gcp_FlsBuffer + 256,                                    
                                   LEN_FLS_BUF - (ui_tmp << 8));//����256
                    }
	
                                            //�滻
                    //*************************************************
                    if(n == wrPage1)        //first page
                    {
                        if(n == wrPage2)    //ֻ��һҳ
                        {
                            memset(gcp_FlsBuffer + (addrInSector & 0xFF), 
                                    templet, 
                                    ui_len);
                                            //��ʼ��bufferIdx
                            bufferIdx = ui_len;
                        }
                        else
                        {
                            memset(gcp_FlsBuffer + (addrInSector & 0xFF), 
                                    templet, 
                                    256 - (addrInSector & 0xFF));
                                            //��ʼ��bufferIdx
                            bufferIdx = 256 - (addrInSector & 0xFF);
                        }
                    }
                    else if((n > wrPage1) && (n < wrPage2))
                    {                       //middle page
                        memset(gcp_FlsBuffer, templet,  256);
                        bufferIdx += 256;
                    }
                    else if(n == wrPage2)   //last page
                    {
                        memset(gcp_FlsBuffer, 
                                templet, 
                                (addrInSector + ui_len) & 0xFF);
                    }
                    //*************************************************
                                            //д��ORGҳ
                    if(WriteFlsPageInChip(gcp_FlsBuffer, 
                                          addrOfSector + (n << 8), 
                                          256, x) != SYS_ERR_OK)
                    {
                        EraseFlsSectorInChip(addrOfSector, x);
                        sidx = 0;
                        eidx = 16;
//	                        printf("WriteFlsPageInChip err !! \n");
                        break;
                    }
                }
                
                if(n >= eidx)               //д��ɹ�,����
                {
//	                    printf("n >= eidx !! \n");
                    break;
                }
            }
            if(m >= 3)
            {
                guc_FlashErrStt |= FLS_ERR_WRITE;//��־д�����ݷ�������
                return SYS_ERR_FT;          //����д�����ݴ���
            }
        }


        /************************************************************************
        //����ORG������,��д��BAK��
        ************************************************************************/
        if(fstt & FLS_WRSTT_WRI)            //��Ҫд��
        {
            if(fstt & FLS_WRSTT_ERA)        //��Ҫ����
            {
                EraseFlsSectorInChip((guls_FLSChipSize[x] >> 1) + addrOfSector, x);
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
                                            // 
            for(m = 0; m < 3; m++)          //ѭ��д,���д3��
            {
                for(n = sidx; n < eidx; n++)//��ҳд��
                {
                                            //��ORGҳ��ȡ����
                    ui_tmp = (n - sidx) % (LEN_FLS_BUF >> 8);//����256
                    if(!ui_tmp)
                    {
                        ReadFlsInChip(gcp_FlsBuffer, 
                                      addrOfSector + (n << 8), 
                                      LEN_FLS_BUF, 
                                      x);
                    }
                    else
                    {
                        memcpy(gcp_FlsBuffer, 
                               gcp_FlsBuffer + 256, 
                               LEN_FLS_BUF - (ui_tmp << 8));//����256
                    }
                                            //д��BAKҳ
                    if(WriteFlsPageInChip(gcp_FlsBuffer, 
                                          (guls_FLSChipSize[x] >> 1) + addrOfSector + (n << 8), 
                                          256, x) != SYS_ERR_OK)
                    {
                        EraseFlsSectorInChip((guls_FLSChipSize[x] >> 1) + addrOfSector, x);
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
                return SYS_ERR_FT;          //����д�����ݴ���
            }
        }

        /************************************************************************
        //�޸Ĵ������,׼���´�ѭ��
        ************************************************************************/
        length -= ui_len;                   //���������ݳ��ȵݼ�
        if(length == 0)                     //�����Ѿ��������
        {
            SYS_OK();                        //����д��ɹ�
        }

        addr += ui_len;                     //ʵ�ʲ�����ַ
    }
}

#else

/*******************************************************************************
** @function_name:  WriteFlsInChip
** @function_file:  mxflash.c
** @����: Flash д����
** 
** 
** @����: 
** @param: buffer(unsigned char *): ��д�����ݵĴ�Ż���
** @param: address(unsigned long ): flashƬ�ڵ�ַ
** @param: length(unsigned short): ���ݳ���
** @param: x(unsigned char ): �ڼ�Ƭflash
** @param: sdmode(unsigned char ): �������ݱ���ģʽ.[0.������������(Ĭ��),1.��������λ�ڴ�д����֮ǰ������,2.ɾ����������]
** 
** @����: 
** @return:  unsigned char    
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy(20111-01-10)
** @�޸�˵��: ���ĺ���ʵ�ַ�ʽ,�򻯳���
*******************************************************************************/
unsigned char  WriteFlsInChip(unsigned char * buffer, unsigned long  addr, unsigned short length, unsigned char  x, unsigned char  sdmode)
{
    unsigned char   sidx, eidx;                      //д�����ʼҳ�ͽ���ҳ
    unsigned char   fwri;                            //�Ƿ���Ҫд���־
    unsigned char   fera;                            //�Ƿ���Ҫ������־
    unsigned char   m;
    unsigned char   n;
    int i = 0;
    //unsigned long  addrInFlash;                     //FLASH�ľ��Ե�ַ
    unsigned long  addrInSector;                    //������Ե�ַ
    unsigned long  addrOfSector;                    //
    unsigned short ui_len = 0;                      //����SECTOR��д��ĳ���
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
        //printf("gcp_FlsBuffer[] = %02X %02X %02X %02X %02X", gcp_FlsBuffer[0], gcp_FlsBuffer[1], gcp_FlsBuffer[2], gcp_FlsBuffer[3], gcp_FlsBuffer[4]);

                                            //��ȡһ������
        while(SYS_ERR_OK != ReadFlsInChip(gcp_FlsBuffer, addr & (~0xFFF) , 4096, x) && (i++ < 100));
        if(i >= 100)
        {
            return SYS_ERR_FT;
        }
//	        for(int i = 0; i < 16; i++)
//	        {
//	            while(SYS_ERR_OK != ReadFlsInChip(gcp_FlsBuffer+ 256 * i, addr & (~0xFFF) + 256 * i, 256, x));
//	
//	        }
        //printf("gcp_FlsBuffer[] = %02X %02X %02X %02X %02X", gcp_FlsBuffer[0], gcp_FlsBuffer[1], gcp_FlsBuffer[2], gcp_FlsBuffer[3], gcp_FlsBuffer[4]);
        /************************************************************************
        //ѭ���ж��Ƿ����д,��дǰ�Ƿ������
        ************************************************************************/
        fwri = 0;                           //��λ��־λ
        fera = 0;
                                            //ѭ���ж��Ƿ����д�����Ƿ���Ҫ����
        for(unsigned long  k = 0; k < ui_len; k++)
        {
            unsigned char  uc_data = gcp_FlsBuffer[(addr & 0xFFF) + k];
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
//	        printf("fwri = %x fera = %x\n", fwri, fera);
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
            memcpy(gcp_FlsBuffer + addrInSector, buffer, ui_len);
                                            // 
            for(m = 0; m < 3; m++)          //ѭ��д,���д3��
            {
                for(n = sidx; n < eidx; n++)//��ҳд��
                {
                                            
                    if(WriteFlsPageInChip(gcp_FlsBuffer + (n << 8), addrOfSector + (n << 8), 256, x) != SYS_ERR_OK)
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
                return SYS_ERR_FT;          //����д�����ݴ���
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
** @function_name:  SetFlsInChip
** @function_file:  mxflash.c
** @����: ��Flashĳ��������Ϊ�ض�ֵ 
** 
** 
** @����: 
** @param: templet(unsigned char): ���������
** @param: address(unsigned long): д�����ݵĵ�ַ
** @param: length(unsigned short): д�����ݵĳ���
** @param: x(unsigned char): flashоƬ���
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy
** @�޸�˵��: ��������ͬ WriteFlsInChip()
*******************************************************************************/
unsigned char SetFlsInChip(unsigned char templet, unsigned long addr, unsigned short length, unsigned char x)
{
    unsigned char  sidx, eidx;                      //д�����ʼҳ�ͽ���ҳ
    unsigned char  fwri;                            //�Ƿ���Ҫд���־
    unsigned char  fera;                            //�Ƿ���Ҫ������־
    unsigned char  m;
    unsigned char  n;
    int i = 0;
    //unsigned long addrInFlash;                     //FLASH�ľ��Ե�ַ
    unsigned long addrInSector;                    //������Ե�ַ
    unsigned long addrOfSector;                    //
    unsigned short ui_len = 0;                      //����SECTOR��д��ĳ���
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
        while(SYS_ERR_OK != ReadFlsInChip(gcp_FlsBuffer, addr & (~0xFFF), 4096, x) && (i++ < 100));
        
        if(i >= 100)
        {
            return SYS_ERR_FT;
        }
        /************************************************************************
        //ѭ���ж��Ƿ����д,��дǰ�Ƿ������
        ************************************************************************/
        fwri = 0;                           //��λ��־λ
        fera = 0;
                                            //ѭ���ж��Ƿ����д�����Ƿ���Ҫ����
        for(unsigned long k = 0; k < ui_len; k++)
        {
            unsigned char uc_data = gcp_FlsBuffer[(addr & 0xFFF) + k];
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
//            printf("gcp_FlsBuffer addr = %d len = %d \n", (addr & 0xFFF), ui_len);
            for(m = 0; m < 3; m++)          //ѭ��д,���д3��
            {
                for(n = sidx; n < eidx; n++)//��ҳд��
                {
                    if(WriteFlsPageInChip(gcp_FlsBuffer + (n << 8), addrOfSector + (n << 8), 256, x) != SYS_ERR_OK)
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
//            printf("set end m = %d\n", m);
            if(m >= 3)
            {
                guc_FlashErrStt |= FLS_ERR_WRITE;//��־д�����ݷ�������
                return SYS_ERR_FT;          //����д�����ݴ���
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


#endif

/*******************************************************************************
** @function_name:  WriteFlsInChip
** @function_file:  mxflash.c
** @����: Flash д����
** 
** 
** @����: 
** @param: buffer(unsigned char*): ��д�����ݵĴ�Ż���
** @param: address(unsigned long): flashƬ�ڵ�ַ
** @param: length(unsigned short): ���ݳ���
** @param: x(unsigned char): �ڼ�Ƭflash
** @param: sdmode(unsigned char): �������ݱ���ģʽ.[0.������������(Ĭ��),1.��������λ�ڴ�д����֮ǰ������,2.ɾ����������]
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy(20111-01-10)
** @�޸�˵��: ���ĺ���ʵ�ַ�ʽ,�򻯳���
*******************************************************************************/
unsigned char WriteFlsInChipSpec(unsigned char* buffer, unsigned long addr, unsigned short length, unsigned char x, unsigned char sdmode)
{
    //unsigned char  sidx, eidx;                      //д�����ʼҳ�ͽ���ҳ
   // unsigned char  fstt = 0;
    unsigned char  m;
    //unsigned char  n;
    unsigned long addrInSector;                    //������Ե�ַ
    //unsigned long addrOfSector;                    //
    unsigned short ui_len = 0;                      //����SECTOR��д��ĳ���
    //unsigned short ui_tmp;
                                            //��ȡ��λ�ò���,�������Ƭ��ȡ
    SYS_VAR_CHECK(length + addr > (guls_FLSChipSize[x] >> 1));//ֻ��ʹ��һ������
    SYS_VAR_CHECK(length == 0);
    SYS_VAR_CHECK(sdmode != 1);

    /************************************************************************
    //ѭ������ÿ����Ҫ���ĵ�SECTOR
    ************************************************************************/
    while(true)                             //ѭ������ÿ����Ҫ���ĵ�SECTOR
    {
        //addrOfSector = addr & (~0xFFF);
        addrInSector = addr & 0xFFF;
        if(length + addrInSector > 4096)    //���
        {
            ui_len = 4096 - addrInSector;   //���������ݳ���
        }
        else                                //���ڲ���
        {
            ui_len = length;                //�������ݳ���
        }

        for(m = 0; m < 3; m++)          //ѭ��д,���д3��
        {
            if(FLS_WritePage(buffer, addr, ui_len) == SYS_ERR_OK)
            {
                break;
            }
        }
        
        if(m >= 3)
        {
            guc_FlashErrStt |= FLS_ERR_WRITE;//��־д�����ݷ�������
            return SYS_ERR_FT;          //����д�����ݴ���
        }
        /************************************************************************
        //�޸Ĵ������,׼���´�ѭ��
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


//	#else
//	
//	/*******************************************************************************
//	** @function_name:  WriteFlsInChip
//	** @function_file:  mxflash.c
//	** @����: Flash д����
//	** 
//	** 
//	** @����: 
//	** @param: buffer(unsigned char*): ��д�����ݵĴ�Ż���
//	** @param: address(unsigned long): flashƬ�ڵ�ַ
//	** @param: length(unsigned short): ���ݳ���
//	** @param: x(unsigned char): �ڼ�Ƭflash
//	** @param: sdmode(unsigned char): �������ݱ���ģʽ.[0.������������(Ĭ��),1.��������λ�ڴ�д����֮ǰ������,2.ɾ����������]
//	** 
//	** @����: 
//	** @return:  unsigned char   
//	** @����: yzy (2010-05-11)
//	**-----------------------------------------------------------------------------
//	** @�޸���: yzy(20111-01-10)
//	** @�޸�˵��: ���ĺ���ʵ�ַ�ʽ,�򻯳���
//	*******************************************************************************/
//	unsigned char WriteFlsInChip(unsigned char* buffer, unsigned long addr, unsigned short length, unsigned char x, unsigned char sdmode)
//	{
//	    unsigned char  sidx, eidx;                      //д�����ʼҳ�ͽ���ҳ
//	    unsigned char  fwri;                            //�Ƿ���Ҫд���־
//	    unsigned char  fera;                            //�Ƿ���Ҫ������־
//	    unsigned char  m;
//	    unsigned char  n;
//	    //unsigned long addrInFlash;                     //FLASH�ľ��Ե�ַ
//	    unsigned long addrInSector;                    //������Ե�ַ
//	    unsigned long addrOfSector;                    //
//	    unsigned short ui_len = 0;                      //����SECTOR��д��ĳ���
//	                                            //��ȡ��λ�ò���,�������Ƭ��ȡ
//	    SYS_VAR_CHECK(length + addr > guls_FLSChipSize[x]);
//	    SYS_VAR_CHECK(length == 0);
//	    SYS_VAR_CHECK(sdmode != 0);
//	
//	    /************************************************************************
//	    //ѭ������ÿ����Ҫ���ĵ�SECTOR
//	    ************************************************************************/
//	    while(true)                             //ѭ������ÿ����Ҫ���ĵ�SECTOR
//	    {
//	        if(length + (addr & 0xFFF) > 4096)  //���
//	        {
//	            ui_len = 4096 - (addr & 0xFFF); //���������ݳ���
//	        }
//	        else                                //���ڲ���
//	        {
//	            ui_len = length;                //�������ݳ���
//	        }
//	
//	                                            //��ȡһ������
//	        while(SYS_ERR_OK != ReadFlsInChip(gcp_FlsBuffer, addr & (~0xFFF), 4096, x));
//	        
//	        /************************************************************************
//	        //ѭ���ж��Ƿ����д,��дǰ�Ƿ������
//	        ************************************************************************/
//	        fwri = 0;                           //��λ��־λ
//	        fera = 0;
//	                                            //ѭ���ж��Ƿ����д�����Ƿ���Ҫ����
//	        for(unsigned long k = 0; k < ui_len; k++)
//	        {
//	            unsigned char uc_data = gcp_FlsBuffer[(addr & 0xFFF) + k];
//	            if(uc_data != buffer[k])        //�ж��Ƿ���ԭ������һ��
//	            {
//	                fwri = 0x01;
//	                if((uc_data & buffer[k]) != buffer[k])//(uc_data != 0xFF)         //����ԭ�����ݲ�һ���ֲ���FF����Ҫ����
//	                {
//	                    fera = 0x01;
//	                    break;
//	                }
//	            }
//	        }
//	        
//	        /************************************************************************
//	        //д��
//	        ************************************************************************/
//	        if(fwri)                            //��Ҫд��
//	        {
//	            addrOfSector = addr & (~0xFFF);
//	            addrInSector = addr & 0xFFF;
//	
//	            if(fera)                        //��Ҫ����
//	            {
//	                EraseFlsSectorInChip(addrOfSector, x);
//	                sidx = 0;                   //������Ҫ����д��
//	                eidx = 16;
//	            }
//	            else                            //��Ҫд���ҳ
//	            {
//	                sidx = addrInSector >> 8;
//	                eidx = (addrInSector + ui_len) >> 8;
//	                if((addrInSector + ui_len) & 0xFF)
//	                {
//	                    eidx += 1;
//	                }
//	            }
//	                                            //ת����Ҫд�������
//	            MoveBuffer(buffer, gcp_FlsBuffer + addrInSector, ui_len);
//	                                            // 
//	            for(m = 0; m < 3; m++)          //ѭ��д,���д3��
//	            {
//	                for(n = sidx; n < eidx; n++)//��ҳд��
//	                {
//	                                            
//	                    if(WriteFlsPageInChip(gcp_FlsBuffer + (n << 8), addrOfSector + (n << 8), 256, x) != SYS_ERR_OK)
//	                    {
//	                        EraseFlsSectorInChip(addrOfSector, x);
//	                        sidx = 0;
//	                        eidx = 16;
//	                        break;
//	                    }
//	                }
//	                
//	                if(n >= eidx)               //д��ɹ�,����
//	                {
//	                    break;
//	                }
//	            }
//	            
//	            if(m >= 3)
//	            {
//	                guc_FlashErrStt |= FLS_ERR_WRITE;//��־д�����ݷ�������
//	                return SYS_ERR_FT;          //����д�����ݴ���
//	            }
//	        }
//	
//	        /************************************************************************
//	        //��
//	        ************************************************************************/
//	        length -= ui_len;                   //���������ݳ��ȵݼ�
//	        if(length == 0)                     //�����Ѿ��������
//	        {
//	            SYS_OK();                        //����д��ɹ�
//	        }
//	
//	        buffer += ui_len;                   //��������ָ��ǰ��
//	        addr += ui_len;                     //ʵ�ʲ�����ַ
//	    }
//	}
//	
//	#endif


/*******************************************************************************
** @function_name:  PCB_FLS_Write
** @function_file:  mxflash.c
** @����: Flash д����
** 
** 
** @����: 
** @param: buffer(unsigned char*): д�����ݵ����ݻ���
** @param: address(unsigned long): д�����ݵĵ�ַ
** @param: length(unsigned short): д�����ݵĳ���
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy
** @�޸�˵��: �����ܵؼ����߳��Ĵ���(��:��д��������ԭ������һ�»�ԭ������ΪFFʱ���ٲ���)
*******************************************************************************/
unsigned char FLS_Write(unsigned char* buffer, unsigned long address, unsigned short length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    unsigned char err = 0;
    unsigned char num;
    
//	    DRV_POW_CHECK(PCB_VBatVoltGet() < 2800);    
    if(gul_FlashSafeCode != 0xA5A5A5A5)     //�жϰ�ȫ�������Ƿ��
    {
        return SYS_ERR_FT;                  //������������
    }
    
    num = MultiFlashFilterExt(fs, address, length);
    if(num == 0)
    {
        gul_FlashSafeCode = 0;
        return 2;
    }
    
    for(unsigned char uc_i = 0; uc_i < num; uc_i++)
    {
        err |= WriteFlsInChip(buffer + fs[uc_i].opoffset, fs[uc_i].opaddr, fs[uc_i].oplen, fs[uc_i].x, 0);
    }
    
    gul_FlashSafeCode = 0;
    return err;
}




/*******************************************************************************
** @function_name:  PCB_FLS_WriteSpec
** @function_file:  mxflash.c
** @����: Flash ����д����(���������뱣��ԭ�����ݵ�д����,�ر������ڴ�����������д��)
** 
** 
** @����: 
** @param: buffer(unsigned char*): д�����ݵ����ݻ���
** @param: address(unsigned long): д�����ݵĵ�ַ
** @param: length(unsigned short): д�����ݵĳ���
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2014-03-27)
**-----------------------------------------------------------------------------
** @�޸���: yzy
** @�޸�˵��: 
*******************************************************************************/
unsigned char FLS_WriteSpec(unsigned char* buffer, unsigned long address, unsigned short length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    unsigned char err = 0;
    unsigned char num;
//	    DRV_POW_CHECK(PCB_VBatVoltGet() < 2800);    
    
    if(gul_FlashSafeCode != 0xA5A5A5A5)     //�жϰ�ȫ�������Ƿ��
    {
        return SYS_ERR_FT;                  //������������
    }
    
    num = MultiFlashFilterExt(fs, address, length);
    if(num == 0)
    {
        gul_FlashSafeCode = 0;
        return 2;
    }
    
    for(unsigned char uc_i = 0; uc_i < num; uc_i++)
    {                                       //˵��:��ȷ������д��ʱΪ��������ʱ,������ģʽ2.����ģʽ1������.
        err |= WriteFlsInChipSpec(buffer + fs[uc_i].opoffset, fs[uc_i].opaddr, fs[uc_i].oplen, fs[uc_i].x, 1);
    }
    
    gul_FlashSafeCode = 0;
    return err;
}



//	unsigned char SetFlsInChip(unsigned char templet, unsigned long addr, unsigned short length, unsigned char x)
//	{
//	    unsigned char  sidx, eidx;                      //д�����ʼҳ�ͽ���ҳ
//	    unsigned char  fwri;                            //�Ƿ���Ҫд���־
//	    unsigned char  fera;                            //�Ƿ���Ҫ������־
//	    unsigned char  m;
//	    unsigned char  n;
//	    //unsigned long addrInFlash;                     //FLASH�ľ��Ե�ַ
//	    unsigned long addrInSector;                    //������Ե�ַ
//	    unsigned long addrOfSector;                    //
//	    unsigned short ui_len = 0;                      //����SECTOR��д��ĳ���
//	                                            //��ȡ��λ�ò���,�������Ƭ��ȡ
//	    SYS_VAR_CHECK(length + addr > guls_FLSChipSize[x]);
//	    SYS_VAR_CHECK(length == 0);
//	
//	    /************************************************************************
//	    //ѭ������ÿ����Ҫ���ĵ�SECTOR
//	    ************************************************************************/
//	    while(true)                             //ѭ������ÿ����Ҫ���ĵ�SECTOR
//	    {
//	        if(length + (addr & 0xFFF) > 4096)  //���
//	        {
//	            ui_len = 4096 - (addr & 0xFFF); //���������ݳ���
//	        }
//	        else                                //���ڲ���
//	        {
//	            ui_len = length;                //�������ݳ���
//	        }
//	        
//	                                            //��ȡһ������
//	        while(SYS_ERR_OK != ReadFlsInChip(gcp_FlsBuffer, addr & (~0xFFF), 4096, x));
//	        
//	        /************************************************************************
//	        //ѭ���ж��Ƿ����д,��дǰ�Ƿ������
//	        ************************************************************************/
//	        fwri = 0;                           //��λ��־λ
//	        fera = 0;
//	                                            //ѭ���ж��Ƿ����д�����Ƿ���Ҫ����
//	        for(unsigned long k = 0; k < ui_len; k++)
//	        {
//	            unsigned char uc_data = gcp_FlsBuffer[(addr & 0xFFF) + k];
//	            if(uc_data != templet)          //�ж��Ƿ���ԭ������һ��
//	            {
//	                fwri = 0x01;
//	                if((uc_data & templet) != templet)//(uc_data != 0xFF)         //����ԭ�����ݲ�һ���ֲ���FF����Ҫ����
//	                {
//	                    fera = 0x01;
//	                    break;
//	                }
//	            }
//	        }
//	     
//	        /************************************************************************
//	        //д��
//	        ************************************************************************/
//	        if(fwri)                            //��Ҫд��
//	        {
//	            addrOfSector = addr & (~0xFFF);
//	            addrInSector = addr & 0xFFF;
//	
//	            if(fera)                        //��Ҫ����
//	            {
//	                EraseFlsSectorInChip(addrOfSector, x);
//	                sidx = 0;                   //������Ҫ����д��
//	                eidx = 16;
//	            }
//	            else                            //��Ҫд���ҳ
//	            {
//	                sidx = addrInSector >> 8;
//	                eidx = (addrInSector + ui_len) >> 8;
//	                if((addrInSector + ui_len) & 0xFF)
//	                {
//	                    eidx += 1;
//	                }
//	            }
//	                                                //��λ��Ҫд�������
//	            memset(gcp_FlsBuffer + (addr & 0xFFF), templet, ui_len);
//	            
//	            for(m = 0; m < 3; m++)          //ѭ��д,���д3��
//	            {
//	                for(n = sidx; n < eidx; n++)//��ҳд��
//	                {
//	                    if(WriteFlsPageInChip(gcp_FlsBuffer + (n << 8), addrOfSector + (n << 8), 256, x) != SYS_ERR_OK)
//	                    {
//	                        EraseFlsSectorInChip(addrOfSector, x);
//	                        sidx = 0;
//	                        eidx = 16;
//	                        break;
//	                    }
//	                }
//	                
//	                if(n >= eidx)               //д��ɹ�,����
//	                {
//	                    break;
//	                }
//	            }
//	            
//	            if(m >= 3)
//	            {
//	                guc_FlashErrStt |= FLS_ERR_WRITE;//��־д�����ݷ�������
//	                return SYS_ERR_FT;          //����д�����ݴ���
//	            }
//	        }
//	
//	        /************************************************************************
//	        //��
//	        ************************************************************************/
//	        length -= ui_len;                   //���������ݳ��ȵݼ�
//	        if(length == 0)                     //�����Ѿ��������
//	        {
//	            SYS_OK();                        //����д��ɹ�
//	        }
//	        
//	        addr += ui_len;                     //ʵ�ʲ�����ַ
//	    }
//	}



/*******************************************************************************
** @function_name:  PCB_FLS_Set
** @function_file:  mxflash.c
** @����: Flash ������־
** 
** 
** @����: 
** @param: templet(unsigned char): ���������
** @param: address(unsigned long): д�����ݵĵ�ַ
** @param: length(unsigned short): д�����ݵĳ���
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy
** @�޸�˵��: ��������ͬFLS_Write()
*******************************************************************************/
unsigned char FLS_Set(unsigned char templet, unsigned long address, unsigned short length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    unsigned char err = 0;
    unsigned char num;
    
//	    DRV_POW_CHECK(PCB_VBatVoltGet() < 2800);    
    if(gul_FlashSafeCode != 0xA5A5A5A5)     //�жϰ�ȫ�������Ƿ��
    {
        return SYS_ERR_FT;                  //������������
    }
    
    num = MultiFlashFilterExt(fs, address, length);
    if(num == 0)
    {
        gul_FlashSafeCode = 0;
        return 2;
    }
    
    for(unsigned char uc_i = 0; uc_i < num; uc_i++)
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
** @return:  unsigned char λͼ.1:Ӳ������,0:Ӳ������.
**                 bit0-bit7�ֱ��ʾ��1-8��flash��Ӳ��״̬
** @����: yzy (2018-07-02)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
//	unsigned char TestFlashChip(void)
//	{
//	    unsigned char cmp1[TEST_FLS_BYTES];             //ԭʼ����
//	    unsigned char cmp2[TEST_FLS_BYTES];             //�޸ĺ�����д������
//	    unsigned char cmp3[TEST_FLS_BYTES];             //�޸ĺ�ʵ��д������
//	    unsigned long chip_addr;                        //Ƭ�ڵ�ַ
//	    unsigned char err = 0;
//	    
//	    for(unsigned char k = 0; k < FLS_CHIP_NO; k++)
//	    {
//	        for(unsigned char w = 0; w < 2; w++)        //��������:һ��Ƭͷ,һ��Ƭβ(ȷ��������ȷ)
//	        {
//	            chip_addr = w ? (guls_FLSChipSize[k] - TEST_FLS_BYTES) : 0;
//	                                            //��ȡԭʼ����
//	            if(ReadFlsInChip(cmp1, chip_addr, TEST_FLS_BYTES, k) != SYS_ERR_OK)
//	            {
//	                err |= Bit_Map8[k];
//	                break;
//	            }
//	                                            //�޸�ԭʼ����
//	            for(unsigned char i = 0; i < TEST_FLS_BYTES; i++)
//	            {
//	                cmp2[i] = cmp1[i] + 2;
//	            }
//	                                            //д������д������
//	            if(WriteFlsInChip(cmp2, chip_addr, TEST_FLS_BYTES, k, 0) != SYS_ERR_OK)
//	            {
//	                err |= Bit_Map8[k];
//	                break;
//	            }
//	                                            //��ȡʵ��д������
//	            if(ReadFlsInChip(cmp3, chip_addr, TEST_FLS_BYTES, k) != SYS_ERR_OK)
//	            {
//	                err |= Bit_Map8[k];
//	                break;
//	            }
//	                                            //�ָ�ԭʼֵ
//	            WriteFlsInChip(cmp1, chip_addr, TEST_FLS_BYTES, k, 0);
//	                                            //�ȶ�����ʵ��ֵ
//	            if(memcmp(cmp2, cmp3, TEST_FLS_BYTES) != 0)
//	            {
//	                err |= Bit_Map8[k];
//	                break;
//	            }
//	        }
//	    }
//	
//	    return err;
//	}







/*******************************************************************************
** @function_name:  Fls_ReadID
** @function_file:  mxflash.c
** @����: Flash��ȡID
** 
** 
** @����: 
** @param: buffer(unsigned char*): 
** @param: x(unsigned long): 
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2018-07-02)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
unsigned char Fls_ReadID(unsigned char* buffer, unsigned long x)
{
    gs_FlashSPI[x].command[0] = MX_CMD_RDID;  //��ȡFlash ID����
    gs_FlashSPI[x].data = buffer;
    gs_FlashSPI[x].cmdnum = 1;
    gs_FlashSPI[x].length = 3;
                                            //��ȡ����
    return SPI_Read(&gs_FlashSPI[x]);
}

/*******************************************************************************
** @function_name:  PCB_Flash_Init
** @function_file:  mxflash.c
** @����: ��ʼ��Flash
** 
** @����: 
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2018-07-02)
**-----------------------------------------------------------------------------
** @�޸���: 
** @�޸�˵��: 
*******************************************************************************/
unsigned char Flash_Init(void)
{
    //gcp_FlsBuffer = (unsigned char*)SYS_GetMEM(LEN_FLS_SECTOR);
    //int ret = 0;
    guc_FlashErrStt = 0;                    //��ʼ������״̬��
    
//	    DRV_POW_CHECK(PCB_VBatVoltGet() < 2800);    
    extern  const SPIIO_PORTS gs_FlashSpiPort;

    Init_SPI(&gs_FlashSpiPort);
    
    FLS_OpenPwr();

    
    FLS_KillHwWp();

    for(unsigned long x = 0; x < FLS_CHIP_NO; x++)
    {
                                            //����cs�����߲����ĺ���
        gs_FlashSPI[x].csup = gs_FlashGetStt[x].csup;
                                            //����cs���õͲ����ĺ���
        gs_FlashSPI[x].csdown = gs_FlashGetStt[x].csdown;
        
        FLS_ExitDP();

        unsigned char buffer[3];
        memset(buffer,0,3);
        Fls_ReadID(buffer, x);              //��ȡFlash ID
//	        printf("line[%d]:buffer[0] = 0x%x ret = %d\n",__LINE__,buffer[0], ret);
//	        printf("line[%d]:buffer[1] = 0x%x\n",__LINE__,buffer[1]);
//	        printf("line[%d]:buffer[2] = 0x%x\n",__LINE__,buffer[2]);
        
                                            //�ж��ͺ�
        if((buffer[0] != FLS_TYPE_MX) && (buffer[0] != FLS_TYPE_EON))    
        {
            Fls_ReadID(buffer, x);          //�ض�һ��
            
//	            printf("line[%d]:buffer[0] = 0x%x ret = %d\n",__LINE__,buffer[0], ret);
//	            printf("line[%d]:buffer[1] = 0x%x\n",__LINE__,buffer[1]);
//	            printf("line[%d]:buffer[2] = 0x%x\n",__LINE__,buffer[2]);
                                            //�ж�״̬
            if((buffer[0] != FLS_TYPE_MX) && (buffer[0] != FLS_TYPE_EON))
            {                               //��������,Flash������
                guc_FlashErrStt = FLS_ERR_EXSIT;
            }
        }
        
        FLS_Disable_WP(x);
        //FLS_FREE(x, true);
        while(!FLS_FREE(x, true));          //�ȴ�Flash����
        
        if(guls_FLSChipSize[x] >= 0x2000000)     //>=32M
        {
            //FLS_Enable4B(x);                    //����ʹ��4�ֽ�ָ��
            FLS_GetSecu(x);
            if(!(guc_FlashSecu[x] & MX_CMD_4BYTEADDR_BIT))
            {
                FLS_Enable4B(x);                //����ʹ��4�ֽ�ָ��
                //return 2;
            }            
        }
        
    }
    
    if(guc_FlashErrStt)
    {
        return SYS_ERR_FT;                  //���ط�����������
    }
    else
    {
        SYS_OK();                            //���س�ʼ���ɹ�
    }
}


void Flash_Close(void)
{
    FLS_ClosePwr();

}
#if 1
const unsigned char flash_test_data[] =
{
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
    0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF,
};
//	const unsigned char flash_test_data[] =
//	{
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	    0xFF,0xEE,0xDD,0xCC,0xBB,0xAA,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
//	};
unsigned char Flash_Test(void)
{
    unsigned char tmp[4096];
    unsigned char ret = 1;
    int i=0,j = 0;
    

    FLS_Erase(0, 256);

    for(i = 0; i < 2048; i++ )
    {
        memset(tmp,0xff,4096);
        FLS_PreWrite();
        ret = FLS_Write((unsigned char *)flash_test_data, 512 * i, 512);

        if(ret != 0)
        {
            break;
        }
        else
        {
            memset(tmp,0xff,4096);
            ret = FLS_Read(tmp, (512 * i) & (~0xFFF), 4096);
            
            if(ret != 0)
            {
                break;
            }
            for( j = 0; j < (512 * (i % 8 + 1)); j++)
            {
                if(tmp[j] != 0xFF && tmp[j] != flash_test_data[j%512])
                {
                    ret = 0xDD;
                    break;
                }
            }
            if(j != (512 * (i % 8 + 1)))
            {
                break;
            }
        }
    }

    return ret;
}

#endif

