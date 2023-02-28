/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright HEX NET
 *
 *    File name   : archives.c
 *    Description : real meter map; white meter; black meter.
 *
 *    History :
 *    1. Date        : oct 12, 2016
 *       Author      : yzy
 *       Description : Create
 *
 **************************************************************************/
#define ARCHIVES_VAR

#include "archives.h"
#include "Nodes.h"
uint8 read_archive(uint8 * buf, uint32 addr, uint16 len)
{
    uint8 ierr = 0;
    while(BFLS_Read(buf, addr, len) && ierr < 3)
    {
        ierr++;
    }
    if(ierr >= 3)
    {
        LOG_DEBUG( DBGFMT" read meter err addr %x\n",DBGARG, addr);

        return SA_ERR_FT;
    }

    return SA_ERR_OK;
}

uint8 write_archive(uint8 * buf, uint32 addr, uint16 len)
{
    BFLS_PreWrite();
    uint8 ierr = 0;
    while( BFLS_Write(buf, addr, len) && ierr < 3)
    {
        ierr++;
    }
    if(ierr >= 3)
    {
        LOG_DEBUG( DBGFMT" upgrade meter err addr %x \n",DBGARG, addr);
        return SA_ERR_FT;
    }
    return SA_ERR_OK;
}

//更新采集器黑白名单的内容
//elem == NULL 时，表示删除此项
//	void Updata_SS_Hash_Table(uint16 hash, WB_SPACE *elem, uint8 type) 
//	{
//	    uint32 i;
//	    uint8 sector;
//	    uint8 location;
//	    WB_SPACE *p;
//	    p=( WB_SPACE *)updata_code_cache; 
//	    sector = hash / MAX_HASH_TABLE_CACHE_SIZE;
//	    location = sector * MAX_HASH_TABLE_CACHE_SIZE;		//取出表中的字段开始位置(256B)
//	
//		if(IapAction != IAP_ACTION_IDLE)		//内存保护
//			return;			//error
//	
//		//取出需要操作的内存区域
//		WB_SPACE* pSSSpace;
//		if(type == SS_VALIDATE_TYPE_WHITE)
//		{
//			if(0 == CurWhiteSSMap)
//				pSSSpace = WhiteSSMap1;
//			else
//				pSSSpace = WhiteSSMap2;
//		}
//		else if(type == SS_VALIDATE_TYPE_BLACK)
//		{
//			if(0 == CurBlackSSMap)
//				pSSSpace = BlackSSMap1;
//			else
//				pSSSpace = BlackSSMap2;	
//		}
//		else
//			return;			//error
//		
//		for (i = 0; i < MAX_HASH_TABLE_CACHE_SIZE; i++)
//		{
//			if(pSSSpace[location + i].status == OCCUPIED)//如果该哈希元素的值有效     
//			{
//				//把原来哈希表里的数据保存在缓存中
//				memcpy(p + i, pSSSpace + location + i, sizeof(WB_SPACE));	
//			}
//			else if(pSSSpace[location + i].status == EMPTY)
//			{   
//				// 如果该哈希元素的值无效                     
//				memset(&p[i], 0xFF, sizeof(WB_SPACE));
//			}
//			else if(pSSSpace[location + i].status == DEAD)
//			{  
//				// 如果该哈希元素的值无效                     
//				memset(&p[i], 0x0, sizeof(WB_SPACE)); 	//缓冲中的数都置成0X0***
//			}
//		}	
//		
//		if (elem != NULL)//如果有新添加的哈希元素        
//		{
//			// 更新哈希缓存
//			memcpy(p[hash - location].addr, elem->addr, 6); 		//把新添加的哈希表元素放到哈希缓存表
//			p[hash - location].status	= OCCUPIED;	
//		}
//		else
//		{
//			memset(&p[hash - location],0x0,sizeof(WB_SPACE));	//删除此项存储内容
//		}
//		
//		if(type == SS_VALIDATE_TYPE_WHITE)
//		{
//			if(0 == CurWhiteSSMap)
//			{
//				IapAction = IAP_ACTION_TRANSFER_WRITE_SS_HASH1;
//				HashUpdateSectorAdd = FLASH_SS_WHITE_MAP_ADD1 + sector * MAX_HASH_TABLE_CACHE_SIZE * 16;
//			}
//			else
//			{
//				IapAction = IAP_ACTION_TRANSFER_WRITE_SS_HASH2;
//				HashUpdateSectorAdd = FLASH_SS_WHITE_MAP_ADD2 + sector * MAX_HASH_TABLE_CACHE_SIZE * 16;
//			}
//		}
//		else if(type == SS_VALIDATE_TYPE_BLACK)
//		{
//			if(0 == CurBlackSSMap)
//			{
//				IapAction = IAP_ACTION_TRANSFER_BLACK_SS_HASH1;
//				HashUpdateSectorAdd = FLASH_SS_BLACK_MAP_ADD1 + sector * MAX_HASH_TABLE_CACHE_SIZE * 16;
//			}
//			else
//			{
//				IapAction = IAP_ACTION_TRANSFER_BLACK_SS_HASH2;
//				HashUpdateSectorAdd = FLASH_SS_BLACK_MAP_ADD2 + sector * MAX_HASH_TABLE_CACHE_SIZE * 16;
//			}
//		}
//		else 
//			return;
//		while (IapAction != IAP_ACTION_IDLE);   // hold on
//		
//		if(Write_Sucess==ERR)//如果失败
//		{
//			SS_Hash_Correct(HashUpdateSectorAdd, (uint8*)&updata_code_cache, type);		
//		}
//	}

uint8 Updata_SS_Hash_Table(uint16 hash, WB_SPACE *elem, uint8 type) 
{
    //uint32 i;
    //uint8 sector;
    //uint8 location;
    WB_SPACE *p;
    uint32 baseAddr = 0;
    p=( WB_SPACE *)updata_code_cache; 
    uint8 berr = 0;

	//取出需要操作的内存区域
	//WB_SPACE* pSSSpace;
	if(type == SS_VALIDATE_TYPE_WHITE)
	{
		if(0 == CurWhiteSSMap)
        {
            baseAddr = EXT_FLASH_WHITE_MAP1_START;
        }      
		else
        {    
            baseAddr = EXT_FLASH_WHITE_MAP2_START;
        }
	}
	else if(type == SS_VALIDATE_TYPE_BLACK)
	{
		if(0 == CurBlackSSMap)
        {      
            baseAddr = EXT_FLASH_BLACK_MAP1_START;
        }
		else
        {      
            baseAddr = EXT_FLASH_BLACK_MAP2_START;
        }
	}
	else
		return SA_ERR_FT;			//error
	
	if (elem != NULL)//如果有新添加的哈希元素        
	{
		// 更新哈希缓存
        
		memcpy(p[0].addr, (uint8 *)elem, 6); 		//把新添加的哈希表元素放到哈希缓存表
		p[0].status	= OCCUPIED;	
        
	}
	else
	{
		memset(&p[0],0x0,sizeof(WB_SPACE));	//删除此项存储内容
	}
    
//	    BFLS_PreWrite();
//	    uint8 ierr = 0;
//	    while( BFLS_Write((uint8 *)p, baseAddr + (hash * sizeof(WB_SPACE)), sizeof(WB_SPACE)) && ierr < 3)
//	    {
//	        ierr++;
//	    }
//	    if(ierr >= 3)
//	    {
//	        LOG_DEBUG( DBGFMT" upgrade meter err addr %x  id[%d]\n",DBGARG, baseAddr, hash);
//	        berr = 1;
//	    }
    if(SA_ERR_OK != write_archive((uint8 *)p, baseAddr + (hash * sizeof(WB_SPACE)), sizeof(WB_SPACE)))
    {
        berr = 1;
    }
	if(berr)//如果失败
	{
		SS_Hash_Correct(baseAddr + (hash * sizeof(WB_SPACE)), (uint8*)&updata_code_cache, type);		
	}
    
    return SA_ERR_OK;
}

/*********************************************************************
*哈希表有效数据转移
*
**********************************************************************/
void SS_Hash_Transfer(uint8 type)
{
	uint16 i;//,j;
	HASHT *p;
	p=(HASHT *)updata_code_cache;
    uint8 buf[TRANSFER_BUFFER_SIZE];

	WB_SPACE* pSSSpace1;
	//WB_SPACE* pSSSpace2;
	//uint8 CurMap = 0;
    uint32 /*i2cAddr = 0, */srcBaseAddr = 0, destBaseAddr = 0;
//		if(type == SS_VALIDATE_TYPE_WHITE)
//		{
//			CurMap = CurWhiteSSMap;
//			pSSSpace1 = WhiteSSMap1;
//			pSSSpace2 = WhiteSSMap2;
//		}
//		else if(type == SS_VALIDATE_TYPE_BLACK)
//		{
//			CurMap = CurBlackSSMap;
//			pSSSpace1 = BlackSSMap1;
//			pSSSpace2 = BlackSSMap2;	
//		}
//		else
//			return;

	if(type == SS_VALIDATE_TYPE_WHITE)
	{
		//CurMap = CurWhiteSSMap;
        //i2cAddr = FM_WRITE_HASH_USED;
		if(1 == CurWhiteSSMap)
        {
            srcBaseAddr = EXT_FLASH_WHITE_MAP1_START;
            destBaseAddr = EXT_FLASH_WHITE_MAP2_START;
        }
        else
        {
            srcBaseAddr = EXT_FLASH_WHITE_MAP2_START;
            destBaseAddr = EXT_FLASH_WHITE_MAP1_START;            
        }
    }
	else if(type == SS_VALIDATE_TYPE_BLACK)
	{
		//CurMap = CurBlackSSMap;
        //i2cAddr = FM_BLACK_HASH_USED;
        
		if(1 == CurBlackSSMap)
        {
            srcBaseAddr = EXT_FLASH_BLACK_MAP1_START;
            destBaseAddr = EXT_FLASH_BLACK_MAP2_START;
        }
        else
        {
            srcBaseAddr = EXT_FLASH_BLACK_MAP2_START;
            destBaseAddr = EXT_FLASH_BLACK_MAP1_START;            
        }
	}
	else
		return ;

	for(uint8 loop=0; loop < WB_BLOCK_NO; loop++)
	{
        memset(buf, 0, TRANSFER_BUFFER_SIZE);
        memset(p, 0xFF, TRANSFER_BUFFER_SIZE);

        if(SA_ERR_OK != read_archive(buf, srcBaseAddr + (loop * MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)), MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)))
        {
            return ;
        }

		for (i =0 ; i <MAX_HASH_TABLE_WB_SIZE; i++)
		{
            pSSSpace1 = (WB_SPACE*)&buf[i * sizeof(WB_SPACE)];
			if (pSSSpace1[i].status != DEAD) //如果该哈希元素的值有效    
			{
				memcpy(p + i, pSSSpace1 + i, sizeof(WB_SPACE));//把有效数据保存在缓存中						
			}
		}				

        if(SA_ERR_OK != write_archive((uint8 *)p, destBaseAddr + (loop * MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)), MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)))
        {
            return ;
        }

	}

    
//		switch(CurMap)
//		{
//			case 1:	//table1-->table2
//			{
//				for(j=0; j<WB_BLOCK_NO; j++)
//				{
//					for (i =0 ; i <MAX_HASH_TABLE_SECTOR_SIZE; i++)
//					{
//				        if (pSSSpace1[i+(j*MAX_HASH_TABLE_SECTOR_SIZE)].status != DEAD) //如果该哈希元素的值有效   
//			                memcpy(p+i, pSSSpace1 + i + j * MAX_HASH_TABLE_SECTOR_SIZE, sizeof(WB_SPACE));//把有效数据保存在缓存中	
//				        else
//			                memset(p+i, 0xff, sizeof(WB_SPACE)); //缓冲中的数都置成0XFF***
//					}		
//					if(type == SS_VALIDATE_TYPE_WHITE)
//					{
//						IapAction = IAP_ACTION_TRANSFER_WRITE_SS_HASH2;
//						HashUpdateSectorAdd = FLASH_SS_WHITE_MAP_ADD2 +(j*256);
//					}
//					else
//					{
//						IapAction = IAP_ACTION_TRANSFER_BLACK_SS_HASH2;
//						HashUpdateSectorAdd = FLASH_SS_BLACK_MAP_ADD2 +(j*256);					
//					}
//					while (IapAction != IAP_ACTION_IDLE);	
//				}
//				
//				if(type == SS_VALIDATE_TYPE_WHITE)
//					IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH1;	//最后擦除原表	     
//				else
//					IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH1;	//最后擦除原表	     	
//	   
//				while (IapAction != IAP_ACTION_IDLE);	
//				break;	
//			}
//			case 0:	//table2-->table1
//			{
//				for(j = 0; j < WB_BLOCK_NO; j++)	
//				{
//					for (i = 0; i < MAX_HASH_TABLE_SECTOR_SIZE; i++)
//					{
//						if (pSSSpace2[i + (j * MAX_HASH_TABLE_SECTOR_SIZE)].status!= DEAD) //如果该哈希元素的值有效    				            
//						         memcpy(p + i, pSSSpace2 + i + j * MAX_HASH_TABLE_SECTOR_SIZE, sizeof(WB_SPACE));//把有效数据保存在缓存中
//						else 	// 如果该哈希元素的值无效    
//						        memset(p+i, 0xff, sizeof(WB_SPACE)); //缓冲中的数都置成0XFF***
//					}
//					if(type == SS_VALIDATE_TYPE_WHITE)
//					{
//						IapAction = IAP_ACTION_TRANSFER_WRITE_SS_HASH1;
//						HashUpdateSectorAdd = FLASH_SS_WHITE_MAP_ADD1 +(j*256);
//					}
//					else
//					{
//						IapAction = IAP_ACTION_TRANSFER_BLACK_SS_HASH1;
//						HashUpdateSectorAdd = FLASH_SS_BLACK_MAP_ADD1 +(j*256);					
//					}
//					while (IapAction != IAP_ACTION_IDLE);	
//				}		
//				if(type == SS_VALIDATE_TYPE_WHITE)
//					IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH2;	//最后擦除原表	     
//				else
//					IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH2;	//最后擦除原表	     	
//					
//				while (IapAction != IAP_ACTION_IDLE);	
//				break;		
//	    	}
//	    }
}

/*************************************************************************
*FLASH写入错误的补救
*
**************************************************************************/
uint8 SS_Hash_Correct(uint32 dst, uint8* src, uint8 type)
{
	uint32 DestAddr = 0;
    uint32 srcBaseAddr = 0, destBaseAddr = 0, i2cAddr = 0;
	uint8 i,loop;//,ierr = 0;
    uint8 buf[TRANSFER_BUFFER_SIZE];
	WB_SPACE *p;
	p=(WB_SPACE *)updata_code_cache;
//	uint8 cache[256]={0};

//	memcpy(cache,src,256);//保存数据

	WB_SPACE* pSSSpace1;
	//WB_SPACE* pSSSpace2;
	uint8 CurMap = 0;
	//保存哈希表转化时需要的存储区域
	if(type == SS_VALIDATE_TYPE_WHITE)
	{
		CurMap = CurWhiteSSMap;
//			pSSSpace1 = WhiteSSMap1;
//			pSSSpace2 = WhiteSSMap2;
        i2cAddr = FM_WRITE_HASH_USED;
		if(0 == CurWhiteSSMap)
        {
		 	DestAddr = dst - EXT_FLASH_WHITE_MAP1_START + EXT_FLASH_WHITE_MAP2_START;
            srcBaseAddr = EXT_FLASH_WHITE_MAP1_START;
            destBaseAddr = EXT_FLASH_WHITE_MAP2_START;
        }
        else
        {
			//DestAddr = dst - FLASH_SS_WHITE_MAP_ADD2 + FLASH_SS_WHITE_MAP_ADD1;
		 	DestAddr = dst - EXT_FLASH_WHITE_MAP2_START + EXT_FLASH_WHITE_MAP1_START;
            srcBaseAddr = EXT_FLASH_WHITE_MAP2_START;
            destBaseAddr = EXT_FLASH_WHITE_MAP1_START;            
        }
    }
	else if(type == SS_VALIDATE_TYPE_BLACK)
	{
		CurMap = CurBlackSSMap;
//			pSSSpace1 = BlackSSMap1;
//			pSSSpace2 = BlackSSMap2;	
        i2cAddr = FM_BLACK_HASH_USED;
        
		if(0 == CurBlackSSMap)
        {
		 	DestAddr = dst - EXT_FLASH_BLACK_MAP1_START + EXT_FLASH_BLACK_MAP2_START;
            srcBaseAddr = EXT_FLASH_BLACK_MAP1_START;
            destBaseAddr = EXT_FLASH_BLACK_MAP2_START;
        }
        else
        {
			//DestAddr = dst - FLASH_SS_WHITE_MAP_ADD2 + FLASH_SS_WHITE_MAP_ADD1;
		 	DestAddr = dst - EXT_FLASH_BLACK_MAP2_START + EXT_FLASH_BLACK_MAP1_START;
            srcBaseAddr = EXT_FLASH_BLACK_MAP2_START;
            destBaseAddr = EXT_FLASH_BLACK_MAP1_START;            
        }
            
//			 	DestAddr = dst - FLASH_SS_BLACK_MAP_ADD1 + FLASH_SS_BLACK_MAP_ADD2;
//			 else
//				DestAddr = dst - FLASH_SS_BLACK_MAP_ADD2 + FLASH_SS_BLACK_MAP_ADD1;	
	}
	else
		return SA_ERR_FT;

//	    BFLS_PreWrite();
//	
//	    while(BFLS_Write((uint8 *)src, DestAddr, sizeof(WB_SPACE)) && ierr < 3)
//	    {
//	        ierr++;
//	    }
//	    if(ierr >= 3)
//	    {
//	        LOG_DEBUG( DBGFMT" transfer meter err addr %x \n",DBGARG, DestAddr);
//	    
//	        return SA_ERR_FT;
//	    }
    
	for(loop=0; loop < WB_BLOCK_NO; loop++)
	{
        memset(buf, 0, TRANSFER_BUFFER_SIZE);
        memset(p, 0xFF, TRANSFER_BUFFER_SIZE);

        if(SA_ERR_OK != read_archive(buf, srcBaseAddr + (loop * MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)), MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)))
        {
            return SA_ERR_FT;
        }

		for (i =0 ; i <MAX_HASH_TABLE_WB_SIZE; i++)
		{
            pSSSpace1 = (WB_SPACE*)&buf[i * sizeof(WB_SPACE)];
			if (pSSSpace1[i].status != DEAD) //如果该哈希元素的值有效    
			{
				memcpy(p + i, pSSSpace1 + i, sizeof(WB_SPACE));//把有效数据保存在缓存中						
			}
		}				

        if(SA_ERR_OK != write_archive((uint8 *)p, destBaseAddr + (loop * MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)), MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)))
        {
            return SA_ERR_FT;
        }

	}

    if(SA_ERR_OK != write_archive((uint8 *)src, DestAddr, sizeof(WB_SPACE)))
    {
        return SA_ERR_FT;
    }
    
//设置当前哈希表的区域值
    CurMap++;
	CurMap = CurMap % 2;
	I2cWrite(0xA0, (uint8*) &CurMap, i2cAddr, 1);    				

    
//		switch(CurMap)
//		{
//			case 0:	//table1-->table2
//			{
//				for(loop=0; loop < WB_BLOCK_NO; loop++)
//				{
//	                memset(buf, 0, TRANSFER_BUFFER_SIZE);
//	                memset(p, 0xFF, TRANSFER_BUFFER_SIZE);
//	
//	                ierr = 0;
//	                while(BFLS_Read(buf, srcBaseAddr + (loop * MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)), MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)) && ierr < 3)
//	                {
//	                    ierr++;
//	                }
//	                if(ierr >= 3)
//	                {
//	                    LOG_DEBUG( DBGFMT" read meter err addr %x  loop[%d]\n",DBGARG, srcBaseAddr, loop);
//	                
//	                    return SA_ERR_FT;
//	                }
//	            
//					for (i =0 ; i <MAX_HASH_TABLE_WB_SIZE; i++)
//					{
//	                    pSSSpace1 = (WB_SPACE*)&buf[0];
//						if (pSSSpace1[i].status != DEAD) //如果该哈希元素的值有效    
//						{
//							memcpy(p + i, pSSSpace1 + i, sizeof(WB_SPACE));//把有效数据保存在缓存中						
//						}
//					}				
//	                BFLS_PreWrite();
//	                ierr = 0;
//	                while(BFLS_Write((uint8 *)p, destBaseAddr + (loop * MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)), MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)) && ierr < 3)
//	                {
//	                    ierr++;
//	                }
//	                if(ierr >= 3)
//	                {
//	                    LOG_DEBUG( DBGFMT" upgrade meter err addr %x  loop[%d]\n",DBGARG, destBaseAddr, loop);
//	
//	                    return SA_ERR_FT;
//	                }
//				}
//				
//	
//				//设置当前哈希表的区域值
//				if(type == SS_VALIDATE_TYPE_WHITE)
//				{
//					CurWhiteSSMap++;
//					CurWhiteSSMap = CurWhiteSSMap % 2;
//					I2cWrite(0xA0, (uint8*) &CurWhiteSSMap, FM_WRITE_HASH_USED, 1);    				
//				}
//				else
//				{
//					CurBlackSSMap++;
//					CurBlackSSMap = CurBlackSSMap % 2;				
//					I2cWrite(0xA0, (uint8*) &CurBlackSSMap, FM_BLACK_HASH_USED, 1);    				
//				} 		
//				//Write_Sucess = SUCCESS;
//				break;	
//			}
//		
//			case 1://table2-->table1
//			{
//				for(loop=0; loop < WB_BLOCK_NO; loop++)
//				{
//					for (i =0 ; i <MAX_HASH_TABLE_CACHE_SIZE; i++)
//					{
//						if (pSSSpace2[i + (loop * MAX_HASH_TABLE_CACHE_SIZE)].status != DEAD) //如果该哈希元素的值有效    
//						{
//							memcpy(p + i, pSSSpace2 + i + loop * MAX_HASH_TABLE_CACHE_SIZE, sizeof(WB_SPACE));//把有效数据保存在缓存中						
//						}
//						else
//						{   // 如果该哈希元素的值无效                     
//							memset(p + i, 0xff, sizeof(WB_SPACE)); //缓冲中的数都置成0XFF***
//						}				
//					}		
//					
//					//设置当前哈希表的区域值
//					if(type == SS_VALIDATE_TYPE_WHITE)
//					{
//						IapAction = IAP_ACTION_TRANSFER_WRITE_SS_HASH1;
//						HashUpdateSectorAdd = FLASH_SS_WHITE_MAP_ADD1 +(loop*256);	
//					}
//					else
//					{
//						IapAction = IAP_ACTION_TRANSFER_BLACK_SS_HASH1;
//						HashUpdateSectorAdd = FLASH_SS_BLACK_MAP_ADD1 +(loop*256);					
//					} 
//					if(HashUpdateSectorAdd == DestAddr)
//					{
//						memcpy(p,src,256);
//					}	
//					while (IapAction != IAP_ACTION_IDLE);			
//				}
//	
//				if(type == SS_VALIDATE_TYPE_WHITE)
//					IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH2;	//最后擦除原表	     
//				else
//					IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH2;	//最后擦除原表	     	
//				while (IapAction != IAP_ACTION_IDLE);	
//	
//				//设置当前哈希表的区域值
//				if(type == SS_VALIDATE_TYPE_WHITE)
//				{
//					CurWhiteSSMap++;
//					CurWhiteSSMap = CurWhiteSSMap % 2;
//					I2cWrite(0xA0, (uint8*) &CurWhiteSSMap, FM_WRITE_HASH_USED, 1);    				
//				}
//				else
//				{
//					CurBlackSSMap++;
//					CurBlackSSMap = CurBlackSSMap % 2;				
//					I2cWrite(0xA0, (uint8*) &CurBlackSSMap, FM_BLACK_HASH_USED, 1);    				
//				} 		
//				Write_Sucess = SUCCESS;
//				break;		
//	        }
//	    }
    return SA_ERR_OK;

}


int SS_Hash_Table_Insert(WB_SPACE* elem, uint8 type) //添加哈希表
{
    uint16 hash;
    uint16 index;
    hash = (Cal_Hash_Value(elem->addr)) % MAX_HASH_TABLE_FLASH_SIZE; //计算得到哈希索引	
    uint32 baseAddr = 0;
    WB_SPACE WBpace;
//		if(type == SS_VALIDATE_TYPE_WHITE)
//		{
//			if(0 == CurWhiteSSMap)
//				pSSSpace = WhiteSSMap1;
//			else
//				pSSSpace = WhiteSSMap2;		
//		}
//		else //if(type == SS_VALIDATE_TYPE_BLACK)
//		{
//			if(0 == CurBlackSSMap)
//				pSSSpace = BlackSSMap1;
//			else
//				pSSSpace = BlackSSMap2;		
//		}
//		else
//			return 0;

	if(type == SS_VALIDATE_TYPE_WHITE)
	{
		if(0 == CurWhiteSSMap)
        {
            baseAddr = EXT_FLASH_WHITE_MAP1_START;
        }      
		else
        {    
            baseAddr = EXT_FLASH_WHITE_MAP2_START;
        }
	}
	else //if(type == SS_VALIDATE_TYPE_BLACK)
	{
		if(0 == CurBlackSSMap)
        {      
            baseAddr = EXT_FLASH_BLACK_MAP1_START;
        }
		else
        {      
            baseAddr = EXT_FLASH_BLACK_MAP2_START;
        }
	}
//		else
//			return SA_ERR_FT;	

    if(SA_ERR_OK != read_archive((uint8 *)&WBpace, baseAddr + (hash * sizeof(WB_SPACE)), sizeof(WB_SPACE)))
    {
        return ( - 1);
    }


	if(WBpace.status == EMPTY) //空
	{                        
		Updata_SS_Hash_Table(hash, elem, type); //更新哈希表
		return hash; //返回索引值并退出
	}
	else if ((WBpace.status==OCCUPIED) && WBpace.addrLen == elem->addrLen
        && (!LPC_Memory_CMP(WBpace.addr, elem->addr, elem->addrLen)))//该位被相同电表地址占用
	{
		// 器件ID一样，不再插入                             
		return ( - 1);
	}
	
	index = (hash + 1) % MAX_HASH_TABLE_FLASH_SIZE;	 //如果被别的数占据，则索引加1再判断
	while (hash != index)
	{
        if(SA_ERR_OK != read_archive((uint8 *)&WBpace, baseAddr + (index * sizeof(WB_SPACE)), sizeof(WB_SPACE)))
        {
            index = (index + 1) % MAX_HASH_TABLE_FLASH_SIZE; //加1、翻转
            continue;
        }

    
        if (WBpace.status==EMPTY)    //如果索引对应的位置为空
        {
            Updata_SS_Hash_Table(index, elem, type); //把哈希所以加1后添加进哈希表
            return index; //返回索引值并退出循环
        }
        else if ((WBpace.status==OCCUPIED) && (!LPC_Memory_CMP(WBpace.addr, elem->addr, elem->addrLen)))//该位被相同电表地址占用
        {
                // 器件ID一样，不再插入                                         		
            return ( - 1);
        }
        index = (index + 1) % MAX_HASH_TABLE_FLASH_SIZE; //加1、翻转
	}
	return (MAX_HASH_TABLE_FLASH_SIZE);	//哈希表已满
}
	
/******************************************************************
*查询采集器信息
*
******************************************************************/ 
void Search_Validate_SS(uint8 *data)
{
//		uint16 i,j,len,id;
//		uint8 handled_no;//已处理采集数量
//		uint8 request_no;//请求数量
//		uint8 report_no=0;//上报数量
//		uint8 up_pkt[200]={0};
//		WB_SPACE* pWriteSSSpace;
//		WB_SPACE* pBlackSSSpace;
//		uint8 bFound;
//		uint8* pData;
//		
//		len=*(data+1);//包长度
//		request_no=*(data+len-3);
//	
//		//开始传输时
//		//清除已处理标识位
//		if(*(data+13)==1)
//		{
//			for(i=0; i < MAX_VALIDATE_SS_NUM; i++)
//				SSMapShadowSpace[i]= UN_HANDLED;
//		}
//	
//		if(0 == CurWhiteSSMap)
//			pWriteSSSpace = WhiteSSMap1;
//		else
//			pWriteSSSpace = WhiteSSMap2;		
//	
//		if(0 == CurBlackSSMap)
//			pBlackSSSpace = BlackSSMap1;
//		else
//			pBlackSSSpace = BlackSSMap2;		
//		
//		if(len > 16)//标记已处理采集器
//		{
//			handled_no=(len-16) / 6;
//			for(i = 0; i < handled_no; i++)
//			{
//				bFound = 0;
//				pData = data + 14 + i * 6;
//				for(id = 0; id < MAX_SS_MAP_SIZE; id++)
//				{
//					if((pWriteSSSpace[id].status == OCCUPIED) && (!LPC_Memory_CMP(pWriteSSSpace[id].addr, pData, 6)))
//					{
//						SSMapShadowSpace[id] = HANDLED;
//						bFound = 1;
//						break;
//					}
//				}
//				if(0 == bFound)
//				{
//					for(id = 0; id < MAX_SS_MAP_SIZE; id++)
//					{
//						if((pBlackSSSpace[id].status == OCCUPIED) && (!LPC_Memory_CMP(pBlackSSSpace[id].addr, pData, 6)))
//						{
//							SSMapShadowSpace[id + MAX_SS_MAP_SIZE] = HANDLED;
//							break;
//						}
//					}
//				}
//			}
//		}
//	
//		for(j=0; j < MAX_SS_MAP_SIZE; j++)
//		{
//			if((SSMapShadowSpace[j] == UN_HANDLED)
//				&& (pWriteSSSpace[j].status == OCCUPIED)) //如果未处理且该SN被占用
//			{
//				pData = up_pkt+14+(report_no * 7);
//				memcpy(pData, pWriteSSSpace[j].addr, 6); //采集器地址
//				*(pData + 6) = 1;
//				report_no++; 
//			}
//			if(report_no >= request_no)
//				break;	 
//		}
//	
//		if(report_no < request_no)
//		{
//			for(j=0; j < MAX_SS_MAP_SIZE; j++)
//			{
//				if((SSMapShadowSpace[MAX_SS_MAP_SIZE + j] == UN_HANDLED) 
//					&& (pBlackSSSpace[j].status == OCCUPIED)) //如果未处理且该SN被占用
//				{
//					pData = up_pkt+14+(report_no * 7);
//					memcpy(pData, pBlackSSSpace[j].addr, 6); //采集器地址
//					*(pData + 6) = 0;
//					report_no++; 
//				}
//				if(report_no >= request_no)
//					break;	 
//			}		
//		}
//			
//		up_pkt[0]=0x68;
//		up_pkt[1]=16+(report_no*7);
//		up_pkt[2]=0;
//		up_pkt[3]=0x8a;
//		up_pkt[4]=0x1;
//		up_pkt[5]=0;
//		up_pkt[6]=0x50;
//		up_pkt[7]=0;
//		up_pkt[8]=0;
//		up_pkt[9]=0;
//	
//		if((bDebugSend) && (0 == bEZMACDebugType))
//		{
//			up_pkt[10]=0xF0;
//			up_pkt[11]=0x10;
//			up_pkt[12]=0x04;			
//		}
//		else
//		{
//			up_pkt[10]=0x3;
//			up_pkt[11]=0x08;
//			up_pkt[12]=0x02;		
//		}
//		up_pkt[13]=report_no;//采集器数量
//	
//		up_pkt[14+(report_no*7)] = Check_SUM(up_pkt + 3, 11+(report_no*7)); //校验位
//		up_pkt[15+(report_no*7)] = 0x16;
//		if(bEZMACDebugType)
//			SendPacketToEzmacProc(pEZMACRecvPacket, up_pkt, 16+(report_no*7), 0);
//		else			
//			UART_PutArray(UART0, up_pkt, 16+(report_no*7)); //回答
}
void Add_Validate_W_B(uint8* pAddr, uint8 len, uint8 type)
{
	uint16 localssloop; 
	uint16 ssloop;//, id;
	WB_SPACE SSTemp;
	uint8 bNeedAdd;
	int return_hash;
	uint8 ValidateSSType;
	uint8 bAddSuccess;
	uint32 destBaseAddr = 0;
    //uint8 CurMap = 0;
	WB_SPACE* pWBpace;
	//WB_SPACE* pBlackSSSpace;

    uint8 buf[TRANSFER_BUFFER_SIZE];
    HASHT* pHasht;
	if(HASH_Table_Used==0)//当前使用table1
 		pHasht = htable1;
	else
		pHasht = htable2;

	//取出当前黑白名单的存放位置
//		if(0 == CurWhiteSSMap)
//			pWriteSSSpace = WhiteSSMap1;
//		else
//			pWriteSSSpace = WhiteSSMap2;
//		if(0 == CurBlackSSMap)
//			pBlackSSSpace = BlackSSMap1;
//		else
//			pBlackSSSpace = BlackSSMap2;		

	if(type == SS_VALIDATE_TYPE_WHITE)
	{
		ValidateSSType = SS_VALIDATE_TYPE_WHITE;

		//CurWhiteSSMap;
		if(0 == CurWhiteSSMap)
        {
            destBaseAddr = EXT_FLASH_WHITE_MAP1_START;
        }
        else
        {
            destBaseAddr = EXT_FLASH_WHITE_MAP2_START;            
        }
    }
	else //(type == SS_VALIDATE_TYPE_BLACK)
	{
		ValidateSSType = SS_VALIDATE_TYPE_BLACK;
	
		//CurBlackSSMap;
        
		if(0 == CurBlackSSMap)
        {
            destBaseAddr = EXT_FLASH_BLACK_MAP1_START;
        }
        else
        {
            destBaseAddr = EXT_FLASH_BLACK_MAP2_START;            
        }
	}
    
    if(len > METER_ADDRESS_LENGTH_MAX)
    {
        len = METER_ADDRESS_LENGTH_MAX;
    }
	//取出要添加的采集器地址与信息
	memset((uint8 *)&SSTemp, 0, sizeof(WB_SPACE));
	memcpy(SSTemp.addr, pAddr, len);
	SSTemp.addrLen = len;
//		ValidateSSType = type;
//		if(ValidateSSType > 1)
//			ValidateSSType = SS_VALIDATE_TYPE_WHITE;

	//遍历白名单，查找是否有同地址的采集器
	bNeedAdd = 1;
	for(ssloop = 0; ssloop < MAX_HASH_TABLE_FLASH_SIZE; ssloop++)
	{
		if((pHasht[ssloop].status == OCCUPIED) && (!LPC_Memory_CMP(SSTemp.addr, pHasht[ssloop].addr, len)))
		{
			//名单中已有同地址的
			if(ValidateSSType == SS_VALIDATE_TYPE_BLACK)
			{
				//当添加的采集器信息为黑名单时，需要删除白名单
				//删除电表地址
				Updata_Hash_Table(ssloop, NULL);  //删除   
			}
			else if(ValidateSSType == SS_VALIDATE_TYPE_WHITE)
			{
				//当添加的采集器信息为白名单时，无需添加，直接返回
				bNeedAdd = 0;
				break;
			}
		}
	}
		
	if(bNeedAdd)
	{
		//遍历黑名单，判断是否有同地址
    	for(uint8 loop=0; loop < WB_BLOCK_NO; loop++)
    	{
            memset(buf, 0, TRANSFER_BUFFER_SIZE);
            //memset(p, 0xFF, TRANSFER_BUFFER_SIZE);

            if(SA_ERR_OK != read_archive(buf, destBaseAddr + (loop * MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)), MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)))
            {
                return ;//SA_ERR_FT;
            }

    		for (uint16 i =0 ; i <MAX_HASH_TABLE_WB_SIZE; i++)
    		{
                pWBpace = (WB_SPACE*)&buf[i * sizeof(WB_SPACE)];
    			if((pWBpace->status == OCCUPIED) && SSTemp.addrLen == pWBpace->addrLen 
                    && (!LPC_Memory_CMP(SSTemp.addr, pWBpace->addr, SSTemp.addrLen)))
    			{
    				//有同地址时，无需添加，直接返回
    				bNeedAdd = 0;
    				break;
    			}
    		}				
    	}
		
//			for(ssloop = 0; ssloop < MAX_HASH_TABLE_FLASH_SIZE; ssloop++)
//			{
//			
//				if((pBlackSSSpace[ssloop].status == OCCUPIED) && (!LPC_Memory_CMP(SSTemp.addr, pBlackSSSpace[ssloop].addr, 6)))
//				{
//					//有同地址时，无需添加，直接返回
//					bNeedAdd = 0;
//					break;
//				}
//			}
		if(bNeedAdd)
		{
			//需要添加此采集器信息
			return_hash = SS_Hash_Table_Insert(&SSTemp, ValidateSSType);
			bAddSuccess = FALSE;
			if(return_hash >= 0)
			{  
				if (return_hash == MAX_HASH_TABLE_FLASH_SIZE)
				{
					//当FLASH存储满时，需要转化存储区域
					if(ValidateSSType == SS_VALIDATE_TYPE_BLACK)
					{
						CurBlackSSMap =  (CurBlackSSMap + 1) % 2;
						I2cWrite(0xA0, &CurBlackSSMap, FM_BLACK_HASH_USED, 1);
					}
					else
					{
						CurWhiteSSMap = (CurWhiteSSMap + 1) % 2;
						I2cWrite(0xA0, &CurWhiteSSMap, FM_WRITE_HASH_USED, 1);					
					}
					SS_Hash_Transfer(ValidateSSType); 	//表满,转移哈希表
					return_hash = SS_Hash_Table_Insert(&SSTemp, ValidateSSType);	//再次插入上次未添加成功的
					if((return_hash >= 0) && (return_hash != MAX_SS_MAP_SIZE))
					{
						bAddSuccess = TRUE;
					}
				}
				else //如果添加成功
				{
					bAddSuccess = TRUE;
				}
			}		
			else
			{
				//有相同的采集器存在
			}
			
			//当添加黑名单成功时，需要离网本地相对应的采集器
			if( (bAddSuccess) && (ValidateSSType == SS_VALIDATE_TYPE_BLACK))
			{
				//需要离网相对应的采集器
				for(localssloop = 2; localssloop <= rfpara.rf_slotnum; localssloop++)
				{
					if(cltor[localssloop].devAddr[0] < 0xFF)
					{
						if(SSTemp.addrLen == cltor[localssloop].addrLen 
                            && !LPC_Memory_CMP(SSTemp.addr, cltor[localssloop].devAddr, cltor[localssloop].addrLen))
						{
							cltor[localssloop].nodestatus.bNeedDelete = TRUE;
                            cltor_shadow[localssloop].nodestatus.ans_pkt_type = 3;
                            if((!cltor_shadow[localssloop].nodestatus.bDataAck) && (!cltor_shadow[localssloop].nodestatus.bNetAck))
                            {
                                cltor_shadow[localssloop].FrameDeadTime = 0;
                            }
                            
							break;
						}
					}
				}					
			}
		}
	}
	
}

void Add_Validate_W_B_List(uint8* pData)
{
	uint8 AddNum = pData[13];
	uint8 SSType, loop, StartAddr;
	for(loop = 0; loop < AddNum; loop++)
	{
		//取出要添加的采集器地址与信息
		StartAddr = loop * 7 + 13 + 1;
		SSType = pData[StartAddr + 6];

		Add_Validate_W_B(pData + StartAddr, 6, SSType);
	}
	//SendReturnpkt(0);
}

void Del_Validate_W_B(uint8* pData, uint8 addrLen)				
{
	uint16 ssloop, localssloop;
	WB_SPACE SSTemp;
    uint8 buf[TRANSFER_BUFFER_SIZE];
	WB_SPACE* pWriteSSSpace;
	//WB_SPACE* pBlackSSSpace;
	uint8 bFound = 0;
    uint16 loop, i;
    uint32 whiteBaseAddr = 0, blackBaseAddr = 0;
	//取出当前黑白名单的存放位置
//		if(0 == CurWhiteSSMap)
//			pWriteSSSpace = WhiteSSMap1;
//		else
//			pWriteSSSpace = WhiteSSMap2;
//		if(0 == CurBlackSSMap)
//			pBlackSSSpace = BlackSSMap1;
//		else
//			pBlackSSSpace = BlackSSMap2;		


	if(0 == CurWhiteSSMap)
    {
        whiteBaseAddr = EXT_FLASH_WHITE_MAP1_START;
    }
    else
    {
        whiteBaseAddr = EXT_FLASH_WHITE_MAP2_START;            
    }
    
	if(0 == CurBlackSSMap)
    {
        blackBaseAddr = EXT_FLASH_BLACK_MAP1_START;
    }
    else
    {
        blackBaseAddr = EXT_FLASH_BLACK_MAP2_START;            
    }

	//取出采集器地址
	memset(&SSTemp, 0, sizeof(WB_SPACE));
	memcpy(SSTemp.addr, pData, addrLen);
	SSTemp.addrLen = addrLen;
//		//遍历白名单，查找采集器
//		for(ssloop = 0; ssloop < MAX_SS_MAP_SIZE; ssloop++)
//		{
//			if((pWriteSSSpace[ssloop].status == OCCUPIED) 
//				&& (!LPC_Memory_CMP(pWriteSSSpace[ssloop].addr, SSTemp.addr, 6)))
//			{
//				//需要离网相对应的采集器
//				if(bSSValidateEnable)
//				{
//					for(localssloop = 2; localssloop <= rfpara.rf_slotnum; localssloop++)
//					{
//						if(cltor[localssloop].devAddr[0] <= 0x99)
//						{
//							if(!LPC_Memory_CMP(pWriteSSSpace[ssloop].addr, cltor[localssloop].devAddr, 6))
//							{
//								//cltor[localssloop].nod.bNeedDelete = TRUE;
//								break;
//							}
//						}
//					}
//				}	
//				//删除采集器
//				Updata_SS_Hash_Table(ssloop, NULL, SS_VALIDATE_TYPE_WHITE);					
//				bFound = 1;
//				break;
//			}
//		}

	for(loop=0; loop < WB_BLOCK_NO; loop++)
	{
        memset(buf, 0, TRANSFER_BUFFER_SIZE);
        //memset(p, 0xFF, TRANSFER_BUFFER_SIZE);

        if(SA_ERR_OK != read_archive(buf, whiteBaseAddr + (loop * MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)), MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)))
        {
            return ;//SA_ERR_FT;
        }

		for (i =0 ; i <MAX_HASH_TABLE_WB_SIZE; i++)
		{
            pWriteSSSpace = (WB_SPACE*)&buf[i * sizeof(WB_SPACE)];
			if((pWriteSSSpace->status == OCCUPIED) && SSTemp.addrLen == pWriteSSSpace->addrLen 
                && (!LPC_Memory_CMP(SSTemp.addr, pWriteSSSpace->addr, SSTemp.addrLen)))
			{
			    //删除白名单
				for(localssloop = 2; localssloop < rfpara.rf_slotnum; localssloop++)
				{
					if(cltor[localssloop].devAddr[0] < 0xFF)
					{
						if(pWriteSSSpace->addrLen == cltor[localssloop].addrLen 
                            && !LPC_Memory_CMP(pWriteSSSpace->addr, cltor[localssloop].devAddr, cltor[localssloop].addrLen))
						{
							//cltor[localssloop].nod.bNeedDelete = TRUE;
                            cltor[localssloop].nodestatus.bNeedDelete = 1;
                            cltor_shadow[localssloop].nodestatus.ans_pkt_type = 3;
                            if((!cltor_shadow[localssloop].nodestatus.bDataAck) && (!cltor_shadow[localssloop].nodestatus.bNetAck))
                            {
                                cltor_shadow[localssloop].FrameDeadTime = 0;
                            }
							
							break;
						}
					}
				}
			    
    			Updata_SS_Hash_Table(ssloop, NULL, SS_VALIDATE_TYPE_WHITE);					
    			bFound = 1;
                break;
			}
		}				
	}

    if(0 == bFound)
    {
    	for(loop=0; loop < WB_BLOCK_NO; loop++)
    	{
            memset(buf, 0, TRANSFER_BUFFER_SIZE);
            //memset(p, 0xFF, TRANSFER_BUFFER_SIZE);

            if(SA_ERR_OK != read_archive(buf, blackBaseAddr + (loop * MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)), MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)))
            {
                return ;//SA_ERR_FT;
            }

    		for (i =0 ; i <MAX_HASH_TABLE_WB_SIZE; i++)
    		{
                pWriteSSSpace = (WB_SPACE*)&buf[i * sizeof(WB_SPACE)];
    			if((pWriteSSSpace->status == OCCUPIED) && SSTemp.addrLen == pWriteSSSpace->addrLen 
                    && (!LPC_Memory_CMP(SSTemp.addr, pWriteSSSpace->addr, SSTemp.addrLen)))
    			{
    			    //删除黑名单
    			    
    				Updata_SS_Hash_Table(ssloop, NULL, SS_VALIDATE_TYPE_BLACK);
                    break;
    			}
    		}				
    	}
    }
    
//		if(0 == bFound)
//		{
//			//遍历黑名单，查找采集器
//			for(ssloop = 0; ssloop < MAX_SS_MAP_SIZE; ssloop++)
//			{
//				if((pBlackSSSpace[ssloop].status == OCCUPIED) 
//					&& (!LPC_Memory_CMP(pBlackSSSpace[ssloop].addr, SSTemp.addr, 6)))
//				{
//					//删除采集器
//					Updata_SS_Hash_Table(ssloop, NULL, SS_VALIDATE_TYPE_BLACK);
//					break;
//				}
//			}
//		}
}

void Del_Validate_W_B_List(uint8* pData)
{
	uint8 DelNum = pData[13];
	uint8  loop, StartAddr;
	for(loop = 0; loop < DelNum; loop++)
	{
		//取出要添加的采集器地址与信息
		StartAddr = loop * 6 + 13 + 1;
		
		Del_Validate_W_B(pData + StartAddr, 6);
	}
}


//通过黑白名单, 检查输入的采集器地址
//	uint8 SS_Sn_Check(uint8* sn_addr)	
//	{
//		uint16 loop;
//		uint8 rc = FALSE;
//		WB_SPACE* pWriteSSSpace;
//		WB_SPACE* pBlackSSSpace;	
//		
//		if(0 == CurWhiteSSMap)
//			pWriteSSSpace = WhiteSSMap1;
//		else
//			pWriteSSSpace = WhiteSSMap2;
//	
//		if(0 == CurBlackSSMap)
//			pBlackSSSpace = BlackSSMap1;
//		else
//			pBlackSSSpace = BlackSSMap2;		
//	
//		uint8 bBlack = 0;
//		uint8 bWrite = 0;
//		for(loop = 0; loop < MAX_SS_MAP_SIZE; loop++)
//		{
//			if((pBlackSSSpace[loop].status == OCCUPIED)	
//				&& (!LPC_Memory_CMP(pBlackSSSpace[loop].addr, sn_addr, 6)))
//			{
//				bBlack = 1;
//				break;
//			}
//		}
//	
//		if(0 == bBlack)
//		{
//			for(loop = 0; loop < MAX_SS_MAP_SIZE; loop++)
//			{
//				if((pWriteSSSpace[loop].status == OCCUPIED)	
//					&& (!LPC_Memory_CMP(pWriteSSSpace[loop].addr, sn_addr, 6)))		
//				{
//					bWrite = 1;
//					break;
//				}
//			}
//		}
//	
//		if((0 == bBlack) && (bWrite))
//			rc = TRUE;
//		return rc; 
//	}
//通过黑白名单, 检查输入的采集器地址
uint8 Check_Black(uint8* addr, uint8 len)	
{
	uint16 loop = 0,i = 0;
	WB_SPACE* pBlackSSSpace;	
    uint32 baseAddr = 0;
    uint8 buf[TRANSFER_BUFFER_SIZE];
    uint8 bBlack = 0;    
	if(0 == CurBlackSSMap)
		baseAddr = EXT_FLASH_BLACK_MAP1_START;
	else
        baseAddr = EXT_FLASH_BLACK_MAP2_START;
//		for(loop = 0; loop < MAX_SS_MAP_SIZE; loop++)
//		{
//			if((pBlackSSSpace[loop].status == OCCUPIED)	
//				&& (!LPC_Memory_CMP(pBlackSSSpace[loop].addr, sn_addr, 6)))
//			{
//				bBlack = 1;
//				break;
//			}
//		}
	for(loop=0; loop < WB_BLOCK_NO; loop++)
	{
        memset(buf, 0, TRANSFER_BUFFER_SIZE);
        //memset(p, 0xFF, TRANSFER_BUFFER_SIZE);
	
        if(SA_ERR_OK != read_archive(buf, baseAddr + (loop * MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)), MAX_HASH_TABLE_WB_SIZE * sizeof(WB_SPACE)))
        {
            continue ;//SA_ERR_FT;
        }
	
		for (i =0 ; i <MAX_HASH_TABLE_WB_SIZE; i++)
		{
            pBlackSSSpace = (WB_SPACE*)&buf[i * sizeof(WB_SPACE)];
			if((pBlackSSSpace->status == OCCUPIED) && len == pBlackSSSpace->addrLen 
                && (!LPC_Memory_CMP(addr, pBlackSSSpace->addr, len)))
			{
			    bBlack = 1;
			    break;
			}
		}				
	}
	return bBlack; 
}


void Init_Hash_Table(void)
{
    IapAction = IAP_ACTION_ERASE_HASH1; //IAP标识位
    while (IapAction != IAP_ACTION_IDLE);

    IapAction = IAP_ACTION_ERASE_HASH2; //IAP标识位
    while (IapAction != IAP_ACTION_IDLE);	// Hold on	

    BFLS_BlockErase(EXT_FLASH_BLACK_MAP1_START);

    Delay_ms(750);

    BFLS_BlockErase(EXT_FLASH_WHITE_MAP1_START);

    Delay_ms(750);
    
//	    IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH1; //IAP标识位
//	    while (IapAction != IAP_ACTION_IDLE);
//	
//	    IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH2; //IAP标识位
//	    while (IapAction != IAP_ACTION_IDLE);
//	
//	    IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH1; //IAP标识位
//	    while (IapAction != IAP_ACTION_IDLE);
//	
//	    IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH2; //IAP标识位
//	    while (IapAction != IAP_ACTION_IDLE);	
}

/*************************************************************************
 * Function Name: htable1_insert
 * Parameters:  insert a record to hashtable1 
 * Return: the location in the hashtable1 if successed ,-1 for negative
 * Description: 
 *
 *************************************************************************/

int Hash_Table_Insert(HASHT *elem, int* pReturnHash) //添加哈希表
{
    uint16 hash;
    uint16 index;
    hash = Cal_Hash_Value(elem->addr); //计算得到哈希索引	
    HASHT* pHasht;
	if(HASH_Table_Used==0)//当前使用table1
 		pHasht = htable1;
	else
		pHasht = htable2;
		
	if(pHasht[hash].status==EMPTY)//空
	{                        
        Updata_Hash_Table(hash, elem); //更新哈希表
        *pReturnHash =  hash;
        return hash; //返回索引值并退出
	}
	 else if (pHasht[hash].status==OCCUPIED && (!LPC_Memory_CMP(pHasht[hash].addr, elem->addr, 6)))//该位被相同电表地址占用
	{
        if (/*(LPC_Memory_CMP(pHasht[hash].falsh_sn, elem->falsh_sn ,6))
            || */(pHasht[hash].SubType != elem->SubType)) 			//如果器件sn不一样                 
        {
            Updata_Hash_Table(hash, NULL);    //置原先的记录无效，再继续插入		                  
        }
        else
        {   // 器件ID一样，不再插入                             
			*pReturnHash =  hash;
            return hash;//( - 1);
        }
	}

	index = (hash + 1) % (MAX_HASH_TABLE_FLASH_SIZE); //如果被别的数占据，则索引加1再判断
	while (hash != index)
	{
        if (pHasht[index].status==EMPTY)    //如果索引对应的位置为空
        {
            Updata_Hash_Table(index, elem); //把哈希所以加1后添加进哈希表
            *pReturnHash =  index;
            return index; //返回索引值并退出循环
        }
        else if ((pHasht[index].status==OCCUPIED) && (!LPC_Memory_CMP(pHasht[index].addr, elem->addr, 6)))//该位被相同电表地址占用
        {
        	 if (/*(LPC_Memory_CMP(pHasht[index].falsh_sn, elem->falsh_sn ,6))
			 || */(pHasht[index].SubType != elem->SubType)) 	//如果器件sn不一样                           
            {
                Updata_Hash_Table(index, NULL);//置原先的记录无效
            }
            else // 器件ID一样，不再插入            
            {                                  
    			*pReturnHash =  index;    		
                 return index;//( - 1);
            }
        }
        index = (index + 1) %(MAX_HASH_TABLE_FLASH_SIZE);//加1、翻转
	}
	*pReturnHash =  MAX_HASH_TABLE_FLASH_SIZE;    
	return ( MAX_HASH_TABLE_FLASH_SIZE);//哈希表已满
}

/*************************************************************************
 * Function Name: htable1_insert
 * Parameters:  insert a record to hashtable1 
 * Return: the location in the hashtable1 if successed ,-1 for negative
 * Description: 
 *
 *************************************************************************/

int Hash_Table_Insert1(HASHT *elem, int* pReturnHash) //添加哈希表
{
    uint16 hash;
    uint16 index;
    hash = Cal_Hash_Value(elem->addr); //计算得到哈希索引	
    HASHT* pHasht;
	if(HASH_Table_Used==0)//当前使用table1
 		pHasht = htable1;
	else
		pHasht = htable2;
		
	if(pHasht[hash].status==EMPTY)//空
	{                        
        Updata_Hash_Table(hash, elem); //更新哈希表
        *pReturnHash =  hash;
        return hash; //返回索引值并退出
	}
	 else if (pHasht[hash].status==OCCUPIED && (!LPC_Memory_CMP(pHasht[hash].addr, elem->addr, METER_ADDRESS_LENGTH_MAX)))//该位被相同电表地址占用
	{
        if ((pHasht[hash].SubType != elem->SubType)) 			//如果器件sn不一样                 
        {
            Updata_Hash_Table(hash, NULL);    //置原先的记录无效，再继续插入		                  
        }
        else
        {   // 器件ID一样，不再插入                             
			*pReturnHash =  hash;
            return hash;//( - 1);
        }
	}

	index = (hash + 1) % (MAX_HASH_TABLE_FLASH_SIZE); //如果被别的数占据，则索引加1再判断
	while (hash != index)
	{
        if (pHasht[index].status == EMPTY)    //如果索引对应的位置为空
        {
            Updata_Hash_Table(index, elem); //把哈希所以加1后添加进哈希表
            *pReturnHash =  index;
            return index; //返回索引值并退出循环
        }
        else if ((pHasht[index].status==OCCUPIED) && (!LPC_Memory_CMP(pHasht[index].addr, elem->addr, METER_ADDRESS_LENGTH_MAX)))//该位被相同电表地址占用
        {
            if ((pHasht[index].SubType != elem->SubType)) 	//如果器件sn不一样                           
            {
                Updata_Hash_Table(index, NULL);//置原先的记录无效
            }
            else // 器件ID一样，不再插入            
            {                                  
    			*pReturnHash =  index;    		
                 return index;//( - 1);
            }
        }
        index = (index + 1) % (MAX_HASH_TABLE_FLASH_SIZE);//加1、翻转
	}
	*pReturnHash =  MAX_HASH_TABLE_FLASH_SIZE;    
	return ( MAX_HASH_TABLE_FLASH_SIZE);//哈希表已满
}



/*************************************************************************
 * Function Name: htable1_search
 * Parameters:  ammeter 's address 
 * Return: none
 * Description: 根据电表地址查找ID
 *
 *************************************************************************/

int Hash_Table_Search(uint8 *addr)
{
    uint16 hash;
    uint16 index;
    uint8 metertable[6];
    uint8 j;
    for (j = 0; j < 6; j++)
    {
        metertable[j] = addr[j];
        /*            
        if (metertable[j] == 0xAA)
        {
        metertable[j] = 0x0;
        }*/
    }
    hash = Cal_Hash_Value(metertable); //计算得到哈希值
    if(HASH_Table_Used==0)
    {
        if ((htable1[hash].status== OCCUPIED) && (!(memcmp(metertable, htable1[hash].addr, 6))))
        {
            return hash; //返回哈希索引
        }
        index = (hash + 1)  % (MAX_HASH_TABLE_FLASH_SIZE); //如果不是它，则加1往下对比
        while (index != hash)
        {
            if ((htable1[index].status== OCCUPIED) && (!(memcmp(metertable, htable1[index].addr, 6)))) //如果哈希表里有其值
            {
                return index; //返回哈希索引
            }
            index = (index + 1)  % (MAX_HASH_TABLE_FLASH_SIZE); //如果不是它，则加1往下对比
        }
        return ( - 1); //如果找不到，返回-1
    }   
    else
    {
        if ((htable2[hash].status== OCCUPIED) && (!(memcmp(metertable, htable2[hash].addr, 6))))    //如果哈希表里有其值	      
        {
            return hash; //返回哈希索引
        }
        index = (hash + 1)  % (MAX_HASH_TABLE_FLASH_SIZE); //如果不是它，则加1往下对比
        while (index != hash)
        {	
            if ((htable2[index].status== OCCUPIED) && (!(memcmp(metertable, htable2[index].addr, 6))))    //如果哈希表里有其值
            {
                return index; //返回哈希索引
            }
            index = (index + 1)  % (MAX_HASH_TABLE_FLASH_SIZE); //如果不是它，则加1往下对比
        }
        return ( - 1); //如果找不到，返回-1
    }   
}
int Hash_Table_Search1(uint8 *addr, uint8 len)
{
    uint16 hash;
    uint16 index;
    uint8 metertable[METER_ADDRESS_LENGTH_MAX];
    uint8 j;
    memset(metertable, 0, METER_ADDRESS_LENGTH_MAX);
    for (j = 0; j < len; j++)
    {
        metertable[j] = addr[j];
        /*            
        if (metertable[j] == 0xAA)
        {
        metertable[j] = 0x0;
        }*/
    }
    hash = Cal_Hash_Value(metertable); //计算得到哈希值
    if(HASH_Table_Used==0)
    {
        if ((htable1[hash].status == OCCUPIED) && (!(memcmp(metertable, htable1[hash].addr, METER_ADDRESS_LENGTH_MAX))))
        {
            return hash; //返回哈希索引
        }
        index = (hash + 1) %(MAX_HASH_TABLE_FLASH_SIZE ); //如果不是它，则加1往下对比
        while (index != hash)
        {
            if ((htable1[index].status== OCCUPIED) && (!(memcmp(metertable, htable1[index].addr, METER_ADDRESS_LENGTH_MAX)))) //如果哈希表里有其值
            {
                return index; //返回哈希索引
            }
            index = (index + 1) %(MAX_HASH_TABLE_FLASH_SIZE ); //如果不是它，则加1往下对比
        }
        return ( - 1); //如果找不到，返回-1
    }   
    else
    {
        if ((htable2[hash].status== OCCUPIED) && (!(memcmp(metertable, htable2[hash].addr, METER_ADDRESS_LENGTH_MAX))))    //如果哈希表里有其值	      
        {
            return hash; //返回哈希索引
        }
        index = (hash + 1) %(MAX_HASH_TABLE_FLASH_SIZE ); //如果不是它，则加1往下对比
        while (index != hash)
        {	
            if ((htable2[index].status== OCCUPIED) && (!(memcmp(metertable, htable2[index].addr, METER_ADDRESS_LENGTH_MAX))))    //如果哈希表里有其值
            {
                return index; //返回哈希索引
            }
            index = (index + 1) %(MAX_HASH_TABLE_FLASH_SIZE); //如果不是它，则加1往下对比
        }
        return ( - 1); //如果找不到，返回-1
    }   
}

int Hash_Table_Search_By_Compressed(uint8 *addr, uint8 len)
{
    uint16 hash;
    uint16 index;
    uint8 metertable[METER_ADDRESS_LENGTH_MAX];
    uint8 tmp[CON_DEV_ADDR_LEN_8];
    uint8 j;
    memset(metertable, 0, METER_ADDRESS_LENGTH_MAX);
    memset(tmp, 0, CON_DEV_ADDR_LEN_8);
    HASHT * pHtable = NULL;
    if(HASH_Table_Used==0)
    {
        pHtable = &htable1[0];
    }
    else
    {
        pHtable = &htable2[0];
    }
    for (j = 0; j < len; j++)
    {
        metertable[j] = addr[j];
    }
    hash = Cal_Hash_Value(metertable); //计算得到哈希值
    
    if ((pHtable[hash].status == OCCUPIED))
    {
        if(CompressAddr(pHtable[hash].addr, pHtable[hash].addrLen, tmp))
        {
            if(!(memcmp(metertable, pHtable[hash].addr, CON_DEV_ADDR_LEN_8)))
            {
                return hash; //返回哈希索引
            }
        }
    }
    index = (hash + 1) %(MAX_HASH_TABLE_FLASH_SIZE ); //如果不是它，则加1往下对比
    while (index != hash)
    {

        if ((pHtable[index].status == OCCUPIED))
        {
            if(CompressAddr(pHtable[index].addr, pHtable[index].addrLen, tmp))
            {
                if(!(memcmp(metertable, pHtable[index].addr, CON_DEV_ADDR_LEN_8)))
                {
                    return index; //返回哈希索引
                }
            }
        }
    
        index = (index + 1) %(MAX_HASH_TABLE_FLASH_SIZE ); //如果不是它，则加1往下对比
    }
    return ( - 1); //如果找不到，返回-1
}
/*************************************************************************
 * Function Name: GetRealArchivesAddr
 * Parameters:  get real address by postion
 * Return: 1 successed ,0 for negative
 * Description: 
 *
 *************************************************************************/
uint8 GetRealArchivesAddr(uint16 id, uint8 * addr, uint8 * len)
{
    uint8 ret = 0;
    HASHT * pHtable = NULL;
    if(HASH_Table_Used==0)
    {
        pHtable = &htable1[0];
    }
    else
    {
        pHtable = &htable2[0];
    }
    if(id > MAX_HASH_TABLE_FLASH_SIZE || addr == NULL)
    {
        return ret;
    }

    memcpy(addr, pHtable[id].addr, pHtable[id].addrLen);
    *len = pHtable[id].addrLen;

    ret = 1;
    return ret;
}

/*************************************************************************
 * Function Name: UpdataHashTable
 * Parameters:  insert a record to hashtable1 
 * Return: the location in the hashtable1 if successed ,-1 for negative
 * Description: 
 *
 *************************************************************************/
void Updata_Hash_Table(uint16 hash, HASHT *elem) //更新哈希表
{
    uint32 i;
    uint32 sector;
    uint32 location;
    HASHT *p;
    p=( HASHT *)updata_code_cache; 
    sector = hash / MAX_HASH_TABLE_SECTOR_SIZE;
    location = sector * MAX_HASH_TABLE_SECTOR_SIZE;

    if(IapAction == IAP_ACTION_IDLE)//内存保护
    {
        switch(HASH_Table_Used)
        {
        case 0://当前使用table1
            for (i = 0; i < MAX_HASH_TABLE_SECTOR_SIZE; i++)
            {
                if(htable1[location + i].status==OCCUPIED)//如果该哈希元素的值有效     
                {
                    p[i] = htable1[location + i]; //把原来哈希表里的数据保存在缓存中
                }
                else if(htable1[location + i].status==EMPTY)
                {   // 如果该哈希元素的值无效                     
                    memset(&p[i], 0xFF, sizeof(HASHT)); //缓冲中的数都置成0X0***
                }
                else if(htable1[location + i].status==DEAD)
                {   // 如果该哈希元素的值无效                     
                    memset(&p[i], 0x0, sizeof(HASHT)); //缓冲中的数都置成0X0***
                }

            }	
            break;
        case 1://当前使用table2
            for (i = 0; i < MAX_HASH_TABLE_SECTOR_SIZE; i++)
            {

                if(htable2[location + i].status==OCCUPIED)//如果该哈希元素的值有效     
                {
                    p[i] = htable2[location + i]; //把原来哈希表里的数据保存在缓存中
                }
                else if(htable2[location + i].status==EMPTY)
                {   // 如果该哈希元素的值无效                     
                    memset(&p[i], 0xFF, sizeof(HASHT)); //缓冲中的数都置成0X0***
                }
                else if(htable2[location + i].status==DEAD)
                {   // 如果该哈希元素的值无效                     
                    memset(&p[i], 0x0, sizeof(HASHT)); //缓冲中的数都置成0X0***
                }
            }	
            break; 
        }
        

        if (elem != NULL)//如果有新添加的哈希元素        
        {
                // 更新哈希缓存
    		memcpy((uint8 *)&p[hash - location], (uint8 *)elem, sizeof(HASHT)); //把新添加的哈希表元素放到哈希缓存表
//	    		memcpy(p[hash - location].falsh_sn, elem->falsh_sn,6);
    		p[hash - location].status	= OCCUPIED;	
//	    		p[hash - location].SubType = elem->SubType;
//	    		p[hash - location].src = elem->src;
//	            p[hash - location].devType = elem->devType;
            
    		shadow_space[hash].handled=UN_HANDLED;              
    		shadow_space[hash].times= MeterDeadCount;
    		I2cWrite(0xA0, (uint8*)&shadow_space[hash], hash,1);//写入E2PR0M		
        }
        else
        {           
        	//p[hash - location].status	= DEAD;	
        	memset(&p[hash - location],0x0,sizeof(HASHT));
        }
	

                // 写入flash
		if(HASH_Table_Used==0)
		{
			IapAction = IAP_ACTION_TRANSFER_HASH1;//写table1
		    HashUpdateSectorAdd = FLASH_MAP_ADD + sector * MAX_HASH_TABLE_SECTOR_SIZE * MAX_HASH_TABLE_CACHE_SIZE ;
		}
		else
		{
			IapAction = IAP_ACTION_TRANSFER_HASH2;//写table2
		    HashUpdateSectorAdd = FLASH_MAP_ADD2 + sector * MAX_HASH_TABLE_SECTOR_SIZE * MAX_HASH_TABLE_CACHE_SIZE;
		}
		while (IapAction != IAP_ACTION_IDLE);   // hold on
		
		if(Write_Sucess==ERR)//如果成功失败
		{
		    Hash_Correct(HashUpdateSectorAdd,(uint8*)&updata_code_cache);		
		}
    }
}

/*********************************************************************
*哈希表有效数据转移
*
**********************************************************************/
void Hash_Transfer(void)
{
	uint16 i,j;
	HASHT *p;
	p=(HASHT *)updata_code_cache;
	switch(HASH_Table_Used)
	{
        case 1:	//table1-->table2
        {
            for(j=0;j<BLOCK_NO;j++)
            {
                for (i = 0 ; i < MAX_HASH_TABLE_SECTOR_SIZE; i++)
                {
                    if (htable1[i+(j*BLOCK_NO)].status!= DEAD) //如果该哈希元素的值有效   
                    {			                        
                        memcpy(p+i,htable1+i+j*BLOCK_NO,sizeof(HASHT));//把有效数据保存在缓存中	
                    }
                    else
                    {   // 如果该哈希元素的值无效                     
                        memset(p+i, 0xff, sizeof(HASHT)); //缓冲中的数都置成0XFF***
                    }				
                }		
                IapAction = IAP_ACTION_TRANSFER_HASH2;
                HashUpdateSectorAdd = FLASH_MAP_ADD2 +(j*256);
                while (IapAction != IAP_ACTION_IDLE);	
            }

            IapAction = IAP_ACTION_ERASE_HASH1;//最后擦除原表	        
            while (IapAction != IAP_ACTION_IDLE);	
            break;	
        }
        	
        case 0://table2-->table1
        {
            for(j=0;j<BLOCK_NO;j++)	
            {
                for (i = 0; i < MAX_HASH_TABLE_SECTOR_SIZE; i++)
                {
                    if (htable2[i+(j*BLOCK_NO)].status!= DEAD) //如果该哈希元素的值有效    				            
                    {
                        memcpy(p+i,htable2+i+j*BLOCK_NO,sizeof(HASHT));//把有效数据保存在缓存中
                    }
                    else // 如果该哈希元素的值无效    
                    {                   
                        memset(p+i, 0xff, sizeof(HASHT)); //缓冲中的数都置成0XFF***
                    }								
                }		
                IapAction = IAP_ACTION_TRANSFER_HASH1;
                HashUpdateSectorAdd = FLASH_MAP_ADD +(j*256) ;
                while (IapAction != IAP_ACTION_IDLE);		
            }		

            IapAction = IAP_ACTION_ERASE_HASH2;	  //最后擦除原表      
            while (IapAction != IAP_ACTION_IDLE);	
            break;		
        }
    }
}

/*************************************************************************
*FLASH写入错误的补救
*
**************************************************************************/
void Hash_Correct(uint32 dst,uint8* src )
{
    uint8 i,loop;
    HASHT *p;
    p=(HASHT *)updata_code_cache;
    //uint8 cache[256]={0};

    //	memcpy(cache,src,256);//保存数据
	
    if(HASH_Table_Used==0)
        dst=dst-FLASH_MAP_ADD+FLASH_MAP_ADD2 ;
    else
        dst=dst-FLASH_MAP_ADD2+FLASH_MAP_ADD;	
	
    switch(HASH_Table_Used)
    {
        case 0:	//table1-->table2
        {
            for(loop=0;loop<BLOCK_NO;loop++)
            {
				  	
                for (i =0 ; i <MAX_HASH_TABLE_SECTOR_SIZE; i++)
                {
                    if (htable1[i+(loop*MAX_HASH_TABLE_SECTOR_SIZE)].status!= DEAD) //如果该哈希元素的值有效    
                    {
                        memcpy(p+i,htable1+i+loop*MAX_HASH_TABLE_SECTOR_SIZE,sizeof(HASHT));//把有效数据保存在缓存中						
                    }
                    else
                    {   // 如果该哈希元素的值无效                     
                        memset(p+i, 0xff, sizeof(HASHT)); //缓冲中的数都置成0XFF***
                    }				
                }				
					 		
                HashUpdateSectorAdd = FLASH_MAP_ADD2 +(loop*256);			        
                if(HashUpdateSectorAdd==dst)
                {
                    memcpy(p,src,256);
                }	
                IapAction = IAP_ACTION_TRANSFER_HASH2;
                while (IapAction != IAP_ACTION_IDLE);			
		  	}
			
			IapAction = IAP_ACTION_ERASE_HASH1;//最后擦除原表	        
			while (IapAction != IAP_ACTION_IDLE);	
			HASH_Table_Used++;
			HASH_Table_Used=(HASH_Table_Used&1);   //1表示之前用Table1,之后用Table2;    0相反
			I2cWrite(0xA0, (uint8*) &HASH_Table_Used, FM_HASH_USED, 1);    
			Write_Sucess=SUCCESS;
			break;	
		}
	
        case 1://table2-->table1
        {
            for(loop=0;loop<BLOCK_NO;loop++)	
            {
                for (i = 0; i < MAX_HASH_TABLE_SECTOR_SIZE; i++)
                {
                    if (htable2[i+(loop*MAX_HASH_TABLE_SECTOR_SIZE)].status!= DEAD) //如果该哈希元素的值有效    				            
                    {
                        memcpy(p+i,htable2+i+(loop*MAX_HASH_TABLE_SECTOR_SIZE),sizeof(HASHT));//把有效数据保存在缓存中							 
                    }
                    else
                    {   // 如果该哈希元素的值无效                     
                        memset(p+i, 0xff, sizeof(HASHT)); //缓冲中的数都置成0XFF***
                    }								
                }		

                HashUpdateSectorAdd = FLASH_MAP_ADD +(loop*256);
                if(HashUpdateSectorAdd==dst)
                {
                    memcpy(p,src,256);//原先写入的错误数据
                }		
                IapAction = IAP_ACTION_TRANSFER_HASH1;					 
                while (IapAction != IAP_ACTION_IDLE);
            }		

            IapAction = IAP_ACTION_ERASE_HASH2;	  //最后擦除原表      
            while (IapAction != IAP_ACTION_IDLE);	
            HASH_Table_Used++;
            HASH_Table_Used=(HASH_Table_Used&1);   //1表示之前用Table1,之后用Table2;    0相反
            I2cWrite(0xA0, (uint8*) &HASH_Table_Used, FM_HASH_USED, 1);   
            Write_Sucess=SUCCESS;
            break;		
        }
    }
}

//通过黑白名单, 检查输入的表地址地址
uint8 Meter_Check1(uint8 cmd, uint8 * ppanid, uint8* addr, uint8 addrLen, uint16 * pos)	
{
	uint8 rc = FALSE;
    int ret = -1;
    uint16 id = 0;
    uint16 panid = 0;

    if(ppanid == NULL || addr == NULL)
    {
        return rc;
    }
    
    memcpy((uint8 *)&panid, ppanid, 2);
    
    if(panid != rfpara.panid && panid != 0xFFFF)
    {
        return rc;
    }

    
    switch(cmd)
    {
    case 1:
    case 3:   
        if(CheckCompressAddr(addr, addrLen))
        {
            ret = Hash_Table_Search_By_Compressed(addr, addrLen);
        }
        else
        {
            ret = Hash_Table_Search1(addr, addrLen);
        }
        
        break;
    case 2:
        memcpy((uint8 *)&id, addr, 2);
        if(memcmp(ppanid, (uint8 *)&rfpara.panid, 2) == 0)
        {
            if(id > 1 && id <= rfpara.rf_slotnum && cltor[id].devAddr[0] < 0xFF)
            {
                ret = cltor[id].realPos;
            }
        }
        break;
    default:
        break;
    }

	if(ret != (-1))
    {   
		rc = TRUE;
        *pos = (uint16)ret;
    }
    
	return rc; 
}

uint8 DevCheck(uint8 * addr, uint8 addrLen, uint8* ppanid)
{
    uint16 panid = 0;
    if(addr == NULL || addrLen > METER_ADDRESS_LENGTH_MAX || ppanid == NULL)
    {
        return 0;
    }
//校对panid
    memcpy((uint8 *)&panid, ppanid, 2);
    if(panid != rfpara.panid && panid != 0xFFFF)
    {
        return 0;
    }

    if(/*addrLen == nDeviceMacAddrLen && */CmpBuffer(addr, nDeviceMacAddr, addrLen) == 0 )
    {
        return 1;
    }
    uint8 tmp[CON_DEV_ADDR_LEN_8];
    memset(tmp, 0, CON_DEV_ADDR_LEN_8);
    if(addrLen == CON_DEV_ADDR_LEN_8 && CompressAddr(nDeviceMacAddr, nDeviceMacAddrLen, tmp))
    {
        if( CmpBuffer(addr, tmp, addrLen) == 0)
        {
            return 1;
        }
    }

    if(memcmp(addr, sBroadAddr99, addrLen) == 0 || 
        memcmp(addr, sBroadAddrAA, addrLen) == 0)
    {
        return 2;
    }

    return 0;
}

//	#define PARA_SYNC_LEN   sizeof(COLLECTOR)
//	#define PARA_ALL_LEN   (sizeof(STMETERPARAFLASH) - 1)
void RecoverCltorPara(void)
{
    STMETERPARAFLASH stMeterParaF;
    COLLECTOR   * pstMeterParaR; 
    int i = 0;//cltor
    for(i = 0; i < MAX_SUP_SS_NUM + 1; i ++)
    {
        pstMeterParaR = (COLLECTOR *)&cltor[i];
        I2cRead(0xA0,  (uint8 *)&stMeterParaF, FM_METER_INFO_BASE_ADDR + i*sizeof(STMETERPARAFLASH), sizeof(STMETERPARAFLASH));
        if((stMeterParaF.devAddr[0] < 0xFF || stMeterParaF.devAddr[1] < 0xFF) && 
            stMeterParaF.checksum == Check_SUM((uint8 *)&stMeterParaF, sizeof(STMETERPARAFLASH) - 1))
        {
            memcpy((uint8 *)pstMeterParaR, (uint8 *)&stMeterParaF, PARA_SYNC_LEN);
//	            pstMeterParaR->nod.bNeedAck = 0;
//	            pstMeterParaR->nod.bNeedDelete = 0;
            LOG_DEBUG( DBGFMT"[%x] recover %02x%02x%02x%02x%02x%02x hop[%d] father[%x] mather[%x] sts[%x]\n",DBGARG, 
                 i, pstMeterParaR->devAddr[5], pstMeterParaR->devAddr[4], pstMeterParaR->devAddr[3], 
                 pstMeterParaR->devAddr[2], pstMeterParaR->devAddr[1], pstMeterParaR->devAddr[0], 
                 pstMeterParaR->hop, pstMeterParaR->father, pstMeterParaR->mother,*(uint8*)&pstMeterParaR->nodestatus);
            
        }
        
    }
}


