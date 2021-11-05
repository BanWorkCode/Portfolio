#ifndef fillMenu_H
#define fillMenu_H
//
//-------------------------------------------------------------
//
//
#include "main.h"
//
//-------------------------------------------------------------
//
//
#define MAIN_MENU_SIZE ((uint8_t)	9)
#define FAN_MENU_SIZE ((uint8_t)	7)
#define SETTING_MENU_SIZE ((uint8_t)	8)
#define MODE_DAY_MENU_SIZE ((uint8_t)	5)
#define MODE_MENU_SIZE ((uint8_t)	4)
#define TIME_MENU_SIZE ((uint8_t)	5)
#define DATE_MENU_SIZE ((uint8_t)	5)
#define PERIOD_MENU_SIZE ((uint8_t)	3)
#define CONFIRMATION_MENU_SIZE ((uint8_t)	3)
//
//-------------------------------------------------------------
//
//
typedef enum {
	MENU_TYPE_ONE_COLUMN = 0U,
	MENU_TYPE_TWO_COLUMN = 1U,
	MENU_TYPE_TREE_COLUMN = 2U
} MENU_Type_t;

typedef enum {
	MENU_CURSOR_FIRST_COLUMN = 0U,
	MENU_CURSOR_SECOND_COLUMN = 1U,
	MENU_CURSOR_THIRD_COLUMN = 2U
} MENU_Cursor_t;

typedef struct MenuItemStruct {
	uint8_t id;
	MENU_Type_t type;
	MENU_Cursor_t cursor;
	uint8_t *value;
	uint8_t maxVal;
	uint8_t minVal;
	uint8_t *value2;
	uint8_t maxVal2;
	uint8_t value3;
	char *name;
	char *sectionName;
	struct MenuItemStruct *up;
	struct MenuItemStruct *down;
	struct MenuItemStruct *enter;
	struct MenuItemStruct* (*function)(struct MenuItemStruct*); //Указатель на функцию выполняющуюся при нажатии на enter

} MenuItemStruct;
//
//-------------------------------------------------------------
//
//
extern MenuItemStruct *curMainMenu;
extern RTC_DateTypeDef userDate;
extern uint8_t g_switchingPeriod;
//
//-------------------------------------------------------------
//
//
void MENU_Fill(void);
MenuItemStruct* FANS_UpdateFirstDay ( MenuItemStruct* current );
//
//-------------------------------------------------------------
//
//
#endif
