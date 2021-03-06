/**
  ******************************************************************************
  * File Name          : stm32l4xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization 
  *                      and de-Initialization codes.
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
#include "stm32l4xx_hal.h"
extern DMA_HandleTypeDef hdma_usart1_rx;

extern DMA_HandleTypeDef hdma_usart1_tx;

extern void _Error_Handler(char *, int);
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/
  /* MemoryManagement_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
  /* BusFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  /* UsageFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
  /* SVCall_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
  /* DebugMonitor_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}
/**
* @brief RTC MSP Initialization
* This function configures the hardware resources used in this example
* @param hrtc: RTC handle pointer
* @retval None
*/
#define RTC_CLOCK_SOURCE_LSE
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
//	    UNUSED(hrtc);
//	    return;
    if(hrtc->Instance==RTC)
    {
        /* USER CODE BEGIN RTC_MspInit 0 */

        /* USER CODE END RTC_MspInit 0 */
        /* Peripheral clock enable */
        //	    __HAL_RCC_RTC_ENABLE();
        /* USER CODE BEGIN RTC_MspInit 1 */

        /* USER CODE END RTC_MspInit 1 */
        RCC_OscInitTypeDef        RCC_OscInitStruct;
        RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

        /*##-1- Configure LSE as RTC clock source ###################################*/ 
#ifdef RTC_CLOCK_SOURCE_LSE

        RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
        RCC_OscInitStruct.LSEState = RCC_LSE_ON;
        RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
        if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        { 
            /* Initialization Error */
            Error_Handler();
        }

        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
        PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
        if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        { 
            /* Initialization Error */
            Error_Handler();
        }
#elif defined (RTC_CLOCK_SOURCE_LSI)  
        RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
        RCC_OscInitStruct.LSIState = RCC_LSI_ON;
        RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
        if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
        {
            /* Initialization Error */
            Error_Handler();
        }

        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
        PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
        if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        { 
            /* Initialization Error */
            Error_Handler();
        }
#else
#error Please select the RTC Clock source inside the main.h file
#endif /*RTC_CLOCK_SOURCE_LSE*/

        /*##-2- Enable RTC peripheral Clocks #######################################*/ 
        /* Enable RTC Clock */ 
        __HAL_RCC_RTC_ENABLE(); 

        /*##-3- Configure the NVIC for RTC Alarm ###################################*/
        //HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0x0F, 0);
        //HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);  
    }

}

/**
* @brief RTC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hrtc: RTC handle pointer
* @retval None
*/
void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
{
  if(hrtc->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */
  
    /**I2C1 GPIO Configuration    
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }

}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{

  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();
  
    /**I2C1 GPIO Configuration    
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

    GPIO_InitTypeDef GPIO_InitStruct;
    if(huart->Instance==USART1)
    {
        /* USER CODE BEGIN USART1_MspInit 0 */

        /* USER CODE END USART1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        /**USART1 GPIO Configuration    
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        //	    /* USART1 DMA Init */
        //	    /* USART1_RX Init */
        //	    hdma_usart1_rx.Instance = DMA1_Channel5;
        //	    hdma_usart1_rx.Init.Request = DMA_REQUEST_2;
        //	    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        //	    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        //	    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
        //	    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        //	    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        //	    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
        //	    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
        //	    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
        //	    {
        //	      _Error_Handler(__FILE__, __LINE__);
        //	    }
        //	
        //	    __HAL_LINKDMA(huart,hdmarx,hdma_usart1_rx);
        //	
        //	    /* USART1_TX Init */
        //	    hdma_usart1_tx.Instance = DMA1_Channel4;
        //	    hdma_usart1_tx.Init.Request = DMA_REQUEST_2;
        //	    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        //	    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        //	    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
        //	    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        //	    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        //	    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
        //	    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
        //	    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
        //	    {
        //	      _Error_Handler(__FILE__, __LINE__);
        //	    }
        //	
        //	    __HAL_LINKDMA(huart,hdmatx,hdma_usart1_tx);

        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        /* USER CODE BEGIN USART1_MspInit 1 */

        /* USER CODE END USART1_MspInit 1 */
    }
    else if(huart->Instance==UART4)
    {
        /* USER CODE BEGIN USART1_MspInit 0 */

        /* USER CODE END USART1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_UART4_CLK_ENABLE();

        /**USART1 GPIO Configuration    
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART4 DMA Init */
        /* USART4_RX Init */
//	        hdma_usart1_rx.Instance = DMA2_Channel5;
//	        hdma_usart1_rx.Init.Request = DMA_REQUEST_2;
//	        hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//	        hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
//	        hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
//	        hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//	        hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//	        hdma_usart1_rx.Init.Mode = DMA_NORMAL;
//	        hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
//	        if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
//	        {
//	          _Error_Handler(__FILE__, __LINE__);
//	        }
//		
//	        __HAL_LINKDMA(huart,hdmarx,hdma_usart1_rx);
//		
//	        /* USART1_TX Init */
//	        hdma_usart1_tx.Instance = DMA2_Channel3;
//	        hdma_usart1_tx.Init.Request = DMA_REQUEST_2;
//	        hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
//	        hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
//	        hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
//	        hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//	        hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//	        hdma_usart1_tx.Init.Mode = DMA_NORMAL;
//	        hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
//	        if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
//	        {
//	          _Error_Handler(__FILE__, __LINE__);
//	        }
//		
//	        __HAL_LINKDMA(huart,hdmatx,hdma_usart1_tx);

        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(UART4_IRQn);
        /* USER CODE BEGIN USART4_MspInit 1 */

        /* USER CODE END USART4_MspInit 1 */
    }
    else if(huart->Instance==USART2)
    {
        /* USER CODE BEGIN USART2_MspInit 0 */

        /* USER CODE END USART2_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        /**USART2 GPIO Configuration    
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* USART2 interrupt Init */
        HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
        /* USER CODE BEGIN USART2_MspInit 1 */

        /* USER CODE END USART2_MspInit 1 */
    }
    else if(huart->Instance==USART3)
    {
        /* USER CODE BEGIN USART3_MspInit 0 */

        /* USER CODE END USART3_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();

        /**USART3 GPIO Configuration    
        PC10     ------> USART3_TX
        PC11     ------> USART3_RX 
        */
        GPIO_InitStruct.Pin = PST_TX_Pin|GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USART2 interrupt Init */
        HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
        /* USER CODE BEGIN USART3_MspInit 1 */

        /* USER CODE END USART3_MspInit 1 */
    }
    else if(huart->Instance==UART5)
    {
        /* USER CODE BEGIN UART5_MspInit 0 */

        /* USER CODE END UART5_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_UART5_CLK_ENABLE();

        /**USART5 GPIO Configuration    
        PC12     ------> UART5_TX
        PD02     ------> UART5_RX 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* USART2 interrupt Init */
        HAL_NVIC_SetPriority(UART5_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(UART5_IRQn);
        /* USER CODE BEGIN UART5_MspInit 1 */

        /* USER CODE END UART5_MspInit 1 */
    }
    else if(huart->Instance==LPUART1)
    {
        /* USER CODE BEGIN LPUART1_MspInit 0 */

        /* USER CODE END LPUART1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_LPUART1_CLK_ENABLE();

        /**USART5 GPIO Configuration    
        PC1     ------> UART5_TX
        PC0     ------> UART5_RX 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_LPUART1;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        
//	        GPIO_InitStruct.Pin = GPIO_PIN_0;
//	        GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//	        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* USART2 interrupt Init */
        HAL_NVIC_SetPriority(LPUART1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(LPUART1_IRQn);
        /* USER CODE BEGIN UART5_MspInit 1 */

        /* USER CODE END UART5_MspInit 1 */
    }

}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if(huart->Instance==USART1)
    {
        /* USER CODE BEGIN USART1_MspDeInit 0 */

        /* USER CODE END USART1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration    
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX 
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

        /* USART1 DMA DeInit */
        HAL_DMA_DeInit(huart->hdmarx);
        HAL_DMA_DeInit(huart->hdmatx);

        /* USART1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
        /* USER CODE BEGIN USART1_MspDeInit 1 */

        /* USER CODE END USART1_MspDeInit 1 */
    }
    else if(huart->Instance==USART2)
    {
        /* USER CODE BEGIN USART2_MspDeInit 0 */

        /* USER CODE END USART2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration    
        PD5     ------> USART2_TX
        PD6     ------> USART2_RX 
        */
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5|GPIO_PIN_6);

        /* USART2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(USART2_IRQn);
        /* USER CODE BEGIN USART2_MspDeInit 1 */

        /* USER CODE END USART2_MspDeInit 1 */
    }
    else if(huart->Instance==USART3)
    {
        /* USER CODE BEGIN USART2_MspDeInit 0 */

        /* USER CODE END USART2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART3_CLK_DISABLE();

        /**USART2 GPIO Configuration    
        PB10     ------> USART2_TX
        PB11     ------> USART2_RX 
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

        /* USART2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(USART3_IRQn);
        /* USER CODE BEGIN USART2_MspDeInit 1 */

        /* USER CODE END USART2_MspDeInit 1 */
    }
    else if(huart->Instance==UART4)
    {
        /* USER CODE BEGIN USART2_MspDeInit 0 */

        /* USER CODE END USART2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_UART4_CLK_DISABLE();

        /**USART2 GPIO Configuration    
        PD5     ------> USART2_TX
        PD6     ------> USART2_RX 
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1);
//	        GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
//	        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//	        GPIO_InitStruct.Pull = GPIO_PULLUP;
//	        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	        
//	        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_SET);   

        /* USART2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(UART4_IRQn);
        /* USER CODE BEGIN USART2_MspDeInit 1 */

        /* USER CODE END USART2_MspDeInit 1 */
    }
    else if(huart->Instance==UART5)
    {
        /* USER CODE BEGIN USART2_MspDeInit 0 */

        /* USER CODE END USART2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_UART5_CLK_DISABLE();

        /**USART2 GPIO Configuration    
        PC12     ------> USART2_TX
        PD2     ------> USART2_RX 
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);
        /* USART2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(UART5_IRQn);
        /* USER CODE BEGIN USART2_MspDeInit 1 */

        /* USER CODE END USART2_MspDeInit 1 */
    }
    else if(huart->Instance==LPUART1)
    {
        /* USER CODE BEGIN USART2_MspDeInit 0 */

        /* USER CODE END USART2_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_LPUART1_CLK_DISABLE();

        /**USART2 GPIO Configuration    
        PC12     ------> USART2_TX
        PD2     ------> USART2_RX 
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_1);
//	        GPIO_InitStruct.Pin = GPIO_PIN_0 |GPIO_PIN_1;
//	        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//	        GPIO_InitStruct.Pull = GPIO_PULLUP;
//	        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//	        
//	        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0 
//	                                |GPIO_PIN_1, GPIO_PIN_SET);
        
        /* USART2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(LPUART1_IRQn);
        /* USER CODE BEGIN USART2_MspDeInit 1 */

        /* USER CODE END USART2_MspDeInit 1 */
    }

}
//	void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
//	{
//	  /* Prevent unused argument(s) compilation warning */
//	    __HAL_RCC_ADC_CLK_ENABLE();
//	  /* NOTE : This function should not be modified. When the callback is needed,
//	            function HAL_ADC_MspInit must be implemented in the user file.
//	   */ 
//	}
//	
//	/* USER CODE BEGIN 1 */
//	void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
//	{
//	
//	  if(adcHandle->Instance==ADC1)
//	  {
//	  /* USER CODE BEGIN ADC1_MspDeInit 0 */
//	
//	  /* USER CODE END ADC1_MspDeInit 0 */
//	    /* Peripheral clock disable */
//	    __HAL_RCC_ADC_CLK_DISABLE();
//	  
//	    /**ADC1 GPIO Configuration    
//	    PB0     ------> ADC1_IN15 
//	    */
//	//	    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);
//	
//	  /* USER CODE BEGIN ADC1_MspDeInit 1 */
//	
//	  /* USER CODE END ADC1_MspDeInit 1 */
//	  }
//	} 
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_ADC_CLK_ENABLE();
  
    /**ADC1 GPIO Configuration    
    PA0     ------> ADC1_IN5
    PA1     ------> ADC1_IN6
    PB1     ------> ADC1_IN16 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* Peripheral DMA init*/
  
    hdma_adc1.Instance = DMA1_Channel1;
    hdma_adc1.Init.Request = DMA_REQUEST_0;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&hdma_adc1);

    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc1);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{

  if(hadc->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC_CLK_DISABLE();
    __HAL_RCC_DMA1_CLK_DISABLE();
    /**ADC1 GPIO Configuration    
    PA0     ------> ADC1_IN5
    PA1     ------> ADC1_IN6
    PB1     ------> ADC1_IN16 
    */
//	    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_1);
//	
//	    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_2 | GPIO_PIN_3);

    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(hadc->DMA_Handle);
    HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
  }
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
} 



/**
* @brief LPTIM MSP Initialization
* This function configures the hardware resources used in this example
* @param hlptim: LPTIM handle pointer
* @retval None
*/
void HAL_LPTIM_MspInit(LPTIM_HandleTypeDef* hlptim)
{
  if(hlptim->Instance==LPTIM1)
  {
  /* USER CODE BEGIN LPTIM1_MspInit 0 */

  /* USER CODE END LPTIM1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_LPTIM1_CLK_ENABLE();
    /* LPTIM1 interrupt Init */
    HAL_NVIC_SetPriority(LPTIM1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(LPTIM1_IRQn);
  /* USER CODE BEGIN LPTIM1_MspInit 1 */

  /* USER CODE END LPTIM1_MspInit 1 */
  }

}

/**
* @brief LPTIM MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hlptim: LPTIM handle pointer
* @retval None
*/
void HAL_LPTIM_MspDeInit(LPTIM_HandleTypeDef* hlptim)
{
  if(hlptim->Instance==LPTIM1)
  {
  /* USER CODE BEGIN LPTIM1_MspDeInit 0 */

  /* USER CODE END LPTIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_LPTIM1_CLK_DISABLE();

    /* LPTIM1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(LPTIM1_IRQn);
  /* USER CODE BEGIN LPTIM1_MspDeInit 1 */

  /* USER CODE END LPTIM1_MspDeInit 1 */
  }

}

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
