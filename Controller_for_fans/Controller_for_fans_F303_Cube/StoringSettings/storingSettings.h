#ifndef storingSetting_H
#define storingSetting_H
//
//-------------------------------------------------------------
//
//
#include <main.h>
//
//-------------------------------------------------------------
//
//
#define FLASH_ADRESS ((uint32_t)	0x0803F000)
#define ROM_MEM_ARR_SIZE 55U
//
//-------------------------------------------------------------
//
//
typedef struct 
{
  uint8_t DONHours; 
  uint8_t DONMinutes;
  uint8_t DONSeconds;
	uint8_t DOFFHours;
  uint8_t DOFFMinutes;
  uint8_t DOFFSeconds;
	uint8_t switchingPeriod;
  uint8_t	Mode0H;
	uint8_t	Mode0L;
	uint8_t	Mode0N;
	uint8_t	Mode1H;
	uint8_t	Mode1L;
	uint8_t	Mode1N;
	uint8_t	Mode2H;
	uint8_t	Mode2L;
	uint8_t	Mode2N;
	uint8_t	Mode3H;
	uint8_t	Mode3L;
	uint8_t	Mode3N;
	
	uint16_t firstDay[8];
		
	uint8_t fanOn[8];
	
	uint8_t modeNumber1[8];
	uint8_t modeNumber2[8];
	uint8_t modeNumber3[8];
	uint8_t modeNumber4[8];
	
	uint8_t numberOfDays1[8];
	uint8_t numberOfDays2[8];
	uint8_t numberOfDays3[8];
	uint8_t numberOfDays4[8];
	
	uint8_t	checkNewFirmware;	
} romMemoryStruct;  // to Flash

union RomMemUnion{   // union для связи Flash и оперативки
romMemoryStruct romMemStr;
uint16_t arrRomMem[ROM_MEM_ARR_SIZE];
};
//
//-------------------------------------------------------------
//
//
void STORING_CheckFirstStart(void);
void STORING_SaveSetting (void);
void STORING_ReadSetting (void);
//
//-------------------------------------------------------------
//
//
extern union RomMemUnion romMemUn;
extern uint16_t arrRomMem[ROM_MEM_ARR_SIZE];

#endif
