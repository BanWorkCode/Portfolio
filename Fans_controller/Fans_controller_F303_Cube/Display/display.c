#include "display.h"
#include "stdio.h"
#include "fans.h"
//
//-------------------------------------------------------------
//
//
static _Bool showTime = 0; //flag for show time
//
//-------------------------------------------------------------
//
//
static void DISPLAY_ShowTopRow(void);
static void DISPLAY_FillInverseColor(void);
static void DISPLAY_ShowBottomRow(void);
static void DISPLAY_ShowBody(void);
//
//-------------------------------------------------------------
//
//
static void DISPLAY_ShowTopRow(void) {

	ssd1306_SetCursor(1, 1);
	ssd1306_WriteString(curMainMenu->sectionName, Font_7x10, White);

	if (showTime) {

		char temp[16];
		sprintf(temp, "%02d", currentTime.Hours);
		ssd1306_SetCursor(33, 1);
		ssd1306_WriteString(temp, Font_7x10, White);
		ssd1306_SetCursor(48, 1);
		ssd1306_WriteString(":", Font_7x10, White);
		sprintf(temp, "%02d", currentTime.Minutes);
		ssd1306_SetCursor(55, 1);
		ssd1306_WriteString(temp, Font_7x10, White);
		ssd1306_SetCursor(69, 1);
		ssd1306_WriteString(":", Font_7x10, White);
		sprintf(temp, "%02d", currentTime.Seconds);
		ssd1306_SetCursor(76, 1);
		ssd1306_WriteString(temp, Font_7x10, White);
	}

}
//
//-------------------------------------------------------------
//
//
static void DISPLAY_FillInverseColor(void) {
	for (int j = 51; j < 64; j++) {
		for (int i = 0; i < 128; i++) {
			ssd1306_DrawPixel(i, j, White);
		}
	}

}
//
//-------------------------------------------------------------
//
//
static void DISPLAY_ShowBottomRow(void) {
	if (curMainMenu->value3 > 0) {

		DISPLAY_FillInverseColor();

		char temp[16];
		//вывод информации о режимах работы
		if (curMainMenu->cursor == MENU_CURSOR_SECOND_COLUMN) {
			ssd1306_SetCursor(1, BOTTOM_ROW);
			ssd1306_WriteString("Md", Font_7x10, Black);
			sprintf(temp, "%01d", *(curMainMenu->value));
			ssd1306_SetCursor(15, BOTTOM_ROW);
			ssd1306_WriteString(temp, Font_7x10, Black);
			ssd1306_SetCursor(22, BOTTOM_ROW);
			ssd1306_WriteString(":", Font_7x10, Black);
			ssd1306_SetCursor(36, BOTTOM_ROW);
			ssd1306_WriteString("N", Font_7x10, Black);
			sprintf(temp, "%d", dlRouteM[*(curMainMenu->value)].night);
			ssd1306_SetCursor(43, BOTTOM_ROW);
			ssd1306_WriteString(temp, Font_7x10, Black);
			ssd1306_SetCursor(64, BOTTOM_ROW);
			ssd1306_WriteString("DH", Font_7x10, Black);
			sprintf(temp, "%d", dlRouteM[*(curMainMenu->value)].dayHight);
			ssd1306_SetCursor(78, BOTTOM_ROW);
			ssd1306_WriteString(temp, Font_7x10, Black);
			ssd1306_SetCursor(99, BOTTOM_ROW);
			ssd1306_WriteString("DL", Font_7x10, Black);
			sprintf(temp, "%d", dlRouteM[*(curMainMenu->value)].dayLow);
			ssd1306_SetCursor(113, BOTTOM_ROW);
			ssd1306_WriteString(temp, Font_7x10, Black);
		}
		//вывод информации о текущем дне и состоянии вентилятора
		else {
			uint8_t coordinateX;
			ssd1306_SetCursor(1, BOTTOM_ROW);
			ssd1306_WriteString("Day:", Font_7x10, Black);
			sprintf(temp, "%d", (ProgramForFun[curMainMenu->value3 - 1].currentDay + 1));
			ssd1306_SetCursor(30, BOTTOM_ROW);
			ssd1306_WriteString(temp, Font_7x10, Black);
			if (ProgramForFun[curMainMenu->value3 - 1].currentDay + 1 > 99) {
				coordinateX = 14;
			} else if (ProgramForFun[curMainMenu->value3 - 1].currentDay + 1 < 100 && ProgramForFun[curMainMenu->value3 - 1].currentDay + 1 > 9) {
				coordinateX = 7;
			} else if (ProgramForFun[curMainMenu->value3 - 1].currentDay + 1 < 10) {
				coordinateX = 0;
			}
			ssd1306_SetCursor(37 + coordinateX, BOTTOM_ROW);
			ssd1306_WriteString("/", Font_7x10, Black);
			uint8_t sumdays = ProgramForFun[curMainMenu->value3 - 1].numberOfDays1 + ProgramForFun[curMainMenu->value3 - 1].numberOfDays2 + ProgramForFun[curMainMenu->value3 - 1].numberOfDays3
					+ ProgramForFun[curMainMenu->value3 - 1].numberOfDays4;
			sprintf(temp, "%d", sumdays);
			ssd1306_SetCursor(44 + coordinateX, BOTTOM_ROW);
			ssd1306_WriteString(temp, Font_7x10, Black);
			ssd1306_SetCursor(92, BOTTOM_ROW);
			if (ProgramForFun[curMainMenu->value3 - 1].fanOn) {
				ssd1306_WriteString("ON", Font_7x10, Black);
			} else {
				ssd1306_WriteString("OFF", Font_7x10, Black);
			}
		}
	}
}
//
//-------------------------------------------------------------
//
//
static void DISPLAY_ShowBody(void) {

	MenuItemStruct *forCounting = curMainMenu;
	MenuItemStruct *menuItemstr[NUMBER_VISIBLE_ROWS];

	int numberItemsInSection = 0; // количество элементов в разделе.
	while (forCounting != NULL)	// определяем id последненго пункта в разделе
	{
		numberItemsInSection = forCounting->id;
		forCounting = forCounting->down;
	}

	int8_t firstItem = 0;
	uint8_t cursorRow = ROW_1;
	char temp[16];

	if (curMainMenu->id == 0) // если первый элемент, то указатель ">" на первой строке
			{
		firstItem = 0;
		cursorRow = ROW_1;

	} else if (curMainMenu->id > 0 && curMainMenu->id < (numberItemsInSection)) // если id>0 но не последний, то указатель на второй строке
			{
		firstItem = -1;
		cursorRow = ROW_2;
	} else if (curMainMenu->id == numberItemsInSection) //если id последний, то указатель на третьей строке
			{
		firstItem = -2;
		cursorRow = ROW_3;
	}

	if (curMainMenu->cursor == MENU_CURSOR_FIRST_COLUMN) {
		ssd1306_SetCursor( COLUMN_1 - 8, cursorRow);
	} else if (curMainMenu->cursor == MENU_CURSOR_SECOND_COLUMN) {
		ssd1306_SetCursor( COLUMN_2 - 8, cursorRow);

	} else if (curMainMenu->cursor == MENU_CURSOR_THIRD_COLUMN) {
		ssd1306_SetCursor( COLUMN_3 - 8, cursorRow);

	}

	ssd1306_WriteString(">", Font_7x10, White);

	for (uint8_t i = 0, row = ROW_1; i < NUMBER_VISIBLE_ROWS; firstItem++, i++, row += NEXT_ROW) {
		menuItemstr[i] = curMainMenu + firstItem;

		ssd1306_SetCursor( COLUMN_1, row);
		ssd1306_WriteString((menuItemstr[i]->name), Font_7x10, White);

		if (menuItemstr[i]->type > MENU_TYPE_ONE_COLUMN) {
			sprintf(temp, "%d", *(menuItemstr[i]->value));
			ssd1306_SetCursor( COLUMN_2, row);
			ssd1306_WriteString(temp, Font_7x10, White);

		}
		if (menuItemstr[i]->type == MENU_TYPE_TREE_COLUMN) {
			sprintf(temp, "%d", *(menuItemstr[i]->value2));
			ssd1306_SetCursor( COLUMN_3, row);
			ssd1306_WriteString(temp, Font_7x10, White);
		}

	}

}
//
//-------------------------------------------------------------
//
//
void DISPLAY_ShowMenu(void) {
	ssd1306_Fill(Black);
	DISPLAY_ShowTopRow();
	DISPLAY_ShowBody();
	DISPLAY_ShowBottomRow();
	ssd1306_UpdateScreen();
}
//
//-------------------------------------------------------------
//
//переход в указанный пункт меню
MenuItemStruct* DISPLAY_GoesTo(MenuItemStruct *current) {
	return current;
}
//
//-------------------------------------------------------------
//
//Переход в меню, где надо отображать время
MenuItemStruct* DISPLAY_GoesToAndShowInfo(MenuItemStruct *current) {
	//showInfo = 1; // отображение времени в меню
	return current;
}
//
//-------------------------------------------------------------
//
//Переход в меню, и отключение отображения времени
MenuItemStruct* DISPLAY_GoesToAndNotShowTime(MenuItemStruct *current) {
	showTime = 0; // отображение времени в меню выкл
	return current;
}
//
//-------------------------------------------------------------
//
//
MenuItemStruct* DISPLAY_GoesToAndFillTime(MenuItemStruct *current) {
	userTime.Hours = currentTime.Hours;
	userTime.Minutes = currentTime.Minutes;
	userTime.Seconds = currentTime.Seconds;
	showTime = 1; // отображение времени в меню
	return current;
}
//
//-------------------------------------------------------------
//
//
MenuItemStruct* DISPLAY_GoesToAndFillDate(MenuItemStruct *current) {
	userDate.Year = currentDate.Year;
	userDate.Month = currentDate.Month;
	userDate.Date = currentDate.Date;
	return current;
}
//
//-------------------------------------------------------------
//
//Переход в меню установки начала дня и получение текущего времени
MenuItemStruct* DISPLAY_GoesToAndFillStartDay(MenuItemStruct *current) {
	userTime.Hours = dayOn.Hours;
	userTime.Minutes = dayOn.Minutes;
	userTime.Seconds = dayOn.Seconds;
	return current;
}
//
//-------------------------------------------------------------
//
//Переход в меню установки конца дня и получение текущего времени
MenuItemStruct* DISPLAY_GoesToAndFillEndDay(MenuItemStruct *current) {
	userTime.Hours = dayOff.Hours;
	userTime.Minutes = dayOff.Minutes;
	userTime.Seconds = dayOff.Seconds;
	return current;
}
