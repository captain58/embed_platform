
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_hal.h"
#include "k_api.h"
#include "aos/kernel.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
int SystemClock_Stop(void);

void MX_GPIO_Init(void);
void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C1_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
extern void USART_DMA_TX_IRQHandler(const void* uartIns);
/**
* @brief This function handles DMA1 channel4 global interrupt.
*/
void DMA1_Channel4_IRQHandler(void)
{
    krhino_intrpt_enter();
    USART_DMA_TX_IRQHandler(USART1);
    krhino_intrpt_exit();
}
void USART_DMA_RX_IRQHandler(const void* uartIns);
/**
* @brief This function handles DMA1 channel5 global interrupt.
*/
void DMA1_Channel5_IRQHandler(void)
{
    krhino_intrpt_enter();
    USART_DMA_RX_IRQHandler(USART1);
    krhino_intrpt_exit();
}
void DMA2_Channel3_IRQHandler(void)
{
    krhino_intrpt_enter();
    USART_DMA_TX_IRQHandler(UART4);
    krhino_intrpt_exit();
}
void USART_DMA_RX_IRQHandler(const void* uartIns);
/**
* @brief This function handles DMA1 channel5 global interrupt.
*/
void DMA2_Channel5_IRQHandler(void)
{
    krhino_intrpt_enter();
    USART_DMA_RX_IRQHandler(UART4);
    krhino_intrpt_exit();
}
extern DMA_HandleTypeDef hdma_adc1;
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */
  krhino_intrpt_enter();

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */
  krhino_intrpt_exit();

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}
/**
  * @brief This function handles LPTIM1 global interrupt.
  */
extern LPTIM_HandleTypeDef hlptim1;
void LPTIM1_IRQHandler(void)
{
  /* USER CODE BEGIN LPTIM1_IRQn 0 */
  krhino_intrpt_enter();

  /* USER CODE END LPTIM1_IRQn 0 */
  HAL_LPTIM_IRQHandler(&hlptim1);
  /* USER CODE BEGIN LPTIM1_IRQn 1 */
  krhino_intrpt_exit();
  /* USER CODE END LPTIM1_IRQn 1 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
//	void SystemClock_Config(void)
//	{
//	
//	  RCC_OscInitTypeDef RCC_OscInitStruct;
//	  RCC_ClkInitTypeDef RCC_ClkInitStruct;
//	  RCC_PeriphCLKInitTypeDef PeriphClkInit;
//	
//	    /**Initializes the CPU, AHB and APB busses clocks 
//	    */
//	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//	  RCC_OscInitStruct.HSICalibrationValue = 16;
//	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//	  RCC_OscInitStruct.PLL.PLLM = 1;
//	  RCC_OscInitStruct.PLL.PLLN = 10;
//	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
//	  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
//	  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
//	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//	  {
//	    _Error_Handler(__FILE__, __LINE__);
//	  }
//	
//	    /**Initializes the CPU, AHB and APB busses clocks 
//	    */
//	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//	
//	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
//	  {
//	    _Error_Handler(__FILE__, __LINE__);
//	  }
//	
//	  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
//	                              |RCC_PERIPHCLK_I2C1;
//	  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
//	  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
//	  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
//	  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//	  {
//	    _Error_Handler(__FILE__, __LINE__);
//	  }
//	
//	    /**Configure the main internal regulator output voltage 
//	    */
//	  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
//	  {
//	    _Error_Handler(__FILE__, __LINE__);
//	  }
//	
//	    /**Configure the Systick interrupt time 
//	    */
//	  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
//	
//	    /**Configure the Systick 
//	    */
//	  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
//	
//	  /* SysTick_IRQn interrupt configuration */
//	  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
//	}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
//	  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//	  //80M
//	  RCC_OscInitStruct.PLL.PLLM = 1;
//	  RCC_OscInitStruct.PLL.PLLN = 20;
//	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
//	  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
//	  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
//40M
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;  
//16M
//	  RCC_OscInitStruct.PLL.PLLM = 1;
//	  RCC_OscInitStruct.PLL.PLLN = 8;
//	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
//	  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
//	  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                                 | RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_UART5
                                 | RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_ADC
                                 | RCC_PERIPHCLK_LPTIM1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;   
    PeriphClkInit.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;    
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
//	    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  
//	    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_LPUART1;
    PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;  
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
//	  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
    PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
    
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

//	  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_LSE, RCC_MCODIV_16);
//	  HAL_RCCEx_EnableLSCO(RCC_LSCOSOURCE_LSE);
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

//	  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/RHINO_CONFIG_TICKS_PER_SECOND);

    /**Configure the Systick 
    */
//	    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  
//	  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


int SystemClock_Stop(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure LSE Drive Capability 
  */
//  HAL_PWR_EnableBkUpAccess();
//  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
//	  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//	  //80M
//	  RCC_OscInitStruct.PLL.PLLM = 1;
//	  RCC_OscInitStruct.PLL.PLLN = 20;
//	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
//	  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
//	  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
//40M
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;  
//16M
//	  RCC_OscInitStruct.PLL.PLLM = 1;
//	  RCC_OscInitStruct.PLL.PLLN = 8;
//	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
//	  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
//	  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return HAL_ERROR;
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    return HAL_ERROR;
  }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                                 | RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_UART4|RCC_PERIPHCLK_UART5
                                 | RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_LPUART1|RCC_PERIPHCLK_ADC
                                 | RCC_PERIPHCLK_LPTIM1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
    PeriphClkInit.Uart4ClockSelection = RCC_UART4CLKSOURCE_PCLK1;   
    PeriphClkInit.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;    
    PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
//	    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  
//	    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_LPUART1;
    PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_LSE;  
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
//	  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
    PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSE;
    
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    return HAL_ERROR;
  }

  //HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_SYSCLK, RCC_MCODIV_1);
//  HAL_RCCEx_EnableLSCO(RCC_LSCOSOURCE_LSI);
  /** Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}


//	void SystemClock_Config(void)
//	{
//	  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//	  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//	  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
//	
//	  /** Configure LSE Drive Capability 
//	  */
//	  HAL_PWR_EnableBkUpAccess();
//	  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
//	  /** Initializes the CPU, AHB and APB busses clocks 
//	  */
//	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_LSE
//	                              |RCC_OSCILLATORTYPE_MSI;
//	  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
//	  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
//	  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
//	  RCC_OscInitStruct.MSICalibrationValue = 0;
//	  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
//	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//	  {
//	    Error_Handler();
//	  }
//	  /** Initializes the CPU, AHB and APB busses clocks 
//	  */
//	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
//	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//	
//	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
//	  {
//	    Error_Handler();
//	  }
//	  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
//	  PeriphClkInit.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSI;
//	  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//	  {
//	    Error_Handler();
//	  }
//	  /** Configure the main internal regulator output voltage 
//	  */
//	  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
//	  {
//	    Error_Handler();
//	  }
//	  /** Enable MSI Auto calibration 
//	  */
//	  HAL_RCCEx_EnableMSIPLLMode();
//	}


/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10909CEC;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
//	    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel4_IRQn interrupt configuration */
//	    HAL_NVIC_SetPriority(DMA2_Channel3_IRQn, 0, 0);
//	    HAL_NVIC_EnableIRQ(DMA2_Channel3_IRQn);
//	    /* DMA1_Channel5_IRQn interrupt configuration */
//	    HAL_NVIC_SetPriority(DMA2_Channel5_IRQn, 0, 0);
//	    HAL_NVIC_EnableIRQ(DMA2_Channel5_IRQn);
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}
void MX_DMA_DeInit(void) 
{
  /* DMA controller clock enable */
//	    __HAL_RCC_DMA2_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_DISABLE();

    /* DMA interrupt init */
    /* DMA1_Channel4_IRQn interrupt configuration */
//	    HAL_NVIC_SetPriority(DMA2_Channel3_IRQn, 0, 0);
//	    HAL_NVIC_EnableIRQ(DMA2_Channel3_IRQn);
//	    /* DMA1_Channel5_IRQn interrupt configuration */
//	    HAL_NVIC_SetPriority(DMA2_Channel5_IRQn, 0, 0);
//	    HAL_NVIC_EnableIRQ(DMA2_Channel5_IRQn);
    //HAL_DMA_DeInit();

//	    HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn, 0, 0);
    
    HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);

}

//	extern ADC_HandleTypeDef hadc1;
/* ADC1 init function */
//	void MX_ADC1_Init(void)
//	{
//	  ADC_ChannelConfTypeDef sConfig;
//	
//	    /**Common config 
//	    */
//	  hadc1.Instance = ADC1;
//	  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
//	  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
//	  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//	  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
//	  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
//	  hadc1.Init.LowPowerAutoWait = DISABLE;
//	  hadc1.Init.ContinuousConvMode = ENABLE;
//	  hadc1.Init.NbrOfConversion = 4;
//	  hadc1.Init.DiscontinuousConvMode = DISABLE;
//	  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//	  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
//	  hadc1.Init.DMAContinuousRequests = ENABLE;
//	  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
//	  hadc1.Init.OversamplingMode = DISABLE;
//	  HAL_ADC_Init(&hadc1);
//	
//	    /**Configure Regular Channel 
//	    */
//	  sConfig.Channel = ADC_CHANNEL_5;
//	  sConfig.Rank = 1;
//	  sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
//	  sConfig.SingleDiff = ADC_SINGLE_ENDED;
//	  sConfig.OffsetNumber = ADC_OFFSET_NONE;
//	  sConfig.Offset = 0;
//	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
//	
//	    /**Configure Regular Channel 
//	    */
//	  sConfig.Channel = ADC_CHANNEL_6;
//	  sConfig.Rank = 2;
//	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
//	
//	    /**Configure Regular Channel 
//	    */
//	  sConfig.Channel = ADC_CHANNEL_VREFINT;
//	  sConfig.Rank = 3;
//	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
//	  sConfig.Channel = ADC_CHANNEL_16;
//	  sConfig.Rank = 4;
//	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
//	}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
//	void MX_GPIO_Init(void)
//	{
//	  GPIO_InitTypeDef GPIO_InitStruct = {0};
//		
//	  /* GPIO Ports Clock Enable */
//	  __HAL_RCC_GPIOE_CLK_ENABLE();
//	  __HAL_RCC_GPIOC_CLK_ENABLE();
//	  __HAL_RCC_GPIOH_CLK_ENABLE();
//	  __HAL_RCC_GPIOA_CLK_ENABLE();
//	  __HAL_RCC_GPIOB_CLK_ENABLE();
//	  __HAL_RCC_GPIOD_CLK_ENABLE();
//		
//	  /*Configure GPIO pins : PE2 PE3 PE4 PE5 
//	                           PE6 PE7 PE8 PE9 
//	                           PE10 PE11 PE12 PE13 
//	                           PE14 PE15 PE0 PE1 
//	                           GPIO_PIN_8|
//	                           GPIO_PIN_12|
//	                           GPIO_PIN_2|*/
//	                           
//	  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
//	                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9 
//	                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_13 
//	                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
//	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//	  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
//		
//	  /*Configure GPIO pins : PC13 PC0 PC1 PC2 
//	                           PC3 PC4 PC5 PC6 
//	                           PC7 PC8 PC9 PC10 
//	                           PC11 PC12 */
//	//	                           GPIO_PIN_2 
//	//	                          |GPIO_PIN_3||GPIO_PIN_6 
//	//	                          |GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_5
//	  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_4
//	                        |GPIO_PIN_8|GPIO_PIN_10 
//	                          |GPIO_PIN_11|GPIO_PIN_12;
//	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//	  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//		
//	  /*Configure GPIO pins : PA0 PA1 PA2 PA3 
//	                           PA4 PA5 PA6 PA7 
//	                           PA8 PA9 PA10 PA11 
//	                           PA12 PA15 
//	                           GPIO_PIN_6|
//	                            |GPIO_PIN_4|GPIO_PIN_5 |GPIO_PIN_8*/
//	  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
//	                         |GPIO_PIN_7 
//	                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
//	                          |GPIO_PIN_12|GPIO_PIN_15;
//	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//	  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//		
//	  /*Configure GPIO pins : PB0 PB1 PB2 PB10 
//	                           PB11 PB12 PB13 PB14 
//	                           PB15 PB3 PB4 PB5 
//	                           PB6 PB7 PB8 PB9 */
//	//	GPIO_PIN_1|      GPIO_PIN_4|      |GPIO_PIN_6      
//	//	  |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 
//	//	                            |GPIO_PIN_15
//		
//	  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_10 
//	                          |GPIO_PIN_11|GPIO_PIN_3|GPIO_PIN_5 
//	                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
//	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//	  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//		
//	  /*Configure GPIO pins : PD8 PD9 PD10 PD11 
//	                           PD12 PD13 PD14 PD15 
//	                           PD0 PD1 PD2 PD3 
//	                           PD4 PD5 PD6 PD7 
//	                           |GPIO_PIN_7
//	                           GPIO_PIN_8|*/
//	  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
//	                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
//	                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
//	                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
//	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//	  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
//		
//	}
//	void MX_GPIO_Init(void)
//	{
//	  GPIO_InitTypeDef GPIO_InitStruct = {0};
//	
//	  /* GPIO Ports Clock Enable */
//	  __HAL_RCC_GPIOE_CLK_ENABLE();
//	  __HAL_RCC_GPIOC_CLK_ENABLE();
//	  __HAL_RCC_GPIOH_CLK_ENABLE();
//	  __HAL_RCC_GPIOA_CLK_ENABLE();
//	  __HAL_RCC_GPIOB_CLK_ENABLE();
//	  __HAL_RCC_GPIOD_CLK_ENABLE();
//	
//	  /*Configure GPIO pins : PE2 PE3 PE4 PE5 
//	                           PE6 PE7 PE8 PE9 
//	                           PE10 PE11 PE12 PE13 
//	                           PE14 PE15 PE0 PE1 */
//	  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
//	                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9 
//	                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 
//	                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
//	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//	  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
//	
//	  /*Configure GPIO pins : PC13 PC0 PC1 PC2 
//	                           PC3 PC4 PC5 PC6 
//	                           PC7 PC8 PC9 PC10 
//	                           PC11 PC12 */
//	  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2 
//	                          |GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6 
//	                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10 
//	                          |GPIO_PIN_11|GPIO_PIN_12;
//	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//	  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//	
//	  /*Configure GPIO pins : PA0 PA1 PA2 PA3 
//	                           PA4 PA5 PA6 PA7 
//	                           PA8 PA9 PA10 PA11 
//	                           PA12 PA15 */
//	  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
//	                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
//	                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
//	                          |GPIO_PIN_12|GPIO_PIN_15;
//	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//	  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	
//	  /*Configure GPIO pins : PB0 PB1 PB2 PB10 
//	                           PB11 PB12 PB13 PB14 
//	                           PB15 PB3 PB4 PB5 
//	                           PB6 PB7 PB8 PB9 */
//	  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10 
//	                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 
//	                          |GPIO_PIN_15|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
//	                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
//	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//	  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//	
//	  /*Configure GPIO pins : PD8 PD9 PD10 PD11 
//	                           PD12 PD13 PD14 PD15 
//	                           PD0 PD1 PD2 PD3 
//	                           PD4 PD5 PD6 PD7 */
//	  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
//	                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
//	                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
//	                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
//	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//	  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
//	
//	}

void MX_GPIO_Init(void)
{
      GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
	
  /*Configure GPIO pins : PE2 PE3 PE4 PE5 
                           PE6 PE7 PE8 PE9 
                           PE10 PE11 PE12 PE13 
                           PE14 PE15 PE0 PE1 */
                           //|GPIO_PIN_8
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9 
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
      
  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9 
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_SET);
	
	
  /*Configure GPIO pins : PC13 PC0 
                           PC1 PC2 PC3 PC4 
                           PC5 PC6 PC7 PC8 
                           PC9 PC10 PC11 PC12 */
                           //GPIO_PIN_2|GPIO_PIN_3|
//	GPIO_PIN_0 
//	                          |GPIO_PIN_1|     GPIO_PIN_5|                      
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_4 
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8 
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13|GPIO_PIN_4 
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8 
                          |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_SET);
	
  /*Configure GPIO pins : PA0 PA1 PA2 PA3 
                           PA4 PA5 PA6 PA7 
                           PA8 PA9 PA10 PA11 
                           PA12 PA15 */
//GPIO_PIN_0|GPIO_PIN_1|   GPIO_PIN_2|                        
  GPIO_InitStruct.Pin = GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_SET);   
	
  /*Configure GPIO pins : PB0 PB1 PB2 PB10 
                           PB11 PB12 PB13 PB14 
                           PB15 PB3 PB4 PB5 
                           PB6 PB7 PB8 PB9 */
                           //GPIO_PIN_1||GPIO_PIN_6
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_10 
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 
                          |GPIO_PIN_15|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_10 
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 
                          |GPIO_PIN_15|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_SET);
	
  /*Configure GPIO pins : PD8 PD9 PD10 PD11 
                           PD12 PD13 PD14 PD15 
                           PD0 PD1 PD2 PD3 
                           PD4 PD5 PD6 PD7 */
                           //|GPIO_PIN_7
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_SET);
	
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6, GPIO_PIN_RESET);  
	
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8, GPIO_PIN_RESET);  
	
    
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET); 
    
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9, GPIO_PIN_RESET);  
    
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7, GPIO_PIN_RESET);  
    //| GPIO_PIN_12
    GPIO_InitStruct.Pin = GPIO_PIN_2 ;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2, GPIO_PIN_RESET);     
	
//	    
//	    GPIO_InitStruct.Pull = GPIO_PULLUP;
//	    GPIO_InitStruct.Pin = GPIO_PIN_3;
//	    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//	    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3, GPIO_PIN_SET);  
    
}
/* USER CODE BEGIN 4 */
//	void MX_GPIO_Init(void)
//	{
//	  GPIO_InitTypeDef GPIO_InitStruct = {0};
//		
//	  /* GPIO Ports Clock Enable */
//	  __HAL_RCC_GPIOE_CLK_ENABLE();
//	  __HAL_RCC_GPIOC_CLK_ENABLE();
//	  __HAL_RCC_GPIOH_CLK_ENABLE();
//	  __HAL_RCC_GPIOA_CLK_ENABLE();
//	  __HAL_RCC_GPIOB_CLK_ENABLE();
//	  __HAL_RCC_GPIOD_CLK_ENABLE();
//	  
//	  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
//	                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
//	                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
//	                          |GPIO_PIN_12|GPIO_PIN_15;
//	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//	  GPIO_InitStruct.Pull = GPIO_PULLUP;
//	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
//	                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
//	                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
//	                          |GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_SET);  
//	  
//	  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
//	                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
//	                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
//	                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
//	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
//	                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
//	                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
//	                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
//	                          
//	  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
//	                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
//	                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
//	                          |GPIO_PIN_12|GPIO_PIN_13;
//	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
//	                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
//	                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
//	                          |GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_SET);
//	                          
//	  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
//	                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
//	                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
//	                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
//	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
//	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
//	                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
//	                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
//	                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);//GPIO_PIN_RESET);
//	    
//	  
//	  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
//	                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
//	                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
//	                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
//	  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
//	  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
//	                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 
//	                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
//	                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
//		
//	  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//	  GPIO_InitStruct.Pin = GPIO_PIN_6| GPIO_PIN_8;
//	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6| GPIO_PIN_8, GPIO_PIN_RESET);    
//	  
//	  GPIO_InitStruct.Pin = GPIO_PIN_3| GPIO_PIN_4|GPIO_PIN_6;
//	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3| GPIO_PIN_4|GPIO_PIN_6, GPIO_PIN_RESET);  
//	  
//	  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9;
//	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9, GPIO_PIN_RESET);  
//	  
//	  GPIO_InitStruct.Pin = GPIO_PIN_7;
//	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
//	  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7, GPIO_PIN_RESET);  
//	  
//	  GPIO_InitStruct.Pin = GPIO_PIN_2;
//	  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
//	  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_2, GPIO_PIN_RESET);  
//	}




/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
