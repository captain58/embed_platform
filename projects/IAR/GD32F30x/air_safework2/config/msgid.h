/****************************************Copyright (c)**************************************************
**                                  ________科技有限公司
**                                          开发部
**
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: msgid.h
**创   建   人: yzy
**最后修改日期: 2013年2月25日
**描        述: 编号列表头文件定义
**注        意:
**--------------历史版本信息----------------------------------------------------------------------------
** 作  者: yzy
** 版  本: v1.0
** 日　期: 2013年2月25日
** 描　述: 原始版本
********************************************************************************************************/




///////////////////////////////////////////////////////////////////
//   
// 软定时器编号
//
/////////////////////////////////////////////////////////////////
#define TIMER_ID_RTC     0                   //定时TIMER编号
#define TIMER_ID_MSG     1
#define TIMER_ID_WDT     2
#define TIMER_ID_LGPI    3
#define TIMER_ID_FGPI    4
#define TIMER_ID_LED     5
#define TIMER_ID_UART    6
#define TIMER_ID_AMI     7



///////////////////////////////////////////////////////////////////
//   
// 硬定时器编号(匹配通道)
//
/////////////////////////////////////////////////////////////////
#define TIME3_ID_UART2_SOT  0
#define TIME3_ID_UART3_SOT  1
#define TIME3_ID_UART4_SOT  2
#define TIME3_ID_UART9_SOT  3




///////////////////////////////////////////////////////////////////
//   
// 消息编号定义
//
/////////////////////////////////////////////////////////////////
//===============系统级消息(0-30,各进程不可重复定义)=======================================
#define MSG_YEAR        0                   //年消息//时间消息应该是年为优先级最高
#define MSG_MON         1                   //月消息
#define MSG_DAY         2                   //天消息
#define MSG_HOUR        3                   //小时消息
#define MSG_MIN         4                   //分消息
#define MSG_SEC         5                   //秒消息


#define MSG_LIVE        6                   //保活消息
#define MSG_UART0       7                   //逻辑串口0收到消息 UART_ENUM
#define MSG_UART1       8                   //逻辑串口1收到消息
#define MSG_UART2       9
#define MSG_UART3       10
#define MSG_UART4       11
#define MSG_UART5       12
#define MSG_UART9       13

#define MSG_ENTER_LP    14                  //进入低功耗

#define MSG_LIFEVT      16                  //慢速口下降沿
#define MSG_LIREVT      17                  //慢速口上升沿
#define MSG_LILEVT      18                  //慢速口保持
#define MSG_FIREVT      19                  //快速上升沿
#define MSG_FIFEVT      20                  //快速下降沿

#define MSG_FARP_RECVDATA           21         //表示收到来自ETH通道的数据
#define MSG_FROMCLNDATA             22          //表示tcp server收到来自客户端通道1的数据
#define MSG_IP1DATA                 23          //表示收到GPRS通道1 的数据
#define MSG_SMRECV                  24          //短信数据
#define MSG_RING         25             //收到语音振铃

//===============(排列需按实际串口编号从小到大排列)=======================================
#define MSG_PSTP         MSG_UART1      
#define MSG_MDMP        (MSG_UART0 + 1)
#define MSG_ESAM        (MSG_UART0 + 2)           //与 SYS_PST_PORT 对应
#define MSG_CARD        (MSG_UART0 + 3) 
#define MSG_NETP        (MSG_UART0 + 4)

//===============应用级消息(30-63,各进程可重复定义)=======================================
//======================================================
#define MSG_HLV_BATCTL      30
#define MSG_HLV_VAR         31
#define MSG_HLV_CLR      32

#define MSG_MAIN_RESET          35      //MAIN主程序复位

#define MSG_MAIN_UPDATA_CHK     40      //升级文件校验
#define MSG_MAIN_HARD_CHK       41      //硬件自检
#define MSG_MAIN_BLE_CHK       42      //硬件自检
#define MSG_MAIN_RFID_CHK       42      //硬件自检
#define MSG_MAIN_ADDR_CHG       43      //地址变更

//	#define MSG_FARP_PRIOCNT        51
//	#define MSG_FARP_DISGPRS        52  //优先级需高于MSG_FARP_CNTGPRS
//	#define MSG_FARP_CNTGPRS        53
//	#define MSG_FARP_RECVVALIDIP    54
//	#define MSG_FARP_DISETH         55  //优先级需高于MSG_FARP_CNTETH
//	#define MSG_FARP_CNTETH         56
//	#define MSG_FARP_VAR            57
//	#define MSG_FARP_ETHINIT        58
//	#define MSG_FARP_PARACHG        61      //联网参数变更
//	#define MSG_FARP_ETHCHG         62      //以太网变更
//	#define MSG_FARP_MACCHG         63      //以太网mac更变
//	#define MSG_FARP_ETHRST         MSG_FARP_ETHCHG//兼具复位以太网模块功能



#define MSG_FARP_PRIOCNT        44

#define MSG_FARP_DISPPP         45  //PPP链接
#define MSG_FARP_CNTPPP         46  //断开PPP链接
#define MSG_FARP_DISSERVER      47  //断开TCP SERVER 侦听
#define MSG_FARP_CNTSERVER      48  //建立TCP SERVER 侦听

#define MSG_FARP_DISFROMCLN1     49  //断开收到的客户端链接1
#define MSG_FARP_DISFROMCLN2     50  //断开收到的客户端链接2

#define MSG_FARP_DISGPRS        51  //优先级需高于MSG_FARP_CNTGPRS
#define MSG_FARP_CNTGPRS        52
#define MSG_FARP_RECVVALIDIP    53
#define MSG_FARP_RECVVALIDSER   54
//	#define MSG_FARP_DISETH         55  //优先级需高于MSG_FARP_CNTETH
//	#define MSG_FARP_CNTETH         56

#define MSG_FARP_VAR            57
#define MSG_FARPE_FCNT          60      //连接失败消息

#define MSG_FARP_PARACHG        61      //联网参数变更
#define MSG_FARP_CHECK          62      //模块自检
#define MSG_MODEM_TIMEOUT       64
#define MSG_MODEM_ABORT         65

#define MSG_CARD_INSERT         70

#define MSG_NETP_IDATA     80              //初始化
#define MSG_NETP_CURDATA   81             //读曲线数据
#define MSG_NETP_BCURDATA  82             //补曲线数据
#define MSG_NETP_DAYDATA   83             //读日冻结数据
#define MSG_NETP_MONDATA   84             //读月冻结数据
#define MSG_NETP_EVNDATA   85             //读事件

#define MSG_NETP_CKTIME    86             //对下挂表校时
#define MSG_NETP_FEE       87             //对下挂表下装费率

#define MSG_NETP_MT_UPDATA_START    88      //对下挂表计升级开始
#define MSG_NETP_MT_UPDATA_INIT     89      //对表升级初始化
#define MSG_NETP_MT_UPDATA_DATAING  90      //传输文件包
#define MSG_NETP_MT_UPDATA_CHECK    91      //校验文件包
#define MSG_NETP_MT_UPDATA_ACTION   92      //执行升级
#define MSG_NETP_MT_READ_VERSION    93      //读表版本
#define MSG_NETP_FINISH             94      //抄表完成消息
#define MSG_NETP_TASK_RUN_FLAG_PER_485    95      //抄表485启动消息

#define MSG_NETP_CHECK      96


#define MSG_HLV_INP     100              //内部模块相关参数更改
#define MSG_HLV_FARP    101              //远程模块相关参数更改
#define MSG_HLV_PST     102              //通讯模块相关参数更改
#define MSG_HLV_TASK    103              //任务模块相关参数更改
#define MSG_PST_VAR     104      //pst进程参数更新消息
#define MSG_ALRT_ERC1           105      //参数及数据初始化 erc1
#define MSG_ALRT_CLR            106      //清除所有记录

#define MSG_SWITCH_CHANGE   110
#define MSG_EVENT_CHANGE    111
#define MSG_ENTER_SLEEP     112
#define MSG_WAKE_UP         113  
//	#define MSG_FTP_OPEN            110
//	#define MSG_FTP_OPEN_SUCC       111
//	#define MSG_FTP_OPEN_FAILED     112
//	#define MSG_FTP_GET             113
//	#define MSG_FTP_GET_SUCC        114
//	#define MSG_FTP_GET_FAILED      115
//	#define MSG_FTP_GET_FNS         116
//	#define MSG_FTP_REPORT_RESULT   117
//	#define MSG_FTP_CLOSE           118
//	#define MSG_FTP_PROJ_FAILED         119

#ifndef EXT_MSGID
#define EXT_MSGID extern 
#endif

EXT_MSGID const uint8_t msgidA[];
