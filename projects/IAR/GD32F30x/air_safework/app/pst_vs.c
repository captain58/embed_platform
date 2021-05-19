/****************************************Copyright (c)**************************************************
**                                  ____科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: pst_bg.c
**创   建   人: yzy
**最后修改日期: 2017年9月1日
**描        述: 通讯模块645规约处理文件包括主动通讯和被动通讯
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: yzy
** 版  本: v1.0
** 日　期: 2017年9月1日
** 描　述: 原始版本
********************************************************************************************************/


#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"



/******************************************************************
**全C地址
******************************************************************/
const uint8 gucs_VSALLAddr[][6] =
{
    {0x99, 0x99, 0x99, 0x99, 0x99, 0x99}, 
    {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA},
    {0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB},
    {0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC},
    {0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD},
    {0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE},
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
};


/*********************************************************************************************************
**函数名称  uint8 VS_Decode(uint8* receive, DLT_Frame* frm, uint8* addr)
**输入  receive 接收到数据的缓存        
        code    数据解析成的结构体
        addr    需要回复的表地址
**输出  PST_ERR_OK
**      PST_ERR_FT
**说明  主动接收到的部规帧的解包函数
** 全局变量:
** 调用模块:
**
**------------------------------------------------------------------------------------------------------
** 作  者: yzy
** 日　期: 2017年9月1日
** 内  容: 
********************************************************************************************************/
uint8 VS_Decode(uint8* receive, DLT_Frame* frm, uint8* addr)
{
    if(addr != __NULL)                  //需要判断地址
    {
                                        //判断接收到的地址是否和要求的一致
        if(CmpBuffer(addr, receive + 1, 6) != 0)
        {
            return PST_ERR_FT;          //返回发生错误
        }
    }
    frm->cmd = receive[8];              //命令字
    frm->len = receive[9];              //长度
    frm->recv = receive + 10;           //接收到的数据的数据区
                                        //循环减33
    for(uint32 uc_i = 0; uc_i < frm->len; uc_i ++)
    {
        receive[uc_i + 10] -= 0x33;     //减33操作
    }

    frm->wd.lword = 0;                  //所有的数据标识清0操作

    if(frm->len > 2)                    //如果长度大于就有数据标识
    {
        frm->wd.byte[0] = receive[10];  //低字节
        frm->wd.byte[1] = receive[11];  //高字节        
        if(frm->len > 4)                //如果长度大于4就可能有扩展数据标识
        {
                                        //低字节
            frm->wd.byte[2] = receive[12];  
                                        //高字节
            frm->wd.byte[3] = receive[13];  
        }
    }
    SYS_OK();
}

/*********************************************************************************************************
**函数名称  uint8 VS_Pack(PST_Frame* frm, uint8* addr, uint16* length)
**输入  frm 打包的结构体
        addr 打包的地址            
        length 输入为打包缓存的最大长度，输出为打包好之后的长度
**输出  PST_ERR_OK
        PST_ERR_VAR
**      PST_ERR_FT
**说明  对645帧进行打包，主动通讯
** 全局变量:
** 调用模块:
**
**------------------------------------------------------------------------------------------------------
** 作  者: yzy
** 日　期: 2017年9月1日
** 内  容: 
********************************************************************************************************/
uint8 VS_Pack(DLT_Frame* frm, uint8* addr, uint16* length)
{
   frm->send[8] = frm->cmd;             //命令字
   frm->send[9] = frm->len;             //长度

   if(frm->wd.word[0] > 0)              //如果存在数据标志
   {
       frm->send[10] = frm->wd.byte[0]; //字节0
       frm->send[11] = frm->wd.byte[1]; //字节1
       if(frm->wd.word[1] < 0xFFFF)     //存在扩展标志
       {
           if(frm->atr & PST_FRM_ATRWR )//判断是否是写数据
           {
                                        //写数据的话扩展数据标志填写在这里
               frm->send[16] = frm->wd.byte[2];
               frm->send[17] = frm->wd.byte[3];
           }
           else
           {                            //否则就是填写在数据标识之后
               frm->send[12] = frm->wd.byte[2];
               frm->send[13] = frm->wd.byte[3];
           }
       }
   }
                                        //+33操作
   for(uint8 uc_i = 0; uc_i < frm->send[9]; uc_i ++)
   {
       frm->send[uc_i + 10] += 0x33;
   }
                                        //打包操作
   *length = PST_DoneFrame(PST_FRM_VS_NO, frm->send);

   if(*length > 0)                      //打包长度>0
   {
       if(addr != __NULL)               //需要传输地址
       {
           MoveBuffer(                  //转移地址中的数据
               addr, 
               frm->send + 1, 
               6
               );

           frm->send[*length - 2] = DoSum(frm->send, *length - 2);
       }
       
       SYS_OK();
   }
   return PST_ERR_FT;                   //返回发生致命错误
}   


/*********************************************************************************************************
**函数名称  uint8 XVS_Pack(PST_Frame* frm, uint8* addr, uint16* length)
**输入  frm 打包的结构体
        addr 打包的地址            
        length 输入为打包缓存的最大长度,输出为打包好之后的长度
**输出  PST_ERR_OK
        PST_ERR_VAR
**      PST_ERR_FT
**说明  对645-2007帧进行打包,主动通讯
** 全局变量:
** 调用模块:
**
**------------------------------------------------------------------------------------------------------
** 作  者: yzy
** 日　期: 2018年3月22日
** 内  容: 针对645-2007的打包
********************************************************************************************************/
uint8 XVS_Pack(DLT_Frame* frm, uint8* addr, uint16* length)
{
   frm->send[8] = frm->cmd;             //命令字
   frm->send[9] = frm->len;             //长度

   if(frm->wd.lword > 0)                //如果存在数据标志
   {
       frm->send[10] = frm->wd.byte[0]; //字节0
       frm->send[11] = frm->wd.byte[1]; //字节1
       frm->send[12] = frm->wd.byte[2]; //字节2
       frm->send[13] = frm->wd.byte[3]; //字节3
   }
                                        //+33操作
   for(uint8 uc_i = 0; uc_i < frm->send[9]; uc_i ++)
   {
       frm->send[uc_i + 10] += 0x33;
   }
                                        //打包操作
   *length = PST_DoneFrame(PST_FRM_VS_NO, frm->send);

   if(*length > 0)                      //打包长度>0
   {
       if(addr != __NULL)               //需要传输地址
       {
           MoveBuffer(                  //转移地址中的数据
               addr, 
               frm->send + 1, 
               6
               );

           frm->send[*length - 2] = DoSum(frm->send, *length - 2);
       }
       
       SYS_OK();
   }
   return PST_ERR_FT;                   //返回发生致命错误
}   






/*********************************************************************************************************
**函数名称  uint8 PST_VS_Decode(uint8* receive, uint8* send, uint16 sbuflen, PST_Frame* code)
**输入  receive 接收到数据的缓存
        send    需要发送的数据的缓存
        code    数据解析成的结构体
**输出  PST_ERR_OK
**      PST_ERR_FT
**说明  判断当前缓存里面是否为合法帧、如果是的话则解析成为被动通讯结构体
** 全局变量:
** 调用模块:
**
**------------------------------------------------------------------------------------------------------
** 作  者: yzy
** 日　期: 2017年9月1日
** 内  容: 
********************************************************************************************************/
uint8 PST_VS_Decode(uint8* receive, uint8* send, uint16 sbuflen, PST_Frame* frm)
{
    frm->_dlt.recv = receive;                //接收到的数据
    frm->_dlt.send = send;                   //发送数据缓存的起始地址
    frm->_dlt.cmd = receive[8];              //控制字
    frm->_dlt.len = receive[9] + 12;         //长度
    frm->_dlt.wd.byte[0] = send[10] - 0x33;  //数据标识字节0
    frm->_dlt.wd.byte[1] = send[11] - 0x33;  //数据标识字节1

    SYS_OK();                            //让数据通过
}


/*********************************************************************************************************
**函数名称  uint8 PST_VS_Proc(PST_Frame* frm)
**输入  
        code    数据解析成的结构体
**输出  PST_ERR_OK
**      PST_ERR_FT
**说明  将数据透明转发给内部
** 全局变量:
** 调用模块:
**
**------------------------------------------------------------------------------------------------------
** 作  者: yzy
** 日　期: 2017年9月1日
** 内  容: 
********************************************************************************************************/
uint8 PST_VS_Proc(PST_Frame* frm)
{
    uint16 len;
    frm->_dlt.cmd = 0xC1;
    frm->_dlt.len = 0x01;
    frm->_dlt.wd.lword = 0;
    frm->_dlt.send[10] = 0x02;
    VS_Pack(&frm->_dlt, gs_PstPara.Addr, &len);
                                        //发送数据
    gfs_PSTChnSend[frm->chninfo & PST_FRM_CHNNO]
    (
        frm->_dlt.send,
        len
    );

    SYS_OK();
}



/*******************************************************************************
 * @function_name:  WaitForHdlc
 * @function_file:  
 * @描述: 等待HDLC帧的到来并处理
 * 
 * 
 * @参数: buffer 缓存
 * @参数: size 缓存大小
 * @参数: fHdlcProc 处理函数. 
 * 
 * @返回: 有效帧长度
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
/*uint16 WaitForHdlc(uint8* buffer, uint16 size, fHdlcProc func, uint8 port)
{
    uint16 len;
    uint16 hdlclen;
    uint8 module;
    
    len = 0;
    for(uint8 uc_i = 0; uc_i < 60; uc_i++)
    {
        len += gs_Uart.Read(port, buffer + len, size - len);
        if(len > 0)                     //收到数据
        {
            hdlclen = fHdlcParseBuf(buffer, len, &module);
            if(hdlclen)                 //有效hdlc格式数据
            {
                if(func)                //特殊处理
                {
                    func(buffer, hdlclen, module);
                }
                else                    //默认处理
                {
                    fHdlcDataProc(buffer, hdlclen, module);
                }
                return hdlclen;
            }
        }
        gs_OS.TK_Sleep(5);
    }
    return 0;
}*/



/*******************************************************************************
 * @function_name:  WaitForBg
 * @function_file:  
 * @描述: 等待VS帧的到来并处理
 * 
 * 
 * @参数: buffer 缓存
 * @参数: size 缓存大小
 * @参数: fBgProc 处理函数. 
 * 
 * @返回: 有效帧长度
 * @作者: yzy (2018年3月22日)
 *-----------------------------------------------------------------------------
 * @修改人: 
 * @修改说明: 
 ******************************************************************************/
uint16 WaitForVs(uint8* buffer, uint16 size, fBgProc func, uint8 port)
{
    uint16 len;
    uint8  bglen;
    
    len = 0;
    for(uint8 uc_i = 0; uc_i < 60; uc_i++)
    {
        len += SER_RecvData(port, buffer + len, size - len,100);
        if(len > 0)                     //收到数据
        {
            bglen = PST_CheckFrame2(PST_FRM_VS_NO, buffer, len);
            if(bglen)                   //有效bg格式数据
            {
                if(func)                //特殊处理
                {
                    func(buffer, bglen);
                }
                return bglen;
            }
        }
        aos_msleep(50);
    }
    return 0;
}



