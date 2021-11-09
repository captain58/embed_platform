/****************************************Copyright (c)*************************
**                               	________科技有限公司
**                                     		开发部
**
**
**--------------文件信息-------------------------------------------------------
**文   件   名: flsmap.h
**创   建   人: yzy
**最后修改日期: 2014年07月25日
**描        述: fls参数配置
**注        意:
**--------------历史版本信息---------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2014年07月25日
** 描　述: 原始版本
******************************************************************************/
#ifndef _FLSMAP_H_
#define _FLSMAP_H_


/******************************************************************************
**片内flash相关宏定义
******************************************************************************/
//boot区定义
#define  FLS_START_ADDR         0x00000000
#define  FLS_ENTRYFUNC_ADDR     0x00000100              //__iar_program_start()指针
#define  FLS_BOOTVER_ADDR       0x00000104              //bootloader版本号

//usr区范围限制
#define  FLS_APPSTART_ADDR      0x08006000
#define  FLS_APPEND_ADDR        0x0007FFFF

//usr区定义
#define  FLS_USRMARKA_ADDR      (FLS_APPSTART_ADDR + 0x200) //用户程序调试程序有效性标志
#define  FLS_USRMARKB_ADDR      (FLS_APPSTART_ADDR + 0x204) //用户程序调试程序有效性标志
#define  FLS_USRPROGLEN_ADDR    (FLS_APPSTART_ADDR + 0x208) //用户程序长度
#define  FLS_USRVER_ADDR        (FLS_APPSTART_ADDR + 0x20C) //用户程序版本号
#define  FLS_USRPRJ_ADDR        (FLS_APPSTART_ADDR + 0x210) //用户工程编码(6字节)

#define FLS_WDTTST_ADDR   32                //存放看门狗测试的上报信息和上报数据(数据长度最大256字节)



#define FLS_CHIP_NO     1
#define FLASH1_SIZE     0x2000000        //32MM
#define FLASH2_SIZE     0x200000        //2M 2010-01-10


#if SYS_FILE_EN > 0
    #define SYS_IAP_EN   1
#else
    #define SYS_IAP_EN   0
#endif

/******************************************************************************
**flash块号宏
******************************************************************************/
//注意:需要与初始化文件一致
#define DB_CODE                 0 

#define DB_UPDATE               1               //升级
//	#define DB_FACTORY              1               //出厂配置

#define DB_PARA                 2               //参数
#define DB_DEV                  3
#define DB_UPDATA_PARA          3               //模块升级参数
#define DB_RUN_PARA             4               //运行参数
#define DB_MON_FREEZE           5               //月冻结
#define DB_EVENT                6               //事件记录
#define DB_USER_DEFINE          7               //用户自定义
#define DB_EVENT_INDEX          8               //事件索引
#define DB_HOUR_FREEZE          9               //小时冻结
#define DB_RECHARGE_INDEX       10              //充值索引
#define DB_RECHARGE             11              //充值事件记录
#define DB_DAY_FREEZE_LAST      12              //最近六条日冻结
#define DB_PARA_NETP            13               //参数
#define DB_CURE                 14                              //电表曲线数据文件号
#define DB_HASH1                15
#define DB_HASH2                16
#ifdef _USR_FLASH                        //应用层主文件中定义

/******************************************************************************
**数据块编号及对应最小数据块数
******************************************************************************/
__root const S_FILEBLOCKS gss_FileBlocks[] = 
{
    //起始文件号, 同类项, 最小数据块数
    
    {DB_CODE,         0, 52, TDB_MODE_RW},    //远程升级
    {DB_UPDATE,         0, 48, TDB_MODE_RW},    //远程升级
//	    {DB_FACTORY,        0, 2, TDB_MODE_RW},     //出厂配置    
    
    {DB_PARA,           0, 2,  TDB_MODE_RW},    //参数
    {DB_DEV,            0, 2,  TDB_MODE_RW},
    {DB_HASH1,            0, 1,  TDB_MODE_RW},    
    {DB_HASH2,            0, 1,  TDB_MODE_RW},    

};

const uint8 LEN_DB_LIST = (sizeof(gss_FileBlocks) / sizeof(S_FILEBLOCKS));

#endif
/******************************************************************************
 *文件以及数据块操作部分
******************************************************************************/
#if (SYS_FILE_EN > 0)
    #define SYS_FILE_DYM_EN  0

    #define DB_LEN_BLOCK(X)    ((X) << 11)     //X段的长度

    #if SYS_FILE_DYM_EN > 0
    #define DB_DYMI_DB          4               //支持动态分配的DB的索引DB,该DB必须支持BACKUP属性
    #define DB_DYM_DB           5               //支持动态分配的DB
    #define DB_DYM_DB_2M        14
    #define DB_DYM_DB_SIZE      0x4000          //16K, 动态块的最小块16k
    #define DB_DYM_DB_NUM       0x40            //64个,
    #define DB_DYM_SIZE         (DB_DYM_DB_SIZE * DB_DYM_DB_NUM)
    #endif
#endif



/******************************************************************************
**EEPROM系统相关
*******************************************************************************/
#define SYS_EEPROM_EN  0


#ifdef    EXT_FILE


#if SYS_EEPROM_EN > 0
    #define SYS_EEPROM_KEYDNO  0x70
    #define SYS_FRAM_CRC     0x10u
    #define SYS_FRAM_CRCBK   0x108u
    #define SYS_FRAM_KEYD    0x200u
    #define SYS_FRAM_KEYDBK  0x1000u
#endif






#endif

//	/************************************************************************ 
//	**参数地址排布宏
//	*************************************************************************/
//	//************************自定义参数***************************************
//	#define F70_PADDR             0        //MAC地址
//	#define F251_PADDR            10       //终端逻辑地址
//	#define F252_PADDR            20       //终端通讯密码
//	#define INPADDR               30       //生产序列号,部规地址
//	#define ZKVADDR               40       //客户化版本(4B HEX)
//	#define K_VCF_PUL             50       //校表时的实时脉冲数
//	#define K_VCF_VOL             60       //校表时的实时电压
//	
//	#define VER_HEXING            70       //采集内测软件版本号


#endif



