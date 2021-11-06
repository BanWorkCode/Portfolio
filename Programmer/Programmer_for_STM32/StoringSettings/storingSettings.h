#ifndef saveSetting_H
#define saveSetting_H
#include "main.h"
//
//
//=====================================================================================================================================
//
#define ADRESS_BEGINNING_SECTOR 0x08060000U
//
//
//=====================================================================================================================================
//
static _Bool STORING_WriteToFlash ( uint32_t adress, uint32_t size, uint32_t* data );
_Bool STORING_SaveSettingsToFlash ( void );
void STORING_ReadSettingsFromFlash ( void );
void STORING_CheckFirstConnection(void);
//
//
//=====================================================================================================================================
//

#endif
