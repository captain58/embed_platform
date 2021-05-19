/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: drv_flash.c
**创   建   人: yzy
**最后修改日期: 2015年3月28日
**描        述: mxic Flash驱动程序:2IO标准SPI口,兼容3字节地址(<32MB),以及4字节地址(>=32MB)芯片MX25L256
**              根据宏配置来区分容量(不根据读取寄存器 memory density) 
**              
**注        意:     141:   1：1片flash；4：4 Mbytes；1：1Kbytes缓存
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2018年6月15日
** 描　述: 原始版本
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
**Flash芯片类型
******************************************************************************/
#define  FLS_TYPE_MX         0xC2
#define  FLS_TYPE_EON        0x1C

/******************************************************************************
**sector 长度
******************************************************************************/
#define  LEN_FLS_SECTOR  4096        //sector 长度
#ifdef MCURS
#define  BIT_FLS_BUFSFT  2           //flash驱动缓存右移位,范围0-4,分别对应内存消耗4096,2048,1024,512,256.//最少不得少于单页长度256字节
#else
#define  BIT_FLS_BUFSFT  0 
#endif

#define  LEN_FLS_BUF  (LEN_FLS_SECTOR >> BIT_FLS_BUFSFT) //分配的buflen

#define  NUM_FLS_SECTBUF (1 << BIT_FLS_BUFSFT)           //单个sector对应的buflen数量
#define  MASK_FLS_BUFLEN (LEN_FLS_BUF - 1)               //buflen的掩码.比如,2048掩码为0x7FF



/******************************************************************************
**定义Flash操作命令字
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
#define     MX_CMD_RDSCUR   0x15//mx25L25645GM2I 驱动 0x2B        //Read 安全寄存器   
#define     MX_CMD_4BYTEADDR_BIT 0x20

#define     MX_CMD_EN4B     0xB7        //Enable 4-Byte address mode(用于大于16M)
#define     MX_CMD_EX4B     0xE9        //Exit 4-Byte mode


/******************************************************************************
**Flash,铁电SPI接口
******************************************************************************/
#define SPI_DEV_ID_FLSA         0
#define SPI_DEV_ID_FLSB         1
#define SPI_DEV_ID_FRAM         2

//	#if (SYS_FILE_EN > 0)
#define _IF_TRUE_RETURN_Z(X,Z)  {if(X){return (Z);}}


/******************************************************************************
**定义FLASH Buffer
******************************************************************************/
//	#pragma location = ".mmm"
unsigned char gcp_FlsBuffer[LEN_FLS_BUF];



/*****************************************************************************
**定义FLASH芯片数量
*****************************************************************************/
#if FLS_CHIP_NO < 1
#warning "没有Flash芯片定义,不需要包含本C文件!"
#endif

//	const uint8_t Bit_Map8[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

/*******************************************************************************
**将CS线拉高的函数格式
********************************************************************************/
void FLS_csupA(void)
{
    SPI_CS_1(SPI_DEV_ID_FLSA);
}


/*******************************************************************************
**将CS线拉低的函数格式
********************************************************************************/
void FLS_csdownA(void)
{
    SPI_CS_0(SPI_DEV_ID_FLSA);
}


#if FLS_CHIP_NO > 1
/*******************************************************************************
**将CS线拉高的函数格式
********************************************************************************/
void FLS_csupB(void)
{
    SPI_CS_1(SPI_DEV_ID_FLSB);
}
/*******************************************************************************
**将CS线拉低的函数格式
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
**定义普通Flash 操作SPI的命令的结构体
********************************************************************************/
SPIIO gs_FlashSPI[FLS_CHIP_NO];

/*******************************************************************************
**从Flash芯片中读取的Flash状态
********************************************************************************/
unsigned char guc_FlashStatus[FLS_CHIP_NO];


/*******************************************************************************
**从Flash芯片中读安全寄存器的值，用于判断在三字节还是四字节状态
********************************************************************************/
unsigned char guc_FlashSecu[FLS_CHIP_NO];


/*******************************************************************************
**Flash操作安全状态字
********************************************************************************/
unsigned long gul_FlashSafeCode;

/*******************************************************************************
**定义Flash错误状态字
**bit0 flash是否存在
**bit1 是否有发生读数据错误
**bit2 是否有发生写数据错误
********************************************************************************/
unsigned char guc_FlashErrStt;


/*******************************************************************************
**定义读取Flash状态的结构体
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
**定义写入Flash状态字的结构体
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
**FLash写使能
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
**FLash写禁能
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
**定义打开写保护操作的结构体
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
**定义禁止写保护操作的结构体
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
**定义禁能4字节指令(>=32M)
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
**定义使能4字节指令(>=32M)
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
**定义读取安全寄存器(>=32M)
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
**获取FLASH状态
******************************************************************************/
#define FLS_GetStatus(X) (SPI_Read((SPIIO*)&gs_FlashGetStt[X]))

#define FLS_WriteStatus(x) {SPI_Write((SPIIO*)&gs_FlsWriteStatus[x]);}

/******************************************************************************
**写使能
******************************************************************************/
#define FLS_EnableWrite(x) {SPI_Write((SPIIO*)&gs_FlsEnableWrite[x]);}

/******************************************************************************
**写禁能
******************************************************************************/
#define FLS_DisableWrite(x) {SPI_Write((SPIIO*)&gs_FlsDisableWrite[x]);}





/******************************************************************************
**允许操作四字节地址(>=32M)
******************************************************************************/
#define FLS_Enable4B(x) {SPI_Write((SPIIO*)&gs_FlashEnable4B[x]);}


/******************************************************************************
**退出操作四字节地址状态(>=32M)
******************************************************************************/
#define FLS_Disable4B(x) {SPI_Write((SPIIO*)&gs_FlashDisable4B[x]);}


/******************************************************************************
**读取安全寄存器的值(>=32M)
******************************************************************************/
#define FLS_GetSecu(x) {SPI_Read((SPIIO*)&gs_FlashGetSecu[x]);}
/************************************************************************
 * @function: SPI_Write
 * @描述: 写操作
 * 
 * @参数: 
 * @param: spi 写入数据的地址的结构信息** 
 * 
 * @返回: 
 * @return: unsigned char  
 * @说明: 
 * @作者: yzy (2018-07-02)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
//	unsigned char SPI_Write(SPIIO* spi);
//	
//	/************************************************************************
//	 * @function: SPI_Read
//	 * @描述: 读操作
//	 * 
//	 * @参数: 
//	 * @param: spi 读取数据的地址的结构信息** 
//	 * 
//	 * @返回: 
//	 * @return: unsigned char  
//	 * @说明: 
//	 * @作者: yzy (2018-07-02)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	unsigned char SPI_Read(SPIIO* spi);
//	
//	/************************************************************************
//	 * @function: SPI_ReadAndCompare
//	 * @描述: 从器件中读取指定长度的数据和缓存里面的内容进行比较
//	 * 
//	 * @参数: 
//	 * @param: spi 读取数据的地址的结构信息**  
//	 * 
//	 * @返回: 
//	 * @return: unsigned char  
//	 * @说明: 
//	 * @作者: yzy (2018-07-02)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	unsigned char SPI_ReadAndCompare(SPIIO* spi);
//	
//	
//	/************************************************************************
//	 * @function: SPI_Close
//	 * @描述: SPI进入低功耗
//	 * 
//	 * @参数: 
//	 * @param: devidx 设备索引(0-N)
//	 * 
//	 * @返回: 
//	 * @return: 
//	 * @说明: 
//	 * @作者: yzy (2018-07-02)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	void SPI_Close(unsigned char devidx);
//	
//	/************************************************************************
//	 * @function: SPI_Write
//	 * @描述: 写操作
//	 * 
//	 * @参数: 
//	 * @param: spi 写入数据的地址的结构信息** 
//	 * 
//	 * @返回: 
//	 * @return: unsigned char  
//	 * @说明: 
//	 * @作者: yzy (2018-07-02)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	unsigned char SPI_Write(SPIIO* spi)
//	{
//	    unsigned short ui_i;                         //循环变量
//	    SYS_VAR_CHECK(spi->cmdnum > 8);
//	    _SPI_SCK_Down();//SPI_SCK_0();
//	    spi->csdown();                      //cs线被拉低
//	                                        //循环发送指令和器件内部地址
//	    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
//	    {                                   //发送指令和地址
//	        _SPI_SendByte(spi->command[ui_i]);
//	    }
//	                                        //循环逐个接收字节
//	    for(ui_i = 0; ui_i < spi->length; ui_i ++)
//	    {                                        
//	        _SPI_SendByte(spi->data[ui_i]); //逐个接收字节        
//	    }
//	    spi->csup();                        //将CS线拉高
//	    SYS_OK();
//	}
//	
//	
//	
//	/************************************************************************
//	 * @function: SPI_Read
//	 * @描述: 读操作
//	 * 
//	 * @参数: 
//	 * @param: spi 读取数据的地址的结构信息** 
//	 * 
//	 * @返回: 
//	 * @return: unsigned char  
//	 * @说明: 
//	 * @作者: yzy (2018-07-02)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	unsigned char SPI_Read(SPIIO* spi)
//	{
//	    unsigned short ui_i;                         //循环变量
//	    SYS_VAR_CHECK(spi->cmdnum > 8);
//	    _SPI_SCK_Down();//SPI_SCK_0();
//	    spi->csdown();                      //cs线被拉低
//	                                        //循环发送指令和器件内部地址
//	    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
//	    {                                   //发送指令和地址
//	        _SPI_SendByte(spi->command[ui_i]);
//	    }
//	                                        //循环逐个接收字节
//	    for(ui_i = 0; ui_i < spi->length; ui_i ++)
//	    {
//	                                        //逐个接收字节
//	        spi->data[ui_i] = _SPI_ReceiveByte();   
//	    }
//	    spi->csup();                        //将CS线拉高
//	    
//	    
//	    return SPI_ReadAndCompare(spi);     //返回读出比较的结果
//	}
//	
//	
//	/************************************************************************
//	 * @function: SPI_ReadAndCompare
//	 * @描述: 从器件中读取指定长度的数据和缓存里面的内容进行比较
//	 * 
//	 * @参数: 
//	 * @param: spi 读取数据的地址的结构信息**  
//	 * 
//	 * @返回: 
//	 * @return: unsigned char  
//	 * @说明: 
//	 * @作者: yzy (2018-07-02)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
//	 ************************************************************************/
//	unsigned char SPI_ReadAndCompare(SPIIO* spi)
//	{
//	    unsigned short ui_i;
//	    SYS_VAR_CHECK(spi->cmdnum > 8);    
//	    _SPI_SCK_Down();//SPI_SCK_0();
//	    spi->csdown();                      //cs线被拉低
//	    
//	                                        //循环发送指令和器件内部地址
//	    //printf("_SPI_SendByte command =");
//	    for(ui_i = 0; ui_i < spi->cmdnum; ui_i ++)
//	    {                                   //发送指令和地址
//	        _SPI_SendByte(spi->command[ui_i]);
//	        //printf(" %x", spi->command[ui_i]);
//	    }   
//	    //printf("\n");
//	                                        //循环逐个接收字节
//	    for(ui_i = 0; ui_i < spi->length; ui_i ++)
//	    {
//	//        unsigned char tmp = _SPI_ReceiveByte();
//	        
//	                                        //逐个接收字节
//	        if(spi->data[ui_i] != _SPI_ReceiveByte())
//	//        if(spi->data[ui_i] != tmp)
//	        {
//	//            printf("i = %d data = %x, tmp = %x\n", ui_i, spi->data[ui_i], tmp);
//	            spi->csup();                //将CS线拉高
//	            
//	            return 2;
//	        }
//	    }
//	    
//	    spi->csup();                        //将CS线拉高
//	    SYS_OK();
//	}
//	
//	
//	/************************************************************************
//	 * @function: SPI_Close
//	 * @描述: SPI进入低功耗
//	 * 
//	 * @参数: 
//	 * @param: devidx 设备索引(0-N)
//	 * 
//	 * @返回: 
//	 * @return: 
//	 * @说明: 
//	 * @作者: yzy (2018-07-02)
//	 *-----------------------------------------------------------------------
//	 * @修改人: 
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
**关闭保护管脚
******************************************************************************/
void FLS_KillHwWp(void)
{
    SPI_WP_1(SPI_DEV_ID_FLSA);
#if FLS_CHIP_NO > 1
    SPI_WP_1(SPI_DEV_ID_FLSB);
#endif
}

/******************************************************************************
**打开供电管脚
******************************************************************************/
void FLS_OpenPwr(void)
{
    SPI_PWR_0(SPI_DEV_ID_FLSA);
#if FLS_CHIP_NO > 1
    SPI_PWR_0(SPI_DEV_ID_FLSB);
#endif
}

/******************************************************************************
**关闭供电管脚
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
**屏蔽Flash写保护
******************************************************************************/
void FLS_Disable_WP(unsigned long X)
{
    FLS_EnableWrite(X);
    SPI_Write((SPIIO*)&gs_FlashDisableWP[X]);
}

/******************************************************************************
**写保护
******************************************************************************/
//	void FLS_Enable_WP(unsigned long X)
//	{
//	    FLS_EnableWrite(X);
//	    SPI_Write((SPIIO*)&gs_FlashEnableWP[X]);
//	}

/******************************************************************************
**Flash进入以及退出低功耗模式
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
 * @描述:判断存储器当前是否空闲
 * 
 * 
 * @参数: 
 * @param:x  存储器序号
 * @param:imd  是否需要快速判断
 * 
 * @返回: 
 * @return: bool true 空闲 false 不空闲
 * @作者:
 *---------------------------------------------------------
 * @修改人: houpm (2009-8-8)
 **********************************************************/
bool FLS_FREE(unsigned long x, bool imd)
{
    int ret = FLS_GetStatus(x);                   //更新Flash的状态
//	    printf("line[%d]:guc_FlashStatus[x] = 0x%x ret = %d\n",__LINE__,guc_FlashStatus[x], ret);

    if(!(guc_FlashStatus[x] & 0x01))    //判断Flash的状态
    {
        FLS_GetStatus(x);               //更新Flash的状态
//	        printf("line[%d]:guc_FlashStatus[x] = 0x%x ret = %d\n",__LINE__,guc_FlashStatus[x], ret);
        if(!(guc_FlashStatus[x] & 0x01))//再次判断,防止判断错误
        {
            return true;                //空闲就马上返回
        }
    }
    for(unsigned short dly = 100; dly > 0; dly--);    
//	    if(imd)                             //判断是否需要快速返回的
//	    {
//	                                        //延迟100个循环
//	        for(unsigned short dly = 100; dly > 0; dly--);
//	    }
//	    else
//	    {
//	        sleep_ms(1);                 //延迟10ms
//	    }
    return false;                       //返回不空闲
}

/*******************************************************************************
** @function_name:  EraseFlsSectorInChip
** @function_file:  mxflash.c
** @描述: MX Flash 段檫除操作
** 
** 
** @参数: 
** @param: address(unsigned long): 
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char EraseFlsSectorInChip(unsigned long address, unsigned char x)
{
    unsigned char err;
                                            //地址参数检验
    SYS_VAR_CHECK(address >= guls_FLSChipSize[x]);
    
    address &= ~0xFFF;                      //块起始地址

    FLS_EnableWrite(x);                     //写使能

    if(guls_FLSChipSize[x] < 0x2000000)     //<32M
    {
        gs_FlashSPI[x].command[0] = MX_CMD_SE;  //块檫除指令
                                                //块地址
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
            FLS_Enable4B(x);                //开启使用4字节指令
            return 2;
        }
        
        gs_FlashSPI[x].command[0] = MX_CMD_SE;  //块檫除指令
                                                //块地址
        gs_FlashSPI[x].command[1] = address >> 24;
        gs_FlashSPI[x].command[2] = address >> 16;
        gs_FlashSPI[x].command[3] = address >> 8;
        gs_FlashSPI[x].command[4] = address;

        gs_FlashSPI[x].data = NULL;
        gs_FlashSPI[x].cmdnum = 5;
        gs_FlashSPI[x].length = 0;
    }
                                            //发送檫除命令
    err = SPI_Write(&gs_FlashSPI[x]);

    while(!FLS_FREE(x, true));              //等待它的空闲

    //FLS_DisableWrite(x);                    //写禁能

    return err;
}



/*******************************************************************************
** @function_name:  EraseFlsBlockInChip
** @function_file:  mxflash.c
** @描述: MX Flash 块檫除操作
** 
** 
** @参数: 
** @param: address(unsigned long): 
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char EraseFlsBlockInChip(unsigned long address, unsigned char x)
{
    unsigned char err;
                                            //地址参数检验
    SYS_VAR_CHECK(address >= guls_FLSChipSize[x]);
    
    address &= ~0xFFF;                      //块起始地址

    FLS_EnableWrite(x);                     //写使能

    if(guls_FLSChipSize[x] < 0x2000000)     //<32M
    {
        gs_FlashSPI[x].command[0] = MX_CMD_BE;  //块檫除指令
                                                //块地址
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
            FLS_Enable4B(x);                //开启使用4字节指令
            return 2;
        }
        
        gs_FlashSPI[x].command[0] = MX_CMD_BE;  //块檫除指令
                                                //块地址
        gs_FlashSPI[x].command[1] = address >> 24;
        gs_FlashSPI[x].command[2] = address >> 16;
        gs_FlashSPI[x].command[3] = address >> 8;
        gs_FlashSPI[x].command[4] = address;

        gs_FlashSPI[x].data = NULL;
        gs_FlashSPI[x].cmdnum = 5;
        gs_FlashSPI[x].length = 0;
    }
                                            //发送檫除命令
    err = SPI_Write(&gs_FlashSPI[x]);

    while(!FLS_FREE(x, true));              //等待它的空闲

    //FLS_DisableWrite(x);                    //写禁能

    return err;
}




/*******************************************************************************
** @function_name:  ReadFlsInChip
** @function_file:  mxflash.c
** @描述: 从Flash指定地址读取指定长度的字节数据
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 数据缓存
** @param: address(unsigned long): 地址
** @param: length(unsigned short): 读取的数据长度
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char ReadFlsInChip(unsigned char* buffer, unsigned long address, unsigned short length, unsigned char x)
{
                                        //读取的位置不对,不能跨片读取
    SYS_VAR_CHECK(length + address > guls_FLSChipSize[x]);
                                        //2xIO 读取数据指令
    if(guls_FLSChipSize[x] < 0x2000000)     //<32M
    {
        gs_FlashSPI[x].command[0] = MX_CMD_READ;
                                            //读取地址
        gs_FlashSPI[x].command[1] = address >> 16;
        gs_FlashSPI[x].command[2] = address >> 8;
        gs_FlashSPI[x].command[3] = address;

        gs_FlashSPI[x].data = buffer;       //数据缓存
        gs_FlashSPI[x].length = length;     //读取长度
        gs_FlashSPI[x].cmdnum = 4;          //命令长度
    }
    else                                    //>=32M
    {
        FLS_GetSecu(x);
        if(!(guc_FlashSecu[x] & MX_CMD_4BYTEADDR_BIT))
        {
            FLS_Enable4B(x);                //开启使用4字节指令
            return 2;
        }
        
        gs_FlashSPI[x].command[0] = MX_CMD_READ;
                                            //读取地址
        gs_FlashSPI[x].command[1] = address >> 24;
        gs_FlashSPI[x].command[2] = address >> 16;
        gs_FlashSPI[x].command[3] = address >> 8;
        gs_FlashSPI[x].command[4] = address;

        gs_FlashSPI[x].data = buffer;       //数据缓存
        gs_FlashSPI[x].length = length;     //读取长度
        gs_FlashSPI[x].cmdnum = 5;          //命令长度
    }

    return SPI_Read(&gs_FlashSPI[x]);
}





/*******************************************************************************
** @function_name:  WriteFlsPageInChip
** @function_file:  mxflash.c
** @描述: Flash页编程
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 数据缓存
** @param: address(unsigned long): 片内地址
** @param: length(unsigned short): 写入数据的长度
** @param: x(unsigned long): 
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char WriteFlsPageInChip(unsigned char* buffer, unsigned long address, unsigned short length, unsigned char x)
{
                                            //判断参数的正确性
    SYS_VAR_CHECK(length + (address & 0xFF) > 256);

    if(length == 0)                         //如果写入的数据长度为0 
    {
        SYS_OK();                            //直接返回OK
    }
    
    FLS_EnableWrite(x);
    
    if(guls_FLSChipSize[x] < 0x2000000)     //<32M
    {
                                                //page program
        gs_FlashSPI[x].command[0] = MX_CMD_PP;     
                                                //地址高字节
        gs_FlashSPI[x].command[1] = address >> 16; 
                                                //地址中字节
        gs_FlashSPI[x].command[2] = address >> 8;
        gs_FlashSPI[x].command[3] = address;          //地址低字节
                                         
        gs_FlashSPI[x].data = buffer;           //缓存数据
        gs_FlashSPI[x].cmdnum = 4;              //命令长度
        gs_FlashSPI[x].length = length;         //数据长度
        
        SPI_Write(&gs_FlashSPI[x]);             //执行命令
        
        while(!FLS_FREE(x, true));              //等待它的空闲
        
                                                //读出比较数据
        gs_FlashSPI[x].command[0] = MX_CMD_READ;
        gs_FlashSPI[x].cmdnum = 4;              //4个字节的命令
    }
    else
    {
        FLS_GetSecu(x);
        if(!(guc_FlashSecu[x] & MX_CMD_4BYTEADDR_BIT))
        {
            FLS_Enable4B(x);                    //开启使用4字节指令
            return 2;
        }
        
        gs_FlashSPI[x].command[0] = MX_CMD_PP;     
                                                //地址
        gs_FlashSPI[x].command[1] = address >> 24;
        gs_FlashSPI[x].command[2] = address >> 16;
        gs_FlashSPI[x].command[3] = address >> 8;
        gs_FlashSPI[x].command[4] = address;
                                         
        gs_FlashSPI[x].data = buffer;           //缓存数据
        gs_FlashSPI[x].cmdnum = 5;              //命令长度
        gs_FlashSPI[x].length = length;         //数据长度
        
        SPI_Write(&gs_FlashSPI[x]);             //执行命令
        while(!FLS_FREE(x, true));              //等待它的空闲
        
                                                //读出比较数据
        gs_FlashSPI[x].command[0] = MX_CMD_READ;
        gs_FlashSPI[x].cmdnum = 5;              //5个字节的命令
    }
                                            //写入数据
    return SPI_ReadAndCompare(&gs_FlashSPI[x]);
}





//**********************************************************************************
//***********************简单函数**********************
//**********************************************************************************






/*******************************************************************************
** @function_name:  MultiFlashFilterExt
** @function_file:  mxflash.c
** @描述: 输出多芯片文件系统要操作的芯片参数
** 
** 
** @参数: 
** @param: s_out: 数据输出
** @param: address(unsigned long): 操作地址
** @param: length(unsigned short): 操作数据的长度
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned long MultiFlashFilterExt(S_FLSINCHIP* s_out, unsigned long address, unsigned long length)
{
    unsigned long index = 0;                   //涉及到的存储芯片序号
    unsigned long s1 = 0;                      //某片FLASH对应的起始逻辑地址
    unsigned long s2 = 0;                      //某片FLASH对应的终止逻辑地址(不含)
    unsigned long back = address;
    unsigned char sidx = 0;
    S_FLSINCHIP* fs;                    //
    
    _IF_TRUE_RETURN_Z(length == 0, 0);  //参数合法性判断
                                        //查找要操作的首个芯片
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
                                        //未找到,退出
    if(index == FLS_CHIP_NO)
    {
        return 0;
    }
                                        //输出每个芯片的操作参数
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
                                        //更新变量(1)
        address = s2;
        length -= fs->oplen;
        sidx++;
                                        //准备下个芯片(2)
        s1 += guls_FLSChipSize[index];
        s2 += guls_FLSChipSize[index];
    }
    
    return sidx;
    
}








/*******************************************************************************
** @function_name:  FLS_SectorErase
** @function_file:  mxflash.c
** @描述: MX Flash 段4K檫除操作
** 
** 
** @参数: 
** @param: address(unsigned long): 
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char FLS_SectorErase(unsigned long address)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    unsigned char num;
                                                //查找存储芯片与片内地址
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
** @描述: MX Flash 块64K檫除操作
** 
** 
** @参数: 
** @param: address(unsigned long): 
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char FLS_BlockErase(unsigned long address)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    unsigned char num;
                                                //查找存储芯片与片内地址
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
** @描述: MX Flash 段空白判断
** 
** 
** @参数: 
** @param: address(unsigned long)
** 
** @返回: 
** @return:  bool true:全空,false:非空
** @作者: yzy (2012-06-09)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
//	bool FLS_SectorBlankCheck(unsigned long address)
//	{
//	    address &= ~0xFFF;                      //块起始地址
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
** @描述: MX Flash 页空白判断
** 
** 
** @参数: 
** @param: address(unsigned long)
** 
** @返回: 
** @return:  bool true:全空,false:非空
** @作者: yzy (2012-06-09)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
//	bool FLS_PageBlankCheck(unsigned long address)
//	{
//	    address &= ~0xFF;                      //页起始地址
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
** @描述: Flash页编程
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 
** @param: address(unsigned long): 
** @param: length(unsigned short): 
** @param: x(unsigned long): 
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char FLS_WritePage(unsigned char* buffer, unsigned long address, unsigned short length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    unsigned char num;
                                                //查找存储芯片与片内地址
    num = MultiFlashFilterExt(fs, address, 1);
    if(num == 0)
    {
        return 2;
    }
    
    return WriteFlsPageInChip(buffer, fs[0].opaddr, length, fs[0].x);
}






//**********************************************************************************
//***********************复杂函数**********************
//**********************************************************************************











/*******************************************************************************
** @function_name:  PCB_FLS_Erase
** @function_file:  mxflash.c
** @描述: MX Flash 檫除操作
** 
** 
** @参数: 
** @param: address: 起始地址
** @param: sectornum 扇区数量
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
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
                                        //循环擦除
    while(ul_i < sectornum)
    {
        //                              //喂狗:避免执行该函数时,喂狗进程等待资源时间过长,导致复位
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
                                        //循环擦除
    while(ul_i < sectornum)
    {
        //                              //喂狗:避免执行该函数时,喂狗进程等待资源时间过长,导致复位
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
** @描述: 从Flash指定地址读取指定长度的字节数据
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 数据缓存
** @param: address(unsigned long): 地址
** @param: length(unsigned short): 读取的数据长度
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
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
 * @描述: 预写操作,在写数据之前必须要进行预写操作
 * @参数: 
 * 
 * @返回: 
 * @return: unsigned char  
 * @说明: 
 * @作者: yzy (2014/6/18)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
unsigned char FLS_PreWrite(void)
{
    gul_FlashSafeCode = 0xA5A5A5A5;
    SYS_OK();
}

//	#ifdef CHIP_R7F0C00X
#if (BIT_FLS_BUFSFT > 0)


/************************************************************************ 
 *函数返回结果
 ************************************************************************/
#define FLS_WRSTT_WRI   0x01                //是否需要写入
#define FLS_WRSTT_ERA   0x02                //是否需要擦除
#define FLS_WRSTT_SIS   0x04                //是否存在主存区与备份区一致性错误

/*******************************************************************************
** @function_name:  CompareFlsInChip
** @function_file:  mxflash.c
** @描述: 从Flash指定地址读取指定长度的字节数据,并与传入数据比较是否相等
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 数据缓存
** @param: address(unsigned long): 地址
** @param: length(unsigned short): 读取的数据长度
** 
** @返回: 
** @return:  unsigned char   SYS_ERR_OK相等,2不相等
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char CompareFlsInChip(unsigned char* buffer, unsigned long address, unsigned short length, unsigned char x)
{
                                            //读取的位置不对,不能跨片读取
    SYS_VAR_CHECK(length + address > guls_FLSChipSize[x]);
    
    if(guls_FLSChipSize[x] < 0x2000000)     //<32M
    {
                                            //2xIO 读取数据指令
        gs_FlashSPI[x].command[0] = MX_CMD_READ;
                                            //读取地址
        gs_FlashSPI[x].command[1] = address >> 16;
        gs_FlashSPI[x].command[2] = address >> 8;
        gs_FlashSPI[x].command[3] = address;

        gs_FlashSPI[x].data = buffer;       //数据缓存
        gs_FlashSPI[x].length = length;     //读取长度
        gs_FlashSPI[x].cmdnum = 4;          //命令长度
    }
    else                                    //>=32M
    {
        FLS_GetSecu(x);
        if(!(guc_FlashSecu[x] & MX_CMD_4BYTEADDR_BIT))
        {
            FLS_Enable4B(x);                //开启使用4字节指令
            return 2;
        }
        
        gs_FlashSPI[x].command[0] = MX_CMD_READ;
                                            //读取地址
        gs_FlashSPI[x].command[1] = address >> 24;
        gs_FlashSPI[x].command[2] = address >> 16;
        gs_FlashSPI[x].command[3] = address >> 8;
        gs_FlashSPI[x].command[4] = address;

        gs_FlashSPI[x].data = buffer;       //数据缓存
        gs_FlashSPI[x].length = length;     //读取长度
        gs_FlashSPI[x].cmdnum = 5;          //命令长度
    }

    return SPI_ReadAndCompare(&gs_FlashSPI[x]);
}

/************************************************************************
 * @function: SectorVerifyForWrite
 * @描述:    1)确保备份区与原始区数据一致性
 *           2)循环判断是否需改写,改写前是否需擦除
 * 
 * @参数:
 * @param: buffer 判断的缓存指针
 * @param: templet 判断的字符模板
 * @param: addr 待操作的flash的地址
 * @param: length 单个SECTOR中操作的长度
 * @param: x 第几片flash
 * 
 * @返回: 
 * @return: unsigned char  位图结果:FLS_WRSTT_WRI/FLS_WRSTT_ERA/FLS_WRSTT_SIS
 * @说明: 
 * @作者: yzy (2014/10/21)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
unsigned char SectorVerifyForWrite(unsigned char* buffer, unsigned char templet, unsigned long addr, unsigned short length, unsigned char x)
{
    unsigned long addrInFlash;                     //FLASH的绝对地址
    unsigned long addrInSector;                    //段内相对地址
    unsigned long addrOfSector;                    //
    unsigned short bufferIdx = 0;
    unsigned short flsIdx = 0;
    unsigned short compLen;                     //单次比较的数据长度
    unsigned char  result = 0;
    unsigned char  m;
    unsigned char  n;
    
    addrInSector = addr & 0xFFF;
    addrOfSector = addr & (~0xFFF);
    addrInFlash = addrOfSector;         //首先指向SECTOR头部
    
    for(m = 0; m < NUM_FLS_SECTBUF; m++)//这里就循环x次(SECTOR = x * LEN_FLS_BUF)
    {
        for(n = 0; n < 2; n++)          //多次比对.当首次比对错误时,再次比对.
        {
            ReadFlsInChip(gcp_FlsBuffer, addrInFlash, LEN_FLS_BUF, x);
//	            printf("line[%d]:gcp_FlsBuffer[0]=0x%x\n", __LINE__, gcp_FlsBuffer[0]);
            if(CompareFlsInChip(gcp_FlsBuffer, (guls_FLSChipSize[x] >> 1) + addrInFlash, LEN_FLS_BUF, x) == SYS_ERR_OK)
            {
                break;                  //正确时跳出循环
            }
        }
        
        if(n == 2)                      //错误.标记为主从区一致性错误.consistency error
        {
            result |= FLS_WRSTT_SIS;
        }
        addrInFlash += LEN_FLS_BUF;
        
                                        //判断是否需改写,改写前是否需擦除
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
                                        //将待写入数据与原有数据进行比较
                for(flsIdx = 0; flsIdx < compLen; flsIdx++)
                {
                    if(buffer == NULL)//字符模板比对
                    {
                        templet = templet;
                    }
                    else                //全缓存逐一比对
                    {
                        templet = buffer[bufferIdx];
                        bufferIdx++;    //next byte
                    }
                                        //判断是否与原有数据一致
                    if(gcp_FlsBuffer[(addrInSector & MASK_FLS_BUFLEN) + flsIdx] != templet)
                    {
                        result |= FLS_WRSTT_WRI;
                        
//	                        printf("line[%d]:addrInSector = 0x%x\n", __LINE__, addrInSector);
//	                        printf("line[%d]:gcp_FlsBuffer[%d] = 0x%x  templet = 0x%x\n", __LINE__, flsIdx, gcp_FlsBuffer[(addrInSector & MASK_FLS_BUFLEN) + flsIdx],templet);
                        if((gcp_FlsBuffer[(addrInSector & MASK_FLS_BUFLEN) + flsIdx] & templet) != templet)
                        {               //既与原有数据不一致又不是FF则需要擦除
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
            //优化
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
 * @描述: 对sector一致性纠错
 * 
 * @参数: 
 * @param: sectoraddr 页的起始地址
 * @param: x 第几片flash
 * @返回: 
 * @说明: 
 * @作者: yzy (2014/10/21)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
void SectorConsisForWrite(unsigned long sectoraddr, unsigned char x)
{
    unsigned long tmp;
    unsigned char m,n;

    tmp = sectoraddr & (~0xFFF);
                                        //擦除备份区
    EraseFlsSectorInChip((guls_FLSChipSize[x] >> 1) + tmp, x);

    for(m = 0; m < NUM_FLS_SECTBUF; m++)//从主存取读取,并写入备份区
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
** @描述: Flash 写操作
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 待写入数据的存放缓存
** @param: address(unsigned long): flash片内地址
** @param: length(unsigned short): 数据长度
** @param: x(unsigned char): 第几片flash
** @param: sdmode(unsigned char): 扇区数据保留模式.[0.保留扇区所有(默认),1.保留扇区位于待写数据之前的数据,2.删除扇区所有]
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy(20111-01-10)
** @修改说明: 更改函数实现方式,简化程序
*******************************************************************************/
unsigned char WriteFlsInChip(unsigned char* buffer, unsigned long addr, unsigned short length, unsigned char x, unsigned char sdmode)
{
    unsigned char  sidx, eidx;                      //写入的起始页和结束页
    unsigned char  fstt = 0;
    unsigned char  m;
    unsigned char  n;
    unsigned long addrInSector;                    //段内相对地址
    unsigned long addrOfSector;                    //
    unsigned short ui_len = 0;                      //单个SECTOR中写入的长度
    unsigned short ui_tmp;
                                            //读取的位置不对,不允许跨片读取
    SYS_VAR_CHECK(length + addr > (guls_FLSChipSize[x] >> 1));//只能使用一半容量
    SYS_VAR_CHECK(length == 0);
    SYS_VAR_CHECK(sdmode != 0);

    /************************************************************************
    //循环处理每个需要更改的SECTOR
    ************************************************************************/
    while(true)                             //循环处理每个需要更改的SECTOR
    {
        addrOfSector = addr & (~0xFFF);
        addrInSector = addr & 0xFFF;
        if(length + addrInSector > 4096)    //跨段
        {
            ui_len = 4096 - addrInSector;   //操作的数据长度
        }
        else                                //段内操作
        {
            ui_len = length;                //操作数据长度
        }

        /************************************************************************
        //判断sector状态:是否存在一致性错误,是否需要擦除,写入
        ************************************************************************/
        fstt = SectorVerifyForWrite(buffer, 0, addr, ui_len, x);

        /************************************************************************
        //一致性错误,重写备份区
        ************************************************************************/
        if(fstt & FLS_WRSTT_SIS)            //不一致,重写备份区
        {
            SectorConsisForWrite(addrOfSector, x);
            guc_FlashErrStt |= FLS_ERR_CSIS;
        }
//	        printf("fstt = 0x%x\n", fstt);
	
        /************************************************************************
        //根据BAK区数据,更新后写入ORG区
        ************************************************************************/
        if(fstt & FLS_WRSTT_WRI)            //需要写入
        {
            if(fstt & FLS_WRSTT_ERA)        //需要擦除
            {
                EraseFlsSectorInChip(addrOfSector, x);
                sidx = 0;                   //整段需要重新写入
                eidx = 16;
            }
            else                            //需要写入的页
            {
                sidx = addrInSector >> 8;
                eidx = (addrInSector + ui_len) >> 8;
                if((addrInSector + ui_len) & 0xFF)
                {
                    eidx += 1;
                }
            }
                                            //声明局部变量
            //当前sector需要更改的页:wrPage1-wrPage2(包含)
            unsigned char wrPage1 = addrInSector >> 8;
            unsigned char wrPage2 = (addrInSector + ui_len - 1) >> 8;
            unsigned short bufferIdx = 0;
            
            for(m = 0; m < 3; m++)          //循环写,最多写3次
            {
                for(n = sidx; n < eidx; n++)//逐页写入
                {
                                            //从BAK页读取数据
                    ui_tmp = (n - sidx) % (LEN_FLS_BUF >> 8);//除以256
                    if(!ui_tmp)             //需要从flash中读取新的数据到BUF
                    {
                        ReadFlsInChip(gcp_FlsBuffer, 
                                      (guls_FLSChipSize[x] >> 1) + addrOfSector + (n << 8), 
                                      LEN_FLS_BUF, 
                                      x);
                    }
                    else                    //数据在上次读取的BUF中.
                    {
                        memcpy(gcp_FlsBuffer, 
                                   gcp_FlsBuffer + 256,                                    
                                   LEN_FLS_BUF - (ui_tmp << 8));//乘以256
                    }
	
                                            //替换
                    //*************************************************
                    if(n == wrPage1)        //first page
                    {
                        if(n == wrPage2)    //只有一页
                        {
                            memcpy(gcp_FlsBuffer + (addrInSector & 0xFF), 
                                    buffer, 
                                    ui_len);
                                            //初始化bufferIdx
                            bufferIdx = ui_len;
                        }
                        else
                        {
                            memcpy(gcp_FlsBuffer + (addrInSector & 0xFF), 
                                    buffer, 
                                    256 - (addrInSector & 0xFF));
                                            //初始化bufferIdx
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
                                            //写入ORG页
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
                
                if(n >= eidx)               //写入成功,返回
                {
                    break;
                }
            }
            
            if(m >= 3)
            {
                guc_FlashErrStt |= FLS_ERR_WRITE;//标志写入数据发生错误
                return SYS_ERR_FT;          //返回写入数据错误
            }
        }
	
	
        /************************************************************************
        //根据ORG区数据,回写到BAK区
        ************************************************************************/
        if(fstt & FLS_WRSTT_WRI)            //需要写入
        {
            if(fstt & FLS_WRSTT_ERA)        //需要擦除
            {
                EraseFlsSectorInChip((guls_FLSChipSize[x] >> 1) + addrOfSector, x);
                sidx = 0;                   //整段需要重新写入
                eidx = 16;
            }
            else                            //需要写入的页
            {
                sidx = addrInSector >> 8;
                eidx = (addrInSector + ui_len) >> 8;
                if((addrInSector + ui_len) & 0xFF)
                {
                    eidx += 1;
                }
            }
                                            // 
            for(m = 0; m < 3; m++)          //循环写,最多写3次
            {
                for(n = sidx; n < eidx; n++)//逐页写入
                {
                                            //从ORG页读取数据
                    ui_tmp = (n - sidx) % (LEN_FLS_BUF >> 8);//除以256
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
                                LEN_FLS_BUF - (ui_tmp << 8));//乘以256
                    }
                                            //写入BAK页
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
                
                if(n >= eidx)               //写入成功,返回
                {
                    break;
                }
            }
            
            if(m >= 3)
            {
                guc_FlashErrStt |= FLS_ERR_WRITE;//标志写入数据发生错误
                return SYS_ERR_FT;          //返回写入数据错误
            }
        }

        /************************************************************************
        //修改传入参数,准备下次循环
        ************************************************************************/
        length -= ui_len;                   //待操作数据长度递减
        if(length == 0)                     //数据已经操作完毕
        {
            SYS_OK();                        //返回写入成功
        }

        buffer += ui_len;                   //操作数据指针前移
        addr += ui_len;                     //实际操作地址
    }
}
/*******************************************************************************
** @function_name:  SetFlsInChip
** @function_file:  mxflash.c
** @描述: 将Flash某块区域置为特定值 
** 
** 
** @参数: 
** @param: templet(uint8_t): 置入的数据
** @param: address(uint32_t): 写入数据的地址
** @param: length(uint16_t): 写入数据的长度
** @param: x(uint8_t): flash芯片编号
** 
** @返回: 
** @return:  uint8_t   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy
** @修改说明: 操作机制同 WriteFlsInChip()
*******************************************************************************/
uint8_t SetFlsInChip(uint8_t templet, uint32_t addr, uint16_t length, uint8_t x)
{
    uint8_t  sidx, eidx;                      //写入的起始页和结束页
    uint8_t  fstt = 0;
    uint8_t  m;
    uint8_t  n;
    uint32_t addrInSector;                    //段内相对地址
    uint32_t addrOfSector;                    //
    uint16_t ui_len = 0;                      //单个SECTOR中写入的长度
    uint16_t ui_tmp;
                                            //读取的位置不对,不允许跨片读取
    SYS_VAR_CHECK(length + addr > (guls_FLSChipSize[x] >> 1));//只能使用一半容量
    SYS_VAR_CHECK(length == 0);

    /************************************************************************
    //循环处理每个需要更改的SECTOR
    ************************************************************************/
    while(true)                             //循环处理每个需要更改的SECTOR
    {
        addrOfSector = addr & (~0xFFF);
        addrInSector = addr & 0xFFF;
        if(length + addrInSector > 4096)    //跨段
        {
            ui_len = 4096 - addrInSector;   //操作的数据长度
        }
        else                                //段内操作
        {
            ui_len = length;                //操作数据长度
        }

        /************************************************************************
        //判断sector状态:是否存在一致性错误,是否需要擦除,写入
        ************************************************************************/
        fstt = SectorVerifyForWrite(NULL, templet, addr, ui_len, x);
//	        printf("line[%d],fstt = 0x%x\n", __LINE__, fstt);

        /************************************************************************
        //一致性错误,重写备份区
        ************************************************************************/
        if(fstt & FLS_WRSTT_SIS)            //不一致,重写备份区
        {
            SectorConsisForWrite(addrOfSector, x);
            guc_FlashErrStt |= FLS_ERR_CSIS;
        }

        /************************************************************************
        //根据BAK区数据,更新后写入ORG区
        ************************************************************************/
        if(fstt & FLS_WRSTT_WRI)            //需要写入
        {
            if(fstt & FLS_WRSTT_ERA)        //需要擦除
            {
                EraseFlsSectorInChip(addrOfSector, x);
                sidx = 0;                   //整段需要重新写入
                eidx = 16;
            }
            else                            //需要写入的页
            {
                sidx = addrInSector >> 8;
                eidx = (addrInSector + ui_len) >> 8;
                if((addrInSector + ui_len) & 0xFF)
                {
                    eidx += 1;
                }
            }

            //声明局部变量
            //当前sector需要更改的页:wrPage1-wrPage2(包含)
            unsigned char wrPage1 = addrInSector >> 8;
            unsigned char wrPage2 = (addrInSector + ui_len - 1) >> 8;
            unsigned short bufferIdx = 0;
            
            for(m = 0; m < 3; m++)          //循环写,最多写3次
            {
                for(n = sidx; n < eidx; n++)//逐页写入
                {
                                            //从BAK页读取数据
                    ui_tmp = (n - sidx) % (LEN_FLS_BUF >> 8);//除以256
                    if(!ui_tmp)             //需要从flash中读取新的数据到BUF
                    {
                        ReadFlsInChip(gcp_FlsBuffer, 
                                      (guls_FLSChipSize[x] >> 1) + addrOfSector + (n << 8), 
                                      LEN_FLS_BUF, 
                                      x);
                    }
                    else                    //数据在上次读取的BUF中.
                    {
                        memcpy(gcp_FlsBuffer, 
                                   gcp_FlsBuffer + 256,                                    
                                   LEN_FLS_BUF - (ui_tmp << 8));//乘以256
                    }
	
                                            //替换
                    //*************************************************
                    if(n == wrPage1)        //first page
                    {
                        if(n == wrPage2)    //只有一页
                        {
                            memset(gcp_FlsBuffer + (addrInSector & 0xFF), 
                                    templet, 
                                    ui_len);
                                            //初始化bufferIdx
                            bufferIdx = ui_len;
                        }
                        else
                        {
                            memset(gcp_FlsBuffer + (addrInSector & 0xFF), 
                                    templet, 
                                    256 - (addrInSector & 0xFF));
                                            //初始化bufferIdx
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
                                            //写入ORG页
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
                
                if(n >= eidx)               //写入成功,返回
                {
//	                    printf("n >= eidx !! \n");
                    break;
                }
            }
            if(m >= 3)
            {
                guc_FlashErrStt |= FLS_ERR_WRITE;//标志写入数据发生错误
                return SYS_ERR_FT;          //返回写入数据错误
            }
        }


        /************************************************************************
        //根据ORG区数据,回写到BAK区
        ************************************************************************/
        if(fstt & FLS_WRSTT_WRI)            //需要写入
        {
            if(fstt & FLS_WRSTT_ERA)        //需要擦除
            {
                EraseFlsSectorInChip((guls_FLSChipSize[x] >> 1) + addrOfSector, x);
                sidx = 0;                   //整段需要重新写入
                eidx = 16;
            }
            else                            //需要写入的页
            {
                sidx = addrInSector >> 8;
                eidx = (addrInSector + ui_len) >> 8;
                if((addrInSector + ui_len) & 0xFF)
                {
                    eidx += 1;
                }
            }
                                            // 
            for(m = 0; m < 3; m++)          //循环写,最多写3次
            {
                for(n = sidx; n < eidx; n++)//逐页写入
                {
                                            //从ORG页读取数据
                    ui_tmp = (n - sidx) % (LEN_FLS_BUF >> 8);//除以256
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
                               LEN_FLS_BUF - (ui_tmp << 8));//乘以256
                    }
                                            //写入BAK页
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
                
                if(n >= eidx)               //写入成功,返回
                {
                    break;
                }
            }
            
            if(m >= 3)
            {
                guc_FlashErrStt |= FLS_ERR_WRITE;//标志写入数据发生错误
                return SYS_ERR_FT;          //返回写入数据错误
            }
        }

        /************************************************************************
        //修改传入参数,准备下次循环
        ************************************************************************/
        length -= ui_len;                   //待操作数据长度递减
        if(length == 0)                     //数据已经操作完毕
        {
            SYS_OK();                        //返回写入成功
        }

        addr += ui_len;                     //实际操作地址
    }
}

#else

/*******************************************************************************
** @function_name:  WriteFlsInChip
** @function_file:  mxflash.c
** @描述: Flash 写操作
** 
** 
** @参数: 
** @param: buffer(unsigned char *): 待写入数据的存放缓存
** @param: address(unsigned long ): flash片内地址
** @param: length(unsigned short): 数据长度
** @param: x(unsigned char ): 第几片flash
** @param: sdmode(unsigned char ): 扇区数据保留模式.[0.保留扇区所有(默认),1.保留扇区位于待写数据之前的数据,2.删除扇区所有]
** 
** @返回: 
** @return:  unsigned char    
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy(20111-01-10)
** @修改说明: 更改函数实现方式,简化程序
*******************************************************************************/
unsigned char  WriteFlsInChip(unsigned char * buffer, unsigned long  addr, unsigned short length, unsigned char  x, unsigned char  sdmode)
{
    unsigned char   sidx, eidx;                      //写入的起始页和结束页
    unsigned char   fwri;                            //是否需要写入标志
    unsigned char   fera;                            //是否需要擦除标志
    unsigned char   m;
    unsigned char   n;
    int i = 0;
    //unsigned long  addrInFlash;                     //FLASH的绝对地址
    unsigned long  addrInSector;                    //段内相对地址
    unsigned long  addrOfSector;                    //
    unsigned short ui_len = 0;                      //单个SECTOR中写入的长度
                                            //读取的位置不对,不允许跨片读取
    SYS_VAR_CHECK(length + addr > guls_FLSChipSize[x]);
    SYS_VAR_CHECK(length == 0);
    SYS_VAR_CHECK(sdmode != 0);

    /************************************************************************
    //循环处理每个需要更改的SECTOR
    ************************************************************************/
    while(true)                             //循环处理每个需要更改的SECTOR
    {
        if(length + (addr & 0xFFF) > 4096)  //跨段
        {
            ui_len = 4096 - (addr & 0xFFF); //操作的数据长度
        }
        else                                //段内操作
        {
            ui_len = length;                //操作数据长度
        }
        //printf("gcp_FlsBuffer[] = %02X %02X %02X %02X %02X", gcp_FlsBuffer[0], gcp_FlsBuffer[1], gcp_FlsBuffer[2], gcp_FlsBuffer[3], gcp_FlsBuffer[4]);

                                            //读取一段数据
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
        //循环判断是否需改写,改写前是否需擦除
        ************************************************************************/
        fwri = 0;                           //复位标志位
        fera = 0;
                                            //循环判断是否需改写部分是否需要擦除
        for(unsigned long  k = 0; k < ui_len; k++)
        {
            unsigned char  uc_data = gcp_FlsBuffer[(addr & 0xFFF) + k];
            if(uc_data != buffer[k])        //判断是否与原有数据一致
            {
                fwri = 0x01;
                if((uc_data & buffer[k]) != buffer[k])//(uc_data != 0xFF)         //既与原有数据不一致又不是FF则需要擦除
                {
                    fera = 0x01;
                    break;
                }
            }
        }
//	        printf("fwri = %x fera = %x\n", fwri, fera);
        /************************************************************************
        //写入
        ************************************************************************/
        if(fwri)                            //需要写入
        {
            addrOfSector = addr & (~0xFFF);
            addrInSector = addr & 0xFFF;

            if(fera)                        //需要擦除
            {
                EraseFlsSectorInChip(addrOfSector, x);
                sidx = 0;                   //整段需要重新写入
                eidx = 16;
            }
            else                            //需要写入的页
            {
                sidx = addrInSector >> 8;
                eidx = (addrInSector + ui_len) >> 8;
                if((addrInSector + ui_len) & 0xFF)
                {
                    eidx += 1;
                }
            }
                                            //转移需要写入的数据
            memcpy(gcp_FlsBuffer + addrInSector, buffer, ui_len);
                                            // 
            for(m = 0; m < 3; m++)          //循环写,最多写3次
            {
                for(n = sidx; n < eidx; n++)//逐页写入
                {
                                            
                    if(WriteFlsPageInChip(gcp_FlsBuffer + (n << 8), addrOfSector + (n << 8), 256, x) != SYS_ERR_OK)
                    {
                        EraseFlsSectorInChip(addrOfSector, x);
                        sidx = 0;
                        eidx = 16;
                        break;
                    }
                }
                
                if(n >= eidx)               //写入成功,返回
                {
                    break;
                }
            }
            
            if(m >= 3)
            {
                guc_FlashErrStt |= FLS_ERR_WRITE;//标志写入数据发生错误
                return SYS_ERR_FT;          //返回写入数据错误
            }
        }

        /************************************************************************
        //杂
        ************************************************************************/
        length -= ui_len;                   //待操作数据长度递减
        if(length == 0)                     //数据已经操作完毕
        {
            SYS_OK();                        //返回写入成功
        }

        buffer += ui_len;                   //操作数据指针前移
        addr += ui_len;                     //实际操作地址
    }
}
/*******************************************************************************
** @function_name:  SetFlsInChip
** @function_file:  mxflash.c
** @描述: 将Flash某块区域置为特定值 
** 
** 
** @参数: 
** @param: templet(unsigned char): 置入的数据
** @param: address(unsigned long): 写入数据的地址
** @param: length(unsigned short): 写入数据的长度
** @param: x(unsigned char): flash芯片编号
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy
** @修改说明: 操作机制同 WriteFlsInChip()
*******************************************************************************/
unsigned char SetFlsInChip(unsigned char templet, unsigned long addr, unsigned short length, unsigned char x)
{
    unsigned char  sidx, eidx;                      //写入的起始页和结束页
    unsigned char  fwri;                            //是否需要写入标志
    unsigned char  fera;                            //是否需要擦除标志
    unsigned char  m;
    unsigned char  n;
    int i = 0;
    //unsigned long addrInFlash;                     //FLASH的绝对地址
    unsigned long addrInSector;                    //段内相对地址
    unsigned long addrOfSector;                    //
    unsigned short ui_len = 0;                      //单个SECTOR中写入的长度
                                            //读取的位置不对,不允许跨片读取
    SYS_VAR_CHECK(length + addr > guls_FLSChipSize[x]);
    SYS_VAR_CHECK(length == 0);

    /************************************************************************
    //循环处理每个需要更改的SECTOR
    ************************************************************************/
    while(true)                             //循环处理每个需要更改的SECTOR
    {
        if(length + (addr & 0xFFF) > 4096)  //跨段
        {
            ui_len = 4096 - (addr & 0xFFF); //操作的数据长度
        }
        else                                //段内操作
        {
            ui_len = length;                //操作数据长度
        }
        
                                            //读取一段数据
        while(SYS_ERR_OK != ReadFlsInChip(gcp_FlsBuffer, addr & (~0xFFF), 4096, x) && (i++ < 100));
        
        if(i >= 100)
        {
            return SYS_ERR_FT;
        }
        /************************************************************************
        //循环判断是否需改写,改写前是否需擦除
        ************************************************************************/
        fwri = 0;                           //复位标志位
        fera = 0;
                                            //循环判断是否需改写部分是否需要擦除
        for(unsigned long k = 0; k < ui_len; k++)
        {
            unsigned char uc_data = gcp_FlsBuffer[(addr & 0xFFF) + k];
            if(uc_data != templet)          //判断是否与原有数据一致
            {
                fwri = 0x01;
                if((uc_data & templet) != templet)//(uc_data != 0xFF)         //既与原有数据不一致又不是FF则需要擦除
                {
                    fera = 0x01;
                    break;
                }
            }
        }
     
        /************************************************************************
        //写入
        ************************************************************************/
        if(fwri)                            //需要写入
        {
            addrOfSector = addr & (~0xFFF);
            addrInSector = addr & 0xFFF;

            if(fera)                        //需要擦除
            {
                EraseFlsSectorInChip(addrOfSector, x);
                sidx = 0;                   //整段需要重新写入
                eidx = 16;
            }
            else                            //需要写入的页
            {
                sidx = addrInSector >> 8;
                eidx = (addrInSector + ui_len) >> 8;
                if((addrInSector + ui_len) & 0xFF)
                {
                    eidx += 1;
                }
            }
                                                //置位需要写入的数据
            memset(gcp_FlsBuffer + (addr & 0xFFF), templet, ui_len);
//            printf("gcp_FlsBuffer addr = %d len = %d \n", (addr & 0xFFF), ui_len);
            for(m = 0; m < 3; m++)          //循环写,最多写3次
            {
                for(n = sidx; n < eidx; n++)//逐页写入
                {
                    if(WriteFlsPageInChip(gcp_FlsBuffer + (n << 8), addrOfSector + (n << 8), 256, x) != SYS_ERR_OK)
                    {
                        EraseFlsSectorInChip(addrOfSector, x);
                        sidx = 0;
                        eidx = 16;
                        break;
                    }
                }
                
                if(n >= eidx)               //写入成功,返回
                {
                    break;
                }
            }
//            printf("set end m = %d\n", m);
            if(m >= 3)
            {
                guc_FlashErrStt |= FLS_ERR_WRITE;//标志写入数据发生错误
                return SYS_ERR_FT;          //返回写入数据错误
            }
        }

        /************************************************************************
        //杂
        ************************************************************************/
        length -= ui_len;                   //待操作数据长度递减
        if(length == 0)                     //数据已经操作完毕
        {
            SYS_OK();                        //返回写入成功
        }
        
        addr += ui_len;                     //实际操作地址
    }
}


#endif

/*******************************************************************************
** @function_name:  WriteFlsInChip
** @function_file:  mxflash.c
** @描述: Flash 写操作
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 待写入数据的存放缓存
** @param: address(unsigned long): flash片内地址
** @param: length(unsigned short): 数据长度
** @param: x(unsigned char): 第几片flash
** @param: sdmode(unsigned char): 扇区数据保留模式.[0.保留扇区所有(默认),1.保留扇区位于待写数据之前的数据,2.删除扇区所有]
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy(20111-01-10)
** @修改说明: 更改函数实现方式,简化程序
*******************************************************************************/
unsigned char WriteFlsInChipSpec(unsigned char* buffer, unsigned long addr, unsigned short length, unsigned char x, unsigned char sdmode)
{
    //unsigned char  sidx, eidx;                      //写入的起始页和结束页
   // unsigned char  fstt = 0;
    unsigned char  m;
    //unsigned char  n;
    unsigned long addrInSector;                    //段内相对地址
    //unsigned long addrOfSector;                    //
    unsigned short ui_len = 0;                      //单个SECTOR中写入的长度
    //unsigned short ui_tmp;
                                            //读取的位置不对,不允许跨片读取
    SYS_VAR_CHECK(length + addr > (guls_FLSChipSize[x] >> 1));//只能使用一半容量
    SYS_VAR_CHECK(length == 0);
    SYS_VAR_CHECK(sdmode != 1);

    /************************************************************************
    //循环处理每个需要更改的SECTOR
    ************************************************************************/
    while(true)                             //循环处理每个需要更改的SECTOR
    {
        //addrOfSector = addr & (~0xFFF);
        addrInSector = addr & 0xFFF;
        if(length + addrInSector > 4096)    //跨段
        {
            ui_len = 4096 - addrInSector;   //操作的数据长度
        }
        else                                //段内操作
        {
            ui_len = length;                //操作数据长度
        }

        for(m = 0; m < 3; m++)          //循环写,最多写3次
        {
            if(FLS_WritePage(buffer, addr, ui_len) == SYS_ERR_OK)
            {
                break;
            }
        }
        
        if(m >= 3)
        {
            guc_FlashErrStt |= FLS_ERR_WRITE;//标志写入数据发生错误
            return SYS_ERR_FT;          //返回写入数据错误
        }
        /************************************************************************
        //修改传入参数,准备下次循环
        ************************************************************************/
        length -= ui_len;                   //待操作数据长度递减
        if(length == 0)                     //数据已经操作完毕
        {
            SYS_OK();                        //返回写入成功
        }

        buffer += ui_len;                   //操作数据指针前移
        addr += ui_len;                     //实际操作地址
    }
}


//	#else
//	
//	/*******************************************************************************
//	** @function_name:  WriteFlsInChip
//	** @function_file:  mxflash.c
//	** @描述: Flash 写操作
//	** 
//	** 
//	** @参数: 
//	** @param: buffer(unsigned char*): 待写入数据的存放缓存
//	** @param: address(unsigned long): flash片内地址
//	** @param: length(unsigned short): 数据长度
//	** @param: x(unsigned char): 第几片flash
//	** @param: sdmode(unsigned char): 扇区数据保留模式.[0.保留扇区所有(默认),1.保留扇区位于待写数据之前的数据,2.删除扇区所有]
//	** 
//	** @返回: 
//	** @return:  unsigned char   
//	** @作者: yzy (2010-05-11)
//	**-----------------------------------------------------------------------------
//	** @修改人: yzy(20111-01-10)
//	** @修改说明: 更改函数实现方式,简化程序
//	*******************************************************************************/
//	unsigned char WriteFlsInChip(unsigned char* buffer, unsigned long addr, unsigned short length, unsigned char x, unsigned char sdmode)
//	{
//	    unsigned char  sidx, eidx;                      //写入的起始页和结束页
//	    unsigned char  fwri;                            //是否需要写入标志
//	    unsigned char  fera;                            //是否需要擦除标志
//	    unsigned char  m;
//	    unsigned char  n;
//	    //unsigned long addrInFlash;                     //FLASH的绝对地址
//	    unsigned long addrInSector;                    //段内相对地址
//	    unsigned long addrOfSector;                    //
//	    unsigned short ui_len = 0;                      //单个SECTOR中写入的长度
//	                                            //读取的位置不对,不允许跨片读取
//	    SYS_VAR_CHECK(length + addr > guls_FLSChipSize[x]);
//	    SYS_VAR_CHECK(length == 0);
//	    SYS_VAR_CHECK(sdmode != 0);
//	
//	    /************************************************************************
//	    //循环处理每个需要更改的SECTOR
//	    ************************************************************************/
//	    while(true)                             //循环处理每个需要更改的SECTOR
//	    {
//	        if(length + (addr & 0xFFF) > 4096)  //跨段
//	        {
//	            ui_len = 4096 - (addr & 0xFFF); //操作的数据长度
//	        }
//	        else                                //段内操作
//	        {
//	            ui_len = length;                //操作数据长度
//	        }
//	
//	                                            //读取一段数据
//	        while(SYS_ERR_OK != ReadFlsInChip(gcp_FlsBuffer, addr & (~0xFFF), 4096, x));
//	        
//	        /************************************************************************
//	        //循环判断是否需改写,改写前是否需擦除
//	        ************************************************************************/
//	        fwri = 0;                           //复位标志位
//	        fera = 0;
//	                                            //循环判断是否需改写部分是否需要擦除
//	        for(unsigned long k = 0; k < ui_len; k++)
//	        {
//	            unsigned char uc_data = gcp_FlsBuffer[(addr & 0xFFF) + k];
//	            if(uc_data != buffer[k])        //判断是否与原有数据一致
//	            {
//	                fwri = 0x01;
//	                if((uc_data & buffer[k]) != buffer[k])//(uc_data != 0xFF)         //既与原有数据不一致又不是FF则需要擦除
//	                {
//	                    fera = 0x01;
//	                    break;
//	                }
//	            }
//	        }
//	        
//	        /************************************************************************
//	        //写入
//	        ************************************************************************/
//	        if(fwri)                            //需要写入
//	        {
//	            addrOfSector = addr & (~0xFFF);
//	            addrInSector = addr & 0xFFF;
//	
//	            if(fera)                        //需要擦除
//	            {
//	                EraseFlsSectorInChip(addrOfSector, x);
//	                sidx = 0;                   //整段需要重新写入
//	                eidx = 16;
//	            }
//	            else                            //需要写入的页
//	            {
//	                sidx = addrInSector >> 8;
//	                eidx = (addrInSector + ui_len) >> 8;
//	                if((addrInSector + ui_len) & 0xFF)
//	                {
//	                    eidx += 1;
//	                }
//	            }
//	                                            //转移需要写入的数据
//	            MoveBuffer(buffer, gcp_FlsBuffer + addrInSector, ui_len);
//	                                            // 
//	            for(m = 0; m < 3; m++)          //循环写,最多写3次
//	            {
//	                for(n = sidx; n < eidx; n++)//逐页写入
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
//	                if(n >= eidx)               //写入成功,返回
//	                {
//	                    break;
//	                }
//	            }
//	            
//	            if(m >= 3)
//	            {
//	                guc_FlashErrStt |= FLS_ERR_WRITE;//标志写入数据发生错误
//	                return SYS_ERR_FT;          //返回写入数据错误
//	            }
//	        }
//	
//	        /************************************************************************
//	        //杂
//	        ************************************************************************/
//	        length -= ui_len;                   //待操作数据长度递减
//	        if(length == 0)                     //数据已经操作完毕
//	        {
//	            SYS_OK();                        //返回写入成功
//	        }
//	
//	        buffer += ui_len;                   //操作数据指针前移
//	        addr += ui_len;                     //实际操作地址
//	    }
//	}
//	
//	#endif


/*******************************************************************************
** @function_name:  PCB_FLS_Write
** @function_file:  mxflash.c
** @描述: Flash 写操作
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 写入数据的内容缓存
** @param: address(unsigned long): 写入数据的地址
** @param: length(unsigned short): 写入数据的长度
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy
** @修改说明: 尽可能地减少檫除的次数(即:当写入数据与原有数据一致或原有数据为FF时不再擦除)
*******************************************************************************/
unsigned char FLS_Write(unsigned char* buffer, unsigned long address, unsigned short length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    unsigned char err = 0;
    unsigned char num;
    
//	    DRV_POW_CHECK(PCB_VBatVoltGet() < 2800);    
    if(gul_FlashSafeCode != 0xA5A5A5A5)     //判断安全操作字是否打开
    {
        return SYS_ERR_FT;                  //返回致命错误
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
** @描述: Flash 特殊写操作(适用于无须保存原有数据的写操作,特别适用于大数据量连续写入)
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 写入数据的内容缓存
** @param: address(unsigned long): 写入数据的地址
** @param: length(unsigned short): 写入数据的长度
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2014-03-27)
**-----------------------------------------------------------------------------
** @修改人: yzy
** @修改说明: 
*******************************************************************************/
unsigned char FLS_WriteSpec(unsigned char* buffer, unsigned long address, unsigned short length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    unsigned char err = 0;
    unsigned char num;
//	    DRV_POW_CHECK(PCB_VBatVoltGet() < 2800);    
    
    if(gul_FlashSafeCode != 0xA5A5A5A5)     //判断安全操作字是否打开
    {
        return SYS_ERR_FT;                  //返回致命错误
    }
    
    num = MultiFlashFilterExt(fs, address, length);
    if(num == 0)
    {
        gul_FlashSafeCode = 0;
        return 2;
    }
    
    for(unsigned char uc_i = 0; uc_i < num; uc_i++)
    {                                       //说明:当确保数据写入时为连续后移时,可以用模式2.但用模式1更保险.
        err |= WriteFlsInChipSpec(buffer + fs[uc_i].opoffset, fs[uc_i].opaddr, fs[uc_i].oplen, fs[uc_i].x, 1);
    }
    
    gul_FlashSafeCode = 0;
    return err;
}



//	unsigned char SetFlsInChip(unsigned char templet, unsigned long addr, unsigned short length, unsigned char x)
//	{
//	    unsigned char  sidx, eidx;                      //写入的起始页和结束页
//	    unsigned char  fwri;                            //是否需要写入标志
//	    unsigned char  fera;                            //是否需要擦除标志
//	    unsigned char  m;
//	    unsigned char  n;
//	    //unsigned long addrInFlash;                     //FLASH的绝对地址
//	    unsigned long addrInSector;                    //段内相对地址
//	    unsigned long addrOfSector;                    //
//	    unsigned short ui_len = 0;                      //单个SECTOR中写入的长度
//	                                            //读取的位置不对,不允许跨片读取
//	    SYS_VAR_CHECK(length + addr > guls_FLSChipSize[x]);
//	    SYS_VAR_CHECK(length == 0);
//	
//	    /************************************************************************
//	    //循环处理每个需要更改的SECTOR
//	    ************************************************************************/
//	    while(true)                             //循环处理每个需要更改的SECTOR
//	    {
//	        if(length + (addr & 0xFFF) > 4096)  //跨段
//	        {
//	            ui_len = 4096 - (addr & 0xFFF); //操作的数据长度
//	        }
//	        else                                //段内操作
//	        {
//	            ui_len = length;                //操作数据长度
//	        }
//	        
//	                                            //读取一段数据
//	        while(SYS_ERR_OK != ReadFlsInChip(gcp_FlsBuffer, addr & (~0xFFF), 4096, x));
//	        
//	        /************************************************************************
//	        //循环判断是否需改写,改写前是否需擦除
//	        ************************************************************************/
//	        fwri = 0;                           //复位标志位
//	        fera = 0;
//	                                            //循环判断是否需改写部分是否需要擦除
//	        for(unsigned long k = 0; k < ui_len; k++)
//	        {
//	            unsigned char uc_data = gcp_FlsBuffer[(addr & 0xFFF) + k];
//	            if(uc_data != templet)          //判断是否与原有数据一致
//	            {
//	                fwri = 0x01;
//	                if((uc_data & templet) != templet)//(uc_data != 0xFF)         //既与原有数据不一致又不是FF则需要擦除
//	                {
//	                    fera = 0x01;
//	                    break;
//	                }
//	            }
//	        }
//	     
//	        /************************************************************************
//	        //写入
//	        ************************************************************************/
//	        if(fwri)                            //需要写入
//	        {
//	            addrOfSector = addr & (~0xFFF);
//	            addrInSector = addr & 0xFFF;
//	
//	            if(fera)                        //需要擦除
//	            {
//	                EraseFlsSectorInChip(addrOfSector, x);
//	                sidx = 0;                   //整段需要重新写入
//	                eidx = 16;
//	            }
//	            else                            //需要写入的页
//	            {
//	                sidx = addrInSector >> 8;
//	                eidx = (addrInSector + ui_len) >> 8;
//	                if((addrInSector + ui_len) & 0xFF)
//	                {
//	                    eidx += 1;
//	                }
//	            }
//	                                                //置位需要写入的数据
//	            memset(gcp_FlsBuffer + (addr & 0xFFF), templet, ui_len);
//	            
//	            for(m = 0; m < 3; m++)          //循环写,最多写3次
//	            {
//	                for(n = sidx; n < eidx; n++)//逐页写入
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
//	                if(n >= eidx)               //写入成功,返回
//	                {
//	                    break;
//	                }
//	            }
//	            
//	            if(m >= 3)
//	            {
//	                guc_FlashErrStt |= FLS_ERR_WRITE;//标志写入数据发生错误
//	                return SYS_ERR_FT;          //返回写入数据错误
//	            }
//	        }
//	
//	        /************************************************************************
//	        //杂
//	        ************************************************************************/
//	        length -= ui_len;                   //待操作数据长度递减
//	        if(length == 0)                     //数据已经操作完毕
//	        {
//	            SYS_OK();                        //返回写入成功
//	        }
//	        
//	        addr += ui_len;                     //实际操作地址
//	    }
//	}



/*******************************************************************************
** @function_name:  PCB_FLS_Set
** @function_file:  mxflash.c
** @描述: Flash 置数标志
** 
** 
** @参数: 
** @param: templet(unsigned char): 置入的数据
** @param: address(unsigned long): 写入数据的地址
** @param: length(unsigned short): 写入数据的长度
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy
** @修改说明: 操作机制同FLS_Write()
*******************************************************************************/
unsigned char FLS_Set(unsigned char templet, unsigned long address, unsigned short length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    unsigned char err = 0;
    unsigned char num;
    
//	    DRV_POW_CHECK(PCB_VBatVoltGet() < 2800);    
    if(gul_FlashSafeCode != 0xA5A5A5A5)     //判断安全操作字是否打开
    {
        return SYS_ERR_FT;                  //返回致命错误
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
** @描述: flash硬件自检
** 
** 
** @参数: 
** 
** @返回: 
** @return:  unsigned char 位图.1:硬件故障,0:硬件正常.
**                 bit0-bit7分别表示第1-8块flash的硬件状态
** @作者: yzy (2018-07-02)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
//	unsigned char TestFlashChip(void)
//	{
//	    unsigned char cmp1[TEST_FLS_BYTES];             //原始数据
//	    unsigned char cmp2[TEST_FLS_BYTES];             //修改后理论写入数据
//	    unsigned char cmp3[TEST_FLS_BYTES];             //修改后实际写入数据
//	    unsigned long chip_addr;                        //片内地址
//	    unsigned char err = 0;
//	    
//	    for(unsigned char k = 0; k < FLS_CHIP_NO; k++)
//	    {
//	        for(unsigned char w = 0; w < 2; w++)        //测试两次:一次片头,一次片尾(确保容量正确)
//	        {
//	            chip_addr = w ? (guls_FLSChipSize[k] - TEST_FLS_BYTES) : 0;
//	                                            //读取原始数据
//	            if(ReadFlsInChip(cmp1, chip_addr, TEST_FLS_BYTES, k) != SYS_ERR_OK)
//	            {
//	                err |= Bit_Map8[k];
//	                break;
//	            }
//	                                            //修改原始数据
//	            for(unsigned char i = 0; i < TEST_FLS_BYTES; i++)
//	            {
//	                cmp2[i] = cmp1[i] + 2;
//	            }
//	                                            //写入理论写入数据
//	            if(WriteFlsInChip(cmp2, chip_addr, TEST_FLS_BYTES, k, 0) != SYS_ERR_OK)
//	            {
//	                err |= Bit_Map8[k];
//	                break;
//	            }
//	                                            //读取实际写入数据
//	            if(ReadFlsInChip(cmp3, chip_addr, TEST_FLS_BYTES, k) != SYS_ERR_OK)
//	            {
//	                err |= Bit_Map8[k];
//	                break;
//	            }
//	                                            //恢复原始值
//	            WriteFlsInChip(cmp1, chip_addr, TEST_FLS_BYTES, k, 0);
//	                                            //比对理论实际值
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
** @描述: Flash读取ID
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 
** @param: x(unsigned long): 
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2018-07-02)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char Fls_ReadID(unsigned char* buffer, unsigned long x)
{
    gs_FlashSPI[x].command[0] = MX_CMD_RDID;  //读取Flash ID命令
    gs_FlashSPI[x].data = buffer;
    gs_FlashSPI[x].cmdnum = 1;
    gs_FlashSPI[x].length = 3;
                                            //读取数据
    return SPI_Read(&gs_FlashSPI[x]);
}

/*******************************************************************************
** @function_name:  PCB_Flash_Init
** @function_file:  mxflash.c
** @描述: 初始化Flash
** 
** @参数: 
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2018-07-02)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char Flash_Init(void)
{
    //gcp_FlsBuffer = (unsigned char*)SYS_GetMEM(LEN_FLS_SECTOR);
    //int ret = 0;
    guc_FlashErrStt = 0;                    //初始化错误状态字
    
//	    DRV_POW_CHECK(PCB_VBatVoltGet() < 2800);    
    extern  const SPIIO_PORTS gs_FlashSpiPort;

    Init_SPI(&gs_FlashSpiPort);
    
    FLS_OpenPwr();

    
    FLS_KillHwWp();

    for(unsigned long x = 0; x < FLS_CHIP_NO; x++)
    {
                                            //设置cs线拉高操作的函数
        gs_FlashSPI[x].csup = gs_FlashGetStt[x].csup;
                                            //设置cs线置低操作的函数
        gs_FlashSPI[x].csdown = gs_FlashGetStt[x].csdown;
        
        FLS_ExitDP();

        unsigned char buffer[3];
        memset(buffer,0,3);
        Fls_ReadID(buffer, x);              //读取Flash ID
//	        printf("line[%d]:buffer[0] = 0x%x ret = %d\n",__LINE__,buffer[0], ret);
//	        printf("line[%d]:buffer[1] = 0x%x\n",__LINE__,buffer[1]);
//	        printf("line[%d]:buffer[2] = 0x%x\n",__LINE__,buffer[2]);
        
                                            //判断型号
        if((buffer[0] != FLS_TYPE_MX) && (buffer[0] != FLS_TYPE_EON))    
        {
            Fls_ReadID(buffer, x);          //重读一遍
            
//	            printf("line[%d]:buffer[0] = 0x%x ret = %d\n",__LINE__,buffer[0], ret);
//	            printf("line[%d]:buffer[1] = 0x%x\n",__LINE__,buffer[1]);
//	            printf("line[%d]:buffer[2] = 0x%x\n",__LINE__,buffer[2]);
                                            //判断状态
            if((buffer[0] != FLS_TYPE_MX) && (buffer[0] != FLS_TYPE_EON))
            {                               //发生错误,Flash不存在
                guc_FlashErrStt = FLS_ERR_EXSIT;
            }
        }
        
        FLS_Disable_WP(x);
        //FLS_FREE(x, true);
        while(!FLS_FREE(x, true));          //等待Flash空闲
        
        if(guls_FLSChipSize[x] >= 0x2000000)     //>=32M
        {
            //FLS_Enable4B(x);                    //开启使用4字节指令
            FLS_GetSecu(x);
            if(!(guc_FlashSecu[x] & MX_CMD_4BYTEADDR_BIT))
            {
                FLS_Enable4B(x);                //开启使用4字节指令
                //return 2;
            }            
        }
        
    }
    
    if(guc_FlashErrStt)
    {
        return SYS_ERR_FT;                  //返回发生致命错误
    }
    else
    {
        SYS_OK();                            //返回初始化成功
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

