#include "fans.h"
#include "rtc.h"
#include "main.h"
//
//-------------------------------------------------------------
//
//
uint8_t const months[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
uint8_t const leapYears[25] = { 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96 };
FANS_Programm_t ProgramForFun[NUMBER_OF_FANS];
_Bool g_flagNewDay = 0; // флаг даты
_Bool g_operatingMode = 0; //define DayHigh or DayLow
uint16_t fullDate = 0;
FANS_Driver_t fanDriver[NUMBER_OF_FANS] = { { .port = GPIOA, .pin = GPIO_PIN_2, .speed = &TIM3->CCR1 }, { .port = GPIOA, .pin = GPIO_PIN_3, .speed = &TIM3->CCR2 }, { .port = GPIOA, .pin = GPIO_PIN_4,
		.speed = &TIM3->CCR3 }, { .port = GPIOA, .pin = GPIO_PIN_5, .speed = &TIM3->CCR4 }, { .port = GPIOB, .pin = GPIO_PIN_12, .speed = &TIM1->CCR1 }, { .port = GPIOB, .pin = GPIO_PIN_13, .speed =
		&TIM1->CCR2 }, { .port = GPIOB, .pin = GPIO_PIN_14, .speed = &TIM1->CCR3 }, { .port = GPIOB, .pin = GPIO_PIN_15, .speed = &TIM1->CCR4 } };

FANS_DailyRoutine_t dlRouteM[4];  // 4 стадии работы вентилятора, в течении всего программируемого периода

uint8_t g_switchingPeriod = 0; // number of the second to switch speed fan between LowDay and HighDay
//
//-------------------------------------------------------------
//
//Каждый вентилятор выполняет свою программу, которая длится несколько дней
//Эта функция после включения контроллера, определяет текущий день работы для каждого вентилятора
void FANS_UpdateCyrrentDays(void) {
	for (uint8_t i = 0; i < NUMBER_OF_FANS; i++) {
		uint8_t sumDays = ProgramForFun[i].numberOfDays1 + ProgramForFun[i].numberOfDays2 + ProgramForFun[i].numberOfDays3 + ProgramForFun[i].numberOfDays4;
		ProgramForFun[i].currentDay = fullDate - ProgramForFun[i].firstDay;
		//если текущий день получается больше максимального количества дней цикла работы, то приравниваем к последнему дню
		if (ProgramForFun[i].currentDay >= sumDays) {
			if (sumDays == 0) {
				ProgramForFun[i].currentDay = (sumDays);
			} else {
				ProgramForFun[i].currentDay = (sumDays - 1);
			}
			ProgramForFun[i].fanOn = 0;
		}
	}
}
//
//-------------------------------------------------------------
//
//Включает/выключает вентилятор.
MenuItemStruct* FANS_Toggle(MenuItemStruct *current) {
	ProgramForFun[current->value3 - 1].fanOn = !ProgramForFun[current->value3 - 1].fanOn;
	return current;
}
//
//-------------------------------------------------------------
//
//Устанавливает текущий день в качестве первого для цикла этого вентилятора
MenuItemStruct* FANS_UpdateFirstDay(MenuItemStruct *current) {
	//current->value3-1 - соответствует номеру вентилятора
	ProgramForFun[current->value3 - 1].firstDay = fullDate;
	//считаем текущий день для вызванного вентилятора
	ProgramForFun[current->value3 - 1].currentDay = fullDate - ProgramForFun[current->value3 - 1].firstDay;
	return current;
}
//
//-------------------------------------------------------------
//
// заполнение программы работы вентилятора
MenuItemStruct* FANS_FillProgram(MenuItemStruct *current) {
	for (int j = 0; j < NUMBER_OF_FANS; j++) {
		for (int i = 0; i < ProgramForFun[j].numberOfDays1; i++) {
			ProgramForFun[j].arrDays[i] = ProgramForFun[j].modeNumber1;
		}
		uint8_t sumdays = ProgramForFun[j].numberOfDays1 + ProgramForFun[j].numberOfDays2;
		for (int i = ProgramForFun[j].numberOfDays1; i < sumdays; i++) {
			ProgramForFun[j].arrDays[i] = ProgramForFun[j].modeNumber2;
		}
		for (int i = sumdays; i < sumdays + ProgramForFun[j].numberOfDays3; i++) {
			ProgramForFun[j].arrDays[i] = ProgramForFun[j].modeNumber3;
		}
		sumdays = sumdays + ProgramForFun[j].numberOfDays3;
		for (int i = sumdays; i < NUM_EL_OF_ARR_DAYS; i++) {
			ProgramForFun[j].arrDays[i] = ProgramForFun[j].modeNumber4;
		}	//END i
	} //END j
	return current;
}
//
//-------------------------------------------------------------
//
//Сброс всех настроек
MenuItemStruct* FANS_ResetSettings(MenuItemStruct *current) {
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
	for (int i = 0; i < 8; i++) // данные о количестве дней и режимов
			{
		ProgramForFun[i].firstDay = fullDate;
		ProgramForFun[i].currentDay = 0;
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

	STORING_SaveSetting();

	return current;
}
//
//-------------------------------------------------------------
//
//Установка времени в RTC
MenuItemStruct* FANS_SetTime(struct MenuItemStruct *current) {
	if (HAL_RTC_SetTime(&hrtc, &userTime, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
	return current;
}
//
//-------------------------------------------------------------
//
//установка даты
MenuItemStruct* FANS_SetDate(struct MenuItemStruct *current) {
	if (HAL_RTC_SetDate(&hrtc, &userDate, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
	HAL_RTC_GetTime(&hrtc, &currentTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
	while (currentDate.Date != userDate.Date || currentDate.Month != userDate.Month || currentDate.Year != userDate.Year) {
		HAL_Delay(1);
	}
	FANS_CalculationDate();
	// приравниваем стартовые дни к текущему
	for (int i = 0; i < 8; i++) {
		ProgramForFun[i].firstDay = fullDate;
	}
	FANS_UpdateCyrrentDays(); //считаем текущие дни
	return current;
}
//
//-------------------------------------------------------------
//
//Переводит дату в одно число
void FANS_CalculationDate(void) {
	fullDate = 0;
	//считаем дни в годах от нулевого до предыдущего установленному
	for (int i = 0; i < currentDate.Year - 1; i++) {
//если год високосный, то +366, если нет, то +365
		for (int j = 0; j < 25; j++) {
			if (i == leapYears[j]) {
				fullDate = fullDate + 1;
			}
		}
		fullDate = fullDate + 365;
	}
	//считаем дни в текущем году в месяцах от первого до предыдущего установленному
	for (int i = 1; i < currentDate.Month - 1; i++) {
		fullDate = fullDate + months[i];
		if (i == 2) // если февраль
				{
			//проверяем, если високосный год, добавляем один день
			for (int j = 0; j < 25; j++) {
				if (currentDate.Year == leapYears[j]) {
					fullDate = fullDate + 1;
				}
			}
		}
	}
	//добавляем дни текущего месяца
	fullDate = fullDate + currentDate.Date;
}
//
//-------------------------------------------------------------
//
//Устанавливает время начала дня
MenuItemStruct* FANS_SetStartDayTime(struct MenuItemStruct *current) {
	dayOn.Hours = userTime.Hours;
	dayOn.Minutes = userTime.Minutes;
	dayOn.Seconds = userTime.Seconds;
	dayOn.FullTime = (uint32_t) (((uint32_t) dayOn.Hours * 3600U) + ((uint32_t) dayOn.Minutes * 60U) + ((uint32_t) dayOn.Seconds));
	return current;
}
//
//-------------------------------------------------------------
//
//Устанавливает время конца дня
MenuItemStruct* FANS_SetEndDayTime(struct MenuItemStruct *current) {
	dayOff.Hours = userTime.Hours;
	dayOff.Minutes = userTime.Minutes;
	dayOff.Seconds = userTime.Seconds;
	dayOff.FullTime = (uint32_t) (((uint32_t) dayOff.Hours * 3600U) + ((uint32_t) dayOff.Minutes * 60U) + ((uint32_t) dayOff.Seconds));
	return current;
}

//
//-------------------------------------------------------------
//
//Set number of second to switch speed fan between lowDay and HighDay
MenuItemStruct* FANS_SetPeriod(struct MenuItemStruct *current) {
	TIM4->ARR = (uint32_t) (g_switchingPeriod * 1000 - 1);
	TIM4->EGR = TIM4->EGR | 1;  // бит UG, что бы вызвать событие обновления и значение TIM4->ARR применилось сразу
	return current;
}
//
//-------------------------------------------------------------
//
//Устанавливаем флаг переключения дня по будильнику
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {
	g_flagNewDay = 1;
}
//
//-------------------------------------------------------------
//
//
FANS_TIME_OF_DAY g_timeOfDay;
//
//-------------------------------------------------------------
//
//
void FANS_SetSpeed(void) {
	if (g_timeOfDay == FANS_TIME_DAY) { //если сейчас день

		if (g_operatingMode) {	//высокая скорость дня
			for (uint8_t i = 0; i < NUMBER_OF_FANS; i++) {
				// для каждого вентилятора выбирается стадия(внутри стадии выбран mode, в зависимости от текущего дня) и время дня, которое определяет скорость

				if (dlRouteM[ProgramForFun[i].arrDays[ProgramForFun[i].currentDay]].dayHight > 0 && ProgramForFun[i].fanOn) { // если введеная пользователем скорость, больше нуля
					HAL_GPIO_WritePin(fanDriver[i].port, fanDriver[i].pin, GPIO_PIN_SET); // включаем 12В питание вентилятора
					*fanDriver[i].speed = dlRouteM[ProgramForFun[i].arrDays[ProgramForFun[i].currentDay]].dayHight; // устанавливаем скорость вентилятора
				} else { //если введеная пользователем скорость, равна нулю
					HAL_GPIO_WritePin(fanDriver[i].port, fanDriver[i].pin, GPIO_PIN_RESET); // отключаем 12В питание вентилятора
					*fanDriver[i].speed = 0;		// скорость вентилятора
				}

			}

		} else { //низкая скорость дня

			for (uint8_t i = 0; i < NUMBER_OF_FANS; i++) {
				if (dlRouteM[ProgramForFun[i].arrDays[ProgramForFun[i].currentDay]].dayLow > 0 && ProgramForFun[i].fanOn) {
					HAL_GPIO_WritePin(fanDriver[i].port, fanDriver[i].pin, GPIO_PIN_SET);
					*fanDriver[i].speed = dlRouteM[ProgramForFun[i].arrDays[ProgramForFun[i].currentDay]].dayLow;
				} else {
					HAL_GPIO_WritePin(fanDriver[i].port, fanDriver[i].pin, GPIO_PIN_RESET);
					*fanDriver[i].speed = 0;
				}

			}

		}

	} else {	// если ночь
		for (uint8_t i = 0; i < NUMBER_OF_FANS; i++) {
			if (dlRouteM[ProgramForFun[i].arrDays[ProgramForFun[i].currentDay]].night > 0 && ProgramForFun[i].fanOn) {
				HAL_GPIO_WritePin(fanDriver[i].port, fanDriver[i].pin, GPIO_PIN_SET);
				TIM3->CCR1 = dlRouteM[ProgramForFun[i].arrDays[ProgramForFun[i].currentDay]].night;
			} else {
				HAL_GPIO_WritePin(fanDriver[i].port, fanDriver[i].pin, GPIO_PIN_RESET);
				*fanDriver[i].speed = 0;
			}
		}

	}
}
