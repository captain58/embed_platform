/**
 * @file      dev_pwrchk.h
 * @brief     “电源AD采样”流程应用头文件
 * @details   该头文件定义了电源AD采样的状态、控制结构体及相关接口；
              也定义了电源状态转换和其它保障系统正常运行的特殊电压分界点；
              也定义了其它保障系统正常运行的特殊电压点数量；
              还定义了电源状态切换的回滞电压值，防止电压微小波动造成的误判。
 * @mainpage  流程应用
 * @author    yzy
 * @email     
 * @version   1.0.0
 * @date      2019-3-18
 * @license   captain
 */
#ifndef _PROC_POWER_ADC_H_
#define _PROC_POWER_ADC_H_
/*------------------------------------------------------------------------------------
************************************类型定义******************************************
------------------------------------------------------------------------------------*/

/** 电源状态枚举 */
typedef enum _ET_POW_STA_	
{
	POWUNKNOW = 0,	///<状态机中状态为0时停止判断，所以正式状态从1开始
	POWNOR,			///<电源正常态
	POWOWN,			///<电源欠压态
	POWLOW,			///<电源低压态
	POWCUT,			///<电源掉电态
}ET_POW_STA;
/** 电源状态转换枚举 */

typedef enum _ET_POW_STA_CHG_	
{
	pNorToOwn = 0,	///<正常→欠压
	pNorToLow,		///<正常→低压
	pNorToCut,		///<正常→掉电
	pOwnToNor,		///<欠压→正常
	pOwnToLow,		///<欠压→低压
	pOwnToCut,		///<欠压→掉电
	pLowToNor,		///<低压→正常
	pLowToOwn,		///<低压→欠压
	pLowToCut,		///<低压→掉电
	pCutToNor,		///<掉电→正常
	pCutToOwn,		///<掉电→欠压
	pCutToLow,		///<掉电→低压
}ET_POW_STA_CHG;

/** 
 * @brief 特殊电压点注册结构体
 * @details 特殊电压点注册结构体，含当前电压状态指针，当前状态
 */
typedef struct _ST_SPC_VOL_PNT_		
{
	unsigned char   *State;		///<回传状态指针
	unsigned char   Cursta;		///<当前状态
	unsigned char   Presta;		///<上一次状态
	unsigned char   SetTimes;	///<设定比较次数
	unsigned char   SameTimes;	///<运行中比较结果相同的次数
	unsigned short  Voltage;	///<注册比较电压点
}ST_SPC_VOL_PNT;

/** 电源电压状态转换点,单位mV */
typedef struct _ST_POW_ALM_PNT_		
{
	unsigned short  VolOwn;		///<欠压点 
	unsigned short  VolLow;		///<低压点
	unsigned short  VolCut;		///<掉电点
}ST_POW_ALM_PNT;
/** 电源状态事件表结构体 */
typedef struct _ST_FSM_TABLE
{
    short           CurState;			///> 当前状态
    short           Event;				///> 事件
    short           EventActID;//void            (*EventAct)();		///> 函数指针
    short           NextState;			///> 下一个状态
}ST_FSM_TABLE, *P_ST_FSM_TABLE;

/** 电源状态变换管理结构体 */
typedef struct _ST_FSM
{
	short               CurState;		///> 当前状态
	short               PreState;		///> 前一状态
	const ST_FSM_TABLE  *FsmTable;		///> 状态事件组指针
}ST_FSM, *P_ST_FSM;
/*------------------------------------------------------------------------------------
*************************************宏定义*******************************************
------------------------------------------------------------------------------------*/
#define SYS_START_VOLT			4000///<系统启动电压，单位mV（4节碱电适用）,超过此电压点表具才会开始加载表具参数进行初始化，

#define POWVOL_FUL				5800///<主电满电显示电压点,单位mV
#define POWVOL_OWN				4800//5200///<主电欠压转换电压点,单位mv
#define POWVOL_LOW				4570//5000/<主电低压转换电压点,单位mv
#define POWVOL_CUT				3600//1000///<主电掉电转换电压点,单位mv

#define HYSTERESISVOL			100	///<100mV，回滞电压 

#ifndef ABOVE
    #define ABOVE				1
    #define BELOW				0
#endif


/*------------------------------------------------------------------------------------
****************************全局使用变量、接口函数申明********************************
------------------------------------------------------------------------------------*/
/**
 * @brief 电源初始运行监测等待函数
 * @details 初始化电源模块硬件接口及软件初始状态，读取外电源电压值，若未达到系统启动电压
 *      值，显示“HELLO”15秒提示，达到系统启动电压，发送上电电源事件（上电常压、上电欠
 *      压、上电低压），最后启动每3秒运行一次的电源电压检测任务后，结束。
 * @param 无参数
 * @return 返回后，表示外供电电压正常，等待完毕
 * @retval 返回1，成功
 * @note 上电运行时优先调用
 * @attention 在调用存储接口前，需要先调用本接口，保证供电正常。
 * @warning 默认系统启动电压点适用于4节碱电情形，与首次上电超级电容充电相关，不要随意改。
 * @exception 若外电电压异常，将显示HELLO提示。
 */
int InitSysPowManage(void);

/**
 * @brief 获取系统外部电源状态接口
 * @details 获取系统电源当前状态，含未知、常压、欠压、低压和掉电。
 * @param 无参数
 * @return 直接返回枚举结构体_ET_POW_STA的枚举状态
 * @retval 0未知，1常压、2欠压、3低压、4掉电
 * @note 立即返回
 * @attention 
 * @warning 
 * @exception 
 */
ET_POW_STA GetSysPowSta(void);

/**
 * @brief 查询系统外部电源电压值接口
 * @details 获取系统外部电源当前电压值，电压值通过AD转换值、线路板电阻分压比运算而成。
 * @param 无参数
 * @return 直接返回电压值，单位mV
 * @retval 返回值为短整型，如3001表示3.001V
 * @note 立即返回
 * @attention 
 * @warning 
 * @exception 
 */
unsigned short GetSysPow(void);
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
void PowManageProcess(void);


void SVD_IRQHandlerWithPara(void * para);
void InitPwrChk(void);
int PwrProtect(void);
void Init_SVD(void);
void Close_SVD(void);

#endif
