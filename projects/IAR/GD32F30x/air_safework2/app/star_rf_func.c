/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: star_rf_func.c
**创   建   人: yzy
**最后修改日期: 2016年5月20日
**描        述: 
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2016年5月20日
** 描　述: 原始版本
**-------------------------------------------------------------------------------------------------------
** 修改人: 
** 版  本: 
** 描  述: 
********************************************************************************************************/




/************************************************************************
 * @function: fGRFFTD00
 * @描述: 确认/否认
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2016/06/26)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fSRFFTD00(const CMD_TABLE_t* tbl, SRF_Frame* frm)
{        
    uint16 id;
    uint8 SN[CON_DEV_ADDR_LEN_6];
    memset(SN, 0, CON_DEV_ADDR_LEN_6);
    if(frm->apdu.stInfo.stUp.bit1.addrFlg == 1)
    {
        memcpy(SN, cltor[frm->id].devAddr, CON_DEV_ADDR_LEN_6);
    }
    else
    {
        memcpy(SN, frm->apdu.addr, CON_DEV_ADDR_LEN_6);
    }
    switch(frm->apdu.fn)
    {
        case MSG_TYPE_FN_01:
            
            if(frm->bNeedReAllocate)
            {
                id = Sn_Search_Id(SN);
                if ((id >= SUP_SS_INDEX_START) && ( id <= MAX_SUP_SS_NUM) )
                {
                    if(cltor[id].nodestatus.bNeedDelete)
                    {
//                        int ret;
//    					ret = Hash_Table_Search(cltor[id].devAddr); //查找哈希表中的索引值				
//                        if (ret != ( - 1))
//                        {                     
//                            Updata_Hash_Table(ret, NULL);  //删除                                              
//
//                            //删除认证采集器操作
//                            //Del_Validate_W_B(cltor[id].devAddr);
//                        }
//    			        Delete_SS_Node(id);
                    }
                }
            }
            break;
        default:
        break;
    }
    
    return SYS_ERR_NOREPLY;  //无需回复
}
/************************************************************************
 * @function: fGRFFTD01
 * @描述: APP层控制帧结构
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2016/06/26)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fSRFFTD01(const CMD_TABLE_t* tbl, SRF_Frame* frm)
{        
    uint16 len, ml = 0;
    uint32 i = 0;
    BOOL berr = FALSE;
    //uint8 m = PKT_HEAD_LEN + frm->apdu.addrlen;
    STINFO3762 stinfo;
    //uint8 fn;
    uint8 *send = frm->send;//源地址后面的可以覆盖，源地址还要用
    memset((uint8 *)&stinfo, 0, sizeof(STINFO3762));
    //uint16 id = 0;
    uint8 SN[CON_DEV_ADDR_LEN_6];
    //uint8 tmp[30];
    //uint8 addrLenPer = 2;
//    extern     uint8 g_ucfactory_mod;
    memset(SN, 0, CON_DEV_ADDR_LEN_6);
    //memset(tmp, 0, 30);
    if(frm->apdu.stInfo.stUp.bit1.addrFlg == 1)
    {
        //addrLenPer = 2;
        memcpy(SN, cltor[frm->id].devAddr, CON_DEV_ADDR_LEN_6);
    }
    else
    {
        //addrLenPer = CON_DEV_ADDR_LEN_6;
        memcpy(SN, frm->apdu.addr, CON_DEV_ADDR_LEN_6);
    }
    
    switch(frm->apdu.fn)
    {    
        case MSG_TYPE_FN_15:
          
            break;
        case MSG_TYPE_FN_01:
            if(frm->bDebugFlg == TRUE)
            {
                send[ml++] = 0xFF;
                send[ml++] = 0xFF;
                
                send[ml++] = 0x00;
                send[ml++] = 0x00;
                
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                frm->apdu.ctrl.ftd = 0;
                
                frm->apdu.fn = 1;
                berr = TRUE;

                g_bResetCmd = 1;
                nChangeChannelTime = curslottime + 360;
            }
            break;
        case MSG_TYPE_FN_02:
            if(frm->bDebugFlg == TRUE)
            {
                send[ml++] = 0xFF;
                send[ml++] = 0xFF;
                
                send[ml++] = 0x00;
                send[ml++] = 0x00;
                
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                frm->apdu.ctrl.ftd = 0;
                
                frm->apdu.fn = 1;
                berr = TRUE;

                uint8 dis[32] ;
    			memset(dis,0xff,32);
    			for(i=0;i<32;i++)
    			{	                                   
    				I2cWrite(0xA0, dis, 0+(i*32), 32); //复位影子空间***
    				memset(shadow_space+(i*32),0xff,32);						 
    			}
                Init_Hash_Table();                
//	    			IapAction = IAP_ACTION_ERASE_HASH1; //IAP标识位
//	    			while (IapAction != IAP_ACTION_IDLE);
//	                
//	    			IapAction = IAP_ACTION_ERASE_HASH2; //IAP标识位
//	    			while (IapAction != IAP_ACTION_IDLE);	// Hold on	
    			
//	    			IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH1; //IAP标识位
//	    			while (IapAction != IAP_ACTION_IDLE);
//	                
//	    			IapAction = IAP_ACTION_ERASE_WRITE_SS_HASH2; //IAP标识位
//	    			while (IapAction != IAP_ACTION_IDLE);
//	                
//	    			IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH1; //IAP标识位
//	    			while (IapAction != IAP_ACTION_IDLE);
//	                
//	    			IapAction = IAP_ACTION_ERASE_BLACK_SS_HASH2; //IAP标识位
//	    			while (IapAction != IAP_ACTION_IDLE);	

                nChangeChannelTime = curslottime + MAX_SLOT_TIME * 10;
                bNeedChangeChannel = TRUE;

                for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM+1; i++)  //遍历
                {
                    if(cltor[i].devAddr[0] < 0xFF)//
                    {
                        Delete_SS_Node(i);          
                    }
                }
            }
            break;

        case MSG_TYPE_FN_03:
            if(frm->bDebugFlg == TRUE)
            {
            
                send[ml++] = 0xFF;
                send[ml++] = 0xFF;
                
                send[ml++] = 0x00;
                send[ml++] = 0x00;
            
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                frm->apdu.ctrl.ftd = 0;
                
                frm->apdu.fn = 1;
                berr = TRUE;
            }
            break;

    case MSG_TYPE_FN_11://设置RSSI门限值
            if(frm->bDebugFlg == TRUE)
            {
                
                if(rfpara.rf_limit != frm->apdu.data[0])
                {
                    rfpara.rf_limit = frm->apdu.data[0];

                    I2cWrite(0xA0, (uint8*)&rfpara.rf_limit, FM_NETPARA_ADDR+3, 1);
                }
                send[ml++] = 0xFF;
                send[ml++] = 0xFF;
                
                send[ml++] = 0x00;
                send[ml++] = 0x00;

                berr = TRUE;

                
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                frm->apdu.ctrl.ftd = 0;
                
                frm->apdu.fn = 1;
            }  
            break;
        case MSG_TYPE_FN_43://读取RSSI门限值
            //if(bDebugFlg == TRUE)
            {
                send[ml++] = rfpara.rf_limit;

                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                berr = TRUE;
            }
            break;
            
        case MSG_TYPE_FN_13:
            if(frm->bDebugFlg == TRUE)
            {
                ml = 0;

                if(LPC_Memory_CMP(frm->apdu.data, nDeviceMacAddr, 6))
                {
                    uint8 addrLen = 6;
                    I2cWrite(0xA0, &addrLen, FM_DEVICE_ADDR_LEN, 1);

                    I2cWrite(0xA0, frm->apdu.data, FM_DEVICE_ADDR , 6);
                    I2cRead(0xA0, nDeviceMacAddr, FM_DEVICE_ADDR, 6); 

                    nChangeChannelTime = curslottime + MAX_SLOT_TIME * 10;
                    bNeedChangeChannel = TRUE;
                }

                send[ml++] = 0xFF;
                send[ml++] = 0xFF;
                
                send[ml++] = 0x00;
                send[ml++] = 0x00;
                
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                frm->apdu.ctrl.ftd = 0;
                frm->apdu.fn = 1;
                berr = TRUE;
            }                      
            break;        
        case MSG_TYPE_FN_44:
            //if(bDebugFlg == TRUE)
            {
                ml = 0;
                
                memcpy(send+ml, nDeviceMacAddr, 6);
                ml+=6;
                
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                berr = TRUE;
            }
            break;
        case MSG_TYPE_FN_17://设置无线通信参数 
            if(frm->bDebugFlg == TRUE)
            {
                ml = 0;
                uint8 moedemType = frm->apdu.data[0] >> 5;
                uint8 channel = frm->apdu.data[0] & 0x1F;
                
                if(moedemType != rfpara.modemType)
                {
                    I2cWrite(0xA0, (uint8*)&moedemType, FM_NETPARA_ADDR + 10, 1);                        
                }
                if(channel != rfpara.rf_channel)
                {
                    nChangeChannelTime = curslottime + MAX_SLOT_TIME * 10;
                    bNeedChangeChannel = TRUE;

                    for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM+1; i++)  //遍历
                    {
                        if(cltor[i].devAddr[0] < 0xFF)//
                        {
                            Delete_SS_Node(i);          
                        }
                    }
                    I2cWrite(0xA0, (uint8*)&channel, FM_NETPARA_ADDR, 1);                        
                }
                //rfpara.rf_channel = channelId;
                send[ml++] = 0xFF;
                send[ml++] = 0xFF;
                
                send[ml++] = 0x00;
                send[ml++] = 0x00;
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                frm->apdu.ctrl.ftd = 0;
                frm->apdu.fn = 1;
                berr = TRUE;
                
            }                      
            break;                 
        case MSG_TYPE_FN_18://GPS信息 
            if(frm->bDebugFlg == TRUE)
            {
                ml = 0;
                
                I2cWrite(0xA0, (uint8*)frm->apdu.data, FM_GPS_PARA, 11);   
                
                send[ml++] = 0xFF;
                send[ml++] = 0xFF;
                
                send[ml++] = 0x00;
                send[ml++] = 0x00;
                
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                frm->apdu.ctrl.ftd = 0;
                frm->apdu.fn = 1;
                berr = TRUE;
                
            }                      
            break;                                     
        case MSG_TYPE_FN_20:
            //if(bDebugFlg == TRUE)
            {
                ml = 0;
                
                send[ml++] = 0;//pandId
                send[ml++] = 0;
                memcpy(send+ml, nDeviceMacAddr, 6);//中心模块地址
                ml+=6;
                send[ml++] = 0;//信标标识
                uint16 nMeterNum = (MAX_SUP_SS_NUM-1);
                uint16 nMeterNum2;
                memcpy(send + ml, &nMeterNum, 2);
                ml+=2;
                send[ml++] = rfpara.rf_limit;//场强门限
                send[ml++] = rfpara.rf_channel;//工作信道组号
                memcpy(send + ml, (uint8 *)&curslottime, 2);//时隙号
                ml+=2;
                send[ml++] = 0;//层号
                send[ml++] = rfpara.rf_power;//发射功率等级
                send[ml++] = 1;//网络状态
                //extern volatile uint32 MSR;                        
                send[ml++] = getMSR();//发送状态
                
                nMeterNum = Meter_Num();//从模块总数量
                memcpy(send + ml, &nMeterNum, 2);
                ml+=2;
                
                nMeterNum2 = getSnLoginedNum();//已入网模块数量
                memcpy(send + ml, &nMeterNum2, 2);
                ml+=2;
                nMeterNum -= nMeterNum2;//未入网模块数量
                memcpy(send + ml, &nMeterNum, 2);
                ml+=2;
                send[ml++] = 0;
                
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                berr = TRUE;
            }
            break;
        case MSG_TYPE_FN_21://设置 SF BW preamble 发射功率 
            if(frm->bDebugFlg == TRUE)
            {
                ml = 0;
                
                rfpara.rf_power = frm->apdu.data[3];

                I2cWrite(0xA0, (uint8*)&rfpara.rf_power, FM_NETPARA_ADDR+1, 1);   

                send[ml++] = 0xFF;
                send[ml++] = 0xFF;
                
                send[ml++] = 0x00;
                send[ml++] = 0x00;
                
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                frm->apdu.ctrl.ftd = 0;
                frm->apdu.fn = 1;
                
//                if(0 == setRfPwr(rfpara.rf_power))
//                {
//                    SYS_ERR();
//                }
                
                berr = TRUE;

//	                g_bResetCmd = 1;
//	                nChangeChannelTime = curslottime + 100;
            }                    
            break;
        case MSG_TYPE_FN_22:
            //if(bDebugFlg == TRUE)
            {
                ml = 0;
//	                extern const STSX1276PARA ChannelSetting[];
//                send[ml++] = getSF();//LoRaSettings.SpreadingFactor;//pandId
//                send[ml++] = getBW();//LoRaSettings.SignalBw;
//                send[ml++] = getPreamble();//(uint8)ChannelSetting[rfpara.rf_channel].preamble;
                send[ml++] = rfpara.rf_power;
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                berr = TRUE;
            }
            break;
        case MSG_TYPE_FN_23:
            if(frm->bDebugFlg == TRUE)
            {
                ml = 0;
                uint8 channel = frm->apdu.data[0];

                if(channel != rfpara.rf_channel)
                {
                    nChangeChannelTime = curslottime + MAX_SLOT_TIME * 10;
                    bNeedChangeChannel = TRUE;

                    for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM+1; i++)  //遍历
                    {
                        if(cltor[i].devAddr[0] < 0xFF)//
                        {
                            Delete_SS_Node(i);          
                        }
                    }
                    I2cWrite(0xA0, (uint8*)&channel, FM_NETPARA_ADDR, 1);                        
                }
                //rfpara.rf_channel = channelId;
                send[ml++] = 0xFF;
                send[ml++] = 0xFF;
                
                send[ml++] = 0x00;
                send[ml++] = 0x00;
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                frm->apdu.ctrl.ftd = 0;
                frm->apdu.fn = 1;
                berr = TRUE;
                
            }                    
            break;
        case MSG_TYPE_FN_24:
            //if(bDebugFlg == TRUE)
            {
                
                send[ml++] = rfpara.rf_channel;
                
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                berr = TRUE;
            }
            break;
            
        case MSG_TYPE_FN_25:
            //if(bDebugFlg == TRUE)
            {
                memcpy(send, (uint8 *)ucModeVer, 20);
                
                send[8] = (sVersiong.swCode[0] / 0x10) + '0';
                send[9] = (sVersiong.swCode[0] % 0x10) + '0';
                
                send[11] = (sVersiong.yy / 0x10) + '0';
                send[12] = (sVersiong.yy % 0x10) + '0';
                send[13] = (sVersiong.mm / 0x10) + '0';
                send[14] = (sVersiong.mm % 0x10) + '0';
                send[15] = (sVersiong.dd / 0x10) + '0';
                send[16] = (sVersiong.dd % 0x10) + '0';

                ml = 20;
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                berr = TRUE;
            }
            break;
        case MSG_TYPE_FN_26:
            if(frm->bDebugFlg == TRUE)
            {
                nChangeChannelTime = curslottime + MAX_SLOT_TIME * 10;
                bNeedChangeChannel = TRUE;

                for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM+1; i++)  //遍历
                {
                    if(cltor[i].devAddr[0] < 0xFF)//
                    {
                        Delete_SS_Node(i);          
                    }
                }

                send[ml++] = 0xFF;
                send[ml++] = 0xFF;
                
                send[ml++] = 0x00;
                send[ml++] = 0x00;

                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                frm->apdu.ctrl.ftd = 0;
                frm->apdu.fn = 1;
                berr = TRUE;
                
            }                    
            break;  
        case MSG_TYPE_FN_27:
        {
            if(frm->bDebugFlg == TRUE)
            {
                uint16 nStartNum, find_id;
                uint8 type, nNeedNum;
                uint8 nMeterNum, nCurSendNum;
                uint8 * pSendData;
                uint8 * pSn;
                ml = 0;

                type = frm->apdu.data[0];
                memcpy((uint8 *)&nStartNum, frm->apdu.data + 1, 2);
                nNeedNum = frm->apdu.data[3];

                nCurSendNum = 0;
                HASHT* pHasht = NULL;

                if(HASH_Table_Used==0)//当前使用table1
                    pHasht = htable1;
                else
                    pHasht = htable2;
                
                nMeterNum = 0;
                ml = 4;
                if(type == 1)
                {
                    for(i=SUP_SS_INDEX_START;i < MAX_SUP_SS_NUM+1; i++)//遍历
                    {
                        if ( cltor[i].devAddr[0] < 0xFF && cltor[i].nodestatus.NetStatus)
                        {
                            nMeterNum ++;
                            if((nMeterNum >= nStartNum) && (nCurSendNum < nNeedNum) && (nCurSendNum < 29))
                            {
                                //组包
                                pSendData =  send + ml;// + nCurSendNum * 12;
                                pSn = cltor[i].devAddr;
                                memcpy(pSendData, pSn, 6);

                                nCurSendNum++;
                                ml+=6;
                            }
                        }
                    }
                }
                else
                {
                    for(i=0; i < MAX_HASH_TABLE_FLASH_SIZE; i++)
                    {
                        if(pHasht[i].status == OCCUPIED)
                        {
                            nMeterNum ++;
                            if((nMeterNum >= nStartNum) && (nCurSendNum < nNeedNum) && (nCurSendNum < 22))
                            {
                                if(type == 4 && pHasht[i].devType != 2)
                                {
                                    continue;
                                }
                                //组包
                                pSendData =  send + ml;
                                pSn = pHasht[i].addr;
                                
                                if(type == 3)//未入网档案
                                {
                                    find_id = Sn_Search_Id(pSn);    //根据SN查找对应ID
                                
                                    if(find_id < 2 || find_id > MAX_SUP_SS_NUM || cltor[find_id].nodestatus.NetStatus)
                                    {
                                        continue;
                                    }
                                }
                                    
                                memcpy(pSendData, pSn, 6);
                                
                                nCurSendNum++;  
                                ml+=6;
                            }
                        }
                    }
                }
                memcpy(send, frm->apdu.data, 3);
                send[3] = nCurSendNum;
                
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                berr = TRUE;
            }

            break;
        }
        case MSG_TYPE_FN_30:
            //if(bDebugFlg == TRUE)
            {
                
                I2cRead(0xA0, (uint8*)send, FM_GPS_PARA, 11);   
                
                ml+=11;

                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                berr = TRUE;
                
            }                      
            break;      
        case MSG_TYPE_FN_31:
            if(frm->bDebugFlg == TRUE)
            {
                ml = 0;
                
                I2cRead(0xA0, (uint8*)&rfpara.rf_channel, FM_NETPARA_ADDR, 1);   
                I2cRead(0xA0, (uint8*)&rfpara.modemType, FM_NETPARA_ADDR+11, 1);  
                send[0] = (rfpara.modemType << 5) | (rfpara.rf_channel & 0x1F);
                
                ml+=1;

                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;

                berr = TRUE;
            }                      
            break;
        case MSG_TYPE_FN_39://故障状态
            if(frm->bDebugFlg == TRUE)
            {
                send[0] = 0;
                
                ml+=1;
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;

                berr = TRUE;
            }                      
            break;                    
        case MSG_TYPE_FN_40://读内存数据
        {
            if(frm->bDebugFlg == TRUE)
            {
                uint32 addr;
                
                memcpy((uint8 *)&addr, frm->apdu.data, 4);
                len = frm->apdu.data[4];
                if(len > 192)
                {
                    len = 192;
                }
                if((addr >= RAM_START && addr <= RAM_END) || (addr >= RAM2_START && addr <= RAM2_END) )
                {
                    send[ml++] = len;
                    memcpy(send + ml, ((unsigned char*)addr), len);
                    //p[0] = len;
                    
                    //ml+=1;
                    ml+=len;
                    //p = tmp;
                    frm->apdu.ctrl.dir = 1;
                    frm->apdu.ctrl.prm = 0;
                    frm->apdu.ctrl.evtmode = 0;
                }
                else
                {
                    frm->apdu.ctrl.dir = 1;
                    frm->apdu.ctrl.prm = 0;
                    frm->apdu.ctrl.evtmode = 0;
                    frm->apdu.ctrl.ftd = 0;
                    frm->apdu.fn = 2;
                }
                
                berr = TRUE;
            }                      
            
            break;
        }
        case MSG_TYPE_FN_41://读flash数据
        {
            if(frm->bDebugFlg == TRUE)
            {
                uint32 addr;
                
                memcpy((uint8 *)&addr, frm->apdu.data, 4);
                len = frm->apdu.data[4];
                if(len > 192)
                {
                    len = 192;
                }
                if((addr <= FLASH_END) )
                {
                    send[ml++] = len;
//                    BFLS_Read(send + ml, (addr), len);
                    send[0] = len;
                    
                    //ml+=1;
                    ml+=len;
                    //p = tmp;
                    frm->apdu.ctrl.dir = 1;
                    frm->apdu.ctrl.prm = 0;
                    frm->apdu.ctrl.evtmode = 0;
                }
                else
                {
                    frm->apdu.ctrl.dir = 1;
                    frm->apdu.ctrl.prm = 0;
                    frm->apdu.ctrl.evtmode = 0;
                    frm->apdu.ctrl.ftd = 0;
                    frm->apdu.fn = 2;
                    
                }
                
                berr = TRUE;
            }                      
            
            break;
        }            
        case MSG_TYPE_FN_42://读eeprom数据
        {
            if(frm->bDebugFlg == TRUE)
            {
                uint32 addr;
                
//	                memset(tmp, 0, 27);
//	                ml = 0;
                memcpy((uint8 *)&addr, frm->apdu.data, 4);
                len = frm->apdu.data[4];
//	                p=(uint8 *)updata_code_cache;
                if(len > 192)
                {
                    len = 192;
                }
                if((addr <= EEPROM_END) )
                {
                    send[ml++] = len;
                
                    I2cRead(0xA0,  send+ml, addr, len); 
                    
                    //ml+=1;
                    ml+=len;
                    //p = tmp;
                    frm->apdu.ctrl.dir = 1;
                    frm->apdu.ctrl.prm = 0;
                    frm->apdu.ctrl.evtmode = 0;
                    //ctrl_bit.ftd = pkt->head.apdu.ctrl.ftd;

                    //fn = pkt->head.apdu.fn;
                }
                else
                {
                    send[ml++] = 0x05;

                    frm->apdu.ctrl.dir = 1;
                    frm->apdu.ctrl.prm = 0;
                    frm->apdu.ctrl.evtmode = 0;
                    frm->apdu.ctrl.ftd = 0;
                    frm->apdu.fn = 2;
                }
                
                berr = TRUE;
            }                      
            
            break;
        case MSG_TYPE_FN_48://设置RSSI门限值
            if(frm->bDebugFlg == TRUE)
            {
                if(rfpara.rf_power != frm->apdu.data[0])
                {
                    rfpara.rf_power = frm->apdu.data[0];

                    I2cWrite(0xA0, (uint8*)&rfpara.rf_limit, FM_NETPARA_ADDR+1, 1);
                }
                send[ml++] = 0xFF;
                send[ml++] = 0xFF;
                
                send[ml++] = 0x00;
                send[ml++] = 0x00;

                berr = TRUE;

                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                frm->apdu.ctrl.ftd = 0;
                frm->apdu.fn = 1;
            }  
            break;
        case MSG_TYPE_FN_49:
            if(frm->bDebugFlg == TRUE)
            {
                send[ml++] = rfpara.rf_power;
                frm->len = ml;
                
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                //frm->apdu.ctrl.ftd = pkt->head.apdu.ctrl.ftd;
                
                //frm->apdu.fn = pkt->head.apdu.fn;
                berr = TRUE;
            }
            else if(g_ucfactory_mod == 0x55)
            {
                cltor_shadow[0].rRssi = (uint8)(0 - frm->rssi);
//                AtcmdReply_RF(AT_CMD_CT, READING, frm->apdu.stInfo.stUp.rssi);

            }

            break;

        case MSG_TYPE_FN_98://设置日志开关
            if(frm->bDebugFlg == TRUE)
            {
                uint8 en = frm->apdu.data[0];
                uint8 level = frm->apdu.data[1];
                
                SYS_LOG_OC(en, level);
                
                send[ml++] = 0xFF;
                send[ml++] = 0xFF;
                
                send[ml++] = 0x00;
                send[ml++] = 0x00;

                berr = TRUE;

                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                frm->apdu.ctrl.ftd = 0;
                frm->apdu.fn = 1;
            }  
            break;
        case MSG_TYPE_FN_99:
            if(frm->bDebugFlg == TRUE)
            {
                send[ml++] = SYS_LOG_OCStt();
                send[ml++] = SYS_LOG_LevelStt();
                frm->len = ml;
                
                frm->apdu.ctrl.dir = 1;
                frm->apdu.ctrl.prm = 0;
                frm->apdu.ctrl.evtmode = 0;
                //frm->apdu.ctrl.ftd = pkt->head.apdu.ctrl.ftd;
                
                //frm->apdu.fn = pkt->head.apdu.fn;
                berr = TRUE;
            }

            break;            
        }                 
        default:
          break;
    }
    if(berr == TRUE)
    {
        frm->len = ml;

//	        stAddr.data = (uint8 *)sBroadAddrFE;
//	        stAddr.len = 6;
//	        stData.data = p;
//	        stData.len = ml;
//	        
//	        SendPacketToMac(&ctrl_bit, fn, 0, &stAddr, &stData, pkt->head.apdu.seq, (uint8)(0 - rssi), &EzHHUTxPkt);
        return SYS_ERR_OK;  //无需回复
    }
    return SYS_ERR_NOREPLY;  //无需回复
}


/************************************************************************
 * @function: fGRFFTD02
 * @描述: 数据转发
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2016/06/26)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fSRFFTD02(const CMD_TABLE_t* tbl, SRF_Frame* frm)
{        
    if(frm->bNeedReAllocate)
    {
#ifdef RF_MESH_LEVEL2
        uint8 * buffer = frm->apdu.data - 4;//需要增加两字节信息和一字节FN
        uint16 length = 0;
        uint8 devAddr[METER_ADDRESS_LENGTH_MAX];
        uint8 addrLen = 0;
        memcpy(buffer, negapkt.data + CON_DEV_ADDR_LEN_10 + 1, 3);
        buffer[3] = frm->apdu.len;
        addrLen = 6;
        memset(devAddr, 0, METER_ADDRESS_LENGTH_MAX);
        memcpy(devAddr, frm->apdu.addr, addrLen);

#if (ADDR_MODE_ASCII > 0)
        memset(devAddr, 0, METER_ADDRESS_LENGTH_MAX);
        
        addrLen = NumToHexString(devAddr, frm->apdu.addr, addrLen, false);

//	                memcpy(buffer + m + 1, devAddr, addrLen);
#endif

        
        length = IE_Compose_LD(senduppkt.data, 1, 1, 1, 
            frm->apdu.seq, addrLen, devAddr, (uint8 *)&frm->apdu.pandId, buffer, frm->apdu.len+4);



        gfs_PSTChnSend[PST_CHN_SET](senduppkt.data, length);
#else
        uint8 m = PKT_HEAD_LEN + frm->apdu.addrlen;
        uint8 ml = 0;

        STINFO3762 stinfo;

        memset((uint8 *)&stinfo, 0, sizeof(STINFO3762));

        STINFO3762 *pstDownInfo;

        ml = 0;
        senduppkt.data[ml++] = START_FLAG;
        ml+=2;
        senduppkt.data[ml++] = 0x82; //控制域:透传回复
        //控制域
        stinfo.stUp.bit1.routeFlg = 1;//pkt->head.apdu.stInfo.stUp.bit1.routeFlg;
        stinfo.stUp.bit1.moduleFlg = 1;//pkt->head.apdu.stInfo.stUp.bit1.moduleFlg;对从节点操作
        stinfo.stUp.bit1.routeNum = 0;//pkt->head.apdu.stInfo.stUp.bit1.routeNum;
        stinfo.stUp.bit3.meterFlg = 0x05;
        stinfo.stUp.bit4.recvSignal = getSignalQuality((int)frm->rssi, RSSI_OFFSET_MF, RSSI_OFFSET_HF);//pkt->head.apdu.stInfo.stUp.bit2.rssi;
        stinfo.stUp.bit3.phaseFlg = frm->apdu.stInfo.stUp.bit3.phaseFlg;
        pstDownInfo = (STINFO3762 *)&negapkt.data[10];

        stinfo.stUp.seq = pstDownInfo->stDown.seq;//pkt->stDownInfo.stDown.seq;
        
        memcpy(senduppkt.data + ml, &stinfo, 6);
        ml+=6;
        memcpy(senduppkt.data + ml, &frm->recv[PKT_HEAD_LEN], 6);
        ml+=6;
        memcpy(senduppkt.data + ml, nDeviceMacAddr, 6);
        ml+=6;

//	                memcpy(senduppkt.data + ml, &pkt->data[PKT_HEAD_LEN], pkt->head.apdu.addrlen);
//	                ml+=pkt->head.apdu.addrlen;
        senduppkt.data[ml++] = negapkt.data[9];//0x02; //AFN
        senduppkt.data[ml++] = 0x01; //数据单元标识
        senduppkt.data[ml++] = 0x00;
        if(negapkt.data[9] == AFN_TYPE_ROUTEDATARELAY)
        {
            senduppkt.data[ml++] = 0x00;//当前报文本地通信上行时长
            senduppkt.data[ml++] = 0x00;
        }
        senduppkt.data[ml++] = frm->apdu.stInfo.stUp.bit3.protocolFlg;
        senduppkt.data[ml++] = frm->apdu.len;
        
        memcpy(&senduppkt.data[ml], &frm->recv[m], frm->apdu.len);
        //去掉SN号的最高字节
        ml+=frm->apdu.len;
        
        senduppkt.data[ml] = Check_SUM(senduppkt.data + 3, ml - 3); //校验位
        ml++;
        senduppkt.data[ml++] = END_FLAG;
        
        senduppkt.data[1] = ml & 0xFF; //长度
        senduppkt.data[2] = ml >> 8;
        
        senduppkt.len = ml;
        
//        UART_PutArray(UART0, senduppkt.data, senduppkt.len); //给集中器发送抄表数据包
#endif        
        
//	                LOG_DEBUG( DBGFMT"uart send p2p data    \n",DBGARG);
//	                LOG_DUMPHEX(LOG_LEVEL_DEBUG, "HexData : \n", senduppkt.data, senduppkt.len);
        
        {
            recvuppkt.isneedack = 0;
            Uppktacked = 1; //已经给集中器回复标志位                        
        }

    }    
    return SYS_ERR_NOREPLY;  //无需回复
}

/************************************************************************
 * @function: fSRFFTD03
 * @描述: APP层控制帧结构
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2016/06/26)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fSRFFTD03(const CMD_TABLE_t* tbl, SRF_Frame* frm)
{        
    uint16 len;
    uint8 m = PKT_HEAD_LEN + frm->apdu.addrlen;
    STINFO3762 stinfo;
    memset((uint8 *)&stinfo, 0, sizeof(STINFO3762));
    uint16 id = 0;
    uint8 SN[CON_DEV_ADDR_LEN_6];
    uint8 errCode = 0;
    uint8 addrLenPer = 2;
    memset(SN, 0, CON_DEV_ADDR_LEN_6);
    if(frm->apdu.stInfo.stUp.bit1.addrFlg == 1)
    {
        addrLenPer = 2;
        memcpy(SN, cltor[frm->id].devAddr, CON_DEV_ADDR_LEN_6);
    }
    else
    {
        addrLenPer = CON_DEV_ADDR_LEN_6;
        memcpy(SN, frm->apdu.addr, CON_DEV_ADDR_LEN_6);
    }
    
    switch(frm->apdu.fn)
    {
        case  0x00:             //F0：转发通信协议数据帧          
          //gfs_PSTChnSend[PST_CHN_AMI](frm->recv, frm->recvLen);
          //gsHXMVari.RFTransmitNRCount=0;
          break;
        case MSG_TYPE_FN_14:
        {
            if(frm->bNeedReAllocate)
            {
            
                len = frm->len - m - 1 - 2;//pkt->data[m++];
                stinfo.stUp.bit1.moduleFlg = 1;
                stinfo.stUp.bit3.meterFlg = 0x05;
                stinfo.stUp.bit4.recvSignal = getSignalQuality((int)frm->rssi, RSSI_OFFSET_MF, RSSI_OFFSET_HF);
                stinfo.stUp.seq = cltor_shadow[1].sendseq++;
//#ifdef RF_MESH_LEVEL2
//                if(g_bTerType == PST_FRM_802_L_NO)
//                {
//                    uint8 * buffer = frm->apdu.data - 1;//V1.0上报数据带一个序号，序号改造成FN和长度
//                    uint16 length = 0;
//                    uint8 addrLen = 6;
//                    uint8 devAddr[METER_ADDRESS_LENGTH_MAX];
//                    
//                    memset(devAddr, 0, METER_ADDRESS_LENGTH_MAX);
//                    memcpy(devAddr, frm->apdu.addr, addrLen);
//                    
//#if (ADDR_MODE_ASCII > 0)
//                    memset(devAddr, 0, METER_ADDRESS_LENGTH_MAX);
//                    
//                    addrLen = NumToHexString(devAddr, frm->apdu.addr, addrLen, false);
//
//    //	                memcpy(buffer + m + 1, devAddr, addrLen);
//#endif
//
//                    
//                    buffer[0] = 0x54;//上报透传返回信息、
//                    buffer[1] = 1;
//                    buffer[2] = len;
//                    length = frm->apdu.len - 2 + 3;
//                    length = IE_Compose_LD(senduppkt.data, 1, 0, 1, 
//                        frm->apdu.seq, addrLen, devAddr, (uint8 *)&frm->apdu.pandId, buffer, length);
//
//
//                    senduppkt.len = length;
//                    gfs_PSTChnSend[PST_CHN_SET](senduppkt.data, length);
//
//                    muchframe_fill(senduppkt.data, senduppkt.len, &g_stMuchframeindex_port, CON_SOUR_PORT, frm->apdu.seq);
//                }
//                else
//                {
//                    uint8 fn = 6;
//                    Compose_3762Data(0xC1, frm->recv+m+2, len, SN,
//                            addrLenPer, &stinfo, 6, ((fn / 8) * 0x100 + (1 << ((fn - 1) % 8))));
//                }
                id = Sn_Search_Id(SN);

                LOG_DEBUG( DBGFMT" search id = %d\n",DBGARG, id);
				
				errCode = 0;

                if ((id > MAX_SUP_SS_NUM) ||( id < SUP_SS_INDEX_START) || (id > rfpara.rf_slotnum)/* || (id >= MAX_REAL_SUP_SS_NUM)*/)
                {               
                    if(frm->apdu.broadCastFlg == 1)
                    {
                        break;
                    }

                    id = 0;                 //当没有分配到有效ID时，分配到0，用于回复SS 
                    errCode = 6;
                }
                
			} 
			else
            {
                if(frm->apdu.broadCastFlg == 1)
                {
                    break;
                }
            
                id = 0;
                errCode = 7;
            }
            
            STMETERPARAFLASH stMeter;
            
            GetCltorPara(id, &stMeter);
            stMeter.reverse = getSignalQuality((int)(-frm->apdu.stInfo.stUp.rssi), RSSI_OFFSET_MF, RSSI_OFFSET_HF);;//frame->app[m++];
            stMeter.hop = frm->apdu.stInfo.stUp.bit1.routeNum;

            UpdataRouteInfo2(id, cltor[id].nodestatus.protocol, frm->apdu.stInfo.stUp.bit1.addrFlg, frm->apdu.addr + addrLenPer,frm->apdu.stInfo.stUp.bit1.routeNum);//joinreq->route.rdepth);

            updataNodeCache(id, CON_NODE_UPDATE_PUSHUP, errCode, frm->apdu.seq, PST_FRM_WL_1_NO, 
                            (uint8)(0 - frm->rssi), SN, CON_DEV_ADDR_LEN_6, &stMeter);
        }
        break;          
        default:
          break;
    }
    
    return SYS_ERR_NOREPLY;  //无需回复
}


/************************************************************************
 * @function: fGRFFTD04
 * @描述: 广播业务帧
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2016/06/26)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fSRFFTD04(const CMD_TABLE_t* tbl, SRF_Frame* frm)
{
//    uint16 len, ml = 0;
    uint8 m = PKT_HEAD_LEN + frm->apdu.addrlen;
    STINFO3762 stinfo;
    uint8 fn;
    memset((uint8 *)&stinfo, 0, sizeof(STINFO3762));
//    uint16 id = 0;
    uint8 SN[CON_DEV_ADDR_LEN_6];
    uint8 addrLenPer = 2;
    memset(SN, 0, CON_DEV_ADDR_LEN_6);
    if(frm->apdu.stInfo.stUp.bit1.addrFlg == 1)
    {
        addrLenPer = 2;
        memcpy(SN, cltor[frm->id].devAddr, CON_DEV_ADDR_LEN_6);
    }
    else
    {
        addrLenPer = CON_DEV_ADDR_LEN_6;
        memcpy(SN, frm->apdu.addr, CON_DEV_ADDR_LEN_6);
    }
    
    switch(frm->apdu.fn)
    {
        case MSG_TYPE_FN_04:
        {
            if(frm->bNeedReAllocate)
            {
            }
            else
            {
                //extern const uint8 sBroadAddrFC[6];
                extern uint8 bNeedRelayBroadCast;
                if(memcmp(SN, sBroadAddrFC, 6) == 0)
                {
                    if(EzBroadCastPkt.bValid)
                    {
                        break;
                    }
                    memset((uint8 *)&EzBroadCastPkt, 0, sizeof(EZ_TX_PKT));
                    EzBroadCastPkt.bValid = 1;
                    EzBroadCastPkt.nBackOffSlot = 0;
                    memcpy(EzBroadCastPkt.TxPkt.data, frm->recv, frm->len);
                    EzBroadCastPkt.TxPkt.len = frm->len;
                    bNeedRelayBroadCast = TRUE;
                    
                    nChangeChannelTime = curslottime + MAX_SLOT_TIME * 2;
                    stinfo.stUp.bit1.moduleFlg = 1;
                    stinfo.stUp.bit3.meterFlg = 0x05;
                    stinfo.stUp.bit4.recvSignal = getSignalQuality((int)frm->rssi, RSSI_OFFSET_MF, RSSI_OFFSET_HF);
                    stinfo.stUp.seq = cltor_shadow[1].sendseq++;
                    fn = 6; 

//                    Compose_3762Data(0xC1, frm->recv+m + 1,frm->apdu.len - 1, &frm->recv[PKT_HEAD_LEN],
//                            frm->apdu.addrlen-addrLenPer,&stinfo, 6, ((fn / 8) * 0x100 + (1 << ((fn - 1) % 8))));
                }

            }
            break;
        }
        break;      
        default:
          break;
    }
    
    return SYS_ERR_NOREPLY;  //无需回复
}

/************************************************************************
 * @function: fSRFFTD05
 * @描述: 查询命令
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2016/06/26)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fSRFFTD05(const CMD_TABLE_t* tbl, SRF_Frame* frm)
{        
    uint16 ml = 0;
    //uint8 m = PKT_HEAD_LEN + frm->apdu.addrlen;
    STINFO3762 stinfo;
    //uint8 fn;
    memset((uint8 *)&stinfo, 0, sizeof(STINFO3762));
    uint16 id = 0;
    uint8 SN[CON_DEV_ADDR_LEN_6];
    uint8 tmp[40];
    uint8 addrLenPer = 2;
    memset(SN, 0, CON_DEV_ADDR_LEN_6);
    memset(tmp, 0, 30);
    if(frm->apdu.stInfo.stUp.bit1.addrFlg == 1)
    {
        addrLenPer = 2;
        memcpy(SN, cltor[frm->id].devAddr, CON_DEV_ADDR_LEN_6);
    }
    else
    {
        addrLenPer = CON_DEV_ADDR_LEN_6;
        memcpy(SN, frm->apdu.addr, CON_DEV_ADDR_LEN_6);
    }
    
    switch(frm->apdu.fn)
    {
        case MSG_TYPE_FN_10:
        {
            if(frm->bNeedReAllocate)
            {
                uint8 version[2];// segmentSeq = pkt->head.apdu.data[2];
                uint8 flashVersiong[4];
                uint8 success = 0;
//                BFLS_Read((uint8 *)flashVersiong, EXT_FLASH_SS_CODE_START+10, 4);

                memcpy(version, frm->apdu.data, 2);
                
                //memcpy(longVersion, pkt->head.apdu.data+2, 20);

                success = !(memcmp(version,flashVersiong,2));

                
                id = Sn_Search_Id(SN);
                
                if(id != g_stUpgrade.p2pSeq ||
                    frm->apdu.seq != g_stUpgrade.frameSeq)//段号和表地址不一样不copy
                {
                    g_stUpgrade.delay = 0;
                    break;
                }
                //memcpy(g_stUpgrade.bitMap, pkt->head.apdu.data+4, bitMapLen);
                g_stUpgrade.eventFlg = 0x55;
                
                stinfo.stUp.bit1.moduleFlg = 0;
                stinfo.stUp.bit3.meterFlg = 0x05;
                stinfo.stUp.seq = cltor_shadow[1].sendseq++;
                stinfo.stUp.bit4.recvSignal = getSignalQuality((int)frm->rssi, RSSI_OFFSET_MF, RSSI_OFFSET_HF);

#ifdef RF_MESH_LEVEL2
                uint8 * buffer = frm->apdu.data - 1;//V1.0上报数据带一个序号，序号改造成FN和长度
                uint16 length = 0;
                uint8 addrLen = 6;
                uint8 devAddr[METER_ADDRESS_LENGTH_MAX];

                if(addrLenPer == 2)
                {
                    memcpy(tmp+1, cltor[id].devAddr, CON_DEV_ADDR_LEN_6);

                }
                else
                {
                    memcpy(tmp+1, frm->apdu.addr, addrLenPer);
                }

#if (ADDR_MODE_ASCII > 0)
                memset(devAddr, 0, METER_ADDRESS_LENGTH_MAX);
                
                addrLen = NumToHexString(devAddr, tmp+1, addrLen, false);

                memcpy(tmp+1, devAddr, addrLen);

#endif
                tmp[ml++] = addrLen;
                ml += addrLen;

                tmp[ml++] = success;
                
                memcpy(tmp+ml, frm->apdu.data+2, 20);
                ml+=20;


                addrLen = 6;
                memset(devAddr, 0, METER_ADDRESS_LENGTH_MAX);
                memcpy(devAddr, frm->apdu.addr, addrLen);
                
#if (ADDR_MODE_ASCII > 0)
                memset(devAddr, 0, METER_ADDRESS_LENGTH_MAX);
                
                addrLen = NumToHexString(devAddr, frm->apdu.addr, addrLen, false);
#endif

                
                buffer[0] = 0x59;//上报从节点升级事件
                memcpy(buffer + 1, tmp, ml);
                length = ml + 1;
                length = IE_Compose_LD(senduppkt.data, 1, 0, 1, 
                    frm->apdu.seq, addrLen, devAddr, (uint8 *)&frm->apdu.pandId, buffer, length);


                senduppkt.len = length;
                gfs_PSTChnSend[PST_CHN_SET](senduppkt.data, length);

                muchframe_fill(senduppkt.data, senduppkt.len, &g_stMuchframeindex_port, CON_SOUR_PORT, frm->apdu.seq);

#else
                uint8 fn = 7;

                if(addrLenPer == 2)
                {
                    memcpy(tmp, cltor[id].devAddr, CON_DEV_ADDR_LEN_6);

                }
                else
                {
                    memcpy(tmp, frm->apdu.addr, addrLenPer);
                }
                ml+=6;
                tmp[ml++] = success;
                
                memcpy(tmp+ml, frm->apdu.data+2, 20);
                ml+=20;

//                Compose_3762Data(0xC0, tmp, ml, NULL, 0, &stinfo, 6, ((fn / 8) * 0x100 + (1 << ((fn - 1) % 8))));
                
#endif                
                g_stUpgrade.crc = CRC_16BIT_Check1((uint8 *)&g_stUpgrade, sizeof(STUPGRADE) - 2);
                I2cWrite(0xA0,  (uint8 *)&g_stUpgrade, DP_DOWNLOAD_PROC_PARA_ADDR, sizeof(STUPGRADE));  
            }
            
            break;
        }
        break;      
        default:
          break;
    }
    
    return SYS_ERR_NOREPLY;  //无需回复
}


/************************************************************************
 * @function: fGRFFTD07
 * @描述: 路由数据
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2016/06/26)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fSRFFTD07(const CMD_TABLE_t* tbl, SRF_Frame* frm)
{      
    uint16 i = 0, nFoundId = 0, id = 0;
    uint8 errCode = 0;
    uint8 nFoundNum = 0;
    uint8 SN[CON_DEV_ADDR_LEN_6];
    uint8 type = 0;
    STMETERPARAFLASH stMeter;
    uint8 addrLenPer = 2;
    memset(SN, 0, CON_DEV_ADDR_LEN_6);
    if(frm->apdu.stInfo.stUp.bit1.addrFlg == 1)
    {
        addrLenPer = 2;
        memcpy(SN, cltor[frm->id].devAddr, CON_DEV_ADDR_LEN_6);
    }
    else
    {
        addrLenPer = CON_DEV_ADDR_LEN_6;
        memcpy(SN, frm->apdu.addr, CON_DEV_ADDR_LEN_6);
    }
    switch(frm->apdu.fn)
    {
        case MSG_TYPE_FN_03://登陆
        {
            
            if(frm->bNeedReAllocate)
            {
                id = getOrAllocateIdByAddr(1, SN, 6);

                errCode = 0;
                
				if ((id > MAX_SUP_SS_NUM) ||( id < SUP_SS_INDEX_START) || (id > rfpara.rf_slotnum)/* || (id >= MAX_REAL_SUP_SS_NUM)*/)
				{				
                    id = 0;					//当没有分配到有效ID时，分配到0，用于回复SS	
                    errCode = 3;
                    break;//不回复
				}
                //cltor_shadow[id].neighnum = 2;//joinreq->neignum;  //邻居个数                         
                uint8 heartBeat = frm->apdu.data[10];
                if(heartBeat > cltparm.routeuptime)
                {
                    cltparm.routeuptime = heartBeat;
                    
                    I2cWrite(0xA0, (uint8*) &cltparm, FM_CLTP_ADDR, 1); //写入路由参数         
                }
                            
                //更新中继节点
                
            }
            else
            {
//                if( Check_Black(SN, 6))
//                {
//                    break;
//                }
                if((uint8)(0 - frm->rssi) > rfpara.rf_limit)
                {
                    break;
                }
                type = frm->apdu.data[1] >> 4;
                
                if(bBroadMeterEnable || (type == 0x02))//开启注册或者中继器
                {
                    id = getOrAllocateIdByAddr(1, SN, 6);


					if ((id > MAX_SUP_SS_NUM) ||( id < SUP_SS_INDEX_START) || (id > rfpara.rf_slotnum))
                    {
                        //分配出错或者满了则不回复
                        break;
                    }               
                
                    errCode = 4;
                }
                else
                {
            
                    id = 0;
                    //cltor[id].nodestatus.result = 2;
                    errCode = 2;
                    if(frm->apdu.broadCastFlg == 1)
                        break;
                }
            }
            GetCltorPara(id, &stMeter);
            memcpy(stMeter.GIS, frm->apdu.data+2, 8);
            memcpy((uint8 *)&stMeter.softver, frm->apdu.data+11, 2);

            stMeter.nHwType = frm->apdu.data[13];//joinreq->softver;//节点版本号   
            stMeter.reverse = getSignalQuality((int)(-frm->apdu.stInfo.stUp.rssi), RSSI_OFFSET_MF, RSSI_OFFSET_HF);;//frame->app[m++];
            stMeter.hop = frm->apdu.stInfo.stUp.bit1.routeNum;
            
            UpdataRouteInfo2(id, cltor[id].nodestatus.protocol, frm->apdu.stInfo.stUp.bit1.addrFlg, frm->apdu.addr + addrLenPer,frm->apdu.stInfo.stUp.bit1.routeNum);//joinreq->route.rdepth);

            updataNodeCache(id, CON_NODE_UPDATE_LOGIN, errCode, frm->apdu.seq, PST_FRM_WL_1_NO, 
                            (uint8)(0 - frm->rssi), SN, CON_DEV_ADDR_LEN_6, &stMeter);
            
            break;
        }        
        case MSG_TYPE_FN_07:
        {

            int return_hash = 0, ReturnHash = 0;
            uint8 bError = 0, devType = 0;
            HASHT elem;

            if(frm->apdu.broadCastFlg == 1)
            {
                break;
            }
            
//            if( Check_Black(SN, 6))
//            {
//                break;
//            }

            for(i = SUP_SS_INDEX_START; i < MAX_SUP_SS_NUM+1; i++)  //遍历
            {
                if(!LPC_Memory_CMP(cltor[i].devAddr, SN, CON_DEV_ADDR_LEN_6))//对比一样
                {
                    nFoundNum++;        
                    nFoundId = i;
                }
                
            }   
            if(0 == nFoundNum)
            {
                id = 0;
                cltor_shadow[id].nodestatus.result = 0x6;
                //id = Re_Allocate_Id(SN);    //根据SN重新分配
            }
            else if(1 == nFoundNum)
            {
                id = nFoundId;
                cltor_shadow[id].nodestatus.result = 0x7;

            }
            else
            {
                for(i=SUP_SS_INDEX_START;i<MAX_SUP_SS_NUM+1;i++)  //遍历
                {
                    if(!LPC_Memory_CMP(cltor[i].devAddr,SN, CON_DEV_ADDR_LEN_6))//对比一样
                        Delete_SS_Node(i);                      
                }                           
                id = Re_Allocate_Id(SN, 6);    //根据SN重新分配
                cltor_shadow[id].nodestatus.result = 0x7;

            }    

			if ((id <= MAX_SUP_SS_NUM) && ( id >= SUP_SS_INDEX_START) && (id <= rfpara.rf_slotnum))
            {
                devType = frm->apdu.data[1] >> 4;
                GetCltorPara(id, &stMeter);
                memcpy(stMeter.GIS, frm->apdu.data+2, 8);
                uint8 heartBeat = frm->apdu.data[10];
                if(heartBeat > cltparm.routeuptime)
                {
                    cltparm.routeuptime = heartBeat;
                    
                    I2cWrite(0xA0, (uint8*) &cltparm, FM_CLTP_ADDR, 1); //写入路由参数         
                }
                memcpy((uint8 *)&stMeter.softver, frm->apdu.data+11, 2);
                
                stMeter.nHwType = frm->apdu.data[13];//joinreq->softver;//节点版本号                   
                            
                
                memcpy(elem.addr, SN, CON_DEV_ADDR_LEN_6); //电表地址
//	                memcpy(elem.falsh_sn, SN, CON_DEV_ADDR_LEN_6);
                elem.SubType = frm->apdu.stInfo.stUp.bit3.protocolFlg;
                switch(devType)
                {
                case 0:
                    elem.devType = 1;
                    break;
                case 1:
                    elem.devType = 0;
                    break;
                default:
                    elem.devType = devType;
                    break;
                }
                elem.src = 0x55;//自动注册
                return_hash=Hash_Table_Insert(&elem, &ReturnHash);
//	                    if (return_hash)        //添加哈希表                                        
//	                    {  
                if (return_hash==MAX_HASH_TABLE_FLASH_SIZE)
                {   
                    HASH_Table_Used++;
                    HASH_Table_Used=(HASH_Table_Used&1);   //1表示之前用Table1,之后Table2;0表示用Table2
                    I2cWrite(0xA0,&HASH_Table_Used, FM_HASH_USED, 1);//保存标识位
                    Hash_Transfer();//表满,转移哈希表
                    return_hash = Hash_Table_Insert(&elem, &ReturnHash);//再次插入上次未添加成功的
                    if((return_hash >= 0 ) && (return_hash == MAX_HASH_TABLE_FLASH_SIZE))
                    {
                        bError = 1;
                    }
                }
//	                    }
//	                    else                                
//	                        bError = 1;      

                if(bError)//非法设备不允许加入
                {
                    id = 0;
                    cltor_shadow[id].nodestatus.result = 0x6;                        
                }
                else
                {
                    cltor_shadow[id].nodestatus.result = 0x07;//表示加入成功
                    if(elem.devType != 2)//中继器不上报
                    {
                        MeterStatus[return_hash].bReceive = 1;
                    }
                }
            }
            else
            {
                id = 0;
                cltor_shadow[id].nodestatus.result = 0x6;                        
            }
            cltor[id].addrLen = CON_DEV_ADDR_LEN_6;
            memset(cltor[id].devAddr, 0, CON_DEV_ADDR_LEN_10);
            
            memcpy(cltor[id].devAddr, SN, CON_DEV_ADDR_LEN_6);  // 记录添加节点的唯一标识SN 
            
            cltor_shadow[id].nodestatus.ans_pkt_type=0x1;     //入网包标识位
//	            cltor[id].nodestatus.fixed= 0x0;           //表示该点未绑定
            cltor_shadow[id].nodestatus.bNetAck= 0x1;         //表示该点需要回复
//	            cltor[id].nodestatus.handled = UN_HANDLED;   //重新标记为未处理
            //cltor[id].nodestatus.NetStatus = NODE_STATUS_OUT;
            //cltor[id].old_id=id;//joinreq->sourid;        //原先上报的节点号
            cltor_shadow[id].recvseq = frm->apdu.seq;
            cltor_shadow[id].FrameDeadTime = 0xff;//joinreq->ttl;     
            cltor_shadow[id].rRssi = (uint8)(0 - frm->rssi);
            cltor[id].nodestatus.protocol = PST_FRM_WL_1_NO;
//	            UpdataRouteInfo1(id, pkt);//joinreq->route.rdepth);
            UpdataRouteInfo2(id, cltor[id].nodestatus.protocol, frm->apdu.stInfo.stUp.bit1.addrFlg, frm->apdu.addr + addrLenPer,frm->apdu.stInfo.stUp.bit1.routeNum);//joinreq->route.rdepth);

            if(id > 1 && (!bError))
            {
                SyncCltorPara(id, &cltor[id], &stMeter);
            }
            break;
        }
        case MSG_TYPE_FN_05://心跳
        case MSG_TYPE_FN_04://登出
        {
//	            u32SendTickToSSDeadTime = curslottime + 40; //收到点名包回复延时后在触发点名
            
            if(frm->bNeedReAllocate)
            {
                id = Sn_Search_Id(SN);
//							for(i=SUP_SS_INDEX_START;i<MAX_SUP_SS_NUM;i++)	//遍历
//							{
//								if(!LPC_Memory_CMP(cltor[i].devAddr,SN, 6))//对比一样
//									Delete_SS_Node(i);						
//							}						
//					cltor[id].nodestatus.handled=UN_HANDLED;			//重新标记为未处理
				
                cltor_shadow[id].nodestatus.result = 0;
                if ((id > MAX_SUP_SS_NUM) ||( id < SUP_SS_INDEX_START) || (id > rfpara.rf_slotnum) /*|| (id >= MAX_REAL_SUP_SS_NUM)*/)
                {               
#ifdef PROJECT_PINGSHUI
                    break;       
#else
                    id = 0;                 //当没有分配到有效ID时，分配到0，用于回复SS 
                    cltor[id].addrLen = CON_DEV_ADDR_LEN_6;
                    memset(cltor[id].devAddr, 0, CON_DEV_ADDR_LEN_10);
                    
                    memcpy(cltor[0].devAddr, SN, CON_DEV_ADDR_LEN_6);//保存SN
                    
                    cltor_shadow[id].nodestatus.result = 3;
#endif
                }
				//删除
				if(frm->apdu.fn == MSG_TYPE_FN_04)
                {
                    cltor[id].nodestatus.NetStatus = NODE_STATUS_OUT;
                }                  
			} 
			else
            {
                id = 0;

                {
                    cltor_shadow[id].nodestatus.result = 2;
                    
                    if(frm->apdu.broadCastFlg == 1)
                        break;
                }
            } 
            cltor[id].addrLen = CON_DEV_ADDR_LEN_6;
            memset(cltor[id].devAddr, 0, CON_DEV_ADDR_LEN_10);

            memcpy(cltor[id].devAddr, SN, CON_DEV_ADDR_LEN_6);   //保存SN    

			if((cltor[id].devAddr[0] < 0xFF))
			{
				cltor_shadow[id].nodestatus.ans_pkt_type=0;		//心跳包标识位
				
				if(frm->apdu.fn == MSG_TYPE_FN_04)
                {
                    cltor_shadow[id].nodestatus.ans_pkt_type = 2;
                }
                
				cltor_shadow[id].nodestatus.bNetAck= 1;		//心跳包时需要回复
				//cltor[id].old_id=id;
//					cltor[id].nodestatus.fixed= 1;				//表示该点绑定				
				cltor_shadow[id].FrameDeadTime=  0xff;		
                
                cltor_shadow[id].recvseq = frm->apdu.seq;
            }
            cltor_shadow[id].rRssi = (uint8)(0 - frm->rssi);
            cltor[id].nodestatus.protocol = PST_FRM_WL_1_NO;
            //更新中继节点
//	            UpdataRouteInfo1(id, pkt);//joinreq->route.rdepth);
            UpdataRouteInfo2(id, cltor[id].nodestatus.protocol, frm->apdu.stInfo.stUp.bit1.addrFlg, frm->apdu.addr + addrLenPer,frm->apdu.stInfo.stUp.bit1.routeNum);//joinreq->route.rdepth);
            if(id > 1)
            {
                SyncCltorPara(id, &cltor[id], NULL);
            }
//	                    if((0 != joinreq->neignum) && (8 != joinreq->neignum))
//	                    {
//	                        i = JOINREQ_SN_OFFSET + UNIQUE_MAC_ADDR_LEN + 1;                
//	                        cltor[id].nHwType = pkt->data[i];       //硬件标识
//	                        cltor[id].nSoftFn = pkt->data[i + 1];       //Fn
//	                        cltor[id].nSubSoftVersion = pkt->data[i + 2];   //附属器件软件版本                      
//	                    }
            break;
        }                     
        default:
        break;
    }
    
    return SYS_ERR_NOREPLY;  //无需回复
}


/************************************************************************
 * @function: fSRFFTD09
 * @描述: 文件传输
 * 
 * @参数: 
 * @param: frm 
 * 
 * @返回: 
 * @return: uint8  
 * @说明: 
 * @作者: yzy (2016/06/26)
 *-----------------------------------------------------------------------
 * @修改人: 
 ************************************************************************/
uint8 fSRFFTD09(const CMD_TABLE_t* tbl, SRF_Frame* frm)
{        
//    uint16 len, ml = 0;
    //uint8 m = PKT_HEAD_LEN + frm->apdu.addrlen;
    STINFO3762 stinfo;
    //uint8 fn;
    memset((uint8 *)&stinfo, 0, sizeof(STINFO3762));
    uint16 id = 0;
    uint8 SN[CON_DEV_ADDR_LEN_6];
    //uint8 tmp[30];
    //uint8 addrLenPer = 2;
    
    memset(SN, 0, CON_DEV_ADDR_LEN_6);
    //memset(tmp, 0, 30);
    if(frm->apdu.stInfo.stUp.bit1.addrFlg == 1)
    {
        //addrLenPer = 2;
        memcpy(SN, cltor[frm->id].devAddr, CON_DEV_ADDR_LEN_6);
    }
    else
    {
        //addrLenPer = CON_DEV_ADDR_LEN_6;
        memcpy(SN, frm->apdu.addr, CON_DEV_ADDR_LEN_6);
    }
    
    switch(frm->apdu.fn)
    {
        case MSG_TYPE_FN_04:
        {
            if(frm->bNeedReAllocate)
            {
                uint8 segmentSeq = frm->apdu.data[2];
                uint8 bitMapLen = frm->apdu.data[3];
                
                id = Sn_Search_Id(SN);
                if(bitMapLen > DOWN_UPGRADE_BITMAP_SIZE)
                {
                    bitMapLen = DOWN_UPGRADE_BITMAP_SIZE;
                }
                if(segmentSeq != g_stUpgrade.p2pBitMapIndex ||
                    id != g_stUpgrade.p2pSeq ||
                    frm->apdu.seq != g_stUpgrade.frameSeq)//段号和表地址不一样不copy
                {
                    g_stUpgrade.delay = 0;
                    break;
                }
                memcpy(g_stUpgrade.bitMap, frm->apdu.data+4, bitMapLen);
                g_stUpgrade.bitMapFlg = 0x55;
                
                g_stUpgrade.crc = CRC_16BIT_Check1((uint8 *)&g_stUpgrade, sizeof(STUPGRADE) - 2);
                I2cWrite(0xA0,  (uint8 *)&g_stUpgrade, DP_DOWNLOAD_PROC_PARA_ADDR, sizeof(STUPGRADE));  
            }
            
            break;
        }
        break; 
        default:
          break;
    }
    
    return SYS_ERR_NOREPLY;  //无需回复
}

