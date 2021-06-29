/****************************************Copyright (c)**************************************************
**                                  ________�Ƽ����޹�˾
**                                          ������
**
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: sys.h
**��   ��   ��: yzy
**����޸�����: 2010��1��14��
**��        ��: rtos����ϵͳ��ͷ�ļ�
**ע        ��:
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: yzy
** ��  ��: v1.0
** �ա���: 2010��1��14��
** �衡��: ԭʼ�汾
********************************************************************************************************/

#ifndef _SYS_H_
#define _SYS_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

//ϵͳƽ̨Ԥ����
#include "aos/bdef.h"


//�������ú�
#include "ext.h"


//cmsis,chip,os
//#include "sys_os.h"
#include "chip.h"

//������ر�������(�����������ļ���HAL/DEV�����õ�������)
#include "cdef.h"


//����ͷ�ļ�
#include "msgid.h"              //����б�
#include "iomap.h"              //io������
//#include "ramap.h"              //ram����




//ͨ�ú������ļ�
#include "libs.h"


#include "aos/kernel.h"

#endif
