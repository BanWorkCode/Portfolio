#ifndef __sd_card_H
#define __sd_card_H
//
//
//=====================================================================================================================================
//
#include "main.h"
//
//
//=====================================================================================================================================
//
#define MAX_LEN_PATH  256U 
#define PATH_TO_FIRMWARE "0:/Firmware/Programmer.hex"
//
//
//=====================================================================================================================================
//
//Файл который выбирается на SD карте, что бы быть загруженным через bootloader
typedef struct{
char name[MAX_LEN_PATH];
char path[MAX_LEN_PATH];	
} SD_SelectedFile_t;
//
//
typedef enum 
{
	CARD_STATUS_OK       = 0x00U, 
	CARD_STATUS_ERROR    = 0x01U  
} SD_CardStatus_t;
//
//
//=====================================================================================================================================
//

extern SD_SelectedFile_t selectedFileToUpload;
//
//
//=====================================================================================================================================
//
SD_CardStatus_t SD_InitCard ( void );
uint8_t SD_GetListFiles ( uint16_t* numberOfDirectories );
void* SD_OpenCard ( void* currentItem );
void SD_OpenFolderOrSelectFile ( uint16_t selectedItem );
void SD_GoToParentDirectory ( void );	
_Bool SD_OpenFileOnCard ( void );
_Bool SD_GetStringFromFileOnCard ( char* stringFromFile, uint16_t leghtOfString );
_Bool SD_CloseFileOnCard ( void );
static _Bool SD_CreateFolder ( void );
void SD_OpenFileToWrite ( char* fileName );
_Bool SD_WriteByteToFile ( void* buffer );
void SD_EnterNameNewFolder ( void );
_Bool SD_DeleteFolderOrFile ( char* nameFolderOrFile );
void SD_ConfirmationDeleting ( char* nameFolderOrFile );
void SD_CreateNewFolder ( void );
void SD_SelectFolderForFtpServer ( uint16_t selectedItem );
void* SD_SelectFileForUpdateFirmware ( void* currentItem );
_Bool SD_CopyFile(char *srcFile, char *destFile);

//
//
//=====================================================================================================================================
//
#endif
