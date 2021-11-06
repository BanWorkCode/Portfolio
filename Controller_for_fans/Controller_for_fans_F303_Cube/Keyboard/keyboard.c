#include "main.h"
#include "keyboard.h"
#include "fillMenu.h"
#include "ssd1306.h"
//
//-------------------------------------------------------------
//
//
uint32_t timeOfLastClick = 0; // время с последнего нажатия
//
//-------------------------------------------------------------
//
// Кнопка прерывание
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_12) {
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
		timeOfLastClick = HAL_GetTick();
	}
}
//
//-------------------------------------------------------------
//
//
//Encoder
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		if (ssd1306_CheckDisplayStatus() == SSD1306_ON) {
			//Энкодер дикремент
			if ( TIM2->CR1 & TIM_CR1_DIR) {
				if (curMainMenu->cursor == MENU_CURSOR_FIRST_COLUMN) {
					if (curMainMenu->up != NULL) {
						curMainMenu = curMainMenu->up;
					}
				} else if (curMainMenu->cursor == MENU_CURSOR_SECOND_COLUMN) {
					if (*curMainMenu->value > (curMainMenu->minVal)) {
						(*curMainMenu->value)--;
					}
				} else if (curMainMenu->cursor == MENU_CURSOR_THIRD_COLUMN) {
					if (*curMainMenu->value2 > 0) {
						(*curMainMenu->value2)--;
					}
				}
			}
			//Энкодер инкремент
			else {
				if (curMainMenu->cursor == MENU_CURSOR_FIRST_COLUMN) {
					if (curMainMenu->down != NULL) {
						curMainMenu = curMainMenu->down;
					}
				} else if (curMainMenu->cursor == MENU_CURSOR_SECOND_COLUMN) {
					if ((*curMainMenu->value) < (curMainMenu->maxVal)) {
						(*curMainMenu->value)++;
					}
				} else if (curMainMenu->cursor == MENU_CURSOR_THIRD_COLUMN) {
					if ((*curMainMenu->value2) < (curMainMenu->maxVal2)) {
						(*curMainMenu->value2)++;
					}
				}
			}
		}
		timeOfLastClick = HAL_GetTick();
	}
}
//
//-------------------------------------------------------------
//
//
void KEYBOARD_ButtonClick(void) {
	if (curMainMenu->type == MENU_TYPE_ONE_COLUMN) {
		curMainMenu = curMainMenu->function(curMainMenu->enter);
	}

	else if (curMainMenu->type == MENU_TYPE_TWO_COLUMN) {
		if (curMainMenu->cursor == MENU_CURSOR_FIRST_COLUMN) {
			curMainMenu->cursor = MENU_CURSOR_SECOND_COLUMN;
		} else {
			curMainMenu->cursor = MENU_CURSOR_FIRST_COLUMN;
		}
	}

	else if (curMainMenu->type == MENU_TYPE_TREE_COLUMN) {
		if (curMainMenu->cursor == MENU_CURSOR_THIRD_COLUMN) {
			curMainMenu->cursor = MENU_CURSOR_FIRST_COLUMN;
		} else {
			curMainMenu->cursor++;

		}

	}
}
