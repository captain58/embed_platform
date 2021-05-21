/**
 * @file dev_pwrchk.c
 * @brief ����ԴAD����������Ӧ��Դ�ļ�
 * @details ��ģ��ʵ���˵�Դ��ʼ���м��ȴ����������ϵ�ʱ���ȵ��ã�����ϵͳ��ӵ�Դ
 *          �����������������ݳ��һ���̶Ⱥ���������б��ϵͳ��Ҳʵ����ϵͳ��������
 *          ��ÿ3����һ�ε�Դ��ѹ����Ԥ�ȶ����״̬ת�����ѹ�Ƚϣ�������״̬�仯��
 *          ���ô����������͵�Դ״̬�仯�¼����д����ﵽ���Ʊ����������Ŀ�ġ���
 *          �⣬��ģ�黹�ṩ���Զ����ѹ���⹦�ܺ͵�Դ���״̬����ѹֵ��ѯ�ӿڡ�
 * @mainpage ���̸���
 * @author    yzy
 * @email     
 * @version   1.0.0
 * @date      2019-3-18
 * @license   captain
 */
 #define EXT_DEV_PWRCHK
#ifdef __MODULE__
#include "WOSsys.h"
#endif
#include "sys.h"
#include "hal.h"
#include "bsp.h"

#include "eventApi.h"
#include "SysS.h"
/*----------------------------------------------------------------------------------
****************************ȫ�ֱ������������弰��������****************************
----------------------------------------------------------------------------------*/
static uint32_t SysVol = 0;			///< ϵͳ������Դ��ѹ 
ST_FSM PmmFsm;						      	///< ��Դ״̬�任������
__no_init u8  gucSyssRun[4] @ 0x20007000;
void EventNorToOwn(void);
void EventNorToLow(void);
void EventNorToCut(void);
void EventOwnToNor(void);
void EventOwnToLow(void);
void EventOwnToCut(void);
void EventLowToNor(void);
void EventLowToOwn(void);
void EventLowToCut(void);
void EventCutToNor(void);
void EventCutToOwn(void);
void EventCutToLow(void);

/** 
 * @brief ��Դ��ѹ״̬ת���㳣���ṹ�� 
 * @details �ṹ���ԱǷѹ��VolOwn��ʼ��ΪPOWVOL_OWN����ѹ��VolLow��ʼ��ΪPOWVOL_LOW��
 *          �����VolCut��ʼ��ΪPOWVOL_CUT�� 
 */
const ST_POW_ALM_PNT PowAlarm =                                                                                                          
{           
    POWVOL_OWN,
	POWVOL_LOW,
	POWVOL_CUT
 };
typedef enum
{
    EventNorToOwnFuncID=0,
    EventNorToLowFuncID,
    EventNorToCutFuncID,
    EventOwnToNorFuncID,
    EventOwnToLowFuncID,
    EventOwnToCutFuncID,
    EventLowToNorFuncID,
    EventLowToOwnFuncID,
    EventLowToCutFuncID,
    EventCutToNorFuncID,
    EventCutToOwnFuncID,
    EventCutToLowFuncID,
}ENEVENTFUNC;

/**
 * @brief ��Դ״̬�¼���
 */
 const ST_FSM_TABLE PowEventTable[13] =      
{
	{POWNOR,    POWOWN,		EventNorToOwnFuncID,		POWOWN      },
	{POWNOR,    POWLOW, 	EventNorToLowFuncID, 		POWLOW      },
	{POWNOR,    POWCUT, 	EventNorToCutFuncID, 		POWCUT      },
	{POWOWN,    POWNOR, 	EventOwnToNorFuncID, 		POWNOR      },
	{POWOWN,    POWLOW, 	EventOwnToLowFuncID, 		POWLOW      },
	{POWOWN,    POWCUT, 	EventOwnToCutFuncID, 		POWCUT      },
	{POWLOW,    POWNOR, 	EventLowToNorFuncID, 		POWNOR      },
	{POWLOW,    POWOWN,		EventLowToOwnFuncID, 		POWOWN      },
	{POWLOW,    POWCUT, 	EventLowToCutFuncID, 		POWCUT      },
	{POWCUT,    POWNOR, 	EventCutToNorFuncID, 		POWNOR      },
	{POWCUT,    POWOWN, 	EventCutToOwnFuncID,		POWOWN      },
	{POWCUT,    POWLOW, 	EventCutToLowFuncID,		POWLOW      },
	{0, 		0,		    0xffff,		            0           },      //���һ�б���
};
//	const ST_FSM_TABLE PowEventTable[13] =      
//	{
//		{POWNOR,    POWOWN,		EventNorToOwn,		POWOWN      },
//		{POWNOR,    POWLOW, 	EventNorToLow, 		POWLOW      },
//		{POWNOR,    POWCUT, 	EventNorToCut, 		POWCUT      },
//		{POWOWN,    POWNOR, 	EventOwnToNor, 		POWNOR      },
//		{POWOWN,    POWLOW, 	EventOwnToLow, 		POWLOW      },
//		{POWOWN,    POWCUT, 	EventOwnToCut, 		POWCUT      },
//		{POWLOW,    POWNOR, 	EventLowToNor, 		POWNOR      },
//		{POWLOW,    POWOWN,		EventLowToOwn, 		POWOWN      },
//		{POWLOW,    POWCUT, 	EventLowToCut, 		POWCUT      },
//		{POWCUT,    POWNOR, 	EventCutToNor, 		POWNOR      },
//		{POWCUT,    POWOWN, 	EventCutToOwn,		POWOWN      },
//		{POWCUT,    POWLOW, 	EventCutToLow,		POWLOW      },
//		{0, 		0,		    0,		            0           },      //���һ�б���
//	};


void * EventFunc(uint16_t id);


/*-----------------------------------------------------------------------------------
*******************************��������ʵ��******************************************
-----------------------------------------------------------------------------------*/
/**
 * @brief ��Դ״̬�л�ע�ắ��
 * @details ���Դ״̬���й���ṹ��ʵ���д���Ԥ����ĵ�Դ״̬�¼���
 * @param pFsm,��Դ״̬���й���ṹ��ʵ��
 * @param *pTable,Ԥ����ĵ�Դ״̬�¼���
 * @return ��
 * @retval ��
 * @note ֱ�ӷ���
 * @attention ��״̬�л�����ǰ�ȵ���ע�ắ������Ԥ���帳ֵ��
 * @warning ��
 * @exception ��
 */
void FSM_Regist(ST_FSM* pFsm, const ST_FSM_TABLE* pTable)
{
    pFsm->FsmTable = pTable;
}
/**
 * @brief ��Դ״̬ת�����к���
 * @details ͨ����ǰ״̬�͵�ǰ�¼����������жϣ�����Ԥ����ĵ�Դ״̬�¼������״̬�л���
 * @param pFsm��Դ״̬���й���ṹ��ʵ��
 * @param event��ǰ�¼�
 * @return ��
 * @retval ��
 * @note ֱ�ӷ���
 * @attention ���б��ӿڸ����¼����е�Դ״̬�л���
 * @warning ��
 * @exception ��
 */
void FSM_EventHandle(ST_FSM* pFsm, int event)
{
    unsigned char i;
    const ST_FSM_TABLE *FsmTable;

    FsmTable = pFsm->FsmTable;
    if(FsmTable == 0)   //�ж�����ָ����Ч��
    {
        return;
    }
    for(i = 0; ; i++)
	{
	    if(FsmTable[i].CurState != 0)
        {   
            if((FsmTable[i].Event == event) && (FsmTable[i].CurState == pFsm->CurState))  //�������������ϲŽ���״̬�л� 
    		{
    		    pFsm->CurState = FsmTable[i].NextState;     //״̬���� 
                pFsm->PreState = pFsm->CurState;            //״̬���ģ�����һ�μ����˵��PreState����CurState
//	                FsmTable[i].EventAct();
                ((void(*)()) EventFunc(FsmTable[i].EventActID))();
                break;
    		}
        }
        else
        {
            break;
        }
	}
} 
/**
 * @brief ��ȡϵͳ�ⲿ��Դ״̬�ӿ�
 */
ET_POW_STA GetSysPowSta(void)
{
    return (ET_POW_STA)PmmFsm.CurState;
}

/**
 * @brief ��ѯϵͳ�ⲿ��ѹֵ�ӿ�
 */
unsigned short GetSysPow(void)
{
    return SysVol;
}


void * EventFunc(uint16_t id)
{
    switch(id)
    {
        case EventNorToOwnFuncID:
            return (void *)&EventNorToOwn;
        case EventNorToLowFuncID:
            return (void *)&EventNorToLow;
        case EventNorToCutFuncID:
            return (void *)&EventNorToCut;
        case EventOwnToNorFuncID:
            return (void *)&EventOwnToNor;
            
        case EventOwnToLowFuncID:
            return (void *)&EventOwnToLow;
        case EventOwnToCutFuncID:
            return (void *)&EventOwnToCut;
        case EventLowToNorFuncID:
            return (void *)&EventLowToNor;
        case EventLowToOwnFuncID:
            return (void *)&EventLowToOwn;
        case EventLowToCutFuncID:
            return (void *)&EventLowToCut;
        case EventCutToNorFuncID:
            return (void *)&EventCutToNor;
        case EventCutToLowFuncID:
            return (void *)&EventCutToLow;
        case EventCutToOwnFuncID:
            return (void *)&EventCutToOwn;            
        default:
            return (void *)&EventNorToOwn;
            break;
    }


}


/**
 * @brief ��ȡ��ѹ״̬�仯ʱ��Ҫ���Ĵ���
 * @details ���ݵ�ǰ��ѹ״̬��Ŀ��״̬������״̬�仯ʱ��ҪAD���Ĵ���
 * @param tempSta Ŀ���Դ��ѹ״̬
 * @param curSta ��ǰ��Դ��ѹ״̬
 * @return ������ҪAD���Ĵ���
 * @retval 1��3
 * @note �̶�����
 * @attention ��ѹ���ϱ仯ʱֻ���һ�Σ���ѹ���±仯ʱ������ǵ��磬���һ�Σ�������3�Ρ�
 * @warning 
 * @exception 
 */
unsigned char GetChkTime(unsigned char tempSta, unsigned char curSta)
{
    switch(curSta)
    {
    case POWNOR:
        if(tempSta == POWOWN)
            return 2;
        else if(tempSta == POWLOW)
            return 1;
        else if(tempSta == POWCUT)
            return 1;
        break;
    case POWOWN:
        if(tempSta == POWNOR)
            return 1;
        else if(tempSta == POWLOW)
            return 2;
        else if(tempSta == POWCUT)
            return 1;
        break;
    case POWLOW:
        if(tempSta == POWNOR)
            return 1;
        else if(tempSta == POWOWN)
            return 1;
        else if(tempSta == POWCUT)
            return 1;
        break;
    case POWCUT:
        if(tempSta == POWNOR)
            return 1;
        else if(tempSta == POWOWN)
            return 1;
        else if(tempSta == POWLOW)
            return 1;
        break;
    default:
        break;
    }
    return 1;
}

/**
 * @brief ��Դ�������
 * @details ÿ3������һ�Σ�����ⲿ��Դ��ѹֵ��һ�ǽ���ѹֵ�������ѹ����бȽ��жϣ��ٸ��������ѹ
 *          ����״̬�����ǽ���ѹֵ��ϵͳ��Դ״̬��ѹ����бȽ��жϣ�ȷ�ϵ�ǰ״̬���������ĵ�Դ�¼�
 *          �����õ�Դ״̬ת�����к�����ת����Դ״̬�����е�Դ�¼���������������Դ�¼���
 * @param ��
 * @return ��
 * @retval ��
 * @note ��Ϊ�͹��Ķ�ʱ��������
 * @attention 
 * @warning 
 * @exception 
 */
void PowManageProcess(void)
{
    static unsigned char    IsSame  = 0;    //�Ƚ�״̬��ͬ�Ĵ������趨Ϊ3��
    unsigned char           temp;
    uint8_t diotstt = 0;
//	    SysVol = PCB_VBatVoltGet();
    SYS_AD_Scan(AD_ID_MBATVOL, &SysVol);

    /* 
     * ϵͳ��ѹ״̬����
     * �жϵ�ǰ������ѹ״̬�����ӵ�ѹ���ͣ����������ڲ����� ���ڵ�ѹ״̬���ǻ��ͣ�
     * �ɵ�����3��ͬ���ĵ�ѹ״̬�����뵱ǰ��ѹ״̬��һ�²Ž��е�ѹ״̬ת�� 
     */  
    temp = PmmFsm.CurState;     //temp����ʼ״̬Ϊ��ǰ״̬����ֹGPRS�ϸ�ʱ������ѹ�仯��ô����ѹ״̬
    SysOptDat(DIOTMDL_BUSID_STT,0,0,1,&diotstt);
    
    LOG_DEBUG(DBGFMT"@==cur: Vol=%dmV stt=%d ==@\r\n", DBGARG, SysVol, PmmFsm.CurState);

    switch(PmmFsm.CurState)
    {
    case POWNOR:
        if((SysVol <= (PowAlarm.VolOwn - HYSTERESISVOL)) && (SysVol > PowAlarm.VolLow))
        {
//	            if((IotInfo.IsIotUpcoming() == 1) || (IotInfo.IsNeedWaiting() == 1))//�ϸ�ʱ�����ϸ�����ȴ���ѹ�ָ�ʱ���Ե�����ĵ�ѹ���
            if(diotstt)
            {
                break;
            }
            LOG_DEBUG("\r\n@==��ѹתǷѹ: Vol=%dmV ==@\r\n", SysVol);
            temp = POWOWN;
        } 
        else if((SysVol <= PowAlarm.VolLow) && (SysVol > PowAlarm.VolCut))
        {
//	            if((IotInfo.IsIotUpcoming() == 1) || (IotInfo.IsNeedWaiting() == 1))//�ϸ�ʱ�����ϸ�����ȴ���ѹ�ָ�ʱ���Ե�����ĵ�ѹ���
            if(diotstt)
            {
                break;
            }
            LOG_DEBUG("\r\n@==��ѹת��ѹ: Vol=%dmV ==@\r\n", SysVol);
            temp = POWLOW;
        }
        else if(SysVol <= PowAlarm.VolCut)
        {
			LOG_INFO("\r\n@==��ѹת����: Vol=%dmV ==@\r\n", SysVol);
            temp = POWCUT;
        }
        else    //������   
        {
            temp = PmmFsm.CurState;
        }
        break;
    case POWOWN:
        if(SysVol > (PowAlarm.VolOwn + HYSTERESISVOL))
        {
            LOG_DEBUG("\r\n@==Ƿѹת����: Vol=%dmV ==@\r\n", SysVol);
            temp = POWNOR;
        }
        else if( (SysVol <= (PowAlarm.VolLow - HYSTERESISVOL)) && (SysVol > PowAlarm.VolCut))
        {
//	            if((IotInfo.IsIotUpcoming() == 1) || (IotInfo.IsNeedWaiting() == 1))//�ϸ�ʱ�����ϸ�����ȴ���ѹ�ָ�ʱ���Ե�����ĵ�ѹ���
            
            if(diotstt)
            {
                break;
            }
            LOG_DEBUG("\r\n@==Ƿѹת��ѹ: Vol=%dmV ==@\r\n", SysVol);
            temp = POWLOW;
        }
        else if(SysVol <= PowAlarm.VolCut)
        {
			LOG_INFO("\r\n@==Ƿѹת����: Vol=%dmV ==@\r\n", SysVol);
            temp = POWCUT;
        }
        else    //������
        {
            temp = PmmFsm.CurState;
        }
        break;
    case POWLOW:
        if(SysVol > PowAlarm.VolOwn)
        {
        
            LOG_DEBUG("\r\n@==��ѹת����: Vol=%dmV ==@\r\n", SysVol);
            temp = POWNOR;
        }
        else if(SysVol > (PowAlarm.VolLow + HYSTERESISVOL))
        {
            
            LOG_DEBUG("\r\n@==��ѹתǷѹ: Vol=%dmV ==@\r\n", SysVol);
            temp = POWOWN;
        }
        else if(SysVol <= (PowAlarm.VolCut - HYSTERESISVOL))
        {
        
            LOG_INFO("\r\n@==��ѹת����: Vol=%dmV ==@\r\n", SysVol);
            temp = POWCUT;
        }
        else
        {
            temp = PmmFsm.CurState;
        }
        break;
    case POWCUT:
        if(SysVol > PowAlarm.VolOwn)
        {
            LOG_INFO("\r\n@==����ת����: Vol=%dmV ==@\r\n", SysVol);
            temp = POWNOR;
        }
        else if(SysVol > PowAlarm.VolLow)
        {
            LOG_INFO("\r\n@==����תǷѹ: Vol=%dmV ==@\r\n", SysVol);
            temp = POWOWN;
        }
        else if(SysVol > PowAlarm.VolCut + HYSTERESISVOL)
        {
            LOG_INFO("\r\n@==����ת��ѹ: Vol=%dmV ==@\r\n", SysVol);
            temp = POWLOW;
        }
        else
        {
            temp = PmmFsm.CurState;
        }
        break;
    default:
        break;
    }
    if(temp != PmmFsm.CurState)
    {
		if(temp == PmmFsm.PreState)
		{
			IsSame++;
		}
		else
		{
			PmmFsm.PreState = temp;
			IsSame    = 1;
		}
		if(IsSame >= GetChkTime(temp, PmmFsm.CurState)) //�ۼƴ��������趨�ļ�����
		{
		   IsSame = 0;
		   FSM_EventHandle(&PmmFsm, temp);   //ϵͳ��ѹ�ı䣬ִ��״̬ת����
		}
    }
    else
    {
        IsSame  = 0;
    }
}

/**
 * @brief ��ѹ��Ƿѹ�¼�����
 * @details ��������ѹ��Ƿѹ�¼������ͳ�ѹ��Ƿѹ�¼�
 * @param ��
 * @return ��
 * @retval ��
 * @note ��ѹ��Ƿѹʱ����
 * @attention ��Դ״̬�¼������1
 * @warning 
 * @exception 
 */
void EventNorToOwn(void)
{
    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_NORMALTOLACK);
    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_NORMALTOLACK);
}

/**
 * @brief ��ѹ����ѹ�¼�����
 * @details ��������ѹ����ѹ�¼������ͳ�ѹ����ѹ�¼�
 * @param ��
 * @return ��
 * @retval ��
 * @note ��ѹ����ѹʱ����
 * @attention ��Դ״̬�¼������2
 * @warning 
 * @exception 
 */
void EventNorToLow(void)
{
    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_NORMALTOLOW);
    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_NORMALTOLOW);
}

/**
 * @brief ��ѹ�������¼�����
 * @details ��������ѹ�������¼������ͳ�ѹ�������¼�
 * @param ��
 * @return ��
 * @retval ��
 * @note ��ѹ������ʱ����
 * @attention ��Դ״̬�¼������3
 * @warning 
 * @exception 
 */
void EventNorToCut(void)
{
//	    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_NORMALTOCUT);
//	    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_NORMALTOCUT);
    SysBdcastMsg(MSG_GLOBAL_POWER_OFF);
}

/**
 * @brief Ƿѹ����ѹ�¼�����
 * @details ��������Ƿѹ����ѹ�¼�������Ƿѹ����ѹ�¼�
 * @param ��
 * @return ��
 * @retval ��
 * @note Ƿѹ����ѹʱ����
 * @attention ��Դ״̬�¼������4
 * @warning 
 * @exception 
 */
void EventOwnToNor(void)
{
    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_LACKTONORMAL);
    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_LACKTONORMAL);                             
}

/**
 * @brief Ƿѹ����ѹ�¼�����
 * @details ��������Ƿѹ����ѹ�¼�������Ƿѹ����ѹ�¼�
 * @param ��
 * @return ��
 * @retval ��
 * @note Ƿѹ����ѹʱ����
 * @attention ��Դ״̬�¼������5
 * @warning 
 * @exception 
 */
void EventOwnToLow(void)
{
    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_LACKTOLOW);
    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_LACKTOLOW);
}

/**
 * @brief Ƿѹ�������¼�����
 * @details ��������Ƿѹ�������¼�������Ƿѹ�������¼�
 * @param ��
 * @return ��
 * @retval ��
 * @note Ƿѹ������ʱ����
 * @attention ��Դ״̬�¼������6
 * @warning 
 * @exception 
 */
void EventOwnToCut(void)
{
//	    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_LACKTOCUT);
//	    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_LACKTOCUT);
    
    SysBdcastMsg(MSG_GLOBAL_POWER_OFF);
}

/**
 * @brief ��ѹ����ѹ�¼�����
 * @details ���������ѹ����ѹ�¼������͵�ѹ����ѹ�¼�
 * @param ��
 * @return ��
 * @retval ��
 * @note ��ѹ����ѹʱ����
 * @attention ��Դ״̬�¼������7
 * @warning 
 * @exception 
 */
void EventLowToNor(void)
{
    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_LOWTONORMAL);
    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_LOWTONORMAL);
}

/**
 * @brief ��ѹ��Ƿѹ�¼�����
 * @details ���������ѹ��Ƿѹ�¼������͵�ѹ��Ƿѹ�¼�
 * @param ��
 * @return ��
 * @retval ��
 * @note ��ѹ��Ƿѹʱ����
 * @attention ��Դ״̬�¼������8
 * @warning 
 * @exception 
 */
void EventLowToOwn(void)
{
    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_LOWTOLACK);
    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_LOWTOLACK);
    
}

/**
 * @brief ��ѹ�������¼�����
 * @details ���������ѹ�������¼������͵�ѹ�������¼�
 * @param ��
 * @return ��
 * @retval ��
 * @note ��ѹ������ʱ����
 * @attention ��Դ״̬�¼������9
 * @warning 
 * @exception 
 */
void EventLowToCut(void)
{
//	    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_LOWTOCUT);
//	    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_LOWTOCUT);
    
    SysBdcastMsg(MSG_GLOBAL_POWER_OFF);
}

/**
 * @brief �������ѹ�¼�����
 * @details ��������������ѹ�¼������͵������ѹ�¼�
 * @param ��
 * @return ��
 * @retval ��
 * @note �������ѹʱ����
 * @attention ��Դ״̬�¼������10
 * @warning 
 * @exception 
 */
void EventCutToNor(void)
{
//    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_CUTTONORMAL);
//    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_CUTTONORMAL);

    SysBdcastMsg(MSG_GLOBAL_POWER_ON);
    SET_SYSE_PWR_UP;
}

/**
 * @brief �����Ƿѹ�¼�����
 * @details ������������Ƿѹ�¼������͵����Ƿѹ�¼�
 * @param ��
 * @return ��
 * @retval ��
 * @note �����Ƿѹʱ����
 * @attention ��Դ״̬�¼������11
 * @warning 
 * @exception 
 */
void EventCutToOwn(void)
{ 
    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_CUTTOLACK);
    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_CUTTOLACK);

    
    SysBdcastMsg(MSG_GLOBAL_POWER_ON);
    
    SET_SYSE_PWR_UP;
}

/**
 * @brief �������ѹ�¼�����
 * @details ��������������ѹ�¼������͵������ѹ�¼�
 * @param ��
 * @return ��
 * @retval ��
 * @note �������ѹʱ����
 * @attention ��Դ״̬�¼������12
 * @warning 
 * @exception 
 */
void EventCutToLow(void)
{
    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_CUTTOLOW);
    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_CUTTOLOW);

    
    SysBdcastMsg(MSG_GLOBAL_POWER_ON);
    
    SET_SYSE_PWR_UP;
}

/**
 * @brief ��Դ����״̬��ʼ��
 * @details ע���Դ���й���״̬�ṹ��
 * @param ��
 * @return ��
 * @retval ��
 * @note ģ���ʼ����ʱ����
 * @attention 
 * @warning 
 * @exception 
 */
void InitPmmFsm(const ST_FSM_TABLE* pTable)
{
    FSM_Regist(&PmmFsm, pTable);
}

/**
 * @brief δ�ﵽ������ѹʱ�ص�����
 * @details δ�ﵽ������ѹʱ��ʾ15����HELLO
 * @param ��
 * @return ��
 * @retval ��
 * @note ģ���ʼ����ʱ����⵽��ѹδ�ﵽ������ѹ��
 * @attention 
 * @warning 
 * @exception 
 */
void Unstartedcall(void)
{
//	    APP_LcdInitPowCheck();
    SysOptDat(LCDMDL_BUSID_SHOW_STR,0x80,0,7,"LOSTPOR");

}
void Init_SVD(void)
{
	ANAC_SVD_InitTypeDef SVD_InitStruct;

//		svd_it_callback = callBack;
//		AnalogIO( GPIOF, GPIO_Pin_11 );//PF11��ʹ��SVS
	
    HAL_GPIO_PinConfig(GPIOF, 11, 0, 1, 0);
	SVD_InitStruct.SVDMOD = ANAC_SVDCFG_SVDMOD_ALWAYSON;	//���ʹ��Ĥʽ
	SVD_InitStruct.SVDITVL = ANAC_SVDCFG_SVDITVL_15P625MS;	//��Ъ���256ms
	SVD_InitStruct.SVDLVL = ANAC_SVDCFG_SVDLVL_SVS;	//���svs

	SVD_InitStruct.DFEN = ENABLE;		//�������˲� 
	SVD_InitStruct.PFIE = DISABLE;		//�ر�Ƿѹ�ж�
	SVD_InitStruct.PRIE = DISABLE;		//�رչ�ѹ�ж�
	SVD_InitStruct.SVDEN = DISABLE;		//�ر�SVD
	
	ANAC_SVD_Init(&SVD_InitStruct);
	
	ANAC_SVDSIF_PFF_Clr();				//���Ƿѹ��־
	ANAC_SVDCFG_PFIE_Setable(ENABLE);	//SVD��ѹ�����ж�ʹ��
	ANAC_SVDSIF_PRF_Clr();				//�����ѹ��־
	ANAC_SVDCFG_PRIE_Setable(ENABLE);	//SVD��Դ�ָ��ж�ʹ��
	NVIC_DisableIRQ(SVD_IRQn);			//NVIC�жϿ���������
	NVIC_SetPriority(SVD_IRQn,2);
	NVIC_EnableIRQ(SVD_IRQn);	
	
	ANAC_SVDCON_SVDEN_Setable(ENABLE);	//SVDʹ��
}
void Close_SVD(void)
{
	ANAC_SVDSIF_PFF_Clr();				//���Ƿѹ��־
	ANAC_SVDCFG_PFIE_Setable(DISABLE);	//SVD��ѹ�����ж�ʹ��
	ANAC_SVDSIF_PRF_Clr();				//�����ѹ��־
	ANAC_SVDCFG_PRIE_Setable(DISABLE);	//SVD��Դ�ָ��ж�ʹ��
	NVIC_DisableIRQ(SVD_IRQn);			//NVIC�жϿ���������
	
	ANAC_SVDCON_SVDEN_Setable(DISABLE);	//SVDʹ��

}
/**
 * @brief ��Դ��ʼ���м��ȴ�����
 */

//	void 

void InitPwrChk(void)
{
    Init_SVD();
    InitPmmFsm(PowEventTable);
    PmmFsm.CurState = POWUNKNOW;
    SysVol  = 0;
}

int PwrProtect(void)
{
    unsigned char cnt = 15;         //��ʼֵΪ15������ʱ����ֱ�Ӽ���Դ��ѹ
    unsigned long count = 0;
    
//	    Init_SVD();
//	    InitPmmFsm(PowEventTable);
//	    PmmFsm.CurState = POWUNKNOW;
//	    SysVol  = 0;
    //��ȡϵͳ��ѹ��ʼֵ
    
    while(1)
    {
        MCU_DogReset();
        cnt ++;
//	        if(cnt >= 15)               //δ������ʱÿ15s���һ�ε�Դ��ѹ��ʡ��
//	        {
//	            cnt = 0;
            SYS_AD_Scan(AD_ID_MBATVOL, &SysVol);
            LOG_INFO(DBGFMT"@==cur: Vol=%dmV stt=%d ==@\r\n", DBGARG, SysVol, PmmFsm.CurState);

            if(SysVol < SYS_START_VOLT )
            {
                count++;
                if(count < 15)
                {
                    Unstartedcall();    //ϵͳ��Դ��ѹδ�ﵽ������ѹʱִ�к�������ʾ�յ�ط��ţ� 
                }
                else
                {
                    HAL_LcdOff();//APP_LcdOnOff(0);  
//	                    SysTimer_DlyStop(0);
                    SysTimer_DlyStop(1);
                    SysTimer_DlyStop(2);
                    SysTimer_DlyStop(3);
                    SysTimer_DlyStop(4);
                }
                
    //	            SYS_Dev_OptBlinkSet(LED_RUN, 1, 1, 1, 0);
            }
            else
            {
                break;
            }
//	        }
//	        SysTimer_DlyStart(5);
        SYS_Pwr_Sleep();
    }
//	    SysTimer_DlyStop(5);
    
    //�ϵ縴λʱ���ݵ�ǰ��ѹ���õ�ѹ״̬���ϵ緧���߼�
    if(SysVol > PowAlarm.VolOwn)        //��ѹ
    {
        PmmFsm.CurState = POWNOR;
        PmmFsm.PreState = POWNOR;
        APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_ONNORMAL);
        APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_ONNORMAL);
    }
    else if(SysVol > PowAlarm.VolLow)   //Ƿѹ
    {
        PmmFsm.CurState = POWOWN;
        PmmFsm.PreState = POWOWN;
        APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_ONOWE);
        APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_ONOWE);
    }
    else if(SysVol > PowAlarm.VolCut)   //��ѹ
    {
        PmmFsm.CurState = POWLOW;
        PmmFsm.PreState = POWLOW;
        APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_ONLOW);
        APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_ONLOW);
    }
    else                                //����    ----(ʵ��Ӧ�ò��ᷢ��)
    {
        PmmFsm.CurState = POWCUT;
        PmmFsm.PreState = POWCUT;
    }  


    if(count < 3)
    {
        msDelay(1000);//��ѹ3��
        MCU_DogReset();
        msDelay(1000);//��ѹ3��
        MCU_DogReset();
        msDelay(1000);//��ѹ3��
        MCU_DogReset();

    }
//	    s_S_MSK_INT_TYPE sTmType;
//	    sTmType.ucType = SYS_TM_1m;
//	    sTmType.ucPri = 0x0f;  ///���ü���ģ��1ms�Ķ�ʱ�����ȼ�
//	    SMskSetTmFunc(sTmType,&PowManageProcess);
//	    ltid_PowManage = SYS_AskTimeNumber();      //�����Դ��ѹ��ⶨʱ�����
//	    SYS_StartTime(SYS_SUBTASK, ltid_PowManage, 3, PowManageProcess);   //ÿ3������ϵͳ��Դ��ѹ���
    return 1;
}

#ifndef __MODULE__

void SVD_IRQHandler(void)
{
	if( SET == ANAC_SVDSIF_PFF_Chk() )//Ƿѹ
		ANAC_SVDSIF_PFF_Clr();

	if(SET == ANAC_SVDSIF_PRF_Chk())//��ѹ
		ANAC_SVDSIF_PRF_Clr();

    SysSndMsg(D_GENDRV_MDL_NO,1);
}
#else
void SVD_IRQHandlerWithPara(void * para)
{
	if( SET == ANAC_SVDSIF_PFF_Chk() )//Ƿѹ
		ANAC_SVDSIF_PFF_Clr();

	if(SET == ANAC_SVDSIF_PRF_Chk())//��ѹ
		ANAC_SVDSIF_PRF_Clr();

    SysSndMsg(D_GENDRV_MDL_NO,1);

}


#endif


