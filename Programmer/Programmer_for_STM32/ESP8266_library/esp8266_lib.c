/* Includes */

#include "main.h"
#include "usart.h"
#include "esp8266_lib.h"
#include "string.h"
#include "displayMenu.h"
#include "cmsis_os.h"
#include <stdlib.h>
#include <stdarg.h>
#include "sd_card.h"
#include "ringBuffer.h"
#include "Ftp.h"
#include "storingSettings.h"
//
//
//=====================================================================================================================================
//
//Template:
//ESP8266_UnionWifi_t g_unionWifi ={{{"aa0x0dbb","qazwsxedc",0,4},{"ASUS_3C","rainman84",0,5},{"arnex","arnex1arnex1",0,3}}};
ESP8266_UnionWifi_t g_unionWifi;
ESP8266_Answer_t g_answerESP8266 = ANSWER_WAITING;

static char g_pathToDirectoryFTP[MAX_NESTED_DIR_DEPTH][PATH_LENGTH];
static uint8_t g_indCurDirFTP = 0; // индекс глубины дирректории для fullPathToDirectoryFTP
//
//
//=====================================================================================================================================
//
static _Bool ESP8266_TransmitCommand ( char* commandForESP8266 )
{
  if ( HAL_UART_Transmit ( &huart6, ( uint8_t* ) commandForESP8266, strlen ( commandForESP8266 ), 100 ) )
    {
      return 1;
    }
  char* rn ="\r";
  if ( HAL_UART_Transmit ( &huart6, ( uint8_t* ) rn, 1, 100 ) )
    {
      return 1;
    }
  return 0;
}
//
//
//=====================================================================================================================================
//
static ESP8266_Answer_t ESP8266_ReceiveAnswer ( uint32_t timeoutESP8266 )
{
  uint32_t tickStart =  xTaskGetTickCount();
  g_answerESP8266 =ANSWER_WAITING;
  while ( xTaskGetTickCount() <tickStart+timeoutESP8266 )
    {
				if ( g_answerESP8266 == ANSWER_LOADING ){
					tickStart =  xTaskGetTickCount();					
					}			
      else if ( g_answerESP8266 !=ANSWER_WAITING )
        {
          break;
        }
      osDelay ( 10 );
    }
  return g_answerESP8266;
}
//
//
//=====================================================================================================================================
//
_Bool ESP8266_Init ( void )
{
  g_dataTypeRingBuf = RB_DATA_TYPE_MESSAGE;
  if ( ESP8266_TransmitCommand ( "init" ) )
    {
      return 1;
    }
  if ( ESP8266_ReceiveAnswer ( 1000 ) ==ANSWER_OK )
    {
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      return 0;
    }
  else
    {
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      return 1;
    }
}
//
//
//=====================================================================================================================================
//
void* ESP8266_CheckConnection ( void* currentItem )
{
  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_WAITING );
  if ( ESP8266_Init() )
    {
      g_infoMessage = "WI-FI ERROR";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  else
    {
      g_infoMessage = "WI-FI OK";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  return currentItem;
}
//
//
//=====================================================================================================================================
//
static _Bool ESP8266_GetNetworksList ( void )
{
  g_dataTypeRingBuf = RB_DATA_TYPE_FILE_SIZE;
  g_dataTypeRingBufAfterSize = RB_DATA_TYPE_NETWORKS;
  g_numberOfListItemsForShow =0;
  //чтение настроек из Flash
  STORING_ReadSettingsFromFlash();
  // Перечисляет доступные точки доступа.
  if ( ESP8266_TransmitCommand ( "netw" ) )
    {
      return 1;
    }
  if ( ESP8266_ReceiveAnswer ( 10000 ) ==ANSWER_OK )
    {
      //g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      //return 0;
      g_dataTypeRingBuf = RB_DATA_TYPE_MESSAGE;
    }
  else
    {
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      return 1;
    }
  //Получаем текущий ssid(имя сети к которой подключены)
  if ( ESP8266_TransmitCommand ( "ssid" ) )
    {
      return 1;
    }
  if ( ESP8266_ReceiveAnswer ( 10000 ) ==ANSWER_MESSAGE )
    {
      for ( uint16_t i=0; i<NUMBER_ITEMS_DATAWIFI; i++ )
        {
          if ( strlen ( g_stringAnswer.string ) >0 && strcmp ( g_unionWifi.settingsWIFI[i].nameWIFI, g_stringAnswer.string ) ==0 )
            {
              g_unionWifi.settingsWIFI[i].connection =1;
            }
          else
            {
              g_unionWifi.settingsWIFI[i].connection =0;
            }
        }
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      return 0;
    }
  else
    {
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      return 1;
    }
}
//
//
//=====================================================================================================================================
//
void* ESP8266_ShowNetworksList ( void* currentItem )
{
  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_WAITING );
  if ( ESP8266_GetNetworksList() )
    {
      g_infoMessage = "WI-FI GET TYPE_WINDOW_LIST NETWORKS ERROR";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  else
    {
      g_typeList = TYPE_LIST_NETWORKS;
      g_currentItemOfList =0;
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_LIST );
    }
  return currentItem;
}
//
//
//=====================================================================================================================================
//
static _Bool ESP8266_EstablishWifiConnection ( char* ssid, char* pwd )
{
  //Template:
  //  ssid = "ASUS_3C";
  //	pwd = "rainman84";
  char tempCommand[256]="";
  strcat ( tempCommand, "conc=" );
  strcat ( tempCommand, "\"" );
  strcat ( tempCommand, ssid );
  strcat ( tempCommand, "\",\"" );
  strcat ( tempCommand, pwd );
  strcat ( tempCommand, "\"" );
  g_dataTypeRingBuf = RB_DATA_TYPE_MESSAGE;
  if ( ESP8266_TransmitCommand ( tempCommand ) )
    {
      return 1;
    }
  if ( ESP8266_ReceiveAnswer ( 16000 ) ==ANSWER_OK )
    {
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      return 0;
    }
  else
    {
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      return 1;
    }
}
//
//
//=====================================================================================================================================
//
static char* ESP8266_CheckForPassword ( char* login )
{
  for ( uint8_t i=0; i<NUMBER_ITEMS_DATAWIFI; i++ )
    {
      if ( strcmp ( g_unionWifi.settingsWIFI[i].nameWIFI, g_rowOfListForScreen[g_currentItemOfList] ) ==0 )
        {
          return g_unionWifi.settingsWIFI[i].passwordWIFI;
        }
    }
  return NULL;
}
//
//
//=====================================================================================================================================
//
//при нажатии Enter
void ESP8266_ConnectViaWifi ( uint16_t selectedItem )
{
  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_WAITING );
  char* password = ESP8266_CheckForPassword ( g_rowOfListForScreen[g_currentItemOfList] );
  //сохранение данных во Flash
  if ( STORING_SaveSettingsToFlash () )
    {
      g_infoMessage = "Error, no data saved in flash memory";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  else
    {
      if ( password!=NULL )
        {
          if ( ESP8266_EstablishWifiConnection ( g_rowOfListForScreen[g_currentItemOfList], ( char* ) password ) )
            {
              g_infoMessage = "Connection ERROR";
              DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
            }
          else
            {
              for ( uint16_t i=0; i<NUMBER_ITEMS_DATAWIFI; i++ )
                {
                  if ( strcmp ( g_unionWifi.settingsWIFI[i].nameWIFI, g_rowOfListForScreen[g_currentItemOfList] ) ==0 )
                    {
                      g_unionWifi.settingsWIFI[i].lastConnection=0;
                      g_unionWifi.settingsWIFI[i].connection=1;
                    }
                  else
                    {
                      g_unionWifi.settingsWIFI[i].lastConnection++;
                      g_unionWifi.settingsWIFI[i].connection=0;
                    }
                }
              g_infoMessage = "Connection Ok";
              DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
            }
        }
      else
        {
          g_infoMessage = "Enter the password";
          DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
        }
    }
}
//
//
//=====================================================================================================================================
//
static _Bool ESP8266_GetListOfFilesViaFtp ( char* pathCwd )
{
	g_currentItemOfList =0;
	g_numberOfListItemsForShow =0;
	for(uint16_t i =0;i<MAX_NUMBER_OF_CATALOG_ITEMS;i++)
  {
		g_fileInfo[i].file =0;
		g_fileInfo[i].size =0;
		g_fileInfo[i].modify[0] =0x00;
		g_fileInfo[i].modiflyCheck = 0;
		g_fileInfo[i].nameCheck = 0;
		g_fileInfo[i].sizeCheck = 0;
		g_fileInfo[i].typeCheck = 0;
  }
	
	
	
  FTP_ServerSettings_t* selectedServer = FTP_GetSettingSelectedFtpServer();
  char transmittedData[300];
  g_dataTypeRingBuf = RB_DATA_TYPE_MESSAGE;
  strcpy ( transmittedData, "logn=USER " );
  strcat ( transmittedData, selectedServer->login );
  //if (ESP8266_TransmitCommand("logn=USER maxim")){
  if ( ESP8266_TransmitCommand ( transmittedData ) )
    {
      return 1;
    }
  if ( ESP8266_ReceiveAnswer ( 1000 ) ==ANSWER_OK )
    {
    }
  else
    {
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      return 1;
    }
  strcpy ( transmittedData, "pass=PASS " );
  strcat ( transmittedData, selectedServer->password );
  //if (ESP8266_TransmitCommand("pass=PASS maxim")){
  if ( ESP8266_TransmitCommand ( transmittedData ) )
    {
      return 1;
    }
  if ( ESP8266_ReceiveAnswer ( 1000 ) ==ANSWER_OK )
    {
    }
  else
    {
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      return 1;
    }
  g_dataTypeRingBuf = RB_DATA_TYPE_FILE_SIZE;
  g_dataTypeRingBufAfterSize = RB_DATA_TYPE_DIRECTORIES;
  g_numberOfListItemsForShow =0;
  //char* tempPathMlsd =   "mlsd=\"192.168.1.224\",\"21\",\"";
  strcpy ( transmittedData, "mlsd=\"" );
  strcat ( transmittedData, selectedServer->address );
  strcat ( transmittedData, "\",\"" );
  strcat ( transmittedData, selectedServer->port );
  strcat ( transmittedData, "\",\"" );
  strcat ( transmittedData, pathCwd );
  strcat ( transmittedData, "\"" );
  // Перечисляет файлы и директории
  if ( ESP8266_TransmitCommand ( transmittedData ) )
    {
      return 1;
    }
  if ( ESP8266_ReceiveAnswer ( 10000 ) ==ANSWER_OK )
    {
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      g_dataTypeRingBufAfterSize = RB_DATA_TYPE_UNNECESSARY;
      return 0;
    }
  else
    {
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      g_dataTypeRingBufAfterSize = RB_DATA_TYPE_UNNECESSARY;
      return 1;
    }
}
//
//
//=====================================================================================================================================
//
void ESP8266_ShowListOfFiles ( void )
{
  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_WAITING );
  //if (ESP8266_GetListOfFilesViaFtp("mlsd=\"192.168.1.172\",\"21\",\"/\"")){//mlsd="192.168.1.172","21","/"
  //if (ESP8266_GetListOfFilesViaFtp("mlsd=\"192.168.1.224\",\"21\",\"/\"")){//mlsd="192.168.1.224","21","/"
  
	//!!! Очистка 
	
	
	if ( ESP8266_GetListOfFilesViaFtp ( "/" ) )
    {
      g_infoMessage = "GET TYPE_WINDOW_LIST ERROR";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  else
    {
      g_typeList = TYPE_LIST_FILES_FTP;
      g_currentItemOfList =0;//!!!??? я это просто скопировал
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_LIST );
    }
}
//
//
//=====================================================================================================================================
//
static _Bool ESP8266_GetFileViaFtp ( char* path, char* name )
{
  SD_OpenFileToWrite ( name ); // дописать обработку ошибки!!!
  FTP_ServerSettings_t* selectedServer = FTP_GetSettingSelectedFtpServer();
  char transmittedData[500];
  g_dataTypeRingBuf = RB_DATA_TYPE_MESSAGE;
  strcpy ( transmittedData, "logn=USER " );
  strcat ( transmittedData, selectedServer->login );
  if ( ESP8266_TransmitCommand ( transmittedData ) )
    {
      return 1;
    }
  if ( ESP8266_ReceiveAnswer ( 1000 ) ==ANSWER_OK )
    {
    }
  else
    {
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      return 1;
    }
  strcpy ( transmittedData, "pass=PASS " );
  strcat ( transmittedData, selectedServer->password );
  if ( ESP8266_TransmitCommand ( transmittedData ) )
    {
      return 1;
    }
  if ( ESP8266_ReceiveAnswer ( 1000 ) ==ANSWER_OK )
    {
    }
  else
    {
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      return 1;
    }
  g_dataTypeRingBuf = RB_DATA_TYPE_FILE_SIZE;
  g_dataTypeRingBufAfterSize = RB_DATA_TYPE_FILE;
  strcpy ( transmittedData, "file=\"" );
  strcat ( transmittedData, selectedServer->address );
  strcat ( transmittedData, "\",\"" );
  strcat ( transmittedData, selectedServer->port );
  strcat ( transmittedData, "\",\"" );
  strcat ( transmittedData, path );
  strcat ( transmittedData, name );
  strcat ( transmittedData, "\"" );
  if ( ESP8266_TransmitCommand ( transmittedData ) )
    {
      return 1;
    }
		

		
		
  if ( ESP8266_ReceiveAnswer ( 2000 ) ==ANSWER_OK )
    {
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      g_dataTypeRingBufAfterSize = RB_DATA_TYPE_UNNECESSARY;
      SD_CloseFileOnCard();//!!! проверить
      return 0;
    }
  else
    {
      g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
      g_dataTypeRingBufAfterSize = RB_DATA_TYPE_UNNECESSARY;
      SD_CloseFileOnCard();// !!! проверить
      return 1;
    }
}
//
//
//=====================================================================================================================================
//
void ESP8266_OpenFolderOrSelectFileFTP ( uint16_t selectedItem )
{
	DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_DOWNLOADING );
  g_numberOfListItemsForShow = 0;
  //Проверяем Файл или папка.
		if (  g_fileInfo[g_currentItemOfList].file ) //если файл	
    {
			//DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
      char fullPathToDirectoryFTP[PATH_LENGTH] = "";
      for ( uint8_t i=0; i<g_indCurDirFTP; i++ )
        {
          strcat ( fullPathToDirectoryFTP, "/" );
          strcat ( fullPathToDirectoryFTP, &g_pathToDirectoryFTP[i][0] );
        }
      strcat ( fullPathToDirectoryFTP, "/" );
      g_indCurDirFTP =0;
				
      if ( ESP8266_GetFileViaFtp ( fullPathToDirectoryFTP, g_rowOfListForScreen[g_currentItemOfList] ) )
        {
          g_infoMessage = "Error. Receiving the file.";
          DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
        }
      else
        {
          g_infoMessage = "OK. File downloaded.";
          DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
        }
    }
  else   //если папка
    {
      strcpy ( &g_pathToDirectoryFTP[g_indCurDirFTP][0], g_rowOfListForScreen[g_currentItemOfList] );
      if ( g_indCurDirFTP<MAX_NESTED_DIR_DEPTH-1 )
        {
          g_indCurDirFTP++;
          char fullPathToDirectoryFTP[PATH_LENGTH] = "";
          for ( uint8_t i=0; i<g_indCurDirFTP; i++ )
            {
              strcat ( fullPathToDirectoryFTP, "/" );
              strcat ( fullPathToDirectoryFTP, &g_pathToDirectoryFTP[i][0] );
            }
          strcat ( fullPathToDirectoryFTP, "/" );
					DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_WAITING );	
          if ( ESP8266_GetListOfFilesViaFtp ( fullPathToDirectoryFTP ) )
            {
							//откатить путь на один назад!!!
							g_indCurDirFTP =0;
              g_infoMessage = "ERROR: Get list files  FTP error!";
              DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
            }
          else
            {
              g_typeList = TYPE_LIST_FILES_FTP;
              DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_LIST );
            }
        }
      else
        {
          g_indCurDirFTP=0;
          g_infoMessage = "ERROR: To many depth";
          DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
        }
    }
}
//
//
//=====================================================================================================================================
//
void ESP8266_GoToParentDirectoryFTP ( void )
{
  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_WAITING );
  g_currentItemOfList=0;
  if ( g_indCurDirFTP )
    {
      g_indCurDirFTP--;
      char fullPathToDirectoryFTP[256] = "";
      for ( uint8_t i=0; i<g_indCurDirFTP; i++ )
        {
          strcat ( fullPathToDirectoryFTP, "/" );
          strcat ( fullPathToDirectoryFTP, &g_pathToDirectoryFTP[i][0] );
        }
      strcat ( fullPathToDirectoryFTP, "/" );
      if ( ESP8266_GetListOfFilesViaFtp ( fullPathToDirectoryFTP ) )
        {
          g_infoMessage = "ERROR: Get list files  FTP error!";
          DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
        }
      else
        {
          g_typeList = TYPE_LIST_FILES_FTP;
          DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_LIST );
        }
    }
  else
    {
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_MENU );
    }
}
//
//
//=====================================================================================================================================
//
void ESP8266_EnterPasswordForNetwork ( char* ssid )
{
  _Bool selected =0;
  //Если такой ssid уже есть в g_unionWifi.settingsWIFI
  for ( uint16_t i=0; i<NUMBER_ITEMS_DATAWIFI; i++ )
    {
      if ( strcmp ( g_unionWifi.settingsWIFI[i].nameWIFI, g_rowOfListForScreen[g_currentItemOfList] ) ==0 )
        {
          g_inputDestination = g_unionWifi.settingsWIFI[i].passwordWIFI;
          selected =1;
          break;
        }
    }
  //Если такого ssid нет в g_unionWifi.settingsWIFI, но есть свободное место. то вносим
  if ( selected==0 )
    {
      for ( uint16_t i=0; i<NUMBER_ITEMS_DATAWIFI; i++ )
        {
          if ( g_unionWifi.settingsWIFI[i].nameWIFI[0]==0x00 || ( uint8_t ) g_unionWifi.settingsWIFI[i].nameWIFI[0]==0xFF )
            {
              strcpy ( g_unionWifi.settingsWIFI[i].nameWIFI, g_rowOfListForScreen[g_currentItemOfList] );
              g_inputDestination = g_unionWifi.settingsWIFI[i].passwordWIFI;
              selected =1;
              break;
            }
        }
    }
  //Если такого ssid нет в g_unionWifi.settingsWIFI, и нет свободного места, то удаляем самый старый и вносим новый
  if ( selected==0 )
    {
      uint8_t maxElement=0;
      for ( uint16_t i=0; i<NUMBER_ITEMS_DATAWIFI; i++ )
        {
          if ( g_unionWifi.settingsWIFI[i].lastConnection>g_unionWifi.settingsWIFI[maxElement].lastConnection )
            {
              maxElement =i;
            }
        }
      strcpy ( g_unionWifi.settingsWIFI[maxElement].nameWIFI, g_rowOfListForScreen[g_currentItemOfList] );
      //очищаем
      for ( uint16_t j=0; j<strlen ( g_unionWifi.settingsWIFI[maxElement].passwordWIFI ); j++ )
        {
          g_unionWifi.settingsWIFI[maxElement].passwordWIFI[j]=0x00;
        }
      g_unionWifi.settingsWIFI[maxElement].connection =0;
      g_unionWifi.settingsWIFI[maxElement].lastConnection =0;
      g_inputDestination = g_unionWifi.settingsWIFI[maxElement].passwordWIFI;
      selected =1;
    }
  strcpy ( g_inputField, g_inputDestination );
  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_KEYBOARD );
}
//
//
//=====================================================================================================================================
//
void* ESP8266_DEinit_uart ( void* currentItem )
{
  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_WAITING );
  if ( HAL_UART_DeInit(&huart6)==HAL_OK )
    {
      g_infoMessage = "Deinit UART OK";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  else
    {
      g_infoMessage = "Deinit UART error";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  return currentItem;
}
//
//
//=====================================================================================================================================
//
void* ESP8266_init_uart ( void* currentItem )
{
  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_WAITING );
  if ( HAL_UART_Init(&huart6)==HAL_OK && HAL_UART_Receive_DMA(&huart6, (uint8_t *)g_receiveBuffer.arr, (uint16_t)REC_BUF_SIZE)==HAL_OK)
    {
      g_infoMessage = "Init UART OK";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  else
    {
      g_infoMessage = "Init UART error";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  return currentItem;
}
