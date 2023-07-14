/*
 * Copyright (C) 2015-2017 Captain Group Holding Limited
 */
//#include "core_cm4.h"
#include "aos/init.h"
#include "aos/kernel.h"

#include <k_api.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef TINY_ENGINE
#define AOS_START_STACK 1536 + 1024*14
#else
#define AOS_START_STACK 256
#endif

static ktask_t demo_task_obj;
cpu_stack_t demo_task_buf[AOS_START_STACK];


ktask_t *g_aos_init;

static kinit_t kinit;

extern int application_start(int argc, char **argv);
extern void board_init(void);
#ifdef AOS_COMP_CPLUSPLUS
extern void cpp_init(void);
#endif

static void var_init()
{
    kinit.argc = 0;
    kinit.argv = NULL;
    kinit.cli_enable = 1;
}

extern void hw_start_hal(void);

#include "aos/hal/uart.h"
//#include "hal/hal_uart_stm32l4.h"
#include "gd32f30x.h"
#include "board.h"
#if defined (AOS_OTA_RECOVERY_TYPE)
#include "rec_clear_ota_flag.h"
#endif
extern void stm32_soc_peripheral_init(void);

static void sys_init(void)
{
//    stm32_soc_peripheral_init();
#ifdef BOOTLOADER
    main();
#else
//    hw_start_hal();
//    board_init();
//    var_init();
//#ifdef AOS_COMP_CPLUSPLUS
//    cpp_init();
//#endif
//#if defined (AOS_OTA_RECOVERY_TYPE)
//    sys_clear_ota_flag();
//#endif
    aos_components_init(&kinit);

    
#ifndef AOS_BINS
    application_start(kinit.argc, kinit.argv);  /* jump to app/example entry */
#endif
#endif
}
void delay(uint32_t count)
{

    for(int i = 0; i < count; i++);
}
extern void aos_heap_set(void);
extern void stm32_soc_init(void);

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void)
{
    uint32_t ahb_frequency = 0;
    ahb_frequency = rcu_clock_freq_get(CK_AHB);
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(ahb_frequency / RHINO_CONFIG_TICKS_PER_SECOND)){
        /* capture error */
        while (1){
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
    NVIC_SetPriority(PendSV_IRQn, 0x00U);
}
//    extern LPTIM_HandleTypeDef hlptim1;
static void sys_start(void)
{
    aos_heap_set();
    systick_config();
//    stm32_soc_init();
//	    HAL_LPTIM_Counter_Start_IT(&hlptim1, 50000);
//	
//	    while(1)
//	    {
//	        
//	        delay(1000000);
//	        MX_GPIO_Init1();
//	        hwEnterSleep();
//	        SystemClock_Config();
//	        MX_GPIO_Init();
//	    }
//    while(1)
//    {
//        delay(1000000);
//    }
    aos_init();
    
    //krhino_task_dyn_create(&g_aos_init, "aos-init", 0, AOS_DEFAULT_APP_PRI, 0, AOS_START_STACK, (task_entry_t)sys_init, 1);
    krhino_task_create(&demo_task_obj, "aos-init", 0,AOS_DEFAULT_APP_PRI, 
        0, demo_task_buf, AOS_START_STACK, (task_entry_t)sys_init, 1);
    
    aos_start();
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *         which reports the name of the source file and the source
  *         line number of the call that failed.
  *         If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{ 
  /* Check the parameters */
  assert_param(IS_NVIC_VECTTAB(NVIC_VectTab));
  assert_param(IS_NVIC_OFFSET(Offset));  
   
  SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

#define NVIC_VectTab_FLASH  ((uint32_t)0x08000000)

int main(void)
{
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000); 

    sys_start();
    return 0;
}


