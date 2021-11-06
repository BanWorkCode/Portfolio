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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "fatfs.h"//тест
#include "ssd1306.h"
#include "make_menu.h"
#include "scanningButtons.h"
#include "esp8266_lib.h"
#include "ringBuffer.h"
#include "storingSettings.h"

#ifdef DEBUG_INIT_EVENT_RECORDER
#include "EventRecorder.h" //отладка
#endif /* DEBUG_INIT_EVENT_RECORDER */
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
static SB_Keyboard_t controlKeyboard  = {KEY_ENTER, KEY_UP, KEY_RIGHT, KEY_FUNCTION, KEY_LEFT, KEY_DOWN};
static SB_Key_t buttonIsPressed;
static volatile uint16_t filledBufferSpace =0;

struct
{
  uint64_t size;
  uint64_t uploadedData;
} g_fileToTransfer = {0,0};

char vTaskList_data [1024];//отладка
size_t HeapSize =0; //отладка
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId keyboardTaskHandle;
osThreadId displayTaskHandle;
osThreadId ringBufferTaskHandle;
osMessageQId buttonsQueueHandle;
osSemaphoreId semRingBufHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void keyboardFunc(void const * argument);
void displayFunc(void const * argument);
void ringBufferFunc(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
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

  /* Create the semaphores(s) */
  /* definition and creation of semRingBuf */
  osSemaphoreDef(semRingBuf);
  semRingBufHandle = osSemaphoreCreate(osSemaphore(semRingBuf), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of buttonsQueue */
  osMessageQDef(buttonsQueue, 8, uint8_t);
  buttonsQueueHandle = osMessageCreate(osMessageQ(buttonsQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 384);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of keyboardTask */
  osThreadDef(keyboardTask, keyboardFunc, osPriorityNormal, 0, 128);
  keyboardTaskHandle = osThreadCreate(osThread(keyboardTask), NULL);

  /* definition and creation of displayTask */
  osThreadDef(displayTask, displayFunc, osPriorityBelowNormal, 0, 256);
  displayTaskHandle = osThreadCreate(osThread(displayTask), NULL);

  /* definition and creation of ringBufferTask */
  osThreadDef(ringBufferTask, ringBufferFunc, osPriorityHigh, 0, 256);
  ringBufferTaskHandle = osThreadCreate(osThread(ringBufferTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	  selectedMenuItem = &Boot_MAIN;
  /* Infinite loop */	
  for ( ;; )
    {
//отладка:
//uxSemaphoreGetCount( SemUART1toTaskCommHandle );
      vTaskList ( vTaskList_data );
      HeapSize = 	xPortGetFreeHeapSize();
      buttonIsPressed = KEYS_ARE_REALESED;
      // из очереди значение нажатой кнопки записывается в buttonIsPressed
      xQueueReceive ( buttonsQueueHandle, ( void * ) &buttonIsPressed, 0 );
      if ( g_currentWindow == TYPE_WINDOW_MENU )
        {
          switch ( buttonIsPressed )
            {
            case KEY_ENTER:
              if ( selectedMenuItem->Child!=&NULL_ENTRY )
                {
                  selectedMenuItem = selectedMenuItem->Child;
                }
              else
                {
                  selectedMenuItem  = selectedMenuItem->pfunc ( ( void* ) selectedMenuItem );
                }
              break;
            case KEY_UP:
              if ( selectedMenuItem->Previous!=&NULL_ENTRY )
                {
                  selectedMenuItem = selectedMenuItem->Previous;
                }
              break;
            case KEY_RIGHT:
              break;
            case KEY_FUNCTION:
              if ( selectedMenuItem->Parent!=&NULL_ENTRY )
                {
                  selectedMenuItem = selectedMenuItem->Parent;
                }
              break;
            case KEY_LEFT:
              break;
            case KEY_DOWN:
              if ( selectedMenuItem->Next!=&NULL_ENTRY )
                {
                  selectedMenuItem = selectedMenuItem->Next;
                }
              break;
            default:
              break;
            }
        }//END if (g_currentWindow == TYPE_WINDOW_MENU)
      else if ( g_currentWindow == TYPE_WINDOW_LIST )
        {
          switch ( buttonIsPressed )
            {
            case KEY_ENTER:
              if ( g_typeList == TYPE_LIST_FILES_SD )
                {
                  SD_OpenFolderOrSelectFile ( g_currentItemOfList );
                }
	              else if ( g_typeList == TYPE_LIST_FTP_SAVE_FOLDER )
                {
                SD_OpenFolderOrSelectFile ( g_currentItemOfList );
                }
              else if ( g_typeList == TYPE_LIST_FIRMWARE )
                {
                  SD_OpenFolderOrSelectFile ( g_currentItemOfList );
                }								
              else if ( g_typeList == TYPE_LIST_NETWORKS )
                {
                  ESP8266_ConnectViaWifi ( g_currentItemOfList );
                }
              else if ( g_typeList == TYPE_LIST_FILES_FTP )
                {
                  ESP8266_OpenFolderOrSelectFileFTP ( g_currentItemOfList );
                }
              else if ( g_typeList == TYPE_LIST_SERVERS_FTP )
                {
                  FTP_SelectFtpServer();
                  ESP8266_ShowListOfFiles();
                }
              break;
            case KEY_UP:
              if ( g_currentItemOfList>0 )
                {
                  g_currentItemOfList--;
                }
              break;
            case KEY_RIGHT:
              if ( g_typeList == TYPE_LIST_NETWORKS )
                {
                  ESP8266_EnterPasswordForNetwork ( g_rowOfListForScreen[g_currentItemOfList] );
                }
              else if ( g_typeList == TYPE_LIST_SERVERS_FTP )
                {
                  FTP_SelectFtpServer();
                  selectedMenuItem = &Adsress_NewFTP;
                  g_currentItemOfList =0;
                  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_MENU );
                }
								 else  if ( g_typeList == TYPE_LIST_FILES_SD )
                {
                  SD_EnterNameNewFolder();
                }
								else  if ( g_typeList == TYPE_LIST_FTP_SAVE_FOLDER )
                {
									// сюда
                  SD_SelectFolderForFtpServer(g_currentItemOfList);
                }
								
								
								
              break;
            case KEY_FUNCTION:
              if ( g_typeList == TYPE_LIST_FILES_SD )
                {
                  SD_GoToParentDirectory();
                }
						else if ( g_typeList == TYPE_LIST_FTP_SAVE_FOLDER )
                {
									SD_GoToParentDirectory();
                }
								else if ( g_typeList == TYPE_LIST_FIRMWARE )
                {
									SD_GoToParentDirectory();
                }
              else if ( g_typeList == TYPE_LIST_NETWORKS )
                {
                  g_currentItemOfList =0;
                  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_MENU );
                }
              else if ( g_typeList == TYPE_LIST_FILES_FTP )
                {
                  ESP8266_GoToParentDirectoryFTP();
                }
              else if ( g_typeList == TYPE_LIST_SERVERS_FTP )
                {
                  g_currentItemOfList =0;
                  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_MENU );
                }
              break;
            case KEY_LEFT:
						if ( g_typeList == TYPE_LIST_FILES_SD )
                {
                  SD_ConfirmationDeleting(g_rowOfListForScreen[g_currentItemOfList] );
                }	
						
						
						
              break;
            case KEY_DOWN:
              if ( g_currentItemOfList<g_numberOfListItemsForShow-1 )
                {
                  g_currentItemOfList++;
                }
              break;
            default:
              break;
            }
        }//END else if(g_currentWindow == TYPE_WINDOW_LIST)
      else if ( g_currentWindow == TYPE_WINDOW_INFO )
        {
          switch ( buttonIsPressed )
            {
            case KEY_ENTER:
              DISPLAY_GoToSpecifiedWindow ( g_previousWindow );
              break;
            case KEY_UP:
              break;
            case KEY_RIGHT:
              break;
            case KEY_FUNCTION:
              DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_MENU );
              break;
            case KEY_LEFT:
              break;
            case KEY_DOWN:
              break;
            default:
              break;
            }
        }//END else if(g_currentWindow == TYPE_WINDOW_INFO)
				      else if ( g_currentWindow == TYPE_WINDOW_CONFIRM )
        {
          switch ( buttonIsPressed )
            {
            case KEY_ENTER:
              DISPLAY_GoToSpecifiedWindow ( g_previousWindow );
              break;
            case KEY_UP:
              break;
            case KEY_RIGHT:
							g_pressR(g_rowOfListForScreen[g_currentItemOfList]);
              break;
            case KEY_FUNCTION:
              DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_MENU );
              break;
            case KEY_LEFT:
              break;
            case KEY_DOWN:
              break;
            default:
              break;
            }
        }//END else if(g_currentWindow == TYPE_WINDOW_CONFIRM)
      else if ( g_currentWindow == TYPE_WINDOW_KEYBOARD )
        {
          switch ( buttonIsPressed )
            {
            case KEY_ENTER:
              DISPLAY_AddCharToInputField();
              break;
            case KEY_UP:
              DISPLAY_SelectTheCharacterToTheUp();
              break;
            case KEY_RIGHT:
              DISPLAY_SelectTheCharacterToTheRight();
              break;
            case KEY_FUNCTION:
              DISPLAY_SwitchKeyboardLayout();
              break;
            case KEY_LEFT:
              DISPLAY_SelectTheCharacterToTheLeft();
              break;
            case KEY_DOWN:
              DISPLAY_SelectTheCharacterToTheDown();
              break;
            default:
              break;
            }
        }//END else if(g_currentWindow == TYPE_WINDOW_KEYBOARD)
      osDelay ( 30 );
    }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_keyboardFunc */
/**
* @brief Function implementing the keyboardTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_keyboardFunc */
void keyboardFunc(void const * argument)
{
  /* USER CODE BEGIN keyboardFunc */
  /* Infinite loop */
  for ( ;; )
    {

			if ( SB_ScanningButton ( &buttonFunction, HAL_GPIO_ReadPin ( GPIOB, GPIO_PIN_1 ) ) )
        {
          xQueueSendToBack ( buttonsQueueHandle, ( void * ) &controlKeyboard.keyFunction, 100 );
        }
			  osDelay ( 2 );
      if ( SB_ScanningButton ( &buttonUp, HAL_GPIO_ReadPin ( GPIOA, GPIO_PIN_0 ) ) )
        {
          xQueueSendToBack ( buttonsQueueHandle, ( void * ) &controlKeyboard.keyUp, 100 );
        }
      osDelay ( 2 );
			if ( SB_ScanningButton ( &buttonEnter, HAL_GPIO_ReadPin ( GPIOC, GPIO_PIN_14 ) ) )
        {
          xQueueSendToBack ( buttonsQueueHandle, ( void * ) &controlKeyboard.keyEnter, 100 );
        }	
			osDelay ( 2 );
				 if ( SB_ScanningButton ( &buttonRight, HAL_GPIO_ReadPin ( GPIOB, GPIO_PIN_10 ) ) )
        {
          xQueueSendToBack ( buttonsQueueHandle, ( void * ) &controlKeyboard.keyRight, 100 );
        }
      osDelay ( 2 );
      if ( SB_ScanningButton ( &buttonDown, HAL_GPIO_ReadPin ( GPIOC, GPIO_PIN_15 ) ) )
        {
          xQueueSendToBack ( buttonsQueueHandle, ( void * ) &controlKeyboard.keyDown, 100 );
        }
      osDelay ( 2 );
      if ( SB_ScanningButton ( &buttonLeft, HAL_GPIO_ReadPin ( GPIOB, GPIO_PIN_2 ) ) )
        {
          xQueueSendToBack ( buttonsQueueHandle, ( void * ) &controlKeyboard.keyLeft, 100 );
        }
			osDelay ( 2 );
    }
  /* USER CODE END keyboardFunc */
}

/* USER CODE BEGIN Header_displayFunc */
/**
* @brief Function implementing the displayTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_displayFunc */
void displayFunc(void const * argument)
{
  /* USER CODE BEGIN displayFunc */
  if ( ssd1306_Init() )
    {
      DISPLAY_PrintString ( 0, 0, "Display OK" );
    }
  else
    {
      while ( 1 )
        {
          HAL_GPIO_TogglePin ( GPIOA, GPIO_PIN_13 );
          osDelay ( 1000 );
        }
    }
  if ( SD_InitCard() )
    {
      DISPLAY_PrintString ( 0, 10, "SDCard ERROR" );
      osDelay ( 10000 );
    }
  else
    {
      DISPLAY_PrintString ( 0, 10, "SDCard OK" );
    }
//if (ESP8266_Init()){
  if ( 0 )
    {
      DISPLAY_PrintString ( 0, 20, "WI-FI ERROR" );
      osDelay ( 10000 );
    }
  else
    {
      STORING_CheckFirstConnection(); //зачем?
      DISPLAY_PrintString ( 0, 20, "WI-FI OK " );
    }
  osDelay ( 2000 );
  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_MENU );
  /* Infinite loop */
  for ( ;; )
    {
      ssd1306_Fill ( Black );
//ssd1306_SetCursor(0, 0);
      if ( g_currentWindow == TYPE_WINDOW_MENU )
        {
          DISPLAY_ShowMenu ( selectedMenuItem );					
        }
      else if ( g_currentWindow == TYPE_WINDOW_LIST )
        {
          DISPLAY_ShowList();
					
					
					if(g_typeList == TYPE_LIST_NETWORKS){
          DISPLAY_ShowAdditionalNetworkInformation();
					}
					else if(g_typeList == TYPE_LIST_FILES_FTP || g_typeList == TYPE_LIST_FILES_SD || g_typeList == TYPE_LIST_FIRMWARE || g_typeList ==TYPE_LIST_FTP_SAVE_FOLDER ){
					DISPLAY_ShowAdditionalFileInformation();
						
						
						
					}
        }
      else if ( g_currentWindow == TYPE_WINDOW_INFO || g_currentWindow == TYPE_WINDOW_UPLOADING ||g_currentWindow == TYPE_WINDOW_CONFIRM)
        {
          DISPLAY_ShowInfo ( SSD1306_WIDTH_7_FONT );					
        }
      else if ( g_currentWindow == TYPE_WINDOW_WAITING )
        {
          DISPLAY_ShowWaiting();
        }
				 else if ( g_currentWindow == TYPE_WINDOW_DOWNLOADING )
        {
          DISPLAY_ShowLoading();
        }				
      else if ( g_currentWindow == TYPE_WINDOW_KEYBOARD )
        {
          DISPLAY_ShowKeyboard();
        }
      ssd1306_UpdateScreen();
      osDelay ( 100 );
    }
  /* USER CODE END displayFunc */
}

/* USER CODE BEGIN Header_ringBufferFunc */
/**
* @brief Function implementing the ringBufferTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ringBufferFunc */
void ringBufferFunc(void const * argument)
{
  /* USER CODE BEGIN ringBufferFunc */
  xSemaphoreTake ( semRingBufHandle, 100 ); // сбрасываем созданый семафор
  /* Infinite loop */
  for ( ;; )
    {
      xSemaphoreTake ( semRingBufHandle, portMAX_DELAY  );
      //DMA1_Stream1->NDTR - это свободное место в буфере, изначально равно размеру буфера, после получения данных уменьшается
      filledBufferSpace = ( REC_BUF_SIZE-DMA2_Stream1->NDTR ); //Заполненное место в буфере. Разница между длиной буфера и головой данных. начальное значение рано 0
      while ( g_receiveBuffer.tail != filledBufferSpace )
        {
          switch ( g_dataTypeRingBuf )
            {
            case RB_DATA_TYPE_FILE_SIZE:
            {
#ifdef DEBUG
              EventRecord2 ( 0xFF, 0x01, 0 ); 
#endif /* DEBUG */
              // в этой функции так же меняется  g_receiveBuffer.tail
              RB_AnswerSize_t sizeAnswer = RB_GetSizeOfMessage ( g_receiveBuffer.tail, filledBufferSpace );
              g_fileToTransfer.size = sizeAnswer.sizeOfMessage;
              g_receiveBuffer.tail = sizeAnswer.tail;
              g_fileToTransfer.uploadedData =0;
              g_dataTypeRingBuf = g_dataTypeRingBufAfterSize;
              //g_answerESP8266 = ANSWER_OK;
              break;
            }
            case RB_DATA_TYPE_FILE:
#ifdef DEBUG
              EventRecord2 ( 0x02, g_receiveBuffer.tail, filledBufferSpace ); 
#endif /* DEBUG */
						
						
						while ( g_fileToTransfer.uploadedData!=g_fileToTransfer.size && g_receiveBuffer.tail!=filledBufferSpace )
						{
                  SD_WriteByteToFile ( ( void* ) &g_receiveBuffer.arr[g_receiveBuffer.tail] );
                  g_receiveBuffer.tail = ( g_receiveBuffer.tail==REC_BUF_LAST_ELEMENT ) ? 0: ( g_receiveBuffer.tail+1 );
                  g_fileToTransfer.uploadedData++;
							
//									sprintf (g_infoMessageArr, "%s%i", "Dowload ", (uint16_t)((g_fileToTransfer.uploadedData/g_fileToTransfer.size)*100) );
//									g_infoMessage = g_infoMessageArr;
							
									g_answerESP8266 = ANSWER_LOADING;
                }
#ifdef DEBUG
              EventRecord2 ( 0x03, g_receiveBuffer.tail, filledBufferSpace ); ///!!! отладка
#endif /* DEBUG */
              if ( g_fileToTransfer.uploadedData==g_fileToTransfer.size )
                {
#ifdef DEBUG
                  EventRecord2 ( 0xFF, 0x03, 0 ); ///!!! отладка
#endif /* DEBUG */
                  g_fileToTransfer.uploadedData =0;
                  g_fileToTransfer.size =0;
                  g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
                  g_answerESP8266 =ANSWER_OK;
                }
								else{
								g_answerESP8266 = ANSWER_WAITING;
								}
								
              break;
            case RB_DATA_TYPE_NETWORKS:
              while ( g_fileToTransfer.uploadedData!=g_fileToTransfer.size && g_receiveBuffer.tail!=filledBufferSpace )
                {
                  RB_WriteByteToScreenList ( &g_receiveBuffer.arr[g_receiveBuffer.tail] );
                  g_receiveBuffer.tail = ( g_receiveBuffer.tail==REC_BUF_LAST_ELEMENT ) ? 0: ( g_receiveBuffer.tail+1 );
                  g_fileToTransfer.uploadedData++;
                }
              if ( g_fileToTransfer.uploadedData==g_fileToTransfer.size )
                {
                  g_fileToTransfer.uploadedData =0;
                  g_fileToTransfer.size =0;
                  g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
                  g_answerESP8266 =ANSWER_OK;
                }
              break;
            case RB_DATA_TYPE_DIRECTORIES:
              // лист файлов и папок
              if ( g_fileToTransfer.size==2 )
                {
                  g_answerESP8266 =ANSWER_EMPTY;
                  g_fileToTransfer.uploadedData =0;
                  g_fileToTransfer.size =0;
                  g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
                  break;
                }
              while ( g_fileToTransfer.uploadedData!=g_fileToTransfer.size && g_receiveBuffer.tail!=filledBufferSpace )
                {
                  RB_WriteByteToScreenListAndFileInfo ( ( void* ) &g_receiveBuffer.arr[g_receiveBuffer.tail] );
                  g_receiveBuffer.tail = ( g_receiveBuffer.tail==REC_BUF_LAST_ELEMENT ) ? 0: ( g_receiveBuffer.tail+1 );
                  g_fileToTransfer.uploadedData++;
                }
              if ( g_fileToTransfer.uploadedData==g_fileToTransfer.size )
                {
                  g_fileToTransfer.uploadedData =0;
                  g_fileToTransfer.size =0;
                  g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
                  g_answerESP8266 =ANSWER_OK;
                }
              break;
            case RB_DATA_TYPE_MESSAGE:
              g_stringAnswer = RB_ConvertAnswerToString ( g_receiveBuffer.tail, filledBufferSpace );
              if ( g_stringAnswer.tail == g_receiveBuffer.tail )
                {
                  g_answerESP8266 =ANSWER_ERROR;
                  //значит  0x0D не найден, попробовать обновить filledBufferSpace и попробовать еще раз!!!
                }
              else if ( strcmp ( g_stringAnswer.string, "OK" ) ==0 ) //плохо!!! надо передавать строку дальше
                {
                  g_receiveBuffer.tail =g_stringAnswer.tail;
                  g_answerESP8266 =ANSWER_OK;
                }
              else
                {
                  g_receiveBuffer.tail = g_stringAnswer.tail;
                  g_answerESP8266 =ANSWER_MESSAGE;
                }
              // а если ответ ок, но конец сообщения не достигнут, то что????
              //где то должен быть переход на мусор!!!
              /*



              */
              break;
            default://пришли мусорные данные
              //g_receiveBuffer.tail =	filledBufferSpace;
              g_receiveBuffer.tail = ( g_receiveBuffer.tail == REC_BUF_LAST_ELEMENT ) ? 0 : ( g_receiveBuffer.tail+1 );
              break;
            }
//								if(g_receiveBuffer.tail != filledBufferSpace){
//								g_receiveBuffer.tail = (g_receiveBuffer.tail == REC_BUF_LAST_ELEMENT)? 0 : (g_receiveBuffer.tail+1);
//								}
        }//end while
      osDelay ( 1 );
    }
  /* USER CODE END ringBufferFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
