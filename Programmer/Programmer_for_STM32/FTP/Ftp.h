#ifndef ftp_H
#define ftp_H
//
//
//=====================================================================================================================================
//
#include <main.h>
//
//
//=====================================================================================================================================
//
#define NUMBER_ITEMS_DATAFTP 10U
#define SIZE_STRING_ADDRESS 256U
#define SIZE_STRING_PORT 8U
#define SIZE_STRING_LOGIN 256U
#define SIZE_STRING_PASSWORD 256U
#define SIZE_STRING_PATH 256U
#define SIZE_FTP_UNION (SIZE_STRING_ADDRESS+SIZE_STRING_PORT+SIZE_STRING_LOGIN+SIZE_STRING_PASSWORD+SIZE_STRING_PATH)*NUMBER_ITEMS_DATAFTP
#define FTP_MAGIC_VALUE_SELECTED_SERVER 0xFFFFU 
//
//
//=====================================================================================================================================
//
typedef struct
{
  char address[SIZE_STRING_ADDRESS];
  char port[SIZE_STRING_PORT];
  char login[SIZE_STRING_LOGIN];
  char password[SIZE_STRING_PASSWORD];
	char path[SIZE_STRING_PATH];
} FTP_ServerSettings_t;
//
//
typedef union
{
  FTP_ServerSettings_t settingsFTP[NUMBER_ITEMS_DATAFTP];
  uint32_t data32[SIZE_FTP_UNION/4];
} FTP_UNION_t;
//
//
//=====================================================================================================================================
//
extern FTP_UNION_t unionFtp;
extern uint16_t g_selectedFtpServer;
//
//
//=====================================================================================================================================
//
void* FTP_AddNewFtpAdress ( void* currentItem );
void* FTP_AddNewFtpPort ( void* currentItem );
void* FTP_AddNewFtpLogin ( void* currentItem );
void* FTP_AddNewFtpPassword ( void* currentItem );
void* FTP_AddNewFtpPath ( void* currentItem );
void* FTP_ConfirmInputNewFtpServer ( void* currentItem );
void FTP_SelectFtpServer ( void );
static _Bool FTP_WriteFtpListToScreenList ( void );
FTP_ServerSettings_t* FTP_GetSettingSelectedFtpServer ( void );
void* FTP_ShowFtpList ( void* currentItem );
void* FTP_InfoAboutConnection ( void* currentItem );
void* FTP_DeleteCurrent ( void* currentItem );
//
//
//=====================================================================================================================================
//
#endif
