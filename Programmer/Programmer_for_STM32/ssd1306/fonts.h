
#include "stm32f4xx_hal.h"

#ifndef Fonts
#define Fonts
#define WIDTH_FONT_7x10 7
#define HEIGHTH_FONT_7x10 10
//
//
//=====================================================================================================================================
//
typedef struct {
	const uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */
} FontDef;
//
//
//=====================================================================================================================================
//
extern FontDef Font_7x10;
//
//
//=====================================================================================================================================
//

#endif

