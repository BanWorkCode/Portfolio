#include <main.h>
#include "fillMenu.h"
#include "fans.h"
#include "display.h"
//
//-------------------------------------------------------------
//
//
MenuItemStruct mainMenu[MAIN_MENU_SIZE];

MenuItemStruct fan1Menu[FAN_MENU_SIZE];
MenuItemStruct fan2Menu[FAN_MENU_SIZE];
MenuItemStruct fan3Menu[FAN_MENU_SIZE];
MenuItemStruct fan4Menu[FAN_MENU_SIZE];
MenuItemStruct fan5Menu[FAN_MENU_SIZE];
MenuItemStruct fan6Menu[FAN_MENU_SIZE];
MenuItemStruct fan7Menu[FAN_MENU_SIZE];
MenuItemStruct fan8Menu[FAN_MENU_SIZE];

MenuItemStruct setting[SETTING_MENU_SIZE];
MenuItemStruct modeDay[MODE_DAY_MENU_SIZE];
MenuItemStruct mode0[MODE_MENU_SIZE];
MenuItemStruct mode1[MODE_MENU_SIZE];
MenuItemStruct mode2[MODE_MENU_SIZE];
MenuItemStruct mode3[MODE_MENU_SIZE];
MenuItemStruct time[TIME_MENU_SIZE];
MenuItemStruct date[DATE_MENU_SIZE];
MenuItemStruct startDay[TIME_MENU_SIZE];
MenuItemStruct endDay[TIME_MENU_SIZE];
MenuItemStruct periodT[PERIOD_MENU_SIZE];
MenuItemStruct areYouSure[CONFIRMATION_MENU_SIZE];

MenuItemStruct *curMainMenu = &mainMenu[0];
//
//-------------------------------------------------------------
//
//
static void MENU_FillSection(MenuItemStruct *section, uint8_t numItemInSection, char *sectionName);
//
//-------------------------------------------------------------
//
//
void MENU_Fill(void) {
	MENU_FillSection(mainMenu, MAIN_MENU_SIZE, "Main");

	mainMenu[0].name = "Fan 1";
	mainMenu[0].enter = &fan1Menu[0];
	mainMenu[0].function = DISPLAY_GoesToAndShowInfo;
	mainMenu[0].value3 = 1;

	mainMenu[1].name = "Fan 2";
	mainMenu[1].enter = &fan2Menu[0];
	mainMenu[1].function = DISPLAY_GoesTo;
	mainMenu[1].value3 = 2;

	mainMenu[2].name = "Fan 3";
	mainMenu[2].enter = &fan3Menu[0];
	mainMenu[2].function = DISPLAY_GoesTo;
	mainMenu[2].value3 = 3;

	mainMenu[3].name = "Fan 4";
	mainMenu[3].enter = &fan4Menu[0];
	mainMenu[3].function = DISPLAY_GoesTo;
	mainMenu[3].value3 = 4;

	mainMenu[4].name = "Fan 5";
	mainMenu[4].enter = &fan5Menu[0];
	mainMenu[4].function = DISPLAY_GoesTo;
	mainMenu[4].value3 = 5;

	mainMenu[5].name = "Fan 6";
	mainMenu[5].enter = &fan6Menu[0];
	mainMenu[5].function = DISPLAY_GoesTo;
	mainMenu[5].value3 = 6;

	mainMenu[6].name = "Fan 7";
	mainMenu[6].enter = &fan7Menu[0];
	mainMenu[6].function = DISPLAY_GoesTo;
	mainMenu[6].value3 = 7;

	mainMenu[7].name = "Fan 8";
	mainMenu[7].enter = &fan8Menu[0];
	mainMenu[7].function = DISPLAY_GoesTo;
	mainMenu[7].value3 = 8;

	mainMenu[8].name = "Setting";
	mainMenu[8].enter = &setting[0];
	mainMenu[8].function = DISPLAY_GoesTo;

	//Fan1 menu
	MENU_FillSection(fan1Menu, FAN_MENU_SIZE, "Fan1  ModeN DaysC");

	fan1Menu[0].name = "Stage1";
	fan1Menu[0].type = MENU_TYPE_TREE_COLUMN;
	fan1Menu[0].value = &ProgramForFun[0].modeNumber1;
	fan1Menu[0].maxVal = 3;
	fan1Menu[0].value2 = &ProgramForFun[0].numberOfDays1;
	fan1Menu[0].maxVal2 = 30;
	fan1Menu[0].value3 = 1;

	fan1Menu[1].name = "Stage2";
	fan1Menu[1].type = MENU_TYPE_TREE_COLUMN;
	fan1Menu[1].value = &ProgramForFun[0].modeNumber2;
	fan1Menu[1].maxVal = 3;
	fan1Menu[1].value2 = &ProgramForFun[0].numberOfDays2;
	fan1Menu[1].maxVal2 = 30;
	fan1Menu[1].value3 = 1;

	fan1Menu[2].name = "Stage3";
	fan1Menu[2].type = MENU_TYPE_TREE_COLUMN;
	fan1Menu[2].value = &ProgramForFun[0].modeNumber3;
	fan1Menu[2].maxVal = 3;
	fan1Menu[2].value2 = &ProgramForFun[0].numberOfDays3;
	fan1Menu[2].maxVal2 = 30;
	fan1Menu[2].value3 = 1;

	fan1Menu[3].name = "Stage4";
	fan1Menu[3].type = MENU_TYPE_TREE_COLUMN;
	fan1Menu[3].value = &ProgramForFun[0].modeNumber4;
	fan1Menu[3].maxVal = 3;
	fan1Menu[3].value2 = &ProgramForFun[0].numberOfDays4;
	fan1Menu[3].maxVal2 = 30;
	fan1Menu[3].value3 = 1;

	fan1Menu[4].name = "Set start day";
	fan1Menu[4].maxVal = 0;  //maxVal - соответствует номеру вентилятора
	fan1Menu[4].function = FANS_UpdateFirstDay;
	fan1Menu[4].enter = &fan1Menu[4];
	fan1Menu[4].value3 = 1;

	fan1Menu[5].name = "ON/OFF";
	fan1Menu[5].enter = &fan1Menu[5];
	fan1Menu[5].function = FANS_Toggle;
	fan1Menu[5].value3 = 1;

	fan1Menu[6].name = "Save and Back";
	fan1Menu[6].function = FANS_FillProgram;
	fan1Menu[6].enter = &mainMenu[0];
	fan1Menu[6].value3 = 1;

	//Fan2 menu
	MENU_FillSection(fan2Menu, FAN_MENU_SIZE, "Fan2  ModeN DaysC");

	fan2Menu[0].name = "Stage1";
	fan2Menu[0].type = MENU_TYPE_TREE_COLUMN;
	fan2Menu[0].value = &ProgramForFun[1].modeNumber1;
	fan2Menu[0].maxVal = 3;
	fan2Menu[0].value2 = &ProgramForFun[1].numberOfDays1;
	fan2Menu[0].maxVal2 = 30;
	fan2Menu[0].value3 = 2;

	fan2Menu[1].name = "Stage2";
	fan2Menu[1].type = MENU_TYPE_TREE_COLUMN;
	fan2Menu[1].value = &ProgramForFun[1].modeNumber2;
	fan2Menu[1].maxVal = 3;
	fan2Menu[1].value2 = &ProgramForFun[1].numberOfDays2;
	fan2Menu[1].maxVal2 = 30;
	fan2Menu[1].value3 = 2;

	fan2Menu[2].name = "Stage3";
	fan2Menu[2].type = MENU_TYPE_TREE_COLUMN;
	fan2Menu[2].value = &ProgramForFun[1].modeNumber3;
	fan2Menu[2].maxVal = 3;
	fan2Menu[2].value2 = &ProgramForFun[1].numberOfDays3;
	fan2Menu[2].maxVal2 = 30;
	fan2Menu[2].value3 = 2;

	fan2Menu[3].name = "Stage4";
	fan2Menu[3].type = MENU_TYPE_TREE_COLUMN;
	fan2Menu[3].value = &ProgramForFun[1].modeNumber4;
	fan2Menu[3].maxVal = 3;
	fan2Menu[3].value2 = &ProgramForFun[1].numberOfDays4;
	fan2Menu[3].maxVal2 = 30;
	fan2Menu[3].value3 = 2;

	fan2Menu[4].name = "Set start day";
	fan2Menu[4].maxVal = 1;  //maxVal - соответствует номеру вентилятора
	fan2Menu[4].function = FANS_UpdateFirstDay;
	fan2Menu[4].enter = &fan2Menu[4];
	fan2Menu[4].value3 = 2;

	fan2Menu[5].name = "ON/OFF";
	fan2Menu[5].function = FANS_Toggle;
	fan2Menu[5].enter = &fan2Menu[5];
	fan2Menu[5].value3 = 2;

	fan2Menu[6].name = "Save and Back";
	fan2Menu[6].function = FANS_FillProgram;
	fan2Menu[6].enter = &mainMenu[1];
	fan2Menu[6].value3 = 2;

	//fan3 menu
	MENU_FillSection(fan3Menu, FAN_MENU_SIZE, "Fan3  ModeN DaysC");

	fan3Menu[0].name = "Stage1";
	fan3Menu[0].type = MENU_TYPE_TREE_COLUMN;
	fan3Menu[0].value = &ProgramForFun[2].modeNumber1;
	fan3Menu[0].maxVal = 3;
	fan3Menu[0].value2 = &ProgramForFun[2].numberOfDays1;
	fan3Menu[0].maxVal2 = 30;
	fan3Menu[0].value3 = 3;

	fan3Menu[1].name = "Stage2";
	fan3Menu[1].type = MENU_TYPE_TREE_COLUMN;
	fan3Menu[1].value = &ProgramForFun[2].modeNumber2;
	fan3Menu[1].maxVal = 3;
	fan3Menu[1].value2 = &ProgramForFun[2].numberOfDays2;
	fan3Menu[1].maxVal2 = 30;
	fan3Menu[1].value3 = 3;

	fan3Menu[2].name = "Stage3";
	fan3Menu[2].type = MENU_TYPE_TREE_COLUMN;
	fan3Menu[2].value = &ProgramForFun[2].modeNumber3;
	fan3Menu[2].maxVal = 3;
	fan3Menu[2].value2 = &ProgramForFun[2].numberOfDays3;
	fan3Menu[2].maxVal2 = 30;
	fan3Menu[2].value3 = 3;

	fan3Menu[3].name = "Stage4";
	fan3Menu[3].type = MENU_TYPE_TREE_COLUMN;
	fan3Menu[3].value = &ProgramForFun[2].modeNumber4;
	fan3Menu[3].maxVal = 3;
	fan3Menu[3].value2 = &ProgramForFun[2].numberOfDays4;
	fan3Menu[3].maxVal2 = 30;
	fan3Menu[3].value3 = 3;

	fan3Menu[4].name = "Set start day";
	fan3Menu[4].maxVal = 2;  //maxVal - соответствует номеру вентилятора
	fan3Menu[4].function = FANS_UpdateFirstDay;
	fan3Menu[4].enter = &fan3Menu[4];
	fan3Menu[4].value3 = 3;

	fan3Menu[5].name = "ON/OFF";
	fan3Menu[5].function = FANS_Toggle;
	fan3Menu[5].enter = &fan3Menu[5];
	fan3Menu[5].value3 = 3;

	fan3Menu[6].name = "Save and Back";
	fan3Menu[6].function = FANS_FillProgram;
	fan3Menu[6].enter = &mainMenu[2];
	fan3Menu[6].value3 = 3;

	//fan4 menu
	MENU_FillSection(fan4Menu, FAN_MENU_SIZE, "Fan4  ModeN DaysC");

	fan4Menu[0].name = "Stage1";
	fan4Menu[0].type = MENU_TYPE_TREE_COLUMN;
	fan4Menu[0].value = &ProgramForFun[3].modeNumber1;
	fan4Menu[0].maxVal = 3;
	fan4Menu[0].value2 = &ProgramForFun[3].numberOfDays1;
	fan4Menu[0].maxVal2 = 30;
	fan4Menu[0].value3 = 4;

	fan4Menu[1].name = "Stage2";
	fan4Menu[1].type = MENU_TYPE_TREE_COLUMN;
	fan4Menu[1].value = &ProgramForFun[3].modeNumber2;
	fan4Menu[1].maxVal = 3;
	fan4Menu[1].value2 = &ProgramForFun[3].numberOfDays2;
	fan4Menu[1].maxVal2 = 30;
	fan4Menu[1].value3 = 4;

	fan4Menu[2].name = "Stage3";
	fan4Menu[2].type = MENU_TYPE_TREE_COLUMN;
	fan4Menu[2].value = &ProgramForFun[3].modeNumber3;
	fan4Menu[2].maxVal = 3;
	fan4Menu[2].value2 = &ProgramForFun[3].numberOfDays3;
	fan4Menu[2].maxVal2 = 30;
	fan4Menu[2].value3 = 4;

	fan4Menu[3].name = "Stage4";
	fan4Menu[3].type = MENU_TYPE_TREE_COLUMN;
	fan4Menu[3].value = &ProgramForFun[3].modeNumber4;
	fan4Menu[3].maxVal = 3;
	fan4Menu[3].value2 = &ProgramForFun[3].numberOfDays4;
	fan4Menu[3].maxVal2 = 30;
	fan4Menu[3].value3 = 4;

	fan4Menu[4].name = "Set start day";
	fan4Menu[4].maxVal = 3;  //maxVal - соответствует номеру вентилятора
	fan4Menu[4].function = FANS_UpdateFirstDay;
	fan4Menu[4].enter = &fan4Menu[4];
	fan4Menu[4].value3 = 4;

	fan4Menu[5].name = "ON/OFF";
	fan4Menu[5].function = FANS_Toggle;
	fan4Menu[5].enter = &fan4Menu[5];
	fan4Menu[5].value3 = 4;

	fan4Menu[6].name = "Save and Back";
	fan4Menu[6].function = FANS_FillProgram;
	fan4Menu[6].enter = &mainMenu[3];
	fan4Menu[6].value3 = 4;

	//fan5 menu
	MENU_FillSection(fan5Menu, FAN_MENU_SIZE, "Fan5  ModeN DaysC");

	fan5Menu[0].name = "Stage1";
	fan5Menu[0].type = MENU_TYPE_TREE_COLUMN;
	fan5Menu[0].value = &ProgramForFun[4].modeNumber1;
	fan5Menu[0].maxVal = 3;
	fan5Menu[0].value2 = &ProgramForFun[4].numberOfDays1;
	fan5Menu[0].maxVal2 = 30;
	fan5Menu[0].value3 = 5;

	fan5Menu[1].name = "Stage2";
	fan5Menu[1].type = MENU_TYPE_TREE_COLUMN;
	fan5Menu[1].value = &ProgramForFun[4].modeNumber2;
	fan5Menu[1].maxVal = 3;
	fan5Menu[1].value2 = &ProgramForFun[4].numberOfDays2;
	fan5Menu[1].maxVal2 = 30;
	fan5Menu[1].value3 = 5;

	fan5Menu[2].name = "Stage3";
	fan5Menu[2].type = MENU_TYPE_TREE_COLUMN;
	fan5Menu[2].value = &ProgramForFun[4].modeNumber3;
	fan5Menu[2].maxVal = 3;
	fan5Menu[2].value2 = &ProgramForFun[4].numberOfDays3;
	fan5Menu[2].maxVal2 = 30;
	fan5Menu[2].value3 = 5;

	fan5Menu[3].name = "Stage4";
	fan5Menu[3].type = MENU_TYPE_TREE_COLUMN;
	fan5Menu[3].value = &ProgramForFun[4].modeNumber4;
	fan5Menu[3].maxVal = 3;
	fan5Menu[3].value2 = &ProgramForFun[4].numberOfDays4;
	fan5Menu[3].maxVal2 = 30;
	fan5Menu[3].value3 = 5;

	fan5Menu[4].name = "Set start day";
	fan5Menu[4].maxVal = 4;  //maxVal - соответствует номеру вентилятора
	fan5Menu[4].function = FANS_UpdateFirstDay;
	fan5Menu[4].enter = &fan5Menu[4];
	fan5Menu[4].value3 = 5;

	fan5Menu[5].name = "ON/OFF";
	fan5Menu[5].function = FANS_Toggle;
	fan5Menu[5].enter = &fan5Menu[5];
	fan5Menu[5].value3 = 5;

	fan5Menu[6].name = "Save and Back";
	fan5Menu[6].function = FANS_FillProgram;
	fan5Menu[6].enter = &mainMenu[4];
	fan5Menu[6].value3 = 5;

	//fan6 menu
	MENU_FillSection(fan6Menu, FAN_MENU_SIZE, "Fan6  ModeN DaysC");

	fan6Menu[0].name = "Stage1";
	fan6Menu[0].type = MENU_TYPE_TREE_COLUMN;
	fan6Menu[0].value = &ProgramForFun[5].modeNumber1;
	fan6Menu[0].maxVal = 3;
	fan6Menu[0].value2 = &ProgramForFun[5].numberOfDays1;
	fan6Menu[0].maxVal2 = 30;
	fan6Menu[0].value3 = 6;

	fan6Menu[1].name = "Stage2";
	fan6Menu[1].type = MENU_TYPE_TREE_COLUMN;
	fan6Menu[1].value = &ProgramForFun[5].modeNumber2;
	fan6Menu[1].maxVal = 3;
	fan6Menu[1].value2 = &ProgramForFun[5].numberOfDays2;
	fan6Menu[1].maxVal2 = 30;
	fan6Menu[1].value3 = 6;

	fan6Menu[2].name = "Stage3";
	fan6Menu[2].type = MENU_TYPE_TREE_COLUMN;
	fan6Menu[2].value = &ProgramForFun[5].modeNumber3;
	fan6Menu[2].maxVal = 3;
	fan6Menu[2].value2 = &ProgramForFun[5].numberOfDays3;
	fan6Menu[2].maxVal2 = 30;
	fan6Menu[2].value3 = 6;

	fan6Menu[3].name = "Stage4";
	fan6Menu[3].type = MENU_TYPE_TREE_COLUMN;
	fan6Menu[3].value = &ProgramForFun[5].modeNumber4;
	fan6Menu[3].maxVal = 3;
	fan6Menu[3].value2 = &ProgramForFun[5].numberOfDays4;
	fan6Menu[3].maxVal2 = 30;
	fan6Menu[3].value3 = 6;

	fan6Menu[4].name = "Set start day";
	fan6Menu[4].maxVal = 5;  //maxVal - соответствует номеру вентилятора
	fan6Menu[4].function = FANS_UpdateFirstDay;
	fan6Menu[4].enter = &fan6Menu[4];
	fan6Menu[4].value3 = 6;

	fan6Menu[5].name = "ON/OFF";
	fan6Menu[5].function = FANS_Toggle;
	fan6Menu[5].enter = &fan6Menu[5];
	fan6Menu[5].value3 = 6;

	fan6Menu[6].name = "Save and Back";
	fan6Menu[6].function = FANS_FillProgram;
	fan6Menu[6].enter = &mainMenu[5];
	fan6Menu[6].value3 = 6;

	//fan7 menu
	MENU_FillSection(fan7Menu, FAN_MENU_SIZE, "Fan7  ModeN DaysC");

	fan7Menu[0].name = "Stage1";
	fan7Menu[0].type = MENU_TYPE_TREE_COLUMN;
	fan7Menu[0].value = &ProgramForFun[6].modeNumber1;
	fan7Menu[0].maxVal = 3;
	fan7Menu[0].value2 = &ProgramForFun[6].numberOfDays1;
	fan7Menu[0].maxVal2 = 30;
	fan7Menu[0].value3 = 7;

	fan7Menu[1].name = "Stage2";
	fan7Menu[1].type = MENU_TYPE_TREE_COLUMN;
	fan7Menu[1].value = &ProgramForFun[6].modeNumber2;
	fan7Menu[1].maxVal = 3;
	fan7Menu[1].value2 = &ProgramForFun[6].numberOfDays2;
	fan7Menu[1].maxVal2 = 30;
	fan7Menu[1].value3 = 7;

	fan7Menu[2].name = "Stage3";
	fan7Menu[2].type = MENU_TYPE_TREE_COLUMN;
	fan7Menu[2].value = &ProgramForFun[6].modeNumber3;
	fan7Menu[2].maxVal = 3;
	fan7Menu[2].value2 = &ProgramForFun[6].numberOfDays3;
	fan7Menu[2].maxVal2 = 30;
	fan7Menu[2].value3 = 7;

	fan7Menu[3].name = "Stage4";
	fan7Menu[3].type = MENU_TYPE_TREE_COLUMN;
	fan7Menu[3].value = &ProgramForFun[6].modeNumber4;
	fan7Menu[3].maxVal = 3;
	fan7Menu[3].value2 = &ProgramForFun[6].numberOfDays4;
	fan7Menu[3].maxVal2 = 30;
	fan7Menu[3].value3 = 7;

	fan7Menu[4].name = "Set start day";
	fan7Menu[4].maxVal = 6;  //maxVal - соответствует номеру вентилятора
	fan7Menu[4].function = FANS_UpdateFirstDay;
	fan7Menu[4].enter = &fan7Menu[4];
	fan7Menu[4].value3 = 7;

	fan7Menu[5].name = "ON/OFF";
	fan7Menu[5].function = FANS_Toggle;
	fan7Menu[5].enter = &fan7Menu[5];
	fan7Menu[5].value3 = 7;

	fan7Menu[6].name = "Save and Back";
	fan7Menu[6].function = FANS_FillProgram;
	fan7Menu[6].enter = &mainMenu[6];
	fan7Menu[6].value3 = 7;

	//fan8 menu
	MENU_FillSection(fan8Menu, FAN_MENU_SIZE, "Fan8  ModeN DaysC");

	fan8Menu[0].name = "Stage1";
	fan8Menu[0].type = MENU_TYPE_TREE_COLUMN;
	fan8Menu[0].value = &ProgramForFun[7].modeNumber1;
	fan8Menu[0].maxVal = 3;
	fan8Menu[0].value2 = &ProgramForFun[7].numberOfDays1;
	fan8Menu[0].maxVal2 = 30;
	fan8Menu[0].value3 = 8;

	fan8Menu[1].name = "Stage2";
	fan8Menu[1].type = MENU_TYPE_TREE_COLUMN;
	fan8Menu[1].value = &ProgramForFun[7].modeNumber2;
	fan8Menu[1].maxVal = 3;
	fan8Menu[1].value2 = &ProgramForFun[7].numberOfDays2;
	fan8Menu[1].maxVal2 = 30;
	fan8Menu[1].value3 = 8;

	fan8Menu[2].name = "Stage3";
	fan8Menu[2].type = MENU_TYPE_TREE_COLUMN;
	fan8Menu[2].value = &ProgramForFun[7].modeNumber3;
	fan8Menu[2].maxVal = 3;
	fan8Menu[2].value2 = &ProgramForFun[7].numberOfDays3;
	fan8Menu[2].maxVal2 = 30;
	fan8Menu[2].value3 = 8;

	fan8Menu[3].name = "Stage4";
	fan8Menu[3].type = MENU_TYPE_TREE_COLUMN;
	fan8Menu[3].value = &ProgramForFun[7].modeNumber4;
	fan8Menu[3].maxVal = 3;
	fan8Menu[3].value2 = &ProgramForFun[7].numberOfDays4;
	fan8Menu[3].maxVal2 = 30;
	fan8Menu[3].value3 = 8;

	fan8Menu[4].name = "Set start day";
	fan8Menu[4].maxVal = 7;  //maxVal - соответствует номеру вентилятора
	fan8Menu[4].function = FANS_UpdateFirstDay;
	fan8Menu[4].enter = &fan8Menu[4];
	fan8Menu[4].value3 = 8;

	fan8Menu[5].name = "ON/OFF";
	fan8Menu[5].function = FANS_Toggle;
	fan8Menu[5].enter = &fan8Menu[5];
	fan8Menu[5].value3 = 8;

	fan8Menu[6].name = "Save and Back";
	fan8Menu[6].function = FANS_FillProgram;
	fan8Menu[6].enter = &mainMenu[7];
	fan8Menu[6].value3 = 8;

	//Setting
	MENU_FillSection(setting, SETTING_MENU_SIZE, "Setting");

	setting[0].name = "Time";
	setting[0].enter = &time[0];
	setting[0].function = DISPLAY_GoesToAndFillTime;

	setting[1].name = "Date";
	setting[1].enter = &date[0];
	setting[1].function = DISPLAY_GoesToAndFillDate;

	setting[2].name = "Start day time";
	setting[2].enter = &startDay[0];
	setting[2].function = DISPLAY_GoesToAndFillStartDay;

	setting[3].name = "End day time";
	setting[3].enter = &endDay[0];
	setting[3].function = DISPLAY_GoesToAndFillEndDay;

	setting[4].name = "Switching period";
	setting[4].function = DISPLAY_GoesTo;
	setting[4].enter = &periodT[0];

	setting[5].name = "Modes";
	setting[5].enter = &modeDay[0];
	setting[5].function = DISPLAY_GoesTo;

	setting[6].name = "Reset all data";
	setting[6].enter = &areYouSure[0];
	setting[6].function = DISPLAY_GoesTo;

	setting[7].name = "Back";
	setting[7].function = DISPLAY_GoesTo;
	setting[7].enter = &mainMenu[8];

	MENU_FillSection(areYouSure, CONFIRMATION_MENU_SIZE, "Are you sure?");

	areYouSure[0].name = "Yes and back";
	areYouSure[0].function = FANS_ResetSettings;
	areYouSure[0].enter = &setting[6];

	areYouSure[1].name = "No and back";
	areYouSure[1].function = DISPLAY_GoesTo;
	areYouSure[1].enter = &setting[6];

	areYouSure[2].name = "";
	areYouSure[2].function = DISPLAY_GoesTo;
	areYouSure[2].enter = &areYouSure[1];

	MENU_FillSection(periodT, PERIOD_MENU_SIZE, "Switching period"); //number of second to switch speed fan between lowDay and HighDay

	periodT[0].name = "Sec";
	periodT[0].value = &g_switchingPeriod;
	periodT[0].maxVal = 60;
	periodT[0].minVal = 1;
	periodT[0].type = MENU_TYPE_TWO_COLUMN;

	periodT[1].name = "Save";
	periodT[1].function = FANS_SetPeriod;
	periodT[1].enter = &periodT[2];

	periodT[2].name = "Back";
	periodT[2].function = DISPLAY_GoesTo;
	periodT[2].enter = &setting[4];

	MENU_FillSection(modeDay, MODE_DAY_MENU_SIZE, "Modes day");

	modeDay[0].name = "mode 0";
	modeDay[0].function = DISPLAY_GoesTo;
	modeDay[0].enter = &mode0[0];

	modeDay[1].name = "mode 1";
	modeDay[1].function = DISPLAY_GoesTo;
	modeDay[1].enter = &mode1[0];

	modeDay[2].name = "mode 2";
	modeDay[2].function = DISPLAY_GoesTo;
	modeDay[2].enter = &mode2[0];

	modeDay[3].name = "mode 3";
	modeDay[3].function = DISPLAY_GoesTo;
	modeDay[3].enter = &mode3[0];

	modeDay[4].name = "Back";
	modeDay[4].function = DISPLAY_GoesTo;
	modeDay[4].enter = &setting[5];

	MENU_FillSection(mode0, MODE_MENU_SIZE, "mode 0");

	mode0[0].type = MENU_TYPE_TWO_COLUMN;
	mode0[0].value = &dlRouteM[0].night;
	mode0[0].maxVal = 10;
	mode0[0].name = "Night";

	mode0[1].type = MENU_TYPE_TWO_COLUMN;
	mode0[1].value = &dlRouteM[0].dayHight;
	mode0[1].maxVal = 10;
	mode0[1].name = "Day H";

	mode0[2].type = MENU_TYPE_TWO_COLUMN;
	mode0[2].value = &dlRouteM[0].dayLow;
	mode0[2].maxVal = 10;
	mode0[2].name = "Day L";

	mode0[3].name = "Save and back";
	mode0[3].function = DISPLAY_GoesTo;
	mode0[3].enter = &modeDay[0];

	MENU_FillSection(mode1, MODE_MENU_SIZE, "mode 1");
	mode1[0].type = MENU_TYPE_TWO_COLUMN;
	mode1[0].value = &dlRouteM[1].night;
	mode1[0].maxVal = 10;
	mode1[0].name = "Night";

	mode1[1].type = MENU_TYPE_TWO_COLUMN;
	mode1[1].value = &dlRouteM[1].dayHight;
	mode1[1].maxVal = 10;
	mode1[1].name = "Day H";

	mode1[2].type = MENU_TYPE_TWO_COLUMN;
	mode1[2].value = &dlRouteM[1].dayLow;
	mode1[2].maxVal = 10;
	mode1[2].name = "Day L";

	mode1[3].name = "Save and back";
	mode1[3].function = DISPLAY_GoesTo;
	mode1[3].enter = &modeDay[1];

	MENU_FillSection(mode2, MODE_MENU_SIZE, "mode 2");

	mode2[0].type = MENU_TYPE_TWO_COLUMN;
	mode2[0].value = &dlRouteM[2].night;
	mode2[0].maxVal = 10;
	mode2[0].name = "Night";

	mode2[1].type = MENU_TYPE_TWO_COLUMN;
	mode2[1].value = &dlRouteM[2].dayHight;
	mode2[1].maxVal = 10;
	mode2[1].name = "Day H";

	mode2[2].type = MENU_TYPE_TWO_COLUMN;
	mode2[2].value = &dlRouteM[2].dayLow;
	mode2[2].maxVal = 10;
	mode2[2].name = "Day L";

	mode2[3].name = "Save and back";
	mode2[3].function = DISPLAY_GoesTo;
	mode2[3].enter = &modeDay[2];

	MENU_FillSection(mode3, MODE_MENU_SIZE, "mode 3");

	mode3[0].type = MENU_TYPE_TWO_COLUMN;
	mode3[0].value = &dlRouteM[3].night;
	mode3[0].maxVal = 10;
	mode3[0].name = "Night";

	mode3[1].type = MENU_TYPE_TWO_COLUMN;
	mode3[1].value = &dlRouteM[3].dayHight;
	mode3[1].maxVal = 10;
	mode3[1].name = "Day H";

	mode3[2].type = MENU_TYPE_TWO_COLUMN;
	mode3[2].value = &dlRouteM[3].dayLow;
	mode3[2].maxVal = 10;
	mode3[2].name = "Day L";

	mode3[3].name = "Save and back";
	mode3[3].value = NULL;
	mode3[3].function = DISPLAY_GoesTo;
	mode3[3].enter = &modeDay[3];

	MENU_FillSection(time, TIME_MENU_SIZE, "Time");

	time[0].name = "Hours";
	time[0].type = MENU_TYPE_TWO_COLUMN;
	time[0].value = &userTime.Hours;
	time[0].maxVal = 23;

	time[1].name = "Minutes";
	time[1].type = MENU_TYPE_TWO_COLUMN;
	time[1].value = &userTime.Minutes;
	time[1].maxVal = 59;

	time[2].name = "Seconds";
	time[2].type = MENU_TYPE_TWO_COLUMN;
	time[2].value = &userTime.Seconds;
	time[2].maxVal = 59;

	time[3].name = "Save";
	time[3].function = FANS_SetTime;
	time[3].enter = &time[4];

	time[4].name = "Back";
	time[4].function = DISPLAY_GoesToAndNotShowTime;
	time[4].enter = &setting[0];

	MENU_FillSection(date, DATE_MENU_SIZE, "Date");

	date[0].name = "Year";
	date[0].value = &userDate.Year;
	date[0].maxVal = 99;
	date[0].type = MENU_TYPE_TWO_COLUMN;

	date[1].name = "Month";
	date[1].value = &userDate.Month;
	date[1].maxVal = 12;
	date[1].minVal = 1;
	date[1].type = MENU_TYPE_TWO_COLUMN;

	date[2].name = "Day";
	date[2].value = &userDate.Date;
	date[2].maxVal = 31;
	date[2].minVal = 1;
	date[2].type = MENU_TYPE_TWO_COLUMN;

	date[3].name = "Save";
	date[3].function = FANS_SetDate;
	date[3].enter = &date[4];

	date[4].name = "Back";
	date[4].function = DISPLAY_GoesToAndNotShowTime;
	date[4].enter = &setting[1];

	MENU_FillSection(startDay, TIME_MENU_SIZE, "Start day time");

	startDay[0].name = "Hours";
	startDay[0].value = &userTime.Hours;
	startDay[0].maxVal = 23;
	startDay[0].type = MENU_TYPE_TWO_COLUMN;

	startDay[1].name = "Minutes";
	startDay[1].value = &userTime.Minutes;
	startDay[1].maxVal = 59;
	startDay[1].type = MENU_TYPE_TWO_COLUMN;

	startDay[2].name = "Seconds";
	startDay[2].value = &userTime.Seconds;
	startDay[2].maxVal = 59;
	startDay[2].type = MENU_TYPE_TWO_COLUMN;

	startDay[3].name = "Save";
	startDay[3].function = FANS_SetStartDayTime;
	startDay[3].enter = &startDay[4];

	startDay[4].name = "Back";
	startDay[4].function = DISPLAY_GoesTo;
	startDay[4].enter = &setting[2];

	MENU_FillSection(endDay, TIME_MENU_SIZE, "End day time");

	endDay[0].name = "Hours";
	endDay[0].value = &userTime.Hours;
	endDay[0].maxVal = 23;
	endDay[0].type = MENU_TYPE_TWO_COLUMN;
	endDay[0].function = DISPLAY_GoesTo;

	endDay[1].name = "Minutes";
	endDay[1].value = &userTime.Minutes;
	endDay[1].maxVal = 59;
	endDay[1].type = MENU_TYPE_TWO_COLUMN;
	endDay[1].function = DISPLAY_GoesTo;

	endDay[2].name = "Seconds";
	endDay[2].value = &userTime.Seconds;
	endDay[2].maxVal = 59;
	endDay[2].type = MENU_TYPE_TWO_COLUMN;
	endDay[2].function = DISPLAY_GoesTo;

	endDay[3].name = "Save";
	endDay[3].function = FANS_SetEndDayTime;
	endDay[3].enter = &endDay[4];

	endDay[4].name = "Back";
	endDay[4].function = DISPLAY_GoesTo;
	endDay[4].enter = &setting[3];
}
//
//-------------------------------------------------------------
//
//Automatic fill: id, sectionName, up, down
static void MENU_FillSection(MenuItemStruct *section, uint8_t numItemInSection, char *sectionName) {

	for (int i = 0; i < numItemInSection; i++) {
		section[i].id = i;

		section[i].sectionName = sectionName;
		if (i == (numItemInSection - 1)) {
			section[i].down = NULL;
		} else {
			section[i].down = &section[i + 1];
		}

		if (i == 0) {
			section[i].up = NULL;
		} else {
			section[i].up = &section[i - 1];
		}
	}
}
