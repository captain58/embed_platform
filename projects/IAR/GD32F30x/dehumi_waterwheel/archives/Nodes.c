/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright HEX NET
 *
 *    File name   : nodes.c
 *    Description : real meter map; white meter; black meter.
 *
 *    History :
 *    1. Date        : oct 12, 2016
 *       Author      : yzy
 *       Description : Create
 *
 **************************************************************************/
#define NODES_VAR
#include "sys.h"
#include "App_Public.h"
#include "user_func.h"
#include "public.h"
#include "Nodes.h"
#include "frmcfg.h"
uint8 SyncCltorPara(uint16 id, COLLECTOR * pCltor, STMETERPARAFLASH *pstMeter)
{
    STMETERPARAFLASH stMeterParaF;
    I2cRead(0xA0,  (uint8 *)&stMeterParaF, FM_METER_INFO_BASE_ADDR + id*sizeof(STMETERPARAFLASH), sizeof(STMETERPARAFLASH));
    if(pstMeter == NULL)
    {
        if(LPC_Memory_CMP((uint8 *)&stMeterParaF,(uint8 *)pCltor,PARA_SYNC_LEN) == 0 &&
            stMeterParaF.checksum == Check_SUM((uint8 *)&stMeterParaF, sizeof(STMETERPARAFLASH) - 1))
        {
            return 1;
        }
        LOG_INFO( DBGFMT"pstMeter is NULL para sync id = %d\n",DBGARG, id);
        LOG_DUMPHEX(LOG_LEVEL_DEBUG, "stMeterParaF : \n", (uint8 *)&stMeterParaF, PARA_SYNC_LEN);
        LOG_DUMPHEX(LOG_LEVEL_DEBUG, "pCltor : \n", (uint8 *)pCltor, PARA_SYNC_LEN);
        
        memcpy((uint8 *)&stMeterParaF, (uint8 *)pCltor, PARA_SYNC_LEN);

        stMeterParaF.checksum = Check_SUM((uint8 *)&stMeterParaF, sizeof(STMETERPARAFLASH) - 1);

        I2cWrite(0xA0,  (uint8 *)&stMeterParaF, FM_METER_INFO_BASE_ADDR + id*sizeof(STMETERPARAFLASH), sizeof(STMETERPARAFLASH));

    }
    else
    {
        if(LPC_Memory_CMP((uint8 *)&stMeterParaF,(uint8 *)pCltor,PARA_SYNC_LEN) == 0 &&
            LPC_Memory_CMP((uint8 *)&stMeterParaF.softver,(uint8 *)&pstMeter->softver,PARA_ALL_LEN - (PARA_SYNC_LEN)) == 0 &&
            stMeterParaF.checksum == Check_SUM((uint8 *)&stMeterParaF, PARA_ALL_LEN))
        {
            return 1;
        }
        LOG_INFO( DBGFMT" para sync id = %d\n",DBGARG, id);
        LOG_DUMPHEX(LOG_LEVEL_DEBUG, "stMeterParaF : \n", (uint8 *)&stMeterParaF, PARA_SYNC_LEN);
        LOG_DUMPHEX(LOG_LEVEL_DEBUG, "pCltor : \n", (uint8 *)pCltor, PARA_SYNC_LEN);
        
        memcpy((uint8 *)&stMeterParaF, (uint8 *)pCltor, PARA_SYNC_LEN);
        
        memcpy((uint8 *)&stMeterParaF.softver, (uint8 *)&pstMeter->softver, PARA_ALL_LEN - (PARA_SYNC_LEN));


        stMeterParaF.checksum = Check_SUM((uint8 *)&stMeterParaF, PARA_ALL_LEN);
        
        I2cWrite(0xA0,  (uint8 *)&stMeterParaF, FM_METER_INFO_BASE_ADDR + id*sizeof(STMETERPARAFLASH), sizeof(STMETERPARAFLASH));
        
    }
    return 1;
}
uint8 SaveCltorPara(uint16 id, STMETERPARAFLASH * pstMeterParaF)
{


    pstMeterParaF->checksum = Check_SUM((uint8 *)pstMeterParaF, PARA_ALL_LEN);

    I2cWrite(0xA0,  (uint8 *)pstMeterParaF, FM_METER_INFO_BASE_ADDR + id*sizeof(STMETERPARAFLASH), sizeof(STMETERPARAFLASH));

    return 1;
}
uint8 GetCltorPara(uint16 id, STMETERPARAFLASH * pstMeterParaF)
{
    //STMETERPARAFLASH stMeterParaF;
    I2cRead(0xA0,  (uint8 *)pstMeterParaF, FM_METER_INFO_BASE_ADDR + id*sizeof(STMETERPARAFLASH), sizeof(STMETERPARAFLASH));
    if(pstMeterParaF->checksum == Check_SUM((uint8 *)pstMeterParaF, PARA_ALL_LEN))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
//-------------------------------------------------------------------------------------------
uint16 Re_Allocate_Id(uint8* sn_addr, uint8 len)//重新分配ID
{
	uint16 new_id, loop;
    uint16 index;	 
//	uint32 TTL;
	uint8 IDFound = 0;
	uint8 AddrCheck = 0;
	for(loop = 0; loop  < len - 1; loop ++)
	{
		if(sn_addr[loop] == 0xFF)
		{
			AddrCheck = 1;
			break;
		}	
	}
	if(0 == AddrCheck)
	{ 
		new_id  =  Cal_Hash_Value(sn_addr);    //取哈希出来的低8位
		if(new_id>rfpara.rf_slotnum)			//防止出现实际采集器个数大于时隙数
			new_id=new_id%(rfpara.rf_slotnum);
     		if((new_id < SUP_SS_INDEX_START) || (new_id >= MAX_SUP_SS_NUM)/* || (new_id >= MAX_REAL_SUP_SS_NUM)*/) 	//避免出现采集器ID号为1的情况
	 		    new_id = SUP_SS_INDEX_START;

		if ( cltor[new_id].devAddr[0] == 0xFF)//如果没有被占用
		{
			memcpy(cltor[new_id].devAddr, sn_addr, len);//保存SN	
			return new_id; 
		} 
		else//如果被占用，对比
		{
			if(/*(len == cltor[new_id].addrLen) && */(!memcmp(cltor[new_id].devAddr,sn_addr, len)))//如果已有的SN和上报的SN一样
				return new_id;//分配ID			
			else
			{
				loop = new_id+1;
				if((loop>rfpara.rf_slotnum) || (loop > MAX_SUP_SS_NUM)/* || (loop >= MAX_REAL_SUP_SS_NUM)*/)
                {
                    loop = 1;//翻转	 
                }
					
				IDFound = 0; 
			//	TTL = MAX_SS_DEAD_TIME;
				while(loop != new_id)
				{
					if(cltor[loop].devAddr[0] == 0xFF)  //如果没有被占用
					{
						IDFound = 1;
						index = loop;
						break;
					/*	if(cltor[loop].DeadTime < TTL)	
						{
							index = loop;
							TTL = cltor[loop].DeadTime;
							if(0 == TTL)
								break;
						}*/
					}
					loop = loop + 1; 	//继续往下找(注意翻转)		
				//	if(loop == rfpara.rf_slotnum) 
					if((loop>rfpara.rf_slotnum) || (loop > MAX_SUP_SS_NUM) /*|| (loop >= MAX_REAL_SUP_SS_NUM)*/)
						loop = 1;//翻转	 
				}

				if(IDFound)
				{
					memcpy( cltor[index].devAddr, sn_addr, len);//保存SN	
					return index;
				}
				else
					return 0;				
			}			
		}
	}
	else
		return 0;   	 	
}
//========================================================
uint16 Sn_Search_Id(uint8* sn_addr)//根据SN查询ID
{
	uint16 i ;
	for(i=SUP_SS_INDEX_START;i < MAX_SUP_SS_NUM+1; i++)//遍历
	{
		if((!memcmp(cltor[i].devAddr,sn_addr, CON_DEV_ADDR_LEN_6))/*&&(i==cltor[i].old_id)*/)//对比一样
			return i;			
	}								 
	return 0;//查询不到	 	
}
//========================================================
uint16 Sn_Search_Id2(uint8* sn_addr, uint8 len)//根据SN查询ID
{
	uint16 i ;
	for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM+1; i++)//遍历
	{
		if(/*len == cltor[i].addrLen && */(!memcmp(cltor[i].devAddr,sn_addr, len)))//对比一样
        {      
			return i;			
    	}
    }
	return 0;//查询不到	 	
}
//========================================================
uint8 getSnNum(void)//
{
	uint16 i,m = 0 ;
	for(i=SUP_SS_INDEX_START;i < MAX_SUP_SS_NUM+1; i++)//遍历
	{
		if ( cltor[i].devAddr[0] < 0xFF/* && cltor[i].nodestatus.NetStatus*/)
        {
			m++;			
        }
    }
	return m;
}
//========================================================
uint8 getSnLoginedNum(void)//
{
	uint16 i,m = 0 ;
	for(i=SUP_SS_INDEX_START;i < MAX_SUP_SS_NUM+1; i++)//遍历
	{
		if ( cltor[i].devAddr[0] < 0xFF && cltor[i].nodestatus.NetStatus)
        {
			m++;			
        }
    }
	return m;
}

#define CON_RF_RSSI_LOGIN_LIMIT     95
#define CON_RF_RSSI_Q_LOGIN_LIMIT   9//信号质量  相当于96  越小信号值越差
uint8 updataNodeCache(uint16 id, uint8 type, uint8 errCode, uint8 seq, uint8 protocol, 
                        uint8 rssi, uint8 * addr, uint8 len, STMETERPARAFLASH *pstMeter)
{
    uint8 rssiQ = pstMeter->reverse;
    uint8 routeNum = pstMeter->hop;
    if(len > CON_DEV_ADDR_LEN_8)
    {
        return 0;
    }
//    if(addr != NULL && errCode == 0)
    {
        cltor[id].addrLen = len;
        memset(cltor[id].devAddr, 0xFF, CON_DEV_ADDR_LEN_8);
        memcpy(cltor[id].devAddr, addr, len);  // 记录添加节点的唯一标识SN 
    }
    
//	    LOG_DEBUG( DBGFMT"updataNodeCache ID[%x] err[%x] cmd[%d] \n",DBGARG, id, errCode, type);                        
//	    LOG_DUMPHEX(LOG_LEVEL_DEBUG, "devAddr : \n", cltor[id].devAddr, len);



    switch(type)
    {
    case CON_NODE_UPDATE_LOGIN:
    case CON_NODE_UPDATE_REGISTER:
        if(protocol == PST_FRM_802_R_NO && routeNum == 0 && errCode == 0)
        {
            if(rssi > CON_RF_RSSI_LOGIN_LIMIT || rssiQ <= CON_RF_RSSI_Q_LOGIN_LIMIT)//信号不好
            {
                cltor[id].loginFailedTimes++;
                if(cltor[id].loginFailedTimes < 10)//大于10次的话认为是孤点，允许入网
                {
                    return 0;
                }
            }
        }
        cltor[id].loginFailedTimes = 0;
        cltor_shadow[id].nodestatus.ans_pkt_type=type;     //入网包标识位
        cltor_shadow[id].nodestatus.bNetAck= 0x1;         //表示该点需要回复
        cltor[id].nodestatus.NetStatus = NODE_STATUS_OUT;
        cltor_shadow[id].nodestatus.result = errCode;
        cltor[id].realPos = pstMeter->realPos;
        cltor[id].panid = pstMeter->panid;
        cltor_shadow[id].nodeRssi = rssiQ;  
        extern uint8_t guc_SwitchSeq;
        if(cltor[id].loginNo == 0)
        {
            cltor[id].loginNo = (++guc_SwitchSeq)%3;
            if(cltor[id].loginNo == 0) cltor[id].loginNo = 1;
        }
        break;
    case CON_NODE_UPDATE_HEARTBEAT:
        cltor_shadow[id].nodestatus.bNetAck= 0x1;         //表示该点需要回复
    case CON_NODE_UPDATE_HEARTBEAT1:
        cltor[id].loginFailedTimes = 0;
        cltor_shadow[id].nodestatus.ans_pkt_type=type;     //心跳 
        cltor_shadow[id].nodestatus.result = errCode;
        cltor[id].realPos = pstMeter->realPos;
        cltor[id].panid = pstMeter->panid;
        cltor_shadow[id].nodeRssi = rssiQ;        
        break;
    case CON_NODE_UPDATE_PUSHUP:
        cltor_shadow[id].nodestatus.bDataAck = 1;      //推送数据
        cltor_shadow[id].nodestatus.errCode = errCode;
        cltor_shadow[id].nodeRssi = rssiQ;
        break;
    default:
        break;
    }


    cltor_shadow[id].recvseq = seq;
    cltor_shadow[id].FrameDeadTime = 0xff;//joinreq->ttl;     
    cltor_shadow[id].rRssi = rssi;

    cltor[id].nodestatus.protocol = protocol;
    
    if(id > 1 && errCode == 0)
    {
        SyncCltorPara(id, &cltor[id], pstMeter);
    }
    return 1;
}
void Delete_SS_Node(uint16 id)
{
	cltor[id].hop = 0;
	cltor[id].father = 0;
	cltor[id].mother = 0;
	cltor[id].nodestatus.NetStatus = NODE_STATUS_OUT;
//		cltor[id].nodestatus.handled=UN_HANDLED;//重新标记为未处理
//		cltor[id].sn.dw[0] = 0xffff;//清空SN
//		cltor[id].sn.dw[1] = 0xffff;
//		cltor[id].sn.dw[2] = 0xffff;    
    memset(cltor[id].devAddr, 0xFF, CON_DEV_ADDR_LEN_10);

	//cltor[id].DeadTime = MAX_SS_DEAD_TIME;		
	cltor[id].nodestatus.bNeedDelete = FALSE;
    SyncCltorPara(id, &cltor[id], NULL);
    
}

uint16 getOrAllocateIdByAddr(uint8 cmd, uint8 * addr, uint8 len)
{
    uint16 i = 0, nFoundId = 0, id = 0;
    uint8 nFoundNum = 0;

    if(len > CON_DEV_ADDR_LEN_8)
    {
        return id;
    }

    switch(cmd)
    {
    case 1:
    case 3:   
    	for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM + 1; i++)	//遍历
    	{
    		if(!LPC_Memory_CMP(cltor[i].devAddr, addr, len))//对比一样
    		{
    			nFoundNum++;		
    			nFoundId = i;
    		}
    	}	
        
    	if(0 == nFoundNum)
        {            
            id = Re_Allocate_Id(addr, len);	//根据SN重新分配
        }
    	else if(1 == nFoundNum)
        {            
            id = nFoundId;
        }
    	else
    	{
    		for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM + 1; i++)	//遍历
    		{
    			if(!LPC_Memory_CMP(cltor[i].devAddr,addr, len))//对比一样
                {
    				Delete_SS_Node(i);			
                }
    		}							
    		id = Re_Allocate_Id(addr, len);	//根据SN重新分配
    	}
        break;
    case 2:
        memcpy((uint8 *)&id, addr, 2);
        if(id > MAX_SUP_SS_NUM)
        {
            id = 0;
        }
        break;
    default:
        break;
    }
    return id;
}

uint16 findIdByAddr(uint8 cmd, uint8 * addr, uint8 len)
{
    uint8 tmp[CON_DEV_ADDR_LEN_8];
    uint16 id = 0;
//	    int ret = -1;    
    memset(tmp, 0, CON_DEV_ADDR_LEN_8);


    switch(cmd)
    {
        case 1:
        case 3:   
        {
            id = Sn_Search_Id2(addr, len);     //这里需要考虑地址压缩      
            if ((id < 2)  || (cltor[id].hop < 2))//|| cltor[id].nodestatus.NetStatus == NODE_STATUS_OUT)    // ID 非法
            {
                if(CompressAddr(addr, len, tmp))
                {
                    id = Sn_Search_Id2(tmp, CON_DEV_ADDR_LEN_8);     //这里需要考虑地址压缩
                    if ((id < 2)  || (cltor[id].hop < 2))
                    {
                        id = 0;
                    }
                }
                else
                {
                    id = 0;
                }
            }
            break;
        }
        case 2:
        {
            memcpy((uint8 *)&id, addr, 2);

            if ((id < 2)  || (cltor[id].hop < 2) )//|| cltor[id].nodestatus.NetStatus == NODE_STATUS_OUT)    // ID 非法
            {
                id = 0;
            }
            break;

        }
        default:
        {
            id = 0;
            break;

        }
    }
    return id;
}

uint8 getNetworkStatus(void)
{
    uint16 nMeterNum, nSSNum, i;
    int ret = 0;
    //m = 0;

	nMeterNum = Meter_Num();

	nSSNum = 0;
	for (i = 2; i <= rfpara.rf_slotnum; i++)
	{  
		if ((cltor[i].devAddr[0] < 0xFF) && (cltor[i].nodestatus.NetStatus == NODE_STATUS_LOGIN))
        {      
			nSSNum ++;
        }
	}
	
//////////////////////////////////路由稳定期，表越多，等待时间越长；一半表计入网以后路由稳定
    int x;

    if(nMeterNum > 512)
    {
        x = 16;
    }
    else if(nMeterNum > 256)
    {
        x = 8;
    }
    else if(nMeterNum > 128)
    {
        x = 4;
    }
    else
    {
        x = 2;
    }
    if((x*nSSNum) >= nMeterNum)
	{
        ret = 3;
    }
    else
    {
        if(curslottime > 2 * ((SYS_TIMER_SLOT_INTERVAL_MS * MAX_SLOT_NUM) / SYS_TIMER_COUNT_MS))// 2 * 40s * x值表)
        {
            ret = 3;
        }
        else if(curslottime >  ((SYS_TIMER_SLOT_INTERVAL_MS * MAX_SLOT_NUM) / SYS_TIMER_COUNT_MS))// 2 * 40s * x值表)
        {
            ret = 2;
        }
        else
        {
            ret = 1;
        }
    }
    return ret;
}

