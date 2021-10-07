/******************************Copyright(c)******************************
**                          
**
** File Name: innerflash.c
** Author: 
** Date Last Update: 2015-05-18
** Description: ��Ƭ���ڲ� Flash��������:
** Note: 
*******************************History***********************************
** Date: 2019-11-02
** Author: yzy
** Description: �ļ�����
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
**sector ����
 ************************************************************************/
#define  LEN_FLS_SECTOR  FLASH_PAGE        //sector ����

#define WRITE_PROTECTION_DISABLE
#define FMC_PAGES_PROTECTED (OB_WP_6 | OB_WP_7)
__IO fmc_state_enum fmc_state = FMC_READY;

/************************************************************************
**����FLASH Buffer
 ************************************************************************/
uint8* gp_ucfBuffer;

//����������(ÿ������512�ֽ�) 
//���������SectorNum   ��Ҫ����������������
//			OperateKey  flash������֤��(���ڿɿ��Կ��ǣ����������û��Զ��壬�洢��ee����flash�У�����flashǰ��ȡ��ֵ��OperateKey)
uint8_t Flash_Erase_Sector( uint32_t address )
{
	uint16_t i;
	uint8_t Result = 0;
	uint32_t *PFlash;
    fmc_flag_clear(FMC_FLAG_BANK0_END);
    fmc_flag_clear(FMC_FLAG_BANK0_WPERR);
    fmc_flag_clear(FMC_FLAG_BANK0_PGERR);
    address += FLASH_ADDR_BASE;
//	if(SectorNum < 32) return 2;//��ֹ����boot��
	PFlash = (uint32_t *)(uint32_t)(address);
//		if( OperateKey == FLASHOPKEY )
//		{
//	RCC_PERCLK_SetableEx(FLSEPCLK, ENABLE);	//Flash��д������ʱ��ʹ�ܣ�����͹�
//		}
//		FLASH_Erase_Sector( address );
//	RCC_PERCLK_SetableEx(FLSEPCLK, DISABLE);	//Flash��д������ʱ��ʹ�ܣ�����͹�
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
//����дflash����
//���������prog_addr  ��дflash�׵�ַ
//			prog_data  ��д���ݻ�����
//			Len        д�볤��(һ�β�Ҫд��̫�࣬���⿴�Ź����)
//			OperateKey  flash������֤��(���ڿɿ��Կ��ǣ����������û��Զ��壬�洢��ee����flash�У�����flashǰ��ȡ��ֵ��OperateKey)
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
//	RCC_PERCLK_SetableEx(FLSEPCLK, ENABLE);	//Flash��д������ʱ��ʹ�ܣ�����͹�
//		}
//		FLASH_Prog_ByteString( prog_addr, prog_data, Len);
//	RCC_PERCLK_SetableEx(FLSEPCLK, DISABLE);	//Flash��д������ʱ��ʹ�ܣ�����͹�
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
uint8 HAL_IFLASH_Write(uint8* buffer, uint32 addr, uint16 length)
{
    //uint8  sidx, eidx;                      //д�����ʼҳ�ͽ���ҳ
    uint8  fwri;                            //�Ƿ���Ҫд���־
    uint8  fera;                            //�Ƿ���Ҫ������־
    uint8  m;
    //uint8  n;
    //uint32 addrInFlash;                     //FLASH�ľ��Ե�ַ
    uint32 addrInSector;                    //������Ե�ַ
    uint32 addrOfSector;                    //
    uint16 ui_len = 0;                      //����SECTOR��д��ĳ���
                                            //��ȡ��λ�ò���,�������Ƭ��ȡ
    SYS_VAR_CHECK(length + addr > FLASH_SIZE);
    SYS_VAR_CHECK(length == 0);

    /************************************************************************
    //ѭ������ÿ����Ҫ���ĵ�SECTOR
    ************************************************************************/
    while(true)                             //ѭ������ÿ����Ҫ���ĵ�SECTOR
    {
        if(length + (addr & FLASH_PAGE_MASK) > FLASH_PAGE)  //���
        {
            ui_len = FLASH_PAGE - (addr & FLASH_PAGE_MASK); //���������ݳ���
        }
        else                                //���ڲ���
        {
            ui_len = length;                //�������ݳ���
        }

                                            //��ȡһ������
        //while(SYS_ERR_OK != ReadFlsInChip(gp_ucfBuffer, addr & (~FLASH_PAGE_MASK), FLASH_PAGE));
        HAL_IFLASH_Read(gp_ucfBuffer, addr & (~FLASH_PAGE_MASK), FLASH_PAGE);
        /************************************************************************
        //ѭ���ж��Ƿ����д,��дǰ�Ƿ������
        ************************************************************************/
        fwri = 0;                           //��λ��־λ
        fera = 0;
                                            //ѭ���ж��Ƿ����д�����Ƿ���Ҫ����
        for(uint32 k = 0; k < ui_len; k++)
        {
            uint8 uc_data = gp_ucfBuffer[(addr & FLASH_PAGE_MASK) + k];
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
         
            addrOfSector = addr & (~FLASH_PAGE_MASK);
            addrInSector = addr & FLASH_PAGE_MASK;
                                            //ת����Ҫд�������
            memcpy(gp_ucfBuffer + addrInSector, buffer, ui_len);
            if(fera)                        //��Ҫ����
            {
WRITE_DIR: 
                Flash_Erase_Sector(addrOfSector);
                                                // 
                for(m = 0; m < 3; m++)          //ѭ��д,���д3��
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
            else                            //��Ҫд���ҳ
            {
//	                sidx = addrInSector >> 8;
//	                eidx = (addrInSector + ui_len) >> 8;
//	                if((addrInSector + ui_len) & 0xFF)
//	                {
//	                    eidx += 1;
//	                }
//	                for(m = 0; m < 3; m++)          //ѭ��д,���д3��
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
                for(m = 0; m < 3; m++)          //ѭ��д,���д3��
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
                                            //ת����Ҫд�������
//	            MoveBuffer(buffer, gcp_FlsBuffer + addrInSector, ui_len);
//	                                            // 
//	            for(m = 0; m < 3; m++)          //ѭ��д,���д3��
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
                //guc_FlashErrStt |= FLS_ERR_WRITE;//��־д�����ݷ�������
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
    //SYS_OK();
}

/*******************************************************************************
** @function_name:  HAL_IFLASH_Set
** @function_file:  Iflash.c
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
uint8 HAL_IFLASH_Set(uint8 templet, uint32 addr, uint16 length)
{
//    uint8  sidx;
//    uint8_t eidx;                      //д�����ʼҳ�ͽ���ҳ
    uint8  fwri;                            //�Ƿ���Ҫд���־
    uint8  fera;                            //�Ƿ���Ҫ������־
    uint8  m;
//    uint8  n;
    //uint32 addrInFlash;                     //FLASH�ľ��Ե�ַ
    uint32 addrInSector;                    //������Ե�ַ
    uint32 addrOfSector;                    //
    uint16 ui_len = 0;                      //����SECTOR��д��ĳ���
                                            //��ȡ��λ�ò���,�������Ƭ��ȡ
    SYS_VAR_CHECK(length + addr > FLASH_SIZE);
    SYS_VAR_CHECK(length == 0);

    /************************************************************************
    //ѭ������ÿ����Ҫ���ĵ�SECTOR
    ************************************************************************/
    while(true)                             //ѭ������ÿ����Ҫ���ĵ�SECTOR
    {
        if(length + (addr & FLASH_PAGE_MASK) > FLASH_PAGE)  //���
        {
            ui_len = FLASH_PAGE - (addr & FLASH_PAGE_MASK); //���������ݳ���
        }
        else                                //���ڲ���
        {
            ui_len = length;                //�������ݳ���
        }
        
                                            //��ȡһ������
//	        while(SYS_ERR_OK != ReadFlsInChip(gcp_FlsBuffer, addr & (~0xFFF), 4096, x));
        
        HAL_IFLASH_Read(gp_ucfBuffer, addr & (~FLASH_PAGE_MASK), FLASH_PAGE);
        /************************************************************************
        //ѭ���ж��Ƿ����д,��дǰ�Ƿ������
        ************************************************************************/
        fwri = 0;                           //��λ��־λ
        fera = 0;
                                            //ѭ���ж��Ƿ����д�����Ƿ���Ҫ����
        for(uint32 k = 0; k < ui_len; k++)
        {
            uint8 uc_data = gp_ucfBuffer[(addr & FLASH_PAGE_MASK) + k];//gcp_FlsBuffer[(addr & 0xFFF) + k];
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
            addrOfSector = addr & (~FLASH_PAGE_MASK);
            addrInSector = addr & FLASH_PAGE_MASK;
            memset(gp_ucfBuffer + addrInSector, templet, ui_len);

            if(fera)                        //��Ҫ����
            {
                Flash_Erase_Sector(addrOfSector);
                                                //ת����Ҫд�������
//	                memcpy_s(gp_ucfBuffer + addrInSector, buffer, ui_len);
                                                // 
                for(m = 0; m < 3; m++)          //ѭ��д,���д3��
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
//	                sidx = 0;                   //������Ҫ����д��
//	                eidx = 16;
            }
            else                            //��Ҫд���ҳ
            {
                addrInSector = addr & FLASH_PAGE_WRBYTE_MASK;
                uint16_t wr_len = (ui_len + FLASH_BYTE_SIZE - 1) / FLASH_BYTE_SIZE * FLASH_BYTE_SIZE;

                for(m = 0; m < 3; m++)          //ѭ��д,���д3��
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
                                                //��λ��Ҫд�������
//	            memset(gcp_FlsBuffer + (addr & 0xFFF), templet, ui_len);
//	            
//	            for(m = 0; m < 3; m++)          //ѭ��д,���д3��
//	            {
//	                for(n = sidx; n < eidx; n++)//��ҳд��
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
//	                if(n >= eidx)               //д��ɹ�,����
//	                {
//	                    break;
//	                }
//	            }
//	            
            if(m >= 3)
            {
//	                guc_FlashErrStt |= FLS_ERR_WRITE;//��־д�����ݷ�������
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
** @function_name:  HAL_IFLASH_Erase
** @function_file:  Iflash.c
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
uint8 HAL_IFLASH_Erase(uint32 address, uint32 sectornum)
{
    uint32 ul_i = 0;
    
    if(address & FLASH_PAGE_MASK)
    {
        return FLS_ERR_VAR;
    }
                                        //ѭ������
    while(ul_i < sectornum)
    {
        //                              //ι��:����ִ�иú���ʱ,ι�����̵ȴ���Դʱ�����,���¸�λ
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
** @����: Flash д����
** 
** 
** @����: 
** @param: buffer(unsigned char*): ��д�����ݵĴ�Ż���
** @param: address(unsigned long): flashƬ�ڵ�ַ
** @param: length(unsigned short): ���ݳ���
** @param: sdmode(unsigned char): �������ݱ���ģʽ.[0.������������(Ĭ��),1.��������λ�ڴ�д����֮ǰ������,2.ɾ����������]
** 
** @����: 
** @return:  unsigned char   
** @����: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @�޸���: yzy(20111-01-10)
** @�޸�˵��: ���ĺ���ʵ�ַ�ʽ,�򻯳���
*******************************************************************************/
unsigned char HAL_IFLASH_WriteSpec(unsigned char* buffer, unsigned long addr, unsigned short length, unsigned char sdmode)
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
    SYS_VAR_CHECK(length + addr > FLASH_SIZE);//ֻ��ʹ��һ������
    SYS_VAR_CHECK(length == 0);
    SYS_VAR_CHECK(sdmode != 1);

    /************************************************************************
    //ѭ������ÿ����Ҫ���ĵ�SECTOR
    ************************************************************************/
    while(true)                             //ѭ������ÿ����Ҫ���ĵ�SECTOR
    {
        //addrOfSector = addr & (~0xFFF);
        addrInSector = addr & FLASH_PAGE_MASK;
        if(length + addrInSector > FLASH_PAGE)    //���
        {
            ui_len = FLASH_PAGE - addrInSector;   //���������ݳ���
        }
        else                                //���ڲ���
        {
            ui_len = length;                //�������ݳ���
        }

        for(m = 0; m < 3; m++)          //ѭ��д,���д3��
        {
            if(Flsah_Write_String(addr, buffer, ui_len) == FLS_ERR_OK)
//	            if(FLS_WritePage(buffer, addr, ui_len) == SYS_ERR_OK)
            {
                break;
            }
        }
        
        if(m >= 3)
        {
//	            guc_FlashErrStt |= FLS_ERR_WRITE;//��־д�����ݷ�������
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

