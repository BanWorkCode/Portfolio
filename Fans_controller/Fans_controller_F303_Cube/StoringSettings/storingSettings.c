#include "storingSettings.h"
#include "fans.h"
//
//-------------------------------------------------------------
//
//
union RomMemUnion romMemUn;

static FLASH_EraseInitTypeDef EraseInitStruct = { //структура для стирания FLASH
		.TypeErase = FLASH_TYPEERASE_PAGES,
		.PageAddress = FLASH_ADRESS,
				.NbPages = 1 };
//
//-------------------------------------------------------------
//
//
void STORING_CheckFirstStart(void) { //проверяем, что прошивка новая
//if (1){  //что бы принудительно обнулить
	if (romMemUn.romMemStr.checkNewFirmware == 0xFF) { //если новая, заполняем нулями

		dayOn.Hours = 0;
		dayOn.Minutes = 0;
		dayOn.Seconds = 0;
		dayOn.FullTime = 0;

		dayOff.Hours = 0;
		dayOff.Minutes = 0;
		dayOff.Seconds = 0;

		dayOff.FullTime = 0;

		g_switchingPeriod = 0;

		TIM4->ARR = 0;

		dlRouteM[0].dayHight = 0;
		dlRouteM[0].dayLow = 0;
		dlRouteM[0].night = 0;

		dlRouteM[1].dayHight = 0;
		dlRouteM[1].dayLow = 0;
		dlRouteM[1].night = 0;

		dlRouteM[2].dayHight = 0;
		dlRouteM[2].dayLow = 0;
		dlRouteM[2].night = 0;

		dlRouteM[3].dayHight = 0;
		dlRouteM[3].dayLow = 0;
		dlRouteM[3].night = 0;

		for (int i = 0; i < 8; i++) { // данные о количестве дней и режимов
			ProgramForFun[i].firstDay = fullDate;
			ProgramForFun[i].fanOn = 0;

			ProgramForFun[i].modeNumber1 = 0;
			ProgramForFun[i].modeNumber2 = 0;
			ProgramForFun[i].modeNumber3 = 0;
			ProgramForFun[i].modeNumber4 = 0;

			ProgramForFun[i].numberOfDays1 = 0;
			ProgramForFun[i].numberOfDays2 = 0;
			ProgramForFun[i].numberOfDays3 = 0;
			ProgramForFun[i].numberOfDays4 = 0;

		}

		romMemUn.romMemStr.checkNewFirmware = 0; //указывает что прошивка больше не новая

	} else {

// если прошивка не новая, заполняем рабочие данные из структуры
		dayOn.Hours = romMemUn.romMemStr.DONHours;
		dayOn.Minutes = romMemUn.romMemStr.DONMinutes;
		dayOn.Seconds = romMemUn.romMemStr.DONSeconds;

		dayOn.FullTime =
				(uint32_t) (((uint32_t) dayOn.Hours * 3600U)
						+ ((uint32_t) dayOn.Minutes * 60U)
						+ ((uint32_t) dayOn.Seconds));

		dayOff.Hours = romMemUn.romMemStr.DOFFHours;
		dayOff.Minutes = romMemUn.romMemStr.DOFFMinutes;
		dayOff.Seconds = romMemUn.romMemStr.DOFFSeconds;

		dayOff.FullTime = (uint32_t) (((uint32_t) dayOff.Hours * 3600U)
				+ ((uint32_t) dayOff.Minutes * 60U)
				+ ((uint32_t) dayOff.Seconds));

		g_switchingPeriod = romMemUn.romMemStr.switchingPeriod;

		TIM4->ARR = (uint32_t) (g_switchingPeriod * 1000 - 1);

		dlRouteM[0].dayHight = romMemUn.romMemStr.Mode0H;
		dlRouteM[0].dayLow = romMemUn.romMemStr.Mode0L;
		dlRouteM[0].night = romMemUn.romMemStr.Mode0N;

		dlRouteM[1].dayHight = romMemUn.romMemStr.Mode1H;
		dlRouteM[1].dayLow = romMemUn.romMemStr.Mode1L;
		dlRouteM[1].night = romMemUn.romMemStr.Mode1N;

		dlRouteM[2].dayHight = romMemUn.romMemStr.Mode2H;
		dlRouteM[2].dayLow = romMemUn.romMemStr.Mode2L;
		dlRouteM[2].night = romMemUn.romMemStr.Mode2N;

		dlRouteM[3].dayHight = romMemUn.romMemStr.Mode3H;
		dlRouteM[3].dayLow = romMemUn.romMemStr.Mode3L;
		dlRouteM[3].night = romMemUn.romMemStr.Mode3N;

		for (int i = 0; i < 8; i++) { // данные о количестве дней и режимов
			ProgramForFun[i].firstDay = romMemUn.romMemStr.firstDay[i];
			ProgramForFun[i].fanOn = romMemUn.romMemStr.fanOn[i];

			ProgramForFun[i].modeNumber1 = romMemUn.romMemStr.modeNumber1[i];
			ProgramForFun[i].modeNumber2 = romMemUn.romMemStr.modeNumber2[i];
			ProgramForFun[i].modeNumber3 = romMemUn.romMemStr.modeNumber3[i];
			ProgramForFun[i].modeNumber4 = romMemUn.romMemStr.modeNumber4[i];

			ProgramForFun[i].numberOfDays1 =
					romMemUn.romMemStr.numberOfDays1[i];
			ProgramForFun[i].numberOfDays2 =
					romMemUn.romMemStr.numberOfDays2[i];
			ProgramForFun[i].numberOfDays3 =
					romMemUn.romMemStr.numberOfDays3[i];
			ProgramForFun[i].numberOfDays4 =
					romMemUn.romMemStr.numberOfDays4[i];

		}

// END заполняем рабочие данные из структуры
	}

}
//
//-------------------------------------------------------------
//
// save all data
void STORING_SaveSetting(void) { //	переносим текущие значения в структуру хранения

	romMemUn.romMemStr.DONHours = dayOn.Hours;
	romMemUn.romMemStr.DONMinutes = dayOn.Minutes;
	romMemUn.romMemStr.DONSeconds = dayOn.Seconds;

	romMemUn.romMemStr.DOFFHours = dayOff.Hours;
	romMemUn.romMemStr.DOFFMinutes = dayOff.Minutes;
	romMemUn.romMemStr.DOFFSeconds = dayOff.Seconds;

	romMemUn.romMemStr.switchingPeriod = g_switchingPeriod;

	romMemUn.romMemStr.Mode0H = dlRouteM[0].dayHight;
	romMemUn.romMemStr.Mode0L = dlRouteM[0].dayLow;
	romMemUn.romMemStr.Mode0N = dlRouteM[0].night;

	romMemUn.romMemStr.Mode1H = dlRouteM[1].dayHight;
	romMemUn.romMemStr.Mode1L = dlRouteM[1].dayLow;
	romMemUn.romMemStr.Mode1N = dlRouteM[1].night;

	romMemUn.romMemStr.Mode2H = dlRouteM[2].dayHight;
	romMemUn.romMemStr.Mode2L = dlRouteM[2].dayLow;
	romMemUn.romMemStr.Mode2N = dlRouteM[2].night;

	romMemUn.romMemStr.Mode3H = dlRouteM[3].dayHight;
	romMemUn.romMemStr.Mode3L = dlRouteM[3].dayLow;
	romMemUn.romMemStr.Mode3N = dlRouteM[3].night;

	for (int i = 0; i < 8; i++) { // данных о количестве дней и режимов
		romMemUn.romMemStr.firstDay[i] = ProgramForFun[i].firstDay;

		romMemUn.romMemStr.fanOn[i] = ProgramForFun[i].fanOn;

//1-4 это номера частей цикла работы вентилятора		
		romMemUn.romMemStr.modeNumber1[i] = ProgramForFun[i].modeNumber1;
		romMemUn.romMemStr.modeNumber2[i] = ProgramForFun[i].modeNumber2;
		romMemUn.romMemStr.modeNumber3[i] = ProgramForFun[i].modeNumber3;
		romMemUn.romMemStr.modeNumber4[i] = ProgramForFun[i].modeNumber4;

		romMemUn.romMemStr.numberOfDays1[i] = ProgramForFun[i].numberOfDays1;
		romMemUn.romMemStr.numberOfDays2[i] = ProgramForFun[i].numberOfDays2;
		romMemUn.romMemStr.numberOfDays3[i] = ProgramForFun[i].numberOfDays3;
		romMemUn.romMemStr.numberOfDays4[i] = ProgramForFun[i].numberOfDays4;
	}

	HAL_FLASH_Unlock();

	uint32_t SectorError = 0; // FLASH
	HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError); // стираем страницу Flash начиначиная с адреса

	uint32_t adress = EraseInitStruct.PageAddress; //адрес во Flash начала хранения
	uint8_t num = 0;

	while (num < ROM_MEM_ARR_SIZE) {
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, adress,
				romMemUn.arrRomMem[num]);

		adress = adress + 2;//сдвигаем адрес на 2 байта, потому что программируем Flash полусловами по 16 bit
		num++;
	}

	HAL_FLASH_Lock();

}
//
//-------------------------------------------------------------
//
// 
void STORING_ReadSetting(void) {
// заполняем  структуру в оперативке(через Union) из Flash
	uint32_t adressInFlash = EraseInitStruct.PageAddress; // адрес начала хранения во Flash
	uint8_t numberOfElement = 0;

	while (numberOfElement < ROM_MEM_ARR_SIZE) {

		romMemUn.arrRomMem[numberOfElement] = *(uint16_t*) (adressInFlash);

		numberOfElement++;
		adressInFlash = adressInFlash + 2; // //сдвигаем адрес на 2 байта, потому что программируем Flash полусловами по 16 bit	
	}
}
