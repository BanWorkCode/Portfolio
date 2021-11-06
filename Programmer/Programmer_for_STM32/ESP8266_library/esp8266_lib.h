#ifndef __esp8266_lib_H
#define __esp8266_lib_H
//
//
//=====================================================================================================================================
//
#include "main.h"
//
//
//=====================================================================================================================================
//
#define NUMBER_ITEMS_DATAWIFI 8U
#define PATH_LENGTH 256U
#define MAX_NESTED_DIR_DEPTH 10U
#define SIZE_STRING_NAME_WIFI 32U
#define SIZE_STRING_PASSWORD_WIFI 256U
#define SIZE_UNION_WIFI (SIZE_STRING_NAME_WIFI+SIZE_STRING_PASSWORD_WIFI+1U+4U)*NUMBER_ITEMS_DATAWIFI
//
//
//=====================================================================================================================================
//
typedef enum
{
  ANSWER_WAITING =0U,
  ANSWER_OK =1U,
  ANSWER_EMPTY =2U,
  ANSWER_ERROR =3U,
  ANSWER_MESSAGE =4U,
	ANSWER_LOADING =5U
} ESP8266_Answer_t;
//
//
typedef struct
{
  char nameWIFI[SIZE_STRING_NAME_WIFI];
  char passwordWIFI[SIZE_STRING_PASSWORD_WIFI];
  _Bool connection;
  uint32_t lastConnection;
} ESP8266_WifiSettings_t;
//
//
typedef union
{
  ESP8266_WifiSettings_t settingsWIFI[NUMBER_ITEMS_DATAWIFI];
  uint32_t data32[SIZE_UNION_WIFI/4];
} ESP8266_UnionWifi_t;
//
//
//=====================================================================================================================================
//
extern ESP8266_UnionWifi_t g_unionWifi;
extern ESP8266_Answer_t g_answerESP8266;
//
//
//=====================================================================================================================================
//
static _Bool ESP8266_TransmitCommand ( char* commandForESP8266 );
static ESP8266_Answer_t ESP8266_ReceiveAnswer ( uint32_t timeoutESP8266 );
_Bool ESP8266_Init ( void );
void* ESP8266_CheckConnection ( void* currentItem );
static _Bool ESP8266_GetNetworksList ( void );
void* ESP8266_ShowNetworksList ( void* currentItem );
static _Bool ESP8266_EstablishWifiConnection ( char* ssid, char* pwd );
static char* ESP8266_CheckForPassword ( char* login );
void ESP8266_ConnectViaWifi ( uint16_t selectedItem );
static _Bool ESP8266_GetListOfFilesViaFtp ( char* pathCwd );
void ESP8266_ShowListOfFiles ( void );
static _Bool ESP8266_GetFileViaFtp ( char* path, char* name );
void ESP8266_OpenFolderOrSelectFileFTP ( uint16_t selectedItem );
void ESP8266_GoToParentDirectoryFTP ( void );
void ESP8266_EnterPasswordForNetwork(char* ssid);
void* ESP8266_DEinit_uart ( void* currentItem );
void* ESP8266_init_uart ( void* currentItem );

//
//
//=====================================================================================================================================
//
#endif /*__esp8266_lib_H */
