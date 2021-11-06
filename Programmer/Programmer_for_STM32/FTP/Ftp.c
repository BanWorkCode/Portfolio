#include "main.h"
#include "ftp.h"
#include "string.h"
#include "displayMenu.h"
#include "cmsis_os.h"
#include <stdlib.h>
#include "storingSettings.h"
#include "sd_card.h"
//
//
//=====================================================================================================================================
//Template:
//FTP_UNION_t unionFtp={{{"192.168.1.224","21","maxim","maxim"},{"192.168.1.172","21","maxim","maxim"}}};
FTP_UNION_t unionFtp;
uint16_t g_selectedFtpServer = FTP_MAGIC_VALUE_SELECTED_SERVER;
	
//
//
//=====================================================================================================================================
//
void FTP_SelectFtpServer ( void )
{
  for ( uint16_t i=0; i<NUMBER_ITEMS_DATAFTP; i++ )
    {
      if ( strcmp ( unionFtp.settingsFTP[i].address, g_rowOfListForScreen[g_currentItemOfList] ) ==0 )
        {
          g_selectedFtpServer = i;
          break;
        }
    }
}
//
//
//=====================================================================================================================================
//
FTP_ServerSettings_t* FTP_GetSettingSelectedFtpServer ( void )
{
  return &unionFtp.settingsFTP[g_selectedFtpServer];
}
//
//
//=====================================================================================================================================
//
//FTP TYPE_WINDOW_LIST-----------------------------------------------------------
static _Bool FTP_WriteFtpListToScreenList ( void )
{
	g_currentItemOfList =0;
  uint16_t i=0;
	for(uint16_t j =0; j<NUMBER_ITEMS_DATAFTP;j++)
  {
		if(unionFtp.settingsFTP[j].address[0] !=0x00){
		strcpy ( g_rowOfListForScreen[i], unionFtp.settingsFTP[j].address );
		i++;
		}
		
  }
	
//  while ( unionFtp.settingsFTP[i].address[0] !=0x00 )
//    {
//      strcpy ( g_rowOfListForScreen[i], unionFtp.settingsFTP[i].address );
//      i++;
//    }
  g_numberOfListItemsForShow =i;
  return 0;
}
//
//
//=====================================================================================================================================
//
void* FTP_ShowFtpList ( void* currentItem )
{
  DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_WAITING);
  STORING_ReadSettingsFromFlash();
  if ( FTP_WriteFtpListToScreenList() )
    {
      g_infoMessage = "Error! No FTP servers available";
      DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_INFO);
    }
  else
    {
      g_typeList = TYPE_LIST_SERVERS_FTP;
      g_currentItemOfList =0;
      DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_LIST);
    }
  return 	currentItem;
}
//
//
//=====================================================================================================================================
//
void* FTP_AddNewFtpAdress ( void* currentItem )
{
  //ищем свободное место в массиве settingsFTP
	
  if ( g_selectedFtpServer == FTP_MAGIC_VALUE_SELECTED_SERVER )
    {
      for ( uint16_t i=0; i<NUMBER_ITEMS_DATAFTP; i++ )
        {
          if ( unionFtp.settingsFTP[i].address[0]==0x00 )
            {
              g_selectedFtpServer =i;
              break;
            }
        }
      if ( g_selectedFtpServer == FTP_MAGIC_VALUE_SELECTED_SERVER )
        {
          g_infoMessage = "No free memory for new FTP record!";
          DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_INFO);
          return	currentItem;
        }
    }
  g_inputDestination = unionFtp.settingsFTP[g_selectedFtpServer].address;
  
  strcpy ( g_inputField, g_inputDestination );

  //g_infoMessage = "Please enter the address of the new FTP server! Press the button \"Enter\"";
  DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_KEYBOARD);
  return 	currentItem;
}
//
//
//=====================================================================================================================================
//
void* FTP_AddNewFtpPort ( void* currentItem )
{
  //ищем свободное место в массиве settingsFTP
  if ( g_selectedFtpServer == FTP_MAGIC_VALUE_SELECTED_SERVER )
    {
      g_infoMessage = "first enter the address";
      DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_INFO);
      return	currentItem;
    }
  g_inputDestination = unionFtp.settingsFTP[g_selectedFtpServer].port;
  
  strcpy ( g_inputField, g_inputDestination );
	DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_KEYBOARD);
  return 	currentItem;
}
//
//
//=====================================================================================================================================
//
void* FTP_AddNewFtpLogin ( void* currentItem )
{
  //ищем свободное место в массиве settingsFTP
  if ( g_selectedFtpServer == FTP_MAGIC_VALUE_SELECTED_SERVER )
    {
      g_infoMessage = "first enter the address";
      DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_INFO);
      return	currentItem;
    }
  g_inputDestination = unionFtp.settingsFTP[g_selectedFtpServer].login;
  
  strcpy ( g_inputField, g_inputDestination );
	DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_KEYBOARD);
  return 	currentItem;
}
//
//
//=====================================================================================================================================
//
void* FTP_AddNewFtpPassword ( void* currentItem )
{
  //ищем свободное место в массиве settingsFTP
  if ( g_selectedFtpServer == FTP_MAGIC_VALUE_SELECTED_SERVER )
    {
      g_infoMessage = "First enter the address";
      DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_INFO);
      return	currentItem;
    }
  g_inputDestination = unionFtp.settingsFTP[g_selectedFtpServer].password;
 
  strcpy ( g_inputField, g_inputDestination );
DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_KEYBOARD);
  return 	currentItem;
}
//
//
//=====================================================================================================================================
//
void* FTP_AddNewFtpPath ( void* currentItem )
{
if ( SD_GetListFiles ( &g_numberOfListItemsForShow ) )
    {
      g_infoMessage = "ERROR: Get list files error!";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  else
    {
      g_typeList = TYPE_LIST_FTP_SAVE_FOLDER;
      g_currentItemOfList =0;
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_LIST );
    }
	
  return 	currentItem;
}
//
//
//=====================================================================================================================================
//
void* FTP_ConfirmInputNewFtpServer ( void* currentItem )
{
  //g_previousWindow = TYPE_WINDOW_MENU;
  if ( g_selectedFtpServer !=FTP_MAGIC_VALUE_SELECTED_SERVER && unionFtp.settingsFTP[g_selectedFtpServer].address[0]!=0x00 &&\
       unionFtp.settingsFTP[g_selectedFtpServer].port[0]!=0x00 && unionFtp.settingsFTP[g_selectedFtpServer].path[0]!=0x00)
    {
      g_selectedFtpServer = FTP_MAGIC_VALUE_SELECTED_SERVER;
//selectedMenuItem = &Boot_MAIN;
//selectedMenuItem = selectedMenuItem->Parent;
      if ( STORING_SaveSettingsToFlash() )
        {
//ошибка
          g_infoMessage = "Error, settings not saved!";
          DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_INFO);
          return currentItem;
        }
      DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_MENU);
    }
  else
    {
      g_infoMessage = "Please enter adress, port and path.";
      DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_INFO);
      return currentItem;
    }
  return selectedMenuItem->Parent;
}

//
//
//=====================================================================================================================================
//
void* FTP_InfoAboutConnection ( void* currentItem )
{

      g_infoMessage = unionFtp.settingsFTP[g_selectedFtpServer].path;
      DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_INFO);

  return currentItem;
}
//
//
//=====================================================================================================================================
//

void* FTP_DeleteCurrent ( void* currentItem )
{
			unionFtp.settingsFTP[g_selectedFtpServer].address[0] =0;
			unionFtp.settingsFTP[g_selectedFtpServer].login[0] =0;
			unionFtp.settingsFTP[g_selectedFtpServer].password[0] =0;
			unionFtp.settingsFTP[g_selectedFtpServer].port[0] =0;
      unionFtp.settingsFTP[g_selectedFtpServer].path[0] =0;
	
	      if ( STORING_SaveSettingsToFlash() )
        {
//ошибка
          g_infoMessage = "Error, settings not saved!";
          DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_INFO);
          return currentItem;
        }
      DISPLAY_GoToSpecifiedWindow(TYPE_WINDOW_MENU);
    
  

  return currentItem;
}
