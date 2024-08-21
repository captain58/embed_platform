/******************************Copyright(c)******************************
**                          
**
** File Name: mxflash.c
** Author: 
** Date Last Update: 2015-05-18
** Description: mxic Flash驱动程序:2IO标准SPI口,兼容3字节地址(<32MB),
**              以及4字节地址(>=32MB)芯片MX25L257
**              根据宏配置来区分容量(不根据读取寄存器 memory density)
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#define EXT_MXFLASH

#include "sys.h"
#include "hal.h"




/************************************************************************
**Flash芯片类型
 ************************************************************************/
#define  FLS_TYPE_MX         0xC2
#define  FLS_TYPE_EON        0x1C




/************************************************************************
**sector 长度
 ************************************************************************/
#define  LEN_FLS_SECTOR  4096        //sector 长度



/************************************************************************
**定义Flash操作命令字
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
**定义FLASH Buffer
 ************************************************************************/
uint8* gcp_FlsBuffer;



/************************************************************************
**定义FLASH芯片数量
 ************************************************************************/
#if FLS_CHIP_NO < 1
#warning "没有Flash芯片定义,不需要包含本C文件!"
#endif


/************************************************************************
**将CS线拉高的函数格式
 ************************************************************************/
void FLS_csupA(void)
{
    SPI_CS_1_FLSA();
}


/************************************************************************
**将CS线拉低的函数格式
 ************************************************************************/
void FLS_csdownA(void)
{
    SPI_CS_0_FLSA();
}


#if FLS_CHIP_NO > 1
/************************************************************************
**将CS线拉高的函数格式
 ************************************************************************/
void FLS_csupB(void)
{
    SPI_CS_1_FLSB();
}


/************************************************************************
**将CS线拉低的函数格式
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
**定义普通Flash 操作SPI的命令的结构体
 ************************************************************************/
SPIIO gs_FlashSPI[FLS_CHIP_NO];

/************************************************************************
**从Flash芯片中读取的Flash状态
 ************************************************************************/
uint8 guc_FlashStatus[FLS_CHIP_NO];




/************************************************************************
**Flash操作安全状态字
 ************************************************************************/
uint32 gul_FlashSafeCode;

/************************************************************************
**定义Flash错误状态字
**bit0 flash是否存在
**bit1 是否有发生读数据错误
**bit2 是否有发生写数据错误
 ************************************************************************/
uint8 guc_FlashErrStt;


/************************************************************************
**定义读取Flash状态的结构体
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
**定义写入Flash状态字的结构体
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
**FLash写使能
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
**FLash写禁能
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
**定义打开写保护操作的结构体
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
**定义禁止写保护操作的结构体
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
**获取FLASH状态
 ************************************************************************/
#define FLS_GetStatus(X) {SPI_Read((SPIIO*)&gs_FlashGetStt[X]);}

#define FLS_WriteStatus(x) {SPI_Write((SPIIO*)&gs_FlsWriteStatus[x]);}

/************************************************************************
**写使能
 ************************************************************************/
#define FLS_EnableWrite(x) {SPI_Write((SPIIO*)&gs_FlsEnableWrite[x]);}

/************************************************************************
**写禁能
 ************************************************************************/
#define FLS_DisableWrite(x) {SPI_Write((SPIIO*)&gs_FlsDisableWrite[x]);}






/************************************************************************
**关闭保护管脚
 ************************************************************************/
void FLS_KillHwWp(void)
{
        SPI_WP_1_FLSA();
    #if FLS_CHIP_NO > 1
        SPI_WP_1_FLSB();
    #endif
}


/************************************************************************
**屏蔽Flash写保护
 ************************************************************************/
void FLS_Disable_WP(uint32 X)
{
    FLS_EnableWrite(X);
    SPI_Write((SPIIO*)&gs_FlashDisableWP[X]);
}

/************************************************************************
**写保护
 ************************************************************************/
void FLS_Enable_WP(uint32 X)
{
    FLS_EnableWrite(X);
    SPI_Write((SPIIO*)&gs_FlashEnableWP[X]);
}

/************************************************************************
**Flash进入以及退出低功耗模式
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
 * @修改人: yzy (2009-8-8)
 **********************************************************/
bool FLS_FREE(uint32 x, bool imd)
{
    FLS_GetStatus(x);                   //更新Flash的状态

    if(!(guc_FlashStatus[x] & 0x01))    //判断Flash的状态
    {
        FLS_GetStatus(x);               //更新Flash的状态

        if(!(guc_FlashStatus[x] & 0x01))//再次判断,防止判断错误
        {
            return true;                //空闲就马上返回
        }
    }
    if(imd)                             //判断是否需要快速返回的
    {
                                        //延迟100个循环
        for(uint16 dly = 100; dly > 0; dly--);
    }
    else
    {
        msleep(10);                 //延迟10ms
    }
    return false;                       //返回不空闲
}

/*******************************************************************************
** @function_name:  EraseFlsSectorInChip
** @function_file:  mxflash.c
** @描述: MX Flash 段檫除操作
** 
** 
** @参数: 
** @param: address(uint32): 
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 EraseFlsSectorInChip(uint32 address, uint8 x)
{
    uint8 err;
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

        gs_FlashSPI[x].data = __NULL;
        gs_FlashSPI[x].cmdnum = 4;
        gs_FlashSPI[x].length = 0;
    }
    else                                    //>=32M
    {
        gs_FlashSPI[x].command[0] = MX_CMD_SE;  //块檫除指令
                                                //块地址
        gs_FlashSPI[x].command[1] = address >> 24;
        gs_FlashSPI[x].command[2] = address >> 16;
        gs_FlashSPI[x].command[3] = address >> 8;
        gs_FlashSPI[x].command[4] = address;

        gs_FlashSPI[x].data = __NULL;
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
** @描述: MX Flash 64K(block)块檫除操作
** 
** 
** @参数: 
** @param: address(uint32): 
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 EraseFlsBlockInChip(uint32 address, uint8 x)
{
    uint8 err;
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

        gs_FlashSPI[x].data = __NULL;
        gs_FlashSPI[x].cmdnum = 4;
        gs_FlashSPI[x].length = 0;
    }
    else                                    //>=32M
    {
        gs_FlashSPI[x].command[0] = MX_CMD_BE;  //块檫除指令
                                                //块地址
        gs_FlashSPI[x].command[1] = address >> 24;
        gs_FlashSPI[x].command[2] = address >> 16;
        gs_FlashSPI[x].command[3] = address >> 8;
        gs_FlashSPI[x].command[4] = address;

        gs_FlashSPI[x].data = __NULL;
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
** @function_name:  EraseFlsInChip
** @function_file:  mxflash.c
** @描述: MX Flash 全片擦除操作
** 
** 
** @参数: 
** @param: 
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 EraseFlsInChip(uint8 x)
{
    uint8 err;

    FLS_EnableWrite(x);                     //写使能

    gs_FlashSPI[x].command[0] = MX_CMD_CE;  //块檫除指令

    gs_FlashSPI[x].data = __NULL;
    gs_FlashSPI[x].cmdnum = 1;
    gs_FlashSPI[x].length = 0;
   
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
** @param: buffer(uint8*): 数据缓存
** @param: address(uint32): 地址
** @param: length(uint16): 读取的数据长度
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 ReadFlsInChip(uint8* buffer, uint32 address, uint16 length, uint8 x)
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
** @function_name:  CompareFlsInChip
** @function_file:  mxflash.c
** @描述: 从Flash指定地址读取指定长度的字节数据,并与传入数据比较是否相等
** 
** 
** @参数: 
** @param: buffer(uint8*): 数据缓存
** @param: address(uint32): 地址
** @param: length(uint16): 读取的数据长度
** 
** @返回: 
** @return:  uint8   SYS_ERR_OK相等,SYS_ERR_FT不相等
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 CompareFlsInChip(uint8* buffer, uint32 address, uint16 length, uint8 x)
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


/*******************************************************************************
** @function_name:  WriteFlsPageInChip
** @function_file:  mxflash.c
** @描述: Flash页编程
** 
** 
** @参数: 
** @param: buffer(uint8*): 数据缓存
** @param: address(uint32): 片内地址
** @param: length(uint16): 写入数据的长度
** @param: x(uint32): 
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 WriteFlsPageInChip(uint8* buffer, uint32 address, uint16 length, uint8 x)
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
** @param: address(uint32): 操作地址
** @param: length(uint16): 操作数据的长度
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint32 MultiFlashFilterExt(S_FLSINCHIP* s_out, uint32 address, uint32 length)
{
    uint32 index = 0;                   //涉及到的存储芯片序号
    uint32 s1 = 0;                      //某片FLASH对应的起始逻辑地址
    uint32 s2 = 0;                      //某片FLASH对应的终止逻辑地址(不含)
    uint32 back = address;
    uint8 sidx = 0;
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
** @function_name:  FLS_AllErase
** @function_file:  mxflash.c
** @描述: MX Flash 全片檫除操作
** 
** 
** @参数: 
** @param: 
** 
** @返回: 
** @return:  void   
** @作者: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
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
** @描述: MX Flash 段4K檫除操作
** 
** 
** @参数: 
** @param: address(uint32): 
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 FLS_SectorErase(uint32 address)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    uint8 num;
                                                //查找存储芯片与片内地址
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
** @描述: MX Flash 块64K檫除操作
** 
** 
** @参数: 
** @param: address(uint32): 
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 FLS_BlockErase(uint32 address)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    uint8 num;
                                                //查找存储芯片与片内地址
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
** @描述: MX Flash 段空白判断
** 
** 
** @参数: 
** @param: address(uint32)
** 
** @返回: 
** @return:  bool true:全空,false:非空
** @作者: yzy (2012-06-09)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
bool FLS_SectorBlankCheck(uint32 address)
{
    address &= ~0xFFF;                      //块起始地址
    
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
** @描述: MX Flash 页空白判断
** 
** 
** @参数: 
** @param: address(uint32)
** 
** @返回: 
** @return:  bool true:全空,false:非空
** @作者: yzy (2012-06-09)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
bool FLS_PageBlankCheck(uint32 address)
{
    address &= ~0xFF;                      //页起始地址
    
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
** @描述: Flash页编程
** 
** 
** @参数: 
** @param: buffer(uint8*): 
** @param: address(uint32): 
** @param: length(uint16): 
** @param: x(uint32): 
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 FLS_WritePage(uint8* buffer, uint32 address, uint16 length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    uint8 num;
                                                //查找存储芯片与片内地址
    num = MultiFlashFilterExt(fs, address, 1);
    if(num == 0)
    {
        return SA_ERR_FT;
    }
    
    return WriteFlsPageInChip(buffer, fs[0].opaddr, length, fs[0].x);
}






//**********************************************************************************
//***********************复杂函数**********************
//**********************************************************************************











/*******************************************************************************
** @function_name:  FLS_Erase
** @function_file:  mxflash.c
** @描述: MX Flash 檫除操作
** 
** 
** @参数: 
** @param: address: 起始地址
** @param: sectornum 扇区数量
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 FLS_Erase(uint32 address, uint32 sectornum)
{
    uint32 ul_i = 0;
    
    if(address & 0xFFF)
    {
        return FLS_ERR_VAR;
    }
                                        //循环擦除
    while(ul_i < sectornum)
    {
        //                              //喂狗:避免执行该函数时,喂狗进程等待资源时间过长,导致复位
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
** @描述: 从Flash指定地址读取指定长度的字节数据
** 
** 
** @参数: 
** @param: buffer(uint8*): 数据缓存
** @param: address(uint32): 地址
** @param: length(uint16): 读取的数据长度
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
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
 * @描述: 预写操作,在写数据之前必须要进行预写操作
 * @参数: 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2014/6/18)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 FLS_PreWrite(void)
{
    gul_FlashSafeCode = 0xA5A5A5A5;
    SYS_OK();
}




/*******************************************************************************
** @function_name:  WriteFlsInChip
** @function_file:  mxflash.c
** @描述: Flash 写操作
** 
** 
** @参数: 
** @param: buffer(uint8*): 待写入数据的存放缓存
** @param: address(uint32): flash片内地址
** @param: length(uint16): 数据长度
** @param: x(uint8): 第几片flash
** @param: sdmode(uint8): 扇区数据保留模式.[0.保留扇区所有(默认),1.保留扇区位于待写数据之前的数据,2.删除扇区所有]
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy(20111-01-10)
** @修改说明: 更改函数实现方式,简化程序
*******************************************************************************/
uint8 WriteFlsInChip(uint8* buffer, uint32 addr, uint16 length, uint8 x, uint8 sdmode)
{
    uint8  sidx, eidx;                      //写入的起始页和结束页
    uint8  fwri;                            //是否需要写入标志
    uint8  fera;                            //是否需要擦除标志
    uint8  m;
    uint8  n;
    //uint32 addrInFlash;                     //FLASH的绝对地址
    uint32 addrInSector;                    //段内相对地址
    uint32 addrOfSector;                    //
    uint16 ui_len = 0;                      //单个SECTOR中写入的长度
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

                                            //读取一段数据
        while(SYS_ERR_OK != ReadFlsInChip(gcp_FlsBuffer, addr & (~0xFFF), 4096, x));
        
        /************************************************************************
        //循环判断是否需改写,改写前是否需擦除
        ************************************************************************/
        fwri = 0;                           //复位标志位
        fera = 0;
                                            //循环判断是否需改写部分是否需要擦除
        for(uint32 k = 0; k < ui_len; k++)
        {
            uint8 uc_data = gcp_FlsBuffer[(addr & 0xFFF) + k];
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
            MoveBuffer(buffer, gcp_FlsBuffer + addrInSector, ui_len);
                                            // 
            for(m = 0; m < 3; m++)          //循环写,最多写3次
            {
                for(n = sidx; n < eidx; n++)//逐页写入
                {
                                            
                    if(WriteFlsPageInChip(gcp_FlsBuffer + (n << 8), addrOfSector + (n << 8), 256, x) != FLS_ERR_OK)
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
                return FLS_ERR_FT;          //返回写入数据错误
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
** @function_name:  FLS_Write
** @function_file:  mxflash.c
** @描述: Flash 写操作
** 
** 
** @参数: 
** @param: buffer(uint8*): 写入数据的内容缓存
** @param: address(uint32): 写入数据的地址
** @param: length(uint16): 写入数据的长度
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy
** @修改说明: 尽可能地减少檫除的次数(即:当写入数据与原有数据一致或原有数据为FF时不再擦除)
*******************************************************************************/
uint8 FLS_Write(uint8* buffer, uint32 address, uint16 length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    uint8 err = 0;
    uint8 num;
    
    if(gul_FlashSafeCode != 0xA5A5A5A5)     //判断安全操作字是否打开
    {
        return FLS_ERR_FT;                  //返回致命错误
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
** @描述: Flash 特殊写操作(适用于无须保存原有数据的写操作,特别适用于大数据量连续写入)
** 
** 
** @参数: 
** @param: buffer(uint8*): 写入数据的内容缓存
** @param: address(uint32): 写入数据的地址
** @param: length(uint16): 写入数据的长度
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2014-03-27)
**-----------------------------------------------------------------------------
** @修改人: yzy
** @修改说明: 
*******************************************************************************/
uint8 FLS_WriteSpec(uint8* buffer, uint32 address, uint16 length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    uint8 err = 0;
    uint8 num;
    
    if(gul_FlashSafeCode != 0xA5A5A5A5)     //判断安全操作字是否打开
    {
        return FLS_ERR_FT;                  //返回致命错误
    }
    
    num = MultiFlashFilterExt(fs, address, length);
    if(num == 0)
    {
        gul_FlashSafeCode = 0;
        return SA_ERR_FT;
    }
    
    for(uint8 uc_i = 0; uc_i < num; uc_i++)
    {                                       //说明:当确保数据写入时为连续后移时,可以用模式2.但用模式1更保险.
        err |= WriteFlsInChip(buffer + fs[uc_i].opoffset, fs[uc_i].opaddr, fs[uc_i].oplen, fs[uc_i].x, 1);
    }
    
    gul_FlashSafeCode = 0;
    return err;
}

/*******************************************************************************
** @function_name:  SetFlsInChip
** @function_file:  mxflash.c
** @描述: 将Flash某块区域置为特定值 
** 
** 
** @参数: 
** @param: templet(uint8): 置入的数据
** @param: address(uint32): 写入数据的地址
** @param: length(uint16): 写入数据的长度
** @param: x(uint8): flash芯片编号
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy
** @修改说明: 操作机制同 WriteFlsInChip()
*******************************************************************************/
uint8 SetFlsInChip(uint8 templet, uint32 addr, uint16 length, uint8 x)
{
    uint8  sidx, eidx;                      //写入的起始页和结束页
    uint8  fwri;                            //是否需要写入标志
    uint8  fera;                            //是否需要擦除标志
    uint8  m;
    uint8  n;
    //uint32 addrInFlash;                     //FLASH的绝对地址
    uint32 addrInSector;                    //段内相对地址
    uint32 addrOfSector;                    //
    uint16 ui_len = 0;                      //单个SECTOR中写入的长度
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
        while(SYS_ERR_OK != ReadFlsInChip(gcp_FlsBuffer, addr & (~0xFFF), 4096, x));
        
        /************************************************************************
        //循环判断是否需改写,改写前是否需擦除
        ************************************************************************/
        fwri = 0;                           //复位标志位
        fera = 0;
                                            //循环判断是否需改写部分是否需要擦除
        for(uint32 k = 0; k < ui_len; k++)
        {
            uint8 uc_data = gcp_FlsBuffer[(addr & 0xFFF) + k];
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
            
            for(m = 0; m < 3; m++)          //循环写,最多写3次
            {
                for(n = sidx; n < eidx; n++)//逐页写入
                {
                    if(WriteFlsPageInChip(gcp_FlsBuffer + (n << 8), addrOfSector + (n << 8), 256, x) != FLS_ERR_OK)
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
                return FLS_ERR_FT;          //返回写入数据错误
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



/*******************************************************************************
** @function_name:  FLS_Set
** @function_file:  mxflash.c
** @描述: Flash 置数标志
** 
** 
** @参数: 
** @param: templet(uint8): 置入的数据
** @param: address(uint32): 写入数据的地址
** @param: length(uint16): 写入数据的长度
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy
** @修改说明: 操作机制同FLS_Write()
*******************************************************************************/
uint8 FLS_Set(uint8 templet, uint32 address, uint16 length)
{
    S_FLSINCHIP fs[FLS_CHIP_NO];
    uint8 err = 0;
    uint8 num;
    
    if(gul_FlashSafeCode != 0xA5A5A5A5)     //判断安全操作字是否打开
    {
        return FLS_ERR_FT;                  //返回致命错误
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
** @描述: flash硬件自检
** 
** 
** @参数: 
** 
** @返回: 
** @return:  uint8 位图.1:硬件故障,0:硬件正常.
**                 bit0-bit7分别表示第1-8块flash的硬件状态
** @作者: yzy (2010-05-12)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 TestFlashChip(void)
{
    uint8 cmp1[TEST_FLS_BYTES];             //原始数据
    uint8 cmp2[TEST_FLS_BYTES];             //修改后理论写入数据
    uint8 cmp3[TEST_FLS_BYTES];             //修改后实际写入数据
    uint32 chip_addr;                        //片内地址
    uint8 err = 0;
    
    for(uint8 k = 0; k < FLS_CHIP_NO; k++)
    {
        for(uint8 w = 0; w < 2; w++)        //测试两次:一次片头,一次片尾(确保容量正确)
        {
            chip_addr = w ? (guls_FLSChipSize[k] - TEST_FLS_BYTES) : 0;
                                            //读取原始数据
            if(ReadFlsInChip(cmp1, chip_addr, TEST_FLS_BYTES, k) != SYS_ERR_OK)
            {
                err |= Bit_Map8[k];
                break;
            }
                                            //修改原始数据
            for(uint8 i = 0; i < TEST_FLS_BYTES; i++)
            {
                cmp2[i] = cmp1[i] + 2;
            }
                                            //写入理论写入数据
            if(WriteFlsInChip(cmp2, chip_addr, TEST_FLS_BYTES, k, 0) != SYS_ERR_OK)
            {
                err |= Bit_Map8[k];
                break;
            }
                                            //读取实际写入数据
            if(ReadFlsInChip(cmp3, chip_addr, TEST_FLS_BYTES, k) != SYS_ERR_OK)
            {
                err |= Bit_Map8[k];
                break;
            }
                                            //恢复原始值
            WriteFlsInChip(cmp1, chip_addr, TEST_FLS_BYTES, k, 0);
                                            //比对理论实际值
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
** @描述: Flash读取ID
** 
** 
** @参数: 
** @param: buffer(uint8*): 
** @param: x(uint32): 
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-12)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 Fls_ReadID(uint8* buffer, uint32 x)
{
    gs_FlashSPI[x].command[0] = MX_CMD_RDID;  //读取Flash ID命令
    gs_FlashSPI[x].data = buffer;
    gs_FlashSPI[x].cmdnum = 1;
    gs_FlashSPI[x].length = 3;
                                            //读取数据
    return SPI_Read(&gs_FlashSPI[x]);
}

/*******************************************************************************
** @function_name:  HAL_InitFlash
** @function_file:  mxflash.c
** @描述: 初始化Flash
** 
** @参数: 
** 
** @返回: 
** @return:  uint8   
** @作者: yzy (2010-05-12)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
uint8 HAL_InitFlash(void)
{
    gcp_FlsBuffer = (uint8*)SYS_GetMEM(LEN_FLS_SECTOR);
    guc_FlashErrStt = 0;                    //初始化错误状态字
    
    Init_SPI(&gs_FlashSpiPort);
    
    FLS_KillHwWp();

    for(uint32 x = 0; x < FLS_CHIP_NO; x++)
    {
                                            //设置cs线拉高操作的函数
        gs_FlashSPI[x].csup = gs_FlashGetStt[x].csup;
                                            //设置cs线置低操作的函数
        gs_FlashSPI[x].csdown = gs_FlashGetStt[x].csdown;
        
        FLS_ExitDP();

        uint8 buffer[3];
        Fls_ReadID(buffer, x);              //读取Flash ID
                                            //判断型号
        if((buffer[0] != FLS_TYPE_MX) && (buffer[0] != FLS_TYPE_EON))    
        {
            Fls_ReadID(buffer, x);          //重读一遍
                                            //判断状态
            if((buffer[0] != FLS_TYPE_MX) && (buffer[0] != FLS_TYPE_EON))
            {                               //发生错误,Flash不存在
                guc_FlashErrStt = FLS_ERR_EXSIT;
            }
        }
        
        FLS_Disable_WP(x);
        while(!FLS_FREE(x, true));          //等待Flash空闲
    }
    
    if(guc_FlashErrStt)
    {
        return FLS_ERR_FT;                  //返回发生致命错误
    }
    else
    {
        SYS_OK();                            //返回初始化成功
    }
}
