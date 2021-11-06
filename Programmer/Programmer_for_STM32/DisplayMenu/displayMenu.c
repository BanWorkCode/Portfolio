#include "displayMenu.h"
#include "ssd1306.h"
#include "stdio.h"
#include "sd_card.h"
#include "string.h"
#include "fatfs.h"
#include <stdlib.h>
#include "esp8266_lib.h"
//
//
//=====================================================================================================================================
//
char* g_infoMessage;
char g_infoMessageArr[90];
uint16_t g_numberOfListItemsForShow;
char g_rowOfListForScreen[MAX_NUMBER_OF_CATALOG_ITEMS][MAX_LENGTH_OF_STRING_IN_CATALOG];//Массив строк для вывода списка на экран
DISPLAY_FileInfo_t g_fileInfo[MAX_NUMBER_OF_CATALOG_ITEMS];

uint16_t g_currentItemOfList = 0;
DISPLAY_TypeList_t g_typeList;
DISPLAY_MenuItem_t	Null_Menu = { ( void* ) 0, ( void* ) 0, ( void* ) 0, ( void* ) 0, 0x00, ( void* ) 0, 0x00};
DISPLAY_MenuItem_t* selectedMenuItem;

DISPLAY_TypeWindow_t g_currentWindow; //Тип отображаемого окна
DISPLAY_TypeWindow_t g_previousWindow;
char g_inputField[LENGHT_INPUT_FIELD]; //строка ввода, отображается при показе экранной клавиатуры
char* g_inputDestination;  //Указатель на строку в которую будет скопировано значение g_inputField, после нажатия Ent экранной клавиатуры.

_Bool g_blinkinCursor =0;

static char* g_visibleList[MAX_NUMBER_OF_VISIBLE_MENU_ITEMS];
static DISPLAY_Coordinates_t selectedChar = {0,0};
void(* g_pressEnt)(void) = 0x00;
_Bool(* g_pressR)(char*) = 0x00;
//
//
//=====================================================================================================================================
//
extern struct
{
  uint64_t size;
  uint64_t uploadedData;
} g_fileToTransfer;
//
//
//=====================================================================================================================================
//
/*
клавиатуры должны иметь одинаковое количество символов, иначе переписать:
DISPLAY_SelectTheCharacterToTheRight
DISPLAY_SelectTheCharacterToTheLeft
DISPLAY_SelectTheCharacterToTheDown
DISPLAY_SelectTheCharacterToTheUp
*/

static char* g_screenKeyboard1[HEIGHT_SCREEN_KEYBOARD][WIDTH_SCREEN_KEYBOARD]=
{
  {"1","2","3","4","5","6","7","8","9","0","-","=","\\"},
  {"q","w","e","r","t","y","u","i","o","p","[","]"," BSp"},
  {"a","s","d","f","g","h","j","k","l",";","\'","`"," Ent"},
  {"z","x","c","v","b","n","m",",",".","/"," Spc",0x00,0x00}
};

static char* g_screenKeyboard2[HEIGHT_SCREEN_KEYBOARD][WIDTH_SCREEN_KEYBOARD]=
{
  {"!","@","#","$","%","^","&","*","(",")","_","+","|"},
  {"Q","W","E","R","T","Y","U","I","O","P","{","}"," BSp"},
  {"A","S","D","F","G","H","J","K","L",":","\"","~"," Ent"},
  {"Z","X","C","V","B","N","M","<",">","?"," Spc",0x00,0x00}
};

static _Bool g_keyboardLayout = 0;
//
//
//=====================================================================================================================================
//
void DISPLAY_GoToSpecifiedWindow ( DISPLAY_TypeWindow_t specifiedWindow )
{
	if(g_currentWindow==TYPE_WINDOW_DOWNLOADING){
	g_previousWindow = TYPE_WINDOW_MENU;
	}
  else if ( g_currentWindow!=TYPE_WINDOW_WAITING && g_currentWindow!=TYPE_WINDOW_UPLOADING)
    {
      g_previousWindow = g_currentWindow;
    }
  g_currentWindow = specifiedWindow;
}
//
//
//=====================================================================================================================================
//

void ClearInputField ( void )
{
  for ( uint16_t i=0; i<LENGHT_INPUT_FIELD; i++ )
    {
      g_inputField[i]=0x00;
    }
}

//
//
//=====================================================================================================================================
//
void DISPLAY_PrintString ( uint8_t x, uint8_t y, char* string )
{
  ssd1306_SetCursor ( x, y );
  ssd1306_WriteString ( string, Font_7x10, White );
  ssd1306_UpdateScreen();
}
//
//
//=====================================================================================================================================
//
static void DISPLAY_ShowGeneralInformationForLists ( void ){
	//Верхняя строка
	char position[5];	
	uint8_t	coordinateNextCharacter;	
	ssd1306_SetCursor ( 0, 0);		
	sprintf(position, "%i",(g_currentItemOfList+1));	
	ssd1306_WriteString (position , Font_7x10, White );// Выводим позиция выбранной строки	
	if ((g_currentItemOfList+1)<10)
	{
	coordinateNextCharacter = WIDTH_FONT_7x10;	
	}
	else if((g_currentItemOfList+1)<100){
	coordinateNextCharacter = WIDTH_FONT_7x10*2;	
	}
	else{
	coordinateNextCharacter = WIDTH_FONT_7x10*3;		
	}	
	
	
			
	ssd1306_SetCursor ( coordinateNextCharacter, 0);		
	ssd1306_WriteString ( "/", Font_7x10, White ); // Выводм слэш
	coordinateNextCharacter +=WIDTH_FONT_7x10;
		
	ssd1306_SetCursor ( coordinateNextCharacter, 0);	
	sprintf(position, "%i",(g_numberOfListItemsForShow));	// Выводим количество строк
	ssd1306_WriteString (position , Font_7x10, White );	
	
	
	
	//Разделяющая линия
	for(uint8_t i =0;i<SSD1306_WIDTH;i++)
  {			
		ssd1306_DrawPixel(i, 10, White);
  }	
	
	
	//Нижняя строка
  ssd1306_SetCursor ( 0, ROW1+HEIGHTH_FONT_7x10*4 );
  ssd1306_WriteString ( "                  ", Font_7x10, Black );//Инвертируем фон
	
		//Выводим информацию о назначении клавиш
 if (g_typeList == TYPE_LIST_FILES_SD){
  ssd1306_SetCursor ( 0, ROW1+HEIGHTH_FONT_7x10*4 );	
	ssd1306_WriteString ( "<-Del", Font_7x10, Black );

	
	ssd1306_SetCursor ( WIDTH_FONT_7x10*10 , ROW1+HEIGHTH_FONT_7x10*4 );	
	ssd1306_WriteString ( "New fl->", Font_7x10, Black );
 }
 else if  (g_typeList == TYPE_LIST_FILES_FTP) {
	 
	 char temp[19]="";
	 strncpy(temp, g_fileInfo[g_currentItemOfList].modify, 4);
	 temp[4] = '.';
	 strncpy(&temp[5], &g_fileInfo[g_currentItemOfList].modify[4], 2);
	 temp[7] = '.';
	 strncpy(&temp[8], &g_fileInfo[g_currentItemOfList].modify[6], 2);
	 temp[10] = ' ';
	 strncpy(&temp[11], &g_fileInfo[g_currentItemOfList].modify[8], 2);
	 temp[13] = ':';
	 strncpy(&temp[14], &g_fileInfo[g_currentItemOfList].modify[10], 2);
	 temp[16] = 0;
	 ssd1306_SetCursor ( 0, ROW1+HEIGHTH_FONT_7x10*4 );	 
	 ssd1306_WriteString ( temp, Font_7x10, Black );
	 
 
 }
 
  else if  (g_typeList == TYPE_LIST_NETWORKS) {
		ssd1306_SetCursor ( WIDTH_FONT_7x10*2, ROW1+HEIGHTH_FONT_7x10*4 );
		ssd1306_WriteString ( "Enter password->", Font_7x10, Black ); 
 }
	  else if  (g_typeList == TYPE_LIST_SERVERS_FTP) {
		ssd1306_SetCursor ( WIDTH_FONT_7x10*1, ROW1+HEIGHTH_FONT_7x10*4 );
		ssd1306_WriteString ( "Server settings->", Font_7x10, Black ); 
 }
	else if  (g_typeList == TYPE_LIST_FIRMWARE ) {		
		ssd1306_SetCursor ( 0, ROW1+HEIGHTH_FONT_7x10*4 );
		if (g_fileInfo[g_currentItemOfList].file){
		ssd1306_WriteString ( "Ent: Select & load", Font_7x10, Black ); 
		}
		else{
		ssd1306_WriteString ( "Ent: Open", Font_7x10, Black ); 
		}
 }
		else if  (g_typeList == TYPE_LIST_FTP_SAVE_FOLDER) {
			if(g_fileInfo[g_currentItemOfList].file==0){
		ssd1306_SetCursor ( WIDTH_FONT_7x10*2, ROW1+HEIGHTH_FONT_7x10*4 );
		ssd1306_WriteString ( "Select the dir->", Font_7x10, Black ); 
			}
 }
	
	
}



void DISPLAY_ShowWaiting ( void )
{
  static uint8_t	xWaiting =0;
  static uint8_t	yWaiting =0;
  static _Bool directionWaiting =0;
  ssd1306_SetCursor ( xWaiting, yWaiting );
  ssd1306_WriteString ( "Please wait...", Font_7x10, White );
  if 	( directionWaiting==0 )
    {
      xWaiting++;
      yWaiting++;
      if ( xWaiting==25 )
        {
          directionWaiting=1;
        }
    }
  else
    {
      xWaiting--;
      yWaiting--;
      if ( xWaiting==0 )
        {
          directionWaiting=0;
        }
    }
}
//
//
//=====================================================================================================================================
//
void DISPLAY_ShowLoading ( void )
{
sprintf (g_infoMessageArr, "%i %%", (uint8_t)(((float)g_fileToTransfer.uploadedData/(float)g_fileToTransfer.size)*100) );

	ssd1306_SetCursor ( 0, 0 );
  ssd1306_WriteString ( "Dowload ", Font_7x10, White );
	ssd1306_SetCursor ( 0, 10 );
  ssd1306_WriteString ( g_infoMessageArr, Font_7x10, White );
	
	
	
}

//
//
//=====================================================================================================================================
//Для работы функции ей нужно:
//g_numberOfListItemsForShow
//заполненый массив g_rowOfListForScreen
//g_currentItemOfList



void DISPLAY_ShowList ( void )
{
  uint16_t numberOfVisibleItems = 0;
	static uint8_t pointerPosition = 0;
  //Вывод указателя на пункт меню ('>').
  if ( g_numberOfListItemsForShow == 0 )
    {
      g_infoMessage = "ERROR: No files and directories!";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  else if ( g_numberOfListItemsForShow >= MAX_NUMBER_OF_CATALOG_ITEMS )
    {
      g_infoMessage = "ERROR: Too many items in the directory!";
      DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
    }
  //Дописать!!!
  //Определяем количество отображаемых элементов списка
  if ( g_numberOfListItemsForShow >= MAX_NUMBER_OF_VISIBLE_MENU_ITEMS )
    {
      numberOfVisibleItems = MAX_NUMBER_OF_VISIBLE_MENU_ITEMS;
    }
  else
    {
      numberOfVisibleItems = g_numberOfListItemsForShow;
    }
  //если количество элементов списка, после текущего элемента больше, чем количество отображаемых
  if ( g_numberOfListItemsForShow - g_currentItemOfList >= numberOfVisibleItems )
    {
			pointerPosition =0;
      //тогда отображаем  от g_currentItemOfList до numberOfVisibleItems
      int j = 0;
      for ( uint16_t i = g_currentItemOfList; i < g_currentItemOfList + numberOfVisibleItems; i++ )
        {
          g_visibleList[j] = g_rowOfListForScreen[i];
          j++;
        }
      ssd1306_SetCursor ( 0, pointerPosition+ROW1 );
      ssd1306_WriteString ( ">", Font_7x10, White );
    }
  else
    {
      //тогда выводим последние, в количестве видимых
      int j = 0;
      for ( uint16_t i = g_numberOfListItemsForShow - numberOfVisibleItems; i < g_numberOfListItemsForShow; i++ )
        {
          g_visibleList[j] = g_rowOfListForScreen[i];
          j++;
        }
      //															3												3												1
      pointerPosition = ( numberOfVisibleItems - ( g_numberOfListItemsForShow - g_currentItemOfList ) );
      
			ssd1306_SetCursor ( 0, pointerPosition*10+ROW1 );
      ssd1306_WriteString ( ">", Font_7x10, White );
    }
		
		
  uint16_t y = ROW1;
  for ( uint16_t i = 0; i < numberOfVisibleItems; i++ )
    {
      ssd1306_SetCursor ( 15, y );
			
			if (i==pointerPosition){
			ssd1306_WriteString ( DISPLAY_MovingString(g_visibleList[i], 15), Font_7x10, White );      
			}
			else{
			ssd1306_WriteString ( g_visibleList[i], Font_7x10, White );
			}
			
      y += 10;
    }
		
		DISPLAY_ShowGeneralInformationForLists();
		
}
//
//
//=====================================================================================================================================
//
void DISPLAY_ShowInfo ( uint16_t widthInCharacters )
{
  uint16_t y = 0;
  uint16_t startOfSubstring = 0;
  uint16_t endOfSubstring = startOfSubstring;
  uint16_t lengthOfMessage = strlen ( g_infoMessage );
  char substring[widthInCharacters + 1];
  uint16_t i = 0;
  uint16_t j = 0;
  while ( i < lengthOfMessage )
    {
			  if ( g_infoMessage[i] == 0x0D ||g_infoMessage[i] == 0x0A )
        {
          endOfSubstring = i;					
          substring[endOfSubstring - startOfSubstring] = 0x00;
          ssd1306_SetCursor ( 0, y );
          ssd1306_WriteString ( substring, Font_7x10, White );
          y = y + 10;
          j = 0;
          i = endOfSubstring + 1;
          startOfSubstring = i;
          endOfSubstring = startOfSubstring;	
        }
				else{
			
      substring[j] = g_infoMessage[i];
      if ( g_infoMessage[i] == ' ' )
        {
          endOfSubstring = i;
        }
      if ( ( i == widthInCharacters + startOfSubstring ) && endOfSubstring != startOfSubstring )
        {
          substring[endOfSubstring - startOfSubstring] = 0x00;
          ssd1306_SetCursor ( 0, y );
          ssd1306_WriteString ( substring, Font_7x10, White );
          y = y + 10;
          j = 0;
          i = endOfSubstring + 1;
          startOfSubstring = i;
          endOfSubstring = startOfSubstring;
        }
				//слово длиннее ширины экрана
      else if ( ( i == widthInCharacters + startOfSubstring ) && endOfSubstring == startOfSubstring )
        {
          substring[widthInCharacters] = 0x00;
          ssd1306_SetCursor ( 0, y );
          ssd1306_WriteString ( substring, Font_7x10, White );
          y = y + 10;
          j = 0;
          i = widthInCharacters + startOfSubstring;
          startOfSubstring = i;
          endOfSubstring = startOfSubstring;
        }
      else
        {
          i++;
          j++;
        }
				
			}
      if ( i == lengthOfMessage )
        {
          substring[j] = 0x00;
          ssd1306_SetCursor ( 0, y );
          ssd1306_WriteString ( substring, Font_7x10, White );
        }
    } // End While
} // End DISPLAY_ShowInfo
//
//
//=====================================================================================================================================
//
void DISPLAY_ShowMenu ( DISPLAY_MenuItem_t* currentMenu )
{
   //Переписать под универсальное количество строк!
  DISPLAY_MenuItem_t* temp;
  uint8_t numberOfItemsHigher = 0;
  uint8_t numberOfItemsBelow = 0;
  uint8_t numberOfItems = 0;
  uint16_t numberOfVisibleItemsMenu = 0;
  //определяем количество пунктов меню выше текущего
  temp = currentMenu;
  while ( temp->Previous != &Null_Menu )
    {
      temp = temp->Previous;
      numberOfItemsHigher++;
    }
  //определяем количество пунктов меню ниже текущего
  temp = currentMenu;
  while ( temp->Next != &Null_Menu )
    {
      temp = temp->Next;
      numberOfItemsBelow++;
    }
  //определяем количество видимых пунктов меню
  numberOfItems = numberOfItemsHigher + numberOfItemsBelow + 1;
  //определяем количество видимых пунктов меню
  if ( numberOfItems >= MAX_NUMBER_OF_VISIBLE_MENU_ITEMS )
    {
      numberOfVisibleItemsMenu = MAX_NUMBER_OF_VISIBLE_MENU_ITEMS;
    }
  else
    {
      numberOfVisibleItemsMenu = numberOfItems;
    }
  temp = currentMenu;
		//информация о количестве пунктов и заряде батареи
	char tempSrt[16];	
	uint8_t	coordinateNextCharacter;	
	ssd1306_SetCursor ( 0, 0);		
	sprintf(tempSrt, "%i",(numberOfItemsHigher+1));	
	ssd1306_WriteString (tempSrt , Font_7x10, White );
	if ((numberOfItemsHigher+1)<10)
	{
	coordinateNextCharacter = 7;	
	}
	else if((numberOfItemsHigher+1)<100){
	coordinateNextCharacter = 14;	
	}
	else{
	coordinateNextCharacter = 21;		
	}	
		
	ssd1306_SetCursor ( coordinateNextCharacter, 0);		
	ssd1306_WriteString ( "/", Font_7x10, White );
	coordinateNextCharacter +=7;
		
	ssd1306_SetCursor ( coordinateNextCharacter, 0);	
	sprintf(tempSrt, "%i",(numberOfItemsBelow+numberOfItemsHigher+1));	
	ssd1306_WriteString (tempSrt , Font_7x10, White );	
		
	ssd1306_SetCursor ( 99, 0);	
	
	
	if (batteryVoltage>2180){
	ssd1306_WriteString ("100%" , Font_7x10, White );
	}
	else if(batteryVoltage<1840){
	ssd1306_WriteString ("0%" , Font_7x10, White );
	}
	else
	sprintf(tempSrt, "%i%%",(uint8_t)((batteryVoltage-1840)/3.4f));	
	ssd1306_WriteString (tempSrt , Font_7x10, White );	
	
	
	
	//Разделяющая строка
	for(uint8_t i =0;i<128;i++)
  {			
		ssd1306_DrawPixel(i, 10, White);
  }
	
	

		
		
		
  //если количество элементов меню, после текущего элемента больше, чем количество отображаемых
  if ( ( numberOfItemsBelow + 1 ) >= numberOfVisibleItemsMenu )
    {
      ssd1306_SetCursor ( 0, ROW1 ); // то указатель на первой строке //исправить
      ssd1306_WriteString ( ">", Font_7x10, White );
      for ( uint8_t i = 0; i < numberOfVisibleItemsMenu; i++ )
        {
          ssd1306_SetCursor ( 15, ROW1+( i * 10 ) );
          ssd1306_WriteString ( ( char* ) temp->Text, Font_7x10, White );
          temp = temp->Next;
        } //End for
    }
  else
    {
      temp = currentMenu;
      uint8_t a = numberOfVisibleItemsMenu - numberOfItemsBelow - 1;
      ssd1306_SetCursor ( 0, ROW1+( a * 10 ) );
      ssd1306_WriteString ( ">", Font_7x10, White );
      for ( uint8_t i = numberOfVisibleItemsMenu - numberOfItemsBelow - 1; i < numberOfVisibleItemsMenu; i++ )
        {
          ssd1306_SetCursor ( 15, ROW1+( i * 10 ) );
          ssd1306_WriteString ( ( char* ) temp->Text, Font_7x10, White );
          temp = temp->Next;
        }
      temp = currentMenu;
      for ( int i = numberOfVisibleItemsMenu - numberOfItemsBelow - 1; i >= 0; i-- )
        {
          ssd1306_SetCursor ( 15, ROW1+( i * 10 ) );
          ssd1306_WriteString ( ( char* ) temp->Text, Font_7x10, White );
          temp = temp->Previous;
        }
    }
}
//
//
//=====================================================================================================================================
//
void DISPLAY_ShowAdditionalNetworkInformation ( void )
{
  _Bool unlock =0;
  uint16_t numStr =0;
  for ( uint16_t i=0; i<NUMBER_ITEMS_DATAWIFI; i++ )
    {
      if ( strcmp ( g_unionWifi.settingsWIFI[i].nameWIFI, g_rowOfListForScreen[g_currentItemOfList] ) ==0 && g_unionWifi.settingsWIFI[i].passwordWIFI[0]!=0x00 )
        {
          unlock =1;
          numStr =i;
        }
    }

  
  if ( unlock )
    {
      if ( g_unionWifi.settingsWIFI[numStr].connection )
        {
          ssd1306_SetCursor ( WIDTH_FONT_7x10*9, 0 );
          ssd1306_WriteString ( "Connected", Font_7x10, White );
        }
				else{
							ssd1306_SetCursor ( WIDTH_FONT_7x10*12, 0 );
      ssd1306_WriteString ( "Unlock", Font_7x10, White );
				
				}				
    }
  else
    {
			ssd1306_SetCursor ( WIDTH_FONT_7x10*14, 0 );
      ssd1306_WriteString ( "Lock", Font_7x10, White );
    }
		

}

//
//
//=====================================================================================================================================
//
void DISPLAY_ShowAdditionalFileInformation ( void ){
	//Верхняя строка
	//Подбираем соответстствующую величину размера файла 
		if(g_fileInfo[g_currentItemOfList].file){
			char sizeStr[10];		
    if (g_fileInfo[g_currentItemOfList].size <= 1024) {

        sprintf(sizeStr, "%llub", g_fileInfo[g_currentItemOfList].size);
    }
    else if (g_fileInfo[g_currentItemOfList].size <= 1048576) {

        sprintf(sizeStr, "%.2fKb", (double)g_fileInfo[g_currentItemOfList].size / 1024);
    }
    else if (g_fileInfo[g_currentItemOfList].size <= 1073741824) {

        sprintf(sizeStr, "%.2fMb", (double)g_fileInfo[g_currentItemOfList].size / 1024/1024);
    }
    else if (g_fileInfo[g_currentItemOfList].size <= 1099511627776) {

        sprintf(sizeStr, "%.2fGb", (double)g_fileInfo[g_currentItemOfList].size / 1024 / 1024/1024);
    }
    else if (g_fileInfo[g_currentItemOfList].size <= 1125899906842624) {

        sprintf(sizeStr, "%.2fTb", (double)g_fileInfo[g_currentItemOfList].size / 1024 / 1024 / 1024/1024);
    }
    else {

        sprintf(sizeStr, "%s", "Too many");
    }

		
		//Выводим размер фаила либо сообщение о том что это папка	
	ssd1306_SetCursor ( (18-strlen(sizeStr))*WIDTH_FONT_7x10, 0);
	ssd1306_WriteString (sizeStr , Font_7x10, White ); //Размер файла
	}
	else{
	ssd1306_SetCursor ( WIDTH_FONT_7x10*15, 0 );	
	ssd1306_WriteString ( "Dir", Font_7x10, White ); //Папка
}	
	
}







static char* DISPLAY_MovingString(char* srcString, uint8_t startPosition){
	static uint16_t offset =0;
	static _Bool blCursor =0;
	static _Bool delayBlinking =0;
	
		if (strlen(srcString+offset)*7+startPosition > SSD1306_WIDTH){	
		if (blCursor!=g_blinkinCursor){
		blCursor=g_blinkinCursor;			

		if (delayBlinking==0 && offset==0){
		delayBlinking=1;
		}
		else{
		offset++;
		delayBlinking=0;	
		}
	
	}
		}	
		else{
offset=0;

}		
		
return 	srcString+offset;

}




//
//
//=====================================================================================================================================
//----------Экранная клавиатура--------------------------------------------------------------------------------------------------------
//=====================================================================================================================================
//
//отладка удалить!!!
void* KeyboardMenu ( void* currentItem )
{
  DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_KEYBOARD );
  return 	currentItem;
}
//
//
//=====================================================================================================================================
//
void DISPLAY_ShowKeyboard ( void )
{
  uint8_t x, y = 0;
  ssd1306_SetCursor ( 0, 0 );
  if ( strlen ( g_inputField ) >17 )
    {
      ssd1306_WriteString ( &g_inputField[strlen ( g_inputField )-17],Font_7x10, White );
      if ( g_blinkinCursor )
        {
          ssd1306_SetCursor ( 17*Font_7x10.FontWidth, 0 );
          ssd1306_WriteChar ( ' ',Font_7x10, Black );
        }
    }
  else
    {
      ssd1306_WriteString ( g_inputField,Font_7x10, White );
      if ( g_blinkinCursor )
        {
          ssd1306_SetCursor ( strlen ( g_inputField ) *Font_7x10.FontWidth, 0 );
          ssd1306_WriteChar ( ' ',Font_7x10, Black );
        }
    }
  for ( uint32_t h=0; h < HEIGHT_SCREEN_KEYBOARD; h++ )
    {
      for ( uint32_t w=0; w < WIDTH_SCREEN_KEYBOARD; w++ )
        {
          x = w*Font_7x10.FontWidth;
          y = h*Font_7x10.FontHeight;
          ssd1306_SetCursor ( x, y+Font_7x10.FontHeight );
          if ( g_keyboardLayout==0 )
            {
              if ( g_screenKeyboard1[h][w]!=0x00 )
                {
                  if ( h==selectedChar.y && w==selectedChar.x )
                    {
                      ssd1306_WriteString ( g_screenKeyboard1[h][w],Font_7x10, Black );
                    }
                  else
                    {
                      ssd1306_WriteString ( g_screenKeyboard1[h][w],Font_7x10, White );
                    }
                }
            }
          else
            {
              if ( g_screenKeyboard2[h][w]!=0x00 )
                {
                  if ( h==selectedChar.y && w==selectedChar.x )
                    {
                      ssd1306_WriteString ( g_screenKeyboard2[h][w],Font_7x10, Black );
                    }
                  else
                    {
                      ssd1306_WriteString ( g_screenKeyboard2[h][w],Font_7x10, White );
                    }
                }
            }
        }
    }
};
//
//
//=====================================================================================================================================
//



void DISPLAY_AddCharToInputField ( void )
{
  if ( g_keyboardLayout==0 )
    {
      if ( strcmp ( g_screenKeyboard1[selectedChar.y][selectedChar.x], " Spc" ) ==0 )
        {
          strcat ( g_inputField, " " );
        }
      else if ( strcmp ( g_screenKeyboard1[selectedChar.y][selectedChar.x], " BSp" ) ==0 )
        {
          g_inputField[strlen ( g_inputField )-1] =0x00;
        }
      else if ( strcmp ( g_screenKeyboard1[selectedChar.y][selectedChar.x], " Ent" ) ==0 )
        {
          strcpy ( g_inputDestination, g_inputField );
					if (g_pressEnt!=0x00){
					g_pressEnt();
					}
          DISPLAY_GoToSpecifiedWindow ( g_previousWindow );
        }
      else
        {
          strcat ( g_inputField, g_screenKeyboard1[selectedChar.y][selectedChar.x] );
        }
    }
  else
    {
      if ( strcmp ( g_screenKeyboard2[selectedChar.y][selectedChar.x], " Spc" ) ==0 )
        {
          strcat ( g_inputField, " " );
        }
      else if ( strcmp ( g_screenKeyboard2[selectedChar.y][selectedChar.x], " BSp" ) ==0 )
        {
          g_inputField[strlen ( g_inputField )-1] =0x00;
        }
      else if ( strcmp ( g_screenKeyboard2[selectedChar.y][selectedChar.x], " Ent" ) ==0 )
        {
          strcpy ( g_inputDestination, g_inputField );
					if (g_pressEnt!=0x00){
					g_pressEnt();
					}
					
          DISPLAY_GoToSpecifiedWindow ( g_previousWindow );
        }
      else
        {
          strcat ( g_inputField, g_screenKeyboard2[selectedChar.y][selectedChar.x] );
        }
    }
}
//
//
//=====================================================================================================================================
//
void DISPLAY_SwitchKeyboardLayout ( void )
{
  if ( g_keyboardLayout==0 )
    {
      g_keyboardLayout=1;
    }
  else
    {
      g_keyboardLayout=0;
    }
}
//
//
//=====================================================================================================================================
//
void DISPLAY_SelectTheCharacterToTheRight ( void )
{
  do
    {
      if 	( selectedChar.x==WIDTH_SCREEN_KEYBOARD-1 )
        {
          selectedChar.x =0;
        }
      else
        {
          selectedChar.x++;
        }
    }
  while ( g_screenKeyboard1[selectedChar.y][selectedChar.x]==0x00 );
}
//
//
//=====================================================================================================================================
//
void DISPLAY_SelectTheCharacterToTheLeft ( void )
{
  do
    {
      if 	( selectedChar.x==0 )
        {
          selectedChar.x =WIDTH_SCREEN_KEYBOARD-1;
        }
      else
        {
          selectedChar.x--;
        }
    }
  while ( g_screenKeyboard1[selectedChar.y][selectedChar.x]==0x00 );
}
//
//
//=====================================================================================================================================
//
void DISPLAY_SelectTheCharacterToTheDown ( void )
{
  do
    {
      if 	( selectedChar.y==HEIGHT_SCREEN_KEYBOARD-1 )
        {
          selectedChar.y =0;
        }
      else
        {
          selectedChar.y++;
        }
    }
  while ( g_screenKeyboard1[selectedChar.y][selectedChar.x]==0x00 );
}
//
//
//=====================================================================================================================================
//
void DISPLAY_SelectTheCharacterToTheUp ( void )
{
  do
    {
      if 	( selectedChar.y==0 )
        {
          selectedChar.y =HEIGHT_SCREEN_KEYBOARD-1;
        }
      else
        {
          selectedChar.y--;
        }
    }
  while ( g_screenKeyboard1[selectedChar.y][selectedChar.x]==0x00 );
}
//
//
//=====================================================================================================================================
//
void* DISPLAY_AboutVersion ( void* currentItem )
{
g_infoMessage = "Proframmer. Version 0.58";	
DISPLAY_GoToSpecifiedWindow ( TYPE_WINDOW_INFO );
return currentItem;
}
