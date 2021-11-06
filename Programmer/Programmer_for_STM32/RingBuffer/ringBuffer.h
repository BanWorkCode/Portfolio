#ifndef __ring_buffer_H
#define __ring_buffer_H
//
//
//=====================================================================================================================================
//
#include "main.h"
//
//
//=====================================================================================================================================
//
#define REC_BUF_SIZE 7168U
//#define REC_BUF_SIZE 997U
//#define REC_BUF_SIZE 8192U
//#define REC_BUF_SIZE 2048U
#define REC_BUF_LAST_ELEMENT (REC_BUF_SIZE-1)
#define MESSAGES_BUF_STRING_LENGHT 128U
#define MAX_LENGHT_ANSWER_STR 256U
//
//
//=====================================================================================================================================
//
struct RB_RingBuffer_t
{
  volatile char arr [REC_BUF_SIZE];
  volatile uint16_t tail;
};
//
//
typedef struct
{
  char string[MAX_LENGHT_ANSWER_STR];
  uint16_t tail;
} RB_AnswerString_t;
//
//
typedef struct
{
  uint64_t sizeOfMessage;
  uint16_t tail;
} RB_AnswerSize_t;
//
//
typedef enum
{
  RB_DATA_TYPE_UNNECESSARY =0U,
  RB_DATA_TYPE_FILE_SIZE = 1U,
  RB_DATA_TYPE_FILE = 2U,
  RB_DATA_TYPE_NETWORKS =3U,
  RB_DATA_TYPE_MESSAGE =4U,
  RB_DATA_TYPE_DIRECTORIES = 5U	
} RB_DataType_t;
//
//
//=====================================================================================================================================
//
extern RB_DataType_t g_dataTypeRingBuf;
extern RB_DataType_t g_dataTypeRingBufAfterSize;
extern volatile struct RB_RingBuffer_t g_receiveBuffer;
extern RB_AnswerString_t g_stringAnswer;
//
//
//=====================================================================================================================================
//

void RB_WriteByteToScreenListAndFileInfo ( volatile char* buffer );
void RB_WriteByteToScreenList ( volatile char* buffer );
RB_AnswerSize_t RB_GetSizeOfMessage ( uint16_t beginStr, uint16_t EndStr );
RB_AnswerString_t RB_ConvertAnswerToString ( uint16_t beginStr, uint16_t EndStr );
static inline uint8_t RB_SymToByte ( char c );
//
//
//=====================================================================================================================================
//
#endif /*__ring_buffer_H */
