#ifndef displayMenu
#define displayMenu
//
//
//=====================================================================================================================================
//
#include "stm32f4xx_hal.h"
//
//
//=====================================================================================================================================
//
#define LENGHT_INPUT_FIELD 256U
#define MAX_NUMBER_OF_VISIBLE_MENU_ITEMS 4U
#define MAX_NUMBER_OF_CATALOG_ITEMS 40U
#define MAX_LENGTH_OF_STRING_IN_CATALOG 256U

#define ROW1 13U

#define WIDTH_SCREEN_KEYBOARD 13
#define HEIGHT_SCREEN_KEYBOARD 4

#define HEIGHT_SCREEN_KEYBOARD 4

//
//
//=====================================================================================================================================
//
typedef const struct {
	void       *Next;
	void       *Previous;
	void       *Parent;
	void       *Child;
	uint8_t     Select;
	//		//0void (*function)(void); //Указатель на функцию выполняющуюся по нажатии на enter/escape
	//struct DISPLAY_MenuItem_t* (*pf)(struct DISPLAY_MenuItem_t*); //Указатель на функцию выполняющуюся при нажатии на enter/escape
	//struct PROGMEM* (*pfunc)(struct PROGMEM*);
	void* (*pfunc)(void*);
	const char  *Text;	
} DISPLAY_MenuItem_t;
//
//
typedef struct {
volatile uint8_t x;
volatile uint8_t y;	
} DISPLAY_Coordinates_t;
//
//
typedef enum {
TYPE_WINDOW_MENU = 1U,
TYPE_WINDOW_LIST = 2U,	
TYPE_WINDOW_INFO = 3U,
TYPE_WINDOW_WAITING = 4U,
TYPE_WINDOW_DOWNLOADING = 5U,
TYPE_WINDOW_UPLOADING = 6U,		
TYPE_WINDOW_KEYBOARD =7U,
TYPE_WINDOW_CONFIRM =8U	
} DISPLAY_TypeWindow_t;
//
//
typedef enum {
TYPE_LIST_FILES_SD = 0,
TYPE_LIST_NETWORKS =1U,
TYPE_LIST_FILES_FTP =2U,	
TYPE_LIST_SERVERS_FTP =3U,
TYPE_LIST_FTP_SAVE_FOLDER =4U,
TYPE_LIST_FIRMWARE =5U
	
}DISPLAY_TypeList_t;
//
//
typedef struct {
_Bool file;
uint64_t size;
//char sizeStr[10];	
char modify[16];
_Bool typeCheck;//файл/папка
_Bool sizeCheck;	
_Bool modiflyCheck;
_Bool nameCheck;	
} DISPLAY_FileInfo_t;
//
//
//=====================================================================================================================================
//
extern char* g_infoMessage;
extern char g_infoMessageArr[90];
extern DISPLAY_TypeWindow_t g_currentWindow; 
extern DISPLAY_TypeWindow_t g_previousWindow; 
extern char g_rowOfListForScreen[MAX_NUMBER_OF_CATALOG_ITEMS][MAX_LENGTH_OF_STRING_IN_CATALOG];
extern uint16_t g_numberOfListItemsForShow;
extern DISPLAY_TypeList_t g_typeList;
extern uint16_t g_currentItemOfList;
extern DISPLAY_MenuItem_t Null_Menu;
extern char g_inputField[LENGHT_INPUT_FIELD];
extern char* g_inputDestination;
extern _Bool g_blinkinCursor;
extern DISPLAY_MenuItem_t* selectedMenuItem;
void ClearInputField ( void );
extern void(* g_pressEnt)(void);
extern _Bool(* g_pressR)(char*);
extern DISPLAY_FileInfo_t g_fileInfo[MAX_NUMBER_OF_CATALOG_ITEMS];

//
//
//=====================================================================================================================================
//
#define MAKE_MENU(Name, Next, Previous, Parent, Child, Select, Func, Text) \
	extern DISPLAY_MenuItem_t Next;     \
	extern DISPLAY_MenuItem_t Previous; \
	extern DISPLAY_MenuItem_t Parent;   \
	extern DISPLAY_MenuItem_t Child;  \
	DISPLAY_MenuItem_t Name = {(void*)&Next, (void*)&Previous, (void*)&Parent, (void*)&Child, (uint8_t)Select, Func, Text}
//
//
//=====================================================================================================================================
//
static void DISPLAY_ShowGeneralInformationForLists ( void );
void DISPLAY_ShowWaiting(void);
void DISPLAY_ShowMenu(DISPLAY_MenuItem_t* currentMenu);
void DISPLAY_ShowList(void );
void DISPLAY_ShowInfo(uint16_t widthInCharacters);
void DISPLAY_ShowAdditionalNetworkInformation(void);
void DISPLAY_ShowAdditionalFileInformation ( void );
void DISPLAY_PrintString(uint8_t x, uint8_t y, char* string);
void* KeyboardMenu(void* currentItem);
void DISPLAY_ShowKeyboard(void);
void DISPLAY_AddCharToInputField(void);
void DISPLAY_SwitchKeyboardLayout(void);
void DISPLAY_SelectTheCharacterToTheRight(void);
void DISPLAY_SelectTheCharacterToTheLeft(void);
void DISPLAY_SelectTheCharacterToTheDown(void);
void DISPLAY_SelectTheCharacterToTheUp(void);
//void ClearInputField(void);
void DISPLAY_GoToSpecifiedWindow(DISPLAY_TypeWindow_t specifiedWindow);
static char* DISPLAY_MovingString(char* srcString, uint8_t startPosition);
void DISPLAY_ShowLoading ( void );
void* DISPLAY_AboutVersion ( void* currentItem );


//
//
//=====================================================================================================================================
//
#endif


