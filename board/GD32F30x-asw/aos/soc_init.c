/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#include <stdint.h>
#include <stdio.h>
#include "aos/hal/uart.h"
#include "aos/hal/i2c.h"
#include "aos/hal/spi.h"

#include "k_config.h"
#include "hal.h"
#include "bsp.h"
#include "board.h"

//#include "stm32l4xx_hal.h"
//#include "hal_uart_stm32l4.h"
//#include "hal/hal_i2c_stm32l4.h"
//#include "hal_spi_stm32l4.h"
//#include "stm32l4xx_hal_spi.h"
//#include "aos/hal/gpio.h"
//#include "hal_gpio_stm32l4.h"

#if defined (__CC_ARM) && defined(__MICROLIB)
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#elif defined(__ICCARM__)
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#else
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#endif /* defined (__CC_ARM) && defined(__MICROLIB) */

#if defined (__CC_ARM)
size_t g_iram1_start = 0x20000000;
size_t g_iram1_total_size = 0x00018000;
#endif

uart_dev_t uart_0;
uart_dev_t uart_1;

uint8_t g_ucPutcharEn = 1;

i2c_dev_t brd_i2c1_dev = {1, {0}, NULL};

static void stduart_init(void);

//UART_MAPPING UART_MAPPING_TABLE[] =
//{
//    { PORT_UART_STD,     LPUART1, { UART_OVERSAMPLING_16, UART_ONE_BIT_SAMPLE_DISABLE, UART_ADVFEATURE_NO_INIT, 512} },
//    { PORT_UART_AT,      UART4,  { UART_OVERSAMPLING_16, UART_ONE_BIT_SAMPLE_DISABLE, UART_ADVFEATURE_NO_INIT, 1024} },
//    { NETP_PORT_NO,      USART1,  { UART_OVERSAMPLING_16, UART_ONE_BIT_SAMPLE_DISABLE, UART_ADVFEATURE_NO_INIT, 256} },
//    { NETP2_PORT_NO,     USART3,  { UART_OVERSAMPLING_16, UART_ONE_BIT_SAMPLE_DISABLE, UART_ADVFEATURE_NO_INIT, 256} },
//    { NETP3_PORT_NO,     USART2,  { UART_OVERSAMPLING_16, UART_ONE_BIT_SAMPLE_DISABLE, UART_ADVFEATURE_NO_INIT, 256} },
//    { NETP4_PORT_NO,     UART5,  { UART_OVERSAMPLING_16, UART_ONE_BIT_SAMPLE_DISABLE, UART_ADVFEATURE_NO_INIT, 256} },
//};
//
//
//static gpio_irq_trigger_t mode_rising = IRQ_TRIGGER_RISING_EDGE;
//static gpio_irq_trigger_t mode_falling = IRQ_TRIGGER_FALLING_EDGE;
//static gpio_irq_trigger_t mode_both = IRQ_TRIGGER_BOTH_EDGES;
static uint8_t gpio_set = 1;
static uint8_t gpio_reset = 0;
static uint8_t gpio_rvs = 1;
static uint8_t gpio_norvs = 0;



static led_dev_priv led_set_norvs =     {&gpio_set,     &gpio_norvs};
static led_dev_priv led_set_rvs =       {&gpio_set,     &gpio_rvs};
static led_dev_priv led_reset_norvs =   {&gpio_reset,   &gpio_norvs};
static led_dev_priv led_reset_rvs =     {&gpio_reset,   &gpio_rvs};


//gpio_dev_t brd_gpio_table[] = {
//    {MODEM_ONOFF, OUTPUT_PUSH_PULL, &gpio_reset},           //PA6
//    {MODEM_PWR, OUTPUT_PUSH_PULL, &gpio_reset},             //PE8
//    {RS485_PWR, OUTPUT_PUSH_PULL, &gpio_reset},             //PE12
//    {BLE_PWR, OUTPUT_PUSH_PULL, &gpio_reset},               //PD7
//    {BLE_MODE, OUTPUT_PUSH_PULL, &gpio_set},                //PB4
//    {BLE_UART_CTL, OUTPUT_PUSH_PULL_PULL_DOWN, &gpio_reset},//PE2    
//};
//
//gpio_dev_t brd_gpio_LPtable[] = {
//    {MODEM_ONOFF, OUTPUT_PUSH_PULL, &gpio_reset},            //PC7
//    {MODEM_PWR, OUTPUT_PUSH_PULL, &gpio_reset},            //PB6
//    {RS485_PWR, OUTPUT_PUSH_PULL, &gpio_reset},            //PB6
//    {BLE_PWR, OUTPUT_PUSH_PULL, &gpio_reset},            //PB6
//    {BLE_MODE, OUTPUT_PUSH_PULL, &gpio_set},            //PB6
//    {BLE_UART_CTL, OUTPUT_PUSH_PULL, &gpio_reset},            //PE2    
//};
//
//
//gpio_dev_t brd_led_table[] = {
//    {LED_RUN, OUTPUT_PUSH_PULL, &led_reset_norvs},         //PA8
//    {LED_MBUS, OUTPUT_PUSH_PULL, &led_reset_norvs},        //PC7
//    {LED_GPRS, OUTPUT_PUSH_PULL, &led_reset_norvs},        //PC9
//    {LED_ERR, OUTPUT_PUSH_PULL, &led_reset_norvs},         //PC6
//};
//
//gpio_dev_t brd_gpi_table[] = {
//    {GPI_Key1, INPUT_PULL_UP, NULL},            //PA4
//    {GPI_BLE_LINK, INPUT_PULL_DOWN, NULL},        //PB6
//    {GPI_Key2, INPUT_PULL_UP, NULL},            //PA5
//    {GPI_EXT_PWR, INPUT_PULL_UP, NULL},         //PC5
//    {GPI_PRGM, INPUT_PULL_UP, NULL},         //PC5
//    
//};
//
//gpio_dev_t brd_gpi_LPtable[] = {
//    {GPI_Key1, OUTPUT_OPEN_DRAIN_PULL_UP, &gpio_set},            //PA4
//    {GPI_BLE_LINK, OUTPUT_OPEN_DRAIN_PULL_DOWN, &gpio_reset},        //PB6
//    {GPI_Key2, OUTPUT_OPEN_DRAIN_PULL_UP, &gpio_set},            //PA5
//    {GPI_EXT_PWR, INPUT_PULL_UP, NULL},            //PA5
//};
//
//gpio_dev_t gs_FlashSpiMISO = {SPI_FLASH_MISO, INPUT_PULL_UP,  NULL};
//gpio_dev_t gs_FlashSpiMOSI = {SPI_FLASH_MOSI, OUTPUT_PUSH_PULL, &gpio_reset};
//gpio_dev_t gs_FlashSpiSCK  = {SPI_FLASH_SCK, OUTPUT_PUSH_PULL, &gpio_reset};
//gpio_dev_t gs_FlashSpiCS   = {SPI_FLASH_CS, OUTPUT_PUSH_PULL, &gpio_reset};
//gpio_dev_t gs_FlashSpiPWR  = {SPI_FLASH_PWR, OUTPUT_PUSH_PULL, &gpio_reset};
//gpio_dev_t gs_FlashSpiWP   = {SPI_FLASH_WP, OUTPUT_PUSH_PULL, &gpio_reset};
//
//const SPIIO_PORTS gs_FlashSpiPort = 
//{
//    (gpio_dev_t*)&gs_FlashSpiMISO,       //MISO,SPI主入从出
//    (gpio_dev_t*)&gs_FlashSpiMOSI,       //MOSI,SPI主出从入
//    (gpio_dev_t*)&gs_FlashSpiSCK,        //SCK,SPI时钟线
//    (gpio_dev_t*)&gs_FlashSpiPWR,        //电源脚        
//    (gpio_dev_t*)&gs_FlashSpiCS,         //FLA,FLASH片选
//    (gpio_dev_t*)&gs_FlashSpiWP,         //写保护
//};



/******************************************************************************
**ADGPI端口信息
******************************************************************************/
//const ADItem gs_ADItem[] = 
//{
//    {GPIO_PIN_1, GPIOB,  ADC1, ADC_CHANNEL_16,  3300, 11},//, 1, 0, 1, 0, 0},      //超级电容电压检测
//    {GPIO_PIN_3, GPIOC,  ADC1, ADC_CHANNEL_4,  3300, 1},//, 1, 0, 1, 0, 0},
//    {GPIO_PIN_2, GPIOC,  ADC1, ADC_CHANNEL_3,  3300, 1},//, 1, 0, 1, 0, 0},
//};
//extern ADC_HandleTypeDef hadc1;
//ADPORT gs_ADPort[] =
//{
//    {&hadc1, 3, 0, gs_ADItem},
//};
//
//
//const uint16 AD_PORT_NUM = (sizeof(gs_ADPort) / sizeof(ADPORT));
//
//
////IWDG_HandleTypeDef hiwdg;
//
//LPTIM_HandleTypeDef hlptim1;

//i2c_dev_t brd_i2c1_dev = {AOS_PORT_I2C1, {0}, NULL};
//i2c_dev_t brd_i2c2_dev = {AOS_PORT_I2C2, {0}, NULL};
static void gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    uint32_t value = 0;
//	    hal_gpio_input_get(&input, &value);
//	    hal_gpio_output_toggle(&led);
//	    LOG("GPIO[%u] intr, val: %u\n", gpio_num, value);
} 

//static void brd_peri_init(void)
//{
//    int i;
//    int gpcfg_num = sizeof(brd_gpio_table) / sizeof(brd_gpio_table[0]);
//
//    for (i = 0; i < gpcfg_num; ++i) {
//        hal_gpio_init(&brd_gpio_table[i]);
//    }
//
//    gpcfg_num = sizeof(brd_led_table) / sizeof(brd_led_table[0]);
//
//    for (i = 0; i < gpcfg_num; ++i) {
//        hal_gpio_init(&brd_led_table[i]);
//    }
//
//    gpcfg_num = sizeof(brd_gpi_table) / sizeof(brd_gpi_table[0]);
//
//    for (i = 0; i < gpcfg_num; ++i) {
//        hal_gpio_init(&brd_gpi_table[i]);
//        if(brd_gpi_table[i].config == IRQ_MODE)
//        {
//            /* gpio interrupt config */
//            hal_gpio_enable_irq(&brd_gpi_table[i], IRQ_TRIGGER_BOTH_EDGES, gpio_isr_handler, (void *) &i);
//        }
//    }
////	    Flash_Init();
////	    hal_i2c_init(&brd_i2c1_dev);
////	    hal_i2c_init(&brd_i2c2_dev);
////	    
//}
//void brd_gpio_suspend(void)
//{
//    int i;
//    int gpcfg_num;
//
//    gpcfg_num = sizeof(brd_gpi_LPtable) / sizeof(brd_gpi_LPtable[0]);
//
//    for (i = 0; i < gpcfg_num; ++i) {
//        hal_gpio_init(&brd_gpi_LPtable[i]);
//        
//    }
//}
//
//void brd_gpio_resume(void)
//{
//    int i;
//    int gpcfg_num;
//
//    gpcfg_num = sizeof(brd_gpi_table) / sizeof(brd_gpi_table[0]);
//
//    for (i = 0; i < gpcfg_num; ++i) {
//        hal_gpio_init(&brd_gpi_table[i]);
//        if(brd_gpi_table[i].config == IRQ_MODE)
//        {
//            /* gpio interrupt config */
//            hal_gpio_enable_irq(&brd_gpi_table[i], IRQ_TRIGGER_BOTH_EDGES, gpio_isr_handler, (void *) &i);
//        }
//    }
////	    Flash_Init();
////	    hal_i2c_init(&brd_i2c1_dev);
////	    hal_i2c_init(&brd_i2c2_dev);
////	    
//}


extern void SystemClock_Config(void);
extern void MX_GPIO_Init(void);
void MX_DMA_Init(void);
/* ADC1 init function */
//void MX_ADC1_Init(void)
//{
//  ADC_ChannelConfTypeDef sConfig;
//
//    /**Common config 
//    */
//  hadc1.Instance = ADC1;
//  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
//  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
//  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
//  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
//  hadc1.Init.LowPowerAutoWait = DISABLE;
//  hadc1.Init.ContinuousConvMode = ENABLE;
//  hadc1.Init.NbrOfConversion = 3;
//  hadc1.Init.DiscontinuousConvMode = DISABLE;
//  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
//  hadc1.Init.DMAContinuousRequests = ENABLE;
//  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
//  hadc1.Init.OversamplingMode = DISABLE;
//  HAL_ADC_Init(&hadc1);
//
//    /**Configure Regular Channel 
//    */
//  sConfig.Channel = ADC_CHANNEL_16;
//  sConfig.Rank = 1;
//  sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
//  sConfig.SingleDiff = ADC_SINGLE_ENDED;
//  sConfig.OffsetNumber = ADC_OFFSET_NONE;
//  sConfig.Offset = 0;
//  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
//
//    /**Configure Regular Channel 
//    */
//  sConfig.Channel = ADC_CHANNEL_4;
//  sConfig.Rank = 2;
//  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
//
//    /**Configure Regular Channel 
//    */
//  sConfig.Channel = ADC_CHANNEL_3;
//  sConfig.Rank = 3;
//  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
////	  sConfig.Channel = ADC_CHANNEL_16;
////	  sConfig.Rank = 4;
////	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
//}
//
//
//int32_t MX_ADC1_DeInit(void)
//{
//    int32_t ret = -1;
//
//    /* adc1 deinitialization */
//    ret = HAL_ADC_DeInit(&hadc1);
//
//    return ret;
//}
//
//
//extern aos_sem_t   g_admutex;
///* USER CODE BEGIN 4 */
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
//{
//    aos_sem_signal(&g_admutex);
//	HAL_ADC_Stop_DMA(&hadc1);
//}


//static void MX_LPTIM1_Init(void)
//{
//
//  /* USER CODE BEGIN LPTIM1_Init 0 */
//
//  /* USER CODE END LPTIM1_Init 0 */
//
//  /* USER CODE BEGIN LPTIM1_Init 1 */
//
//  /* USER CODE END LPTIM1_Init 1 */
//  hlptim1.Instance = LPTIM1;
//  hlptim1.Init.Clock.Source = LPTIM_CLOCKSOURCE_APBCLOCK_LPOSC;
//  hlptim1.Init.Clock.Prescaler = LPTIM_PRESCALER_DIV32;
//  hlptim1.Init.Trigger.Source = LPTIM_TRIGSOURCE_SOFTWARE;
//  hlptim1.Init.OutputPolarity = LPTIM_OUTPUTPOLARITY_HIGH;
//  hlptim1.Init.UpdateMode = LPTIM_UPDATE_IMMEDIATE;
//  hlptim1.Init.CounterSource = LPTIM_COUNTERSOURCE_INTERNAL;
//  hlptim1.Init.Input1Source = LPTIM_INPUT1SOURCE_GPIO;
//  hlptim1.Init.Input2Source = LPTIM_INPUT2SOURCE_GPIO;
//  if (HAL_LPTIM_Init(&hlptim1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN LPTIM1_Init 2 */
//
//  /* USER CODE END LPTIM1_Init 2 */
//
//}
//IWDG_HandleTypeDef hiwdg;
//
//static void MX_IWDG_Init(void)
//{
//
//  /* USER CODE BEGIN IWDG_Init 0 */
//
//  /* USER CODE END IWDG_Init 0 */
//
//  /* USER CODE BEGIN IWDG_Init 1 */
//
//  /* USER CODE END IWDG_Init 1 */
//  hiwdg.Instance = IWDG;
//  hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
//  hiwdg.Init.Window = IWDG_WINDOW_DISABLE;
//  hiwdg.Init.Reload = 0xfff; //3s
//  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN IWDG_Init 2 */
//
//  /* USER CODE END IWDG_Init 2 */
//
//}

extern void SystemClock_Config(void);
void SYS_FeedDOG()
{
#ifndef DEBUG        
    HAL_IWDG_Refresh(&hiwdg); //喂狗
#endif

}


//void stm32_soc_init(void)
//{
//    //
////    HAL_Init();
//      /* Configure the system clock */
//    SystemClock_Config();
////	    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/RHINO_CONFIG_TICKS_PER_SECOND);
//
//    /**Configure the Systick 
//    */
//    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
////	
//    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
//    
//    /**Configure the Systick interrupt time */
//    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/RHINO_CONFIG_TICKS_PER_SECOND);
//
//    MX_GPIO_Init();
//
//    MX_DMA_Init();
//
//    MX_ADC1_Init();
//
//    MX_LPTIM1_Init();
//
////	    __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);    
//#ifndef DEBUG    
//    MX_IWDG_Init();
//#endif
//}

void stm32_soc_peripheral_init(void)
{
    /*default uart init*/
//	    stduart_init();
    brd_peri_init();

//	    hal_i2c_init(&brd_i2c1_dev);
}

static void stduart_init(void)
{
    uart_0.port = 0;
    uart_0.config.baud_rate = 115200;
    uart_0.config.data_width = DATA_WIDTH_8BIT;
    uart_0.config.flow_control = FLOW_CONTROL_DISABLED;
    uart_0.config.mode = MODE_TX_RX;
    uart_0.config.parity = NO_PARITY;
    uart_0.config.stop_bits = STOP_BITS_1;

    hal_uart_init(&uart_0);
}

static void pstuart_init(void)
{
    uart_1.port = PORT_UART_PST;
    uart_1.config.baud_rate = 9600;
    uart_1.config.data_width = DATA_WIDTH_8BIT;
    uart_1.config.flow_control = FLOW_CONTROL_DISABLED;
    uart_1.config.mode = MODE_TX_RX;
    uart_1.config.parity = NO_PARITY;
    uart_1.config.stop_bits = STOP_BITS_1;

    hal_uart_init(&uart_1);
}

#include "k_api.h"
/**
* @brief This function handles System tick timer.
*/
extern void HAL_IncTick(void);
void SysTick_Handler(void)
{
  krhino_intrpt_enter();
  HAL_IncTick();
  krhino_tick_proc();
  krhino_intrpt_exit();
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    if(g_ucPutcharEn)
    {
//        if (ch == '\n') {
            //hal_uart_send(&console_uart, (void *)"\r", 1, 30000);
//	            hal_uart_send(&uart_0, (void *)"\r", 1, 30000);
//            SER_SendData(PORT_UART_STD, (void *)"\r", 1, 300);
//        }
        SYS_SER_Write(PORT_UART_STD, &ch, 1, 300);
    }
  return ch;
}

/**
  * @brief  Retargets the C library scanf function to the USART.
  * @param  None
  * @retval None
  */
GETCHAR_PROTOTYPE
{
  /* Place your implementation of fgetc here */
  /* e.g. readwrite a character to the USART2 and Loop until the end of transmission */
  uint8_t ch = EOF;
  int32_t ret = -1;
  
  uint32_t recv_size;
  ret = hal_uart_recv_II(&uart_0, &ch, 1, &recv_size, HAL_WAIT_FOREVER);

  if (ret == 0) {
      return ch;
  } else {
      return -1;
  }
}

extern int32_t hal_spi_init(spi_dev_t *spi);
int32_t hal_spi_init(spi_dev_t *spi) {
    return 0;
}

extern int32_t hal_spi_recv(spi_dev_t *spi, uint8_t *data, uint16_t size, uint32_t timeout);
int32_t hal_spi_recv(spi_dev_t *spi, uint8_t *data, uint16_t size, uint32_t timeout) {
    return 0;
}
extern int32_t hal_spi_send(spi_dev_t *spi, const uint8_t *data, uint16_t size, uint32_t timeout);
int32_t hal_spi_send(spi_dev_t *spi, const uint8_t *data, uint16_t size, uint32_t timeout) {
      return 0;
}

extern int32_t hal_spi_finalize(spi_dev_t *spi);
int32_t hal_spi_finalize(spi_dev_t *spi) {
      return 0;
}

