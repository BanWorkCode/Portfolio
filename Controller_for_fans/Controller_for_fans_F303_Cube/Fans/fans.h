#ifndef fans_H
#define fans_H
//
//-------------------------------------------------------------
//
//
#include <main.h>
#include "fillMenu.h"
#include "storingSettings.h"
//
//-------------------------------------------------------------
//
//
#define NUM_EL_OF_ARR_DAYS 120U
#define NUMBER_OF_FANS 8U
//
//-------------------------------------------------------------
//
//
typedef enum{
	FANS_TIME_NIGHT = 0U,
	FANS_TIME_DAY =1U
} FANS_TIME_OF_DAY;


typedef struct {
uint8_t fanNumber;
uint8_t fanOn;
uint8_t currentDay;	
uint16_t firstDay;
uint8_t arrDays[NUM_EL_OF_ARR_DAYS]; //для каждого дня записано значение Режима(mode), одно из четырех
	
uint8_t modeNumber1;
uint8_t numberOfDays1;

uint8_t modeNumber2;
uint8_t numberOfDays2;

uint8_t modeNumber3;
uint8_t numberOfDays3;

uint8_t modeNumber4;
uint8_t numberOfDays4;



} FANS_Programm_t;

typedef const struct {
	GPIO_TypeDef* port;
	uint16_t pin;
	__IO uint32_t* speed;
}FANS_Driver_t;

//saves variable of speed fan for everyone mode
typedef struct{
uint8_t night;
uint8_t dayHight;
uint8_t dayLow;	
} FANS_DailyRoutine_t;
//
//-------------------------------------------------------------
//
//
extern FANS_Programm_t ProgramForFun[8];
extern _Bool  g_flagNewDay;
extern _Bool g_operatingMode;
extern uint16_t fullDate;
extern FANS_DailyRoutine_t dlRouteM[4]; 
extern uint8_t g_switchingPeriod;
extern FANS_TIME_OF_DAY g_timeOfDay;
extern FANS_Driver_t fanDriver[NUMBER_OF_FANS];
//
//-------------------------------------------------------------
//
//
void FANS_UpdateCyrrentDays ( void );
MenuItemStruct* FANS_UpdateFirstDay ( MenuItemStruct* current );
MenuItemStruct* FANS_FillProgram ( MenuItemStruct* current );
MenuItemStruct* FANS_Toggle ( MenuItemStruct* current );
MenuItemStruct* FANS_ResetSettings ( MenuItemStruct* current );
MenuItemStruct* FANS_SetTime ( struct MenuItemStruct* current );
MenuItemStruct* FANS_SetDate ( struct MenuItemStruct* current );
MenuItemStruct* FANS_SetStartDayTime ( struct MenuItemStruct* current );
MenuItemStruct* FANS_SetEndDayTime ( struct MenuItemStruct* current );
MenuItemStruct* FANS_SetPeriod ( struct MenuItemStruct* current );
void FANS_CalculationDate ( void );
void FANS_SetSpeed (void);
//
//-------------------------------------------------------------
//
//
#endif
