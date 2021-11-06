/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "dma.h"
#include "fatfs.h"
#include "sdio.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "firmware.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FLASH_DISK_START_ADDRESS    0X08008000//адрес начала программы
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static FATFS fileSystem;
static FRESULT res;
static DIR directory;
static FILINFO fileInfo;
//static FIL fil;



static _Bool g_upload = 0;// Наличие новой прошивки на Флешке.
static _Bool g_goto = 1; // Переход в основную прошивку.

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//void jump_app( uint32_t address ) {
//   __ASM ("MSR MSP, R0\n"); // Load new stack pointer address
//		__ASM ("MSR PC, [R0, #4]\n");  //Load new program counter address;
//};
void branth(uint32_t);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
	
	

int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
	
  res = f_mount ( &fileSystem, "0", 1 );
	//printf("res1 =%i",res );
  res = f_opendir ( &directory, "0:/Firmware" ); /* Открытие директории */
	//printf("res2 =%i",res );
  if ( res == FR_OK )
    {
      for ( ;; )
        {
          res = f_readdir ( &directory, &fileInfo ); /* Чтение объекта директории */
          //сравнение строк
          if ( strncmp ( "Programmer", fileInfo.fname, 10 ) ==0 )
            {
              g_upload =1;
              break;
            }
          if ( res != FR_OK || fileInfo.fname[0] == 0 )
            {
              break; // Останов цикла при ошибке или при достижении
            }
        }
    }

  if ( g_upload )
    {
      HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_13, GPIO_PIN_RESET );
      if ( BOOTLOADER_UploadFirmware() ==0 )
        {
          //удаляем файл
          f_unlink ( "0:/Firmware/Programmer.hex" );
          g_goto =1;
        }
      else
        {
          g_goto =0;
        }
      HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_13, GPIO_PIN_SET );
    }
		
//		
//--------------------------------------------------------------------
  if ( g_goto==1 )
    {
	
	   uint32_t appJumpAddress;
	   void (*GoToApp)(void);

	   appJumpAddress = *((volatile uint32_t*)(FLASH_DISK_START_ADDRESS + 4));
	   GoToApp = (void (*)(void))appJumpAddress;				

	    __disable_irq();			
			
			HAL_RCC_DeInit();
      HAL_DeInit();
			
	   SCB->VTOR = FLASH_DISK_START_ADDRESS;
	    __enable_irq();

	    __set_MSP(*((volatile uint32_t*) FLASH_DISK_START_ADDRESS)); //stack pointer (to RAM) for USER app in this address
			SysTick->CTRL = 0;
			SysTick->LOAD = 0;
			SysTick->VAL = 0;
	   GoToApp();
    }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		  HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_13, GPIO_PIN_RESET );
      HAL_Delay ( 1000 );
      HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_13, GPIO_PIN_SET );
      HAL_Delay ( 1500 );
      HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_13, GPIO_PIN_RESET );
      HAL_Delay ( 300 );
      HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_13, GPIO_PIN_SET );
      HAL_Delay ( 600 );
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
//  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
