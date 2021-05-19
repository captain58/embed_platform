#ifndef __USER_ERR_CODE_H__
#define __USER_ERR_CODE_H__

#define USER_ERR_NOCARD                 1   //用户卡拔卡过快
#define USER_ERR_UNKNOW                 2   //非法卡
#define USER_ERR_CARDID                 3   //用户号出错
#define USER_ERR_WRITE_BACK             4   //回写返回数据出错
#define USER_ERR_CHARGE_MAX             5   //购气量加剩余量超限制额
#define USER_ERR_WARNING_CLOSE          6   //预关阀值关阀提示
#define USER_ERR_MAX_FLOW               7   //过流关阀提示
#define USER_ERR_MAGNET_1               8   //单边干扰
#define USER_ERR_MAGNET_2               9   //双边干扰
#define USER_ERR_IDLE_CLOSE_LOCK        10  //几天不用气关阀，阀门锁定
#define USER_ERR_RND_MATCH              11  //购气随机数错误
#define USER_ERR_IDLE_CLOSE             12  //不用气关阀提示
#define USER_ERR_GET_RND                13  //取随机码错误
#define USER_ERR_METER_TYPE             14  //用气类型出错
#define USER_ERR_CHARGE_INIT            15  //圈存初始化失败
#define USER_ERR_CHARGE                 16  //圈存失败  
#define USER_ERR_UPDATE_BINARY          17  //更新二进制文件出错
#define USER_ERR_GET_BANLANCE           18  //读余额出错
#define USER_ERR_GENERATE_KEY           19  //子密钥过程密钥生成出错
#define USER_ERR_CHANGE_CARDTYPE        20  //改写卡类型失败
#define USER_ERR_CARD_INT_AUTHER        21  //CPU 卡内部认证错误
#define USER_ERR_CONSUME_INIT           22  //消费初始化失败
#define USER_ERR_CHECK_CARD_LOCK        23  //开户后不能使用生产测试卡
#define USER_ERR_CONSUME                24  //消费出错
#define USER_ERR_TRAN_CHANGE            25  //回收转移卡改变状态出错
#define USER_ERR_SELECT_FILE            26  //选择文件错误
#define USER_ERR_CARD_EXT_AUTHER        27  //CPU 卡外部认证错误
#define USER_ERR_READ_BINARY            28  //读二进制文件出错
#define USER_ERR_PIN                    29  //PIN 密码验证错误
#define USER_ERR_HAD_USER               30  //表已开户插开户卡
#define USER_ERR_GET_RESPONDENT         31  //取响应数据出错
#define USER_ERR_ESAM_EXT_AUTHER        32  //ESAM 外部认证错误
#define USER_ERR_SECURITY_CMD           33  //安全模块命令错误
#define USER_ERR_CRC                    34  //卡内数据校验错误
#define USER_ERR_TRANS_STATE            35  //回收转移卡使用状态错误  
#define USER_ERR_MAC                    36  //MAC码生成错误
#define USER_ERR_CHECK_TIMES_0          37  //生产卡已无使用次数
#define USER_ERR_CHECK_INVALD_TIME      38  //生产卡使用时间错误
#define USER_ERR_TRANS_END              39  //回收转移卡处于转移完毕状态
#define USER_ERR_HAD_TRANS_ONCE         40  //表具已使用过一次转移卡，不能再转移
#define USER_ERR_TRAN_LOCK              41  //转移卡使用限制

#define USER_ERR_EEP                    50  //ESAM数据错误
#define USER_ERR_ESAM_INIT              51  //
#define USER_ERR_NOUSER                 52  //没有数据供转移
#define USER_ERR_CLEAR_ALL              53  //错误的恢复出厂设置卡片
#define USER_ERR_DOORBROKEN             54  //阀门故障
#define USER_ERR_INVALID_TIME           55  //无效的时间
#define USER_ERR_ERR_CLOCK              56  //时间错误

#endif


