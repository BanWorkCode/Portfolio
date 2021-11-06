#include "firmware.h"
#include "fatfs.h"
#include "stdio.h"

/*
Вычисление контрольной суммы строки:
Сама сумма(как последнее число в ) вычисляется:
:00000001FF где:

00 Количество байт записи
0000 Поле игнорируется. Не имеет значения, что там записано, но обычно всегда равно 0
01 Тип записи 01 (Конец Файла)
FF Контрольная сумма, вычисленная как 01h + NOT(00h + 00h + 00h + 01h).
//-------
А сумма строки вычисляется как сумма всех чисел строки включая контрольную сумму по модулю 256, должна быть равна нулю.
проще всего складывать в uint8_t

*/




static FIL fil; //Структура FIL (file object, файловый объект) содержит в себе состояние открытого файла.
static FIRMWARE_STRING_OF_HEX_FILE g_stringHex;

static SECTOR_t sector[NUMBER_OF_SECTORS] =
{
  {0U, 0x08000000U, 0x4000U, NOT_ERASE},
  {1U, 0x08004000U, 0x4000U, NOT_ERASE},
  {2U, 0x08008000U, 0x4000U, NOT_ERASE},
  {3U, 0x0800C000U, 0x4000U, NOT_ERASE},
  {4U, 0x08010000U, 0x10000U, NOT_ERASE},
  {5U, 0x08020000U, 0x20000U, NOT_ERASE},
  {6U, 0x08040000U, 0x20000U, NOT_ERASE},
  {7U, 0x08060000U, 0x20000U, NOT_ERASE},
};

//
//
//=====================================================================================================================================
//
static inline uint8_t FW_SumToByte ( char c )
{
  return ( uint8_t ) ( c <= '9' ? c - '0' : c - 'A' + 10 );
}
//
//
//=====================================================================================================================================
//
_Bool SD_GetStringFromFileOnCard ( char* stringFromFile, uint16_t leghtOfString )
{
  if ( f_gets ( stringFromFile, leghtOfString, &fil ) )
    {
      return 0;
    }
  else
    {
      return 1;
    }
}
//
//
//=====================================================================================================================================
//
FW_RESULT FW_ProcessingString ( char* processingString )
{
//	uint8_t status =0;
  switch ( processingString[8] )
    {
    case 0x31: //  запись обозначает конец файла, данных не содержит.
      g_stringHex.size = ( FW_SumToByte ( processingString[1] ) << 4 ) | FW_SumToByte ( processingString[2] );
      g_stringHex.checksum = ( processingString[9 + g_stringHex.size * 2] << 4 | processingString[10 + g_stringHex.size * 2] );
      return RESULT_END;
    // break;
    case 0x34: // запись расширенного адреса
      g_stringHex.adress[3] = ( FW_SumToByte ( processingString[9] ) << 4 ) | FW_SumToByte ( processingString[10] );
      g_stringHex.adress[2] = ( FW_SumToByte ( processingString[11] ) << 4 ) | FW_SumToByte ( processingString[12] );
      return RESULT_ADDRESS;
    //break;
    case 0x30:   //запись содержит данные двоичного файла
    {
      g_stringHex.size = ( FW_SumToByte ( processingString[1] ) << 4 ) | FW_SumToByte ( processingString[2] );
      g_stringHex.adress[1] = ( FW_SumToByte ( processingString[3] ) << 4 ) | FW_SumToByte ( processingString[4] );
      g_stringHex.adress[0] = ( FW_SumToByte ( processingString[5] ) << 4 ) | FW_SumToByte ( processingString[6] );
      for ( uint8_t k = 0; k < g_stringHex.size; k++ )
        {
          g_stringHex.data[k] = ( FW_SumToByte ( processingString[9 + ( k * 2 )] ) << 4 ) | FW_SumToByte ( processingString[9 + ( k * 2 ) + 1] );
        }
      g_stringHex.checksum =   ( processingString[9 + g_stringHex.size * 2] << 4 | processingString[10 + g_stringHex.size * 2] );
    }
    return RESULT_DATA;
    // break;
    case 0x35: // ненужные данные
      return RESULT_UNNECESSARY;
    // break;
    default:
      return RESULT_ERROR;
      // break;
    }
}




//
//
//=====================================================================================================================================
//
SECTOR_t* FW_SELECT_SECTOR ( uint32_t address11 )
{
  for ( uint16_t i = 0; i < NUMBER_OF_SECTORS; i++ )
    {
      if ( address11 >= sector[i].address && address11 < ( sector[i].address + sector[i].size ) )
        {
          return &sector[i];
        }
    }
  return 0;
}


//
//
//=====================================================================================================================================
//

static _Bool FW_WriteToFlash ( uint32_t adress, uint32_t size, uint8_t* data )
{
  for ( uint16_t i = 0; i < size; i++ )
    {
      if ( HAL_FLASH_Program ( FLASH_TYPEPROGRAM_BYTE, adress +  i, data[i] ) != HAL_OK )
        {
          return 1;
        }
    }
  return 0;
}
//
//
//=====================================================================================================================================
//

HAL_StatusTypeDef stat222 = HAL_OK;
_Bool stat333 = 0;




_Bool FW_ERASE_AND_PROGRAM ( SECTOR_t* selSector )
{
  FLASH_EraseInitTypeDef eraseInitStruct =
  {
    .TypeErase = FLASH_TYPEERASE_SECTORS, // постраничная очистка
    //.Sector = FLASH_SECTOR_7, // адрес страницы
    .Sector = selSector->number, // адрес страницы
    .NbSectors = 1, // кол-во страниц для стирания
    .VoltageRange = FLASH_VOLTAGE_RANGE_3,
    .Banks = FLASH_BANK_1
  };
  uint32_t page_error = 0;
  if ( HAL_FLASH_Unlock() != HAL_OK )
    {
      return 1;
    }
  if ( selSector->erase == NOT_ERASE )  	//Erase
    {
      for ( uint8_t i = 0; i < 2; i++ )
        {
          stat222 = HAL_FLASHEx_Erase ( &eraseInitStruct, &page_error );
          if ( stat222 == HAL_OK )
            {
              break;
            }
        }
      if ( stat222 != HAL_OK )
        {
          HAL_FLASH_Lock();
          return 1;
        }
      else
        {
          selSector->erase = ERASE;
        }
    }//End Erase
  stat333 = FW_WriteToFlash ( * ( uint32_t* ) g_stringHex.adress, g_stringHex.size, g_stringHex.data );
  printf ( "stat333 =%i\r\n", stat333 );
  HAL_FLASH_Lock();
  return 0;
}

//
//
//=====================================================================================================================================
//

_Bool BOOTLOADER_UploadFirmware ( void )
{
  _Bool status = 0;
  _Bool running = 1;

  if (f_open (  &fil,   "0:/Firmware/Programmer.hex",  FA_READ | FA_OPEN_EXISTING ) )
    {
      status = 1;
    }
  char processingString[522];
  while ( running && status == 0 )
    {
      if ( SD_GetStringFromFileOnCard ( processingString, 522 ) )
        {
          status = 1; //если пустая строка, то это ошибка, потому что до нее не доджно дойти,\
        выход из цикла в последней  сроке 	switch case 0x31:
        break;
      }
    else
      {
        FW_RESULT res = FW_ProcessingString ( processingString );
          if ( res == RESULT_DATA )
            {
              //контрольную сумму проверить здесь или в FW_ProcessingString!!!
              SECTOR_t* selectedSector = FW_SELECT_SECTOR ( * ( uint32_t* ) g_stringHex.adress );
              if ( selectedSector == 0 )
                {
                  status = 1;
                  break;
                }
              else
                {
                  FW_ERASE_AND_PROGRAM ( selectedSector );
                }
            }
          else if ( res == RESULT_END )
            {
              running = 0;
            }
        }
    }
  if ( f_close (  &fil ) )
    {
      status = 1;
    }
  if ( status == 0 )
    {
      return 0;
    }
  else
    {
      return 1;
    }
}
