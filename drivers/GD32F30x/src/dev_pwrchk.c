/**
 * @file dev_pwrchk.c
 * @brief “电源AD采样”流程应用源文件
 * @details 本模块实现了电源初始运行监测等待函数，在上电时优先调用，保障系统外接电源
 *          供电正常，超级电容充电一定程度后才正常运行表具系统；也实现了系统正常运行
 *          后每3秒检测一次电源电压，与预先定义的状态转换点电压比较，发现有状态变化就
 *          调用处理函数，发送电源状态变化事件进行处理，达到控制表具正常运行目的。另
 *          外，本模块还提供了自定义电压点监测功能和电源相关状态、电压值查询接口。
 * @mainpage 工程概览
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
****************************全局变量、常量定义及函数申明****************************
----------------------------------------------------------------------------------*/
static uint32_t SysVol = 0;			///< 系统采样电源电压 
ST_FSM PmmFsm;						      	///< 电源状态变换管理器
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
 * @brief 电源电压状态转换点常量结构体 
 * @details 结构体成员欠压点VolOwn初始化为POWVOL_OWN、低压点VolLow初始化为POWVOL_LOW、
 *          掉电点VolCut初始化为POWVOL_CUT。 
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
 * @brief 电源状态事件表
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
	{0, 		0,		    0xffff,		            0           },      //最后一行必须
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
//		{0, 		0,		    0,		            0           },      //最后一行必须
//	};


void * EventFunc(uint16_t id);


/*-----------------------------------------------------------------------------------
*******************************函数功能实现******************************************
-----------------------------------------------------------------------------------*/
/**
 * @brief 电源状态切换注册函数
 * @details 向电源状态运行管理结构体实例中传递预定义的电源状态事件表。
 * @param pFsm,电源状态运行管理结构体实例
 * @param *pTable,预定义的电源状态事件表
 * @return 无
 * @retval 无
 * @note 直接返回
 * @attention 在状态切换运行前先调用注册函数进行预定义赋值。
 * @warning 无
 * @exception 无
 */
void FSM_Regist(ST_FSM* pFsm, const ST_FSM_TABLE* pTable)
{
    pFsm->FsmTable = pTable;
}
/**
 * @brief 电源状态转换运行函数
 * @details 通过当前状态和当前事件两个条件判断，根据预定义的电源状态事件表进行状态切换。
 * @param pFsm电源状态运行管理结构体实例
 * @param event当前事件
 * @return 无
 * @retval 无
 * @note 直接返回
 * @attention 运行本接口根据事件进行电源状态切换。
 * @warning 无
 * @exception 无
 */
void FSM_EventHandle(ST_FSM* pFsm, int event)
{
    unsigned char i;
    const ST_FSM_TABLE *FsmTable;

    FsmTable = pFsm->FsmTable;
    if(FsmTable == 0)   //判断数组指针有效性
    {
        return;
    }
    for(i = 0; ; i++)
	{
	    if(FsmTable[i].CurState != 0)
        {   
            if((FsmTable[i].Event == event) && (FsmTable[i].CurState == pFsm->CurState))  //两个条件都符合才进行状态切换 
    		{
    		    pFsm->CurState = FsmTable[i].NextState;     //状态更改 
                pFsm->PreState = pFsm->CurState;            //状态更改，对下一次检查来说，PreState就是CurState
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
 * @brief 获取系统外部电源状态接口
 */
ET_POW_STA GetSysPowSta(void)
{
    return (ET_POW_STA)PmmFsm.CurState;
}

/**
 * @brief 查询系统外部电压值接口
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
 * @brief 获取电压状态变化时需要检测的次数
 * @details 根据当前电压状态和目标状态，给出状态变化时需要AD检测的次数
 * @param tempSta 目标电源电压状态
 * @param curSta 当前电源电压状态
 * @return 返回需要AD检测的次数
 * @retval 1或3
 * @note 固定返回
 * @attention 电压向上变化时只检测一次，电压向下变化时，如果是掉电，检测一次，否则检测3次。
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
 * @brief 电源管理进程
 * @details 每3秒运行一次，检测外部电源电压值，一是将电压值与特殊电压点进行比较判断，再更新特殊电压
 *          点检测状态；二是将电压值与系统电源状态电压点进行比较判断，确认当前状态及所发生的电源事件
 *          最后调用电源状态转换运行函数，转换电源状态并运行电源事件处理函数，发出电源事件。
 * @param 无
 * @return 无
 * @retval 无
 * @note 作为低功耗定时任务运行
 * @attention 
 * @warning 
 * @exception 
 */
void PowManageProcess(void)
{
    static unsigned char    IsSame  = 0;    //比较状态相同的次数，设定为3次
    unsigned char           temp;
    uint8_t diotstt = 0;
//	    SysVol = PCB_VBatVoltGet();
    SYS_AD_Scan(AD_ID_MBATVOL, &SysVol);

    /* 
     * 系统电压状态处理
     * 判断当前采样电压状态，增加电压回滞，回滞区间内不处理（ 相邻电压状态考虑回滞）
     * 采到连续3次同样的电压状态，且与当前电压状态不一致才进行电压状态转换 
     */  
    temp = PmmFsm.CurState;     //temp赋初始状态为当前状态，防止GPRS上告时不检测电压变化获得错误电压状态
    SysOptDat(DIOTMDL_BUSID_STT,0,0,1,&diotstt);
    
    LOG_DEBUG(DBGFMT"@==cur: Vol=%dmV stt=%d ==@\r\n", DBGARG, SysVol, PmmFsm.CurState);

    switch(PmmFsm.CurState)
    {
    case POWNOR:
        if((SysVol <= (PowAlarm.VolOwn - HYSTERESISVOL)) && (SysVol > PowAlarm.VolLow))
        {
//	            if((IotInfo.IsIotUpcoming() == 1) || (IotInfo.IsNeedWaiting() == 1))//上告时或者上告结束等待电压恢复时忽略掉电外的电压检测
            if(diotstt)
            {
                break;
            }
            LOG_DEBUG("\r\n@==常压转欠压: Vol=%dmV ==@\r\n", SysVol);
            temp = POWOWN;
        } 
        else if((SysVol <= PowAlarm.VolLow) && (SysVol > PowAlarm.VolCut))
        {
//	            if((IotInfo.IsIotUpcoming() == 1) || (IotInfo.IsNeedWaiting() == 1))//上告时或者上告结束等待电压恢复时忽略掉电外的电压检测
            if(diotstt)
            {
                break;
            }
            LOG_DEBUG("\r\n@==常压转低压: Vol=%dmV ==@\r\n", SysVol);
            temp = POWLOW;
        }
        else if(SysVol <= PowAlarm.VolCut)
        {
			LOG_INFO("\r\n@==常压转掉电: Vol=%dmV ==@\r\n", SysVol);
            temp = POWCUT;
        }
        else    //回滞区   
        {
            temp = PmmFsm.CurState;
        }
        break;
    case POWOWN:
        if(SysVol > (PowAlarm.VolOwn + HYSTERESISVOL))
        {
            LOG_DEBUG("\r\n@==欠压转正常: Vol=%dmV ==@\r\n", SysVol);
            temp = POWNOR;
        }
        else if( (SysVol <= (PowAlarm.VolLow - HYSTERESISVOL)) && (SysVol > PowAlarm.VolCut))
        {
//	            if((IotInfo.IsIotUpcoming() == 1) || (IotInfo.IsNeedWaiting() == 1))//上告时或者上告结束等待电压恢复时忽略掉电外的电压检测
            
            if(diotstt)
            {
                break;
            }
            LOG_DEBUG("\r\n@==欠压转低压: Vol=%dmV ==@\r\n", SysVol);
            temp = POWLOW;
        }
        else if(SysVol <= PowAlarm.VolCut)
        {
			LOG_INFO("\r\n@==欠压转掉电: Vol=%dmV ==@\r\n", SysVol);
            temp = POWCUT;
        }
        else    //回滞区
        {
            temp = PmmFsm.CurState;
        }
        break;
    case POWLOW:
        if(SysVol > PowAlarm.VolOwn)
        {
        
            LOG_DEBUG("\r\n@==低压转正常: Vol=%dmV ==@\r\n", SysVol);
            temp = POWNOR;
        }
        else if(SysVol > (PowAlarm.VolLow + HYSTERESISVOL))
        {
            
            LOG_DEBUG("\r\n@==低压转欠压: Vol=%dmV ==@\r\n", SysVol);
            temp = POWOWN;
        }
        else if(SysVol <= (PowAlarm.VolCut - HYSTERESISVOL))
        {
        
            LOG_INFO("\r\n@==低压转掉电: Vol=%dmV ==@\r\n", SysVol);
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
            LOG_INFO("\r\n@==掉电转正常: Vol=%dmV ==@\r\n", SysVol);
            temp = POWNOR;
        }
        else if(SysVol > PowAlarm.VolLow)
        {
            LOG_INFO("\r\n@==掉电转欠压: Vol=%dmV ==@\r\n", SysVol);
            temp = POWOWN;
        }
        else if(SysVol > PowAlarm.VolCut + HYSTERESISVOL)
        {
            LOG_INFO("\r\n@==掉电转低压: Vol=%dmV ==@\r\n", SysVol);
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
		if(IsSame >= GetChkTime(temp, PmmFsm.CurState)) //累计次数超过设定的检测次数
		{
		   IsSame = 0;
		   FSM_EventHandle(&PmmFsm, temp);   //系统电压改变，执行状态转换机
		}
    }
    else
    {
        IsSame  = 0;
    }
}

/**
 * @brief 常压→欠压事件函数
 * @details 互斥清理常压→欠压事件，发送常压→欠压事件
 * @param 无
 * @return 无
 * @retval 无
 * @note 常压→欠压时运行
 * @attention 电源状态事件表序号1
 * @warning 
 * @exception 
 */
void EventNorToOwn(void)
{
    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_NORMALTOLACK);
    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_NORMALTOLACK);
}

/**
 * @brief 常压→低压事件函数
 * @details 互斥清理常压→低压事件，发送常压→低压事件
 * @param 无
 * @return 无
 * @retval 无
 * @note 常压→低压时运行
 * @attention 电源状态事件表序号2
 * @warning 
 * @exception 
 */
void EventNorToLow(void)
{
    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_NORMALTOLOW);
    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_NORMALTOLOW);
}

/**
 * @brief 常压→掉电事件函数
 * @details 互斥清理常压→掉电事件，发送常压→掉电事件
 * @param 无
 * @return 无
 * @retval 无
 * @note 常压→掉电时运行
 * @attention 电源状态事件表序号3
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
 * @brief 欠压→常压事件函数
 * @details 互斥清理欠压→常压事件，发送欠压→常压事件
 * @param 无
 * @return 无
 * @retval 无
 * @note 欠压→常压时运行
 * @attention 电源状态事件表序号4
 * @warning 
 * @exception 
 */
void EventOwnToNor(void)
{
    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_LACKTONORMAL);
    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_LACKTONORMAL);                             
}

/**
 * @brief 欠压→低压事件函数
 * @details 互斥清理欠压→低压事件，发送欠压→低压事件
 * @param 无
 * @return 无
 * @retval 无
 * @note 欠压→低压时运行
 * @attention 电源状态事件表序号5
 * @warning 
 * @exception 
 */
void EventOwnToLow(void)
{
    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_LACKTOLOW);
    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_LACKTOLOW);
}

/**
 * @brief 欠压→掉电事件函数
 * @details 互斥清理欠压→掉电事件，发送欠压→掉电事件
 * @param 无
 * @return 无
 * @retval 无
 * @note 欠压→掉电时运行
 * @attention 电源状态事件表序号6
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
 * @brief 低压→常压事件函数
 * @details 互斥清理低压→常压事件，发送低压→常压事件
 * @param 无
 * @return 无
 * @retval 无
 * @note 低压→常压时运行
 * @attention 电源状态事件表序号7
 * @warning 
 * @exception 
 */
void EventLowToNor(void)
{
    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_LOWTONORMAL);
    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_LOWTONORMAL);
}

/**
 * @brief 低压→欠压事件函数
 * @details 互斥清理低压→欠压事件，发送低压→欠压事件
 * @param 无
 * @return 无
 * @retval 无
 * @note 低压→欠压时运行
 * @attention 电源状态事件表序号8
 * @warning 
 * @exception 
 */
void EventLowToOwn(void)
{
    APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_LOWTOLACK);
    APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_LOWTOLACK);
    
}

/**
 * @brief 低压→掉电事件函数
 * @details 互斥清理低压→掉电事件，发送低压→掉电事件
 * @param 无
 * @return 无
 * @retval 无
 * @note 低压→掉电时运行
 * @attention 电源状态事件表序号9
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
 * @brief 掉电→常压事件函数
 * @details 互斥清理掉电→常压事件，发送掉电→常压事件
 * @param 无
 * @return 无
 * @retval 无
 * @note 掉电→常压时运行
 * @attention 电源状态事件表序号10
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
 * @brief 掉电→欠压事件函数
 * @details 互斥清理掉电→欠压事件，发送掉电→欠压事件
 * @param 无
 * @return 无
 * @retval 无
 * @note 掉电→欠压时运行
 * @attention 电源状态事件表序号11
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
 * @brief 掉电→低压事件函数
 * @details 互斥清理掉电→低压事件，发送掉电→低压事件
 * @param 无
 * @return 无
 * @retval 无
 * @note 掉电→低压时运行
 * @attention 电源状态事件表序号12
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
 * @brief 电源管理状态初始化
 * @details 注册电源运行管理状态结构体
 * @param 无
 * @return 无
 * @retval 无
 * @note 模块初始运行时调用
 * @attention 
 * @warning 
 * @exception 
 */
void InitPmmFsm(const ST_FSM_TABLE* pTable)
{
    FSM_Regist(&PmmFsm, pTable);
}

/**
 * @brief 未达到启动电压时回调函数
 * @details 未达到启动电压时显示15秒钟HELLO
 * @param 无
 * @return 无
 * @retval 无
 * @note 模块初始运行时并检测到电压未达到启动电压点
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
//		AnalogIO( GPIOF, GPIO_Pin_11 );//PF11，使用SVS
	
    HAL_GPIO_PinConfig(GPIOF, 11, 0, 1, 0);
	SVD_InitStruct.SVDMOD = ANAC_SVDCFG_SVDMOD_ALWAYSON;	//间隔使能膜式
	SVD_InitStruct.SVDITVL = ANAC_SVDCFG_SVDITVL_15P625MS;	//间歇间隔256ms
	SVD_InitStruct.SVDLVL = ANAC_SVDCFG_SVDLVL_SVS;	//检测svs

	SVD_InitStruct.DFEN = ENABLE;		//打开数字滤波 
	SVD_InitStruct.PFIE = DISABLE;		//关闭欠压中断
	SVD_InitStruct.PRIE = DISABLE;		//关闭过压中断
	SVD_InitStruct.SVDEN = DISABLE;		//关闭SVD
	
	ANAC_SVD_Init(&SVD_InitStruct);
	
	ANAC_SVDSIF_PFF_Clr();				//清除欠压标志
	ANAC_SVDCFG_PFIE_Setable(ENABLE);	//SVD电压跌落中断使能
	ANAC_SVDSIF_PRF_Clr();				//清除过压标志
	ANAC_SVDCFG_PRIE_Setable(ENABLE);	//SVD电源恢复中断使能
	NVIC_DisableIRQ(SVD_IRQn);			//NVIC中断控制器配置
	NVIC_SetPriority(SVD_IRQn,2);
	NVIC_EnableIRQ(SVD_IRQn);	
	
	ANAC_SVDCON_SVDEN_Setable(ENABLE);	//SVD使能
}
void Close_SVD(void)
{
	ANAC_SVDSIF_PFF_Clr();				//清除欠压标志
	ANAC_SVDCFG_PFIE_Setable(DISABLE);	//SVD电压跌落中断使能
	ANAC_SVDSIF_PRF_Clr();				//清除过压标志
	ANAC_SVDCFG_PRIE_Setable(DISABLE);	//SVD电源恢复中断使能
	NVIC_DisableIRQ(SVD_IRQn);			//NVIC中断控制器配置
	
	ANAC_SVDCON_SVDEN_Setable(DISABLE);	//SVD使能

}
/**
 * @brief 电源初始运行监测等待函数
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
    unsigned char cnt = 15;         //初始值为15，生产时可以直接检测电源电压
    unsigned long count = 0;
    
//	    Init_SVD();
//	    InitPmmFsm(PowEventTable);
//	    PmmFsm.CurState = POWUNKNOW;
//	    SysVol  = 0;
    //获取系统电压初始值
    
    while(1)
    {
        MCU_DogReset();
        cnt ++;
//	        if(cnt >= 15)               //未上主电时每15s检测一次电源电压，省电
//	        {
//	            cnt = 0;
            SYS_AD_Scan(AD_ID_MBATVOL, &SysVol);
            LOG_INFO(DBGFMT"@==cur: Vol=%dmV stt=%d ==@\r\n", DBGARG, SysVol, PmmFsm.CurState);

            if(SysVol < SYS_START_VOLT )
            {
                count++;
                if(count < 15)
                {
                    Unstartedcall();    //系统电源电压未达到启动电压时执行函数（显示空电池符号） 
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
    
    //上电复位时根据当前电压设置电压状态和上电阀门逻辑
    if(SysVol > PowAlarm.VolOwn)        //常压
    {
        PmmFsm.CurState = POWNOR;
        PmmFsm.PreState = POWNOR;
        APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_ONNORMAL);
        APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_ONNORMAL);
    }
    else if(SysVol > PowAlarm.VolLow)   //欠压
    {
        PmmFsm.CurState = POWOWN;
        PmmFsm.PreState = POWOWN;
        APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_ONOWE);
        APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_ONOWE);
    }
    else if(SysVol > PowAlarm.VolCut)   //低压
    {
        PmmFsm.CurState = POWLOW;
        PmmFsm.PreState = POWLOW;
        APP_EventMutexClean(EVT_TYPE_COMMPROC, APP_EVT_POW_ONLOW);
        APP_EventWrite(EVT_TYPE_COMMPROC, APP_EVT_POW_ONLOW);
    }
    else                                //掉电    ----(实际应该不会发生)
    {
        PmmFsm.CurState = POWCUT;
        PmmFsm.PreState = POWCUT;
    }  


    if(count < 3)
    {
        msDelay(1000);//稳压3秒
        MCU_DogReset();
        msDelay(1000);//稳压3秒
        MCU_DogReset();
        msDelay(1000);//稳压3秒
        MCU_DogReset();

    }
//	    s_S_MSK_INT_TYPE sTmType;
//	    sTmType.ucType = SYS_TM_1m;
//	    sTmType.ucPri = 0x0f;  ///设置计量模块1ms的定时的优先级
//	    SMskSetTmFunc(sTmType,&PowManageProcess);
//	    ltid_PowManage = SYS_AskTimeNumber();      //申请电源电压检测定时器编号
//	    SYS_StartTime(SYS_SUBTASK, ltid_PowManage, 3, PowManageProcess);   //每3秒启动系统电源电压检测
    return 1;
}

#ifndef __MODULE__

void SVD_IRQHandler(void)
{
	if( SET == ANAC_SVDSIF_PFF_Chk() )//欠压
		ANAC_SVDSIF_PFF_Clr();

	if(SET == ANAC_SVDSIF_PRF_Chk())//过压
		ANAC_SVDSIF_PRF_Clr();

    SysSndMsg(D_GENDRV_MDL_NO,1);
}
#else
void SVD_IRQHandlerWithPara(void * para)
{
	if( SET == ANAC_SVDSIF_PFF_Chk() )//欠压
		ANAC_SVDSIF_PFF_Clr();

	if(SET == ANAC_SVDSIF_PRF_Chk())//过压
		ANAC_SVDSIF_PRF_Clr();

    SysSndMsg(D_GENDRV_MDL_NO,1);

}


#endif


