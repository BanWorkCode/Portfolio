/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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
#include <main.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "stdio.h"
#include "stdlib.h"
#include "fillMenu.h"
#include "rtc.h"
#include "storingSettings.h"
#include "display.h"
#include "fans.h"
#include "iwdg.h"
#include "keyboard.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

static uint32_t timeNow = 0;
/* USER CODE END Variables */
osThreadId updateScreenHandle;
osThreadId configuringParaHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void ScreenAndButtonTask(void const *argument);
void FansTask(void const *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = &xIdleStack[0];
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
	/* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of updateScreen */
	osThreadDef(updateScreen, ScreenAndButtonTask, osPriorityNormal, 0, 256);
	updateScreenHandle = osThreadCreate(osThread(updateScreen), NULL);

	/* definition and creation of configuringPara */
	osThreadDef(configuringPara, FansTask, osPriorityNormal, 0, 256);
	configuringParaHandle = osThreadCreate(osThread(configuringPara), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the updateScreen thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void ScreenAndButtonTask(void const *argument) {
	/* USER CODE BEGIN ScreenAndButtonTask */
	/* Infinite loop */
	for (;;) {
		//кнопка
		if (NVIC_GetEnableIRQ(EXTI15_10_IRQn) == 0) {//если прерывания выключены
			if (ssd1306_CheckDisplayStatus() == SSD1306_ON) { // если дисплей включен
				KEYBOARD_ButtonClick();
			} else { //если дисплей выключен, включаем дисплей
				ssd1306_on();
			}
			// в любом случае включаем прирывания кнопки и сбрасываем флаг
			__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_12);
			NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
			HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		} //кнопка	END

		// выключаем дисплей, после указаного периода
		if (((HAL_GetTick() - timeOfLastClick) > DISPLAY_DOWNTIME) && ssd1306_CheckDisplayStatus()) {
			ssd1306_off();
			STORING_SaveSetting();
		} else {
			DISPLAY_ShowMenu();
		}
		osDelay(50);
	}
	/* USER CODE END ScreenAndButtonTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
 * @brief Function implementing the configuringPara thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask02 */
void FansTask(void const *argument) {
	/* USER CODE BEGIN FansTask */
	/* Infinite loop */
	for (;;) {
		uint32_t startTimeFansTask = HAL_GetTick();

		HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);	// получение времени
		HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);	// получение даты. НЕОБХОДИМО для получения времени.

		timeNow = (uint32_t) (((uint32_t) currentTime.Hours * 3600U) + ((uint32_t) currentTime.Minutes * 60U) + ((uint32_t) currentTime.Seconds)); //Преобразуем время из структуры в одно число

		//  определяем день или ночь
		if (timeNow < dayOn.FullTime || timeNow > dayOff.FullTime) {
			g_timeOfDay = FANS_TIME_NIGHT;
		} else {
			g_timeOfDay = FANS_TIME_DAY;
		}

		FANS_SetSpeed(); //устанавливаем скорость для всех вентиляторов

		//Переключение суток
		if (g_flagNewDay) {
			FANS_CalculationDate();  // переводим текущую дату в число
			FANS_UpdateCyrrentDays(); //считаем текущие дни
			g_flagNewDay = 0;
		}

		HAL_IWDG_Refresh(&hiwdg); //Сбрасываем сторожевой таймер
		uint32_t runTimeFansTask = HAL_GetTick() - startTimeFansTask;

		osDelay(1000 - runTimeFansTask);

	}
	/* USER CODE END FansTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
