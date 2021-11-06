#include "ringBuffer.h"
#include "string.h"
#include "displayMenu.h"
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
//
//
//=====================================================================================================================================
//
#ifdef DEBUG_INIT_EVENT_RECORDER
#include "EventRecorder.h" //отладка
#endif /* DEBUG_INIT_EVENT_RECORDER */
//
//
//=====================================================================================================================================
//
volatile struct RB_RingBuffer_t g_receiveBuffer __attribute__ ( ( section ( "RING_BUFFER" ) ) );
//volatile struct RB_RingBuffer_t g_receiveBuffer;

RB_DataType_t g_dataTypeRingBuf = RB_DATA_TYPE_UNNECESSARY;
RB_DataType_t g_dataTypeRingBufAfterSize = RB_DATA_TYPE_UNNECESSARY;
RB_AnswerString_t g_stringAnswer; //В случае если мы хотим получит ответ через кольцевой буфер в виде строки, он записывается сюда.
//
//
//=====================================================================================================================================
//
static inline uint8_t RB_SymToByte ( char c )
{
  return ( uint8_t ) ( c <= '9' ? c - '0' : c - 'A' + 10 );
}
//
//
//=====================================================================================================================================
//
static inline _Bool RB_isNumber ( char c )
{
  return ( c >= '0' && c <= '9' );
}
//
//
//=====================================================================================================================================
//ОТ ESP8266 получаем размера файла и в конце 0x0D 0x0A
RB_AnswerSize_t RB_GetSizeOfMessage ( uint16_t beginStr, uint16_t EndStr )
{
  RB_AnswerSize_t sizeAnswer = {0, beginStr};
  _Bool status = 1;
  while ( beginStr != EndStr )
    {
      if ( g_receiveBuffer.arr[beginStr] == 0x0D )
        {
          status = 0;
          break;
        }
      else if ( RB_isNumber ( g_receiveBuffer.arr[beginStr] )==0 ) //если не число
        {
          status = 1;
          break;
        }
    
          sizeAnswer.sizeOfMessage	= sizeAnswer.sizeOfMessage * 10 + RB_SymToByte ( g_receiveBuffer.arr[beginStr] );
          beginStr = ( beginStr == REC_BUF_LAST_ELEMENT ) ? 0 : ( beginStr + 1 );
     
    }
  if ( status == 0 )
    {
      beginStr = ( beginStr == REC_BUF_LAST_ELEMENT ) ? 0 : ( beginStr + 1 );
      if ( g_receiveBuffer.arr[beginStr] != 0x0A )
        {
          sizeAnswer.sizeOfMessage = 0;
        }
    }
  else
    {
      sizeAnswer.sizeOfMessage = 0;
    }
  sizeAnswer.tail = ( beginStr == REC_BUF_LAST_ELEMENT ) ? 0 : ( beginStr + 1 );
  return sizeAnswer;
}
//
//
//=====================================================================================================================================
//
void RB_WriteByteToScreenListAndFileInfo ( volatile char* buffer )
{
  static uint16_t numElStr = 0;
  if ( g_fileInfo[g_numberOfListItemsForShow].typeCheck == 0 )
    {
      if ( * buffer == 0x0D )
        {
          g_fileInfo[g_numberOfListItemsForShow].typeCheck = 1;
          return;
        }
      else
        {
          g_fileInfo[g_numberOfListItemsForShow].file = RB_SymToByte ( * buffer );
          return;
        }
    }
  else if ( g_fileInfo[g_numberOfListItemsForShow].modiflyCheck == 0 )
    {
      if ( * buffer == 0x0D )
        {
          g_fileInfo[g_numberOfListItemsForShow].modiflyCheck = 1;
          g_fileInfo[g_numberOfListItemsForShow].modify[numElStr] = 0x00;
          numElStr = 0;
          return;
        }
      else
        {
          g_fileInfo[g_numberOfListItemsForShow].modify[numElStr] = * buffer;
          numElStr++;
          return;
        }
    }
  else if ( g_fileInfo[g_numberOfListItemsForShow].sizeCheck == 0 && g_fileInfo[g_numberOfListItemsForShow].file == 1 )
    {
      if ( * buffer == 0x0D )
        {
          g_fileInfo[g_numberOfListItemsForShow].sizeCheck = 1;
          return;
        }
      else
        {
          g_fileInfo[g_numberOfListItemsForShow].size = RB_SymToByte ( * buffer ) + g_fileInfo[g_numberOfListItemsForShow].size * 10;
          return;
        }
    }
  else if ( g_fileInfo[g_numberOfListItemsForShow].nameCheck == 0 )
    {
      if ( * buffer == 0x0D )
        {
          g_fileInfo[g_numberOfListItemsForShow].nameCheck = 1;
          g_rowOfListForScreen[g_numberOfListItemsForShow][numElStr] = 0x00;
          numElStr = 0;
          return;
        }
      else
        {
          g_rowOfListForScreen[g_numberOfListItemsForShow][numElStr] = *buffer;
          numElStr++;
          return;
        }
    }
  else if ( *buffer == 0x0A )
    {
      g_numberOfListItemsForShow++;
    }
}

//
//
//=====================================================================================================================================
//
void RB_WriteByteToScreenList ( volatile char* buffer )
{
  static uint16_t numElStr = 0;

  if ( *buffer == 0x0D )
    {
      g_rowOfListForScreen[g_numberOfListItemsForShow][numElStr] = 0x00;
      g_numberOfListItemsForShow++;
      numElStr = 0;
    }
  else
    {
      g_rowOfListForScreen[g_numberOfListItemsForShow][numElStr] = *buffer;
      numElStr++;
    }
}

//
//
//=====================================================================================================================================
//
RB_AnswerString_t RB_ConvertAnswerToString ( uint16_t beginStr, uint16_t EndStr )
{
  static RB_AnswerString_t answer;
  answer.tail = beginStr;
  uint16_t i = 0;
  //где то должен быть переход на мусор!!!
  while ( beginStr != EndStr )
    {
      if ( g_receiveBuffer.arr[beginStr] == 0x0D )
        {
          answer.string[i] = 0x00;
          answer.tail = ( beginStr == REC_BUF_LAST_ELEMENT ) ? 0 : ( beginStr + 1 );
          return answer;
        }
      answer.string[i] = g_receiveBuffer.arr[beginStr];
      beginStr = ( beginStr == REC_BUF_LAST_ELEMENT ) ? 0 : ( beginStr + 1 );
      i++;
      if ( i == MAX_LENGHT_ANSWER_STR )
        {
          return answer;
        }
    }
  return answer;//значит  0x0D не найден
}
