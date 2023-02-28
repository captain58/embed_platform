#include "sys.h"
#include "App_Public.h"
#include "user_func.h"

uint8 CurStaSeq = 0; //当前的统计周期
uint32 CurStaMap[4]; // 当前的已经接收的统计节点

/**********************************************************************
 *软复位函数
 *
 **********************************************************************/
void Reset(void)
{
//	        void(*preset)(void);
//	        preset = (void(*)())0x0;
//	        preset();
//    WDTC = 0x4F00 ; // 大概超时时间为左右
//    WDMOD = 0x03;                               // 设置看门狗模式：中断且复位
//    WDFEED = 0xAA;                              // 第一次喂狗启动WDT
//    WDFEED = 0x55;
//    while(1);
}

/*************************************************************************
 * Function Name: CheckID
 * Parameters:  ID
 * Return: 1 ，正常；0，非法。
 * Description: 检查ID的合法性
 *
 *************************************************************************/
uint8 Check_ID(uint16 id)
{
        if ((id > MAX_SUP_SS_NUM) || (id == 0))
                return 0;
        else
                return 1;
}

/*************************************************************************
 * Function Name: CleanPkt
 * Parameters:  ID
 * Return: 1 ，正常；0，非法。
 * Description: 清除数据包内的相关内容
 *
 *************************************************************************/
void Clean_PKT(PKT *pkt)
{
        if (pkt == NULL)
                return ;
        pkt->len = 0;
        pkt->sentoffset = 0;
        memset(pkt->data, 0, 256);
      
}

/*************************************************************************
 * Function Name: SetCrastyle
 * Parameters:  value
 * Return: 1 ，正常；0，非法。
 * Description: 设置晶振的补偿电容值
 *
 *************************************************************************/
void Set_Crastyle(uint8 value)
{
        SpiWriteRegister(0x09, value);

}

//*******************************************
uint8 Check_Sta_PKT(uint8 seq, uint8 id)
{
        #if 1
                uint8 index;
                uint8 location;
                if (seq == 0)
                        return 1;
                if (seq != CurStaSeq)
                        return 0;

                index = id / 32;
                location = id % 32;

                if ((CurStaMap[index] &(1 << location)) == (1 << location))
                // 重复请求包
                        return 0;

                CurStaMap[index] |= (1 << location);
        #endif 
        return 1;


}

//====================================================
// 如果x >y ,则返回0xff
// 否则返回0
uint8 After(uint32 x, uint32 y)
{
        if (x > y)
                return 0xff;
        else
                return 0x00;
}

//====================================================
uint8 Before(uint32 x, uint32 y) //判断X是否比Y小
{
        if (x < y)
              return 1;//如果X小于Y，返回1
        else
              return 0;
}


