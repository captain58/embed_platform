/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: frmcfg
**创   建   人: yzy
**最后修改日期: 2013年6月8日
**描        述: 帧解析处理模块配置文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
**创建人: yzy
**版  本: v1.0
**日　期: 2013年6月8日
**描　述: 原始版本
**-------------------------------------------------------------------------------------------------------
** 修改人: 
** 版  本: 
** 描  述: 
********************************************************************************************************/


#ifndef _FRMCFG_H_
#define _FRMCFG_H_


/******************************************************************************
**PST_Frame结构体共同体使能
******************************************************************************/
#define _DLT_FRM_INC    1               //645规约结构体
#define _VS_FRM_INC     1               //
#define _ZY_FRM_INC     1
#define _NETP_FRM_INC     0
#define _HXRF_FRM_INC       1               //海兴RF通信


/******************************************************************************
**通讯规约的配置
******************************************************************************/
#define PST_FRM_VS_EN   1               //威星规约
#define PST_FRM_VS_NO   0

#define PST_FRM_BG_EN   0               //645规约
#define PST_FRM_BG_NO   (PST_FRM_VS_NO + PST_FRM_VS_EN)

#define PST_FRM_ZY_EN   1               //____扩展
#define PST_FRM_ZY_NO   (PST_FRM_BG_NO + PST_FRM_BG_EN)

#define PST_FRM_MB_EN   1               //____扩展
#define PST_FRM_MB_NO   (PST_FRM_ZY_NO + PST_FRM_ZY_EN)

#define PST_FRM_WL_1_EN  1              //全域微功率无线协议1.0
#define PST_FRM_WL_1_NO  (PST_FRM_MB_EN + PST_FRM_MB_NO)
                                        //总的帧类型数量
#define PST_FRM_NO      (PST_FRM_WL_1_EN + PST_FRM_WL_1_NO)


//	typedef enum 
//	{
//	    CID_TXV12   = 0x00,  // 天信V1.2
//	    CID_TXV13   = 0x01,  // 天信V1.3 
//	    CID_TXVMA   = 0x02,  // 天信MODBUS/A
//	    CID_TXVMB   = 0x03,  // 天信MODBUS/B
//	    CID_TXLUX   = 0x04,  // 天信LUX协议
//	    CID_XKVZD   = 0x05,  // 新科自定义
//	    CID_XKVM1   = 0x06,  // 新科MODBUS/1
//	    CID_XKVM2   = 0x07,  // 新科MODBUS/2
//	    CID_CNV12   = 0x08,  // 苍南MODBUS V1.2
//	    CID_CNVZD   = 0x09,  // 苍南自定义
//	    CID_DWVPM2  = 0x0a,  // 德闻MODBUS PTZ-BOX2.0
//	    CID_DWVPM3  = 0x0b,  // 德闻MODBUS PTZ-BOX3.0
//	    CID_ETM1    = 0x0c,  // 埃尔斯特MODBUS(EK-205)
//	    CID_ITRON   = 0x0d,  // 埃创MODBUS
//	    CID_DTZD    = 0x0e,  // 德通自定义
//	    CID_DTM1    = 0x0f,  // 德通MODBUS/1
//	    CID_DWVPM3X = 0x10,  // 德闻MODBUS PTZ-BOX3.0 新协议（long型）
//	    CID_WXFKQ   = 0X11,  // 威星阀门控制器
//	    CID_WXIC    = 0x12,  // 威星IC卡控制器
//	    CID_WXFMG   = 0x13,  // 威星FMG流量计
//	    CID_WXUMU   = 0x14,  // 威星超声波流量计
//	    CID_CNV20   = 0x15,  // 苍南MODBUS V2.0
//	    CID_SDAS    = 0x16,  // 斯达特AS通信协议(AS-25\32\40\50\80)(实为标准Modbus RTU协议)
//	    CID_SKFL    = 0x17,  // 德国西克(SICK)FL500系列
//	    CID_SDM2    = 0x18,  // 斯达特AS通信协议(AS-100\150\200)
//	    CID_TXCPU   = 0x19,  // 天信CPU卡
//	    CID_PT100T  = 0x1A,  // PT100/PT100型温度变送器
//	    CID_PT100P  = 0x1B,  // PT100/PT100型压力变送器
//	}E_DEVICE_PYTE;
// 天信V1.2                                                         
#define				   CID_TXV12    0x00   
// 天信V1.3                                                         
#define				   CID_TXV13    0x01   
// 天信MODBUS/A                                                     
#define				   CID_TXVMA    0x02   
// 天信MODBUS/B                                                     
#define				   CID_TXVMB    0x03   
// 天信LUX协议                                                      
#define				   CID_TXLUX    0x04   
// 新科自定义                                                       
#define				   CID_XKVZD    0x05   
// 新科MODBUS/1                                                     
#define				   CID_XKVM1    0x06   
// 新科MODBUS/2                                                     
#define				   CID_XKVM2    0x07   
// 苍南MODBUS V1.2                                                  
#define				   CID_CNV12    0x08   
// 苍南自定义                                                       
#define				   CID_CNVZD    0x09   
// 德闻MODBUS PTZ-BOX2.0                                            
#define				   CID_DWVPM2   0x0a   
// 德闻MODBUS PTZ-BOX3.0                                            
#define				   CID_DWVPM3   0x0b   
// 埃尔斯特MODBUS(EK-205)                                           
#define				   CID_ETM1     0x0c   
// 埃创MODBUS                                                       
#define				   CID_ITRON    0x0d   
// 德通自定义                                                       
#define				   CID_DTZD     0x0e   
// 德通MODBUS/1                                                     
#define				   CID_DTM1     0x0f   
// 德闻MODBUS PTZ-BOX3.0 新协议（long型）                           
#define				   CID_DWVPM3X  0x10   
// 威星阀门控制器                                                   
#define				   CID_WXFKQ    0X11   
// 威星IC卡控制器                                                   
#define				   CID_WXIC     0x12   
// 威星FMG流量计                                                    
#define				   CID_WXFMG    0x13   
// 威星超声波流量计                                                 
#define				   CID_WXUMU    0x14   
// 苍南MODBUS V2.0                                                  
#define				   CID_CNV20    0x15   
// 斯达特AS通信协议(AS-25\32\40\50\80)(实为标准Modbus RTU协议)      
#define				   CID_SDAS     0x16   
// 德国西克(SICK)FL500系列                                          
#define				   CID_SKFL     0x17   
// 斯达特AS通信协议(AS-100\150\200)                                 
#define				   CID_SDM2     0x18   
// 天信CPU卡                                                        
#define				   CID_TXCPU    0x19   
// PT100/PT100型温度变送器                                          
#define				   CID_PT100T   0x1A
// PT100/PT100型压力变送器    
#define    			   CID_PT100P   0x1B  
//上海信东仪器仪表Modbus
#define    			   CID_XDMA     0x1C  
//上海思达斯易仪器仪表Modbus
#define    			   CID_SYMA     0x1D  
//杭州思筑仪器仪表Modbus
#define    			   CID_SZMA     0x1E  
//中核维思Modbus
#define    			   CID_ZHMA     0x1F 
//胜赛斯Modbus
#define    			   CID_SSMA     0x20 
//哈德斯Modbus
#define    			   CID_HDMA     0x21 
//罗美特Modbus
#define    			   CID_RTMA     0x22 

/*******************************************************************************
通道序号定义
*******************************************************************************/
//	typedef enum
//	{
//	    PST_CHN_SET = 0,             //本地485
//	    PST_CHN_IRD,                 //红外
//	    PST_CHN_TCP,                 //GPRS客户端连接
//	    PST_CHN_NETP,
//	    PST_CHN_DUMB,                //空通道
//	}CHN_ENUM;



#define PST_LEN_MAX     (uint16)0x200   //定义一帧最长为256字节
#define PST_CHN_NO      2               //总的通道数量



#ifdef EXT_DEV_FRM



/*******************************************************************************
**定义通道信息
*******************************************************************************/
#if PST_CHN_NO > 0                      //定义通道0的信息
const uint8 gucs_PSTChn0frms[] =        //定义通道0支持的规约的编号
{
    PST_FRM_VS_NO,
    PST_FRM_ZY_NO,    
};
#define PST_CHN0_HCHARS  6              //定义通道0允许的前导符的数量
#endif

#if PST_CHN_NO > 1                      //定义通道1的信息             
const uint8 gucs_PSTChn1frms[] =        //定义通道1支持的规约的编号   
{                                                                     
    PST_FRM_VS_NO,
    PST_FRM_ZY_NO,    
    PST_FRM_WL_1_NO,
};                                                                    
#define PST_CHN1_HCHARS  6              //定义通道1允许的前导符的数量 
#endif

#if PST_CHN_NO > 2                      //定义通道2的信息             
const uint8 gucs_PSTChn2frms[] =        //定义通道2支持的规约的编号   
{                                                                     
    PST_FRM_VS_NO,
    PST_FRM_ZY_NO,    
};                                                                    
#define PST_CHN2_HCHARS  6              //定义通道2允许的前导符的数量 
#endif

#if PST_CHN_NO > 3                      //定义通道3的信息             
const uint8 gucs_PSTChn3frms[] =        //定义通道3支持的规约的编号   
{                                                                     
    PST_FRM_VS_NO,
    PST_FRM_ZY_NO,    
};                                                                    
#define PST_CHN3_HCHARS  6             //定义通道3允许的前导符的数量 
#endif

#endif              //#ifdef EXT_DEV_FRM
#endif

