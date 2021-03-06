/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: drv_flash.h
**创   建   人: yzy
**最后修改日期: 2018年6月7日
**描        述: mx25L12845E Flash驱动程序配置头文件
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2018年6月7日
** 描　述: 原始版本
**-------------------------------------------------------------------------------------------------------
** 修改人: 
** 版  本: 
** 描  述: 
********************************************************************************************************/

#ifndef _MXFLASH_H_
#define _MXFLASH_H_

#include <stdbool.h>






#define FLS_ERR_EXSIT   (unsigned char)0x10
#define FLS_ERR_READ    (unsigned char)0x20
#define FLS_ERR_WRITE   (unsigned char)0x40
#define FLS_ERR_CSIS    (unsigned char)0x80             //一致性错误:备份区与主存区内容不一致

/*******************************************************************************
**
*******************************************************************************/
typedef struct
{
    unsigned char  x;                   //存储芯片序号
    unsigned long opoffset;            //操作的芯片内数据相对于外部数据的偏移
    unsigned long opaddr;              //操作的片内起始地址
    unsigned long oplen;               //操作的片内长度

}S_FLSINCHIP;


//	/*******************************************************************************
//	** @function_name:  MultiFlashFilterExt
//	** @function_file:  mxflash.c
//	** @描述: 输出多芯片文件系统要操作的芯片参数
//	** 
//	** 
//	** @参数: 
//	** @param: s_out: 数据输出
//	** @param: address(unsigned long): 操作地址
//	** @param: length(unsigned short): 操作数据的长度
//	** 
//	** @返回: 
//	** @return:  unsigned char   
//	** @作者: yzy (2010-05-11)
//	**-----------------------------------------------------------------------------
//	** @修改人: 
//	** @修改说明: 
//	*******************************************************************************/
//	unsigned long MultiFlashFilterExt(S_FLSINCHIP* s_out, unsigned long address, unsigned long length);
//	
//	/*******************************************************************************
//	** @function_name:  FLS_AllErase
//	** @function_file:  mxflash.c
//	** @描述: MX Flash 全片檫除操作
//	** 
//	** 
//	** @参数: 
//	** @param: 
//	** 
//	** @返回: 
//	** @return:  void   
//	** @作者: yzy (2010-05-10)
//	**-----------------------------------------------------------------------------
//	** @修改人: 
//	** @修改说明: 
//	*******************************************************************************/
//	void FLS_AllErase(void);
//	
//	/*******************************************************************************
//	** @function_name:  FLS_SectorErase
//	** @function_file:  mxflash.c
//	** @描述: MX Flash 段檫除操作
//	** 
//	** 
//	** @参数: 
//	** @param: address(unsigned long): 
//	** 
//	** @返回: 
//	** @return:  unsigned char   
//	** @作者: yzy (2010-05-10)
//	**-----------------------------------------------------------------------------
//	** @修改人: 
//	** @修改说明: 
//	*******************************************************************************/
//	unsigned char FLS_SectorErase(unsigned long address);
//	
//	/*******************************************************************************
//	** @function_name:  FLS_BlockErase
//	** @function_file:  mxflash.c
//	** @描述: MX Flash 块64K檫除操作
//	** 
//	** 
//	** @参数: 
//	** @param: address(unsigned long): 
//	** 
//	** @返回: 
//	** @return:  unsigned char   
//	** @作者: yzy (2010-05-10)
//	**-----------------------------------------------------------------------------
//	** @修改人: 
//	** @修改说明: 
//	*******************************************************************************/
//	unsigned char FLS_BlockErase(unsigned long address);
//	
//	
//	/*******************************************************************************
//	** @function_name:  FLS_SectorBlankCheck
//	** @function_file:  mxflash.c
//	** @描述: MX Flash 段空白判断
//	** 
//	** 
//	** @参数: 
//	** @param: address(unsigned long)
//	** 
//	** @返回: 
//	** @return:  bool true:全空,false:非空
//	** @作者: yzy (2012-06-09)
//	**-----------------------------------------------------------------------------
//	** @修改人: 
//	** @修改说明: 
//	*******************************************************************************/
//	bool FLS_SectorBlankCheck(unsigned long address);
//	
//	
//	
//	/*******************************************************************************
//	** @function_name:  FLS_PageBlankCheck
//	** @function_file:  mxflash.c
//	** @描述: MX Flash 页空白判断
//	** 
//	** 
//	** @参数: 
//	** @param: address(unsigned long)
//	** 
//	** @返回: 
//	** @return:  bool true:全空,false:非空
//	** @作者: yzy (2012-06-09)
//	**-----------------------------------------------------------------------------
//	** @修改人: 
//	** @修改说明: 
//	*******************************************************************************/
//	bool FLS_PageBlankCheck(unsigned long address);
//	
//	
//	/*******************************************************************************
//	** @function_name:  FLS_WritePage
//	** @function_file:  mxflash.c
//	** @描述: Flash页编程
//	** 
//	** 
//	** @参数: 
//	** @param: buffer(unsigned char*): 
//	** @param: address(unsigned long): 
//	** @param: length(unsigned short): 
//	** @param: x(unsigned long): 
//	** 
//	** @返回: 
//	** @return:  unsigned char   
//	** @作者: yzy (2010-05-11)
//	**-----------------------------------------------------------------------------
//	** @修改人: 
//	** @修改说明: 
//	*******************************************************************************/
//	unsigned char FLS_WritePage(unsigned char* buffer, unsigned long address, unsigned short length);



/*******************************************************************************
** @function_name:  PCB_FLS_Erase
** @function_file:  mxflash.c
** @描述: MX Flash 檫除操作
** 
** 
** @参数: 
** @param: address: 起始地址
** @param: sectornum 扇区数量
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-10)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char FLS_Erase(unsigned long address, unsigned long sectornum);
unsigned char FLS_BErase(unsigned long address, unsigned long sectornum);

/*******************************************************************************
** @function_name:  PCB_FLS_Read
** @function_file:  mxflash.c
** @描述: 从Flash指定地址读取指定长度的字节数据
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 数据缓存
** @param: address(unsigned long): 地址
** @param: length(unsigned short): 读取的数据长度
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char FLS_Read(unsigned char* buffer, unsigned long address, unsigned short length);


/*******************************************************************************
** 函数名称  void FLS_PreWrite(void)
** 输入  无
** 输出  无
** 说明  预写操作,在写数据之前必须要进行预写操作
** 全局变量:
** 调用模块:
**-----------------------------------------------------------------------------
** 作  者: yzy
** 日　期: 2007年8月8日
*******************************************************************************/
unsigned char FLS_PreWrite(void);


/*******************************************************************************
** @function_name:  PCB_FLS_Write
** @function_file:  mxflash.c
** @描述: Flash 写操作
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 
** @param: address(unsigned long): 
** @param: length(unsigned short): 
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char FLS_Write(unsigned char* buffer, unsigned long address, unsigned short length);

/*******************************************************************************
** @function_name:  PCB_FLS_WriteSpec
** @function_file:  mxflash.c
** @描述: Flash 特殊写操作
** 
** 
** @参数: 
** @param: buffer(unsigned char*): 写入数据的内容缓存
** @param: address(unsigned long): 写入数据的地址
** @param: length(unsigned short): 写入数据的长度
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy
** @修改说明: 尽可能地减少檫除的次数(即:当写入数据与原有数据一致或原有数据为FF时不再擦除)
*******************************************************************************/
unsigned char FLS_WriteSpec(unsigned char* buffer, unsigned long address, unsigned short length);

/*******************************************************************************
** @function_name:  PCB_FLS_Set
** @function_file:  mxflash.c
** @描述: Flash 置数标志
** 
** 
** @参数: 
** @param: templet(unsigned char): 置入的数据
** @param: address(unsigned long): 写入数据的地址
** @param: length(unsigned short): 写入数据的长度
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2010-05-11)
**-----------------------------------------------------------------------------
** @修改人: yzy
** @修改说明: 操作机制同FLS_Write()
*******************************************************************************/
unsigned char FLS_Set(unsigned char templet, unsigned long address, unsigned short length);

/*******************************************************************************
** @function_name:  TestFlashChip
** @function_file:  
** @描述: flash硬件自检
** 
** 
** @参数: 
** 
** @返回: 
** @return:  unsigned char 位图.1:硬件故障,0:硬件正常.
**                 bit0-bit7分别表示第1-8块flash的硬件状态
** @作者: yzy (2018-07-02)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char TestFlashChip(void);

/*******************************************************************************
** @function_name:  PCB_Flash_Init
** @描述: 初始化Flash
** 
** @参数: 
** 
** @返回: 
** @return:  unsigned char   
** @作者: yzy (2018-07-02)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
unsigned char Flash_Init(void);
/*******************************************************************************
** @function_name:  PCB_Flash_Close
** @描述: 关闭Flash，进入低功耗
** 
** @参数: 
** 
** @返回: 
** @return:   
** @作者: yzy (2018-07-02)
**-----------------------------------------------------------------------------
** @修改人: 
** @修改说明: 
*******************************************************************************/
void Flash_Close(void);



#endif

