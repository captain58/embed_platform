/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <k_api.h>
#include <stdio.h>
#include <stdlib.h>
#include "public.h"
/* 200 * cpu_stack_t = 400byte */
#define DEMO_TASK_STACKSIZE     200
#define DEMO_TASK_PRIORITY1     20
#define DEMO_TASK_PRIORITY2     21

extern void tick_init(uint32_t ticks_per_sec);
extern void tick_start(void);

static ktask_t      demo_task_obj1;
static ktask_t      demo_task_obj2;

static cpu_stack_t  demo_task_buf1[DEMO_TASK_STACKSIZE];
static cpu_stack_t  demo_task_buf2[DEMO_TASK_STACKSIZE];

static uint32_t     tst_cnt;
static uint32_t     tmr_cnt;

static ksem_t       sem_tst;
static ktimer_t     timer_test;
static kbuf_queue_t queue_test;

static char         queue_buf[50];
static char         queue_buf_recv[10];
static size_t       queue_recv_size;

static void timer_cb(void *timer, void *arg)
{
    char queue_buf_recv[10];
    memset(queue_buf_recv, 'a', 10);
    tmr_cnt++;
    krhino_buf_queue_send(&queue_test, queue_buf_recv, 10);
    krhino_timer_stop(&timer_test);
}
    static unsigned char         buf_recv[100];
    static unsigned char         buf_read[100];    
void demo_task1(void *arg)
{
    //sleep_ms(1);
    tick_start();
    
    krhino_sem_create(&sem_tst, "sem_tst", 0);
	
    krhino_buf_queue_create(&queue_test, "queue_test",
                             queue_buf, 50, 50);
	
    krhino_timer_create(&timer_test, "timer_test", timer_cb,
                            10, 10, 0, 0);
	
    krhino_timer_start(&timer_test);
	
//	        CPSR_ALLOC();
//	        RHINO_CPU_INTRPT_DISABLE();
//	    //      rx_data = sid->SDRx->SDRR;    
//	    //      IRQServerUart0Receive(rx_data);
//	        RHINO_CPU_INTRPT_ENABLE();
    krhino_buf_queue_recv(&queue_test, RHINO_WAIT_FOREVER, queue_buf_recv,
                          &queue_recv_size);
    
//	    PCLOE0 = 0U;    /* disable PCLBUZ0 operation */
//	    CKS0 = 0x01;
//	    /* Set PCLBUZ0 pin */
//	    PFSEG5 &= 0xBFU;
//	    PM0 &= 0xFBU;
//	    PCLOE0 = 1U;    /* enable PCLBUZ0 operation */
    uint32 rcnt = 0;
    uint8 err = 0;
    UART_Init();
    SER_Open(UART_CHANNEL_DEBUG, TDB_MODE_R | TDB_MODE_W);
    LogInit(0,LOG_LEVEL_DEBUG,256);
    
    SYS_FILE_Init();
    SYS_FILE_Start();
    
    while (1)
    {
        memset(buf_recv,0,100);
        memset(buf_read,0,100);
        rcnt = SER_RecvData(UART_CHANNEL_DEBUG, buf_recv, 100, 1000);
        if(rcnt > 0)
        {
            if(SYS_EEPROM_Open() != SYS_ERR_OK)
            {
                LOG_DEBUG(DBGFMT"SYS_EEPROM_Open failed !\n", DBGARG);
                continue;
            }
            //SER_SendData(UART_CHANNEL_DEBUG, buf_recv, rcnt, 1000);
            if(Eeprom_SafeWrite(buf_recv, 0, rcnt) != SYS_ERR_OK)
            {
                LOG_DEBUG("ee write failed !\n");
                SYS_EEPROM_Close();
                continue;
            }
            if(Eeprom_SafeRead(buf_read, 0, rcnt) != SYS_ERR_OK)
            {
                LOG_DEBUG("ee read failed !\n");
                SYS_EEPROM_Close();
                continue;
            }
            
            LOG_DEBUG("read %d\n", rcnt);
            sleep_ms(10);
            LOG_DUMPHEX(LOG_LEVEL_DEBUG, "recv:\n", buf_read, rcnt);
            
            SYS_EEPROM_Close();
        }
//	        krhino_sem_give(&sem_tst);
        //krhino_task_sleep(RHINO_CONFIG_TICKS_PER_SECOND * 2);
        sleep_ms(10);
        //sprintf((char *)buf_recv, "%s\n", "abcdefg");

        //SER_SendData(UART_CHANNEL_DEBUG, buf_recv, strlen((char *)buf_recv), 1000);
    }
}

void demo_task2(void *arg)
{
    while (1)
    {
        tst_cnt++;
//	        krhino_sem_take(&sem_tst, RHINO_WAIT_FOREVER);
        krhino_task_sleep(RHINO_CONFIG_TICKS_PER_SECOND);
    }
}

int main(void)
{
    PIOR = 0x05U;
    krhino_init();

    
//	    PIOR = 0x05U;
//	    PFSEG0 = _80_PFSEG07_SEG | _40_PFSEG06_SEG | _20_PFSEG05_SEG | _10_PFSEG04_SEG;
//	    PFSEG1 = _80_PFSEG15_SEG | _40_PFSEG14_SEG | _20_PFSEG13_SEG | _10_PFSEG12_SEG | _08_PFSEG11_SEG | 
//	             _04_PFSEG10_SEG | _02_PFSEG09_SEG | _01_PFSEG08_SEG;
//	    PFSEG2 = _80_PFSEG23_SEG | _40_PFSEG22_SEG | _20_PFSEG21_SEG | _10_PFSEG20_SEG | _08_PFSEG19_SEG | 
//	             _04_PFSEG18_SEG | _02_PFSEG17_SEG | _01_PFSEG16_SEG;
//	    PFSEG3 = _00_PFSEG30_PORT | _00_PFSEG29_PORT | _00_PFSEG28_PORT | _00_PFSEG27_PORT | _00_PFSEG26_PORT | 
//	             _02_PFSEG25_SEG | _01_PFSEG24_SEG | _00_PFDEG_PORT;
//	    PFSEG4 = _00_PFSEG38_PORT | _00_PFSEG37_PORT | _00_PFSEG36_PORT | _00_PFSEG35_PORT | _00_PFSEG34_PORT | 
//	             _00_PFSEG33_PORT | _00_PFSEG32_PORT | _00_PFSEG31_PORT;
//	    PFSEG5 = _80_PFSEG46_SEG | _40_PFSEG45_SEG | _20_PFSEG44_SEG | _10_PFSEG43_SEG | _08_PFSEG42_SEG | 
//	             _00_PFSEG41_PORT | _00_PFSEG40_PORT | _00_PFSEG39_PORT;
//	    PFSEG6 = _00_PFSEG50_PORT | _00_PFSEG49_PORT | _00_PFSEG48_PORT | _01_PFSEG47_SEG;
//	    ISCLCD = _01_ISCCAP_VALID;
//	    P0 = _00_Pn5_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn7_OUTPUT_0;
//	    P1 = _00_Pn0_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _08_Pn3_OUTPUT_1 | _00_Pn4_OUTPUT_0 | 
//	         _00_Pn5_OUTPUT_0 | _40_Pn6_OUTPUT_1;
//	    P2 = _00_Pn1_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _00_Pn3_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn5_OUTPUT_0 | 
//	         _00_Pn7_OUTPUT_0;
//	    P4 = _00_Pn4_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn7_OUTPUT_0;
//	    P6 = _02_Pn1_OUTPUT_1;
//	    P12 = _00_Pn6_OUTPUT_0 | _00_Pn7_OUTPUT_0;
//	    P13 = _01_Pn0_OUTPUT_1;
//	    PMC1 = _00_PMCn0_DI_ON | _00_PMCn1_DI_ON | _00_PMCn2_DI_ON | _00_PMCn3_DI_ON | _F0_PMC1_DEFAULT;
//	    PMC2 = _00_PMCn2_DI_ON | _00_PMCn3_DI_ON | _00_PMCn4_DI_ON | _00_PMCn5_DI_ON | _00_PMCn6_DI_ON | _00_PMCn7_DI_ON | 
//	           _03_PMC2_DEFAULT;
//	    ADPC = _01_ADPC_DI_ON;
//	    PM0 = _01_PMn0_NOT_USE | _02_PMn1_NOT_USE | _04_PMn2_NOT_USE | _08_PMn3_NOT_USE | _10_PMn4_NOT_USE | 
//	          _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT;
//	    PM1 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT | 
//	          _00_PMn4_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _80_PMn7_NOT_USE;
//	    PM2 = _01_PMn0_NOT_USE | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT | 
//	          _00_PMn4_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _40_PMn6_MODE_INPUT | _00_PMn7_MODE_OUTPUT;
//	    PM4 = _01_PMn0_NOT_USE | _02_PMn1_NOT_USE | _04_PMn2_NOT_USE | _08_PMn3_NOT_USE | _00_PMn4_MODE_OUTPUT | 
//	          _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT;
//	    PM6 = _01_PMn0_MODE_INPUT | _00_PMn1_MODE_OUTPUT | _FC_PM6_DEFAULT;
//	    PM12 = _20_PMn5_NOT_USE | _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT | _1F_PM12_DEFAULT;
//	    PM13 = _00_PMn0_MODE_OUTPUT | _FE_PM13_DEFAULT;

    bsp_init();


    tick_init(RHINO_CONFIG_TICKS_PER_SECOND);

    
    krhino_task_create(&demo_task_obj1, "demo_task", 0,DEMO_TASK_PRIORITY1, 
        50, demo_task_buf1, DEMO_TASK_STACKSIZE, demo_task1, 1);

    krhino_task_create(&demo_task_obj2, "demo_task2", 0,DEMO_TASK_PRIORITY2, 
        50, demo_task_buf2, DEMO_TASK_STACKSIZE, demo_task2, 1);
    
    krhino_start();
    
    return 0;
}

