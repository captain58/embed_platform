/******************************Copyright(c)******************************
**                          
**
** File Name: innerflash.c
** Author: 
** Date Last Update: 2015-05-18
** Description: 单片机内部 Flash驱动程序:
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: 文件创建
*************************************************************************/
#define INNER_FLASH

#include "sys.h"
#include "hal.h"


#define FLS_ERR_OK      (uint8_t)0
#define FLS_ERR_VAR     (uint8_t)1
#define FLS_ERR_FT      (uint8_t)2

#define FLS_ERR_EXSIT   (uint8)0x10
#define FLS_ERR_READ    (uint8)0x20
#define FLS_ERR_WRITE   (uint8)0x40


#define FLASH_SIZE     0x040000         //128k
#define FLASH_PAGE     2048              //512
#define FLASH_PAGE_MASK     (FLASH_PAGE-1)              //512
#define FLASH_ADDR_BASE     0x08000000
#define FLASH_BYTE_SIZE 2     
#define FLASH_PAGE_WRBYTE_MASK     (FLASH_PAGE-2)              //512
//	#define FLASH_BYTE_SIZE 4     

/************************************************************************
**sector 长度
 ************************************************************************/
#define  LEN_FLS_SECTOR  FLASH_PAGE        //sector 长度

#define WRITE_PROTECTION_DISABLE
#define FMC_PAGES_PROTECTED (OB_WP_6 | OB_WP_7)
__IO fmc_state_enum fmc_state = FMC_READY;

/************************************************************************
**定义FLASH Buffer
 ************************************************************************/
uint8* gp_ucfBuffer;

//扇区擦函数(每个扇区512字节) 
//输入参数：SectorNum   需要擦除扇区的扇区号
//			OperateKey  flash操作验证码(出于可靠性考虑，具体数据用户自定义，存储在ee或者flash中，操作flash前读取赋值给OperateKey)
uint8_t Flash_Erase_Sector( uint32_t address )
{
	uint16_t i;
	uint8_t Result = 0;
	uint32_t *PFlash;
    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    address += FLASH_ADDR_BASE;
//	if(SectorNum < 32) return 2;//禁止擦除boot区
	PFlash = (uint32_t *)(uint32_t)(address);
//		if( OperateKey == FLASHOPKEY )
//		{
//	RCC_PERCLK_SetableEx(FLSEPCLK, ENABLE);	//Flash擦写控制器时钟使能，用完就关
//		}
//		FLASH_Erase_Sector( address );
//	RCC_PERCLK_SetableEx(FLSEPCLK, DISABLE);	//Flash擦写控制器时钟使能，用完就关
	fmc_page_erase(address);
	for( i=0;i<128;i++ )
	{
		if( PFlash[i] != 0xFFFFFFFF ) 
		{
			Result = 1;
			break;
		}
	}
	
	return Result;
}
//连续写flash函数
//输入参数：prog_addr  被写flash首地址
//			prog_data  待写数据缓冲区
//			Len        写入长度(一次不要写入太多，以免看门狗溢出)
//			OperateKey  flash操作验证码(出于可靠性考虑，具体数据用户自定义，存储在ee或者flash中，操作flash前读取赋值给OperateKey)
uint8_t Flsah_Write_String( uint32_t prog_addr,uint8_t* prog_data, uint16_t Len )
{
	uint16_t i;
	uint8_t Result = 0;
	uint8_t *PFlash;
	uint8_t step = 2;
    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    fmc_state_enum sta = FMC_READY;
    prog_addr += FLASH_ADDR_BASE;
//		if( OperateKey == FLASHOPKEY )
//		{
//	RCC_PERCLK_SetableEx(FLSEPCLK, ENABLE);	//Flash擦写控制器时钟使能，用完就关
//		}
//		FLASH_Prog_ByteString( prog_addr, prog_data, Len);
//	RCC_PERCLK_SetableEx(FLSEPCLK, DISABLE);	//Flash擦写控制器时钟使能，用完就关
//		sta = fmc_halfword_program(Address, data);
    for (i = 0; i < Len; i += step)
    {
        __disable_irq();
//	        ret = HAL_FLASH_Program(TypeProgram,
//	                              address + i,
//	                              *(pData + (i / step)));
        sta = fmc_halfword_program(prog_addr + i, *(uint16_t *)(prog_data + i));
        __enable_irq();
        if (sta != FMC_READY)
        {
            break;
        }
    }


	PFlash = (uint8_t*)prog_addr;
	for( i=0;i<Len;i++ )
	{
		if( PFlash[i] != prog_data[i] ) 
		{
			Result = 1;
			break;
		}
	}	
	
	return Result;
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
uint8 HAL_IFLASH_Read(uint8* buffer, uint32_t address, uint16_t length)
{

    SYS_VAR_CHECK(buffer == NULL);
    SYS_VAR_CHECK(length == 0);
    
    uint8_t *PFlash;

    address += FLASH_ADDR_BASE;
    PFlash = (uint8_t*)address;
	for( int i=0;i<length;i++ )
	{
	    buffer[i] = PFlash[i];
//			if( PFlash[i] != prog_data[i] ) 
//			{
//				Result = 1;
//				break;
//			}
	}
    return 0;
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
uint8 HAL_IFLASH_Write(uint8* buffer, uint32 addr, uint16 length)
{
    //uint8  sidx, eidx;                      //写入的起始页和结束页
    uint8  fwri;                            //是否需要写入标志
    uint8  fera;                            //是否需要擦除标志
    uint8  m;
    //uint8  n;
    //uint32 addrInFlash;                     //FLASH的绝对地址
    uint32 addrInSector;                    //段内相对地址
    uint32 addrOfSector;                    //
    uint16 ui_len = 0;                      //单个SECTOR中写入的长度
                                            //读取的位置不对,不允许跨片读取
    SYS_VAR_CHECK(length + addr > FLASH_SIZE);
    SYS_VAR_CHECK(length == 0);

    /************************************************************************
    //循环处理每个需要更改的SECTOR
    ************************************************************************/
    while(true)                             //循环处理每个需要更改的SECTOR
    {
        if(length + (addr & FLASH_PAGE_MASK) > FLASH_PAGE)  //跨段
        {
            ui_len = FLASH_PAGE - (addr & FLASH_PAGE_MASK); //操作的数据长度
        }
        else                                //段内操作
        {
            ui_len = length;                //操作数据长度
        }

                                            //读取一段数据
        //while(SYS_ERR_OK != ReadFlsInChip(gp_ucfBuffer, addr & (~FLASH_PAGE_MASK), FLASH_PAGE));
        HAL_IFLASH_Read(gp_ucfBuffer, addr & (~FLASH_PAGE_MASK), FLASH_PAGE);
        /************************************************************************
        //循环判断是否需改写,改写前是否需擦除
        ************************************************************************/
        fwri = 0;                           //复位标志位
        fera = 0;
                                            //循环判断是否需改写部分是否需要擦除
        for(uint32 k = 0; k < ui_len; k++)
        {
            uint8 uc_data = gp_ucfBuffer[(addr & FLASH_PAGE_MASK) + k];
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
         
            addrOfSector = addr & (~FLASH_PAGE_MASK);
            addrInSector = addr & FLASH_PAGE_MASK;
                                            //转移需要写入的数据
            memcpy(gp_ucfBuffer + addrInSector, buffer, ui_len);
            if(fera)                        //需要擦除
            {
WRITE_DIR: 
                Flash_Erase_Sector(addrOfSector);
                                                // 
                for(m = 0; m < 3; m++)          //循环写,最多写3次
                {
                    if(Flsah_Write_String(addrOfSector, gp_ucfBuffer, FLASH_PAGE) != FLS_ERR_OK)
                    {
                        Flash_Erase_Sector(addrOfSector);
                    }
                    else
                    {
                        break;
                    }
                
                }
            }
            else                            //需要写入的页
            {
//	                sidx = addrInSector >> 8;
//	                eidx = (addrInSector + ui_len) >> 8;
//	                if((addrInSector + ui_len) & 0xFF)
//	                {
//	                    eidx += 1;
//	                }
//	                for(m = 0; m < 3; m++)          //循环写,最多写3次
//	                {
//	                    if(Flsah_Write_String(addr, buffer, ui_len) != FLS_ERR_OK)
//	                    {
//	//	                        Flash_Erase_Sector(addrOfSector);
//	                    }
//	                    else
//	                    {
//	                        break;
//	                    }
//	                
//	                }
                addrInSector = addr & FLASH_PAGE_WRBYTE_MASK;
                uint16_t wr_len = (ui_len + FLASH_BYTE_SIZE - 1) / FLASH_BYTE_SIZE * FLASH_BYTE_SIZE;
                for(m = 0; m < 3; m++)          //循环写,最多写3次
                {
                    if(Flsah_Write_String(addr&(~(FLASH_BYTE_SIZE-1)), (uint8_t *)(gp_ucfBuffer + addrInSector), wr_len) != FLS_ERR_OK)
                    {
//                          Flash_Erase_Sector(addrOfSector);
                        goto WRITE_DIR;
                    }
                    else
                    {
                        break;
                    }
                
                }

            }
                                            //转移需要写入的数据
//	            MoveBuffer(buffer, gcp_FlsBuffer + addrInSector, ui_len);
//	                                            // 
//	            for(m = 0; m < 3; m++)          //循环写,最多写3次
//	            {
//	
//	                if(Flsah_Write_String(gcp_FlsBuffer, addrOfSector, FLASH_PAGE) != FLS_ERR_OK)
//	                {
//	                    Flash_Erase_Sector(addrOfSector);
//	                }
//	                else
//	                {
//	                    break;
//	                }
//	  
//	            }
            
            if(m >= 3)
            {
                //guc_FlashErrStt |= FLS_ERR_WRITE;//标志写入数据发生错误
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
    //SYS_OK();
}

/*******************************************************************************
** @function_name:  HAL_IFLASH_Set
** @function_file:  Iflash.c
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
uint8 HAL_IFLASH_Set(uint8 templet, uint32 addr, uint16 length)
{
//    uint8  sidx;
//    uint8_t eidx;                      //写入的起始页和结束页
    uint8  fwri;                            //是否需要写入标志
    uint8  fera;                            //是否需要擦除标志
    uint8  m;
//    uint8  n;
    //uint32 addrInFlash;                     //FLASH的绝对地址
    uint32 addrInSector;                    //段内相对地址
    uint32 addrOfSector;                    //
    uint16 ui_len = 0;                      //单个SECTOR中写入的长度
                                            //读取的位置不对,不允许跨片读取
    SYS_VAR_CHECK(length + addr > FLASH_SIZE);
    SYS_VAR_CHECK(length == 0);

    /************************************************************************
    //循环处理每个需要更改的SECTOR
    ************************************************************************/
    while(true)                             //循环处理每个需要更改的SECTOR
    {
        if(length + (addr & FLASH_PAGE_MASK) > FLASH_PAGE)  //跨段
        {
            ui_len = FLASH_PAGE - (addr & FLASH_PAGE_MASK); //操作的数据长度
        }
        else                                //段内操作
        {
            ui_len = length;                //操作数据长度
        }
        
                                            //读取一段数据
//	        while(SYS_ERR_OK != ReadFlsInChip(gcp_FlsBuffer, addr & (~0xFFF), 4096, x));
        
        HAL_IFLASH_Read(gp_ucfBuffer, addr & (~FLASH_PAGE_MASK), FLASH_PAGE);
        /************************************************************************
        //循环判断是否需改写,改写前是否需擦除
        ************************************************************************/
        fwri = 0;                           //复位标志位
        fera = 0;
                                            //循环判断是否需改写部分是否需要擦除
        for(uint32 k = 0; k < ui_len; k++)
        {
            uint8 uc_data = gp_ucfBuffer[(addr & FLASH_PAGE_MASK) + k];//gcp_FlsBuffer[(addr & 0xFFF) + k];
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
            addrOfSector = addr & (~FLASH_PAGE_MASK);
            addrInSector = addr & FLASH_PAGE_MASK;
            memset(gp_ucfBuffer + addrInSector, templet, ui_len);

            if(fera)                        //需要擦除
            {
                Flash_Erase_Sector(addrOfSector);
                                                //转移需要写入的数据
//	                memcpy_s(gp_ucfBuffer + addrInSector, buffer, ui_len);
                                                // 
                for(m = 0; m < 3; m++)          //循环写,最多写3次
                {
                    if(Flsah_Write_String(addrOfSector, gp_ucfBuffer, FLASH_PAGE) != FLS_ERR_OK)
                    {
                        Flash_Erase_Sector(addrOfSector);
                    }
                    else
                    {
                        break;
                    }
                
                }            
//	                EraseFlsSectorInChip(addrOfSector, x);
//	                sidx = 0;                   //整段需要重新写入
//	                eidx = 16;
            }
            else                            //需要写入的页
            {
                addrInSector = addr & FLASH_PAGE_WRBYTE_MASK;
                uint16_t wr_len = (ui_len + FLASH_BYTE_SIZE - 1) / FLASH_BYTE_SIZE * FLASH_BYTE_SIZE;

                for(m = 0; m < 3; m++)          //循环写,最多写3次
                {
                    if(Flsah_Write_String(addr, gp_ucfBuffer + addrInSector, wr_len) != FLS_ERR_OK)
                    {
//                          Flash_Erase_Sector(addrOfSector);
                    }
                    else
                    {
                        break;
                    }
                
                }


            
//	                sidx = addrInSector >> 8;
//	                eidx = (addrInSector + ui_len) >> 8;
//	                if((addrInSector + ui_len) & 0xFF)
//	                {
//	                    eidx += 1;
//	                }
            }
                                                //置位需要写入的数据
//	            memset(gcp_FlsBuffer + (addr & 0xFFF), templet, ui_len);
//	            
//	            for(m = 0; m < 3; m++)          //循环写,最多写3次
//	            {
//	                for(n = sidx; n < eidx; n++)//逐页写入
//	                {
//	                    if(WriteFlsPageInChip(gcp_FlsBuffer + (n << 8), addrOfSector + (n << 8), 256, x) != FLS_ERR_OK)
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
            if(m >= 3)
            {
//	                guc_FlashErrStt |= FLS_ERR_WRITE;//标志写入数据发生错误
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
** @function_name:  HAL_IFLASH_Erase
** @function_file:  Iflash.c
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
uint8 HAL_IFLASH_Erase(uint32 address, uint32 sectornum)
{
    uint32 ul_i = 0;
    
    if(address & FLASH_PAGE_MASK)
    {
        return FLS_ERR_VAR;
    }
                                        //循环擦除
    while(ul_i < sectornum)
    {
        //                              //喂狗:避免执行该函数时,喂狗进程等待资源时间过长,导致复位
//	        IWDT_Clr();//HAL_WDT_Feed();
        //
        
        Flash_Erase_Sector(address);
        ul_i++;
        address += FLASH_PAGE;
    }

    return FLS_ERR_OK;
}
/*******************************************************************************
** @function_name:  HAL_IFLASH_WriteSpec
** @function_file:  Iflash.c
** @描述: Flash 写操作
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 待写入数据的存放缓存
** @param: address(unsigned long): flash片内地址
** @param: length(unsigned short): 数据长度
** @param: sdmode(unsigned char): 扇区数据保留模式.[0.保留扇区所有(默认),1.保留扇区位于待写数据之前的数据,2.删除扇区所有]
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy(20111-01-10)
** @修改说明: 更改函数实现方式,简化程序
*******************************************************************************/
unsigned char HAL_IFLASH_WriteSpec(unsigned char* buffer, unsigned long addr, unsigned short length, unsigned char sdmode)
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
    SYS_VAR_CHECK(length + addr > FLASH_SIZE);//只能使用一半容量
    SYS_VAR_CHECK(length == 0);
    SYS_VAR_CHECK(sdmode != 1);

    /************************************************************************
    //循环处理每个需要更改的SECTOR
    ************************************************************************/
    while(true)                             //循环处理每个需要更改的SECTOR
    {
        //addrOfSector = addr & (~0xFFF);
        addrInSector = addr & FLASH_PAGE_MASK;
        if(length + addrInSector > FLASH_PAGE)    //跨段
        {
            ui_len = FLASH_PAGE - addrInSector;   //操作的数据长度
        }
        else                                //段内操作
        {
            ui_len = length;                //操作数据长度
        }

        for(m = 0; m < 3; m++)          //循环写,最多写3次
        {
            if(Flsah_Write_String(addr, buffer, ui_len) == FLS_ERR_OK)
//	            if(FLS_WritePage(buffer, addr, ui_len) == SYS_ERR_OK)
            {
                break;
            }
        }
        
        if(m >= 3)
        {
//	            guc_FlashErrStt |= FLS_ERR_WRITE;//标志写入数据发生错误
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

uint8 HAL_IFlash_Init(void)
{
    gp_ucfBuffer = (uint8*)m_malloc(LEN_FLS_SECTOR);
    uint32_t wp_value = 0xFFFFFFFF, protected_pages = 0x0;
    /* unlock the flash program/erase controller */
    fmc_unlock();
    ob_unlock();

    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);

    /* Get pages write protection status */
    wp_value = ob_write_protection_get();

#ifdef WRITE_PROTECTION_DISABLE
    /* Get pages already write protected */
    protected_pages = ~(wp_value | FMC_PAGES_PROTECTED);
  
    /* Check if desired pages are already write protected */
    if((wp_value | (~FMC_PAGES_PROTECTED)) != 0xFFFFFFFF ){
        /* Erase all the option Bytes */
        fmc_state = ob_erase();

        /* Check if there is write protected pages */
        if(protected_pages != 0x0){
            /* Restore write protected pages */
            fmc_state = ob_write_protection_enable(protected_pages);
        }
        /* Generate System Reset to load the new option byte values */
        NVIC_SystemReset();
    }

#elif defined WRITE_PROTECTION_ENABLE
    /* Get current write protected pages and the new pages to be protected */
    protected_pages =  (~wp_value) | FMC_PAGES_PROTECTED; 

    /* Check if desired pages are not yet write protected */
    if(((~wp_value) & FMC_PAGES_PROTECTED )!= FMC_PAGES_PROTECTED){

        /* Erase all the option Bytes because if a program operation is 
        performed on a protected page, the Flash memory returns a 
        protection error */
        fmc_state = ob_erase();

        /* Enable the pages write protection */
        fmc_state = ob_write_protection_enable(protected_pages);

        /* Generate System Reset to load the new option byte values */
        NVIC_SystemReset();
    }
#endif /* WRITE_PROTECTION_DISABLE */



    
    return 0;
}

