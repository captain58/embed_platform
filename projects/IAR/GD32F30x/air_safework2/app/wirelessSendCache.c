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
#define WIRELESS_CACHE
#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"
#include "user_func.h"
#include "wirelessSendCache.h"
#include "A7139reg.h"
EZ_TX_PKT TxPktH[CON_SEND_PRIORITY_HIGH_NUM];//ҵ�������
EZ_TX_PKT TxPktN[CON_SEND_PRIORITY_NORMAL_NUM];//@"AHB_RAM_MEMORY";//ҵ�������
EZ_TX_PKT TxPktL[CON_SEND_PRIORITY_LOW_NUM];//@"AHB_RAM_MEMORY";//ҵ�������


void sendCacheInit(void)
{
    int i;
    for(i=0; i < CON_SEND_PRIORITY_HIGH_NUM; i++)
    {
        memset((uint8 *)&TxPktH[i], 0, sizeof(EZ_TX_PKT));
    }
    for(i=0; i < CON_SEND_PRIORITY_NORMAL_NUM; i++)
    {
        memset((uint8 *)&TxPktN[i], 0, sizeof(EZ_TX_PKT));
    }
    for(i=0; i < CON_SEND_PRIORITY_LOW_NUM; i++)
    {
        memset((uint8 *)&TxPktL[i], 0, sizeof(EZ_TX_PKT));
    }
    g_stSendCacheIndex.ezPkt = NULL;
    
}

EZ_TX_PKT * getIdelSendCacheByPri(uint8 level)//���ݵȼ�ѡȡ
{
    int i = 0;
    switch(level)
    {
    case CON_SEND_PRIORITY_HIGH:
        for(i=0; i < CON_SEND_PRIORITY_HIGH_NUM; i++)
        {
            if(TxPktH[i].bValid == 0)
            {
                return &TxPktH[i];
            }
        }
        break;
    case CON_SEND_PRIORITY_NORMAL:
        for(i=0; i < CON_SEND_PRIORITY_NORMAL_NUM; i++)
        {
            if(TxPktN[i].bValid == 0)
            {
                return &TxPktN[i];
            }
        }
        break;
    case CON_SEND_PRIORITY_LOW:
        for(i=0; i < CON_SEND_PRIORITY_LOW_NUM; i++)
        {
            if(TxPktL[i].bValid == 0)
            {
                return &TxPktL[i];
            }
        }
        break;
    }

    return NULL;

}

EZ_TX_PKT * getIdelSendCache(uint8 level)//�������ѱȲ������ȼ��͵Ķ�����仺��
{
    int i = 0;
    switch(level)
    {
    case CON_SEND_PRIORITY_HIGH:
        for(i=0; i < CON_SEND_PRIORITY_HIGH_NUM; i++)
        {
            if(TxPktH[i].bValid == 0)
            {
                return &TxPktH[i];
            }
        }
    case CON_SEND_PRIORITY_NORMAL:
        for(i=0; i < CON_SEND_PRIORITY_NORMAL_NUM; i++)
        {
            if(TxPktN[i].bValid == 0)
            {
                return &TxPktN[i];
            }
        }
    case CON_SEND_PRIORITY_LOW:
        for(i=0; i < CON_SEND_PRIORITY_LOW_NUM; i++)
        {
            if(TxPktL[i].bValid == 0)
            {
                return &TxPktL[i];
            }
        }
        break;
    }

    return NULL;
}

//��������
EZ_TX_PKT * getIdelSendCacheCanbeSleep(uint8 level, int32 ms)//�������ѱȲ������ȼ��͵Ķ�����仺��
{
    int i = 0;
    
    uint32 timeout = 0;//GET_TICK_COUNT( );
    if(ms == 0)
    {
        ms = 5000;//Ĭ��5��
    }
    while( timeout <= ms )
    {
        switch(level)
        {
        case CON_SEND_PRIORITY_HIGH:
            for(i=0; i < CON_SEND_PRIORITY_HIGH_NUM; i++)
            {
                if(TxPktH[i].bValid == 0)
                {
                    return &TxPktH[i];
                }
            }
        case CON_SEND_PRIORITY_NORMAL:
            for(i=0; i < CON_SEND_PRIORITY_NORMAL_NUM; i++)
            {
                if(TxPktN[i].bValid == 0)
                {
                    return &TxPktN[i];
                }
            }
        case CON_SEND_PRIORITY_LOW:
            for(i=0; i < CON_SEND_PRIORITY_LOW_NUM; i++)
            {
                if(TxPktL[i].bValid == 0)
                {
                    return &TxPktL[i];
                }
            }
            break;
        }

        msleep(50);
        timeout+=50;
    }
    return NULL;
}

uint8 checkSendCache(void)
{
    int i = 0;
    //������ȼ�
    for(i = 0; i < CON_SEND_PRIORITY_HIGH_NUM; i++)
    {
        if(TxPktH[i].bValid == 1 && (0 == TxPktH[i].nBackOffSlot) && (TxPktH[i].TxPkt.ttl > 0))
        {
           
            return 1;//ֱ��return��ȥ������Ҫif�ж��ˣ����ٱ������
        }
    }
    //�θ����ȼ�
    for(i = 0; i < CON_SEND_PRIORITY_NORMAL_NUM; i++)
    {
        if(TxPktN[i].bValid == 1 && (0 == TxPktN[i].nBackOffSlot) && (TxPktN[i].TxPkt.ttl > 0))
        {
            
            return 1;
        }
    }
    //�����ȼ�
    for(i = 0; i < CON_SEND_PRIORITY_LOW_NUM; i++)
    {
        if(TxPktL[i].bValid == 1 && (0 == TxPktL[i].nBackOffSlot) && (TxPktL[i].TxPkt.ttl > 0))
        {
            
            return 1;
        }
    }
    
    return 0;
}

uint8 checkDuplicateWithPri(uint8 level, uint16 id)
{
    int i = 0;
    
    switch(level)
    {
    case CON_SEND_PRIORITY_HIGH:
        for(i = 0; i < CON_SEND_PRIORITY_HIGH_NUM; i++)
        {
            if(TxPktH[i].bValid == 1 && TxPktH[i].TxPkt.index == id)
            {
                return 1;
            }
        }
    case CON_SEND_PRIORITY_NORMAL:
        for(i=0; i < CON_SEND_PRIORITY_NORMAL_NUM; i++)
        {
            if(TxPktN[i].bValid == 1 && TxPktN[i].TxPkt.index == id)
            {
                return 1;
            }
        }
    case CON_SEND_PRIORITY_LOW:
        for(i=0; i < CON_SEND_PRIORITY_LOW_NUM; i++)
        {
            if(TxPktL[i].bValid == 1 && TxPktN[i].TxPkt.index == id)
            {
                return 1;
            }
        }
        break;
    }
    return 0;
}

EZ_TX_PKT * getReadySendCache(uint8 * level)
{
    int i = 0;
    //������ȼ�
    for(i = 0; i < CON_SEND_PRIORITY_HIGH_NUM; i++)
    {
        if(TxPktH[i].bValid == 1 && (0 == TxPktH[i].nBackOffSlot) && (TxPktH[i].TxPkt.ttl > 0))
        {
            *level = CON_SEND_PRIORITY_HIGH;
            return &TxPktH[i];//ֱ��return��ȥ������Ҫif�ж��ˣ����ٱ������
        }
    }
    //�θ����ȼ�
    for(i = 0; i < CON_SEND_PRIORITY_NORMAL_NUM; i++)
    {
        if(TxPktN[i].bValid == 1 && (0 == TxPktN[i].nBackOffSlot) && (TxPktN[i].TxPkt.ttl > 0))
        {
            *level = CON_SEND_PRIORITY_NORMAL;
            return &TxPktN[i];
        }
    }
    //�����ȼ�
    for(i = 0; i < CON_SEND_PRIORITY_LOW_NUM; i++)
    {
        if(TxPktL[i].bValid == 1 && (0 == TxPktL[i].nBackOffSlot) && (TxPktL[i].TxPkt.ttl > 0))
        {
            *level = CON_SEND_PRIORITY_LOW;
            return &TxPktL[i];
        }
    }
    
    return NULL;
}
//��ҵ��������ά���Ļ�����TTL���ϻ�
void decTTL(uint8 msr)
{
    int i = 0;
    int ttl = 0;
    uint16 index = 0;
    //������ȼ�
    for(i = 0; i < CON_SEND_PRIORITY_HIGH_NUM; i++)
    {
        ttl = 0;
        index = 0;
        if(TxPktH[i].bValid)
        {
			ttl = TxPktH[i].TxPkt.ttl;//data[0];
			if(ttl > 0)
			{
				ttl--;
				TxPktH[i].TxPkt.ttl = ttl;//data[0]						
			}
			else
			{
				if((msr & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
				{
					TxPktH[i].bValid = 0;
					TxPktH[i].nBackOffSlot = 0;

                    index = TxPktH[i].TxPkt.index;
                    cltor_shadow[index].nodestatus.bNetAck= 0;	//�����ѻ�ACK							
                    cltor_shadow[index].nodestatus.bDataAck = 0;
				}
			}
        }

        if(TxPktH[i].nBackOffSlot > 0)
        {
            TxPktH[i].nBackOffSlot --;
        }

    }
    //�θ����ȼ�
    for(i = 0; i < CON_SEND_PRIORITY_NORMAL_NUM; i++)
    {
//	        if(TxPktN[i].bValid == 1)
//	        {
//	            //*level = CON_SEND_PRIORITY_NORMAL;
//	            //return &TxPktN[i];
//	        }
        ttl = 0;
        index = 0;
        if(TxPktN[i].bValid)
        {
			ttl = TxPktN[i].TxPkt.ttl;//data[0];
			if(ttl > 0)
			{
				ttl--;
				TxPktN[i].TxPkt.ttl = ttl;//data[0]						
			}
			else
			{
//				if((msr & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
				{
					TxPktN[i].bValid = 0;
					TxPktN[i].nBackOffSlot = 0;

                    index = TxPktN[i].TxPkt.index;
                    cltor_shadow[index].nodestatus.bNetAck= 0;	//�����ѻ�ACK							
                    cltor_shadow[index].nodestatus.bDataAck = 0;
				}
			}
        }
        if(TxPktN[i].nBackOffSlot > 0)
        {
            TxPktN[i].nBackOffSlot --;
        }
        
    }
    //�����ȼ�
    for(i = 0; i < CON_SEND_PRIORITY_LOW_NUM; i++)
    {
        ttl = 0;
        index = 0;
        if(TxPktL[i].bValid)
        {
			ttl = TxPktL[i].TxPkt.ttl;//data[0];
			if(ttl > 0)
			{
				ttl--;
				TxPktL[i].TxPkt.ttl = ttl;//data[0]						
			}
			else
			{
				if((msr & EZMAC_STATE_BIT) == EZMAC_PRO_IDLE)
				{
					TxPktL[i].bValid = 0;
					TxPktL[i].nBackOffSlot = 0;

                    index = TxPktL[i].TxPkt.index;
                    cltor_shadow[index].nodestatus.bNetAck= 0;	//�����ѻ�ACK							
                    cltor_shadow[index].nodestatus.bDataAck = 0;
				}
			}
        }    
        if(TxPktL[i].nBackOffSlot > 0)
        {
            TxPktL[i].nBackOffSlot --;
        }

    }
}

