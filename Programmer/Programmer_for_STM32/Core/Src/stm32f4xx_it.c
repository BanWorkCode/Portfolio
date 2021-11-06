/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdio.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "cmsis_os.h"
#include "esp8266_lib.h"
#include "ringBuffer.h"
#include "displayMenu.h"
#ifdef DEBUG
#include "EventRecorder.h" //отладка
#endif /* DEBUG */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
BaseType_t xHigherPriorityTaskWoken;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_sdio_rx;
extern DMA_HandleTypeDef hdma_sdio_tx;
extern SD_HandleTypeDef hsd;
extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;
extern TIM_HandleTypeDef htim1;

/* USER CODE BEGIN EV */
extern osSemaphoreId semRingBufHandle;

void prvGetRegistersFromStack( uint32_t *pulFaultStackAddress )
{
   /* Здесь используется volatile в попытке предотвратить оптимизацию
      компилятора/линкера, которые любят выбрасывать переменные,
      которые реально нигде не используются. Если отладчик не показывает
      значение этих переменных, то сделайте их глобальными, вытащив
      их декларацию за пределы этой функции. */
   volatile uint32_t r0x;
   volatile uint32_t r1x;
   volatile uint32_t r2x;
   volatile uint32_t r3x;
   volatile uint32_t r12x;
   volatile uint32_t lrx; /* регистр связи (Link Register) */
   volatile uint32_t pcx; /* программный счетчик (Program Counter) */
   volatile uint32_t psrx;/* регистр состояния (Program Status Register) */
 
   r0x = pulFaultStackAddress[ 0 ];
   r1x = pulFaultStackAddress[ 1 ];
   r2x = pulFaultStackAddress[ 2 ];
   r3x = pulFaultStackAddress[ 3 ];
 
   r12x = pulFaultStackAddress[ 4 ];
   lrx = pulFaultStackAddress[ 5 ];
   pcx = pulFaultStackAddress[ 6 ];
   psrx = pulFaultStackAddress[ 7 ];
 
   /* Когда выполнение дойдет до этой точки, переменные будут
      содержать значения регистров. */
   for( ;; );
}

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
//void HardFault_Handler(void)
//{
//  /* USER CODE BEGIN HardFault_IRQn 0 */

//  /* USER CODE END HardFault_IRQn 0 */
//  while (1)
//  {
//    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
//				printf("HardFault\r\n");
//    /* USER CODE END W1_HardFault_IRQn 0 */
//  }
//}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
	printf("MemManage_Handler\r\n");
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
//void BusFault_Handler(void)
//{
//  /* USER CODE BEGIN BusFault_IRQn 0 */
//	
//	   __asm volatile
//   (

//      " tst lr, #4                                                n"
//   );
//		printf("BusFault_\r\n");
//  /* USER CODE END BusFault_IRQn 0 */
//  while (1)
//  {
//    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
//    /* USER CODE END W1_BusFault_IRQn 0 */
//  }
//}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles ADC1 global interrupt.
  */
void ADC_IRQHandler(void)
{
  /* USER CODE BEGIN ADC_IRQn 0 */
	
		batteryVoltage = HAL_ADC_GetValue(&hadc1);
return;
  /* USER CODE END ADC_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  /* USER CODE BEGIN ADC_IRQn 1 */

  /* USER CODE END ADC_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
  */
void TIM1_UP_TIM10_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
	
	TIM3->SR &= ~0x01;
	
	if (g_blinkinCursor){
				g_blinkinCursor=0;
				}
				else{
				g_blinkinCursor=1U;
				}
	
return;
  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles SDIO global interrupt.
  */
void SDIO_IRQHandler(void)
{
  /* USER CODE BEGIN SDIO_IRQn 0 */

  /* USER CODE END SDIO_IRQn 0 */
  HAL_SD_IRQHandler(&hsd);
  /* USER CODE BEGIN SDIO_IRQn 1 */

  /* USER CODE END SDIO_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream1 global interrupt.
  */
void DMA2_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream1_IRQn 0 */
	
		
		//половина буфера
	if (DMA2->LISR & DMA_LISR_HTIF1){
	
	
		
#ifdef DEBUG
EventRecord2 (0xFF, 0x05, 0);///!!! отладка	
#endif /* DEBUG */
		
	DMA2->LIFCR |= DMA_LIFCR_CHTIF1;
		
					xHigherPriorityTaskWoken = pdFALSE;
				xSemaphoreGiveFromISR( semRingBufHandle, &xHigherPriorityTaskWoken );
		
		
	}
	
	//конец буфера
if (DMA2->LISR & DMA_LISR_TCIF1){
#ifdef DEBUG
EventRecord2 (0xFF, 0x06, 0);///!!! отладка	
#endif /* DEBUG */

	
		
	DMA2->LIFCR |= DMA_LIFCR_CTCIF1;
	
//		if((USART3->SR | USART3->DR) != 0)		//Cleared IDLE bit in the USART_CR1 (It is cleared by a software sequence (an read to the USART_SR register followed by a read to the USART_DR register).)
//{

//}
	
				xHigherPriorityTaskWoken = pdFALSE;
				xSemaphoreGiveFromISR( semRingBufHandle, &xHigherPriorityTaskWoken );	
	
	}
	

	
	
return;
  /* USER CODE END DMA2_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
  /* USER CODE BEGIN DMA2_Stream1_IRQn 1 */

  /* USER CODE END DMA2_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream3 global interrupt.
  */
void DMA2_Stream3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream3_IRQn 0 */

  /* USER CODE END DMA2_Stream3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_sdio_rx);
  /* USER CODE BEGIN DMA2_Stream3_IRQn 1 */

  /* USER CODE END DMA2_Stream3_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream6 global interrupt.
  */
void DMA2_Stream6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream6_IRQn 0 */

  /* USER CODE END DMA2_Stream6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_sdio_tx);
  /* USER CODE BEGIN DMA2_Stream6_IRQn 1 */

  /* USER CODE END DMA2_Stream6_IRQn 1 */
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART6_IRQn 0 */
	if((USART6->SR | USART6->DR) != 0)		//Cleared IDLE bit in the USART_CR1 (It is cleared by a software sequence (an read to the USART_SR register followed by a read to the USART_DR register).)
{
	
	#ifdef DEBUG
EventRecord2 (0xFF, 0x04, 0);///!!! отладка
#endif /* DEBUG */

					xHigherPriorityTaskWoken = pdFALSE;
				xSemaphoreGiveFromISR( semRingBufHandle, &xHigherPriorityTaskWoken );
}
//				
	

		
				if( xHigherPriorityTaskWoken == pdTRUE )
  {

	
	}



	return;
  /* USER CODE END USART6_IRQn 0 */
  HAL_UART_IRQHandler(&huart6);
  /* USER CODE BEGIN USART6_IRQn 1 */

  /* USER CODE END USART6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
