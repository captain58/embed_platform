/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright HEX NET
 *
 *    File name   : LinkMng.c
 *    Description : real meter map; white meter; black meter.
 *
 *    History :
 *    1. Date        : oct 12, 2016
 *       Author      : yzy
 *       Description : Create
 *
 **************************************************************************/
#define LINK_VAR
#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"
#include "user_func.h"
#include "wirelessSendCache.h"
#include "LinkMng.h"
#include  "frmcfg.h"
#include "Nodes.h"
/*************************************************************************
 * Function Name: FillRouteInfo
 * Parameters:  
 * Return:      往包里填路由
 * Description:
 *
 *************************************************************************/
uint8 Fill_Route_INFO(uint8 *data, RINFO route, uint8 id)
{

    uint8 i;        
    if (route.rdepth > MAX_ROUTE_DEPTH)
    {
        return id;
    }
    if (route.rtype == MASTER_ROUTE_PATH)
    // master router
    {               
        for (i = 0; i < route.rdepth; i++)
        {
            data[i] = cltor[id].father;
            id = cltor[id].father;
        }
    }
    else
    {                
        for (i = 0; i < route.rdepth; i++)
        {
            data[i] = cltor[id].mother;
            id = cltor[id].mother;
        }
    }
    return id;
}

/*************************************************************************
 * Function Name: FillRouteInfo
 * Parameters:  
 * Return:  
 * Description:
 *
 *************************************************************************/\
uint16 Select_Route(uint16 Sid, uint16* pData, uint8* pLen)
{
	uint8 rc = 0;
	COLLECTOR* pCilor = &cltor[Sid];
	uint8 bMother  = 0;
	uint8 Loop = 0;
	uint8 Error = 0;;
	if((*pLen) <= 12)
	{
		if((*pLen) > 1)
		{
			for(Loop = 0; Loop < (*pLen) - 1; Loop++)
			{
				if(Sid == pData[Loop])
				{
					Error = 1;

#ifdef _Debug
                    memset(Test, 0X11, sizeof(Test));
                    UART_PutArray(UART1, Test, 10); // for test		
#endif
					break;
				}
			}
		}

		if(0 == Error)
		{
			if(pCilor->father != 0)
			{
				if(pCilor->father == localid)
					rc = 1;
				else
				{
					pData[*pLen] = pCilor->father;
					(*pLen) ++;			
					rc = Select_Route(pCilor->father, pData, pLen);
					if(0 == rc)	
					{
						(*pLen) --;
						bMother = 1;
					}
				}
			}
			else
				bMother = 1;

			 if(bMother)
			 {
				if(pCilor->mother != 0)
				{
					if(pCilor->mother == localid)
						rc = 1;		
					else
					{
						pData[*pLen] = pCilor->mother;
						(*pLen) ++;			
						rc = Select_Route(pCilor->mother, pData, pLen);
						if(0 == rc)
							(*pLen)--;
					}	
				}
			 }
		}
	}
    else 
      Error = 1;
	return rc;
}


//------------------------------------------------------------------------
//	ROUTE Fill_Route(uint8 *data, uint8 type, uint8 id)
//	{
//	    uint8 temp;
//	    ROUTE route;
//	    uint8 Path[20];
//	    // 默认配置
//	    route.rinfo.downlink = ROUTE_DOWN_LINK;
//	    route.rinfo.rtype = type;
//	    route.rinfo.rdepth = 0;
//	    route.did = id;
//	
//	
//		uint8 Len = 0;        
//		if(Select_Route(id, Path, &Len))
//		{
//			if(Len > 0)
//				route.did = Path[Len - 1];
//			else
//				route.did = id;
//			route.rinfo.rdepth = Len;
//			route.rinfo.rtype = MASTER_ROUTE_PATH;
//	                
//			for(temp = 0; temp < Len; temp++)
//			//	data[temp] = Path[Len - temp - 1];
//				data[temp] = Path[temp];
//			
//			//TEST
//	#ifdef _Debug
//	        memset(Test, 0, sizeof(Test));
//	        Test[0] = 0x55;
//	        Test[1] = id;
//	        for(uint8 Loop = 0; Loop < Len; Loop++)
//	        {
//	        	Test[Loop + 2] =  data[Loop];
//	        }
//	        UART_PutArray(UART1, Test, Len + 2); // for test		
//	#endif
//		}
//		else
//	    {
//			route.rinfo.rtype = ERROR_ROUTE_PATH;
//			
//	#ifdef _Debug
//	        Test[0] = 0x56;
//	        UART_PutArray(UART1, Test, 1); // for test
//	#endif
//	    }
//	
//	
//	    return route;
//	}
ROUTE Fill_Route(uint8 *data, uint8 type, uint16 id)
{
    uint8 temp;
    ROUTE route;
    uint16 Path[20];
    // 默认配置
    route.rinfo.downlink = ROUTE_DOWN_LINK;
    route.rinfo.rtype = type;
    route.rinfo.rdepth = 0;
    route.did = id;

    memset((uint8 *)&Path, 0, 40);
	uint8 Len = 0;        
	if(Select_Route(id, Path, &Len))
	{
		if(Len > 0)
			route.did = Path[Len - 1];
		else
			route.did = id;
		route.rinfo.rdepth = Len;
		route.rinfo.rtype = MASTER_ROUTE_PATH;
                
//			for(temp = 0; temp < Len; temp++)
//			//	data[temp] = Path[Len - temp - 1];
//				data[temp] = Path[temp];
        for(temp = 0; temp < Len; temp++)
        {
            //  data[temp] = Path[Len - temp - 1];
            //data[temp] = Path[temp];
            memcpy(data + temp * 6, cltor[Path[Len - temp - 1]].devAddr, 6);
        }
	}
	else
    {
		route.rinfo.rtype = ERROR_ROUTE_PATH;
		
    }


    return route;
}


ROUTE Fill_Route2(uint8 *data, uint8 type, uint8 protocol, uint8 addrType, uint16 id)
{
    uint8 temp;
    ROUTE route;
    uint16 Path[20];
    uint8 m = 0, addrlen = 0;
    // 默认配置
    route.rinfo.downlink = ROUTE_DOWN_LINK;
    route.rinfo.rtype = type;
    route.rinfo.rdepth = 0;
    route.did = id;

    memset((uint8 *)&Path, 0, 40);
	uint8 Len = 0;        
	if(Select_Route(id, Path, &Len))
	{
		if(Len > 0)
			route.did = Path[Len - 1];
		else
			route.did = id;
		route.rinfo.rdepth = Len;
		route.rinfo.rtype = MASTER_ROUTE_PATH;
                
//			for(temp = 0; temp < Len; temp++)
//			//	data[temp] = Path[Len - temp - 1];
//				data[temp] = Path[temp];
        if(protocol == PST_FRM_802_R_NO)
        {
            switch(addrType & 0x03)
            {
            case 1:
                for(temp = 0; temp < Len; temp++)
                {
                    addrlen = cltor[Path[Len - temp - 1]].addrLen;
                    data[m++] = addrlen - 1;
                    memcpy(data + m, cltor[Path[Len - temp - 1]].devAddr, addrlen); 
                    m+=addrlen;
                    //memcpy(data + temp * CON_DEV_ADDR_LEN_8, cltor[Path[Len - temp - 1]].devAddr, CON_DEV_ADDR_LEN_8);
                }
                
                break;
            case 2:
                for(temp = 0; temp < Len; temp++)
                {
                    memcpy(data + temp * 2, (uint8 *)&Path[Len - temp - 1], 2);
                }
                
                m+=2*Len;
                break;
            default:
            case 0:
            case 3:
                for(temp = 0; temp < Len; temp++)
                {
                    memcpy(data + temp * CON_DEV_ADDR_LEN_8, cltor[Path[Len - temp - 1]].devAddr, CON_DEV_ADDR_LEN_8);
                }
                m+=CON_DEV_ADDR_LEN_8*Len;
                break;
            }

        }
        else
        {
            if(addrType == 3)
            {

                for(temp = 0; temp < Len; temp++)
                {
                    memcpy(data + temp * CON_DEV_ADDR_LEN_8, cltor[Path[Len - temp - 1]].devAddr, CON_DEV_ADDR_LEN_8);
                }
                m+=CON_DEV_ADDR_LEN_8*Len;
            }
            else
            {
                for(temp = 0; temp < Len; temp++)
                {
                    memcpy(data + temp * 2, (uint8 *)&Path[Len - temp - 1], 2);
                }
                m+=2*Len;
            }
        }
	}
	else
    {
		route.rinfo.rtype = ERROR_ROUTE_PATH;
		
    }

    route.len = m;
    return route;
}


//--------------------------------------------------------------------------------------
void UpdataRouteInfo(uint16 SourceID, uint8* pRouteData, uint8 RouteNum)
{
//		uint8 father; 
//	    uint8 hop, loop, Sid;
//		COLLECTOR * pCltor;
//		COLLECTOR_SHADOW * pCltor_shadow;    
//		uint8 Sourcefather = 0;
//		uint32 u32CurSlot = curslottime;
//		//更新源节点的路由信息
//		if(0 == RouteNum)
//			Sourcefather = 1;
//		else if((pRouteData[0] < MAX_SUP_SS_NUM) && (pRouteData[0]  > 1) && (pRouteData[0] <= rfpara.rf_slotnum))
//	//			&& (pRouteData[0] < MAX_REAL_SUP_SS_NUM))
//		{
//			if(cltor[pRouteData[0]].devAddr[0] <= 0x99)
//				Sourcefather = pRouteData[0];
//		}
//		//TODO 无线调试点需要支持中继转发时，需保存路径
//		if(((Sourcefather > 0) && (SourceID != 1) && (SourceID < rfpara.rf_slotnum)))// && (SourceID < MAX_REAL_SUP_SS_NUM))
//			//|| ((SourceID >= MAX_REAL_SUP_SS_NUM) && (SourceID <= MAX_SUP_SS_NUM)))
//		{
//			pCltor = &(cltor[SourceID]);
//	        pCltor_shadow = &(cltor_shadow[SourceID]);
//	
//			if(pCltor->father == Sourcefather)
//			{
//				pCltor_shadow->father_update = u32CurSlot;
//			}
//			else 
//			{
//				if(0 == pCltor->mother)
//					pCltor->mother = Sourcefather;
//				else
//					pCltor->mother = pCltor->father;
//				pCltor->father =  Sourcefather;
//				pCltor_shadow->father_update = u32CurSlot;
//				pCltor_shadow->mother_update = u32CurSlot; 
//			}
//			pCltor->hop = RouteNum + 2;
//		}
//		
//	    uint8 bSend = 0;
//	    uint8 loop1;
//		if(RouteNum > 1)
//		{
//			for(loop1 = 0; loop1 < RouteNum -1 ; loop1++)
//			{
//				uint8 temp = pRouteData[loop1];
//				for(loop = loop1 + 1; loop < RouteNum; loop++)
//				{
//					if(pRouteData[loop] == temp)
//					{
//						temp = 0;
//						bSend = 1;
//						loop1 = RouteNum;
//						break;
//					}
//				}
//			}
//		}
//		if(bSend)
//		{
//	#ifdef _Debug
//	        memset(Test, 0XFF, sizeof(Test));
//	        UART_PutArray(UART1, Test, 10); // for test
//	
//	        memset(Test, 0, sizeof(Test));
//	        Test[0] = 0xf4;
//	        Test[1] = SourceID;
//	        for(uint8 Loop = 0; Loop < RouteNum; Loop++)
//	        {
//	        	Test[Loop + 2] = pRouteData[Loop];
//	        }
//	        UART_PutArray(UART1, Test, RouteNum + 2); // for test
//	#endif
//		}
//		else
//		{
//	#ifdef _Debug
//	        memset(Test, 0, sizeof(Test));
//	        Test[0] = 0xfc;
//	        Test[1] = SourceID;
//	        for(uint8 Loop = 0; Loop < RouteNum; Loop++)
//	        {
//	        	Test[Loop + 2] = pRouteData[Loop];
//	        }
//	        UART_PutArray(UART1, Test, RouteNum + 2); // for test 
//	#endif
//	
//			//更新路由过程中的中继节点
//			father = 1;
//			hop =1;
//			for(loop = RouteNum; loop > 0; loop--)
//			{
//				Sid = pRouteData[loop - 1];
//				if((Sid > 0) && (Sid < rfpara.rf_slotnum)/* && (Sid < MAX_REAL_SUP_SS_NUM)*/)
//				{
//					pCltor = &(cltor[Sid]);	
//	                pCltor_shadow = &(cltor_shadow[Sid]);
//	                
//					if(pCltor->devAddr[0] <= 0x99)
//					{
//						if(pCltor->father == father)
//						{
//							pCltor_shadow->father_update = u32CurSlot + hop;
//						}
//						else
//						{				
//							if(0 == pCltor->mother)
//								pCltor->mother = father;
//							else	
//								pCltor->mother = pCltor->father;
//	                        
//							pCltor->father =  father;
//							pCltor_shadow->father_update = u32CurSlot + hop;
//							pCltor_shadow->mother_update = u32CurSlot + hop; 
//						}
//						father = Sid;
//						pCltor->hop = hop +1;								
//						hop ++;									
//					}
//					else
//						break;				
//				}
//				else
//					break;
//			}
//		}
			
}

void UpdataRouteInfo1(uint16 SourceID, PKT *pkt)
{
	uint16 father; 
    uint16 hop, loop, Sid;
	COLLECTOR* pCltor;
	COLLECTOR_SHADOW * pCltor_shadow;    
    
	uint16 Sourcefather = 0;
	uint32 u32CurSlot = curslottime;
    uint16 pRouteData[16];    
    memset((uint8 *)&pRouteData[0], 0, 32);
    uint8 RouteNum = pkt->head.apdu.stInfo.stUp.bit1.routeNum;
    //uint8 addrPerLen = (pkt->apdu.stInfo.stUp.bit1.addrFlg == 1) ? 2 : 6;
    uint8 bSend = 0;
    uint8 loop1;
    if(pkt->head.apdu.stInfo.stUp.bit1.addrFlg == 1)
    {
        
        memcpy(pRouteData, pkt->head.apdu.addr + 2, pkt->head.apdu.addrlen - 2);
    }
    else
    {
		for(loop1 = 0; loop1 < RouteNum ; loop1++)
        {
            pRouteData[loop1] = Sn_Search_Id(pkt->head.apdu.addr + 6 * (loop1 + 1));
        }      
    }
	//更新源节点的路由信息
	if(0 == RouteNum)
    {   
		Sourcefather = 1;
    }
	else if((pRouteData[0] <= MAX_SUP_SS_NUM) && (pRouteData[0]  > 0) && (pRouteData[0] <= rfpara.rf_slotnum))
//			&& (pRouteData[0] < MAX_REAL_SUP_SS_NUM))
	{
		if(cltor[pRouteData[0]].devAddr[0] < 0xFF)
			Sourcefather = pRouteData[0];
	}
	//TODO 无线调试点需要支持中继转发时，需保存路径
	if(((Sourcefather > 0) && (SourceID != 1) && (SourceID <= rfpara.rf_slotnum)))// && (SourceID < MAX_REAL_SUP_SS_NUM))
//		|| ((SourceID >= MAX_REAL_SUP_SS_NUM) && (SourceID <= MAX_SUP_SS_NUM)))
	{
		pCltor = &(cltor[SourceID]);
        pCltor_shadow = &(cltor_shadow[SourceID]);
        
		if(pCltor->father == Sourcefather)
		{
			pCltor_shadow->father_update = u32CurSlot;
		}
		else 
		{
			if(0 == pCltor->mother)
				pCltor->mother = Sourcefather;
			else
				pCltor->mother = pCltor->father;
			pCltor->father =  Sourcefather;
			pCltor_shadow->father_update = u32CurSlot;
//				pCltor_shadow->mother_update = u32CurSlot; 
		}
		pCltor->hop = RouteNum + 2;
	}
	
	if(RouteNum > 1)
	{
		for(loop1 = 0; loop1 < RouteNum -1 ; loop1++)
		{
			uint16 temp = pRouteData[loop1];
			for(loop = loop1 + 1; loop < RouteNum; loop++)
			{
				if(pRouteData[loop] == temp)
				{
					temp = 0;
					bSend = 1;
					loop1 = RouteNum;
					break;
				}
			}
		}
	}
	if(bSend)
	{
#ifdef _Debug
        memset(Test, 0XFF, sizeof(Test));
        UART_PutArray(UART1, Test, 10); // for test

        memset(Test, 0, sizeof(Test));
        Test[0] = 0xf4;
        Test[1] = SourceID;
        for(uint8 Loop = 0; Loop < RouteNum; Loop++)
        {
        	Test[Loop + 2] = pRouteData[Loop];
        }
        UART_PutArray(UART1, Test, RouteNum + 2); // for test
#endif
	}
	else
	{
#ifdef _Debug
        memset(Test, 0, sizeof(Test));
        Test[0] = 0xfc;
        Test[1] = SourceID;
        for(uint8 Loop = 0; Loop < RouteNum; Loop++)
        {
        	Test[Loop + 2] = pRouteData[Loop];
        }
        UART_PutArray(UART1, Test, RouteNum + 2); // for test 
#endif

		//更新路由过程中的中继节点
		father = 1;
		hop =1;
		for(loop = RouteNum; loop > 0; loop--)
		{
			Sid = pRouteData[loop - 1];
			if((Sid > 1) && (Sid <= rfpara.rf_slotnum))// && (Sid < MAX_REAL_SUP_SS_NUM))
			{
				pCltor = &(cltor[Sid]);	
                pCltor_shadow = &(cltor_shadow[Sid]);
                
				if(pCltor->devAddr[0] < 0xFF)
				{
					if(pCltor->father == father)
					{
						pCltor_shadow->father_update = u32CurSlot + hop;
					}
					else
					{				
						if(0 == pCltor->mother)
							pCltor->mother = father;
						else	
							pCltor->mother = pCltor->father;
                        
						pCltor->father =  father;
						pCltor_shadow->father_update = u32CurSlot + hop;
//							pCltor_shadow->mother_update = u32CurSlot + hop; 
					}
					father = Sid;
					pCltor->hop = hop +1;								
					hop ++;									
				}
				else
					break;				
			}
			else
				break;
		}
	}
			
}

//	void UpdataRouteInfo2(uint16 SourceID, uint8 addrFlg, uint8* pAddr, uint8 RouteNum)
//	{
//		uint16 father; 
//	    uint16 hop, loop, Sid;
//		COLLECTOR* pCltor;
//		COLLECTOR_SHADOW * pCltor_shadow;    
//	    
//		uint16 Sourcefather = 0;
//		uint32 u32CurSlot = curslottime;
//	    uint16 pRouteData[16];    
//	    memset((uint8 *)&pRouteData[0], 0, 32);
//	    //uint8 RouteNum = pkt->apdu.stInfo.stUp.bit1.routeNum;
//	    //uint8 addrPerLen = (pkt->apdu.stInfo.stUp.bit1.addrFlg == 1) ? 2 : 6;
//	    uint8 bSend = 0;
//	    uint8 loop1;
//	    if(addrFlg == 1)
//	    {
//	        
//	        memcpy(pRouteData, pAddr, RouteNum);
//	    }
//	    else
//	    {
//			for(loop1 = 0; loop1 < RouteNum ; loop1++)
//	        {
//	            pRouteData[loop1] = Sn_Search_Id(pAddr + 6 * (loop1));
//	        }      
//	    }
//		//更新源节点的路由信息
//		if(0 == RouteNum)
//	    {   
//			Sourcefather = 1;
//	    }
//		else if((pRouteData[0] <= MAX_SUP_SS_NUM) && (pRouteData[0]  > 0) && (pRouteData[0] <= rfpara.rf_slotnum))
//	//			&& (pRouteData[0] < MAX_REAL_SUP_SS_NUM))
//		{
//			if(cltor[pRouteData[0]].devAddr[0] <= 0x99)
//				Sourcefather = pRouteData[0];
//		}
//		//TODO 无线调试点需要支持中继转发时，需保存路径
//		if(((Sourcefather > 0) && (SourceID != 1) && (SourceID <= rfpara.rf_slotnum)))// && (SourceID < MAX_REAL_SUP_SS_NUM))
//	//		|| ((SourceID >= MAX_REAL_SUP_SS_NUM) && (SourceID <= MAX_SUP_SS_NUM)))
//		{
//			pCltor = &(cltor[SourceID]);
//	        pCltor_shadow = &(cltor_shadow[SourceID]);
//	        
//			if(pCltor->father == Sourcefather)
//			{
//				pCltor_shadow->father_update = u32CurSlot;
//			}
//			else 
//			{
//				if(0 == pCltor->mother)
//					pCltor->mother = Sourcefather;
//				else
//					pCltor->mother = pCltor->father;
//				pCltor->father =  Sourcefather;
//				pCltor_shadow->father_update = u32CurSlot;
//	//				pCltor_shadow->mother_update = u32CurSlot; 
//			}
//			pCltor->hop = RouteNum + 2;
//		}
//		
//		if(RouteNum > 1)
//		{
//			for(loop1 = 0; loop1 < RouteNum -1 ; loop1++)
//			{
//				uint16 temp = pRouteData[loop1];
//				for(loop = loop1 + 1; loop < RouteNum; loop++)
//				{
//					if(pRouteData[loop] == temp)
//					{
//						temp = 0;
//						bSend = 1;
//						loop1 = RouteNum;
//						break;
//					}
//				}
//			}
//		}
//		if(bSend)
//		{
//	#ifdef _Debug
//	        memset(Test, 0XFF, sizeof(Test));
//	        UART_PutArray(UART1, Test, 10); // for test
//	
//	        memset(Test, 0, sizeof(Test));
//	        Test[0] = 0xf4;
//	        Test[1] = SourceID;
//	        for(uint8 Loop = 0; Loop < RouteNum; Loop++)
//	        {
//	        	Test[Loop + 2] = pRouteData[Loop];
//	        }
//	        UART_PutArray(UART1, Test, RouteNum + 2); // for test
//	#endif
//		}
//		else
//		{
//	#ifdef _Debug
//	        memset(Test, 0, sizeof(Test));
//	        Test[0] = 0xfc;
//	        Test[1] = SourceID;
//	        for(uint8 Loop = 0; Loop < RouteNum; Loop++)
//	        {
//	        	Test[Loop + 2] = pRouteData[Loop];
//	        }
//	        UART_PutArray(UART1, Test, RouteNum + 2); // for test 
//	#endif
//	
//			//更新路由过程中的中继节点
//			father = 1;
//			hop =1;
//			for(loop = RouteNum; loop > 0; loop--)
//			{
//				Sid = pRouteData[loop - 1];
//				if((Sid > 1) && (Sid <= rfpara.rf_slotnum))// && (Sid < MAX_REAL_SUP_SS_NUM))
//				{
//					pCltor = &(cltor[Sid]);	
//	                pCltor_shadow = &(cltor_shadow[Sid]);
//	                
//					if(pCltor->devAddr[0] <= 0x99)
//					{
//						if(pCltor->father == father)
//						{
//							pCltor_shadow->father_update = u32CurSlot + hop;
//						}
//						else
//						{				
//							if(0 == pCltor->mother)
//								pCltor->mother = father;
//							else	
//								pCltor->mother = pCltor->father;
//	                        
//							pCltor->father =  father;
//							pCltor_shadow->father_update = u32CurSlot + hop;
//	//							pCltor_shadow->mother_update = u32CurSlot + hop; 
//						}
//						father = Sid;
//						pCltor->hop = hop +1;								
//						hop ++;									
//					}
//					else
//						break;				
//				}
//				else
//					break;
//			}
//		}
//				
//	}

void UpdataRouteInfo2(uint16 SourceID, uint8 protocol, uint8 addrFlg, uint8* pAddr, uint8 RouteNum)
{
	uint16 father; 
    uint16 hop, loop, Sid;
	COLLECTOR* pCltor;
	COLLECTOR_SHADOW * pCltor_shadow;    
    uint8 m = 0, addrlen = 0;
	uint16 Sourcefather = 0;
	uint32 u32CurSlot = curslottime;
    uint16 pRouteData[32];    
    memset((uint8 *)&pRouteData[0], 0, 32);
    //uint8 RouteNum = pkt->apdu.stInfo.stUp.bit1.routeNum;
    //uint8 addrPerLen = (pkt->apdu.stInfo.stUp.bit1.addrFlg == 1) ? 2 : 6;
    uint8 bSend = 0;
    uint8 loop1;
    
    if(protocol == PST_FRM_802_R_NO)
    {
        switch(addrFlg & 0x03)
        {
        case 0:
                    
            break;
        case 1:
            for(uint8 i = 0; i < RouteNum; i++)
            {
                addrlen = pAddr[m] & 0x0f; 
                pRouteData[i] = Sn_Search_Id2(pAddr + m + 1, addrlen + 1);

                addrlen += 2;//第一个字节和4bit从0开始计数
                m+=addrlen;
            }
            break;
        case 2:
            memcpy(pRouteData, pAddr, RouteNum * 2);
            break;
        case 3:
            for(uint8 i = 0; i < RouteNum; i++)
            {
                pRouteData[i] = Sn_Search_Id2(pAddr + m, CON_DEV_ADDR_LEN_8);

                m+=CON_DEV_ADDR_LEN_8;
            }
            break;
        default:
            break;     
        }
    }
    else
    {
        if(addrFlg == 1)
        {
            
            memcpy(pRouteData, pAddr, RouteNum * 2);
        }
        else
        {
    		for(loop1 = 0; loop1 < RouteNum ; loop1++)
            {
                pRouteData[loop1] = Sn_Search_Id(pAddr + 6 * (loop1));
            }      
        }
    }

//	    LOG_DEBUG( DBGFMT"UpdataRouteInfo2 ID[%x]  \n",DBGARG, SourceID);                        
//	    LOG_DUMPHEX(LOG_LEVEL_DEBUG, "RouteData : \n", (uint8 *)&pRouteData[0], RouteNum * 2);

    
	//更新源节点的路由信息
	if(0 == RouteNum)
    {   
		Sourcefather = 1;
    }
	else if((pRouteData[0] <= MAX_SUP_SS_NUM) && (pRouteData[0]  > 0) && (pRouteData[0] <= rfpara.rf_slotnum))
//			&& (pRouteData[0] < MAX_REAL_SUP_SS_NUM))
	{
		if(cltor[pRouteData[0]].devAddr[0] < 0xFF)
			Sourcefather = pRouteData[0];
	}
	//TODO 无线调试点需要支持中继转发时，需保存路径
	if(((Sourcefather > 0) && (SourceID != 1) && (SourceID <= rfpara.rf_slotnum)))// && (SourceID < MAX_REAL_SUP_SS_NUM))
//		|| ((SourceID >= MAX_REAL_SUP_SS_NUM) && (SourceID <= MAX_SUP_SS_NUM)))
	{
		pCltor = &(cltor[SourceID]);
        pCltor_shadow = &(cltor_shadow[SourceID]);
        
		if(pCltor->father == Sourcefather)
		{
			pCltor_shadow->father_update = u32CurSlot;
		}
		else 
		{
			if(0 == pCltor->mother)
				pCltor->mother = Sourcefather;
			else
				pCltor->mother = pCltor->father;
			pCltor->father =  Sourcefather;
			pCltor_shadow->father_update = u32CurSlot;
//				pCltor_shadow->mother_update = u32CurSlot; 
		}
		pCltor->hop = RouteNum + 2;
	}
	
	if(RouteNum > 1)
	{
		for(loop1 = 0; loop1 < RouteNum -1 ; loop1++)
		{
			uint16 temp = pRouteData[loop1];
			for(loop = loop1 + 1; loop < RouteNum; loop++)
			{
				if(pRouteData[loop] == temp)
				{
					temp = 0;
					bSend = 1;
					loop1 = RouteNum;
					break;
				}
			}
		}
	}
	if(bSend)
	{
#ifdef _Debug
        memset(Test, 0XFF, sizeof(Test));
        UART_PutArray(UART1, Test, 10); // for test

        memset(Test, 0, sizeof(Test));
        Test[0] = 0xf4;
        Test[1] = SourceID;
        for(uint8 Loop = 0; Loop < RouteNum; Loop++)
        {
        	Test[Loop + 2] = pRouteData[Loop];
        }
        UART_PutArray(UART1, Test, RouteNum + 2); // for test
#endif
	}
	else
	{
#ifdef _Debug
        memset(Test, 0, sizeof(Test));
        Test[0] = 0xfc;
        Test[1] = SourceID;
        for(uint8 Loop = 0; Loop < RouteNum; Loop++)
        {
        	Test[Loop + 2] = pRouteData[Loop];
        }
        UART_PutArray(UART1, Test, RouteNum + 2); // for test 
#endif

		//更新路由过程中的中继节点
		father = 1;
		hop =1;
		for(loop = RouteNum; loop > 0; loop--)
		{
			Sid = pRouteData[loop - 1];
			if((Sid > 1) && (Sid <= rfpara.rf_slotnum))// && (Sid < MAX_REAL_SUP_SS_NUM))
			{
				pCltor = &(cltor[Sid]);	
                pCltor_shadow = &(cltor_shadow[Sid]);
                
				if(pCltor->devAddr[0] < 0xFF)
				{
					if(pCltor->father == father)
					{
						pCltor_shadow->father_update = u32CurSlot + hop;
					}
					else
					{				
						if(0 == pCltor->mother)
							pCltor->mother = father;
						else	
							pCltor->mother = pCltor->father;
                        
						pCltor->father =  father;
						pCltor_shadow->father_update = u32CurSlot + hop;
//							pCltor_shadow->mother_update = u32CurSlot + hop; 
					}
					father = Sid;
					pCltor->hop = hop +1;								
					hop ++;									
				}
				else
					break;				
			}
			else
				break;
		}
	}
			
}




