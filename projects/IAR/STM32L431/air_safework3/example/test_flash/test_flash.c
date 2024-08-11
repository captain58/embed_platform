/*
 * Copyright (C) 2015-2017 Captain Group Holding Limited
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
            if(SYS_FILE_Open() != SYS_ERR_OK)
            {
                LOG_DEBUG(DBGFMT"SYS_FILE_Open failed !\n", DBGARG);
                continue;
            }
            //SER_SendData(UART_CHANNEL_DEBUG, buf_recv, rcnt, 1000);
            if(SYS_FILE_Write(buf_recv, 0, rcnt) != SYS_ERR_OK)
            {
                LOG_DEBUG("flash write failed !\n");
                SYS_FILE_Close();
                continue;
            }
            if(SYS_FILE_Read(buf_read, 0, rcnt) != SYS_ERR_OK)
            {
                LOG_DEBUG("flash read failed !\n");
                SYS_FILE_Close();
                continue;
            }
            
            LOG_DEBUG("read %d\n", rcnt);
            sleep_ms(10);
            LOG_DUMPHEX(LOG_LEVEL_DEBUG, "recv:\n", buf_read, rcnt);
            
            SYS_FILE_Close();
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
    //PIOR = 0x05U;
    krhino_init();

    
    bsp_init();


    tick_init(RHINO_CONFIG_TICKS_PER_SECOND);

    
    krhino_task_create(&demo_task_obj1, "demo_task", 0,DEMO_TASK_PRIORITY1, 
        50, demo_task_buf1, DEMO_TASK_STACKSIZE, demo_task1, 1);

    krhino_task_create(&demo_task_obj2, "demo_task2", 0,DEMO_TASK_PRIORITY2, 
        50, demo_task_buf2, DEMO_TASK_STACKSIZE, demo_task2, 1);
    
    krhino_start();
    
    return 0;
}

