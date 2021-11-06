#ifndef _firmware_H
#define _firmware_H


#include "main.h"

#define NUMBER_OF_SECTORS 8U

typedef struct{
uint8_t size;	
uint8_t adress[4];
uint8_t data[256];
uint8_t checksum;		
	
} FIRMWARE_STRING_OF_HEX_FILE;



typedef enum {
RESULT_DATA =0U,
RESULT_END = 1U,
RESULT_ADDRESS = 2U,	
RESULT_UNNECESSARY=3U,	
RESULT_ERROR = 4U	
} FW_RESULT;


//FLASH_BASE
//#define SECTOR_0 0x08000000U
//#define SECTOR_1 0x08004000U
//#define SECTOR_2 0x08008000U
//#define SECTOR_3 0x0800C000U
//#define SECTOR_4 0x08010000U
//#define SECTOR_5 0x08020000U
//#define SECTOR_6 0x08040000U
//#define SECTOR_7 0x08060000U

typedef enum {
NOT_ERASE =0U,
ERASE = 1U
} SECTOR_ERASE_t;

typedef struct {
uint8_t const number;	
uint32_t const	address;
uint32_t const	size;	
SECTOR_ERASE_t erase;	
} SECTOR_t;

_Bool BOOTLOADER_UploadFirmware ( void );


#endif
