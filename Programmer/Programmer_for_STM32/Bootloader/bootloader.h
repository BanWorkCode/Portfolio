#ifndef __bootloader_H
#define __bootloader_H
//
//
//=====================================================================================================================================
//
#include "main.h"
//
//
//=====================================================================================================================================
//
#define NUMBER_OF_DEVICES	70
#define MAX_NUMBER_OF_SECTORS	256
#define MAX_NUMBER_OF_CHARACTERS_IN_HEX_FILE_LINE 128

//
//
//=====================================================================================================================================
//
typedef const struct {
uint32_t address;
uint16_t	occurence;
uint32_t size; 	
} PROGRAMMER_MemoryAllocated_t;
//
//
typedef const struct {
uint16_t targetPID;
uint16_t targetBLID;

char* 	targetName;
uint16_t	numRecords;
PROGRAMMER_MemoryAllocated_t* memoryAllocated;	
} PROGRAMMER_Device_t;
//
//
typedef  struct {
uint8_t bootloaderVercion;	
uint8_t getCommand;	
uint8_t getVersionAndReadProtectionStatus;	
uint8_t getID;	
uint8_t readMemorycommand;	
uint8_t goCommand;	
uint8_t writeMemoryCommand;	
uint8_t eraseCommand;	
uint8_t writeProtectCommand;	
uint8_t writeUnprotectCommand;	
uint8_t readoutProtectCommand;	
uint8_t readoutUnprotectCommand;
uint16_t targetPID;
char* 	targetName;		
PROGRAMMER_Device_t*  currentDevice;	
} PROGRAMMER_DeviceData_t;
//


//
//
//=====================================================================================================================================
//
static inline uint8_t PROGRAMMER_SumToByte ( char c );
static void PROGRAMMER_ReseteTarget ( uint16_t delay );
static _Bool PROGRAMMER_InitTarget ( void );
static _Bool PROGRAMMER_GetSupportedCommands ( void );
static _Bool PROGRAMMER_GetIDCommand ( void );
static _Bool PROGRAMMER_GetCurrentDevice ( void );
static _Bool PROGRAMMER_FullEraseMemory(void);
void* PROGRAMMER_InitBootloader ( void* currentItem );
void* PROGRAMMER_EraseMemory ( void* currentItem );
static _Bool PROGRAMMER_EraseMemorySectors ( void );
//void* PROGRAMMER_UploadFirmware ( void* currentItem );
static _Bool PROGRAMMER_UploadFirmware ( void );
void* PROGRAMMER_RunProgramm ( void* currentItem );
static _Bool PROGRAMMER_GetAddressesOfSectorsToCleanUp ( void );
void PROGRAMMER_ClearSectorNumbersForErase ( void);	
void* PROGRAMMER_UploadFirmwareAfterFullErase ( void* currentItem );
void* PROGRAMMER_UploadFirmwareAfterEraseSectors ( void* currentItem );
//
//
//=====================================================================================================================================
//
#endif /*__bootloader_H */
