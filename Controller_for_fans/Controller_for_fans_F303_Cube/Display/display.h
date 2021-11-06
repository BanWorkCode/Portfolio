#ifndef display_H
#define display_H
#include "fillMenu.h"
#include "ssd1306.h"
//
//-------------------------------------------------------------
//
//
#define NUMBER_VISIBLE_ROWS 3
#define ROW_1 16U
#define ROW_2 28U
#define ROW_3 40U
#define BOTTOM_ROW 53U
#define NEXT_ROW 12U
#define COLUMN_1 9U
#define COLUMN_2 72U
#define COLUMN_3 105U

#define DISPLAY_DOWNTIME 60000U
//
//-------------------------------------------------------------
//
//
void DISPLAY_ShowMenu(void);
MenuItemStruct* DISPLAY_GoesTo(MenuItemStruct *current);
MenuItemStruct* DISPLAY_GoesToAndShowInfo(MenuItemStruct *current);
MenuItemStruct* DISPLAY_GoesToAndNotShowTime(MenuItemStruct *current);
MenuItemStruct* DISPLAY_GoesToAndFillTime(MenuItemStruct *current);
MenuItemStruct* DISPLAY_GoesToAndFillDate(MenuItemStruct *current);
MenuItemStruct* DISPLAY_GoesToAndFillStartDay(MenuItemStruct *current);
MenuItemStruct* DISPLAY_GoesToAndFillEndDay(MenuItemStruct *current);
//
//-------------------------------------------------------------
//
//
#endif
