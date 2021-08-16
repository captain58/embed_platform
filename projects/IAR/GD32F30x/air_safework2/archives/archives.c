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

//���²ɼ����ڰ�����������
//elem == NULL ʱ����ʾɾ������
//	void Updata_SS_Hash_Table(uint16 hash, WB_SPACE *elem, uint8 type) 
//	{
//	    uint32 i;
//	    uint8 sector;
//	    uint8 location;
//	    WB_SPACE *p;
//	    p=( WB_SPACE *)updata_code_cache; 
//	    sector = hash / MAX_HASH_TABLE_CACHE_SIZE;
//	    location = sector * MAX_HASH_TABLE_CACHE_SIZE;		//ȡ�����е��ֶο�ʼλ��(256B)
//	
//		if(IapAction != IAP_ACTION_IDLE)		//�ڴ汣��
//			return;			//error
//	
//		//ȡ����Ҫ�������ڴ�����
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
//			if(pSSSpace[location + i].status == OCCUPIED)//����ù�ϣԪ�ص�ֵ��Ч     
//			{
//				//��ԭ����ϣ��������ݱ����ڻ�����
//				memcpy(p + i, pSSSpace + location + i, sizeof(WB_SPACE));	
//			}
//			else if(pSSSpace[location + i].status == EMPTY)
//			{   
//				// ����ù�ϣԪ�ص�ֵ��Ч                     
//				memset(&p[i], 0xFF, sizeof(WB_SPACE));
//			}
//			else if(pSSSpace[location + i].status == DEAD)
//			{  
//				// ����ù�ϣԪ�ص�ֵ��Ч                     
//				memset(&p[i], 0x0, sizeof(WB_SPACE)); 	//�����е������ó�0X0***
//			}
//		}	
//		
//		if (elem != NULL)//���������ӵĹ�ϣԪ��        
//		{
//			// ���¹�ϣ����
//			memcpy(p[hash - location].addr, elem->addr, 6); 		//������ӵĹ�ϣ��Ԫ�طŵ���ϣ�����
//			p[hash - location].status	= OCCUPIED;	
//		}
//		else
//		{
//			memset(&p[hash - location],0x0,sizeof(WB_SPACE));	//ɾ������洢����
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
//		if(Write_Sucess==ERR)//���ʧ��
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

	//ȡ����Ҫ�������ڴ�����
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
	
	if (elem != NULL)//���������ӵĹ�ϣԪ��        
	{
		// ���¹�ϣ����
        
		memcpy(p[0].addr, (uint8 *)elem, 6); 		//������ӵĹ�ϣ��Ԫ�طŵ���ϣ�����
		p[0].status	= OCCUPIED;	
        
	}
	else
	{
		memset(&p[0],0x0,sizeof(WB_SPACE));	//ɾ������洢����
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
	if(berr)//���ʧ��
	{
		SS_Hash_Correct(baseAddr + (hash * sizeof(WB_SPACE)), (uint8*)&updata_code_cache, type);		
	}
    
    return SA_ERR_OK;
}

/*********************************************************************
*��ϣ����Ч����ת��
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
			if (pSSSpace1[i].status != DEAD) //����ù�ϣԪ�ص�ֵ��Ч    
			{
				memcpy(p + i, pSSSpace1 + i, sizeof(WB_SPACE));//����Ч���ݱ����ڻ�����						
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
//				        if (pSSSpace1[i+(j*MAX_HASH_TABLE_SECTOR_SIZE)].status != DEAD) //����ù�ϣԪ�ص�ֵ��Ч   
//			                memcpy(p+i, pSSSpace1 + i + j * MAX_HASH_TABLE_SECTOR_SIZE, sizeof(WB_SPACE));//����Ч���ݱ����ڻ�����	
//				        else
//			                memset(p+i, 0xff, sizeof(WB_SPACE)); //�����е������ó�0XFF***
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
//					IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH1;	//������ԭ��	     
//				else
//					IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH1;	//������ԭ��	     	
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
//						if (pSSSpace2[i + (j * MAX_HASH_TABLE_SECTOR_SIZE)].status!= DEAD) //����ù�ϣԪ�ص�ֵ��Ч    				            
//						         memcpy(p + i, pSSSpace2 + i + j * MAX_HASH_TABLE_SECTOR_SIZE, sizeof(WB_SPACE));//����Ч���ݱ����ڻ�����
//						else 	// ����ù�ϣԪ�ص�ֵ��Ч    
//						        memset(p+i, 0xff, sizeof(WB_SPACE)); //�����е������ó�0XFF***
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
//					IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH2;	//������ԭ��	     
//				else
//					IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH2;	//������ԭ��	     	
//					
//				while (IapAction != IAP_ACTION_IDLE);	
//				break;		
//	    	}
//	    }
}

/*************************************************************************
*FLASHд�����Ĳ���
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

//	memcpy(cache,src,256);//��������

	WB_SPACE* pSSSpace1;
	//WB_SPACE* pSSSpace2;
	uint8 CurMap = 0;
	//�����ϣ��ת��ʱ��Ҫ�Ĵ洢����
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
			if (pSSSpace1[i].status != DEAD) //����ù�ϣԪ�ص�ֵ��Ч    
			{
				memcpy(p + i, pSSSpace1 + i, sizeof(WB_SPACE));//����Ч���ݱ����ڻ�����						
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
    
//���õ�ǰ��ϣ�������ֵ
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
//						if (pSSSpace1[i].status != DEAD) //����ù�ϣԪ�ص�ֵ��Ч    
//						{
//							memcpy(p + i, pSSSpace1 + i, sizeof(WB_SPACE));//����Ч���ݱ����ڻ�����						
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
//				//���õ�ǰ��ϣ�������ֵ
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
//						if (pSSSpace2[i + (loop * MAX_HASH_TABLE_CACHE_SIZE)].status != DEAD) //����ù�ϣԪ�ص�ֵ��Ч    
//						{
//							memcpy(p + i, pSSSpace2 + i + loop * MAX_HASH_TABLE_CACHE_SIZE, sizeof(WB_SPACE));//����Ч���ݱ����ڻ�����						
//						}
//						else
//						{   // ����ù�ϣԪ�ص�ֵ��Ч                     
//							memset(p + i, 0xff, sizeof(WB_SPACE)); //�����е������ó�0XFF***
//						}				
//					}		
//					
//					//���õ�ǰ��ϣ�������ֵ
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
//					IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH2;	//������ԭ��	     
//				else
//					IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH2;	//������ԭ��	     	
//				while (IapAction != IAP_ACTION_IDLE);	
//	
//				//���õ�ǰ��ϣ�������ֵ
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


int SS_Hash_Table_Insert(WB_SPACE* elem, uint8 type) //��ӹ�ϣ��
{
    uint16 hash;
    uint16 index;
    hash = (Cal_Hash_Value(elem->addr)) % MAX_HASH_TABLE_FLASH_SIZE; //����õ���ϣ����	
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


	if(WBpace.status == EMPTY) //��
	{                        
		Updata_SS_Hash_Table(hash, elem, type); //���¹�ϣ��
		return hash; //��������ֵ���˳�
	}
	else if ((WBpace.status==OCCUPIED) && WBpace.addrLen == elem->addrLen
        && (!LPC_Memory_CMP(WBpace.addr, elem->addr, elem->addrLen)))//��λ����ͬ����ַռ��
	{
		// ����IDһ�������ٲ���                             
		return ( - 1);
	}
	
	index = (hash + 1) % MAX_HASH_TABLE_FLASH_SIZE;	 //����������ռ�ݣ���������1���ж�
	while (hash != index)
	{
        if(SA_ERR_OK != read_archive((uint8 *)&WBpace, baseAddr + (index * sizeof(WB_SPACE)), sizeof(WB_SPACE)))
        {
            index = (index + 1) % MAX_HASH_TABLE_FLASH_SIZE; //��1����ת
            continue;
        }

    
        if (WBpace.status==EMPTY)    //���������Ӧ��λ��Ϊ��
        {
            Updata_SS_Hash_Table(index, elem, type); //�ѹ�ϣ���Լ�1����ӽ���ϣ��
            return index; //��������ֵ���˳�ѭ��
        }
        else if ((WBpace.status==OCCUPIED) && (!LPC_Memory_CMP(WBpace.addr, elem->addr, elem->addrLen)))//��λ����ͬ����ַռ��
        {
                // ����IDһ�������ٲ���                                         		
            return ( - 1);
        }
        index = (index + 1) % MAX_HASH_TABLE_FLASH_SIZE; //��1����ת
	}
	return (MAX_HASH_TABLE_FLASH_SIZE);	//��ϣ������
}
	
/******************************************************************
*��ѯ�ɼ�����Ϣ
*
******************************************************************/ 
void Search_Validate_SS(uint8 *data)
{
//		uint16 i,j,len,id;
//		uint8 handled_no;//�Ѵ���ɼ�����
//		uint8 request_no;//��������
//		uint8 report_no=0;//�ϱ�����
//		uint8 up_pkt[200]={0};
//		WB_SPACE* pWriteSSSpace;
//		WB_SPACE* pBlackSSSpace;
//		uint8 bFound;
//		uint8* pData;
//		
//		len=*(data+1);//������
//		request_no=*(data+len-3);
//	
//		//��ʼ����ʱ
//		//����Ѵ����ʶλ
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
//		if(len > 16)//����Ѵ���ɼ���
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
//				&& (pWriteSSSpace[j].status == OCCUPIED)) //���δ�����Ҹ�SN��ռ��
//			{
//				pData = up_pkt+14+(report_no * 7);
//				memcpy(pData, pWriteSSSpace[j].addr, 6); //�ɼ�����ַ
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
//					&& (pBlackSSSpace[j].status == OCCUPIED)) //���δ�����Ҹ�SN��ռ��
//				{
//					pData = up_pkt+14+(report_no * 7);
//					memcpy(pData, pBlackSSSpace[j].addr, 6); //�ɼ�����ַ
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
//		up_pkt[13]=report_no;//�ɼ�������
//	
//		up_pkt[14+(report_no*7)] = Check_SUM(up_pkt + 3, 11+(report_no*7)); //У��λ
//		up_pkt[15+(report_no*7)] = 0x16;
//		if(bEZMACDebugType)
//			SendPacketToEzmacProc(pEZMACRecvPacket, up_pkt, 16+(report_no*7), 0);
//		else			
//			UART_PutArray(UART0, up_pkt, 16+(report_no*7)); //�ش�
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
	if(HASH_Table_Used==0)//��ǰʹ��table1
 		pHasht = htable1;
	else
		pHasht = htable2;

	//ȡ����ǰ�ڰ������Ĵ��λ��
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
	//ȡ��Ҫ��ӵĲɼ�����ַ����Ϣ
	memset((uint8 *)&SSTemp, 0, sizeof(WB_SPACE));
	memcpy(SSTemp.addr, pAddr, len);
	SSTemp.addrLen = len;
//		ValidateSSType = type;
//		if(ValidateSSType > 1)
//			ValidateSSType = SS_VALIDATE_TYPE_WHITE;

	//�����������������Ƿ���ͬ��ַ�Ĳɼ���
	bNeedAdd = 1;
	for(ssloop = 0; ssloop < MAX_HASH_TABLE_FLASH_SIZE; ssloop++)
	{
		if((pHasht[ssloop].status == OCCUPIED) && (!LPC_Memory_CMP(SSTemp.addr, pHasht[ssloop].addr, len)))
		{
			//����������ͬ��ַ��
			if(ValidateSSType == SS_VALIDATE_TYPE_BLACK)
			{
				//����ӵĲɼ�����ϢΪ������ʱ����Ҫɾ��������
				//ɾ������ַ
				Updata_Hash_Table(ssloop, NULL);  //ɾ��   
			}
			else if(ValidateSSType == SS_VALIDATE_TYPE_WHITE)
			{
				//����ӵĲɼ�����ϢΪ������ʱ��������ӣ�ֱ�ӷ���
				bNeedAdd = 0;
				break;
			}
		}
	}
		
	if(bNeedAdd)
	{
		//�������������ж��Ƿ���ͬ��ַ
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
    				//��ͬ��ַʱ��������ӣ�ֱ�ӷ���
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
//					//��ͬ��ַʱ��������ӣ�ֱ�ӷ���
//					bNeedAdd = 0;
//					break;
//				}
//			}
		if(bNeedAdd)
		{
			//��Ҫ��Ӵ˲ɼ�����Ϣ
			return_hash = SS_Hash_Table_Insert(&SSTemp, ValidateSSType);
			bAddSuccess = FALSE;
			if(return_hash >= 0)
			{  
				if (return_hash == MAX_HASH_TABLE_FLASH_SIZE)
				{
					//��FLASH�洢��ʱ����Ҫת���洢����
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
					SS_Hash_Transfer(ValidateSSType); 	//����,ת�ƹ�ϣ��
					return_hash = SS_Hash_Table_Insert(&SSTemp, ValidateSSType);	//�ٴβ����ϴ�δ��ӳɹ���
					if((return_hash >= 0) && (return_hash != MAX_SS_MAP_SIZE))
					{
						bAddSuccess = TRUE;
					}
				}
				else //�����ӳɹ�
				{
					bAddSuccess = TRUE;
				}
			}		
			else
			{
				//����ͬ�Ĳɼ�������
			}
			
			//����Ӻ������ɹ�ʱ����Ҫ�����������Ӧ�Ĳɼ���
			if( (bAddSuccess) && (ValidateSSType == SS_VALIDATE_TYPE_BLACK))
			{
				//��Ҫ�������Ӧ�Ĳɼ���
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
		//ȡ��Ҫ��ӵĲɼ�����ַ����Ϣ
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
	//ȡ����ǰ�ڰ������Ĵ��λ��
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

	//ȡ���ɼ�����ַ
	memset(&SSTemp, 0, sizeof(WB_SPACE));
	memcpy(SSTemp.addr, pData, addrLen);
	SSTemp.addrLen = addrLen;
//		//���������������Ҳɼ���
//		for(ssloop = 0; ssloop < MAX_SS_MAP_SIZE; ssloop++)
//		{
//			if((pWriteSSSpace[ssloop].status == OCCUPIED) 
//				&& (!LPC_Memory_CMP(pWriteSSSpace[ssloop].addr, SSTemp.addr, 6)))
//			{
//				//��Ҫ�������Ӧ�Ĳɼ���
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
//				//ɾ���ɼ���
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
			    //ɾ��������
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
    			    //ɾ��������
    			    
    				Updata_SS_Hash_Table(ssloop, NULL, SS_VALIDATE_TYPE_BLACK);
                    break;
    			}
    		}				
    	}
    }
    
//		if(0 == bFound)
//		{
//			//���������������Ҳɼ���
//			for(ssloop = 0; ssloop < MAX_SS_MAP_SIZE; ssloop++)
//			{
//				if((pBlackSSSpace[ssloop].status == OCCUPIED) 
//					&& (!LPC_Memory_CMP(pBlackSSSpace[ssloop].addr, SSTemp.addr, 6)))
//				{
//					//ɾ���ɼ���
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
		//ȡ��Ҫ��ӵĲɼ�����ַ����Ϣ
		StartAddr = loop * 6 + 13 + 1;
		
		Del_Validate_W_B(pData + StartAddr, 6);
	}
}


//ͨ���ڰ�����, �������Ĳɼ�����ַ
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
//ͨ���ڰ�����, �������Ĳɼ�����ַ
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
    IapAction = IAP_ACTION_ERASE_HASH1; //IAP��ʶλ
    while (IapAction != IAP_ACTION_IDLE);

    IapAction = IAP_ACTION_ERASE_HASH2; //IAP��ʶλ
    while (IapAction != IAP_ACTION_IDLE);	// Hold on	

    BFLS_BlockErase(EXT_FLASH_BLACK_MAP1_START);

    Delay_ms(750);

    BFLS_BlockErase(EXT_FLASH_WHITE_MAP1_START);

    Delay_ms(750);
    
//	    IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH1; //IAP��ʶλ
//	    while (IapAction != IAP_ACTION_IDLE);
//	
//	    IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH2; //IAP��ʶλ
//	    while (IapAction != IAP_ACTION_IDLE);
//	
//	    IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH1; //IAP��ʶλ
//	    while (IapAction != IAP_ACTION_IDLE);
//	
//	    IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH2; //IAP��ʶλ
//	    while (IapAction != IAP_ACTION_IDLE);	
}

/*************************************************************************
 * Function Name: htable1_insert
 * Parameters:  insert a record to hashtable1 
 * Return: the location in the hashtable1 if successed ,-1 for negative
 * Description: 
 *
 *************************************************************************/

int Hash_Table_Insert(HASHT *elem, int* pReturnHash) //��ӹ�ϣ��
{
    uint16 hash;
    uint16 index;
    hash = Cal_Hash_Value(elem->addr); //����õ���ϣ����	
    HASHT* pHasht;
	if(HASH_Table_Used==0)//��ǰʹ��table1
 		pHasht = htable1;
	else
		pHasht = htable2;
		
	if(pHasht[hash].status==EMPTY)//��
	{                        
        Updata_Hash_Table(hash, elem); //���¹�ϣ��
        *pReturnHash =  hash;
        return hash; //��������ֵ���˳�
	}
	 else if (pHasht[hash].status==OCCUPIED && (!LPC_Memory_CMP(pHasht[hash].addr, elem->addr, 6)))//��λ����ͬ����ַռ��
	{
        if (/*(LPC_Memory_CMP(pHasht[hash].falsh_sn, elem->falsh_sn ,6))
            || */(pHasht[hash].SubType != elem->SubType)) 			//�������sn��һ��                 
        {
            Updata_Hash_Table(hash, NULL);    //��ԭ�ȵļ�¼��Ч���ټ�������		                  
        }
        else
        {   // ����IDһ�������ٲ���                             
			*pReturnHash =  hash;
            return hash;//( - 1);
        }
	}

	index = (hash + 1) % (MAX_HASH_TABLE_FLASH_SIZE); //����������ռ�ݣ���������1���ж�
	while (hash != index)
	{
        if (pHasht[index].status==EMPTY)    //���������Ӧ��λ��Ϊ��
        {
            Updata_Hash_Table(index, elem); //�ѹ�ϣ���Լ�1����ӽ���ϣ��
            *pReturnHash =  index;
            return index; //��������ֵ���˳�ѭ��
        }
        else if ((pHasht[index].status==OCCUPIED) && (!LPC_Memory_CMP(pHasht[index].addr, elem->addr, 6)))//��λ����ͬ����ַռ��
        {
        	 if (/*(LPC_Memory_CMP(pHasht[index].falsh_sn, elem->falsh_sn ,6))
			 || */(pHasht[index].SubType != elem->SubType)) 	//�������sn��һ��                           
            {
                Updata_Hash_Table(index, NULL);//��ԭ�ȵļ�¼��Ч
            }
            else // ����IDһ�������ٲ���            
            {                                  
    			*pReturnHash =  index;    		
                 return index;//( - 1);
            }
        }
        index = (index + 1) %(MAX_HASH_TABLE_FLASH_SIZE);//��1����ת
	}
	*pReturnHash =  MAX_HASH_TABLE_FLASH_SIZE;    
	return ( MAX_HASH_TABLE_FLASH_SIZE);//��ϣ������
}

/*************************************************************************
 * Function Name: htable1_insert
 * Parameters:  insert a record to hashtable1 
 * Return: the location in the hashtable1 if successed ,-1 for negative
 * Description: 
 *
 *************************************************************************/

int Hash_Table_Insert1(HASHT *elem, int* pReturnHash) //��ӹ�ϣ��
{
    uint16 hash;
    uint16 index;
    hash = Cal_Hash_Value(elem->addr); //����õ���ϣ����	
    HASHT* pHasht;
	if(HASH_Table_Used==0)//��ǰʹ��table1
 		pHasht = htable1;
	else
		pHasht = htable2;
		
	if(pHasht[hash].status==EMPTY)//��
	{                        
        Updata_Hash_Table(hash, elem); //���¹�ϣ��
        *pReturnHash =  hash;
        return hash; //��������ֵ���˳�
	}
	 else if (pHasht[hash].status==OCCUPIED && (!LPC_Memory_CMP(pHasht[hash].addr, elem->addr, METER_ADDRESS_LENGTH_MAX)))//��λ����ͬ����ַռ��
	{
        if ((pHasht[hash].SubType != elem->SubType)) 			//�������sn��һ��                 
        {
            Updata_Hash_Table(hash, NULL);    //��ԭ�ȵļ�¼��Ч���ټ�������		                  
        }
        else
        {   // ����IDһ�������ٲ���                             
			*pReturnHash =  hash;
            return hash;//( - 1);
        }
	}

	index = (hash + 1) % (MAX_HASH_TABLE_FLASH_SIZE); //����������ռ�ݣ���������1���ж�
	while (hash != index)
	{
        if (pHasht[index].status == EMPTY)    //���������Ӧ��λ��Ϊ��
        {
            Updata_Hash_Table(index, elem); //�ѹ�ϣ���Լ�1����ӽ���ϣ��
            *pReturnHash =  index;
            return index; //��������ֵ���˳�ѭ��
        }
        else if ((pHasht[index].status==OCCUPIED) && (!LPC_Memory_CMP(pHasht[index].addr, elem->addr, METER_ADDRESS_LENGTH_MAX)))//��λ����ͬ����ַռ��
        {
            if ((pHasht[index].SubType != elem->SubType)) 	//�������sn��һ��                           
            {
                Updata_Hash_Table(index, NULL);//��ԭ�ȵļ�¼��Ч
            }
            else // ����IDһ�������ٲ���            
            {                                  
    			*pReturnHash =  index;    		
                 return index;//( - 1);
            }
        }
        index = (index + 1) % (MAX_HASH_TABLE_FLASH_SIZE);//��1����ת
	}
	*pReturnHash =  MAX_HASH_TABLE_FLASH_SIZE;    
	return ( MAX_HASH_TABLE_FLASH_SIZE);//��ϣ������
}



/*************************************************************************
 * Function Name: htable1_search
 * Parameters:  ammeter 's address 
 * Return: none
 * Description: ���ݵ���ַ����ID
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
    hash = Cal_Hash_Value(metertable); //����õ���ϣֵ
    if(HASH_Table_Used==0)
    {
        if ((htable1[hash].status== OCCUPIED) && (!(memcmp(metertable, htable1[hash].addr, 6))))
        {
            return hash; //���ع�ϣ����
        }
        index = (hash + 1)  % (MAX_HASH_TABLE_FLASH_SIZE); //��������������1���¶Ա�
        while (index != hash)
        {
            if ((htable1[index].status== OCCUPIED) && (!(memcmp(metertable, htable1[index].addr, 6)))) //�����ϣ��������ֵ
            {
                return index; //���ع�ϣ����
            }
            index = (index + 1)  % (MAX_HASH_TABLE_FLASH_SIZE); //��������������1���¶Ա�
        }
        return ( - 1); //����Ҳ���������-1
    }   
    else
    {
        if ((htable2[hash].status== OCCUPIED) && (!(memcmp(metertable, htable2[hash].addr, 6))))    //�����ϣ��������ֵ	      
        {
            return hash; //���ع�ϣ����
        }
        index = (hash + 1)  % (MAX_HASH_TABLE_FLASH_SIZE); //��������������1���¶Ա�
        while (index != hash)
        {	
            if ((htable2[index].status== OCCUPIED) && (!(memcmp(metertable, htable2[index].addr, 6))))    //�����ϣ��������ֵ
            {
                return index; //���ع�ϣ����
            }
            index = (index + 1)  % (MAX_HASH_TABLE_FLASH_SIZE); //��������������1���¶Ա�
        }
        return ( - 1); //����Ҳ���������-1
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
    hash = Cal_Hash_Value(metertable); //����õ���ϣֵ
    if(HASH_Table_Used==0)
    {
        if ((htable1[hash].status == OCCUPIED) && (!(memcmp(metertable, htable1[hash].addr, METER_ADDRESS_LENGTH_MAX))))
        {
            return hash; //���ع�ϣ����
        }
        index = (hash + 1) %(MAX_HASH_TABLE_FLASH_SIZE ); //��������������1���¶Ա�
        while (index != hash)
        {
            if ((htable1[index].status== OCCUPIED) && (!(memcmp(metertable, htable1[index].addr, METER_ADDRESS_LENGTH_MAX)))) //�����ϣ��������ֵ
            {
                return index; //���ع�ϣ����
            }
            index = (index + 1) %(MAX_HASH_TABLE_FLASH_SIZE ); //��������������1���¶Ա�
        }
        return ( - 1); //����Ҳ���������-1
    }   
    else
    {
        if ((htable2[hash].status== OCCUPIED) && (!(memcmp(metertable, htable2[hash].addr, METER_ADDRESS_LENGTH_MAX))))    //�����ϣ��������ֵ	      
        {
            return hash; //���ع�ϣ����
        }
        index = (hash + 1) %(MAX_HASH_TABLE_FLASH_SIZE ); //��������������1���¶Ա�
        while (index != hash)
        {	
            if ((htable2[index].status== OCCUPIED) && (!(memcmp(metertable, htable2[index].addr, METER_ADDRESS_LENGTH_MAX))))    //�����ϣ��������ֵ
            {
                return index; //���ع�ϣ����
            }
            index = (index + 1) %(MAX_HASH_TABLE_FLASH_SIZE); //��������������1���¶Ա�
        }
        return ( - 1); //����Ҳ���������-1
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
    hash = Cal_Hash_Value(metertable); //����õ���ϣֵ
    
    if ((pHtable[hash].status == OCCUPIED))
    {
        if(CompressAddr(pHtable[hash].addr, pHtable[hash].addrLen, tmp))
        {
            if(!(memcmp(metertable, pHtable[hash].addr, CON_DEV_ADDR_LEN_8)))
            {
                return hash; //���ع�ϣ����
            }
        }
    }
    index = (hash + 1) %(MAX_HASH_TABLE_FLASH_SIZE ); //��������������1���¶Ա�
    while (index != hash)
    {

        if ((pHtable[index].status == OCCUPIED))
        {
            if(CompressAddr(pHtable[index].addr, pHtable[index].addrLen, tmp))
            {
                if(!(memcmp(metertable, pHtable[index].addr, CON_DEV_ADDR_LEN_8)))
                {
                    return index; //���ع�ϣ����
                }
            }
        }
    
        index = (index + 1) %(MAX_HASH_TABLE_FLASH_SIZE ); //��������������1���¶Ա�
    }
    return ( - 1); //����Ҳ���������-1
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
void Updata_Hash_Table(uint16 hash, HASHT *elem) //���¹�ϣ��
{
    uint32 i;
    uint32 sector;
    uint32 location;
    HASHT *p;
    p=( HASHT *)updata_code_cache; 
    sector = hash / MAX_HASH_TABLE_SECTOR_SIZE;
    location = sector * MAX_HASH_TABLE_SECTOR_SIZE;

    if(IapAction == IAP_ACTION_IDLE)//�ڴ汣��
    {
        switch(HASH_Table_Used)
        {
        case 0://��ǰʹ��table1
            for (i = 0; i < MAX_HASH_TABLE_SECTOR_SIZE; i++)
            {
                if(htable1[location + i].status==OCCUPIED)//����ù�ϣԪ�ص�ֵ��Ч     
                {
                    p[i] = htable1[location + i]; //��ԭ����ϣ��������ݱ����ڻ�����
                }
                else if(htable1[location + i].status==EMPTY)
                {   // ����ù�ϣԪ�ص�ֵ��Ч                     
                    memset(&p[i], 0xFF, sizeof(HASHT)); //�����е������ó�0X0***
                }
                else if(htable1[location + i].status==DEAD)
                {   // ����ù�ϣԪ�ص�ֵ��Ч                     
                    memset(&p[i], 0x0, sizeof(HASHT)); //�����е������ó�0X0***
                }

            }	
            break;
        case 1://��ǰʹ��table2
            for (i = 0; i < MAX_HASH_TABLE_SECTOR_SIZE; i++)
            {

                if(htable2[location + i].status==OCCUPIED)//����ù�ϣԪ�ص�ֵ��Ч     
                {
                    p[i] = htable2[location + i]; //��ԭ����ϣ��������ݱ����ڻ�����
                }
                else if(htable2[location + i].status==EMPTY)
                {   // ����ù�ϣԪ�ص�ֵ��Ч                     
                    memset(&p[i], 0xFF, sizeof(HASHT)); //�����е������ó�0X0***
                }
                else if(htable2[location + i].status==DEAD)
                {   // ����ù�ϣԪ�ص�ֵ��Ч                     
                    memset(&p[i], 0x0, sizeof(HASHT)); //�����е������ó�0X0***
                }
            }	
            break; 
        }
        

        if (elem != NULL)//���������ӵĹ�ϣԪ��        
        {
                // ���¹�ϣ����
    		memcpy((uint8 *)&p[hash - location], (uint8 *)elem, sizeof(HASHT)); //������ӵĹ�ϣ��Ԫ�طŵ���ϣ�����
//	    		memcpy(p[hash - location].falsh_sn, elem->falsh_sn,6);
    		p[hash - location].status	= OCCUPIED;	
//	    		p[hash - location].SubType = elem->SubType;
//	    		p[hash - location].src = elem->src;
//	            p[hash - location].devType = elem->devType;
            
    		shadow_space[hash].handled=UN_HANDLED;              
    		shadow_space[hash].times= MeterDeadCount;
    		I2cWrite(0xA0, (uint8*)&shadow_space[hash], hash,1);//д��E2PR0M		
        }
        else
        {           
        	//p[hash - location].status	= DEAD;	
        	memset(&p[hash - location],0x0,sizeof(HASHT));
        }
	

                // д��flash
		if(HASH_Table_Used==0)
		{
			IapAction = IAP_ACTION_TRANSFER_HASH1;//дtable1
		    HashUpdateSectorAdd = FLASH_MAP_ADD + sector * MAX_HASH_TABLE_SECTOR_SIZE * MAX_HASH_TABLE_CACHE_SIZE ;
		}
		else
		{
			IapAction = IAP_ACTION_TRANSFER_HASH2;//дtable2
		    HashUpdateSectorAdd = FLASH_MAP_ADD2 + sector * MAX_HASH_TABLE_SECTOR_SIZE * MAX_HASH_TABLE_CACHE_SIZE;
		}
		while (IapAction != IAP_ACTION_IDLE);   // hold on
		
		if(Write_Sucess==ERR)//����ɹ�ʧ��
		{
		    Hash_Correct(HashUpdateSectorAdd,(uint8*)&updata_code_cache);		
		}
    }
}

/*********************************************************************
*��ϣ����Ч����ת��
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
                    if (htable1[i+(j*BLOCK_NO)].status!= DEAD) //����ù�ϣԪ�ص�ֵ��Ч   
                    {			                        
                        memcpy(p+i,htable1+i+j*BLOCK_NO,sizeof(HASHT));//����Ч���ݱ����ڻ�����	
                    }
                    else
                    {   // ����ù�ϣԪ�ص�ֵ��Ч                     
                        memset(p+i, 0xff, sizeof(HASHT)); //�����е������ó�0XFF***
                    }				
                }		
                IapAction = IAP_ACTION_TRANSFER_HASH2;
                HashUpdateSectorAdd = FLASH_MAP_ADD2 +(j*256);
                while (IapAction != IAP_ACTION_IDLE);	
            }

            IapAction = IAP_ACTION_ERASE_HASH1;//������ԭ��	        
            while (IapAction != IAP_ACTION_IDLE);	
            break;	
        }
        	
        case 0://table2-->table1
        {
            for(j=0;j<BLOCK_NO;j++)	
            {
                for (i = 0; i < MAX_HASH_TABLE_SECTOR_SIZE; i++)
                {
                    if (htable2[i+(j*BLOCK_NO)].status!= DEAD) //����ù�ϣԪ�ص�ֵ��Ч    				            
                    {
                        memcpy(p+i,htable2+i+j*BLOCK_NO,sizeof(HASHT));//����Ч���ݱ����ڻ�����
                    }
                    else // ����ù�ϣԪ�ص�ֵ��Ч    
                    {                   
                        memset(p+i, 0xff, sizeof(HASHT)); //�����е������ó�0XFF***
                    }								
                }		
                IapAction = IAP_ACTION_TRANSFER_HASH1;
                HashUpdateSectorAdd = FLASH_MAP_ADD +(j*256) ;
                while (IapAction != IAP_ACTION_IDLE);		
            }		

            IapAction = IAP_ACTION_ERASE_HASH2;	  //������ԭ��      
            while (IapAction != IAP_ACTION_IDLE);	
            break;		
        }
    }
}

/*************************************************************************
*FLASHд�����Ĳ���
*
**************************************************************************/
void Hash_Correct(uint32 dst,uint8* src )
{
    uint8 i,loop;
    HASHT *p;
    p=(HASHT *)updata_code_cache;
    //uint8 cache[256]={0};

    //	memcpy(cache,src,256);//��������
	
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
                    if (htable1[i+(loop*MAX_HASH_TABLE_SECTOR_SIZE)].status!= DEAD) //����ù�ϣԪ�ص�ֵ��Ч    
                    {
                        memcpy(p+i,htable1+i+loop*MAX_HASH_TABLE_SECTOR_SIZE,sizeof(HASHT));//����Ч���ݱ����ڻ�����						
                    }
                    else
                    {   // ����ù�ϣԪ�ص�ֵ��Ч                     
                        memset(p+i, 0xff, sizeof(HASHT)); //�����е������ó�0XFF***
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
			
			IapAction = IAP_ACTION_ERASE_HASH1;//������ԭ��	        
			while (IapAction != IAP_ACTION_IDLE);	
			HASH_Table_Used++;
			HASH_Table_Used=(HASH_Table_Used&1);   //1��ʾ֮ǰ��Table1,֮����Table2;    0�෴
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
                    if (htable2[i+(loop*MAX_HASH_TABLE_SECTOR_SIZE)].status!= DEAD) //����ù�ϣԪ�ص�ֵ��Ч    				            
                    {
                        memcpy(p+i,htable2+i+(loop*MAX_HASH_TABLE_SECTOR_SIZE),sizeof(HASHT));//����Ч���ݱ����ڻ�����							 
                    }
                    else
                    {   // ����ù�ϣԪ�ص�ֵ��Ч                     
                        memset(p+i, 0xff, sizeof(HASHT)); //�����е������ó�0XFF***
                    }								
                }		

                HashUpdateSectorAdd = FLASH_MAP_ADD +(loop*256);
                if(HashUpdateSectorAdd==dst)
                {
                    memcpy(p,src,256);//ԭ��д��Ĵ�������
                }		
                IapAction = IAP_ACTION_TRANSFER_HASH1;					 
                while (IapAction != IAP_ACTION_IDLE);
            }		

            IapAction = IAP_ACTION_ERASE_HASH2;	  //������ԭ��      
            while (IapAction != IAP_ACTION_IDLE);	
            HASH_Table_Used++;
            HASH_Table_Used=(HASH_Table_Used&1);   //1��ʾ֮ǰ��Table1,֮����Table2;    0�෴
            I2cWrite(0xA0, (uint8*) &HASH_Table_Used, FM_HASH_USED, 1);   
            Write_Sucess=SUCCESS;
            break;		
        }
    }
}

//ͨ���ڰ�����, �������ı��ַ��ַ
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
//У��panid
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


