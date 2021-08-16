#ifndef _GLOBALVARIABLE_H_
#define _GLOBALVARIABLE_H_

#ifndef MAIN_VAR
#define MAIN_VAR    extern
#endif

#ifndef USER_VAR
#define USER_VAR    extern
#endif

#ifndef SX1276_VAR
#define SX1276_VAR    extern
#endif
#ifndef UPGRADE_VAR
#define UPGRADE_VAR extern
#endif

//MAIN_VAR STVERSION sVersiong;
MAIN_VAR uint8 Uart_Status;
MAIN_VAR uint32 ExtCtrlDeadCont;
MAIN_VAR const uint8 ucModeVer[];
MAIN_VAR const STVERSION sVersiong;
MAIN_VAR tRadioDriver *Radio;
//MAIN_VAR const uint32 gul_UsrFuncVer;
MAIN_VAR volatile uint8 g_bTerType;
MAIN_VAR ST_SEND_CACHE_INDEX g_stSendCacheIndex;

USER_VAR uint8 nDeviceMacAddrLen;
USER_VAR uint8 nDeviceMacAddr[METER_ADDRESS_LENGTH_MAX];
USER_VAR uint8 nFreqChannelNum;
USER_VAR uint32 nChangeChannelTime;
USER_VAR uint8 bNeedChangeChannel;
USER_VAR uint32 curslottime;
USER_VAR RF_PARAMETER rfpara;
USER_VAR volatile int TickSysFlag;
USER_VAR UPPKT recvuppkt;
USER_VAR UPPKT senduppkt;
USER_VAR UPPKT SendMeterpkt;
USER_VAR COLLECTOR cltor[MAX_SUP_SS_NUM + 1];
USER_VAR uint16 nBroadMeterTime;//�ӽڵ�����ע�����ʱ��
USER_VAR uint8 bBroadMeterEnable;//�ӽڵ�����ע���־
USER_VAR uint8 HASH_Table_Used;
USER_VAR COLLECTOR_SHADOW cltor_shadow[MAX_SUP_SS_NUM + 1];
USER_VAR uint8 	CurWhiteSSMap;		// 0: ������1;     1: ������2
USER_VAR uint8 	CurBlackSSMap;		// 0: ������1;     1: ������2  
USER_VAR uint8 bMeterDeadStart;
USER_VAR uint8 bMeterSearchStart;
USER_VAR METER_STATUS MeterStatus[MAX_HASH_TABLE_FLASH_SIZE];
USER_VAR uint8 bCltorSearchStart;
USER_VAR uint8 bNeedDeleteMeter;
USER_VAR uint8 bNeedMeterDeleteAck;
USER_VAR CLTP cltparm; // �ɼ�����ز���
USER_VAR uint16 localid;
USER_VAR const uint8 sBroadAddr99[8];
USER_VAR const uint8 sBroadAddrAA[8];
USER_VAR const uint8 sBroadAddrFC[8];
USER_VAR const uint8 sBroadAddrFD[8];
USER_VAR const uint8 sBroadAddrFE[8];
USER_VAR uint8 StaAck;
USER_VAR PKT negapkt;
//	USER_VAR EZ_TX_PKT EzDataTxPkt;//ҵ�������
//	USER_VAR EZ_TX_PKT EzNetTxPkt;//���������
//	USER_VAR EZ_TX_PKT EzHHUTxPkt;//���������
USER_VAR EZ_TX_PKT EzBeaconTxPkt;//ҵ�������
USER_VAR uint32 sysSlotTime;

USER_VAR uint8 g_bResetCmd;
USER_VAR QUEUE rxdopktq;
USER_VAR uint32 Uppktacked;
USER_VAR HASH_INFO shadow_space[MAX_HASH_TABLE_FLASH_SIZE];
USER_VAR uint8 IapAction;
USER_VAR EZ_TX_PKT EzBroadCastPkt;//�㲥������
USER_VAR STMUCHFRAMEINDEX g_stMuchframeindex_port;
USER_VAR uint8 g_ucfactory_mod;
USER_VAR uint32 updata_code_cache[128];//256byte����
USER_VAR uint32 HashUpdateSectorAdd;
USER_VAR uint8 Write_Sucess;
USER_VAR uint8 MeterDeadCount;
//�ɼ�����֤ʹ�ܲ���
USER_VAR uint8 bSSValidateEnable;
USER_VAR ST_PENDING_ADDR_LIST g_stPendingAddrList;
USER_VAR uint32 g_nModuleResetTime;
//�����㲥�����־
USER_VAR uint8 bSendResetCmd;
//���������������־	0: ����  1:��ֹ
USER_VAR uint8 bJcDisallowFalg;
//SX1276_VAR tLoRaSettings LoRaSettings;
//SX1276_VAR uint8   g_ucChannelNum;
//SX1276_VAR const int8 powerArray[5];
//SX1276_VAR uint32 g_nModuleResetTimeOut;
//

UPGRADE_VAR STUPGRADE g_stUpgrade;
UPGRADE_VAR UPDATE_PKT upd_pkt;//�������ṹ��
UPGRADE_VAR uint8 g_ucUpgradeFlgForPush;
#endif
